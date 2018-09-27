#include "main.h"

#include <stdint.h>

#include "stm32f446xx.h"

#include "systick.h"
#include "led.h"

volatile uint8_t bit;
volatile uint8_t row;
volatile uint32_t frame_count;

volatile uint32_t uart_counter;
volatile uint8_t uart_colour;
volatile uint8_t uart_frame_rx;
volatile uint8_t uart_buffer[WIDTH*HEIGHT*3];

volatile uint8_t busyFlag;

uint8_t *nextBuffer;

static void init(void);
static void initClock(void);
static void initGPIO(void);
static void initTimers(void);
static void initDMA(void);
static void initUART(void);

int main(void) {
    init();
    uint32_t current_buffer, start_time;
    while(1) {
//        start_time = millis();
        while(!uart_frame_rx); // wait for a frame to be recieved
        uart_frame_rx = 0;
        for(uint32_t i = 0, j = 0; i < WIDTH*HEIGHT*3; i+= 3, j++) {
           frame[j].R = uart_buffer[i];
           frame[j].G = uart_buffer[i+1];
           frame[j].B = uart_buffer[i+2];
        }
        // refresh sync
        while(busyFlag);
        busyFlag = 1;
        current_buffer = DMA2_Stream2->CR | DMA_SxCR_CT;
        nextBuffer = current_buffer ? buffer2 : buffer1;
        while(busyFlag);
        busyFlag = 1;
        //put the frame in the buffer
        LED_fillBuffer(frame, nextBuffer);
//        LED_waveEffect(frame);
//        while(millis() - start_time < 30);

    }
    return 0;
}

static void init(void) {
    // enable the data and instruction cache and prefetch
    FLASH->ACR |= FLASH_ACR_DCEN | FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;
    
    // This will set the clock to 180MHz
    initClock();
    SysTick_Init();

    // Set up any input/output pins
    initGPIO();
    initDMA();
    initUART();
    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM5_STOP;
    DBGMCU->APB2FZ |= DBGMCU_APB2_FZ_DBG_TIM8_STOP; 
    initTimers();
    busyFlag = 1;
    uart_counter = 0;
    uart_colour = 0;
    uart_frame_rx = 0;
}

static void initGPIO(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    
    GPIOA->MODER |= (0x1U << GPIO_MODER_MODE5_Pos);
    GPIOA->OTYPER |= (0x0U << GPIO_OTYPER_OT5_Pos);
    GPIOA->OSPEEDR |= (0x0U << GPIO_OSPEEDR_OSPEED5_Pos);
    GPIOA->PUPDR |= (0x0U << GPIO_PUPDR_PUPD5_Pos);
    
    // PB0-4 are A,B,C,D,E pins
    GPIOB->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3 | GPIO_MODER_MODE4); 
    GPIOB->MODER |= GPIO_MODER_MODE0_0 |
                    GPIO_MODER_MODE1_0 |
                    GPIO_MODER_MODE2_0 |
                    GPIO_MODER_MODE3_0 |
                    GPIO_MODER_MODE4_0;

    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED0_1 |
                      GPIO_OSPEEDR_OSPEED1_0 | GPIO_OSPEEDR_OSPEED1_1 |
                      GPIO_OSPEEDR_OSPEED2_0 | GPIO_OSPEEDR_OSPEED2_1 |
                      GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED3_1 |
                      GPIO_OSPEEDR_OSPEED4_0 | GPIO_OSPEEDR_OSPEED4_1; 
 
    // PC0-PC5 are RGB pins
    GPIOC->MODER |= GPIO_MODER_MODE0_0 |
                    GPIO_MODER_MODE1_0 |
                    GPIO_MODER_MODE2_0 |
                    GPIO_MODER_MODE3_0 |
                    GPIO_MODER_MODE4_0 |
                    GPIO_MODER_MODE5_0;

    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED0_1 |
                      GPIO_OSPEEDR_OSPEED1_0 | GPIO_OSPEEDR_OSPEED1_1 |
                      GPIO_OSPEEDR_OSPEED2_0 | GPIO_OSPEEDR_OSPEED2_1 |
                      GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED3_1 |
                      GPIO_OSPEEDR_OSPEED4_0 | GPIO_OSPEEDR_OSPEED4_1 |
                      GPIO_OSPEEDR_OSPEED5_0 | GPIO_OSPEEDR_OSPEED5_1; 
}

