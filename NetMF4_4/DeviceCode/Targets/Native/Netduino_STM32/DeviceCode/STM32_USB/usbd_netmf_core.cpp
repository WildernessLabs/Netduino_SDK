/////////////////////////////////////////////////////////////////////////////
//

#include <tinyhal.h>
#include <pal\com\usb\USB.h>
#if defined(PLATFORM_ARM_STM32F4_ANY)
#include "..\stm32f4xx.h"
#elif defined(PLATFORM_ARM_STM32F2_ANY)
#include "..\stm32f2xx.h"
#endif
#include "usbd_netmf_core.h"

static UINT8 USBD_NETMF_Init(void *pdev, UINT8 cfgidx);
static UINT8 USBD_NETMF_DeInit(void *pdev, UINT8 cfgidx);
static UINT8 USBD_NETMF_Setup(void *pdev, USB_SETUP_PACKET *req);
static UINT8 USBD_NETMF_DataIn(void *pdev, UINT8 epnum);
static UINT8 USBD_NETMF_DataOut(void *pdev, UINT8 epnum);
static UINT8* USBD_NETMF_GetCfgDesc(UINT8 speed, UINT16 *length);

UINT32 USBD_ep_status = 0;
UINT32 USBD_cfg_status = 0;  
UINT32 USBD_default_cfg = 0;

UINT8 USBD_StrDesc[USB_MAX_STR_DESC_SIZ];

// Class-specific Callbacks
USBD_Class_cb_TypeDef USBD_NETMF_cb =
{
    USBD_NETMF_Init,
    USBD_NETMF_DeInit,
    // Control EndPoints
    USBD_NETMF_Setup,
    NULL,                 // EP0_TxSent
    NULL,                 // EP0_RxReady
    // Class-specific EndPoints
    USBD_NETMF_DataIn,
    USBD_NETMF_DataOut,
    NULL,                 // SOF
    NULL,                 // IsoINIncomplete
    NULL,                 // IsoOUTIncomplete
    // Configuration
    USBD_NETMF_GetCfgDesc,
};

static UINT32 USBD_NETMF_AltSet = 0;

UINT8 USB_Rx_Buffer[USB_MAX_DATA_PACKET_SIZE];

#define APP_RX_DATA_SIZE 4096

UINT8 APP_Rx_Buffer[USB_MAX_DATA_PACKET_SIZE] ;

/////////////////////////////////////////////////////////////////////////////
// Descriptors
//
const UINT16 USB_CW_CONFIG_DESC_SIZE = USB_LEN_CFG_DESC + 9  + 7 + 7;

 static UINT8 USBD_NETMF_CfgDesc[USB_CW_CONFIG_DESC_SIZE]  =
{
          // Configuration Descriptor
/* 01 */  USB_LEN_CFG_DESC,                     // bLength
/* 02 */  USB_CONFIGURATION_DESCRIPTOR_TYPE,    // bDescriptorType
/* 03 */  LOBYTE(USB_CW_CONFIG_DESC_SIZE),      // wTotalLength
/* 04 */  HIBYTE(USB_CW_CONFIG_DESC_SIZE),
/* 05 */  0x01,                                 // bNumInterfaces
/* 06 */  0x01,                                 // bConfigurationValue
/* 07 */  0x00,                                 // iConfiguration
/* 08 */  0xC0,                                 // bmAttributes
/* 09 */  0x32,                                 // bMaxPower

          // Interface Descriptor
/* 10 */  USB_LEN_IF_DESC,                      // bLength
/* 11 */  USB_INTERFACE_DESCRIPTOR_TYPE,        // bDescriptorType
/* 12 */  0x00,                                 // bInterfaceNumber
/* 13 */  0x00,                                 // bAlternateSetting
/* 14 */  0x02,                                 // bNumEndpoints
/* 15 */  0xFF,                                 // bInterfaceClass
/* 16 */  0x01,                                 // bInterfaceSubClass
/* 17 */  0x01,                                 // nInterfaceProtocol
/* 18 */  0x00,                                 // iInterface

          // Endpoint 1 Descriptor
/* 19 */  USB_LEN_EP_DESC,                      // bLength
/* 20 */  USB_ENDPOINT_DESCRIPTOR_TYPE,         // bDescriptorType
/* 21 */  NETMF_IN_EP,                          // bEndpointAddress
/* 22 */  USB_ENDPOINT_ATTRIBUTE_BULK,          // bmAttributes
/* 23 */  LOBYTE(USB_MAX_DATA_PACKET_SIZE),     // wMaxPacketSize
/* 24 */  HIBYTE(USB_MAX_DATA_PACKET_SIZE),
/* 25 */  0x00,                                 // bInterval

          // Endpoint 2 Descriptor
/* 26 */  USB_LEN_EP_DESC,                      // bLength
/* 27 */  USB_ENDPOINT_DESCRIPTOR_TYPE,         // bDescriptorType
/* 28 */  NETMF_OUT_EP,                         // bEndpointAddress
/* 29 */  USB_ENDPOINT_ATTRIBUTE_BULK,          // bmAttributes
/* 30 */  LOBYTE(USB_MAX_DATA_PACKET_SIZE),     // wMaxPacketSize
/* 31 */  HIBYTE(USB_MAX_DATA_PACKET_SIZE),
/* 32 */  0x00,                                 // bInterval
};

