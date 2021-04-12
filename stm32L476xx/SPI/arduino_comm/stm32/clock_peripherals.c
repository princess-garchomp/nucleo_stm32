#include <stm32l476xx.h>

#include "clock_peripherals.h"
#include "system_clock.h"
#include "I_O.h"
/*
this is example code for a variety of functions using the timer peripherals
if a pin is being used as an output for a timer, assume that there is an led connected to it via its cathoed, and that the led is also connected to gnd

this is missing an example of an ouptut capture, will do that at a latter time
*/

#define TWO_CONFIG_BITS(X) X*2


#define CONCAT(A,B) A ## B
#define CONCAT3 (A,B,C) A ## B ## C


#define GPIO_PORT(PORT_LETTER) CONCAT(GPIO,PORT_LETTER) 
#define GPIO_CLOCK_EN(PORT_LETTER) CONCAT3(RCC_AHB2ENR_GPIO,PORT_LETTER,EN)

//portA


#define TM1_CH1N_LED 7
#define TM1_CH1_LED 8
#define TM1_CH1_LEDS_PORT A

#define START_AFR_PA7 28
#define START_AFR_PA8 0

#define TM1_CH2N_LED 0
#define TM1_CH2_LED 9
#define TM1_CH2N_LED_PORT B
#define TM1_CH2_LED_PORT A


#define START_AFR_PA9 4
#define START_AFR_PB0 0

#define TIM4_CH1_LED 6
#define TIM4_CH1_LED_PORT B
#define START_AFR_PB6 24

#define TIM4_CH2_LED 7
#define TIM4_CH2_LED_PORT B
#define START_AFR_PB7 28

volatile uint32_t counter;


void TIM1_CH1_cmp_output_config()
{
enable_HSI16();
///////////////////////////////////////////////////////////config pins connected to timer1 ch1N and ch1
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    //set mode of pin 2 as output
    // GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
    GPIO_PORT(TM1_CH1_LEDS_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH1N_LED)));
    GPIO_PORT(TM1_CH1_LEDS_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(TM1_CH1N_LED))); //mode 10 is the alternatvie function mode

    //set output speed
   	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
   GPIO_PORT(TM1_CH1_LEDS_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH1N_LED)));   //low speed

    //set internal resitors
    // GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
    GPIO_PORT(TM1_CH1_LEDS_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH1N_LED))); //no pull up, no pull down

    GPIOA->AFR[0] &= ~(0xF<<START_AFR_PA7);
    GPIOA->AFR[0] |= (1ul<<START_AFR_PA7);
    //select alternative function for PA7
    //afr[0] for pins 0 to 7

    //set mode of pin 2 as output
    // GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
    GPIO_PORT(TM1_CH1_LEDS_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH1_LED)));
    GPIO_PORT(TM1_CH1_LEDS_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(TM1_CH1_LED)));//mode 10 is the alternatvie function mode

   	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
   GPIO_PORT(TM1_CH1_LEDS_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH1_LED)));   //low speed

    //set internal resitors
    // GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
    GPIO_PORT(TM1_CH1_LEDS_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH1_LED))); //no pull up, no pull down

    GPIOA->AFR[1] &= ~(0xF<<START_AFR_PA8);
    GPIOA->AFR[1] |= (1ul<<START_AFR_PA8);
    //select alternative function for PA8
    //afr[1] for pins 8,9,11 to 16

////////////////////////////////////////////////////////////////////////config TIM1 timer
/*
counting period = (1+ARR)*(1/f_timer_peripheral) 
f_timer_peripheral=(f_systic_clock)/(prescaler+1)

my goal is to have the counting period be one scecond using a systick clock of 16MHZ
    
    set the prescaler such that f_tiemr_peripheral is 2KHZ
    f_timer_peripheral=(16MHZ)/(prescaler+1)=2KHZ
        prescaler = 7999
    
    now find ARR from the given information
        counting period = (1+ARR)*(1/2khz) =1sec
        ARR = 1999

    my compare can be between 0 and 1999
*/
RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

