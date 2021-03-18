#include <stm32l476xx.h>

#include "UART.h"
#include "system_clock.h"

#define CONCAT(A,B) A ## B



#define GPIO_PORT(PORT_LETTER) CONCAT(GPIO,PORT_LETTER) 

//PB6
#define USART1_TX 6
#define USART1_TX_PORT B
#define START_AFR_USART1_TX_PB6 24

//PB7
#define USART1_RX 7
#define USART1_RX_PORT B
#define START_AFR_USART1_RX_PB7 28

//some of these confugurations use two bits to control the function
#define TWO_CONFIG_BITS(X) X*2

//BAUD=clcok/9600
//16mhz/9600=1667
void generic_USART_config(USART_TypeDef * USARTx, uint32_t BAUD_SET)
{
    //disable USART
    USARTx->CR1 &= ~USART_CR1_UE;

    //set data length to 8 bits
    //00 = 8bits, 01 = 9bits, 10 = 7 bits
    USARTx->CR1 &= ~USART_CR1_M;

    //select 1 stop bit
    //00 = 1 stop bit, 01 .5 stop bits, 10 2 stop bits, 11 = 1.5 stop bits
    USARTx->CR2 &= ~USART_CR2_STOP;

    //set parity as no parity
    //0 = no parity, 1= parity
    USARTx->CR1 &= ~USART_CR1_PCE;

    //oversampling by 16
    //0= oversampling by 16, 1= oversampling by 8
    USARTx->CR1 &= ~USART_CR1_OVER8;

    //set baud rate
    USARTx->BRR = BAUD_SET;

    //enable send and recieve
    USARTx->CR1 |= (USART_CR1_TE|USART_CR1_RE);

    //enable usart
    USARTx->CR1 |= USART_CR1_UE;
}
void config_UART1_blocking_9600baud()
{
    enable_HSI16();

    //config PB6 USRT1_TX (AF7), config PB7 USART_RX (AF7) 
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;//enable clock to GPIO portB
    GPIO_PORT(USART1_TX_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(USART1_TX))); //set alt functinos for PB7 and PB6
    GPIO_PORT(USART1_TX_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(USART1_TX)));
    GPIO_PORT(USART1_RX_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_RX_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_TX_PORT)->AFR[0] &= ~(0xF<<START_AFR_USART1_TX_PB6);
    GPIO_PORT(USART1_TX_PORT)->AFR[0] |= (7ul<<START_AFR_USART1_TX_PB6);
    GPIO_PORT(USART1_RX_PORT)->AFR[0] &= ~(0xF<<START_AFR_USART1_RX_PB7);
    GPIO_PORT(USART1_RX_PORT)->AFR[0] |= (7ul<<START_AFR_USART1_RX_PB7);//finish setting alt functinos for PB7 and PB6
    GPIO_PORT(USART1_TX_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(USART1_TX)));//config as high speed
    GPIO_PORT(USART1_RX_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_TX_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(USART1_TX)));//select pull-up
    GPIO_PORT(USART1_TX_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(USART1_TX)));
    GPIO_PORT(USART1_RX_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_RX_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_TX_PORT)->OTYPER &= ~(1ul<<USART1_TX);//push pull
    GPIO_PORT(USART1_RX_PORT)->OTYPER &= ~(1ul<<USART1_RX);
    
    //config clocks for USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->CCIPR &= ~(RCC_CCIPR_USART1SEL);
    RCC->CCIPR |= (RCC_CCIPR_USART1SEL_0);
    
    //config uart
    generic_USART_config(USART1, 1667);
}


void UART_BYTE_send_blocking(USART_TypeDef *USARTx, uint8_t input_byte)
{
    while(!(USARTx->ISR & USART_ISR_TXE));//wait for hardware to set TXE,transmission is enabled
    USARTx->TDR = input_byte;
    while(!(USARTx->ISR & USART_ISR_TC));//wait for transmission complete bit to be set, transimission has completed
    USARTx->ISR |= USART_ICR_TCCF;//set the transmission complete flah
}
uint8_t UART_BYTE_recieve_blocking(USART_TypeDef *USARTx)
{
    static uint8_t last_updade = 0;

    if(USARTx->ISR & USART_ISR_RXNE)//if hardware set RXNE return the value
    {
    last_updade = USARTx->RDR;
    return USARTx->RDR;
    }
    else 
    {
        return last_updade;
    }
}

void config_UART1_interupt_9600baud()
{
    enable_HSI16();

    //config PB6 USRT1_TX (AF7), config PB7 USART_RX (AF7) 
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;//enable clock to GPIO portB
    GPIO_PORT(USART1_TX_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(USART1_TX))); //set alt functinos for PB7 and PB6
    GPIO_PORT(USART1_TX_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(USART1_TX)));
    GPIO_PORT(USART1_RX_PORT)->MODER &= ~(3ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_RX_PORT)->MODER |= (2ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_TX_PORT)->AFR[0] &= ~(0xF<<START_AFR_USART1_TX_PB6);
    GPIO_PORT(USART1_TX_PORT)->AFR[0] |= (7ul<<START_AFR_USART1_TX_PB6);
    GPIO_PORT(USART1_RX_PORT)->AFR[0] &= ~(0xF<<START_AFR_USART1_RX_PB7);
    GPIO_PORT(USART1_RX_PORT)->AFR[0] |= (7ul<<START_AFR_USART1_RX_PB7);//finish setting alt functinos for PB7 and PB6
    GPIO_PORT(USART1_TX_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(USART1_TX)));//config as high speed
    GPIO_PORT(USART1_RX_PORT)->OSPEEDR |= (3ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_TX_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(USART1_TX)));//select pull-up
    GPIO_PORT(USART1_TX_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(USART1_TX)));
    GPIO_PORT(USART1_RX_PORT)->PUPDR &= ~(3ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_RX_PORT)->PUPDR |= (1ul<<(TWO_CONFIG_BITS(USART1_RX)));
    GPIO_PORT(USART1_TX_PORT)->OTYPER &= ~(1ul<<USART1_TX);//push pull
    GPIO_PORT(USART1_RX_PORT)->OTYPER &= ~(1ul<<USART1_RX);
    
    //config clocks for USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->CCIPR &= ~(RCC_CCIPR_USART1SEL);
    RCC->CCIPR |= (RCC_CCIPR_USART1SEL_0);

    //interrupt config
    USART1->CR1 |= USART_CR1_RXNEIE;//enable interrupt on recieve register not emty
    USART1->CR1 &= ~USART_CR1_TXEIE;//disable interrupt on send register empty
    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);
    
    //config uart
    generic_USART_config(USART1, 1667);

}
void UART_send_byte_interrupt_start(USART_TypeDef *USARTx, uint8_t input_byte)
{
    USARTx->CR1 |= USART_CR1_TXEIE; //enable transmit recieved empty interrupt
    USARTx->TDR = input_byte;
}
void UART_interrupt_byte_send(USART_TypeDef *USARTx)
{
     if(USARTx->ISR & USART_ISR_TXE)
     {
        USARTx->CR1 &= ~USART_CR1_TXEIE;//disable interrupt on send register empty  
     }
}
uint8_t UART_interrupt_byte_recieve(USART_TypeDef *USARTx)
{
    if (USARTx->ISR & USART_ISR_RXNE)
    {
        return USARTx->RDR;
    }
    return 0;

}
