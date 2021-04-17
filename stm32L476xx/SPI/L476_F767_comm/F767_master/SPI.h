#ifndef SPI_H
#define SPI_H

#include <stm32f767xx.h>

void general_config_SPI_master(SPI_TypeDef * SPIx);
void config_SPI1();
void SPI1_select_slave();
void SPI1_deselect_slave();

uint8_t SPI_read(SPI_TypeDef * SPIx);
void SPI_write(SPI_TypeDef * SPIx, uint8_t input);

uint8_t SPI_interrupt_read(SPI_TypeDef * SPIx);


#endif