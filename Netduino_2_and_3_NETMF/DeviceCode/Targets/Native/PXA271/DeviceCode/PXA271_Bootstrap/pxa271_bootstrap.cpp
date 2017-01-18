////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include <Drivers\Blockstorage\flash\I28F_16\I28F_16.h>

///////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

// Define this to test the SDRAM addressing, size, and data retention
#undef SDRAM_TEST

// the arm3.0 compiler optimizes out much of the boot strap code which causes
// the device not to boot for RTM builds (optimization level 3), adding this pragma 
// assures that the compiler will use the proper optimization level for this code
#if !defined(DEBUG)
#pragma O2
#endif

///////////////////////////////////////////////////////////////////////////////

// DUMMY for macro
#if !defined(SRAM2_MEMORY_Base)
#define SRAM2_MEMORY_Base               0
#endif

#if !defined(SRAM2_MEMORY_Size)
#define SRAM2_MEMORY_Size               0
#endif


#pragma arm section code = "SectionForFlashOperations"


static const UINT32  c_Bootstrap_Register_Begin =           0x40000000;
static const UINT32  c_Bootstrap_Register_End   =           SRAM2_MEMORY_Base;
static const UINT32  c_Bootstrap_SRAM_Begin     =           SRAM2_MEMORY_Base;
static const UINT32  c_Bootstrap_SRAM_End       =           SRAM2_MEMORY_Base + SRAM2_MEMORY_Size;
static const UINT32  c_Bootstrap_SDRAM_Begin    =           SRAM1_MEMORY_Base;
static const UINT32  c_Bootstrap_SDRAM_End      =           SRAM1_MEMORY_Base + SRAM1_MEMORY_Size;
static const UINT32  c_Bootstrap_FLASH_Begin    =           FLASH_MEMORY_Base;
static const UINT32  c_Bootstrap_FLASH_End      =           FLASH_MEMORY_Base + FLASH_MEMORY_Size;
static UINT32* const c_Bootstrap_BaseOfTTBs     = (UINT32*)(c_Bootstrap_SRAM_End - ARM9_MMU::c_TTB_size);


FORCEINLINE void __section(SectionForFlashOperations) Bootstrap_Delay( INT32 count )
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

// under 416 MHz, ~ 10us
FORCEINLINE void __section(SectionForFlashOperations) Bootstrap_Delay()
{
    Bootstrap_Delay( 2000 );
}


#pragma arm section code = "SectionForBootstrapInRAM"

