////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Implementation for the MCBSTM32F400 board (STM32F4): Copyright (c) Oberon microsystems, Inc.
//
//  *** Netduino3Ethernet Board specific IO Port Initialization ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\..\..\..\DeviceCode\Targets\Native\STM32F4\DeviceCode\stm32f4xx.h"

// Define the generic port table, only one generic extensionn port type supported
// and that is the ITM hardware trace port on Channel 0.
extern GenericPortTableEntry const Itm0GenericPort;
extern GenericPortTableEntry const* const g_GenericPorts[TOTAL_GENERIC_PORTS] = { &Itm0GenericPort };

extern void STM32F4_GPIO_Pin_Config( GPIO_PIN pin, UINT32 mode, GPIO_RESISTOR resistor, UINT32 alternate );    // Workaround, since CPU_GPIO_DisablePin() does not correctly initialize pin speeds



void __section(SectionForBootstrapOperations) Netduino3Ethernet_InitSRam() {

    /* Enable FMC clock */
    RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;
  
    // Configure FMC pins
    // PB5,6
    GPIOB->MODER   = 0x00002800; // AF
    GPIOB->OSPEEDR = 0x00002800; // 50MHz
    GPIOB->AFR[0]  = 0x0CC00000; // AF12
    // PC0
    GPIOC->MODER   = 0x00000002; // AF
    GPIOC->OSPEEDR = 0x00000002; // 50MHz
    GPIOC->AFR[0]  = 0x0000000C; // AF12
    // PD0,1,8-10,14,15
    GPIOD->MODER   = 0xA02A000A; // AF
    GPIOD->OSPEEDR = 0xA02A000A; // 50MHz
    GPIOD->AFR[0]  = 0x000000CC; // AF12
    GPIOD->AFR[1]  = 0xCC000CCC; // AF12
    // PE0,1,7-15
    GPIOE->MODER   = 0xAAAA800A; // AF
    GPIOE->OSPEEDR = 0xAAAA800A; // 50MHz
    GPIOE->AFR[0]  = 0xC00000CC; // AF12
    GPIOE->AFR[1]  = 0xCCCCCCCC; // AF12
    // PF0-5,11-15
    GPIOF->MODER   = 0xAA800AAA; // AF
    GPIOF->OSPEEDR = 0xAA800AAA; // 50MHz
    GPIOF->AFR[0]  = 0x00CCCCCC; // AF12
    GPIOF->AFR[1]  = 0xCCCCC000; // AF12
    // PG0,1,4,5,8,15
    GPIOG->MODER   = 0x80020A0A; // AF
    GPIOG->OSPEEDR = 0x80020A0A; // 50MHz
    GPIOG->AFR[0]  = 0x00CC00CC; // AF12
    GPIOG->AFR[1]  = 0xC000000C; // AF12
  

    // Configure FMC bank 2
    // (some bits must be set in bank 1)
    
    FMC_Bank5_6->SDCR[0] = FMC_SDCR1_SDCLK_0 * 2  // periode clocks
                         | FMC_SDCR1_RBURST  * 0  // no burst
                         | FMC_SDCR1_RPIPE_0 * 1; // pipe delay
    FMC_Bank5_6->SDCR[1] = FMC_SDCR1_NC_0   * 0   // 8  col bits
                         | FMC_SDCR1_NR_0   * 1   // 12 row bits
                         | FMC_SDCR1_MWID_0 * 1   // 16data  bits
                         | FMC_SDCR1_NB     * 1   // 4 internal banks
                         | FMC_SDCR1_CAS_0  * 3   // 3 cycles
                         | FMC_SDCR1_WP     * 0;  // no write protection

    FMC_Bank5_6->SDTR[0] = FMC_SDTR1_TRC_0  * (7-1)  // row cycle delay
                         | FMC_SDTR1_TRP_0  * (2-1); // row precharge delay
    FMC_Bank5_6->SDTR[1] = FMC_SDTR1_TMRD_0 * (2-1)  // load to acvtive
                         | FMC_SDTR1_TXSR_0 * (7-1)  // exit self refresh
                         | FMC_SDTR1_TRAS_0 * (4-1)  // self refresh
                         | FMC_SDTR1_TWR_0  * (2-1)  // recovery delay
                         | FMC_SDTR1_TRCD_0 * (2-1); // row to column

    
    while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
    FMC_Bank5_6->SDCMR = 1               // clock config enable
                       | FMC_SDCMR_CTB2; // bank 2
    
    HAL_Time_Sleep_MicroSeconds_InterruptEnabled(10000); // wait 10ms
    
    while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
    FMC_Bank5_6->SDCMR = 2               // PALL
                       | FMC_SDCMR_CTB2; // bank 2
    
    while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
    FMC_Bank5_6->SDCMR = 3               // auto refresh (first)
                       | FMC_SDCMR_CTB2  // bank 2
                       | (4-1) << 5;     // cycles
    
    while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
    FMC_Bank5_6->SDCMR = 3               // auto refresh (second)
                       | FMC_SDCMR_CTB2  // bank 2
                       | (4-1) << 5;     // cycles
    
    while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
    FMC_Bank5_6->SDCMR = 4               // load mode
                       | FMC_SDCMR_CTB2  // bank 2
                       | 0x0231 << 9;    // burst len = 2, cas latency = 3, burst single

    
    FMC_Bank5_6->SDRTR = 636 << 1; // Set refresh timer
    
    while(FMC_Bank5_6->SDSR & FMC_SDSR_BUSY);
}
void __section("SectionForBootstrapOperations") BootstrapCode_GPIO()
{
    // Enable GPIO clocks for ports A - G
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN
                  | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN;

	//Netduino3Ethernet_InitSRam(); // initialize FMC
}