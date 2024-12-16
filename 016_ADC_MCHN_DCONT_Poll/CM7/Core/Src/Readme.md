
markdown
Copy code
# STM32 ADC and UART Communication: Detailed Walkthrough

## **Goal**

The main goal of this code is to:
1. **Configure the STM32 microcontroller's ADC (Analog-to-Digital Converter)** to read two analog inputs.
2. **Transmit the ADC readings** over UART (Universal Asynchronous Receiver Transmitter) for debugging or data monitoring.

## **Objective**

The specific objectives of this program are:
- **Initialize the ADC** in multi-channel mode to read analog signals.
- **Transmit ADC results** via UART to a terminal (or debugging tool).
- **Periodically poll and fetch ADC values** from two different channels: `ADC_CHANNEL_18` and `ADC_CHANNEL_1`.
- **Transmit the results over UART** every 2 seconds for monitoring.

## **Key Components**
- **ADC1 (Analog-to-Digital Converter)**: Used for reading the analog signals from the input pins and converting them to digital values.
- **UART1 (Universal Asynchronous Receiver Transmitter)**: Used for communication between the STM32 and external devices like a terminal or PC for debugging or data monitoring.
- **GPIO**: Configured for basic functionality such as enabling the UART and ADC peripherals.

## **Walkthrough**

### 1. **System Initialization**
   - The microcontroller is initialized by calling `HAL_Init()`. This function resets peripherals, configures the Flash interface, and sets up the SysTick timer for basic timing.
   - The system clock is configured using the `SystemClock_Config()` function, ensuring the microcontroller operates at the correct frequency.

### 2. **ADC Configuration**
   - The ADC is configured in scan mode, which allows reading multiple channels sequentially.
   - The channels configured are `ADC_CHANNEL_18` and `ADC_CHANNEL_1`. 
   - The configuration also includes enabling a discontinuous mode to read only a specified number of channels (2 in this case). The ADC is set to poll for conversion completion, and the results are stored in 16-bit resolution.

### 3. **UART Initialization**
   - UART is initialized to communicate with external devices. The configuration includes setting the baud rate to `115200`, no parity, and 8-bit word length.
   - The UART will transmit the results of the ADC conversions through `HAL_UART_Transmit()`.

### 4. **Main Loop**
   - The main loop performs ADC readings every 2 seconds for the two configured channels.
   - For each channel, the code:
     1. **Starts the ADC conversion**.
     2. **Waits for the conversion to complete**.
     3. **Retrieves the ADC value**.
     4. **Formats the result into a string** and sends it via UART.
   - A delay of 2 seconds (`HAL_Delay(2000)`) is introduced before the next set of conversions to avoid overloading the communication channel.

### 5. **Transmission of Data**
   - The ADC values are transmitted via UART. The `sprintf()` function is used to format the ADC readings into a human-readable string, which is then transmitted using `HAL_UART_Transmit()`.
   - The data sent includes the ADC readings from both channels, clearly indicating which channel the reading corresponds to.

### 6. **Error Handling**
   - The `Error_Handler()` function is used to handle any issues encountered during the setup or execution of the program. If any configuration or operation fails (like ADC or UART initialization), the program enters an infinite loop to signal an error.

## **ADC Initialization Configuration**

The ADC (Analog-to-Digital Converter) initialization is a crucial step for configuring the ADC to read analog values from multiple input channels. Below are the key points explaining the ADC configuration:

1. **Scan Conversion Mode**: 
   - The ADC is configured in **scan conversion mode** which allows reading multiple channels sequentially.
   - This mode is essential when there is a need to read from more than one input pin (e.g., `ADC_CHANNEL_18` and `ADC_CHANNEL_1`).

2. **End of Conversion (EOC) Selection**: 
   - The ADC is set to trigger the end of conversion interrupt after a single conversion for each channel. This is configured using `ADC_EOC_SINGLE_CONV`.

3. **Low Power Auto-Wait**: 
   - This option is **disabled**. Disabling this ensures that the ADC keeps functioning and doesn't wait automatically for a new conversion when in low power mode.

4. **Continuous Conversion Mode**:
   - The ADC is configured to **not operate in continuous mode**. This means it will perform conversions only once per trigger.

5. **Discontinuous Conversion Mode**:
   - The ADC is configured for **discontinuous conversion mode**, which means that after each conversion, it will stop and wait before starting the next one.

6. **Number of Channels (Conversions)**: 
   - The number of conversions is set to **2** since two channels (e.g., `ADC_CHANNEL_18` and `ADC_CHANNEL_1`) are configured.

7. **External Trigger Configuration**:
   - The ADC is set to be triggered **manually via software**, meaning the conversion process starts explicitly through software commands rather than an external event.

8. **Resolution**:
   - The ADC is configured to use **16-bit resolution**, meaning the ADC will return values ranging from 0 to 65535, providing a fine level of detail for analog readings.

9. **Oversampling Mode**:
   - The **oversampling mode is disabled**, which means that no additional sampling is performed to increase resolution.

10. **Multi-Mode Configuration**:
    - The ADC is set to work in **independent mode**, meaning the channels operate independently rather than in a linked configuration.

11. **Channel Configuration**:
    - Two channels are configured: 
      - **ADC_CHANNEL_18**: Set as the first channel with a specific sampling time.
      - **ADC_CHANNEL_1**: Set as the second channel with its own configuration.

12. **Sampling Time**:
    - The **sampling time** for each channel is configured to **1.5 ADC clock cycles** (`ADC_SAMPLETIME_1CYCLE_5`), determining how long the ADC will sample each input signal.

This configuration ensures that the ADC operates efficiently and accurately, allowing the system to read and process analog signals from multiple channels in a sequential manner.
