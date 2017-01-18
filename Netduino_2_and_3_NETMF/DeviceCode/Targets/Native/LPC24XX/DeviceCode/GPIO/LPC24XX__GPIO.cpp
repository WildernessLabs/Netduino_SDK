//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include <tinyhal.h>
#include "..\LPC24XX.h"

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rodata = "c_Gpio_Attributes_LPC24XX"
#endif

const UINT8 __section(rodata) LPC24XX_GPIO_Driver::c_Gpio_Attributes[LPC24XX_GPIO_Driver::c_MaxPins] =
{
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 00 Port 0 UART0 - TXD0
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 01        UART0 - RXD0
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
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 17
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 18
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 19
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 20
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 21
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 22
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 23
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 24
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 25
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 26
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 27
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 28
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 29
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 30
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 31
    
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  32   (port 1)
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
    
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  64   (port 2)
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

    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  96   (port 3)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 127

    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 128   (port 4)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //  
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, // 159

};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_GPIO_Driver"
#endif

LPC24XX_GPIO_Driver g_LPC24XX_GPIO_Driver;
LPC24XX_GPIO_Driver g_LPC24XX_PINSEL_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL CPU_GPIO_Initialize()
{
    return LPC24XX_GPIO_Driver::Initialize();
}

BOOL CPU_GPIO_Uninitialize()
{
    return LPC24XX_GPIO_Driver::Uninitialize();
}

UINT32 CPU_GPIO_Attributes( GPIO_PIN Pin )
{
    return LPC24XX_GPIO_Driver::Attributes( Pin );
}

void CPU_GPIO_DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
    LPC24XX_GPIO_Driver::DisablePin( Pin, ResistorState, Direction, AltFunction);
}

void CPU_GPIO_EnableOutputPin( GPIO_PIN Pin, BOOL InitialState )
{
    LPC24XX_GPIO_Driver::EnableOutputPin( Pin, InitialState );
}

BOOL CPU_GPIO_EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return LPC24XX_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, ISR, 0, IntEdge, ResistorState );
}

BOOL CPU_GPIO_EnableInputPin2( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState )
{
    return LPC24XX_GPIO_Driver::EnableInputPin( Pin, GlitchFilterEnable, ISR, ISR_Param, IntEdge, ResistorState );
}

BOOL CPU_GPIO_GetPinState( GPIO_PIN Pin )
{
    return LPC24XX_GPIO_Driver::GetPinState( Pin );
}

void CPU_GPIO_SetPinState( GPIO_PIN Pin, BOOL PinState )
{
    LPC24XX_GPIO_Driver::SetPinState( Pin, PinState );
}

INT32 CPU_GPIO_GetPinCount()
{
    return LPC24XX_GPIO_Driver::GetPinCount();
}

void CPU_GPIO_GetPinsMap( UINT8* pins, size_t size )
{
    LPC24XX_GPIO_Driver::GetPinsMap( pins, size );
}

UINT8 CPU_GPIO_GetSupportedResistorModes(GPIO_PIN pin)
{
    return LPC24XX_GPIO_Driver::GetSupportedResistorModes( pin);
}

UINT8 CPU_GPIO_GetSupportedInterruptModes(GPIO_PIN pin)
{
    return LPC24XX_GPIO_Driver::GetSupportedInterruptModes( pin );
}

BOOL CPU_GPIO_PinIsBusy( GPIO_PIN Pin )
{
    return LPC24XX_GPIO_Driver::PinIsBusy( Pin );
}

BOOL CPU_GPIO_ReservePin( GPIO_PIN Pin, BOOL fReserve )
{
    return LPC24XX_GPIO_Driver::ReservePin( Pin, fReserve );
}

UINT32 CPU_GPIO_GetDebounce()
{
    return LPC24XX_GPIO_Driver::GetDebounce();
}

BOOL CPU_GPIO_SetDebounce( INT64 debounceTimeMilliseconds )
{
    return LPC24XX_GPIO_Driver::SetDebounce( debounceTimeMilliseconds );
}

//--//

