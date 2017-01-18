////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

/*
#define PXA271_EP_COUNT                 23
#define CPU_USB_DEFAULT_PACKET_SIZE     32
#define CPU_USB_BULK_PACKET_SIZE        64
#define CPU_USB_MAX_TX_RETRY            3
*/

// the v3.1 arm compiler optimizations for RTM cause the USB not to work
#if !defined(DEBUG)
#pragma O1
#endif

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PXA271_USB_Driver"
#endif

PXA271_USB_Driver g_PXA271_USB_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "UsbBusState"
#endif

USB_CONTROLLER_STATE UsbControllerState[1];     // Only 1 USB Controller for this device

Hal_Queue_KnownSize<USB_PACKET64,USB_QUEUE_PACKET_COUNT> QueueBuffers[PXA271_USB_Driver::c_Used_Endpoints-1];

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//
  //#if defined(DRIVER_PXA271_USB)
#if 0
#if defined(BUILD_RTM)
inline void USB_debug_printf( const char*format, ... ) {}
#else
void USB_debug_printf( const char* format, ... )
{
    char    buffer[256];
    va_list arg_ptr;

    va_start( arg_ptr, format );

    int len = hal_vsnprintf( buffer, sizeof(buffer)-1, format, arg_ptr );
                               
    // flush existing characters
    DebuggerPort_Flush( USART_DEFAULT_PORT );

    // write string
    DebuggerPort_Write( USART_DEFAULT_PORT, buffer, len );

    // flush new characters
    DebuggerPort_Flush( USART_DEFAULT_PORT );

    va_end( arg_ptr );
}


//--//

USB_SETUP_PACKET RequestPacket = {0,0,0,0,0};

/***************************************************************************/

void DisplayState()
{
    PXA271_USB& USB = PXA271::USB();
    
    USB_debug_printf( "current state %x \r\n"           , USB.UDCCR );
    USB_debug_printf( "Int state %x:%x , mask %x:%x\r\n", USB.UDCISR1, USB.UDCISR0, USB.UDCICR1, USB.UDCICR0 );

    for (int i=0; i<=23;i++)
    {
        USB_debug_printf( " EP %d - stat %x \r\n", i, USB.UDCCSR[i] );
    }
}

#endif
#endif
/***************************************************************************/

static PXA271_USB::EndpointConfiguration EndpointInit[PXA271_USB_Driver::c_Used_Endpoints];     // Corresponds to endpoint configuration RAM at PXA271_USB::UDCCRx

//--//

USB_CONTROLLER_STATE * PXA271_USB_Driver::GetState( int Controller )
{
    if( Controller != 0 )       // There is only one controller for this device
        return NULL;
    return &UsbControllerState[0];
}

