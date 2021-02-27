//#include <cstdint>
#include <stm32l476xx.h>
#include "system_clock.h"


volatile uint32_t TimeDelay;


void enable_HSI16(){
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
/*
the MSI allows for a variety of clock speeds. we pass the clock speed we want to have based on the typdef MSI_clock_freq_set
*/
void enable_MSI(MSI_clock_freq_set MSI_input_freq){
//set MSI speed

    RCC->CR &= ~(0b1111<<RCC_CR_MSIRANGE_Pos);
	 RCC->CR |= (MSI_input_freq << RCC_CR_MSIRANGE_Pos);
	
	// Enable MSI
  RCC->CR |= RCC_CR_MSION_Msk;
		
  // wait until MSI is ready
  while ( (RCC->CR & RCC_CR_MSIRDY_Msk) == 0 );
	
  // Select MSI as system clock source 
  RCC->CFGR &= ~(0b11<<RCC_CFGR_SW_Pos);
  RCC->CFGR |=(RCC_CFGR_SW_MSI << RCC_CFGR_SW_Pos); // 01: HSI16 oscillator used as system clock
	


  // Wait till HSI is used as system clock source 
 // while ((RCC->CFGR != (0b01<<RCC_CFGR_SWS)));
   while ((RCC->CFGR & (0b11 << RCC_CFGR_SWS_Pos)) != (0b00<<RCC_CFGR_SWS_Pos));
   // while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}

}
/*

//there is no HSE connected to my board, need to add this hardware befoire this can work
void enable_HSE(){
//set MSI speed
   // RCC->CR &= ~(0b1111<<RCC_CR_MSIRANGE_Pos);
//	 RCC->CR |= (0b1000 << RCC_CR_MSIRANGE_Pos);
	
	// Enable HSE
  RCC->CR |= RCC_CR_HSEON_Msk;
		
  // wait until MSI is ready
  while ( (RCC->CR & RCC_CR_HSERDY_Msk) == 0 );
	
  // Select MSI as system clock source 
  RCC->CFGR &= ~(0b11<<RCC_CFGR_SW_Pos);
  RCC->CFGR |=(RCC_CFGR_SW_HSE << RCC_CFGR_SW_Pos); // 01: HSI16 oscillator used as system clock
	


  // Wait till HSI is used as system clock source 
 // while ((RCC->CFGR != (0b01<<RCC_CFGR_SWS)));
   while ((RCC->CFGR & (0b11 << RCC_CFGR_SWS_Pos)) != (0b010<<RCC_CFGR_SWS_Pos));
   // while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 ) {;}

}
*/
/*
the following is the formula for the systic interrupt period

sysatic interupt period = (1+systick_LOAD)*(1/(systick counter clock frequencey))
systick couiner frequencey is set by the HSI MSI or HSE clock.
config_systick() allows you to define the value of systick_LOAD and control the systick interrupt period
*/
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

/*Delay is based on knowing the time of the systic interrupt period
    if we have the interrupt fireing at every 1ms, then to get a 1 second delay, we need to pass 1000 as nTime
*/
void Delay(uint32_t nTime)
{
  TimeDelay=nTime;
  while(TimeDelay !=0);
}
