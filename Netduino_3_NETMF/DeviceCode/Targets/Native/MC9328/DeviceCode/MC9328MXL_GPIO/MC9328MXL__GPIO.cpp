////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "c_Gpio_Attributes_MC9328MXL"
#endif

const UINT8 __section(rodata) MC9328MXL_GPIO_Driver::c_Gpio_Attributes[MC9328MXL_GPIO_Driver::c_MaxPins] =
{
    GPIO_ATTRIBUTE_NONE                         , // 00 Port A mux with A24
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 01
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 02
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 03
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 04
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 05
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 06
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 07
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 08
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 09
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 10
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 11
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 12
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 13
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 14
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 15
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 16
    GPIO_ATTRIBUTE_NONE                         , // 17 advise by Freescale errata, inadvertently affects when not use for DTACK.
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 18
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 19
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 20
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 21
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 22
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 23
    GPIO_ATTRIBUTE_NONE                         , // 24 // mux with A16
    GPIO_ATTRIBUTE_NONE                         , // 25 // mux with A17
    GPIO_ATTRIBUTE_NONE                         , // 26 // mux with A18
    GPIO_ATTRIBUTE_NONE                         , // 27 // mux with A19
    GPIO_ATTRIBUTE_NONE                         , // 28 // mux with A20
    GPIO_ATTRIBUTE_NONE                         , // 29 // mux with A21
    GPIO_ATTRIBUTE_NONE                         , // 30 // mux with A22
    GPIO_ATTRIBUTE_NONE                         , // 31 // mux with A23
    GPIO_ATTRIBUTE_NONE                         , // 00 Port B
    GPIO_ATTRIBUTE_NONE                         , // 01
    GPIO_ATTRIBUTE_NONE                         , // 02
    GPIO_ATTRIBUTE_NONE                         , // 03
    GPIO_ATTRIBUTE_NONE                         , // 04
    GPIO_ATTRIBUTE_NONE                         , // 05
    GPIO_ATTRIBUTE_NONE                         , // 06
    GPIO_ATTRIBUTE_NONE                         , // 07
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 08
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 09
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 10
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 11
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 12
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 13
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 14
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 15
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 16
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 17
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 18
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 19
    GPIO_ATTRIBUTE_NONE                         , // 20 // USB pins 
    GPIO_ATTRIBUTE_NONE                         , // 21 // USB pins
    GPIO_ATTRIBUTE_NONE                         , // 22 // USB pins
    GPIO_ATTRIBUTE_NONE                         , // 23 // USB pins
    GPIO_ATTRIBUTE_NONE                         , // 24 // USB pins
    GPIO_ATTRIBUTE_NONE                         , // 25 // USB pins
    GPIO_ATTRIBUTE_NONE                         , // 26 // USB pins
    GPIO_ATTRIBUTE_NONE                         , // 27 // USB pins
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 28
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 29
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 30
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 31
    GPIO_ATTRIBUTE_NONE                         , // 00 Port C
    GPIO_ATTRIBUTE_NONE                         , // 01
    GPIO_ATTRIBUTE_NONE                         , // 02
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 03
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 04
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 05
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 06
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 07
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 08
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 09
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 10
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 11
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 12
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 13
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 14
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 15
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 16
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 17
    GPIO_ATTRIBUTE_NONE                         , // 18
    GPIO_ATTRIBUTE_NONE                         , // 19
    GPIO_ATTRIBUTE_NONE                         , // 20
    GPIO_ATTRIBUTE_NONE                         , // 21
    GPIO_ATTRIBUTE_NONE                         , // 22
    GPIO_ATTRIBUTE_NONE                         , // 23
    GPIO_ATTRIBUTE_NONE                         , // 24
    GPIO_ATTRIBUTE_NONE                         , // 25
    GPIO_ATTRIBUTE_NONE                         , // 26
    GPIO_ATTRIBUTE_NONE                         , // 27
    GPIO_ATTRIBUTE_NONE                         , // 28
    GPIO_ATTRIBUTE_NONE                         , // 29
    GPIO_ATTRIBUTE_NONE                         , // 30
    GPIO_ATTRIBUTE_NONE                         , // 31
    GPIO_ATTRIBUTE_NONE                         , // 00 Port D
    GPIO_ATTRIBUTE_NONE                         , // 01
    GPIO_ATTRIBUTE_NONE                         , // 02
    GPIO_ATTRIBUTE_NONE                         , // 03
    GPIO_ATTRIBUTE_NONE                         , // 04
    GPIO_ATTRIBUTE_NONE                         , // 05
    GPIO_ATTRIBUTE_NONE                         , // 06 // mux with LSCLK
    GPIO_ATTRIBUTE_NONE                         , // 07 // mux with REV
    GPIO_ATTRIBUTE_NONE                         , // 08 // mux with CLS
    GPIO_ATTRIBUTE_NONE                         , // 09 // mux with PS
    GPIO_ATTRIBUTE_NONE                         , // 10 // mux with SPL_SPR
    GPIO_ATTRIBUTE_NONE                         , // 11 // mux with CONTRAST
    GPIO_ATTRIBUTE_NONE                         , // 12 // mux with ACD/OD
    GPIO_ATTRIBUTE_NONE                         , // 13 // mux with LP/HSYNC
    GPIO_ATTRIBUTE_NONE                         , // 14 // mux with FLM/VSYNC
    GPIO_ATTRIBUTE_NONE                         , // 15 // mux with LD00
    GPIO_ATTRIBUTE_NONE                         , // 16 // mux with LD01
    GPIO_ATTRIBUTE_NONE                         , // 17 // mux with LD02
    GPIO_ATTRIBUTE_NONE                         , // 18 // mux with LD03
    GPIO_ATTRIBUTE_NONE                         , // 19 // mux with LD04
    GPIO_ATTRIBUTE_NONE                         , // 20 // mux with LD05
    GPIO_ATTRIBUTE_NONE                         , // 21 // mux with LD06
    GPIO_ATTRIBUTE_NONE                         , // 22 // mux with LD07
    GPIO_ATTRIBUTE_NONE                         , // 23 // mux with LD08
    GPIO_ATTRIBUTE_NONE                         , // 24 // mux with LD09
    GPIO_ATTRIBUTE_NONE                         , // 25 // mux with LD10
    GPIO_ATTRIBUTE_NONE                         , // 26 // mux with LD11
    GPIO_ATTRIBUTE_NONE                         , // 27 // mux with LD00
    GPIO_ATTRIBUTE_NONE                         , // 28 // mux with LD13
    GPIO_ATTRIBUTE_NONE                         , // 29 // mux with LD14 
    GPIO_ATTRIBUTE_NONE                         , // 30 // mux with LD15 
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 31
};

