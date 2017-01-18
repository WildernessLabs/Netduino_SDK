////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

/*
#define MC9328MXL_EP_COUNT              2
#define CPU_USB_DEFAULT_PACKET_SIZE     32
#define CPU_USB_BULK_PACKET_SIZE        64
#define CPU_USB_MAX_TX_RETRY            3
*/

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_MC9328MXL_USB_Driver"
#endif

MC9328MXL_USB_Driver g_MC9328MXL_USB_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "UsbBusState"
#endif

USB_CONTROLLER_STATE UsbControllerState[1];     // Only 1 USB Controller for this device

Hal_Queue_KnownSize<USB_PACKET64,USB_QUEUE_PACKET_COUNT> QueueBuffers[MC9328MXL_USB::c_USB_MAX_EP-1];

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

/***************************************************************************/

void DisplayState()
{
    MC9328MXL_USB& USB = MC9328MXL::USB();
    
    USB_debug_printf( "current state %x \r\n"     , USB.USB_STAT               );
    USB_debug_printf( "Int state %x , mask %x\r\n", USB.USB_INTR, USB.USB_MASK );

    for (int i=0; i<=5;i++)
    {
        USB_debug_printf( " EP %d - intr %x \r\n", i, USB.EP[i].EPn_INTR );
    }
}

/***************************************************************************/

static MC9328MXL_USB::EndpointConfiguration EndpointInit[MC9328MXL_USB::c_USB_MAX_LOGICAL_EP];      // Corresponds to endpoint configuration RAM at MC9328MXL::USB_DDAT

//--//

USB_CONTROLLER_STATE * MC9328MXL_USB_Driver::GetState( int Controller )
{
    if( Controller != 0 )       // There is only one controller for this device
        return NULL;
    return &UsbControllerState[0];
}

HRESULT MC9328MXL_USB_Driver::Initialize( int Controller )
{
    UINT8 logicalEndpoint;
    UINT8 endpointNum;
    UINT8 altInterface;
    USB_CONTROLLER_STATE     &State = UsbControllerState[Controller];
    const USB_ENDPOINT_DESCRIPTOR  *ep    = NULL;
    const USB_INTERFACE_DESCRIPTOR *itfc  = NULL;
    
    ASSERT(Controller == 0);       // There is only one USB controller for this device

    GLOBAL_LOCK(irq);

    MC9328MXL_USB& USB = MC9328MXL::USB();
    
    // Initialize HW endpoint configuration RAM image so that all fields default to zero
    // This array is used by StartHardware() to initialize the endpoint configuration
    for( logicalEndpoint = 0; logicalEndpoint < USB.c_USB_MAX_LOGICAL_EP; logicalEndpoint++ )
    {
        EndpointInit[logicalEndpoint].words[0] = 0;
        EndpointInit[logicalEndpoint].words[1] = 0;      // Set all unused bits to 0
    }

    // make sure clock is enabled
    MC9328MXL::SC().GCCR |= MC9328MXL_SC::GCCR__USBD_CLK_EN;

    // The only part of the control endpoint that must be initialized is the size
    // - and the hardware dictates that it must have a size of 8
    EndpointInit[0].bits.MAXPKTSIZE = c_default_ctrl_packet_size;

    // Control endpoint and USB global interrupts must always be enabled
    CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT0, EP0_ISR, (void*)(size_t)0  );
    CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT6, Global_ISR,          NULL  );

    // For all endpoints in the USB Configuration list
    logicalEndpoint = 1;
    while( USB_NextEndpoint( &State, ep, itfc) && logicalEndpoint < USB.c_USB_MAX_LOGICAL_EP )
    {
        // Figure out which endpoint we are initializing
        endpointNum = ep->bEndpointAddress & 0x7F;
        
        // Check interface and endpoint numbers against hardware capability
        if( endpointNum >= USB.c_USB_MAX_EP || itfc->bInterfaceNumber > 3 )
            return S_FALSE;
        
        EndpointInit[logicalEndpoint].bits.EPNUM      = endpointNum;
        EndpointInit[logicalEndpoint].bits.FIFONUM    = endpointNum;
        EndpointInit[logicalEndpoint].bits.DIR        = (ep->bEndpointAddress & 0x80) ? 1 : 0;
        EndpointInit[logicalEndpoint].bits.INTERFACE  = itfc->bInterfaceNumber;
        EndpointInit[logicalEndpoint].bits.TYPE       = ep->bmAttributes & 0x03;
        EndpointInit[logicalEndpoint].bits.TRXTYP     = 3;      // Always 3 for non-control endpoints
        EndpointInit[logicalEndpoint].bits.CONFIG     = 1;      // Always only 1 configuration: #1

        // Set the maximum size of the endpoint hardware FIFO
        if( (ep->bmAttributes & 0x03) == USB_ENDPOINT_ATTRIBUTE_BULK )
        {
            int endpointSize = ep->wMaxPacketSize;

            // If the endpoint maximum size in the configuration list is bogus
            if( endpointSize != 8 && endpointSize != 16 && endpointSize != 32 && endpointSize != 64 )
                return S_FALSE;
            // If too large an endpoint size was requested
            if( endpointSize > USB.MaxEPsize(endpointNum) )
                return S_FALSE;
            EndpointInit[logicalEndpoint].bits.MAXPKTSIZE = endpointSize;
            State.MaxPacketSize[endpointNum] = endpointSize;
        }
        else
        {
            // If Isochronous or Interupt type endpoint, always use the maximum size
            EndpointInit[logicalEndpoint].bits.MAXPKTSIZE = USB.MaxEPsize( endpointNum );
            State.MaxPacketSize[endpointNum] = USB.MaxEPsize( endpointNum );
        }

        // Since endpoint 0 is only used for control, there is never a need to allocate a buffer for it
        // In fact State.Queues[0] is always NULL - it is a cheap placeholder to make the queueIndex = endpointIndex
        QueueBuffers[endpointNum-1].Initialize();                       // Clear queue before use
        State.Queues[endpointNum] = &QueueBuffers[endpointNum-1];       // Attach queue to endpoint

        // Enable an interrupt for the endpoint & set its direction
        if( EndpointInit[logicalEndpoint].bits.DIR )        // If transmit endpoint
        {
            CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT0+endpointNum, EP_TxISR, (void*)(size_t)endpointNum );
            State.IsTxQueue[endpointNum] = TRUE;
        }
        else        // Receive endpoint
        {
            CPU_INTC_ActivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT0+endpointNum, EP_RxISR, (void*)(size_t)endpointNum );
            State.IsTxQueue[endpointNum] = FALSE;
        }

        // Move onto the next logical endpoint
        logicalEndpoint++;
    }

    // If no endpoints were initialized, something is seriously wrong with the configuration list
    if( 1 == logicalEndpoint )
    {
        CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT0 );
        CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT6 );
        return S_FALSE;
    }

    // Initialize the rest of the logical endpoints with bogus alternate endpoint #'s
    // (they must be initialized with something or the USB HW goes bonkers)
    endpointNum  = 1;
    altInterface = 1;
    while( logicalEndpoint < USB.c_USB_MAX_LOGICAL_EP )
    {
        EndpointInit[logicalEndpoint].bits.EPNUM      = endpointNum;
        EndpointInit[logicalEndpoint].bits.FIFONUM    = endpointNum;
        EndpointInit[logicalEndpoint].bits.ALTSETTING = altInterface;
        EndpointInit[logicalEndpoint].bits.TRXTYP     = 3;      // Always 3 for non-control endpoints
        EndpointInit[logicalEndpoint].bits.CONFIG     = 1;      // Always only 1 configuration: #1
        EndpointInit[logicalEndpoint].bits.MAXPKTSIZE = USB.MaxEPsize( endpointNum );     

        // Move to next logical endpoint
        logicalEndpoint++;
        if( ++endpointNum >= USB.c_USB_MAX_EP )
        {
            endpointNum = 1;
            altInterface++;
        }
    }

    g_MC9328MXL_USB_Driver.pControllerState  = &State;
    g_MC9328MXL_USB_Driver.PinsProtected = TRUE;

    State.EndpointStatus = &g_MC9328MXL_USB_Driver.EndpointStatus[0];
    State.EndpointCount  = MC9328MXL_USB::c_USB_MAX_EP;
    //State.DeviceStatus   = USB_STATUS_DEVICE_SELF_POWERED;
    State.PacketSize     = c_default_ctrl_packet_size;
    
    State.FirstGetDescriptor = TRUE;

    ProtectPins( Controller, FALSE );

    return S_OK;
}

