# STM32 USART Program: Transmit String Over UART

## Goal:

The goal of this program is to demonstrate how to send a string over UART using the STM32 microcontroller. Specifically, the program sends the message `"prem\r\n"` over USART1, using interrupt-based transmission. The message is formatted to include a carriage return and a newline at the end.

---

## Steps for Peripheral Initialization:

### 1. **HAL_Init** – HAL Initialization

**Goal**: Initializes the Hardware Abstraction Layer (HAL), which is essential for setting up the microcontroller’s peripherals and system-level operations.

**Steps**:
- **Initialize HAL**: The `HAL_Init()` function sets up the basic HAL infrastructure, which includes configuring system services, peripherals, and the interrupt controller.
- **System Tick Configuration**: The `HAL_Init()` function also configures the SysTick timer to generate time delays and to keep track of time for the system.

---

### 2. **SystemClock_Config** – System Clock Configuration

**Goal**: Configures the system clock to ensure the microcontroller runs at the correct speed and power settings.

**Steps**:
- **Power Configuration**: The `HAL_PWREx_ConfigSupply()` function is called to configure the power supply for direct SMPS (Switch Mode Power Supply).
- **Voltage Scaling**: The voltage scaling is set to level 1 (`PWR_REGULATOR_VOLTAGE_SCALE1`) for better performance.
- **Clock Configuration**: The High-Speed Internal (HSI) oscillator is used as the clock source. The system clock (SYSCLK) and other peripheral clock dividers are configured to distribute the clock to the microcontroller’s subsystems.
- **Error Handling**: If any error occurs during the configuration, the `Error_Handler()` function is called to halt execution.

---

### 3. **MX_USART1_Init** – USART1 Initialization

**Goal**: Initializes the USART1 peripheral to allow for serial communication via UART.

**Steps**:
- **Configure USART Parameters**: The program sets up USART1 with the following configurations:
  - **Baud Rate**: 115200 bps
  - **Word Length**: 8 bits per data frame
  - **Stop Bits**: 1 stop bit
  - **Parity**: None
  - **Mode**: Transmission and reception enabled (TX and RX)
  - **Hardware Flow Control**: None (no RTS/CTS)
- **Enable USART**: After configuring the settings, the `HAL_UART_Init()` function is called to initialize the UART peripheral.

---

### 4. **Main Loop and UART Transmission**

**Goal**: Implement the main program loop that transmits data over UART.

**Steps**:
- **String Declaration**: A string `"prem\r\n"` is declared to be transmitted via UART. The carriage return (`\r`) and newline (`\n`) characters are added for formatting.
- **Transmit Data**: The program uses `HAL_UART_Transmit_IT()` for asynchronous transmission of the data string. The transmission is done in interrupt mode, meaning the MCU continues executing other tasks while the transmission is handled in the background.
- **Delay**: A small delay (`HAL_Delay(1000)`) is added after initiating the transmission to allow the UART interrupt to complete before the program enters the infinite loop.

---

### 5. **Error Handler**

**Goal**: Handle errors and stop the program if something goes wrong during the initialization or operation.

**Steps**:
- **Disable Interrupts**: The `__disable_irq()` function disables interrupts globally, preventing further interrupts from occurring.
- **Infinite Loop**: The `while(1)` loop halts the program in case of any error, effectively freezing the MCU.

---