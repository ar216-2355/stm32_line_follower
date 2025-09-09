#pragma once
#include <cstdint>

// マイコンはblue pillを想定
// ライントレーサーのLEDの番号割り当ては左から順に1,2,3,4

namespace LED{

void write_led(int led_num, int duty);

//----------------------------------------------------------------------------------以下LED用の使用可能関数

void on(int led_num);                                 // 指定したLEDをON
void all_on();                                        // 全てのLEDをON
void off(int led_num);                                // 指定したLEDをOFF
void all_off();                                       // 全てのLEDをOFF
void toggle(int led_num);                             // 指定したLEDをトグル
void all_toggle();                                    // 全てのLEDをトグル（少しでも光ってたらOFFになる）
void write_mask(int led_nums);                        // 引数に`0b1010`と入れれば、一番左のLEDと右から２番目のLEDがつく
void write_mask_brightness(int led_nums, int duty);
void set_brightness(int led_num, int duty);           // 指定したLEDの明るさを変更　　0% -> 0  ,  100% -> 1000

//------------------------------------------------------------------------------------------------------

}
