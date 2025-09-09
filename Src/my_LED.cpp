#include "stm32f1xx_hal.h"
#include "tim.h"
#include "my_LED.hpp"

namespace LED{

static int now_duty[4] = {0};
static int counter_period = 1000;

void write_led(int led_num, int duty){
	switch(led_num){
		case 1:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, duty);
			now_duty[0] = duty;
			break;
		case 2:
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, duty);
			now_duty[1] = duty;
			break;
		case 3:
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, duty);
			now_duty[2] = duty;
			break;
		case 4:
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, duty);
			now_duty[3] = duty;
			break;
	}
}

void on(int led_num){
	write_led(led_num, counter_period);
}

void all_on(){
	for(int i = 0; i < 4; i++){
		write_led(i + 1, counter_period);
	}
}

void off(int led_num){
	write_led(led_num, 0);
}

void all_off(){
	for(int i = 0; i < 4; i++){
		write_led(i + 1, 0);
	}
}

void toggle(int led_num){
	if(now_duty[led_num - 1] == 0){
		write_led(led_num, counter_period);
	}else{
		write_led(led_num, 0);
	}
}

void all_toggle(){
	for(int i = 0; i < 4; i++){
		if(now_duty[i] == 0){
			write_led(i + 1, counter_period);
		}else{
			write_led(i + 1, 0);
		}
	}
}

void write_mask(int led_nums){
	for(int i = 0; i < 4; i++){
		if(led_nums & (1 << i)){
			write_led(i + 1, counter_period);
		}else{
			write_led(i + 1, 0);
		}
	}
}

void write_mask_brightness(int led_nums, int duty){
	for(int i = 0; i < 4; i++){
		if(led_nums & (1 << i)){
			write_led(i + 1, duty);
		}else{
			write_led(i + 1, 0);
		}
	}
}

void set_brightness(int led_num, int duty){
	write_led(led_num, duty);
}

}





