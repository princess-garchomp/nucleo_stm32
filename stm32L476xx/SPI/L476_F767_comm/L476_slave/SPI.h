#ifndef SPI_H
#define SPI_H

#include <stm32l476xx.h>

void general_config_SPI_master(SPI_TypeDef * SPIx);
void general_config_SPI_slave(SPI_TypeDef * SPIx);


void config_SPI1_master();
void SPI1_select_slave();
void SPI1_deselect_slave();

void config_SPI1_slave();

uint8_t SPI_read(SPI_TypeDef * SPIx);
void SPI_write(SPI_TypeDef * SPIx, uint8_t input);

uint8_t SPI_interrupt_read(SPI_TypeDef * SPIx);

void SPI_slave_update_DR(SPI_TypeDef * SPIx, uint8_t input);


#endif