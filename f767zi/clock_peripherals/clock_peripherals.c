#include <stm32f767xx.h>

#include "clock_peripherals.h"
#include "system_clock.h"
#include "I_O.h"

#define TWO_CONFIG_BITS(X) X*2
#define CONCAT(A,B) A ## B
#define GPIO_PORT(PORT_LETTER) CONCAT(GPIO,PORT_LETTER) 

volatile uint32_t counter;


void TIM2_interrupt_config()
{
    enable_HSI16();

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

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