HRESULT PXA271_USB_Driver::Initialize( int Controller )
{
    int endpointsUsed = 0;
    const USB_ENDPOINT_DESCRIPTOR  *ep    = NULL;
    const USB_INTERFACE_DESCRIPTOR *itfc  = NULL;
    USB_CONTROLLER_STATE &State = UsbControllerState[0];

    ASSERT( Controller == 0 );

    GLOBAL_LOCK(irq);

    PXA271_USB& USB = PXA271::USB();

    // make sure clock is enabled
    PXA271::CLKMNGR().CKEN |= PXA271_CLOCK_MANAGER::CKEN__USB_CLIENT_48MHZ;

    // Enable the only interrupt
    CPU_INTC_ActivateInterrupt( PXA271_AITC::c_IRQ_INDEX_USB_CLIENT, Global_ISR, NULL );

    for( int i = 0; i < c_Used_Endpoints; i++ )
        EndpointInit[i].word = 0;       // All useable endpoints initialize to unused

    // For all endpoints in the USB configuration
    while( USB_NextEndpoint( &State, ep, itfc) )
    {
        UINT8 endpointNum   = ep->bEndpointAddress & 0x7F;
        UINT16 endpointSize = ep->wMaxPacketSize;
        
        // Check interface and endpoint numbers against hardware capability
        if( endpointNum >= c_Used_Endpoints || itfc->bInterfaceNumber > 7 )
            return S_FALSE;
        
        EndpointInit[endpointNum].bits.EN   = endpointNum;
        EndpointInit[endpointNum].bits.ED   = (ep->bEndpointAddress & 0x80) ? 1 : 0;
        EndpointInit[endpointNum].bits.IN   = itfc->bInterfaceNumber;
        EndpointInit[endpointNum].bits.ET   = ep->bmAttributes & 0x03;
        EndpointInit[endpointNum].bits.CN   = 1;        // Always only 1 configuration = 1
        EndpointInit[endpointNum].bits.AISN = 0;        // No alternate interfaces
        EndpointInit[endpointNum].bits.EE   = 1;        // Enable this endpoint

        // Set the maximum size of the endpoint hardware FIFO
        if( (ep->bmAttributes & 0x03) == USB_ENDPOINT_ATTRIBUTE_BULK )
        {
            // If the endpoint maximum size in the configuration list is bogus
            if( endpointSize != 8 && endpointSize != 16 && endpointSize != 32 && endpointSize != 64 )
                return S_FALSE;
            EndpointInit[endpointNum].bits.MPS = endpointSize;
            State.MaxPacketSize[endpointNum]   = endpointSize;
        }
        else if( (ep->bmAttributes & 0x03) == USB_ENDPOINT_ATTRIBUTE_INTERRUPT )
        {
            if( endpointSize == 0 || endpointSize > 64 )
                return S_FALSE;
            EndpointInit[endpointNum].bits.MPS = endpointSize;
            State.MaxPacketSize[endpointNum]   = endpointSize;
        }
        else        // Isochronous endpoint
        {
            if( endpointSize > 64 )
                endpointSize = 64;
            EndpointInit[endpointNum].bits.MPS = endpointSize;
            State.MaxPacketSize[endpointNum]   = endpointSize;
        }

        // Since endpoint 0 is only used for control, there is never a need to allocate a buffer for it
        // In fact State.Queues[0] is always NULL - it is a cheap placeholder to make the queueIndex = endpointIndex
        QueueBuffers[endpointNum-1].Initialize();                       // Clear queue before use
        State.Queues[endpointNum] = &QueueBuffers[endpointNum-1];       // Attach queue to endpoint

        // Set up direction information
        if( EndpointInit[endpointNum].bits.ED )         // If transmit endpoint
        {
            EndpointInit[endpointNum].bits.DE = 1;      // Only transmit is double buffered
            State.IsTxQueue[endpointNum]      = TRUE;
        }
        else        // If receive endpoint
        {
            EndpointInit[endpointNum].bits.DE = 0;
            State.IsTxQueue[endpointNum]      = FALSE;
        }
        endpointsUsed++;
    }
    
    // If no endpoints were initialized, something is seriously wrong with the configuration list
    if( 0 == endpointsUsed )
    {
        CPU_INTC_DeactivateInterrupt( PXA271_AITC::c_IRQ_INDEX_USB_CLIENT );
        return S_FALSE;
    }

    g_PXA271_USB_Driver.pUsbControllerState  = &State;
    g_PXA271_USB_Driver.PinsProtected        = TRUE;

    State.EndpointStatus = &g_PXA271_USB_Driver.EndpointStatus[0];
    State.EndpointCount  = c_Used_Endpoints;
    //State->DeviceStatus   = USB_STATUS_DEVICE_SELF_POWERED;
    State.PacketSize     = c_default_ctrl_packet_size;
    
    State.FirstGetDescriptor = TRUE;

    ProtectPins( Controller, FALSE );

    return S_OK;
}

HRESULT PXA271_USB_Driver::Uninitialize( int Controller )
{
    ASSERT( 0 == Controller );
    
    GLOBAL_LOCK(irq);

    ProtectPins( Controller, TRUE );

    g_PXA271_USB_Driver.pUsbControllerState = NULL;

    CPU_INTC_DeactivateInterrupt( PXA271_AITC::c_IRQ_INDEX_USB_CLIENT );

    // Deactivate the clock for power savings
    PXA271::CLKMNGR().CKEN &= ~PXA271_CLOCK_MANAGER::CKEN__USB_CLIENT_48MHZ;

    return S_OK;
}

BOOL PXA271_USB_Driver::StartOutput( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT( State );
    ASSERT(endpoint < c_Used_Endpoints);

    GLOBAL_LOCK(irq);

    // If endpoint is not an output
    if( State->Queues[endpoint] == NULL || !State->IsTxQueue[endpoint] )
        return FALSE;

    /* if the halt feature for this endpoint is set, then just
       clear all the characters */
    if(State->EndpointStatus[endpoint] & USB_STATUS_ENDPOINT_HALT)
    {
        ClearTxQueue( State, endpoint );
        return TRUE;
    }
    

    //If TxRunning, interrupts will drain the queue
    if(!g_PXA271_USB_Driver.TxRunning[endpoint])
    {
        g_PXA271_USB_Driver.TxRunning[endpoint] = TRUE;

        // Calling both TxPacket & EP_TxISR in this routine could cause a TX FIFO overflow
        TxPacket( State, endpoint );
    }
    else if(irq.WasDisabled())
    {                        
        PXA271_USB& USB = PXA271::USB();
        
        // This could be called during Flush with all interrupts off.  Just taking care of the endpoint
        // in question may cause a logjam if the host is expecting a response from another endpoint.
        // All endpoints must be examined for activity.
        if( USB.UDCISR0 & USB.UDCICR0 & USB.UDCICR__BOTH )        // If endpoint 0 needs attention
            EP0_ISR( 0 );
        for( int ep = 1; ep < c_Used_Endpoints; ep++ )
        {
            // If no interrupt for this endpoint
            if( (((USB.UDCISR0 & USB.UDCICR0) >> (ep * 2)) & USB.UDCICR__BOTH) == 0 )
                continue;
            
            if(State->Queues[ep])
            {
                if( State->IsTxQueue[endpoint] )
                    EP_TxISR( endpoint );
                else
                    EP_RxISR( endpoint );
            }
        }
    }

    return TRUE;
}

