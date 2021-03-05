#ifndef CLOCK_PERIPHERAL_H
#define CLOCK_PERIPHERAL_H
#include <stdint.h>

void TIM1_CH1_cmp_output_config();
void TIM1_CH2_cmp_output_config();

void TIM2_interrupt_config();
uint32_t get_TIM2_count();

void TIM7_delay_config();
void TIM7_ms_delay(uint16_t input);

//tim 3 and tim 4 have IRQ associated with them
void TIM4_PWM_config();

#endif