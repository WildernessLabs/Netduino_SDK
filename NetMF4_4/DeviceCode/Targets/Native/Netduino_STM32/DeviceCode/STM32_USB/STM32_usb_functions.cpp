/////////////////////////////////////////////////////////////////////////////
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
//  Copyright (c) Microsoft Corporation. All rights reserved.
//  Portions Copyright (c) Oberon microsystems, Inc.
//  Portions Copyright (c) Secret Labs LLC and the Netduino community. All rights reserved.
//
//  *** USB OTG Full Speed Device Mode Driver ***
//
/////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <pal\com\usb\USB.h>
#if defined(PLATFORM_ARM_STM32F4_ANY)
#include "..\stm32f4xx.h"
#elif defined(PLATFORM_ARM_STM32F2_ANY)
#include "..\stm32f2xx.h"
#endif
#include "usbd_netmf_core.h"
#include "..\STM32_GPIO\STM32_GPIO_functions.h"
#include "..\STM32_RCC\STM32_RCC_functions.h"

// State variables for the controller (one controller only)
USB_CONTROLLER_STATE STM32_USB_ControllerState;

#define STM32_USB_CLEAR_IN_EP_INTR(epnum,intr) \
  diepint.d32 = 0; \
  diepint.b.intr = 1; \
  USB_OTG_WRITE_REG32(&USB_OTG_dev.regs.INEP_REGS[epnum]->DIEPINT, diepint.d32);

#define STM32_USB_CLEAR_OUT_EP_INTR(epnum,intr) \
  doepint.d32 = 0; \
  doepint.b.intr = 1; \
  USB_OTG_WRITE_REG32(&USB_OTG_dev.regs.OUTEP_REGS[epnum]->DOEPINT, doepint.d32);

#define USB_Debug(x)

USB_OTG_CORE_HANDLE USB_OTG_dev;

static UINT8 USBD_DataOutStage(USB_OTG_CORE_HANDLE *pdev , UINT8 epnum)
{
  USB_OTG_EP *ep;
  
  if(epnum == 0) 
  {
    ep = &pdev->dev.out_ep[0];
    if ( pdev->dev.device_state == USB_OTG_EP0_DATA_OUT)
    {
      if(ep->rem_data_len > ep->maxpacket)
      {
        ep->rem_data_len -=  ep->maxpacket;
      
        STM32_USB_CtlContinueRx(pdev, ep->xfer_buff, MIN(ep->rem_data_len, ep->maxpacket));
      }
      else
      {
        if((pdev->dev.class_cb->EP0_RxReady != NULL)&& (pdev->dev.device_status == USB_OTG_CONFIGURED))
        {
          pdev->dev.class_cb->EP0_RxReady(pdev); 
        }
        STM32_USB_CtlSendStatus(pdev);
      }
    }
  }
  else if((pdev->dev.class_cb->DataOut != NULL)&& (pdev->dev.device_status == USB_OTG_CONFIGURED))
  {
    pdev->dev.class_cb->DataOut(pdev, epnum); 
  }
  
  return USBD_OK;
}

static UINT8 USBD_DataInStage(USB_OTG_CORE_HANDLE *pdev , UINT8 epnum)
{
  USB_OTG_EP *ep;
  
  if(epnum == 0) 
  {
    ep = &pdev->dev.in_ep[0];
    if ( pdev->dev.device_state == USB_OTG_EP0_DATA_IN)
    {
      if(ep->rem_data_len > ep->maxpacket)
      {
        ep->rem_data_len -=  ep->maxpacket;

        STM32_USB_CtlContinueSendData(pdev, ep->xfer_buff, ep->rem_data_len);
      }
      else
      { /* last packet is MPS multiple, so send ZLP packet */
        if((ep->total_data_len % ep->maxpacket == 0) &&
           (ep->total_data_len >= ep->maxpacket) &&
             (ep->total_data_len < ep->ctl_data_len ))
        {
          
          STM32_USB_CtlContinueSendData(pdev , NULL, 0);
          ep->ctl_data_len = 0;
        }
        else
        {
          if((pdev->dev.class_cb->EP0_TxSent != NULL)&&
             (pdev->dev.device_status == USB_OTG_CONFIGURED))
          {
            pdev->dev.class_cb->EP0_TxSent(pdev); 
          }          
          STM32_USB_CtlReceiveStatus(pdev);
        }
      }
    }
  }
  else if((pdev->dev.class_cb->DataIn != NULL)&& (pdev->dev.device_status == USB_OTG_CONFIGURED))
  {
    pdev->dev.class_cb->DataIn(pdev, epnum); 
  }
  
  return USBD_OK;
}

static UINT8 STM32_USB_HandleSetupPacket(USB_OTG_CORE_HANDLE *pdev)
{
    USB_SETUP_PACKET request;
  
    STM32_USB_ParseSetupRequest(pdev , &request);
  
	//STM32_USB_ControllerState.Data = pdev->dev.setup_packet;
	//STM32_USB_ControllerState.DataSize = request.wLength;

    switch (request.bmRequestType & 0x1F) 
    {
        case USB_SETUP_RECIPIENT_DEVICE:   
            STM32_USB_StandardDeviceRequest(pdev, &request);
            break;

        case USB_SETUP_RECIPIENT_INTERFACE:     
            STM32_USB_StandardInterfaceRequest(pdev, &request);
            break;

        case USB_SETUP_RECIPIENT_ENDPOINT:        
            STM32_USB_StandardEndpointRequest(pdev, &request);   
            break;

        default:
            // TODO: how is endpoint number coming from request type.
            STM32_USB_EP_Stall(pdev , request.bmRequestType & 0x80);
            break;
    }
  
    return USBD_OK;
}

static UINT8 USBD_Reset(USB_OTG_CORE_HANDLE  *pdev)
{
  /* Open EP0 OUT */
  STM32_USB_EP_Open(pdev, 0x00, USB_OTG_MAX_EP0_SIZE, EP_TYPE_CTRL);
  
  /* Open EP0 IN */
  STM32_USB_EP_Open(pdev, 0x80, USB_OTG_MAX_EP0_SIZE, EP_TYPE_CTRL);
  
  /* Upon Reset call usr call back */
  pdev->dev.device_status = USB_OTG_DEFAULT;
  pdev->dev.usr_cb->DeviceReset(pdev->cfg.speed);
  
  return USBD_OK;
}

static UINT8 USBD_Suspend(USB_OTG_CORE_HANDLE  *pdev)
{
  
  pdev->dev.device_status  = USB_OTG_SUSPENDED;
  /* Upon Resume call usr call back */
  pdev->dev.usr_cb->DeviceSuspended(); 
  return USBD_OK;
}

static UINT8 USBD_Resume(USB_OTG_CORE_HANDLE  *pdev)
{
  /* Upon Resume call usr call back */
  pdev->dev.usr_cb->DeviceResumed(); 
  pdev->dev.device_status = USB_OTG_CONFIGURED;  
  return USBD_OK;
}