const UINT32 MC9328MXL_GPIO_Driver::GPIO_MODE_TO_MC9328MXL_MODE[MC9328MXL_GPIO_Driver::c_Alt_Max_Mode]=
{
    MC9328MXL_GPIO_Driver::c_Alt_Primary,       // GPIO_ALT_PRIMARY 
    MC9328MXL_GPIO_Driver::c_GPIO_AIN,          // GPIO_ALT_MODE_1
    MC9328MXL_GPIO_Driver::c_GPIO_BIN,          // GPIO_ALT_MODE_2
    MC9328MXL_GPIO_Driver::c_GPIO_CIN,          // GPIO_ALT_MODE_3
    MC9328MXL_GPIO_Driver::c_GPIO_AOUT,         // GPIO_ALT_MODE_4
    MC9328MXL_GPIO_Driver::c_GPIO_BOUT,         // GPIO_ALT_MODE_5
    MC9328MXL_GPIO_Driver::c_Alt_Altenate_H     // GPIO_ALT_MODE_6
};


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata
#endif

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_MC9328MXL_GPIO_Driver"
#endif

MC9328MXL_GPIO_Driver g_MC9328MXL_GPIO_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL MC9328MXL_GPIO_Driver::Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    int i;

    // initialize the interrupt information for all pins
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_MC9328MXL_GPIO_Driver.m_PinIsr;

        for(i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_pin     = i;
            pinIsr->m_intEdge = GPIO_INT_NONE;
            pinIsr->m_isr     = STUB_GPIOISRVector;
            pinIsr->m_param   = NULL;

            pinIsr->m_completion.InitializeForISR( &PIN_ISR_DESCRIPTOR::Fire, pinIsr );

            pinIsr++;
        }
    }

    SetDebounce( 20 ); 

    // specify that output pins are driven by the data register (DR_X)
    for(i = 0; i < c_MaxPorts; i++)
    {
        MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( i );

        GPIO.OCR1_X = 0xFFFFFFFF;
        GPIO.OCR2_X = 0xFFFFFFFF;

        // initialize pins as free
        g_MC9328MXL_GPIO_Driver.m_PinReservationInfo[i] = 0;
    }

    // set unsed pins as input