USB_OTG_STS STM32_USB_EPActivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_DEPCTL_TypeDef  depctl;
  USB_OTG_DAINT_TypeDef  daintmsk;
  __IO UINT32 *addr;
  
  depctl.d32 = 0;
  daintmsk.d32 = 0;
  /* Read DEPCTLn register */
  if (ep->is_in == 1)
  {
    addr = &pdev->regs.INEP_REGS[ep->num]->DIEPCTL;
    daintmsk.ep.in = 1 << ep->num;
  }
  else
  {
    addr = &pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL;
    daintmsk.ep.out = 1 << ep->num;
  }
  /* If the EP is already active don't change the EP Control
  * register. */
  depctl.d32 = USB_OTG_READ_REG32(addr);
  if (!depctl.b.usbactep)
  {
    depctl.b.mps    = ep->maxpacket;
    depctl.b.eptype = ep->type;
    depctl.b.txfnum = ep->tx_fifo_num;
    depctl.b.setd0pid = 1;
    depctl.b.usbactep = 1;
    USB_OTG_WRITE_REG32(addr, depctl.d32);
  }
  /* Enable the Interrupt for this EP */  
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DAINTMSK, 0, daintmsk.d32);
  return USB_OTG_OK;
}

UINT32 STM32_USB_EP_Open(USB_OTG_CORE_HANDLE *pdev, UINT8 ep_addr, UINT16 ep_mps, UINT8 ep_type)
{
  USB_OTG_EP *ep;
  
  if ((ep_addr & 0x80) == 0x80)
  {
    ep = &pdev->dev.in_ep[ep_addr & 0x7F];
  }
  else
  {
    ep = &pdev->dev.out_ep[ep_addr & 0x7F];
  }
  ep->num   = ep_addr & 0x7F;
  
  ep->is_in = (0x80 & ep_addr) != 0;
  ep->maxpacket = ep_mps;
  ep->type = ep_type;
  if (ep->is_in)
  {
    /* Assign a Tx FIFO */
    ep->tx_fifo_num = ep->num;
  }
  /* Set initial data PID. */
  if (ep_type == USB_OTG_EP_BULK )
  {
    ep->data_pid_start = 0;
  }
  STM32_USB_EPActivate(pdev , ep );
  return 0;
}

USB_OTG_STS STM32_USB_EP0StartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_DEPCTL_TypeDef      depctl;
  USB_OTG_DEP0XFRSIZ_TypeDef  deptsiz;
  USB_OTG_INEPREGS          *in_regs;
  UINT32 fifoemptymsk = 0;
  
  depctl.d32   = 0;
  deptsiz.d32  = 0;
  /* IN endpoint */
  if (ep->is_in == 1)
  {
    in_regs = pdev->regs.INEP_REGS[0];
    depctl.d32  = USB_OTG_READ_REG32(&in_regs->DIEPCTL);
    deptsiz.d32 = USB_OTG_READ_REG32(&in_regs->DIEPTSIZ);
    /* Zero Length Packet? */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = 0;
      deptsiz.b.pktcnt = 1;
      
    }
    else
    {
      if (ep->xfer_len > ep->maxpacket)
      {
        ep->xfer_len = ep->maxpacket;
        deptsiz.b.xfersize = ep->maxpacket;
      }
      else
      {
        deptsiz.b.xfersize = ep->xfer_len;
      }
      deptsiz.b.pktcnt = 1;
    }
    USB_OTG_WRITE_REG32(&in_regs->DIEPTSIZ, deptsiz.d32);
    
    /* EP enable, IN data in FIFO */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&in_regs->DIEPCTL, depctl.d32);

      /* Enable the Tx FIFO Empty Interrupt for this EP */
      if (ep->xfer_len > 0)
      {
        {
          fifoemptymsk |= 1 << ep->num;
          USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, 0, fifoemptymsk);
        }
      }
  }
  else
  {
    /* OUT endpoint */
    depctl.d32  = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
    deptsiz.d32 = USB_OTG_READ_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ);
    /* Program the transfer size and packet count as follows:
    * xfersize = N * (maxpacket + 4 - (maxpacket % 4))
    * pktcnt = N           */
    if (ep->xfer_len == 0)
    {
      deptsiz.b.xfersize = ep->maxpacket;
      deptsiz.b.pktcnt = 1;
    }
    else
    {
      ep->xfer_len = ep->maxpacket;
      deptsiz.b.xfersize = ep->maxpacket;
      deptsiz.b.pktcnt = 1;
    }
    USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPTSIZ, deptsiz.d32);

    /* EP enable */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32 (&(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL), depctl.d32);
    
  }
  return USB_OTG_OK;
}

USB_OTG_STS STM32_USB_EPStartXfer(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_DEPCTL_TypeDef     depctl;
  USB_OTG_DEPXFRSIZ_TypeDef  deptsiz; 
  UINT32 fifoemptymsk = 0;  
  
  depctl.d32 = 0;
  deptsiz.d32 = 0;
  /* IN endpoint */
  if (ep->is_in == 1)
  {
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

      //if (ep->type == EP_TYPE_ISOC)
      //{
      //  deptsiz.b.mc = 1;
      //}       
    }
    USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPTSIZ, deptsiz.d32);
    
      //if (ep->type != EP_TYPE_ISOC)
      //{
        /* Enable the Tx FIFO Empty Interrupt for this EP */
        if (ep->xfer_len > 0)
        {
          fifoemptymsk = 1 << ep->num;
          USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DIEPEMPMSK, 0, fifoemptymsk);
        }
      //}
    
    /*
    if (ep->type == EP_TYPE_ISOC)
    {
      dsts.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DSTS);
      
      if (((dsts.b.soffn)&0x1) == 0)
      {
        depctl.b.setd1pid = 1;
      }
      else
      {
        depctl.b.setd0pid = 1;
      }
    } 
    */
    /* EP enable, IN data in FIFO */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&pdev->regs.INEP_REGS[ep->num]->DIEPCTL, depctl.d32);

    //if (ep->type == EP_TYPE_ISOC)
    //{
    //  USB_OTG_WritePacket(pdev, ep->xfer_buff, ep->num, ep->xfer_len);   
    //}    
  }
  else
  {
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
    
    //if (ep->type == EP_TYPE_ISOC)
    //{
    //  if (ep->even_odd_frame)
    //  {
    //    depctl.b.setd1pid = 1;
    //  }
    //  else
    //  {
    //    depctl.b.setd0pid = 1;
    //  }
    //}
    /* EP enable */
    depctl.b.cnak = 1;
    depctl.b.epena = 1;
    USB_OTG_WRITE_REG32(&pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL, depctl.d32);
  }
  return USB_OTG_OK;
}

