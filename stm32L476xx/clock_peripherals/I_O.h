#ifndef I_O_H
#define I_O_H

#include <stdbool.h>

void config_led();
void config_led2();
void config_button();

void led_on();
void led_off();
void led_toggle();

void led2_on();
void led2_off();

_Bool is_button_pressed();


#endif