HRESULT MC9328MXL_USB_Driver::Uninitialize( int Controller )
{
    ASSERT( Controller == 0 );
    
    GLOBAL_LOCK(irq);

    ProtectPins( Controller, TRUE );

    g_MC9328MXL_USB_Driver.pControllerState = NULL;

    CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT0 );
    CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT1 );
    CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT2 );
    CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT3 );
    CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT4 );
    CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT5 );
    CPU_INTC_DeactivateInterrupt( MC9328MXL_AITC::c_IRQ_INDEX_USBD_INT6 );

    return S_OK;
}

BOOL MC9328MXL_USB_Driver::StartOutput( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT(State && (endpoint < MC9328MXL_USB::c_USB_MAX_EP));

    GLOBAL_LOCK(irq);

    // If endpoint is not an output
    if( State->Queues[endpoint] == NULL || !State->IsTxQueue[endpoint] )
        return FALSE;

    /* if the halt feature for this endpoint is set, then just
       clear all the characters */
    if(g_MC9328MXL_USB_Driver.EndpointStatus[endpoint] & USB_STATUS_ENDPOINT_HALT)
    {
        ClearTxQueue( State, endpoint );
        return TRUE;
    }

    //If TxRunning, interrupts will drain the queue
    if(!g_MC9328MXL_USB_Driver.TxRunning[endpoint])
    {
        g_MC9328MXL_USB_Driver.TxRunning[endpoint] = TRUE;

        // Calling both TxPacket & EP_TxISR in this routine could cause a TX FIFO overflow
        TxPacket( State, endpoint );
    }
    else if(irq.WasDisabled())      // If interrupts are disabled, queues must be drained manually
    {
        MC9328MXL_USB& USB = MC9328MXL::USB();
        
        // This could be called during Flush with all interrupts off.  Just taking care of the endpoint
        // in question may cause a logjam if the host is expecting a response from another endpoint.
        // All endpoints must be examined for activity.

        // First, check for global interrupts
        if((USB.USB_INTR & (~USB.USB_MASK )) != 0)
            Global_ISR(0);

        // Next, check endpoint 0 since it is handled differently
        if( USB.EP[0].EPn_INTR & (~ USB.EP[0].EPn_MASK) )
            EP0_ISR(0);

        // Last, check all the other endpoints
        for (int ep = 1; ep < MC9328MXL_USB::c_USB_MAX_EP; ep++)
        {
            if( State->Queues[ep] && (USB.EP[ep].EPn_INTR & (~USB.EP[ep].EPn_MASK )))
            {
                if( State->IsTxQueue[ep] )
                    EP_TxISR( (void *)ep );
                else
                    EP_RxISR( (void *)ep );
            }
        }
    }

    return TRUE;
}

