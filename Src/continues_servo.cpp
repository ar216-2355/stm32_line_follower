#include "tim.h"
#include "continues_servo.hpp"

static float bask_per_front = 0.71f;
static float front_max = 1740;
static float back_max = 1100;
static float front_move_min = 1500;
static float back_move_min = 1450;

void left_servo(int in_vel){


	int out_vel;
	if(in_vel == 0){
		out_vel = 1500;
	}
	else if(in_vel > 0){
		out_vel = front_move_min + in_vel * ((front_max - front_move_min) / 100.0f * bask_per_front);
	}
	else if(in_vel < 0){
		out_vel = back_move_min + in_vel * ((back_move_min - back_max) / 100.0f);
	}
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, out_vel);
}

void right_servo(int in_vel){

	int out_vel;
	if(in_vel == 0){
		out_vel = 1500;
	}
	else if(in_vel > 0){
		out_vel = back_move_min - in_vel * ((back_move_min - back_max) / 100.0f);
	}
	else if(in_vel < 0){
		out_vel = front_move_min - in_vel * ((front_max - front_move_min) / 100.0f * bask_per_front);
	}
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, out_vel);
}
