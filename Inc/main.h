/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
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
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
/**
*	@file main.h
*	@version 1.0
*	@date 20/10/2017
*	@author Christian Rangel
*	@title Principal program
*	@brief Principal program to execution for the MCU
*
*/
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define buttonOk_Pin GPIO_PIN_13
#define buttonOk_GPIO_Port GPIOC
#define buttonOk_EXTI_IRQn EXTI15_10_IRQn
#define higro2_Pin GPIO_PIN_1
#define higro2_GPIO_Port GPIOC
#define buttonUp_Pin GPIO_PIN_2
#define buttonUp_GPIO_Port GPIOC
#define buttonUp_EXTI_IRQn EXTI2_IRQn
#define buttonDown_Pin GPIO_PIN_3
#define buttonDown_GPIO_Port GPIOC
#define buttonDown_EXTI_IRQn EXTI3_IRQn
#define ldr1_Pin GPIO_PIN_0
#define ldr1_GPIO_Port GPIOA
#define ldr2_Pin GPIO_PIN_1
#define ldr2_GPIO_Port GPIOA
#define ldr3_Pin GPIO_PIN_4
#define ldr3_GPIO_Port GPIOA
#define resCalefAmb_Pin GPIO_PIN_5
#define resCalefAmb_GPIO_Port GPIOA
#define DHT11_1_Pin GPIO_PIN_6
#define DHT11_1_GPIO_Port GPIOA
#define valAdmi_Pin GPIO_PIN_7
#define valAdmi_GPIO_Port GPIOA
#define Si_Pin GPIO_PIN_4
#define Si_GPIO_Port GPIOC
#define Si_EXTI_IRQn EXTI4_IRQn
#define E_LCD_Pin GPIO_PIN_5
#define E_LCD_GPIO_Port GPIOC
#define higro1_Pin GPIO_PIN_0
#define higro1_GPIO_Port GPIOB
#define Sf_Pin GPIO_PIN_1
#define Sf_GPIO_Port GPIOB
#define Sf_EXTI_IRQn EXTI1_IRQn
#define Humif_Pin GPIO_PIN_2
#define Humif_GPIO_Port GPIOB
#define D4_LCD_Pin GPIO_PIN_12
#define D4_LCD_GPIO_Port GPIOB
#define D7_LCD_Pin GPIO_PIN_13
#define D7_LCD_GPIO_Port GPIOB
#define D6_LCD_Pin GPIO_PIN_14
#define D6_LCD_GPIO_Port GPIOB
#define D5_LCD_Pin GPIO_PIN_15
#define D5_LCD_GPIO_Port GPIOB
#define Sim800l_RX_Pin GPIO_PIN_6
#define Sim800l_RX_GPIO_Port GPIOC
#define Sim800l_TX_Pin GPIO_PIN_7
#define Sim800l_TX_GPIO_Port GPIOC
#define RS_LCD_Pin GPIO_PIN_8
#define RS_LCD_GPIO_Port GPIOC
#define DHT11_2_Pin GPIO_PIN_8
#define DHT11_2_GPIO_Port GPIOA
#define oneWireDS18B20_Pin GPIO_PIN_9
#define oneWireDS18B20_GPIO_Port GPIOA
#define A_LCD_Pin GPIO_PIN_12
#define A_LCD_GPIO_Port GPIOA
#define ledGreenOn_Pin GPIO_PIN_10
#define ledGreenOn_GPIO_Port GPIOC
#define ledRedOff_Pin GPIO_PIN_11
#define ledRedOff_GPIO_Port GPIOC
#define resCalefComp_Pin GPIO_PIN_3
#define resCalefComp_GPIO_Port GPIOB
#define valExtr_Pin GPIO_PIN_6
#define valExtr_GPIO_Port GPIOB
#define buttonOnOff_Pin GPIO_PIN_7
#define buttonOnOff_GPIO_Port GPIOB
#define buttonOnOff_EXTI_IRQn EXTI9_5_IRQn
#define door_Pin GPIO_PIN_8
#define door_GPIO_Port GPIOB
#define A_A_Pin GPIO_PIN_9
#define A_A_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
