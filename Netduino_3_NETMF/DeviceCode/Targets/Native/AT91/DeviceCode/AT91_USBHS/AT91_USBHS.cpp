////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "AT91_USBHS.h"

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "Usb Controller State"
#endif

AT91_USBHS_Driver g_AT91_USBHS_Driver;

USB_CONTROLLER_STATE UsbControllerState[1];  // Only 1 USB Client Controller for this device

Hal_Queue_KnownSize<USB_PACKET64,USB_QUEUE_PACKET_COUNT> QueueBuffers[AT91_USBHS_Driver::c_Used_Endpoints-1];

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

#define AT91C_CKGR_UPLLEN_ENABLED  (0x1 << 16) // (PMC) The UTMI PLL is enabled
#define AT91C_CKGR_BIASEN_ENABLED  (0x1 << 24) // (PMC) The UTMI BIAS is enabled
#define SHIFT_INTERUPT             8

UINT32 ep_dir[6]={0,0,0,0,0,0};
UINT32 num_ep_int1 = 0;
UINT32 num_ep_int2 = 0;

__inline void AT91_PMC_EnableUSBClock(void)
{
    (*(volatile UINT32 *)0xFFFFFC10) = 1 << AT91C_ID_UDP;
    (*(volatile UINT32 *)0xFFFFFC1C) |= AT91C_CKGR_UPLLEN_ENABLED;
}

__inline void AT91_PMC_EnableUTMIBIAS(void)
{
    (*(volatile UINT32 *)0xFFFFFC1C) |= AT91C_CKGR_BIASEN_ENABLED;
}
__inline void AT91_PMC_DisableUSBClock(void)
{
    (*(volatile UINT32 *)0xFFFFFC14) = 1 << AT91C_ID_UDP;
    (*(volatile UINT32 *)0xFFFFFC1C) &= ~AT91C_CKGR_UPLLEN_ENABLED;
}

__inline void AT91_PMC_DisableUTMIBIAS(void)
{
    (*(volatile UINT32 *)0xFFFFFC1C) &= ~AT91C_CKGR_BIASEN_ENABLED;
}

///////////////////////////////////////////////////////////////////////////////
AT91_USBHS_Driver::UDP_EPATTRIBUTE AT91_USBHS_Driver::s_EpAttr[] =
{
    {AT91C_UDPHS_EPT_TYPE_CTL_EPT|AT91C_UDPHS_EPT_DIR_OUT,         64,      FALSE,     AT91C_UDPHS_BK_NUMBER_1},
    {AT91C_UDPHS_EPT_TYPE_BUL_EPT|AT91C_UDPHS_EPT_DIR_IN,      64,     TRUE,       AT91C_UDPHS_BK_NUMBER_2},
    {AT91C_UDPHS_EPT_TYPE_BUL_EPT|AT91C_UDPHS_EPT_DIR_OUT, 64,     TRUE,       AT91C_UDPHS_BK_NUMBER_2},
};

UINT32 AT91_USBHS_Driver::MAX_EP = sizeof(AT91_USBHS_Driver::s_EpAttr)  \
/ sizeof(AT91_USBHS_Driver::UDP_EPATTRIBUTE);
///////////////////////////////////////////////////////////////////////////////

USB_CONTROLLER_STATE * AT91_USBHS_Driver::GetState( int Controller )
{
    if( Controller != 0 )       // There is only one USB device controller for this device
        return NULL;
    return &UsbControllerState[0];
}

HRESULT AT91_USBHS_Driver::Initialize( int Controller )
{
    
    ASSERT(0 == Controller);

    //UINT8 logicalEndpoint;
    UINT8 endpointNum;
    //UINT8 altInterface;
    USB_CONTROLLER_STATE     &State = UsbControllerState[Controller];
    const USB_ENDPOINT_DESCRIPTOR  *ep    = NULL;
    const USB_INTERFACE_DESCRIPTOR *itfc  = NULL;

    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP);
    struct AT91_UDPHS_EPT *pEp = (struct AT91_UDPHS_EPT *) (AT91C_BASE_UDP + 0x100);
    ep_dir[0]=0;
    ep_dir[1]=0;
    ep_dir[2]=0;
    ep_dir[3]=0;
    ep_dir[4]=0;
    ep_dir[5]=0;
    
    num_ep_int1= 0;
    num_ep_int2= 0;

    GLOBAL_LOCK(irq);
    
    
    // Enable USB device clock
    AT91_PMC_EnableUSBClock();
    AT91_PMC_EnableUTMIBIAS();

    // Enable the interrupt for  UDP
    CPU_INTC_ActivateInterrupt( AT91C_ID_UDP, Global_ISR, NULL);

    pUdp->UDPHS_IEN |= AT91C_UDPHS_EPT_INT_0;
    pEp->UDPHS_EPTCFG |= 0x00000043; //configuration info for control ep
