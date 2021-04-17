#ifndef I_O_H
#define I_O_H

#include <stdbool.h>
#include <stdint.h>

/*
there are three led's on the board
I want one function that controls all of the leds and represents a 3 bit binary value based on LED on or off

red     |   blue   |    green   |   binary  |   dec
off     |   off    |    off     |   0b000   |   0
off     |   off    |    on      |   0b001   |   1
off     |   on     |    off     |   0b010   |   2
off     |   on     |    on      |   0b011   |   3
on      |   off    |    off     |   0b100   |   4
on      |   off    |    on      |   0b101   |   5
on      |   on     |    off     |   0b110   |   6
on      |   on     |    on      |   0b111   |   7       

I want three other functions that independently controll each led

in software I will need to know not to mix the two functionalitie
led_array_binary_value() will change the state of all three
led_*colol*_on() or led_*color*_off() will only change the state of a single led
*/

typedef enum {WAIT_FOR_PUSH=0b00, PUSHED=0b01, WAIT_FOR_RELEASE=0b10, RELEASED=0b11}
	switch_state;


void config_red_led();
void config_blue_led();
void config_green_led();
void config_leds();

void red_led_on();
void red_led_off();
void blue_led_on();
void blue_led_off();
void green_led_on();
void green_led_off();

//this fucntio will ignore bits other then the 1st three LSB
void led_array_binary_value_3bit(uint8_t input);

//this fucntio will ignore bits other then the 1st two LSB
void led_array_binary_value_2bit(uint8_t input);

void config_button();

_Bool is_button_pressed();
_Bool is_button_released();

void switch_state_FSM(switch_state* g_button_state, uint32_t* g_debounce_tiemr, _Bool button_pushed, uint32_t debounce_constant);


void config_test_led();
void test_led_on();
void test_led_off();
#endif