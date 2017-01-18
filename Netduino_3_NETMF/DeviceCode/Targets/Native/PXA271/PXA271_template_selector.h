////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLATFORM_<TEMPLATE>_SELECTOR_H_
#define _PLATFORM_<TEMPLATE>_SELECTOR_H_ 1

/////////////////////////////////////////////////////////
//
// processor and features
//
#if defined(PLATFORM_ARM_<TEMPLATE>)
#define HAL_SYSTEM_NAME                     "<TEMPLATE>"

#define PLATFORM_ARM_PXA271                     1
//
// processor and features
//
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
//
// constants
//
#define SYSTEM_CLOCK_HZ                 3250000
#define CLOCK_COMMON_FACTOR               10000
#define SLOW_CLOCKS_PER_SECOND          3250000
#define SLOW_CLOCKS_TEN_MHZ_GCD           10000
#define SLOW_CLOCKS_MILLISECOND_GCD        1000

#define SRAM2_MEMORY_Base               0x5C000000
#define SRAM2_MEMORY_Size               (256*1024)

// SRAM1 is actually SDRAM. These defines are used in the global HAL config
// which tinybooter uses to determine valid RAM addresses.  This means that 
// the real SRAM (SRAM2) will not allow programming in tinybooter. 
#define SRAM1_MEMORY_Base              0xA0000000
#define SRAM1_MEMORY_Size              (32*1024*1024)


#define FLASH_MEMORY_Base               0x00000000
#define FLASH_MEMORY_Size               (32*1024*1024)

#define TXPROTECTRESISTOR               RESISTOR_DISABLED
#define RXPROTECTRESISTOR               RESISTOR_DISABLED
#define CTSPROTECTRESISTOR              RESISTOR_DISABLED
#define RTSPROTECTRESISTOR              RESISTOR_DISABLED

 #define PXA271_UNUSED_GPIOS   \
    UNUSED_GPIO_PULLDOWN(14),  \
    UNUSED_GPIO_PULLDOWN(15),  \
    UNUSED_GPIO_PULLDOWN(18),  \
    UNUSED_GPIO_PULLDOWN(19),  \
    UNUSED_GPIO_PULLDOWN(20),  \
    UNUSED_GPIO_PULLDOWN(21),  \
    UNUSED_GPIO_PULLDOWN(27),  \
    UNUSED_GPIO_PULLDOWN(33),  \
    UNUSED_GPIO_PULLDOWN(49),  \
    UNUSED_GPIO_PULLDOWN(58),  \
    UNUSED_GPIO_PULLDOWN(59),  \
    UNUSED_GPIO_PULLDOWN(60),  \
    UNUSED_GPIO_PULLDOWN(61),  \
    UNUSED_GPIO_PULLDOWN(62),  \
    UNUSED_GPIO_PULLDOWN(63),  \
    UNUSED_GPIO_PULLDOWN(64),  \
    UNUSED_GPIO_PULLDOWN(65),  \
    UNUSED_GPIO_PULLDOWN(66),  \
    UNUSED_GPIO_PULLDOWN(67),  \
    UNUSED_GPIO_PULLDOWN(68),  \
    UNUSED_GPIO_PULLDOWN(69),  \
    UNUSED_GPIO_PULLDOWN(70),  \
    UNUSED_GPIO_PULLDOWN(71),  \
    UNUSED_GPIO_PULLDOWN(72),  \
    UNUSED_GPIO_PULLDOWN(73),  \
    UNUSED_GPIO_PULLDOWN(74),  \
    UNUSED_GPIO_PULLDOWN(75),  \
    UNUSED_GPIO_PULLDOWN(76),  \
    UNUSED_GPIO_PULLDOWN(77),  \
    UNUSED_GPIO_PULLDOWN(78),  \
    UNUSED_GPIO_PULLDOWN(79),  \
    UNUSED_GPIO_PULLDOWN(80),  \
    UNUSED_GPIO_PULLDOWN(86),  \
    UNUSED_GPIO_PULLDOWN(87),  \
    UNUSED_GPIO_PULLDOWN(90),  \
    UNUSED_GPIO_PULLDOWN(91),  \
    UNUSED_GPIO_PULLUP  (103), \
    UNUSED_GPIO_PULLUP  (104), \
    UNUSED_GPIO_PULLUP  (105), \
    UNUSED_GPIO_PULLDOWN(115), \
    UNUSED_GPIO_PULLDOWN(119), \
    UNUSED_GPIO_PULLDOWN(120)
    
// Pins 103, 104 & 105 are used to drive the LEDs, and setting them as PULLUP initializes
// the LEDs as turned off.  Pin 115 is the Zigbee chip voltage regulator enable and is
// pulled down to turn it off until it is used.

// Per Lama Nachman at Intel, the following GPIO lines, although not mentioned
// on the current i<TEMPLATE> data sheet, are connected as follows:
//
// GPIO1:   NPMC_IRQ - don't drive
// GPIO2:   SYS_EN   - don't drive?
// GPIO9:   13MHZ_TO_DA9030 - don't drive
// GPIO13:  USBC_GPION_DET - don't drive
// GPIO40:  alternate CC2420_FIFO - but not connected unless R12 is installed
// GPIO88:  USBC_GPIOX_EN - don't drive
// GPIO89:  USBH_PWREN - don't drive
// GPIO95:  <TEMPLATE> version ID - don't drive
// GPIO97:  <TEMPLATE> version ID - don't drive
// GPIO101: <TEMPLATE> version ID - don't drive
// GPIO102: <TEMPLATE> version ID - don't drive
// GPIO108: PMIC_TXON - don't drive

// Light emitting diode defines
#define LED1_RED    PXA271_GPIO::c_Port_103
#define LED1_GREEN  PXA271_GPIO::c_Port_104
#define LED1_BLUE   PXA271_GPIO::c_Port_105
#define LED_ON      FALSE                        /* LED control is active low */
#define LED_OFF     TRUE
    
#define INSTRUMENTATION_H_GPIO_PIN      PXA271_GPIO::c_Pin_None

#define DEBUG_TEXT_PORT            USB1
#define STDIO                      USB1
#define DEBUGGER_PORT              USB1
#define MESSAGING_PORT             USB1

//
// constants
/////////////////////////////////////////////////////////


#include <processor_selector.h>

#endif // PLATFORM_ARM_<TEMPLATE>

#endif // _PLATFORM_<TEMPLATE>_SELECTOR_H_ 1