#if defined(MC9328XML_UNUSED_GPIOS)
    {
        struct UnusedGPIO
        {
            UINT8 Pin;
            UINT8 Resistor;
        };

        static const UnusedGPIO c_Unused[] = { MC9328XML_UNUSED_GPIOS };

        const UnusedGPIO* ptr = c_Unused;

        for(size_t i = 0; i < ARRAYSIZE_CONST_EXPR(c_Unused); i++, ptr++)
        {
            EnableInputPin( (GPIO_PIN)ptr->Pin, FALSE, NULL, 0, GPIO_INT_NONE, (GPIO_RESISTOR)ptr->Resistor );
        }
    }
#endif

    // register interrupt handler for all ports
    if(!CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTA, ISR, (void*)(size_t)0 )) return FALSE;
    if(!CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTB, ISR, (void*)(size_t)1 )) return FALSE;
    if(!CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTC, ISR, (void*)(size_t)2 )) return FALSE;
    if(!CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTD, ISR, (void*)(size_t)3 )) return FALSE;

    return TRUE;
}


BOOL MC9328MXL_GPIO_Driver::Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    GLOBAL_LOCK(irq);

    // uninitialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_MC9328MXL_GPIO_Driver.m_PinIsr;
        UINT32 volatile trash;
        int i;

        for(i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_completion.Abort();

            pinIsr++;
        }

        for(i = 0; i < c_MaxPorts; i++)
        {
            MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( i );

            GPIO.IMR_X = 0x00000000;    // disable interrupts
            GPIO.ISR_X = 0xFFFFFFFF;    // flush pending interrupts
        }
    }

    // 5 sources for these
    if(!CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTA )) return FALSE;
    if(!CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTB )) return FALSE;
    if(!CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTC )) return FALSE;
    if(!CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_GPIO_INT_PORTD )) return FALSE;

    return TRUE;
}

UINT32 MC9328MXL_GPIO_Driver::Attributes( GPIO_PIN Pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if(Pin < c_MaxPins)
    {
        return c_Gpio_Attributes[Pin];
    }

    return GPIO_ATTRIBUTE_NONE;
}

void MC9328MXL_GPIO_Driver::GetPinsMap( UINT8* pins, size_t size )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();

    const UINT8*    src = c_Gpio_Attributes;
    UINT8* dst = pins;   
    UINT32 maxpin = c_MaxPins;

    if ( size ==0 ) return;
    while ((size--) && (maxpin --))
    {
        *dst = *src;  
        ++dst; ++src;
    }
}

UINT8 MC9328MXL_GPIO_Driver::GetSupportedResistorModes( GPIO_PIN pin)
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();

    return MC9328MXL_GPIO_Driver::c_GPIO_ResistorMode;
}


UINT8 MC9328MXL_GPIO_Driver::GetSupportedInterruptModes( GPIO_PIN pin)
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();

    return MC9328MXL_GPIO_Driver::c_GPIO_InterruptMode;
}

