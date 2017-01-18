////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\at91.h"

////////////////////////////////////////////////////////////////////////////////
#undef  TRACE_ALWAYS
#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////
#define DEFINE_IRQ(index, priority) { priority, { NULL, (void*)(size_t)index } }

AT91_AIC_Driver::IRQ_VECTORING AT91_AIC_Driver::s_IsrTable[] =
{
#ifdef PLATFORM_ARM_SAM9RL64_ANY

    DEFINE_IRQ(0,   7),      // Advanced Interrupt Controller
    DEFINE_IRQ(1,   7),      // System Peripherals
    DEFINE_IRQ(2,   1),      // Parallel IO Controller A
    DEFINE_IRQ(3,   1),      // Parallel IO Controller B
    DEFINE_IRQ(4,   1),      // Parallel IO Controller C
    DEFINE_IRQ(5,   1),      // Parallel IO Controller D
    DEFINE_IRQ(6,   5),      // USART 0
    DEFINE_IRQ(7,   5),      // USART 1
    DEFINE_IRQ(8,   5),      // USART 2
    DEFINE_IRQ(9,   5),      // USART 3
    DEFINE_IRQ(10,  0),      // Multimedia Card Interface
    DEFINE_IRQ(11,  6),      // Two-Wire Interface
    DEFINE_IRQ(12,  6),      // Two-Wire Interface
    DEFINE_IRQ(13,  5),      // Serial Peripheral Interface 
    DEFINE_IRQ(14,  4),      // Serial Synchronous Controller 0
    DEFINE_IRQ(15,  4),      // Serial Synchronous Controller 1
    DEFINE_IRQ(16,  6),      // Timer Counter 0
    DEFINE_IRQ(17,  0),      // Timer Counter 1
    DEFINE_IRQ(18,  0),      // Timer Counter 2
    DEFINE_IRQ(19,  0),      // PWMC
    DEFINE_IRQ(20,  5),      // TSADCC
    DEFINE_IRQ(21,  6),      // DMAC
    DEFINE_IRQ(22,  6),      //High Speed USB
    DEFINE_IRQ(23,  3),      //LCDC
    DEFINE_IRQ(24,  3),      //AC97
    DEFINE_IRQ(25,  0),
    DEFINE_IRQ(26,  0),
    DEFINE_IRQ(27,  0),
    DEFINE_IRQ(28,  0),
    DEFINE_IRQ(29,  0),     
    DEFINE_IRQ(30,  0),     
    DEFINE_IRQ(31,  0),      // Advanced Interrupt Controller       

#elif defined(PLATFORM_ARM_SAM9261_ANY)
    DEFINE_IRQ(0,   7),      // Advanced Interrupt Controller
    DEFINE_IRQ(1,   7),      // System Peripherals
    DEFINE_IRQ(2,   1),      // Parallel IO Controller A
    DEFINE_IRQ(3,   1),      // Parallel IO Controller B
    DEFINE_IRQ(4,   1),      // Parallel IO Controller C
    DEFINE_IRQ(5,   0),
    DEFINE_IRQ(6,   5),      // USART 0
    DEFINE_IRQ(7,   5),      // USART 1
    DEFINE_IRQ(8,   5),      // USART 2
    DEFINE_IRQ(9,   0),      // Multimedia Card Interface
    DEFINE_IRQ(10,  2),      // USB Device Port
    DEFINE_IRQ(11,  6),      // Two-Wire Interface
    DEFINE_IRQ(12,  5),      // Serial Peripheral Interface 0
    DEFINE_IRQ(13,  5),      // Serial Peripheral Interface 1
    DEFINE_IRQ(14,  4),      // Serial Synchronous Controller 0
    DEFINE_IRQ(15,  4),      // Serial Synchronous Controller 1
    DEFINE_IRQ(16,  4),      // Serial Synchronous Controller 2
    DEFINE_IRQ(17,  6),      // Timer Counter 0
    DEFINE_IRQ(18,  0),      // Timer Counter 1
    DEFINE_IRQ(19,  0),      // Timer Counter 2
    DEFINE_IRQ(20,  2),      // USB Host port
    DEFINE_IRQ(21,  3),      // LCD Controller
    DEFINE_IRQ(22,  0),
    DEFINE_IRQ(23,  0),
    DEFINE_IRQ(24,  0),
    DEFINE_IRQ(25,  0),
    DEFINE_IRQ(26,  0),
    DEFINE_IRQ(27,  0),
    DEFINE_IRQ(28,  0),
    DEFINE_IRQ(29,  0),     // Advanced Interrupt Controller 
    DEFINE_IRQ(30,  0),     // Advanced Interrupt Controller 
    DEFINE_IRQ(31,  0),     // Advanced Interrupt Controller       

#elif defined(PLATFORM_ARM_SAM7X_ANY) 

    DEFINE_IRQ(0,   7),      // Advanced Interrupt Controller
    DEFINE_IRQ(1,   7),      // System Peripherals
    DEFINE_IRQ(2,   1),      // Parallel IO Controller A
    DEFINE_IRQ(3,   1),      // Parallel IO Controller B
    DEFINE_IRQ(4,   4),      // Serial Peripheral Interface 0
    DEFINE_IRQ(5,   4),      // Serial Peripheral Interface 1

    DEFINE_IRQ(6,   4),      // USART 0
    DEFINE_IRQ(7,   4),      // USART 1
    DEFINE_IRQ(8,   4),      // Serial Synchronous Controller 0
    DEFINE_IRQ(9,   3),      // Two-Wire Interface
    DEFINE_IRQ(10,  3),      // PWM
    DEFINE_IRQ(11,  5),      // USB Device Port
    DEFINE_IRQ(12,  3),      // Timer Counter 0
    DEFINE_IRQ(13,  0),      // Timer Counter 1
    DEFINE_IRQ(14,  0),      // Timer Counter 2
    DEFINE_IRQ(15,  6),      // CAM Controller
    DEFINE_IRQ(16,  4),      // EMAC
    DEFINE_IRQ(17,  0),      // ADC

    DEFINE_IRQ(18,  0),      // 
    DEFINE_IRQ(19,  0),      // 
    DEFINE_IRQ(20,  0),      // 
    DEFINE_IRQ(21,  0),      // 
    DEFINE_IRQ(22,  0),
    DEFINE_IRQ(23,  0),
    DEFINE_IRQ(24,  0),
    DEFINE_IRQ(25,  0),
    DEFINE_IRQ(26,  0),
    DEFINE_IRQ(27,  0),
    DEFINE_IRQ(28,  0),
    DEFINE_IRQ(29,  0),     
    DEFINE_IRQ(30,  0),     // Advanced Interrupt Controller 
    DEFINE_IRQ(31,  0),     // Advanced Interrupt Controller       


#elif defined (PLATFORM_ARM_SAM7S_ANY)
    DEFINE_IRQ(0,   7),      // Advanced Interrupt Controller
    DEFINE_IRQ(1,   7),      // System Peripherals
    DEFINE_IRQ(2,   1),      // Parallel IO Controller A
    DEFINE_IRQ(3,   0),      // reserved
    DEFINE_IRQ(4,   0),      // ADC
    DEFINE_IRQ(5,   4),      // Serial Peripheral Interface 0

    DEFINE_IRQ(6,   4),      // USART 0
    DEFINE_IRQ(7,   4),      // USART 1
    DEFINE_IRQ(8,   4),      // Serial Synchronous Controller 0
    DEFINE_IRQ(9,   3),      // Two-Wire Interface
    DEFINE_IRQ(10,  3),      // PWM
    DEFINE_IRQ(11,  5),      // USB Device Port
    DEFINE_IRQ(12,  3),      // Timer Counter 0
    DEFINE_IRQ(13,  0),      // Timer Counter 1
    DEFINE_IRQ(14,  0),      // Timer Counter 2

    DEFINE_IRQ(15,  0),      // 
    DEFINE_IRQ(16,  0),      // 
    DEFINE_IRQ(17,  0),      // 
    DEFINE_IRQ(18,  0),      // 
    DEFINE_IRQ(19,  0),      // 
    DEFINE_IRQ(20,  0),      // 
    DEFINE_IRQ(21,  0),      // 
    DEFINE_IRQ(22,  0),
    DEFINE_IRQ(23,  0),
    DEFINE_IRQ(24,  0),
    DEFINE_IRQ(25,  0),
    DEFINE_IRQ(26,  0),
    DEFINE_IRQ(27,  0),
    DEFINE_IRQ(28,  0),
    DEFINE_IRQ(29,  0),     
    DEFINE_IRQ(30,  0),     // Advanced Interrupt Controller 
    DEFINE_IRQ(31,  0),     // Advanced Interrupt Controller       


#endif

};


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
    #pragma arm section rwdata
