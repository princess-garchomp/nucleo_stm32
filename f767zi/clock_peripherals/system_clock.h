#ifndef SYSTEM_CLOCK_H
#define SySTEM_CLOCK_H

#include <stdint.h>

void enable_HSI16();

void config_systic (uint32_t ticks);
void config_systic_for_ms (uint32_t sysclock_freq_input);

void Delay(uint32_t nTime);

#endif