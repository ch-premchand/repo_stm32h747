# Documentation for Main Program (`main.c`)

## Overview
The `main.c` file is part of an embedded application running on an STM32 microcontroller. The program is designed to handle the reading of analog values from an ADC (Analog-to-Digital Converter) input pin, convert those values into corresponding voltage levels, and then transmit this information over UART for external communication or display. The program runs in an infinite loop, periodically reading the analog data every 2 seconds and sending the results via UART.

## Goal
The primary goal of the application is to:
- Continuously read analog data from a specified ADC input pin.
- Convert the ADC readings into a voltage value based on the microcontroller's reference voltage.
- Transmit the voltage value over UART to be monitored or processed by an external system.

This process is achieved by setting up and configuring the ADC, UART, and GPIO peripherals, with the ADC regularly sampling the input voltage, and the results being communicated via UART.

## Objective
1. **System Initialization**:
    - Configure the system clock for optimal performance.
    - Initialize peripherals such as the UART (for communication), ADC (for analog-to-digital conversion), and GPIO (for managing input/output pins).
    - Ensure the system is ready to begin processing data.

2. **Analog-to-Digital Conversion**:
    - Trigger ADC conversions using interrupts.
    - Retrieve the converted ADC value and calculate the corresponding voltage.
    - Ensure accurate and reliable data conversion to maintain the integrity of the readings.

3. **UART Communication**:
    - Send the calculated voltage value over UART to an external system or display for real-time monitoring.
    - Use a formatted string to display the voltage value with appropriate units (e.g., "VOLT 3.25\r\n").

4. **Continuous Operation**:
    - Operate in an infinite loop that periodically reads the ADC, calculates the voltage, and transmits the data over UART.
    - The loop is controlled by a 2-second delay between each ADC conversion to avoid overloading the communication channel.

---

## 1. **Header Files**
- **main.h**: Contains necessary configurations for peripherals such as UART, ADC, and GPIO.
- **stdio.h**: Provides functionality for standard input and output, specifically for formatting data before transmitting it over UART.
- **string.h**: Provides functions for manipulating strings, used here for tasks like measuring the length of data to transmit.

---

## 2. **Global Variables**
- **hadc1**: A handle for the ADC peripheral, used for managing ADC operations.
- **huart1**: A handle for the UART peripheral, responsible for managing UART communication.
- **ADC_Val**: Stores the result of the ADC conversion, which is a raw digital value.
- **volt**: Holds the calculated voltage value corresponding to the ADC result.
- **UART1_rxBuffer**: A buffer used to hold the data (voltage) that will be transmitted via UART.

---

## 3. **Function Prototypes**
- **SystemClock_Config()**: Configures the system clock to ensure optimal performance of the microcontroller.
- **MX_GPIO_Init()**: Initializes the GPIO pins needed for the application.
- **MX_USART1_UART_Init()**: Initializes and configures the UART for communication.
- **MX_ADC1_Init()**: Configures the ADC for reading analog values from a specified input channel.

---

## 4. **Main Program Flow**

### System Initialization
The program starts by initializing the hardware abstraction layer (HAL) and configuring the system clock. The peripherals such as UART and ADC are initialized, and GPIO pins are set up as needed.

### UART Transmission
Once the system is initialized, a greeting message ("hi hello") is transmitted over UART to establish communication with the external device.

### ADC Operation
The program continuously starts ADC conversions every 2 seconds. The ADC is configured to use interrupt mode, and once a conversion is complete, an interrupt triggers a callback function to handle the result.

### Voltage Calculation
When the ADC conversion is complete, the result is retrieved, and the corresponding voltage value is calculated using the formula:
This formula converts the 16-bit ADC value into a voltage in the range of 0 to 3.3V, assuming a 3.3V reference voltage for the ADC.

### Data Transmission
The calculated voltage value is formatted into a string and transmitted over UART. This allows external monitoring systems to receive the voltage data in real-time.

---

## 5. **System Clock Configuration**
The system clock configuration ensures that the microcontroller runs at the required frequency and has stable power supply settings. The HSI (High-Speed Internal) oscillator is used as the system clock source, and the voltage scaling is set to ensure optimal power consumption.

---

## 6. **ADC Initialization**
The ADC is configured in single conversion mode with 16-bit resolution. ADC channel 1 is selected for reading the analog input, and the sampling time is set to an appropriate value to ensure accurate readings.

---

## 7. **USART1 Initialization**
The UART peripheral is initialized with a baud rate of 115200, 8 data bits, and no parity. This configuration ensures reliable communication between the microcontroller and the external system.

---

## 8. **GPIO Initialization**
The GPIO pins are initialized to support the necessary functions for communication and ADC sampling. This may include setting specific pins as input or output, enabling the clock for GPIO ports, and configuring any needed alternate functions.

---

## 9. **ADC Interrupt Callback**
When the ADC conversion completes, the interrupt service routine (ISR) is triggered. The `HAL_ADC_ConvCpltCallback` function is called, which retrieves the ADC result, calculates the corresponding voltage, and formats it into a string for transmission over UART.

---

## 10. **Error Handling**
In the event of a hardware or configuration error, the program enters an infinite loop to prevent further execution. This is managed by the `Error_Handler` function, which disables interrupts and halts the system to allow debugging.

---

## 11. **Assertion Handler**
The assertion failure handler is used for debugging purposes. If an assertion fails, this function provides information about the error to help the developer identify the issue. This function can be customized to report specific errors, such as logging the error to a terminal or storing it in memory for later analysis.

---

## Conclusion
This program is designed to continuously read an analog input using the ADC, convert the value to a voltage, and transmit this data over UART for monitoring. The process is performed in a loop with periodic ADC conversions and UART transmissions. The main functions of the program include system initialization, ADC reading, voltage calculation, and UART communication, all of which are handled efficiently to ensure reliable operation of the embedded system.