#if 0
// Historic code placed here as reference - before Flash and SDRAM init combined...
void __section(SectionForBootstrapOperations) BootstrapCode_FLASH()
{
    UINT32 val;
    PXA271_MEMORY &mem = PXA271::MEMORY();
    volatile UINT16 *Flash_Reg = (UINT16*)CPU_GetUncachableAddress((UINT32*)FLASH_MEMORY_Base);
    UINT16  val_16;

    // Flash at CS0, no large memory, no byte addressing
    mem.SA1110 = PXA271_MEMORY::SA1110__SXSTACK_nCS0 << PXA271_MEMORY::SA1110__SXSTACK_SHIFT;


    // 16 bit bus width
    val = mem.MSC[0] & (~ PXA271_MEMORY::MSC__RRR_024_MASK) & (~ PXA271_MEMORY::MSC__RDN_024_MASK) &  (~ PXA271_MEMORY::MSC__RDF_024_MASK)& (~ PXA271_MEMORY::MSC__RT_024_MASK);
    
    mem.MSC[0]=  val | PXA271_MEMORY::MSC__RT_024_4_ROM| PXA271_MEMORY::MSC__RBW_024| PXA271_MEMORY::MSC__RBUFF_024 | ( 2 << PXA271_MEMORY::MSC__RRR_024_SHIFT) | ( 7 << PXA271_MEMORY::MSC__RDN_024_SHIFT) | ( 12 << PXA271_MEMORY::MSC__RDF_024_SHIFT);

    // Turn off the SDCLK 0 free running, use 52MHz=CLK_MEM/4, NOTE: do not change other bit
    // CAS latency 
    mem.SXCNFG = PXA271_MEMORY::SXCNFG__RESERVE_BITS | ( 0x3 << PXA271_MEMORY::SXCNFG__SXTP0_SHIFT )| (0x4 << PXA271_MEMORY::SXCNFG__SXCL0_SHIFT ) | PXA271_MEMORY::SXCNFG__SXEN0;

    mem.MECER  =0; // no PC card


// Initialize Flash here instead of flash driver, as it must be done from RAM
    val_16            = PXA271_16_Driver::FLASH_CONFIG_RESERVED;
    val_16           |= PXA271_16_Driver::FLASH_CONFIG_SYNC | PXA271_16_Driver::FLASH_CONFIG_CLK_HI_EDGE;
    val_16           |= PXA271_16_Driver::FLASH_CONFIG_DATA_HOLD_1 | (4 << PXA271_16_Driver::FLASH_CONFIG_LAT_SHIFT);
    val_16           |= PXA271_16_Driver::FLASH_CONFIG_BURST_16 | PXA271_16_Driver::FLASH_CONFIG_BURST_WRAP;
    Flash_Reg[val_16] = PXA271_16_Driver::CONFIG_SETUP;
    Flash_Reg[val_16] = PXA271_16_Driver::CONFIG_WRITE;
    Flash_Reg[0]      = PXA271_16_Driver::ENTER_READ_ARRAY_MODE;
    val_16            = Flash_Reg[0];
    
    val =( mem.MDREFR & (~ PXA271_MEMORY::MDREFR__K0FREE)) | PXA271_MEMORY::MDREFR__K0DB4 | PXA271_MEMORY::MDREFR__K0RUN;

    mem.MDREFR =  val;
}
#endif

