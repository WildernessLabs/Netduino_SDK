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
#pragma arm section rodata = "c_Gpio_Attributes_PXA271"
#endif

const UINT8 __section(rodata) PXA271_GPIO_Driver::c_Gpio_Attributes[PXA271_GPIO_Driver::c_MaxPins] =
{
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   0   (register 0)
    GPIO_ATTRIBUTE_NONE                         , //   1
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   2
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   3
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   4
    GPIO_ATTRIBUTE_NONE                         , //   5
    GPIO_ATTRIBUTE_NONE                         , //   6
    GPIO_ATTRIBUTE_NONE                         , //   7
    GPIO_ATTRIBUTE_NONE                         , //   8
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   9
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  10
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  11
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  12
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  13
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  14
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  15
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  16
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  17
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  18
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  19
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  20
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  21
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  22
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  23
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  24
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  25
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  26
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  27
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  28
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  29
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  30
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  31
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  32   (register 1)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  33
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  34
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  35
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  36
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  37
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  38
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  39
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  40
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  41
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  42
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  43
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  44
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  45
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  46
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  47
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  48
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  49
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  50
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  51
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  52
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  53
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  54
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  55
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  56
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  57
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  58
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  59
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  60
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  61
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  62
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  63
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  64   (register 2)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  65
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  66
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  67
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  68
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  69
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  70
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  71
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  72
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  73
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  74
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  75
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  76
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  77
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  78
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  79
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  80
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  81
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  82
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  83
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  84
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  85
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  86
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  87
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  88
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  89
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  90
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  91
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  92
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  93
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  94
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  95
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  96   (register 3)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  97
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  98
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  99
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 100
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 101
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 102
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 103
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 104
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 105
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 106
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 107
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 108
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 109
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 110
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 111
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 112
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 113
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 114
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 115
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 116
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 117
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 118
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 119
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT  // 120
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata
#endif

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PXA271_GPIO_Driver"
#endif

PXA271_GPIO_Driver g_PXA271_GPIO_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL PXA271_GPIO_Driver::Initialize()
{
    int i;

    // initialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_PXA271_GPIO_Driver.m_PinIsr;

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

    SetDebounce( 10 ); 

    // specify that output pins are driven by the data register (DR_X)
    PXA271_GPIO& GPIO = PXA271::GPIO();
    for(i = 0; i < c_MaxPorts; i++)
    {
        GPIO.GAFR[i*2]   = 0;
        GPIO.GAFR[i*2+1] = 0;

        // initialize pins as free
        g_PXA271_GPIO_Driver.m_PinReservationInfo[i] = 0;
    }

    // set unsed pins as specified in platform selector
#if defined(PXA271_UNUSED_GPIOS)
    {
        struct UnusedGPIO
        {
            UINT8 Pin;
            UINT8 State;
        };

        static const UnusedGPIO c_Unused[] = { PXA271_UNUSED_GPIOS };

        const UnusedGPIO* ptr = c_Unused;

        for(size_t i = 0; i < ARRAYSIZE_CONST_EXPR(c_Unused); i++, ptr++)
        {
            if(ptr->State == RESISTOR_DISABLED)
            {
                EnableInputPin( (GPIO_PIN)ptr->Pin, FALSE, NULL, 0, GPIO_INT_NONE, RESISTOR_DISABLED );
            }
            else
            {
                EnableOutputPin( (GPIO_PIN)ptr->Pin, (BOOL)(ptr->State == RESISTOR_PULLUP) );
            }
        }
    }
#endif

    // All inputs are automatically disabled on Reset to cut down on power dissipation during intialization
    // This operation re-enables them after the unused GPIOs have been properly set up.
    PXA271::PWRMNGR().PSSR = PXA271_POWER_MANAGER::PSSR__RDH;       // Enable all PXA271 inputs

    // register interrupt handler for all ports
    return CPU_INTC_ActivateInterrupt( PXA271_AITC::c_IRQ_INDEX_GPIOx, ISR, (void*)(size_t)0);

}


BOOL PXA271_GPIO_Driver::Uninitialize()
{
    GLOBAL_LOCK(irq);

    // uninitialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_PXA271_GPIO_Driver.m_PinIsr;
        PXA271_GPIO& GPIO = PXA271::GPIO();
        int i;

        for(i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_completion.Abort();

            pinIsr++;
        }

        for(i = 0; i < c_MaxPorts; i++)
        {
            // turn off interrupts
            GPIO.rGRER(i*c_PinsPerPort) = 0;
            GPIO.rGFER(i*c_PinsPerPort) = 0;

            // flush pending interrupts
            GPIO.rGEDR(i*c_PinsPerPort) = 0xFFFFFFFF;
        }
    }

    return CPU_INTC_DeactivateInterrupt( PXA271_AITC::c_IRQ_INDEX_GPIOx );
}

