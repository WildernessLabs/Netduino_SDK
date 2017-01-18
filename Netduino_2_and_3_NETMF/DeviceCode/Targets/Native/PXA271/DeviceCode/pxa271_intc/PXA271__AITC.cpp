////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

////////////////////////////////////////////////////////////////////////////////

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

////////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "s_IsrTable_PXA271"
#endif

#define DEFINE_IRQ(index, priority) { priority, { NULL, (void*)(size_t)index } }

PXA271_AITC_Driver::IRQ_VECTORING __section(rwdata) PXA271_AITC_Driver::s_IsrTable[] =
{

    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_SSP_3          ,PXA271_AITC::c_IRQ_Priority_18  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_MSL            ,PXA271_AITC::c_IRQ_Priority_19  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_USB_HOST_2     ,PXA271_AITC::c_IRQ_Priority_20  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_USB_HOST_1     ,PXA271_AITC::c_IRQ_Priority_21  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_KEYPAD_CTRL    ,PXA271_AITC::c_IRQ_Priority_22  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_MEMORY_STICK   ,PXA271_AITC::c_IRQ_Priority_23  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_PWR_I2C        ,PXA271_AITC::c_IRQ_Priority_24  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_OS_TIMER       ,PXA271_AITC::c_IRQ_Priority_13  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_GPIO0          ,PXA271_AITC::c_IRQ_Priority_14  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_GPIO1          ,PXA271_AITC::c_IRQ_Priority_15  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_GPIOx          ,PXA271_AITC::c_IRQ_Priority_16  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_USB_CLIENT     ,PXA271_AITC::c_IRQ_Priority_17  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_PMU            ,PXA271_AITC::c_IRQ_Priority_25  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_I2S            ,PXA271_AITC::c_IRQ_Priority_26  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_AC97           ,PXA271_AITC::c_IRQ_Priority_27  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_USIM           ,PXA271_AITC::c_IRQ_Priority_28  ),

    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_SSP_2          ,PXA271_AITC::c_IRQ_Priority_12  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_LCD            ,PXA271_AITC::c_IRQ_Priority_11  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_I2C            ,PXA271_AITC::c_IRQ_Priority_10  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_INFRA_RED_COM  ,PXA271_AITC::c_IRQ_Priority_29  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_STUART         ,PXA271_AITC::c_IRQ_Priority_7   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_BTUART         ,PXA271_AITC::c_IRQ_Priority_8   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_FFUART         ,PXA271_AITC::c_IRQ_Priority_9   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_FLASH_CARD     ,PXA271_AITC::c_IRQ_Priority_30  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_SSP_1          ,PXA271_AITC::c_IRQ_Priority_6   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_DMA_CTRL       ,PXA271_AITC::c_IRQ_Priority_5   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_OS_TIMER0      ,PXA271_AITC::c_IRQ_Priority_0   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_OS_TIMER1      ,PXA271_AITC::c_IRQ_Priority_1   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_OS_TIMER2      ,PXA271_AITC::c_IRQ_Priority_2   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_OS_TIMER3      ,PXA271_AITC::c_IRQ_Priority_3   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_RTC_1HZ_TIC    ,PXA271_AITC::c_IRQ_Priority_31  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_RTC_ALARM      ,PXA271_AITC::c_IRQ_Priority_4   ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_TRUSTED_PLFM   ,PXA271_AITC::c_IRQ_Priority_32  ),
    DEFINE_IRQ( PXA271_AITC::c_IRQ_INDEX_QK_CAP         ,PXA271_AITC::c_IRQ_Priority_33  ),


    DEFINE_IRQ( PXA271_AITC_Driver::c_VECTORING_GUARD   ,PXA271_AITC::c_IRQ_Priority_34  ),
};

#undef DEFINE_IRQ

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif

////////////////////////////////////////////////////////////////////////////////

