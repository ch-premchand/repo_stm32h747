#include <stm32h7xx_hal.h>
#include <string.h>

// Function Prototypes
void TIM7_Init(void);
void SystemClock_Config(void);
void Error_Handler(void);
void USART1_Init(void);
void GPIO_Init(void);

// Timer and UART handle declarations
TIM_HandleTypeDef HTim7;
UART_HandleTypeDef huart1;

int main(){

    HAL_Init();  // Initialize the HAL library, this sets up the system clock and other necessary peripherals
    SystemClock_Config();  // Configure system clock settings

    GPIO_Init();  // Initialize GPIO for LED control
    TIM7_Init();  // Initialize Timer 7 for timing operations
    USART1_Init();  // Initialize USART1 for serial communication

    HAL_UART_Transmit(&huart1,(uint8_t *)"HELLO", 5,HAL_MAX_DELAY);  // Send "HELLO" string via UART1
    HAL_TIM_Base_Start(&HTim7);  // Start Timer 7 for periodic interrupts

    while(1){
        // Wait for the update interrupt flag (UIF) to indicate the timer has overflowed
        while(! (TIM7->SR & TIM_SR_UIF));
            HAL_GPIO_TogglePin(GPIOJ, GPIO_PIN_0);  // Toggle LED on GPIOJ pin 0
            HAL_GPIO_TogglePin(GPIOI, GPIO_PIN_12); // Toggle LED on GPIOI pin 12
            TIM7->SR &= ~(TIM_SR_UIF);  // Clear the update interrupt flag (UIF) to reset timer overflow
    }

    return 0;
}

// USART1 Initialization Function
void USART1_Init(void){

    huart1.Instance = USART1;  // Set USART1 instance
    huart1.Init.BaudRate = 115200;  // Set baud rate for USART1
    huart1.Init.WordLength = UART_WORDLENGTH_8B;  // Set 8-bit data word length
    huart1.Init.StopBits = UART_STOPBITS_1;  // Set 1 stop bit
    huart1.Init.Parity = UART_PARITY_NONE;  // No parity
    huart1.Init.Mode = UART_MODE_TX_RX;  // Enable both transmit and receive modes
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;  // Disable hardware flow control

    // Initialize USART1 peripheral with the specified configuration
    if (HAL_UART_Init(&huart1) != HAL_OK){
        Error_Handler();  // If initialization fails, enter error handling
    }
}

// GPIO Initialization Function
void GPIO_Init(){

    // Enable clocks for GPIOI and GPIOJ
    __HAL_RCC_C1_GPIOI_CLK_ENABLE();
    __HAL_RCC_C1_GPIOJ_CLK_ENABLE();

    GPIO_InitTypeDef ledgpio;

    // Initialize GPIOJ Pin 0 as output (for controlling LED)
    ledgpio.Pin = GPIO_PIN_0;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull mode
    ledgpio.Pull = GPIO_NOPULL;  // No pull-up or pull-down resistors
    HAL_GPIO_Init(GPIOJ, &ledgpio);  // Apply the GPIO configuration

    // Initialize GPIOI Pin 12 as output (for controlling another LED)
    ledgpio.Pin = GPIO_PIN_12;
    ledgpio.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull mode
    ledgpio.Pull = GPIO_NOPULL;  // No pull-up or pull-down resistors
    HAL_GPIO_Init(GPIOI, &ledgpio);  // Apply the GPIO configuration
}

// Timer 7 Initialization Function
void TIM7_Init(void){

    HTim7.Instance = TIM7;  // Set the TIM7 instance
    HTim7.Init.Prescaler = 24;  // Set prescaler to 24 (clock division factor)
    HTim7.Init.Period = 64000 - 1;  // Set period to 64000-1 (timer overflow count)
    //100ms
    // Initialize Timer 7 with the specified configuration
    if(HAL_TIM_Base_Init(&HTim7) != HAL_OK){
        Error_Handler();  // If initialization fails, enter error handling
    }
}

// System Clock Configuration Function
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};  // Declare RCC Oscillator configuration struct
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};  // Declare RCC Clock configuration struct

    /** Supply configuration update enable **/
    HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);  // Configure the power supply type

    /** Configure the main internal regulator output voltage **/
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  // Set voltage scaling to scale 1 for performance

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}  // Wait for voltage scaling to be ready

    /** Initializes the RCC Oscillators **/
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;  // Use HSI (High-Speed Internal) oscillator
    RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;  // Set HSI divider to 1 (no division)
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;  // Use default calibration for HSI
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;  // Disable PLL (Phase-Locked Loop)

    // Apply oscillator configuration
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
        Error_Handler();  // If oscillator configuration fails, enter error handling
    }

    /** Initializes the CPU, AHB and APB buses clocks **/
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                  RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;  // Use HSI as system clock source
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;  // No division of SYSCLK
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;  // No division of AHB clock
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;  // No division of APB3 clock
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV8;  // Set APB1 clock division to 8
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;  // No division of APB2 clock
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;  // No division of APB4 clock

    // Apply clock configuration
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK){
        Error_Handler();  // If clock configuration fails, enter error handling
    }
}

// Error Handler Function
void Error_Handler(void)
{
    __disable_irq();  // Disable all interrupts
    while (1)
    {
        // Infinite loop for error handling, usually used for debugging or system failure cases
    }
}