void MC9328MXL_GPIO_Driver::DisablePin( GPIO_PIN pin, GPIO_RESISTOR resistorState, UINT32 direction, GPIO_ALT_MODE AltFunction )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);
    ASSERT(AltFunction < MC9328MXL_GPIO_Driver::c_Alt_Max_Mode);
    
    UINT32 resMode;
    UINT32 altMode = GPIO_MODE_TO_MC9328MXL_MODE[AltFunction];

    switch(resistorState)
    {
    case RESISTOR_DISABLED: resMode = MC9328MXL_GPIO::PUEN__TRISTATED; break;
    case RESISTOR_PULLUP  : resMode = MC9328MXL_GPIO::PUEN__PULLHIGH ; break;
    case RESISTOR_PULLDOWN:
    default               : ASSERT(0); return;
    }

    UINT32          bit  = PinToBit ( pin );
    UINT32          port = PinToPort( pin );
    MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( port );

    GLOBAL_LOCK(irq);

    switch(altMode & MC9328MXL_GPIO_Driver::c_Alt_Header_MASK)
    {
    case MC9328MXL_GPIO_Driver::c_Alt_Primary: 

        GPIO.GIUS_X &= ~(1 << bit);
        GPIO.GPR_X  &= ~(1 << bit);
        if(direction) GPIO.DDIR_X |=  (1 << bit);
        else          GPIO.DDIR_X &= ~(1 << bit);

        break;

    case MC9328MXL_GPIO_Driver::c_Mux_In_H:
        GPIO.GIUS_X |= (1 << bit);
        // direction must be output for Ain, Bin, Cin
        ASSERT(direction);
        GPIO.DDIR_X |= (1 << bit);
        
        if (bit < 16) 
        {
            GPIO.OCR1_X &= (~(MC9328MXL_GPIO::OCR__mask<< (bit*2)));
            GPIO.OCR1_X |= ((altMode & MC9328MXL_GPIO_Driver::c_Alt_Function_MASK ) << (bit*2));  
        }
        else
        {
            GPIO.OCR2_X &= (~(MC9328MXL_GPIO::OCR__mask<< (bit-16)*2));
            GPIO.OCR2_X |= ((altMode & MC9328MXL_GPIO_Driver::c_Alt_Function_MASK) << (bit-16)*2);  
        }
        break;    
    case MC9328MXL_GPIO_Driver::c_Mux_Aout_H:
        GPIO.GIUS_X |=  (1 << bit);
        // direction must be input for Aout, Bout
        ASSERT(!direction);
        GPIO.DDIR_X &= ~(1 << bit);
        if (bit < 16) 
        {
            GPIO.ICONFA1_X &= (~(MC9328MXL_GPIO::ICONF__mask<< (bit*2)));
            GPIO.ICONFA1_X |= ((altMode & MC9328MXL_GPIO_Driver::c_Alt_Function_MASK)<< (bit*2));  
        }
        else
        {
            GPIO.ICONFA2_X &= (~(MC9328MXL_GPIO::ICONF__mask<< (bit-16)*2));
            GPIO.ICONFA2_X |= ((altMode & MC9328MXL_GPIO_Driver::c_Alt_Function_MASK) << (bit-16)*2);  
        }
        break;

    case MC9328MXL_GPIO_Driver::c_Mux_Bout_H:
        GPIO.GIUS_X |= (1 << bit);
        // direction must be input for Aout, Bout
        ASSERT(!direction);
        GPIO.DDIR_X &= ~(1 << bit);

        if (bit < 16) 
        {
            GPIO.ICONFB1_X &= (~(MC9328MXL_GPIO::ICONF__mask<< (bit*2)));
            GPIO.ICONFB1_X |= ((altMode & MC9328MXL_GPIO_Driver::c_Alt_Function_MASK) << (bit*2));  
        }
        else
        {
            GPIO.ICONFB2_X &= (~(MC9328MXL_GPIO::ICONF__mask<< (bit-16)*2));
            GPIO.ICONFB2_X |= ((altMode & MC9328MXL_GPIO_Driver::c_Alt_Function_MASK )<< (bit-16)*2);  
        }
        break;
        
    default :
        break;
    }

    GPIO.IMR_X  &= ~(1 << bit);
    if(resMode)   GPIO.PUEN_X |=  (1 << bit);
    else          GPIO.PUEN_X &= ~(1 << bit);

    
    PIN_ISR_DESCRIPTOR& pinIsr = g_MC9328MXL_GPIO_Driver.m_PinIsr[ pin ];

    pinIsr.m_intEdge = GPIO_INT_NONE;
    pinIsr.m_isr     = STUB_GPIOISRVector;
    pinIsr.m_param   = NULL;

