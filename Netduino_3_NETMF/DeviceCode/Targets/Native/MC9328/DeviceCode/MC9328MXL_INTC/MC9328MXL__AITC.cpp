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
#pragma arm section rwdata = "s_IsrTable_MC9328MXL"
#endif

#define DEFINE_IRQ(index) { index, { NULL, (void*)(size_t)index } }

MC9328MXL_AITC_Driver::IRQ_VECTORING __section(rwdata) MC9328MXL_AITC_Driver::s_IsrTable[] =
{
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused0          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused1          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused2          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused3          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused4          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused5          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_CSI_INT          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_MMA_MAC_INT      ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_MMA_INT          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused9          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_MSIRQ            ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTA   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTB   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTC   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_LCDC_INT         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused15         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused16         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_RTC_INT          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_RTC_SAM_INT      ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_PFERR ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_RTS   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_DTR   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_UARTC ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_TX    ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART2_MINT_RX    ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_PFERR ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_RTS   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_DTR   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_UARTC ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_TX    ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_UART1_MINT_RX    ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused31         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused32         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused33         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_PWM_INT          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_MMC_IRQ          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused36         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused37         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused38         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_I2C_INT          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_SPI2_INT         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_SPI1_INT         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_SSI_TX_INT       ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_SSI_TX_ERR_INT   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_SSI_RX_INT       ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_SSI_RX_ERR_INT   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused46         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT0        ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT1        ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT2        ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT3        ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT4        ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT5        ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT6        ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused54         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused55         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused56         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_unused57         ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_TIMER2_INT       ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_TIMER1_INT       ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_DMA_ERR          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_DMA_INT          ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTD   ),
    DEFINE_IRQ( MC9328MXL_AITC::c_IRQ_INDEX_WDT_INT          ),

    DEFINE_IRQ( MC9328MXL_AITC_Driver::c_VECTORING_GUARD     ),
};

#undef DEFINE_IRQ

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif

////////////////////////////////////////////////////////////////////////////////

void MC9328MXL_AITC_Driver::Initialize()
{
    MC9328MXL_AITC& AITC = MC9328MXL::AITC();

    // disable all interrupts
    AITC.INTENABLEH = 0;
    AITC.INTENABLEL = 0;
        
    AITC.INTCTL = 0; // disregard normal and fast interrupt flags on system bus requests
#if defined(FIQ_SAMPLING_PROFILER)    
    AITC.INTTYPEH = 0;//disregard all initial garbage
#endif    

    // set all priorities to the lowest
    IRQ_VECTORING* IsrVector = s_IsrTable;

    while(IsrVector->Index != c_VECTORING_GUARD)
    {
        AITC.SetPriority( IsrVector->Index, MC9328MXL_AITC::c_IRQ_Priority_0 );

        IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Index );

        IsrVector++;
    }
}

BOOL MC9328MXL_AITC_Driver::ActivateInterrupt( UINT32 Irq_Index, BOOL Fast, HAL_CALLBACK_FPN ISR, void* ISR_Param, UINT8 Priority )
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;

    {
        GLOBAL_LOCK(irq);

        MC9328MXL_AITC& AITC = MC9328MXL::AITC();

        // disable this interrupt while we change it
        AITC.INTDISNUM = IsrVector->Index;

        // set the correct type
        AITC.SetType( IsrVector->Index, Fast );

        // set the correct priority
        AITC.SetPriority( IsrVector->Index, Priority );

        // set the vector
        IsrVector->Handler.Initialize( ISR, ISR_Param );

        // enable the interrupt if we have a vector
        AITC.INTENNUM = IsrVector->Index;
    }

    return TRUE;
}

BOOL MC9328MXL_AITC_Driver::DeactivateInterrupt( UINT32 Irq_Index )
{
    // figure out the interrupt
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index ); if(!IsrVector) return FALSE;

    {
        GLOBAL_LOCK(irq);

        MC9328MXL_AITC& AITC = MC9328MXL::AITC();

        // disable this interrupt while we change it
        AITC.INTDISNUM = IsrVector->Index;

        IsrVector->Handler.Initialize( STUB_ISRVector, (void*)(size_t)IsrVector->Index );
    }

    return TRUE;
}

void __irq IRQ_Handler()
{
    MC9328MXL_AITC_Driver::IRQ_Handler();
}

void MC9328MXL_AITC_Driver::IRQ_Handler()
{
    UINT32 index;

    MC9328MXL_AITC& AITC = MC9328MXL::AITC();

    // set before jumping elsewhere or allowing other interrupts
    SystemState_SetNoLock( SYSTEM_STATE_ISR              );
    SystemState_SetNoLock( SYSTEM_STATE_NO_CONTINUATIONS );

    while((index = AITC.NormalInterruptPending()) != AITC.c_MaxInterruptIndex)
    {
        IRQ_VECTORING* IsrVector = &s_IsrTable[ index ];
        
        // In case the interrupt was forced, remove the flag.
        AITC.RemoveForcedInterrupt( index );

        IsrVector->Handler.Execute();
    }

    SystemState_ClearNoLock( SYSTEM_STATE_NO_CONTINUATIONS ); // nestable
    SystemState_ClearNoLock( SYSTEM_STATE_ISR              ); // nestable
}

BOOL MC9328MXL_AITC_Driver::InterruptEnable( UINT32 Irq_Index  )
{
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index );

    if(!IsrVector) return FALSE;

    MC9328MXL_AITC& AITC = MC9328MXL::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.INTENNUM = IsrVector->Index;

    return WasEnabled;
}


BOOL MC9328MXL_AITC_Driver::InterruptDisable( UINT32 Irq_Index )
{
    IRQ_VECTORING* IsrVector = IRQToIRQVector( Irq_Index );

    if(!IsrVector) return 0;

    MC9328MXL_AITC& AITC = MC9328MXL::AITC();

    GLOBAL_LOCK(irq);

    BOOL WasEnabled = AITC.IsInterruptEnabled( Irq_Index );

    AITC.INTDISNUM = IsrVector->Index;

    return WasEnabled;
}


BOOL MC9328MXL_AITC_Driver::InterruptEnableState( UINT32 Irq_Index )
{
    MC9328MXL_AITC& AITC = MC9328MXL::AITC();

    return AITC.IsInterruptEnabled( Irq_Index );
}


BOOL  MC9328MXL_AITC_Driver::InterruptState( UINT32 Irq_Index )
{
    MC9328MXL_AITC& AITC = MC9328MXL::AITC();

    return AITC.GetInterruptState( Irq_Index );
}

MC9328MXL_AITC_Driver::IRQ_VECTORING* MC9328MXL_AITC_Driver::IRQToIRQVector( UINT32 IRQ )
{
    IRQ_VECTORING* IsrVector = s_IsrTable;

    while(IsrVector->Index != c_VECTORING_GUARD)
    {
        if(IsrVector->Index == IRQ) return IsrVector;

        IsrVector++;
    }

    return NULL;
}

void MC9328MXL_AITC_Driver::STUB_ISRVector( void* Param )
{
    lcd_printf( "\fSTUB_ISR %08x\r\n", (size_t)Param );
    HARD_BREAKPOINT();
}

