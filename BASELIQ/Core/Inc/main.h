/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GNSS/GNSSCom.h"
#include "LORA/RFM9x.h"
#include "LORA/LORACom.h"
#include "FreeRTOS.h"
#include "semphr.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */


/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PROC_M2_Pin GPIO_PIN_2
#define PROC_M2_GPIO_Port GPIOE
#define PROC_RESET_FPGA_Pin GPIO_PIN_3
#define PROC_RESET_FPGA_GPIO_Port GPIOE
#define UI_LED_R_Pin GPIO_PIN_4
#define UI_LED_R_GPIO_Port GPIOE
#define UI_LED_G_Pin GPIO_PIN_5
#define UI_LED_G_GPIO_Port GPIOE
#define UI_WakeUp_Pin GPIO_PIN_6
#define UI_WakeUp_GPIO_Port GPIOE
#define UI_WakeUp_EXTI_IRQn EXTI9_5_IRQn
#define GPS_PROC_TIME_INT_Pin GPIO_PIN_13
#define GPS_PROC_TIME_INT_GPIO_Port GPIOC
#define OSC_RTC_IN_Pin GPIO_PIN_14
#define OSC_RTC_IN_GPIO_Port GPIOC
#define OSC_RTC_OUT_Pin GPIO_PIN_15
#define OSC_RTC_OUT_GPIO_Port GPIOC
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOH
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOH
#define USB_VBUS_DETECT_WU_Pin GPIO_PIN_0
#define USB_VBUS_DETECT_WU_GPIO_Port GPIOA
#define UI_ARM_Pin GPIO_PIN_2
#define UI_ARM_GPIO_Port GPIOA
#define STM_DAC_TRIG_Pin GPIO_PIN_4
#define STM_DAC_TRIG_GPIO_Port GPIOA
#define STM_DAC_CS_Pin GPIO_PIN_5
#define STM_DAC_CS_GPIO_Port GPIOA
#define PROC_FPGA_BUSY_Pin GPIO_PIN_5
#define PROC_FPGA_BUSY_GPIO_Port GPIOC
#define PROC_FPGA_BUSY_EXTI_IRQn EXTI9_5_IRQn
#define UI_LED_B_Pin GPIO_PIN_1
#define UI_LED_B_GPIO_Port GPIOB
#define RFM_EN_Pin GPIO_PIN_7
#define RFM_EN_GPIO_Port GPIOE
#define RFM_RST_Pin GPIO_PIN_8
#define RFM_RST_GPIO_Port GPIOE
#define PROC_nCS_Pin GPIO_PIN_12
#define PROC_nCS_GPIO_Port GPIOE
#define PROC_SCLK_Pin GPIO_PIN_13
#define PROC_SCLK_GPIO_Port GPIOE
#define PROC_MISO_Pin GPIO_PIN_14
#define PROC_MISO_GPIO_Port GPIOE
#define PROC_MOSI_Pin GPIO_PIN_15
#define PROC_MOSI_GPIO_Port GPIOE
#define GPS_TX_STM_Pin GPIO_PIN_8
#define GPS_TX_STM_GPIO_Port GPIOD
#define GPS_RX_STM_Pin GPIO_PIN_9
#define GPS_RX_STM_GPIO_Port GPIOD
#define CLK_64M_EN_Pin GPIO_PIN_10
#define CLK_64M_EN_GPIO_Port GPIOD
#define ACQ_POW_EN_Pin GPIO_PIN_11
#define ACQ_POW_EN_GPIO_Port GPIOD
#define STM_ACQ_TRIG_SRC0_Pin GPIO_PIN_14
#define STM_ACQ_TRIG_SRC0_GPIO_Port GPIOD
#define STM_ACQ_TRIG_SRC1_Pin GPIO_PIN_15
#define STM_ACQ_TRIG_SRC1_GPIO_Port GPIOD
#define SD_DETECT_INT_Pin GPIO_PIN_7
#define SD_DETECT_INT_GPIO_Port GPIOC
#define SD_DETECT_INT_EXTI_IRQn EXTI9_5_IRQn
#define USB_VBUS_DETECT_Pin GPIO_PIN_9
#define USB_VBUS_DETECT_GPIO_Port GPIOA
#define STM_VCP_RX_Pin GPIO_PIN_10
#define STM_VCP_RX_GPIO_Port GPIOA
#define USB_D_N_Pin GPIO_PIN_11
#define USB_D_N_GPIO_Port GPIOA
#define USB_D_P_Pin GPIO_PIN_12
#define USB_D_P_GPIO_Port GPIOA
#define STM_SWDIO_Pin GPIO_PIN_13
#define STM_SWDIO_GPIO_Port GPIOA
#define STM_SWCLK_Pin GPIO_PIN_14
#define STM_SWCLK_GPIO_Port GPIOA
#define RFM_SPI_nCS_Pin GPIO_PIN_0
#define RFM_SPI_nCS_GPIO_Port GPIOD
#define RFM_SPI_SCK_Pin GPIO_PIN_1
#define RFM_SPI_SCK_GPIO_Port GPIOD
#define RFM_SPI_MISO_Pin GPIO_PIN_3
#define RFM_SPI_MISO_GPIO_Port GPIOD
#define RFM_SPI_MOSI_Pin GPIO_PIN_4
#define RFM_SPI_MOSI_GPIO_Port GPIOD
#define GPS_RTCM3_RX_STM_TX_Pin GPIO_PIN_5
#define GPS_RTCM3_RX_STM_TX_GPIO_Port GPIOD
#define GPS_RTCM3_TX_STM_RX_Pin GPIO_PIN_6
#define GPS_RTCM3_TX_STM_RX_GPIO_Port GPIOD
#define GPS_RESET_Pin GPIO_PIN_7
#define GPS_RESET_GPIO_Port GPIOD
#define STM_SWO_Pin GPIO_PIN_3
#define STM_SWO_GPIO_Port GPIOB
#define STM_VCP_TX_Pin GPIO_PIN_6
#define STM_VCP_TX_GPIO_Port GPIOB
#define RFM_IRQ_Pin GPIO_PIN_9
#define RFM_IRQ_GPIO_Port GPIOB
#define RFM_IRQ_EXTI_IRQn EXTI9_5_IRQn
#define PROC_M0_Pin GPIO_PIN_0
#define PROC_M0_GPIO_Port GPIOE
#define PROC_M1_Pin GPIO_PIN_1
#define PROC_M1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
