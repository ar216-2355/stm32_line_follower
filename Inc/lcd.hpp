#pragma once
#include "i2c.h"               // CubeMX の I2C_HandleTypeDef 用
#include "stm32f1xx_hal.h"     // HAL_I2C_* を使う場合（他シリーズなら適宜変更）
#include <cstdint>
#include <cstdarg>

/* 必要に応じてプロジェクト側で上書き可（-Dで定義など） */
#ifndef AQM1602_COLS
#define AQM1602_COLS 16
#endif
#ifndef AQM1602_ROWS
#define AQM1602_ROWS 2
#endif
#ifndef AQM1602_CONTRAST
#define AQM1602_CONTRAST 0x28   // 3.3Vで見やすい範囲：0x24〜0x2C
#endif

namespace aqm1602 {

class LCD {
public:
  /* addr7 は 7bit アドレス（例：0x3E） */
  LCD(I2C_HandleTypeDef* hi2c, uint8_t addr7);

  void init();                              // 初期化
  void clear();                             // 全消去＋カーソル原点
  void setCursor(uint8_t col, uint8_t row); // カーソル移動（範囲外はクリップ）
  void putc(char ch);                       // 1文字出力（\nで次行先頭へ）
  void print(const char* s);                // 文字列出力
  void printInt(int v);                     // 整数出力（軽量）
  void printf(const char* fmt, ...);        // printf 互換（vsnprintf 経由）

  uint8_t cols() const { return AQM1602_COLS; }
  uint8_t rows() const { return AQM1602_ROWS; }

private:
  void cmd(uint8_t c);                      // コマンド送信（RS=0）
  void data(uint8_t d);                     // データ送信（RS=1）
  void applyCursor();                       // 現在の (cur_row,cur_col) をDDRAMへ反映

  I2C_HandleTypeDef* hi_;
  uint8_t addr7_;
  uint8_t cur_col_;
  uint8_t cur_row_;
};

} // namespace aqm1602
