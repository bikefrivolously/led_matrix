#include "systick.h"

#include <stdint.h>

#include "stm32f446xx.h"

volatile uint32_t ticks;

// Call this after setting up the clock
void SysTick_Init(void) {
    // Enable the SysTick interrupt every 1ms
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_EnableIRQ(SysTick_IRQn);
}

void delay_ms(uint32_t t) {
  uint32_t elapsed;
  uint32_t start = millis();
  do {
    elapsed = millis() - start;
  } while (elapsed < t) ;
}

void SysTick_Handler(void) {
    ticks++;
}