//    pUdp->UDP_CSR[0] |= AT91C_UDP_EPTYPE_CTRL;

    // For all endpoints in the USB Configuration list
    //logicalEndpoint = 1;
    while( USB_NextEndpoint( &State, ep, itfc) ) // && logicalEndpoint < 11 )
    {
        // Figure out which endpoint we are initializing
        endpointNum = ep->bEndpointAddress & 0x7F;
        
        // Check interface and endpoint numbers against hardware capability
        if( endpointNum >= AT91_USBHS_Driver::c_Used_Endpoints || itfc->bInterfaceNumber > 3 )
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
            if( endpointSize != 8 && endpointSize != 16 && endpointSize != 32 && endpointSize != 64 && endpointSize != 128 && endpointSize != 512 && endpointSize != 1024 )
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
        
        
        
        pEp = (struct AT91_UDPHS_EPT *) (AT91C_BASE_UDP + 0x100 + 0x20 * (endpointNum));
        // Enable an interrupt for the endpoint & set its direction
        if( (ep->bEndpointAddress & 0x80) ? 1 : 0 )        // If transmit endpoint
        {
            State.IsTxQueue[endpointNum] = TRUE;
            
            switch(ep->bmAttributes & 0x03)
            {
                case 1:
                    pEp->UDPHS_EPTCFG |= AT91C_UDPHS_EPT_TYPE_ISO_EPT|AT91C_UDPHS_EPT_DIR_OUT;
                    
                    break;
                case 2:
                    pEp->UDPHS_EPTCFG |= AT91C_UDPHS_EPT_TYPE_BUL_EPT|AT91C_UDPHS_EPT_DIR_OUT;
                    
                    break;
                case 3:
                    pEp->UDPHS_EPTCFG |= AT91C_UDPHS_EPT_TYPE_INT_EPT|AT91C_UDPHS_EPT_DIR_OUT;
                    
                    break;
            }
            
        }
        else        // Receive endpoint
        {
            State.IsTxQueue[endpointNum] = FALSE;
            switch(ep->bmAttributes & 0x03)
            {
                case 1:
                    pEp->UDPHS_EPTCFG |= AT91C_UDPHS_EPT_TYPE_ISO_EPT|AT91C_UDPHS_EPT_DIR_IN;
                    
                    break;
                case 2:
                    pEp->UDPHS_EPTCFG |= AT91C_UDPHS_EPT_TYPE_BUL_EPT|AT91C_UDPHS_EPT_DIR_IN;
                    
                    break;
                case 3:
                    pEp->UDPHS_EPTCFG |= AT91C_UDPHS_EPT_TYPE_INT_EPT|AT91C_UDPHS_EPT_DIR_IN;
                    
                    break;
            }
        }
        
                
        pUdp->UDPHS_IEN |= (AT91C_UDPHS_EPT_INT_0 << endpointNum);

        // Move onto the next logical endpoint
        //logicalEndpoint++;
    }

    g_AT91_USBHS_Driver.pUsbControllerState  = &State;
    g_AT91_USBHS_Driver.PinsProtected = TRUE;

    State.EndpointStatus = &g_AT91_USBHS_Driver.EndpointStatus[0];
    State.EndpointCount  = c_Used_Endpoints;
    State.PacketSize     = s_EpAttr[0].Payload;

    State.FirstGetDescriptor = TRUE;
    //State.DeviceState = USB_DEVICE_STATE_DETACHED;

    ProtectPins( Controller, FALSE );
//    debug_printf("ep_dir[1]: %x, ep_dir[2]: %x, %d, %d\r\n", ep_dir[1], ep_dir[2], State.MaxPacketSize[1], State.MaxPacketSize[2]);
    

    return S_OK;
}

HRESULT AT91_USBHS_Driver::Uninitialize( int Controller )
{
    GLOBAL_LOCK(irq);

    AT91_PMC_DisableUTMIBIAS();
    AT91_PMC_DisableUSBClock();

    ProtectPins( Controller, TRUE );

    CPU_INTC_DeactivateInterrupt( AT91C_ID_UDP);

    g_AT91_USBHS_Driver.pUsbControllerState = NULL;


    return S_OK;
}

