
@  STM32 Peripheral Clock Configuration for ADC3
This document outlines the steps required to enable the clock for the ADC3 peripheral and set a bit in its control register. It provides the necessary details on how to interact with the STM32's clock control and ADC3 control registers based on the provided code.
------------------------------------------------------------------------------------------------------


1. Know which bus the ADC3 is operating on (AHB4)
The ADC3 peripheral in STM32 operates on the AHB4 bus. In STM32 microcontrollers, ADC3 is connected to this bus, meaning the clock for ADC3 is controlled through the AHB4 clock control register.

2. Know the Base Address of RCC (Reset and Clock Control)
The RCC (Reset and Clock Control) base address is part of the memory map of the STM32 microcontroller. It is crucial for enabling clocks for various peripherals, including ADC3.

Base address of RCC: 0x58024400UL (in the code example for STM32H7)
This address is critical because you need to find the AHB4 control register for clock configuration.

3. Know the Offset Address of AHB4 Clock Control Register
The AHB4 clock control register is responsible for controlling the clock for peripherals connected to the AHB4 bus, such as ADC3.

Offset of AHB4 Clock Control Register: 0x140UL (for STM32H7)
To enable the clock for ADC3, you will need to access this register and set the specific bit responsible for ADC3. In the code, this is done by manipulating bit 24 of the RCC_C1 register.

4. Know the Base Address of ADC3
The base address of ADC3 is part of the memory map of the STM32 microcontroller, and it is used to access the ADC3 registers.

Base address of ADC3: 0x58026000UL (for STM32H7)
This address is essential because it allows access to the ADC3 control registers, which are used to configure and control the ADC operations.

5. Know the Offset Address of ADC3 Control Register
The control register of ADC3 is responsible for setting various operational parameters, such as the ADC resolution, conversion mode, and other settings.

Offset of ADC3 Control Register: 0x08UL (in the provided code example, this offset is used for ADC CR1 register)
The ADC_CR1 register is used to configure the ADC's operation. In the code, bit 4 of the CR1 register is set.