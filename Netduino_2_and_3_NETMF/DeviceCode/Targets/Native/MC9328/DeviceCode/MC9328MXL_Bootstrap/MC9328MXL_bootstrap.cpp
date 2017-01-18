////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\MC9328MXL.h"

///////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//#define PLL_192_MHZ

// the arm3.0 compiler optimizes out much of the boot strap code which causes
// the device not to boot for RTM builds (optimization level 3), adding this pragma 
// assures that the compiler will use the proper optimization level for this code
#if !defined(DEBUG)
#pragma O2
#endif

#pragma arm section code = "SectionForBootstrapOperations"

///////////////////////////////////////////////////////////////////////////////


void __section(SectionForBootstrapOperations) MC9328MXL::Debug_EmitHEX( UINT32 value )
{
    for(int i=7; i>=0; i--)
    {
        UINT32 digit = (value >> (4*i)) & 0xF;

        Debug_Emit( digit < 10 ? digit + '0' : digit + 'A' - 10 );
    }
}

void __section(SectionForBootstrapOperations) MC9328MXL::Debug_EmitHEX2( UINT32 value )
{
    for(int i=7; i>=0; i--)
    {
        UINT32 digit = (value >> (4*i)) & 0xF;

        Debug_Emit2( digit < 10 ? digit + '0' : digit + 'A' - 10 );
    }
}

//--//

static const UINT32  c_Bootstrap_Registers   =           0x00200000;
static const UINT32  c_Bootstrap_SDRAM_Begin =           SRAM1_MEMORY_Base;
static const UINT32  c_Bootstrap_SDRAM_End   =           SRAM1_MEMORY_Base + SRAM1_MEMORY_Size;
static const UINT32  c_Bootstrap_FLASH_Begin =           FLASH_MEMORY_Base;
static const UINT32  c_Bootstrap_FLASH_End   =           FLASH_MEMORY_Base + FLASH_MEMORY_Size;
static UINT32* const c_Bootstrap_BaseOfTTBs  = (UINT32*)(c_Bootstrap_SDRAM_End - ARM9_MMU::c_TTB_size);

#if defined(COMPILE_ARM) || defined(COMPILE_THUMB)
FORCEINLINE void __section(SectionForBootstrapOperations) Bootstrap_Delay( INT32 count )
{
#ifdef __GNUC__
    asm("1: subs %0, %0, #4\nbgt 1b" :: "r" (count));
#else
    __asm
    {
Loop:
        subs    count, count, #4
        bgt     Loop
    }
#endif
}
#elif defined(COMPILE_THUMB2)
FORCEINLINE void __section(SectionForBootstrapOperations) Bootstrap_Delay( INT32 count )
{
    // TODO: WRITE THUMB2 code
}
#endif

FORCEINLINE void __section(SectionForBootstrapOperations) Bootstrap_Delay()
{
    Bootstrap_Delay( 800 );
}

void __section(SectionForBootstrapOperations) BootstrapCode_DumpTTBs()
{
    UINT32  address = 0;
    UINT32* dst     = ARM9_MMU::GetL1Entry( c_Bootstrap_BaseOfTTBs, address );

    do
    {
        MC9328MXL::Debug_EmitHEX( address );
        MC9328MXL::Debug_Emit   ( ' '     );

        MC9328MXL::Debug_EmitHEX( (UINT32)dst );
        MC9328MXL::Debug_Emit   ( ' '         );

        MC9328MXL::Debug_EmitHEX( *dst );
        MC9328MXL::Debug_Emit   ( '\r' );
        MC9328MXL::Debug_Emit   ( '\n' );

        dst++;
        address += ARM9_MMU::c_MMU_L1_size;
    } while(address < c_Bootstrap_FLASH_End);
}

#if defined(PLL_192_MHZ)
#if defined(COMPILE_ARM)  || defined(COMPILE_THUMB)

void __section(SectionForBootstrapOperations) TurnOnAsyncMode(/*int x, int y, int z*/)
{
#ifdef __GNUC__
	unsigned int r0, r2;
	asm("MRC p15, 0, %0, c1, c0, 0" : "=r" (r0));
	asm("MOV %0, #0xC0000000" : "=r" (r2));
	asm("ORR %0, %1, %2" : "=r" (r0) : "r" (r2), "r" (r0));
	asm("MCR p15, 0, %0, c1, c0, 0" :: "r" (r0));
#else
     __asm
    {
        mrc p15,0,r0,c1,c0,0
        mov r2,#0xC0000000
        orr r0,r2,r0
        mcr p15,0,r0,c1,c0,0
    }
#endif
}

