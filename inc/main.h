#pragma once

#include <stdint.h>

#define row_mask 0x1F

#define PRESCALE 0

#define BITS_PER_PIXEL 24
#define BITS_PER_CHANNEL 8

#define WIDTH 64
#define HEIGHT 64
#define SCAN_RATE 32 // this is a 1/32 display

#define BRIGHTNESS 1 // this can be from 1 to 10

#define PIXEL(f, x, y) f[y * 64 + x]

extern volatile uint32_t ticks;

inline uint32_t millis(void);
void delay_ms(uint32_t t);
void _error_handler(void);
void TIM5_IRQHandler(void);
void SysTick_Handler(void);

struct RGB {
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

typedef struct RGB RGB_t;

