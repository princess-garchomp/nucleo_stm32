#include "I_O.h"
#include "system_clock.h"

//this value will need to change based on system clock speed
#define DEBOUNCE_THRESHOLD 1000

switch_state g_switch_state = WAIT_FOR_PUSH;
uint32_t g_switch_debounce =0;
uint32_t g_color_counter = 0;

void color_set_1();
/*
red led stays on untill the delay finishes
in this time we cannot change the 2 bit led array (blue and green led)
after the delayy the red led turns off and we can alter the 2 bit led array using the button 
*/
int main()
{
    config_leds();
    config_button();
    config_systic_for_ms(16000000);
    red_led_on();
    Delay(1000);
    red_led_off();

    while(1)
    {
        /*
        //this show the leds on and off individulally and that the psuh button works
      if (is_button_pressed()) 
      {
      //  led_array_binary_value(0);
      red_led_on();
      blue_led_on();
      green_led_on();
      } 
      else 
      {
      //  led_array_binary_value(3);
      red_led_off();
      blue_led_off();
      green_led_off();
      }
      */
        //the following proves my binary_array function works as expected
        color_set_1();
        switch_state_FSM(&g_switch_state, &g_switch_debounce, is_button_pressed(), DEBOUNCE_THRESHOLD);
      }
}

void color_set_1(){
	if(g_switch_state==PUSHED){
		g_color_counter = (g_color_counter + 1)%4;
		led_array_binary_value_2bit(g_color_counter);
	}
	else{}
}
