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

#include <tinyhal.h>
#include "..\LPC24XX.h"


void PrepareImageRegions_IRAM();
void  PrepareImageRegions_IRAM_TC();

extern void Initialize_SyncOn();


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_Driver"
#endif

LPC24XX_Driver g_LPC24XX_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif


// Copy vector table from External FLASH/RAM to LPC24XX internal RAM

extern UINT32 Load$$ER_IRAM$$Base;
extern UINT32 Image$$ER_IRAM$$Base;
extern UINT32 Image$$ER_IRAM$$Length;

extern UINT32 Load$$ER_IRAM_TC$$Base;
extern UINT32 Image$$ER_IRAM_TC$$Base;
extern UINT32 Image$$ER_IRAM_TC$$Length;



BOOL CPU_Initialize()
{
    return LPC24XX_Driver::Initialize();
}

void CPU_ChangePowerLevel(POWER_LEVEL level)
{
    switch(level)
    {
        case POWER_LEVEL__MID_POWER:
            break;

        case POWER_LEVEL__LOW_POWER:
            break;

        case POWER_LEVEL__HIGH_POWER:
        default:
            break;
    }
}


void CPU_DisplayProcessorInformation()
{
    LPC24XX_Driver::DisplayProcessorInformation();
}


void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents )
{
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            LPC24XX_Driver::Hibernate();
            break;
        case SLEEP_LEVEL__OFF:
            LPC24XX_Driver::Shutdown();
            break;
        case SLEEP_LEVEL__SLEEP:
        default:
         LPC24XX_Driver::Sleep();
            break;
    }
}




void CPU_Halt()
{
    LPC24XX_Driver::Halt();
}

BOOL CPU_PowerOnReset()
{
#if defined(HAL_REDUCESIZE)
    return LPC24XX_Driver::PowerOnReset_STUB(); // this is mainly for flashlite to preserve POR in to app, without acting on it
#else
    return LPC24XX_Driver::PowerOnReset();
#endif
}


void CPU_Reset()
{
    LPC24XX_Driver::Reset();
}

BOOL CPU_IsSoftRebootSupported ()
{
    return PLATFORM_SUPPORTS_SOFT_REBOOT;
}


//--//


BOOL LPC24XX_Driver::Initialize()
{
    // Copy Vector table to internal RAM
    PrepareImageRegions_IRAM(); // extra image copy for Internal RAM    

//    // Remap Interrupt vectors to internal RAM
//    // Vector table is copied by PrepareImageRegions() (HAL\ARM\Configuration\tinyhal.cpp)
    Initialize_SyncOn();

    // Copy TimeCritical code/data to internal SRAM
    PrepareImageRegions_IRAM_TC();
    
    CPU_INTC_Initialize();
    // Debug Uart channel is initialized by HAL Init
    // I2C is initialized by PAL I2C driver
    // TODO TODO TODO complete implementation
    return TRUE;
}

void LPC24XX_Driver::Pause()
{
    // TODO TODO TODO complete implementation
}

void LPC24XX_Driver::Sleep()
{
    //LPC22XX::SYSCON().INTWAKE = 
    //LPC22XX::SYSCON().PCON = 0x81;
    while(LPC24XX_VIC::c_MaxInterruptIndex == LPC24XX::VIC().NormalInterruptPending());
}

void LPC24XX_Driver::Halt()
{
    GLOBAL_LOCK(irq);

    // disable all interrupt source in controller that might wake us from our slumber
    CPU_INTC_Initialize();

    Pause();
}

void LPC24XX_Driver::Reset()
{
    LPC24XX_WATCHDOG_Driver::ResetCpu();
}

void LPC24XX_Driver::Shutdown()
{
    GLOBAL_LOCK(irq);

    while(true);
}

void LPC24XX_Driver::Hibernate()
{
    // TODO TODO TODO complete implementation
}

BOOL LPC24XX_Driver::PowerOnReset()
{
    return TRUE;
}

BOOL LPC24XX_Driver::PowerOnReset_STUB()
{
    return TRUE;
}

void LPC24XX_Driver::DisplayProcessorInformation()
{
    // TODO TODO TODO complete implementation
}

void HAL_AssertEx()
{
    // cause an abort and let the abort handler take over
    *((char*)0xFFFFFFFF) = 'a';
}

/////////////////////////////////////////////////////
// the following session of codes need to be no optimization, as the compiler will 
// assume the Image$xxx$Length is a non-zero value and skip the checking of "0"
// when optimization is enabled.

#pragma O0

#pragma arm section code = "SectionForBootstrapOperations"

// Dummy function force code generation for the following bootstrap functions
void __section(SectionForBootstrapOperations) BootstrapCode_Dummy()
{
}

void __section(SectionForBootstrapOperations) PrepareImageRegions_IRAM()
{

    UINT32  len = (UINT32 )&Image$$ER_IRAM$$Length; 
    volatile UINT32* src = (volatile UINT32*)&Load$$ER_IRAM$$Base; 
    volatile UINT32* dst = (volatile UINT32*)&Image$$ER_IRAM$$Base;

    if(dst == src) return;
    
    while(len )
    {
        *dst++ = *src++;

        len -= 4;
    }
        
}

void __section(SectionForBootstrapOperations) PrepareImageRegions_IRAM_TC()
{
    volatile UINT32* src = (volatile UINT32*)&Load$$ER_IRAM_TC$$Base; 
    volatile UINT32* dst = (volatile UINT32*)&Image$$ER_IRAM_TC$$Base;
    UINT32  len = (UINT32 )&Image$$ER_IRAM_TC$$Length; 

    if(dst == src)  return;

     while(len )
     {
         *dst++ = *src++;
 
         len -= 4;
     }

}


#pragma arm section code