void PXA271_AITC_Driver::Initialize()
{
    PXA271_AITC& AITC = PXA271::AITC();


    // disable all interrupts
    AITC.ICMR = 0;
    AITC.ICMR2 = 0;
    // Only active interrupt will trigger the IRQ on idle
    AITC.ICCR = AITC.ICCR__DIM_OTHER; 
        

    // set all priorities to the lowest
    IRQ_VECTORING* IsrVector = s_IsrTable;

    // set the priority level for each IRQ and stub the IRQ callback
    for (int i=0; i<c_VECTORING_GUARD;i++)
    {
        AITC.SetPriority(i, IsrVector[i].Priority );
        IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Priority);
    }

}

BOOL PXA271_AITC_Driver::ActivateInterrupt( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param)
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;

    {
        GLOBAL_LOCK(irq);

        PXA271_AITC& AITC = PXA271::AITC();

        // disable this interrupt while we change it
        AITC.DisableInterrupt(Irq_Index);

        // set the correct type
        AITC.SetType( Irq_Index, Fast );

        // set the vector
        IsrVector->Handler.Initialize( ISR, ISR_Param );

        // enable the interrupt if we have a vector
        AITC.EnableInterrupt(Irq_Index);
    }

    return TRUE;
}

BOOL PXA271_AITC_Driver::DeactivateInterrupt( UINT32 Irq_Index )
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;

    {
        GLOBAL_LOCK(irq);

        PXA271_AITC& AITC = PXA271::AITC();

        // disable this interrupt while we change it
        AITC.DisableInterrupt(Irq_Index);
        // as it is stub, just put the Priority to the ISR parameter
        IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Priority );
    }

    return TRUE;
}

void __irq IRQ_Handler()
{
    UINT32 index;

    PXA271_AITC& AITC = PXA271::AITC();

    // set before jumping elsewhere or allowing other interrupts
    SystemState_SetNoLock( SYSTEM_STATE_ISR              );
    SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

    while((index = AITC.NormalInterruptPending()) != AITC.c_MaxInterruptIndex)
    {
        PXA271_AITC_Driver::IRQ_VECTORING* IsrVector = &PXA271_AITC_Driver::s_IsrTable[ index ];
        
        // In case the interrupt was forced, remove the flag.
        AITC.RemoveForcedInterrupt( index );

        IsrVector->Handler.Execute();
    }

    SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
    SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
}

BOOL PXA271_AITC_Driver::InterruptEnable( UINT32 Irq_Index  )
{

    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

    PXA271_AITC& AITC = PXA271::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.EnableInterrupt(Irq_Index);

    return WasEnabled;
}


BOOL PXA271_AITC_Driver::InterruptDisable( UINT32 Irq_Index )
{
    if(Irq_Index >= c_VECTORING_GUARD ) return FALSE;

    PXA271_AITC& AITC = PXA271::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.DisableInterrupt(Irq_Index);

    return WasEnabled;
}


BOOL PXA271_AITC_Driver::InterruptEnableState( UINT32 Irq_Index )
{
    PXA271_AITC& AITC = PXA271::AITC();

    return AITC.IsInterruptEnabled( Irq_Index );
}


BOOL  PXA271_AITC_Driver::InterruptState( UINT32 Irq_Index )
{
    PXA271_AITC& AITC = PXA271::AITC();

    return AITC.GetInterruptState( Irq_Index );
}

PXA271_AITC_Driver::IRQ_VECTORING* PXA271_AITC_Driver::IRQToIRQVector( UINT32 IRQ )
{
    IRQ_VECTORING* IsrVector = s_IsrTable;



    if (IRQ < c_VECTORING_GUARD)
    {
        return &IsrVector[IRQ];
       
    }

    return NULL;
}

void PXA271_AITC_Driver::STUB_ISRVector( void* Param )
{
    lcd_printf( "\fSTUB_ISR %08x\r\n", (size_t)Param );
    HARD_BREAKPOINT();
}

