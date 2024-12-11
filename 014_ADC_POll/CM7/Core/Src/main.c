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
#include <stdio.h>
#include <string.h>

/* Private defines -----------------------------------------------------------*/
#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0 */
#endif

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;  // ADC handle
UART_HandleTypeDef huart1; // UART handle

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART1_UART_Init(void);
void floatToString(float value, char *buffer, int decimalPlaces); // Converts float to string

/* Main function -------------------------------------------------------------*/
int main(void)
{
  /* Initialize the HAL Library */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();

  uint16_t ADC_Val = 0;
  float volt = 0;
  char UART1_rxBuffer[20] = {0}; // UART buffer for transmitting voltage value

  /* Infinite loop */
  while (1)
  {
    // Start ADC conversion
    HAL_ADC_Start(&hadc1);

    // Wait for ADC conversion to complete
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
    {
      // Get the ADC value
      ADC_Val = HAL_ADC_GetValue(&hadc1);

      // Calculate voltage value
      volt = (ADC_Val * 3.3) / 65535;

      // Transmit "voltage is" message via UART
      HAL_UART_Transmit(&huart1, (uint8_t *)"voltage is", strlen("voltage is"), HAL_MAX_DELAY);

      // Convert float voltage value to string and transmit
      floatToString(volt, UART1_rxBuffer, 2);
      HAL_UART_Transmit(&huart1, (uint8_t *)UART1_rxBuffer, 20, HAL_MAX_DELAY);

      // Transmit newline character
      HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", strlen("\r\n"), HAL_MAX_DELAY);

      // Stop ADC conversion
      HAL_ADC_Stop(&hadc1);
    }

    // Wait for 2 seconds before the next reading
    HAL_Delay(2000);
  }
}

/* Function to convert float to string with specified decimal places */
void floatToString(float value, char *buffer, int decimalPlaces)
{
    int intPart = (int)value;  // Integer part of the float
    float fracPart = value - intPart;  // Fractional part of the float
    int index = 0;

    if (intPart < 0) {
        buffer[index++] = '-';  // Add negative sign for negative numbers
        intPart = -intPart;
    }

    int tempIndex = index;
    // Convert integer part to string
    do {
        buffer[tempIndex++] = (intPart % 10) + '0';
        intPart /= 10;
    } while (intPart > 0);

    // Reverse the integer part string
    int start = index;
    int end = tempIndex - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }

    index = tempIndex;
    buffer[index++] = '.';  // Add decimal point

    // Convert fractional part to string with specified decimal places
    for (int j = 0; j < decimalPlaces; j++) {
        fracPart *= 10;
        int digit = (int)fracPart;
        buffer[index++] = digit + '0';
        fracPart -= digit;
    }

    buffer[index] = '\0';  // Null-terminate the string
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Configure the main internal regulator output voltage */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // Wait until the voltage regulator is stable
  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Initialize the RCC Oscillators */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initialize the CPU, AHB, and APB buses clocks */
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
    Error_Handler();
  }
}

/* ADC1 Initialization */
static void MX_ADC1_Init(void)
{
  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* Common ADC configuration */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.Oversampling.Ratio = 1;

  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure the ADC multi-mode */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /* Configure Regular Channel */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  sConfig.OffsetSignedSaturation = DISABLE;

  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USART1 Initialization */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
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
    Error_Handler();
  }

  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

/* Error Handler function */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
