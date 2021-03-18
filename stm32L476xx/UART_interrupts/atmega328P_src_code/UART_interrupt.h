#ifndef UART_INTERRUPT_H
#define UART_INTERRUPT_H



void USART_config(unsigned char ubrr);
//unsigned char USART_Receive(void);
void USART_Transmit(uint8_t data, _Bool bool);

#endif