// SDRAM and synchronous Flash must be brought up together via code
// executing in static RAM
void __section(SectionForBootstrapOperations) BootstrapCode_SynchronousMemory()
{
    UINT32 val;
    PXA271_MEMORY &mem = PXA271::MEMORY();
    volatile UINT16 *Flash_Reg = (UINT16*)CPU_GetUncachableAddress((UINT32*)FLASH_MEMORY_Base);
    UINT16  val_16;

    if(mem.MDCMFG & PXA271_MEMORY::MDCMFG__DE0)     // If synchronous memory has already been set up
        return;                                     // Do not attempt to set it up again

    // Flash at CS0, no large memory, no byte addressing
    mem.SA1110 = PXA271_MEMORY::SA1110__SXSTACK_nCS0 << PXA271_MEMORY::SA1110__SXSTACK_SHIFT;

    // 16 bit bus width
    val = mem.MSC[0] & (~ PXA271_MEMORY::MSC__RRR_024_MASK) & (~ PXA271_MEMORY::MSC__RDN_024_MASK) &  (~ PXA271_MEMORY::MSC__RDF_024_MASK)
        & (~ PXA271_MEMORY::MSC__RT_024_MASK);
    
    mem.MSC[0] =  val | PXA271_MEMORY::MSC__RT_024_4_ROM| PXA271_MEMORY::MSC__RBW_024| PXA271_MEMORY::MSC__RBUFF_024
        | ( 2 << PXA271_MEMORY::MSC__RRR_024_SHIFT) | ( 7 << PXA271_MEMORY::MSC__RDN_024_SHIFT) | ( 12 << PXA271_MEMORY::MSC__RDF_024_SHIFT);

    mem.MECER  = 0;     // no PC card

    val = (PXA271_MEMORY::MDCMFG__SETALWAYS0 | PXA271_MEMORY::MDCMFG__SETALWAYS2);

    // normal SDRAM memory map
    val &= (~ PXA271_MEMORY::MDCMFG__MDENX);
    // select Stack1:0 = 0b01 for MA(24:23,13,1)
    val &= (~ PXA271_MEMORY::MDCMFG__STACK1);
    val |= PXA271_MEMORY::MDCMFG__STACK0;
    // use normal addressing mode, 3CLK cycle, tRP=3, CL=3,tRAS=7,tRC=11
    val |= PXA271_MEMORY::MDCMFG__DTC0_03<< PXA271_MEMORY::MDCMFG__DTC0_SHIFT;
    val |= PXA271_MEMORY::MDCMFG__DNB0;    // 4 internal bank
    val |= PXA271_MEMORY::MDCMFG__DRAC0_ROWADDR_13 << PXA271_MEMORY::MDCMFG__DRAC0_SHIFT;   // 13 row address
    val |= PXA271_MEMORY::MDCMFG__DCAC0_COLADDR_9  << PXA271_MEMORY::MDCMFG__DCAC0_SHIFT ;  // 9 col address ( DCAC0X =0)
    val |= PXA271_MEMORY::MDCMFG__DWID0;   // 16 bit bus
    // set SDRAM configuration
    mem.MDCMFG = val;

// Initialize Flash here instead of flash driver, as it must be done from RAM
    val_16            = I28F_16_BS_Driver::FLASH_CONFIG_RESERVED;
    val_16           |= I28F_16_BS_Driver::FLASH_CONFIG_SYNC | I28F_16_BS_Driver::FLASH_CONFIG_CLK_HI_EDGE;
    val_16           |= I28F_16_BS_Driver::FLASH_CONFIG_DATA_HOLD_1 | (4 << I28F_16_BS_Driver::FLASH_CONFIG_LAT_SHIFT);
    val_16           |= I28F_16_BS_Driver::FLASH_CONFIG_BURST_16 | I28F_16_BS_Driver::FLASH_CONFIG_BURST_WRAP;
    Flash_Reg[val_16] = I28F_16_BS_Driver::CONFIG_SETUP;
    Flash_Reg[val_16] = I28F_16_BS_Driver::CONFIG_WRITE;
    Flash_Reg[0]      = I28F_16_BS_Driver::ENTER_READ_ARRAY_MODE;
    val_16            = Flash_Reg[0];
    
    // Enable partition 0 clock for synchronous memory
    val = (mem.MDREFR & (~ PXA271_MEMORY::MDREFR__DRI_MASK)) | 25;      // 25 = (refresh interval in SDRAM clocks)/32 = ((64mS/8192 rows) * 104 MHz)/32 (rounded down) 
    val &= ~(PXA271_MEMORY::MDREFR__K0FREE | PXA271_MEMORY::MDREFR__K2FREE | PXA271_MEMORY::MDREFR__K1FREE | PXA271_MEMORY::MDREFR__APD);
    mem.MDREFR = val | PXA271_MEMORY::MDREFR__K0DB4 | PXA271_MEMORY::MDREFR__SLFRSH | PXA271_MEMORY::MDREFR__K0RUN;

    // Turn off the SDCLK 0 free running, use 52MHz=CLK_MEM/4, NOTE: do not change other bit
    // CAS latency 
    mem.SXCNFG = PXA271_MEMORY::SXCNFG__RESERVE_BITS | (0x3 << PXA271_MEMORY::SXCNFG__SXTP0_SHIFT)
        | (0x4 << PXA271_MEMORY::SXCNFG__SXCL0_SHIFT ) | PXA271_MEMORY::SXCNFG__SXEN0;

    mem.MDREFR = mem.MDREFR | PXA271_MEMORY::MDREFR__K1DB2 | PXA271_MEMORY::MDREFR__K1RUN;
    mem.MDREFR = mem.MDREFR & ~PXA271_MEMORY::MDREFR__SLFRSH;
    mem.MDREFR = mem.MDREFR | PXA271_MEMORY::MDREFR__E1PIN;
    
    // dummy read for entering NOP state, A20=0
    (void)*(volatile UINT32*)c_Bootstrap_SDRAM_Begin;

    // delay for 200usec
    for (int i=20; --i>=0;)
    {
        Bootstrap_Delay();      // let it stabilize
    }

    mem.MDCMFG = mem.MDCMFG | PXA271_MEMORY::MDCMFG__DE0;       // Enable SDRAM

    // Issue precharge all command
    (void)*(volatile UINT32*)(c_Bootstrap_SDRAM_Begin + 0x100000); // MA10 ( or  A20) bit set to high for precharge ALL

    // Issue 2 AutoRefresh Command
    for(int i=2; --i >= 0; )
    {
        (void)*(volatile UINT32*)(c_Bootstrap_SDRAM_Begin);
    }

    //
    // Set Mode Register
    mem.MDMRS = mem.MDMRS & (~ PXA271_MEMORY::MDMRS__MDMRS0_MASK);

    //
    // Issue Extended Mode Register Command, enable Low power, Amibent temp = 70deg cel, Partial array self refresh = four banks.
    mem.MDMRSLP = PXA271_MEMORY::MDMRSLP__MDLPEN0; 

    //
    // Set Mode Register, back to NOP
    mem.MDMRS = mem.MDMRS & (~ PXA271_MEMORY::MDMRS__MDMRS0_MASK);

    // Turn on Auto Power Down
    mem.MDREFR = mem.MDREFR | PXA271_MEMORY::MDREFR__APD;

}

