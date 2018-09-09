/*
 * led_matrix.h
 *
 *  Created on: Sep 6, 2018
 *      Author: dmartins
 */

#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_

#include <stdint.h>

extern uint8_t frame[];

#define BITS_PER_PIXEL 5

//#define PIXEL(x, y) x % 2 == 0 ? frame[y * 32 + x / 2] >> 4 : frame[y * 32 + x / 2] & 0x0F
#define PIXEL(x, y) frame[y * 64 + x]

void LED_displayFrame(void);

#endif /* LED_MATRIX_H_ */
