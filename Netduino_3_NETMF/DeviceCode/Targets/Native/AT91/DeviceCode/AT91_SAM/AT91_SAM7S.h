////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AT91SAM7S_H_1
#define _AT91SAM7S_H_1 1

//  PERIPHERAL ID DEFINITIONS FOR AT91SAM9261
// Advanced Interrupt Controller (FIQ)
#define AT91C_ID_FIQ    ((unsigned int)  0) 

// System Peripheral
#define AT91C_ID_SYS    ((unsigned int)  1) 

// Parallel IO Controller A
#define AT91C_ID_PIOA   ((unsigned int)  2) 

// 3 reserve


// Analog-to-Digital Converter
#define AT91C_ID_ADC    ((unsigned int) 4) 

// Serial Peripheral Interface 0
#define AT91C_ID_SPI0   ((unsigned int)  5) 



// USART 0
#define AT91C_ID_US0    ((unsigned int)  6) 
// USART 1
#define AT91C_ID_US1    ((unsigned int)  7) 

// Serial Synchronous Controller
#define AT91C_ID_SSC    ((unsigned int)  8) 
// Two-Wire Interface
#define AT91C_ID_TWI    ((unsigned int)  9) 
// PWM Controller
#define AT91C_ID_PWMC   ((unsigned int) 10) 
// USB Device Port
#define AT91C_ID_UDP    ((unsigned int) 11) 
// Timer Counter 0
#define AT91C_ID_TC0    ((unsigned int) 12) 
// Timer Counter 1
#define AT91C_ID_TC1    ((unsigned int) 13) 
// Timer Counter 2
#define AT91C_ID_TC2    ((unsigned int) 14) 

// reserve 15-29



// Advanced Interrupt Controller (IRQ0)
#define AT91C_ID_IRQ0   ((unsigned int) 30) 
// Advanced Interrupt Controller (IRQ1)
#define AT91C_ID_IRQ1   ((unsigned int) 31) 
// ALL VALID INTERRUPTS
#define AT91C_ALL_INT   ((unsigned int) 0xC003FFFF) 

// BASE ADDRESS DEFINITIONS FOR AT91SAM7S256
// (SYS) Base Address
#define AT91C_BASE_SYS          0xFFFFF000 
// (AIC) Base Address
#define AT91C_BASE_AIC          0xFFFFF000 
// (PDC_DBGU) Base Address
#define AT91C_BASE_PDC_DBGU     0xFFFFF300 
// (DBGU) Base Address
#define AT91C_BASE_DBGU         0xFFFFF200 
// (PIOA) Base Address
#define AT91C_BASE_PIOA         0xFFFFF400 
// (PIOB) Base Address
#define AT91C_BASE_PIOB         0xFFFFF600 
// (CKGR) Base Address
#define AT91C_BASE_CKGR         0xFFFFFC20 
// (PMC) Base Address
#define AT91C_BASE_PMC          0xFFFFFC00 
// (RSTC) Base Address
#define AT91C_BASE_RSTC         0xFFFFFD00 
    #define AT91C_RTSC__PROCRST     0x01         // processor reset bit
    #define AT91C_RTSC__PERRST      0x04         // Peripheral reset bit
    #define AT91C_RTSC__EXTRST      0x08         // asserts NRST pin
    #define AT91C_RSTC__RESET_KEY   0xA5000000   // reset key
#define AT91C_BASE_RSTC_SR      0xFFFFFD04 
    #define AT91C_RTSC_SR__SRCMP    (1ul << 17)   // Software Reset Command in progress
    #define AT91C_RTSC_SR__NRSTL    (1ul << 16)   // Registers the NRST Pin Level at Master Clock (MCK)
    #define AT91C_RTSC_SR__RSTTYP   (7ul <<  8)   // Reset Type mask
    #define AT91C_RTSC_SR__BODSTS   (1ul <<  1)   // Brownout Detection Status
    #define AT91C_RTSC_SR__URSTS    (1ul <<  0)   // User Reset Status
#define AT91C_BASE_RSTC_MR      0xFFFFFD08 
// (RTTC) Base Address
#define AT91C_BASE_RTTC         0xFFFFFD20 
// (PITC) Base Address
#define AT91C_BASE_PITC         0xFFFFFD30 
// (WDTC) Base Address
#define AT91C_BASE_WDTC         0xFFFFFD40 
// (VREG) Base Address
#define AT91C_BASE_VREG         0xFFFFFD60 
// (EFC0) Base Address
#define AT91C_BASE_EFC0         0xFFFFFF60 
// (EFC1) Base Address
#define AT91C_BASE_EFC1         0xFFFFFF70 
// (MC) Base Address
#define AT91C_BASE_MC           0xFFFFFF00 
// (PDC_SPI1) Base Address
#define AT91C_BASE_PDC_SPI1     0xFFFE4100 
// (SPI1) Base Address
#define AT91C_BASE_SPI1         0xFFFE4000 
// (PDC_SPI0) Base Address
#define AT91C_BASE_PDC_SPI0     0xFFFE0100 
// (SPI0) Base Address
#define AT91C_BASE_SPI0         0xFFFE0000 
// (PDC_US1) Base Address
#define AT91C_BASE_PDC_US1      0xFFFC4100 
// (US1) Base Address
#define AT91C_BASE_US1          0xFFFC4000 
// (PDC_US0) Base Address
#define AT91C_BASE_PDC_US0      0xFFFC0100 
// (US0) Base Address
#define AT91C_BASE_US0          0xFFFC0000 
// (PDC_SSC) Base Address
#define AT91C_BASE_PDC_SSC      0xFFFD4100 
// (SSC) Base Address
#define AT91C_BASE_SSC          0xFFFD4000 
// (TWI) Base Address
#define AT91C_BASE_TWI          0xFFFB8000 
// (PWMC_CH3) Base Address
#define AT91C_BASE_PWMC_CH3     0xFFFCC260 
// (PWMC_CH2) Base Address
#define AT91C_BASE_PWMC_CH2     0xFFFCC240 
// (PWMC_CH1) Base Address
#define AT91C_BASE_PWMC_CH1     0xFFFCC220 
// (PWMC_CH0) Base Address
#define AT91C_BASE_PWMC_CH0     0xFFFCC200 
// (PWMC) Base Address
#define AT91C_BASE_PWMC         0xFFFCC000 
// (UDP) Base Address
#define AT91C_BASE_UDP          0xFFFB0000 
// (TC0) Base Address
#define AT91C_BASE_TC0          0xFFFA0000 
// (TC1) Base Address
#define AT91C_BASE_TC1          0xFFFA0040 
// (TC2) Base Address
#define AT91C_BASE_TC2          0xFFFA0080 
// (TCB) Base Address
#define AT91C_BASE_TCB          0xFFFA0000 

