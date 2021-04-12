#include <avr/io.h>
#include "I_O_board.h"

#define MASK(X) (1ul<<(X))

/*
 corresponding arduino pins:

	FIRST
		13
		12
		11
	SECOND
		10
		9
		8
	BUTTONS
		7           a
		6           e
		5           i
*/

#define FIRST_RGB_LED_RED_SHIFT (PC5)
#define FIRST_RGB_LED_GREEN_SHIFT (PC4)
#define FIRST_RGB_LED_BLUE_SHIFT (PC3)

#define SECOND_RGB_LED_RED_SHIFT (PB2)
#define SECOND_RGB_LED_GREEN_SHIFT (PB1)
#define SECOND_RGB_LED_BLUE_SHIFT (PB0)

#define A_BUTTON_SHIFT (PD7)
#define E_BUTTON_SHIFT (PD6)
#define I_BUTTON_SHIFT (PD5)



static void first_rgb_led_red_on();
static void first_rgb_led_red_off();
static void first_rgb_led_green_on();
static void first_rgb_led_green_off();
static void first_rgb_led_blue_on();
static void first_rgb_led_blue_off();


static void second_rgb_led_red_on();
static void second_rgb_led_red_off();
static void second_rgb_led_green_on();
static void second_rgb_led_green_off();
static void second_rgb_led_blue_on();
static void second_rgb_led_blue_off();

void all_I_O_config(){
	first_rgb_led_config();
	second_rgb_led_config();
	switches_config();
}
void first_rgb_led_config(){
	DDRC |= MASK(FIRST_RGB_LED_RED_SHIFT)|MASK(FIRST_RGB_LED_GREEN_SHIFT)|MASK(FIRST_RGB_LED_BLUE_SHIFT);
	first_rgb_led_color(WHITE);
}
void second_rgb_led_config(){
	DDRB |= MASK(SECOND_RGB_LED_RED_SHIFT)|MASK(SECOND_RGB_LED_GREEN_SHIFT)|MASK(SECOND_RGB_LED_BLUE_SHIFT);
	second_rgb_led_color(WHITE);
}
void A_switch_config(){
	DDRD &= ~(MASK(A_BUTTON_SHIFT));
	PORTD|= MASK(A_BUTTON_SHIFT);
}
void switches_config(){
	DDRD &= ~(MASK(A_BUTTON_SHIFT))& ~(MASK(E_BUTTON_SHIFT))& ~(MASK(I_BUTTON_SHIFT));
	PORTD |= MASK(A_BUTTON_SHIFT)|(MASK(E_BUTTON_SHIFT))|(MASK(I_BUTTON_SHIFT));
}


void first_rgb_led_color(rgb_led_color color){
	switch(color){
		case(WHITE):
			first_rgb_led_red_off();
			first_rgb_led_green_off();
			first_rgb_led_blue_off();
		break;		
		case(RED):
			first_rgb_led_red_off();
			first_rgb_led_green_off();
			first_rgb_led_blue_on();
		break;		
		case(GREEN):
			first_rgb_led_red_off();
			first_rgb_led_green_on();
			first_rgb_led_blue_off();
		break;		
		case(CYAN):
			first_rgb_led_red_off();
			first_rgb_led_green_on();
			first_rgb_led_blue_on();
		break;		
		case(BLUE):
			first_rgb_led_red_on();
			first_rgb_led_green_off();
			first_rgb_led_blue_off();
		break;
		case(MAGENTA):
			first_rgb_led_red_on();
			first_rgb_led_green_off();
			first_rgb_led_blue_on();
		break;		
		case(YELLOW):
			first_rgb_led_red_on();
			first_rgb_led_green_on();
			first_rgb_led_blue_off();
		break;	
		case(BLACK):
			first_rgb_led_red_on();
			first_rgb_led_green_on();
			first_rgb_led_blue_on();
		break;
	}
}

void second_rgb_led_color(rgb_led_color color){
	switch(color){
		case(WHITE):
			second_rgb_led_red_off();
			second_rgb_led_green_off();
			second_rgb_led_blue_off();
		break;
		case(BLUE):
			second_rgb_led_red_off();
			second_rgb_led_green_off();
			second_rgb_led_blue_on();
		break;		
		case(GREEN):
			second_rgb_led_red_off();
			second_rgb_led_green_on();
			second_rgb_led_blue_off();
		break;		
		case(CYAN):
			second_rgb_led_red_off();
			second_rgb_led_green_on();
			second_rgb_led_blue_on();
		break;		
		case(RED):
			second_rgb_led_red_on();
			second_rgb_led_green_off();
			second_rgb_led_blue_off();
		break;		
		case(MAGENTA):
			second_rgb_led_red_on();
			second_rgb_led_green_off();
			second_rgb_led_blue_on();
		break;	
		case(YELLOW):
			second_rgb_led_red_on();
			second_rgb_led_green_on();
			second_rgb_led_blue_off();
		break;	
		case(BLACK):
			second_rgb_led_red_on();
			second_rgb_led_green_on();
			second_rgb_led_blue_on();
		break;	
	}
}


_Bool A_is_pressed(){
	return!(PIND & MASK(A_BUTTON_SHIFT));
}
_Bool A_is_not_pressed(){
	return!(A_is_pressed());
}
_Bool E_is_pressed(){
	return!(PIND & MASK(E_BUTTON_SHIFT));
}
_Bool E_is_not_pressed(){
	return!(E_is_pressed());
}
_Bool I_is_pressed(){
	return!(PIND & MASK(I_BUTTON_SHIFT));
}
_Bool I_is_not_pressed(){
	return!(I_is_pressed());
}

static void first_rgb_led_red_on(){
	PORTC|= MASK(FIRST_RGB_LED_RED_SHIFT);
}
static void first_rgb_led_red_off(){
	PORTC &= ~MASK(FIRST_RGB_LED_RED_SHIFT);
}
static void first_rgb_led_green_on(){
	PORTC |= MASK(FIRST_RGB_LED_GREEN_SHIFT);
}
static void first_rgb_led_green_off(){
	PORTC &= ~MASK(FIRST_RGB_LED_GREEN_SHIFT);
}
static void first_rgb_led_blue_on(){
	PORTC |= MASK(FIRST_RGB_LED_BLUE_SHIFT);
}
static void first_rgb_led_blue_off(){
	PORTC &= ~MASK(FIRST_RGB_LED_BLUE_SHIFT);
}

static void second_rgb_led_red_on(){
	PORTB |= MASK(SECOND_RGB_LED_RED_SHIFT);
}
static void second_rgb_led_red_off(){
	PORTB &= ~MASK(SECOND_RGB_LED_RED_SHIFT);
}
static void second_rgb_led_green_on(){
	PORTB |= MASK(SECOND_RGB_LED_GREEN_SHIFT);
}
static void second_rgb_led_green_off(){
	PORTB &= ~MASK(SECOND_RGB_LED_GREEN_SHIFT);
}
static void second_rgb_led_blue_on(){
	PORTB |= MASK(SECOND_RGB_LED_BLUE_SHIFT);
}
static void second_rgb_led_blue_off(){
	PORTB &= ~MASK(SECOND_RGB_LED_BLUE_SHIFT);
}