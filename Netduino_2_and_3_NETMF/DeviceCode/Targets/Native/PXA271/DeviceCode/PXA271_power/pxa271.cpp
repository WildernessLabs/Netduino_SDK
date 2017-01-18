////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
///////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//--//

//--//
#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PXA271_Driver"
#endif

PXA271_Driver g_PXA271_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//
BOOL PXA271_Driver::Initialize()
{
    CPU_INTC_Initialize();

    // TODO TODO TODO
    // start out with all peripheral clocks disabled (this resets most peripherals too)

/*
    // initialize EIM
    {
        PXA271_EIM& EIM = PXA271::EIM();

        // 6 devices, device 0 has different setup, default is used until Flash_Initialize is called to set parameters properly
        for(int i = 1; i < PXA271_EIM::c_Devices; i++)
        {
            UINT32 reg;

            reg = EIM.Device[i].CS_L;
            reg |= PXA271_EIM::DEVICE::CS_L__PA;               // Make sure CS disable state is correct (reset)
            reg &= ~PXA271_EIM::DEVICE::CS_L__NEEDED_CONTROL;  // 0 at CSEN  = disables device
            EIM.Device[i].CS_L = reg;
        }
    }
*/
    // TODO TODO TODO
    // enable buswatcher data write abort interrupts

    DMA_Initialize();

    return TRUE;
}

void PXA271_Driver::Pause()
{
    ASSERT_IRQ_MUST_BE_OFF();

    // Writing to this location changes the Run mode of the PXA271.  The modes are as follows:
    // 1:  Idle       - CPU clock stops while peripherals keep running.  Starts up again on interrupt
    // 1:  Deep Idle  - Same as Idle, but 13M mode enabled, too - peripheral timing is slowed
    // 2:  Standby    - CPU and peripheral clocks are off - except to RTC
    // 3:  Sleep      - Powers off all but RTC - everything except select blocks lose state
    // 7:  Deep sleep - Same as Sleep, but all blocks (except RTC) lose state

#if defined(CPU_SPIN_NOT_SLEEP)
    while(!PXA271::AITC().IsInterruptPending());    // use this for profiling runs.
#else
    {
        UINT32 reg;

    #ifdef __GNUC__
        asm("MOV	%0, #1" : "=r" (reg));
        asm("MCR	p14, 0, %0, c7, c0, 0" :: "r" (reg));
    #else
        __asm
        {
            mov     reg,#1                          // Idle mode
            mcr     p14, 0, reg, c7, c0, 0          // Shut down the PXA271 CPU clock until interrupted
        }
    #endif
    }
#endif
}

void PXA271_Driver::Sleep()
{
    // it should never be called from an ISR
    ASSERT(!SystemState_Query(SYSTEM_STATE_ISR));

    // this doesn't work if we are taking interrupts!
    ASSERT_IRQ_MUST_BE_OFF();

    Pause();
}

void PXA271_Driver::Halt()
{
    GLOBAL_LOCK(irq);

    // disable all interrupt source in controller that might wake us from our slumber
    CPU_INTC_Initialize();

    Pause();
}

void PXA271_Driver::Reset()
{
    PXA271_WATCHDOG_Driver::ResetCpu();
}

void PXA271_Driver::Shutdown()
{
    GLOBAL_LOCK(irq);

    while(true);
}

void PXA271_Driver::Hibernate()
{
    // TODO TODO TODO
    // provide implementation
}

void HAL_AssertEx()
{
    CPU_GPIO_SetPinState( LED1_RED, LED_ON );
    
#if defined(TARGETLOCATION_RAM) && defined(DEBUG)
    while(1);
#endif

    // cause an abort and let the abort handler take over
    *((char*)0xFFFFFFFF) = 'a';
}

