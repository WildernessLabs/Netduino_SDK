////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <cores\arm\include\cpu.h>

//--//

void HAL_AssertEx()
{
    // cause an abort and let the abort handler take over
    volatile BOOL fContinue = FALSE;

    lcd_printf("\f***** ASSERT *****\r\n");
#if _DEBUG
    while(!fContinue);
#else
    char* ptr = NULL;
    *ptr = 'a';
#endif    
}


BOOL CPU_Initialize()
{
#if defined(AT91EMAC_SOCKETS_ENABLED)
    CPU_GPIO_EnableOutputPin(AT91_ERX0 , TRUE );
    CPU_GPIO_EnableOutputPin(AT91_ERX1 , TRUE );
    CPU_GPIO_EnableOutputPin(AT91_ERX2 , TRUE );
    CPU_GPIO_EnableOutputPin(AT91_ERX3 , TRUE );
    CPU_GPIO_EnableOutputPin(AT91_ECRS , TRUE );
    CPU_GPIO_EnableOutputPin(AT91_ECOL , FALSE);
    CPU_GPIO_EnableOutputPin(AT91_ERXDV, FALSE);
    CPU_GPIO_EnableOutputPin(AT91_ERXCK, TRUE );
    CPU_GPIO_EnableOutputPin(AT91_ERXER, FALSE);

    AT91_RSTC_EXTRST();
#endif

    // The SAM7 devices cannot reset the USB controller so we need to skip uninitialization
    // of the debug transport
#if defined(PLATFORM_ARM_SAM7_ANY)
    if(COM_IsUsb(HalSystemConfig.DebuggerPorts[ 0 ]))
    {
        g_fDoNotUninitializeDebuggerPort = true;
    }
#endif

    return AT91_SAM_Driver::Initialize();
}

//--//

#pragma arm section code

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

void CPU_Sleep(SLEEP_LEVEL level, UINT64 wakeEvents)
{
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            AT91_SAM_Driver::Hibernate();
            break;
        case SLEEP_LEVEL__OFF:
            AT91_SAM_Driver::Shutdown();
            break;
        case SLEEP_LEVEL__SLEEP:
        default:
            AT91_SAM_Driver::Sleep();
            break;
    }
}

void CPU_Halt()
{
    AT91_SAM_Driver::Halt();
}

void CPU_Reset()
{
    AT91_SAM_Driver::Reset();
}

BOOL CPU_IsSoftRebootSupported ()
{
#if defined(PLATFORM_ARM_SAM7X_ANY)
    return FALSE;
#else
    return TRUE;
#endif
}

