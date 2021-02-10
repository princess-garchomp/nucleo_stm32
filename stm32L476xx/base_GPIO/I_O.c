#include <stm32l476xx.h>
#include "I_O.h"

//portA
#define LED_PIN 5
//some of these confugurations use two bits to control the function
#define TWO_CONFIG_BITS(X) X*2

//portC
#define BUTTON_PIN 13

void config_led()
{
    //periphertals on the stm32 have their clocks de-powered on startup to save power
    //need to enable clock to pins we wish to use
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    //set mode of pin 2 as output
    // GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
    GPIOA->MODER &= ~(3ul<<(TWO_CONFIG_BITS(LED_PIN)));
    GPIOA->MODER |= (1ul<<(TWO_CONFIG_BITS(LED_PIN)));  //mode01, digital output

    //set output type as push pull
   	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
    GPIOA->OTYPER &= ~(1ul<<LED_PIN);   //push-pull

    //set output speed
   	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
    GPIOA->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(LED_PIN)));   //low speed

    //set internal resitors
    // GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
    GPIOA->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(LED_PIN))); //no pull up, no pull down

    //set the LED pin to not output voltage
    GPIOA->ODR &= ~(1ul<<LED_PIN);


}
void config_button()
{
    //enable clokc to peripheral
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    //set as input
    GPIOC->MODER &= ~(3ul<<(TWO_CONFIG_BITS(BUTTON_PIN)));

    //set as no pull up, no pull down
    GPIOC->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(BUTTON_PIN)));
}

void led_on()
{
    //set the pin associated with the LED
    GPIOA->ODR |= (1ul<<LED_PIN);
}
void led_off()
{
    //clear the pin associated with the LED 
    GPIOA->ODR &= ~(1ul<<LED_PIN);
}

_Bool is_button_pressed()
{
    return !(GPIOC->IDR & (1ul<<BUTTON_PIN));
}
