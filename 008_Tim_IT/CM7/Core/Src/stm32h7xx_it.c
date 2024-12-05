#include "stm32h7xx_hal.h"

// External declaration of Timer 7 handler
extern TIM_HandleTypeDef HTim7;

/**
 * @brief  This function handles the SysTick interrupt (System Tick Timer).
 *         The SysTick timer is a built-in timer in ARM Cortex-M processors
 *         that is used for timekeeping, and it triggers periodically.
 */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
  // This section can be used to add custom code before HAL_IncTick() is called.
  // For example, you could use it to handle custom tasks before incrementing the tick counter.

  /* USER CODE END SysTick_IRQn 0 */

  // Increment the SysTick counter, this is necessary for HAL to manage the tick time.
  // HAL_IncTick() is called every time SysTick interrupt occurs, so it helps track system uptime.
  HAL_IncTick();

  /* USER CODE BEGIN SysTick_IRQn 1 */
  // This section can be used for custom code to handle tasks after HAL_SYSTICK_IRQHandler() is called.
  // For example, adding other actions triggered by the SysTick timer.

  // Call the HAL library's SysTick handler to process the SysTick interrupt.
  HAL_SYSTICK_IRQHandler();

  /* USER CODE END SysTick_IRQn 1 */
}

/**
 * @brief  This function handles the Timer 7 interrupt.
 *         It is called whenever Timer 7 generates an interrupt (e.g., after it overflows or reaches a compare value).
 */
void TIM7_IRQHandler(void)
{
  // Call the HAL_TIM_IRQHandler function to process the Timer 7 interrupt.
  // This will check if the interrupt was caused by Timer 7 and handle it accordingly.
  HAL_TIM_IRQHandler(&HTim7);
}
