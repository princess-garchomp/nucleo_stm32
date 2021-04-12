#include <stm32l476xx.h>
#include "I_O.h"

#define CONCAT(A,B) A ## B
#define CONCAT3 (A,B,C) A ## B ## C


#define GPIO_PORT(PORT_LETTER) CONCAT(GPIO,PORT_LETTER) 
#define GPIO_CLOCK_EN(PORT_LETTER) CONCAT3(RCC_AHB2ENR_GPIO,PORT_LETTER,EN)

//portA
#define LED_PIN 5
#define LED_PORT A

//originaly 0
#define LED2_PIN 0
#define LED2_PORT A

//some of these confugurations use two bits to control the function
#define TWO_CONFIG_BITS(X) X*2

//portC
#define BUTTON_PIN 13

void config_led()
{

    //periphertals on the stm32 have their clocks de-powered on startup to save power
    //need to enable clock to pins we wish to use
    //RCC->AHB2ENR |= GPIO_CLOCK_EN(LED_PORT);
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    //set mode of pin 2 as output
    // GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
    GPIO_PORT(LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(LED_PIN)));
    GPIO_PORT(LED_PORT)->MODER |= (1ul<<(TWO_CONFIG_BITS(LED_PIN)));  //mode01, digital output for normal opperation
                                                                      //mode 10 is the alternatvie function mode

    //set output type as push pull
   	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
   GPIO_PORT(LED_PORT)->OTYPER &= ~(1ul<<LED_PIN);   //push-pull

    //set output speed
   	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
   GPIO_PORT(LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(LED_PIN)));   //low speed

    //set internal resitors
    // GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
    GPIO_PORT(LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(LED_PIN))); //no pull up, no pull down

    //set the LED pin to not output voltage
    GPIO_PORT(LED_PORT)->ODR &= ~(1ul<<LED_PIN);


}
void config_led2()
{
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    GPIO_PORT(LED2_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(LED2_PIN)));
    GPIO_PORT(LED2_PORT)->MODER |= (1ul<<(TWO_CONFIG_BITS(LED2_PIN)));
   GPIO_PORT(LED2_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(LED2_PIN)));
    GPIO_PORT(LED2_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(LED2_PIN))); 
    GPIO_PORT(LED2_PORT)->ODR &= ~(1ul<<LED2_PIN);
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
    	//GPIOA->ODR |= 1 << LED_PIN;

    GPIO_PORT(LED_PORT)->ODR |= (1ul<<LED_PIN);
    //*((volatile uint32_t *)0x48000014) |=(1ul<<LED_PIN);
}
void led_off()
{
    //clear the pin associated with the LED 
    GPIO_PORT(LED_PORT)->ODR &= ~(1ul<<LED_PIN);
}

void led_toggle()
{
	GPIO_PORT(LED_PORT)->ODR ^= (1ul << LED_PIN);
}
void led2_on()
{
    GPIO_PORT(LED2_PORT)->ODR |= (1ul<<LED2_PIN);
}
void led2_off()
{
    GPIO_PORT(LED2_PORT)->ODR &= ~(1ul<<LED2_PIN);
}

_Bool is_button_pressed()
{
    return !(GPIOC->IDR & (1ul<<BUTTON_PIN));
}