static void initDMA(void) {
    /*
     * Channel 7
     * Double Buffer
     * Very high priority
     * Memory Inc
     * 1byte-1byte transfers
     * Memory to Peripheral
     */

    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

    DMA2_Stream2->CR |= (7<<DMA_SxCR_CHSEL_Pos) | DMA_SxCR_DBM | DMA_SxCR_PL_0 | DMA_SxCR_PL_1 | DMA_SxCR_MINC | DMA_SxCR_DIR_0;
    DMA2_Stream2->NDTR = 0x4000; // 64 col, 8 Bits per colour channel, 32 rows
    DMA2_Stream2->PAR = (uint32_t)&(GPIOC->ODR);
    DMA2_Stream2->M0AR = (uint32_t)buffer1;
    DMA2_Stream2->M1AR = (uint32_t)buffer2;
    DMA2_Stream2->FCR |= DMA_SxFCR_DMDIS; // turn on the FIFO
    DMA2_Stream2->CR |= DMA_SxCR_TCIE; // enable the transfer complete interrupt
    DMA2->LIFCR |= DMA_LIFCR_CTCIF2; // make sure the interrupt flag is clear

    NVIC_EnableIRQ(DMA2_Stream2_IRQn);

    DMA2_Stream2->CR |= DMA_SxCR_EN; // enable the stream
}

