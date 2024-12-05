#include "main.h"

// Function prototypes
void SystemClock_Config(void);         // Function to configure the system clock
static void MX_GPIO_Init(void);       // Function to initialize GPIO
void Error_Handler(void);             // Function to handle errors

int main(void)
{
  HAL_Init();  // Initialize the Hardware Abstraction Layer (HAL)

  SystemClock_Config();  // Configure the system clock
  MX_GPIO_Init();       // Initialize GPIO pins

  while (1) {
    // Check if the button on PC13 is pressed (active low)
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) {
#if 0
      // Block of code is disabled (used for debouncing and toggling the GPIO)
      HAL_Delay(200);  // Small delay to debounce the button
      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12, RESET);  // Turn off LED on GPIOI_PIN_12
      HAL_Delay(1000);  // Delay for 1 second
      HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12, SET);  // Turn on LED on GPIOI_PIN_12
      // Add a small delay to debounce the button and avoid rapid toggling
      // HAL_Delay(200);  // Adjust delay as necessary
#endif

      // Toggle the LED connected to GPIOI_PIN_12 when the button is pressed
      HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_12);
      HAL_Delay(1000);  // Delay for 1 second between toggles
    }
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};  // Structure to configure the oscillators
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};  // Structure to configure the system clock

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);  // Configure the power supply mode

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  // Set the voltage scaling to Scale 1 for performance

  // Wait until the voltage scaling is ready
  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;  // Select High-Speed Internal oscillator (HSI)
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;  // Use HSI with no division
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;  // Default HSI calibration value
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;  // No PLL enabled
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();  // If oscillator configuration fails, handle error
  }

  /** Initializes the CPU, AHB, and APB bus clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;  // Use HSI as system clock source
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;  // No division of system clock
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;  // No division of AHB clock
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;  // No division of APB3 clock
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;  // No division of APB1 clock
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;  // No division of APB2 clock
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;  // No division of APB4 clock

  // Configure the system clocks
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();  // If clock configuration fails, handle error
  }
}

void Error_Handler(void) {
  __disable_irq();  // Disable interrupts
  while(1) {
    // Stay in an infinite loop if an error occurs
  }
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};  // Structure to initialize GPIO pins

  // Enable clocks for GPIOC and GPIOI
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12, GPIO_PIN_SET);  // Set the GPIO pin I12 to high (LED off)

  /*Configure GPIO pin : PC13 (Button Input Pin) */
  GPIO_InitStruct.Pin = GPIO_PIN_13;  // Configure pin 13 on port C
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;  // Set pin mode to input
  GPIO_InitStruct.Pull = GPIO_PULLUP;  // Enable pull-up resistor (button pressed = low)
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);  // Initialize GPIOC pin 13

  /*Configure GPIO pin : PI12 (LED Output Pin) */
  GPIO_InitStruct.Pin = GPIO_PIN_12;  // Configure pin 12 on port I
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Set pin mode to push-pull output
  GPIO_InitStruct.Pull = GPIO_NOPULL;  // No pull-up or pull-down resistor
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // Set GPIO speed to low frequency
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);  // Initialize GPIOI pin 12
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