#elif defined(COMPILE_THUMB2)
void __section(SectionForBootstrapOperations) TurnOnAsyncMode(/*int x, int y, int z*/)
{
    // TODO: Write thumb2 code
}
#endif // #if defined(COMPILE_ARM)  || defined(COMPILE_THUMB)

#endif // #if defined(PLL_192_MHZ)


void __section(SectionForBootstrapOperations) BootstrapCode_Clocks()
{
    //
    // Ensure that FLASH access time is at its reset value.
    //
    {
        //MC9328MXL_EIM::DEVICE &dev = MC9328MXL::EIM().Device[0];

        // The reset value of this register gives FLASH its maximum access time
		MC9328MXL::EIM().Device[0].CS_U  = 62 << MC9328MXL_EIM::DEVICE::CS_U__WSC__shift;     // Set FLASH CS_U reg to reset value
    }

    //
    // Change drive strength to 50Mhz, 30pF.
    //
    {
        MC9328MXL::SC().GPCR = (MC9328MXL_SC::GPCR__DS_CNTL__50MHz_30pF << MC9328MXL_SC::GPCR__DS_CNTL_shift)	|
							   (MC9328MXL_SC::GPCR__DS_ADDR__50MHz_30pF << MC9328MXL_SC::GPCR__DS_ADDR_shift)	|
							   (MC9328MXL_SC::GPCR__DS_DATA__50MHz_30pF << MC9328MXL_SC::GPCR__DS_DATA_shift)	|
							    MC9328MXL_SC::GPCR__IP_CLK_GATING_EN											|
							    MC9328MXL_SC::GPCR__HCLK_GATING_EN												;

        Bootstrap_Delay();
    }

#if defined(PLL_192_MHZ)
    {
        UINT32 val = 0;
       
        // Set the new MCU PLL values for 16.384 MHz in / 192 MHz out
        val |=  0 << MC9328MXL_CMU::MPCTL0__PD_shift;
        val |= 63 << MC9328MXL_CMU::MPCTL0__MFD_shift;
        val |= 55 << MC9328MXL_CMU::MPCTL0__MFN_shift;
        val |=  5 << MC9328MXL_CMU::MPCTL0__MFI_shift;

        MC9328MXL::CMU().MPCTL0 = val;
        MC9328MXL::CMU().MPCTL1 = MC9328MXL_CMU::MPCTL1__BRMO;

        Bootstrap_Delay();
    }
#endif

#if defined(PLATFORM_ARM_AUXD) && (HARDWARE_BOARD_TYPE == HARDWARE_BOARD_CSB536MS_REV_V1_0 || HARDWARE_BOARD_TYPE == HARDWARE_BOARD_CSB935FS_REV_V1_1)
    {
        UINT32 val = 0;

        // Set the new system PLL values for 16.000 MHz in / 96 MHz out
        val |= 1 << MC9328MXL_CMU::SPCTL0__PD_shift;
        val |= 1 << MC9328MXL_CMU::SPCTL0__MFD_shift;
        val |= 0 << MC9328MXL_CMU::SPCTL0__MFN_shift;
        val |= 6 << MC9328MXL_CMU::SPCTL0__MFI_shift;

        MC9328MXL::CMU().SPCTL0 = val;

        MC9328MXL::CMU().SPCTL1 = MC9328MXL_CMU::SPCTL1__BRMO;      // Set BRM to 2nd order (MF = .0 < .1)

        Bootstrap_Delay();
    }
#elif (HARDWARE_BOARD_TYPE >= HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2)
    {
        // The oddball numbers below are due to the fact that this board uses a 32.768 KHz crystal
        // rather than the 32 KHz crystal called out on the schematic.  This gives 32768 x 512 or
        // a 16.777216 MHz clock feeding the DPLL.  This times 5-678/939 gets us within 0.2 ppm
        // of 96 MHz.
        UINT32 val = 0;

        // Set the new system PLL values for 32.768 KHz in / 96 MHz out
        val |=   1 << MC9328MXL_CMU::SPCTL0__PD_shift;
        val |= 938 << MC9328MXL_CMU::SPCTL0__MFD_shift;
        val |= 678 << MC9328MXL_CMU::SPCTL0__MFN_shift;
        val |=   5 << MC9328MXL_CMU::SPCTL0__MFI_shift;

        MC9328MXL::CMU().SPCTL0 = val;

        Bootstrap_Delay();
    }
#else
    {
        UINT32 val = 0;

        // Set the new system PLL values for 16.384 MHz in / 96 MHz out
        val |=  1 << MC9328MXL_CMU::SPCTL0__PD_shift;
        val |= 63 << MC9328MXL_CMU::SPCTL0__MFD_shift;
        val |= 55 << MC9328MXL_CMU::SPCTL0__MFN_shift;
        val |=  5 << MC9328MXL_CMU::SPCTL0__MFI_shift;

        MC9328MXL::CMU().SPCTL0 = val;

        Bootstrap_Delay();
    }
#endif

    {
        UINT32 val = 0;

        // Sysclk = Fclk = 96MHz
        // Bclk = HClk = Sysclk / 1 = 96MHz
        // Enable the 16MHz oscillator and the PLLs

        val |=  MC9328MXL_CMU::CSCR__CLKO_SEL__HCLK  << MC9328MXL_CMU::CSCR__CLKO_SEL_shift;
        val |=  (2 - 1)                              << MC9328MXL_CMU::CSCR__USB_DIV_shift;
        val |=  (4 - 1)                              << MC9328MXL_CMU::CSCR__SD_CNT_shift;
        val |=  MC9328MXL_CMU::CSCR__SPLL_RESTART;
#if (HARDWARE_BOARD_TYPE < HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2)
        // The version 1.2 AUXD board has no 16MHz oscillator
        val |=  MC9328MXL_CMU::CSCR__OSC_EN;
        val |=  MC9328MXL_CMU::CSCR__System_SEL;
#endif
        val |=  MC9328MXL_CMU::CSCR__PRESC__DIV_BY_1 << MC9328MXL_CMU::CSCR__PRESC_shift;
        val |=  (1 - 1)                              << MC9328MXL_CMU::CSCR__BCLK_DIV_shift;
        val |=  MC9328MXL_CMU::CSCR__SPEN;
#if defined(PLL_192_MHZ)
        val |= MC9328MXL_CMU::CSCR__MPEN;                
        val |= MC9328MXL_CMU::CSCR__MPLL_RESTART;
#else        
        val &= ~MC9328MXL_CMU::CSCR__MPEN;
#endif

        MC9328MXL::CMU().CSCR = val;
        Bootstrap_Delay();
    }

    {
        UINT32 val = 0;

        // Write the peripheral clock dividers.
        val |= (1 - 1) << MC9328MXL_CMU::PCDR__PCLK_DIV3_shift; // (div by 1 [96MHz])
        val |= (1 - 1) << MC9328MXL_CMU::PCDR__PCLK_DIV2_shift; // (div by 1 [96MHz])
        val |= (6 - 1) << MC9328MXL_CMU::PCDR__PCLK_DIV1_shift; // (div by 6 [16MHz])
        MC9328MXL::CMU().PCDR = val;
        Bootstrap_Delay();
    }

#if defined(PLL_192_MHZ)
    TurnOnAsyncMode();
#endif
}

