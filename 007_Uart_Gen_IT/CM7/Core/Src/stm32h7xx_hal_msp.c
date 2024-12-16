#include "main.h"  // Includes the main header file containing declarations for functions and peripherals

// HAL_MspInit function initializes low-level hardware components (system-level configuration)
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();  // Enables the clock for the SYSCFG (System Configuration) peripheral
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);  // Configures the priority grouping of interrupts to NVIC_PRIORITYGROUP_4

  SCB->SHCSR |= 0X7 << 16;  // Sets the system handler control and state register for USAGE, MEMORY, and BUS fault handling (enables these exceptions)

  // Sets the priority for the memory management, bus fault, and usage fault exceptions
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);  // Memory management fault priority set to 0 (highest priority)
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);  // Bus fault priority set to 0 (highest priority)
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);  // Usage fault priority set to 0 (highest priority)
}

// HAL_UART_MspInit function initializes the low-level hardware for USART (Universal Synchronous/Asynchronous Receiver-Transmitter)
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_uart;  // Structure for GPIO initialization (used to configure GPIO pins for UART)

  // Step 1: Enable the clock for UART1 and GPIOA peripherals
  __HAL_RCC_C1_USART1_CLK_ENABLE();  // Enables the clock for USART1 peripheral (UART1)
  __HAL_RCC_C1_GPIOA_CLK_ENABLE();  // Enables the clock for GPIOA (used for UART pins)

  // Step 2: Configure the GPIO pins for UART1 (Pin 9 for TX, Pin 10 for RX)
  gpio_uart.Pin = GPIO_PIN_9 | GPIO_PIN_10;  // Specifies that pins 9 and 10 are used for UART TX and RX
  gpio_uart.Mode = GPIO_MODE_AF_PP;  // Configures the pins as Alternate Function Push-Pull (AF_PP)
  gpio_uart.Pull = GPIO_NOPULL;  // No pull-up or pull-down resistors on the pins
  gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;  // Sets the GPIO speed to low frequency
  gpio_uart.Alternate = GPIO_AF7_USART1;  // Sets the alternate function to AF7 for USART1 (TX/RX functionality)
  HAL_GPIO_Init(GPIOA, &gpio_uart);  // Initializes the GPIOA pins 9 and 10 with the specified configuration

  // Step 3: Enable the USART1 interrupt and set its priority
  HAL_NVIC_EnableIRQ(USART1_IRQn);  // Enables the interrupt for USART1 (allows interrupt handling when data is received or transmitted)
  HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);  // Sets the priority of USART1 interrupt to 1 (lower priority than faults)
}

// The following code is commented out and not used in the current implementation
#if 0
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};  // Structure to initialize GPIO pins for UART
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};  // Structure to configure peripheral clock for USART

  if (huart->Instance == USART1)  // Check if the instance is USART1
  {
    // Configure the peripheral clock for USART1
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;  // Select the clock source for USART1
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)  // Check if peripheral clock configuration is successful
    {
      Error_Handler();  // If configuration fails, enter the error handler
    }

    // Enable clock for USART1 peripheral
    __HAL_RCC_USART1_CLK_ENABLE();  // Enable USART1 clock

    // GPIO pin initialization for UART1 (TX on PB6, RX on PB7)
    GPIO_InitStruct.Pin = GPIO_PIN_6;  // Pin 6 is used for USART1_TX
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  // Configure as alternate function push-pull
    GPIO_InitStruct.Pull = GPIO_PULLUP;  // Enable pull-up resistor for TX pin
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;  // Set GPIO speed to low frequency
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;  // Set alternate function for USART1 (TX)
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  // Initialize pin PB6

    GPIO_InitStruct.Pin = GPIO_PIN_7;  // Pin 7 is used for USART1_RX
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  // Initialize pin PB7

    // Configure the priority and enable the USART1 interrupt
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);  // Set the priority of the USART1 interrupt to 0 (highest priority)
    HAL_NVIC_EnableIRQ(USART1_IRQn);  // Enable USART1 interrupt in the NVIC (nested vector interrupt controller)
  }
}
#endif
