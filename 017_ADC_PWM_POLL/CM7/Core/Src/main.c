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
TIM_HandleTypeDef htim1;   // Timer handler for TIM1 (PWM generation)
UART_HandleTypeDef huart1; // UART handler for USART1

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Initialize all configured peripherals */
  HAL_Init();                  // Initialize the Hardware Abstraction Layer (HAL)
  SystemClock_Config();         // Configure system clock
  MX_GPIO_Init();              // Initialize GPIO
  MX_USART1_UART_Init();       // Initialize USART1 (UART for communication)
  MX_ADC1_Init();              // Initialize ADC1 (for analog reading)
  MX_TIM1_Init();              // Initialize Timer 1 for PWM generation

  /* Start PWM signal on TIM1 Channel 1 */
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

  uint16_t AD_RES = 0, Duty = 0;
  char UART_BUFF[25] = {0};

  /* Transmit greeting via UART */
  HAL_UART_Transmit(&huart1, (uint8_t *)"hi\r\n", strlen("hi\r\n"), HAL_MAX_DELAY);

  /* Infinite loop */
  while (1)
  {
    /* Start ADC conversion */
    HAL_ADC_Start(&hadc1);

    /* Wait for the conversion to complete */
    HAL_ADC_PollForConversion(&hadc1, 10);

    /* Read the ADC result */
    AD_RES = HAL_ADC_GetValue(&hadc1);

    /* Transmit ADC value over UART */
    sprintf(UART_BUFF, "ADC1_ch1 is %d\r\n", AD_RES);
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BUFF, strlen(UART_BUFF), HAL_MAX_DELAY);

    /* Set PWM duty cycle based on ADC value */
    Duty = AD_RES;  // Use ADC result directly for duty cycle
    sprintf(UART_BUFF, "Duty cycle is %d\r\n", Duty);
    HAL_UART_Transmit(&huart1, (uint8_t *)UART_BUFF, strlen(UART_BUFF), HAL_MAX_DELAY);

    /* Update PWM duty cycle */
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, Duty);

    /* Delay before next ADC reading */
    HAL_Delay(2000);
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

  /* Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* Configure the main internal regulator output voltage */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Wait for voltage scaling to be ready */
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /* Initialize the RCC Oscillators */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();  // Handle errors during RCC configuration
  }

  /* Initialize the CPU, AHB, and APB buses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();  // Handle errors during clock configuration
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /* Configure ADC1 */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;             // Single channel conversion
  hadc1.Init.ContinuousConvMode = DISABLE;                // Disable continuous mode
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;             // 16-bit resolution
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;       // Trigger conversion via software
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;          // Single conversion per trigger
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();  // Handle errors during ADC initialization
  }

  /* Configure ADC channel */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;                      // First rank in regular sequence
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;         // Sampling time for ADC
  sConfig.SingleDiff = ADC_SINGLE_ENDED;                  // Single-ended mode (no differential input)
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();  // Handle errors during channel configuration
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* Configure TIM1 for PWM output */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;                              // Maximum period for 16-bit resolution
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();  // Handle errors during PWM initialization
  }

  /* Configure PWM output on TIM1 Channel 1 */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;                                     // Initial duty cycle set to 0
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;              // High polarity for PWM signal
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();  // Handle errors during PWM channel configuration
  }
  HAL_TIM_MspPostInit(&htim1);  // Perform post initialization of the TIM1 peripheral
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  /* Configure USART1 for communication */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();  // Handle errors during UART initialization
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* Enable clock for GPIO ports */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
    // Stay here if error occurs
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add implementation to report the file name and line number */
}
#endif /* USE_FULL_ASSERT */
