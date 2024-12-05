# STM32 Low-Level Peripheral Initialization – Goal and Steps

This document outlines the goal and steps required to initialize various peripherals, such as system clock, UART, GPIO, and Timer, on the STM32H7 microcontroller. The final goal is to configure the microcontroller to transmit a "HELLO" message via UART, toggle two GPIO pins (connected to LEDs) at regular intervals using Timer 7, and handle errors efficiently.

---

## Goal:

The final output of this code is:

- **UART Communication**: The message "HELLO" will be transmitted via USART1.
- **LED Control**: Two LEDs (connected to GPIO pins GPIOJ_PIN_0 and GPIOI_PIN_12) will toggle at regular intervals based on the Timer 7 interrupt.
- **Timer-based Interrupts**: Timer 7 generates interrupts, triggering the GPIO toggling at defined intervals.
- **Error Handling**: If any peripheral initialization fails, the system will enter an infinite loop for error handling.

---

## Steps for Peripheral Initialization

### 1. `HAL_MspInit` – Processor-Specific Initialization

**Goal**: Configure interrupt priorities and enable system exceptions.

**Steps**:
- **Set NVIC Priority Grouping**: Configure the Cortex-Mx processor's priority grouping to 4 bits for better interrupt management.
- **Enable System Exceptions**: Enable critical system exceptions like Usage Fault, Memory Fault, and Bus Fault by modifying the `SCB->SHCSR` register.
- **Set Interrupt Priorities**: Assign high priority (0) to exceptions like `MemoryManagement_IRQn`, `BusFault_IRQn`, and `UsageFault_IRQn` to ensure proper system error handling.

---

### 2. `HAL_UART_MspInit` – UART Peripheral Initialization

**Goal**: Configure and initialize USART1 for serial communication.

**Steps**:
- **Enable Clocks for USART1 and GPIOA**: Enable clocks for the USART1 peripheral and the GPIOA port where USART1 TX (PA9) and RX (PA10) pins are located.
- **Configure GPIO Pins for USART1**: Set PA9 and PA10 as alternate function push-pull, with a pull-up resistor to ensure correct signal levels for transmission and reception.
- **Enable USART1 Interrupt**: Enable the interrupt for USART1, and set its priority to the lowest level to handle communication-related interrupts.

---

### 3. `HAL_TIM_Base_MspInit` – Timer 7 Initialization

**Goal**: Initialize Timer 7 to generate periodic interrupts for controlling the timing of GPIO pin toggling.

**Steps**:
- **Enable Timer 7 Clock**: Enable the clock for Timer 7 so that it can generate the necessary timing events.
- **Enable Timer 7 Interrupt**: Enable the interrupt for Timer 7 and set its priority to ensure proper handling of periodic events.

---

### 4. `USART1_Init` – USART1 Configuration

**Goal**: Set up USART1 for serial communication at a baud rate of 115200.

**Steps**:
- **Configure USART1 Parameters**: Set the baud rate to 115200, data word length to 8 bits, one stop bit, and no parity.
- **Set UART Mode**: Configure USART1 to work in both transmission (TX) and reception (RX) modes.
- **Initialize USART1**: Apply the configuration using `HAL_UART_Init()`. If initialization fails, call the `Error_Handler()` function.

---

### 5. `GPIO_Init` – GPIO Pin Initialization

**Goal**: Configure GPIO pins for controlling external LEDs.

**Steps**:
- **Enable Clocks for GPIO Ports**: Enable the clocks for GPIOI and GPIOJ, which control the LEDs connected to pins GPIOJ_PIN_0 and GPIOI_PIN_12.
- **Configure GPIO Pins for LEDs**: Set the GPIO pins as output push-pull with no pull-up resistors and low speed.
- **Initialize GPIO Pins**: Use `HAL_GPIO_Init()` to apply the configurations to both GPIOJ_PIN_0 and GPIOI_PIN_12.

---

### 6. `TIM7_Init` – Timer 7 Initialization

**Goal**: Set up Timer 7 to generate periodic interrupts at a defined interval to control LED toggling.

**Steps**:
- **Configure Timer 7**: Set the prescaler to 24 and the period to 64000 - 1 to achieve a desired timing interval.
- **Initialize Timer 7**: Initialize Timer 7 with the configured prescaler and period using `HAL_TIM_Base_Init()`. If initialization fails, the `Error_Handler()` function is called.

---

### 7. `SystemClock_Config` – System Clock Configuration

**Goal**: Configure the system clock to ensure the microcontroller operates correctly.

**Steps**:
- **Configure Power Supply**: Use `HAL_PWREx_ConfigSupply()` to select the appropriate power supply mode.
- **Set Voltage Scaling**: Set the voltage scaling for optimal power consumption using `__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1)`.
- **Initialize Oscillators**: Set up the HSI (High-Speed Internal) oscillator as the system clock source.
- **Configure Clock Dividers**: Set up clock dividers for AHB, APB1, and APB2 buses to achieve the desired frequencies for the CPU and peripherals.
- **Error Handling**: If the system clock configuration fails, call the `Error_Handler()` function.

---

## Final Output Behavior

- **UART Communication**: The "HELLO" message is transmitted via USART1 immediately after initialization.
- **LED Control**: The two LEDs toggle at regular intervals, defined by Timer 7. This is done in the main loop after Timer 7 is started.
- **Timer Interrupt Handling**: Timer 7 periodically generates an interrupt, causing the GPIO pins to toggle and control the LEDs.
- **Error Handling**: If any peripheral initialization fails, the system enters an infinite loop in the `Error_Handler()` function, preventing further execution.

---