UINT32 STM32_USB_EP_PrepareRx(USB_OTG_CORE_HANDLE *pdev, UINT8 ep_addr, UINT8 *pbuf, UINT16 buf_len)
{
  USB_OTG_EP *ep;
  
  ep = &pdev->dev.out_ep[ep_addr & 0x7F];
  
  /*setup and start the Xfer */
  ep->xfer_buff = pbuf;  
  ep->xfer_len = buf_len;
  ep->xfer_count = 0;
  ep->is_in = 0;
  ep->num = ep_addr & 0x7F;
  
  if ( ep->num == 0 )
  {
    STM32_USB_EP0StartXfer(pdev , ep);
  }
  else
  {
    STM32_USB_EPStartXfer(pdev, ep );
  }
  return 0;
}

USBD_Status STM32_USB_CtlContinueRx(USB_OTG_CORE_HANDLE *pdev, UINT8 *pbuf, UINT16 len)
{  
  STM32_USB_EP_PrepareRx(pdev, 0,  pbuf, len);
  
  return USBD_OK;
}

static UINT8 USBD_NETMF_Init(void *pdev, UINT8 cfgidx)
{
  STM32_USB_EP_Open((USB_OTG_CORE_HANDLE*)pdev, NETMF_IN_EP,  USB_MAX_DATA_PACKET_SIZE, USB_OTG_EP_BULK);
  STM32_USB_EP_Open((USB_OTG_CORE_HANDLE*)pdev, NETMF_OUT_EP, USB_MAX_DATA_PACKET_SIZE, USB_OTG_EP_BULK);

  STM32_USB_EP_PrepareRx((USB_OTG_CORE_HANDLE*)pdev, NETMF_OUT_EP, (UINT8*)USB_Rx_Buffer, USB_MAX_DATA_PACKET_SIZE);
  
  return USBD_OK;
}

USB_OTG_STS STM32_USB_EPDeactivate(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef  depctl;
  USB_OTG_DAINT_TypeDef  daintmsk;
  __IO UINT32 *addr;
  
  depctl.d32 = 0;
  daintmsk.d32 = 0;  
  /* Read DEPCTLn register */
  if (ep->is_in == 1)
  {
    addr = &pdev->regs.INEP_REGS[ep->num]->DIEPCTL;
    daintmsk.ep.in = 1 << ep->num;
  }
  else
  {
    addr = &pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL;
    daintmsk.ep.out = 1 << ep->num;
  }
  depctl.b.usbactep = 0;
  USB_OTG_WRITE_REG32(addr, depctl.d32);
  /* Disable the Interrupt for this EP */
     
    USB_OTG_MODIFY_REG32(&pdev->regs.DREGS->DAINTMSK, daintmsk.d32, 0);
  return status;
}

UINT32 STM32_USB_EP_Close(USB_OTG_CORE_HANDLE *pdev , UINT8  ep_addr)
{
  USB_OTG_EP *ep;
  
  if ((ep_addr&0x80) == 0x80)
  {
    ep = &pdev->dev.in_ep[ep_addr & 0x7F];
  }
  else
  {
    ep = &pdev->dev.out_ep[ep_addr & 0x7F];
  }
  ep->num   = ep_addr & 0x7F;
  ep->is_in = (0x80 & ep_addr) != 0;
  STM32_USB_EPDeactivate(pdev , ep );
  return 0;
}

static UINT8 USBD_NETMF_DeInit (void *pdev, UINT8 cfgidx)
{
  STM32_USB_EP_Close((USB_OTG_CORE_HANDLE*)pdev, NETMF_IN_EP);
  STM32_USB_EP_Close((USB_OTG_CORE_HANDLE*)pdev, NETMF_OUT_EP);
  
  return USBD_OK;
}

USB_OTG_STS STM32_USB_EPSetStall(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_DEPCTL_TypeDef  depctl;
  __IO UINT32 *depctl_addr;
  
  depctl.d32 = 0;
  if (ep->is_in == 1)
  {
    depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
    /* set the disable and stall bits */
    if (depctl.b.epena)
    {
      depctl.b.epdis = 1;
    }
    depctl.b.stall = 1;
    USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  }
  else
  {
    depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
    depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
    /* set the stall bit */
    depctl.b.stall = 1;
    USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  }
  
  return USB_OTG_OK;
}

UINT32  STM32_USB_EP_Stall (USB_OTG_CORE_HANDLE *pdev, UINT8   epnum)
{
  USB_OTG_EP *ep;
  if ((0x80 & epnum) == 0x80)
  {
    ep = &pdev->dev.in_ep[epnum & 0x7F];
  }
  else
  {
    ep = &pdev->dev.out_ep[epnum];
  }

  ep->is_stall = 1;
  ep->num   = epnum & 0x7F;
  ep->is_in = ((epnum & 0x80) == 0x80);
  
  STM32_USB_EPSetStall(pdev , ep);
  
  return (0);
}