USBD_DCD_INT_cb_TypeDef USBD_DCD_INT_cb = 
{
  USBD_DataOutStage,
  USBD_DataInStage,
  STM32_USB_HandleSetupPacket,
  USBD_Reset,
  USBD_Suspend,
  USBD_Resume,
};

USBD_DCD_INT_cb_TypeDef  *USBD_DCD_INT_fops = &USBD_DCD_INT_cb;

//////////////////////////////////////////////////////////////

#ifdef DEBUG
#define _ASSERT(x) ASSERT(x)
#else
#define _ASSERT(x)
#endif

#define USB_MAX_EP USB_MAX_QUEUES
#define MAX_EP0_SIZE 8

// Queues for all data endpoints
Hal_Queue_KnownSize<USB_PACKET64,USB_QUEUE_PACKET_COUNT> QueueBuffers[USB_MAX_EP - 1];

// Auxiliary state variables
static UINT16 STM32_USB_EndpointStatus[USB_MAX_EP];
static BOOL STM32_USB_PinsProtected;
static UINT16 STM32_USB_EP_Type = 0;

void uDelay (const UINT32 usec)
{
  UINT32 count = 0;
  const UINT32 utime = (120 * usec / 7);
  do
  {
    if ( ++count > utime )
    {
      return ;
    }
  }
  while (1);
}

void mDelay (const UINT32 msec)
{
  uDelay(msec * 1000);   
}

void STM32_USB_StartWrite(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
	USB_OTG_DEPCTL_TypeDef     depctl;
	USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;
	UINT32 fifoemptymsk = 0;
  
	depctl.d32 = 0;
	deptsiz.d32 = 0;

    depctl.d32  = USB_OTG_READ_REG32(&(pdev->regs.INEP_REGS[ep->num]->DIEPCTL));
    deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.INEP_REGS[ep->num]->DIEPTSIZ));
    /* Zero Length Packet? */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = 0;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      /* Program the transfer size and packet count
      * as follows: xfersize = N * maxpacket +
      * short_packet pktcnt = N + (short_packet
      * exist ? 1 : 0)
      */
      deptsiz.b.xfersize = ep->xfer_len;
      deptsiz.b.pktcnt = (ep->xfer_len - 1 + ep->maxpacket) / ep->maxpacket;    
    }
    USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPTSIZ, deptsiz.d32);

    /* Enable the Tx FIFO Empty Interrupt for this EP */
    if (ep->xfer_len > 0)
    {
      fifoemptymsk = 1 << ep->num;
      USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, 0, fifoemptymsk);
    }
    
    /* EP enable, IN data in FIFO */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPCTL, depctl.d32);   
}

void STM32_USB_StartRead(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
	  USB_OTG_DEPCTL_TypeDef     depctl;
  USB_OTG_DEPXFRSIZ_TypeDef  deptsiz;    
  //UINT32 fifoemptymsk = 0;  
  
  depctl.d32 = 0;
  deptsiz.d32 = 0;

	/* OUT endpoint */
    depctl.d32  = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL));
    deptsiz.d32 = USB_OTG_READ_REG32(&(pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ));

    /* Program the transfer size and packet count as follows:
    * pktcnt = N
    * xfersize = N * maxpacket
    */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = ep->maxpacket;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      deptsiz.b.pktcnt = (ep->xfer_len + (ep->maxpacket - 1)) / ep->maxpacket;
      deptsiz.b.xfersize = deptsiz.b.pktcnt * ep->maxpacket;
    }
    USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ, deptsiz.d32);

    /* EP enable */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL, depctl.d32);
}

USB_OTG_STS STM32_USB_EndpointXStartTransfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  /* IN endpoint */
  if (ep->is_in == 1)
  {
	  STM32_USB_StartWrite(pdev, ep);
  }
  else
  {
	  STM32_USB_StartRead(pdev, ep);
  }
  
  return USB_OTG_OK;
}

//USB_OTG_STS STM32_USB_Endpoint0StartTransfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
//{
//  /* IN endpoint */
//  if (ep->is_in == 1)
//  {
//	  STM32_USB_StartWrite(pdev, ep);
//  }
//  else
//  {
//	  STM32_USB_StartRead(pdev, ep);
//  }
//  return status;
//}

void STM32_USB_EndpointTransmit(UINT8 endpointAddress, UINT8 *buffer, UINT32 length)
{
  USB_OTG_EP *ep;
  
  ep = &USB_OTG_dev.dev.in_ep[endpointAddress & 0x7F];
  
  /* Setup and start the transfer */
  ep->is_in = 1;
  ep->num = endpointAddress & 0x7F;  
  ep->xfer_buff = buffer;
  ep->xfer_count = 0;
  ep->xfer_len  = length;
  
  //if ( ep->num == 0 )
  //{
  //  STM32_USB_Endpoint0StartTransfer(&USB_OTG_dev, ep);
  //}
  //else
  //{
    STM32_USB_EndpointXStartTransfer(&USB_OTG_dev, ep);
  //}    
}

void STM32_USB_EndpointReceive(UINT8 endpointAddress, UINT8 *buffer, UINT16 length)
{
  USB_OTG_EP *ep;
  
  ep = &USB_OTG_dev.dev.out_ep[endpointAddress & 0x7F];
  
  /*setup and start the Xfer */
  ep->is_in = 0;
  ep->num = endpointAddress & 0x7F;  
  ep->xfer_buff = buffer;  
  ep->xfer_count = 0;
  ep->xfer_len = length;

  //if ( ep->num == 0 )
  //{
  //  STM32_USB_Endpoint0StartTransfer(&USB_OTG_dev , ep);
  //}
  //else
  //{
    STM32_USB_EndpointXStartTransfer(&USB_OTG_dev, ep);
  //}    
}

//
// Data In (Tx) Endpoint Interrupt Handler
//
void STM32_USB_Driver_EP_In_Int(UINT32 , USB_CONTROLLER_STATE* State, int endpoint)
{
  ASSERT_IRQ_MUST_BE_OFF();

  // If this is not a legal transmit endpoint, there is nothing more to do
  if((State->Queues[endpoint] != NULL) && State->IsTxQueue[endpoint])
  {
    USB_PACKET64* Packet64 = USB_TxDequeue(State, endpoint, TRUE);
    if(Packet64)
    {
      // More data to send
      // copy buffer
      // FIXME: Optimize, both buffers are always 64 bytes long
      int count = Packet64->Size;
      for(int i = 0; i < count; i++)
      {
        APP_Rx_Buffer[i] = Packet64->Buffer[i];
      }

      STM32_USB_EndpointTransmit(NETMF_IN_EP, (UINT8*)APP_Rx_Buffer, count);
    }
  }
}

