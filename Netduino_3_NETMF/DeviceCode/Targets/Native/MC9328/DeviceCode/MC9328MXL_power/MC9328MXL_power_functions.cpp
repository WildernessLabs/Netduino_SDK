////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>


BOOL CPU_Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    return MC9328MXL_Driver::Initialize();
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
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    
    switch(level)
    {
        case SLEEP_LEVEL__DEEP_SLEEP:
            MC9328MXL_Driver::Hibernate();
            break;
        case SLEEP_LEVEL__OFF:
            MC9328MXL_Driver::Shutdown();
            break;            
        case SLEEP_LEVEL__SLEEP:
        default:
            MC9328MXL_Driver::Pause();
            break;
    }
}

void CPU_Halt()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    MC9328MXL_Driver::Halt();
}

void CPU_Shutdown()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    MC9328MXL_Driver::Shutdown();
}

void CPU_Hibernate()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    MC9328MXL_Driver::Hibernate();
}

void CPU_Reset()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    MC9328MXL_Driver::Reset();
}

BOOL CPU_IsSoftRebootSupported ()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    return PLATFORM_SUPPORTS_SOFT_REBOOT;
}

//--//

