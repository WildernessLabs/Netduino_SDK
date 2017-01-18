////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MC9328MXL_H_
#define _MC9328MXL_H_ 1

#include <cores\arm\include\cpu.h>

extern "C"
{
    void BootstrapCode_Clocks();
    void BootstrapCode_SDRAM();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// LAYOUT SECTION
//
//

//////////////////////////////////////////////////////////////////////////////
// EIM
//
struct MC9328MXL_EIM
{
    struct DEVICE
    {
        /****/ volatile UINT32 CS_U;
        static const    UINT32 CS_U__DTACK_SEL  = 0x80000000; // Not for CS0
        static const    UINT32 CS_U__BCD__mask  = 0x30000000;
        static const    UINT32 CS_U__BCD__shift =         28;
        static const    UINT32 CS_U__BCX__mask  = 0x0F000000;
        static const    UINT32 CS_U__BCX__shift =         24;
        static const    UINT32 CS_U__PSZ__mask  = 0x00C00000;
        static const    UINT32 CS_U__PSZ__shift =         20;
        static const    UINT32 CS_U__PME        = 0x00200000;
        static const    UINT32 CS_U__SYNC       = 0x00100000;
        static const    UINT32 CS_U__DOL__mask  = 0x000F0000;
        static const    UINT32 CS_U__DOL__shift =         16;
        static const    UINT32 CS_U__CNC__mask  = 0x0000C000;
        static const    UINT32 CS_U__CNC__shift =         12;
        static const    UINT32 CS_U__WSC__mask  = 0x00003F00;
        static const    UINT32 CS_U__WSC__shift =          8;
        static const    UINT32 CS_U__WWS__mask  = 0x00000070;
        static const    UINT32 CS_U__WWS__shift =          4;
        static const    UINT32 CS_U__EDC__mask  = 0x0000000F;
        static const    UINT32 CS_U__EDC__shift =          0;

        static const    UINT32 CS_U__NEEDED_CONTROL = CS_U__WSC__mask | CS_U__EDC__mask; // fields mask we care for

        __inline static UINT32 CS_U__EDC__set( UINT32 w ) { return (w << CS_U__EDC__shift) & CS_U__EDC__mask; }
        __inline static UINT32 CS_U__WWS__set( UINT32 w ) { return (w << CS_U__WWS__shift) & CS_U__WWS__mask; }
        __inline static UINT32 CS_U__WSC__set( UINT32 w ) { return (w << CS_U__WSC__shift) & CS_U__WSC__mask; }
        __inline static UINT32 CS_U__CNC__set( UINT32 w ) { return (w << CS_U__CNC__shift) & CS_U__CNC__mask; }
        __inline static UINT32 CS_U__DOL__set( UINT32 w ) { return (w << CS_U__DOL__shift) & CS_U__DOL__mask; }
        __inline static UINT32 CS_U__PSZ__set( UINT32 w ) { return (w << CS_U__PSZ__shift) & CS_U__PSZ__mask; }
        __inline static UINT32 CS_U__BCX__set( UINT32 w ) { return (w << CS_U__BCX__shift) & CS_U__BCX__mask; }
        __inline static UINT32 CS_U__BCD__set( UINT32 w ) { return (w << CS_U__BCD__shift) & CS_U__BCD__mask; }


        /****/ volatile UINT32 CS_L;
        static const    UINT32 CS_L__OEA__mask  = 0xF0000000;
        static const    UINT32 CS_L__OEA__shift =         28;
        static const    UINT32 CS_L__OEN__mask  = 0x0F000000;
        static const    UINT32 CS_L__OEN__shift =         24;
        static const    UINT32 CS_L__WEA__mask  = 0x00F00000;
        static const    UINT32 CS_L__WEA__shift =         20;
        static const    UINT32 CS_L__WEN__mask  = 0x000F0000;
        static const    UINT32 CS_L__WEN__shift =         16;
        static const    UINT32 CS_L__CSA__mask  = 0x0000F000;
        static const    UINT32 CS_L__CSA__shift =         12;
        static const    UINT32 CS_L__EBC        = 0x00000800;
        static const    UINT32 CS_L__DSZ__mask  = 0x00000700;
        static const    UINT32 CS_L__DSZ__shift =          8;
        static const    UINT32 CS_L__DSZ__UU8   =          0; // bit 31-bit 24
        static const    UINT32 CS_L__DSZ__UL8   =          1; // bit 23-bit 16
        static const    UINT32 CS_L__DSZ__LU8   =          2; // bit 15-bit 8
        static const    UINT32 CS_L__DSZ__LL8   =          3; // bit 7-bit 0
        static const    UINT32 CS_L__DSZ__U16   =          4; // bit 31- bit 16
        static const    UINT32 CS_L__DSZ__L16   =          5; // bit 15- bit 0
        static const    UINT32 CS_L__DSZ__32    =          6; // bit 31- bit 0
        static const    UINT32 CS_L__SP         = 0x00000040;
        static const    UINT32 CS_L__WP         = 0x00000010;
        static const    UINT32 CS_L__PA         = 0x00000002;
        static const    UINT32 CS_L__CSEN       = 0x00000001;

        static const    UINT32 CS_L__NEEDED_CONTROL = CS_L__EBC | CS_L__DSZ__mask | CS_L__WP | CS_L__CSEN; // required fields mask

        __inline static UINT32 CS_L__DSZ__set( UINT32 w ) { return (w << CS_L__DSZ__shift) & CS_L__DSZ__mask; }
        __inline static UINT32 CS_L__CSA__set( UINT32 w ) { return (w << CS_L__CSA__shift) & CS_L__CSA__mask; }
        __inline static UINT32 CS_L__WEA__set( UINT32 w ) { return (w << CS_L__WEA__shift) & CS_L__WEA__mask; }
        __inline static UINT32 CS_L__OEN__set( UINT32 w ) { return (w << CS_L__OEN__shift) & CS_L__OEN__mask; }
        __inline static UINT32 CS_L__OEA__set( UINT32 w ) { return (w << CS_L__OEA__shift) & CS_L__OEA__mask; }
    };

    //--//

    static const UINT32 c_Base    = 0x00220000;
    static const UINT32 c_Devices =          6;

    //--//


    DEVICE                 Device[c_Devices];

    /****/ volatile UINT32 EIM;
    static const    UINT32 EIM__BCM = 0x00000004;
};
//
// EIM
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Clock
//
struct MC9328MXL_CMU
{
    static const UINT32 c_Base = 0x0021B000;


    /****/ volatile UINT32 CSCR;                                    // Clock Source Control Register.
    static const    UINT32 CSCR__CLKO_SEL_mask        = 0xE0000000; // CLKO Select - Selects the clock signal source that is output on the CLKO pin.
    static const    UINT32 CSCR__CLKO_SEL_shift       =         29;
    static const    UINT32 CSCR__CLKO_SEL__PERCLK1    =          0;
    static const    UINT32 CSCR__CLKO_SEL__HCLK       =          1;
    static const    UINT32 CSCR__CLKO_SEL__CLK48M     =          2;
    static const    UINT32 CSCR__CLKO_SEL__CLK16M     =          3;
    static const    UINT32 CSCR__CLKO_SEL__PREMCLK    =          4;
    static const    UINT32 CSCR__CLKO_SEL__FCLK       =          5;
    static const    UINT32 CSCR__USB_DIV_mask         = 0x1C000000; // USB Divider - Contains the integer divider value used to generate the CLK48M signal for the USB modules.
    static const    UINT32 CSCR__USB_DIV_shift        =         26; // 0 = divide by 1, 1 = divide by 2, etc.
    static const    UINT32 CSCR__SD_CNT_mask          = 0x03000000; // Shut-Down Control - Contains the value that sets the duration of System PLL clock output after 0 is written to the SPEN bit.
    static const    UINT32 CSCR__SD_CNT_shift         =         24;
    static const    UINT32 CSCR__SPLL_RESTART         = 0x00400000; // SPLL Restart - Restarts System PLL at new assigned frequency.
    static const    UINT32 CSCR__MPLL_RESTART         = 0x00200000; // MPLL Restart - Restarts the MCU PLL at a new assigned frequency.
    static const    UINT32 CSCR__CLK16_SEL            = 0x00040000; // CLK16 Select - Selects the clock source of the 16 MHz clock.
    static const    UINT32 CSCR__OSC_EN               = 0x00020000; // Oscillator Enable - Enables the 16 MHz oscillator circuit when set (available when using an external crystal input).
    static const    UINT32 CSCR__System_SEL           = 0x00010000; // System Select - Selects the clock source of the System PLL input.
    static const    UINT32 CSCR__PRESC_mask           = 0x00008000; // Prescaler - Defines the MPU PLL clock prescaler.
    static const    UINT32 CSCR__PRESC_shift          =         15;
    static const    UINT32 CSCR__PRESC__DIV_BY_1      =          0;
    static const    UINT32 CSCR__PRESC__DIV_BY_2      =          1;
    static const    UINT32 CSCR__BCLK_DIV_mask        = 0x000003C0; // BClock Divider - Contains the 4-bit integer divider values for the generation of the BCLK and HCLK.
    static const    UINT32 CSCR__BCLK_DIV_shift       =         10; // 0 = System PLLCLK divided by 1, 1 = System PLLCLK divided by 2, etc.
    static const    UINT32 CSCR__SPEN                 = 0x00000002; // System PLL Enable - Enables/Disables the System PLL.
    static const    UINT32 CSCR__MPEN                 = 0x00000001; // MCU PLL Enable - Enables/Disables the MCU PLL.

    /****/ volatile UINT32 MPCTL0;                                  // MCU PLL Control Register 0
    static const    UINT32 MPCTL0__PD_mask            = 0x3C000000; // Predivider Factor - Defines the predivider factor (PD) applied to the PLL input frequency.
    static const    UINT32 MPCTL0__PD_shift           =         26;
    static const    UINT32 MPCTL0__MFD_mask           = 0x03FF0000; // Multiplication Factor (Denominator Part) - Defines the denominator part of the BRM value for the MF.
    static const    UINT32 MPCTL0__MFD_shift          =         16;
    static const    UINT32 MPCTL0__MFI_mask           = 0x00003C00; // Multiplication Factor (Integer) - Defines the integer part of the BRM value for the MF.
    static const    UINT32 MPCTL0__MFI_shift          =         10;
    static const    UINT32 MPCTL0__MFN_mask           = 0x000003FF; // Multiplication Factor (Numerator) - Defines the numerator of the BRM value for the MF.
    static const    UINT32 MPCTL0__MFN_shift          =          0;

    /****/ volatile UINT32 MPCTL1;                                  // MCU PLL and System Clock Control Register 1.
    static const    UINT32 MPCTL1__BRMO               = 0x00000040; // BRM Order - Controls the BRM order.

    /****/ volatile UINT32 SPCTL0;                                  // System PLL Control Register 0.
    static const    UINT32 SPCTL0__PD_mask            = 0x3C000000; // Predivider Factor - Defines the predivider factor (PD) applied to the PLL input frequency.
    static const    UINT32 SPCTL0__PD_shift           =         26;
    static const    UINT32 SPCTL0__MFD_mask           = 0x03FF0000; // Multiplication Factor (Denominator Part) - Defines the denominator part of the BRM value for the MF.
    static const    UINT32 SPCTL0__MFD_shift          =         16;
    static const    UINT32 SPCTL0__MFI_mask           = 0x00003C00; // Multiplication Factor (Integer) - Defines the integer part of the BRM value for the MF.
    static const    UINT32 SPCTL0__MFI_shift          =         10;
    static const    UINT32 SPCTL0__MFN_mask           = 0x000003FF; // Multiplication Factor (Numerator) - Defines the numerator of the BRM value for the MF.
    static const    UINT32 SPCTL0__MFN_shift          =          0;

    /****/ volatile UINT32 SPCTL1;                                  // System PLL Control Register 1.
    static const    UINT32 SPCTL1__LF                 = 0x00008000; // Lock Flag - Indicates whether the System PLL is locked.
    static const    UINT32 SPCTL1__BRMO               = 0x00000040; // BRM Order - Controls the BRM order.

    /****/ volatile UINT32 Padding1[3];

    /****/ volatile UINT32 PCDR;                                    // Peripheral Clock Divider Register.
    static const    UINT32 PCDR__PCLK_DIV3_mask       = 0x007F0000; // Peripheral Clock Divider 3 - Contains the 7-bit integer divider that produces the PERCLK3 clock signal for the peripherals.
    static const    UINT32 PCDR__PCLK_DIV3_shift      =         16; // 0 = Divide by 1, 1 = Divide by 2, etc.
    static const    UINT32 PCDR__PCLK_DIV2_mask       = 0x000000F0; // Peripheral Clock Divider 2 - Contains the 4-bit integer divider that produces the PERCLK2 clock signal for the peripherals.
    static const    UINT32 PCDR__PCLK_DIV2_shift      =          4; // 0 = Divide by 1, 1 = Divide by 2, etc.
    static const    UINT32 PCDR__PCLK_DIV1_mask       = 0x0000000F; // Peripheral Clock Divider 1 — Contains the 4-bit integer divider that produces the PERCLK1 clock signal for the peripherals.
    static const    UINT32 PCDR__PCLK_DIV1_shift      =          0; // 0 = Divide by 1, 1 = Divide by 2, etc.
};
//
// Clock
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// System Control
//
struct MC9328MXL_SC
{
    static const UINT32 c_Base = 0x0021B800;


    /****/ volatile UINT32 Padding1;

    /****/ volatile UINT32 SIDR;                                    // Silicon ID Register.

    /****/ volatile UINT32 FMCR;                                    // Function Multiplexing Control Register.
    static const    UINT32 FMCR__SPI2_RXD_SEL         = 0x00000100; // Receive Data Input Select           - Selects the SPI2 Receive Data input source.
    static const    UINT32 FMCR__SSI_RXFS_SEL         = 0x00000080; // SSI Receive Frame Sync Input Select - Selects the Receive Frame Sync input source.
    static const    UINT32 FMCR__SSI_RXCLK_SEL        = 0x00000040; // SSI Receive Clock Select            - Selects the Receive Clock input source.
    static const    UINT32 FMCR__SSI_RXDAT_SEL        = 0x00000020; // SSI Receive Data Select             - Selects the Receive Data input source.
    static const    UINT32 FMCR__SSI_TXFS_SEL         = 0x00000010; // SSI Transmit Frame Sync Select      - Selects the Transmit Frame Sync input source.
    static const    UINT32 FMCR__SSI_TXCLK_SEL        = 0x00000008; // SSI Transmit Clock Select           - Selects the Transmit Clock input source.
    static const    UINT32 FMCR__EXT_BR_EN            = 0x00000004; // External Bus Request Control        - Chooses whether the external bus request function is masked or enabled.
    static const    UINT32 FMCR__SDCS1_SEL            = 0x00000002; // SDRAM/SyncFlash Chip-Select         - Selects the function of the CS3/CSD1 pin.
    static const    UINT32 FMCR__SDCS0_SEL            = 0x00000001; // SDRAM/SyncFlash Chip-Select         - Selects the function of the CS2/CSD0 pin.

    /****/ volatile UINT32 GPCR;                                    // Global Peripheral Control Register.
    static const    UINT32 GPCR__MEMC_TEST_EN         = 0x00008000; // Active high of this bit switches the inner test signals from MEMC to GPIO for debug purposes.
    static const    UINT32 GPCR__DS_SLOW_mask         = 0x00000C00; // Driving Strength Slow I/O - Controls the driving strength of all slow I/O signals.
    static const    UINT32 GPCR__DS_SLOW_shift        =         10;
    static const    UINT32 GPCR__DS_SLOW__26MHz_15pF  =          0;
    static const    UINT32 GPCR__DS_SLOW__26MHz_30pF  =          1;
    static const    UINT32 GPCR__DS_SLOW__26MHz_45pF  =          2;
    static const    UINT32 GPCR__DS_SLOW__26MHz_xxpF  =          3;
    static const    UINT32 GPCR__DS_CNTL_mask         = 0x00000300; // Driving Strength Bus Control Signals - Controls the driving strength of bus control signals.
    static const    UINT32 GPCR__DS_CNTL_shift        =          8;
    static const    UINT32 GPCR__DS_CNTL__50MHz_15pF  =          0;
    static const    UINT32 GPCR__DS_CNTL__50MHz_30pF  =          1;
    static const    UINT32 GPCR__DS_CNTL__100MHz_15pF =          2;
    static const    UINT32 GPCR__DS_CNTL__100MHz_30pF =          3;
    static const    UINT32 GPCR__DS_ADDR_mask         = 0x000000C0; // Driving Strength Address Bus - Controls the driving strength of the address bus.
    static const    UINT32 GPCR__DS_ADDR_shift        =          6;
    static const    UINT32 GPCR__DS_ADDR__50MHz_15pF  =          0;
    static const    UINT32 GPCR__DS_ADDR__50MHz_30pF  =          1;
    static const    UINT32 GPCR__DS_ADDR__100MHz_15pF =          2;
    static const    UINT32 GPCR__DS_ADDR__100MHz_30pF =          3;
    static const    UINT32 GPCR__DS_DATA_mask         = 0x00000030; // Driving Strength Data Bus - Controls the driving strength of the data bus.
    static const    UINT32 GPCR__DS_DATA_shift        =          4;
    static const    UINT32 GPCR__DS_DATA__50MHz_15pF  =          0;
    static const    UINT32 GPCR__DS_DATA__50MHz_30pF  =          1;
    static const    UINT32 GPCR__DS_DATA__100MHz_15pF =          2;
    static const    UINT32 GPCR__DS_DATA__100MHz_30pF =          3;
    static const    UINT32 GPCR__IP_CLK_GATING_EN     = 0x00000008; // IP Clock Gating Enabled - Controls ips_gated_clk signal.
    static const    UINT32 GPCR__HCLK_GATING_EN       = 0x00000004; // This bit only applies to functional and register access clocks; DMA, CSI, MMA and AITC.
    static const    UINT32 GPCR__MMA_PROT_EN          = 0x00000002; // MMA Privileged Mode Access - Selects whether the MMA can only be accessed in privileged mode or if it can be accessed in all modes.
    static const    UINT32 GPCR__CSI_PROT_EN          = 0x00000001; // CMOS Sensor Interface Privileged Mode Access - Selects whether the CSI can only be accessed in privileged mode or if it can be accessed in all modes.

    /****/ volatile UINT32 GCCR;                                    // Global Clock Control Register
    static const    UINT32 GCCR__BROM_CLK_EN          = 0x00000010; // BROM Clock Enable - Only available in Bootstrap mode.
    static const    UINT32 GCCR__DMA_CLK_EN           = 0x00000008; // DMA Clock Enable - Enables/Disables clock input to the DMA module.
    static const    UINT32 GCCR__CSI_CLK_EN           = 0x00000004; // CMOS Sensor Interface Clock Enable - Enables/Disables clock input to the CSI module.
    static const    UINT32 GCCR__MMA_CLK_EN           = 0x00000002; // Multimedia Accelerator Clock Enable - Enables/Disables clock input to the MMA module.
    static const    UINT32 GCCR__USBD_CLK_EN          = 0x00000001; // USBD Clock Enable - Enables/Disables clock input to the USB module.
};
//
// System Control
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// SDRAM Control
//
struct MC9328MXL_SDRAM
{
    static const UINT32 c_Base = 0x00221000;


    /****/ volatile UINT32 SDCTL[2];                                                   // SDRAM x Control Register.
    static const    UINT32 SDCTL__SDE_mask                               = 0x80000000; // SDRAM Controller Enable - Enables/Disables the SDRAM controller.
    static const    UINT32 SDCTL__SDE_shift                              =         31;
    static const    UINT32 SDCTL__SDE__disable                           =          0;
    static const    UINT32 SDCTL__SDE__enable                            =          1;
    static const    UINT32 SDCTL__SMODE_mask                             = 0x70000000; // SDRAM Controller Operating Mode - Determines the operating mode of the SDRAM controller.
    static const    UINT32 SDCTL__SMODE_shift                            =         28;
    static const    UINT32 SDCTL__SMODE__Normal_ReadWrite                =          0;
    static const    UINT32 SDCTL__SMODE__Precharge_Command               =          1;
    static const    UINT32 SDCTL__SMODE__Auto_Refresh_Command            =          2;
    static const    UINT32 SDCTL__SMODE__Set_Mode_Register_Command       =          3;
    static const    UINT32 SDCTL__SMODE__SyncFlash_Load_Command_Register =          6;
    static const    UINT32 SDCTL__SMODE__SyncFlash_Program_ReadWrite     =          7;
    static const    UINT32 SDCTL__SP                                     = 0x08000000; // Supervisor Protect - Restricts user accesses within the chip-select region.
    static const    UINT32 SDCTL__ROW_mask                               = 0x03000000; // Row Address Width - Specifies the number of row addresses used by the memory array.
    static const    UINT32 SDCTL__ROW_shift                              =         24;
    static const    UINT32 SDCTL__ROW__11                                =          0;
    static const    UINT32 SDCTL__ROW__12                                =          1;
    static const    UINT32 SDCTL__ROW__13                                =          2;
    static const    UINT32 SDCTL__ROW__Reserved                          =          3;
    static const    UINT32 SDCTL__COL_mask                               = 0x00300000; // Column Address Width - Specifies the number of column addresses in the memory array and will determine the break point in the address multiplexer.
    static const    UINT32 SDCTL__COL_shift                              =         20;
    static const    UINT32 SDCTL__COL__8                                 =          0;
    static const    UINT32 SDCTL__COL__9                                 =          1;
    static const    UINT32 SDCTL__COL__10                                =          2;
    static const    UINT32 SDCTL__COL__11                                =          3;
    static const    UINT32 SDCTL__IAM_mask                               = 0x00080000; // (Bank) Interleaved Address Mode - Controls bank address alignment.
    static const    UINT32 SDCTL__IAM_shift                              =         19;
    static const    UINT32 SDCTL__IAM__disable                           =          0;
    static const    UINT32 SDCTL__IAM__enable                            =          1;
    static const    UINT32 SDCTL__DSIZ_mask                              = 0x00030000; // SDRAM Memory Data Width - Defines the width of the SDRAM memory and its alignment on the external data bus.
    static const    UINT32 SDCTL__DSIZ_shift                             =         16;
    static const    UINT32 SDCTL__DSIZ__16bit__31_16                     =          0;
    static const    UINT32 SDCTL__DSIZ__16bit__15_0                      =          1;
    static const    UINT32 SDCTL__DSIZ__32bit                            =          2;
    static const    UINT32 SDCTL__SREFR_mask                             = 0x0000C000; // Column Address Width - Specifies the number of column addresses in the memory array and will determine the break point in the address multiplexer.
    static const    UINT32 SDCTL__SREFR_shift                            =         14;
    static const    UINT32 SDCTL__SREFR__Disabled                        =          0;
    static const    UINT32 SDCTL__SREFR__1Row_per_clock                  =          1;
    static const    UINT32 SDCTL__SREFR__2Row_per_clock                  =          2;
    static const    UINT32 SDCTL__SREFR__4Row_per_clock                  =          3;
    static const    UINT32 SDCTL__CLKST_mask                             = 0x00003000; // Clock Suspend Time-out - Determines if and when the SDRAM will be placed in a clock suspend condition.
    static const    UINT32 SDCTL__CLKST_shift                            =         12;
    static const    UINT32 SDCTL__CLKST__Disabled                        =          0;
    static const    UINT32 SDCTL__CLKST__WhenInactive                    =          1;
    static const    UINT32 SDCTL__CLKST__PrechargePowerdown              =          2;
    static const    UINT32 SDCTL__CLKST__ActivePowerdown                 =          3;
    static const    UINT32 SDCTL__SCL_mask                               = 0x00000300; // SDRAM CAS Latency - Determines the latency between a read command and the availability of data on the bus.
    static const    UINT32 SDCTL__SCL_shift                              =          8;
    static const    UINT32 SDCTL__SCL__Reserved                          =          0;
    static const    UINT32 SDCTL__SCL__1_clock                           =          1;
    static const    UINT32 SDCTL__SCL__2_clocks                          =          2;
    static const    UINT32 SDCTL__SCL__3_clocks                          =          3;
    static const    UINT32 SDCTL__SRP_mask                               = 0x00000040; // SDRAM Row Precharge Delay - Determines the number of idle clocks inserted between a precharge command and the next-row activate command to the same bank.
    static const    UINT32 SDCTL__SRP_shift                              =          6;
    static const    UINT32 SDCTL__SRP__3clks                             =          0;
    static const    UINT32 SDCTL__SRP__2clks                             =          1;
    static const    UINT32 SDCTL__SRCD_mask                              = 0x00000030; // SDRAM Row-to-Column Delay - Determines the number of clocks inserted between a row activate command and a subsequent read or write command to the same bank.
    static const    UINT32 SDCTL__SRCD_shift                             =          4;
    static const    UINT32 SDCTL__SRCD__4_clocks                         =          0;
    static const    UINT32 SDCTL__SRCD__1_clock                          =          1;
    static const    UINT32 SDCTL__SRCD__2_clocks                         =          2;
    static const    UINT32 SDCTL__SRCD__3_clocks                         =          3;
    static const    UINT32 SDCTL__SRC_mask                               = 0x0000000F; // SDRAM Row Cycle Delay - Determines the minimum delay (in number of clocks) between a refresh and any subsequent refresh or read/write access.
    static const    UINT32 SDCTL__SRC_shift                              =          0;
    static const    UINT32 SDCTL__SRC__8_clocks                          =          0;
    static const    UINT32 SDCTL__SRC__1_clock                           =          1;
    static const    UINT32 SDCTL__SRC__2_clocks                          =          2;
    static const    UINT32 SDCTL__SRC__3_clocks                          =          3;
    static const    UINT32 SDCTL__SRC__4_clocks                          =          4;
    static const    UINT32 SDCTL__SRC__5_clocks                          =          5;
    static const    UINT32 SDCTL__SRC__6_clocks                          =          6;
    static const    UINT32 SDCTL__SRC__7_clocks                          =          7;

    /****/ volatile UINT32 Padding1[3];

    /****/ volatile UINT32 MISCELLANEOUS;                                              // Miscellaneous Register.
    static const    UINT32 MISCELLANEOUS__OMA                            = 0x80000000; // Multiplexed Address Override - Enables/Disables the MA0 pin to be a meaningful bit.
    static const    UINT32 MISCELLANEOUS__RMA0                           = 0x00000001; // MA0 Replacement - Contains value of MA0 when OMA is set.

    /****/ volatile UINT32 SDRST;                                                      // SDRAM Reset Register.
    static const    UINT32 SDRST__RST_mask                               = 0xC0000000; // Software Initiated Local Module Reset Bits - Generates local module reset to SDRAM/SyncFlash controller.
    static const    UINT32 SDRST__RST_shift                              =         30;
    static const    UINT32 SDRST__RST__noop                              =          0;
    static const    UINT32 SDRST__RST__1HCLK                             =          1;
    static const    UINT32 SDRST__RST__2HCLK                             =          2;
    static const    UINT32 SDRST__RST__3HCLK                             =          3;
};
//
// SDRAM Control
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller
//
struct MC9328MXL_AITC
{
    static const UINT32 c_Base = 0x00223000;

    //--//

    static const UINT32 c_IRQ_Priority_0  =  0;
    static const UINT32 c_IRQ_Priority_1  =  1;
    static const UINT32 c_IRQ_Priority_2  =  2;
    static const UINT32 c_IRQ_Priority_3  =  3;
    static const UINT32 c_IRQ_Priority_4  =  4;
    static const UINT32 c_IRQ_Priority_5  =  5;
    static const UINT32 c_IRQ_Priority_6  =  6;
    static const UINT32 c_IRQ_Priority_7  =  7;
    static const UINT32 c_IRQ_Priority_8  =  8;
    static const UINT32 c_IRQ_Priority_9  =  9;
    static const UINT32 c_IRQ_Priority_10 = 10;
    static const UINT32 c_IRQ_Priority_11 = 11;
    static const UINT32 c_IRQ_Priority_12 = 12;
    static const UINT32 c_IRQ_Priority_13 = 13;
    static const UINT32 c_IRQ_Priority_14 = 14;
    static const UINT32 c_IRQ_Priority_15 = 15;

    static const UINT32 c_IRQ_INDEX_unused0          =  0;
    static const UINT32 c_IRQ_INDEX_unused1          =  1;
    static const UINT32 c_IRQ_INDEX_unused2          =  2;
    static const UINT32 c_IRQ_INDEX_unused3          =  3;
    static const UINT32 c_IRQ_INDEX_unused4          =  4;
    static const UINT32 c_IRQ_INDEX_unused5          =  5;
    static const UINT32 c_IRQ_INDEX_CSI_INT          =  6;
    static const UINT32 c_IRQ_INDEX_MMA_MAC_INT      =  7;
    static const UINT32 c_IRQ_INDEX_MMA_INT          =  8;
    static const UINT32 c_IRQ_INDEX_unused9          =  9;
    static const UINT32 c_IRQ_INDEX_MSIRQ            = 10;
    static const UINT32 c_IRQ_INDEX_GPIO_INT_PORTA   = 11;
    static const UINT32 c_IRQ_INDEX_GPIO_INT_PORTB   = 12;
    static const UINT32 c_IRQ_INDEX_GPIO_INT_PORTC   = 13;
    static const UINT32 c_IRQ_INDEX_LCDC_INT         = 14;
    static const UINT32 c_IRQ_INDEX_unused15         = 15;
    static const UINT32 c_IRQ_INDEX_unused16         = 16;
    static const UINT32 c_IRQ_INDEX_RTC_INT          = 17;
    static const UINT32 c_IRQ_INDEX_RTC_SAM_INT      = 18;
    static const UINT32 c_IRQ_INDEX_UART2_MINT_PFERR = 19;
    static const UINT32 c_IRQ_INDEX_UART2_MINT_RTS   = 20;
    static const UINT32 c_IRQ_INDEX_UART2_MINT_DTR   = 21;
    static const UINT32 c_IRQ_INDEX_UART2_MINT_UARTC = 22;
    static const UINT32 c_IRQ_INDEX_UART2_MINT_TX    = 23;
    static const UINT32 c_IRQ_INDEX_UART2_MINT_RX    = 24;
    static const UINT32 c_IRQ_INDEX_UART1_MINT_PFERR = 25;
    static const UINT32 c_IRQ_INDEX_UART1_MINT_RTS   = 26;
    static const UINT32 c_IRQ_INDEX_UART1_MINT_DTR   = 27;
    static const UINT32 c_IRQ_INDEX_UART1_MINT_UARTC = 28;
    static const UINT32 c_IRQ_INDEX_UART1_MINT_TX    = 29;
    static const UINT32 c_IRQ_INDEX_UART1_MINT_RX    = 30;
    static const UINT32 c_IRQ_INDEX_unused31         = 31;
    static const UINT32 c_IRQ_INDEX_unused32         = 32;
    static const UINT32 c_IRQ_INDEX_unused33         = 33;
    static const UINT32 c_IRQ_INDEX_PWM_INT          = 34;
    static const UINT32 c_IRQ_INDEX_MMC_IRQ          = 35;
    static const UINT32 c_IRQ_INDEX_unused36         = 36;
    static const UINT32 c_IRQ_INDEX_unused37         = 37;
    static const UINT32 c_IRQ_INDEX_unused38         = 38;
    static const UINT32 c_IRQ_INDEX_I2C_INT          = 39;
    static const UINT32 c_IRQ_INDEX_SPI2_INT         = 40;
    static const UINT32 c_IRQ_INDEX_SPI1_INT         = 41;
    static const UINT32 c_IRQ_INDEX_SSI_TX_INT       = 42;
    static const UINT32 c_IRQ_INDEX_SSI_TX_ERR_INT   = 43;
    static const UINT32 c_IRQ_INDEX_SSI_RX_INT       = 44;
    static const UINT32 c_IRQ_INDEX_SSI_RX_ERR_INT   = 45;
    static const UINT32 c_IRQ_INDEX_unused46         = 46;
    static const UINT32 c_IRQ_INDEX_USBD_INT0        = 47;
    static const UINT32 c_IRQ_INDEX_USBD_INT1        = 48;
    static const UINT32 c_IRQ_INDEX_USBD_INT2        = 49;
    static const UINT32 c_IRQ_INDEX_USBD_INT3        = 50;
    static const UINT32 c_IRQ_INDEX_USBD_INT4        = 51;
    static const UINT32 c_IRQ_INDEX_USBD_INT5        = 52;
    static const UINT32 c_IRQ_INDEX_USBD_INT6        = 53;
    static const UINT32 c_IRQ_INDEX_unused54         = 54;
    static const UINT32 c_IRQ_INDEX_unused55         = 55;
    static const UINT32 c_IRQ_INDEX_unused56         = 56;
    static const UINT32 c_IRQ_INDEX_unused57         = 57;
    static const UINT32 c_IRQ_INDEX_TIMER2_INT       = 58;
    static const UINT32 c_IRQ_INDEX_TIMER1_INT       = 59;
    static const UINT32 c_IRQ_INDEX_DMA_ERR          = 60;
    static const UINT32 c_IRQ_INDEX_DMA_INT          = 61;
    static const UINT32 c_IRQ_INDEX_GPIO_INT_PORTD   = 62;
    static const UINT32 c_IRQ_INDEX_WDT_INT          = 63;

    //--//

    /****/ volatile UINT32 INTCTL;
    static const    UINT32 INTCTL__NIAD_BLOCK_BUS_ON_FI = 0x00080000;
    static const    UINT32 INTCTL__NIAD_BLOCK_BUS_ON_NI = 0x00100000;

    /****/ volatile UINT32 NIMASK;
    static const    UINT32 NIMASK__mask                 = 0x0000001F;
    static const    UINT32 NIMASK__shift                =          0;

    /****/ volatile UINT32 INTENNUM;
    static const    UINT32 INTENNUM__mask               = 0x0000003F;
    static const    UINT32 INTENNUM__shift              =          0;

    /****/ volatile UINT32 INTDISNUM;
    static const    UINT32 INTDISNUM__mask              = 0x0000003F;
    static const    UINT32 INTDISNUM__shift             =          0;

    /****/ volatile UINT32 INTENABLEH;
    /****/ volatile UINT32 INTENABLEL;

    /****/ volatile UINT32 INTTYPEH;
    /****/ volatile UINT32 INTTYPEL;

    /****/ volatile UINT32 NIPRIORITY7;
    /****/ volatile UINT32 NIPRIORITY6;
    /****/ volatile UINT32 NIPRIORITY5;
    /****/ volatile UINT32 NIPRIORITY4;
    /****/ volatile UINT32 NIPRIORITY3;
    /****/ volatile UINT32 NIPRIORITY2;
    /****/ volatile UINT32 NIPRIORITY1;
    /****/ volatile UINT32 NIPRIORITY0;
    static const    UINT32 NIPRIORITY__mask             = 0x0000000F;

    /****/ volatile UINT32 NIVECSR;
    static const    UINT32 NIVECSR__NIVECTOR__mask      = 0xFFFF0000; // Normal Interrupt Vector - Indicates vector index for the highest pending normal interrupt
    static const    UINT32 NIVECSR__NIVECTOR__shift     =         16;
    static const    UINT32 NIVECSR__NIPRILVL__mask      = 0x0000FFFF; // Normal Interrupt Priority Level - Indicates the priority level of the highest priority normal interrupt. This number can be written to the NIMASK to disable the current priority normal interrupts to build a reentrant normal interrupt system.
    static const    UINT32 NIVECSR__NIPRILVL__shift     =          0;

    /****/ volatile UINT32 FIVECSR;

    /****/ volatile UINT32 INTSRCH;
    /****/ volatile UINT32 INTSRCL;

    /****/ volatile UINT32 INTFRCH;
    /****/ volatile UINT32 INTFRCL;

    /****/ volatile UINT32 NIPNDH;
    /****/ volatile UINT32 NIPNDL;

    /****/ volatile UINT32 FIPNDH;
    /****/ volatile UINT32 FIPNDL;

    //--//

    static const UINT32 c_MaxInterruptIndex = 64;

    //--//

    BOOL IsInterruptEnabled( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < 64);

        volatile UINT32* IntEnable = (Irq_Index < 32) ? &INTENABLEL : &INTENABLEH;

        Irq_Index %= 32;

        return (*IntEnable >> Irq_Index) & 0x1;
    }

    BOOL GetInterruptState( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < 64);

        volatile UINT32* IntSrc = (Irq_Index < 32) ? &INTSRCL : &INTSRCH;

        Irq_Index %= 32;

        return (*IntSrc >> Irq_Index) & 0x1;
    }

    UINT32 NormalInterruptPending()
    {
        UINT32 index;

        if((index = NIVECSR >> 16) < c_MaxInterruptIndex)
        {
            return index;
        }

        return c_MaxInterruptIndex;
    }

    UINT32 FastInterruptPending()
    {
        UINT32 index;

        if((index = FIVECSR) < c_MaxInterruptIndex)
        {
            return index;
        }

        return c_MaxInterruptIndex;
    }

    BOOL IsInterruptPending()
    {
        if((NormalInterruptPending() != c_MaxInterruptIndex))
        {
            return TRUE;
        }

        return FALSE;
    }

    void SetType( UINT32 Irq_Index, BOOL Fast )
    {
        ASSERT(Irq_Index < 64);

        volatile UINT32* INTTYPE = (Irq_Index < 32) ? &INTTYPEL : &INTTYPEH;
        
        Irq_Index %= 32;
        
        if(Fast) *INTTYPE |=  (1 << Irq_Index);
        else     *INTTYPE &= ~(1 << Irq_Index);
    }

    void SetPriority( UINT32 Irq_Index, UINT32 priority )
    {
        ASSERT(Irq_Index < 64);

        UINT32 offset = ((63 - Irq_Index) / 8);
        UINT32 shift  = (Irq_Index & 0xF) * 4 ;

        volatile UINT32* NIPRIORITY = &NIPRIORITY7 + offset;

        *NIPRIORITY = (*NIPRIORITY & ~(0xF << shift)) | ((priority & 0xF) << shift);
    }

    UINT32 GetPriority( UINT32 Irq_Index )
    {
        ASSERT(Irq_Index < 64);

        UINT32 offset = ((63 - Irq_Index) / 8);
        UINT32 shift  = (Irq_Index & 0xF) * 4 ;

        volatile UINT32* NIPRIORITY = &NIPRIORITY7 + offset;

        return (*NIPRIORITY >> shift) & 0xF;
    }

    void ForceInterrupt( UINT32 Irq_Index )
    {
        volatile UINT32* IntForce = (Irq_Index < 32) ? &INTFRCL : &INTFRCH;

        Irq_Index %= 32;

        *IntForce |= 1 << Irq_Index;
    }

    void RemoveForcedInterrupt( UINT32 Irq_Index )
    {
        volatile UINT32* IntForce = (Irq_Index < 32) ? &INTFRCL : &INTFRCH;

        Irq_Index %= 32;

        *IntForce &= ~(1 << Irq_Index);
    }
};
//
// Interrupt Controller
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// GP I/O
//
struct MC9328MXL_GPIO
{
    static const UINT32 c_Base        = 0x0021C000;
    static const UINT32 c_Base_Offset = 0x00000100;

