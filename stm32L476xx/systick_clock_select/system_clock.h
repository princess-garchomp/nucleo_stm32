#ifndef SYSTEM_CLOCK_H
#define SySTEM_CLOCK_H

#include <stdint.h>

typedef enum{_100kHZ = 0b0000, _200kHZ = 0b0001, _400kHZ = 0b0010, _800kHZ = 0b0011, _1MHZ = 0b0100, _2MHZ = 0b0101, 
    _4MHZ = 0b0110, _8MHZ = 0b0111, _16MHZ = 0b1000, _24MHZ = 0b1001, _32MHZ = 0b1010, _48MHZ = 0b1011}
MSI_clock_freq_set;
//not sure if my vboard MSI can get up to 48MHZ, but the data sheet says it is an option so I am including it for completnes


void enable_HSI16();
void enable_MSI(MSI_clock_freq_set MSI_input_freq);
//void enable_HSE();

void config_systic (uint32_t ticks);
void config_systic_for_ms (uint32_t sysclock_freq_input);

void Delay(uint32_t nTime);


#endif