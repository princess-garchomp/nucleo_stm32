/*
this is spi slave
this prints over serial port (com5) to the terminal applivation putty via uart after spi data is reviebed from the master
in this way I am visulaizing what data is being sent to the slave over SPI
 
the master initaiates a send by sending '0' over the MOSI line at this point the LED turns green and the slave sends 'b' over the MISO line
the slave should print a '0' via uart to the terminal application
next after a delay the master sends 'b +3" over the MOSI line   
the slave LED now turns red and it should print 'e' via uart to the terminal application
*/


#include <avr/io.h>
#include <avr/interrupt.h>

#include "I_O_board.h"
#include "UART_interrupt.h"
#include "SPI.h"
#include "timer_global_count.h"

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

volatile uint8_t g_data = 'b';


void send_data();

int main(void)
{
	all_I_O_config();
	cli();
	USART_config(MYUBRR);
	config_SPI_slave();
	SPDR = g_data;
	sei();
	//while(1){first_rgb_led_color(BLUE);}

	while (1)
	{
		
		send_data();	
		/*if (g_data == '0')
		{
			slave_SPI_send(g_data);
			first_rgb_led_color(GREEN);
		}*/
	}
}

void send_data()
{
	USART_Transmit_with_button_push('5', A_is_pressed());
	USART_Transmit_with_button_push('6',E_is_pressed());
}

ISR(SPI_STC_vect)
{
	SPDR = g_data;
	//slave_SPI_send(g_data);
	g_data = SPDR;
	UART_Transmit(g_data);
	first_rgb_led_color(RED);
}