#if (HARDWARE_BOARD_TYPE >= HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2)

// Version 1.2 AUXD board has larger x16 SDRAM instead of x32
void __section(SectionForBootstrapOperations) BootstrapCode_SDRAM()
{
    // SDRAM 4Mx16x4, IAM = 0, CS2, CL2, HCLK = 48MHz, D[0:15]
    UINT32 val_base = 0;

    // Even with the pragma at the top of this file, the 3.0 compiler still optimizes out
    // SDRAM reads -- *(volatile UINT16*)(c_Bootstrap_SDRAM_Begin + 0x100000);
    volatile UINT32 tmp = 0;
   

    val_base |= MC9328MXL_SDRAM::SDCTL__SDE__enable       << MC9328MXL_SDRAM::SDCTL__SDE_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__ROW__13           << MC9328MXL_SDRAM::SDCTL__ROW_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__COL__9            << MC9328MXL_SDRAM::SDCTL__COL_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__IAM__disable      << MC9328MXL_SDRAM::SDCTL__IAM_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__DSIZ__16bit__15_0 << MC9328MXL_SDRAM::SDCTL__DSIZ_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__SCL__3_clocks     << MC9328MXL_SDRAM::SDCTL__SCL_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__SRP__2clks        << MC9328MXL_SDRAM::SDCTL__SRP_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__SRCD__2_clocks    << MC9328MXL_SDRAM::SDCTL__SRCD_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__SRC__6_clocks     << MC9328MXL_SDRAM::SDCTL__SRC_shift;

    //
    // Set precharge all command
    //
    {
        UINT32 val = val_base;

        val |= MC9328MXL_SDRAM::SDCTL__SMODE__Precharge_Command << MC9328MXL_SDRAM::SDCTL__SMODE_shift;

        MC9328MXL::SDRAM().SDCTL[0] = val;
    }

    // Issue precharge all command
    tmp = *(volatile UINT16*)(c_Bootstrap_SDRAM_Begin + 0x100000); // A10 bit set to high for precharge ALL


    //
    // Set AutoRefresh Command
    //
    {
        UINT32 val = val_base;

        val |= MC9328MXL_SDRAM::SDCTL__SMODE__Auto_Refresh_Command << MC9328MXL_SDRAM::SDCTL__SMODE_shift;

        MC9328MXL::SDRAM().SDCTL[0] = val;
    }

    // Issue 8 AutoRefresh Command
    for(int i=8; --i >= 0; )
    {
        tmp = *(volatile UINT16*)(c_Bootstrap_SDRAM_Begin + 0x100000);
    }


    //
    // Set Mode Register
    //
    {
        UINT32 val = val_base;

        val |= MC9328MXL_SDRAM::SDCTL__SMODE__Set_Mode_Register_Command << MC9328MXL_SDRAM::SDCTL__SMODE_shift;

        MC9328MXL::SDRAM().SDCTL[0] = val;
    }

    // Issue Mode Register Command
    tmp = *(volatile UINT16*)(c_Bootstrap_SDRAM_Begin + 0x8CC00); // WB = 1, CAS Latency = 011, Burst Length = 011


    //
    // Set SMODE to Normal Read/Write and Enable SREFR to refresh 4 rows every 32kHz clock edge
    //
    {
        UINT32 val = val_base;

        val |= MC9328MXL_SDRAM::SDCTL__SMODE__Normal_ReadWrite << MC9328MXL_SDRAM::SDCTL__SMODE_shift;
        val |= MC9328MXL_SDRAM::SDCTL__SREFR__4Row_per_clock   << MC9328MXL_SDRAM::SDCTL__SREFR_shift;

        MC9328MXL::SDRAM().SDCTL[0] = val;
    }
}