//counting direction 0= count up, 1= count down
TIM1->CR1 &= ~(TIM_CR1_DIR);

//clock prescaler (16bits, up to 65535)
//TIM1->PSC = 39999;
TIM1->PSC = 7999;

//value to count up to 
TIM1->ARR = 2000-1;

//value to compare at
TIM1->CCR1 = 0;

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

//enable ouptut for channel 1 output
TIM1->CCER |= TIM_CCER_CC1E;

//enable timer
TIM1->CR1 |= TIM_CR1_CEN;
}

void TIM1_CH2_cmp_output_config()
{
    TIM1_CH1_cmp_output_config();
    ///////////////////////////////////////////////////////////config pins connected to timer1 ch1N and ch1
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    //set mode of pin 2 as output
    // GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
    GPIO_PORT(TM1_CH2N_LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH2N_LED)));
    GPIO_PORT(TM1_CH2N_LED_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(TM1_CH2N_LED))); //mode 10 is the alternatvie function mode

    //set output speed
   	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
   GPIO_PORT(TM1_CH2N_LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH2N_LED)));   //low speed

    //set internal resitors
    // GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
    GPIO_PORT(TM1_CH2N_LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH2N_LED))); //no pull up, no pull down

    GPIOA->AFR[0] &= ~(0xF<<START_AFR_PB0);
    GPIOA->AFR[0] |= (1ul<<START_AFR_PB0);
    //select alternative function for PB0
    //afr[0] for pins 0 to 7

    //set mode of pin 2 as output
    // GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
    GPIO_PORT(TM1_CH2_LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH2_LED)));
    GPIO_PORT(TM1_CH2_LED_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(TM1_CH2_LED)));//mode 10 is the alternatvie function mode

   	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
   GPIO_PORT(TM1_CH2_LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH2_LED)));   //low speed

    //set internal resitors
    // GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
    GPIO_PORT(TM1_CH2_LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(TM1_CH2_LED))); //no pull up, no pull down

    GPIOA->AFR[1] &= ~(0xF<<START_AFR_PA9);
    GPIOA->AFR[1] |= (1ul<<START_AFR_PA9);
    //select alternative function for PA9
    //afr[1] for pins 8,9,11 to 16


    /////////////////timer config
    TIM1->CCR2 = 0;
//clear output compare mode bits for channel 1
TIM1->CCMR1 &= ~(TIM_CCMR1_OC2M);

//select toggle mode
TIM1->CCMR1 |= TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2M_1;

//select output polarity, o= active high 1= active low
TIM1->CCER &= ~(TIM_CCER_CC2NP);

    //enable ouptut for channel 1 complementary output
//TIM1->CCER |= TIM_CCER_CC2NE;
TIM1->CCER |= (1ul<<6);


//enable ouptut for channel 1 output
TIM1->CCER |= TIM_CCER_CC2E;
}

void TIM2_interrupt_config()
{
    enable_HSI16();

    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;

    //counting direction 0= count up, 1= count down
    TIM2->CR1 &= ~(TIM_CR1_DIR);

    //clock prescaler (16bits, up to 65535)
    //TIM1->PSC = 39999;
    TIM2->PSC = 7999;

    //value to count up to 
    TIM2->ARR = 2000-1;

    //value to compare at
    TIM2->CCR1 = 0;

    //enable undate interrupts
    TIM2->DIER |= TIM_DIER_UIE;

    NVIC_SetPriority(TIM2_IRQn, 1);


    //enable nvic
    NVIC_EnableIRQ(TIM2_IRQn);

    //enable timer
    TIM2->CR1 |= TIM_CR1_CEN;


}

void TIM2_IRQHandler()
{
    if((TIM2->SR & TIM_SR_UIF) != 0)
    {
    counter++;
    TIM2->SR &= ~TIM_SR_UIF;
    }
}

uint32_t get_TIM2_count()
{
    return counter;
}

void TIM7_delay_config()
{
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM7EN;
    TIM7->PSC=15999;  //16MHZ/(x+value)=1khz
}

