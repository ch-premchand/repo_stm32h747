

#include "stm32h7xx_hal.h"

void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();   // for cube stm32 increment sys tick timer
  /* USER CODE BEGIN SysTick_IRQn 1 */
  HAL_SYSTICK_IRQHandler();

  /* USER CODE END SysTick_IRQn 1 */
}
