#include "main.h"  // Includes the main header file that contains declarations for functions and peripherals
#include "stm32h7xx_it.h"  // Includes the STM32H7 interrupt vector table header file

extern UART_HandleTypeDef huart1;  // Extern declaration of the UART handle (used for USART1 communication)

// SysTick_Handler function is called whenever the SysTick interrupt occurs
void SysTick_Handler(void)
{
  HAL_IncTick();  // Increments the HAL tick (used for delay functions, timeouts, etc.)
}

// USART1_IRQHandler function handles the interrupt for USART1
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);  // Handles the UART interrupt for USART1 (processing received or transmitted data)
}
