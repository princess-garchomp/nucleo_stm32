#ifndef I_O_H
#define I_O_H

#include <stdbool.h>

void config_led();
void config_button();

void led_on();
void led_off();

_Bool is_button_pressed();


#endif