    //--//

    static const UINT32 c_Pin_None  = 0xFFFFFFFF;
    static const UINT32 c_Port_A_00 =   0;
    static const UINT32 c_Port_A_01 =   1;
    static const UINT32 c_Port_A_02 =   2;
    static const UINT32 c_Port_A_03 =   3;
    static const UINT32 c_Port_A_04 =   4;
    static const UINT32 c_Port_A_05 =   5;
    static const UINT32 c_Port_A_06 =   6;
    static const UINT32 c_Port_A_07 =   7;
    static const UINT32 c_Port_A_08 =   8;
    static const UINT32 c_Port_A_09 =   9;
    static const UINT32 c_Port_A_10 =  10;
    static const UINT32 c_Port_A_11 =  11;
    static const UINT32 c_Port_A_12 =  12;
    static const UINT32 c_Port_A_13 =  13;
    static const UINT32 c_Port_A_14 =  14;
    static const UINT32 c_Port_A_15 =  15;
    static const UINT32 c_Port_A_16 =  16;
    static const UINT32 c_Port_A_17 =  17;
    static const UINT32 c_Port_A_18 =  18;
    static const UINT32 c_Port_A_19 =  19;
    static const UINT32 c_Port_A_20 =  20;
    static const UINT32 c_Port_A_21 =  21;
    static const UINT32 c_Port_A_22 =  22;
    static const UINT32 c_Port_A_23 =  23;
    static const UINT32 c_Port_A_24 =  24;
    static const UINT32 c_Port_A_25 =  25;
    static const UINT32 c_Port_A_26 =  26;
    static const UINT32 c_Port_A_27 =  27;
    static const UINT32 c_Port_A_28 =  28;
    static const UINT32 c_Port_A_29 =  29;
    static const UINT32 c_Port_A_30 =  30;
    static const UINT32 c_Port_A_31 =  31;
    // 0 => 7 unavailable on port B
    static const UINT32 c_Port_B_08 =  40;
    static const UINT32 c_Port_B_09 =  41;
    static const UINT32 c_Port_B_10 =  42;
    static const UINT32 c_Port_B_11 =  43;
    static const UINT32 c_Port_B_12 =  44;
    static const UINT32 c_Port_B_13 =  45;
    static const UINT32 c_Port_B_14 =  46;
    static const UINT32 c_Port_B_15 =  47;
    static const UINT32 c_Port_B_16 =  48;
    static const UINT32 c_Port_B_17 =  49;
    static const UINT32 c_Port_B_18 =  50;
    static const UINT32 c_Port_B_19 =  51;
    static const UINT32 c_Port_B_20 =  52;
    static const UINT32 c_Port_B_21 =  53;
    static const UINT32 c_Port_B_22 =  54;
    static const UINT32 c_Port_B_23 =  55;
    static const UINT32 c_Port_B_24 =  56;
    static const UINT32 c_Port_B_25 =  57;
    static const UINT32 c_Port_B_26 =  58;
    static const UINT32 c_Port_B_27 =  59;
    static const UINT32 c_Port_B_28 =  60;
    static const UINT32 c_Port_B_29 =  61;
    static const UINT32 c_Port_B_30 =  62;
    static const UINT32 c_Port_B_31 =  63;
    // 0 => 2 unavailable on port C
    static const UINT32 c_Port_C_03 =  67;
    static const UINT32 c_Port_C_04 =  68;
    static const UINT32 c_Port_C_05 =  69;
    static const UINT32 c_Port_C_06 =  70;
    static const UINT32 c_Port_C_07 =  71;
    static const UINT32 c_Port_C_08 =  72;
    static const UINT32 c_Port_C_09 =  73;
    static const UINT32 c_Port_C_10 =  74;
    static const UINT32 c_Port_C_11 =  75;
    static const UINT32 c_Port_C_12 =  76;
    static const UINT32 c_Port_C_13 =  77;
    static const UINT32 c_Port_C_14 =  78;
    static const UINT32 c_Port_C_15 =  79;
    static const UINT32 c_Port_C_16 =  80;
    static const UINT32 c_Port_C_17 =  81;
    // 18 => 31 unavailable on port C
    //  0 =>  5 unavailable on port D
    static const UINT32 c_Port_D_06 = 102;
    static const UINT32 c_Port_D_07 = 103;
    static const UINT32 c_Port_D_08 = 104;
    static const UINT32 c_Port_D_09 = 105;
    static const UINT32 c_Port_D_10 = 106;
    static const UINT32 c_Port_D_11 = 107;
    static const UINT32 c_Port_D_12 = 108;
    static const UINT32 c_Port_D_13 = 109;
    static const UINT32 c_Port_D_14 = 110;
    static const UINT32 c_Port_D_15 = 111;
    static const UINT32 c_Port_D_16 = 112;
    static const UINT32 c_Port_D_17 = 113;
    static const UINT32 c_Port_D_18 = 114;
    static const UINT32 c_Port_D_19 = 115;
    static const UINT32 c_Port_D_20 = 116;
    static const UINT32 c_Port_D_21 = 117;
    static const UINT32 c_Port_D_22 = 118;
    static const UINT32 c_Port_D_23 = 119;
    static const UINT32 c_Port_D_24 = 120;
    static const UINT32 c_Port_D_25 = 121;
    static const UINT32 c_Port_D_26 = 122;
    static const UINT32 c_Port_D_27 = 123;
    static const UINT32 c_Port_D_28 = 124;
    static const UINT32 c_Port_D_29 = 125;
    static const UINT32 c_Port_D_30 = 126;
    static const UINT32 c_Port_D_31 = 127;