// (PDC_ADC) Base Address
#define AT91C_BASE_PDC_ADC      0xFFFD8100 
// (ADC) Base Address
#define AT91C_BASE_ADC          0xFFFD8000 


// Internal SRAM
// Internal SRAM base address (No Remap)
#define AT91C_ISRAM         (0x00200000) 
// Internal SRAM size in byte (128 Kbytes)
#define AT91C_ISRAM_SIZE    (0x00020000) 

// Internal FLASH
// Internal FLASH base address
#define AT91C_IFLASH                    (0x00100000)    

// Internal FLASH size in byte (256Kbytes)
#define AT91C_IFLASH_SIZE               (0x00040000)    
// Internal FLASH Page Size: 256 bytes
#define AT91C_IFLASH_PAGE_SIZE          (256)           

// Internal FLASH Lock Region Size: 16 Kbytes
#define AT91C_IFLASH_LOCK_REGION_SIZE   (16384)         

// Internal FLASH Number of Pages: 1024 bytes
#define AT91C_IFLASH_NB_OF_PAGES        (1024)          

// Internal FLASH Number of Lock Bits: 16 bytes
#define AT91C_IFLASH_NB_OF_LOCK_BITS    (16)            
#define AT91C_IFLASH_SECTOR_SIZE        (4)

#define AT91C_IFLASH_SECTOR_PER_PAGE    (AT91C_IFLASH_PAGE_SIZE/AT91C_IFLASH_SECTOR_SIZE)

#define AT91C_IFLASH_PAGE_PER_BLOCK     32

#define AT91C_IFLASH_SECTOR_PER_BLOCK   (AT91C_IFLASH_SECTOR_PER_PAGE * AT91C_IFLASH_PAGE_PER_BLOCK)


// Timer number - See AT91.H
#define AT91_MAX_TIMER  3

// Max GPIO number - 1 ports
#define AT91_MAX_GPIO   32 

// Max USART Number
#define AT91_MAX_USART   2 

// Define the multiplexing pins - USART
#define AT91_DRXD   AT91_GPIO_Driver::PA9
#define AT91_DTXD   AT91_GPIO_Driver::PA10

#define AT91_TXD0   AT91_GPIO_Driver::PA6
#define AT91_RXD0   AT91_GPIO_Driver::PA5
#define AT91_RTS0   AT91_GPIO_Driver::PA7
#define AT91_CTS0   AT91_GPIO_Driver::PA8

#define AT91_TXD1   AT91_GPIO_Driver::PA22
#define AT91_RXD1   AT91_GPIO_Driver::PA21
#define AT91_RTS1   AT91_GPIO_Driver::PA24
#define AT91_CTS1   AT91_GPIO_Driver::PA25

// PMC
// (PMC) Processor Clock
#define AT91C_PMC_PCK         (0x1 <<  0) 
// (PMC) USB Device Port Clock
#define AT91C_PMC_UDP         (0x1 <<  7) 
// (PMC) Programmable Clock Output
#define AT91C_PMC_PCK0        (0x1 <<  8) 
// (PMC) Programmable Clock Output
#define AT91C_PMC_PCK1        (0x1 <<  9) 
// (PMC) Programmable Clock Output
#define AT91C_PMC_PCK2        (0x1 << 10) 
// (PMC) Programmable Clock Output
#define AT91C_PMC_PCK3        (0x1 << 11) 

#include "../AT91_GPIO/AT91_GPIO.h"
#include "../AT91_SAM/AT91_SAM.h"
#include "../AT91_TIME/AT91_TIME.h"
#include "../AT91_USART/AT91_USART.h"
#include "../AT91_USB/AT91_USB.h"
#include "../AT91_BL_EFC/AT91_BL_EFC.h"

#define AT91C_MAX_SPI       1
#define AT91C_SPI_CS        4


#define AT91_SPI0_MISO		AT91_GPIO_Driver::PA12
#define AT91_SPI0_MOSI   	AT91_GPIO_Driver::PA13
#define AT91_SPI0_SCLK		AT91_GPIO_Driver::PA14
#define AT91_SPI0_NSS   	AT91_GPIO_Driver::PA11

//the selecting of CS pins could vary according to different hardware designs
#define AT91_SPI0_NCPS0     AT91_GPIO_Driver::PA30
#define AT91_SPI0_NCPS1     AT91_GPIO_Driver::PA31
#define AT91_SPI0_NCPS2     AT91_GPIO_Driver::PA14
#define AT91_SPI0_NCPS3     AT91_GPIO_Driver::PA15

#define AT91_TWI_SDA        AT91_GPIO_Driver::PA3
#define AT91_TWI_SCL        AT91_GPIO_Driver::PA4

#endif // _AT91SAM7S_H_1

