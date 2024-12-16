#include "main.h"  // Includes the main header file containing declarations for functions and peripherals
#include <string.h>  // Includes the standard string library for string manipulation functions

// Global Variable Declarations
void SystemClock_Config(void);  // Declaration for function to configure the system clock
void MX_USART1_Init(void);  // Declaration for function to initialize USART1

UART_HandleTypeDef huart1;  // Declaration of UART handle for USART1 communication
char *data = "prem\r\n";  // String to be transmitted over UART (a message "prem" with a carriage return and newline)

int main(void)  // Main function where program execution starts
{
  HAL_Init();  // Initializes the HAL (Hardware Abstraction Layer), required to set up peripherals
  SystemClock_Config();  // Configures the system clock (calls function to set up clock sources)

  //MX_GPIO_Init();  // This is commented out, but it would initialize GPIOs (General Purpose Input/Output)

  MX_USART1_Init();  // Initializes the USART1 peripheral with the configured settings
  uint16_t len_data = strlen(data);  // Calculates the length of the data string ("prem\r\n")

  //HAL_UART_Transmit(&huart1, (uint8_t *)data, len_data, HAL_MAX_DELAY);  // This line is commented out, but it would transmit data via UART

  //HAL_Delay(2000);  // This line is commented out, but it would introduce a 2-second delay

  HAL_UART_Transmit_IT(&huart1, (uint8_t *)data, len_data);  // Transmits the data string asynchronously using interrupt-based UART transmission
  HAL_Delay(1000);  // Introduces a 1-second delay after initiating transmission

  while (1)  // Infinite loop that keeps the program running (ideal for embedded systems)
  {
    // The code inside this loop would be continuously executed. It's empty here.
  }
}

// Function to configure the system clock for the microcontroller
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};  // Structure for oscillator configuration (initializes to zero)
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};  // Structure for clock configuration (initializes to zero)

  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);  // Configures the power supply for direct SMPS (Switch Mode Power Supply)

  // Configures the main internal regulator output voltage
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  // Configures voltage scaling to level 1 (higher performance)

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}  // Waits until the voltage scaling is stable (flag indicates readiness)

  // Initializes the RCC Oscillators based on the specified parameters in RCC_OscInitStruct
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;  // Uses High-Speed Internal (HSI) oscillator
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;  // Sets the HSI oscillator to its default state with no division
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;  // Uses the default calibration value for HSI
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;  // No PLL (Phase-Locked Loop) is used in this configuration

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)  // Configures the RCC oscillators and checks for errors
  {
    Error_Handler();  // If an error occurs, it calls the error handler function
  }

  // Initializes the CPU, AHB, and APB buses clocks based on the settings in RCC_ClkInitStruct
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                               RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                               RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;  // Specifies the types of clocks to configure
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;  // Uses the HSI oscillator as the system clock source
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;  // No division of the system clock
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;  // No division of the AHB clock
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;  // No division of the APB3 clock
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;  // No division of the APB1 clock
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;  // No division of the APB2 clock
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;  // No division of the APB4 clock

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)  // Configures the RCC clock settings and checks for errors
  {
    Error_Handler();  // If an error occurs, it calls the error handler function
  }
}

// Function to initialize USART1 (Universal Asynchronous Receiver-Transmitter)
void MX_USART1_Init(void)
{
	huart1.Instance = USART1;  // Specifies USART1 as the instance for the UART handle
	huart1.Init.BaudRate = 115200;  // Sets the baud rate for communication to 115200 bps
	huart1.Init.WordLength = UART_WORDLENGTH_8B;  // Specifies 8-bit word length for data transmission
	huart1.Init.StopBits = UART_STOPBITS_1;  // Uses 1 stop bit for data framing
	huart1.Init.Parity = UART_PARITY_NONE;  // No parity bit is used for error detection
	huart1.Init.Mode = UART_MODE_TX_RX;  // Enables both transmission and reception modes
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // No hardware flow control for UART (no RTS/CTS)
	if (HAL_UART_Init(&huart1) != HAL_OK)  // Initializes the UART peripheral with the specified configuration
	{
	    Error_Handler();  // If initialization fails, the error handler is called
	}
}

// Error handler function that disables interrupts and enters an infinite loop
void Error_Handler(void)
{
  __disable_irq();  // Disables all interrupts globally (no further interrupts can occur)
  while (1)  // Infinite loop to halt execution if an error occurs
  {
    // This loop keeps the system in a halted state when an error occurs.
  }
}

// Alternative USART initialization function (not used here, as it's commented out)
#if 0
void MX_USART1_UART_Init(void)
{
  __HAL_RCC_C1_USART1_CLK_ENABLE();  // Enables USART1 clock (in this case, an incorrect or redundant function)
  huart1.Instance = USART1;  // USART1 instance
  huart1.Init.BaudRate = 115200;  // Baud rate configuration
  huart1.Init.WordLength = UART_WORDLENGTH_8B;  // 8-bit word length
  huart1.Init.StopBits = UART_STOPBITS_1;  // 1 stop bit
  huart1.Init.Parity = UART_PARITY_NONE;  // No parity
  huart1.Init.Mode = UART_MODE_TX_RX;  // TX and RX enabled
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // No hardware flow control
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;  // 16x oversampling
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;  // No one-bit sampling
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;  // Clock prescaler for the UART

  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;  // No advanced features enabled

  if (HAL_UART_Init(&huart1) != HAL_OK)  // Initializes UART1 with the specified configuration
  {
    Error_Handler();  // Calls error handler on failure
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){  // UART transmission complete callback function (empty)
}

void MX_GPIO_Init(void)  // Initializes GPIO pins (not used in the main code)
{
  __HAL_RCC_GPIOB_CLK_ENABLE();  // Enables the clock for GPIOB port
}
#endif