    //--//

    /****/ volatile UINT32 DDIR_X;
    static const    UINT32 DDIR__IN        = 0;
    static const    UINT32 DDIR__OUT       = 1;

    /****/ volatile UINT32 OCR1_X;
    /****/ volatile UINT32 OCR2_X;
    static const    UINT32 OCR__mask       = 3;
    static const    UINT32 OCR__AIN_REG    = 0;
    static const    UINT32 OCR__BIN_REG    = 1;
    static const    UINT32 OCR__CIN_REG    = 2;
    static const    UINT32 OCR__DATA_REG   = 3;

    /****/ volatile UINT32 ICONFA1_X;
    /****/ volatile UINT32 ICONFA2_X;
    /****/ volatile UINT32 ICONFB1_X;
    /****/ volatile UINT32 ICONFB2_X;
    static const    UINT32 ICONF__mask     = 3;
    static const    UINT32 ICONF__GPIO_IN  = 0;
    static const    UINT32 ICONF__INT_STAT = 1;
    static const    UINT32 ICONF__0        = 2;
    static const    UINT32 ICONF__1        = 3;

    /****/ volatile UINT32 DR_X;

    /****/ volatile UINT32 GIUS_X;

    /****/ volatile UINT32 SSR_X;

    /****/ volatile UINT32 ICR1_X;
    /****/ volatile UINT32 ICR2_X;
    static const    UINT32 ICR__mask       = 3;
    static const    UINT32 ICR__INTRPE     = 0;
    static const    UINT32 ICR__INTRNE     = 1;
    static const    UINT32 ICR__INTRPL     = 2;
    static const    UINT32 ICR__INTRNL     = 3;

    /****/ volatile UINT32 IMR_X;

    /****/ volatile UINT32 ISR_X;

    /****/ volatile UINT32 GPR_X;
    static const    UINT32 GPR__PRIMARY    = 0;
    static const    UINT32 GPR__ALTERNATE  = 1;

    /****/ volatile UINT32 SWR_X;

    /****/ volatile UINT32 PUEN_X;
    static const    UINT32 PUEN__TRISTATED = 0;
    static const    UINT32 PUEN__PULLHIGH  = 1;

    //--//

    void SetInterruptMode( UINT32 bit, UINT32 intMode )
    {
        volatile UINT32* ICR;

        if(bit < 16)
        {
            ICR = &ICR1_X;
        }
        else
        {
            ICR = &ICR2_X;
        }

        bit %= 16; bit *= 2;

        *ICR = (*ICR & ~(ICR__mask << bit)) | (intMode << bit);
    }

    UINT32 GetInterruptMode( UINT32 bit )
    {
        volatile UINT32* ICR;

        if(bit < 16)
        {
            ICR = &ICR1_X;
        }
        else
        {
            ICR = &ICR2_X;
        }

        bit %= 16; bit *= 2;

        return (*ICR >> bit) & ICR__mask;
    }


};
//
// GP I/O
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// TIMERS
//
struct MC9328MXL_TIMER
{
    static const UINT32 c_Base_1 = 0x00202000;
    static const UINT32 c_Base_2 = 0x00203000;


    /****/ volatile UINT32 TCTL;
    static const    UINT32 TCTL_SWR                   = 0x00008000;
    static const    UINT32 TCTL_FRR                   = 0x00000100;
    static const    UINT32 TCTL_CAP_shift             =          6;
    static const    UINT32 TCTL_CAP_mask              = 0x000000C0;
    static const    UINT32 TCTL_OM                    = 0x00000020;
    static const    UINT32 TCTL_IRQEN                 = 0x00000010;
    static const    UINT32 TCTL_CLKSOURCE__DISABLED   =          0;
    static const    UINT32 TCTL_CLKSOURCE__PERCLK1    =          1;
    static const    UINT32 TCTL_CLKSOURCE__PERCLK1_16 =          2;
    static const    UINT32 TCTL_CLKSOURCE__TIN        =          3;
    static const    UINT32 TCTL_CLKSOURCE__32KHZ      =          4;
    static const    UINT32 TCTL_CLKSOURCE_shift       =          1;
    static const    UINT32 TCTL_CLKSOURCE_mask        = 0x0000000E;
    static const    UINT32 TCTL_TEN                   = 0x00000001;

    /****/ volatile UINT32 TPRER;
    static const    UINT32 TPRER_PRESCALER            = 0x000000FF;

    /****/ volatile UINT32 TCMP;

    /****/ volatile UINT32 TCR;

    /****/ volatile UINT32 TCN;

    //
    // These bits are cleared by writing 0x0, write 0 to clear - w0c,
    // and will clear only if they have been read while set.
    //
    /****/ volatile UINT32 TSTAT;
    static const    UINT32 TSTAT_CAPT                 = 0x00000002;
    static const    UINT32 TSTAT_COMP                 = 0x00000001;
};
//
// TIMER
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// USART
//
struct MC9328MXL_USART
{
    static const UINT32 c_Base_0    = 0x00206000;
    static const UINT32 c_Base_1    = 0x00207000;

    //--//

    static const UINT32 c_SER1_CTS  = MC9328MXL_GPIO::c_Port_C_09;
    static const UINT32 c_SER1_RTS  = MC9328MXL_GPIO::c_Port_C_10;
    static const UINT32 c_SER1_TDX  = MC9328MXL_GPIO::c_Port_C_11;
    static const UINT32 c_SER1_RDX  = MC9328MXL_GPIO::c_Port_C_12;
    static const UINT32 c_SER2_CTS  = MC9328MXL_GPIO::c_Port_B_28;
    static const UINT32 c_SER2_RTS  = MC9328MXL_GPIO::c_Port_B_29;
    static const UINT32 c_SER2_TDX  = MC9328MXL_GPIO::c_Port_B_30;
    static const UINT32 c_SER2_RDX  = MC9328MXL_GPIO::c_Port_B_31;

    static const UINT32 c_FIFO_SIZE = 32;

    static const UINT32 c_MAX_BAUDRATE = 1000000; // 1 MHz
    static const UINT32 c_MIN_BAUDRATE = 0;

    //--//

    /****/ volatile UINT32 URXDn_X[16];
    static const    UINT32 URXDn__CHARRDY   = 0x00008000;
    static const    UINT32 URXDn__ERR       = 0x00004000;
    static const    UINT32 URXDn__OVRRUN    = 0x00002000;
    static const    UINT32 URXDn__FRMERR    = 0x00001000;
    static const    UINT32 URXDn__BRK       = 0x00000800;
    static const    UINT32 URXDn__PRERR     = 0x00000400;
    static const    UINT32 URXDn__DATA_mask = 0x000000FF;
    static const    UINT32 URXDn__ERRORS    = URXDn__ERR | URXDn__OVRRUN | URXDn__BRK | URXDn__PRERR;

    /****/ volatile UINT32 UTXDn_X[16];
    static const    UINT32 UTXDn__DATA_mask = 0x000000FF;

    /****/ volatile UINT32 UCR1_X;
    static const    UINT32 UCR1__ADEN       = 0x00008000; // enable automatic baud rate detection interrupt    d
    static const    UINT32 UCR1__ADBR       = 0x00004000; // enable automatic baud rate detection              d
    static const    UINT32 UCR1__TRDYEN     = 0x00002000; // enable transmitter ready interrupt                d
    static const    UINT32 UCR1__IDEN       = 0x00001000; // enable IDLE bit                                   d
    static const    UINT32 UCR1__ICD_mask   = 0x00000C00; // number of frames of idleness
    static const    UINT32 UCR1__RRDYEN     = 0x00000200; // enable receiver ready interrupt                   e
    static const    UINT32 UCR1__RDMAEN     = 0x00000100; // enable rx DMA request                             d
    static const    UINT32 UCR1__IREN       = 0x00000080; // enable IR interface                               d
    static const    UINT32 UCR1__TXMPTYEN   = 0x00000040; // enable transmitter FIFO empty interrupt           e
    static const    UINT32 UCR1__RTSDEN     = 0x00000020; // enable RTSD interrupt                             d
    static const    UINT32 UCR1__SNDBRK     = 0x00000010; // send a break character                            d
    static const    UINT32 UCR1__TDMAEN     = 0x00000008; // enable tx DMA request                             d
    static const    UINT32 UCR1__UARTCLKEN  = 0x00000004; // enable USART clocks                               e
    static const    UINT32 UCR1__DOZE       = 0x00000002; // disable UART when in  doze state                  d
    static const    UINT32 UCR1__UARTEN     = 0x00000001; // enable UART                                       e

    /****/ volatile UINT32 UCR2_X;
    static const    UINT32 UCR2__ESCI       = 0x00008000; // enable the escape sequence interrupt              d
    static const    UINT32 UCR2__IRTS       = 0x00004000; // ignore the UARTx_RTS_L pin                        e
    static const    UINT32 UCR2__CTSC       = 0x00002000; // the UART_CTS_L is controlled by the receiver      d
    static const    UINT32 UCR2__CTS        = 0x00001000; // the UART_CTS_L is asserted                        d
    static const    UINT32 UCR2__ESCEN      = 0x00000800; // enable escape sequence detection                  d
    static const    UINT32 UCR2__RTEC_mask  = 0x00000600; // request to send edge control
    static const    UINT32 UCR2__PREN       = 0x00000100; // enable parity and checker                         d
    static const    UINT32 UCR2__PROE       = 0x00000080; // odd parity
    static const    UINT32 UCR2__STPB       = 0x00000040; // 2 stop bits transmitted                           d
    static const    UINT32 UCR2__WS         = 0x00000020; // 8-bit transmit and receive character length       e
    static const    UINT32 UCR2__RTSEN      = 0x00000010; // enalbe request to send interrupt                  d
    static const    UINT32 UCR2__TXEN       = 0x00000004; // enable the trasmitter                             e
    static const    UINT32 UCR2__RXEN       = 0x00000002; // enable the receiver                               e
    static const    UINT32 UCR2__SRST_L     = 0x00000001; // no reset (write 0 to reset)

    /****/ volatile UINT32 UCR3_X;
    static const    UINT32 UCR3__PARERREN   = 0x00001000; // enable the parity error interrupt                 d
    static const    UINT32 UCR3__FRAERREN   = 0x00000800; // enable the frame error interrupt                  d
    static const    UINT32 UCR3__RXDSEN     = 0x00000020; // enable the RXDS (idle) interrupt                  d
    static const    UINT32 UCR3__AIRINTEN   = 0x00000010; // enable the async IR wake interrupt                d
    static const    UINT32 UCR3__AWAKEN     = 0x00000008; // enable the awake interrupt                        d
    static const    UINT32 UCR3__REF25      = 0x00000004; // 25Mhz reference clock used                        d
    static const    UINT32 UCR3__REF30      = 0x00000004; // 30Mhz reference clock used                        d
    static const    UINT32 UCR3__INVT       = 0x00000002; // Inverted infrared transmission
    static const    UINT32 UCR3__BPEN       = 0x00000001; // BRM preset registers are used                     d

    /****/ volatile UINT32 UCR4_X;
    static const    UINT32 UCR4__CTSTL_mask = 0x0000FC00; // CTS trigger level
    static const    UINT32 UCR4__CTSTL_shift=         10;
    static const    UINT32 UCR4__INVR       = 0x00000200; // Inverted infrared reception
    static const    UINT32 UCR4__ENIRI      = 0x00000100; // serial infrared input enabled                     d
    static const    UINT32 UCR4__WKEN       = 0x00000080; // enable the WAKE interrupt                         d
    static const    UINT32 UCR4__REF16      = 0x00000040; // 16Mhz ref clock is used                           e
    static const    UINT32 UCR4__IRSC       = 0x00000020; // IR special case
    static const    UINT32 UCR4__TCEN       = 0x00000008; // enable transmit complete interrupt                d
    static const    UINT32 UCR4__BKEN       = 0x00000004; // enable the break detection interrupt              d
    static const    UINT32 UCR4__OREN       = 0x00000002; // enable receiver overrun interrupt                 d
    static const    UINT32 UCR4__DREN       = 0x00000001; // enable DRD                                        d

    /****/ volatile UINT32 UFCR_X;
    static const    UINT32 UFCR__TXTL_mask   = 0x0000FC00; // Tx fifo lower threshold
    static const    UINT32 UFCR__TXTL_shift  =         10; //
    static const    UINT32 UFCR__RFDIV_mask  = 0x00000380; // divide ratio for reference clock
    static const    UINT32 UFCR__RFDIV_shift =          7; //
    static const    UINT32 UFCR__RXTL_mask   = 0x0000003F; // Rx fifo upper threshold
    static const    UINT32 UFCR__RXTL_shift  =          0; //