USB_OTG_STS STM32_USB_WritePacket(USB_OTG_CORE_HANDLE *pdev, UINT8 *src, UINT8 ch_ep_num, UINT16 len)
{
  UINT32 count32b= 0 , i= 0;
  __IO UINT32 *fifo;

  count32b =  (len + 3) / 4;
  fifo = pdev->regs.DFIFO[ch_ep_num];
  for (i = 0; i < count32b; i++, src+=4)
  {
    USB_OTG_WRITE_REG32( fifo, *((__packed UINT32 *)src) );
  }
  
  return USB_OTG_OK;
}

void *STM32_USB_ReadPacket(USB_OTG_CORE_HANDLE *pdev, UINT8 *dest, UINT16 len)
{
  UINT32 i=0;
  UINT32 count32b = (len + 3) / 4;
  
  __IO UINT32 *fifo = pdev->regs.DFIFO[0];
  
  for ( i = 0; i < count32b; i++, dest += 4 )
  {
    *(ADS_PACKED UINT32 *)dest = USB_OTG_READ_REG32(fifo);
  }
  return ((void *)dest);
}

void STM32_USB_HandleReceiveStatusQueueLevelInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_GINTMSK_TypeDef  int_mask;
  USB_OTG_DRXSTS_TypeDef   status;
  USB_OTG_EP *ep;
  
  /* Disable the Rx Status Queue Level interrupt */
  int_mask.d32 = 0;
  int_mask.b.rxstsqlvl = 1;
  USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, int_mask.d32, 0);
  
  /* Get the Status from the top of the FIFO */
  status.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRXSTSP );
  
  ep = &pdev->dev.out_ep[status.b.epnum];
  
  switch (status.b.pktsts)
  {
  case STS_GOUT_NAK:
    break;
  case STS_DATA_UPDT:
    if (status.b.bcnt)
    {
      STM32_USB_ReadPacket(pdev,ep->xfer_buff, status.b.bcnt);
      ep->xfer_buff += status.b.bcnt;
      ep->xfer_count += status.b.bcnt;
    }
    break;
  case STS_XFER_COMP:
    break;
  case STS_SETUP_COMP:
    break;
  case STS_SETUP_UPDT:
    /* Copy the setup packet received in FIFO into the setup buffer in RAM */
    STM32_USB_ReadPacket(pdev , pdev->dev.setup_packet, 8);
    ep->xfer_count += status.b.bcnt;
    break;
  default:
    break;
  }
  
  /* Enable the Rx Status Queue Level interrupt */
  USB_OTG_MODIFY_REG32( &pdev->regs.GREGS->GINTMSK, 0, int_mask.d32);
}

void STM32_USB_WriteEmptyTransferFifo(UINT32 epnum)
{
  USB_OTG_DTXFSTSn_TypeDef  txstatus;
  USB_OTG_EP *ep;
  UINT32 len = 0;
  UINT32 len32b;
  txstatus.d32 = 0;
  
  ep = &USB_OTG_dev.dev.in_ep[epnum];    
  
  len = ep->xfer_len - ep->xfer_count;
  
  if (len > ep->maxpacket)
  {
    len = ep->maxpacket;
  }
  
  len32b = (len + 3) / 4;
  txstatus.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.INEP_REGS[epnum]->DTXFSTS);

  while(txstatus.b.txfspcavail > len32b && ep->xfer_count < ep->xfer_len && ep->xfer_len != 0)
  {
    /* Write the FIFO */
    len = ep->xfer_len - ep->xfer_count;
    
    if (len > ep->maxpacket)
    {
      len = ep->maxpacket;
    }
    len32b = (len + 3) / 4;
    
    STM32_USB_WritePacket (&USB_OTG_dev, ep->xfer_buff, epnum, len);
    
    ep->xfer_buff  += len;
    ep->xfer_count += len;
    
    txstatus.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.INEP_REGS[epnum]->DTXFSTS);
  }
}

UINT32 STM32_USB_ReadDevAllInEPItr(USB_OTG_CORE_HANDLE *pdev)
{
  UINT32 v;
  v = USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINT);
  v &= USB_OTG_READ_REG32(&pdev->regs.DREGS->DAINTMSK);
  
  return (v & 0xffff);
}

void STM32_USB_HandleEndpointInputInterrupt()
{
  USB_OTG_DIEPINTn_TypeDef  diepint;
  
  UINT32 ep_intr;
  UINT32 epnum = 0;
  UINT32 fifoemptymsk;
  diepint.d32 = 0;
  ep_intr = STM32_USB_ReadDevAllInEPItr(&USB_OTG_dev);
  
  while ( ep_intr )
  {
    if (ep_intr & 0x1) /* In ITR */
    {
      //diepint.d32 = DCD_ReadDevInEP(&USB_OTG_dev, epnum); /* Get In ITR status */
      UINT32 msk, emp;
      msk = USB_OTG_READ_REG32(&USB_OTG_dev.regs.DREGS->DIEPMSK);
      emp = USB_OTG_READ_REG32(&USB_OTG_dev.regs.DREGS->DIEPEMPMSK);
      msk |= ((emp >> epnum) & 0x1) << 7;
      diepint.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.INEP_REGS[epnum]->DIEPINT) & msk;
  
      if ( diepint.b.xfercompl )
      {
        fifoemptymsk = 0x1 << epnum;
        USB_OTG_MODIFY_REG32(&USB_OTG_dev.regs.DREGS->DIEPEMPMSK, fifoemptymsk, 0);
        STM32_USB_CLEAR_IN_EP_INTR(epnum, xfercompl);
        /* TX COMPLETE */
        USBD_DCD_INT_fops->DataInStage(&USB_OTG_dev, epnum);          
      }
      
      if ( diepint.b.ahberr )
      {
        STM32_USB_CLEAR_IN_EP_INTR(epnum, ahberr);
      }
      if ( diepint.b.timeout )
      {
        STM32_USB_CLEAR_IN_EP_INTR(epnum, timeout);
      }
      if (diepint.b.intktxfemp)
      {
        STM32_USB_CLEAR_IN_EP_INTR(epnum, intktxfemp);
      }
      if (diepint.b.intknepmis)
      {
        STM32_USB_CLEAR_IN_EP_INTR(epnum, intknepmis);
      }
      if (diepint.b.inepnakeff)
      {
        STM32_USB_CLEAR_IN_EP_INTR(epnum, inepnakeff);
      }
      if ( diepint.b.epdisabled )
      {
        STM32_USB_CLEAR_IN_EP_INTR(epnum, epdisabled);
      }       
      if (diepint.b.emptyintr)
      {        
        STM32_USB_WriteEmptyTransferFifo(epnum);
        
        STM32_USB_CLEAR_IN_EP_INTR(epnum, emptyintr);
      }
    }
    
    epnum++;
    ep_intr >>= 1;
  }    
}

