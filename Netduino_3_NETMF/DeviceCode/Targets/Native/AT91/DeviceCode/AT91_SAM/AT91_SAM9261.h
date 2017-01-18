////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AT91SAM9261_H_1
#define _AT91SAM9261_H_1 1
    
//
//  PERIPHERAL ID DEFINITIONS FOR AT91SAM9261
//
#define AT91C_ID_FIQ    ((unsigned int)  0) // Advanced Interrupt Controller (FIQ)
#define AT91C_ID_SYS    ((unsigned int)  1) // System Interrupt
#define AT91C_ID_PIOA   ((unsigned int)  2) // Parallel IO Controller A
#define AT91C_ID_PIOB   ((unsigned int)  3) // Parallel IO Controller B
#define AT91C_ID_PIOC   ((unsigned int)  4) // Parallel IO Controller C
#define AT91C_ID_US0    ((unsigned int)  6) // USART 0
#define AT91C_ID_US1    ((unsigned int)  7) // USART 1
#define AT91C_ID_US2    ((unsigned int)  8) // USART 2
#define AT91C_ID_MCI    ((unsigned int)  9) // Multimedia Card Interface
#define AT91C_ID_UDP    ((unsigned int) 10) // USB Device Port
#define AT91C_ID_TWI    ((unsigned int) 11) // Two-Wire Interface
#define AT91C_ID_SPI0   ((unsigned int) 12) // Serial Peripheral Interface 0
#define AT91C_ID_SPI1   ((unsigned int) 13) // Serial Peripheral Interface 1
#define AT91C_ID_SSC0   ((unsigned int) 14) // Serial Synchronous Controller 0
#define AT91C_ID_SSC1   ((unsigned int) 15) // Serial Synchronous Controller 1
#define AT91C_ID_SSC2   ((unsigned int) 16) // Serial Synchronous Controller 2
#define AT91C_ID_TC0    ((unsigned int) 17) // Timer Counter 0
#define AT91C_ID_TC1    ((unsigned int) 18) // Timer Counter 1
#define AT91C_ID_TC2    ((unsigned int) 19) // Timer Counter 2
#define AT91C_ID_UHP    ((unsigned int) 20) // USB Host Port
#define AT91C_ID_LCDC   ((unsigned int) 21) // LCD Controller
#define AT91C_ID_IRQ0   ((unsigned int) 29) // Advanced Interrupt Controller (IRQ0)
#define AT91C_ID_IRQ1   ((unsigned int) 30) // Advanced Interrupt Controller (IRQ1)
#define AT91C_ID_IRQ2   ((unsigned int) 31) // Advanced Interrupt Controller (IRQ2)
#define AT91C_ALL_INT   ((unsigned int) 0xE03FFFDF) // ALL VALID INTERRUPTS