    /****/ volatile UINT32 USR1_X;
    static const    UINT32 USR1__PARITYERR   = 0x00008000; // enable parity error detection (clear writing 1)   d
    static const    UINT32 USR1__RTSS        = 0x00004000; // RTS pin status
    static const    UINT32 USR1__TRDY        = 0x00002000; // Tx ready interrupt
    static const    UINT32 USR1__RTSD        = 0x00001000; // UARTx_RTS_L pin changed state
    static const    UINT32 USR1__ESCF        = 0x00000800; // escape sequence detected
    static const    UINT32 USR1__FRAMEERR    = 0x00000400; // frame error detected
    static const    UINT32 USR1__RRDY        = 0x00000200; // Rx ready interrupt
    static const    UINT32 USR1__RXDS        = 0x00000020; // Rx idle
    static const    UINT32 USR1__AIRINT      = 0x00000010; // pulse on RXD pin
    static const    UINT32 USR1__AWAKE       = 0x00000080; // falling edge on RXD pin

    /****/ volatile UINT32 USR2_X;
    static const    UINT32 USR2__ADET        = 0x00008000; // automatic baud rate detection complete
    static const    UINT32 USR2__TXFE        = 0x00004000; // Tx FIFO is empty
    static const    UINT32 USR2__IDLE        = 0x00001000; // idle consition detected
    static const    UINT32 USR2__WAKE        = 0x00000080; // start bit detected
    static const    UINT32 USR2__TXDC        = 0x00000008; // Tx is complete
    static const    UINT32 USR2__BRCD        = 0x00000004; // break condition detected
    static const    UINT32 USR2__ORE         = 0x00000002; // overrun error
    static const    UINT32 USR2__RDR         = 0x00000001; // receive data ready

    /****/ volatile UINT32 UESC_X;
    /****/ volatile UINT32 UTIM_X;
    /****/ volatile UINT32 UBIR_X;
    /****/ volatile UINT32 UBMR_X;
    /****/ volatile UINT32 UBRC_X;
    /****/ volatile UINT32 BIPR1_X;
    /****/ volatile UINT32 BIPR2_X;
    /****/ volatile UINT32 BIPR3_X;
    /****/ volatile UINT32 BIPR4_X;
    /****/ volatile UINT32 BMPR1_X;
    /****/ volatile UINT32 BMPR2_X;
    /****/ volatile UINT32 BMPR3_X;
    /****/ volatile UINT32 BMPR4_X;
    /****/ volatile UINT32 URTS_X;
};
//
// USART
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// USB
// 
struct MC9328MXL_USB
{

    struct EndPoint{

    /****/ volatile UINT32 EPn_STAT;
    static const UINT32 EPn_STAT_BYTE_CNT   = 0x007F0000;
    static const UINT32 EPn_STAT_BYTE_CNT_shift = 16;
    static const UINT32 EPn_STAT_SIP        = 0x00000100;
    static const UINT32 EPn_STAT_DIR        = 0x00000080;
    static const UINT32 EPn_STAT_DIR_shift  = 7;
    static const UINT32 EPn_STAT_DIR_OUT    = 0 << EPn_STAT_DIR_shift;
    static const UINT32 EPn_STAT_DIR_IN     = 1 << EPn_STAT_DIR_shift;
    static const UINT32 EPn_STAT_MAX        = 0x00000060;
    static const UINT32 EPn_STAT_MAX_shift  = 5;
    static const UINT32 EPn_STAT_MAX_8byte  = 0 << EPn_STAT_MAX_shift;
    static const UINT32 EPn_STAT_MAX_16byte = 1 << EPn_STAT_MAX_shift;        
    static const UINT32 EPn_STAT_MAX_32byte = 2 << EPn_STAT_MAX_shift;
    static const UINT32 EPn_STAT_MAX_64byte = 3 << EPn_STAT_MAX_shift;    
    static const UINT32 EPn_STAT_TYP        = 0x00000018;
    static const UINT32 EPn_STAT_TYP_shift  = 3;
    static const UINT32 EPn_STAT_TYP_CTRL   = 0 << EPn_STAT_TYP_shift;
    static const UINT32 EPn_STAT_TYP_ISO    = 1 << EPn_STAT_TYP_shift;
    static const UINT32 EPn_STAT_TYP_BULK   = 2 << EPn_STAT_TYP_shift;
    static const UINT32 EPn_STAT_TYP_INTR   = 3 << EPn_STAT_TYP_shift;
    
    static const UINT32 EPn_STAT_ZLPS       = 0x00000004;
    static const UINT32 EPn_STAT_FLUSH      = 0x00000002;
    static const UINT32 EPn_STAT_FORCE_STALL= 0x00000001;
    
    /****/ volatile UINT32 EPn_INTR;
    static const UINT32 EPn_INTR_FIFO_FULL  = 0x00000100;
    static const UINT32 EPn_INTR_FIFO_EMPTY = 0x00000080;
    static const UINT32 EPn_INTR_FIFO_ERROR = 0x00000040;
    static const UINT32 EPn_INTR_FIFO_HIGH  = 0x00000020;
    static const UINT32 EPn_INTR_FIFO_LOW   = 0x00000010;
    static const UINT32 EPn_INTR_MDEVREQ    = 0x00000008;
    static const UINT32 EPn_INTR_EOT        = 0x00000004;
    static const UINT32 EPn_INTR_DEVREQ     = 0x00000002;
    static const UINT32 EPn_INTR_EOF        = 0x00000001;
    static const UINT32 EPn_INTR_ALL_CLEAR  = EPn_INTR_FIFO_FULL | EPn_INTR_FIFO_EMPTY | EPn_INTR_FIFO_ERROR | EPn_INTR_FIFO_HIGH | EPn_INTR_FIFO_LOW
                                                | EPn_INTR_MDEVREQ | EPn_INTR_EOT | EPn_INTR_DEVREQ | EPn_INTR_EOF;

    /****/ volatile UINT32 EPn_MASK;
    static const UINT32 EPn_MASK_FIFO_FULL  = 0xFFFFFEFF;
    static const UINT32 EPn_MASK_FIFO_EMPTY = 0xFFFFFF7F;
    static const UINT32 EPn_MASK_FIFO_ERROR = 0xFFFFFFBF;
    static const UINT32 EPn_MASK_FIFO_HIGH  = 0xFFFFFFDF;
    static const UINT32 EPn_MASK_FIFO_LOW   = 0xFFFFFFEF;
    static const UINT32 EPn_MASK_MDEVREQ    = 0xFFFFFFF7;
    static const UINT32 EPn_MASK_EOT        = 0xFFFFFFFB;
    static const UINT32 EPn_MASK_DEVREQ     = 0xFFFFFFFD;
    static const UINT32 EPn_MASK_EOF        = 0xFFFFFFFE;    
    static const UINT32 EPn_MASK_ALL_ON     = EPn_MASK_FIFO_FULL & EPn_MASK_FIFO_EMPTY & EPn_MASK_FIFO_ERROR & EPn_MASK_FIFO_HIGH & EPn_MASK_FIFO_LOW
                                                & EPn_MASK_MDEVREQ & EPn_MASK_EOT & EPn_MASK_DEVREQ & EPn_MASK_EOF;
    static const UINT32 EPn_MASK_ALL_OFF    = 0x000001FF;

    /****/ volatile UINT32 EPn_FDAT;
    
    /****/ volatile UINT32 EPn_FSTAT;
    static const UINT32 EPn_FSTAT_FRAME0    = 0x08000000;
    static const UINT32 EPn_FSTAT_FRAME1    = 0x04000000;
    static const UINT32 EPn_FSTAT_FRAME2    = 0x02000000;
    static const UINT32 EPn_FSTAT_FRAME3    = 0x01000000;
    static const UINT32 EPn_FSTAT_ERROR     = 0x00400000;
    static const UINT32 EPn_FSTAT_UF        = 0x00200000;
    static const UINT32 EPn_FSTAT_OF        = 0x00100000;
    static const UINT32 EPn_FSTAT_FR        = 0x00080000;
    static const UINT32 EPn_FSTAT_FULL      = 0x00040000;
    static const UINT32 EPn_FSTAT_ALARM     = 0x00020000;    
    static const UINT32 EPn_FSTAT_EMPTY     = 0x00010000;

    /****/ volatile UINT32 EPn_FCTRL;
    static const UINT32 EPn_FCTRL_WFR       = 0x20000000;
    static const UINT32 EPn_FCTRL_FRAME     = 0x08000000;
    static const UINT32 EPn_FCRTL_GR        = 0x07000000;
    static const UINT32 EPn_FCTRL_GR_shift  = 24;
    static const UINT32 EPn_FCTRL_GR_1byte  = 0 << EPn_FCTRL_GR_shift;    
    static const UINT32 EPn_FCTRL_GR_2byte  = 1 << EPn_FCTRL_GR_shift;    
    static const UINT32 EPn_FCTRL_GR_3byte  = 2 << EPn_FCTRL_GR_shift;    
    static const UINT32 EPn_FCTRL_GR_4byte  = 3 << EPn_FCTRL_GR_shift;    
    static const UINT32 EPn_FCTRL_GR_5byte  = 4 << EPn_FCTRL_GR_shift;    
    static const UINT32 EPn_FCTRL_GR_6byte  = 5 << EPn_FCTRL_GR_shift;    
    static const UINT32 EPn_FCTRL_GR_7byte  = 6 << EPn_FCTRL_GR_shift;    
    static const UINT32 EPn_FCTRL_GR_8byte  = 7 << EPn_FCTRL_GR_shift;    
    
    /****/ volatile UINT32 EPn_LRFP;
    static const UINT32 EPn_LRFP_LRFP       = 0x0000003F;
    
    /****/ volatile UINT32 EPn_LWFP;
    static const UINT32 EPn_LWFP_LWFP       = 0x0000003F;
    
    /****/ volatile UINT32 EPn_FALRM;
    static const UINT32 EPn_FALRM_FALRM     = 0x0000003F;

    /****/ volatile UINT32 EPn_FRDP;
    static const UINT32 EPn_FRDP_RP         = 0x0000003F;
    
    /****/ volatile UINT32 EPn_FWRP;
    static const UINT32 EPn_FWRP_WP         = 0x0000003F;    

    /****/ volatile UINT32 Padding[1];

    };



    static const UINT32 c_Base = 0x00212000;
    /****/ volatile UINT32 USB_FRAME;
    static const UINT32 USB_FRAME_MATCH     = 0x07FF0000;
    static const UINT32 USB_FRAME_FRAME     = 0x000007FF;

    /****/ volatile UINT32 USB_SPEC;
    static const UINT32 USB_SPEC_NO         = 0x00000FFF;    

    /****/ volatile UINT32 USB_STAT;
    static const UINT32 USB_STAT_RST_shift  = 8;
    static const UINT32 USB_STAT_RST        = 0x100;
    static const UINT32 USB_STAT_SUSP_shift = 7;
    static const UINT32 USB_STAT_SUSP       = 0x80;
    static const UINT32 USB_STAT_CFG_shift  = 5;
    static const UINT32 USB_STAT_CFG        = 0x60;
    static const UINT32 USB_STAT_INTF_shift  = 3;
    static const UINT32 USB_STAT_INTF       = 0x18;
    static const UINT32 USB_STAT_ALTSET     = 0x7;
        
    /****/ volatile UINT32 USB_CTRL;
    static const UINT32 USB_CTRL_CMD_OVER   = 0x40;
    static const UINT32 USB_CTRL_CMD_ERROR  = 0x20;
    static const UINT32 USB_CTRL_SPD        = 0x10;
    static const UINT32 USB_CTRL_ENA        = 0x08;
    static const UINT32 USB_CTRL_UDC_RST    = 0x04;
    static const UINT32 USB_CTRL_AFE_ENA    = 0x02;
    static const UINT32 USB_CTRL_RESUME     = 0x01;
    
    /****/ volatile UINT32 USB_DADR;
    static const UINT32 USB_DADR_CFG        = 0x80000000;
    static const UINT32 USB_DADR_BSY        = 0x40000000;
    static const UINT32 USB_DADR_DADR       = 0x000001FF;
        
    /****/ volatile UINT32 USB_DDAT;
    static const UINT32 USB_DDAT_DDAT       = 0x000000FF;
    
    
    /****/ volatile UINT32 USB_INTR;
    static const UINT32 USB_INTR_WAKEUP     = 0x80000000;
    static const UINT32 USB_INTR_MSOF       = 0x00000080;
    static const UINT32 USB_INTR_SOF        = 0x00000040;
    static const UINT32 USB_INTR_RESET_STOP = 0x00000020;
    static const UINT32 USB_INTR_RESET_START= 0x00000010;
    static const UINT32 USB_INTR_RES        = 0x00000008;
    static const UINT32 USB_INTR_SUSP       = 0x00000004;
    static const UINT32 USB_INTR_FR_MATCH   = 0x00000002;
    static const UINT32 USB_INTR_CFG_CHG    = 0x00000001;
    static const UINT32 USB_INTR_ALL_CLEAR  = USB_INTR_WAKEUP | USB_INTR_MSOF | USB_INTR_SOF | USB_INTR_RESET_STOP | USB_INTR_RESET_START     
                                                 | USB_INTR_RES | USB_INTR_SUSP | USB_INTR_FR_MATCH | USB_INTR_CFG_CHG ;

    
    /****/ volatile UINT32 USB_MASK;
    static const UINT32 USB_MASK_WAKEUP     = 0x7FFFFFFF;
    static const UINT32 USB_MASK_MSOF       = 0xFFFFFF7F;
    static const UINT32 USB_MASK_SOF        = 0xFFFFFFBF;
    static const UINT32 USB_MASK_RESET_STOP = 0xFFFFFFDF;
    static const UINT32 USB_MASK_RESET_START= 0xFFFFFFEF;
    static const UINT32 USB_MASK_RES        = 0xFFFFFFF7;
    static const UINT32 USB_MASK_SUSP       = 0xFFFFFFFB;
    static const UINT32 USB_MASK_FR_MATCH   = 0xFFFFFFFD;
    static const UINT32 USB_MASK_CFG_CHG    = 0xFFFFFFFE;
    static const UINT32 USB_MASK_ALL_ON     = USB_MASK_WAKEUP & USB_MASK_MSOF & USB_MASK_SOF & USB_MASK_RESET_STOP & USB_MASK_RESET_START     
                                                 & USB_MASK_RES & USB_MASK_SUSP & USB_MASK_FR_MATCH & USB_MASK_CFG_CHG ;
    static const UINT32 USB_MASK_ALL_OFF    = 0x800000FF;    

    /****/ volatile UINT32 Padding0;
    
    /****/ volatile UINT32 USB_ENAB;
    static const UINT32 USB_ENAB_RST        = 0x80000000;
    static const UINT32 USB_ENAB_ENAB       = 0x40000000;
    static const UINT32 USB_ENAB_SUSPEND    = 0x20000000;
    static const UINT32 USB_ENAB_ENDIAN_MD  = 0x10000000;
    static const UINT32 USB_ENAB_PWRMD      = 0x00000001;
        
    /****/ volatile UINT32 Padding1[2];
    static const UINT32 c_USB_MAX_EP         = 6;
    static const UINT32 c_USB_MAX_LOGICAL_EP = 11;
   /****/ EndPoint EP[c_USB_MAX_EP];




    // multiplex USB pin with GPIO
    static const UINT32 c_USBD_AFE_PIN_MUX_GPIO_B20     = MC9328MXL_GPIO::c_Port_B_20;
    static const UINT32 c_USBD_AFE_PIN_DIR              = MC9328MXL_GPIO::DDIR__OUT;
    static const UINT32 c_USBD_SUSPND_PIN_MUX_GPIO_B23  = MC9328MXL_GPIO::c_Port_B_23;
    static const UINT32 c_USBD_SUSPND_PIN_DIR           = MC9328MXL_GPIO::DDIR__OUT;  
    static const UINT32 c_USBD_VMO_PIN_MUX_GPIO_B27     = MC9328MXL_GPIO::c_Port_B_27;
    static const UINT32 c_USBD_VMO_PIN_DIR              = MC9328MXL_GPIO::DDIR__OUT;
    static const UINT32 c_USBD_VPO_PIN_MUX_GPIO_B26     = MC9328MXL_GPIO::c_Port_B_26;
    static const UINT32 c_USBD_VPO_PIN_DIR              = MC9328MXL_GPIO::DDIR__OUT;
    static const UINT32 c_USBD_ROE_PIN_MUX_GPIO_B21     = MC9328MXL_GPIO::c_Port_B_21;
    static const UINT32 c_USBD_ROE_PIN_DIR              = MC9328MXL_GPIO::DDIR__OUT;
    static const UINT32 c_USBD_VM_PIN_MUX_GPIO_B25      = MC9328MXL_GPIO::c_Port_B_25;
    static const UINT32 c_USBD_VM_PIN_DIR               = MC9328MXL_GPIO::DDIR__IN;
    static const UINT32 c_USBD_VP_PIN_MUX_GPIO_B24      = MC9328MXL_GPIO::c_Port_B_24;
    static const UINT32 c_USBD_VP_PIN_DIR               = MC9328MXL_GPIO::DDIR__IN;
    static const UINT32 c_USBD_RCV_PIN_MUX_GPIO_B22     = MC9328MXL_GPIO::c_Port_B_22;
    static const UINT32 c_USBD_RCV_PIN_DIR              = MC9328MXL_GPIO::DDIR__IN;

    union EndpointConfiguration
    {
        struct
        {
            unsigned FIFONUM    :  3;       // Hardware Endpoint FIFO number (0-5)
            unsigned Reserved   : 11;       // Reserved bits (set to 0)
            unsigned TRXTYP     :  2;       // 0 = Control, 3 = otherwise
            unsigned MAXPKTSIZE : 10;       // 8, 16, 32 or 64 only
            unsigned DIR        :  1;       // 0 = OUT, 1 = IN
            unsigned TYPE       :  2;       // 0 = CTRL, 1 = ISOCHR, 2 = BULK, 3 = INT
            unsigned ALTSETTING :  3;       // Interface alternate number
            unsigned INTERFACE  :  2;       // Interface number
            unsigned CONFIG     :  2;       // Configuration number = 1 or 2
            unsigned EPNUM      :  4;       // Logical endpoint number
        } bits;
        UINT32 words[2];
    };

    int MaxEPsize( int epNum )
    {
        switch( epNum )
        {
        case 0:
            return 8;
        case 1:
        case 2:
            return 64;
        case 3:
        case 4:
        case 5:
            return 32;
        }
        return 0;
    }