#if 0
    hal_printf("Pin 0x%x ,bit 0x%x function %x\r\n",pin,bit, AltFunction);
    hal_printf("Dir 0x%x ,Resistor 0x%x \r\n",direction,resistorState);
    hal_printf(" GIU 0x%x\r\n",GPIO.GIUS_X);
    hal_printf(" IMR 0x%x\r\n",GPIO.IMR_X);
    hal_printf(" GPR 0x%x\r\n",GPIO.GPR_X);
    hal_printf(" PUEN_X 0x%x\r\n",GPIO.PUEN_X);
    hal_printf(" DDIR_X 0x%x\r\n",GPIO.DDIR_X);    
    hal_printf(" OCR1 0x%x\r\n",GPIO.OCR1_X);
    hal_printf(" OCR2 0x%x\r\n",GPIO.OCR2_X);
    hal_printf(" ICONFA1 0x%x\r\n",GPIO.ICONFA1_X);
    hal_printf(" ICONFA2 0x%x\r\n",GPIO.ICONFA2_X);
    hal_printf(" ICONFB1 0x%x\r\n",GPIO.ICONFB1_X);
    hal_printf(" ICONFB2 0x%x\r\n",GPIO.ICONFB2_X);
#endif     

}


void MC9328MXL_GPIO_Driver::EnableOutputPin( GPIO_PIN pin, BOOL initialState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    UINT32          bit  = PinToBit ( pin );
    UINT32          port = PinToPort( pin );
    MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( port );

    GLOBAL_LOCK(irq);

    /**************/ GPIO.IMR_X  &= ~(1 << bit);
    /**************/ GPIO.GIUS_X |=  (1 << bit);
    if(initialState) GPIO.DR_X   |=  (1 << bit);
    else             GPIO.DR_X   &= ~(1 << bit);
    /**************/ GPIO.DDIR_X |=  (1 << bit);

    PIN_ISR_DESCRIPTOR& pinIsr = g_MC9328MXL_GPIO_Driver.m_PinIsr[ pin ];

    pinIsr.m_intEdge = GPIO_INT_NONE;
    pinIsr.m_isr     = STUB_GPIOISRVector;
    pinIsr.m_param   = NULL;
}


