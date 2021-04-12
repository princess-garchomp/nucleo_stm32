#ifndef TIEMR_GLOBAL_COUNT_H
#define TIEMR_GLOBAL_COUNT_H


#include <avr/io.h>
#include <avr/interrupt.h>

void _16_bit_tiemr1_config();


//.1ms
unsigned long _mS_counter();

void delay(unsigned long input);

#endif