//
// BASE ADDRESS DEFINITIONS FOR AT91SAM9261
//
#define AT91C_BASE_SYS          0xFFFFEA00 // (SYS) Base Address
#define AT91C_BASE_SDRAMC       0xFFFFEA00 // (SDRAMC) Base Address
#define AT91C_BASE_SMC          0xFFFFEC00 // (SMCTRL) Base Address
#define AT91C_BASE_MATRIX       0xFFFFEE00 // (MATRIX) Base Address
#define AT91C_BASE_AIC          0xFFFFF000 // (AIC) Base Address
#define AT91C_BASE_PDC_DBGU     0xFFFFF300 // (PDC_DBGU) Base Address
#define AT91C_BASE_DBGU         0xFFFFF200 // (DBGU) Base Address
#define AT91C_BASE_PIOA         0xFFFFF400 // (PIOA) Base Address
#define AT91C_BASE_PIOB         0xFFFFF600 // (PIOB) Base Address
#define AT91C_BASE_PIOC         0xFFFFF800 // (PIOC) Base Address
#define AT91C_BASE_CKGR         0xFFFFFC20 // (CKGR) Base Address
#define AT91C_BASE_PMC          0xFFFFFC00 // (PMC) Base Address
#define AT91C_BASE_RSTC         0xFFFFFD00 // (RSTC) Base Address
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
#define AT91C_BASE_SHDWC        0xFFFFFD10 // (SHDWC) Base Address
#define AT91C_BASE_RTTC         0xFFFFFD20 // (RTTC) Base Address
#define AT91C_BASE_PITC         0xFFFFFD30 // (PITC) Base Address
#define AT91C_BASE_WDTC         0xFFFFFD40 // (WDTC) Base Address
#define AT91C_BASE_TC0          0xFFFA0000 // (TC0) Base Address
#define AT91C_BASE_TC1          0xFFFA0040 // (TC1) Base Address
#define AT91C_BASE_TC2          0xFFFA0080 // (TC2) Base Address
#define AT91C_BASE_TCB0         0xFFFA0000 // (TCB0) Base Address
#define AT91C_BASE_UDP          0xFFFA4000 // (UDP) Base Address
#define AT91C_BASE_PDC_MCI      0xFFFA8100 // (PDC_MCI) Base Address
#define AT91C_BASE_MCI          0xFFFA8000 // (MCI) Base Address
#define AT91C_BASE_TWI          0xFFFAC000 // (TWI) Base Address
#define AT91C_BASE_PDC_US0      0xFFFB0100 // (PDC_US0) Base Address
#define AT91C_BASE_US0          0xFFFB0000 // (US0) Base Address
#define AT91C_BASE_PDC_US1      0xFFFB4100 // (PDC_US1) Base Address
#define AT91C_BASE_US1          0xFFFB4000 // (US1) Base Address
#define AT91C_BASE_PDC_US2      0xFFFB8100 // (PDC_US2) Base Address
#define AT91C_BASE_US2          0xFFFB8000 // (US2) Base Address
#define AT91C_BASE_PDC_SSC0     0xFFFBC100 // (PDC_SSC0) Base Address
#define AT91C_BASE_SSC0         0xFFFBC000 // (SSC0) Base Address
#define AT91C_BASE_PDC_SSC1     0xFFFC0100 // (PDC_SSC1) Base Address
#define AT91C_BASE_SSC1         0xFFFC0000 // (SSC1) Base Address
#define AT91C_BASE_PDC_SSC2     0xFFFC4100 // (PDC_SSC2) Base Address
#define AT91C_BASE_SSC2         0xFFFC4000 // (SSC2) Base Address
#define AT91C_BASE_PDC_SPI0     0xFFFC8100 // (PDC_SPI0) Base Address
#define AT91C_BASE_SPI0         0xFFFC8000 // (SPI0) Base Address
#define AT91C_BASE_PDC_SPI1     0xFFFCC100 // (PDC_SPI1) Base Address
#define AT91C_BASE_SPI1         0xFFFCC000 // (SPI1) Base Address
#define AT91C_BASE_SHDWC        0xFFFFFD10 // (SHDWC) shutdown controller
#define AT91C_SHDWC__SHUTDOWN_KEY   0xA5000000   // reset key
#define AT91C_SHDWC__SHDW           0x01         // processor reset bit

#define AT91C_BASE_UHP          0x00500000 // (UHP) Base Address
#define AT91C_BASE_LCDC         0x00600000 // (LCDC) Base Address
#define AT91C_BASE_LCDC_16B_TFT 0x00600000 // (LCDC_16B_TFT) Base Address

// Timer number - See AT91.H
#define AT91_MAX_TIMER      3

// Max GPIO number 
#define AT91_MAX_GPIO       96 

// Max USART Number
#define AT91_MAX_USART   4 

// Define the multiplexing pins USART
#define AT91_DRXD   AT91_GPIO_Driver::PA9
#define AT91_DTXD   AT91_GPIO_Driver::PA10
#define AT91_TXD0   AT91_GPIO_Driver::PC8
#define AT91_RXD0   AT91_GPIO_Driver::PC9
#define AT91_RTS0   AT91_GPIO_Driver::PC10
#define AT91_CTS0   AT91_GPIO_Driver::PC11    
#define AT91_TXD1   AT91_GPIO_Driver::PC12
#define AT91_RXD1   AT91_GPIO_Driver::PC13
#define AT91_TXD2   AT91_GPIO_Driver::PC14
#define AT91_RXD2   AT91_GPIO_Driver::PC15

// Define the multiplexing pins LCD
#define AT91_LCDVSYNC  AT91_GPIO_Driver::PB0
#define AT91_LCDHSYNC  AT91_GPIO_Driver::PB1
#define AT91_LCDDOTCK  AT91_GPIO_Driver::PB2       
#define AT91_LCDDEN    AT91_GPIO_Driver::PB3
#define AT91_LCDCC     AT91_GPIO_Driver::PB4
#define AT91_LCDD0     AT91_GPIO_Driver::PB5
#define AT91_LCDD1     AT91_GPIO_Driver::PB6
#define AT91_LCDD2     AT91_GPIO_Driver::PB7
#define AT91_LCDD3     AT91_GPIO_Driver::PB8
#define AT91_LCDD4     AT91_GPIO_Driver::PB9
#define AT91_LCDD5     AT91_GPIO_Driver::PB10
#define AT91_LCDD6     AT91_GPIO_Driver::PB11
#define AT91_LCDD7     AT91_GPIO_Driver::PB12
#define AT91_LCDD8     AT91_GPIO_Driver::PB13
#define AT91_LCDD9     AT91_GPIO_Driver::PB14
#define AT91_LCDD10    AT91_GPIO_Driver::PB15
#define AT91_LCDD11    AT91_GPIO_Driver::PB16
#define AT91_LCDD12    AT91_GPIO_Driver::PB17
#define AT91_LCDD13    AT91_GPIO_Driver::PB18
#define AT91_LCDD14    AT91_GPIO_Driver::PB19
#define AT91_LCDD15    AT91_GPIO_Driver::PB20
#define AT91_LCDD16    AT91_GPIO_Driver::PB21
#define AT91_LCDD17    AT91_GPIO_Driver::PB22
#define AT91_LCDD18    AT91_GPIO_Driver::PB23
#define AT91_LCDD19    AT91_GPIO_Driver::PB24
#define AT91_LCDD20    AT91_GPIO_Driver::PB25
#define AT91_LCDD21    AT91_GPIO_Driver::PB26
#define AT91_LCDD22    AT91_GPIO_Driver::PB27
#define AT91_LCDD23    AT91_GPIO_Driver::PB28       