BOOL PXA271_USB_Driver::GetInterruptState()
{
    PXA271_USB& USB = PXA271::USB();

    GLOBAL_LOCK(irq);

    if((USB.UDCISR0 & USB.UDCICR0) || (USB.UDCISR1 & USB.UDCICR1)) 
    {
        return TRUE;
    }
    
    return FALSE;
}

//--//

void PXA271_USB_Driver::ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT(State);

    State->Queues[endpoint]->Initialize();
}

//--//
void PXA271_USB_Driver::StartHardware()
{    
    // clk and the PLL is already enabled at bootup time.
    // since it is always attached to PC - no PC presence check

    PXA271_USB& USB   = PXA271::USB();

    // Perform a hardware reset for the USB core.  This is done by disabling the UDC.  This resets all the FIFOs,
    // Disables the USBC_N and USBC_P pins and allows the configuration to be rewritten.
    USB.UDCCR = 0;

    // This is needed, especially when going from PortBooter to the main program.  The last USB module
    // could still be active and in some unknown state.
    USB.UDCCR = USB.UDCCR__EMCE;        // Disable USB port and clear any memory error that may exist

    // Disable "On The Go" and other unused features of the USB port
    USB.UDCOTGICR = 0;                                      // USB port 1 "On The Go" features are disabled
    USB.UP2OCR    = USB.UP2OCR__HXOE;                       // Set up differential port for UDC control
    USB.UP3OCR    = USB.UP3OCR__CFG_NU;                     // USB port 3 is not used
    
    // Write the configuration registers for all Endpoints (Except endpoint 0 which is hard-coded to have a 16 byte input and 16 byte output buffer).
    // This sets up their logical endpoint numbers, how much of the 4K of buffer memory is allocated to each FIFO, etc.
    for(int i = 1; i < c_Used_Endpoints; i++)
    {
        USB.UDCCRx[i] = EndpointInit[i].word;
    }

    // Disable the rest of the endpoints
    for(int i = c_Used_Endpoints; i < USB.c_MaxEndpoints; i++)
        USB.UDCCRx[i] = 0;
    
    // clear any pending interrupts
    USB.UDCISR0 = USB.UDCISR0;
    USB.UDCISR1 = USB.UDCISR1;          // Interrupts are cleared by writing a 1 to them

    //////////////////////////////////////////////////////////////////////////////
    // program USB Interrupt Mask, enable reset, suspend and configure change

    USB.UDCICR0 = 0;
    USB.UDCICR1 = USB.UDCICR1__IERS | USB.UDCICR1__IESU | USB.UDCICR1__IECC | USB.UDCICR1__IERU;

    // Enable Endpoint interrupts
    USB.Set_Interrupt( 0, USB.UDCICR__BOTH );           // Always enable endpoint 0
    for(int ep = 1; ep < c_Used_Endpoints; ep++)
    {
        if( EndpointInit[ep].bits.EE )                  // If this endpoint is enabled
            USB.Set_Interrupt( ep, USB.UDCICR__BOTH );  // Enable both packet complete and FIFO error interrupts
    }

    // Enable USB - the USB port should be able to respond after this
    USB.UDCCR = USB.UDCCR__UDE;

    ASSERT((USB.UDCCR & USB.UDCCR__EMCE) == 0);     // The configuration above must cause no memory problems
    
    // Signal the host that the USB port is available
    CPU_GPIO_SetPinState(PXA271_USB::c_USBC_GPIOX_EN, TRUE);

}

void PXA271_USB_Driver::StopHardware()
{
    PXA271_USB & USB = PXA271::USB();
    
    // turn off the UDC core
    USB.UDCCR = 0;
}

