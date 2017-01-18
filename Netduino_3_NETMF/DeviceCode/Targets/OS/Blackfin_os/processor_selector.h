////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(PLATFORM_BLACKFIN_OS)
ERROR - WE SHOULD NOT INCLUDE THIS HEADER IF NOT BUILDING A BLACKFIN_OS PLATFORM
#endif

/////////////////////////////////////////////////////////
//
// processor and features
#ifndef _PLATFORM_BLACKFIN_OS_SELECTOR_H_
#define _PLATFORM_BLACKFIN_OS_SELECTOR_H_ 1

#define PLATFORM_BLACKFIN_DEFINED

/////////////////////////////////////////////////////////
//
// macros
//

#ifndef GLOBAL_LOCK
#define GLOBAL_LOCK(x)             SmartPtr_IRQ x
#define DISABLE_INTERRUPTS()       SmartPtr_IRQ::ForceDisabled()
#define ENABLE_INTERRUPTS()        SmartPtr_IRQ::ForceEnabled()
#define INTERRUPTS_ENABLED_STATE() SmartPtr_IRQ::GetState()
#endif

#ifndef GLOBAL_LOCK_SOCKETS
#define GLOBAL_LOCK_SOCKETS(x)     SmartPtr_IRQ x
#endif

#ifndef ASSERT_IRQ_MUST_BE_OFF
#if defined(_DEBUG)
#define ASSERT_IRQ_MUST_BE_OFF()   ASSERT(!SmartPtr_IRQ::GetState())
#define ASSERT_IRQ_MUST_BE_ON()    ASSERT( SmartPtr_IRQ::GetState())
#else
#define ASSERT_IRQ_MUST_BE_OFF()
#define ASSERT_IRQ_MUST_BE_ON()
#endif
#endif

void SystemState_SetNoLock  ( SYSTEM_STATE State );
void SystemState_ClearNoLock( SYSTEM_STATE State );
BOOL SystemState_QueryNoLock( SYSTEM_STATE State );


//
// macros
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// constants
//

#define SYSTEM_CYCLE_CLOCK_HZ       600000000

#ifndef TOTAL_USART_PORT
// Port definitions
#define TOTAL_USART_PORT       2
#define COM1                   ConvertCOM_ComHandle(0)
#define COM2                   ConvertCOM_ComHandle(1)

#define TOTAL_USB_CONTROLLER   1
#define USB1                   ConvertCOM_UsbHandle(0)

//TODO:
#define USB_IRQ_INDEX          0 

#define PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE    512  // there is one TX for each usart port
#define PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE    512  // there is one RX for each usart port
#define PLATFORM_DEPENDENT_USB_QUEUE_PACKET_COUNT  2    // there is one queue for each pipe of each endpoint and the size of a single packet is sizeof(USB_PACKET64) == 68 bytes

#define TOTAL_DEBUG_PORT       1
#define COM_DEBUG              ConvertCOM_DebugHandle(0)

#define COM_MESSAGING          ConvertCOM_MessagingHandle(0)

//TODO: 
#define USART_TX_IRQ_INDEX(x)       0
#define USART_DEFAULT_PORT          COM1
#define USART_DEFAULT_BAUDRATE      115200
#endif

#if !defined(USART_TX_XOFF_TIMEOUT_INFINITE)
#define USART_TX_XOFF_TIMEOUT_INFINITE   0xFFFFFFFF
#endif

// USART_TX_XOFF_TIMEOUT_TICKS = the number of ticks to leave TX in the XOFF state.  The 
// timeout is a failsafe so that if the XON is lost or an spurious XOFF is received the 
// TX won't be off indefinitely.
//
// The platform selector file should override this value, default to 1min
#if !defined(USART_TX_XOFF_TIMEOUT_TICKS) 
#define USART_TX_XOFF_TIMEOUT_TICKS      (CPU_TicksPerSecond() * 60)
#endif

//
// constants
//
/////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////
//
// macros
//    
#define UNUSED_GPIO_PULLDOWN(x) { (UINT8)BLACKFIN_GPIO::c_Port_##x, (UINT8)RESISTOR_PULLDOWN }
#define UNUSED_GPIO_PULLUP(x)   { (UINT8)BLACKFIN_GPIO::c_Port_##x, (UINT8)RESISTOR_PULLUP   }
#define UNUSED_GPIO_DISABLED(x) { (UINT8)BLACKFIN_GPIO::c_Port_##x, (UINT8)RESISTOR_DISABLED }
//
// macros
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// global functions
//

//
// global functions
//
/////////////////////////////////////////////////////////

#endif // _PLATFORM_ARM_BLACKFIN_OS_SELECTOR_H_ 1
//