//PMC
#define AT91C_PMC_PCK         (0x1 << 0 ) /**< (PMC) Processor Clock */
#define AT91C_PMC_UHP         (0x1 << 6 ) /**< (PMC) USB Host Port Clock */
#define AT91C_PMC_UDP         (0x1 << 7 ) /**< (PMC) USB Device Port Clock */
#define AT91C_PMC_PCK0        (0x1 << 8 ) /**< (PMC) Programmable Clock Output */
#define AT91C_PMC_PCK1        (0x1 << 9 ) /**< (PMC) Programmable Clock Output */
#define AT91C_PMC_HCK0        (0x1 << 16) /**< (PMC) AHB UHP Clock Output */
#define AT91C_PMC_HCK1        (0x1 << 17) /**< (PMC) AHB LCDC Clock Output */

#include "../AT91_GPIO/AT91_GPIO.h"
#include "../AT91_SAM/AT91_SAM.h"
#include "../AT91_TIME/AT91_TIME.h"
#include "../AT91_USART/AT91_USART.h"
#include "../AT91_USB/AT91_USB.h"


static const UINT8 c_LCD_1[] =
{
    AT91_LCDHSYNC,
    AT91_LCDDOTCK,
    AT91_LCDDEN,
    AT91_LCDCC,
    AT91_LCDD2,
    AT91_LCDD3,
    AT91_LCDD4,
    AT91_LCDD5,
    AT91_LCDD6,
    AT91_LCDD7,
    AT91_LCDD10,
    AT91_LCDD11,
    AT91_LCDD12,
    AT91_LCDD13,
    AT91_LCDD14,
    AT91_LCDD15,
};

static const UINT8 c_LCD_2[] =
{  
    AT91_LCDD18,
    AT91_LCDD19,
    AT91_LCDD20,
    AT91_LCDD21,
    AT91_LCDD22,
    AT91_LCDD23,
};

// Define the multiplexing pins NAND Flash
#define AT91_NAND_RE    AT91_GPIO_Driver::PC0
#define AT91_NAND_WE    AT91_GPIO_Driver::PC1

#define AT91C_MAX_SPI   2
#define AT91C_SPI_CS    4

// Define the multiplexing pins SPI0
#define AT91_SPI0_MISO  AT91_GPIO_Driver::PA0
#define AT91_SPI0_MOSI  AT91_GPIO_Driver::PA1
#define AT91_SPI0_SCLK  AT91_GPIO_Driver::PA2
#define AT91_SPI0_NSS   AT91_GPIO_Driver::PA3

// Define the multiplexing pins SPI1
#define AT91_SPI1_MISO  AT91_GPIO_Driver::PB30
#define AT91_SPI1_MOSI  AT91_GPIO_Driver::PB31
#define AT91_SPI1_SCLK  AT91_GPIO_Driver::PB29
#define AT91_SPI1_NSS   AT91_GPIO_Driver::PB28


//the selecting of CS pins could vary according to different hardware designs
#define AT91_SPI0_NCPS0      AT91_GPIO_Driver::PA3
#define AT91_SPI0_NCPS1      AT91_GPIO_Driver::PA4
#define AT91_SPI0_NCPS2      AT91_GPIO_Driver::PA5
#define AT91_SPI0_NCPS3      AT91_GPIO_Driver::PA6

// this has to at Altern B
#define AT91_SPI1_NCPS0      AT91_GPIO_Driver::PB27
#define AT91_SPI1_NCPS1      AT91_GPIO_Driver::PA24
#define AT91_SPI1_NCPS2      AT91_GPIO_Driver::PA25
#define AT91_SPI1_NCPS3      AT91_GPIO_Driver::PA26


// define the TWI (i2C) pins
#define AT91_TWI_SDA    AT91_GPIO_Driver::PA7
#define AT91_TWI_SCL    AT91_GPIO_Driver::PA8

#endif // _AT91SAM9261_H_1