UINT32 STM32_USB_EP_Tx(USB_OTG_CORE_HANDLE *pdev, UINT8 ep_addr, UINT8 *pbuf, UINT32 buf_len)
{
  USB_OTG_EP *ep;
  
  ep = &pdev->dev.in_ep[ep_addr & 0x7F];
  
  /* Setup and start the Transfer */
  ep->is_in = 1;
  ep->num = ep_addr & 0x7F;  
  ep->xfer_buff = pbuf;
  //ep->dma_addr = (UINT32)pbuf;  
  ep->xfer_count = 0;
  ep->xfer_len  = buf_len;
  
  if ( ep->num == 0 )
  {
    STM32_USB_EP0StartXfer(pdev , ep);
  }
  else
  {
    STM32_USB_EPStartXfer(pdev, ep );
  }
  return 0;
}

USBD_Status STM32_USB_CtlContinueSendData(USB_OTG_CORE_HANDLE *pdev, UINT8 *pbuf, UINT16 len)
{  
  STM32_USB_EP_Tx(pdev, 0, pbuf, len);
   
  return USBD_OK;
}

void STM32_USB_EP0_OutStart(USB_OTG_CORE_HANDLE *pdev)
{
  USB_OTG_DEP0XFRSIZ_TypeDef  doeptsize0;
  doeptsize0.d32 = 0;
  doeptsize0.b.supcnt = 3;
  doeptsize0.b.pktcnt = 1;
  doeptsize0.b.xfersize = 8 * 3;
  USB_OTG_WRITE_REG32( &pdev->regs.OUTEP_REGS[0]->DOEPTSIZ, doeptsize0.d32 );
}

void STM32_USB_CtlError(USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET  *req)
{
  if((req->bmRequestType & 0x80) == 0x80)
  {
    STM32_USB_EP_Stall(pdev , 0x80);
  }
  else 
  {
    if(req->wLength == 0)
    {
       STM32_USB_EP_Stall(pdev , 0x80);
    }
    else
    {
      STM32_USB_EP_Stall(pdev , 0);
    }
  }
  
  STM32_USB_EP0_OutStart(pdev);
}

USBD_Status STM32_USB_CtlReceiveStatus(USB_OTG_CORE_HANDLE  *pdev)
{
  pdev->dev.device_state = USB_OTG_EP0_STATUS_OUT;  
  STM32_USB_EP_PrepareRx ( pdev, 0, NULL, 0);  

  STM32_USB_EP0_OutStart(pdev);
  
  return USBD_OK;
}

USBD_Status STM32_USB_CtlSendStatus(USB_OTG_CORE_HANDLE  *pdev)
{
  USBD_Status ret = USBD_OK;
  pdev->dev.device_state = USB_OTG_EP0_STATUS_IN;
  STM32_USB_EP_Tx(pdev, 0, NULL, 0); 
  
  STM32_USB_EP0_OutStart(pdev);  
  
  return ret;
}

USBD_Status STM32_USB_CtlSendData(USB_OTG_CORE_HANDLE *pdev, UINT8 *pbuf, UINT16 len)
{
  pdev->dev.in_ep[0].total_data_len = len;
  pdev->dev.in_ep[0].rem_data_len   = len;
  pdev->dev.device_state = USB_OTG_EP0_DATA_IN;

  STM32_USB_EP_Tx(pdev, 0, pbuf, len);
 
  return USBD_OK;
}

static void USBD_GetDescriptor(USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET  *req)
{
  UINT16 len;
  UINT8 *pbuf;
  
  switch (req->wValue >> 8)
  {
  case USB_DEVICE_DESCRIPTOR_TYPE:
    pbuf = pdev->dev.usr_device->GetDeviceDescriptor(pdev->cfg.speed, &len);
    if ((req->wLength == 64) ||( pdev->dev.device_status == USB_OTG_DEFAULT))  
    {                  
      // CW!!!
      //len = 8;
    }
    break;
    
  case USB_CONFIGURATION_DESCRIPTOR_TYPE:
    pbuf   = (UINT8 *)pdev->dev.class_cb->GetConfigDescriptor(pdev->cfg.speed, &len); 
    pbuf[1] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
    pdev->dev.pConfig_descriptor = pbuf;    
    break;
    
  case USB_STRING_DESCRIPTOR_TYPE:
    switch ((UINT8)(req->wValue))
    {
        case USBD_IDX_LANGID_STR:
            pbuf = pdev->dev.usr_device->GetLangIDStrDescriptor(pdev->cfg.speed, &len);        
            break;
      
        case USBD_IDX_MFC_STR:
            pbuf = pdev->dev.usr_device->GetManufacturerStrDescriptor(pdev->cfg.speed, &len);
            break;
      
        case USBD_IDX_PRODUCT_STR:
            pbuf = pdev->dev.usr_device->GetProductStrDescriptor(pdev->cfg.speed, &len);
            break;
      
        case USBD_IDX_SERIAL_STR:
            pbuf = pdev->dev.usr_device->GetSerialStrDescriptor(pdev->cfg.speed, &len);
            break;
      
        case USBD_IDX_CONFIG_STR:
            pbuf = pdev->dev.usr_device->GetConfigurationStrDescriptor(pdev->cfg.speed, &len);
            break;
      
        case USBD_IDX_INTERFACE_STR:
            pbuf = pdev->dev.usr_device->GetInterfaceStrDescriptor(pdev->cfg.speed, &len);
            break;
      
        default:
            STM32_USB_CtlError(pdev , req);
            return;    
    }
    break;
  default: 
     STM32_USB_CtlError(pdev , req);
    return;
  }
  
  if((len != 0)&& (req->wLength != 0))
  {
    len = MIN(len , req->wLength);

    STM32_USB_CtlSendData(pdev, pbuf, len);
  }
}