void PXA271_USB_Driver::TxPacket( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT( endpoint < c_Used_Endpoints );    
    ASSERT( State );

    PXA271_USB& USB = PXA271::USB();
    
    GLOBAL_LOCK(irq); 

    // transmit a packet on UsbPortNum, if there are no more packets to transmit, then die    
    USB_PACKET64* Packet64;


    // If this is not a legal transmit endpoint, there is nothing to do
    if( State->Queues[endpoint] == NULL || !State->IsTxQueue[endpoint] )
        return;

    for(;;)
    {
        Packet64 = USB_TxDequeue( State, endpoint, TRUE );

        if( Packet64 == NULL || Packet64->Size > 0 )
            break;
    }
    
    if( Packet64 )
    {
        if(USB.UDCCSRx[endpoint] & USB.UDCCSR__PC)              // If a packet has been sent
            USB.UDCCSRx[endpoint] = USB.UDCCSR__PC;             // Clear the Packet Complete bit
        
        ASSERT((USB.UDCCSRx[endpoint] & USB.UDCCSR__FS) != 0);  // We should absolutely have an empty buffer to use

        UINT32* packet4 = (UINT32*)Packet64->Buffer;            // FIFO may only be loaded with full words
        int nLeft = Packet64->Size;
        
        while( nLeft >= 4 )
        {
            USB.UDCDRx[endpoint] = *packet4++;                  // Shove packet into FIFO 4 bytes at a time
            nLeft -= 4;
        }

        if( Packet64->Size < State->MaxPacketSize[endpoint] )   // If packet is not the full size
        {
            UINT16* packet2 = (UINT16*)packet4;
            
            if( nLeft >= 2 )
            {
                *(volatile UINT16*)&USB.UDCDRx[endpoint] = *packet2++;
                nLeft -= 2;
            }
            if( nLeft > 0)
            {
                *(volatile UINT8*)&USB.UDCDRx[endpoint] = *(UINT8*)packet2;
            }
            USB.UDCCSRx[endpoint] = USB.UDCCSR__SP;             // Send a short packet (full length packets are automatically sent)
        }

        g_PXA271_USB_Driver.TxNeedZLPS[endpoint] = (Packet64->Size == State->MaxPacketSize[endpoint]);
    }
    else
    {
        // send the zero length packet since we landed on the FIFO boundary before
        // (and we queued a zero length packet to transmit) 
        if(g_PXA271_USB_Driver.TxNeedZLPS[endpoint])
        {
            if(USB.UDCCSRx[endpoint] & USB.UDCCSR__PC)              // If a packet has been sent
                USB.UDCCSRx[endpoint] = USB.UDCCSR__PC;             // Clear the Packet Complete bit
            
            ASSERT((USB.UDCCSRx[endpoint] & USB.UDCCSR__FS) != 0);  // We should absolutely have an empty buffer to use

            USB.UDCCSRx[endpoint] = USB.UDCCSR__SP;                 // Send a short (zero length) packet
            g_PXA271_USB_Driver.TxNeedZLPS[endpoint] = FALSE;
        }

        // no more data
        g_PXA271_USB_Driver.TxRunning[endpoint] = FALSE;
    }
}

void PXA271_USB_Driver::ControlNext()
{
    ASSERT(g_PXA271_USB_Driver.pUsbControllerState);
    PXA271_USB& USB = PXA271::USB();
    USB_CONTROLLER_STATE *State = g_PXA271_USB_Driver.pUsbControllerState;

    if( State->DataCallback )
    {
        /* this call can't fail */
        State->DataCallback( State );


        if( State->DataSize == 0 )
        {
            USB.UDCCSRx[0] = PXA271_USB::UDCCSR__IPR;           // Send zero length packet
            State->DataCallback = NULL;                         // Stop sending stuff if we're done
        }
        else
        {
            UINT32* packet4 = (UINT32*)State->Data;             // FIFO may only be loaded with full words
            int     nLeft   = State->DataSize;
            while( nLeft >= 4 )
            {
                USB.UDCDRx[0] = *packet4++;                     // Shove packet into FIFO 4 bytes at a time
                nLeft -= 4;
            }

            if(State->DataSize < c_default_ctrl_packet_size)    // If packet is less than full length
            {
                UINT16* packet2 = (UINT16*)packet4;
                
                if( nLeft >= 2 )
                {
                    *(volatile UINT16*)&USB.UDCDRx[0] = *packet2++;
                    nLeft -= 2;
                }
                if( nLeft > 0)
                {
                    *(volatile UINT8*)&USB.UDCDRx[0] = *(UINT8*)packet2;
                }
                USB.UDCCSRx[0] = USB.UDCCSR__IPR;               // Send a short packet (full length packets are automatically sent)
                State->DataCallback = NULL;                     // Stop sending stuff if we're done
            }


            // special handling the USB driver set address test, cannot use the first descriptor as the ADDRESS state is handle in the hardware
            if(g_PXA271_USB_Driver.FirstDescriptorPacket)
            {
                State->DataCallback = NULL;
            }

       }
    }

}

