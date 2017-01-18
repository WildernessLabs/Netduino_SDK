////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "AT91_GPIO.h"

//--//

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "c_Gpio_Attributes_AT91"
#endif

const UINT8 __section(rodata) AT91_GPIO_Driver::c_Gpio_Attributes[AT91_GPIO_Driver::c_MaxPins] =
{
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   0   (register 0)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   1
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   2
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   3
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   4
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   5
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   6
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   7
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   8
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
#if (AT91_MAX_GPIO > 32)
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
#endif
#if (AT91_MAX_GPIO > 64)
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
#endif
#if (AT91_MAX_GPIO > 96)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  96   (register 3)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  97
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  98
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  99
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  100
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  101
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  102
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  103
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  104
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  105
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  106
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  107
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  108
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  109
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  110
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  111
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  112
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  113
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  114
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  115
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  116
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  117
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  118
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  119
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  120
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  121
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  122
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  123
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  124
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  125
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  126
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  127	

#endif
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata
#endif

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_AT91_GPIO_Driver"
#endif

AT91_GPIO_Driver g_AT91_GPIO_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL AT91_GPIO_Driver::Initialize()
{
    int i;

    // initialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_AT91_GPIO_Driver.m_PinIsr;

        for(i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_pin     = i;
            pinIsr->m_intEdge = GPIO_INT_NONE;
            pinIsr->m_isr     = STUB_GPIOISRVector;
            pinIsr->m_param   = NULL;
            pinIsr->m_flags   = 0;
#if !defined(PLATFORM_ARM_SAM7_ANY)
            pinIsr->m_completion.Initialize( );
            pinIsr->m_completion.InitializeForISR( &PIN_ISR_DESCRIPTOR::Fire, pinIsr );
#endif
            pinIsr++;
        }
    }

#if !defined(PLATFORM_ARM_SAM7_ANY)
    SetDebounce( 20 );
#endif

    for(i = 0; i < c_MaxPorts; i++)
    {
        // initialize pins as free
        AT91_PIO &pioX = AT91::PIO(i);

        g_AT91_GPIO_Driver.m_PinReservationInfo[i] = 0;
        
        pioX.PIO_IDR = 0xffffffff;                // Disable all interrupts
        pioX.PIO_ISR ^= 0xffffffff;
    }

    // set unsed pins as specified in platform selector
#if defined(AT91_UNUSED_GPIOS)
    {
        struct UnusedGPIO
        {
            UINT8 Pin;
            UINT8 State;
        };

        static const UnusedGPIO c_Unused[] = { AT91_UNUSED_GPIOS };

        const UnusedGPIO* ptr = c_Unused;

        for(size_t i = 0; i < ARRAYSIZE(c_Unused); i++, ptr++)
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

    // register interrupt handler for all ports
    if(!CPU_INTC_ActivateInterrupt( AT91C_ID_PIOA, ISR, (void*)(size_t)0 )) return FALSE;
#if (AT91_MAX_GPIO > 32)
    if(!CPU_INTC_ActivateInterrupt( AT91C_ID_PIOB, ISR, (void*)(size_t)1 )) return FALSE;
#endif 

#if (AT91_MAX_GPIO > 64)
    if(!CPU_INTC_ActivateInterrupt( AT91C_ID_PIOC, ISR, (void*)(size_t)2 )) return FALSE;
#endif

#if (AT91_MAX_GPIO > 96)
    if(!CPU_INTC_ActivateInterrupt( AT91C_ID_PIOD, ISR, (void*)(size_t)3 )) return FALSE;
#endif


    // TODO: will be replaced by PMC API

    AT91_PMC &pmc = AT91::PMC();
    pmc.EnablePeriphClock(AT91C_ID_PIOA);

#if  (AT91_MAX_GPIO > 32)
    pmc.EnablePeriphClock(AT91C_ID_PIOB);
#endif

#if  (AT91_MAX_GPIO > 64)
    pmc.EnablePeriphClock(AT91C_ID_PIOC);
#endif

#if  (AT91_MAX_GPIO > 96)
    pmc.EnablePeriphClock(AT91C_ID_PIOD);
#endif

    return TRUE;
}

//--//

BOOL AT91_GPIO_Driver::Uninitialize()
{
    GLOBAL_LOCK(irq);

    // uninitialize the interrupt information
    {
        UINT32 volatile trash;
        int i;

#if !defined(PLATFORM_ARM_SAM7_ANY)
        PIN_ISR_DESCRIPTOR* pinIsr = g_AT91_GPIO_Driver.m_PinIsr;
        for(i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_completion.Abort();

            pinIsr++;
        }
#endif

        for(i = 0; i < c_MaxPorts; i++)
        {
            AT91_PIO& pPioX = AT91::PIO(i);
            pPioX.PIO_IDR   = 0xffffffff;           // Disable all interrupts
            trash           = pPioX.PIO_ISR;        // flush pending
        }
    }

    if(!CPU_INTC_DeactivateInterrupt( AT91C_ID_PIOA )) return FALSE;
#if (AT91_MAX_GPIO > 32)
    if(!CPU_INTC_DeactivateInterrupt( AT91C_ID_PIOB )) return FALSE;
#endif 

#if (AT91_MAX_GPIO > 64)
    if(!CPU_INTC_DeactivateInterrupt( AT91C_ID_PIOC )) return FALSE;
#endif

    // TODO: will be replaced by PMC API

    AT91_PMC &PMC = AT91::PMC();
    PMC.DisablePeriphClock(AT91C_ID_PIOA);
#if  (AT91_MAX_GPIO > 32)
    PMC.DisablePeriphClock(AT91C_ID_PIOB);
#endif

#if (AT91_MAX_GPIO > 64)
    PMC.DisablePeriphClock(AT91C_ID_PIOC);
#endif

    return TRUE;
}

//--//

UINT32 AT91_GPIO_Driver::Attributes( GPIO_PIN Pin )
{
    if(Pin < c_MaxPins)
    {
        return c_Gpio_Attributes[Pin];
    }

    return GPIO_ATTRIBUTE_NONE;
}

//--//

void AT91_GPIO_Driver::DisablePin( GPIO_PIN pin, GPIO_RESISTOR resistorState, UINT32 dir_out, GPIO_ALT_MODE function )
{
    ASSERT(pin < c_MaxPins);

    UINT32  bitmask = 1 << PinToBit ( pin );
    UINT32  port = PinToPort( pin );


    AT91_PIO &pioX = AT91::PIO (port);


    GLOBAL_LOCK(irq);

    if(resistorState == RESISTOR_DISABLED) 
        pioX.PIO_PPUDR = bitmask;         // Disable the pull up resistor
    else if(resistorState == RESISTOR_PULLUP)       
        pioX.PIO_PPUER =  bitmask;            // Enable the pull up resistor

    switch (function)
    {
        case GPIO_ALT_MODE_1:               // Enable Peripheral A function
            pioX.PIO_PDR = bitmask;   
            pioX.PIO_ASR = bitmask;
            break;

            
        case GPIO_ALT_MODE_2:               // Enable Peripheral B function
            pioX.PIO_PDR = bitmask;   
            pioX.PIO_BSR = bitmask;
            break;
            
        default:        
            break;              
    }

    pioX.PIO_IDR = bitmask;                   // Disable the Input Change Interrupt       

    PIN_ISR_DESCRIPTOR& pinIsr = g_AT91_GPIO_Driver.m_PinIsr[ pin ];

    pinIsr.m_intEdge = GPIO_INT_NONE;
    pinIsr.m_isr     = STUB_GPIOISRVector;
    pinIsr.m_param   = NULL;
}

//--//

void AT91_GPIO_Driver::EnableOutputPin( GPIO_PIN pin, BOOL initialState )
{

    
    UINT32   bitmask  = 1 << PinToBit( pin );
    UINT32  port = PinToPort( pin );


    AT91_PIO &pioX = AT91::PIO (port);

    GLOBAL_LOCK(irq);

    pioX.PIO_PER = bitmask;                   // Enable PIO function
    
        if(initialState)
        pioX.PIO_SODR = bitmask;
    else
        pioX.PIO_CODR = bitmask;

    pioX.PIO_OER = bitmask;                   // Enable Output

    PIN_ISR_DESCRIPTOR& pinIsr = g_AT91_GPIO_Driver.m_PinIsr[ pin ];

    pinIsr.m_intEdge = GPIO_INT_NONE;
    pinIsr.m_isr     = STUB_GPIOISRVector;
    pinIsr.m_param   = NULL;

}

//--//

BOOL AT91_GPIO_Driver::EnableInputPin( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* pinIsrParam, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{
    ASSERT(pin < c_MaxPins);


    UINT32   bitmask  = 1 << PinToBit( pin );
    UINT32  port = PinToPort( pin );

    AT91_PIO &pioX = AT91::PIO (port);

    GLOBAL_LOCK(irq);   

    switch (resistorState)
    {
        case RESISTOR_DISABLED:
            pioX.PIO_PPUDR = bitmask;     // Disable the pull up resistor
            break;
        case RESISTOR_PULLUP:
            pioX.PIO_PPUER =  bitmask;        // Enable the pull up resistor
            break;
        case RESISTOR_PULLDOWN:
            return FALSE;                       // There are no pulldown resistors on the SAM9
    }

    PIN_ISR_DESCRIPTOR& pinIsr = g_AT91_GPIO_Driver.m_PinIsr[ pin ];

    pioX.PIO_PER = bitmask;                   // Enable PIO function
    pioX.PIO_ODR = bitmask;               // Disable Output

    pinIsr.m_intEdge = intEdge;
    pinIsr.m_isr     = (ISR != NULL) ? ISR : STUB_GPIOISRVector;
    pinIsr.m_param   = pinIsrParam;
    pinIsr.m_flags   = (GlitchFilterEnable == TRUE) ? PIN_ISR_DESCRIPTOR::c_Flags_Debounce : 0;
    pinIsr.m_status  = 0;      
    
#if !defined(PLATFORM_ARM_SAM7_ANY)
    pinIsr.m_completion.Abort();
    pinIsr.m_completion.Initialize();
#endif
    
    if(ISR)
    {
        // since there is no HW debounce, we always have to listen to both edges and 
        // filter the interrupt based on the desired edge and the debounce time 
        switch(intEdge)
        {
            case GPIO_INT_NONE:
                return FALSE;
            
            case GPIO_INT_EDGE_LOW :
            case GPIO_INT_LEVEL_LOW:
                pinIsr.m_status |= PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
                break;

            case GPIO_INT_EDGE_HIGH:
            case GPIO_INT_LEVEL_HIGH:
                pinIsr.m_status |= PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
                break;

            case GPIO_INT_EDGE_BOTH:
            {                 
                pinIsr.m_flags |= PIN_ISR_DESCRIPTOR::c_Flags_RequireHighEdge | PIN_ISR_DESCRIPTOR::c_Flags_RequireLowEdge;
                
                // check pin state before setting interrupt
                while(true)
                {
                    BOOL state = pioX.PIO_PDSR & bitmask;

                    if(state)
                    {
                        pinIsr.m_status &= ~PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
                        pinIsr.m_status |=  PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;                      

                        // make sure we didn't miss an edge interrupt (and leave it deadlocked!)
                        if( !(pioX.PIO_PDSR & bitmask) )
                        {
                            continue;
                        }
                    }
                    else
                    {
                        pinIsr.m_status &= ~PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
                        pinIsr.m_status |=  PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;

                        // make sure we didn't miss an edge interrupt (and leave it deadlocked!)
                        if( pioX.PIO_PDSR & bitmask )
                        {
                            continue;
                        }
                    }
                    break;
                }
            }
            break;

        default:
            ASSERT(0);
            return FALSE;
        }

        pioX.PIO_IER = bitmask;               // Enable interrupt
    }
    else
    {
        pioX.PIO_IDR = bitmask;               // Disable interrupt
    }
        
    return TRUE;
}


BOOL AT91_GPIO_Driver::GetPinState( GPIO_PIN pin )
{

    ASSERT(pin < c_MaxPins);

    UINT32  bit  = PinToBit( pin );
    UINT32  port = PinToPort( pin );

    AT91_PIO &pioX = AT91::PIO (port);

        return (((pioX.PIO_PDSR) >> bit) & 1);
}

//--//

void AT91_GPIO_Driver::SetPinState( GPIO_PIN pin, BOOL pinState )
{
    ASSERT(pin < c_MaxPins);

    UINT32  port = PinToPort( pin );
    UINT32  bit  = PinToBit ( pin );

    AT91_PIO &pioX = AT91::PIO (port);

    GLOBAL_LOCK(irq);

    if(pinState)
        pioX.PIO_SODR =  1 << bit;
    else
        pioX.PIO_CODR  = 1 << bit;
}

//--//

void AT91_GPIO_Driver::ISR( void* Param )
{
    
    UINT32  port = (UINT32)Param;
    
    AT91_PIO &pioX = AT91::PIO (port);    
    
    // fire off each change, one at a time
    UINT32 bitMask  = 0x00000001;
    UINT32 bitIndex = 0;

    UINT32 interruptsActive = pioX.PIO_ISR;
    
    while(interruptsActive)
    {
        while((interruptsActive & bitMask) == 0)
        {
            bitMask  <<= 1;
            ++bitIndex;
        }

        //--//

        GPIO_PIN pin = BitToPin( bitIndex, port);

        PIN_ISR_DESCRIPTOR& pinIsr = g_AT91_GPIO_Driver.m_PinIsr[ pin ];

        if(pinIsr.m_intEdge != GPIO_INT_NONE)
        {
            BOOL negativeEdge = TRUE;
            
            if(pinIsr.m_intEdge == GPIO_INT_EDGE_BOTH)
            {
                // which edge are we interrupting on?
                if(pinIsr.m_status & PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge)
                {
                    if( !(pioX.PIO_PDSR & bitMask) )
                        negativeEdge = TRUE;
                    else 
                        return;             // A pair of falling edge and rising edge may have been lost, exit without any handling
                }
                else if(pinIsr.m_status & PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge)
                {
                    if( pioX.PIO_PDSR & bitMask )
                        negativeEdge = FALSE;
                    else 
                        return;             // A pair of rising edge and falling edge may have been lost, exit without any handling 
                }                           
            }
            else
            {
                if(pinIsr.m_status & PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge)
                {
                    if( !(pioX.PIO_PDSR & bitMask) )
                    {
                        negativeEdge = TRUE;
                    }
                    else
                    {
                        return;             // throw out the unexpected falling edge
                    }
                }
                else if(pinIsr.m_status & PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge)
                {
                    if( pioX.PIO_PDSR & bitMask )
                    {
                        negativeEdge = FALSE;
                    }
                    else
                    {       
                        return;             // throw out the unexpected rising edge
                    }
                }
            }

            // send the interrupted state to the higher level
            if(pinIsr.m_flags & PIN_ISR_DESCRIPTOR::c_Flags_Debounce)
            {
                pinIsr.HandleDebounce( !negativeEdge );
            }
            else
            {
                pinIsr.Fire( (void*)&pinIsr );
            }
        }

        // remove the pin from the local check
        interruptsActive ^= bitMask;
    } 
}

//--//

void AT91_GPIO_Driver::SetResistor( GPIO_PIN pin, GPIO_RESISTOR resistorState )
{
    ASSERT(pin < c_MaxPins);

    UINT32  bitmask  = 1 << PinToBit( pin );
    UINT32  port = PinToPort( pin );

    AT91_PIO &pioX = AT91::PIO (port);

    GLOBAL_LOCK(irq);   

    switch (resistorState)
    {
        case RESISTOR_DISABLED:
            pioX.PIO_PPUDR = bitmask;     // Disable the pull up resistor
            break;
        case RESISTOR_PULLUP:
            pioX.PIO_PPUER =  bitmask;        // Enable the pull up resistor
            break;
        default:
            ASSERT(0);                          // There are no pulldown resistors on the SAM9
    }
    return;
}

//--//

BOOL AT91_GPIO_Driver::PinIsBusy( GPIO_PIN pin )
{
    UINT32  port     = pin / c_PinsPerPort;
    UINT32  bitMask  = 1 << PinToBit( pin );
    UINT32& res      = g_AT91_GPIO_Driver.m_PinReservationInfo[port];

    return (res & bitMask) != 0 ? TRUE : FALSE;
}

//--//

BOOL AT91_GPIO_Driver::ReservePin( GPIO_PIN pin, BOOL fReserve )
{
    GLOBAL_LOCK(irq);

    UINT32  port = pin / c_PinsPerPort;
    UINT32  bit  = PinToBit( pin );
    UINT32& res  = g_AT91_GPIO_Driver.m_PinReservationInfo[port];
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

//--//

UINT32 AT91_GPIO_Driver::GetDebounce()
{
#if !defined(PLATFORM_ARM_SAM7_ANY)
    return CPU_TicksToTime( g_AT91_GPIO_Driver.m_DebounceTicks ) / TIME_CONVERSION__TO_MILLISECONDS;
#else
    // assume that it takes 16 ticks for each ms the user wants
    // this of course needs calibration and it boils down to assessing 
    // what the loop in HandleDebounce takes
    return (AT91_GPIO_Driver::PIN_ISR_DESCRIPTOR::c_DebounceCount >> 4);
#endif
}

BOOL AT91_GPIO_Driver::SetDebounce( INT64 debounceTimeMilliseconds )
{   
#if !defined(PLATFORM_ARM_SAM7_ANY)
    if(debounceTimeMilliseconds < c_MinDebounceTimeMs || c_MaxDebounceTimeMs < debounceTimeMilliseconds )
    {
        return FALSE;
    }

    g_AT91_GPIO_Driver.m_DebounceTicks = CPU_MillisecondsToTicks( (UINT32)debounceTimeMilliseconds );

    return TRUE;
#else
    // cannot specify the debounce time
    return FALSE;
#endif
}

//--//

void AT91_GPIO_Driver::GetPinsMap( UINT8* pins, size_t size )
{
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

    
//--//

void AT91_GPIO_Driver::STUB_GPIOISRVector( GPIO_PIN Pin, BOOL PinState, void* Param )
{
    lcd_printf( "\fSTUB_GPI %04x\r\n", (size_t)Param );

    DEBUG_HARD_BREAKPOINT();
}

//--//

void AT91_GPIO_Driver::PIN_ISR_DESCRIPTOR::Fire( void* arg )
{
    PIN_ISR_DESCRIPTOR* desc = (PIN_ISR_DESCRIPTOR*)arg;
        
    desc->m_isr( desc->m_pin, (desc->m_status & c_Status_AllowHighEdge) != 0, desc->m_param );

    if(desc->m_intEdge == GPIO_INT_EDGE_BOTH)
    {
        desc->m_status ^= PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge | PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
    }
}

//--//

//
// extern declarations are required by GCC compiler as it cannot resolve the constant values.
//
#if defined(__GNUC__)
extern const UINT8 AT91_GPIO_Driver::PIN_ISR_DESCRIPTOR::c_Status_AllowHighEdge;
extern const UINT8 AT91_GPIO_Driver::PIN_ISR_DESCRIPTOR::c_Status_AllowLowEdge;
#endif


#if !defined(PLATFORM_ARM_SAM7_ANY)
void AT91_GPIO_Driver::PIN_ISR_DESCRIPTOR::HandleDebounce( BOOL edge )
{
    ASSERT_IRQ_MUST_BE_OFF();

    m_completion.Abort();

    UINT8 statusMask = edge ? c_Status_AllowHighEdge  : c_Status_AllowLowEdge ;

    if(m_status & statusMask)
    {
        m_completion.EnqueueTicks( HAL_Time_CurrentTicks() + g_AT91_GPIO_Driver.m_DebounceTicks );
    }
}
#else
void AT91_GPIO_Driver::PIN_ISR_DESCRIPTOR::HandleDebounce( BOOL edge )
{
    ASSERT_IRQ_MUST_BE_OFF();

    UINT32 count = c_DebounceCount;

    // we implement debouce by reading the pin a finite number of times
    while(--count > 0)
    {
        if(GetPinState( m_pin ) != edge)
        {
            // it was a glitch
            return;
        }
    }

    // debounce test passed
    Fire( this );
}
#endif

