////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

///////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)




BOOL MC9328MXL_Driver::Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    CPU_INTC_Initialize();

    // TODO TODO TODO
    // start out with all peripheral clocks disabled (this resets most peripherals too)

    // initialize EIM
    {
        MC9328MXL_EIM& EIM = MC9328MXL::EIM();

        // 6 devices, device 0 has different setup, default is used until Flash_Initialize is called to set parameters properly
        for(int i = 1; i < MC9328MXL_EIM::c_Devices; i++)
        {
            UINT32 reg;

            reg = EIM.Device[i].CS_L;
            reg |= MC9328MXL_EIM::DEVICE::CS_L__PA;               // Make sure CS disable state is correct (reset)
            reg &= ~MC9328MXL_EIM::DEVICE::CS_L__NEEDED_CONTROL;  // 0 at CSEN  = disables device
            EIM.Device[i].CS_L = reg;
        }
    }

    // TODO TODO TODO
    // enable buswatcher data write abort interrupts

    DMA_Initialize();

    // We need to set up a dummy DMA transfer when MMU is enabled to force the Memory Controller to

    // release the bus to prevent LCDC underrun (A MC9328MXL hardware bug, errata #7)
    if(CPU_IsMMUEnabled())
    {
        DMA_StartDummyDMA();
    }

    return TRUE;
}

void MC9328MXL_Driver::Pause()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    ASSERT_IRQ_MUST_BE_OFF();

    // simply writing this location cause the CPU to halt, waiting for an interrupt

#if defined(CPU_SPIN_NOT_SLEEP) 
    while(!MC9328MXL::AITC().IsInterruptPending());    // use this for profiling runs.
#else

#if defined(COMPILE_ARM) || defined(COMPILE_THUMB)
    {
        UINT32 reg;

#ifdef __GNUC__
        asm("MOV %0, #0" : "=r" (reg));
        asm("MCR p15, 0, %0, c7, c0, 4" :: "r" (reg));
#else
        __asm
        {
            mov     reg,#0
            mcr     p15, 0, reg, c7, c0, 4 // Place ARM920T into a low-power state
        }
#endif
    }

#endif

#endif
}

void MC9328MXL_Driver::Halt()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    GLOBAL_LOCK(irq);

    // disable all interrupt source in controller that might wake us from our slumber
    CPU_INTC_Initialize();

    Pause();
}

void MC9328MXL_Driver::Reset()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    MC9328MXL_WATCHDOG_Driver::ResetCpu();
}

void MC9328MXL_Driver::Shutdown()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    GLOBAL_LOCK(irq);

    while(true);
}

void MC9328MXL_Driver::Hibernate()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    ASSERT_IRQ_MUST_BE_ON();

    LCD_PowerSave(TRUE);

    Pause();

    LCD_PowerSave(FALSE);
}

void HAL_AssertEx()
{
    NATIVE_PROFILE_HAL_PROCESSOR_POWER();
    lcd_printf("\f!! ASSERT !!\n");
#if defined(JTAG_DEBUGGING)
    volatile BOOL fContinue=FALSE;
    while(!fContinue);
#else    
    // cause an abort and let the abort handler take over
    *((char*)0xFFFFFFFF) = 'a';
#endif
}


//--//