BOOL MC9328MXL_GPIO_Driver::EnableInputPin( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* pinIsrParam, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    UINT32 resMode;

    // if debounce is required, we need to take some precautions... 
    if(GlitchFilterEnable == TRUE)
    {
        if(intEdge == GPIO_INT_LEVEL_HIGH)
        {
            // we cannot support this combination because this processor does not have 
            // HW debounce; the SW debounce will require to queue an asynchonous 
            // completion that will clear the level interrupt; the completion will never 
            // have an accasion to complete though because the interrupt will continue 
            // firing as it is a level one; even if we had a a special case for queueing 
            // the completion for level interrupts we would cause an interrupt storm 
            // until the completion time, which is customizable by the user 
            // we cannot support this combination for any kind of resistor
            return FALSE;
        }

        if(
           (intEdge       == GPIO_INT_LEVEL_LOW) && 
           (resistorState == RESISTOR_DISABLED ) 
          )
        {
            // we cannot support this combination because this processor does not have 
            // HW debounce; the SW debounce will require to queue an asynchonous 
            // completion that will clear the level interrupt; the completion will never 
            // have an accasion to complete though because the interrupt will continue 
            // firing as it is a level one; even if we had a a special case for queueing 
            // the completion for level interrupts we would cause an interrupt storm 
            // until the completion time, which is customizable by the user 
            return FALSE;
        }
    }

    switch(resistorState)
    {
    case RESISTOR_DISABLED: resMode = MC9328MXL_GPIO::PUEN__TRISTATED; break;
    case RESISTOR_PULLUP  : resMode = MC9328MXL_GPIO::PUEN__PULLHIGH ; break;
    case RESISTOR_PULLDOWN:
    default               : return FALSE;
    }

    UINT32          port = PinToPort( pin );
    UINT32          bit  = PinToBit ( pin );
    MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( port );

    GLOBAL_LOCK(irq);
    
    PIN_ISR_DESCRIPTOR& pinIsr = g_MC9328MXL_GPIO_Driver.m_PinIsr[ pin ];

    /*********/ GPIO.IMR_X  &= ~(1 << bit);
    /*********/ GPIO.GIUS_X |=  (1 << bit);
    /*********/ GPIO.DDIR_X &= ~(1 << bit);
    if(resMode) GPIO.PUEN_X |=  (1 << bit);
    else        GPIO.PUEN_X &= ~(1 << bit);

    pinIsr.m_intEdge = intEdge;
    pinIsr.m_isr     = (ISR != NULL) ? ISR : STUB_GPIOISRVector;
    pinIsr.m_param   = pinIsrParam;
    pinIsr.m_flags   = GlitchFilterEnable ? PIN_ISR_DESCRIPTOR::c_Flags_Debounce : 0;
    pinIsr.m_status  = 0;                        
    pinIsr.m_completion.Abort();
    pinIsr.m_completion.Initialize();

    if(ISR)
    {
        UINT32 intMode;


        // since there is no HW debounce, we always have to listen to both edges and 
        // filter the interrupt based on the desired edge and the debounce time 
        switch(intEdge)
        {
        case GPIO_INT_NONE:
            return FALSE;
            
        case GPIO_INT_EDGE_LOW :
            pinIsr.m_status |= PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
            intMode          = MC9328MXL_GPIO::ICR__INTRNE;
            break;

        case GPIO_INT_EDGE_HIGH:
            pinIsr.m_status |= PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
            intMode          = MC9328MXL_GPIO::ICR__INTRPE;
            break;

        case GPIO_INT_EDGE_BOTH:
            {                 
                pinIsr.m_flags |= PIN_ISR_DESCRIPTOR::c_Flags_RequireHighEdge | PIN_ISR_DESCRIPTOR::c_Flags_RequireLowEdge;
                
                // check pin state before setting interrupt
                while(true)
                {
                    BOOL state = (GPIO.SSR_X >> bit) & 1;

                    if(state)
                    {
                        pinIsr.m_status &= ~PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
                        pinIsr.m_status |=  PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;

                        intMode = MC9328MXL_GPIO::ICR__INTRNE;

                        // make sure we didn't miss an edge interrupt (and leave it deadlocked!)
                        if((GPIO.SSR_X & (1 << bit)) == 0)
                        {
                            continue;
                        }
                    }
                    else
                    {
                        pinIsr.m_status &= ~PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
                        pinIsr.m_status |=  PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;

                        intMode = MC9328MXL_GPIO::ICR__INTRPE;

                        // make sure we didn't miss an edge interrupt (and leave it deadlocked!)
                        if((GPIO.SSR_X & (1 << bit)) == (1 << bit))
                        {
                            continue;
                        }
                    }

                    break;
                }
            }
            break;

        case GPIO_INT_LEVEL_HIGH:
            pinIsr.m_status |= PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
            intMode          = MC9328MXL_GPIO::ICR__INTRPL;
            break;

        case GPIO_INT_LEVEL_LOW:
            pinIsr.m_status |= PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
            intMode          = MC9328MXL_GPIO::ICR__INTRNL;
            break;

        default:
            ASSERT(0);
            return FALSE;
        }

        // set mode
        GPIO.SetInterruptMode( bit, intMode );

        // unmask interrupt
        GPIO.IMR_X |= 1 << bit;
    }
    else
    {
        // mask interrupt
        GPIO.IMR_X &= ~(1 << bit);
    }

    return TRUE;
}


BOOL MC9328MXL_GPIO_Driver::GetPinState( GPIO_PIN pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    UINT32          port = PinToPort( pin );
    UINT32          bit  = PinToBit ( pin );
    MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( port );

    return ((GPIO.SSR_X >> bit) & 1);
}