    void Set_EPBuf(EndpointConfiguration *endpointConfig)
    {
        // write bit 39-32
        while( USB_DADR & USB_DADR_BSY);
        USB_DDAT = endpointConfig->words[1] & USB_DDAT_DDAT;
        for( int i = 24; i >= 0; i -= 8 )
        {
            while( USB_DADR & USB_DADR_BSY);

            USB_DDAT = (endpointConfig->words[0] >> i) & USB_DDAT_DDAT;
        }
    }
};
//
// USB
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// I2C
//
struct MC9328MXL_I2C
{
    static const UINT32 c_Base = 0x00217000;

    //--//

    static const UINT32 c_I2C_SDA = MC9328MXL_GPIO::c_Port_A_15;
    static const UINT32 c_I2C_SCL = MC9328MXL_GPIO::c_Port_A_16;

    //--//

    /****/ volatile UINT32 IADR;
    static const    UINT32 IADR_mask  = 0x0000007E;

    /****/ volatile UINT32 IFDR;
    static const    UINT32 IFDR_mask  = 0x0000003F;

    /****/ volatile UINT32 I2CR;
    static const    UINT32 I2CR__IEN  = 0x00000080;
    static const    UINT32 I2CR__IIEN = 0x00000040;
    static const    UINT32 I2CR__MSTA = 0x00000020;
    static const    UINT32 I2CR__MTX  = 0x00000010;
    static const    UINT32 I2CR__TXAK = 0x00000008;
    static const    UINT32 I2CR__RSTA = 0x00000004;
    static const    UINT32 I2CR_mask  = 0x000000FC;

    /****/ volatile UINT32 I2SR;
    static const    UINT32 I2SR__ICF  = 0x00000080;
    static const    UINT32 I2SR__IAAS = 0x00000040;
    static const    UINT32 I2SR__IBB  = 0x00000020;
    static const    UINT32 I2SR__IAL  = 0x00000010;
    static const    UINT32 I2SR__SRW  = 0x00000004;
    static const    UINT32 I2SR__IIF  = 0x00000002;
    static const    UINT32 I2SR__RXAK = 0x00000001;
    static const    UINT32 I2SR_mask  = 0x000000F7;

    /****/ volatile UINT32 I2DR;
    static const    UINT32 I2DR_mask  = 0x000000FF;
};
//
// I2C
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// SPI
//
struct MC9328MXL_SPI
{

    static const UINT32 c_Base_1 = 0x00213000;
    static const UINT32 c_Base_2 = 0x00219000;

    //--//
    
    static const UINT32 c_SPI1_SPI_RDY = MC9328MXL_GPIO::c_Port_C_13;
    static const UINT32 c_SPI1_SCLK    = MC9328MXL_GPIO::c_Port_C_14;
    static const UINT32 c_SPI1_SS      = MC9328MXL_GPIO::c_Port_C_15;
    static const UINT32 c_SPI1_MISO    = MC9328MXL_GPIO::c_Port_C_16;
    static const UINT32 c_SPI1_MOSI    = MC9328MXL_GPIO::c_Port_C_17;


#if defined(PLATFORM_ARM_MC9328MXS)

    static const UINT32 c_MAX_SPI =1;
    static const UINT32 c_SPI1 = 0;
    static const UINT32 c_SPI2 = 1;    
    static const UINT32 c_SPI2_SCLK    = MC9328MXL_GPIO::c_Pin_None;
    static const UINT32 c_SPI2_SS      = MC9328MXL_GPIO::c_Pin_None;
    static const UINT32 c_SPI2_MISO    = MC9328MXL_GPIO::c_Pin_None;
    static const UINT32 c_SPI2_MOSI    = MC9328MXL_GPIO::c_Pin_None;
#else
    static const UINT32 c_MAX_SPI =2;
    static const UINT32 c_SPI1 = 0;
    static const UINT32 c_SPI2 = 1;
    static const UINT32 c_SPI2_SCLK    = MC9328MXL_GPIO::c_Port_D_07;
    static const UINT32 c_SPI2_SS      = MC9328MXL_GPIO::c_Port_D_08;
    static const UINT32 c_SPI2_MISO    = MC9328MXL_GPIO::c_Port_D_09;
    static const UINT32 c_SPI2_MOSI    = MC9328MXL_GPIO::c_Port_D_10;
#endif

    //--//

    /****/ volatile UINT32 RXDATAREG;

    /****/ volatile UINT32 TXDATAREG;

    /****/ volatile UINT32 CONTROLREG;
    static const    UINT32 CONTROLREG_DATARATE_SHIFT    = 13;
    static const    UINT32 CONTROLREG_DATARATE_MASK     = 0x0000E000;    
    static const    UINT32 CONTROLREG_DATARATE_INTERVAL = 0x00002000;
    static const    UINT32 CONTROLREG_DATARATE_DIV4     = 0;
    static const    UINT32 CONTROLREG_DATARATE_DIV8     = 0x00002000;
    static const    UINT32 CONTROLREG_DATARATE_DIV16    = 0x00004000;
    static const    UINT32 CONTROLREG_DATARATE_DIV32    = 0x00006000;
    static const    UINT32 CONTROLREG_DATARATE_DIV64    = 0x00008000;
    static const    UINT32 CONTROLREG_DATARATE_DIV128   = 0x0000A000;
    static const    UINT32 CONTROLREG_DATARATE_DIV256   = 0x0000C000;
    static const    UINT32 CONTROLREG_DATARATE_DIV512   = 0x0000E000;
    #define CONTROL_REG_SETCLK(x) (x << CONTROLREG_DATARATE_SHIFT) & CONTROLREG_DATARATE_MASK
    
    static const    UINT32 CONTROLREG_MODE_Master     = 0x00000400;
    static const    UINT32 CONTROLREG_SPIEN           = 0x00000200;
    static const    UINT32 CONTROLREG_XCH             = 0x00000100;
    static const    UINT32 CONTROLREG_SSPOL_HIGH      = 0x00000080;
    static const    UINT32 CONTROLREG_SSCTL_PULSE     = 0x00000040;
    static const    UINT32 CONTROLREG_PHA_1           = 0x00000020;
    static const    UINT32 CONTROLREG_POL_1           = 0x00000010;
    static const    UINT32 CONTROLREG_BIT_CNT_16      = 0x0000000F;
    static const    UINT32 CONTROLREG_BIT_CNT_8       = 0x00000007;

    /****/ volatile UINT32 INTREG;
    static const    UINT32 INTREG_BO                  = 0x00000080;    
    static const    UINT32 INTREG_RO                  = 0x00000040;
    static const    UINT32 INTREG_RF                  = 0x00000020;
    static const    UINT32 INTREG_RH                  = 0x00000010;
    static const    UINT32 INTREG_RR                  = 0x00000008;    
    static const    UINT32 INTREG_TF                  = 0x00000004;
    static const    UINT32 INTREG_TH                  = 0x00000002;
    static const    UINT32 INTREG_TE                  = 0x00000001;
    
    /****/ volatile UINT32 TESTREG;
    static const    UINT32 TESTREG_LBC                = 0x00004000;
    static const    UINT32 TESTREG_TXCNT_mask         = 0x0000000F;
    static const    UINT32 TESTREG_RXCNT_mask         = 0x000000F0;

    /****/ volatile UINT32 PERIODREG;
    static const    UINT32 PERIODREG_CSRC_32KHz       = 0x00008000;

    /****/ volatile UINT32 DMAREG;

    /****/ volatile UINT32 RESETREG;
    static const    UINT32 RESETREG_START             = 0x00000001;


    // __inline Helpers for this processor core

    // 8 data words in the TXFIFO
    __inline BOOL TransmitBufferFull()
    {
        return (INTREG & INTREG_TF ) != 0;
    }

    // no data in TX FIFO
    __inline BOOL TransmitBufferEmpty()
    {
        return (INTREG & INTREG_TE ) != 0;
    }

    // no data in RX FIFO
    __inline BOOL ReceiveBufferEmpty()
    {
        return (INTREG & INTREG_RR) == 0;
    }


    __inline BOOL ShiftBufferEmpty()
    {
        return (CONTROLREG & CONTROLREG_XCH) == 0;
    }


    __inline static UINT32 ConvertClockRateToDivisor(UINT32 Clock_RateKHz)
    {
        #define SYSTEM_CLOCK_KHZ  SYSTEM_CLOCK_HZ/1000

        UINT32 ref_clock = (SYSTEM_CLOCK_KHZ*6 ) >> 2;   // first clock rate available
        UINT32 divisor = CONTROLREG_DATARATE_DIV4;  // use the divisor bit map directly to save the instruction to do the shifting later.

        while ( divisor < CONTROLREG_DATARATE_DIV512)
        {
            if (Clock_RateKHz >= ref_clock)
            {
                return divisor;
            }
            else
            {
                ref_clock = ref_clock >>1;
                divisor  += CONTROLREG_DATARATE_INTERVAL;
            }
        }
        return divisor;
    }


    // only used in 1 place in RTM builds, so inline this for quicker access
    __inline static UINT16 ConfigurationToMode( const SPI_CONFIGURATION& Configuration )
    {
        UINT32 Mode;

        
        Mode = CONTROLREG_MODE_Master | CONTROLREG_SSPOL_HIGH ;

        if(Configuration.MD_16bits)
        {
            Mode |= CONTROLREG_BIT_CNT_16;
        }
        else
        {
            Mode |= CONTROLREG_BIT_CNT_8;
        }

        if(Configuration.MSK_IDLE)
        {
           // Idle is Active high, set 1, 
            Mode |= CONTROLREG_POL_1;   
           
            if(Configuration.MSK_SampleEdge)
            {
                Mode |= CONTROLREG_PHA_1;        // sample on rising edge
            }
            // else phase set to 0
        }
        else
        {
            if(!Configuration.MSK_SampleEdge)
            {
                Mode |= CONTROLREG_PHA_1;        // sample on rising edge
            }
            // else phase set to 0;
        }
        Mode |=  ConvertClockRateToDivisor(Configuration.Clock_RateKHz) ;
        return Mode;
    }

};
//
// SPI
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// LCD Controller
//
struct MC9328MXL_LCDC
{
    static const UINT32 c_Base = 0x00205000;

    /****/ volatile UINT32 SSA;                                        // Screen Start Address Register
    static const    UINT32 SSA__SSA_mask                 = 0xFFFFFFFC; // Must start at a location that enables a complete picture to be stored in a 4 Mbyte memory boundary (A [21:0]). A [31:22] has a fixed value for a picture’s image.
    static const    UINT32 SSA__SSA_shift                =          0;

    /****/ volatile UINT32 SIZE;                                       // Size Register
    static const    UINT32 SIZE__XMAX_mask               = 0x03F00000; // Screen Width
    static const    UINT32 SIZE__XMAX_shift              =         16;
    static const    UINT32 SIZE__YMAX_mask               = 0x000001FF; // Screen Height
    static const    UINT32 SIZE__YMAX_shift              =          0;

    /****/ volatile UINT32 VPW;                                        // Virtual Page Width Register
    static const    UINT32 VPW__VPW_mask                 = 0x000003FF; // Virtual Page Width
    static const    UINT32 VPW__VPW_shift                =          0;

    /****/ volatile UINT32 CPOS;                                       // LCD Cursor Position Register
    static const    UINT32 CPOS__CC_mask                 = 0xC0000000; // Cursor Control
    static const    UINT32 CPOS__CC_shift                =         30;
    static const    UINT32 CPOS__CC__OP0_Cursor_Disabled =          0;
    static const    UINT32 CPOS__CC__OP0_Full_Cursor     =          1;
    static const    UINT32 CPOS__CC__OP0_Reversed_Video  =          2;
    static const    UINT32 CPOS__CC__OP0_White_Cursor    =          3;
    static const    UINT32 CPOS__CC__OP1_Cursor_Disabled =          0;
    static const    UINT32 CPOS__CC__OP1_OR              =          1;
    static const    UINT32 CPOS__CC__OP1_XOR             =          2;
    static const    UINT32 CPOS__CC__OP1_AND             =          3;
    static const    UINT32 CPOS__OP_mask                 = 0x10000000; // Arithmetic Operation Control - Enable / Disable arithmetic operation between background and cursor in Cursor Control (CC)
    static const    UINT32 CPOS__OP_shift                =         28;
    static const    UINT32 CPOS__OP__Disable             =          0;
    static const    UINT32 CPOS__OP__Enable              =          1;
    static const    UINT32 CPOS__CXP_mask                = 0x03FF0000; // Cursor X Position
    static const    UINT32 CPOS__CXP_shift               =         16;
    static const    UINT32 CPOS__CYP_mask                = 0x000001FF; // Cursor Y Position
    static const    UINT32 CPOS__CYP_shift               =          0;

    /****/ volatile UINT32 LCWHB;                                      // LCD Cursor Width Height and Blink Register
    static const    UINT32 LCWHB__BK_EN_mask             = 0x80000000; // Blink Enable
    static const    UINT32 LCWHB__BK_EN_shift            =         31;
    static const    UINT32 LCWHB__BK_EN__Disable         =          0;
    static const    UINT32 LCWHB__BK_EN__Enable          =          1;
    static const    UINT32 LCWHB__CW_mask                = 0x1F000000; // Cursor Width
    static const    UINT32 LCWHB__CW_shift               =         24;
    static const    UINT32 LCWHB__CH_mask                = 0x001F0000; // Cursor Height
    static const    UINT32 LCWHB__CH_shift               =         16;
    static const    UINT32 LCWHB__BD_mask                = 0x000000FF; // Blink Divisor
    static const    UINT32 LCWHB__BD_shift               =          0;

    /****/ volatile UINT32 LCHCC;                                      // LCD Color Cursor Mapping Register
    static const    UINT32 LCHCC__CUR_COL_R_mask         = 0x0000F800; // Cursor Red Field
    static const    UINT32 LCHCC__CUR_COL_R_shift        =         11;
    static const    UINT32 LCHCC__CUR_COL_G_mask         = 0x000007E0; // Cursor Green Field
    static const    UINT32 LCHCC__CUR_COL_G_shift        =          5;
    static const    UINT32 LCHCC__CUR_COL_B_mask         = 0x0000001F; // Cursor Blue Field
    static const    UINT32 LCHCC__CUR_COL_B_shift        =          0;

    /****/ volatile UINT32 PCR;                                        // Panel Configuration Register
    static const    UINT32 PCR__TFT_mask                 = 0x80000000; // Interfaces to TFT Display
    static const    UINT32 PCR__TFT_shift                =         31;
    static const    UINT32 PCR__TFT__Disable             =          0;
    static const    UINT32 PCR__TFT__Enable              =          1;
    static const    UINT32 PCR__COLOR_mask               = 0x40000000; // Interfaces to Color Display
    static const    UINT32 PCR__COLOR_shift              =         30;
    static const    UINT32 PCR__COLOR__Disable           =          0;
    static const    UINT32 PCR__COLOR__Enable            =          1;
    static const    UINT32 PCR__PBSIZ_mask               = 0x30000000; // Panel Bus Width
    static const    UINT32 PCR__PBSIZ_shift              =         28;
    static const    UINT32 PCR__PBSIZ__1_Bit             =          0;
    static const    UINT32 PCR__PBSIZ__2_Bit             =          1;
    static const    UINT32 PCR__PBSIZ__4_Bit             =          2;
    static const    UINT32 PCR__PBSIZ__8_Bit             =          3;
    static const    UINT32 PCR__PBSIZ__TFT_16_Bit        =          3; // For active (TFT) color panels, the panel bus width is fixed at 16.
    static const    UINT32 PCR__BPIX_mask                = 0x0E000000; // Bits Per Pixel
    static const    UINT32 PCR__BPIX_shift               =         25;
    static const    UINT32 PCR__BPIX__1bpp               =          0;
    static const    UINT32 PCR__BPIX__2bpp               =          1;
    static const    UINT32 PCR__BPIX__4bpp               =          2;
    static const    UINT32 PCR__BPIX__8bpp               =          3;
    static const    UINT32 PCR__BPIX__12bpp_16bpp        =          4;
    static const    UINT32 PCR__PIXPOL_mask              = 0x01000000; // Pixel Polarity
    static const    UINT32 PCR__PIXPOL_shift             =         24;
    static const    UINT32 PCR__PIXPOL__Active_High      =          0;
    static const    UINT32 PCR__PIXPOL__Active_Low       =          1;
    static const    UINT32 PCR__FLMPOL_mask              = 0x00800000; // First Line Marker Polarity
    static const    UINT32 PCR__FLMPOL_shift             =         23;
    static const    UINT32 PCR__FLMPOL__Active_High      =          0;
    static const    UINT32 PCR__FLMPOL__Active_Low       =          1;
    static const    UINT32 PCR__LPPOL_mask               = 0x00400000; // Line Pulse Polarity
    static const    UINT32 PCR__LPPOL_shift              =         22;
    static const    UINT32 PCR__LPPOL__Active_High       =          0;
    static const    UINT32 PCR__LPPOL__Active_Low        =          1;
    static const    UINT32 PCR__CLKPOL_mask              = 0x00200000; // LCD Shift Clock Polarity
    static const    UINT32 PCR__CLKPOL_shift             =         21;
    static const    UINT32 PCR__CLKPOL__Active_High      =          0;
    static const    UINT32 PCR__CLKPOL__Active_Low       =          1;
    static const    UINT32 PCR__OEPOL_mask               = 0x00100000; // Output Enable Polarity
    static const    UINT32 PCR__OEPOL_shift              =         20;
    static const    UINT32 PCR__OEPOL__Active_High       =          0;
    static const    UINT32 PCR__OEPOL__Active_Low        =          1;
    static const    UINT32 PCR__SCLKIDLE_mask            = 0x00080000; // LSCLK Idle Enable
    static const    UINT32 PCR__SCLKIDLE_shift           =         19;
    static const    UINT32 PCR__SCLKIDLE__Disable_LSCLK  =          0;
    static const    UINT32 PCR__SCLKIDLE__Enable_LSCLK   =          1;
    static const    UINT32 PCR__END_SEL_mask             = 0x00040000; // Endian Select
    static const    UINT32 PCR__END_SEL_shift            =         18;
    static const    UINT32 PCR__END_SEL__Little_Endian   =          0;
    static const    UINT32 PCR__END_SEL__Big_Endian      =          1;
    static const    UINT32 PCR__SWAP_SEL_mask            = 0x00020000; // Swap Select - Controls the swap of data in little endian mode (when END_SEL = 1 this bit has no effect).
    static const    UINT32 PCR__SWAP_SEL_shift           =         17;
    static const    UINT32 PCR__SWAP_SEL__16bpp          =          0;
    static const    UINT32 PCR__SWAP_SEL__8_4_2_1bpp     =          1;
    static const    UINT32 PCR__REV_VS_mask              = 0x00010000; // Reverse Vertical Scan
    static const    UINT32 PCR__REV_VS_shift             =         16;
    static const    UINT32 PCR__REV_VS__Normal_Dir       =          0;
    static const    UINT32 PCR__REV_VS__Reverse_Dir      =          1;
    static const    UINT32 PCR__ACDSEL_mask              = 0x00008000; // ACD Clock Source Select
    static const    UINT32 PCR__ACDSEL_shift             =         15;
    static const    UINT32 PCR__ACDSEL__FRM              =          0;
    static const    UINT32 PCR__ACDSEL__LP_HSYN          =          1;
    static const    UINT32 PCR__ACD_mask                 = 0x00007F00; // Alternate Crystal Direction - For active mode (TFT=1), this parameter is not used.
    static const    UINT32 PCR__ACD_shift                =          8;
    static const    UINT32 PCR__SCLKSEL_mask             = 0x00000080; // LSCLK Select - Selects whether to enable or disable LSCLK in TFT mode when there is no data output.
    static const    UINT32 PCR__SCLKSEL_shift            =          7;
    static const    UINT32 PCR__SCLKSEL_Disable_LSCLK    =          0;
    static const    UINT32 PCR__SCLKSEL_Enable_LSCLK     =          1;
    static const    UINT32 PCR__SHARP_mask               = 0x00000040; // Sharp Panel Enable - Enables/Disables signals for Sharp HR-TFT panels.
    static const    UINT32 PCR__SHARP_shift              =          6;
    static const    UINT32 PCR__SHARP__Disable           =          0;
    static const    UINT32 PCR__SHARP__Enable            =          1;
    static const    UINT32 PCR__PCD_mask                 = 0x0000003F; // Pixel Clock Divider
    static const    UINT32 PCR__PCD_shift                =          0; // 0 = Divide by 1, 1 = Divide by 2, etc.

