# led_matrix
Driving a 64x64 LED matrix using STM32F4.

This has been developed on a ST Nucleo board. Specifically NUCLEO-F446RE.
After initially starting off using TrueStudio and the STM32 HAL drivers, the project has been converted to use a Makefile for building and CMSIS headers. The toolchain is gcc-arm. I found I was spending more time than I'd like running into issues with eclipse or trying to dechipher the HAL libraries.

The current code makes use of two timers, DMA, and GPIO to drive the display leaving the CPU relatively free to compute the next frame. With the microcontroller running at it's maximum speed of 180MHz, one frame can be shown on the display in ~1.82ms, giving a refresh rate of a bit over 549Hz. This is easily scaled back to something more reasonable by setting a timer prescaler value. (PRESCALER = 8 will divide the timer clock by 9, giving 61Hz screen updates). Doing so gives more time for the CPU to prepare the next frame and load it in the buffer.

The LED matrix is a 64x64 grid of RGB LEDs with a 2.5mm pitch. It has a "HUB75" connector on the back for clocking in data and is a 1/32 scan rate. This means there are five row select pins (A,B,C,D,E) giving 32 selections. Each selection enables row y and row y+32. There are six data pins (R1,G1,B1,R2,G2,B2) for providing information about which LEDs should be turned on in the selected rows. A CLK pin is used to clock the data in to the LED driver chips shift register. A LAT pin latches the shift register in to the output driver on a rising edge. A OE pin (active low) determines if the selected rows are lit.

To light up each pair of rows, 64 values need to be clocked in from the data pins. This is then latched and OE is pulled low for some period of time. This is repeated 8 times with the on time varying each time in order to regulate the LED brightness and provide 24bit colour before moving on to the next rows.

TIM8 CH1 is configured in PWM mode and provides a continuous CLK signal to the display via pin PC6. On each output compare match (falling edge of the clock), a DMA transfer request is triggered via DMA2_Stream2_Channel7.
DMA2_Stream2 is configured in double buffer, circular, memory-to-peripheral mode and writes one byte from memory to GPIO PC0:7 (only PC0:5 are used) which are connected to the RGB pins of the LED display.
On the following rising edge of TIM8 CH1 (CLK), the new RGB settings are clocked in to the display's shift register. This repeats continuously giving a constant clock rate and updates to the RGB pins.

TIM5 is synchronized with TIM8 and also provides a PWM output. The period of it's PWM signal is 64 times lower than TIM8 and is slightly out of phase. Two channels are driven by this timer:
CH1 - LAT (PA0)
CH2 - OE (PA1)
CH1 has a short duty cycle and outputs a pulse after every new row of data has been clocked out by TIM8/DMA2
CH2 has a variable duty cycle and drives the OE pin to control LED modulation

On every update event of TIM5 (so every time a new row is latched), an interrupt is triggered. This interrupt does two things:
1. Adjust the duty cycle of CH2 (OE)
2. Every 8 interrupts, select the next row of LEDs by writing to GPIOB pins PB0:4.

These two timers and the DMA stream completely handle continuous updates to the display.
