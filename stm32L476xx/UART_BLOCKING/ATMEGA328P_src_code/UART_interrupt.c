#include <avr/io.h>
#include "UART_interrupt.h"
#include "I_O_board.h"


// #define FOSC 16000000 // Clock Speed
// #define BAUD 9600
// #define MYUBRR FOSC/16/BAUD-1
// USART_config(MYUBRR);

void USART_config(unsigned char ubrr){
	/*Set baud rate */
	UBRR0H = /*(unsigned char)*/(ubrr>>8);
	UBRR0L = /*(unsigned char)*/ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)/*|(1<<TXCIE0)*/;
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(1<<UCSZ00)|(1<<UCSZ01);
}
	
// unsigned int USART_Receive(void){
// 		/* Wait for data to be received */
// 		while (!(UCSR0A & (1<<RXC0)));
// 		/* Get and return received data from buffer */
// 		return UDR0;
// }

void USART_Transmit(uint8_t data, _Bool bool){
		//check if the buffer is empty and teh bool is true
	if ((UCSR0A & (1<<UDRE0))&&bool){
	UDR0 = data;
	first_rgb_led_color(BLACK);
	}
	else{return;}
}
