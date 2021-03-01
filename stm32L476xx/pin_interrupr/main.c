  

#include "I_O.h"
#include "pin_interrupr.h"
#include <stm32l476xx.h>


int main(void)
{
    //when pin C3 is connected to gnd, the led will turn off
    config_button_interupr();
    config_led();
    led_on();
    while(1)
    {/*
        if(is_button_pressed())
        {
            led_off();
        }
        else 
        {
        led_on();
        }*/
    }
}