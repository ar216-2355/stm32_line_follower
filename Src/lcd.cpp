#include "lcd.hpp"
#include <cstdio>
#include <cstring>

namespace aqm1602 {

static constexpr uint8_t ROW_ADDR[2] = {0x00, 0x40}; // 2行LCDのDDRAM先頭

LCD::LCD(I2C_HandleTypeDef* hi2c, uint8_t addr7)
: hi_(hi2c), addr7_(addr7), cur_col_(0), cur_row_(0) {}

void LCD::cmd(uint8_t c) {
  uint8_t b[2] = { 0x00, c }; // control=0x00 (RS=0)
  HAL_I2C_Master_Transmit(hi_, static_cast<uint16_t>(addr7_ << 1), b, 2, 10);
}

void LCD::data(uint8_t d) {
  uint8_t b[2] = { 0x40, d }; // control=0x40 (RS=1)
  HAL_I2C_Master_Transmit(hi_, static_cast<uint16_t>(addr7_ << 1), b, 2, 10);
}

void LCD::init() {
  HAL_Delay(40);

  cmd(0x38);                               // Function Set IS=0
  cmd(0x39);                               // Function Set IS=1 (拡張)
  cmd(0x14);                               // Internal OSC freq
  cmd(static_cast<uint8_t>(0x70 | (AQM1602_CONTRAST & 0x0F)));      // コントラスト下位4bit
  cmd(static_cast<uint8_t>(0x5C | ((AQM1602_CONTRAST >> 4) & 0x03)));// Bon=1(3.3V),C5-C4
  cmd(0x6C);                               // Follower control
  HAL_Delay(200);

  cmd(0x38);                               // IS=0 に戻す
  cmd(0x0C);                               // Display ON（カーソル/ブリンクOFF）
  cmd(0x01); HAL_Delay(2);                 // Clear
  cmd(0x06);                               // Entry mode（アドレス+1, シフトなし）

  cur_col_ = 0; cur_row_ = 0;
}

void LCD::clear() {
  cmd(0x01); HAL_Delay(2);
  cur_col_ = 0; cur_row_ = 0;
}

void LCD::applyCursor() {
  uint8_t r = (cur_row_ >= AQM1602_ROWS) ? (AQM1602_ROWS - 1) : cur_row_;
  uint8_t c = (cur_col_ >= AQM1602_COLS) ? (AQM1602_COLS - 1) : cur_col_;
  uint8_t addr = static_cast<uint8_t>(ROW_ADDR[r] + c);
  cmd(static_cast<uint8_t>(0x80 | addr));  // Set DDRAM address
}

void LCD::setCursor(uint8_t col, uint8_t row) {
  cur_row_ = (row >= AQM1602_ROWS) ? (AQM1602_ROWS - 1) : row;
  cur_col_ = (col >= AQM1602_COLS) ? (AQM1602_COLS - 1) : col;
  applyCursor();
}

void LCD::putc(char ch) {
  if (ch == '\n') {
    // 次行の先頭へ（最終行の次は最初の行に戻る簡易仕様）
    cur_row_ = static_cast<uint8_t>((cur_row_ + 1) % AQM1602_ROWS);
    cur_col_ = 0;
    applyCursor();
    return;
  }

  data(static_cast<uint8_t>(ch));
  cur_col_++;
  if (cur_col_ >= AQM1602_COLS) {
    cur_col_ = 0;
    cur_row_ = static_cast<uint8_t>((cur_row_ + 1) % AQM1602_ROWS);
    applyCursor();
  }
}

void LCD::print(const char* s) {
  while (*s) putc(*s++);
}

/* 軽量な10進整数表示（printf非依存） */
void LCD::printInt(int value) {
  char buf[12];                // "-2147483648" + NUL
  char* p = &buf[sizeof(buf) - 1];
  *p = '\0';

  unsigned int u;
  if (value < 0) {
    u = static_cast<unsigned int>(-(value + 1)) + 1u; // INT_MIN 対策
  } else {
    u = static_cast<unsigned int>(value);
  }

  do {
    *--p = static_cast<char>('0' + (u % 10u));
    u /= 10u;
  } while (u);

  if (value < 0) *--p = '-';
  print(p);
}

/* printf 風：fmt → 一時バッファ → LCD
   ※ バッファ長は用途に応じて調整。超過分は切り捨て。 */
void LCD::printf(const char* fmt, ...) {
  char buf[128];
  std::va_list ap;
  va_start(ap, fmt);
  int n = std::vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (n <= 0) return;
  int limit = (n < static_cast<int>(sizeof(buf))) ? n : static_cast<int>(sizeof(buf)) - 1;
  for (int i = 0; i < limit; ++i) {
    putc(buf[i]);
  }
}

} // namespace aqm1602
