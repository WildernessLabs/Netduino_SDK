////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Portions Copyright (c) Oberon microsystems, Inc.
//
//  *** CPU Power States ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\stm32.h"


BOOL CPU_Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    CPU_INTC_Initialize();
    return TRUE;
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

    PWR->CR &= ~(PWR_CR_LPDS | PWR_CR_PDDS); // reset deepsleep bits

    switch(level)
    {

        case SLEEP_LEVEL__DEEP_SLEEP: // stop
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            PWR->CR |= PWR_CR_CWUF | PWR_CR_LPDS; // low power deepsleep
            break;
        case SLEEP_LEVEL__OFF: // standby
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
            PWR->CR |= PWR_CR_CWUF | PWR_CR_PDDS; // power down deepsleep
            break;
        default: // sleep
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;       // no deepsleep
            PWR->CR |= PWR_CR_CWUF;
            break;
    }

    __WFI(); // sleep and wait for interrupt
}

void CPU_Halt()  // unrecoverable error
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    while(1);
}

void CPU_Reset()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    SCB->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos)  // unlock key
               | (1 << SCB_AIRCR_SYSRESETREQ_Pos); // reset request
     while(1); // wait for reset
}

BOOL CPU_IsSoftRebootSupported ()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    return FALSE;
}

#ifndef __GNUC__
__asm void HAL_AssertEx()
{
    BKPT     #0
L1  B        L1
    BX       lr
}
#else
void HAL_AssertEx()
{
	asm("BKPT #0");
	asm("L1: B L1");
	asm("BX lr");
}
#endif
//--//