BOOL AT91_USBHS_Driver::StartOutput( USB_CONTROLLER_STATE* State, int endpoint )
{
    
    ASSERT(State);
    ASSERT(endpoint < c_Used_Endpoints);
    
    GLOBAL_LOCK(irq);
    
    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) AT91C_BASE_UDP;

   // If endpoint is not an output
    if( State->Queues[endpoint] == NULL || !State->IsTxQueue[endpoint] )
        return FALSE;

    pUdp->UDPHS_IEN |= 1 << SHIFT_INTERUPT << endpoint;
    pUdp->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = AT91C_UDPHS_TX_PK_RDY;
    

    /* if the halt feature for this endpoint is set, then just
            clear all the characters */
    if(g_AT91_USBHS_Driver.EndpointStatus[endpoint] & USB_STATUS_ENDPOINT_HALT)
    {
        ClearTxQueue( State, endpoint );
        return TRUE;
    }

    //If TxRunning, interrupts will drain the queue

    if(!(BOOL)((UINT8)g_AT91_USBHS_Driver.TxRunning[endpoint]))
    {

        num_ep_int1++;
        g_AT91_USBHS_Driver.TxRunning[endpoint] = TRUE;

        TxPacket( State, endpoint );
    }
    else if(irq.WasDisabled()) // We should not call EP_TxISR after calling TxPacket becuase it can cause a TX FIFO overflow error.
    {
        Endpoint_ISR(endpoint);
    }

    return TRUE;
}

BOOL AT91_USBHS_Driver::GetInterruptState()
{
    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP) ;

    GLOBAL_LOCK(irq);

    if((pUdp->UDPHS_IEN & pUdp->UDPHS_INTSTA) || (pUdp->UDPHS_INTSTA & AT91C_UDPHS_ENDRESET))
    {
        return TRUE;
    }

    return FALSE;
}

//--//

void AT91_USBHS_Driver::ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT_IRQ_MUST_BE_OFF();
    
    ASSERT( State && (endpoint < USB_MAX_QUEUES) );
            
    // it is much faster to just re-initialize the queue and it does the same thing
    //while(NULL != USB_TxDequeue( State, endpoint, TRUE ));
    if (State->Queues[endpoint] != NULL)
    {
        State->Queues[endpoint]->Initialize();
    }
}

//--//

void AT91_USBHS_Driver::TxPacket( USB_CONTROLLER_STATE* State, int endpoint )
{
    
    ASSERT( endpoint < c_Used_Endpoints );    
    ASSERT( State );

    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP) ;
    UINT8 * pDest = (UINT8*) ((((struct AT91_UDPHS_EPTFIFO *)AT91C_BASE_UDP_DMA)->UDPHS_READEPT0) + 16384 * endpoint);
    
    GLOBAL_LOCK(irq);
    
    
    // transmit a packet on UsbPortNum, if there are no more packets to transmit, then die
    USB_PACKET64* Packet64;

    Packet64 = USB_TxDequeue( State, endpoint, TRUE );

    if(Packet64 != NULL && Packet64->Size == 0)
    {
        g_AT91_USBHS_Driver.TxNeedZLPS[endpoint] = TRUE;
        Packet64 = NULL;
    }

    while (pUdp->UDPHS_EPT[endpoint].UDPHS_EPTSTA & AT91C_UDPHS_TX_PK_RDY);
    
    if(Packet64)
    {
        int i;

        // We should absolutely have an empty buffer to use
//        ASSERT(pEp->UDP_CSR[endpoint] & AT91C_UDP_TXPKTRDY);

        // fill fifo

        for(i = 0; i < Packet64->Size; i++)
        {
            *pDest++ = Packet64->Buffer[i];
        }

        g_AT91_USBHS_Driver.TxNeedZLPS[endpoint] = (Packet64->Size == 64);
    }
    else
    {
        // send the zero leght packet since we landed on the FIFO boundary before
        // (and we queued a zero length packet to transmit)
        if(g_AT91_USBHS_Driver.TxNeedZLPS[endpoint])
        {
             g_AT91_USBHS_Driver.TxNeedZLPS[endpoint] = FALSE;
        }

        // no more data
        g_AT91_USBHS_Driver.TxRunning[endpoint] = FALSE;
        pUdp->UDPHS_EPT[endpoint].UDPHS_EPTCTLDIS = AT91C_UDPHS_TX_PK_RDY;
        
    }
    pUdp->UDPHS_EPT[endpoint].UDPHS_EPTSETSTA = AT91C_UDPHS_TX_PK_RDY;

}

//--//

