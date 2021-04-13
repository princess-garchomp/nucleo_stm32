#include "I_O.h"
#include "clock_peripherals.h"

//this value will need to change based on system clock speed
#define DEBOUNCE_THRESHOLD 1000

switch_state g_switch_state = WAIT_FOR_PUSH;
uint32_t g_switch_debounce =0;
uint32_t g_color_counter = 0;

uint32_t TIM2_entered;
void color_set_1();
/*
red led stays on untill the delay finishes, the delay is based on the timer2 interrupt
in this time we cannot change the 2 bit led array (blue and green led)
after the delayy the red led turns off and we can alter the 2 bit led array using the button 
*/
int main()
{
    config_leds();
    config_button();
    TIM2_interrupt_config();

    red_led_on();

    TIM2_entered = get_TIM2_count();
    while((get_TIM2_count() - TIM2_entered) < 2);
    red_led_off();

    while(1)
    {
        
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