BOOL MC9328MXL_USB_Driver::GetInterruptState()
{
    MC9328MXL_USB& USB = MC9328MXL::USB();

    GLOBAL_LOCK(irq);

    if((USB.USB_INTR & (~USB.USB_MASK )) != 0) 
    {
        return TRUE;
    }
    else
    {
        for (int ep = 0; ep < MC9328MXL_USB::c_USB_MAX_EP; ep++)
        {
            if( USB.EP[ep].EPn_INTR & (~USB.EP[ep].EPn_MASK )) return TRUE;
        }
    }
    
    return FALSE;
}

//--//

void MC9328MXL_USB_Driver::ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT(State && endpoint < MC9328MXL_USB::c_USB_MAX_EP);
    while(NULL != USB_TxDequeue( State, endpoint, TRUE ));
}

//--//
void MC9328MXL_USB_Driver::StartHardware()
{
    // clk and the PLL is already enabled at bootup time.
    // since it is always attached to PC - no PC presence check

    MC9328MXL_USB& USB   = MC9328MXL::USB();
    MC9328MXL_GPIO &GPIO = MC9328MXL::GPIO(1);
    USB_CONTROLLER_STATE *State = g_MC9328MXL_USB_Driver.pControllerState;

    ASSERT(State);

    // Disable all GPIO pins that are multiplexed with USB signals
    CPU_GPIO_DisablePin( USB.c_USBD_AFE_PIN_MUX_GPIO_B20   , RESISTOR_DISABLED, USB.c_USBD_AFE_PIN_DIR   , GPIO_ALT_PRIMARY );
    CPU_GPIO_DisablePin( USB.c_USBD_SUSPND_PIN_MUX_GPIO_B23, RESISTOR_DISABLED, USB.c_USBD_SUSPND_PIN_DIR, GPIO_ALT_PRIMARY );
    CPU_GPIO_DisablePin( USB.c_USBD_VMO_PIN_MUX_GPIO_B27   , RESISTOR_DISABLED, USB.c_USBD_VMO_PIN_DIR   , GPIO_ALT_PRIMARY );
    CPU_GPIO_DisablePin( USB.c_USBD_VPO_PIN_MUX_GPIO_B26   , RESISTOR_DISABLED, USB.c_USBD_VPO_PIN_DIR   , GPIO_ALT_PRIMARY );
    CPU_GPIO_DisablePin( USB.c_USBD_ROE_PIN_MUX_GPIO_B21   , RESISTOR_DISABLED, USB.c_USBD_ROE_PIN_DIR   , GPIO_ALT_PRIMARY );
    CPU_GPIO_DisablePin( USB.c_USBD_VM_PIN_MUX_GPIO_B25    , RESISTOR_DISABLED, USB.c_USBD_VM_PIN_DIR    , GPIO_ALT_PRIMARY );
    CPU_GPIO_DisablePin( USB.c_USBD_VP_PIN_MUX_GPIO_B24    , RESISTOR_DISABLED, USB.c_USBD_VP_PIN_DIR    , GPIO_ALT_PRIMARY );
    CPU_GPIO_DisablePin( USB.c_USBD_RCV_PIN_MUX_GPIO_B22   , RESISTOR_DISABLED, USB.c_USBD_RCV_PIN_DIR   , GPIO_ALT_PRIMARY );

    // perform a hardware reset for the USB core, set the RST bit, also set the USB_ENAB.ENAB bit

    // turn off the USB module first, switch off, this is needed, especially when PortBooter -> main program, the last USB module
    // is still active and in some unknown state.
    USB.USB_ENAB  = USB.USB_ENAB_ENAB;
    USB.USB_CTRL &= ~(MC9328MXL_USB::USB_CTRL_ENA | MC9328MXL_USB::USB_CTRL_AFE_ENA);
    USB.USB_ENAB =0;
    
    HAL_Time_Sleep_MicroSeconds(10);
    
    USB.USB_CTRL |= USB.USB_CTRL_UDC_RST;

    HAL_Time_Sleep_MicroSeconds(10);
    
    USB.USB_ENAB |= USB.USB_ENAB_RST;

    // wait until reset done, reset pin clear
    while ( USB.USB_ENAB & USB.USB_ENAB_RST );

    // wait until the CFG pin sets after the reset to set the descriptor RAM        
    while (!(USB.USB_DADR & USB.USB_DADR_CFG)) ;

    // write ENDPTBUF, have to write up to 55 bytes to fill up the RAM and get the USB_DADR_CFG to turn off.
    // reading from the RAM is useless as it won't return anything except 0
    for( int ep = 0; ep < USB.c_USB_MAX_LOGICAL_EP; ep++ )
    {
        USB.Set_EPBuf( &EndpointInit[ep] );
    }

    while (USB.USB_DADR & USB.USB_DADR_CFG ) USB.USB_DDAT = 0;

    // USB_DADR_CFG should be 0 at this point
    if( USB.USB_DADR & USB.USB_DADR_CFG )
    {
        ASSERT(0);
        return;
    }

    // clear pending int
    USB.USB_INTR = USB.USB_INTR_ALL_CLEAR;

    //////////////////////////////////////////////////////////////////////////////
    // program USB Interrupt Mask, enable reset, suspend and configu change
   
    //enable reset only
    USB.USB_MASK = USB.USB_MASK_RESET_STOP & USB.USB_MASK_RESET_START ;

   // clear all the old interrupt
    for (int ep = 0; ep < USB.c_USB_MAX_EP; ep++)
    {
        USB.EP[ep].EPn_INTR = MC9328MXL_USB::EndPoint::EPn_INTR_ALL_CLEAR;
    }

    // Program Control EndPoint interrupt mask    
    USB.EP[0].EPn_MASK = USB.EP[0].EPn_MASK_FIFO_ERROR & 
                         USB.EP[0].EPn_MASK_MDEVREQ    &  
                         USB.EP[0].EPn_MASK_EOT        &  
                         USB.EP[0].EPn_MASK_DEVREQ     &  
                         USB.EP[0].EPn_MASK_EOF        ;
    USB.EP[0].EPn_STAT = USB.EP[0].EPn_STAT_MAX_8byte | USB.EP[0].EPn_STAT_DIR_OUT | USB.EP[0].EPn_STAT_TYP_CTRL | USB.EP[0].EPn_STAT_FLUSH;

    // For all endpoints
    for( int ep = 1; ep < MC9328MXL_USB::c_USB_MAX_EP; ep++)
    {
        MC9328MXL_USB::EndpointConfiguration *pEpConfig;
        UINT32 EpControlByte;

        // Find endpoint initialization info for this endpoint
        pEpConfig = NULL;
        EpControlByte = USB.EP[0].EPn_STAT_FLUSH;
        for( int lep = 0; lep < USB.c_USB_MAX_LOGICAL_EP; lep++ )
        {
            // If this is one of the active endpoints
            if( (EndpointInit[lep].bits.EPNUM == ep) && (EndpointInit[lep].bits.ALTSETTING == 0) )
            {
                pEpConfig = &EndpointInit[lep];
                break;
            }
        }

        if( pEpConfig )      // If queue for this endpoint is active
        {
            // Activate corresponding endpoint FIFO if queue is being used
            USB.EP[ep].EPn_MASK = USB.EP[ep].EPn_MASK_FIFO_ERROR & 
                                  USB.EP[ep].EPn_MASK_EOT        & 
                                  USB.EP[ep].EPn_MASK_EOF        ;
            // Set the FIFO size
            switch( pEpConfig->bits.MAXPKTSIZE )
            {
            case 64:
                EpControlByte |= USB.EP[1].EPn_STAT_MAX_64byte;
                break;
            case 32:
                EpControlByte |= USB.EP[1].EPn_STAT_MAX_32byte;
                break;
            case 16:
                EpControlByte |= USB.EP[1].EPn_STAT_MAX_16byte;
                break;
            }
            if( State->IsTxQueue[ep] )
                EpControlByte |= USB.EP[1].EPn_STAT_DIR_IN;
            EpControlByte |= (pEpConfig->bits.TYPE << USB.EP[1].EPn_STAT_TYP_shift);
            USB.EP[ep].EPn_STAT = EpControlByte;        // Set the Endpoint Control register
        }
        else
        {
            // Turn off all unused FIFOs
            USB.EP[ep].EPn_MASK = 0x1FF;
            EpControlByte |= USB.EP[1].EPn_STAT_TYP_BULK;       // Safest type - ?
            USB.EP[ep].EPn_STAT = EpControlByte;
        }
    }
    
    // program FIFO control register ???TBD

    for (int ep = 0; ep < USB.c_USB_MAX_EP; ep++)
    {
        USB.EP[ep].EPn_FCTRL = USB.EP[ep].EPn_FCTRL_FRAME | USB.EP[ep].EPn_FCTRL_GR_1byte;
        USB.EP[ep].EPn_FALRM = 0x0;
    }
    // Wait for the RESET_START to clear
    while( USB.USB_INTR & USB.USB_INTR_RESET_START );

    // Enable USB - the PC host should be able to see the device from now
    USB.USB_CTRL = MC9328MXL_USB::USB_CTRL_SPD | MC9328MXL_USB::USB_CTRL_ENA | MC9328MXL_USB::USB_CTRL_AFE_ENA;

}