void PXA271_USB_Driver::Global_ISR( void* Param )
{
    PXA271_USB& USB       = PXA271::USB();
    UINT32      USB_INTR1 = (USB.UDCISR1 & USB.UDCICR1);
    USB_CONTROLLER_STATE *State = g_PXA271_USB_Driver.pUsbControllerState;

    if( NULL == State )
        return;
    
//    PXA271_USB_Failure_Assert("inside ISR ", 488);

    if( (USB_INTR1 & (USB.UDCISR1__IRCC | USB.UDCISR1__IRRS | USB.UDCISR1__IRRU | USB.UDCISR1__IRSU)) == 0 )      // If an Endpoint interrupt
    {
        UINT32 USB_INTR0 = (USB.UDCISR0 & USB.UDCICR0);
        UINT32 mask      = USB.UDCICR__BOTH;
        UINT32 endpoint  = 0;

        while(endpoint < c_Used_Endpoints)
        {
            if(USB_INTR0 & mask)         // If endpoint found
                break;
            endpoint++;
            mask <<= 2;
        }

        if( endpoint == c_Used_Endpoints )      // If interrupt is not from one of the used endpoints (shouldn't happen!)
        {
            USB.UDCISR0 = USB_INTR0;
            USB.UDCISR1 = USB_INTR1;            // Clear the interrupt
            ASSERT(0);
        }
            
        if( endpoint == 0 )             // If Endpoint 0 interrupt
        {
            EP0_ISR( 0 );
            return;
        }
        // Only if the queue has been set up for this endpoint (should always be the case)
        if( State->Queues[endpoint] )
        {
            if( State->IsTxQueue[endpoint] )      // If this is an IN or transmit endpoint
            {
                EP_TxISR( endpoint );
                return;
            }
            EP_RxISR( endpoint );    // Must be an OUT or receive endpoint
        }
        return;

    }

    if (USB_INTR1 & PXA271_USB::UDCISR1__IRCC)      // If configuration change interrupt
    {
        USB.UDCISR1 = USB.UDCISR1__IRCC;            // Clear the interrupt

        // Setup the Configure packet
        RequestPacket.bmRequestType = 0;
        RequestPacket.bRequest      = USB_SET_CONFIGURATION;
        RequestPacket.wValue        = (USB.UDCCR & PXA271_USB::UDCCR__ACN_mask) >> PXA271_USB::UDCCR__ACN_shift;
        RequestPacket.wIndex        = 0;
        RequestPacket.wLength       = 0;
        
        // when get CFG_CHG, it should already be in Address state
        State->DeviceState = USB_DEVICE_STATE_ADDRESS;

        if(RequestPacket.wValue == 1)                           // If this is a configuration we can handle
            USB.UDCCR = USB.UDCCR__SMAC | USB.UDCCR__UDE;       // Configure FIFOs to new memory layout

        USB_HandleSetConfiguration(State, &RequestPacket, FALSE);
        /* USB spec 9.4.5 SET_CONFIGURATION resets halt conditions, resets toggle bits */
        // it is handled by the hardware and the toggle bit is handle by hardware

        // Start all used transmit endpoints
        for( int ep = 0; ep < c_Used_Endpoints; ep++ )
        {
            if( State->Queues[ep] && State->IsTxQueue[ep] )
                StartOutput( State, ep );
        }
        
    }
        
    if (USB_INTR1 &  PXA271_USB::UDCISR1__IRRU)     // If Resume interrupt
    {
        USB.UDCISR1 = USB.UDCISR1__IRRU;            // Clear the interrupt
        ResumeEvent();
    }

    if (USB_INTR1 &  PXA271_USB::UDCISR1__IRRS)     // If Reset interrupt
    {
        USB.UDCISR1 = USB.UDCISR1__IRRS;            // Clear the interrupt
        ResetEvent();
    }

    if (USB_INTR1 &  PXA271_USB::UDCISR1__IRSU)     // If Suspend interrupt
    {
        USB.UDCISR1 = USB.UDCISR1__IRSU;            // Clear the interrupt
        SuspendEvent();
    }
        
    // the following interrupts are not enabled. If triggered =>ERR0R
    
    if (USB_INTR1 & PXA271_USB::UDCISR1__IRSOF)     // Start of frame interrupt
    {
        USB.UDCISR1 = USB.UDCISR1__IRSOF;           // Clear the interrupt
        ASSERT(0);
    }    

}


