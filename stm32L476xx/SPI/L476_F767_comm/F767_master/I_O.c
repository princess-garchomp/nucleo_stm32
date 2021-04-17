#include "I_O.h"
#include <stm32f767xx.h>

#define CONCAT(A,B) A ## B


#define GPIO_PORT(PORT_LETTER) CONCAT(GPIO,PORT_LETTER) 

//some of these confugurations use two bits to control the function
#define TWO_CONFIG_BITS(X) X*2

#define RED_LED_PIN  14
#define RED_LED_PORT B

#define BLUE_LED_PIN  7
#define BLUE_LED_PORT B

#define GREEN_LED_PIN 0
#define GREEN_LED_PORT B

#define BUTTON_PIN 13
#define BUTTON_PORT C

#define TEST_LED_PIN 3
#define TEST_LED_PORT A

void config_red_led()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;//clock
    GPIO_PORT(RED_LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(RED_LED_PIN)));
    GPIO_PORT(RED_LED_PORT)->MODER |= (1ul<<(TWO_CONFIG_BITS(RED_LED_PIN))); //digital output functon
    GPIO_PORT(RED_LED_PORT)->OTYPER &= ~(1ul<<RED_LED_PIN);   //push-pull
    GPIO_PORT(RED_LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(RED_LED_PIN)));   //low speed
    GPIO_PORT(RED_LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(RED_LED_PIN))); //no pull up, no pull down
    red_led_off();
}
void config_blue_led()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;//clock
    GPIO_PORT(BLUE_LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(BLUE_LED_PIN)));
    GPIO_PORT(BLUE_LED_PORT)->MODER |= (1ul<<(TWO_CONFIG_BITS(BLUE_LED_PIN))); //digital output functon
    GPIO_PORT(BLUE_LED_PORT)->OTYPER &= ~(1ul<<BLUE_LED_PIN);   //push-pull
    GPIO_PORT(BLUE_LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(BLUE_LED_PIN)));   //low speed
    GPIO_PORT(BLUE_LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(BLUE_LED_PIN))); //no pull up, no pull down
    blue_led_off();
}
void config_green_led()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;//clock
    GPIO_PORT(GREEN_LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(GREEN_LED_PIN)));
    GPIO_PORT(GREEN_LED_PORT)->MODER |= (1ul<<(TWO_CONFIG_BITS(GREEN_LED_PIN))); //digital output functon
    GPIO_PORT(GREEN_LED_PORT)->OTYPER &= ~(1ul<<GREEN_LED_PIN);   //push-pull
    GPIO_PORT(GREEN_LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(GREEN_LED_PIN)));   //low speed
    GPIO_PORT(GREEN_LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(GREEN_LED_PIN))); //no pull up, no pull down
    green_led_off();
}
void config_leds()
{
    config_red_led();
    config_blue_led();
    config_green_led();
}

void red_led_on()
{
    GPIO_PORT(RED_LED_PORT)->ODR |= (1ul<<RED_LED_PIN);
}
void red_led_off()
{
    GPIO_PORT(RED_LED_PORT)->ODR &= ~(1ul<<RED_LED_PIN);
}
void blue_led_on()
{
    GPIO_PORT(BLUE_LED_PORT)->ODR |= (1ul<<BLUE_LED_PIN);

}
void blue_led_off()
{
    GPIO_PORT(BLUE_LED_PORT)->ODR &= ~(1ul<<BLUE_LED_PIN);

}
void green_led_on()
{
    GPIO_PORT(GREEN_LED_PORT)->ODR |= (1ul<<GREEN_LED_PIN);

}
void green_led_off()
{
    GPIO_PORT(GREEN_LED_PORT)->ODR &= ~(1ul<<GREEN_LED_PIN);
}

