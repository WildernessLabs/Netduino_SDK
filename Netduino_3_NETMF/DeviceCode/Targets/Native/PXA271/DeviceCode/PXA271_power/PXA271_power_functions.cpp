////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>


//--//

BOOL CPU_Initialize()
{
    return PXA271_Driver::Initialize();
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

void CPU_Sleep( SLEEP_LEVEL level, UINT64 wakeEvents )
{
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            PXA271_Driver::Hibernate();
            break;
        case SLEEP_LEVEL__OFF:
            PXA271_Driver::Shutdown();
            break;
        case SLEEP_LEVEL__SLEEP:
        default:
            PXA271_Driver::Sleep();
            break;
    }
}

void CPU_Halt()
{
    PXA271_Driver::Halt();
}

void CPU_Reset()
{
    PXA271_Driver::Reset();
}

BOOL CPU_IsSoftRebootSupported ()
{
    return TRUE;
}