    /****/ volatile UINT32 HCR;                                        // Horizontal Configuration Register
    static const    UINT32 HCR__H_WIDTH_mask             = 0xFC000000; // Horizontal Sync Pulse Width - Specifies the number of pixel clk periods that HSYNC is activated.
    static const    UINT32 HCR__H_WIDTH_shift            =         26; // The active time is equal to (H_WIDTH + 1) of the pixel clk period.
    static const    UINT32 HCR__H_WAIT_1_mask            = 0x0000FF00; // Wait Between OE and HSYNC - Specifies the number of pixel clk periods between the last LD of each line and the beginning of the HSYNC.
    static const    UINT32 HCR__H_WAIT_1_shift           =          8; // Total delay time equals (H_WAIT_1 + 1).
    static const    UINT32 HCR__H_WAIT_2_mask            = 0x000000FF; // Wait Between HSYNC and Start of Next Line - Specifies the number of pixel clk periods between the end of HSYNC and the beginning of the first data of next line.
    static const    UINT32 HCR__H_WAIT_2_shift           =          0; // Total delay time equals (H_WAIT_2 + 3).

    /****/ volatile UINT32 VCR;                                        // Vertical Configuration Register
    static const    UINT32 VCR__V_WIDTH_mask             = 0xFC000000; // Vertical Sync Pulse Width - Specifies the width, in lines, of the VSYNC pulse for active (TFT =1) mode.
    static const    UINT32 VCR__V_WIDTH_shift            =         26; // For a value of “000001”, the vertical sync pulse encompasses one HSYNC pulse. For a value of “000002”, the vertical sync pulse encompasses two HSYNC pulses, and so on.
    static const    UINT32 VCR__V_WAIT_1_mask            = 0x0000FF00; // Wait Between Frames 1 - Defines the delay, in lines, between the end of the OE pulse and the beginning of the VSYNC pulse for active (TFT=1) mode.
    static const    UINT32 VCR__V_WAIT_1_shift           =          8; // The actual delay is (V_WAIT_1) lines.
    static const    UINT32 VCR__V_WAIT_2_mask            = 0x000000FF; // Wait Between Frames 2 - Defines the delay, in lines, between the end of the VSYNC pulse and the beginning of the OE pulse of the first line in active (TFT=1) mode.
    static const    UINT32 VCR__V_WAIT_2_shift           =          0; // The actual delay is (V_WAIT_2) lines.

    /****/ volatile UINT32 POS;                                        // Panning Offset Register
    static const    UINT32 POS__POS_mask                 = 0x0000001F; // Panning Offset - Defines the number of bits that the data from memory is panned to the left before processing.
    static const    UINT32 POS__POS_shift                =          0;

    /****/ volatile UINT32 LSCR1;                                      // Sharp Configuration 1 Register
    static const    UINT32 LSCR1__PS_RISE_DELAY_mask     = 0xFC000000; // PS Rise Delay - in LSCLK periods
    static const    UINT32 LSCR1__PS_RISE_DELAY_shift    =         26;
    static const    UINT32 LSCR1__CLS_RISE_DELAY_mask    = 0x00FF0000; // CLS Rise Delay - in LSCLK periods
    static const    UINT32 LSCR1__CLS_RISE_DELAY_shift   =         16;
    static const    UINT32 LSCR1__REV_TOGGLE_DELAY_mask  = 0x00000F00; // REV Toggle Delay - in LSCLK periods
    static const    UINT32 LSCR1__REV_TOGGLE_DELAY_shift =          8;
    static const    UINT32 LSCR1__GRAY2_mask             = 0x000000F0; // Gray-Scale 2 - values from 1 ~ 15 are valid
    static const    UINT32 LSCR1__GRAY2_shift            =          4;
    static const    UINT32 LSCR1__GRAY1_mask             = 0x0000000F; // Gray-Scale 1 - values from 1 ~ 15 are valid
    static const    UINT32 LSCR1__GRAY1_shift            =          0;

    /****/ volatile UINT32 PWMR;                                       // PWM Contrast Control Register
    static const    UINT32 PWMR__CLS_HI_WIDTH_mask       = 0x01FF0000; // CLS High Pulse Width - in SCLK periods, actual pulse width = CLS_HI_WIDTH + 1
    static const    UINT32 PWMR__CLS_HI_WIDTH_shift      =         16;
    static const    UINT32 PWMR__LDMSK_mask              = 0x00008000; // LD Mask
    static const    UINT32 PWMR__LDMSK_shift             =         15;
    static const    UINT32 PWMR__LDMSK__DISABLE          =          0;
    static const    UINT32 PWMR__LDMSK__ENABLE           =          1;
    static const    UINT32 PWMR__SCR_mask                = 0x00000600; // Source Select
    static const    UINT32 PWMR__SCR_shift               =          9;
    static const    UINT32 PWMR__SCR__LINE_PULSE         =          0;
    static const    UINT32 PWMR__SCR__PIXEL_CLK          =          1;
    static const    UINT32 PWMR__SCR__LCD_CLK            =          2;
    static const    UINT32 PWMR__CC_EN_mask              = 0x00000100; // Contrast Control Enable
    static const    UINT32 PWMR__CC_EN_shift             =          8;
    static const    UINT32 PWMR__CC_EN__DISABLE          =          0;
    static const    UINT32 PWMR__CC_EN__ENABLE           =          1;
    static const    UINT32 PWMR__PW_mask                 = 0x000000FF; // Pulse-Width
    static const    UINT32 PWMR__PW_shift                =          0;

    /****/ volatile UINT32 DMACR;                                      // DMA Control Register
    static const    UINT32 DMACR__BURST_mask             = 0x80000000; // Burst Length
    static const    UINT32 DMACR__BURST_shift            =         31;
    static const    UINT32 DMACR__BURST__DYNAMIC         =          0;
    static const    UINT32 DMACR__BURST__FIXED           =          1;
    static const    UINT32 DMACR__HM_mask                = 0x000F0000; // DMA High Mark
    static const    UINT32 DMACR__HM_shift               =         16;
    static const    UINT32 DMACR__TM_mask                = 0x0000000F; // DMA Trigger Mark
    static const    UINT32 DMACR__TM_shift               =          0;

    /****/ volatile UINT32 RMCR;                                       // Refresh Mode Control Register
    static const    UINT32 RMCR__LCDC_EN_mask            = 0x00000002; // LCDC Enable
    static const    UINT32 RMCR__LCDC_EN_shift           =          1;
    static const    UINT32 RMCR__LCDC_EN__DISABLE        =          0;
    static const    UINT32 RMCR__LCDC_EN__ENABLE         =          1;
    static const    UINT32 RMCR__SELF_REF_mask           = 0x00000001; // Self-Refresh
    static const    UINT32 RMCR__SELF_REF_shift          =          0;
    static const    UINT32 RMCR__SELF_REF__DISABLE       =          0;
    static const    UINT32 RMCR__SELF_REF__ENABLE        =          1;

    /****/ volatile UINT32 LCDICR;                                     // Interrupt Configuration Register
    static const    UINT32 LCDICR__INTSYN_mask           = 0x00000004; // Interrupt Source
    static const    UINT32 LCDICR__INTSYN_shift          =          2;
    static const    UINT32 LCDICR__INTSYN__ON_LOADING    =          0; // Interrupt set on loading
    static const    UINT32 LCDICR__INTSYN__ON_OUTPUT     =          1; // Interrupt set on output
    static const    UINT32 LCDICR__INTCON_mask           = 0x00000001; // Interrupt Condition
    static const    UINT32 LCDICR__INTCON_shift          =          0;
    static const    UINT32 LCDICR__INTCON__EOF           =          0; // Interrupt set when End of Frame is reached
    static const    UINT32 LCDICR__INTCON__BOF           =          1; // Interrupt set when Beginning of Frame is reached

    /****/ volatile UINT32 Padding;

    /****/ volatile UINT32 LCDISR;                                     // Interrupt Status Register (read-only)
    static const    UINT32 LCDISR__UDR_ERR_mask          = 0x00000008; // Under Run Error
    static const    UINT32 LCDISR__UDR_ERR_shift         =          3;
    static const    UINT32 LCDISR__ERR_RES_mask          = 0x00000004; // Error Response
    static const    UINT32 LCDISR__ERR_RES_shift         =          2;
    static const    UINT32 LCDISR__EOF_mask              = 0x00000002; // End of Frame
    static const    UINT32 LCDISR__EOF_shift             =          1;
    static const    UINT32 LCDISR__BOF_mask              = 0x00000001; // Beginning of Frame
    static const    UINT32 LCDISR__BOF_shift             =          0;

};
//
// LCD Controller
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// WATCHDOG
//
struct MC9328MXL_WATCHDOG
{
    static const UINT32 c_Base = 0x00201000;

    //--//

    /****/ volatile UINT32 WCR;
    static const    UINT32 WCR__WHALT    = 0x00008000;
    static const    UINT32 WCR__WT_mask  = 0x00007F00;
    static const    UINT32 WCR__WT_shift =          8;
    static const    UINT32 WCR__WIE      = 0x00000010;
    static const    UINT32 WCR__TMD      = 0x00000008;
    static const    UINT32 WCR__SWR      = 0x00000004;
    static const    UINT32 WCR__WDEC     = 0x00000002;
    static const    UINT32 WCR__WDE      = 0x00000001;

    /****/ volatile UINT32 WSR;
    static const    UINT32 WSR_reload_1  = 0x00005555;
    static const    UINT32 WSR_reload_2  = 0x0000AAAA;
    static const    UINT32 WSR_mask      = 0x0000FFFF;

    /****/ volatile UINT32 WSTR;
    static const    UINT32 WSTR__TINT    = 0x00000100;
    static const    UINT32 WSTR__TOUT    = 0x00000001;
};
//
// WATCHDOG
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// DMA Controller
//
struct MC9328MXL_DMA
{
    static const UINT32 c_Base = 0x00209000;

    //--//

    /****/ volatile UINT32 DCR;
    static const    UINT32 DCR__DRST_mask               = 0x00000002;
    static const    UINT32 DCR__DRST_shift              =          1;
    static const    UINT32 DCR__DRST__RESET             = 0x00000001;
    static const    UINT32 DCR__DEN_mask                = 0x00000001;
    static const    UINT32 DCR__DEN_shift               =          0;
    static const    UINT32 DCR__DEN__Enable             = 0x00000001;
    static const    UINT32 DCR__DEN__Disable            = 0x00000000;

    /****/ volatile UINT32 DISR;
    static const    UINT32 DISR__CH10_mask              = 0x00000400;
    static const    UINT32 DISR__CH10_shift             =         10;
    static const    UINT32 DISR__CH9_mask               = 0x00000200;
    static const    UINT32 DISR__CH9_shift              =          9;
    static const    UINT32 DISR__CH8_mask               = 0x00000100;
    static const    UINT32 DISR__CH8_shift              =          8;
    static const    UINT32 DISR__CH7_mask               = 0x00000080;
    static const    UINT32 DISR__CH7_shift              =          7;
    static const    UINT32 DISR__CH6_mask               = 0x00000040;
    static const    UINT32 DISR__CH6_shift              =          6;
    static const    UINT32 DISR__CH5_mask               = 0x00000020;
    static const    UINT32 DISR__CH5_shift              =          5;
    static const    UINT32 DISR__CH4_mask               = 0x00000010;
    static const    UINT32 DISR__CH4_shift              =          4;
    static const    UINT32 DISR__CH3_mask               = 0x00000008;
    static const    UINT32 DISR__CH3_shift              =          3;
    static const    UINT32 DISR__CH2_mask               = 0x00000004;
    static const    UINT32 DISR__CH2_shift              =          2;
    static const    UINT32 DISR__CH1_mask               = 0x00000002;
    static const    UINT32 DISR__CH1_shift              =          1;
    static const    UINT32 DISR__CH0_mask               = 0x00000001;
    static const    UINT32 DISR__CH0_shift              =          0;

    /****/ volatile UINT32 DIMR;
    static const    UINT32 DIMR__CH10_mask              = 0x00000400;
    static const    UINT32 DIMR__CH10_shift             =         10;
    static const    UINT32 DIMR__CH9_mask               = 0x00000200;
    static const    UINT32 DIMR__CH9_shift              =          9;
    static const    UINT32 DIMR__CH8_mask               = 0x00000100;
    static const    UINT32 DIMR__CH8_shift              =          8;
    static const    UINT32 DIMR__CH7_mask               = 0x00000080;
    static const    UINT32 DIMR__CH7_shift              =          7;
    static const    UINT32 DIMR__CH6_mask               = 0x00000040;
    static const    UINT32 DIMR__CH6_shift              =          6;
    static const    UINT32 DIMR__CH5_mask               = 0x00000020;
    static const    UINT32 DIMR__CH5_shift              =          5;
    static const    UINT32 DIMR__CH4_mask               = 0x00000010;
    static const    UINT32 DIMR__CH4_shift              =          4;
    static const    UINT32 DIMR__CH3_mask               = 0x00000008;
    static const    UINT32 DIMR__CH3_shift              =          3;
    static const    UINT32 DIMR__CH2_mask               = 0x00000004;
    static const    UINT32 DIMR__CH2_shift              =          2;
    static const    UINT32 DIMR__CH1_mask               = 0x00000002;
    static const    UINT32 DIMR__CH1_shift              =          1;
    static const    UINT32 DIMR__CH0_mask               = 0x00000001;
    static const    UINT32 DIMR__CH0_shift              =          0;
    static const    UINT32 DIMR__Enable_INT             =          0;
    static const    UINT32 DIMR__Disable_INT            =          0;

    /****/ volatile UINT32 DBTOSR;
    static const    UINT32 DBTOSR__CH10_mask            = 0x00000400;
    static const    UINT32 DBTOSR__CH10_shift           =         10;
    static const    UINT32 DBTOSR__CH9_mask             = 0x00000200;
    static const    UINT32 DBTOSR__CH9_shift            =          9;
    static const    UINT32 DBTOSR__CH8_mask             = 0x00000100;
    static const    UINT32 DBTOSR__CH8_shift            =          8;
    static const    UINT32 DBTOSR__CH7_mask             = 0x00000080;
    static const    UINT32 DBTOSR__CH7_shift            =          7;
    static const    UINT32 DBTOSR__CH6_mask             = 0x00000040;
    static const    UINT32 DBTOSR__CH6_shift            =          6;
    static const    UINT32 DBTOSR__CH5_mask             = 0x00000020;
    static const    UINT32 DBTOSR__CH5_shift            =          5;
    static const    UINT32 DBTOSR__CH4_mask             = 0x00000010;
    static const    UINT32 DBTOSR__CH4_shift            =          4;
    static const    UINT32 DBTOSR__CH3_mask             = 0x00000008;
    static const    UINT32 DBTOSR__CH3_shift            =          3;
    static const    UINT32 DBTOSR__CH2_mask             = 0x00000004;
    static const    UINT32 DBTOSR__CH2_shift            =          2;
    static const    UINT32 DBTOSR__CH1_mask             = 0x00000002;
    static const    UINT32 DBTOSR__CH1_shift            =          1;
    static const    UINT32 DBTOSR__CH0_mask             = 0x00000001;
    static const    UINT32 DBTOSR__CH0_shift            =          0;

    /****/ volatile UINT32 DRTOSR;
    static const    UINT32 DRTOSR__CH10_mask            = 0x00000400;
    static const    UINT32 DRTOSR__CH10_shift           =         10;
    static const    UINT32 DRTOSR__CH9_mask             = 0x00000200;
    static const    UINT32 DRTOSR__CH9_shift            =          9;
    static const    UINT32 DRTOSR__CH8_mask             = 0x00000100;
    static const    UINT32 DRTOSR__CH8_shift            =          8;
    static const    UINT32 DRTOSR__CH7_mask             = 0x00000080;
    static const    UINT32 DRTOSR__CH7_shift            =          7;
    static const    UINT32 DRTOSR__CH6_mask             = 0x00000040;
    static const    UINT32 DRTOSR__CH6_shift            =          6;
    static const    UINT32 DRTOSR__CH5_mask             = 0x00000020;
    static const    UINT32 DRTOSR__CH5_shift            =          5;
    static const    UINT32 DRTOSR__CH4_mask             = 0x00000010;
    static const    UINT32 DRTOSR__CH4_shift            =          4;
    static const    UINT32 DRTOSR__CH3_mask             = 0x00000008;
    static const    UINT32 DRTOSR__CH3_shift            =          3;
    static const    UINT32 DRTOSR__CH2_mask             = 0x00000004;
    static const    UINT32 DRTOSR__CH2_shift            =          2;
    static const    UINT32 DRTOSR__CH1_mask             = 0x00000002;
    static const    UINT32 DRTOSR__CH1_shift            =          1;
    static const    UINT32 DRTOSR__CH0_mask             = 0x00000001;
    static const    UINT32 DRTOSR__CH0_shift            =          0;

    /****/ volatile UINT32 DSESR;
    static const    UINT32 DSESR__CH10_mask             = 0x00000400;
    static const    UINT32 DSESR__CH10_shift            =         10;
    static const    UINT32 DSESR__CH9_mask              = 0x00000200;
    static const    UINT32 DSESR__CH9_shift             =          9;
    static const    UINT32 DSESR__CH8_mask              = 0x00000100;
    static const    UINT32 DSESR__CH8_shift             =          8;
    static const    UINT32 DSESR__CH7_mask              = 0x00000080;
    static const    UINT32 DSESR__CH7_shift             =          7;
    static const    UINT32 DSESR__CH6_mask              = 0x00000040;
    static const    UINT32 DSESR__CH6_shift             =          6;
    static const    UINT32 DSESR__CH5_mask              = 0x00000020;
    static const    UINT32 DSESR__CH5_shift             =          5;
    static const    UINT32 DSESR__CH4_mask              = 0x00000010;
    static const    UINT32 DSESR__CH4_shift             =          4;
    static const    UINT32 DSESR__CH3_mask              = 0x00000008;
    static const    UINT32 DSESR__CH3_shift             =          3;
    static const    UINT32 DSESR__CH2_mask              = 0x00000004;
    static const    UINT32 DSESR__CH2_shift             =          2;
    static const    UINT32 DSESR__CH1_mask              = 0x00000002;
    static const    UINT32 DSESR__CH1_shift             =          1;
    static const    UINT32 DSESR__CH0_mask              = 0x00000001;
    static const    UINT32 DSESR__CH0_shift             =          0;

