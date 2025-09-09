#pragma once
#include "adc.h"                 // CubeMX が生成（hadc1 の extern あり）
#include "stm32f1xx_hal.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 初回だけキャリブレーション等を実施（電源投入後に1回でOK） */
void ADC_InitOnce(void);

/* 単発読み取り（0..4095）。PA0は意図的に実装しない */
uint16_t ADC_Read_PA1(void);
uint16_t ADC_Read_PA2(void);
uint16_t ADC_Read_PA3(void);
uint16_t ADC_Read_PA4(void);
uint16_t ADC_Read_PA5(void);

/* 12bit RAW→電圧[V] */
float ADC_ToVoltage(uint16_t raw, float vdda);

/* 互換API：既存コードのままでも動くように */
static inline void     ADC_A5_InitOnce(void) { ADC_InitOnce(); }
static inline uint16_t ADC_Read_A5(void)     { return ADC_Read_PA5(); }

#ifdef __cplusplus
}
#endif
