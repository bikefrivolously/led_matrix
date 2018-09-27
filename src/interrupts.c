#include "stm32f446xx.h"
#include "main.h"
#include "led.h"

void TIM5_IRQHandler(void) {
    /* Clear the interrupt flag right away.
     * Due to pipelining, the register itself might not get updated for several
     * cycles. If we wait until the end of the ISR to clear the flag,
     * it can trigger again immediately */
    TIM5->SR &= ~TIM_SR_UIF_Msk;

    if(bit == 0) { // the first 64 bits of a new row have been latched. Set the row select to match.
        GPIOB->ODR = (GPIOB->ODR & ~(row_mask)) | row;
        row++;
        row &= row_mask;
    }
    bit++;
    bit &=  0x7;
    TIM5->CCR2 = 1280 - (BRIGHTNESS * (1 << bit)); // set the duty cycle of the NEXT ~OE pulse
}

void DMA2_Stream2_IRQHandler(void) {
    DMA2->LIFCR |= DMA_LIFCR_CTCIF2; // make sure the interrupt flag is clear
    frame_count++;
    busyFlag = 0; // main loop watches this flag to know when to fill up the next buffer
}

void USART2_IRQHandler(void) {
//    volatile uint8_t val;
 //   val = USART2->DR;
    USART2->SR = 0;
    uart_buffer[uart_counter] = USART2->DR;
    uart_counter++;
    if(uart_counter >= WIDTH*HEIGHT*3) {
        uart_counter = 0;
        uart_frame_rx = 1;
    }
    /*switch(uart_colour) {
        case 0:
            frame[uart_counter].R = val;
            uart_colour++;
            break;
        case 1:
            frame[uart_counter].G = val;
            uart_colour++;
            break;
        case 2:
            frame[uart_counter].B = val;
            uart_colour++;
            break;
        default:
            uart_colour = 0;
            uart_counter++;
    }
    if(uart_counter >= WIDTH*HEIGHT) {
        uart_counter = 0;
        uart_colour = 0;
        uart_frame_rx = 1;
    }*/
}