void STM32_USB_HandleEndpointOutputInterrupt()
{
  UINT32 ep_intr;
  USB_OTG_DOEPINTn_TypeDef  doepint;
  UINT32 epnum = 0;
  
  doepint.d32 = 0;
  
  /* Read in the device interrupt bits */
  //ep_intr = USB_OTG_ReadDevAllOutEp_itr(&USB_OTG_dev);
    UINT32 v;
  v  = USB_OTG_READ_REG32(&USB_OTG_dev.regs.DREGS->DAINT);
  v &= USB_OTG_READ_REG32(&USB_OTG_dev.regs.DREGS->DAINTMSK);
  ep_intr = ((v & 0xffff0000) >> 16);

  while ( ep_intr )
  {
    if (ep_intr & 0x1)
    {  
      //doepint.d32 = USB_OTG_ReadDevOutEP_itr(&USB_OTG_dev, epnum);
      UINT32 v;
      v  = USB_OTG_READ_REG32(&USB_OTG_dev.regs.OUTEP_REGS[epnum]->DOEPINT);
      v &= USB_OTG_READ_REG32(&USB_OTG_dev.regs.DREGS->DOEPMSK);
      doepint.d32 = v;

      /* Transfer complete */
      if ( doepint.b.xfercompl )
      {
        /* Clear the bit in DOEPINTn for this interrupt */
        STM32_USB_CLEAR_OUT_EP_INTR(epnum, xfercompl);

        /* Inform upper layer: data ready */
        /* RX COMPLETE */
        USBD_DCD_INT_fops->DataOutStage(&USB_OTG_dev , epnum);       
      }
      /* Endpoint disable  */
      if ( doepint.b.epdisabled )
      {
        /* Clear the bit in DOEPINTn for this interrupt */
        STM32_USB_CLEAR_OUT_EP_INTR(epnum, epdisabled);
      }
      /* AHB Error */
      if ( doepint.b.ahberr )
      {
        STM32_USB_CLEAR_OUT_EP_INTR(epnum, ahberr);
      }

      if ( doepint.b.setup )
      {
        USBD_DCD_INT_fops->SetupStage(&USB_OTG_dev);
        STM32_USB_CLEAR_OUT_EP_INTR(epnum, setup);
      }
    }
    
    epnum++;
    ep_intr >>= 1;
  }    
}

void STM32_USB_HandleEnumerationDoneInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_GINTSTS_TypeDef  gintsts;
  USB_OTG_GUSBCFG_TypeDef  gusbcfg;
  
  //USB_OTG_EP0Activate(pdev);
  USB_OTG_DSTS_TypeDef    dsts;
  USB_OTG_DEPCTL_TypeDef  diepctl;
  USB_OTG_DCTL_TypeDef    dctl;
  
  dctl.d32 = 0;
  /* Read the Device Status and Endpoint 0 Control registers */
  dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
  diepctl.d32 = USB_OTG_READ_REG32(&pdev->regs.INEP_REGS[0]->DIEPCTL);
  /* Set the MPS of the IN EP based on the enumeration speed */
  switch (dsts.b.enumspd)
  {
  case DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ:
  case DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ:
  case DSTS_ENUMSPD_FS_PHY_48MHZ:
    diepctl.b.mps = DEP0CTL_MPS_64;
    break;
  case DSTS_ENUMSPD_LS_PHY_6MHZ:
    diepctl.b.mps = DEP0CTL_MPS_8;
    break;
  }
  USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[0]->DIEPCTL, diepctl.d32);
  dctl.b.cgnpinnak = 1;
  USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, dctl.d32);
 
  
  
  
  /* Set USB turn-around time based on device speed and PHY interface. */
  gusbcfg.d32 = USB_OTG_READ_REG32(&pdev->regs.GREGS->GUSBCFG);
  

    pdev->cfg.speed            = USB_OTG_SPEED_FULL;
    pdev->cfg.mps              = USB_OTG_FS_MAX_PACKET_SIZE ;  
    gusbcfg.b.usbtrdtim = 5;

  
  USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GUSBCFG, gusbcfg.d32);
  
  /* Clear interrupt */
  gintsts.d32 = 0;
  gintsts.bits.enumdne = 1;
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GINTSTS, gintsts.d32 );
}

void STM32_USB_HandleResetInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_DAINT_TypeDef    daintmsk;
  USB_OTG_DOEPMSK_TypeDef  doepmsk;
  USB_OTG_DIEPMSK_TypeDef  diepmsk;
  USB_OTG_DCFG_TypeDef     dcfg;
  USB_OTG_DCTL_TypeDef     dctl;
  USB_OTG_GINTSTS_TypeDef  gintsts;
  UINT32 i;
  
  dctl.d32 = 0;
  daintmsk.d32 = 0;
  doepmsk.d32 = 0;
  diepmsk.d32 = 0;
  dcfg.d32 = 0;
  gintsts.d32 = 0;
  
  /* Clear the Remote Wake-up Signaling */
  dctl.b.rmtwkupsig = 1;
  USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, dctl.d32, 0 );
  
  /* Flush the Tx FIFO */
  //USB_OTG_FlushTxFifo(pdev ,  0 );
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  
  UINT32 count = 0;
  greset.d32 = 0;
  greset.b.txfflsh = 1;
  greset.b.txfnum  = 0;
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GRSTCTL, greset.d32 );
  do
  {
    greset.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      break;
    }
  }
  while (greset.b.txfflsh == 1);
  /* Wait for 3 PHY Clocks*/
  uDelay(3);

  
  
  
  for (i = 0; i < pdev->cfg.dev_endpoints ; i++)
  {
    USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFF);
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFF);
  }
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFF );
  
  daintmsk.ep.in = 1;
  daintmsk.ep.out = 1;
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINTMSK, daintmsk.d32 );
  
  doepmsk.b.setup = 1;
  doepmsk.b.xfercompl = 1;
  doepmsk.b.ahberr = 1;
  doepmsk.b.epdisabled = 1;
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOEPMSK, doepmsk.d32 );

  diepmsk.b.xfercompl = 1;
  diepmsk.b.timeout = 1;
  diepmsk.b.epdisabled = 1;
  diepmsk.b.ahberr = 1;
  diepmsk.b.intknepmis = 1;
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DIEPMSK, diepmsk.d32 );

  /* Reset Device Address */
  dcfg.d32 = USB_OTG_READ_REG32( &pdev->regs.DREGS->DCFG);
  dcfg.b.devaddr = 0;
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DCFG, dcfg.d32);
  
  
  /* setup EP0 to receive SETUP packets */
  STM32_USB_EP0_OutStart(pdev);
  
  /* Clear interrupt */
  gintsts.d32 = 0;
  gintsts.bits.usbrst = 1;
  USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
  
  /*Reset internal state machine */
  USBD_DCD_INT_fops->Reset(pdev);
}

