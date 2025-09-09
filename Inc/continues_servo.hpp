#pragma once
#include "stm32f1xx_hal.h"     // HAL_I2C_* を使う場合（他シリーズなら適宜変更）

void left_servo(int in_vel);   // -100 ~ 100
void right_servo(int in_vel);