#ifdef SDRAM_TEST
// SDRAM test - writes a non-repeating address dependent pattern to SDRAM
// Then reads it back as a test
// Requires about 22 seconds to execute
UINT32 __section(SectionForBootstrapOperations) SDRAM_Test(UINT32 Start, UINT32 nBytes)
{
    UINT32 *Address = (UINT32*)Start;
    UINT32 Data;
    UINT32 counter;
    volatile UINT32 * const LED_Out   = (volatile UINT32 *)0x40E0010C;
    volatile UINT32 * const LED_Off   = (volatile UINT32 *)0x40E00118;
    volatile UINT32 * const LED_On    = (volatile UINT32 *)0x40E00124;
    const UINT32            All_LED   =                    0x380;
    const UINT32            Red_LED   =                    0x080;
    const UINT32            Green_LED =                    0x100;
    const UINT32            Blue_LED  =                    0x200;

    *LED_Out  = All_LED;        // Insure that these GPIO are outputs
    *LED_Off  = All_LED;        // Turn all LEDs off for memory fill phase
    for(counter = nBytes/4; counter; counter--)
    {
        Data = ((UINT32)Address ^ ((UINT32)Address >> 8) ^ ((UINT32)Address >> 16) ^ ((UINT32)Address >> 24)) & 0xFF;
        Data = Data | ((Data ^ 1) << 8) | ((Data ^ 2) << 16) | ((Data ^ 3) << 24);
        *Address++ = Data;
    }

    // Wait about 1 second for memory to fade (if refresh is improper)
    Bootstrap_Delay(SYSTEM_CYCLE_CLOCK_HZ);

    // Start SDRAM testing
    *LED_On  = Blue_LED;                // Turn on Blue LED for readback phase
    Address = (UINT32*)Start;
    for(counter = nBytes/4; counter; counter--)
    {
        Data = ((UINT32)Address ^ ((UINT32)Address >> 8) ^ ((UINT32)Address >> 16) ^ ((UINT32)Address >> 24)) & 0xFF;
        Data = Data | ((Data ^ 1) << 8) | ((Data ^ 2) << 16) | ((Data ^ 3) << 24);
        if(*Address++ != Data)
        {
            *LED_Off = All_LED;
            *LED_On  = Red_LED;                         // Turn on Red LED
            Bootstrap_Delay(SYSTEM_CYCLE_CLOCK_HZ);     // Make sure error LED is visible for 1 second
            return((UINT32)(--Address));                // Return the failed address
        }
    }
    return (UINT32)Address;
}
#endif

#pragma arm section code = "SectionForBootstrapOperations"

void __section(SectionForBootstrapOperations) PXA271::Debug_EmitHEX( UINT32 value )
{
    for(int i=7; i>=0; i--)
    {
        UINT32 digit = (value >> (4*i)) & 0xF;

        Debug_Emit( digit < 10 ? digit + '0' : digit + 'A' - 10 );
    }
}

