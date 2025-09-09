#include "my_adc.h"

/* 読み取り時のサンプリング時間（高インピーダンス対策で長め推奨） */
#ifndef ADC_SAMPLE_TIME
#define ADC_SAMPLE_TIME ADC_SAMPLETIME_239CYCLES_5
#endif

/* 内部: 指定チャネルを1回だけ読む */
static uint16_t adc_read_channel(uint32_t channel)
{
    ADC_ChannelConfTypeDef s = {0};
    s.Channel      = channel;
    s.Rank         = ADC_REGULAR_RANK_1;
    s.SamplingTime = ADC_SAMPLE_TIME;
    HAL_ADC_ConfigChannel(&hadc1, &s);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint16_t v = (uint16_t)HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return v; // 0..4095
}

/* 公開API */
void ADC_InitOnce(void)
{
    static uint8_t done = 0;
    if (done) return;
    /* F1系はキャリブレーション推奨 */
    HAL_ADCEx_Calibration_Start(&hadc1);
    done = 1;
}

uint16_t ADC_Read_PA1(void) { return adc_read_channel(ADC_CHANNEL_1); } // PA1
uint16_t ADC_Read_PA2(void) { return adc_read_channel(ADC_CHANNEL_2); } // PA2
uint16_t ADC_Read_PA3(void) { return adc_read_channel(ADC_CHANNEL_3); } // PA3
uint16_t ADC_Read_PA4(void) { return adc_read_channel(ADC_CHANNEL_4); } // PA4
uint16_t ADC_Read_PA5(void) { return adc_read_channel(ADC_CHANNEL_5); } // PA5

float ADC_ToVoltage(uint16_t raw, float vdda)
{
    return ((float)raw * vdda) / 4095.0f; // 12bit換算
}
