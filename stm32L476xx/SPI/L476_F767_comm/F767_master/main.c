#include <stm32f767xx.h>


#include "I_O.h"
//#include "system_clock.h"
#include "clock_peripherals.h"
#include "SPI.h"



#define DEBOUNCE_THRESHOLD 1000

switch_state g_switch_state = WAIT_FOR_PUSH;
uint32_t g_switch_debounce =0;
uint32_t g_color_counter = 0;


uint8_t last_USART_RX;
volatile uint8_t  g_SPI_RX_DATA = '5';

void act_on_button_push();

void send_alphabet_fsm();

int main()
{

    config_leds();
    config_button();
    //config_UART1_interupt_9600baud();
    TIM2_interrupt_config();
    config_SPI1();
    //config_test_led();

  
   // config_UART1_blocking_9600baud();


        
        while(1)
        {

            act_on_button_push();
            SPI1_select_slave();
            switch_state_FSM(&g_switch_state, &g_switch_debounce, is_button_pressed(), DEBOUNCE_THRESHOLD);

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
void SPI2_IRQHandler()
{

           g_SPI_RX_DATA = SPI_interrupt_read(SPI2);
           //led2_on();
           if(g_SPI_RX_DATA == 'b')
           {
                red_led_on();
           }
           else 
           {
              // red_led_off();
           }
           if(g_SPI_RX_DATA == '5')
           {
                blue_led_on();
           }
           else 
           {
               //blue_led_off();
           }
          // g_SPI_RX_DATA = 'q';
           
         //  led2_on();

}

void act_on_button_push()
{
	if(g_switch_state==PUSHED)
    {
        SPI_write(SPI2, '5');
		//g_color_counter = (g_color_counter + 1)%4;
		//led_array_binary_value_2bit(g_color_counter);
        green_led_on();

	}
    if(g_switch_state == RELEASED)
    {
        green_led_off();
        SPI_write(SPI2, 'w');
    }

}