void STM32_USB_HandleResumeInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_GINTSTS_TypeDef  gintsts;
  USB_OTG_DCTL_TypeDef     devctl;
  USB_OTG_PCGCCTL_TypeDef  power;
  
  if(pdev->cfg.low_power)
  {
    /* un-gate USB Core clock */
    power.d32 = USB_OTG_READ_REG32(&pdev->regs.PCGCCTL);
    power.b.gatehclk = 0;
    power.b.stoppclk = 0;
    USB_OTG_WRITE_REG32(pdev->regs.PCGCCTL, power.d32);
  }
  
  /* Clear the Remote Wake-up Signaling */
  devctl.d32 = 0;
  devctl.b.rmtwkupsig = 1;
  USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DCTL, devctl.d32, 0);
  
  /* Inform upper layer by the Resume Event */
  USBD_DCD_INT_fops->Resume(pdev);
  
  /* Clear interrupt */
  gintsts.d32 = 0;
  gintsts.bits.wkupint = 1;
  USB_OTG_WRITE_REG32 (&pdev->regs.GREGS->GINTSTS, gintsts.d32);
}

void STM32_USB_HandleSuspendInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_GINTSTS_TypeDef  gintsts;
  USB_OTG_PCGCCTL_TypeDef  power;
  USB_OTG_DSTS_TypeDef     dsts;
  
  USBD_DCD_INT_fops->Suspend (pdev);      
  
  dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
    
  /* Clear interrupt */
  gintsts.d32 = 0;
  gintsts.bits.usbsusp = 1;
  USB_OTG_WRITE_REG32(&pdev->regs.GREGS->GINTSTS, gintsts.d32);
  
  if((pdev->cfg.low_power) && (dsts.b.suspsts == 1))
  {
	/*  switch-off the clocks */
    power.d32 = 0;
    power.b.stoppclk = 1;
    USB_OTG_MODIFY_REG32(pdev->regs.PCGCCTL, 0, power.d32);  
    
    power.b.gatehclk = 1;
    USB_OTG_MODIFY_REG32(pdev->regs.PCGCCTL, 0, power.d32);
    
    /* Request to enter Sleep mode after exit from current ISR */
    SCB->SCR |= (SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);
  }
}

UINT32 STM32_USB_GetMode(USB_OTG_CORE_HANDLE *pdev)
{
  return (USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTSTS ) & 0x1);
}

UINT32 STM32_USB_ReadCoreItr(USB_OTG_CORE_HANDLE *pdev)
{
  return USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTSTS) & USB_OTG_READ_REG32(&pdev->regs.GREGS->GINTMSK);
}

//
// Main Interrupt Handler
//
void STM32_USB_Driver_Interrupt(void*)
{
  INTERRUPT_START;

  USB_OTG_GINTSTS_TypeDef  gintr_status;

  // Make sure we are in device mode.
  if(STM32_USB_GetMode(&USB_OTG_dev) == DEVICE_MODE)
  {  
    gintr_status.d32 = STM32_USB_ReadCoreItr(&USB_OTG_dev);
    
	// Ignore interrupts being triggered without anything to do.
    if(gintr_status.d32)
    {
        if(gintr_status.bits.oepint)
        {
            STM32_USB_HandleEndpointOutputInterrupt();
        }
        
        if (gintr_status.bits.iepint)
        {
            STM32_USB_HandleEndpointInputInterrupt();
        }    
        
        if (gintr_status.bits.wkupint)
        {
          STM32_USB_HandleResumeInterrupt(&USB_OTG_dev);
        }

        if (gintr_status.bits.usbsusp)
        {
          STM32_USB_HandleSuspendInterrupt(&USB_OTG_dev);
        }

        if (gintr_status.bits.rxflvl)
        {
            STM32_USB_HandleReceiveStatusQueueLevelInterrupt(&USB_OTG_dev);
        }
        
        if (gintr_status.bits.usbrst)
        {
          STM32_USB_HandleResetInterrupt(&USB_OTG_dev);
        }        
        
        if (gintr_status.bits.enumdne)
        {
          STM32_USB_HandleEnumerationDoneInterrupt(&USB_OTG_dev);
        }     
    }
  }

  INTERRUPT_END;
}

/////////////////////////////////////////////////////////////////////////////
// CPU_USB_xxx functions

USB_CONTROLLER_STATE *CPU_USB_GetState(int Controller)
{
  if(Controller != 0)
  {
    return NULL;
  }
  return &STM32_USB_ControllerState;
}

void STM32_USB_Reset()
{
__IO USB_OTG_GRSTCTL_TypeDef  greset;
  UINT32 count = 0;
  
  greset.d32 = 0;
  /* Wait for AHB master IDLE state. */
  do
  {
    uDelay(3);
    greset.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      //return USB_OTG_OK;
        return;
    }
  }
  while (greset.b.ahbidle == 0);
  /* Core Soft Reset */
  count = 0;
  greset.b.csftrst = 1;
  USB_OTG_WRITE_REG32(&USB_OTG_dev.regs.GREGS->GRSTCTL, greset.d32 );
  do
  {
    greset.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      break;
    }
  }
  while (greset.b.csftrst == 1);
  /* Wait for 3 PHY Clocks*/
  uDelay(3);     
}

void STM32_USB_EnableGlobalInterrupts()
{
  USB_OTG_GAHBCFG_TypeDef  ahbcfg;
  ahbcfg.d32 = 0;
  ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
  USB_OTG_MODIFY_REG32(&USB_OTG_dev.regs.GREGS->GAHBCFG, 0, ahbcfg.d32);
}

void STM32_USB_DisableGlobalInterrupts()
{
  USB_OTG_GAHBCFG_TypeDef  ahbcfg;
  ahbcfg.d32 = 0;
  ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
  USB_OTG_MODIFY_REG32(&USB_OTG_dev.regs.GREGS->GAHBCFG, ahbcfg.d32, 0);
}

