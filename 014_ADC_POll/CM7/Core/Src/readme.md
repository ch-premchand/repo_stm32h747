# STM32 ADC to UART Program – Goal and Steps

This document outlines the goal and steps required to initialize the ADC, UART, and system configuration for reading an analog value from a sensor, converting it to a digital value, and transmitting it via UART on an STM32 microcontroller.

---

## Goal:

The final output of this code is:

- **ADC Conversion**: The program reads an analog voltage from an input pin (ADC channel 1), converts it into a digital value using the ADC, and calculates the corresponding voltage.
- **UART Communication**: The calculated voltage is transmitted via UART to an external device (e.g., a PC or another microcontroller).
- **Voltage Output**: The voltage is output as a formatted string, displaying the measured voltage with two decimal places.

---

## Steps for Peripheral Initialization

### 1. **HAL_Init** – HAL Initialization

**Goal**: Initialize the HAL library to set up the basic system infrastructure.

**Steps**:
- **Initialize the HAL Library**: The `HAL_Init()` function is called to initialize the HAL library, which sets up the microcontroller system for peripherals and system-level operations.
- **Configure SysTick Timer**: This function also configures the SysTick timer for delay functions and timekeeping.

---

### 2. **SystemClock_Config** – System Clock Configuration

**Goal**: Configure the system clock to ensure the microcontroller operates correctly and efficiently.

**Steps**:
- **Configure the Power Supply**: The `__HAL_RCC_PWR_CLK_ENABLE()` function is called to enable the power interface clock, and `__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1)` sets the voltage scaling for optimal performance and power consumption.
- **Enable and Configure the PLL**: The system clock is derived from the PLL, using the High-Speed Internal (HSI) oscillator as the source. The PLL configuration includes setting the PLL multiplier, divider, and the output frequency parameters.
- **Set Clock Dividers**: The clock dividers for AHB, APB1, and APB2 are configured to distribute the clock signal to different parts of the microcontroller, ensuring proper operation of all peripherals.
- **Configure the MCO (Microcontroller Clock Output)**: The clock output is routed to pin `PC9`, enabling real-time monitoring of the clock on an external device.

---

### 3. **MX_GPIO_Init** – GPIO Initialization

**Goal**: Configure the GPIO pins for ADC input and UART communication.

**Steps**:
- **Enable GPIO Clocks**: The clocks for GPIO ports are enabled with `__HAL_RCC_GPIOA_CLK_ENABLE()` for port A (ADC input) and `__HAL_RCC_GPIOB_CLK_ENABLE()` for port B (UART communication).
- **Configure GPIO Pin for ADC Input**: Pin `PA1` is configured as an analog input to read the voltage from a sensor. No pull-up or pull-down resistors are used.
- **Configure GPIO Pin for UART TX**: Pin `PB6` is configured as an alternate function (AF) push-pull output for UART transmission. Pin `PB7` is configured for UART RX (receive).

---

### 4. **MX_ADC1_Init** – ADC Initialization

**Goal**: Initialize the ADC for analog-to-digital conversion.

**Steps**:
- **Configure ADC Channel**: The ADC is initialized for single-channel conversion (channel 1), which corresponds to the input pin `PA1`.
- **Set ADC Resolution and Sampling Time**: The ADC resolution is set to 12 bits, and the sampling time is adjusted for optimal accuracy.
- **Enable ADC**: The ADC is enabled and ready to start conversions upon command.

---

### 5. **MX_USART1_UART_Init** – UART Initialization

**Goal**: Configure UART for serial communication.

**Steps**:
- **Set UART Parameters**: The UART1 peripheral is configured with the following parameters:
  - Baud rate: 115200
  - Word length: 8 bits
  - Parity: None
  - Stop bits: 1
  - Flow control: None
- **Enable UART**: The UART is enabled for transmission and reception.

---

### 6. **Main Loop** – ADC Reading and UART Transmission

**Goal**: Implement the main program loop for ADC readings and UART communication.

**Steps**:
- **Start ADC Conversion**: The program starts the ADC conversion using `HAL_ADC_Start()`.
- **Poll for Conversion Completion**: The program waits for the conversion to complete using `HAL_ADC_PollForConversion()`.
- **Read ADC Value**: The raw ADC value is retrieved using `HAL_ADC_GetValue()`.
- **Convert ADC Value to Voltage**: The ADC value is scaled to a voltage using the formula:
  ```c
  volt = (ADC_Val * 3.3) / 65535;
