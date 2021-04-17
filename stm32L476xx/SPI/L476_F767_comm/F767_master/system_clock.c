#include <stm32f767xx.h>
#include "system_clock.h"

volatile uint32_t TimeDelay;

void enable_HSI16()
{
	// Enable High Speed Internal Clock (HSI = 16 MHz)
  RCC->CR |= RCC_CR_HSION_Msk;
	
  // wait until HSI is ready
  while ( (RCC->CR & RCC_CR_HSIRDY_Msk) == 0 );
	
  // Select HSI as system clock source 
  RCC->CFGR &= ~(0b11<<RCC_CFGR_SW_Pos);
  RCC->CFGR |=(RCC_CFGR_SW_HSI << RCC_CFGR_SW_Pos); // 01: HSI16 oscillator used as system clock

  // Wait till HSI is used as system clock source 
 // while ((RCC->CFGR != (0b01<<RCC_CFGR_SWS)));
   while ((RCC->CFGR & (0b11 << RCC_CFGR_SWS_Pos)) != (0b00<<RCC_CFGR_SWS_Pos));
   // while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}

}

void config_systic (uint32_t ticks)
{
  //disable systic IRQ and systic conter
  SysTick->CTRL =0;
  
  //set reload register
  SysTick->LOAD = ticks -1;
  
  //set interrupt priority of systic
  NVIC_SetPriority (SysTick_IRQn, (1ul<<__NVIC_PRIO_BITS)-1);
  
  ////reset systic counter
  SysTick->VAL=0;
  
  //select processor clock
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  
  //enable exception reqiest
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  
  ////enable systick timer
  //SysTick->CTRL |= (1ul<<0);
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
void config_systic_for_ms (uint32_t sysclock_freq_input)
{
    enable_HSI16();
  //disable systic IRQ and systic conter
  SysTick->CTRL =0;
  
  //set reload register
  /*
  systicinterruptperiod = (1+systick_LOAD)/systick_counter_clock_freq
  systick_LOAD = (systicinterruptperiod * systick_counter_clock_freq) -1
  systicinterruptperiod = 1ms =.001s 
  systick_counter_clock_freq = sysclock_freq_input
  */
  SysTick->LOAD = ((.001*sysclock_freq_input)-1);
  
  //set interrupt priority of systic
  NVIC_SetPriority (SysTick_IRQn, (1ul<<__NVIC_PRIO_BITS)-1);
  
  ////reset systic counter
  SysTick->VAL=0;
  
  //select processor clock
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  
  //enable exception reqiest
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  
  ////enable systick timer
  //SysTick->CTRL |= (1ul<<0);
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler (void)
{
         // led_on();

  if (TimeDelay>0)
   {
      TimeDelay--;//variable is global and volatile
   }
}

void Delay(uint32_t nTime)
{
  TimeDelay=nTime;
  while(TimeDelay !=0);
}