//this fucntio will ignore bits other then the 1st three LSB
void led_array_binary_value_3bit(uint8_t input)
{
    //there are only 8 possible values that this function will use as input
    //this shortens the input to only 3 bits
    uint8_t shorten_input = (0x7 & input);
/*
red     |   blue   |    green   |   binary  |   dec
off     |   off    |    off     |   0b000   |   0
off     |   off    |    on      |   0b001   |   1
off     |   on     |    off     |   0b010   |   2
off     |   on     |    on      |   0b011   |   3
on      |   off    |    off     |   0b100   |   4
on      |   off    |    on      |   0b101   |   5
on      |   on     |    off     |   0b110   |   6
on      |   on     |    on      |   0b111   |   7   
*/
    switch(shorten_input)
    {
        case(0b000):
            red_led_off();
            blue_led_off();
            green_led_off();
        break;

        case(0b001):
            red_led_off();
            blue_led_off();
            green_led_on();
        break;

        case(0b010):
            red_led_off();
            blue_led_on();
            green_led_off();
        break;

        case(0b011):
            red_led_off();
            blue_led_on();
            green_led_on();
        break;

        case(0b100):
            red_led_on();
            blue_led_off();
            green_led_off();
        break;

        case(0b101):
            red_led_on();
            blue_led_off();
             green_led_on();
        break;

        case(0b110):
            red_led_on();
             blue_led_on();
            green_led_off();
        break;

        case(0b111):
            red_led_on();
            blue_led_on();
            green_led_on();
        break;
    }
}

void led_array_binary_value_2bit(uint8_t input)
{
    //there are only 8 possible values that this function will use as input
    //this shortens the input to only 3 bits
    uint8_t shorten_input = (0x3 & input);
/*
blue   |    green   |   binary  |   dec
off    |    off     |   0b000   |   0
off    |    on      |   0b001   |   1
on     |    off     |   0b010   |   2
on     |    on      |   0b011   |   3
*/
    switch(shorten_input)
    {
        case(0b00):
            blue_led_off();
            green_led_off();
        break;
        
        case(0b01):
            blue_led_off();
            green_led_on();
        break;

        case(0b10):
            blue_led_on();
            green_led_off();
        break;

        case(0b11):
            blue_led_on();
            green_led_on();
        break;
    }
}


void config_button()
{
    //enable clokc to peripheral
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    //set as input
    GPIO_PORT(BUTTON_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(BUTTON_PIN)));

    //set as no pull up, no pull down
    GPIO_PORT(BUTTON_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(BUTTON_PIN)));
}

_Bool is_button_pressed()
{
    return (GPIO_PORT(BUTTON_PORT)->IDR & (1ul<<BUTTON_PIN));
}
_Bool is_button_released()
{
    return !(is_button_pressed());
}

void switch_state_FSM(switch_state* g_button_state, uint32_t* g_debounce_tiemr, _Bool button_pushed, uint32_t debounce_constant)
{
	switch(*g_button_state)
    {
		case(WAIT_FOR_PUSH):
			if(button_pushed==1){
				*g_debounce_tiemr=*g_debounce_tiemr + 1;
				if(*g_debounce_tiemr>debounce_constant){
					*g_debounce_tiemr=0;
					*g_button_state=PUSHED;
				}
				else{}
			}
			else{*g_debounce_tiemr=0;}
		break;
			
		case(PUSHED):
			*g_button_state=WAIT_FOR_RELEASE;
		break;
			
		case(WAIT_FOR_RELEASE):
			if(button_pushed==0){
				*g_debounce_tiemr=*g_debounce_tiemr + 1;
				if(*g_debounce_tiemr>debounce_constant){
					*g_debounce_tiemr=0;
					*g_button_state=RELEASED;
				}
				else{}
			}
			else{*g_debounce_tiemr=0;}
		break;
		
		case(RELEASED):
			*g_button_state=WAIT_FOR_PUSH;
		break;
	}	
	
}

void config_test_led()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;//clock
    GPIO_PORT(TEST_LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(TEST_LED_PIN)));
    GPIO_PORT(TEST_LED_PORT)->MODER |= (1ul<<(TWO_CONFIG_BITS(TEST_LED_PIN))); //digital output functon
    GPIO_PORT(TEST_LED_PORT)->OTYPER &= ~(1ul<<TEST_LED_PIN);   //push-pull
    GPIO_PORT(TEST_LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(TEST_LED_PIN)));   //low speed
    GPIO_PORT(TEST_LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(TEST_LED_PIN))); //no pull up, no pull down
    test_led_off();
}
void test_led_on()
{
    GPIO_PORT(TEST_LED_PORT)->ODR |= (1ul<<TEST_LED_PIN);
}
void test_led_off()
{
    GPIO_PORT(TEST_LED_PORT)->ODR &= ~(1ul<<TEST_LED_PIN);
}