static void initClock(void) {
    // 1. Enable clock to PWR in APB1
    // 2. Set up the HSE and PLL
    // 3. Enable overdrive and wait for it to be enabled
    // 4. Adjust the flash latency
    // 5. Set AHB/APB1/APB2 prescalers
    // 6. Wait for PLL lock
    // 7. Select PLL as sysclock
    // 8. Call SystemCoreClockUpdate();

    RCC->APB1ENR |= RCC_APB1ENR_PWREN;

    // Enable the HSE in bypass mode (there is a 8MHz signal coming from the ST-LINK)
    RCC->CR |= RCC_CR_HSEBYP | RCC_CR_HSEON;
    while(!(RCC->CR & RCC_CR_HSERDY));

    // Set the main PLL M, N, P, Q, R, and HSE as the input
    // M = 4, N = 180, P = 2 = 180MHz SYSCLK
    // Q = 8, R = 4 (Q & R not used)
    RCC->PLLCFGR = RCC_PLLCFGR_PLLM_2 | (180 << RCC_PLLCFGR_PLLN_Pos) | (0 << RCC_PLLCFGR_PLLP_Pos) | RCC_PLLCFGR_PLLQ_3 | RCC_PLLCFGR_PLLR_2 | RCC_PLLCFGR_PLLSRC_HSE;
    RCC->CR |= RCC_CR_PLLON;

    // enable and switch on overdrive
    PWR->CR |= PWR_CR_ODEN;
    while(!(PWR->CSR & PWR_CSR_ODRDY));

    PWR->CR |= PWR_CR_ODSWEN;
    while(!(PWR->CSR & PWR_CSR_ODSWRDY));

    // Set the flash wait time
    FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
    if((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_5WS) {
        _error_handler();
    }

    // set APB prescalers
    // APB1 = 4, APB2 = 2
    // 45MHz and 90Mhz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;
    
    // wait for PLL lock
    while(!(RCC->CR & RCC_CR_PLLRDY));

    // Switch SYSCLK to PLL
    RCC->CFGR |= RCC_CFGR_SW_PLL;


    SystemCoreClockUpdate();
    
    // set TIMPRE so the timers run at 180MHz
    RCC->DCKCFGR |= RCC_DCKCFGR_TIMPRE;
}

static void initTimers(void) {
    // Timer 8, running 9Mhz 50% duty cycle
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    
    GPIOC->MODER |= GPIO_MODER_MODE6_1; // PC6 in AF mode
    GPIOC->OSPEEDR |= GPIO_OSPEEDR_OSPEED6_0 | GPIO_OSPEEDR_OSPEED6_1; // high speed
    GPIOC->AFR[0] |= (0x3U << GPIO_AFRL_AFSEL6_Pos); // AF3 for PC6 is TIM8_CH1

    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
    
    TIM8->ARR = 19;
    TIM8->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; //PWM mode 1
    TIM8->DIER |= TIM_DIER_CC1DE; // DMA request on CH1 (falling edge of clock)
    TIM8->CCR1 = 10;
    TIM8->CCER |= TIM_CCER_CC1E; // OC1 enabled

    TIM8->BDTR |= TIM_BDTR_MOE; // Main output enabled.. if this isn't set, there is no output on the pins!

    TIM8->PSC = PRESCALE; // scale down for testing
    TIM8->EGR |= TIM_EGR_UG; // trigger a UEV to update the preload, auto-reload, and capture-compare shadow registers
    TIM8->SR = 0;
    TIM8->CR2 |= TIM_CR2_MMS_1; // Master mode - update

    // Timer 5
    // LAT on CH1, output to PA0
    // OE on CH2, output to PA1 
    GPIOA->MODER |= GPIO_MODER_MODE0_1; // PA0 in AF mode
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED0_1; // high speed
    GPIOA->AFR[0] |= (0x2U << GPIO_AFRL_AFSEL0_Pos); // AF2 for PA0 is TIM5_CH1

    GPIOA->MODER |= GPIO_MODER_MODE1_1; // PA1 in AF mode
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED1_0 | GPIO_OSPEEDR_OSPEED1_1; // high speed
    GPIOA->AFR[0] |= (0x2U << GPIO_AFRL_AFSEL1_Pos); // AF2 for PA1 is TIM5_CH2

    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    
    TIM5->SMCR |= TIM_SMCR_TS_1 | TIM_SMCR_TS_0; // select ITR3 as the trigger input (for TIM5, this is TIM8) 
    TIM5->SMCR |= TIM_SMCR_SMS_2 | TIM_SMCR_SMS_1; // slave mode set to trigger mode
    TIM5->ARR = 1279;
    TIM5->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; //PWM mode 1
    TIM5->CCR1 = 12;
    TIM5->CCER |= TIM_CCER_CC1E; // OC1 enabled

    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; //PWM mode 1
    TIM5->CCMR1 |= TIM_CCMR1_OC2PE; // preload enabled for CCR2
    TIM5->CCR2 = 1280; // start with OC2 (OE) held at 1 (off)
    TIM5->CCER |= TIM_CCER_CC2E; // OC2 enabled

    TIM5->PSC = PRESCALE; // scale down for testing
    TIM5->EGR |= TIM_EGR_UG; // trigger a UEV to update the preload, auto-reload, and capture-compare shadow registers
    TIM5->SR = 0; // clear the status flags
    TIM5->DIER |= TIM_DIER_UIE; // enable TIM5 interrupt on UEV
    TIM5->CNT = 15;

    NVIC_EnableIRQ(TIM5_IRQn);

    // Start the timer
    TIM8->CR1 |= TIM_CR1_CEN;
    TIM5->CCR2 = 1280 - BRIGHTNESS; // put the first value in CCR2 preload it will be loaded at the first UEV
}


static void initUART(void) {
    // Use USART2 as it is connected to the ST-LINK virtual COM on the Nucleo board
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    //Pins PA2 (TX) and PA3 (RX) - AF7
    GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED2_0 | GPIO_OSPEEDR_OSPEED2_1;
    GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED3_1;
    GPIOA->AFR[0] |= (0x7U << GPIO_AFRL_AFSEL2_Pos) | (0x7U << GPIO_AFRL_AFSEL3_Pos);

    USART2->CR1 |= USART_CR1_UE;
    // 921600 baud when OVER8=0 and APB1 Clk = 45MHz
    USART2->BRR = (3 << USART_BRR_DIV_Mantissa_Pos) | (1 << USART_BRR_DIV_Fraction_Pos);
    
    // Enable the reciever and reciever interrupt
    USART2->CR1 |= USART_CR1_RE | USART_CR1_RXNEIE;
    NVIC_EnableIRQ(USART2_IRQn);
}

void _error_handler(void) {
    while(1);
}