    /****/ volatile UINT32 DBOSR;
    static const    UINT32 DBOSR__CH10_mask             = 0x00000400;
    static const    UINT32 DBOSR__CH10_shift            =         10;
    static const    UINT32 DBOSR__CH9_mask              = 0x00000200;
    static const    UINT32 DBOSR__CH9_shift             =          9;
    static const    UINT32 DBOSR__CH8_mask              = 0x00000100;
    static const    UINT32 DBOSR__CH8_shift             =          8;
    static const    UINT32 DBOSR__CH7_mask              = 0x00000080;
    static const    UINT32 DBOSR__CH7_shift             =          7;
    static const    UINT32 DBOSR__CH6_mask              = 0x00000040;
    static const    UINT32 DBOSR__CH6_shift             =          6;
    static const    UINT32 DBOSR__CH5_mask              = 0x00000020;
    static const    UINT32 DBOSR__CH5_shift             =          5;
    static const    UINT32 DBOSR__CH4_mask              = 0x00000010;
    static const    UINT32 DBOSR__CH4_shift             =          4;
    static const    UINT32 DBOSR__CH3_mask              = 0x00000008;
    static const    UINT32 DBOSR__CH3_shift             =          3;
    static const    UINT32 DBOSR__CH2_mask              = 0x00000004;
    static const    UINT32 DBOSR__CH2_shift             =          2;
    static const    UINT32 DBOSR__CH1_mask              = 0x00000002;
    static const    UINT32 DBOSR__CH1_shift             =          1;
    static const    UINT32 DBOSR__CH0_mask              = 0x00000001;
    static const    UINT32 DBOSR__CH0_shift             =          0;

    /****/ volatile UINT32 DBTOCR;
    static const    UINT32 DBTOCR__EN_mask              = 0x00008000;
    static const    UINT32 DBTOCR__EN_shift             =         15;
    static const    UINT32 DBTOCR__EN__Enable           = 0x00000001;
    static const    UINT32 DBTOCR__EN__Disable          = 0x00000000;
    static const    UINT32 DBTOCR__CNT_mask             = 0x00007FFF;
    static const    UINT32 DBTOCR__CNT_shift            =          0;

    /****/ volatile UINT32 Padding0[8];

    /****/ volatile UINT32 WSRA;
    static const    UINT32 WSRA__WS_mask                = 0x0000FFFF;
    static const    UINT32 WSRA__WS_shift               =          0;

    /****/ volatile UINT32 XSRA;
    static const    UINT32 XSRA__XS_mask                = 0x0000FFFF;
    static const    UINT32 XSRA__XS_shift               =          0;

    /****/ volatile UINT32 YSRA;
    static const    UINT32 YSRA__YS_mask                = 0x0000FFFF;
    static const    UINT32 YSRA__YS_shift               =          0;

    /****/ volatile UINT32 WSRB;
    static const    UINT32 WSRB__WS_mask                = 0x0000FFFF;
    static const    UINT32 WSRB__WS_shift               =          0;

    /****/ volatile UINT32 XSRB;
    static const    UINT32 XSRB__XS_mask                = 0x0000FFFF;
    static const    UINT32 XSRB__XS_shift               =          0;

    /****/ volatile UINT32 YSRB;
    static const    UINT32 YSRB__YS_mask                = 0x0000FFFF;
    static const    UINT32 YSRB__YS_shift               =          0;

    /****/ volatile UINT32 Padding1[10];

    struct CHANNEL_REGISTERS
    {
        volatile UINT32 SAR;
        volatile UINT32 DAR;
        volatile UINT32 CNTR;
        volatile UINT32 CCR;
        volatile UINT32 RSSR;
        volatile UINT32 BLR;
        volatile UINT32 RTORBUCR;

        volatile UINT32 Padding[9];
    } CH[11];

    static const    UINT32 SAR__SA_mask                 = 0xFFFFFFFC;
    static const    UINT32 SAR__SA_shift                =          0;

    static const    UINT32 DAR__DA_mask                 = 0xFFFFFFFC;
    static const    UINT32 DAR__DA_shift                =          0;

    static const    UINT32 CNTR__CNT_mask               = 0x00FFFFFF;
    static const    UINT32 CNTR__CNT_shift              =          0;

    static const    UINT32 CCR__DMOD_mask               = 0x00003000;
    static const    UINT32 CCR__DMOD_shift              =         12;
    static const    UINT32 CCR__DMOD__Linear            = 0x00000000;
    static const    UINT32 CCR__DMOD__2D                = 0x00000001;
    static const    UINT32 CCR__DMOD__FIFO              = 0x00000002;
    static const    UINT32 CCR__DMOD__End_of_Burst_FIFO = 0x00000003;
    static const    UINT32 CCR__SMOD_mask               = 0x00000C00;
    static const    UINT32 CCR__SMOD_shift              =         10;
    static const    UINT32 CCR__SMOD__Linear            = 0x00000000;
    static const    UINT32 CCR__SMOD__2D                = 0x00000001;
    static const    UINT32 CCR__SMOD__FIFO              = 0x00000002;
    static const    UINT32 CCR__SMOD__End_of_Burst_FIFO = 0x00000003;
    static const    UINT32 CCR__MDIR_mask               = 0x00000200;
    static const    UINT32 CCR__MDIR_shift              =          9;
    static const    UINT32 CCR__MDIR__Inc               = 0x00000000;
    static const    UINT32 CCR__MDIR__Dec               = 0x00000001;
    static const    UINT32 CCR__MSEL_mask               = 0x00000100;
    static const    UINT32 CCR__MSEL_shift              =          8;
    static const    UINT32 CCR__MSEL__2D_Memory_A       = 0x00000000;
    static const    UINT32 CCR__MSEL__2D_Memory_B       = 0x00000001;
    static const    UINT32 CCR__DSIZ_mask               = 0x000000C0;
    static const    UINT32 CCR__DSIZ_shift              =          6;
    static const    UINT32 CCR__DSIZ__32_Bit_Port       = 0x00000000;
    static const    UINT32 CCR__DSIZ__8_Bit_Port        = 0x00000001;
    static const    UINT32 CCR__DSIZ__16_Bit_Port       = 0x00000002;
    static const    UINT32 CCR__SSIZ_mask               = 0x00000030;
    static const    UINT32 CCR__SSIZ_shift              =          4;
    static const    UINT32 CCR__SSIZ__32_Bit_Port       = 0x00000000;
    static const    UINT32 CCR__SSIZ__8_Bit_Port        = 0x00000001;
    static const    UINT32 CCR__SSIZ__16_Bit_Port       = 0x00000002;
    static const    UINT32 CCR__REN_mask                = 0x00000008;
    static const    UINT32 CCR__REN_shift               =          3;
    static const    UINT32 CCR__REN__Request_Enable     = 0x00000001;
    static const    UINT32 CCR__REN__Request_Disable    = 0x00000000;
    static const    UINT32 CCR__RPT_mask                = 0x00000004;
    static const    UINT32 CCR__RPT_shift               =          2;
    static const    UINT32 CCR__RPT__Repeat_Enable      = 0x00000001;
    static const    UINT32 CCR__RPT__Repeat_Disable     = 0x00000000;
    static const    UINT32 CCR__FRC_mask                = 0x00000002;
    static const    UINT32 CCR__FRC_shift               =          1;
    static const    UINT32 CCR__FRC__Force_DMA_Cycle    = 0x00000001;
    static const    UINT32 CCR__CEN_mask                = 0x00000001;
    static const    UINT32 CCR__CEN_shift               =          0;
    static const    UINT32 CCR__CEN__Enable             = 0x00000001;
    static const    UINT32 CCR__CEN__Disable            = 0x00000000;
        
    static const    UINT32 RSSR__RSS_mask               = 0x0000001F;
    static const    UINT32 RSSR__RSS_shift              =          0;

    static const    UINT32 BLR__BL_mask                 = 0x0000003F;
    static const    UINT32 BLR__BL_shift                =          0;

    static const    UINT32 RTOR__EN_mask                = 0x00008000;
    static const    UINT32 RTOR__EN_shift               =         15;
    static const    UINT32 RTOR__EN__Enable_Req_TO      = 0x00000001;
    static const    UINT32 RTOR__EN__Disable_Req_TO     = 0x00000000;
    static const    UINT32 RTOR__CLK_mask               = 0x00004000;
    static const    UINT32 RTOR__CLK_shift              =         14;
    static const    UINT32 RTOR__CLK__HCLK              = 0x00000000;
    static const    UINT32 RTOR__CLK__32_768_Khz        = 0x00000001;
    static const    UINT32 RTOR__PSC_mask               = 0x00002000;
    static const    UINT32 RTOR__PSC_shift              =         13;
    static const    UINT32 RTOR__PSC__Div_By_1          = 0x00000000;
    static const    UINT32 RTOR__PSC__Div_By_256        = 0x00000001;
    static const    UINT32 RTOR__CNT_mask               = 0x00001FFF;
    static const    UINT32 RTOR__CNT_shift              =          0;
    static const    UINT32 BUCR__CCNT_mask              = 0x0000FFFF;
    static const    UINT32 BUCR__CCNT_shift             =          0;
};
//
// DMA Controller
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PWM
//
struct MC9328MXL_PWM
{
    static const UINT32 c_Base = 0x00208000;

    //--//
    
    static const UINT32 c_PWM_OUTPUT = MC9328MXL_GPIO::c_Port_A_02;

    //--//

    /****/ volatile UINT32 PWMC;
    static const    UINT32 PWMC__HCTR         = 0x00040000;
    static const    UINT32 PWMC__BCTR         = 0x00020000;
    static const    UINT32 PWMC__SWR          = 0x00010000;
    static const    UINT32 PWMC__CLKSRC       = 0x00008000;
    static const    UINT32 PWMC__PRESC_mask   = 0x00007F00;
    static const    UINT32 PWMC__PRESC_shift  =          8;
    static const    UINT32 PWMC__IRQ          = 0x00000080;
    static const    UINT32 PWMC__IRQEN        = 0x00000040;
    static const    UINT32 PWMC__FIFOAV       = 0x00000020;
    static const    UINT32 PWMC__EN           = 0x00000010;
    static const    UINT32 PWMC__REPEAT_mask  = 0x0000000C;
    static const    UINT32 PWMC__REPEAT_shift =          2;
    static const    UINT32 PWMC__CLKSEL_DIV2  = 0x00000000;
    static const    UINT32 PWMC__CLKSEL_DIV4  = 0x00000001;
    static const    UINT32 PWMC__CLKSEL_DIV8  = 0x00000010;
    static const    UINT32 PWMC__CLKSEL_DIV16 = 0x00000011;
    static const    UINT32 PWMC__CLKSEL_mask  = 0x00000003;
    static const    UINT32 PWMC__CLKSEL_shift =          0;

    /****/ volatile UINT32 PWMS;
    static const    UINT32 PWMS__SAMPLE_mask  = 0x0000FFFF;

    /****/ volatile UINT32 PWMP;
    static const    UINT32 PWMP__PERIOD_mask  = 0x0000FFFF;
    
    /****/ volatile UINT32 PWMCNT;
    static const    UINT32 PWMCNT__COUNT_mask = 0x0000FFFF;
};
//
// PWM
//////////////////////////////////////////////////////////////////////////////

/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/
/**********************************************************************************************************************/

struct MC9328MXL
{
    static const UINT32 c_UncachableMask = 0x80000000;

    static MC9328MXL_EIM     & EIM  (         ) { return *(MC9328MXL_EIM     *)(size_t)(      MC9328MXL_EIM     ::c_Base                                      ); }
    static MC9328MXL_SC      & SC   (         ) { return *(MC9328MXL_SC      *)(size_t)(      MC9328MXL_SC      ::c_Base                                      ); }
    static MC9328MXL_SDRAM   & SDRAM(         ) { return *(MC9328MXL_SDRAM   *)(size_t)(      MC9328MXL_SDRAM   ::c_Base                                      ); }
    static MC9328MXL_AITC    & AITC (         ) { return *(MC9328MXL_AITC    *)(size_t)(      MC9328MXL_AITC    ::c_Base                                      ); }
    static MC9328MXL_GPIO    & GPIO ( int sel ) { return *(MC9328MXL_GPIO    *)(size_t)(      MC9328MXL_GPIO    ::c_Base + MC9328MXL_GPIO::c_Base_Offset * sel); }
    static MC9328MXL_CMU     & CMU  (         ) { return *(MC9328MXL_CMU     *)(size_t)(      MC9328MXL_CMU     ::c_Base                                      ); }
    static MC9328MXL_TIMER   & TIMER( int sel );
    static MC9328MXL_I2C     & I2C  (         ) { return *(MC9328MXL_I2C     *)(size_t)(      MC9328MXL_I2C     ::c_Base                                      ); }
    static MC9328MXL_LCDC    & LCDC (         ) { return *(MC9328MXL_LCDC    *)(size_t)(      MC9328MXL_LCDC    ::c_Base                                      ); }
    static MC9328MXL_WATCHDOG& WTDG (         ) { return *(MC9328MXL_WATCHDOG*)(size_t)(      MC9328MXL_WATCHDOG::c_Base                                      ); }
    static MC9328MXL_USB     & USB  (         ) { return *(MC9328MXL_USB     *)(size_t)(      MC9328MXL_USB     ::c_Base                                      ); }
    static MC9328MXL_PWM     & PWM  (         ) { return *(MC9328MXL_PWM     *)(size_t)(      MC9328MXL_PWM     ::c_Base                                      ); }
    static MC9328MXL_DMA     & DMA  (         ) { return *(MC9328MXL_DMA     *)(size_t)(      MC9328MXL_DMA     ::c_Base                                      ); }
    static MC9328MXL_SPI     & SPI  ( int sel ) { if(sel) return *(MC9328MXL_SPI     *)(size_t)MC9328MXL_SPI::c_Base_2										   ;
												  else	  return *(MC9328MXL_SPI     *)(size_t)MC9328MXL_SPI::c_Base_1										   ; }
    static MC9328MXL_USART   & USART( int sel ) { if(sel) return *(MC9328MXL_USART   *)(size_t)MC9328MXL_USART::c_Base_1									   ;
												  else    return *(MC9328MXL_USART   *)(size_t)MC9328MXL_USART::c_Base_0									   ; }

    //--//

    __forceinline static void Debug_Emit( char c )
    {
        MC9328MXL_USART& USART = MC9328MXL::USART( 0 );

        USART.UTXDn_X[0] = c;

        while((USART.USR2_X & MC9328MXL_USART::USR2__TXDC) == 0);
    }

    static void Debug_NewLine()
    {
        MC9328MXL_USART& USART = MC9328MXL::USART( 0 );

        USART.UTXDn_X[0] = '\r';
        USART.UTXDn_X[0] = '\n';

        while((USART.USR2_X & MC9328MXL_USART::USR2__TXDC) == 0);
    }

    static void Debug_EmitHEX( UINT32 value );

    //--//

    __forceinline static void Debug_Emit2( char c )
    {
        MC9328MXL_USART& USART = MC9328MXL::USART( 1 );

        USART.UTXDn_X[0] = c;

        while((USART.USR2_X & MC9328MXL_USART::USR2__TXDC) == 0);
    }

    static void Debug_NewLine2()
    {
        MC9328MXL_USART& USART = MC9328MXL::USART( 1 );

        USART.UTXDn_X[0] = '\r';
        USART.UTXDn_X[0] = '\n';

        while((USART.USR2_X & MC9328MXL_USART::USR2__TXDC) == 0);
    }

    static void Debug_EmitHEX2( UINT32 value );
};

//
//
//
// LAYOUT SECTION
/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
// DRIVER SECTION
//
//


//////////////////////////////////////////////////////////////////////////////
// Cache
//
//

#if defined(PLATFORM_ARM_MC9328)

//--//

#if defined(COMPILE_ARM) || defined(COMPILE_THUMB)

template <typename T> void CPU_InvalidateAddress( T* address )
{
    UINT32 reg;

#ifdef __GNUC__
	asm("MOV %0, #0" : "=r" (reg) : "r" (reg));
	asm("MCR p15, 0, %0, c7, c10, 4" :: "r" (reg));
	asm("MCR p15, 0, %0, c7, c5, 1" :: "r" (reg));
	asm("MCR p15, 0, %0, c7, c6, 1" :: "r" (reg));    
#else
    __asm
    {
        mov     reg, #0
        mcr     p15, 0, reg, c7, c10, 4           // Drain Write Buffers.

        mcr     p15, 0, address, c7, c5, 1        // Invalidate DCache.
        mcr     p15, 0, address, c7, c6, 1        // Invalidate DCache.
    }
#endif
}

#elif defined(COMPILE_THUMB2)

template <typename T> void CPU_InvalidateAddress( T* address )
{
}

#endif

//--//

#endif

//
// Cache
//////////////////////////////////////////////////////////////////////////////

struct MC9328MXL_Clock_Driver
{
    static const UINT32 PERCLK1 = 0;
    static const UINT32 PERCLK2 = 1;
    static const UINT32 PERCLK3 = 2;

    static void ClockDivisor( UINT32 peripheral, UINT32 divisor );

    static UINT32 ReadClockDivisor( UINT32 peripheral );
};

//
// Clock Management Unit driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Watchdog driver
//

struct MC9328MXL_WATCHDOG_Driver
{
    static const UINT32 c_MaxCounter  = 127;
    static const UINT32 c_Granularity = 500; // ms

    //--//

    static HRESULT Enable( UINT32 TimeoutMilliseconds, WATCHDOG_INTERRUPT_CALLBACK callback, void* context );

    static void Disable();

    static void ResetCounter();

    static void ResetCpu();

private:

    static void ISR( void* Param );

    WATCHDOG_INTERRUPT_CALLBACK m_callback;
    void*                       m_context;
};

extern MC9328MXL_WATCHDOG_Driver g_MC9328MXL_WATCHDOG_Driver;

//
// Watchdog driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// MC9328MXL driver
//

struct MC9328MXL_Driver
{
    static const UINT32 c_SystemTime_Timer         = 0;
    static const UINT32 c_PerformanceCounter_Timer = 1;

#if !defined(BUILD_RTM)
    volatile UINT32* m_PerformanceCounter;
#endif

    //--//

    static BOOL Initialize();

    static void Pause();

    static void Halt();

    static void Reset();

    static void Shutdown();

    static void Hibernate();

#if !defined(BUILD_RTM)
    static void   PerformanceCounter_Initialize  ();
    static void   PerformanceCounter_Uninitialize();
    static UINT32 PerformanceCounter             ();
#endif
};