void __section(SectionForBootstrapOperations) PXA271::Debug_EmitHEX2( UINT32 value )
{
    for(int i=7; i>=0; i--)
    {
        UINT32 digit = (value >> (4*i)) & 0xF;

        Debug_Emit2( digit < 10 ? digit + '0' : digit + 'A' - 10 );
    }
}

//--//

void __section(SectionForBootstrapOperations) BootstrapCode_DumpTTBs()
{
    UINT32  address = 0;
    UINT32* dst     = ARM9_MMU::GetL1Entry( c_Bootstrap_BaseOfTTBs, address );

    do
    {
        PXA271::Debug_EmitHEX( address );
        PXA271::Debug_Emit   ( ' '     );

        PXA271::Debug_EmitHEX( (UINT32)dst );
        PXA271::Debug_Emit   ( ' '         );

        PXA271::Debug_EmitHEX( *dst );
        PXA271::Debug_Emit   ( '\r' );
        PXA271::Debug_Emit   ( '\n' );

        dst++;
        address += ARM9_MMU::c_MMU_L1_size;
    } while(address < c_Bootstrap_FLASH_End);
}

// CPWAIT() for the PXA271 processor is implemented here
// because it is needed before RO area is initialized
void __section(SectionForBootstrapOperations) BOOT_CPWAIT()
{
    UINT32 Rx;

#ifdef __GNUC__
	asm("MRC	p15, 0, %0, c2, c0, 0"  : "=r" (Rx));
	asm("NOP");
#else
    __asm
    {
        mrc     p15, 0, Rx, c2, c0, 0;
        nop;
    }
#endif
}

////////////////////////////////////////////////////////////////////////


void __section(SectionForBootstrapOperations) BootstrapCode_Clocks()
{

    UINT32 reg;
    //
    // Ensure that FLASH access time is at its reset value.
    //
    
    
    //
    // Change drive strength to 312Mhz.
    //

    PXA271_CLOCK_MANAGER &clk = PXA271::CLKMNGR();

    // turn off all the clock,except internal memory, let each unit to turn on at its code
    clk.CKEN = PXA271_CLOCK_MANAGER::CKEN__INTERNAL_MEMORY_CLK |PXA271_CLOCK_MANAGER::CKEN__MEMORY_CNTRL | PXA271_CLOCK_MANAGER::CKEN__PWR_MNG_I2C_CLK;  
    
//    clk.OSCC |= PXA271_CLOCK_MANAGER::OSCC__OON;             // turn on 32.768KHz, once on, cannot off unless power off
//    while ((clk.OSCC & PXA271_CLOCK_MANAGER::OSCC__OOK)==0); // wait until 32KHz stablize

    clk.OSCC |= PXA271_CLOCK_MANAGER::OSCC__TOUT_EN;         // enable TOUT for debugging.

    // run freq = 208MHz, turbo = 416MHz, core PLLs must on, CLK_MEM = 208MHz
    clk.CCCR =  PXA271_CLOCK_MANAGER::CCCR__A | (4 << PXA271_CLOCK_MANAGER::CCCR__2N_SHIFT ) | (16 <<  PXA271_CLOCK_MANAGER::CCCR__L_SHIFT );

    reg = 0x03;     // Start frequency change + enable Turbo mode (416 MHz clock)

#ifdef __GNUC__
	asm("MCR	p14, 0, %0, c6, c0, 0" :: "r" (reg)); 
#else
	__asm
    {
        mcr p14, 0, reg, c6, c0, 0
    }
#endif

    CPU_CPWAIT();
    // wait until clk stable
    while ((clk.CCSR & PXA271_CLOCK_MANAGER::CCSR__CPLCK) == 0);
    while ((clk.CCSR & PXA271_CLOCK_MANAGER::CCSR__PPLCK) == 0);
         

//    *********

}

