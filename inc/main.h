#pragma once

#include <stdint.h>

#define row_mask 0x1F

#define PRESCALE 0

#define BITS_PER_PIXEL 24
#define BITS_PER_CHANNEL 8

#define WIDTH 64
#define HEIGHT 64
#define SCAN_RATE 32 // this is a 1/32 display

#define BRIGHTNESS 10 // this can be from 1 to 10

#define PIXEL(f, x, y) f[y * 64 + x]


void _error_handler(void);

extern volatile uint8_t bit;
extern volatile uint8_t row;
extern volatile uint8_t busyFlag;
extern volatile uint32_t frame_count;
extern volatile uint32_t uart_counter;
extern volatile uint8_t uart_colour;
extern volatile uint8_t uart_frame_rx;
extern volatile uint8_t uart_buffer[];