HRESULT CPU_USB_Initialize(int Controller)
{
    if(Controller != 0)
    {
        return S_FALSE;
    }

    GLOBAL_LOCK(irq);

    USB_CONTROLLER_STATE *State = &STM32_USB_ControllerState;

    // Setup USB state variables
    STM32_USB_PinsProtected = TRUE;
    State->EndpointStatus = &STM32_USB_EndpointStatus[0];
    State->EndpointCount  = USB_MAX_EP;
    //State->DeviceStatus = USB_STATUS_DEVICE_SELF_POWERED;
    State->PacketSize = MAX_EP0_SIZE;  // default control port packet size

    // use defaults for unused endpoints
    int idx;
    for(idx = 1; idx < USB_MAX_EP; idx++)
    {
        State->IsTxQueue[idx] = FALSE;
        State->MaxPacketSize[idx] = USB_MAX_DATA_PACKET_SIZE;  // 64
    }
    STM32_USB_EP_Type = 0; // set all endpoints to unused

    // get endpoint configuration
    const USB_ENDPOINT_DESCRIPTOR  *ep    = NULL;
    const USB_INTERFACE_DESCRIPTOR *itfc  = NULL;
    while(USB_NextEndpoint(State, ep, itfc))
    {
        // Figure out which endpoint we are initializing
        idx = ep->bEndpointAddress & 0x7F;

        // Check interface and endpoint numbers against hardware capability
        if((idx >= USB_MAX_EP) || (itfc->bInterfaceNumber > 3))
        {
            return S_FALSE;
        }

        if(ep->bEndpointAddress & 0x80)
        {
            State->IsTxQueue[idx] = TRUE;
        }

        // Set the maximum size of the endpoint hardware FIFO
        int endpointSize = ep->wMaxPacketSize;

        // If the endpoint maximum size in the configuration list is bogus
        if((endpointSize != 8) && (endpointSize != 16) && (endpointSize != 32) && (endpointSize != 64))
        {
            return S_FALSE;
        }
        State->MaxPacketSize[idx] = endpointSize;

        // Since endpoint 0 is only used for control, there is never a need to allocate a buffer for it
        // In fact State->Queues[0] is always NULL - it is a cheap placeholder to make the queueIndex = endpointIndex
        QueueBuffers[idx-1].Initialize();           // Clear queue before use
        State->Queues[idx] = &QueueBuffers[idx-1];  // Attach queue to endpoint

        // *****************************************
        // iso endpoints are currently not supported
        // *****************************************
        if((ep->bmAttributes & 3) == USB_ENDPOINT_ATTRIBUTE_ISOCHRONOUS)
        {
            return FALSE;
        }

        // remember endpoint types
        STM32_USB_EP_Type |= (ep->bmAttributes & 3) << (idx * 2);
    }

    // If no endpoints were initialized, something is seriously wrong with the configuration list
    if(STM32_USB_EP_Type == 0)
    {
        return S_FALSE;
    }

    HAL_Time_Sleep_MicroSeconds(1000); // assure host recognizes detach/attach

    // Setup hardware
    CPU_USB_ProtectPins(Controller, FALSE);
    CPU_INTC_ActivateInterrupt(OTG_FS_IRQn, STM32_USB_Driver_Interrupt, 0);
    //CPU_INTC_ActivateInterrupt(OTG_FS_WKUP_IRQn, STM32_USB_Driver_Interrupt, 0);

    STM32_RCC_AHB1PeripheralClockEnable(RCC_AHB1Peripheral_GPIOA);

//    STM32_GPIO_AFConfig(0x08, 0x0A);
    STM32_GPIO_AFConfig(0x09, 0x0A);
    STM32_GPIO_AFConfig(0x0B, 0x0A);
    STM32_GPIO_AFConfig(0x0C, 0x0A);

//    CPU_GPIO_DisablePin(0x08, RESISTOR_DISABLED, 1, GPIO_ALT_MODE_2);
    CPU_GPIO_DisablePin(0x09, RESISTOR_DISABLED, 1, GPIO_ALT_MODE_2);
    CPU_GPIO_DisablePin(0x0B, RESISTOR_DISABLED, 1, GPIO_ALT_MODE_2);
    CPU_GPIO_DisablePin(0x0C, RESISTOR_DISABLED, 1, GPIO_ALT_MODE_2);

    STM32_RCC_APB2PeripheralClockEnable(RCC_APB2Peripheral_SYSCFG);
    STM32_RCC_AHB2PeripheralClockEnable(RCC_AHB2Peripheral_OTG_FS);

    STM32_RCC_APB1PeripheralResetEnable(RCC_APB1Peripheral_PWR);
  
  /*Register class and user callbacks */
  USB_OTG_dev.dev.class_cb = &USBD_NETMF_cb;
  USB_OTG_dev.dev.usr_cb = &USR_cb;  
  USB_OTG_dev.dev.usr_device = &USR_desc;    
  
  /* set USB OTG core params */
  //DCD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID);
  UINT32 i;
  USB_OTG_EP *endpoint;
  
  //USB_OTG_SelectCore (&USB_OTG_dev , USB_OTG_FS_CORE_ID);
  UINT32 baseAddress = 0;
  
  /* at startup the core is in FS mode */
  USB_OTG_dev.cfg.speed            = USB_OTG_SPEED_FULL;
  USB_OTG_dev.cfg.mps              = USB_OTG_FS_MAX_PACKET_SIZE;  

  baseAddress                      = USB_OTG_FS_BASE_ADDR;
  USB_OTG_dev.cfg.coreID           = USB_OTG_FS_CORE_ID;
  USB_OTG_dev.cfg.host_channels    = 8;
  USB_OTG_dev.cfg.dev_endpoints    = 4;
  USB_OTG_dev.cfg.TotalFifoSize    = 320; /* in 32-bits */
  USB_OTG_dev.cfg.phy_itface       = USB_OTG_EMBEDDED_PHY;  
  
  /* Device Endpoint Initialization */
  USB_OTG_dev.regs.GREGS = (USB_OTG_GREGS *)(baseAddress + USB_OTG_CORE_GLOBAL_REGS_OFFSET);
  USB_OTG_dev.regs.DREGS = (USB_OTG_DREGS *)(baseAddress + USB_OTG_DEV_GLOBAL_REG_OFFSET);  
  
  for (i = 0; i < USB_OTG_dev.cfg.dev_endpoints; i++)
  {
    USB_OTG_dev.regs.INEP_REGS[i]  = (USB_OTG_INEPREGS *)(baseAddress + USB_OTG_DEV_IN_EP_REG_OFFSET + (i * USB_OTG_EP_REG_OFFSET));
    USB_OTG_dev.regs.OUTEP_REGS[i] = (USB_OTG_OUTEPREGS *)(baseAddress + USB_OTG_DEV_OUT_EP_REG_OFFSET + (i * USB_OTG_EP_REG_OFFSET));
  }
  
  for (i = 0; i < USB_OTG_dev.cfg.host_channels; i++)
  {
    USB_OTG_dev.regs.DFIFO[i] = (UINT32 *)(baseAddress + USB_OTG_DATA_FIFO_OFFSET + (i * USB_OTG_DATA_FIFO_SIZE));
  }
  
  USB_OTG_dev.regs.PCGCCTL = (UINT32 *)(baseAddress + USB_OTG_PCGCCTL_OFFSET);

  USB_OTG_dev.dev.device_status = USB_OTG_DEFAULT;
  USB_OTG_dev.dev.device_address = 0;
  
    /* Init ep structure */
  for (i = 0; i < USB_OTG_dev.cfg.dev_endpoints ; i++)
  {
    endpoint = &USB_OTG_dev.dev.in_ep[i];
    /* Init ep structure */
    endpoint->is_in = 1;
    endpoint->num = i;
    endpoint->tx_fifo_num = i;
    /* Control until ep is actvated */
    endpoint->type = EP_TYPE_CTRL;
    endpoint->maxpacket =  USB_OTG_MAX_EP0_SIZE;
    endpoint->xfer_buff = 0;
    endpoint->xfer_len = 0;
  }
  
  for (i = 0; i < USB_OTG_dev.cfg.dev_endpoints; i++)
  {
    endpoint = &USB_OTG_dev.dev.out_ep[i];
    /* Init ep structure */
    endpoint->is_in = 0;
    endpoint->num = i;
    endpoint->tx_fifo_num = i;
    /* Control until ep is activated */
    endpoint->type = EP_TYPE_CTRL;
    endpoint->maxpacket = USB_OTG_MAX_EP0_SIZE;
    endpoint->xfer_buff = 0;
    endpoint->xfer_len = 0;
  }
  
  STM32_USB_DisableGlobalInterrupts();
  
  /*Init the Core (common init.) */
  //USB_OTG_CoreInit(&USB_OTG_dev);
    USB_OTG_GUSBCFG_TypeDef  usbcfg;
    USB_OTG_GCCFG_TypeDef    gccfg;
  
    usbcfg.d32 = 0;
    gccfg.d32 = 0;
  
    usbcfg.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.GREGS->GUSBCFG);;
    usbcfg.b.physel  = 1; /* FS Interface */
    usbcfg.b.force_dev = 1; /* Device Mode */
    USB_OTG_WRITE_REG32 (&USB_OTG_dev.regs.GREGS->GUSBCFG, usbcfg.d32);

    /* Reset after a PHY select and set Host mode */
    //USB_OTG_CoreReset(&USB_OTG_dev);
    STM32_USB_Reset(); 

    /* Enable the I2C interface and deactivate the power down*/

    gccfg.d32 = 0;
    gccfg.b.pwdn = 1;

    gccfg.b.vbussensingA = 1 ;
    gccfg.b.vbussensingB = 1 ;        

    if(USB_OTG_dev.cfg.Sof_output)
    {
        gccfg.b.sofouten = 1;  
    }

    USB_OTG_WRITE_REG32 (&USB_OTG_dev.regs.GREGS->GCCFG, gccfg.d32);
    mDelay(20);

 
  

  
  /* Init Device */
  //USB_OTG_CoreInitDev(&USB_OTG_dev);
  USB_OTG_DEPCTL_TypeDef  depctl;
  USB_OTG_DCFG_TypeDef    dcfg;
  USB_OTG_FSIZ_TypeDef    nptxfifosize;
  USB_OTG_FSIZ_TypeDef    txfifosize;
  USB_OTG_DIEPMSK_TypeDef msk;
  //USB_OTG_DTHRCTL_TypeDef dthrctl;  
  
  depctl.d32 = 0;
  dcfg.d32 = 0;
  nptxfifosize.d32 = 0;
  txfifosize.d32 = 0;
  msk.d32 = 0;
  
  /* Restart the Phy Clock */
  USB_OTG_WRITE_REG32(USB_OTG_dev.regs.PCGCCTL, 0);
  /* Device configuration register */
  dcfg.d32 = USB_OTG_READ_REG32( &USB_OTG_dev.regs.DREGS->DCFG);
  dcfg.b.perfrint = DCFG_FRAME_INTERVAL_80;
  //USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.DREGS->DCFG, dcfg.d32 );

  
   /* Set Full speed phy */
    //USB_OTG_InitDevSpeed (&USB_OTG_dev, USB_OTG_SPEED_PARAM_FULL);
  //dcfg.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.DREGS->DCFG);
  dcfg.b.devspd = USB_OTG_SPEED_PARAM_FULL;
  USB_OTG_WRITE_REG32(&USB_OTG_dev.regs.DREGS->DCFG, dcfg.d32);
  
    /* set Rx FIFO size */
    USB_OTG_WRITE_REG32(&USB_OTG_dev.regs.GREGS->GRXFSIZ, RX_FIFO_FS_SIZE);
    
    /* EP0 TX*/
    nptxfifosize.b.depth     = TX0_FIFO_FS_SIZE;
    nptxfifosize.b.startaddr = RX_FIFO_FS_SIZE;
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.GREGS->DIEPTXF0_HNPTXFSIZ, nptxfifosize.d32 );
    
    
    /* EP1 TX*/
    txfifosize.b.startaddr = nptxfifosize.b.startaddr + nptxfifosize.b.depth;
    txfifosize.b.depth = TX1_FIFO_FS_SIZE;
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.GREGS->DIEPTXF[0], txfifosize.d32 );
    
    
    /* EP2 TX*/
    txfifosize.b.startaddr += txfifosize.b.depth;
    txfifosize.b.depth = TX2_FIFO_FS_SIZE;
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.GREGS->DIEPTXF[1], txfifosize.d32 );
    
    
    /* EP3 TX*/  
    txfifosize.b.startaddr += txfifosize.b.depth;
    txfifosize.b.depth = TX3_FIFO_FS_SIZE;
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.GREGS->DIEPTXF[2], txfifosize.d32 );

      /* Flush the FIFOs */
  STM32_USB_FlushTxFifo(&USB_OTG_dev , 0x10); /* all Tx FIFOs */
  STM32_USB_FlushRxFifo(&USB_OTG_dev);
  /* Clear all pending Device Interrupts */
  USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.DREGS->DIEPMSK, 0 );
  USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.DREGS->DOEPMSK, 0 );
  USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.DREGS->DAINT, 0xFFFFFFFF );
  USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.DREGS->DAINTMSK, 0 );
  
  for (i = 0; i < USB_OTG_dev.cfg.dev_endpoints; i++)
  {
    depctl.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.INEP_REGS[i]->DIEPCTL);
    if (depctl.b.epena)
    {
      depctl.d32 = 0;
      depctl.b.epdis = 1;
      depctl.b.snak = 1;
    }
    else
    {
      depctl.d32 = 0;
    }
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.INEP_REGS[i]->DIEPCTL, depctl.d32);
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.INEP_REGS[i]->DIEPTSIZ, 0);
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.INEP_REGS[i]->DIEPINT, 0xFF);
  }
  for (i = 0; i <  USB_OTG_dev.cfg.dev_endpoints; i++)
  {
    USB_OTG_DEPCTL_TypeDef  depctl;
    depctl.d32 = USB_OTG_READ_REG32(&USB_OTG_dev.regs.OUTEP_REGS[i]->DOEPCTL);
    if (depctl.b.epena)
    {
      depctl.d32 = 0;
      depctl.b.epdis = 1;
      depctl.b.snak = 1;
    }
    else
    {
      depctl.d32 = 0;
    }
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.OUTEP_REGS[i]->DOEPCTL, depctl.d32);
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.OUTEP_REGS[i]->DOEPTSIZ, 0);
    USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.OUTEP_REGS[i]->DOEPINT, 0xFF);
  }
  msk.d32 = 0;
  msk.b.txfifoundrn = 1;
  USB_OTG_MODIFY_REG32(&USB_OTG_dev.regs.DREGS->DIEPMSK, msk.d32, msk.d32);

  USB_OTG_GINTMSK_TypeDef  intmsk;
  
  intmsk.d32 = 0;
  
  /* Disable all interrupts. */
  USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.GREGS->GINTMSK, 0);
  /* Clear any pending interrupts */
  USB_OTG_WRITE_REG32( &USB_OTG_dev.regs.GREGS->GINTSTS, 0xFFFFFFFF);
 
  intmsk.b.rxstsqlvl = 1;
  intmsk.b.wkupintr   = 1;
  intmsk.b.usbsuspend = 1;
  intmsk.b.usbreset   = 1;
  intmsk.b.enumdone   = 1;
  intmsk.b.inepintr   = 1;
  intmsk.b.outepintr  = 1;

  //intmsk.b.sofintr    = 1;
  //intmsk.b.incomplisoin = 1; 
  //intmsk.b.incomplisoout = 1;

  USB_OTG_MODIFY_REG32(&USB_OTG_dev.regs.GREGS->GINTMSK, intmsk.d32, intmsk.d32);  

  /* Enable USB Global interrupt */
  STM32_USB_EnableGlobalInterrupts();
  
  /* Upon initialization call user callback */
  USB_OTG_dev.dev.usr_cb->Init();

  return S_OK;
}

