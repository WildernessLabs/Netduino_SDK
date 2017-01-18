////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "AT91_USB.h"

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "Usb Controller State"
#endif

AT91_USB_Driver g_AT91_USB_Driver;

USB_CONTROLLER_STATE UsbControllerState[1];     // Only 1 USB Client Controller for this device

Hal_Queue_KnownSize<USB_PACKET64,USB_QUEUE_PACKET_COUNT> QueueBuffers[AT91_USB_Driver::c_Used_Endpoints-1];

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
    #pragma arm section zidata
#endif

///////////////////////////////////////////////////////////////////////////////
AT91_USB_Driver::UDP_EPATTRIBUTE AT91_USB_Driver::s_EpAttr[] =
{
    {AT91_UDP::UDP_EPTYPE_CTRL,      8,     FALSE,      AT91_UDP::UDP_RX_DATA_BK0},
    {AT91_UDP::UDP_EPTYPE_BULK_IN,  64,     TRUE,       AT91_UDP::UDP_RX_DATA_BK0},
    {AT91_UDP::UDP_EPTYPE_BULK_OUT, 64,     TRUE,       AT91_UDP::UDP_RX_DATA_BK0},
};

UINT32 AT91_USB_Driver::MAX_EP = sizeof(AT91_USB_Driver::s_EpAttr)  \
/ sizeof(AT91_USB_Driver::UDP_EPATTRIBUTE);
///////////////////////////////////////////////////////////////////////////////


USB_CONTROLLER_STATE * AT91_USB_Driver::GetState( int Controller )
{
    if( Controller != 0 )       // There is only one USB device controller for this device
        return NULL;
    return &UsbControllerState[0];
}