void  STM32_USB_EP_SetAddress (USB_OTG_CORE_HANDLE *pdev, UINT8 address)
{
    USB_OTG_DCFG_TypeDef  dcfg;
    dcfg.d32 = 0;
    dcfg.b.devaddr = address;
    USB_OTG_MODIFY_REG32( &pdev->regs.DREGS->DCFG, 0, dcfg.d32);
}

static void USBD_SetAddress(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_PACKET  *req)
{
  UINT8 dev_addr; 
  
  if ((req->wIndex == 0) && (req->wLength == 0)) 
  {
    dev_addr = (UINT8)(req->wValue) & 0x7F;     
    
    if (pdev->dev.device_status == USB_OTG_CONFIGURED) 
    {
      STM32_USB_CtlError(pdev , req);
    } 
    else 
    {
      pdev->dev.device_address = dev_addr;
      STM32_USB_EP_SetAddress(pdev, dev_addr);               
      STM32_USB_CtlSendStatus(pdev);                         
      
      if (dev_addr != 0) 
      {
        pdev->dev.device_status  = USB_OTG_ADDRESSED;
      } 
      else 
      {
        pdev->dev.device_status  = USB_OTG_DEFAULT; 
      }
    }
  } 
  else 
  {
     STM32_USB_CtlError(pdev , req);                        
  } 
}

USBD_Status STM32_USB_SetCfg(USB_OTG_CORE_HANDLE  *pdev, UINT8 cfgidx)
{
  pdev->dev.class_cb->Init(pdev, cfgidx); 
  
  /* Upon set config call usr call back */
  pdev->dev.usr_cb->DeviceConfigured();
  
  return USBD_OK; 
}

USBD_Status STM32_USB_ClrCfg(USB_OTG_CORE_HANDLE  *pdev, UINT8 cfgidx)
{
  pdev->dev.class_cb->DeInit(pdev, cfgidx);
  
  return USBD_OK;
}

static void USBD_SetConfig(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_PACKET  *req)
{
  static UINT8 cfgidx;
  
  cfgidx = (UINT8)(req->wValue);                 
  
  if (cfgidx > USBD_CFG_MAX_NUM ) 
  {            
     STM32_USB_CtlError(pdev , req);                              
  } 
  else 
  {
    switch (pdev->dev.device_status) 
    {
    case USB_OTG_ADDRESSED:
      if (cfgidx) 
      {                                			   							   							   				
        pdev->dev.device_config = cfgidx;
        pdev->dev.device_status = USB_OTG_CONFIGURED;
        STM32_USB_SetCfg(pdev , cfgidx);
        STM32_USB_CtlSendStatus(pdev);
      }
      else 
      {
         STM32_USB_CtlSendStatus(pdev);
      }
      break;
      
    case USB_OTG_CONFIGURED:
      if (cfgidx == 0) 
      {                           
        pdev->dev.device_status = USB_OTG_ADDRESSED;
        pdev->dev.device_config = cfgidx;          
        STM32_USB_ClrCfg(pdev , cfgidx);
        STM32_USB_CtlSendStatus(pdev);
        
      } 
      else  if (cfgidx != pdev->dev.device_config) 
      {
        /* Clear old configuration */
        STM32_USB_ClrCfg(pdev , pdev->dev.device_config);
        
        /* set new configuration */
        pdev->dev.device_config = cfgidx;
        STM32_USB_SetCfg(pdev , cfgidx);
        STM32_USB_CtlSendStatus(pdev);
      }
      else
      {
        STM32_USB_CtlSendStatus(pdev);
      }
      break;
      
    default:					
       STM32_USB_CtlError(pdev , req);                     
      break;
    }
  }
}

static void USBD_GetConfig(USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET  *req)
{
  if (req->wLength != 1) 
  {                   
     STM32_USB_CtlError(pdev , req);
  }
  else 
  {
    switch (pdev->dev.device_status )  
    {
    case USB_OTG_ADDRESSED:                     
      
      STM32_USB_CtlSendData(pdev, (UINT8 *)&USBD_default_cfg, 1);
      break;
      
    case USB_OTG_CONFIGURED:                   
      STM32_USB_CtlSendData(pdev, &pdev->dev.device_config, 1);
      break;
      
    default:
       STM32_USB_CtlError(pdev , req);
      break;
    }
  }
}

static void USBD_GetStatus(USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET  *req)
{
    switch (pdev->dev.device_status) 
    {
        case USB_OTG_ADDRESSED:
        case USB_OTG_CONFIGURED:
            if (pdev->dev.DevRemoteWakeup) 
            {
              USBD_cfg_status = USB_STATUS_DEVICE_SELF_POWERED | USB_STATUS_DEVICE_REMOTE_WAKEUP;                                
            }
            else
            {
              USBD_cfg_status = USB_STATUS_DEVICE_SELF_POWERED;   
            }

            STM32_USB_CtlSendData(pdev, (UINT8 *)&USBD_cfg_status, 1);
            break;
        default:
            STM32_USB_CtlError(pdev , req);                        
            break;
    }
}