///////////////////////////////////////////////////////////////////////////////
//  Initialize
///////////////////////////////////////////////////////////////////////////////
BOOL LPC24XX_GPIO_Driver::Initialize()
{
    int i;
    UINT32 port;
    LPC24XX_GPIO& GPIO = LPC24XX::GPIO();

    /* Enable fast GPIO on Port 0 & 1 */
    LPC24XX::SYSCON().SCS |= LPC24XX_SYSCON::HS_IO;

    for (port=0; port < c_MaxPins; port++)
    {
        g_LPC24XX_GPIO_Driver.m_PinReservationInfo[port] = FALSE;
    }

    // initialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_LPC24XX_GPIO_Driver.m_PinIsr;

        for(i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_pin     = i;
            pinIsr->m_intEdge = GPIO_INT_NONE;
            pinIsr->m_isr     = LPC24XX_GPIO_Driver::STUB_ISRVector;
            pinIsr->m_param   = NULL;
            
            pinIsr->m_completion.Initialize( );
            pinIsr->m_completion.InitializeForISR( &PIN_ISR_DESCRIPTOR::Fire, pinIsr );
            
            pinIsr++;
        }
    }

    
    // register interrupt handler (GPIO uses EINT3)
    if(!CPU_INTC_ActivateInterrupt( LPC24XX_VIC::c_IRQ_INDEX_EINT3, GPIO_ISR, NULL )) return FALSE;
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  Uninitialize
///////////////////////////////////////////////////////////////////////////////