HRESULT AT91_USB_Driver::Initialize( int Controller )
{
    ASSERT(0 == Controller);

    //UINT8 logicalEndpoint;
    UINT8 endpointNum;
    //UINT8 altInterface;
    USB_CONTROLLER_STATE     &State = UsbControllerState[Controller];
    const USB_ENDPOINT_DESCRIPTOR  *ep    = NULL;
    const USB_INTERFACE_DESCRIPTOR *itfc  = NULL;

    AT91_UDP &udp = AT91::UDP();
    AT91_PMC &pmc = AT91::PMC();

    if(State.Initialized) return S_OK;

    g_AT91_USB_Driver.pUsbControllerState = NULL;

    GLOBAL_LOCK(irq);


    CPU_INTC_DeactivateInterrupt( AT91C_ID_UDP);


    // init the USB core
    udp.UDP_RSTEP = AT91_UDP::UDP_EP0 | AT91_UDP::UDP_EP1 | AT91_UDP::UDP_EP2 | AT91_UDP::UDP_EP3 | AT91_UDP::UDP_EP4 | AT91_UDP::UDP_EP5;
    udp.UDP_IDR = AT91_UDP::UDP_EPINT0 | AT91_UDP::UDP_EPINT1 |AT91_UDP::UDP_EPINT2 | AT91_UDP::UDP_EPINT3;

    // Enable USB device clock
    pmc.EnablePeriphClock(AT91C_ID_UDP);
    pmc.EnableSystemClock(AT91C_PMC_UDP);    


    // Enable the interrupt for  UDP
    CPU_INTC_ActivateInterrupt( AT91C_ID_UDP, Global_ISR, NULL);

    udp.UDP_IER |= AT91_UDP::UDP_EPINT0;
    udp.UDP_CSR[0] |= AT91_UDP::UDP_EPTYPE_CTRL;

    // For all endpoints in the USB Configuration list
    //logicalEndpoint = 1;
    while( USB_NextEndpoint( &State, ep, itfc) ) // && logicalEndpoint < 11 )
    {
        // Figure out which endpoint we are initializing
        endpointNum = ep->bEndpointAddress & 0x7F;
        
        // Check interface and endpoint numbers against hardware capability
        if( endpointNum >= AT91_USB_Driver::c_Used_Endpoints || itfc->bInterfaceNumber > 3 )
            return S_FALSE;

/*        
        EndpointInit[logicalEndpoint].bits.EPNUM      = endpointNum;
        EndpointInit[logicalEndpoint].bits.FIFONUM    = endpointNum;
        EndpointInit[logicalEndpoint].bits.DIR        = (ep->bEndpointAddress & 0x80) ? 1 : 0;
        EndpointInit[logicalEndpoint].bits.INTERFACE  = itfc->bInterfaceNumber;
        EndpointInit[logicalEndpoint].bits.TYPE       = ep->bmAttributes & 0x03;
        EndpointInit[logicalEndpoint].bits.TRXTYP     = 3;      // Always 3 for non-control endpoints
        EndpointInit[logicalEndpoint].bits.CONFIG     = 1;      // Always only 1 configuration: #1
*/
        // Set the maximum size of the endpoint hardware FIFO
        if( (ep->bmAttributes & 0x03) == USB_ENDPOINT_ATTRIBUTE_BULK )
        {
            int endpointSize = ep->wMaxPacketSize;

            // If the endpoint maximum size in the configuration list is bogus
            if( endpointSize != 8 && endpointSize != 16 && endpointSize != 32 && endpointSize != 64 )
                return S_FALSE;
            // If too large an endpoint size was requested
            if( endpointSize > s_EpAttr[endpointNum].Payload )
                return S_FALSE;
//            EndpointInit[logicalEndpoint].bits.MAXPKTSIZE = endpointSize;
            State.MaxPacketSize[endpointNum] = endpointSize;
        }
        else
        {
            // If Isochronous or Interupt type endpoint, always use the maximum size
//            EndpointInit[logicalEndpoint].bits.MAXPKTSIZE = s_EpAttr[endpointNum].Payload;
            State.MaxPacketSize[endpointNum] = s_EpAttr[endpointNum].Payload;
        }

        // Since endpoint 0 is only used for control, there is never a need to allocate a buffer for it
        // In fact State.Queues[0] is always NULL - it is a cheap placeholder to make the queueIndex = endpointIndex
        QueueBuffers[endpointNum-1].Initialize();                       // Clear queue before use
        State.Queues[endpointNum] = &QueueBuffers[endpointNum-1];       // Attach queue to endpoint

        // Enable an interrupt for the endpoint & set its direction
        if( (ep->bEndpointAddress & 0x80) ? 1 : 0 )        // If transmit endpoint
        {
            State.IsTxQueue[endpointNum] = TRUE;
            switch(ep->bmAttributes & 0x03)
            {
                case 1:
                    udp.UDP_CSR[endpointNum] |= AT91_UDP::UDP_EPTYPE_ISO_OUT;
                    break;
                case 2:
                    udp.UDP_CSR[endpointNum] |= AT91_UDP::UDP_EPTYPE_BULK_OUT;
                    break;
                case 3:
                    udp.UDP_CSR[endpointNum] |= AT91_UDP::UDP_EPTYPE_ISO_OUT;
                    break;
            }
        }
        else        // Receive endpoint
        {
            State.IsTxQueue[endpointNum] = FALSE;
            switch(ep->bmAttributes & 0x03)
            {
                case 1:
                    udp.UDP_CSR[endpointNum] |= AT91_UDP::UDP_EPTYPE_ISO_IN;
                    break;
                case 2:
                    udp.UDP_CSR[endpointNum] |= AT91_UDP::UDP_EPTYPE_BULK_IN;
                    break;
                case 3:
                    udp.UDP_CSR[endpointNum] |= AT91_UDP::UDP_EPTYPE_ISO_IN;
                    break;
            }
        }

        udp.UDP_IER |= (AT91_UDP::UDP_EPINT0 << endpointNum);

        // Move onto the next logical endpoint
        //logicalEndpoint++;
    }

    g_AT91_USB_Driver.pUsbControllerState  = &State;
    g_AT91_USB_Driver.PinsProtected = TRUE;

    State.EndpointStatus = &g_AT91_USB_Driver.EndpointStatus[0];
    State.EndpointCount  = c_Used_Endpoints;
    State.PacketSize     = s_EpAttr[0].Payload;

    State.FirstGetDescriptor = TRUE;
    //State.DeviceState = USB_DEVICE_STATE_DETACHED;

    // 
    #if defined(AT91_UDP_EXTERNAL_PULLUP)
        CPU_GPIO_EnableOutputPin(AT91_DP_PULLUP, TRUE);
    #endif

    ProtectPins( Controller, FALSE );

    return S_OK;
}