void PXA271_USB_Driver::EP0_ISR( UINT32 Param )
{   
    UINT32 EP_INTR;

    PXA271_USB& USB = PXA271::USB();

    EP_INTR = USB.UDCISR0 & 3;      // The endpoint 0 interrupt state

    // set up packet receive
    if((EP_INTR & PXA271_USB::UDCICR__PCKT) && (USB.UDCCSRx[0] & PXA271_USB::UDCCSR__OPC))
    {
        USB_CONTROLLER_STATE *State = g_PXA271_USB_Driver.pUsbControllerState;
        
        // read data 

        UINT8   len          = USB.UDCBCRx[0] & PXA271_USB::UDCBCR_mask;        // Received packet length
        UINT32* packetBuffer = (UINT32*)g_PXA271_USB_Driver.ControlPacketBuffer;

        for(int nWords = (len + 3)/4; nWords; nWords--)
            *packetBuffer++ = USB.UDCDRx[0];                // OK to read in too much junk, len takes care of it & FIFO MUST be read as words
        
        // clear interrupt & status
        USB.UDCISR0    = USB.UDCICR__PCKT;
        USB.UDCCSRx[0] = USB.UDCCSRx[0];                    // Allow another packet to be sent (reset all set bits)

        // special handling for the very first SETUP command - Getdescriptor[DeviceType], the host looks for 8 bytes data only
        USB_SETUP_PACKET* Setup = (USB_SETUP_PACKET* )&g_PXA271_USB_Driver.ControlPacketBuffer[0];
        if((Setup->bRequest == USB_GET_DESCRIPTOR) && (((Setup->wValue & 0xFF00) >> 8) == USB_DEVICE_DESCRIPTOR_TYPE) && (Setup->wLength != 0x12))
            g_PXA271_USB_Driver.FirstDescriptorPacket = TRUE;
        else
            g_PXA271_USB_Driver.FirstDescriptorPacket = FALSE;

        /* send it to the upper layer */
        State->Data     = &g_PXA271_USB_Driver.ControlPacketBuffer[0];
        State->DataSize = len;

        UINT8 result = USB_ControlCallback( State );

        switch(result)
        {
        case USB_STATE_DATA:
            /* setup packet was handled and the upper layer has data to send */
            break;

        case USB_STATE_ADDRESS:
            /* upper layer needs us to change the address */
            // address stage handles in hardware
            break;

        case USB_STATE_DONE:
            State->DataCallback = NULL;
            break;

        case USB_STATE_STALL:
            // since the setup command all handled in the hardware, should not have this state 
            //
            // setup packet failed to process successfully
            // set stall condition on the default control
            // endpoint
            //
            break;

        case USB_STATE_STATUS:
            // handle by hardware            
            break;
        case USB_STATE_CONFIGURATION:
            // handle partly by hardware and the GLOBAL_ISR will take care.
            /* USB spec 9.4.5 SET_CONFIGURATION resets halt conditions, resets toggle bits */
            break;

        case USB_STATE_REMOTE_WAKEUP:
            // It is not using currently as the device side won't go into SUSPEND mode unless
            // the PC is purposely to select it to SUSPEND, as there is always SOF in the bus
            // to keeping the device from SUSPEND.
            break;

        default:
            ASSERT(0);
            break;            
            // the status change is only seen and taken care in hardware
        }
        if(result != USB_STATE_STALL)
        {
            ControlNext();

            // If the port is configured, then output any possible withheld data
            if( result == USB_STATE_CONFIGURATION )
            {
                for( int ep = 1; ep < c_Used_Endpoints; ep++ )
                {
                    if( State->Queues[ep] && State->IsTxQueue[ep] )
                        StartOutput( State, ep );
                }
            }
        }
    }
    else if ((EP_INTR & PXA271_USB::UDCICR__PCKT) && (USB.UDCCSRx[0] & (PXA271_USB::UDCCSR__IPR | PXA271_USB::UDCCSR__SST)) == 0)
    {
        // If previous packet has been sent and UDC is ready for more
        USB.UDCISR0    = USB.UDCICR__PCKT;              // Clear the interrupt
        ControlNext();      // See if there is more to send
    }
    else if (EP_INTR & PXA271_USB::UDCICR__PCKT)        // If some other error condition issued
    {
        USB.UDCISR0 = USB.UDCICR__PCKT;                 // Clear the interrupt
        USB.UDCCSRx[0] = USB.UDCCSRx[0];                // Clear the condition
    }

    if (EP_INTR & PXA271_USB::UDCICR__FIFO_ERR)     // If FIFO error
    {
        USB.UDCISR0 = PXA271_USB::UDCICR__FIFO_ERR;
        // This should never happen
        USB_debug_printf("FIFO status %X \r\n", USB.UDCCSRx[0]);
        ASSERT(0);
    }  
}

void PXA271_USB_Driver::EP_TxISR( UINT32 endpoint )
{
    ASSERT_IRQ_MUST_BE_OFF();

    UINT32 EP_INTR;
    
    PXA271_USB& USB = PXA271::USB();

    EP_INTR = (USB.UDCISR0 >> (endpoint * 2)) & USB.UDCICR__BOTH;       // Keep both interrupt bits

    if  ((EP_INTR & PXA271_USB::UDCICR__PCKT) && (USB.UDCCSRx[endpoint] & USB.UDCCSR__FS))      // If another packet may be sent
    {
        USB.UDCISR0 = USB.UDCICR__PCKT << (endpoint * 2);       // Clear the packet interrupt

        // successfully transmitted packet, time to send the next one 
        TxPacket( g_PXA271_USB_Driver.pUsbControllerState, endpoint );

    }
    else if( EP_INTR & PXA271_USB::UDCICR__PCKT )
        USB.UDCISR0 = USB.UDCICR__PCKT << (endpoint * 2);       // Clear the packet interrupt

    if (EP_INTR & PXA271_USB::UDCICR__FIFO_ERR)
    {
        USB.UDCISR0 = USB.UDCICR__FIFO_ERR << (endpoint * 2);       // Clear the error interrupt
        // This should not happen
        USB_debug_printf("FIFO status %X \r\n", USB.UDCCSRx[endpoint]);
        ASSERT(0);
    }

}