UINT32 PXA271_GPIO_Driver::Attributes( GPIO_PIN Pin )
{
    if(Pin < c_MaxPins)
    {
        return c_Gpio_Attributes[Pin];
    }

    return GPIO_ATTRIBUTE_NONE;
}

void PXA271_GPIO_Driver::GetPinsMap( UINT8* pins, size_t size )
{
    const UINT8*    src = c_Gpio_Attributes;
    UINT8* dst = pins;   
    UINT32 maxpin = c_MaxPins;

    while ((size--) && (maxpin --))
    {
        *dst++ = *src++;  
    }

}

UINT8 PXA271_GPIO_Driver::GetSupportedResistorModes( GPIO_PIN pin)
{
    return PXA271_GPIO_Driver::c_GPIO_ResistorMode;
}


UINT8 PXA271_GPIO_Driver::GetSupportedInterruptModes( GPIO_PIN pin)
{
    return PXA271_GPIO_Driver::c_GPIO_InterruptMode;
}
   
    
void PXA271_GPIO_Driver::DisablePin( GPIO_PIN pin, GPIO_RESISTOR resistorState, UINT32 dir_out, GPIO_ALT_MODE function )
{
    UINT32   bitMask  = 1 << PinToBit( pin );
    PXA271_GPIO& GPIO = PXA271::GPIO();

    GLOBAL_LOCK(irq);

    PXA271::GPIO().SetPinFunction( pin, function );       // Set pin to specified alternate function

    GPIO.rGRER(pin) &= ~bitMask;
    GPIO.rGFER(pin) &= ~bitMask;          // Disable edge interrupts for pin
    GPIO.rGEDR(pin)  =  bitMask;          // Clear any pending interrupt for pin

    if(resistorState == RESISTOR_PULLUP)
        GPIO.rGPSR(pin) = bitMask;        // If set as output, will be driven high
    else if(resistorState == RESISTOR_PULLDOWN)
        GPIO.rGPCR(pin) = bitMask;        // If set as output, will be driven low

    if(dir_out)
        GPIO.rGPDR(pin) |=  bitMask;      // Set pin as output
    else
        GPIO.rGPDR(pin) &= ~bitMask;      // Set pin as input

    PIN_ISR_DESCRIPTOR& pinIsr = g_PXA271_GPIO_Driver.m_PinIsr[ pin ];

    pinIsr.m_intEdge = GPIO_INT_NONE;
    pinIsr.m_isr     = STUB_GPIOISRVector;
    pinIsr.m_param   = NULL;
}


void PXA271_GPIO_Driver::EnableOutputPin( GPIO_PIN pin, BOOL initialState )
{
    UINT32   bitMask  = 1 << PinToBit( pin );
    PXA271_GPIO& GPIO = PXA271::GPIO();

    GLOBAL_LOCK(irq);

    PXA271::GPIO().SetPinFunction( pin, PXA271_GPIO::GAFR__IO );       // Set pin to GPIO mode (disable any alternate functions)

    GPIO.rGRER(pin) &= ~bitMask;
    GPIO.rGFER(pin) &= ~bitMask;          // Disable edge interrupts for pin
    GPIO.rGEDR(pin) =  bitMask;           // Clear any pending interrupt for pin

    if(initialState)
        GPIO.rGPSR(pin) = bitMask;        // Drive output high
    else
        GPIO.rGPCR(pin) = bitMask;        // Drive output low

    GPIO.rGPDR(pin) |= bitMask;           // Set pin as output

    PIN_ISR_DESCRIPTOR& pinIsr = g_PXA271_GPIO_Driver.m_PinIsr[ pin ];

    pinIsr.m_intEdge = GPIO_INT_NONE;
    pinIsr.m_isr     = STUB_GPIOISRVector;
    pinIsr.m_param   = NULL;

}