void MC9328MXL_GPIO_Driver::SetPinState( GPIO_PIN pin, BOOL pinState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    UINT32          port = PinToPort( pin );
    UINT32          bit  = PinToBit ( pin );
    MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( port );

    GLOBAL_LOCK(irq);

    if(pinState) GPIO.DR_X |=  (1 << bit);
    else         GPIO.DR_X &= ~(1 << bit);
}


void MC9328MXL_GPIO_Driver::ISR( void* Param )
{
    UINT32          port = (UINT32)Param;
    MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( port );

    
    // fire off each change, one at a time
    UINT32 bitMask  = 0x00000001;
    UINT32 bitIndex = 0;

    UINT32 interruptsActive = GPIO.ISR_X;
    
    while(interruptsActive)
    {
        while((interruptsActive & bitMask) == 0)
        {
            bitMask  <<= 1;
            ++bitIndex;
        }

        //--//

        UINT32 interruptMode = GPIO.GetInterruptMode( bitIndex );

        // clear interrupts immediately
        GPIO.ISR_X = bitMask;

        GPIO_PIN pin = BitToPin( bitIndex, port);

        PIN_ISR_DESCRIPTOR& pinIsr = g_MC9328MXL_GPIO_Driver.m_PinIsr[ pin ];

        // we only toggle the EDGEness if we are actually sending out PIN_ISRs to something meaningful
        if(pinIsr.m_intEdge != GPIO_INT_NONE)
        {
            BOOL negativeEdge;

            // if we are either or both edge triggered, clear that now
            if(pinIsr.m_intEdge == GPIO_INT_EDGE_BOTH)
            {
                // which edge are we interrupting on?
                negativeEdge = (interruptMode == MC9328MXL_GPIO::ICR__INTRNE);
            }
            else
            {
                negativeEdge = ((pinIsr.m_intEdge == GPIO_INT_EDGE_LOW) || (pinIsr.m_intEdge == GPIO_INT_LEVEL_LOW));
            }

            // reconcile the interrupt being sent, next one programmed, with exit edge of input
            while(true)
            {
                // send the interrupted state to the higher level
                if(pinIsr.m_flags & PIN_ISR_DESCRIPTOR::c_Flags_Debounce)
                {
                    pinIsr.HandleDebounce( !negativeEdge );
                }
                else
                {
                    pinIsr.m_isr( pin, !negativeEdge, pinIsr.m_param );  // send low = FALSE, high = TRUE, plus preset parameter
                }

                if(GPIO_INT_EDGE_BOTH == pinIsr.m_intEdge)
                {
                    // what does the pin say?
                    UINT32 pinStates1 = GPIO.SSR_X;

                    // if we already aren't on the right level, send this edge off
                    if(negativeEdge != ((pinStates1 & bitMask) == 0))
                    {
                        negativeEdge = !negativeEdge;
                        continue;
                    }

                    // ok, so far everything seems fine with all observed edges sent, pin in correct state
                    // so now change the direction of the interrupt
                    if(negativeEdge)
                        GPIO.SetInterruptMode( bitIndex,  MC9328MXL_GPIO::ICR__INTRPE );
                    else
                        GPIO.SetInterruptMode( bitIndex,  MC9328MXL_GPIO::ICR__INTRNE );
            
                    GPIO.ISR_X = bitMask;

                    // one final snapshot to check for missed edges
                    UINT32 pinStates2 = GPIO.SSR_X;

                    // if our edge changed, and it wasn't caught in an interrupt, then send new edge and start over again
                    if(((pinStates1 ^ pinStates2) & bitMask) != 0 && (GPIO.ISR_X & bitMask) == 0)
                    {
                        negativeEdge = !negativeEdge;
                        continue;
                    }
                }

                break;
            }
        }

        // remove the pin from the local check
        interruptsActive ^= bitMask;
    }
}

