# STM32F746NUCLEO Solution

This is a sample solution for STMicroelectronics [NUCLEO-F746ZG](http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f746zg.html)
board.

## Features

* STM32F746ZGT6 Cortex-M7 @216 MHz (max), 1 MB Flash, 320 KB SRAM
* On-board ST-LINK/V2-1 debugger/programmer
  * USB Mass Storage device for drag'n'drop programming
  * USB Virtual COM port (USART3)
* Three user LEDs
* Two push buttons: USER (blue) and RESET (black)
* USB OTG, USART/UART, I2C, SPI, ADC, DAC, CAN, GPIO
* Ethernet 10/100 Mbps

## Configuration

The Nucleo-144 board has numerous solder bridges (SBxx/SB1xx on top/bottom layer),
which are used to configure I/O pinout and various onboard features. Factory default
settings are assumed as described in the User Manual section 6.12.

[STM32F746NUCLEO.ioc](STM32F746NUCLEO.ioc) is a baseline project file for [STM32CubeMX](http://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-configurators-and-code-generators/stm32cubemx.html)
software configuration tool that allows configuring the required set of peripherals
via a graphical interface, with automatic conflict detection and alternate pin mapping
selection.

> _Note: The project is provided mainly for pinout and clock tree configuration,
> middleware features and C code generation are not used, so there could be some
> values missing or not set properly._

> _Note: Certain peripheral configurations are not possible due to limitations
> of the NETMF or because they are not yet implemented in the STM32F7 port drivers._

The configuration options are set in [platform_selector.h](platform_selector.h)
via C++ language `#define` directives.

### System clock

The external high-speed clock (HSE) input PH0/OSC_IN is connected to ST-LINK MCO
output, which is fixed at 8 MHz. The value of the system clock frequency is limited
by the use of USB peripheral, which requires 48 MHz (±0.25%). Thus, when USB is
used the highest two allowed frequencies are 192 MHz and 216 MHz (max).

The peripheral bus clocks frequencies are configured to maximum values using
'/4' and '/2' prescaler for APB1 and ABP2 respectively.

### NETMF clock

The NETMF clock frequency is set to 1 MHz, which results in 1 µs resolution.

> _Note: This clock is also known as the 'Slow clock'. Not to be confused with
> the hardware real-time clock (RTC)._

## Diagnostics and Troubleshooting

Both debugging communication channels are configured to use the integrated
[Instrumentation Trace Macrocell (ITM)](http://infocenter.arm.com/help/topic/com.arm.doc.ddi0489d/BIICGJAF.html)
unit and generate output via ITM stimulus register 0.
```C++
#define DEBUG_TEXT_PORT                 ITM0
#define STDIO                           ITM0
```
The ITM output is delivered through the onboard ST-LINK/V2-1 debugger and can be
easily viewed in [STM32 ST-LINK utility](http://www.st.com/content/st_com/en/products/embedded-software/development-tool-software/stsw-link004.html)
application. For more detailed instructions and alternate transport configuration
please refer to [Diagnostics and Troubleshooting](https://github.com/NETMF/netmf-interpreter/wiki/Diagnostics-and-Troubleshooting)
topic on the NETMF wiki.

## Resources

* [NUCLEO-F746ZG - STMicroelectronics](http://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/mcu-eval-tools/stm32-mcu-eval-tools/stm32-mcu-nucleo/nucleo-f746zg.html)
  * [User Manual (PDF)](http://www.st.com/resource/en/user_manual/dm00244518.pdf)
  * [ST-LINK/V2-1 Firmware upgrade](http://www.st.com/content/st_com/en/products/embedded-software/development-tool-software/stsw-link007.html)
  * [ST-LINK/V2-1 USB driver for Windows](http://www.st.com/content/st_com/en/products/embedded-software/development-tool-software/stsw-link009.html)
* [NUCLEO-F746ZG - ARM mBed](https://developer.mbed.org/platforms/ST-Nucleo-F746ZG/)
  * Includes graphical board pinout


* [STM32F746ZG](http://www.st.com/content/st_com/en/products/microcontrollers/stm32-32-bit-arm-cortex-mcus/stm32f7-series/stm32f7x6/stm32f746zg.html?s_searchtype=partnumber)
  * [RM0385 - Reference Manual (PDF)](http://www.st.com/resource/en/reference_manual/dm00124865.pdf)
  * [PM0253 - Programming Manual (PDF)](http://www.st.com/resource/en/programming_manual/dm00237416.pdf)


* [LAN8742A Ethernet PHY](http://www.microchip.com/wwwproducts/en/LAN8742A)
  * [LAN8742A/LAN8742Ai Data Sheet (PDF)](http://ww1.microchip.com/downloads/en/DeviceDoc/DS_LAN8742_00001989A.pdf)

### Tools

* [STM32 ST-Link utility](http://www.st.com/content/st_com/en/products/embedded-software/development-tool-software/stsw-link004.html)
  * [UM0892 - STM32 ST-LINK utility software description (PDF)](http://www.st.com/resource/en/user_manual/cd00262073.pdf)
* [STM32CubeMX](http://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-configurators-and-code-generators/stm32cubemx.html)