#else

void __section(SectionForBootstrapOperations) BootstrapCode_SDRAM()
{

    // SDRAM 8Mx16x2, IAM = 0, CS2, CL2, HCLK = 48MHz
             UINT32 val_base = 0;
    // Even with the pragma at the top of this file, the 3.0 compiler still optimizes out
    // SDRAM reads -- *(volatile UINT16*)(c_Bootstrap_SDRAM_Begin + 0x100000);
    volatile UINT32 tmp;

    val_base |= MC9328MXL_SDRAM::SDCTL__SDE__enable    << MC9328MXL_SDRAM::SDCTL__SDE_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__ROW__12        << MC9328MXL_SDRAM::SDCTL__ROW_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__COL__9         << MC9328MXL_SDRAM::SDCTL__COL_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__IAM__disable   << MC9328MXL_SDRAM::SDCTL__IAM_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__DSIZ__32bit    << MC9328MXL_SDRAM::SDCTL__DSIZ_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__SCL__3_clocks  << MC9328MXL_SDRAM::SDCTL__SCL_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__SRP__3clks     << MC9328MXL_SDRAM::SDCTL__SRP_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__SRCD__3_clocks << MC9328MXL_SDRAM::SDCTL__SRCD_shift;
    val_base |= MC9328MXL_SDRAM::SDCTL__SRC__8_clocks  << MC9328MXL_SDRAM::SDCTL__SRC_shift;

    //
    // Set precharge all command
    //
    {
        UINT32 val = val_base;

        val |= MC9328MXL_SDRAM::SDCTL__SMODE__Precharge_Command << MC9328MXL_SDRAM::SDCTL__SMODE_shift;

        MC9328MXL::SDRAM().SDCTL[0] = val;
    }

    // Issue precharge all command
    tmp = *(volatile UINT32*)(c_Bootstrap_SDRAM_Begin + 0x200000); // A10 bit set to high for precharge ALL


    //
    // Set AutoRefresh Command
    //
    {
        UINT32 val = val_base;

        val |= MC9328MXL_SDRAM::SDCTL__SMODE__Auto_Refresh_Command << MC9328MXL_SDRAM::SDCTL__SMODE_shift;

        MC9328MXL::SDRAM().SDCTL[0] = val;
    }

    // Issue 8 AutoRefresh Command
    for(int i=8; --i >= 0; )
    {
        tmp = *(volatile UINT32*)(c_Bootstrap_SDRAM_Begin + 0x200000);
    }


    //
    // Set Mode Register
    //
    {
        UINT32 val = val_base;

        val |= MC9328MXL_SDRAM::SDCTL__SMODE__Set_Mode_Register_Command << MC9328MXL_SDRAM::SDCTL__SMODE_shift;

        MC9328MXL::SDRAM().SDCTL[0] = val;
    }

    // Issue Mode Register Command
    tmp = *(volatile UINT32*)(c_Bootstrap_SDRAM_Begin + 0x119800); // WB = 1, CAS Latency = 011, Burst Length = 011


    //
    // Set SMODE to Normal Read/Write and Enable SREFR to refresh 2 rows every 32kHz clock edge
    //
    {
        UINT32 val = val_base;

        val |= MC9328MXL_SDRAM::SDCTL__SMODE__Normal_ReadWrite << MC9328MXL_SDRAM::SDCTL__SMODE_shift;
        val |= MC9328MXL_SDRAM::SDCTL__SREFR__2Row_per_clock   << MC9328MXL_SDRAM::SDCTL__SREFR_shift;

        MC9328MXL::SDRAM().SDCTL[0] = val;
    }
}