USB_OTG_STS STM32_USB_FlushTxFifo (USB_OTG_CORE_HANDLE *pdev , UINT32 num )
{
  USB_OTG_STS status = USB_OTG_OK;
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  
  UINT32 count = 0;
  greset.d32 = 0;
  greset.b.txfflsh = 1;
  greset.b.txfnum  = num;
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GRSTCTL, greset.d32 );
  do
  {
    greset.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      break;
    }
  }
  while (greset.b.txfflsh == 1);
  /* Wait for 3 PHY Clocks*/
  uDelay(3);
  return status;
}

USB_OTG_STS STM32_USB_FlushRxFifo( USB_OTG_CORE_HANDLE *pdev )
{
  USB_OTG_STS status = USB_OTG_OK;
  __IO USB_OTG_GRSTCTL_TypeDef  greset;
  UINT32 count = 0;
  
  greset.d32 = 0;
  greset.b.rxfflsh = 1;
  USB_OTG_WRITE_REG32( &pdev->regs.GREGS->GRSTCTL, greset.d32 );
  do
  {
    greset.d32 = USB_OTG_READ_REG32( &pdev->regs.GREGS->GRSTCTL);
    if (++count > 200000)
    {
      break;
    }
  }
  while (greset.b.rxfflsh == 1);
  /* Wait for 3 PHY Clocks*/
  uDelay(3);
  return status;
}

