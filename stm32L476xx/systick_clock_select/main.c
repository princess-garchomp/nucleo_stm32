/*#include "I_O.h"

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

int main()
{

///////////////////////////////////
/*
//the follwing show how to configure the system clock to 16MHZ with either the MSI or HSI
//and it shows who to configure the systic to fire every 1ms given the 16mhz clock
    config_systic (16000);
    config_button();
    config_led();
//enable_HSI16();
enable_MSI(_16MHZ);
*/
///////////////////////////////////

//show how to use the config_systick ms

config_systic_for_ms(16000000);
    config_button();
    config_led();
//enable_HSI16();
enable_MSI(_16MHZ);

////////////////////////////////////

/*
RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

GPIOE->MODER &= ~ (3ul<<14);
GPIOE->MODER |= (2ul<<14);

//TIM_CH1N
GPIOE->AFR[1] &= ~(0xF);
GPIOE->AFR[1] |= 1ul;

GPIOE->OSPEEDR &= ~ (3ul<<14);

GPIOE->PUPDR &= ~ (3ul<<14);

RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

//counting direction 0= count up, 1= count down
TIM1->CR1 &= ~(TIM_CR1_DIR);

//clock prescaler (16bits, up to 65535)
TIM1->PSC = 39999;

//value to count up to 
TIM1->ARR = 2000-1;

//value to compare at
TIM1->CCR1 = 500;

//main output enable, 0=disavled, 1=enabled
TIM1->BDTR |= TIM_BDTR_MOE;

//clear output compare mode bits for channel 1
TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M);

//select toggle mode
TIM1->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1;

//select output polarity, o= active high 1= active low
TIM1->CCER &= ~(TIM_CCER_CC1NP);

//enable ouptut for channel 1 complementary output
TIM1->CCER |= TIM_CCER_CC1NE;

//enable timer
TIM1->CR1 |= TIM_CR1_CEN;
*/
    while(1)
    {
        //delay of 1000 when the systic interrupt is set to fire every 1ms, gives us a 1s delay
        //delay of 1000000 when the systic interupt is set to fire every 1us, gives us a 1 us delay
        Delay(1000);
        led_on();
        Delay(1000);
        led_off();

    }
    /*
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
*/
}
//*/