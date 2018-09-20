#include "led.h"

#include <stdint.h>
#include <math.h>

#include "main.h"
#include "gamma.h"

RGB_t frame[WIDTH*HEIGHT];
uint8_t buffer1[WIDTH*BITS_PER_CHANNEL*SCAN_RATE];
uint8_t buffer2[WIDTH*BITS_PER_CHANNEL*SCAN_RATE];

float mapf(float value, float c_min, float c_max, float t_min, float t_max) {
    return (value - c_min) / (c_max - c_min) * (t_max - t_min) + t_min;
}


void LED_waveEffect(RGB_t *frame) {
    static float time;
    float xx, yy, v;
    uint8_t r, g, b;

    if(time > 2*M_PI) {
        time = 0.0;
    }

    for(uint8_t y = 0; y < HEIGHT; y++) {
        yy = mapf(y, 0, HEIGHT-1, 0, 2*M_PI);
        for(uint8_t x = 0; x < WIDTH; x++) {
            xx = mapf(x, 0, WIDTH-1, 0, 2*M_PI);
            //v = sinf(xx + time); // between -1 and 1
            //r = 64 + 180 * sinf(v * M_PI); // sin result is between 0 and 1;
            //g = 64 + 180 * cosf(v * M_PI);
            //b = 64;
            r = 16 + 100 * (sinf(xx + time) + 1);
            g = 16 + 100 * (sinf(xx + M_PI / 3 + time) + 1);
            b = 16 + 100 * (sinf(xx + 2 * M_PI / 3 + time) + 1);
            PIXEL(frame, x, y).R = r;
            PIXEL(frame, x, y).G = g;
            PIXEL(frame, x, y).B = b;
        }
    }
    time += 0.05;
}

void LED_plasmaEffect(RGB_t *frame) {
    static float time;
    uint8_t r, g, b;
    float xx, yy;
    float v;
    float delta = 0.025;

    time += 0.025;
    if(time > 12*M_PI) {
        delta *= -1;
    }

    for(uint8_t y = 0; y < HEIGHT; y++) {
        yy = mapf(y, 0, HEIGHT-1, 0, 2*M_PI);
        for(uint8_t x = 0; x < WIDTH; x++) {
            xx = mapf(x, 0, WIDTH-1, 0, 2*M_PI);

            v = sinf(xx + time);
            v += sinf((yy + time) / 2.0);
            v += sinf((xx + yy + time) / 2.0);
            float cx = xx + .5 * sinf(time/5.0);
            float cy = yy + .5 * cosf(time/3.0);
            v += sinf(sqrtf((cx*cx+cy*cy)+1)+time);
            v /= 2.0;
            r = 255 * fabsf(sinf(v * M_PI));
            g = 255 * fabsf(cosf(v * M_PI));
            b = 0;
            PIXEL(frame, x, y).R = r;
            PIXEL(frame, x, y).G = g;
            PIXEL(frame, x, y).B = b;
        }
    }
}

void LED_fillBuffer(RGB_t *frame, uint8_t *buffer) {
    uint32_t i = 0, p1, p2;
    uint8_t bit, mask;
    for(uint8_t row = 0; row < SCAN_RATE; row++) {
        p1 = row * WIDTH;
        p2 = p1 + WIDTH * SCAN_RATE;
        for(bit = 0; bit < BITS_PER_CHANNEL; bit++) {
            mask = 1<<bit;
            for(uint8_t col = 0; col < WIDTH; col++) {
                buffer[i] =
                    ((((gammaR[frame[p2+col].R]) & mask) >> bit) << 5) |
                    ((((gammaG[frame[p2+col].G]) & mask) >> bit) << 4) |
                    ((((gammaB[frame[p2+col].B]) & mask) >> bit) << 3) |
                    ((((gammaR[frame[p1+col].R]) & mask) >> bit) << 2) |
                    ((((gammaG[frame[p1+col].G]) & mask) >> bit) << 1) |
                    ((((gammaB[frame[p1+col].B]) & mask) >> bit) << 0);
                i++;
            }
        }
    }
}