BOOL LPC24XX_GPIO_Driver::Uninitialize()
{
    GLOBAL_LOCK(irq);

    // uninitialize the interrupt information
    {
        PIN_ISR_DESCRIPTOR* pinIsr = g_LPC24XX_GPIO_Driver.m_PinIsr;
        LPC24XX_GPIOIRQ& GPIOIRQ = LPC24XX::GPIOIRQ();

        for(int i = 0; i < c_MaxPins; i++)
        {
            pinIsr->m_completion.Abort();

            pinIsr++;
        }

        // turn off interrupts
        GPIOIRQ.IO0IntEnR = 0x00000000;
        GPIOIRQ.IO0IntEnF = 0x00000000;

        GPIOIRQ.IO2IntEnR = 0x00000000;
        GPIOIRQ.IO2IntEnF = 0x00000000;

        // flush pending interrupts
        GPIOIRQ.IO0IntClr = 0xFFFFFFFF;
        GPIOIRQ.IO2IntClr = 0xFFFFFFFF;
    }
    
    if(!CPU_INTC_DeactivateInterrupt( LPC24XX_VIC::c_IRQ_INDEX_EINT3 )) return FALSE;
    
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  Attributes
///////////////////////////////////////////////////////////////////////////////

UINT32 LPC24XX_GPIO_Driver::Attributes( GPIO_PIN pin )
{
    ASSERT(pin < c_MaxPins);

    if(pin < c_MaxPins)
    {
        return c_Gpio_Attributes[pin];
    }

    return GPIO_ATTRIBUTE_NONE;
}
 
///////////////////////////////////////////////////////////////////////////////
//  DisablePin
///////////////////////////////////////////////////////////////////////////////

void LPC24XX_GPIO_Driver::DisablePin( GPIO_PIN pin, GPIO_RESISTOR resistorState, UINT32 Direction, GPIO_ALT_MODE AltFunction )
{
    UINT32 PCBReg, PCBRegBit, temp;

    LPC24XX_PCB& PCB = LPC24XX::PCB();

    PCBReg = PinToPCBReg(pin);
    PCBRegBit = PinToPCBRegBit(pin);

    // Configure the pin to its first alternate function.
    GLOBAL_LOCK(irq);

    temp = (PCB.Regs[PCBReg].PINSEL) & ~( 0x3 << PCBRegBit );
    PCB.Regs[PCBReg].PINSEL = temp | ( AltFunction << PCBRegBit );
    
    // Disable ISR
    PIN_ISR_DESCRIPTOR& PinIsr = g_LPC24XX_GPIO_Driver.m_PinIsr[pin];

    PinIsr.m_intEdge = GPIO_INT_NONE;
    PinIsr.m_isr     = STUB_ISRVector;
    PinIsr.m_param   = (void*)(size_t)pin;
}

///////////////////////////////////////////////////////////////////////////////
//  EnableOutputPin
///////////////////////////////////////////////////////////////////////////////

void LPC24XX_GPIO_Driver::EnableOutputPin( GPIO_PIN pin, BOOL initialState )
{
    ASSERT(pin < c_MaxPins);

    UINT32 Port, Bit;
    
    LPC24XX_GPIO& GPIO = LPC24XX::GPIO();

    GLOBAL_LOCK(irq);

//  hal_printf( "EnableOutputPin: %08x\r\n", pin);
    Port = PinToPort(pin);
    Bit = PinToBit(pin);
    
//  Set initial state before changing direction of the port pin
    if(initialState) GPIO.Regs[Port].FIOSET_PX |=  (0x1 << Bit);
    else             GPIO.Regs[Port].FIOCLR_PX |=  (0x1 << Bit);

    GPIO.Regs[Port].FIODIR_PX |= 0x1 << Bit;
    
    // Disable ISR
    PIN_ISR_DESCRIPTOR& PinIsr = g_LPC24XX_GPIO_Driver.m_PinIsr[pin];

    PinIsr.m_intEdge = GPIO_INT_NONE;
    PinIsr.m_isr     = STUB_ISRVector;
    PinIsr.m_param   = (void*)(size_t)pin;
}

///////////////////////////////////////////////////////////////////////////////
//  EnableInputPin
///////////////////////////////////////////////////////////////////////////////
BOOL LPC24XX_GPIO_Driver::EnableInputPin( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* Param, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{
    ASSERT(pin < c_MaxPins);

    UINT32 Port, Bit;
    
    LPC24XX_GPIO& GPIO = LPC24XX::GPIO();

    GLOBAL_LOCK(irq);

    
    Port = PinToPort(pin);
    Bit = PinToBit(pin);
    //debug_printf( "EnableInputPin: 0x%08x (P%u.%u)\r\n", pin,Port,Bit);
    
    if((Port != 0) && (Port != 2))  //Only port 0 & 2 can generate interrupts on the LPC24XX
    {
        return FALSE;
    }
    switch (resistorState)
    {
        case RESISTOR_DISABLED:
            return FALSE;     // TODO: Disable the resistor instead of failing
            break;
        case RESISTOR_PULLUP:
                                // TODO: Enable the pull up resistor instead of relying on it being on since reset
            break;
        case RESISTOR_PULLDOWN:
            return FALSE;                       // TODO: Enable pull down resistor instead of failing
    }
    
    PIN_ISR_DESCRIPTOR& PinIsr = g_LPC24XX_GPIO_Driver.m_PinIsr[pin];

    PinIsr.m_intEdge = intEdge;
    PinIsr.m_isr     = (ISR != NULL)?ISR:STUB_ISRVector;
    PinIsr.m_param   = (Param != NULL)?Param:(void*)(size_t)pin;
    PinIsr.m_flags   = GlitchFilterEnable ? PIN_ISR_DESCRIPTOR::c_Flags_Debounce : 0;
    PinIsr.m_status  = 0;                        
    PinIsr.m_completion.Abort();
    PinIsr.m_completion.Initialize();

    GPIO.Regs[Port].FIODIR_PX &= ~(0x1 << Bit);
    
    if(ISR)
    {
        
        //TODO: IMPLEMENT BothEdges triggering
        LPC24XX_GPIOIRQ& GPIOIRQ = LPC24XX::GPIOIRQ();
        if(Port == 0)
        {
            switch(intEdge)
            {
                case GPIO_INT_EDGE_HIGH:
                    GPIOIRQ.IO0IntEnR |= 0x1 << Bit;
                break;
                
                case GPIO_INT_EDGE_LOW:
                    GPIOIRQ.IO0IntEnF |= 0x1 << Bit;
                break;
                default:
                    //TinyCLR will generate an exception in response to this
                    return FALSE;
            }
        }
        else    //Port can only be 2 now (we return for other values)
        {
            switch(intEdge)
            {
                case GPIO_INT_EDGE_HIGH:
                    GPIOIRQ.IO2IntEnR |= 0x1 << Bit;
                break;
                
                case GPIO_INT_EDGE_LOW:
                    GPIOIRQ.IO2IntEnF |= 0x1 << Bit;
                break;
                
                case GPIO_INT_EDGE_BOTH:
                    GPIOIRQ.IO2IntEnR |= 0x1 << Bit;
                    GPIOIRQ.IO2IntEnF |= 0x1 << Bit;
                break;
                
                default:
                    return FALSE;
            }
        }
    
    }
    
    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
//  GetPinState
///////////////////////////////////////////////////////////////////////////////

BOOL LPC24XX_GPIO_Driver::GetPinState( GPIO_PIN pin )
{
    ASSERT(pin < c_MaxPins);
    
    UINT32 Port, Bit;

//    hal_printf( "GetPinState: %08x\r\n", pin);
    LPC24XX_GPIO& GPIO = LPC24XX::GPIO();

    Port = PinToPort(pin);
    Bit = PinToBit(pin);

    return(((GPIO.Regs[Port].FIOPIN_PX) >> Bit) & 0x1);
}


///////////////////////////////////////////////////////////////////////////////
//  SetPinState
///////////////////////////////////////////////////////////////////////////////
void LPC24XX_GPIO_Driver::SetPinState( GPIO_PIN pin, BOOL pinState )
{

    ASSERT(pin < c_MaxPins);

    UINT32 Port, Bit;

    LPC24XX_GPIO& GPIO = LPC24XX::GPIO();
//hal_printf( "SetPinState: %08x\r\n", pin);
    GLOBAL_LOCK(irq);

    Port = PinToPort(pin);
    Bit = PinToBit(pin);

    if(pinState) GPIO.Regs[Port].FIOSET_PX |=  (0x1 << Bit);
    else         GPIO.Regs[Port].FIOCLR_PX |=  (0x1 << Bit);
}


///////////////////////////////////////////////////////////////////////////////
//  PinIsBusy
///////////////////////////////////////////////////////////////////////////////
BOOL LPC24XX_GPIO_Driver::PinIsBusy( GPIO_PIN pin )
{
    return g_LPC24XX_GPIO_Driver.m_PinReservationInfo[pin];
}

///////////////////////////////////////////////////////////////////////////////
//  ReservePin
///////////////////////////////////////////////////////////////////////////////
BOOL LPC24XX_GPIO_Driver::ReservePin( GPIO_PIN pin, BOOL fReserve )
{
    GLOBAL_LOCK(irq);

    g_LPC24XX_GPIO_Driver.m_PinReservationInfo[pin] = fReserve;

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//  GetDebounce
///////////////////////////////////////////////////////////////////////////////
UINT32 LPC24XX_GPIO_Driver::GetDebounce()
{

    return CPU_TicksToTime( g_LPC24XX_GPIO_Driver.m_DebounceTicks ) / 10000;
}

///////////////////////////////////////////////////////////////////////////////
//  SetDebounce
///////////////////////////////////////////////////////////////////////////////
BOOL LPC24XX_GPIO_Driver::SetDebounce( INT64 debounceTimeMilliseconds )
{   

    if(debounceTimeMilliseconds < c_MinDebounceTimeMs || c_MaxDebounceTimeMs < debounceTimeMilliseconds )
    {
        return FALSE;
    }

    g_LPC24XX_GPIO_Driver.m_DebounceTicks = CPU_MillisecondsToTicks( (UINT32)debounceTimeMilliseconds );

    return TRUE;
}

INT32 LPC24XX_GPIO_Driver::GetPinCount()
{
    return c_MaxPins;
}

void LPC24XX_GPIO_Driver::GetPinsMap( UINT8* pins, size_t size )
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

UINT8 LPC24XX_GPIO_Driver::GetSupportedResistorModes( GPIO_PIN pin)
{
    return LPC24XX_GPIO_Driver::c_GPIO_ResistorMode;
}


UINT8 LPC24XX_GPIO_Driver::GetSupportedInterruptModes( GPIO_PIN pin)
{

    return LPC24XX_GPIO_Driver::c_GPIO_InterruptMode;
}

void LPC24XX_GPIO_Driver::STUB_ISRVector( GPIO_PIN Pin, BOOL PinState, void* Param )
{
    lcd_printf( "\fSTUB_GPI %04x\r\n", (size_t)Param );
    //HARD_BREAKPOINT();
}

void LPC24XX_GPIO_Driver::GPIO_ISR( void* Param )
{
    //manage all flagged pin IRQs on port 0
    LPC24XX_GPIOIRQ& GPIOIRQ = LPC24XX::GPIOIRQ();
    LPC24XX_GPIO& GPIO = LPC24XX::GPIO();
    
    UINT32 bit;
    PIN_ISR_DESCRIPTOR* pinIsr = NULL;
    
    for(bit = 0; (GPIOIRQ.IOIntStatus & 0x1) && (bit < 32); bit++) //exit when there are no pending IRQs or we reach P0.31 (should always be the first)
    {
        if(!(GPIOIRQ.IO0IntStatR & 0x1<<bit)&&!(GPIOIRQ.IO0IntStatF & 0x1<<bit)) //this is not the pin that detected an edge
            continue;
        
        //clear this pin's IRQ
        GPIOIRQ.IO0IntClr |= 0x1 << bit;
        
        pinIsr = &g_LPC24XX_GPIO_Driver.m_PinIsr[ bit /*+ 0 * LPC24XX_GPIO_Driver::c_PinsPerPort*/ ];
        
        //Debounce
        if( (pinIsr->m_flags & PIN_ISR_DESCRIPTOR::c_Flags_Debounce)&&
            (Time_CurrentTicks() - pinIsr->m_lastExecTicks < g_LPC24XX_GPIO_Driver.m_DebounceTicks))
            continue; //ignore this request
        
        //call this pin's ISR
            
        pinIsr->m_status = ((GPIO.Regs[0].FIOPIN_PX) >> bit) & 0x1;
        pinIsr->m_lastExecTicks = Time_CurrentTicks();
        pinIsr->Fire((void*)pinIsr);

        //clear this pin's IRQ
        GPIOIRQ.IO0IntClr |= 0x1 << bit;
    }
    
    //manage all flagged pin IRQs on port 2
    for(bit = 0; (GPIOIRQ.IOIntStatus & 0x4) && (bit < 32); bit++) // as above, but the guard value is P2.31
    {
        if(!(GPIOIRQ.IO2IntStatR & 0x1<<bit)&&!(GPIOIRQ.IO2IntStatF & 0x1<<bit)) //this is not the pin that detected an edge
            continue;
        
        //clear this pin's IRQ
        GPIOIRQ.IO2IntClr |= 0x1 << bit;
        
        pinIsr = &g_LPC24XX_GPIO_Driver.m_PinIsr[ bit + 2 * LPC24XX_GPIO_Driver::c_PinsPerPort ];
        
        //Debounce
        if( (pinIsr->m_flags & PIN_ISR_DESCRIPTOR::c_Flags_Debounce)&&
            (Time_CurrentTicks() - pinIsr->m_lastExecTicks < g_LPC24XX_GPIO_Driver.m_DebounceTicks))
            {
                //debug_printf(" ignoring.\r\n");
                continue; //ignore this request
            }
        
        //call this pin's ISR
        pinIsr->m_status = ((GPIO.Regs[2].FIOPIN_PX) >> bit) & 0x1;
        pinIsr->m_lastExecTicks = Time_CurrentTicks();
        pinIsr->Fire((void*)pinIsr);

    }
    
    
    
}

void LPC24XX_GPIO_Driver::PIN_ISR_DESCRIPTOR::Fire( void* arg )
{
    PIN_ISR_DESCRIPTOR* desc = (PIN_ISR_DESCRIPTOR*)arg;
    
    //debug_printf("Servicing GPIO. m_pin=%u, m_status=%u\r\n",desc->m_pin,desc->m_status);
    desc->m_isr( desc->m_pin, desc->m_status, desc->m_param );

}
