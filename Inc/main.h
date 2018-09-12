/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
#include <stdint.h>

extern volatile uint8_t dma_in_progress;
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LM_B1_Pin GPIO_PIN_0
#define LM_B1_GPIO_Port GPIOC
#define LM_G1_Pin GPIO_PIN_1
#define LM_G1_GPIO_Port GPIOC
#define LM_R1_Pin GPIO_PIN_2
#define LM_R1_GPIO_Port GPIOC
#define LM_B2_Pin GPIO_PIN_3
#define LM_B2_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LM_G2_Pin GPIO_PIN_4
#define LM_G2_GPIO_Port GPIOC
#define LM_R2_Pin GPIO_PIN_5
#define LM_R2_GPIO_Port GPIOC
#define LM_E_Pin GPIO_PIN_4 //MSB
#define LM_E_GPIO_Port GPIOB
#define LM_D_Pin GPIO_PIN_3
#define LM_D_GPIO_Port GPIOB
#define LM_C_Pin GPIO_PIN_2
#define LM_C_GPIO_Port GPIOB
#define LM_CLK_Pin GPIO_PIN_6
#define LM_CLK_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define LM_B_Pin GPIO_PIN_1
#define LM_B_GPIO_Port GPIOB
#define LM_A_Pin GPIO_PIN_0 //LSB
#define LM_A_GPIO_Port GPIOB
#define LM_LAT_Pin GPIO_PIN_5
#define LM_LAT_GPIO_Port GPIOB
#define LM_OE_Pin GPIO_PIN_6
#define LM_OE_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */
#define TIM8_Pin GPIO_PIN_6
#define TIM8_Port GPIOC

/* With APB2 CLK @ 90MHz and TIMPRE=0, TIM8 CLK is 180MHz
 * Prescaler of 17999, Period of 9 gives 1kHZ PWM
 * Pulse defines duty cycle (5 is 50%)
 */
//#define TIM8_Prescaler 17999
//#define TIM8_Period 9
//#define TIM8_Pulse TIM8_Period - (TIM8_Period - 5)

#define TIM8_Prescaler 9
#define TIM8_Period 9
#define TIM8_Pulse TIM8_Period - (TIM8_Period - 5)




/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
