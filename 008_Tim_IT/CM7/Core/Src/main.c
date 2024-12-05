#include <stm32h7xx_hal.h>
#include <string.h>

// Function prototypes for initialization and error handling
void TIM7_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);
void USART1_Init(void);
void GPIO_Init(void);

// Declare handles for Timer and UART peripherals
TIM_HandleTypeDef HTim7;
UART_HandleTypeDef huart1;

int main(){

    // Initialize the HAL Library (hardware abstraction layer)
    HAL_Init();

    // Configure the system clock
    SystemClock_Config();

    // Initialize GPIO for LED control
    GPIO_Init();

    // Initialize USART1 for UART communication
    USART1_Init();

    // Transmit the "HELLO" message over UART
    HAL_UART_Transmit(&huart1, (uint8_t *)"HELLO", 5, HAL_MAX_DELAY);

    // Initialize Timer 7 for periodic interrupts
    if(HAL_TIM_Base_Start_IT(&HTim7) != HAL_OK){
        // If Timer 7 initialization fails, enter the error handler
        Error_Handler();
    }

    // Main loop (remains empty as we are using interrupts for actions)
    while(1){
        // The main loop is empty because the action is handled in the timer interrupt
    }

    return 0;
}

// USART1 Initialization Function
void USART1_Init(void){

    // Configure USART1 peripheral settings
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;  // Set baud rate to 115200
    huart1.Init.WordLength = UART_WORDLENGTH_8B;  // Set word length to 8 bits
    huart1.Init.StopBits = UART_STOPBITS_1;  // Set 1 stop bit
    huart1.Init.Parity = UART_PARITY_NONE;  // No parity
    huart1.Init.Mode = UART_MODE_TX_RX;  // Enable both transmit and receive modes
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // No hardware flow control

    // Initialize USART1 with the configured settings
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        // If initialization fails, enter the error handler
        Error_Handler();
    }
}

// GPIO Initialization Function
void GPIO_Init(){
    // Enable clock for GPIOI port
    __HAL_RCC_GPIOI_CLK_ENABLE();

    GPIO_InitTypeDef ledgpio;

    // Set initial state of GPIO pin 12 to high (LED OFF)
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_12, GPIO_PIN_SET);

    // Configure GPIO pin 12 as output push-pull (for controlling LED)
    ledgpio.Pin = GPIO_PIN_12;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull mode for output
    ledgpio.Pull = GPIO_NOPULL;  // No pull-up or pull-down resistors
    HAL_GPIO_Init(GPIOI, &ledgpio);  // Apply configuration to GPIOI pin 12
}

// Timer 7 Initialization Function
void TIM7_Init(void){
    // Configure Timer 7 instance
    HTim7.Instance = TIM7;
    HTim7.Init.Prescaler = 3999;  // Set prescaler to 3999 (to divide the clock)
    HTim7.Init.Period = 3999;     // Set period to 3999 (timer counts to 3999)

    // Initialize the timer with the given configuration
    if(HAL_TIM_Base_Init(&HTim7) != HAL_OK){
        // If initialization fails, enter the error handler
        Error_Handler();
    }
}

// System Clock Configuration Function
void SystemClock_Config(void)
{
    // Declare structures for configuring the oscillator and clock sources
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure power supply (for direct SMPS supply)
    HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

    // Set voltage scaling to optimize power consumption (set to scale 1)
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    // Wait until the voltage scaling is ready
    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    // Configure the internal HSI (High-Speed Internal) oscillator
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;  // Use HSI without division
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;  // Default calibration
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;  // No PLL configuration
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        // If oscillator configuration fails, enter error handler
        Error_Handler();
    }

    // Configure CPU, AHB, APB1, APB2, and APB4 clock dividers
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2
                                 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;  // Use HSI as the system clock
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;  // No divider for the system clock
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;  // No divider for AHB bus
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;  // No divider for APB3 bus
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV8;  // Set APB1 divider to 8
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;  // No divider for APB2 bus
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;  // No divider for APB4 bus

    // Apply the clock configuration
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        // If clock configuration fails, enter error handler
        Error_Handler();
    }
}

// Timer Period Elapsed Callback - This function is called when the timer period expires
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    // Toggle the state of GPIO pin 12 (LED) on each timer interrupt
    HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_12);
}

// Error Handler Function
void Error_Handler(void)
{
    // Disable interrupts globally
    __disable_irq();
    // Stay in infinite loop to indicate an error state
    while (1)
    {
        // You can add code to signal the error condition (e.g., blink an LED)
    }
}
