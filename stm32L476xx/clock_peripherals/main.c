
/*
#include "I_O.h"

int main(void)
{
    config_button();
    config_led();
    while(1)
    {
        if(is_button_pressed())
        {
            led_off();
        }
        else 
        {
        led_on();
        }
    }
}
*/
///*
#include <stm32l476xx.h>
#include "I_O.h"
#include "system_clock.h"
#include "clock_peripherals.h"





int main()
{
    //TIM1_CH1_cmp_output_config();
    TIM7_delay_config();
    TIM1_CH2_cmp_output_config();
    TIM2_interrupt_config();
    TIM4_PWM_config();
    config_led();
    config_led2();
    //led2_off();
    config_button();
    //config_button_interupr();
   // led_on();
  
            TIM7_ms_delay(1000);
                led2_on();

            TIM7_ms_delay(1000);
                led2_off();

        
        while(1)
        {
            if((get_TIM2_count()%2)==0)
            {
                led_on();
            }
            else 
            {
                led_off();
            }
            if(is_button_pressed())
            {
                 led2_on();
            }
            else 
            {
                    led2_off();
            }


        }
    
}