#endif

////////////////////////////////////////////////////////////////////////////////

void AT91_AIC_Driver::Initialize()
{
    unsigned int i;

    AT91_AIC &aic = AT91::AIC();
    aic.AIC_IDCR = AT91_AIC::AIC_IDCR_DIABLE_ALL;           // Disable the interrupt on the interrupt controller
    aic.AIC_ICCR = AT91_AIC::AIC_ICCR_CLEAR_ALL;            // Clear the interrupt on the Interrupt Controller ( if one is pending )

    for (i = 0; i < c_VECTORING_GUARD; ++i) 
    {
        (void) aic.AIC_IVR;
        aic.AIC_EOICR = (unsigned int) i;
    }

    // set all priorities to the lowest
    IRQ_VECTORING* IsrVector = s_IsrTable;

    // set the priority level for each IRQ and stub the IRQ callback
    for (int i=0; i<c_VECTORING_GUARD;i++)
    {
        aic.AIC_SVR[i] = (unsigned int) i;

        if(i == (AT91C_ID_TC0 + AT91_TIMER_Driver::c_SystemTimer))
        {
            aic.AIC_SMR[i] = AT91_AIC::AIC_SRCTYPE_INT_POSITIVE_EDGE;
        }
        
        aic.AIC_SMR[i] &= ~AT91_AIC::AIC_PRIOR;
        aic.AIC_SMR[i] |= IsrVector[i].Priority;
        
        IsrVector[i].Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector[i].Priority);
    }

    // Set Spurious interrupt vector 
    aic.AIC_SPU = c_VECTORING_GUARD;
}

