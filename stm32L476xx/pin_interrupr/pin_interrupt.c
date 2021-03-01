#include <stm32l476xx.h>
#include "pin_interrupr.h"
#include "I_O.h"

void config_button_interupr()
{
//on my board there is 3 interrupts associated with the pins pin 1,2,3,4 you can have any port trigger these interrupts
//the defenition of IRQn_Type in the header file shows the list of interrupts available 
//I am using pin 3C in this example configured as internall pull up 

    config_button();

//enable SYSCFG clk
  RCC-> APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  
//select PA.3 as the trigger source of EXTI 3

SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PC;
SYSCFG->EXTICR[0] &= ~(0x000F);

//enable rising edge trigger for EXTI3, 0= disabled, 1=enabled
EXTI->RTSR1 |= EXTI_RTSR1_RT3;

//disable falling edge trigger for EXTI3
EXTI->FTSR1 &= ~EXTI_FTSR1_FT3;

//enable EXTI3 interrupt
EXTI->IMR1 |= EXTI_IMR1_IM3;

//setEXTI3 priority
NVIC_SetPriority(EXTI3_IRQn, 1);

//enable EXTI 3 interrupr
NVIC_EnableIRQ(EXTI3_IRQn);
}
void EXTI3_IRQHandler()
{
            led_off();
}

