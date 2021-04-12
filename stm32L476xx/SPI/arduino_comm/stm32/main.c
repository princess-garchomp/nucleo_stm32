

#include <stm32l476xx.h>
#include "I_O.h"
//#include "system_clock.h"
#include "clock_peripherals.h"
#include "UART.h"
#include "SPI.h"



enum {WAIT_FOR_PUSH, PUSHED, WAIT_FOR_RELEASE, RELEASED}
	g_switch_state = WAIT_FOR_PUSH;

int g_flash_duration_selector = 1;
long  g_button_is_pushed_timer = 0;
int g_color_counter = 0;

uint8_t last_USART_RX;
volatile uint8_t  g_SPI_RX_DATA = '5';

void switch_state_FSM();
void send_alphabet_fsm();

int main()
{

    config_led();
    config_led2();
    config_button();
    //config_UART1_interupt_9600baud();
    TIM2_interrupt_config();
   config_SPI1();
   NVIC_EnableIRQ(SPI1_IRQn);
  
   // config_UART1_blocking_9600baud();


        
        while(1)
        {

            
             SPI1_select_slave();
             switch_state_FSM();



           if(g_switch_state == PUSHED)
            {
                SPI1_select_slave();
                SPI_write(SPI1, g_SPI_RX_DATA);      
            }
            if(g_switch_state == RELEASED)
            {

            }

             

            if(is_button_pressed())
            {
               // led2_on();

            }
            else 
            {
                //led2_off();
            }



            
        }
    
   
}

void switch_state_FSM(){
	
	static int fake_timer =0;
	static int fake_counter_2 =0;
	switch(g_switch_state){
		case(WAIT_FOR_PUSH):
			if(is_button_pressed())
            {
				fake_timer ++;
			}
			else{fake_timer = 0;}

			if(fake_timer >500)
            {
				g_switch_state = PUSHED;
			}
			else{}

			break;
		case(PUSHED):
			g_switch_state = WAIT_FOR_RELEASE;
		break;
		case(WAIT_FOR_RELEASE):
			g_button_is_pushed_timer++;
			if(!(is_button_pressed()))
            {
				fake_counter_2++;
			}
			else{fake_counter_2=0;}
			if(fake_counter_2 >500)
            {
				g_switch_state = RELEASED;
			}
			else{}
		break;

		case(RELEASED):
			g_button_is_pushed_timer = 0;
			g_switch_state = WAIT_FOR_PUSH;
		break;
	}
}

void send_alphabet_fsm()
{
	static enum {UPDATE_STATE, WAIT_STATE, SEND_STATE}
		local_send_fsm_state = WAIT_STATE;
		
	static unsigned long time_entered = 0;
	static uint8_t send_incramenter=0;

    static uint8_t alphabet_read = '1';

	switch(local_send_fsm_state)
	{
		case(UPDATE_STATE):
		time_entered = get_TIM2_count();
		send_incramenter = (send_incramenter+1)%26;
		local_send_fsm_state = WAIT_STATE;
		break;
		
		case(WAIT_STATE):
        //SPI_write(SPI1,('a'+ send_incramenter));

		if((get_TIM2_count() - time_entered) > 1)
		{
			local_send_fsm_state = SEND_STATE;
		}
		break;
		
		case(SEND_STATE):
      //  SPI_write(SPI1, alphabet_read);
        SPI_write(SPI1,('a'+ send_incramenter));
       // alphabet_read = SPI_read(SPI1);
		local_send_fsm_state = UPDATE_STATE;
		break;
	}
}
void SPI1_IRQHandler()
{

           g_SPI_RX_DATA = SPI_interrupt_read(SPI1);
           //led2_on();
           if(g_SPI_RX_DATA == 'b')
           {
                led2_on();
           }
           else 
           {
               led2_off();
           }
           if(g_SPI_RX_DATA == '5')
           {
                led_on();
           }
           else 
           {
               led_off();
           }
           
         //  led2_on();

}

