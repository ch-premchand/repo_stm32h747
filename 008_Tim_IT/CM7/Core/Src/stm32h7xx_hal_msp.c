#include <stm32h7xx_hal.h>

// This function is used for low-level initialization of the microcontroller
// and setting up global system settings like interrupt priority and system exceptions.
void HAL_MspInit(void)
{
    // 1. Set priority grouping for the Cortex-Mx processor
    // Priority grouping defines how many bits of the priority are used for preemption and subpriority.
    // NVIC_PRIORITYGROUP_4 means 4 bits for preemption priority and 0 bits for subpriority.
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    // 2. Enable critical system exceptions like Usage Fault, Memory Management Fault, and Bus Fault.
    // The bitwise OR operation sets these exceptions to be enabled in the System Control Register.
    SCB->SHCSR |= 0x7 << 16; // Enable Usage Fault, Memory Fault, Bus Fault

    // 3. Set the priority for the system exceptions.
    // Set high priority (0) for critical exceptions to ensure they are handled first.
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);  // Highest priority for Memory Management Fault
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);           // Highest priority for Bus Fault
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);         // Highest priority for Usage Fault
}

// This function is responsible for initializing the UART (Universal Asynchronous Receiver Transmitter)
// for communication, including enabling clocks, configuring GPIO pins, and enabling the USART interrupt.
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_uart;

    // 1. Enable the clock for USART1 and GPIOA (which controls UART pins)
    __HAL_RCC_C1_USART1_CLK_ENABLE(); // Enable clock for USART1
    __HAL_RCC_C1_GPIOA_CLK_ENABLE();  // Enable clock for GPIOA (which holds the TX/RX pins for USART1)

    // 2. Configure the GPIO pins for USART1 (TX on PA9, RX on PA10)
    // Configure PA9 (TX) pin for USART1 functionality
    gpio_uart.Pin = GPIO_PIN_9;
    gpio_uart.Mode = GPIO_MODE_AF_PP;      // Alternate function push-pull mode (for UART)
    gpio_uart.Pull = GPIO_PULLUP;          // Enable pull-up resistor to keep the signal high when idle
    gpio_uart.Speed = GPIO_SPEED_FREQ_LOW; // Low speed to save power (this is often sufficient for UART)
    gpio_uart.Alternate = GPIO_AF7_USART1; // Set the alternate function to USART1 for PA9
    HAL_GPIO_Init(GPIOA, &gpio_uart);     // Apply configuration to PA9 pin

    // Configure PA10 (RX) pin for USART1 functionality
    gpio_uart.Pin = GPIO_PIN_10;          // Use PA10 pin for RX
    HAL_GPIO_Init(GPIOA, &gpio_uart);     // Apply configuration to PA10 pin

    // 3. Enable USART1 interrupt and set its priority
    // Enable the interrupt for USART1 (to handle data reception)
    HAL_NVIC_EnableIRQ(USART1_IRQn);

    // Set the interrupt priority for USART1
    // Low priority (15) because we don't need immediate response for this interrupt
    HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);
}

// This function initializes Timer 7, enabling its clock and setting up its interrupt priority.
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    // Enable the clock for Timer 7
    __HAL_RCC_TIM7_CLK_ENABLE();

    // Enable the interrupt for Timer 7
    HAL_NVIC_EnableIRQ(TIM7_IRQn);

    // Set the interrupt priority for Timer 7 to low (15) because it is not critical
    HAL_NVIC_SetPriority(TIM7_IRQn, 15, 0);
}
