

#ifndef SPI_H

void config_SPI_master_one_slave();
void SPI_master_send(uint8_t data);
//I also want to be able to recieve over MISO(as an interupt?)

void config_USART_as_SPI_one_slave();
void USART_SPI_master_send(uint8_t data);
//I also want to be able to reciece over MISO (as an interupt?)

void config_SPI_slave();//this will recieve over MOSI via an interrupt
void slave_SPI_send(uint8_t data);

void select_slave();
void deselect_slave();

#define SPI_H
#endif
