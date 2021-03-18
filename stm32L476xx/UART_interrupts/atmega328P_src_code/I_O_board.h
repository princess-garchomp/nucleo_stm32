#ifndef I_O_BOARD_H
#define I_O_BOARD_H

typedef enum {WHITE, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, BLACK}
	rgb_led_color;

//this configs the entier driver if there is no need to use the pins for other shit
void all_I_O_config();

//configing first and second seperately so if i need the pins they are avalible for other drivers


void first_rgb_led_config();
void second_rgb_led_config();

//configning A seperatly again to save pins if i need them for other drivers

void A_switch_config();
void switches_config();

void first_rgb_led_color(rgb_led_color color);
void second_rgb_led_color(rgb_led_color color);
	
_Bool A_is_pressed();
_Bool A_is_not_pressed();

_Bool E_is_pressed();
_Bool E_is_not_pressed();

_Bool I_is_pressed();
_Bool I_is_not_pressed();


#endif