void TIM7_ms_delay(uint16_t input)
{
    if(input==0)
    {
    return;
    }
    TIM7->CR1 &= ~TIM_CR1_CEN;      //disable while it is configed
    TIM7->SR=0;
    TIM7->CNT=0;
    TIM7->ARR = input-1;
    TIM7->CR1|= TIM_CR1_CEN;        //enable the thing 

    while ((TIM7->SR & TIM_SR_UIF) == 0);


}

void TIM4_PWM_config()
{//I am using the same timer 4 PWM to control two ouptts on differnt channgels, setting CCRn to differnet values creates differnt duty cycles
    //pb6 tim4ch1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    GPIO_PORT(TIM4_CH1_LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(TIM4_CH1_LED)));
    GPIO_PORT(TIM4_CH1_LED_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(TIM4_CH1_LED)));//alt function
    GPIO_PORT(TIM4_CH1_LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(TIM4_CH1_LED)));
    GPIO_PORT(TIM4_CH1_LED_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(TIM4_CH1_LED)));//high speed
    GPIO_PORT(TIM4_CH1_LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(TIM4_CH1_LED)));//no pu/pd resitor
    GPIO_PORT(TIM4_CH1_LED_PORT)->OTYPER &= ~(1ul<<(TWO_CONFIG_BITS(TIM4_CH1_LED)));//0 is push pull, 1 is open drain 
    GPIO_PORT(TIM4_CH1_LED_PORT)->AFR[0] &= ~(0xF<<START_AFR_PB6);
    GPIO_PORT(TIM4_CH1_LED_PORT)->AFR[0] |= (2ul<<START_AFR_PB6);

    //pb7 tim4ch1
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    GPIO_PORT(TIM4_CH2_LED_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(TIM4_CH2_LED)));
    GPIO_PORT(TIM4_CH2_LED_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(TIM4_CH2_LED)));//alt function
    GPIO_PORT(TIM4_CH2_LED_PORT)->OSPEEDR &= ~(3ul<<(TWO_CONFIG_BITS(TIM4_CH2_LED)));
    GPIO_PORT(TIM4_CH2_LED_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(TIM4_CH2_LED)));//high speed
    GPIO_PORT(TIM4_CH2_LED_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(TIM4_CH2_LED)));//no pu/pd resitor
    GPIO_PORT(TIM4_CH2_LED_PORT)->OTYPER &= ~(1ul<<(TWO_CONFIG_BITS(TIM4_CH2_LED)));//0 is push pull, 1 is open drain 
    GPIO_PORT(TIM4_CH2_LED_PORT)->AFR[0] &= ~(0xF<<START_AFR_PB7);
    GPIO_PORT(TIM4_CH2_LED_PORT)->AFR[0] |= (2ul<<START_AFR_PB7);
 
    enable_HSI16();
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN; 
    TIM4->CR1 &= ~TIM_CR1_DIR;//upcount
    TIM4->PSC = 159; //16MHZ/(159+1) = 100khz
    TIM4-> ARR=999; //pwm period=(999+1)*(1/100khz)=/01s
    TIM4->CCMR1 &= ~TIM_CCMR1_OC1M; TIM4->CCMR1 |= TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2;//this enables channel one output as PWM (OX1M=110)
    TIM4->CCMR1 |= TIM_CCMR1_OC1PE; //ouput preload enable
    TIM4->CCER &= ~TIM_CCER_CC1NP; TIM4->CCER |= TIM_CCER_CC1E; //ouptu polarity active high, enale CH1 ouprt
    TIM4->BDTR |= TIM_BDTR_MOE;
    TIM4->CCR1 = 150; //set duty cycle to 15%
    TIM4->CR1 |= TIM_CR1_CEN;
    //config tim4ch2
    TIM4->CCMR1 &= ~TIM_CCMR1_OC2M; TIM4->CCMR1 |= TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2;//this enables channel one output as PWM (OX1M=110)
    TIM4->CCER &= ~TIM_CCER_CC2NP; TIM4->CCER |= TIM_CCER_CC2E; //ouptu polarity active high, enale CH1 ouprt
    TIM4->CCR2 = 20; //set duty cycle to 2%

    }
