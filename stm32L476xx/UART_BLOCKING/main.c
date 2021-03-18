

#include <stm32l476xx.h>
#include "I_O.h"
//#include "system_clock.h"
//#include "clock_peripherals.h"
#include "UART.h"

_Bool USART_sent_this_button_push = 0;

enum {WAIT_FOR_PUSH, PUSHED, WAIT_FOR_RELEASE, RELEASED}
	g_switch_state = WAIT_FOR_PUSH;

int g_flash_duration_selector = 1;
long  g_button_is_pushed_timer = 0;
int g_color_counter = 0;

uint8_t last_USART_RX;

void switch_state_FSM();
int main()
{
    //TIM1_CH1_cmp_output_config();

   // TIM2_interrupt_config();
    //TIM4_PWM_config();//cant use this and my uart one
    config_led();
    config_led2();
    //led2_off();
    config_button();
    config_UART_blocking_9600baud();
   // config_UART1_blocking_9600baud();
    //config_button_interupr();
   // led_on();

/*
I have my stm32 uart1 connected to an arduino uart
stm32 TX ---> arduino RX
stm32 RX ---> arduino TX

the arduino is connected to my pc via usb and I am monitoring its uart output using PuTTY so I can see what is being output
*/
        
        while(1)
        {
            //switch FSM is used to only send data opver uart once
             switch_state_FSM();

         //send data to the arduino based on button push, we send a value that is not '5' and then '5' and the PuTTY arduino sends '5' to 
         //the putty application and a '5' is printed
           if(g_switch_state == PUSHED)
            {
                //USART1_send_byte_blocking('5');
                UART_BYTE_send_blocking(USART1, '5');
            }
            if(g_switch_state == RELEASED)
            {
                //USART1_send_byte_blocking('1');
                UART_BYTE_send_blocking(USART1, '1');
            }

             //just a vusual check to see that the code is not stuck blocking anywhere
            if(is_button_pressed())
            {
                led2_on();

            }
            else 
            {
                led2_off();
            }

            //on button push the arduino will send a '5' and a 200 to the stm32
            //if I send these values to the stm32 and the led will light up accordiingly
            last_USART_RX = UART_BYTE_recieve_blocking(USART1);
            if(last_USART_RX == '5')
            {
                led_on();
            }
            else 
            {
                led_off();
            }

        }
    
}

void switch_state_FSM(){
	
	static int fake_timer =0;
	static int fake_counter_2 =0;
	switch(g_switch_state){
		case(WAIT_FOR_PUSH):
			if(is_button_pressed()){
				fake_timer ++;
			}
			else{fake_timer = 0;}
			if(fake_timer >500){
				g_switch_state = PUSHED;
			}
			else{}
			break;
		case(PUSHED):
			g_switch_state = WAIT_FOR_RELEASE;
		break;
		case(WAIT_FOR_RELEASE):
			g_button_is_pushed_timer = g_button_is_pushed_timer + 1;
			if(!(is_button_pressed())){
				fake_counter_2++;
			}
			else{fake_counter_2=0;}
			if(fake_counter_2 >500){
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