void PXA271_USB_Driver::EP_RxISR( UINT32 endpoint )
{
    UINT32 EP_INTR;
    
    PXA271_USB& USB = PXA271::USB();

    EP_INTR = (USB.UDCISR0 >> (endpoint * 2)) & PXA271_USB::UDCICR__BOTH;

    if ((EP_INTR & PXA271_USB::UDCICR__PCKT) && (USB.UDCCSRx[endpoint] & PXA271_USB::UDCCSR__PC))
    {
        BOOL          DisableRx;
        USB_PACKET64* Packet64 = USB_RxEnqueue( g_PXA271_USB_Driver.pUsbControllerState, endpoint, DisableRx );

        USB.UDCISR0 = PXA271_USB::UDCICR__PCKT << (endpoint * 2);       // Clear the packet interrupt

        /* copy packet in, making sure that Packet64->Buffer is never overflowed */
        if( Packet64 )
        {
            UINT8   len          = USB.UDCBCRx[endpoint] & PXA271_USB::UDCBCR_mask;
            UINT32* packetBuffer = (UINT32*)Packet64->Buffer;

            for(int nWords = (len + 3)/4; nWords; nWords--)
                *packetBuffer++ = USB.UDCDRx[endpoint];         // OK to read in too much junk, len takes care of it & FIFO MUST be read as words
            
            // clear packet status
            USB.UDCCSRx[endpoint] = USB.UDCCSR__PC;             // Allow another packet to be received

            Packet64->Size = len;
        }
        else
        {
            /* flow control should absolutely protect us from ever
            getting here, so if we do, it is a bug */
            USB.UDCCSRx[endpoint] = USB.UDCCSR__PC;             // Allow another packet to be received
            
            ASSERT(0);
        }
        if(DisableRx)
        {
            /* if there is no room left, then we mask out the receive event
            for this fifo */
            USB.Set_Interrupt(endpoint, PXA271_USB::UDCICR__NONE);
        }

    }
    else if ( EP_INTR & PXA271_USB::UDCICR__PCKT )
        USB.UDCISR0 = PXA271_USB::UDCICR__PCKT << (endpoint * 2);       // Clear the packet interrupt

    if(USB.UDCCSRx[endpoint] & (USB.UDCCSR__DPE | USB.UDCCSR__BNx | USB.UDCCSR__SST | USB.UDCCSR__TRN | USB.UDCCSR__FS))      // These bits should never be set
    {
        USB.UDCCSRx[endpoint] = USB.UDCCSR__SST | USB.UDCCSR__TRN | USB.UDCCSR__DPE;      // Clear all error bits
    }

    if (EP_INTR & PXA271_USB::UDCICR__FIFO_ERR)
    {
        USB.UDCISR0 = PXA271_USB::UDCICR__FIFO_ERR << (endpoint * 2);       // Clear the interrupt
        // This should not have happened
        USB_debug_printf("error:  status %x \r\n", USB.UDCCSRx[endpoint]);
        ASSERT(0);        
    }

}

void PXA271_USB_Driver::SuspendEvent()
{
    ASSERT(g_PXA271_USB_Driver.pUsbControllerState);

    PXA271_USB& USB = PXA271::USB();
    USB_CONTROLLER_STATE *State = g_PXA271_USB_Driver.pUsbControllerState;

    // SUSPEND event only happened when Host(PC) set the device to SUSPEND
    // as there is always SOF every 1ms on the BUS to keep the device from
    // suspending. Therefore, the REMOTE wake up is not necessary at the ollie side

    USB.UDCICR1 = PXA271_USB::UDCICR1__IECC | PXA271_USB::UDCICR1__IERS | PXA271_USB::UDCICR1__IERU | PXA271_USB::UDCICR1__IESU;

    g_PXA271_USB_Driver.PreviousDeviceState = State->DeviceState;

    State->DeviceState = USB_DEVICE_STATE_SUSPENDED;

    USB_StateCallback( State );

#if defined(DEBUG_USB)
    USB_debug_printf("In suspend event previous %d\n\r", g_PXA271_USB_Driver.PreviousDeviceState);
#endif

}


void PXA271_USB_Driver::ResumeEvent()
{
    ASSERT(g_PXA271_USB_Driver.pUsbControllerState);

    PXA271_USB& USB = PXA271::USB();
    USB_CONTROLLER_STATE *State = g_PXA271_USB_Driver.pUsbControllerState;

#if defined(DEBUG_USB)
    USB_debug_printf(" in Resume\r\n");
#endif

    USB.UDCICR1 = PXA271_USB::UDCICR1__IECC | PXA271_USB::UDCICR1__IERS | PXA271_USB::UDCICR1__IERU | PXA271_USB::UDCICR1__IESU;

    State->DeviceState = g_PXA271_USB_Driver.PreviousDeviceState;

    USB_StateCallback( State );
}