extern MC9328MXL_Driver g_MC9328MXL_Driver;

//
// MC9328MXL driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Interrupt Controller driver
//

struct MC9328MXL_AITC_Driver
{
    static const UINT32 c_VECTORING_GUARD = 64;

    //--//

    struct IRQ_VECTORING
    {
        UINT32       Index;
        HAL_CALLBACK Handler;
    };

    //--//

    static IRQ_VECTORING s_IsrTable[];

    //--//

    static void Initialize();

    static BOOL ActivateInterrupt  ( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param, UINT8 Priority );
    static BOOL DeactivateInterrupt( UINT32 Irq_Index                                                                   );

    static BOOL InterruptEnable( UINT32 Irq_Index );

    static BOOL InterruptDisable( UINT32 Irq_Index );

    static BOOL InterruptEnableState( UINT32 Irq_Index );

    static BOOL InterruptState( UINT32 Irq_Index );

#if defined(PLATFORM_ARM)
    static void IRQ_Handler();
#endif

    static IRQ_VECTORING* IRQToIRQVector( UINT32 IRQ );

private:
    static void STUB_ISRVector( void* Param );
};

//
// Interrupt Controller driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// GP I/O driver
//

struct MC9328MXL_GPIO_Driver
{
    struct PIN_ISR_DESCRIPTOR
    {
        HAL_COMPLETION                 m_completion;
        GPIO_INTERRUPT_SERVICE_ROUTINE m_isr;
        void*                          m_param;
        GPIO_PIN                       m_pin;
        GPIO_INT_EDGE                  m_intEdge;
        UINT8                          m_flags;
        UINT8                          m_status;

        //--//
        
        static const UINT8 c_Flags_Debounce        = 0x01;
        static const UINT8 c_Flags_RequireLowEdge  = 0x02;
        static const UINT8 c_Flags_RequireHighEdge = 0x04;

        static const UINT8 c_Status_AllowLowEdge   = 0x01;
        static const UINT8 c_Status_AllowHighEdge  = 0x02;

        //--//

        static void Fire( void* arg );

        void HandleDebounce( BOOL edge );
    };

    //--//

    static const UINT32 c_MaxPins           = 128;
    static const UINT32 c_MaxPorts          =   4;
    static const UINT32 c_PinsPerPort       =  32;

    static const INT32  c_MinDebounceTimeMs =    1; // 1 ms
    static const INT32  c_MaxDebounceTimeMs = 5000; // 5  s

    //--//

    static const UINT8 c_Gpio_Attributes[c_MaxPins];

    // this applies to all GPIO pins
    static const UINT8 c_NumberofResistorMode = 2;
    static const UINT8 c_GPIO_ResistorMode=(1<<RESISTOR_DISABLED) | (1<<RESISTOR_PULLUP);

    static const UINT8 c_NumberofInterruptMode = 5;
    static const UINT8 c_GPIO_InterruptMode=(1<<GPIO_INT_EDGE_LOW) | (1<<GPIO_INT_EDGE_HIGH ) | (1<<GPIO_INT_EDGE_BOTH) | (1<<GPIO_INT_LEVEL_HIGH) | (1<<GPIO_INT_LEVEL_LOW); 

    static const UINT32 c_Alt_Max_Mode       = 7;
    static const UINT32 c_Alt_Header_MASK    = 0xFFFF0000;
    static const UINT32 c_Alt_Function_MASK  = 0x0000FFFF;
    static const UINT32 c_Alt_Primary        = 0x00000000;
    static const UINT32 c_Mux_In_H           = 0x00010000;
    static const UINT32 c_Mux_Aout_H         = 0x00020000;
    static const UINT32 c_Mux_Bout_H         = 0x00040000;
    static const UINT32 c_Alt_Altenate_H     = 0x00080000;
    static const UINT32 c_GPIO_AIN           = c_Mux_In_H   | MC9328MXL_GPIO::OCR__AIN_REG;
    static const UINT32 c_GPIO_BIN           = c_Mux_In_H   | MC9328MXL_GPIO::OCR__BIN_REG;
    static const UINT32 c_GPIO_CIN           = c_Mux_In_H   | MC9328MXL_GPIO::OCR__CIN_REG;
    static const UINT32 c_GPIO_AOUT          = c_Mux_Aout_H | MC9328MXL_GPIO::ICONF__GPIO_IN;
    static const UINT32 c_GPIO_BOUT          = c_Mux_Bout_H | MC9328MXL_GPIO::ICONF__GPIO_IN;

    static const UINT32 GPIO_MODE_TO_MC9328MXL_MODE[c_Alt_Max_Mode];
    // --//

    PIN_ISR_DESCRIPTOR m_PinIsr            [c_MaxPins ];
    UINT32             m_PinReservationInfo[c_MaxPorts];
    UINT32             m_DebounceTicks;

    //--//

    static BOOL Initialize  ();

    static BOOL Uninitialize();

    static UINT32 Attributes( GPIO_PIN Pin );

    static void DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 direction, GPIO_ALT_MODE AltFunction );

    static void EnableOutputPin( GPIO_PIN Pin, BOOL InitialState );

    static BOOL EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState );

    static BOOL GetPinState( GPIO_PIN Pin );

    static void SetPinState( GPIO_PIN Pin, BOOL PinState );

    static void ISR( void* Param );

    static void SetResistor( GPIO_PIN Pin, GPIO_RESISTOR ResistorState );

    static BOOL PinIsBusy( GPIO_PIN Pin );

    static BOOL ReservePin( GPIO_PIN Pin, BOOL fReserve );

    static UINT32 GetDebounce();

    static BOOL SetDebounce( INT64 debounceTimeMilliseconds );

    static void GetPinsMap( UINT8* pins, size_t size );
    
    static UINT8 GetSupportedResistorModes( GPIO_PIN pin );

    static UINT8 GetSupportedInterruptModes( GPIO_PIN pin );
    
private:

    static UINT32 PinToPort( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin / c_PinsPerPort;
    }

    static UINT32 PinToBit( GPIO_PIN pin )
    {
        ASSERT(pin < c_MaxPins);

        return pin % c_PinsPerPort;
    }

    static UINT32 BitToPin( UINT32 bit, UINT32 port )
    {
        ASSERT(bit  < c_PinsPerPort);
        ASSERT(port < c_MaxPorts);

        return bit + (port * c_PinsPerPort);
    }

    static void STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param );
};

extern MC9328MXL_GPIO_Driver g_MC9328MXL_GPIO_Driver;

//
// GP I/O driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// USART driver
//

struct MC9328MXL_USART_Driver
{
    static const UINT32 c_RefFlagRx = 0x01;
    static const UINT32 c_RefFlagTx = 0x02;

    static const UINT32 c_COM1      = 0;
    static const UINT32 c_COM2      = 1;

    //--//

    INT8 m_RefFlags[2];

    //--//

    static BOOL Initialize( int comPort, int baudRate, int Parity, int DataBits, int StopBits, int FlowValue );

    static BOOL Uninitialize( int comPort );

    static BOOL TxBufferEmpty( int comPort );

    static BOOL TxShiftRegisterEmpty( int comPort );

    static void WriteCharToTxBuffer( int comPort,  UINT8 c );

    static void TxBufferEmptyInterruptEnable( int comPort, BOOL enable );

    static BOOL TxBufferEmptyInterruptState( int comPort );

    static void RxBufferFullInterruptEnable( int comPort, BOOL enable );

    static BOOL RxBufferFullInterruptState( int comPort );

    static BOOL TxHandshakeEnabledState( int comPort );

    static void ProtectPins( int comPort, BOOL on );

    static void GetPins(int comPort,  GPIO_PIN& rxPin, GPIO_PIN& txPin,GPIO_PIN& ctsPin, GPIO_PIN& rtsPin );

    static void BaudrateBoundary(int ComPortNum, UINT32& maxBaudrateHz, UINT32& minBaudrateHz);

    static BOOL IsBaudrateSupported(int ComPortNum, UINT32 & BaudrateHz);
private:

    static void RxISR( void* Param );

    static void TxISR( void* Param );

    static void ParityError( void* Param );

    static void RefFlags( int ComPortNum, INT8 RefFlags, BOOL Add );
};

extern MC9328MXL_USART_Driver g_MC9328MXL_USART_Driver;

//
// USART driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// TIMER driver
//

struct MC9328MXL_TIMER_Driver
{
    const static UINT32 c_MaxTimers = 2;
#if defined(FIQ_SAMPLING_PROFILER)
    HAL_COMPLETION m_dump;
#endif

    //--//

    BOOL m_configured[c_MaxTimers];

    static BOOL   Initialize   ( UINT32 Timer, BOOL FreeRunning, UINT32 ClkSource, UINT32 Prescaler, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL   Uninitialize ( UINT32 Timer                                                                                              );
    static UINT32 ReadCounter  ( UINT32 Timer                                                                                              );

    static void EnableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        MC9328MXL_TIMER& TIMER = MC9328MXL::TIMER( Timer );

        TIMER.TCTL |= MC9328MXL_TIMER::TCTL_IRQEN;
    }

    static void DisableCompareInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        MC9328MXL_TIMER& TIMER = MC9328MXL::TIMER( Timer );

        TIMER.TCTL &= ~MC9328MXL_TIMER::TCTL_IRQEN;
    }

    static void ForceInterrupt( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);
		
		if(Timer)
		{
			MC9328MXL::AITC().ForceInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_TIMER2_INT );
		}
		else
		{
			MC9328MXL::AITC().ForceInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_TIMER1_INT );
		}
    }

    static void SetCompare( UINT32 Timer, UINT32 Compare )
    {
        ASSERT(Timer < c_MaxTimers);

        MC9328MXL_TIMER& TIMER = MC9328MXL::TIMER( Timer );

        TIMER.TCMP = Compare;
    }

    static BOOL DidCompareHit( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        MC9328MXL_TIMER& TIMER = MC9328MXL::TIMER( Timer );

        return (TIMER.TSTAT & MC9328MXL_TIMER::TSTAT_COMP) != 0;
    }

    //
    // TSTAT has a particular reset semantic: you have to read the TSTAT with the bit set before you can
    //
    static void ResetCompareHit( UINT32 Timer )
    {
        ASSERT(Timer < c_MaxTimers);

        MC9328MXL_TIMER& TIMER = MC9328MXL::TIMER( Timer );

        TIMER.TSTAT = 0;
    }
};

extern MC9328MXL_TIMER_Driver g_MC9328MXL_TIMER_Driver;

//
// TIMER driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// System Time driver
//

struct MC9328MXL_TIME_Driver
{
    static const UINT32 c_OverflowCheck = 0x80000000;

    UINT64 m_lastRead;
    UINT64 m_nextCompare;

    static BOOL Initialize  ();
    static BOOL Uninitialize();

    static UINT64 CounterValue();

    static void SetCompareValue( UINT64 CompareValue );

    static INT64 TicksToTime( UINT64 Ticks );

    static INT64 CurrentTime();

    static void Sleep_uSec( UINT32 uSec );

    static void Sleep_uSec_Loop( UINT32 uSec );

private:
    static void ISR( void* Param );
};

extern MC9328MXL_TIME_Driver g_MC9328MXL_TIME_Driver;

//
// System Time driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// I2C driver
//

struct I2C_HAL_XACTION_UNIT;
struct I2C_HAL_XACTION;
struct I2C_CONFIGURATION;

struct MC9328MXL_I2C_Driver
{
	BOOL m_initialized;

	//--//

    I2C_HAL_XACTION*       m_currentXAction;
    I2C_HAL_XACTION_UNIT*  m_currentXActionUnit;

    //--//

    static const UINT8 c_DirectionWrite = 0x00;
    static const UINT8 c_DirectionRead  = 0x01;
    static const UINT8 c_AddressCycle   = 0x01;

    //--//

    static UINT16 s_Divider[64]; 

    //--//

    static BOOL  Initialize         (                                              );
    static BOOL  Uninitialize       (                                              );
    static void  MasterXAction_Start( I2C_HAL_XACTION* xAction, bool repeatedStart );
    static void  MasterXAction_Stop (                                              );
    static UINT8 GetClockRate       ( UINT32 rateKhz                               );
    static void  GetPins            ( GPIO_PIN& scl, GPIO_PIN& sda                 );

private:
    static BOOL IsBusBusy    (                               );
    static void WriteToSubordinate ( I2C_HAL_XACTION_UNIT* xAction );
    static void ReadFromSubordinate( I2C_HAL_XACTION_UNIT* xAction );
    static void ISR          ( void*            arg          );
};

extern MC9328MXL_I2C_Driver g_MC9328MXL_I2C_Driver;

//
// I2C driver
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// SPI  driver
//

struct MC9328MXL_SPI_Driver
{
    BOOL m_initialized[MC9328MXL_SPI::c_MAX_SPI];
    BOOL m_Enabled    [MC9328MXL_SPI::c_MAX_SPI];

    static BOOL Initialize  ();
    static void Uninitialize();
    static void GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi );

    static BOOL nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset );
    static BOOL nWrite8_nRead8  ( const SPI_CONFIGURATION& Configuration, UINT8*  Write8 , INT32 WriteCount, UINT8*  Read8 , INT32 ReadCount, INT32 ReadStartOffset );

    static BOOL Xaction_Start( const SPI_CONFIGURATION& Configuration );
    static BOOL Xaction_Stop ( const SPI_CONFIGURATION& Configuration );

    static BOOL Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction );
    static BOOL Xaction_nWrite8_nRead8  ( SPI_XACTION_8&  Transaction );

private:
    static void ISR( void* Param );
};

extern MC9328MXL_SPI_Driver g_MC9328MXL_SPI_Driver;

//
// SPI driver
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// External Bus Interface Unit
//

struct MC9328MXL_EIM_Driver
{
    static void ConfigMemoryBlock( const CPU_MEMORY_CONFIG& CPUMemoryConfig);

    static BOOL Memory_ReadOnly( const CPU_MEMORY_CONFIG& CPUMemoryConfig, BOOL ReadOnly );

private:
    static void MemoryConfig_To_Control( const CPU_MEMORY_CONFIG& CPUMemoryConfig, UINT32& CS_L, UINT32& CS_U );
};

//
// External Bus Interface Unit
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// USB driver
//

#include <..\pal\com\usb\USB.h>

struct MC9328MXL_USB_Driver
{
    static const UINT32 c_default_ctrl_packet_size = 8;

    USB_CONTROLLER_STATE*          pControllerState;
    
#if defined(USB_REMOTE_WAKEUP)
    #define USB_MAX_REMOTE_WKUP_RETRY 5

    HAL_COMPLETION          RemoteWKUP10msCompletion;
    HAL_COMPLETION          RemoteWKUP100msEOPCompletion;
    UINT32                  RemoteWkUpRetry;
#endif

    UINT8                   ControlPacketBuffer[c_default_ctrl_packet_size];
    UINT16                  EndpointStatus[MC9328MXL_USB::c_USB_MAX_EP];
    BOOL                    TxRunning [MC9328MXL_USB::c_USB_MAX_EP];
    BOOL                    TxNeedZLPS[MC9328MXL_USB::c_USB_MAX_EP];

    UINT8                   PreviousDeviceState;
    UINT8                   RxExpectedToggle[MC9328MXL_USB::c_USB_MAX_EP];
    BOOL                    PinsProtected;
    BOOL                    FirstDescriptorPacket;

#if defined(USB_METRIC_COUNTING)
    USB_PERFORMANCE_METRICS PerfMetrics;
#endif

    //--//

    static USB_CONTROLLER_STATE *GetState( int Controller );

    static HRESULT Initialize( int Controller );

    static HRESULT Uninitialize( int Controller );

    static BOOL StartOutput( USB_CONTROLLER_STATE* State, int endpoint );

    static BOOL RxEnable( USB_CONTROLLER_STATE* State, int endpoint );

    static BOOL GetInterruptState();

    static BOOL ProtectPins( int Controller, BOOL On );

private:
    static void ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint );

    static void StartHardware();

    static void StopHardware ();

    static void TxPacket( USB_CONTROLLER_STATE* State, int endpoint );

    static void ControlNext();

    static void SuspendEvent();
    static void ResumeEvent ();
    static void ResetEvent  ();

    static void   Global_ISR      ( void* Param );
    static void   EP0_ISR         ( void* Param );    
    static void   EP_TxISR        ( void* Param );    
    static void   EP_RxISR        ( void* Param );    

#if defined(USB_REMOTE_WAKEUP)
    static void RemoteWkUp_ISR    ( void* Param );
    static void RemoteWkUp_Process( void* Param );
#endif
};

extern MC9328MXL_USB_Driver g_MC9328MXL_USB_Driver;


//
// USB DRIVER
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// DMA driver
//

struct MC9328MXL_DMA_Driver
{
    static const int c_DummyDMAChannel_10 = 10;
    static const int c_DummyDMAChannel_5  =  5;
    static const int c_MemCpy2DChannel    =  9;
    static const int c_MemCpyChannelStart =  8; // We start at high numbers because the higher the channel number the higher the priority
    static const int c_MemCpyChannelEnd   =  0;

    static const UINT32 c_MemCpy2DWaitTimeOut = 100000; // cycles

    //--//

    static BOOL Initialize  ();
    static BOOL Uninitialize();

    static void MemCpy  ( void* dst, void* src, UINT32 size, BOOL async );
    static void MemCpy2D( void* dst, void* src, UINT32 width, UINT32 height, UINT32 dataWidth, BOOL async );

    static void StartDummyDMA();

private:
    static void StartDummyDMAChannel(int DMAChannel);
    static void* TranslateAddress( void* virtualAddress );
};

extern MC9328MXL_DMA_Driver g_MC9328MXL_DMA_Driver;

//
// DMA driver
//////////////////////////////////////////////////////////////////////////////

//
//
// DRIVER SECTION
/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////
// CONFIG SECTION
//
//

///////////////////////////////////////////////////////////////////////////////
// USART SECTION
//

struct MC9328MXL_USART_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    UINT8         TxProtectInput;
    UINT8         RxProtectInput;
    UINT8         CTSProtectInput;
    UINT8         RTSProtectInput;
    GPIO_RESISTOR TxProtectResistor;
    GPIO_RESISTOR RxProtectResistor;
    GPIO_RESISTOR CTSProtectResistor;
    GPIO_RESISTOR RTSProtectResistor;
    UINT8         TxProtectOutputValue;
    UINT8         RxProtectOutputValue;
    UINT8         CTSProtectOutputValue;
    UINT8         RTSProtectOutputValue;

    //--//

    static LPCSTR GetDriverName() { return "MC9328MXL_USART"; }
};

extern MC9328MXL_USART_CONFIG g_MC9328MXL_USART_Config;
//
// USART SECTION
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// USB SECTION
//

struct MC9328MXL_USB_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    static LPCSTR GetDriverName() { return "MC9328MXL_USB"; }
};

extern MC9328MXL_USB_CONFIG g_MC9328MXL_USB_Config;

//
// USB SECTION
///////////////////////////////////////////////////////////////////////////////
//
//
// CONFIG SECTION
/////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _MC9328MXL_H_ 1