void AT91_USBHS_Driver::ControlNext()
{
    USB_CONTROLLER_STATE *State = g_AT91_USBHS_Driver.pUsbControllerState;
    
    ASSERT(State);

    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP);
    UINT8 *pFifo = (UINT8 *)&(((struct AT91_UDPHS_EPTFIFO *)AT91C_BASE_UDP_DMA)->UDPHS_READEPT0[0]);

    if(State->DataCallback)
    {
        /* this call can't fail */
        State->DataCallback( State );

        if (State->DataSize == 0)
        {
            while(!(pUdp->UDPHS_EPT[0].UDPHS_EPTSTA & AT91C_UDPHS_TX_PK_RDY)) 
                    pUdp->UDPHS_EPT[0].UDPHS_EPTSETSTA = AT91C_UDPHS_TX_PK_RDY;    // Send zero length packet

            // State->DataCallback = NULL;
        }
        else
        {
            for(int i = 0; i < State->DataSize; i++)
            {
                *pFifo++ = State->Data[i];
            }
            while(!(pUdp->UDPHS_EPT[0].UDPHS_EPTSTA & AT91C_UDPHS_TX_PK_RDY)) 
                pUdp->UDPHS_EPT[0].UDPHS_EPTSETSTA = AT91C_UDPHS_TX_PK_RDY; // Ready to send

            // special handling the USB driver set address test, cannot use the first descriptor as the ADDRESS state is handle in the hardware
            if(g_AT91_USBHS_Driver.FirstDescriptorPacket)
            {
                State->DataCallback = NULL;
            }
        }
    }
    else
    {
        pUdp->UDPHS_EPT[0].UDPHS_EPTCLRSTA = AT91C_UDPHS_TX_COMPLT;
        pUdp->UDPHS_EPT[0].UDPHS_EPTCTLDIS = AT91C_UDPHS_TX_PK_RDY;
    }
}

void AT91_USBHS_Driver::Global_ISR( void* Param )
{
    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP);

    UINT32 USB_INTR = (pUdp->UDPHS_INTSTA & pUdp->UDPHS_IEN);
    UINT32 endpoint  = 0;

//    debug_printf("%x", USB_INTR);
    // Handle all UDP interrupts
    while(USB_INTR != 0)
    {
        // Start Of Frame (SOF)
        if(USB_INTR & AT91C_UDPHS_IEN_SOF)
        {

            // Acknowledge interrupt
            pUdp->UDPHS_CLRINT = AT91C_UDPHS_IEN_SOF;
            USB_INTR &= ~AT91C_UDPHS_IEN_SOF;
            // This interrupt should not happen, as it is not enabled.
            
            ASSERT(0);
        }

        // Suspend
        if (USB_INTR & AT91C_UDPHS_DET_SUSPD)
        {
            AT91_PMC_DisableUTMIBIAS();
            // Acknowledge interrupt
            pUdp->UDPHS_CLRINT  = AT91C_UDPHS_DET_SUSPD | AT91C_UDPHS_WAKE_UP;
            USB_INTR &= ~AT91C_UDPHS_DET_SUSPD;
            
            SuspendEvent();        
        }

        // Resume or Wakeup
        if((USB_INTR & AT91C_UDPHS_WAKE_UP) ||(USB_INTR & AT91C_UDPHS_ENDOFRSM))
        {
            ResumeEvent();

            // Acknowledge interrupt
            pUdp->UDPHS_CLRINT = AT91C_UDPHS_WAKE_UP | AT91C_UDPHS_ENDOFRSM;
            USB_INTR &= ~(AT91C_UDPHS_WAKE_UP | AT91C_UDPHS_ENDOFRSM);
        }

        // End of bus reset
        if(USB_INTR & AT91C_UDPHS_ENDRESET)
        {
            // Acknowledge end of bus reset interrupt
            

            ResetEvent();
            pUdp->UDPHS_CLRINT = AT91C_UDPHS_WAKE_UP | AT91C_UDPHS_DET_SUSPD | AT91C_UDPHS_ENDRESET;
            pUdp->UDPHS_IEN |= AT91C_UDPHS_DET_SUSPD;

            USB_INTR &= ~AT91C_UDPHS_ENDRESET;
        }
        
        if(USB_INTR & AT91C_UDPHS_UPSTR_RES)
        {
            pUdp->UDPHS_CLRINT = AT91C_UDPHS_UPSTR_RES;
            USB_INTR &= ~AT91C_UDPHS_UPSTR_RES;
        }
        else //Endpoint Interrupt
        {
            UINT32 i = 0;
            USB_INTR >>= 8;
            while(USB_INTR != 0)
            {
                if (USB_INTR & 1)
                {
                        
                    endpoint  = i;
                    Endpoint_ISR(endpoint);
                    if (endpoint == 1)
                    {
//                        debug_printf("1\r\n");
//                        num_ep_int1 += 1;
                    }
                    if (endpoint == 2)
                    {
//                        debug_printf("2\r\n");
//                        num_ep_int2 += 1;
                    }
                    
                }
                USB_INTR >>= 1;
                i++;
            }
        }
        USB_INTR = pUdp->UDPHS_INTSTA & pUdp->UDPHS_IEN;
    }
}

