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

#ifndef _PLATFORM_<TEMPLATE>_SELECTOR_H_
#define _PLATFORM_<TEMPLATE>_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//

#if defined(PLATFORM_ARM_<TEMPLATE>)
#define HAL_SYSTEM_NAME                   "<TEMPLATE>"
#define PLATFORM_ARM_LPC22XX              1

//
// processor and features
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// constants
//

#define SYSTEM_CLOCK_HZ                 15000000 // Timer runs @CCLK/4
#define SYSTEM_CYCLE_CLOCK_HZ           60000000 // CCLK = 60MHz
#define CLOCK_COMMON_FACTOR             1000000
#define SLOW_CLOCKS_PER_SECOND          SYSTEM_CLOCK_HZ
#define SLOW_CLOCKS_TEN_MHZ_GCD         10000000
#define SLOW_CLOCKS_MILLISECOND_GCD     1000

#define SRAM1_MEMORY_Base               0x81000000
#define SRAM1_MEMORY_Size               (1*1024*1024)

#define FLASH_MEMORY_Base               0x80000000
#define FLASH_MEMORY_Size               (2*1024*1024)

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

#define DRIVER_GPIO_BUTTON_MAPPING                                           \
    { LPC22XX_GPIO::c_Pin_None, BUTTON_B0 }, /* Upper Left    - Backlight (spare n)   RP */ \
    { LPC22XX_GPIO::c_Pin_None, BUTTON_B1 }, /* Lower Left    - Channel               RP */ \
    { LPC22XX_GPIO::c_Pin_None, BUTTON_B2 }, /* Upper Right   - Up                    R  */ \
    { LPC22XX_GPIO::c_Pin_None, BUTTON_B3 },                                                \
    { LPC22XX_GPIO::c_Pin_None, BUTTON_B4 }, /* Middle Right  - Enter                    */ \
    { LPC22XX_GPIO::c_Pin_None, BUTTON_B5 }, /* Lower Right   - Down                   P */
    
#define INSTRUMENTATION_H_GPIO_PIN      0

#define DEBUG_TEXT_PORT         COM1
#define STDIO                   COM1
#define DEBUGGER_PORT           COM1
#define MESSAGING_PORT          COM1

//
// constants
/////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////
//
//  SOCKETS 
//


#define <TEMPLATE>_SOCKETS_ENABLED               1

#if defined(<TEMPLATE>_SOCKETS_ENABLED)

#define NETWORK_MEMORY_PROFILE__medium       1

#include <PAL\RTIP\Network_Defines.h>

#define NETWORK_INTERFACE_COUNT              1

#define NETWORK_USE_LOOPBACK                 1
#define NETWORK_USE_DHCP                     1

#endif //defined(<TEMPLATE>_SOCKETS_ENABLED)

//
// 
//
//////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////
//
// macros
//

#define GLOBAL_LOCK(x)             SmartPtr_IRQ x
#define DISABLE_INTERRUPTS()       SmartPtr_IRQ::ForceDisabled()
#define ENABLE_INTERRUPTS()        SmartPtr_IRQ::ForceEnabled()
#define INTERRUPTS_ENABLED_STATE() SmartPtr_IRQ::GetState()

#define GLOBAL_LOCK_SOCKETS(x)     SmartPtr_IRQ x


#if defined(_DEBUG)
#define ASSERT_IRQ_MUST_BE_OFF()   ASSERT(!SmartPtr_IRQ::GetState())
#define ASSERT_IRQ_MUST_BE_ON()    ASSERT( SmartPtr_IRQ::GetState())
#else
#define ASSERT_IRQ_MUST_BE_OFF()
#define ASSERT_IRQ_MUST_BE_ON()
#endif


//
// macros
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// communicaiton facilities
//

// Port definitions
#define TOTAL_USART_PORT       2
#define COM1                   ConvertCOM_ComHandle(0)
#define COM2                   ConvertCOM_ComHandle(1)

#define TOTAL_USB_CONTROLLER   1
#define USB1                   ConvertCOM_UsbHandle(0)

#define TOTAL_SOCK_PORT        0

#define TOTAL_DEBUG_PORT       2
#define COM_DEBUG_RPC          ConvertCOM_DebugHandle(0)
#define COM_DEBUG              ConvertCOM_DebugHandle(1)

#define USART_TX_IRQ_INDEX(x)       ( (x) ? 7 : 6 )
#define USART_DEFAULT_PORT          COM1
#define USART_DEFAULT_BAUDRATE      115200

#define USB_IRQ_INDEX               0  // TODO set right index

#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    512   // there is one TX for each usart port

// Has to set the Rx Buffer large enough base on the data reading time, if the Rx Buffer is not large enough, it will hit Rx Buffer Overflow and the communciation will be out of sync.
// the 2048 size is good for both thumb and arm code operaiton and the longer flash wait state.
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    2048  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT  2    // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes
//
// communicaiton facilities
/////////////////////////////////////////////////////////

#include <processor_selector.h>


#endif // PLATFORM_ARM_<TEMPLATE>
//
// drivers
/////////////////////////////////////////////////////////

#endif // _PLATFORM_<TEMPLATE>_SELECTOR_H_