BOOL AT91_AIC_Driver::ActivateInterrupt(UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param)
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index );
    if(!IsrVector) 
        return FALSE;

    AT91_AIC &aic = AT91::AIC();

    GLOBAL_LOCK(irq);

    // disable this interrupt while we change it
    aic.AIC_IDCR= (0x01 << Irq_Index);

    // Clear the interrupt on the interrupt controller
    aic.AIC_ICCR = (0x01 << Irq_Index);
        
    // set the vector
    IsrVector->Handler.Initialize( ISR, ISR_Param );

    // enable the interrupt if we have a vector
    aic.AIC_IECR= 0x01<<Irq_Index;

    return TRUE;
}

BOOL AT91_AIC_Driver::DeactivateInterrupt( UINT32 Irq_Index )
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index );

    if(!IsrVector)
        return FALSE;

    GLOBAL_LOCK(irq);

    AT91_AIC &aic = AT91::AIC();

    // Clear the interrupt on the Interrupt Controller ( if one is pending )
    aic.AIC_ICCR = (1 << Irq_Index);

    // Disable the interrupt on the interrupt controller
    aic.AIC_IDCR = (1 << Irq_Index);

    // as it is stub, just put the Priority to the ISR parameter
    IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Priority );

    return TRUE;
}

void /*__irq*/ AT91_AIC_Driver::IRQ_Handler(void)
{
    unsigned int index;
    AT91_AIC &aic = AT91::AIC();

    // set before jumping elsewhere or allowing other interrupts
    SystemState_SetNoLock( SYSTEM_STATE_ISR              );
    SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

    while( (index = aic.AIC_IVR) < c_VECTORING_GUARD)
    {
        // Read IVR register (de-assert NIRQ) & check if we a spurous IRQ
        IRQ_VECTORING* IsrVector = &s_IsrTable[ index ];

        // In case the interrupt was forced, remove the flag.
        RemoveForcedInterrupt( index );

        IsrVector->Handler.Execute();

        // Mark end of Interrupt
        aic.AIC_EOICR = 1;
    }

    SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
    SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable

    // Mark end of Interrupt (Last IVR read)
    aic.AIC_EOICR = 1;
}

BOOL AT91_AIC_Driver::InterruptEnable( UINT32 Irq_Index  )
{
    BOOL WasEnabled;

    if(Irq_Index >= c_VECTORING_GUARD )
        return FALSE;

    AT91_AIC &aic = AT91::AIC();

    GLOBAL_LOCK(irq);

    WasEnabled = ((aic.AIC_IMR & (1 << Irq_Index)) !=0 ) ? TRUE : FALSE;

    aic.AIC_IECR = (1 << Irq_Index);

    return WasEnabled;
}


BOOL AT91_AIC_Driver::InterruptDisable( UINT32 Irq_Index )
{
    BOOL WasEnabled;

    if(Irq_Index >= c_VECTORING_GUARD )
        return FALSE;

    AT91_AIC &aic = AT91::AIC();

    GLOBAL_LOCK(irq);

    WasEnabled = ((aic.AIC_IMR & (1 << Irq_Index)) !=0 ) ? TRUE : FALSE;

    aic.AIC_IDCR = (1 << Irq_Index);

    return WasEnabled;
}


BOOL AT91_AIC_Driver::InterruptEnableState( UINT32 Irq_Index )
{
    BOOL IsEnabled;

    if(Irq_Index >= c_VECTORING_GUARD )
        return FALSE;

    AT91_AIC &aic = AT91::AIC();

    IsEnabled = ((aic.AIC_IMR & (1 << Irq_Index)) !=0 ) ? TRUE : FALSE;

    return IsEnabled;
}


BOOL  AT91_AIC_Driver::InterruptState( UINT32 Irq_Index )
{
    BOOL IsPending;

    if(Irq_Index >= c_VECTORING_GUARD )
        return FALSE;

    AT91_AIC &aic = AT91::AIC();

    IsPending = ((aic.AIC_IPR & (1 << Irq_Index)) !=0 ) ? TRUE : FALSE;

    return IsPending;
}

AT91_AIC_Driver::IRQ_VECTORING*AT91_AIC_Driver::IRQToIRQVector( UINT32 IRQ )
{
    IRQ_VECTORING* IsrVector = s_IsrTable;

    if (IRQ < c_VECTORING_GUARD)
    {
        return &IsrVector[IRQ];
    }

    return NULL;
}

void AT91_AIC_Driver::STUB_ISRVector( void* Param )
{
    lcd_printf( "\fSTUB_ISR %08x\r\n", (size_t)Param );
    HARD_BREAKPOINT();
}

void __irq IRQ_Handler(void)
{
    AT91_AIC_Driver::IRQ_Handler();
}