void PXA271_USB_Driver::ResetEvent()
{
    USB_CONTROLLER_STATE *State = g_PXA271_USB_Driver.pUsbControllerState;
    
    // The hardware handles most of the reset procedure.
    
    ASSERT( State );
    
    PXA271_USB& USB = PXA271::USB();

    // flush whatever is in the FIFO
    USB.UDCCSRx[0] = PXA271_USB::UDCCSR__FTF;                   // Flush endpoint 0 FIFO
    for(int ep = 1; ep < c_Used_Endpoints; ep++)
    {
        if( EndpointInit[ep].bits.EE )       // If this endpoint is enabled
        {
            USB.UDCCSRx[ep] = PXA271_USB::UDCCSR__FEF;
            while( USB.UDCCSRx[ep] & PXA271_USB::UDCCSR__FEF ) ;     // Wait for the FIFO to clear
        }
    }
    USB.UDCCSRx[0] = USB.UDCCSRx[0];        // Clear all EP0 status

    USB.UDCICR1 = PXA271_USB::UDCICR1__IECC | PXA271_USB::UDCICR1__IERS | PXA271_USB::UDCICR1__IERU | PXA271_USB::UDCICR1__IESU;
    USB.UDCISR0 = USB.UDCISR0;
    USB.UDCISR1 = USB.UDCISR1;              // Clear all pending interrupts

    /* clear all flags */
    USB_ClearEvent( 0, USB_EVENT_ALL);

#if defined(DEBUG_USB)
    USB_debug_printf(" in Reset\r\n");

    if( State->DeviceState >= USB_DEVICE_STATE_ADDRESS )
    {
        static int count = 0;
        USB_debug_printf("\fRESET: %d  \r\n", ++count);
    }
#endif

    for(int ep = 0; ep < c_Used_Endpoints; ep++)
    {
        g_PXA271_USB_Driver.TxRunning [ep] = FALSE;
        g_PXA271_USB_Driver.TxNeedZLPS[ep] = FALSE;
    }


    State->DeviceState        = USB_DEVICE_STATE_DEFAULT;
    State->Address            = 0;
    USB_StateCallback( State );
    
}

//--//

BOOL PXA271_USB_Driver::RxEnable( USB_CONTROLLER_STATE *State, int endpoint )
{
    PXA271_USB& USB = PXA271::USB();

    if( NULL == State || endpoint >= c_Used_Endpoints )
        return FALSE;
    
    GLOBAL_LOCK(irq);

    /* unmask receive event */
    USB.Set_Interrupt(endpoint, PXA271_USB::UDCICR__BOTH);      // Enable both interrupts for this endpoint
    
    return TRUE;
}

BOOL PXA271_USB_Driver::ProtectPins( int Controller, BOOL On )
{
    ASSERT( 0 == Controller );
    USB_CONTROLLER_STATE *State = g_PXA271_USB_Driver.pUsbControllerState;
    
    GLOBAL_LOCK(irq);

    // Initialized yet?
    if( State )
    {
        if( On )
        {
            if(!g_PXA271_USB_Driver.PinsProtected)
            {
                // Disable the USB com, state change from Not protected to Protected

                g_PXA271_USB_Driver.PinsProtected = TRUE;

                // clear USB Txbuffers
                for(int ep = 0; ep < c_Used_Endpoints; ep++)
                {
                    if( State->IsTxQueue[ep] )
                        ClearTxQueue( State, ep );
                }

                // stop clock, disable interrupt, dettach USB port
                // set the USBBus->DeviceState = DETACHED
                CPU_GPIO_EnableInputPin(PXA271_USB::c_USBC_GPIOX_EN, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED);     // Disable host signal driver
                State->DeviceState = USB_DEVICE_STATE_DETACHED;
                USB_StateCallback( State );

#if defined(DEBUG_USB)
                USB_debug_printf(" DETACHED USB in protect pin\n\r");
                lcd_printf("Detached USB in protect pin\n\r");
#endif
                StopHardware();
            }
        }
        else
        {
            if( g_PXA271_USB_Driver.PinsProtected )
            {
                // Ready for USB to enable, state change from Protected to Not protected
                g_PXA271_USB_Driver.PinsProtected = FALSE;

                // enable the clock,
                // set USB to attached/powered
                // set the device to a known state- Attached before it is set to the powered state (USB specf 9.1.1)
                CPU_GPIO_EnableInputPin(PXA271_USB::c_USBC_GPION_DET, FALSE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED);
                CPU_GPIO_EnableOutputPin(PXA271_USB::c_USBC_GPIOX_EN, FALSE);       // Don't signal the host yet
                State->DeviceState = USB_DEVICE_STATE_ATTACHED;
                
                USB_StateCallback( State );

                StartHardware();
            }
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