void MC9328MXL_USB_Driver::StopHardware()
{
    MC9328MXL_USB & USB = MC9328MXL::USB();
    
    // turn off the UDC core , the clocks are handled by the system
    // Note that this also disables the USB bus driver which tells the host that
    // the device has been disconnected.
    USB.USB_CTRL &= ~(MC9328MXL_USB::USB_CTRL_ENA | MC9328MXL_USB::USB_CTRL_AFE_ENA);
}

void MC9328MXL_USB_Driver::TxPacket( USB_CONTROLLER_STATE* State, int endpoint )
{
    ASSERT(State && endpoint < MC9328MXL_USB::c_USB_MAX_EP);    

    MC9328MXL_USB& USB = MC9328MXL::USB();
    
    GLOBAL_LOCK(irq); 
    
    // transmit a packet on UsbPortNum, if there are no more packets to transmit, then die    
    USB_PACKET64* Packet64;

    // If this is not a legal transmit endpoint, there is nothing to do
    if( State->Queues[endpoint] == NULL || !State->IsTxQueue[endpoint] )
        return;

    for(;;)
    {
        Packet64 = USB_TxDequeue( State, endpoint, TRUE );

        if(Packet64 == NULL || Packet64->Size > 0) break;
    }
    
    if(Packet64)
    {
        int i;
        
        ASSERT(((USB.EP[endpoint].EPn_STAT & MC9328MXL_USB::EndPoint::EPn_STAT_BYTE_CNT) >> MC9328MXL_USB::EndPoint::EPn_STAT_BYTE_CNT_shift) == 0);

        //reset the zlps bit, there is more data to send.
        USB.EP[endpoint].EPn_STAT &= ~MC9328MXL_USB::EndPoint::EPn_STAT_ZLPS;

        // fill fifo
        for(i = 0; i < Packet64->Size - 1; i++)
        {            
            *(volatile UINT8 *)((UINT32)&(USB.EP[endpoint].EPn_FDAT))= Packet64->Buffer[i];
        }

        // sending last byte of data, toggle bit are handle by the hardware
        USB.EP[endpoint].EPn_FCTRL |= MC9328MXL_USB::EndPoint::EPn_FCTRL_WFR;
        *(volatile UINT8 *)((UINT32 )&USB.EP[endpoint].EPn_FDAT)= Packet64->Buffer[i];        
                
        g_MC9328MXL_USB_Driver.TxNeedZLPS[endpoint] = (Packet64->Size == State->MaxPacketSize[endpoint]);
    }
    else
    {
        // send the zero length packet since we landed on the FIFO boundary before
        // (and we queued a zero length packet to transmit) 
        if(g_MC9328MXL_USB_Driver.TxNeedZLPS[endpoint])
        {
            USB.EP[endpoint].EPn_STAT |= MC9328MXL_USB::EndPoint::EPn_STAT_ZLPS;
            g_MC9328MXL_USB_Driver.TxNeedZLPS[endpoint] = FALSE;
        }

        // no more data
        g_MC9328MXL_USB_Driver.TxRunning[endpoint] = FALSE;
    }
}