void AT91_USBHS_Driver::Endpoint_ISR(UINT32 endpoint)
{
    USB_CONTROLLER_STATE *State = g_AT91_USBHS_Driver.pUsbControllerState;
    UINT32 Status;
    
    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP);
    struct AT91_UDPHS_EPTFIFO *pFifo = (struct AT91_UDPHS_EPTFIFO *)AT91C_BASE_UDP_DMA;
    
    Status = pUdp->UDPHS_EPT[endpoint].UDPHS_EPTSTA;
    
    
    // Control Endpoint
    if(endpoint == 0)
    {
        // ugly
        if(Status & AT91C_UDPHS_RX_BK_RDY)
        {
            while(pUdp->UDPHS_EPT[0].UDPHS_EPTSTA & AT91C_UDPHS_RX_BK_RDY) pUdp->UDPHS_EPT[0].UDPHS_EPTCLRSTA &= ~AT91C_UDPHS_RX_BK_RDY;
        }

        // set up packet receive
        if(Status & AT91C_UDPHS_RX_SETUP)
        {

#if 1            
            UINT8 len = (pUdp->UDPHS_EPT[0].UDPHS_EPTSTA >> 20) & 0x7F;
            (*(UINT32 *)(&g_AT91_USBHS_Driver.ControlPacketBuffer[0])) = pFifo->UDPHS_READEPT0[0];
            (*(UINT32 *)(&g_AT91_USBHS_Driver.ControlPacketBuffer[4])) = pFifo->UDPHS_READEPT0[0];
#else
            UINT8 len = 8;
            
#endif
            
//            UINT8 len = (EP_CSR >> 16) & 0x7F;
        
            
            // special handling for the very first SETUP command - Getdescriptor[DeviceType], the host looks for 8 bytes data only
            USB_SETUP_PACKET* Setup= (USB_SETUP_PACKET* )&g_AT91_USBHS_Driver.ControlPacketBuffer[0];
            
            
            if((Setup->bRequest == USB_GET_DESCRIPTOR) && (((Setup->wValue & 0xFF00) >> 8) == USB_DEVICE_DESCRIPTOR_TYPE) && (Setup->wLength != 0x12))
                g_AT91_USBHS_Driver.FirstDescriptorPacket = TRUE;
            else
                g_AT91_USBHS_Driver.FirstDescriptorPacket = FALSE;
            
            
/*
            if(Setup->bmRequestType & 0x80)
            {
                while(!(pUdp->UDP_CSR[0] & AT91C_UDP_DIR)) pUdp->UDP_CSR[0] |= AT91C_UDP_DIR;
            }
*/
            
            while(pUdp->UDPHS_EPT[0].UDPHS_EPTSTA & AT91C_UDPHS_RX_SETUP)
                pUdp->UDPHS_EPT[0].UDPHS_EPTCLRSTA = AT91C_UDPHS_RX_SETUP;        
                
            


            /* send it to the upper layer */
            State->Data     = &g_AT91_USBHS_Driver.ControlPacketBuffer[0];
            State->DataSize = len;
            
            pUdp->UDPHS_EPT[0].UDPHS_EPTCTLENB = AT91C_UDPHS_TX_PK_RDY;
            
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
        else if(!(Status & AT91C_UDPHS_TX_PK_RDY) && (pUdp->UDPHS_EPT[0].UDPHS_EPTCTL & AT91C_UDPHS_TX_PK_RDY))
        {
            
            ControlNext();

            if(State->Address)
            {
                if(!(pUdp->UDPHS_CTRL & AT91C_UDPHS_FADDR_EN))
                    pUdp->UDPHS_CTRL |= AT91C_UDPHS_FADDR_EN | State->Address;                
            }
        }
    }

    else
    {
//        debug_printf("status: %x\r\n",Status);
        // IN packet sent
        if((!(Status & AT91C_UDPHS_TX_PK_RDY)) && (pUdp->UDPHS_EPT[endpoint].UDPHS_EPTCTL & AT91C_UDPHS_TX_PK_RDY))
        {
            TxPacket( State, endpoint );
        }
        // OUT packet received
        if (Status & AT91C_UDPHS_RX_BK_RDY)
        {
            BOOL          DisableRx;


            USB_PACKET64* Packet64 = USB_RxEnqueue( State, endpoint, DisableRx );

            /* copy packet in, making sure that Packet64->Buffer is never overflowed */
            if(Packet64)
            {
                UINT32 len = (( Status & AT91C_UDPHS_BYTE_COUNT) >> 20)&0x7FF;
                UINT8 *pDest = (UINT8 *)(pFifo->UDPHS_READEPT0 + 16384*endpoint);
                
                for(int i = 0; i < len; i++)
                {
                    Packet64->Buffer[i] = *pDest++;
                }

                Packet64->Size = len;

             

            }
            else
            {
                /* flow control should absolutely protect us from ever getting here, so if we do, it is a bug */
                
                ASSERT(0);
            }

            pUdp->UDPHS_EPT[endpoint].UDPHS_EPTCLRSTA = AT91C_UDPHS_RX_BK_RDY;
            
            if(DisableRx)
            {

                pUdp->UDPHS_IEN &= ~(1 << SHIFT_INTERUPT << endpoint);
                
                pUdp->UDPHS_EPT[endpoint].UDPHS_EPTCTLDIS = AT91C_UDPHS_RX_BK_RDY;
            }
            

        }
    }
}