#endif

void __section(SectionForBootstrapOperations) BootstrapCode_MMU()
{
    //
    // Fill Translation table with faults.
    //

    ARM9_MMU::InitializeL1(c_Bootstrap_BaseOfTTBs);

    //--//

    // Direct map for the control registers (0x200000 ~ 0x226FFF)
    ARM9_MMU::GenerateL1_Sections(
        c_Bootstrap_BaseOfTTBs,  // base of TTBs
        c_Bootstrap_Registers,   // mapped address
        c_Bootstrap_Registers,   // physical address
        ARM9_MMU::c_MMU_L1_size,  // length to be mapped
        ARM9_MMU::c_AP__Manager, // AP
        0,                       // Domain
        FALSE,                   // Cacheable
        FALSE);                  // Buffered
    


    // The last 1MB of RAM (0x9F00000 ~ 0x9FFFFFF) is double-mapped to (0x0 ~ 0xFFFFF)
    // Make it cache write-through (when cache hit on write, it also issue an update to the memory in the write buffer (it doesn't block execution)
    // This is so that components such as DMA and LCDC which are unaware of the cache will still have consistency without flushing out cache at
    // every transaction.
    ARM9_MMU::GenerateL1_Sections(
        c_Bootstrap_BaseOfTTBs,             // base of TTBs
        0x00000000,                         // mapped address
        c_Bootstrap_SDRAM_End - 0x100000,   // physical address
        0x100000,                           // length to be mapped
        ARM9_MMU::c_AP__Manager,            // AP
        0,                                  // Domain
        TRUE,                               // Cacheable
        FALSE);                             // Buffered


    //
    // Direct map RAM.
    //
    ARM9_MMU::GenerateL1_Sections(
        c_Bootstrap_BaseOfTTBs,                          // base of TTBs 
        c_Bootstrap_SDRAM_Begin,                         // mapped address
        c_Bootstrap_SDRAM_Begin,                         // physical address
        c_Bootstrap_SDRAM_End - c_Bootstrap_SDRAM_Begin, // length to be mapped
        ARM9_MMU::c_AP__Manager,                         // AP
        0,                                               // Domain
        TRUE,                                            // Cacheable 
        FALSE );                                         // Buffered 

    //
    // Second mapping of RAM.
    //
    
    ARM9_MMU::GenerateL1_Sections(
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_Bootstrap_SDRAM_Begin | MC9328MXL::c_UncachableMask,  // mapped address
        c_Bootstrap_SDRAM_Begin,                                // physical address
        c_Bootstrap_SDRAM_End - c_Bootstrap_SDRAM_Begin,        // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE );                                                // Buffered

    //
    // Direct map FLASH.
    //
    ARM9_MMU::GenerateL1_Sections(
        c_Bootstrap_BaseOfTTBs,                         // base of TTBs
        c_Bootstrap_FLASH_Begin,                        // mapped address
        c_Bootstrap_FLASH_Begin,                        // physical address
        c_Bootstrap_FLASH_End - c_Bootstrap_FLASH_Begin,// length to be mapped 
        ARM9_MMU::c_AP__Manager,                        // AP
        0,                                              // Domain
        TRUE,                                           // Cacheable
        FALSE );                                        // Buffered

    //
    // Second mapping of FLASH, this time uncachable.
    //
    ARM9_MMU::GenerateL1_Sections(
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_Bootstrap_FLASH_Begin | MC9328MXL::c_UncachableMask,  // mapped address
        c_Bootstrap_FLASH_Begin,                                // physical address
        c_Bootstrap_FLASH_End - c_Bootstrap_FLASH_Begin,        // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE );                                                // Buffered

#if 0
    BootstrapCode_DumpTTBs();
#endif

    CPU_FlushCaches();

    CPU_EnableMMU( c_Bootstrap_BaseOfTTBs );
}

void __section(SectionForBootstrapOperations) BootstrapCode_FLASH()
{
#if (defined(PLATFORM_ARM_AUXD) && (HARDWARE_BOARD_TYPE > HARDWARE_BOARD_CSB536MS_REV_V1_0)) || \
    !defined(PLATFORM_ARM_AUXD) \
     
    static const UINT32 BOOTSTRAP_FLASH_WIDTH      = MC9328MXL_EIM::DEVICE::CS_L__DSZ__L16;

#else

    static const UINT32 BOOTSTRAP_FLASH_WIDTH      = MC9328MXL_EIM::DEVICE::CS_L__DSZ__U16;

#endif

#if (HARDWARE_BOARD_TYPE >= HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2)
    static const UINT32 BOOTSTRAP_FLASH_WAITSTATES = 7;     // For 75 ns parts and 96 MHz
#else
    static const UINT32 BOOTSTRAP_FLASH_WAITSTATES = 10;    // For 110 ns parts and 96 MHz.
#endif

    static const UINT32 BOOTSTRAP_FLASH_DEADSTATES = 0;     // Part requires no cycles to tristate
    MC9328MXL_EIM::DEVICE& dev = MC9328MXL::EIM().Device[0];

    // set wait_state for FLASH, only change the wait state
    dev.CS_U = MC9328MXL_EIM::DEVICE::CS_U__WSC__set( BOOTSTRAP_FLASH_WAITSTATES ) | BOOTSTRAP_FLASH_DEADSTATES;
    dev.CS_L = MC9328MXL_EIM::DEVICE::CS_L__DSZ__set( BOOTSTRAP_FLASH_WIDTH      ) | MC9328MXL_EIM::DEVICE::CS_L__EBC | MC9328MXL_EIM::DEVICE::CS_L__CSEN;
}

extern "C"
{
void __section(SectionForBootstrapOperations) BootstrapCode()
{
    // SDRAM and clocks initialized in PreStackInit
    
    //
    // ***** Initialize MMU *****
    //
    BootstrapCode_MMU();

    //
    // ***** Initialize FLASH *****
    //
    BootstrapCode_FLASH();


    PrepareImageRegions();

    CPU_EnableCaches();
}
}


#pragma arm section code

///////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_MC9328MXL_Driver"
#endif

MC9328MXL_Driver g_MC9328MXL_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