static void USBD_SetFeature(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_PACKET  *req)
{
  USB_OTG_DCTL_TypeDef     dctl;
  UINT8 test_mode = 0;
 
  if (req->wValue == USB_FEATURE_REMOTE_WAKEUP)
  {
    pdev->dev.DevRemoteWakeup = 1;  
    pdev->dev.class_cb->Setup (pdev, req);   
    STM32_USB_CtlSendStatus(pdev);
  }
  else if ((req->wValue == USB_FEATURE_TEST_MODE) && ((req->wIndex & 0xFF) == 0))
  {
    dctl.d32 = USB_OTG_READ_REG32(&pdev->regs.DREGS->DCTL);
    
    test_mode = req->wIndex >> 8;
    switch (test_mode) 
    {
    case 1: // TEST_J
      dctl.b.tstctl = 1;
      break;
      
    case 2: // TEST_K	
      dctl.b.tstctl = 2;
      break;
      
    case 3: // TEST_SE0_NAK
      dctl.b.tstctl = 3;
      break;
      
    case 4: // TEST_PACKET
      dctl.b.tstctl = 4;
      break;
      
    case 5: // TEST_FORCE_ENABLE
      dctl.b.tstctl = 5;
      break;
    }
    USB_OTG_WRITE_REG32(&pdev->regs.DREGS->DCTL, dctl.d32);
    STM32_USB_CtlSendStatus(pdev);
  }

}

static void USBD_ClrFeature(USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET  *req)
{
  switch (pdev->dev.device_status)
  {
  case USB_OTG_ADDRESSED:
  case USB_OTG_CONFIGURED:
    if (req->wValue == USB_FEATURE_REMOTE_WAKEUP) 
    {
      pdev->dev.DevRemoteWakeup = 0; 
      pdev->dev.class_cb->Setup (pdev, req);   
      STM32_USB_CtlSendStatus(pdev);
    }
    break;
    
  default :
     STM32_USB_CtlError(pdev , req);
    break;
  }
}

USBD_Status  STM32_USB_StandardDeviceRequest(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_PACKET  *req)
{  
  switch (req->bRequest) 
  {
  case USB_GET_DESCRIPTOR:  // 6
  
    USBD_GetDescriptor (pdev, req) ;
    break;
    
  case USB_SET_ADDRESS:     // 5                      
    USBD_SetAddress(pdev, req);
    break;
    
  case USB_SET_CONFIGURATION:                    
    USBD_SetConfig(pdev , req);
    break;
    
  case USB_GET_CONFIGURATION:                 
    USBD_GetConfig(pdev , req);
    break;
    
  case USB_GET_STATUS:                                  
    USBD_GetStatus(pdev , req);
    break;
    
  case USB_SET_FEATURE:   
    USBD_SetFeature(pdev , req);    
    break;
    
  case USB_CLEAR_FEATURE:                                   
    USBD_ClrFeature(pdev , req);
    break;
    
  default:  
    STM32_USB_CtlError(pdev , req);
    break;
  }
  
  return USBD_OK;
}

USBD_Status STM32_USB_StandardInterfaceRequest(USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET   *req)
{
  switch (pdev->dev.device_status) 
  {
  case USB_OTG_CONFIGURED:
    
    if (LOBYTE(req->wIndex) <= USBD_ITF_MAX_NUM) 
    {
      pdev->dev.class_cb->Setup (pdev, req); 
      
      if(req->wLength == 0)
      {
         STM32_USB_CtlSendStatus(pdev);
      }
    } 
    else 
    {                                               
       STM32_USB_CtlError(pdev , req);
    }
    break;
    
  default:
     STM32_USB_CtlError(pdev , req);
    break;
  }
  return USBD_OK;
}

USB_OTG_STS STM32_USB_EPClearStall(USB_OTG_CORE_HANDLE *pdev , USB_OTG_EP *ep)
{
  USB_OTG_STS status = USB_OTG_OK;
  USB_OTG_DEPCTL_TypeDef  depctl;
  __IO uint32_t *depctl_addr;
  
  depctl.d32 = 0;

/*

  The 'if' statement below generates two errors with GCC.  The pragmas wrapped around the 'if'
  statement should turn the the error off.

  TODO: Provide a more permanent fix following the initial port.

  DeviceCode\Targets\Native\Netduino_STM32\DeviceCode\STM32_USB\usbd_netmf_core.cpp(979,19):
  error G55E5081F: invalid conversion from 'volatile UINT32* {aka volatile unsigned int*}' to 'volatile uint32_t* {aka volatile long unsigned int*}'
  [-fpermissive] [C:\Netduino_SDK\NetMF4_4\DeviceCode\Targets\Native\Netduino_STM32\DeviceCode\STM32_USB\dotNetMF.proj]
       depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
                     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DeviceCode\Targets\Native\Netduino_STM32\DeviceCode\STM32_USB\usbd_netmf_core.cpp(983,19): 
  error G55E5081F: invalid conversion from 'volatile UINT32* {aka volatile unsigned int*}' to 'volatile uint32_t* {aka volatile long unsigned int*}'
  [-fpermissive] [C:\Netduino_SDK\NetMF4_4\DeviceCode\Targets\Native\Netduino_STM32\DeviceCode\STM32_USB\dotNetMF.proj]
       depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
                     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

  if (ep->is_in == 1)
  {
    depctl_addr = (__IO uint32_t *) (&(pdev->regs.INEP_REGS[ep->num]->DIEPCTL));
//    depctl_addr = &(pdev->regs.INEP_REGS[ep->num]->DIEPCTL);
  }
  else
  {
    depctl_addr = (__IO uint32_t *) (&(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL));
//    depctl_addr = &(pdev->regs.OUTEP_REGS[ep->num]->DOEPCTL);
  }

  depctl.d32 = USB_OTG_READ_REG32(depctl_addr);
  /* clear the stall bits */
  depctl.b.stall = 0;
  if (ep->type == EP_TYPE_INTR || ep->type == EP_TYPE_BULK)
  {
    depctl.b.setd0pid = 1; /* DATA0 */
  }
  USB_OTG_WRITE_REG32(depctl_addr, depctl.d32);
  return status;
}