void MC9328MXL_USB_Driver::ControlNext()
{
    ASSERT(g_MC9328MXL_USB_Driver.pControllerState);
    MC9328MXL_USB& USB = MC9328MXL::USB();

    USB_CONTROLLER_STATE *State = g_MC9328MXL_USB_Driver.pControllerState;

    if(State->DataCallback)
    {
        /* this call can't fail */
        State->DataCallback( State );


        if (State->DataSize ==0)
        {
            USB.EP[0].EPn_STAT |= MC9328MXL_USB::EndPoint::EPn_STAT_ZLPS;
        }
        else
        {
            int i;
            
            for( i = 0; i < State->DataSize - 1; i++ )
            {
                *(volatile UINT8 *)((UINT32 )&(USB.EP[0].EPn_FDAT))= State->Data[i];
            }

            USB.EP[0].EPn_FCTRL |= MC9328MXL_USB::EndPoint::EPn_FCTRL_WFR;
            *(volatile UINT8 *)((UINT32 )&USB.EP[0].EPn_FDAT)= State->Data[i];        

            // special handling the USB driver set address test, cannot use the first descriptor as the ADDRESS state is handle in the hardware
            if(g_MC9328MXL_USB_Driver.FirstDescriptorPacket)
            {
                USB.USB_CTRL |=MC9328MXL_USB::USB_CTRL_CMD_OVER;
                State->DataCallback = NULL;
            }

       }
    }
    else
    {
        // no more packets, we're done with the transfer
        // re-enable RX and we're done.
        USB.USB_CTRL |=MC9328MXL_USB::USB_CTRL_CMD_OVER;
    }

}

void MC9328MXL_USB_Driver::Global_ISR( void* Param )
{    
    MC9328MXL_USB& USB = MC9328MXL::USB();

    UINT32 USB_INTR = USB.USB_INTR & (~USB.USB_MASK);
    
    if (USB_INTR & MC9328MXL_USB::USB_INTR_CFG_CHG)
    {
        USB_CONTROLLER_STATE *State = g_MC9328MXL_USB_Driver.pControllerState;
        USB.USB_INTR |= USB.USB_INTR_CFG_CHG;

        // setup the Configure packet
        RequestPacket.bmRequestType = 0;
        RequestPacket.bRequest = USB_SET_CONFIGURATION;
        RequestPacket.wValue = (USB.USB_STAT & MC9328MXL_USB::USB_STAT_CFG )>> MC9328MXL_USB::USB_STAT_CFG_shift;
        RequestPacket.wIndex = 0;
        RequestPacket.wLength = 0;
        // when get CFG_CHG, it should already be in Address state
        State->DeviceState = USB_DEVICE_STATE_ADDRESS;
        USB_HandleSetConfiguration(g_MC9328MXL_USB_Driver.pControllerState, &RequestPacket, FALSE);
        /* USB spec 9.4.5 SET_CONFIGURATION resets halt conditions, resets toggle bits */
        // it is handled by the hardware and the toggle bit is handle by hardware 

        // Start all active output queues
        for( int ep = 0; ep < MC9328MXL_USB::c_USB_MAX_EP; ep++ )
        {
            if( State->Queues[ep] && State->IsTxQueue[ep] )       // If this is an active transmit endpoint
                StartOutput( State, ep );
        }        
    }
        
    if (USB_INTR &  MC9328MXL_USB::USB_INTR_RES)
    {
        USB.USB_INTR |= USB.USB_INTR_RES;
        ResumeEvent();
    }

    if (USB_INTR &  MC9328MXL_USB::USB_INTR_RESET_START) 
    {
        USB.USB_INTR |=USB.USB_INTR_RESET_START;
    }

    if (USB_INTR &  MC9328MXL_USB::USB_INTR_RESET_STOP)
    {
        USB.USB_INTR |=USB.USB_INTR_RESET_STOP;
        ResetEvent();
    }

    if (USB_INTR &  MC9328MXL_USB::USB_INTR_SUSP)
    {
        USB.USB_INTR |=USB.USB_INTR_SUSP;
        SuspendEvent();
    }
        
    if (USB_INTR &  MC9328MXL_USB::USB_INTR_WAKEUP)
    {
        USB.USB_INTR |=USB.USB_INTR_WAKEUP;
        ResumeEvent();
     }

    // the following interrupts are not enabled. If triggered =>ERR0R
    if (USB_INTR & MC9328MXL_USB::USB_INTR_MSOF)
    {
        USB.USB_INTR |= USB.USB_INTR_MSOF;
        ASSERT(0);
    }

    if (USB_INTR & MC9328MXL_USB::USB_INTR_SOF)
    {
        USB.USB_INTR |=USB.USB_INTR_SOF;
        ASSERT(0);
    }

    if (USB_INTR & MC9328MXL_USB::USB_INTR_FR_MATCH)
    {
        USB.USB_INTR |= USB.USB_INTR_FR_MATCH;
        ASSERT(0);
    }

}