BOOL PXA271_GPIO_Driver::EnableInputPin( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* pinIsrParam, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{
    if((intEdge == GPIO_INT_LEVEL_HIGH) || intEdge == GPIO_INT_LEVEL_LOW)
    {
        // The PXA271 processor does not support pin level interrupts - only edges
        return FALSE;
    }

    if(resistorState != RESISTOR_DISABLED)
    {
        return FALSE;       // There are no pullup/pulldown resistors on the PXA271
    }
    UINT32        bit = PinToBit( pin );
    UINT32    bitMask = 1 << bit;
    PXA271_GPIO& GPIO = PXA271::GPIO();

    GLOBAL_LOCK(irq);

    PIN_ISR_DESCRIPTOR& pinIsr = g_PXA271_GPIO_Driver.m_PinIsr[ pin ];

    PXA271::GPIO().SetPinFunction( pin, PXA271_GPIO::GAFR__IO );       // Set pin to GPIO mode (disable any alternate functions)

    GPIO.rGRER(pin) &= ~bitMask;
    GPIO.rGFER(pin) &= ~bitMask;          // Disable edge interrupts for pin
    GPIO.rGEDR(pin)  =  bitMask;          // Clear any pending interrupt for pin
    GPIO.rGPDR(pin) &= ~bitMask;          // Set pin as input

    BOOL pinState = (GPIO.rGPLR(pin) >> PinToBit(pin)) & 1;

    if(ISR)
    {
        UINT32           intMode;
        volatile UINT32& GPLRport = GPIO.rGPLR(pin);

        /********************/ pinIsr.m_intEdge = intEdge;
        /********************/ pinIsr.m_isr     = ISR;
        /********************/ pinIsr.m_param   = pinIsrParam;
        if(GlitchFilterEnable) pinIsr.m_flags   = PIN_ISR_DESCRIPTOR::c_Flags_Debounce;
        else                   pinIsr.m_flags   = 0;
        /********************/ pinIsr.m_status  = 0;

        // since there is no HW debounce, we always have to listen to both edges and 
        // filter the interrupt based on the desired edge and the debounce time 
        switch(intEdge)
        {
        case GPIO_INT_NONE:
            ASSERT(0);
            return FALSE;
            
        case GPIO_INT_EDGE_LOW :

            if(!GlitchFilterEnable)
            {                
                intMode = PXA271_GPIO::GPIO_INT_EDGE_FALLING;
                break;
            }

        case GPIO_INT_EDGE_HIGH:
            
            if(!GlitchFilterEnable)
            {            
                intMode = PXA271_GPIO::GPIO_INT_EDGE_RISING;
                break;
            }

        case GPIO_INT_EDGE_BOTH:
            {                                
                pinIsr.m_intEdge = GPIO_INT_EDGE_BOTH;

                if(intEdge == GPIO_INT_EDGE_BOTH)
                {
                    pinIsr.m_flags |= PIN_ISR_DESCRIPTOR::c_Flags_RequireHighEdge | PIN_ISR_DESCRIPTOR::c_Flags_RequireLowEdge;
                }
                else
                {
                    if(intEdge == GPIO_INT_EDGE_HIGH)
                    {
                        pinIsr.m_flags |= PIN_ISR_DESCRIPTOR::c_Flags_RequireHighEdge;
                    }
                    else
                    {
                        pinIsr.m_flags |= PIN_ISR_DESCRIPTOR::c_Flags_RequireLowEdge;
                    }                 
                }
                
                // check pin state before setting interrupt
                while(true)
                {
                    pinState = (GPLRport >> bit) & 1;

                    if(pinState)
                    {
                        pinIsr.m_status &= ~PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
                        pinIsr.m_status |=  PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;

                        intMode = PXA271_GPIO::GPIO_INT_EDGE_FALLING;

                        // make sure we didn't miss an edge interrupt (and leave it deadlocked!)
                        if((GPLRport & bitMask))     // If pin state unchanged
                        {
                            break;
                        }
                    }
                    else
                    {
                        pinIsr.m_status &= ~PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
                        pinIsr.m_status |=  PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;

                        intMode = PXA271_GPIO::GPIO_INT_EDGE_RISING;

                        // make sure we didn't miss an edge interrupt (and leave it deadlocked!)
                        if((GPLRport & bitMask) == 0)      // If pin state unchanged
                        {
                            break;
                        }
                    }
                }
            }
            break;

        default:            
            ASSERT(0);
            return FALSE;
        }

        // set mode and enable interrupt
        GPIO.SetInterruptMode( pin, intMode );
        
    }
    else
    {
        ASSERT(GPIO_INT_NONE == intEdge);

        // mask interrupt
        GPIO.SetInterruptMode( pin, 0 );      // Disable edge interrupts for this pin (redundant)

        pinIsr.m_intEdge = GPIO_INT_NONE;
        pinIsr.m_isr     = STUB_GPIOISRVector;
        pinIsr.m_param   = NULL;
    }

    return TRUE;
}


BOOL PXA271_GPIO_Driver::GetPinState( GPIO_PIN pin )
{
    ASSERT(pin < c_MaxPins);

    UINT32       bit  = PinToBit( pin );
    PXA271_GPIO& GPIO = PXA271::GPIO();

    return ((GPIO.rGPLR(pin) >> bit) & 1);
}


void PXA271_GPIO_Driver::SetPinState( GPIO_PIN pin, BOOL pinState )
{
    UINT32    bitMask = 1 << PinToBit( pin );
    PXA271_GPIO& GPIO = PXA271::GPIO();

    GLOBAL_LOCK(irq);

    if(pinState) GPIO.rGPSR(pin) = bitMask;
    else         GPIO.rGPCR(pin) = bitMask;
}


void PXA271_GPIO_Driver::ISR( void* Param )
{
    UINT32       pins;
    UINT32       bitIndex;
    UINT32       bitMask;
    UINT32       interruptsActive;
    PXA271_GPIO& GPIO              = PXA271::GPIO();

    // fire off each change, one at a time
    for( pins = 0; pins < c_MaxPins; pins += c_PinsPerPort )
    {
        volatile UINT32& GEDRport = GPIO.rGEDR(pins);
        interruptsActive          = GEDRport;
        
        for(bitIndex = 0, bitMask = 1; interruptsActive; bitIndex++, bitMask <<= 1)
        {
            if(!(interruptsActive & bitMask))
            {
                continue;
            }

            //--//

            UINT32 pin           = pins + bitIndex;
            UINT32 interruptMode = GPIO.GetInterruptMode( pin );

            // clear interrupts immediately
            GEDRport = bitMask;

            PIN_ISR_DESCRIPTOR& pinIsr = g_PXA271_GPIO_Driver.m_PinIsr[ pin ];

            // we only toggle the EDGEness if we are actually sending out PIN_ISRs to something meaningful
            if(pinIsr.m_intEdge != GPIO_INT_NONE)
            {
                BOOL negativeEdge;

                // if we are either or both edge triggered, clear that now
                if(pinIsr.m_intEdge == GPIO_INT_EDGE_BOTH)
                {
                    // which edge are we interrupting on?
                    negativeEdge = (interruptMode == PXA271_GPIO::GPIO_INT_EDGE_FALLING);
                }
                else
                {
                    negativeEdge = (pinIsr.m_intEdge == GPIO_INT_EDGE_LOW);
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
                        UINT32 pinStates1 = GPIO.GPLR[pins/c_PinsPerPort];

                        // if we already aren't on the right level, send this edge off
                        if(negativeEdge != ((pinStates1 & bitMask) == 0))
                        {
                            negativeEdge = !negativeEdge;
                            continue;
                        }

                        // ok, so far everything seems fine with all observed edges sent, pin in correct state
                        // so now change the direction of the interrupt
						
						if(negativeEdge)
						{
							GPIO.SetInterruptMode( pin, PXA271_GPIO::GPIO_INT_EDGE_RISING );
						}
						else
						{
							GPIO.SetInterruptMode( pin, PXA271_GPIO::GPIO_INT_EDGE_FALLING );
						}
                
                        GEDRport = bitMask;     // Clear any pending interrupts

                        // one final snapshot to check for missed edges
                        UINT32 pinStates2 = GPIO.rGPLR(pin);

                        // if our edge changed, and it wasn't caught in an interrupt, then send new edge and start over again
                        if(((pinStates1 ^ pinStates2) & bitMask) != 0 && (GEDRport & bitMask) == 0)
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
}

void PXA271_GPIO_Driver::SetResistor( GPIO_PIN pin, GPIO_RESISTOR resistorState )
{
    // There are no resistors for the PXA271
}

BOOL PXA271_GPIO_Driver::PinIsBusy( GPIO_PIN pin )
{
    UINT32  port     = pin / c_PinsPerPort;
    UINT32  bitMask  = 1 << PinToBit( pin );
    UINT32& res      = g_PXA271_GPIO_Driver.m_PinReservationInfo[port];

    return (res & bitMask) != 0 ? TRUE : FALSE;
}

BOOL PXA271_GPIO_Driver::ReservePin( GPIO_PIN pin, BOOL fReserve )
{
    GLOBAL_LOCK(irq);

    UINT32  port = pin / c_PinsPerPort;
    UINT32  bit  = PinToBit( pin );
    UINT32& res  = g_PXA271_GPIO_Driver.m_PinReservationInfo[port];
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

UINT32 PXA271_GPIO_Driver::GetDebounce()
{
    return CPU_TicksToTime( g_PXA271_GPIO_Driver.m_DebounceTicks ) / 10000;
}

BOOL PXA271_GPIO_Driver::SetDebounce( INT64 debounceTimeMilliseconds )
{   
    if(debounceTimeMilliseconds < c_MinDebounceTimeMs || c_MaxDebounceTimeMs < debounceTimeMilliseconds )
    {
        return FALSE;
    }

    g_PXA271_GPIO_Driver.m_DebounceTicks = CPU_MillisecondsToTicks( (UINT32)debounceTimeMilliseconds );

    return TRUE;
}

//--//

void PXA271_GPIO_Driver::STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param )
{
    lcd_printf( "\fSTUB_GPI %04x\r\n", (size_t)Param );

    DEBUG_HARD_BREAKPOINT();
}

//--//


void PXA271_GPIO_Driver::PIN_ISR_DESCRIPTOR::Fire( void* arg )
{
    PIN_ISR_DESCRIPTOR* desc = (PIN_ISR_DESCRIPTOR*)arg;
        
    desc->m_isr( desc->m_pin, (desc->m_status & c_Status_AllowHighEdge) != 0, desc->m_param );

    // we need to clear (invert) the status only for actual dual edge pins
    UINT8 mask = c_Flags_RequireHighEdge | c_Flags_RequireLowEdge;
    
    if((desc->m_flags & mask) == mask)
    {
        desc->m_status ^= PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge | PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
    }
}

void PXA271_GPIO_Driver::PIN_ISR_DESCRIPTOR::HandleDebounce( BOOL edge )
{
    ASSERT_IRQ_MUST_BE_OFF();

    m_completion.Abort();

    UINT8 flagsMask;
	UINT8 statusMask;

	if(edge)
	{
		flagsMask = c_Flags_RequireHighEdge;
		statusMask = c_Status_AllowHighEdge;
	}
	else
	{
		flagsMask = c_Flags_RequireLowEdge;
		statusMask = c_Status_AllowLowEdge;
	}

    if((m_flags & flagsMask) && (m_status & statusMask))
    {
        m_completion.EnqueueTicks( Time_CurrentTicks() + g_PXA271_GPIO_Driver.m_DebounceTicks );
    }
}

