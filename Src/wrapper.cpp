#include <my_adc.h>
#include "main.h"
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include <cstdio>
#include <cstdint>
#include <cstring>
#include "my_adc.h"
#include "lcd.hpp"
#include "continues_servo.hpp"
#include "my_LED.hpp"
#include "uart_frame.hpp"

extern "C" void main_cpp(){

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	extern I2C_HandleTypeDef hi2c1;

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	ADC_InitOnce();   // 最初に1回
	aqm1602::LCD lcd(&hi2c1, 0x3E);
	lcd.init();

	uart_frame::begin();

  ControllerFrame f{};



	while (1)
	{
		if (uart_frame::read(f)) {
		  // ここにあなたの制御ロジックを記述
		  // 例：
		  // 左スティックで並進、右スティックで旋回、RTで加速、LTでブレーキ
		  // if (f.buttons & (1u<<0)) { /* Aボタン */ }
		  // bool dpad_up = (f.buttons >> 10) & 1;
			if(f.buttons & (1u<<0)){
				LED::on(1);
			}else{
				LED::off(1);
			}
			float left = f.ay / 350.0f;
			float right = f.ay / 350.0f;
			right -= f.ax / 350.0f;
			left += f.ax / 350.0f;


			left_servo((int)left);
			right_servo((int)right);


		}
		HAL_Delay(1);



//		uint16_t r1 = ADC_Read_PA1();
//		uint16_t r2 = ADC_Read_PA2();
//		uint16_t r3 = ADC_Read_PA3();
//		uint16_t r4 = ADC_Read_PA4();
//		uint16_t r5 = ADC_Read_PA5();

//		bool line[4];

//		int half_line = 800;
//		if(r1 >= half_line){
//			line[0] = true;
//			LED::on(1);
//		}else{
//			line[0] = false;
//			LED::off(1);
//		}
//		if(r2 >= half_line){
//			line[1] = true;
//			LED::on(2);
//		}else{
//			line[1] = false;
//			LED::off(2);
//		}
//		if(r3 >= half_line){
//			line[2] = true;
//			LED::on(3);
//		}else{
//			line[2] = false;
//			LED::off(3);
//		}
//		if(r4 >= half_line){
//			line[3] = true;
//			LED::on(4);
//		}else{
//			line[3] = false;
//			LED::off(4);
//		}
//
//
//
//		if(line[0] == true){
//			right_servo(100);
//			left_servo(0);
//		}
//		else if(line[3] == true){
//			right_servo(0);
//			left_servo(100);
//		}
//		else{
//			right_servo(100);
//			left_servo(100);
//		}


	}

}
