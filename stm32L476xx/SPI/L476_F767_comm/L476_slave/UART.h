#ifndef UART_H
#define UART_H

#include <stm32l476xx.h>
#include <stdint.h>

void generic_USART_config(USART_TypeDef * USARTx, uint32_t BAUD_SET);

void config_UART1_blocking_9600baud();
void UART_BYTE_send_blocking(USART_TypeDef *USARTx, uint8_t input_byte);
uint8_t UART_BYTE_recieve_blocking(USART_TypeDef *USARTx);

void config_UART1_interupt_9600baud();
void UART_send_byte_interrupt_start(USART_TypeDef *USARTx, uint8_t input_byte);
void UART_interrupt_byte_send(USART_TypeDef *USARTx);
uint8_t UART_interrupt_byte_recieve(USART_TypeDef *USARTx);



#endif