void MC9328MXL_USB_Driver::EP0_ISR( void* Param )
{
    UINT32 i;
    UINT32 EP_INTR;

    MC9328MXL_USB& USB = MC9328MXL::USB();

    EP_INTR = USB.EP[0].EPn_INTR & (~USB.EP[0].EPn_MASK);

    // Multiple setup command found =>error
    if(EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_MDEVREQ)
    {
        USB.EP[0].EPn_INTR |=MC9328MXL_USB::EndPoint::EPn_INTR_MDEVREQ;

        UINT8 len =(USB.EP[0].EPn_STAT & MC9328MXL_USB::EndPoint::EPn_STAT_BYTE_CNT) >> MC9328MXL_USB::EndPoint::EPn_STAT_BYTE_CNT_shift   ;
        UINT8 data8;
        // clear the buffer and discard it
        for(i=0; i<len; i++)
        {
            // read one byte each time, instead of 4 bytes
             data8= *(volatile UINT8 *)((UINT32)&USB.EP[0].EPn_FDAT);
        }
    }


    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_EOT)
    {
        USB.EP[0].EPn_INTR |=MC9328MXL_USB::EndPoint::EPn_INTR_EOT;
        USB.USB_CTRL |=MC9328MXL_USB::USB_CTRL_CMD_OVER;
    }
    
    // set up packet receive
    if((EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_DEVREQ) && (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_EOF))
    {
        // read data 

        USB_CONTROLLER_STATE *State = g_MC9328MXL_USB_Driver.pControllerState;
        UINT8 len =(USB.EP[0].EPn_STAT & MC9328MXL_USB::EndPoint::EPn_STAT_BYTE_CNT) >> MC9328MXL_USB::EndPoint::EPn_STAT_BYTE_CNT_shift   ;
        /* copy in packet */
        for( i=0; i<len; i++)
        {
            // read one byte each time, instead of 4 bytes
            g_MC9328MXL_USB_Driver.ControlPacketBuffer[i] = *(volatile UINT8 *)((UINT32)&USB.EP[0].EPn_FDAT);
        }
        // clear interrupt
        USB.EP[0].EPn_INTR |= (MC9328MXL_USB::EndPoint::EPn_INTR_DEVREQ | MC9328MXL_USB::EndPoint::EPn_INTR_EOF);

        // special handling for the very first SETUP command - Getdescriptor[DeviceType], the host looks for 8 bytes data only
        USB_SETUP_PACKET* Setup= (USB_SETUP_PACKET* )&g_MC9328MXL_USB_Driver.ControlPacketBuffer[0];
        if((Setup->bRequest == USB_GET_DESCRIPTOR) && (((Setup->wValue & 0xFF00) >> 8) == USB_DEVICE_DESCRIPTOR_TYPE) &&(Setup->wLength != 0x12))
            g_MC9328MXL_USB_Driver.FirstDescriptorPacket = TRUE;
        else
            g_MC9328MXL_USB_Driver.FirstDescriptorPacket = FALSE;

        /* send it to the upper layer */
        State->Data     = &g_MC9328MXL_USB_Driver.ControlPacketBuffer[0];
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
            //
            // setup packet failed to process successfully
            // set stall condition on the default control
            // endpoint
            //
            // Stall condition has been ignored in the past, however, it should be done as follows:
            // USB.USB_CTRL |= MC9328MXL_USB::USB_CTRL_CMD_OVER | MC9328MXL_USB::USB_CTRL_CMD_ERROR;
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

            for( int endpoint = 1; endpoint < MC9328MXL_USB::c_USB_MAX_EP; endpoint++ )
            {
                // If this is an active transmit endpoint
                if( State->Queues[endpoint] && State->IsTxQueue[endpoint] )
                    StartOutput( State, endpoint );
            }
            // if debug port is USB, then output all the logged data
//            if(result == USB_STATE_CONFIGURATION && COM_IsUsb(HalSystemConfig.DebugTextPort))
//            {
//                StartOutput( ConvertCOM_UsbStream( HalSystemConfig.DebugTextPort ) );
//            }
        }
    }
    else if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_DEVREQ)
    {
        USB.EP[0].EPn_INTR |=MC9328MXL_USB::EndPoint::EPn_INTR_DEVREQ;
    }
    else if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_EOF)
    {
        USB.EP[0].EPn_INTR |=MC9328MXL_USB::EndPoint::EPn_INTR_EOF;
        ControlNext();
    }

    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_ERROR)
    {
        USB.EP[0].EPn_INTR |=MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_ERROR;
        // The FIFO should not be happened.
        USB_debug_printf("FIFO status %x \r\n",USB.EP[0].EPn_FSTAT);
        ASSERT(0);
    }
  

    // the following interrupts are not activate
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_FULL)
    {
        USB.EP[0].EPn_INTR |= MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_FULL;
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_EMPTY)
    {
        USB.EP[0].EPn_INTR |=MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_EMPTY;
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_HIGH)
    {
        USB.EP[0].EPn_INTR |=MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_HIGH;
        ASSERT(0);
    }
    if( EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_LOW)
    {
        USB.EP[0].EPn_INTR |=MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_LOW;
        ASSERT(0);
    }
}