void STM32_USB_StopDevice(USB_OTG_CORE_HANDLE *pdev)
{
  UINT32 i;
  
  pdev->dev.device_status = 1;
    
  for (i = 0; i < pdev->cfg.dev_endpoints ; i++)
  {
    USB_OTG_WRITE_REG32( &pdev->regs.INEP_REGS[i]->DIEPINT, 0xFF);
    USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[i]->DOEPINT, 0xFF);
  }

  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DIEPMSK, 0 );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DOEPMSK, 0 );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINTMSK, 0 );
  USB_OTG_WRITE_REG32( &pdev->regs.DREGS->DAINT, 0xFFFFFFFF );  
  
  /* Flush the FIFO */
  STM32_USB_FlushRxFifo(pdev);
  STM32_USB_FlushTxFifo(pdev ,  0x10);  
}

void  STM32_USB_DevDisconnect(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_DCTL_TypeDef dctl;
  dctl.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCTL);
  /* Disconnect device for 3ms */
  dctl.b.sftdiscon  = 1;
  USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCTL, dctl.d32);
  mDelay(3);
}

HRESULT CPU_USB_Uninitialize(int Controller)
{
  STM32_USB_DevDisconnect(&USB_OTG_dev);
  STM32_USB_StopDevice(&USB_OTG_dev);

  CPU_USB_ProtectPins(Controller, TRUE);
  STM32_USB_EP_Type = 0; // reset endpoint types

  return S_OK;
}

BOOL CPU_USB_StartOutput(USB_CONTROLLER_STATE* State, int endpoint)
{
  if((State == NULL) || (endpoint >= USB_MAX_EP))
  {
    return FALSE;
  }

  GLOBAL_LOCK(irq);

  // If endpoint is not an output
  if((State->Queues[endpoint] == NULL) || !State->IsTxQueue[endpoint])
  {
    return FALSE;
  }

  // If the halt feature for this endpoint is set, then just clear all the characters
  if(STM32_USB_EndpointStatus[endpoint] & USB_STATUS_ENDPOINT_HALT)
  {
    while(USB_TxDequeue(State, endpoint, TRUE) != NULL);  // clear TX queue
    return TRUE;
  }

  // Write first packet if not done yet
  STM32_USB_Driver_EP_In_Int(0, State, endpoint);

  return TRUE;
}

BOOL CPU_USB_RxEnable( USB_CONTROLLER_STATE* State, int endpoint )
{
  // If this is not a legal Rx queue
  if((State == NULL) || (State->Queues[endpoint] == NULL) || State->IsTxQueue[endpoint])
  {
    return FALSE;
  }

  STM32_USB_EndpointReceive(NETMF_OUT_EP, (UINT8*)(USB_Rx_Buffer), USB_MAX_DATA_PACKET_SIZE);

  return TRUE;
}

BOOL CPU_USB_GetInterruptState()
{
  UINT32 v = 0;
  v = USB_OTG_READ_REG32(&USB_OTG_dev.regs.GREGS->GINTSTS);
  v &= USB_OTG_READ_REG32(&USB_OTG_dev.regs.GREGS->GINTMSK);
  return v;
}

BOOL CPU_USB_ProtectPins(int Controller, BOOL On)
{
  if(STM32_USB_EP_Type == 0)
  {
    return FALSE;  // not yet initialized
  }

  USB_CONTROLLER_STATE *State = &STM32_USB_ControllerState;

  GLOBAL_LOCK(irq);

  if(On)
  {
    if(!STM32_USB_PinsProtected)
    {
      USB_Debug("+");

      STM32_USB_PinsProtected = TRUE;

      // detach usb port
      //STM32_USB_Detach;

      // clear USB Txbuffer
      for(int ep = 1; ep < USB_MAX_EP; ep++)
      {
        if(State->Queues[ep] && State->IsTxQueue[ep])
        {
          while (USB_TxDequeue(State, ep, TRUE) != NULL);  // clear TX queue
        }
      }

      State->DeviceState = USB_DEVICE_STATE_DETACHED;
      USB_StateCallback(State);
    }
  }
  else
  {
    if(STM32_USB_PinsProtected)
    {
      USB_Debug("-");

      STM32_USB_PinsProtected = FALSE;

      // attach usb port
      //STM32_USB_Attach;

      State->DeviceState = USB_DEVICE_STATE_ATTACHED;
      USB_StateCallback(State);
    }
  }
  return TRUE;
}
