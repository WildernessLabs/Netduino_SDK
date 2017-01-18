//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#ifndef _PLATFORM_EA_LPC2478_SELECTOR_H_
#define _PLATFORM_EA_LPC2478_SELECTOR_H_ 1


#if defined(PLATFORM_ARM_EA_LPC2478)
#define HAL_SYSTEM_NAME                             "EA_LPC2478"
#define PLATFORM_ARM_LPC24XX                        1

#if !defined(HAL_REDUCESIZE)
#define EA_LPC2478_SOCKETS_ENABLED                  1
#endif

#if defined(EA_LPC2478_SOCKETS_ENABLED)

#define NETWORK_INTERFACE_COUNT                     1

#define PLATFORM_DEPENDENT__SOCKETS_MAX_SEND_LENGTH 8192

#define NETWORK_MEMORY_PROFILE__medium              1

#define NETWORK_MEMORY_POOL__INCLUDE_SSL            1
#if defined(TCPIP_LWIP)
#include <pal\net\Network_defines_lwip.h>
#else
#include <pal\net\Network_Defines.h>
#endif

#define NETWORK_USE_LOOPBACK                        1
#define NETWORK_USE_DHCP                            1

//--//

#define PLATFORM_DEPENDENT__NETWORK_DNS_MIN_DELAY  4 
#define PLATFORM_DEPENDENT__NETWORK_DNS_MAX_DELAY  32
#define PLATFORM_DEPENDENT__NETWORK_DNS_RETRIES    3

#endif

//#define PLATFORM_ARM_EA_LPC2478
#define PLATFORM_SUPPORTS_SOFT_REBOOT   TRUE
#define SYSTEM_CLOCK_HZ                 16800000
#define SLOW_CLOCKS_PER_SECOND          16800000
#define CLOCK_COMMON_FACTOR             200000 // old value: 1000000
#define SLOW_CLOCKS_TEN_MHZ_GCD         400000 //// old value: 10000000
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

// manually filled in
// 12 MHz Crystal, cclk = 67.2 MHz, usb_clk = 48 MHz
// pll_out         = (12MHz * (2 * PLL_MVAL) / PLL_NVAL)
// cclk            = pll_out / CCLK_DIVIDER
// usb_clk         = pll_out / USB_DIVIDER 
// SYSTEM_CLOCK_HZ = cclk / 4 (default divider for perhipherals - timer)
#define PLL_MVAL                        14
#define PLL_NVAL                        1
#define CCLK_DIVIDER                    5
#define USB_DIVIDER                     7

#define SYSTEM_CYCLE_CLOCK_HZ           SYSTEM_CLOCK_HZ * 4
#define LPC24XX_TIMER_PCLK              SYSTEM_CLOCK_HZ
#define LPC24XX_UART_PCLK               SYSTEM_CLOCK_HZ
#define LPC24XX_SPI_PCLK_KHZ            SYSTEM_CLOCK_HZ/1000
#define LPC24XX_I2C_PCLK_KHZ            SYSTEM_CLOCK_HZ/1000
#define LPC24XX_DAC_PCLK                SYSTEM_CYCLE_CLOCK_HZ

//external SDRAM, 32MB
#define SRAM1_MEMORY_Base   0xA0000000
#define SRAM1_MEMORY_Size   0x04000000
//external SST39VF320_16 NOR Flash, 4MB
#define FLASH_MEMORY_Base   0x80000000
#define FLASH_MEMORY_Size    0x00400000

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED


#define GLOBAL_LOCK(x)                  SmartPtr_IRQ x
#define DISABLE_INTERRUPTS()            SmartPtr_IRQ::ForceDisabled()
#define ENABLE_INTERRUPTS()             SmartPtr_IRQ::ForceEnabled()
#define INTERRUPTS_ENABLED_STATE()      SmartPtr_IRQ::GetState()
#define GLOBAL_LOCK_SOCKETS(x)          SmartPtr_IRQ x

#if defined(_DEBUG)
#define ASSERT_IRQ_MUST_BE_OFF()        ASSERT(!SmartPtr_IRQ::GetState())
#define ASSERT_IRQ_MUST_BE_ON()         ASSERT( SmartPtr_IRQ::GetState())
#else
#define ASSERT_IRQ_MUST_BE_OFF()
#define ASSERT_IRQ_MUST_BE_ON()
#endif


#define TOTAL_USART_PORT                3
#define COM1                            ConvertCOM_ComHandle(0) // UART 0
#define COM2                            ConvertCOM_ComHandle(1) // UART 1
#define COM3                            ConvertCOM_ComHandle(2) // UART 2

#define TOTAL_USB_CONTROLLER            0
#define USB1                            ConvertCOM_UsbHandle(0)

//#define TOTAL_SOCK_PORT               0

#define TOTAL_DEBUG_PORT                1
#define COM_DEBUG                       ConvertCOM_DebugHandle(0)

#define COM_MESSAGING                   ConvertCOM_MessagingHandle(0)

#define USART_TX_IRQ_INDEX(x)           ( LPC24XX_USART::getIntNo(x) )
#define USART_DEFAULT_PORT              COM3
#define USART_DEFAULT_BAUDRATE          115200

#define USB_IRQ_INDEX                   0                   // TODO set right index


#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    1024  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    4096  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT  2  // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes

#define DEBUG_ON_UART

//For UART#2 debug
#if defined(DEBUG_ON_UART)
    #define DEBUG_TEXT_PORT    COM3
    #define STDIO              COM3
    #define DEBUGGER_PORT      COM3
    #define MESSAGING_PORT     COM3
#else
//For ethernet debug
    #define DEBUG_TEXT_PORT    COM_SOCKET_DBG
    #define STDIO              COM_SOCKET_DBG
    #define DEBUGGER_PORT      COM_SOCKET_DBG
    #define MESSAGING_PORT     COM_SOCKET_DBG
#endif

//#define RUNTIME_MEMORY_PROFILE__medium 1
#include <processor_selector.h>

#endif // PLATFORM_ARM_EA_LPC2478


#endif // _PLATFORM_EA_LPC2478_SELECTOR_H_ 1
