/*
 * led_matrix.h
 *
 *  Created on: Sep 6, 2018
 *      Author: dmartins
 */

#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_

#include <stdint.h>

struct RGB {
	uint8_t R;
	uint8_t G;
	uint8_t B;
};

typedef struct RGB RGB_t;

#define BITS_PER_PIXEL 24
#define BITS_PER_CHANNEL 8

#define WIDTH 64
#define HEIGHT 64
#define SCAN_RATE 32 // this is a 1/32 display

//#define PIXEL(x, y) x % 2 == 0 ? frame[y * 32 + x / 2] >> 4 : frame[y * 32 + x / 2] & 0x0F
#define PIXEL(x, y) frame[y * 64 + x]

void LED_fillBuffer(void);
void LED_displayFrame(void);

#endif /* LED_MATRIX_H_ */
