
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