HRESULT AT91_USB_Driver::Uninitialize( int Controller )
{
#if defined(PLATFORM_ARM_SAM7_ANY)
    // FOR SAM7 we cannot restart the USB controller with out a hard reboot
    // and this method is only called for soft reboot, so do not reset 
    // Also, change the state to indicate it is initialized
    USB_CONTROLLER_STATE     &State = UsbControllerState[Controller];
    State.Initialized = TRUE;
#else
    GLOBAL_LOCK(irq);    

    ProtectPins( Controller, TRUE );
    // Enable USB device clock
    AT91_PMC &pmc = AT91::PMC();
    AT91_UDP &udp = AT91::UDP();

    
    pmc.DisablePeriphClock(AT91C_ID_UDP);
    pmc.DisableSystemClock(AT91C_PMC_UDP);    

    g_AT91_USB_Driver.pUsbControllerState = NULL;

    CPU_INTC_DeactivateInterrupt( AT91C_ID_UDP);
    
    memset(&UsbControllerState[Controller], 0, sizeof(UsbControllerState[Controller]));

    udp.UDP_FADDR  = 0;
    udp.UDP_RSTEP = udp.UDP_EP0 | udp.UDP_EP1 | udp.UDP_EP2 | udp.UDP_EP3;
    udp.UDP_ICR   = 0x3B;
  
    #if defined(AT91_UDP_EXTERNAL_PULLUP)
        CPU_GPIO_EnableOutputPin(AT91_DP_PULLUP, TRUE);
    #endif
#endif
    return S_OK;
}

BOOL AT91_USB_Driver::StartOutput( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT(State);
    ASSERT(endpoint < c_Used_Endpoints);

    GLOBAL_LOCK(irq);

   // If endpoint is not an output
    if( State->Queues[endpoint] == NULL || !State->IsTxQueue[endpoint] )
        return FALSE;

    /* if the halt feature for this endpoint is set, then just
            clear all the characters */
    if(g_AT91_USB_Driver.EndpointStatus[endpoint] & USB_STATUS_ENDPOINT_HALT)
    {
        ClearTxQueue( State, endpoint );
        return TRUE;
    }

    //If TxRunning, interrupts will drain the queue
    if(!g_AT91_USB_Driver.TxRunning[endpoint])
    {
        g_AT91_USB_Driver.TxRunning[endpoint] = TRUE;

        TxPacket( State, endpoint );
    }
    else if(irq.WasDisabled()) // We should not call EP_TxISR after calling TxPacket becuase it can cause a TX FIFO overflow error.
    {
        Endpoint_ISR(endpoint);
    }

    return TRUE;
}

BOOL AT91_USB_Driver::GetInterruptState()
{
    AT91_UDP &udp = AT91::UDP();

    GLOBAL_LOCK(irq);

    if((udp.UDP_IMR & udp.UDP_ISR) || (udp.UDP_ISR & AT91_UDP::UDP_ENDBUSRES))
    {
        return TRUE;
    }

    return FALSE;
}

//--//

void AT91_USB_Driver::ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT_IRQ_MUST_BE_OFF();
    ASSERT( State && (endpoint < USB_MAX_QUEUES) );
    ASSERT( State->Queues[endpoint] && !State->IsTxQueue[endpoint] );
        
    // it is much faster to just re-initialize the queue and it does the same thing
    //while(NULL != USB_TxDequeue( State, endpoint, TRUE ));
    State->Queues[endpoint]->Initialize();
}

//--//

