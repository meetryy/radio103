/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern SPI_HandleTypeDef hspi1;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern SPI_HandleTypeDef hspi1;


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim1_ch1;

extern UART_HandleTypeDef huart1;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC_TIM_PERIOD (1024-1)
#define LED_BUILTIN_Pin GPIO_PIN_13
#define LED_BUILTIN_GPIO_Port GPIOC
#define ADC_RX_I_Pin GPIO_PIN_0
#define ADC_RX_I_GPIO_Port GPIOA
#define ADC_RX_Q_Pin GPIO_PIN_1
#define ADC_RX_Q_GPIO_Port GPIOA
#define PIN_LCD_CS_Pin GPIO_PIN_0
#define PIN_LCD_CS_GPIO_Port GPIOB
#define TX_Q_PWM_Pin GPIO_PIN_8
#define TX_Q_PWM_GPIO_Port GPIOA
#define PIN_CS_EXT_Pin GPIO_PIN_9
#define PIN_CS_EXT_GPIO_Port GPIOA
#define AUDIO_PWM_Pin GPIO_PIN_10
#define AUDIO_PWM_GPIO_Port GPIOA
#define TX_Q_PWMA11_Pin GPIO_PIN_11
#define TX_Q_PWMA11_GPIO_Port GPIOA
#define PIN_LCD_CS_2_Pin GPIO_PIN_12
#define PIN_LCD_CS_2_GPIO_Port GPIOA
#define PIN_LCD_DC_Pin GPIO_PIN_15
#define PIN_LCD_DC_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