uint32_t  STM32_USB_EP_ClrStall (USB_OTG_CORE_HANDLE *pdev, UINT8 epnum)
{
  USB_OTG_EP *ep;
  if ((0x80 & epnum) == 0x80)
  {
    ep = &pdev->dev.in_ep[epnum & 0x7F];    
  }
  else
  {
    ep = &pdev->dev.out_ep[epnum];
  }
  
  ep->is_stall = 0;  
  ep->num   = epnum & 0x7F;
  ep->is_in = ((epnum & 0x80) == 0x80);
  
  STM32_USB_EPClearStall(pdev , ep);
  return (0);
}

USBD_Status STM32_USB_StandardEndpointRequest(USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET   *req)
{
  
  UINT8   ep_addr;
  USBD_Status ret = USBD_OK;

  ep_addr  = LOBYTE(req->wIndex);   
  
  switch (req->bRequest) 
  {
    
  case USB_SET_FEATURE :
    
    switch (pdev->dev.device_status) 
    {
    case USB_OTG_ADDRESSED:          
      if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
      {
        STM32_USB_EP_Stall(pdev , ep_addr);
      }
      break;	
      
    case USB_OTG_CONFIGURED:   
      if (req->wValue == USB_FEATURE_EP_HALT)
      {
        if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
        { 
          STM32_USB_EP_Stall(pdev , ep_addr);
          
        }
      }
      pdev->dev.class_cb->Setup (pdev, req);   
      STM32_USB_CtlSendStatus(pdev);
      
      break;
      
    default:                         
      STM32_USB_CtlError(pdev , req);
      break;    
    }
    break;
    
  case USB_CLEAR_FEATURE :
    
    switch (pdev->dev.device_status) 
    {
    case USB_OTG_ADDRESSED:          
      if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
      {
        STM32_USB_EP_Stall(pdev , ep_addr);
      }
      break;	
      
    case USB_OTG_CONFIGURED:   
      if (req->wValue == USB_FEATURE_EP_HALT)
      {
        if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
        {        
          STM32_USB_EP_ClrStall(pdev , ep_addr);
          pdev->dev.class_cb->Setup (pdev, req);
        }
        STM32_USB_CtlSendStatus(pdev);
      }
      break;
      
    default:                         
       STM32_USB_CtlError(pdev , req);
      break;    
    }
    break;
    
  case USB_GET_STATUS:                  
    switch (pdev->dev.device_status) 
    {
    case USB_OTG_ADDRESSED:          
      if ((ep_addr != 0x00) && (ep_addr != 0x80)) 
      {
        STM32_USB_EP_Stall(pdev , ep_addr);
      }
      break;	
      
    case USB_OTG_CONFIGURED:         
      
      
      if ((ep_addr & 0x80)== 0x80)
      {
        if(pdev->dev.in_ep[ep_addr & 0x7F].is_stall)
        {
          USBD_ep_status = 0x0001;     
        }
        else
        {
          USBD_ep_status = 0x0000;  
        }
      }
      else if ((ep_addr & 0x80)== 0x00)
      {
        if(pdev->dev.out_ep[ep_addr].is_stall)
        {
          USBD_ep_status = 0x0001;     
        }
        
        else 
        {
          USBD_ep_status = 0x0000;     
        }      
      }
      STM32_USB_CtlSendData (pdev, (UINT8 *)&USBD_ep_status, 2);
      break;
      
    default:                         
       STM32_USB_CtlError(pdev , req);
      break;
    }
    break;
    
  default:
    break;
  }
  return ret;
}

void STM32_USB_ParseSetupRequest(USB_OTG_CORE_HANDLE *pDevice, USB_SETUP_PACKET *request)
{
    request->bmRequestType     = *(UINT8 *)  (pDevice->dev.setup_packet);
    request->bRequest          = *(UINT8 *)  (pDevice->dev.setup_packet +  1);
    request->wValue            = SWAPBYTE    (pDevice->dev.setup_packet +  2);
    request->wIndex            = SWAPBYTE    (pDevice->dev.setup_packet +  4);
    request->wLength           = SWAPBYTE    (pDevice->dev.setup_packet +  6);

    pDevice->dev.in_ep[0].ctl_data_len = request->wLength;
    pDevice->dev.device_state = USB_OTG_EP0_SETUP;
}

static UINT8 USBD_NETMF_Setup(void *pdev, USB_SETUP_PACKET *request)
{  
  //switch(request->bmRequestType & USB_REQ_TYPE_MASK)
    switch(USB_SETUP_TYPE(request->bmRequestType))
    {
        case USB_SETUP_TYPE_CLASS:
            switch(request->bRequest)
            {
                default:
                    STM32_USB_CtlError((USB_OTG_CORE_HANDLE*)pdev, request);
                    return USBD_FAIL;
            }
            break;

        case USB_SETUP_TYPE_STANDARD:
            switch (request->bRequest)
            {
                case USB_GET_DESCRIPTOR:
                  //if( req->wValue >> 8 == HID_REPORT_DESC)
                  //{
                  //  len = MIN(HID_MOUSE_REPORT_DESC_SIZE , req->wLength);
                  //  pbuf = HID_MOUSE_ReportDesc;
                  //}
                  //else
                  //if((req->wValue >> 8) == HID_DESCRIPTOR_TYPE)
                  //{
                //#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
                //        pbuf = USBD_HID_Desc;   
                //#else
                    //pbuf = USBD_HID_CfgDesc + 0x12;
                //#endif 
                    //len = MIN(USB_HID_DESC_SIZ , req->wLength);
                  //}
                  //USBD_CtlSendData ((USB_OTG_CORE_HANDLE*)pdev, pbuf, len);
                  STM32_USB_CtlError((USB_OTG_CORE_HANDLE*)pdev, request);
                  return USBD_FAIL;
                  break;

                case USB_GET_INTERFACE:
                  STM32_USB_CtlSendData((USB_OTG_CORE_HANDLE*)pdev, (UINT8 *)&USBD_NETMF_AltSet, 1);
                  break;

                case USB_SET_INTERFACE:
                  USBD_NETMF_AltSet = (UINT8)(request->wValue);
                  break;
            }
    }
    
  return USBD_OK;
}

