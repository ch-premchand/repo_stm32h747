/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include <string.h>
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;   // ADC handler for ADC1
UART_HandleTypeDef huart1; // UART handler for USART1

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  // Initialize the HAL library and configure the system clock
  HAL_Init();
  SystemClock_Config();

  // Initialize peripherals: GPIO, UART, and ADC
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();

  uint16_t AD_RES = 0; // Variable to store ADC result
  char UART_BUFF[20] = {0}; // Buffer for UART data transmission

  // Send a greeting message over UART
  HAL_UART_Transmit(&huart1, (uint8_t *)"hi\r\n", strlen("hi\r\n"), HAL_MAX_DELAY);

  while (1)
  {
    // Loop to read ADC values from two channels
    for (uint16_t i = 0; i < 2; i++) {
      // Start ADC conversion
      HAL_ADC_Start(&hadc1);
      HAL_ADC_PollForConversion(&hadc1, 10);  // Wait for conversion to complete
      AD_RES = HAL_ADC_GetValue(&hadc1);      // Get ADC result

      // Format and send ADC value over UART
      switch (i) {
        case 0:
          sprintf(UART_BUFF, "ADC1_c0 is %d \r\n", AD_RES);
          HAL_UART_Transmit(&huart1, (uint8_t *)UART_BUFF, 20, HAL_MAX_DELAY);
          break;
        case 1:
          sprintf(UART_BUFF, "ADC1_c1 is %d \r\n", AD_RES);
          HAL_UART_Transmit(&huart1, (uint8_t *)UART_BUFF, 20, HAL_MAX_DELAY);
          break;
      }
    }
    HAL_Delay(2000); // Wait for 2 seconds before the next ADC reading
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure the main internal regulator output voltage
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Wait until voltage scaling is ready
  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  // Initialize the RCC oscillators with HSI as the source
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(); // Call error handler if configuration fails
  }

  // Initialize the CPU, AHB and APB buses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                              | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler(); // Call error handler if clock configuration fails
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  // Configure ADC1
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;    // Enable scan mode for multiple channels
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV; // Wait for a single conversion completion
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE; // Single conversion mode
  hadc1.Init.NbrOfConversion = 2;           // Use 2 channels
  hadc1.Init.DiscontinuousConvMode = ENABLE; // Enable discontinuous conversion mode
  hadc1.Init.NbrOfDiscConversion = 1;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Software trigger for conversion
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED; // Preserve data on overrun
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.Oversampling.Ratio = 1;

  // Initialize ADC
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler(); // Call error handler if initialization fails
  }

  // Configure ADC multi-mode
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler(); // Call error handler if multi-mode configuration fails
  }

  // Configure ADC channels
  sConfig.Channel = ADC_CHANNEL_18;           // First ADC channel
  sConfig.Rank = ADC_REGULAR_RANK_1;          // First rank for conversion
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;      // Single-ended input
  sConfig.OffsetNumber = ADC_OFFSET_NONE;     // No offset
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler(); // Call error handler if channel configuration fails
  }

  // Configure second ADC channel
  sConfig.Channel = ADC_CHANNEL_1;            // Second ADC channel
  sConfig.Rank = ADC_REGULAR_RANK_2;          // Second rank for conversion
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler(); // Call error handler if channel configuration fails
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  // Configure USART1 (UART communication)
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;              // Baud rate for UART communication
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler(); // Call error handler if UART initialization fails
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  // Enable GPIO ports clock for communication
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  // Enter an infinite loop in case of an error
  __disable_irq();
  while (1)
  {
  }
}
