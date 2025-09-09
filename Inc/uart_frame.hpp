#pragma once
#include <cstring>
#include "stm32f1xx_hal.h"
#include "usart.h"

// ===== F710フル入力 =====
#pragma pack(push,1)
struct ControllerFrame {
  int16_t ax, ay;
  int16_t lx, ly;
  int16_t lt, rt;
  uint16_t buttons;
};
#pragma pack(pop)

// ===== CRC-16/IBM =====
static inline uint16_t crc16_ibm(const uint8_t* p, size_t n) {
  uint16_t c = 0xFFFF;
  for (size_t i = 0; i < n; ++i) {
    c ^= p[i];
    for (int b = 0; b < 8; ++b) c = (c & 1) ? (c >> 1) ^ 0xA001 : (c >> 1);
  }
  return c;
}

namespace uart_frame {
  static UART_HandleTypeDef* const U = &huart1;

  enum class RxState : uint8_t { H0, H1, GLEN, PAY, CRC0, CRC1 };
  static volatile RxState st = RxState::H0;

  static volatile uint8_t rx_byte = 0;
  static uint8_t len = 0;
  static uint8_t buf[sizeof(ControllerFrame)];
  static size_t idx = 0;
  static uint16_t rx_crc = 0;

  static volatile bool ready = false;
  static ControllerFrame latest;

  inline void _arm_rx() { HAL_UART_Receive_IT(U, (uint8_t*)&rx_byte, 1); }

  inline void begin() {
    st = RxState::H0; len = 0; idx = 0; rx_crc = 0; ready = false;
    _arm_rx();
  }

  inline bool read(ControllerFrame& out) {
    if (!ready) return false;
    __disable_irq();
    out = latest;
    ready = false;
    __enable_irq();
    return true;
  }
}

// ===== 受信割り込みコールバック（この定義は *.cpp 1か所だけに含める） =====
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
  using namespace uart_frame;
  if (huart != U) { _arm_rx(); return; }

  switch (st) {
    case RxState::H0:
      st = (rx_byte == 0xAA) ? RxState::H1 : RxState::H0;
      break;
    case RxState::H1:
      st = (rx_byte == 0x55) ? RxState::GLEN : ((rx_byte == 0xAA) ? RxState::H1 : RxState::H0);
      break;
    case RxState::GLEN:
      len = rx_byte; idx = 0;
      st = (len == sizeof(ControllerFrame)) ? RxState::PAY : RxState::H0;
      break;
    case RxState::PAY:
      buf[idx++] = rx_byte;
      if (idx >= len) st = RxState::CRC0;
      break;
    case RxState::CRC0:
      rx_crc = rx_byte; // LSB
      st = RxState::CRC1;
      break;
    case RxState::CRC1: {
      rx_crc |= (uint16_t)rx_byte << 8; // MSB
      const uint16_t calc = crc16_ibm(buf, len);
      if (calc == rx_crc) {
        std::memcpy((void*)&latest, buf, sizeof(ControllerFrame));
        ready = true;
      }
      st = RxState::H0; len = 0; idx = 0; rx_crc = 0;
    } break;
  }
  _arm_rx();
}