void MC9328MXL_USB_Driver::EP_TxISR( void* Param )
{
    ASSERT_IRQ_MUST_BE_OFF();

    UINT32 EP_INTR;
    
    UINT32 EPno = (UINT32)Param;

    MC9328MXL_USB& USB = MC9328MXL::USB();

    EP_INTR = USB.EP[EPno].EPn_INTR & (~USB.EP[EPno].EPn_MASK);

    if  (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_EOF)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_EOF;

        // successfully transmitted packet, time to send the next one 
        TxPacket( g_MC9328MXL_USB_Driver.pControllerState, EPno );
        
        //Can this really be right?  Why is the Tx endpoint dealing with control transfers.
        USB.USB_CTRL |=MC9328MXL_USB::USB_CTRL_CMD_OVER;            
    }

    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_EOT)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_EOT;
    }

    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_ERROR)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_FIFO_ERROR;
        // The FIFO should not be happened.
        USB_debug_printf("FIFO status %x \r\n",USB.EP[EPno].EPn_FSTAT);
        ASSERT(0);
    }


    // the following interrupts are not activate
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_FULL)
    {
        USB.EP[EPno].EPn_INTR |= (MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_FULL);
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_EMPTY)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_FIFO_EMPTY;
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_HIGH)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_FIFO_HIGH;
        ASSERT(0);
    }
    if( EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_LOW)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_FIFO_LOW;
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_MDEVREQ)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_MDEVREQ;
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_DEVREQ)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_DEVREQ;
        ASSERT(0);
    }
}

void MC9328MXL_USB_Driver::EP_RxISR( void* Param )
{
    UINT32 EP_INTR;
    
    UINT32 EPno = (UINT32)Param;
    
    MC9328MXL_USB& USB = MC9328MXL::USB();

    EP_INTR = USB.EP[EPno].EPn_INTR & (~USB.EP[EPno].EPn_MASK);

    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_EOT)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_EOT;
    }

    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_EOF)
    {
        BOOL          DisableRx;
        USB_PACKET64* Packet64 = USB_RxEnqueue( g_MC9328MXL_USB_Driver.pControllerState, EPno, DisableRx );

        /* copy packet in, making sure that Packet64->Buffer is never overflowed */
        if(Packet64)
        {
            // clear the interrupt first, make sure no lost of next frame Interrupt
            USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_EOF;

            UINT8 len = ( USB.EP[EPno].EPn_STAT& MC9328MXL_USB::EndPoint::EPn_STAT_BYTE_CNT) >>  MC9328MXL_USB::EndPoint::EPn_STAT_BYTE_CNT_shift;

            for(int i=0; i<len; i++)
            {
                Packet64->Buffer[ i] = *(volatile UINT8 *)((UINT32)&USB.EP[EPno].EPn_FDAT);
            }
            
            UINT32 state = USB.EP[EPno].EPn_FSTAT;

            Packet64->Size = len;
        }
        else
        {
            /* flow control should absolutely protect us from ever
            getting here, so if we do, it is a bug */
            USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_EOF;
            
            ASSERT(0);
            return;
        }

        if(DisableRx)
        {
            /* if there is no room left, then we mask out the receive event
            for this fifo */
            USB.EP[EPno].EPn_MASK = MC9328MXL_USB::EndPoint::EPn_MASK_ALL_OFF;
        }

//TBD
        //Can this really be right?  Why is the Rx endpoint dealing with control transfers.
        USB.USB_CTRL |=MC9328MXL_USB::USB_CTRL_CMD_OVER;
    
    }

    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_ERROR)
    {
        USB.EP[EPno].EPn_INTR |= USB.EP[EPno].EPn_INTR_FIFO_ERROR;
        // The FIFO should not be happened.
        USB_debug_printf("error:  status %x \r\n",USB.EP[EPno].EPn_FSTAT);
        ASSERT(0);
    }

    // the following interrupts are not activate
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_FULL)
    {
        USB.EP[EPno].EPn_INTR |= (MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_FULL);
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_EMPTY)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_FIFO_EMPTY;
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_HIGH)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_FIFO_HIGH;
        ASSERT(0);
    }
    if( EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_FIFO_LOW)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_FIFO_LOW;
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_MDEVREQ)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_MDEVREQ;
        ASSERT(0);
    }
    if (EP_INTR & MC9328MXL_USB::EndPoint::EPn_INTR_DEVREQ)
    {
        USB.EP[EPno].EPn_INTR |=USB.EP[EPno].EPn_INTR_DEVREQ;
        ASSERT(0);
    }
}