#if 0
// Historic code left as reference - but doesn't seem to work at certain addresses...
void __section(SectionForBootstrapOperations) BootstrapCode_SDRAM()
{
    PXA271_MEMORY &mem = PXA271::MEMORY();

    // SDRAM 32Mx8, IAM = 0, , CL2, HCLK = 104MHz
    
    UINT32 val_base = 0;

    // set up the SDRAM configure
    val_base = (PXA271_MEMORY::MDCMFG__SETALWAYS0 | PXA271_MEMORY::MDCMFG__SETALWAYS2);

    // normal SDRAM memory map
    val_base &= (~ PXA271_MEMORY::MDCMFG__MDENX);
    // select Stack1:0 = 0b01 for MA(24:23,13,1)
    val_base &= (~ PXA271_MEMORY::MDCMFG__STACK1);
    val_base |= PXA271_MEMORY::MDCMFG__STACK0;
    // use normal addressing mode, 3CLK cycle, tRP=3, CL=3,tRAS=7,tRC=11
    val_base |= PXA271_MEMORY::MDCMFG__DTC0_03<< PXA271_MEMORY::MDCMFG__DTC0_SHIFT;
    val_base |= PXA271_MEMORY::MDCMFG__DNB0;    // 4 internal bank
    val_base |= PXA271_MEMORY::MDCMFG__DRAC0_ROWADDR_13 << PXA271_MEMORY::MDCMFG__DRAC0_SHIFT; // 13 row address
    val_base |= PXA271_MEMORY::MDCMFG__DCAC0_COLADDR_9 <<PXA271_MEMORY::MDCMFG__DCAC0_SHIFT ;  // 9 col address ( DCAC0X =0)
    val_base |= PXA271_MEMORY::MDCMFG__DWID0;   // 16bit bus
    val_base |= PXA271_MEMORY::MDCMFG__DE0;     // enable partition 0
    // set SDRAM configuration
    mem.MDCMFG = val_base;

    // set auto refresh count
    mem.MDREFR = mem.MDREFR & (~PXA271_MEMORY::MDREFR__DRI_MASK) | (( 15*104-31)/32);
        

    // SDRAM Initialization 
    // turn off the SDRAM clocks free running, turn on SDCLK1 (=CLK_MEM/2) => Power down state
    mem.MDREFR = mem.MDREFR & (~ PXA271_MEMORY::MDREFR__K2FREE ) & (~ PXA271_MEMORY::MDREFR__K1FREE) & (~ PXA271_MEMORY::MDREFR__SLFRSH) 
                    | PXA271_MEMORY::MDREFR__K1DB2 | PXA271_MEMORY::MDREFR__K1RUN | PXA271_MEMORY::MDREFR__APD;

    // turn on SDCKEN =>exit Power down 
    mem.MDREFR = mem.MDREFR | PXA271_MEMORY::MDREFR__E1PIN; 
    // dummy read for entering NOP state, A20=0
    (void)*(volatile UINT32*)c_Bootstrap_SDRAM_Begin;

    // delay for 200usec
    for (int i=20; --i>=0;)
    {
        Bootstrap_Delay();      // let it stabilize
    }
    
    // Issue precharge all command
    (void)*(volatile UINT32*)(c_Bootstrap_SDRAM_Begin + 0x100000); // MA10 ( or  A20) bit set to high for precharge ALL


    // Issue 2 AutoRefresh Command
    for(int i=2; --i >= 0; )
    {
        (void)*(volatile UINT32*)(c_Bootstrap_SDRAM_Begin );
    }


    //
    // Set Mode Register
    mem.MDMRS = mem.MDMRS & (~ PXA271_MEMORY::MDMRS__MDMRS0_MASK);
    
    
    //
    // Issue Extended Mode Register Command, enable Low power, Amibent temp = 70deg cel, Partial array self refresh = four banks.
    mem.MDMRSLP = PXA271_MEMORY::MDMRSLP__MDLPEN0; 

    //
    // Set Mode Register, back to NOP
    mem.MDMRS = mem.MDMRS & (~ PXA271_MEMORY::MDMRS__MDMRS0_MASK);
}
#endif