//--//

void AT91_USBHS_Driver::SuspendEvent()
{
    USB_CONTROLLER_STATE *State = g_AT91_USBHS_Driver.pUsbControllerState;
    
    
    ASSERT(State);

    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP);

    // Enable wakeup and resume
    pUdp->UDPHS_IEN |= AT91C_UDPHS_WAKE_UP | AT91C_UDPHS_ENDOFRSM;
    // Disbale suspend interrupt
    pUdp->UDPHS_IEN &= ~AT91C_UDPHS_DET_SUSPD;

    // The device enters the Suspended state
    // MCK + UDPCK must be off
    // Pull-Up must be connected
    // Transceiver must be disabled
    

    // TODO: will be replaced by PMC API
    AT91_PMC_DisableUSBClock();

    g_AT91_USBHS_Driver.PreviousDeviceState = State->DeviceState;

    State->DeviceState = USB_DEVICE_STATE_SUSPENDED;

    USB_StateCallback( State );

    #if defined(DEBUG_USB)
        USB_debug_printf("In suspend event previous %d\n\r", g_AT91_USBHS_Driver.PreviousDeviceState);
        lcd_printf("In suspend \n\r");
    #endif

}


void AT91_USBHS_Driver::ResumeEvent()
{
    USB_CONTROLLER_STATE *State = g_AT91_USBHS_Driver.pUsbControllerState;
    
    ASSERT(State);

    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP);

    #if defined(DEBUG_USB)
        USB_debug_printf(" in Resume\r\n");
    #endif

    // The device enters Configured state
    // MCK + UDPCK must be on
    // Pull-Up must be connected
    // Transceiver must be enabled

    // TODO: will be replaced by PMC API
    AT91_PMC_EnableUSBClock();
    AT91_PMC_EnableUTMIBIAS();

    

    State->DeviceState = g_AT91_USBHS_Driver.PreviousDeviceState;

    USB_StateCallback( State );

    // Enable end of reset and suspend interrupt
   pUdp->UDPHS_IEN |= AT91C_UDPHS_ENDOFRSM | AT91C_UDPHS_DET_SUSPD;
   
   // Disable Wakeup interrupt
   pUdp->UDPHS_IEN &= ~AT91C_UDPHS_WAKE_UP;

}



void AT91_USBHS_Driver::ResetEvent()
{
    USB_CONTROLLER_STATE *State = g_AT91_USBHS_Driver.pUsbControllerState;
    
    // the hardware handles most of the reset procedure.
    
    ASSERT(State);
    
    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP);

    // MCK + UDPCK are already enabled
    // Pull-Up is already connected
    // Transceiver must be enabled
    // Endpoint 0 must be enabled

    
    
    


    // program Endpoint Status/control
    for(int i = 0; i < MAX_EP; i++)
    {
        UINT32 dwEptcfg = 0;
        // Reset endpoint fifos
        pUdp->UDPHS_EPTRST = 1 << i;
        // Enable endpoint interrupt
        pUdp->UDPHS_IEN |= (1 << SHIFT_INTERUPT << i);     
        // Set endpoint configration
        dwEptcfg = s_EpAttr[i].Dir_Type | s_EpAttr[i].dFlag;
        switch (s_EpAttr[i].Payload)
        {
        case 8:
            dwEptcfg |= AT91C_UDPHS_EPT_SIZE_8;
            break;
        case 16:
            dwEptcfg |= AT91C_UDPHS_EPT_SIZE_16;
            break;
        case 32:
            dwEptcfg |= AT91C_UDPHS_EPT_SIZE_32;
            break;
        case 64:
            dwEptcfg |= AT91C_UDPHS_EPT_SIZE_64;
            break;
        case 128:
            dwEptcfg |= AT91C_UDPHS_EPT_SIZE_128;
            break;
        case 256:
            dwEptcfg |= AT91C_UDPHS_EPT_SIZE_256;
            break;
        case 512:
            dwEptcfg |= AT91C_UDPHS_EPT_SIZE_512;
            break;
        case 1024:
            dwEptcfg |= AT91C_UDPHS_EPT_SIZE_1024;
            break;
        default:
        break;
        }
                
        pUdp->UDPHS_EPT[i].UDPHS_EPTCFG = dwEptcfg;
        
        if(!(AT91C_UDPHS_EPT_MAPD & pUdp->UDPHS_EPT[i].UDPHS_EPTCFG)) while(1);        
        pUdp->UDPHS_EPT[i].UDPHS_EPTCTLENB = AT91C_UDPHS_EPT_ENABL | AT91C_UDPHS_RX_BK_RDY;// | AT91C_UDPHS_TX_PK_RDY;
    }

    
    
    pUdp->UDPHS_EPT[0].UDPHS_EPTCTLENB = AT91C_UDPHS_RX_SETUP | AT91C_UDPHS_TX_PK_RDY;
    
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
        g_AT91_USBHS_Driver.TxRunning [ep] = FALSE;
        g_AT91_USBHS_Driver.TxNeedZLPS[ep] = FALSE;
    }
    
    State->DeviceState        = USB_DEVICE_STATE_DEFAULT;
    State->Address            = 0;
    USB_StateCallback( State );

    
}

