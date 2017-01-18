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
#include "..\LPC22XX.h"

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC22XX_Driver"
#endif

LPC22XX_Driver g_LPC22XX_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif


BOOL CPU_Initialize()
{
    return LPC22XX_Driver::Initialize();
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

void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents )
{
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            LPC22XX_Driver::Hibernate();
            break;
        case SLEEP_LEVEL__OFF:
            LPC22XX_Driver::Shutdown();
            break;
        case SLEEP_LEVEL__SLEEP:
        default:
            LPC22XX_Driver::Sleep();
            break;
    }
}

void CPU_Halt()
{
    LPC22XX_Driver::Halt();
}

void CPU_Reset()
{
    LPC22XX_Driver::Reset();
}

BOOL CPU_IsSoftRebootSupported ()
{
    return TRUE;
}


//--//


BOOL LPC22XX_Driver::Initialize()
{
//    // Remap Interrupt vectors to internal RAM
//    // Vector table is copied by PrepareImageRegions() (HAL\ARM\Configuration\tinyhal.cpp)

    Initialize_SyncOn();

    CPU_INTC_Initialize();
    // Debug Uart channel is initialized by HAL Init
    // I2C is initialized by PAL I2C driver
    // TODO TODO TODO complete implementation
    return TRUE;
}

void LPC22XX_Driver::Sleep()
{
    // TODO TODO TODO complete implementation to sleep using co-processor
    while(!LPC22XX::VIC().IsInterruptPending());
}

void LPC22XX_Driver::Halt()
{
    GLOBAL_LOCK(irq);

    // disable all interrupt source in controller that might wake us from our slumber
    CPU_INTC_Initialize();

    Sleep();
}

void LPC22XX_Driver::Reset()
{
    LPC22XX_WATCHDOG_Driver::ResetCpu();
}

void LPC22XX_Driver::Shutdown()
{
    GLOBAL_LOCK(irq);

    while(true);
}

void LPC22XX_Driver::Hibernate()
{
    // TODO TODO TODO complete implementation
    Sleep();
}

void HAL_AssertEx()
{
    // cause an abort and let the abort handler take over
    *((char*)0xFFFFFFFF) = 'a';
}