void __section(SectionForBootstrapOperations) BootstrapCode_MMU()
{
    //
    // Fill Translation table with faults.
    //
    ARM9_MMU::InitializeL1(c_Bootstrap_BaseOfTTBs);

    //--//
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                                 // base of TTBs
        c_Bootstrap_Register_Begin,                             // mapped address
        c_Bootstrap_Register_Begin,                             // physical address
        c_Bootstrap_Register_End - c_Bootstrap_Register_Begin,  // length to be mapped
        ARM9_MMU::c_AP__Manager,                                // AP
        0,                                                      // Domain
        FALSE,                                                  // Cacheable
        FALSE,                                                  // Buffered
        FALSE);                                                 // Extended

    // Make SRAM cache write-through (when cache hit on write, it also issue an update to the memory in the write buffer (it doesn't block execution)
    // This is so that components such as DMA and LCDC which are unaware of the cache will still have consistency without flushing out cache at
    // every transaction.
    // map SRAM - 0x5c00_0000- 0x5C040000 to 0x0 - 0x00040000 ( the first 256KB for ram)
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs, // base of TTBs
        0,                      // mapped address
        c_Bootstrap_SRAM_Begin, // physical address
        ARM9_MMU::c_MMU_L1_size,// length to be mapped
        ARM9_MMU::c_AP__Manager,// AP
        0,                      // Domain
        TRUE,                   // Cacheable
        FALSE,                  // Buffered
        FALSE);                 // Extended

    //
    // Direct mapping of SRAM (cachable)
    //
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs, // base of TTBs
        c_Bootstrap_SRAM_Begin, // mapped address
        c_Bootstrap_SRAM_Begin, // physical address
        ARM9_MMU::c_MMU_L1_size,// length to be mapped
        ARM9_MMU::c_AP__Manager,// AP
        0,                      // Domain
        TRUE,                   // Cacheable
        FALSE,                  // Buffered
        FALSE);                 // Extended

    //
    // Second mapping of SRAM (Uncachable address)
    //
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                             // base of TTBs 
        c_Bootstrap_SRAM_Begin | PXA271::c_UncachableMask,  // mapped address
        c_Bootstrap_SRAM_Begin,                             // physical address
        ARM9_MMU::c_MMU_L1_size,                            // length to be mapped
        ARM9_MMU::c_AP__Manager,                            // AP
        0,                                                  // Domain
        FALSE,                                              // Cacheable
        FALSE,                                              // Buffered
        FALSE);                                             // Extended


    //
    // Direct map SDRAM. - ADDRESS IS 0xA0000000 (which includes the cache mask bit) - !!!!!!! SO DO NOT MAP AN UNCACHABLE ADDRESS !!!!!!!
    //
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                         // base of TTBs
        c_Bootstrap_SDRAM_Begin,                        // mapped address
        c_Bootstrap_SDRAM_Begin,                        // physical address
        c_Bootstrap_SDRAM_End - c_Bootstrap_SDRAM_Begin,// length to be mapped 
        ARM9_MMU::c_AP__Manager,                        // AP
        0,                                              // Domain
        TRUE,                                           // Cacheable
        FALSE,                                          // Buffered
        FALSE);                                         // Extended

    //
    // direct map FLASH address (except first sector which is mapped away by SRAM mapping)
    // it should only contain the vector table.
    //
    {
        UINT32 address = c_Bootstrap_FLASH_Begin + ARM9_MMU::c_MMU_L1_size;
        
        ARM9_MMU::GenerateL1_Sections( 
            c_Bootstrap_BaseOfTTBs,         // base of TTBs
            address,                        // mapped address
            address,                        // physical address
            c_Bootstrap_FLASH_End - address,// length to be mapped
            ARM9_MMU::c_AP__Manager,        // AP
            0,                              // Domain
            TRUE,                           // Cacheable
            FALSE,                          // Buffered
            FALSE);                         // Extended
    }

    //
    // Second mapping of FLASH, this time uncachable.
    //
    ARM9_MMU::GenerateL1_Sections( 
        c_Bootstrap_BaseOfTTBs,                             // base of TTBs
        c_Bootstrap_FLASH_Begin | PXA271::c_UncachableMask, // mapped address
        c_Bootstrap_FLASH_Begin,                            // physical address
        c_Bootstrap_FLASH_End - c_Bootstrap_FLASH_Begin,    // length to be mapped
        ARM9_MMU::c_AP__Manager,                            // AP
        0,                                                  // Domain
        FALSE,                                              // Cacheable
        FALSE,                                              // Buffered
        FALSE);                                             // Extended

