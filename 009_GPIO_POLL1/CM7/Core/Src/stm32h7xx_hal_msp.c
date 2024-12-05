/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32h7xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-initialization functions.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"  // Includes the main header file that provides access to peripherals and HAL functions

/**
  * @brief  Initializes the Global MSP (MCU Support Package).
  * @retval None
  */
void HAL_MspInit(void)
{
  /* Enable SYSCFG clock (System Configuration controller) */
  __HAL_RCC_SYSCFG_CLK_ENABLE();  // Enables the clock for the SYSCFG (System Configuration) peripheral, necessary for system configuration

  /* Additional system-level interrupt and peripheral initializations can be done here. */

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  /* System interrupt init can be configured here if needed */

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}