void MC9328MXL_USB_Driver::SuspendEvent()
{

    ASSERT(g_MC9328MXL_USB_Driver.pControllerState);

    MC9328MXL_USB& USB = MC9328MXL::USB();

    // SUSPEND event only happened when Host(PC) set the device to SUSPEND
    // as there is always SOF every 1ms on the BUS to keep the device from
    // suspending. Therefore, the REMOTE wake up is not necessary at the device side

    USB.USB_MASK = (MC9328MXL_USB::USB_MASK_WAKEUP & MC9328MXL_USB::USB_MASK_RESET_STOP & MC9328MXL_USB::USB_MASK_RESET_START & MC9328MXL_USB::USB_MASK_RES & MC9328MXL_USB::USB_MASK_CFG_CHG  );

    g_MC9328MXL_USB_Driver.PreviousDeviceState = g_MC9328MXL_USB_Driver.pControllerState->DeviceState;

    g_MC9328MXL_USB_Driver.pControllerState->DeviceState = USB_DEVICE_STATE_SUSPENDED;

    USB_StateCallback( g_MC9328MXL_USB_Driver.pControllerState );

#if defined(DEBUG_USB)
    USB_debug_printf("In suspend event previous %d\n\r", g_MC9328MXL_USB_Driver.PreviousDeviceState);
    lcd_printf("In suspend \n\r");
#endif

}


void MC9328MXL_USB_Driver::ResumeEvent()
{
    ASSERT(g_MC9328MXL_USB_Driver.pControllerState);

    MC9328MXL_USB& USB = MC9328MXL::USB();

#if defined(DEBUG_USB)
    USB_debug_printf(" in Resume\r\n");
#endif

    USB.USB_MASK = MC9328MXL_USB::USB_MASK_RESET_STOP  & 
                   MC9328MXL_USB::USB_MASK_RESET_START & 
                   MC9328MXL_USB::USB_MASK_SUSP        & 
                   MC9328MXL_USB::USB_MASK_CFG_CHG     ;

    g_MC9328MXL_USB_Driver.pControllerState->DeviceState = g_MC9328MXL_USB_Driver.PreviousDeviceState;

    USB_StateCallback( g_MC9328MXL_USB_Driver.pControllerState );
}



void MC9328MXL_USB_Driver::ResetEvent()
{
    // the hardware handle most of the reset procedure.

    USB_CONTROLLER_STATE *State = g_MC9328MXL_USB_Driver.pControllerState;
    
    ASSERT(State);
    
    MC9328MXL_USB& USB = MC9328MXL::USB();

    // flush whatever in the FIFO
    for(int ep = 0; ep < MC9328MXL_USB::c_USB_MAX_EP; ep++)
    {
        USB.EP[ep].EPn_STAT |= MC9328MXL_USB::EndPoint::EPn_STAT_FLUSH;
    }

    USB.USB_MASK = (MC9328MXL_USB::USB_MASK_RESET_STOP & MC9328MXL_USB::USB_MASK_RESET_START & MC9328MXL_USB::USB_MASK_SUSP & MC9328MXL_USB::USB_MASK_CFG_CHG  );

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

    for(int ep = 0; ep < MC9328MXL_USB::c_USB_MAX_EP; ep++)
    {
        g_MC9328MXL_USB_Driver.TxRunning [ep] = FALSE;
        g_MC9328MXL_USB_Driver.TxNeedZLPS[ep] = FALSE;
    }


    State->DeviceState        = USB_DEVICE_STATE_DEFAULT;
    State->Address            = 0;
    USB_StateCallback( State );
}

//--//

BOOL MC9328MXL_USB_Driver::RxEnable( USB_CONTROLLER_STATE* State, int endpoint )
{
    MC9328MXL_USB& USB = MC9328MXL::USB();

    // If this is not a legal Rx queue
    if( State == NULL || State->Queues[endpoint] == NULL || State->IsTxQueue[endpoint] )
        return FALSE;

    GLOBAL_LOCK(irq);

    /* unmask receive event */
    USB.EP[endpoint].EPn_MASK = MC9328MXL_USB::EndPoint::EPn_MASK_EOT & MC9328MXL_USB::EndPoint::EPn_MASK_EOF & MC9328MXL_USB::EndPoint::EPn_MASK_FIFO_ERROR;
    
    return TRUE;
}

BOOL MC9328MXL_USB_Driver::ProtectPins( int Controller, BOOL On )
{
    ASSERT( Controller == 0 );

    USB_CONTROLLER_STATE *State = g_MC9328MXL_USB_Driver.pControllerState;
    
    GLOBAL_LOCK(irq);

    // initialized yet?
    if( State )
    {
    //hal_fprintf( STREAM_LCD,"in Protect Pin - ON %d\n\r",On);

        if(On)
        {
            if(!g_MC9328MXL_USB_Driver.PinsProtected)
            {
                // Disable the USB com, state change from Not protected to Protected

                g_MC9328MXL_USB_Driver.PinsProtected = TRUE;
                // clear USB Txbuffer

                for(int ep = 1; ep < MC9328MXL_USB::c_USB_MAX_EP; ep++)
                {
                    // If actual transmit queue
                    if( State->Queues[ep] && State->IsTxQueue[ep] )
                        ClearTxQueue( State, ep );
                }

                // stop clock, disable interrupt, dettach USB port
                // set the USB Controller->DeviceState = DETACHED

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
            if(g_MC9328MXL_USB_Driver.PinsProtected)
            {
                // Ready for USB to enable, state change from Protected to Not protected
                g_MC9328MXL_USB_Driver.PinsProtected = FALSE;

                // enable the clock,
                // set USB to attached/powered
                // set the device to a known state- Attached before it is set to the powered state (USB specf 9.1.1)
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
