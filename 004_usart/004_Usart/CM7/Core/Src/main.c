#include <stm32h7xx_hal.h>  // Include STM32 HAL library for the H7 series
#include <string.h>          // Include string manipulation functions

void SystemClockConfig(void);   // Function prototype for system clock configuration
void USART1_Init(void);         // Function prototype for UART1 initialization
void Error_Handler(void);       // Function prototype for error handler

UART_HandleTypeDef huart1;      // Declare UART handle for UART1

int main(){

	HAL_Init();  // Initialize the Hardware Abstraction Layer (HAL) for the MCU
	SystemClockConfig();  // Configure the system clock (uses default HSI)

	USART1_Init(); // Initialize UART1 with configured settings
	uint8_t UART1_rxBuffer[12] = {0}; // Declare a buffer to receive data via UART1
	while(1){
		HAL_UART_Receive (&huart1, UART1_rxBuffer, 12, 5000);  // Receive 12 bytes with a 5000ms timeout
		HAL_UART_Transmit(&huart1, UART1_rxBuffer, 12, 100);   // Transmit 12 bytes of data back
		memset(UART1_rxBuffer,0, sizeof(UART1_rxBuffer));  // Clear the buffer after transmission
	}

	return 0;  // Main function should return, though the program loops infinitely
}

void SystemClockConfig(){
	// Empty function, can be configured if needed for custom clock settings
}


void USART1_Init(void){

	huart1.Instance = USART1;  // Set UART1 instance
	huart1.Init.BaudRate = 115200;  // Set the baud rate to 115200
	huart1.Init.WordLength = UART_WORDLENGTH_8B;  // Set word length to 8 bits
	huart1.Init.StopBits = UART_STOPBITS_1;  // Set stop bits to 1
	huart1.Init.Parity = UART_PARITY_NONE;  // Set parity to none
	huart1.Init.Mode = UART_MODE_TX_RX;  // Enable both transmit and receive modes
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // No hardware flow control
	if (HAL_UART_Init(&huart1) != HAL_OK)  // Check if UART initialization is successful
	{
	    Error_Handler();  // Call error handler if initialization fails
	}

}

void Error_Handler(void)
{
  __disable_irq();  // Disable interrupts to prevent further system actions
  while (1)
  {
    // Infinite loop for error handling, the system is stuck here on errors
  }
}