void MC9328MXL_GPIO_Driver::SetResistor( GPIO_PIN pin, GPIO_RESISTOR resistorState )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT(pin < c_MaxPins);

    UINT32 resMode;

    switch(resistorState)
    {
    case RESISTOR_DISABLED: resMode = MC9328MXL_GPIO::PUEN__TRISTATED; break;
    case RESISTOR_PULLUP  : resMode = MC9328MXL_GPIO::PUEN__PULLHIGH ; break;
    case RESISTOR_PULLDOWN: ASSERT(0); return;
    default               : ASSERT(0); return;
    }

    UINT32          port = PinToPort( pin );
    UINT32          bit  = PinToBit ( pin );
    MC9328MXL_GPIO& GPIO = MC9328MXL::GPIO( port );

    GLOBAL_LOCK(irq);

    if(resMode) GPIO.PUEN_X |=  (1 << bit);
    else        GPIO.PUEN_X &= ~(1 << bit);
}

BOOL MC9328MXL_GPIO_Driver::PinIsBusy( GPIO_PIN pin )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    UINT32  port = PinToPort( pin );
    UINT32  bit  = PinToBit ( pin );
    UINT32& res  = g_MC9328MXL_GPIO_Driver.m_PinReservationInfo[port];

    return (res & (1u << bit)) != 0 ? TRUE : FALSE;
}

BOOL MC9328MXL_GPIO_Driver::ReservePin( GPIO_PIN pin, BOOL fReserve )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    GLOBAL_LOCK(irq);

    UINT32  port = PinToPort( pin );
    UINT32  bit  = PinToBit ( pin );
    UINT32& res  = g_MC9328MXL_GPIO_Driver.m_PinReservationInfo[port];
    UINT32  mask = (fReserve ? 1u : 0u) << bit;

    if((res & (1u << bit)) == mask) return FALSE;

    if(fReserve)
    {
        res |= (1u << bit);
    }
    else
    {
        res &= ~(1u << bit);
    }

    return TRUE;
}

UINT32 MC9328MXL_GPIO_Driver::GetDebounce()
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    return CPU_TicksToTime( g_MC9328MXL_GPIO_Driver.m_DebounceTicks ) / 10000;
}

BOOL MC9328MXL_GPIO_Driver::SetDebounce( INT64 debounceTimeMilliseconds )
{   
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    if(debounceTimeMilliseconds < c_MinDebounceTimeMs || c_MaxDebounceTimeMs < debounceTimeMilliseconds )
    {
        return FALSE;
    }

    g_MC9328MXL_GPIO_Driver.m_DebounceTicks = CPU_MillisecondsToTicks( (UINT32)debounceTimeMilliseconds );

    return TRUE;
}

//--//

void MC9328MXL_GPIO_Driver::STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param )
{
    lcd_printf( "\fSTUB_GPI %04x\r\n", (size_t)Param );
    hal_printf( "\fSTUB_GPI %04x %04x\r\n", Pin,(size_t)Param );
    DEBUG_HARD_BREAKPOINT();
}

//--//


void MC9328MXL_GPIO_Driver::PIN_ISR_DESCRIPTOR::Fire( void* arg )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    PIN_ISR_DESCRIPTOR* desc = (PIN_ISR_DESCRIPTOR*)arg;
        
    desc->m_isr( desc->m_pin, (desc->m_status & c_Status_AllowHighEdge) != 0, desc->m_param );

    // we need to clear (invert) the status only for actual dual edge pins
    UINT8 mask = c_Flags_RequireHighEdge | c_Flags_RequireLowEdge;
    
    if((desc->m_flags & mask) == mask)
    {
        desc->m_status ^= PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge | PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
    }
}

void MC9328MXL_GPIO_Driver::PIN_ISR_DESCRIPTOR::HandleDebounce( BOOL edge )
{
    NATIVE_PROFILE_HAL_PROCESSOR_GPIO();
    ASSERT_IRQ_MUST_BE_OFF();

    m_completion.Abort();

    UINT8 statusMask;

    if(edge)
        statusMask = c_Status_AllowHighEdge;
    else
        statusMask = c_Status_AllowLowEdge;

    if(m_status & statusMask)
    {
        m_completion.EnqueueTicks( HAL_Time_CurrentTicks() + g_MC9328MXL_GPIO_Driver.m_DebounceTicks );
    }
}

