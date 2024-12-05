# STM32 Low-Level Peripheral Initialization – Goal and Steps

This document outlines the goal and steps required to initialize the system, GPIO pins, and the HAL MSP (Microcontroller Support Package) on the STM32H7 microcontroller. The final goal is to configure the microcontroller for basic GPIO operation and ensure the MSP initialization for the system.

---

## Goal:

The final output of this code is:

- **GPIO Initialization**: The GPIO pins will be configured to control an LED (connected to GPIO pin `PI12`).
- **Button Input**: A button on pin `PC13` is monitored for state changes.
- **LED Control**: The LED toggles every time the button is pressed (active high) with a delay for debouncing.
- **Clock Configuration**: The system clock is configured to use the High-Speed Internal (HSI) oscillator.

---

## Steps for Peripheral Initialization

### 1. **HAL_Init** – HAL Initialization

**Goal**: Initialize the HAL library.

**Steps**:
- **Initialize the HAL Library**: The `HAL_Init()` function is called to initialize the HAL library. This function configures the Flash prefetch buffer, Flash latency, and the system tick timer (SysTick) for timekeeping.

---

### 2. **SystemClock_Config** – System Clock Configuration

**Goal**: Configure the system clock to ensure the microcontroller operates correctly.

**Steps**:
- **Configure the Power Supply**: The `HAL_PWREx_ConfigSupply()` function is called to select the appropriate power supply mode (`PWR_DIRECT_SMPS_SUPPLY`).
- **Set Voltage Scaling**: The voltage scaling for optimal power consumption is set using `__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1)`.
- **Initialize Oscillators**: The system clock is configured to use the HSI oscillator by setting `RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI`.
- **Configure Clocks for System and Peripherals**: The system clock and peripheral clocks are set up with `HAL_RCC_ClockConfig()` to use the HSI as the source with appropriate dividers for AHB, APB1, APB2, and other buses.

---

### 3. **MX_GPIO_Init** – GPIO Initialization

**Goal**: Configure the GPIO pins for LED control and button input.

**Steps**:
- **Enable GPIO Clocks**: The clocks for the GPIO ports are enabled with `__HAL_RCC_GPIOC_CLK_ENABLE()` for port `C` (for button input) and `__HAL_RCC_GPIOI_CLK_ENABLE()` for port `I` (for LED output).
- **Configure GPIO Pin for Button (PC13)**: Pin `PC13` is configured as an input pin with a pull-up resistor, which detects when the button is pressed (active low).
- **Configure GPIO Pin for LED (PI12)**: Pin `PI12` is configured as an output pin (push-pull) to control an external LED.

---

### 4. **Main Loop** – Button and LED Control

**Goal**: Continuously monitor the button and toggle the LED on button press.

**Steps**:
- **Button Press Detection**: The main loop checks the state of `PC13` (button pin). If the button is pressed (active high), the LED connected to `PI12` will toggle its state.
- **Debounce Logic**: A `HAL_Delay(1000)` is added to prevent the LED from toggling too rapidly due to button bouncing.

---

### 5. **HAL_MspInit** – MSP Initialization

**Goal**: Initialize the system-level MSP configuration.

**Steps**:
- **Enable SYSCFG Clock**: The clock for the SYSCFG peripheral is enabled using `__HAL_RCC_SYSCFG_CLK_ENABLE()`.
- **System Interrupts**: The system interrupts are initialized (though not explicitly defined in the code, this step can be configured in this section for advanced setups).

---

### 6. **Error_Handler** – Error Handling

**Goal**: Handle errors by entering an infinite loop.

**Steps**:
- **Enter Infinite Loop on Error**: The `Error_Handler()` function disables interrupts and enters an infinite loop if there is a failure in any peripheral initialization or configuration.

---

## Final Output Behavior

- **Button Press Detection**: When the button is pressed on `PC13`, the LED connected to `PI12` toggles every second. The LED toggling is debounced using a 1000 ms delay to ensure stable operation.
- **Clock Configuration**: The system uses the HSI oscillator as the clock source, providing reliable operation of the STM32H7 microcontroller.
- **GPIO Control**: The code allows basic GPIO control for monitoring input and controlling output based on the button state.

---

## Additional Information

### System Configuration and Clocks:
- **HSI Oscillator**: The High-Speed Internal (HSI) oscillator is used as the system clock source.
- **System Voltage Scaling**: The voltage scaling configuration is set to `PWR_REGULATOR_VOLTAGE_SCALE1` for optimal power usage.

### GPIO Configuration:
- **Button (PC13)**: Configured as an input with pull-up resistor, detecting the button state (active low).
- **LED (PI12)**: Configured as an output pin, controlling an external LED with a toggle operation in the main loop.

---

This code provides a simple but effective way to initialize and configure the STM32H7 microcontroller for GPIO operations, including handling a button press and toggling an LED in response.
