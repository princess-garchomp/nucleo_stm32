#include <avr/io.h>
#include <avr/interrupt.h>

#include "I_O_board.h"
#include "UART_interrupt.h"

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

uint8_t data_in = 1;

void send_data();
void check_recieved_data();

int main(void)
{
all_I_O_config();
cli();
USART_config(MYUBRR);
sei();
    while (1) 
    {
	//	first_rgb_led_color(MAGENTA);
		send_data();
		check_recieved_data();
		//first_rgb_led_color(GREEN);
		
    }
}

// ISR(USART0_TX_vect){
// 	
// }
ISR(USART_RX_vect){
	data_in = UDR0;
			//first_rgb_led_color(BLACK);
}

void send_data(){
	USART_Transmit('5', A_is_pressed());
	USART_Transmit(200,E_is_pressed());
	USART_Transmit(256,I_is_pressed());
}

void check_recieved_data(){
	static uint8_t old_data_in = 1;
	if(old_data_in==data_in){
		return;
	}
	else{
		if(data_in == '5'){
			USART_Transmit(data_in, 1);
			first_rgb_led_color(BLUE);
		}
		else if(data_in == 200){
			first_rgb_led_color(RED);
		}
		else if (data_in == 1){
			first_rgb_led_color(GREEN);
		}
		else{
			first_rgb_led_color(BLACK);		
		}
	}
		old_data_in = data_in;
	
}