void AT91_USB_Driver::TxPacket( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT( endpoint < c_Used_Endpoints );    
    ASSERT( State );

    AT91_UDP &udp = AT91::UDP();

    GLOBAL_LOCK(irq);

    // transmit a packet on UsbPortNum, if there are no more packets to transmit, then die
    USB_PACKET64* Packet64;

    Packet64 = USB_TxDequeue( State, endpoint, TRUE );

    if(Packet64 != NULL && Packet64->Size == 0)
    {
        g_AT91_USB_Driver.TxNeedZLPS[endpoint] = TRUE;
        Packet64 = NULL;
    }

    if(Packet64)
    {
        int i;

        // We should absolutely have an empty buffer to use
        while(0 != (udp.UDP_CSR[endpoint] & AT91_UDP::UDP_TXPKTRDY));

        // fill fifo
        for(i = 0; i < Packet64->Size; i++)
        {
            udp.UDP_FDR[endpoint] = Packet64->Buffer[i];
        }
        while(!(udp.UDP_CSR[endpoint] & AT91_UDP::UDP_TXPKTRDY)) udp.UDP_CSR[endpoint] |= AT91_UDP::UDP_TXPKTRDY;

        g_AT91_USB_Driver.TxNeedZLPS[endpoint] = (Packet64->Size == 64);
    }
    else
    {
        // send the zero leght packet since we landed on the FIFO boundary before
        // (and we queued a zero length packet to transmit)
        if(g_AT91_USB_Driver.TxNeedZLPS[endpoint])
        {
            while(!(udp.UDP_CSR[endpoint] & AT91_UDP::UDP_TXPKTRDY)) udp.UDP_CSR[endpoint] |= AT91_UDP::UDP_TXPKTRDY;

            g_AT91_USB_Driver.TxNeedZLPS[endpoint] = FALSE;
        }

        // no more data
        g_AT91_USB_Driver.TxRunning[endpoint] = FALSE;
    }
    while(udp.UDP_CSR[endpoint]  & AT91_UDP::UDP_TXCOMP)  udp.UDP_CSR[endpoint]  &= ~AT91_UDP::UDP_TXCOMP;

}

//--//

void AT91_USB_Driver::ControlNext()
{
    USB_CONTROLLER_STATE *State = g_AT91_USB_Driver.pUsbControllerState;
    
    ASSERT(State);

    AT91_UDP &udp = AT91::UDP();

    if(State->DataCallback)
    {
        /* this call can't fail */
        State->DataCallback( State );

        if (State->DataSize == 0)
        {
            while(!(udp.UDP_CSR[0] & AT91_UDP::UDP_TXPKTRDY)) udp.UDP_CSR[0] |= AT91_UDP::UDP_TXPKTRDY; // Send zero length packet

            // State->DataCallback = NULL;
        }
        else
        {
            for(int i = 0; i < State->DataSize; i++)
            {
                udp.UDP_FDR[0] = State->Data[i];
            }
            while(!(udp.UDP_CSR[0] & AT91_UDP::UDP_TXPKTRDY)) udp.UDP_CSR[0] |= AT91_UDP::UDP_TXPKTRDY; // Ready to send

            // special handling the USB driver set address test, cannot use the first descriptor as the ADDRESS state is handle in the hardware
            if(g_AT91_USB_Driver.FirstDescriptorPacket)
            {
                State->DataCallback = NULL;
            }
        }
    }
    while(udp.UDP_CSR[0]  & AT91_UDP::UDP_TXCOMP) udp.UDP_CSR[0] &= ~AT91_UDP::UDP_TXCOMP;
}

void AT91_USB_Driver::Global_ISR( void* Param )
{
    AT91_UDP &udp = AT91::UDP();

    UINT32 USB_INTR = (udp.UDP_ISR & udp.UDP_IMR) & AT91_UDP::UDP_ISRMASK;
    UINT32 endpoint  = 0;

    // Handle all UDP interrupts
    while(USB_INTR != 0)
    {
        // Start Of Frame (SOF)
        if(USB_INTR & AT91_UDP::UDP_SOFINT)
        {

            // Acknowledge interrupt
            udp.UDP_ICR = AT91_UDP::UDP_SOFINT;
            USB_INTR &= ~AT91_UDP::UDP_SOFINT;
            // This interrupt should not happen, as it is not enabled.
            ASSERT(0);
        }

        // Suspend
        if (USB_INTR & AT91_UDP::UDP_RXSUSP)
        {
            // Acknowledge interrupt
            udp.UDP_ICR = AT91_UDP::UDP_RXSUSP;
            USB_INTR &= ~AT91_UDP::UDP_RXSUSP;

            SuspendEvent();
        }

        // Resume or Wakeup
        if((USB_INTR & AT91_UDP::UDP_WAKEUP) ||(USB_INTR & AT91_UDP::UDP_RXRSM))
        {
            ResumeEvent();

            // Acknowledge interrupt
            udp.UDP_ICR = AT91_UDP::UDP_WAKEUP | AT91_UDP::UDP_RXRSM;
            USB_INTR &= ~(AT91_UDP::UDP_WAKEUP | AT91_UDP::UDP_RXRSM);
        }

        // End of bus reset
        if(USB_INTR & AT91_UDP::UDP_ENDBUSRES)
        {
            // Acknowledge end of bus reset interrupt
            udp.UDP_ICR = AT91_UDP::UDP_ENDBUSRES;
            USB_INTR &= ~AT91_UDP::UDP_ENDBUSRES;

            ResetEvent();
        }
        else //Endpoint Interrupt
        {
            UINT32 i = 0;
            while(USB_INTR != 0)
            {
                if (USB_INTR & 1)
                {
                    endpoint  = i;
                    if(udp.UDP_IMR & (1ul << endpoint))
                    {
                        Endpoint_ISR(endpoint);
                    }
                }
                USB_INTR >>= 1;
                i++;
            }
        }
    }
}

