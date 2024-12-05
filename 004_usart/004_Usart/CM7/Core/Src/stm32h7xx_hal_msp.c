#include <stm32h7xx_hal.h>  // Include STM32 HAL library for the H7 series

void HAL_MspInit(void)
{
  // Low-level processor-specific initializations
  // 1. Set the priority grouping for the Cortex-Mx processor
  // 2. Enable system exceptions for the ARM Cortex processor
  // 3. Configure priority for system exceptions

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);  // Set priority grouping to 4 bits for pre-emption priority and 4 bits for subpriority

  SCB->SHCSR |= 0x7 << 16;  // Enable system exceptions: USAGE, MEMFAULT, and BUSFAULT (bit 16-18)

  // Set the highest priority (0, 0) for system exceptions
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);  // Set priority for Memory Management fault
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);          // Set priority for Bus Fault
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);        // Set priority for Usage Fault
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_uart;  // GPIO configuration structure for UART

  // Low-level initialization of the UART1 peripheral
  // 1. Enable clocks for UART1 and its associated GPIO port
  __HAL_RCC_C1_USART1_CLK_ENABLE();  // Enable clock for UART1 peripheral
  __HAL_RCC_C1_GPIOA_CLK_ENABLE();   // Enable clock for GPIOA port

  // 2. Configure pin multiplexing for UART1
  gpio_uart.Pin = GPIO_PIN_9;                // Configure GPIO pin 9 for UART1 TX
  gpio_uart.Mode = GPIO_MODE_AF_PP;          // Set alternate function push-pull mode
  gpio_uart.Pull = GPIO_PULLUP;              // Enable pull-up resistor
  gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;     // Set low speed for GPIO
  gpio_uart.Alternate = GPIO_AF7_USART1;     // Set alternate function for USART1 (AF7)
  HAL_GPIO_Init(GPIOA, &gpio_uart);          // Initialize GPIOA pin 9 for UART1 TX

  gpio_uart.Pin = GPIO_PIN_10;               // Configure GPIO pin 10 for UART1 RX
  HAL_GPIO_Init(GPIOA, &gpio_uart);          // Initialize GPIOA pin 10 for UART1 RX

  // 3. Enable UART IRQ and set up interrupt priority
  HAL_NVIC_EnableIRQ(USART1_IRQn);           // Enable UART1 interrupt (USART1_IRQn)
  HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);  // Set interrupt priority for UART1 (lowest priority)
}
