#pragma once

#include <stdint.h>

extern volatile uint32_t ticks;

void SysTick_Init(void);
void delay_ms(uint32_t t);

inline uint32_t millis(void) {
    return ticks;
}