static UINT8 USBD_NETMF_DataIn(void *pdev, UINT8 epnum)
{
  //debug_printf( "USBD_NETMF_DataIn\r\n" );
  //DCD_EP_Flush((USB_OTG_CORE_HANDLE*)pdev, NETMF_IN_EP);

  USB_CONTROLLER_STATE *State = &STM32_USB_ControllerState;

  USB_PACKET64* Packet64 = USB_TxDequeue(State, epnum, TRUE);
  if(Packet64)
  {
    // copy buffer
    int count = Packet64->Size;
    for(int i = 0; i < count; i++)
    {
      APP_Rx_Buffer[i] = Packet64->Buffer[i];
    }
    STM32_USB_EP_Tx((USB_OTG_CORE_HANDLE*)pdev, NETMF_IN_EP, (UINT8*)APP_Rx_Buffer, count);
  }
  return USBD_OK;
}


static UINT8 USBD_NETMF_DataOut(void *pdev, UINT8 epnum)
{
  //debug_printf( "USBD_NETMF_DataOut\r\n" );
  //USB_OTG_EP* pEP = &((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum];

  // Get the received data buffer and update the counter
  UINT16 USB_Rx_Cnt = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;

  //USB_Debug("USBD_NETMF_DataOut, epnum=0x%02X, xferCount=%i, xferLen=%i\r\n",
  //  (int)epnum, (int)pEP->xfer_count, (int)pEP->xfer_len);

  USB_CONTROLLER_STATE *State = &STM32_USB_ControllerState;

  BOOL bFull;
  USB_PACKET64* packet64 = USB_RxEnqueue(State, epnum, bFull);

  //USB_Debug("USBD_NETMF_DataOut, USB_RxEnqueue( , %i, ) packet64=0x%08X, bFull=%i\r\n", epnum, packet64, bFull);

  if(packet64)
  {
    int count = USB_Rx_Cnt;
    if(count > USB_MAX_DATA_PACKET_SIZE)
    {
      //USB_Debug("******* received too much (%i)\r\n", count);
      count = USB_MAX_DATA_PACKET_SIZE;
    }

    // FIXME: memcpy ?
    BYTE* pSource = USB_Rx_Buffer;
    BYTE *pTarget = packet64->Buffer;
    for(int i = 0; i < count; i++)
    {
      *pTarget++ = *pSource++;
    }
    packet64->Size = count;

    if(!bFull)
    {
      //STM32_USB_SetRxStatus(endpoint, USB_EP_RX_VALID);
      //DCD_SetEPStatus((USB_OTG_CORE_HANDLE*)pdev, epnum, USB_OTG_EP_RX_VALID);
      STM32_USB_EP_PrepareRx((USB_OTG_CORE_HANDLE*)pdev, NETMF_OUT_EP, (UINT8*)(USB_Rx_Buffer), USB_MAX_DATA_PACKET_SIZE);
    }
  }
  
  return USBD_OK;
}


static UINT8 *USBD_NETMF_GetCfgDesc(UINT8 speed, UINT16 *length)
{
  *length = sizeof(USBD_NETMF_CfgDesc);
  
  return USBD_NETMF_CfgDesc;
}
/*
static UINT8 USBD_GetLen(UINT8 *buf)
{
    UINT8  len = 0;

    while (*buf != NULL) 
    {
        len++;
        buf++;
    }

    return len;
}
*/

void STM32_USB_GetString(UINT8 *desc, UINT8 *unicode, UINT16 *len)
{
  UINT8 idx = 0;
  
  if (desc != NULL) 
  {
    // FIXME: Do not call GetLen() function, when the loop below ends 'idx' contains string length
    //*len =  USBD_GetLen(desc) * 2 + 2;    
    //unicode[idx++] = *len;
      unicode[idx++] = 0;
    unicode[idx++] =  USB_STRING_DESCRIPTOR_TYPE;
    
    while (*desc != NULL) 
    {
      unicode[idx++] = *desc++;
      unicode[idx++] =  0x00;
    }
    
    *len = idx;
    unicode[0] = *len;
  } 
}

/////////////////////////////////////////////////////////////////////////////
// User Callbacks

static void USBD_USR_Init(void)
{
}

static void USBD_USR_DeviceReset(UINT8 speed)
{
  //switch (speed)
  //{
    //case USB_OTG_SPEED_HIGH:
    //  break;
    //case USB_OTG_SPEED_FULL:
    //  break;
    //default:
    //  break;
  //}
}

static void USBD_USR_DeviceConfigured(void)
{
  USB_CONTROLLER_STATE *State = &STM32_USB_ControllerState;

  State->DeviceState = USB_DEVICE_STATE_CONFIGURED;
  USB_StateCallback(State);
}

static void USBD_USR_DeviceConnected(void)
{
}

static void USBD_USR_DeviceDisconnected(void)
{
}

static void USBD_USR_DeviceSuspended(void)
{
}

static void USBD_USR_DeviceResumed(void)
{
}

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};
