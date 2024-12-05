#include <stm32h7xx_hal.h>

// HAL_MspInit: Initializes low-level processor-specific settings
void HAL_MspInit(void)
{
    // 1. Set the priority grouping for the Cortex-M processor (NVIC) to 4 bits for pre-emption priority
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // 2. Enable system exceptions for ARM Cortex-M processor (Usage Fault, Memory Fault, Bus Fault)
    SCB->SHCSR |= 0X7 << 16; // Enable Usage Fault, Memory Fault, Bus Fault exceptions

    // 3. Set the highest priority for MemoryManagement, BusFault, and UsageFault interrupts
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

// HAL_UART_MspInit: Initializes low-level peripherals for USART1
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_uart;

    // 1. Enable clocks for USART1 and GPIOA (pins used for UART communication)
    __HAL_RCC_C1_USART1_CLK_ENABLE();
    __HAL_RCC_C1_GPIOA_CLK_ENABLE();

    // 2. Configure GPIO pins for USART1 (TX on PA9, RX on PA10)
    gpio_uart.Pin = GPIO_PIN_9;  // USART1 TX
    gpio_uart.Mode = GPIO_MODE_AF_PP;  // Alternate function, push-pull mode
    gpio_uart.Pull = GPIO_PULLUP;  // Enable pull-up resistor
    gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;  // Low-speed setting for power saving
    gpio_uart.Alternate = GPIO_AF7_USART1;  // Set alternate function for USART1
    HAL_GPIO_Init(GPIOA, &gpio_uart);  // Apply GPIO configuration for TX (PA9)

    gpio_uart.Pin = GPIO_PIN_10;  // USART1 RX
    HAL_GPIO_Init(GPIOA, &gpio_uart);  // Apply GPIO configuration for RX (PA10)

    // 3. Enable USART1 interrupt and configure its priority
    HAL_NVIC_EnableIRQ(USART1_IRQn);  // Enable USART1 interrupt
    HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);  // Set lowest priority for USART1 interrupt
}

// HAL_TIM_Base_MspInit: Initializes low-level peripherals for Timer 7
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    // 1. Enable clock for Timer 7 (TIM7 is used in the system)
    __HAL_RCC_TIM7_CLK_ENABLE();

    // 2. Enable Timer 7 interrupt and configure its priority
    HAL_NVIC_EnableIRQ(TIM17_IRQn);  // Enable the Timer 17 interrupt (TIM7 uses TIM17_IRQn)
    HAL_NVIC_SetPriority(TIM17_IRQn, 14, 0);  // Set Timer 17 interrupt priority
}