//--//

BOOL AT91_USBHS_Driver::RxEnable( USB_CONTROLLER_STATE *State, int endpoint )
{
    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) AT91C_BASE_UDP;
        
    pUdp->UDPHS_IEN |= 1 << SHIFT_INTERUPT << endpoint;
//    while(!(pUdp->UDPHS_EPT[endpoint].UDPHS_EPTCTL & AT91C_UDPHS_RX_BK_RDY))
    pUdp->UDPHS_EPT[endpoint].UDPHS_EPTCTLENB = AT91C_UDPHS_RX_BK_RDY;
    return TRUE;
}

BOOL AT91_USBHS_Driver::ProtectPins( int Controller, BOOL On )
{
    
    ASSERT( 0 == Controller );
    USB_CONTROLLER_STATE *State = g_AT91_USBHS_Driver.pUsbControllerState;

    GLOBAL_LOCK(irq);

    // initialized yet?
    if(State)
    {
        if(On)
        {
            if(!g_AT91_USBHS_Driver.PinsProtected)
            {
                // Disable the USB com, state change from Not protected to Protected
 
                g_AT91_USBHS_Driver.PinsProtected = TRUE;

                // clear USB Txbuffer
                for(int ep = 0; ep < c_Used_Endpoints; ep++)
                {
                    if( State->Queues[ep] && State->IsTxQueue[ep] )
                        ClearTxQueue( State, ep );
                    
                }

                //--//
                #if defined(AT91_VBUS_DETECTION)
                    CPU_GPIO_EnableInputPin( AT91_VBUS, TRUE, NULL, GPIO_INT_NONE, RESISTOR_DISABLED );
                #endif

                //detach D plus line
                #if defined(AT91_UDP_EXTERNAL_PULLUP)
                    CPU_GPIO_SetPinState(AT91_DP_PULLUP, FALSE);
                #elif defined(AT91_UDP_INTERNAL_PULLUP)
                    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) AT91C_BASE_UDP;
                    pUdp->UDPHS_CTRL |= AT91C_UDPHS_DETACH; // dettach
                #elif defined(AT91_UDP_MATRIX_PULLUP)
                    struct AT91_MATRIX *pMatrix = (struct AT91_MATRIX *)AT91C_BASE_MATRIX;
                    pMatrix->MATRIX_USBPCR &= ~AT91C_MATRIX_USBPCR_PUON;
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
            if(g_AT91_USBHS_Driver.PinsProtected)
            {
                // Ready for USB to enable, state change from Protected to Not protected
                g_AT91_USBHS_Driver.PinsProtected = FALSE;

                #if defined(AT91_VBUS_DETECTION)
                    CPU_GPIO_EnableInputPin( AT91_VBUS, TRUE, VBus_ISR, GPIO_INT_EDGE_BOTH, RESISTOR_DISABLED );
                #else
                    VBus_ISR(0, TRUE, NULL);
                #endif

                #if defined(AT91_UDP_EXTERNAL_PULLUP)
                    CPU_GPIO_SetPinState(AT91_DP_PULLUP, FALSE);
                #elif defined(AT91_UDP_INTERNAL_PULLUP)
                    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) AT91C_BASE_UDP;
                    pUdp->UDPHS_CTRL &= ~AT91C_UDPHS_DETACH; // attach*)
                    
                #elif defined(AT91_UDP_MATRIX_PULLUP)
                    struct AT91_MATRIX *pMatrix = (struct AT91_MATRIX *)AT91C_BASE_MATRIX;
                    pMatrix->MATRIX_USBPCR |= AT91C_MATRIX_USBPCR_PUON;
                #endif
                
                if(CPU_GPIO_GetPinState (AT91_VBUS))
                {
                    VBus_ISR(0, TRUE, NULL);
                }

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
void AT91_USBHS_Driver::VBus_ISR(GPIO_PIN Pin, BOOL PinState, void* Param)
{
    USB_CONTROLLER_STATE *State = g_AT91_USBHS_Driver.pUsbControllerState;
    struct AT91_UDPHS *pUdp = (struct AT91_UDPHS *) (AT91C_BASE_UDP) ;

    // VBus High
    if(PinState)
    {
        // Enable USB clock
        AT91_PMC_EnableUSBClock();

        #if defined(AT91_UDP_EXTERNAL_PULLUP)
            CPU_GPIO_EnableOutputPin(AT91_DP_PULLUP, TRUE);
        #elif defined(AT91_UDP_INTERNAL_PULLUP)
            // Detach DP
            pUdp->UDPHS_CTRL |= AT91C_UDPHS_DETACH; 
        #elif defined(AT91_UDP_MATRIX_PULLUP)
            struct AT91_MATRIX *pMatrix = (struct AT91_MATRIX *)AT91C_BASE_MATRIX;
            pMatrix->MATRIX_USBPCR |= AT91C_MATRIX_USBPCR_PUON;
        #endif
        
        // Reset and enable IP UDPHS
        pUdp->UDPHS_CTRL &= ~AT91C_UDPHS_EN_UDPHS;
        pUdp->UDPHS_CTRL |= AT91C_UDPHS_EN_UDPHS;
            
        // With OR without DMA !!!
        // Initialization of DMA
        for( int i = 1; i <= ((pUdp->UDPHS_IPFEATURES & AT91C_UDPHS_DMA_CHANNEL_NBR) >> 4); i++ )
        {
            // RESET endpoint canal DMA:
            // DMA stop channel command
            pUdp->UDPHS_DMA[i].UDPHS_DMACONTROL = 0;  // STOP command

            // Disable endpoint
            pUdp->UDPHS_EPT[i].UDPHS_EPTCTLDIS = 0xFFFFFFFF;

            // Reset endpoint config
            pUdp->UDPHS_EPT[i].UDPHS_EPTCTLENB = 0;

            // Reset DMA channel (Buff count and Control field)
            pUdp->UDPHS_DMA[i].UDPHS_DMACONTROL = 0x02;  // NON STOP command

            // Reset DMA channel 0 (STOP)
            pUdp->UDPHS_DMA[i].UDPHS_DMACONTROL = 0;  // STOP command

            // Clear DMA channel status (read the register for clear it)
            pUdp->UDPHS_DMA[i].UDPHS_DMASTATUS = pUdp->UDPHS_DMA[i].UDPHS_DMASTATUS;

    }
    
        pUdp->UDPHS_TST = 0;
        //pUdp->UDPHS_IEN = 0;
        pUdp->UDPHS_IEN = AT91C_UDPHS_ENDOFRSM | AT91C_UDPHS_WAKE_UP | AT91C_UDPHS_DET_SUSPD;
        pUdp->UDPHS_CLRINT = 0xFE;
        
        // Pull up the DP line
        pUdp->UDPHS_CTRL &= ~AT91C_UDPHS_DETACH; // attach
        
        State->DeviceState = USB_DEVICE_STATE_ATTACHED;
        
        USB_StateCallback( State );
    }
    // VBus Low
    else          
    {
        // clear USB Txbuffer
        for(int ep = 0; ep < c_Used_Endpoints; ep++)
        {
            if (State->IsTxQueue[ep] && State->Queues[ep] != NULL)
                State->Queues[ep]->Initialize();
        }

        #if defined(AT91_UDP_EXTERNAL_PULLUP)
            CPU_GPIO_SetPinState(AT91_DP_PULLUP, FALSE);
        #elif defined(AT91_UDP_INTERNAL_PULLUP)
            // Detach DP
            pUdp->UDPHS_CTRL |= AT91C_UDPHS_DETACH; 
        #elif defined(AT91_UDP_MATRIX_PULLUP)
            struct AT91_MATRIX *pMatrix = (struct AT91_MATRIX *)AT91C_BASE_MATRIX;
            pMatrix->MATRIX_USBPCR &= ~AT91C_MATRIX_USBPCR_PUON;
        #endif

        // TODO: will be replaced by PMC API
        // Disable  USB clock
        AT91_PMC_DisableUSBClock();

        State->DeviceState = USB_DEVICE_STATE_DETACHED;
        USB_StateCallback( State );
    }
}