void AT91_USB_Driver::Endpoint_ISR(UINT32 endpoint)
{
    USB_CONTROLLER_STATE *State = g_AT91_USB_Driver.pUsbControllerState;
    UINT32 i;
    UINT32 EP_CSR;

    AT91_UDP &udp = AT91::UDP();

    EP_CSR = udp.UDP_CSR[endpoint];

    // Control Endpoint
    if(endpoint == 0)
    {
        // ugly
        if(EP_CSR & 0x02)
        {
            while(udp.UDP_CSR[0] & 0x02) udp.UDP_CSR[0] &= ~0x02;
        }

        // set up packet receive
        if(EP_CSR & AT91_UDP::UDP_RXSETUP)
        {
            UINT8 len = (EP_CSR >> 16) & 0x7F;

            /* copy in packet */
            for( i = 0; i < len; i++)
            {
                // read one byte each time, instead of 4 bytes
                g_AT91_USB_Driver.ControlPacketBuffer[i] = *(volatile UINT8 *)((UINT32)&udp.UDP_FDR[0]);
            }

            // special handling for the very first SETUP command - Getdescriptor[DeviceType], the host looks for 8 bytes data only
            USB_SETUP_PACKET* Setup= (USB_SETUP_PACKET* )&g_AT91_USB_Driver.ControlPacketBuffer[0];
            if((Setup->bRequest == USB_GET_DESCRIPTOR) && (((Setup->wValue & 0xFF00) >> 8) == USB_DEVICE_DESCRIPTOR_TYPE) && (Setup->wLength != 0x12))
                g_AT91_USB_Driver.FirstDescriptorPacket = TRUE;
            else
                g_AT91_USB_Driver.FirstDescriptorPacket = FALSE;

            if(Setup->bmRequestType & 0x80)
            {
                while(!(udp.UDP_CSR[0] & AT91_UDP::UDP_DIR)) udp.UDP_CSR[0] |= AT91_UDP::UDP_DIR;
            }

            // clear interrupt
            while(udp.UDP_CSR[0] & AT91_UDP::UDP_RXSETUP) udp.UDP_CSR[0] &= ~AT91_UDP::UDP_RXSETUP;

            /* send it to the upper layer */
            State->Data     = &g_AT91_USB_Driver.ControlPacketBuffer[0];
            State->DataSize = len;

            UINT8 result = USB_ControlCallback(State);

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
                /* USB spec 9.4.5 SET_CONFIGURATION resets halt conditions, resets toggle bits */
                    udp.UDP_GLBSTATE |= AT91_UDP::UDP_CONFG;
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

                // If port is now configured, output any queued data
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
        else if(EP_CSR & AT91_UDP::UDP_TXCOMP)
        {
            ControlNext();

            if(!(udp.UDP_GLBSTATE & AT91_UDP::UDP_FADDEN) && State->Address)
            {
                udp.UDP_FADDR = AT91_UDP::UDP_FEN | State->Address;
                udp.UDP_GLBSTATE |= AT91_UDP::UDP_FADDEN;
            }
        }
    }

    else
    {
        // IN packet sent
        if(EP_CSR & AT91_UDP::UDP_TXCOMP)
        {
            TxPacket( State, endpoint );
        }
        // OUT packet received
        if ((EP_CSR & AT91_UDP::UDP_RX_DATA_BK0) || (EP_CSR & AT91_UDP::UDP_RX_DATA_BK1))
        {
            BOOL          DisableRx;
            USB_PACKET64* Packet64 = USB_RxEnqueue( State, endpoint, DisableRx );

            /* copy packet in, making sure that Packet64->Buffer is never overflowed */
            if(Packet64)
            {
                UINT8 len = ( EP_CSR & AT91_UDP::UDP_RXBYTECNT) >> 16;

                for(int i = 0; i < len; i++)
                {
                    Packet64->Buffer[i] = (UINT8)(udp.UDP_FDR[endpoint]);
                }

                Packet64->Size = len;
            }
            else
            {
                /* flow control should absolutely protect us from ever getting here, so if we do, it is a bug */
                ASSERT(0);
            }

            if(!DisableRx)
            {
                udp.UDP_CSR[endpoint] &= ~g_AT91_USB_Driver.s_EpAttr[endpoint].dFlag;
                // Swap banks
                if (g_AT91_USB_Driver.s_EpAttr[endpoint].dFlag == AT91_UDP::UDP_RX_DATA_BK0)
                {
                    if (g_AT91_USB_Driver.s_EpAttr[endpoint].DualBank)
                    {
                        // Swap bank if in dual-fifo mode
                        g_AT91_USB_Driver.s_EpAttr[endpoint].dFlag = AT91_UDP::UDP_RX_DATA_BK1;
                    }
                }
                else
                {
                    g_AT91_USB_Driver.s_EpAttr[endpoint].dFlag = AT91_UDP::UDP_RX_DATA_BK0;
                }
            }
            else
            {
                udp.UDP_IDR = AT91_UDP::UDP_EPINT0 << endpoint;
            }
        }
    }
}

//--//

void AT91_USB_Driver::SuspendEvent()
{
    USB_CONTROLLER_STATE *State = g_AT91_USB_Driver.pUsbControllerState;
    
    ASSERT(State);

    AT91_UDP &udp = AT91::UDP();

    // Enable wakeup and resume
    udp.UDP_IER = AT91_UDP::UDP_WAKEUP | AT91_UDP::UDP_RXRSM;
    // Disbale suspend interrupt
    udp.UDP_IDR = AT91_UDP::UDP_RXSUSP;

    // The device enters the Suspended state
    // MCK + UDPCK must be off
    // Pull-Up must be connected
    // Transceiver must be disabled
    udp.UDP_TXVC |= AT91_UDP::UDP_TXVDIS;

    AT91_PMC &pmc = AT91::PMC();
    pmc.DisablePeriphClock(AT91C_ID_UDP);
    pmc.DisableSystemClock(AT91C_PMC_UDP);    

    g_AT91_USB_Driver.PreviousDeviceState = State->DeviceState;

    State->DeviceState = USB_DEVICE_STATE_SUSPENDED;

    USB_StateCallback( State );

    #if defined(DEBUG_USB)
        USB_debug_printf("In suspend event previous %d\n\r", g_AT91_USB_Driver.PreviousDeviceState);
        lcd_printf("In suspend \n\r");
    #endif

}


void AT91_USB_Driver::ResumeEvent()
{
    USB_CONTROLLER_STATE *State = g_AT91_USB_Driver.pUsbControllerState;
    
    ASSERT(State);

    AT91_UDP &udp = AT91::UDP();

    #if defined(DEBUG_USB)
        USB_debug_printf(" in Resume\r\n");
    #endif

    // The device enters Configured state
    // MCK + UDPCK must be on
    // Pull-Up must be connected
    // Transceiver must be enabled


    AT91_PMC &pmc = AT91::PMC();
    pmc.EnablePeriphClock(AT91C_ID_UDP);
    pmc.EnableSystemClock(AT91C_PMC_UDP);    

    udp.UDP_TXVC &= ~AT91_UDP::UDP_TXVDIS;

    State->DeviceState = g_AT91_USB_Driver.PreviousDeviceState;

    USB_StateCallback( State );

    // Enable wakeup and resume
    udp.UDP_IER = AT91_UDP::UDP_RXSUSP;
    udp.UDP_IDR = AT91_UDP::UDP_WAKEUP | AT91_UDP::UDP_RXRSM;

}



void AT91_USB_Driver::ResetEvent()
{
    USB_CONTROLLER_STATE *State = g_AT91_USB_Driver.pUsbControllerState;
    
    // the hardware handles most of the reset procedure.
    ASSERT(State);

    AT91_UDP &udp = AT91::UDP();

    // MCK + UDPCK are already enabled
    // Pull-Up is already connected
    // Transceiver must be enabled
    // Endpoint 0 must be enabled

    udp.UDP_TXVC &= ~AT91_UDP::UDP_TXVDIS;

    udp.UDP_ICR = AT91_UDP::UDP_WAKEUP | AT91_UDP::UDP_RXRSM | AT91_UDP::UDP_RXSUSP;

    udp.UDP_IER = AT91_UDP::UDP_WAKEUP | AT91_UDP::UDP_RXRSM | AT91_UDP::UDP_RXSUSP;


    // program Endpoint Status/control
    for(int i = 0; i < MAX_EP; i++)
    {
        udp.UDP_IER |= 1 << i;
        udp.UDP_CSR[i] = s_EpAttr[i].Dir_Type | AT91_UDP::UDP_EPEDS;
        g_AT91_USB_Driver.s_EpAttr[i].dFlag = AT91_UDP::UDP_RX_DATA_BK0;
    }

    // flush whatever in the FIFO from EP0 to EP5
    udp.UDP_RSTEP = 0x3F;

    /* clear all flags */
    USB_ClearEvent( 0, USB_EVENT_ALL);

    #if defined(DEBUG_USB)
        USB_debug_printf(" in Reset\r\n");
    
        if(State->DeviceState >= USB_DEVICE_STATE_ADDRESS)
        {
            static int count = 0;
            lcd_printf("\fRESET: %d  \r\n", ++count);
        }
    #endif

    for(int ep = 0; ep < c_Used_Endpoints; ep++)
    {
        g_AT91_USB_Driver.TxRunning [ep] = FALSE;
        g_AT91_USB_Driver.TxNeedZLPS[ep] = FALSE;
    }

    State->DeviceState        = USB_DEVICE_STATE_DEFAULT;
    State->Address            = 0;
    USB_StateCallback( State );

    udp.UDP_RSTEP = 0;
}

//--//

BOOL AT91_USB_Driver::RxEnable( USB_CONTROLLER_STATE *State, int endpoint )
{
    AT91_UDP &udp = AT91::UDP();

    GLOBAL_LOCK(irq);

    udp.UDP_IER = (AT91_UDP::UDP_EPINT0 << endpoint);

    return TRUE;
}

BOOL AT91_USB_Driver::ProtectPins( int Controller, BOOL On )
{
    ASSERT( 0 == Controller );
    USB_CONTROLLER_STATE *State = g_AT91_USB_Driver.pUsbControllerState;

    GLOBAL_LOCK(irq);

    // initialized yet?
    if(State)
    {
        if(On)
        {
            if(!g_AT91_USB_Driver.PinsProtected)
            {
                // Disable the USB com, state change from Not protected to Protected
 
                g_AT91_USB_Driver.PinsProtected = TRUE;

                // clear USB Txbuffer
                for(int ep = 0; ep < c_Used_Endpoints; ep++)
                {
                    if(State->IsTxQueue[ep]) State->Queues[ep]->Initialize();
                }

                //--//
                #if defined(AT91_VBUS_DETECTION)
                    CPU_GPIO_EnableInputPin( AT91_VBUS, TRUE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );
                #endif

                //detach D plus line
                #if defined(AT91_UDP_EXTERNAL_PULLUP)
                    CPU_GPIO_SetPinState(AT91_DP_PULLUP, TRUE);

                #elif defined(AT91_UDP_INTERNAL_PULLUP)
                    AT91_UDP &udp = AT91::UDP();
                    udp.UDP_TXVC &= ~AT91_UDP::UDP_PUON;
                #elif defined(AT91_UDP_MATRIX_PULLUP)
                    AT91_MATRIX &matrix = AT91::MATRIX();
                    matrix.MATRIX_USBPCR &= ~ AT91_MATRIX::MATRIX_USBPCR_PUON;
                #endif

                //--//
                State->DeviceState = USB_DEVICE_STATE_DETACHED;
                USB_StateCallback( State );

                #if defined(DEBUG_USB)
                    USB_debug_printf(" DETACHED USB in protect pin\n\r");
                    lcd_printf("Detached USB in protect pin\n\r");
                #endif
            }
        }
        else
        {
            if(g_AT91_USB_Driver.PinsProtected)
            {
                #if defined(AT91_VBUS_DETECTION)
                    CPU_GPIO_EnableInputPin( AT91_VBUS, TRUE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );
                #endif

                //detach D plus line
                #if defined(AT91_UDP_EXTERNAL_PULLUP)
                    CPU_GPIO_SetPinState(AT91_DP_PULLUP, FALSE);
                #elif defined(AT91_UDP_INTERNAL_PULLUP)
                {
                    AT91_UDP &udp = AT91::UDP();
                    udp.UDP_TXVC &= ~AT91_UDP::UDP_PUON;
                }
                #elif defined(AT91_UDP_MATRIX_PULLUP)
                {
                    AT91_MATRIX &matrix = AT91::MATRIX();

                    matrix.MATRIX_USBPCR &= ~AT91_MATRIX::MATRIX_USBPCR_PUON;
                }
                #endif

                HAL_Time_Sleep_MicroSeconds(500);

                // Ready for USB to enable, state change from Protected to Not protected
                g_AT91_USB_Driver.PinsProtected = FALSE;

                #if defined(AT91_VBUS_DETECTION)
                    CPU_GPIO_EnableInputPin( AT91_VBUS, TRUE, VBus_ISR, GPIO_INT_EDGE_BOTH, RESISTOR_DISABLED );
                #else
                    VBus_ISR(0, TRUE, NULL);
                #endif

                #if defined(AT91_UDP_EXTERNAL_PULLUP)
                    CPU_GPIO_SetPinState(AT91_DP_PULLUP, FALSE);
                #elif defined(AT91_UDP_INTERNAL_PULLUP)
                    AT91_UDP &udp = AT91::UDP();
                    udp.UDP_TXVC |= AT91_UDP::UDP_PUON;
                #elif defined(AT91_UDP_MATRIX_PULLUP)
                    AT91_MATRIX &matrix = AT91::MATRIX();
                    matrix.MATRIX_USBPCR |= AT91_MATRIX::MATRIX_USBPCR_PUON;
                #endif
            }
        }

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//--//
void AT91_USB_Driver::VBus_ISR(GPIO_PIN Pin, BOOL PinState, void* Param)
{
    USB_CONTROLLER_STATE *State = g_AT91_USB_Driver.pUsbControllerState;
    AT91_UDP &udp = AT91::UDP();

    // VBus High
    if(PinState)
    {

        AT91_PMC &pmc = AT91::PMC();

        // Enable MCK
        pmc.EnablePeriphClock(AT91C_ID_UDP);

        //Enable UDPCK
        pmc.EnableSystemClock(AT91C_PMC_UDP);    


        udp.UDP_ICR = AT91_UDP::UDP_WAKEUP | AT91_UDP::UDP_RXRSM | AT91_UDP::UDP_RXSUSP;

        #if defined(AT91_UDP_EXTERNAL_PULLUP)
            CPU_GPIO_SetPinState(AT91_DP_PULLUP, TRUE);
        #elif defined(AT91_UDP_INTERNAL_PULLUP)
            udp.UDP_TXVC |= AT91_UDP::UDP_PUON;
        #elif defined(AT91_UDP_MATRIX_PULLUP)
            AT91_MATRIX &matrix = AT91::MATRIX();

            matrix.MATRIX_USBPCR |= AT91_MATRIX::MATRIX_USBPCR_PUON;
        #endif

        State->DeviceState = USB_DEVICE_STATE_ATTACHED;
        USB_StateCallback( State );
    }
    // VBus Low
    else          
    {
        // clear USB Txbuffer
        for(int ep = 0; ep < c_Used_Endpoints; ep++)
        {
            if(State->IsTxQueue[ep]) State->Queues[ep]->Initialize();
        }

        #if defined(AT91_UDP_EXTERNAL_PULLUP)
            CPU_GPIO_SetPinState(AT91_DP_PULLUP, FALSE);
        #elif defined(AT91_UDP_INTERNAL_PULLUP)
            udp.UDP_TXVC &= ~AT91_UDP::UDP_PUON;
        #elif defined(AT91_UDP_MATRIX_PULLUP)
            AT91_MATRIX &matrix = AT91::MATRIX();
            matrix.MATRIX_USBPCR &= ~AT91_MATRIX::MATRIX_USBPCR_PUON;
        #endif

        AT91_PMC &pmc = AT91::PMC();

        // Disable MCK
        pmc.DisablePeriphClock(AT91C_ID_UDP);
        //Disable UDPCK
        pmc.DisableSystemClock(AT91C_PMC_UDP);    

        State->DeviceState = USB_DEVICE_STATE_DETACHED;
        USB_StateCallback( State );
    }
}