#if 0
    BootstrapCode_DumpTTBs();
#endif

    CPU_FlushCaches();

    CPU_EnableMMU( c_Bootstrap_BaseOfTTBs );
}

void __section(SectionForBootstrapOperations) BootstrapCode_ARM()
{
    UINT32 reg;

    //--//

    //
    // MMU                              : disabled
    // Alignment fault checking         : disabled.
    // Data Cache                       : disabled.
    // Instruction Cache                : disabled.
    // Write Buffer                     : enabled. It is always enabled.
    // Exception handlers               : 32bits
    // 26-bit address exception checking: disabled.
    // Late Abort Model selected.
    // Configured for little-endian memory system.
    //
    reg = 0x78;
#ifdef __GNUC__
	asm("MCR	p15, 0, %0, c1, c0, 0" :: "r" (reg));
#else
    __asm
    {
        mcr     p15, 0, reg, c1, c0, 0 // Control register.
    }
#endif
	
	CPU_CPWAIT();

    //--//

    reg = 0;

#ifdef __GNUC__
	asm("MCR	p15, 0, %0, c8, c7,  0" :: "r" (reg));
	asm("MCR	p15, 0, %0, c7, c7,  0" :: "r" (reg));
	asm("MCR	p15, 0, %0, c7, c10, 4" :: "r" (reg));
#else
    __asm
    {
        mcr     p15, 0, reg, c8, c7, 0  // Invalidate all TLBs.
        mcr     p15, 0, reg, c7, c7, 0  // Invalidate all caches & BTB.
        mcr     p15, 0, reg, c7, c10, 4 // Drain write buffers.
    }
#endif

    CPU_CPWAIT();    
    //--//

    reg = 0xFFFFFFFF;

#ifdef __GNUC__
	asm("MCR	p15, 0, %0, c3, c0, 0" :: "r" (reg));
#else
    __asm
    {
        mcr     p15, 0, reg, c3, c0, 0  // Domain access control, set Manager access for 16 domains.
    }
#endif

    CPU_CPWAIT();

}


void __section(SectionForBootstrapOperations) BootstrapCode()
{
    //PXA271::Debug_Emit( 'A' );
    //PXA271::Debug_NewLine();
    
    // Time to stablize the clock after hardware reset (at least 200 uS)
    Bootstrap_Delay(200 * (SYSTEM_CYCLE_CLOCK_HZ/ONE_MHZ) / 2);

    //
    // ***** Initialize processor *****
    //
    CPU_ARM9_BootstrapCode();

    //
    // ***** Initialize clocks *****
    //
    BootstrapCode_Clocks();

    //
    // ***** Initialize MMU *****
    //
    BootstrapCode_MMU();

    PrepareImageRegions();

    //
    // ***** Initialize FLASH *****
    //
    // This must be called AFTER SDRAM is initialized since SDRAM init mucks with synchronous clocks
    // that must not be mucked with after Flash is placed in synchronous mode
//    BootstrapCode_FLASH();      // This code must be in RO RAM, so MMU & ImageRegions must be initialized before this is called
#if !defined(TARGETLOCATION_RAM)
    BootstrapCode_SynchronousMemory();
#endif

#ifdef SDRAM_TEST
    SDRAM_Test(c_Bootstrap_SDRAM_Begin, SDRAM1_MEMORY_Size);
#endif

    CPU_EnableCaches();
}

#pragma arm section code

