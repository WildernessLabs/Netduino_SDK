#ifndef __USBD_NETMF_CORE_H__
#define __USBD_NETMF_CORE_H__

#define USB_LANGID_ENG_US 0x0409

//#define USE_USB_OTG_FS
//#define USE_DEVICE_MODE
//#define USB_OTG_FS_CORE

#define USB_OTG_READ_REG32(reg)  (*(__IO UINT32 *)reg)
#define USB_OTG_WRITE_REG32(reg,value) (*(__IO UINT32 *)reg = value)
#define USB_OTG_MODIFY_REG32(reg,clear_mask, set_mask) USB_OTG_WRITE_REG32(reg, (((USB_OTG_READ_REG32(reg)) & ~clear_mask) | set_mask ))

#undef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#undef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))

//#define USB_DEVICE_DESCRIPTOR_TYPE 1

#define USB_MAX_STR_DESC_SIZ       64

#define USB_LEN_DEV_QUALIFIER_DESC                     0x0A

#define USB_SIZ_DEVICE_DESC                     18
#define USB_SIZ_STRING_LANGID                   4

#define USBD_CFG_MAX_NUM           1

#define USBD_IDX_LANGID_STR                            0x00 
#define USBD_IDX_MFC_STR                               0x01 
#define USBD_IDX_PRODUCT_STR                           0x02
#define USBD_IDX_SERIAL_STR                            0x03 
#define USBD_IDX_CONFIG_STR                            0x04 
#define USBD_IDX_INTERFACE_STR                         0x05

#define USB_FEATURE_EP_HALT                                0
#define USB_FEATURE_REMOTE_WAKEUP                          1
#define USB_FEATURE_TEST_MODE                              2

#define USBD_ITF_MAX_NUM           1

#define DEVICE_MODE                            0

#define USB_OTG_SPEED_PARAM_FULL 3

#define DEP0CTL_MPS_64                         0
#define DEP0CTL_MPS_8                          3

#define DSTS_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ     0
#define DSTS_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ     1
#define DSTS_ENUMSPD_LS_PHY_6MHZ               2
#define DSTS_ENUMSPD_FS_PHY_48MHZ              3

#define DCFG_FRAME_INTERVAL_80                 0

#define RX_FIFO_FS_SIZE                          128
#define TX0_FIFO_FS_SIZE                          64
#define TX1_FIFO_FS_SIZE                         128
#define TX2_FIFO_FS_SIZE                          0
#define TX3_FIFO_FS_SIZE                          0

typedef enum
{
  USB_OTG_FS_CORE_ID = 1
} USB_OTG_CORE_ID_TypeDef;

#define USB_OTG_EMBEDDED_PHY  2

#define USB_OTG_FS_BASE_ADDR                 0x50000000

#define USB_OTG_CORE_GLOBAL_REGS_OFFSET      0x000
#define USB_OTG_DEV_GLOBAL_REG_OFFSET        0x800
#define USB_OTG_DEV_IN_EP_REG_OFFSET         0x900
#define USB_OTG_EP_REG_OFFSET                0x20
#define USB_OTG_DEV_OUT_EP_REG_OFFSET        0xB00
#define USB_OTG_PCGCCTL_OFFSET               0xE00
#define USB_OTG_DATA_FIFO_OFFSET             0x1000
#define USB_OTG_DATA_FIFO_SIZE               0x1000

#define USB_OTG_EP0_STATUS_IN                     4
#define USB_OTG_EP0_STATUS_OUT                    5

#define EP_TYPE_CTRL                           0
#define EP_TYPE_BULK                           2
#define EP_TYPE_INTR                           3

#define USB_OTG_FS_MAX_PACKET_SIZE           64
#define USB_OTG_MAX_EP0_SIZE                 64

#define USB_OTG_DEFAULT                          1
#define USB_OTG_ADDRESSED                        2
#define USB_OTG_CONFIGURED                       3
#define USB_OTG_SUSPENDED                        4

#define USB_OTG_EP0_SETUP                         1
#define USB_OTG_EP0_DATA_IN                       2
#define USB_OTG_EP0_DATA_OUT                      3

#define USB_OTG_EP_BULK 2

#define USB_OTG_MAX_TX_FIFOS 15

#define USB_LEN_CFG_DESC 0x09
#define USB_LEN_IF_DESC 0x09
#define USB_LEN_EP_DESC 0x07

#define  SWAPBYTE(addr)        (((UINT16)(*((UINT8 *)(addr)))) + \
                               (((UINT16)(*(((UINT8 *)(addr)) + 1))) << 8))

#define LOBYTE(x)  ((UINT8)(x & 0x00FF))
#define HIBYTE(x)  ((UINT8)((x & 0xFF00) >>8))

#define USBD_DYNAMIC_DESCRIPTOR_CHANGE_ENABLED

#define NETMF_IN_EP 0x81
#define NETMF_OUT_EP 0x02

#define USB_OTG_SPEED_FULL      1

#define STS_GOUT_NAK                           1
#define STS_DATA_UPDT                          2
#define STS_XFER_COMP                          3
#define STS_SETUP_COMP                         4
#define STS_SETUP_UPDT                         6

typedef enum {
  USB_OTG_OK = 0,
  USB_OTG_FAIL
}USB_OTG_STS;

typedef enum {
  USBD_OK   = 0,
  USBD_BUSY,
  USBD_FAIL,
}USBD_Status;

typedef struct _Device_TypeDef
{
  UINT8  *(*GetDeviceDescriptor)( UINT8 speed , UINT16 *length);  
  UINT8  *(*GetLangIDStrDescriptor)( UINT8 speed , UINT16 *length); 
  UINT8  *(*GetManufacturerStrDescriptor)( UINT8 speed , UINT16 *length);  
  UINT8  *(*GetProductStrDescriptor)( UINT8 speed , UINT16 *length);  
  UINT8  *(*GetSerialStrDescriptor)( UINT8 speed , UINT16 *length);  
  UINT8  *(*GetConfigurationStrDescriptor)( UINT8 speed , UINT16 *length);  
  UINT8  *(*GetInterfaceStrDescriptor)( UINT8 speed , UINT16 *length);   
} USBD_DEVICE, *pUSBD_DEVICE;

typedef struct _USBD_USR_PROP
{
  void (*Init)(void);   
  void (*DeviceReset)(UINT8 speed); 
  void (*DeviceConfigured)(void);
  void (*DeviceSuspended)(void);
  void (*DeviceResumed)(void);  
  
  void (*DeviceConnected)(void);  
  void (*DeviceDisconnected)(void);    
  
} USBD_Usr_cb_TypeDef;

typedef struct USB_OTG_core_cfg
{
  UINT8       host_channels;
  UINT8       dev_endpoints;
  UINT8       speed;
  UINT16      mps;
  UINT16      TotalFifoSize;
  UINT8       phy_itface;
  UINT8       Sof_output;
  UINT8       low_power;
  UINT8       coreID;
 
} USB_OTG_CORE_CFGS, *PUSB_OTG_CORE_CFGS;

typedef struct _USB_OTG_GREGS  //000h
{
  __IO UINT32 GOTGCTL;      /* USB_OTG Control and Status Register    000h*/
  __IO UINT32 GOTGINT;      /* USB_OTG Interrupt Register             004h*/
  __IO UINT32 GAHBCFG;      /* Core AHB Configuration Register    008h*/
  __IO UINT32 GUSBCFG;      /* Core USB Configuration Register    00Ch*/
  __IO UINT32 GRSTCTL;      /* Core Reset Register                010h*/
  __IO UINT32 GINTSTS;      /* Core Interrupt Register            014h*/
  __IO UINT32 GINTMSK;      /* Core Interrupt Mask Register       018h*/
  __IO UINT32 GRXSTSR;      /* Receive Sts Q Read Register        01Ch*/
  __IO UINT32 GRXSTSP;      /* Receive Sts Q Read & POP Register  020h*/
  __IO UINT32 GRXFSIZ;      /* Receive FIFO Size Register         024h*/
  __IO UINT32 DIEPTXF0_HNPTXFSIZ;   /* EP0 / Non Periodic Tx FIFO Size Register 028h*/
  __IO UINT32 HNPTXSTS;     /* Non Periodic Tx FIFO/Queue Sts reg 02Ch*/
  __IO UINT32 GI2CCTL;      /* I2C Access Register                030h*/
  UINT32 Reserved34;  /* PHY Vendor Control Register        034h*/
  __IO UINT32 GCCFG;        /* General Purpose IO Register        038h*/
  __IO UINT32 CID;          /* User ID Register                   03Ch*/
  UINT32  Reserved40[48];   /* Reserved                      040h-0FFh*/
  __IO UINT32 HPTXFSIZ; /* Host Periodic Tx FIFO Size Reg     100h*/
  __IO UINT32 DIEPTXF[USB_OTG_MAX_TX_FIFOS];/* dev Periodic Transmit FIFO */
}
USB_OTG_GREGS;
/**
  * @}
  */


/** @defgroup __device_Registers
  * @{
  */
typedef struct _USB_OTG_DREGS // 800h
{
  __IO UINT32 DCFG;         /* dev Configuration Register   800h*/
  __IO UINT32 DCTL;         /* dev Control Register         804h*/
  __IO UINT32 DSTS;         /* dev Status Register (RO)     808h*/
  UINT32 Reserved0C;           /* Reserved                     80Ch*/
  __IO UINT32 DIEPMSK;   /* dev IN Endpoint Mask         810h*/
  __IO UINT32 DOEPMSK;  /* dev OUT Endpoint Mask        814h*/
  __IO UINT32 DAINT;     /* dev All Endpoints Itr Reg    818h*/
  __IO UINT32 DAINTMSK; /* dev All Endpoints Itr Mask   81Ch*/
  UINT32  Reserved20;          /* Reserved                     820h*/
  UINT32 Reserved9;       /* Reserved                     824h*/
  __IO UINT32 DVBUSDIS;    /* dev VBUS discharge Register  828h*/
  __IO UINT32 DVBUSPULSE;  /* dev VBUS Pulse Register      82Ch*/
  __IO UINT32 DTHRCTL;     /* dev thr                      830h*/
  __IO UINT32 DIEPEMPMSK; /* dev empty msk             834h*/
  __IO UINT32 DEACHINT;    /* dedicated EP interrupt       838h*/
  __IO UINT32 DEACHMSK;    /* dedicated EP msk             83Ch*/  
  UINT32 Reserved40;      /* dedicated EP mask           840h*/
  __IO UINT32 DINEP1MSK;  /* dedicated EP mask           844h*/
  UINT32  Reserved44[15];      /* Reserved                 844-87Ch*/
  __IO UINT32 DOUTEP1MSK; /* dedicated EP msk            884h*/   
}
USB_OTG_DREGS;
/**
  * @}
  */


/** @defgroup __IN_Endpoint-Specific_Register
  * @{
  */
typedef struct _USB_OTG_INEPREGS
{
  __IO UINT32 DIEPCTL; /* dev IN Endpoint Control Reg 900h + (ep_num * 20h) + 00h*/
  UINT32 Reserved04;             /* Reserved                       900h + (ep_num * 20h) + 04h*/
  __IO UINT32 DIEPINT; /* dev IN Endpoint Itr Reg     900h + (ep_num * 20h) + 08h*/
  UINT32 Reserved0C;             /* Reserved                       900h + (ep_num * 20h) + 0Ch*/
  __IO UINT32 DIEPTSIZ; /* IN Endpoint Txfer Size   900h + (ep_num * 20h) + 10h*/
  __IO UINT32 DIEPDMA; /* IN Endpoint DMA Address Reg    900h + (ep_num * 20h) + 14h*/
  __IO UINT32 DTXFSTS;/*IN Endpoint Tx FIFO Status Reg 900h + (ep_num * 20h) + 18h*/
  UINT32 Reserved18;             /* Reserved  900h+(ep_num*20h)+1Ch-900h+ (ep_num * 20h) + 1Ch*/
}
USB_OTG_INEPREGS;
/**
  * @}
  */


/** @defgroup __OUT_Endpoint-Specific_Registers
  * @{
  */
typedef struct _USB_OTG_OUTEPREGS
{
  __IO UINT32 DOEPCTL;       /* dev OUT Endpoint Control Reg  B00h + (ep_num * 20h) + 00h*/
  __IO UINT32 DOUTEPFRM;   /* dev OUT Endpoint Frame number B00h + (ep_num * 20h) + 04h*/
  __IO UINT32 DOEPINT;              /* dev OUT Endpoint Itr Reg      B00h + (ep_num * 20h) + 08h*/
  UINT32 Reserved0C;                    /* Reserved                         B00h + (ep_num * 20h) + 0Ch*/
  __IO UINT32 DOEPTSIZ; /* dev OUT Endpoint Txfer Size   B00h + (ep_num * 20h) + 10h*/
  __IO UINT32 DOEPDMA;              /* dev OUT Endpoint DMA Address  B00h + (ep_num * 20h) + 14h*/
  UINT32 Reserved18[2];                 /* Reserved B00h + (ep_num * 20h) + 18h - B00h + (ep_num * 20h) + 1Ch*/
}
USB_OTG_OUTEPREGS;
/**
  * @}
  */

typedef struct USB_OTG_core_regs //000h
{
  USB_OTG_GREGS         *GREGS;
  USB_OTG_DREGS         *DREGS;
  USB_OTG_INEPREGS      *INEP_REGS[USB_OTG_MAX_TX_FIFOS];
  USB_OTG_OUTEPREGS     *OUTEP_REGS[USB_OTG_MAX_TX_FIFOS];
  __IO UINT32         *DFIFO[USB_OTG_MAX_TX_FIFOS];
  __IO UINT32         *PCGCCTL;
}
USB_OTG_CORE_REGS , *PUSB_OTG_CORE_REGS;

typedef struct _Device_cb
{
    UINT8  (*Init)         (void *pdev , UINT8 cfgidx);
    UINT8  (*DeInit)       (void *pdev , UINT8 cfgidx);
    /* Control Endpoints*/
    UINT8  (*Setup)        (void *pdev , USB_SETUP_PACKET *req);  
    UINT8  (*EP0_TxSent)   (void *pdev );    
    UINT8  (*EP0_RxReady)  (void *pdev );  
    /* Class Specific Endpoints*/
    UINT8  (*DataIn)       (void *pdev , UINT8 epnum);   
    UINT8  (*DataOut)      (void *pdev , UINT8 epnum); 
    UINT8  (*SOF)          (void *pdev); 
    UINT8  (*IsoINIncomplete)  (void *pdev); 
    UINT8  (*IsoOUTIncomplete)  (void *pdev);   
    UINT8  *(*GetConfigDescriptor)( UINT8 speed , UINT16 *length);   
} USBD_Class_cb_TypeDef;

typedef struct USB_OTG_ep
{
  UINT8        num;
  UINT8        is_in;
  UINT8        is_stall;  
  UINT8        type;
  UINT8        data_pid_start;
  UINT8        even_odd_frame;
  UINT16       tx_fifo_num;
  UINT32       maxpacket;
  /* transaction level variables*/
  UINT8        *xfer_buff;
  //UINT32       dma_addr;  
  UINT32       xfer_len;
  UINT32       xfer_count;
  /* Transfer level variables*/  
  UINT32       rem_data_len;
  UINT32       total_data_len;
  UINT32       ctl_data_len;  
} USB_OTG_EP , *PUSB_OTG_EP;

typedef struct _DCD
{
  UINT8        device_config;
  UINT8        device_state;
  UINT8        device_status;
  UINT8        device_address;
  UINT32       DevRemoteWakeup;
  USB_OTG_EP     in_ep   [USB_OTG_MAX_TX_FIFOS];
  USB_OTG_EP     out_ep  [USB_OTG_MAX_TX_FIFOS];
  UINT8        setup_packet [8*3];
  USBD_Class_cb_TypeDef         *class_cb;
  USBD_Usr_cb_TypeDef           *usr_cb;
  USBD_DEVICE                   *usr_device;  
  UINT8        *pConfig_descriptor;
 }
DCD_DEV , *DCD_PDEV;

typedef struct _OTG
{
  UINT8    OTG_State;
  UINT8    OTG_PrevState;  
  UINT8    OTG_Mode;    
}
OTG_DEV , *USB_OTG_USBO_PDEV;

typedef struct USB_OTG_handle
{
  USB_OTG_CORE_CFGS    cfg;
  USB_OTG_CORE_REGS    regs;
  DCD_DEV              dev;
} USB_OTG_CORE_HANDLE , *PUSB_OTG_CORE_HANDLE;

typedef union _USB_OTG_DAINT_TypeDef 
{
    UINT32 d32;
    struct
    {
        UINT32 in : 16;
        UINT32 out : 16;
    } ep;
} USB_OTG_DAINT_TypeDef;

typedef union _USB_OTG_DEPCTL_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 mps :
    11;
UINT32 reserved :
    4;
UINT32 usbactep :
    1;
UINT32 dpid :
    1;
UINT32 naksts :
    1;
UINT32 eptype :
    2;
UINT32 snp :
    1;
UINT32 stall :
    1;
UINT32 txfnum :
    4;
UINT32 cnak :
    1;
UINT32 snak :
    1;
UINT32 setd0pid :
    1;
UINT32 setd1pid :
    1;
UINT32 epdis :
    1;
UINT32 epena :
    1;
  }
  b;
} USB_OTG_DEPCTL_TypeDef ;

typedef union _USB_OTG_DEP0XFRSIZ_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 xfersize :
    7;
UINT32 Reserved7_18 :
    12;
UINT32 pktcnt :
    2;
UINT32 Reserved20_28 :
    9;
UINT32 supcnt :
    2;
    UINT32 Reserved31;
  }
  b;
} USB_OTG_DEP0XFRSIZ_TypeDef ;

typedef union _USB_OTG_DEPXFRSIZ_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 xfersize :
    19;
UINT32 pktcnt :
    10;
UINT32 mc :
    2;
UINT32 Reserved :
    1;
  }
  b;
} USB_OTG_DEPXFRSIZ_TypeDef ;

typedef union _USB_OTG_GINTMSK_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 Reserved0 :
    1;
UINT32 modemismatch :
    1;
UINT32 otgintr :
    1;
UINT32 sofintr :
    1;
UINT32 rxstsqlvl :
    1;
UINT32 nptxfempty :
    1;
UINT32 ginnakeff :
    1;
UINT32 goutnakeff :
    1;
UINT32 Reserved8 :
    1;
UINT32 i2cintr :
    1;
UINT32 erlysuspend :
    1;
UINT32 usbsuspend :
    1;
UINT32 usbreset :
    1;
UINT32 enumdone :
    1;
UINT32 isooutdrop :
    1;
UINT32 eopframe :
    1;
UINT32 Reserved16 :
    1;
UINT32 epmismatch :
    1;
UINT32 inepintr :
    1;
UINT32 outepintr :
    1;
UINT32 incomplisoin :
    1;
UINT32 incomplisoout :
    1;
UINT32 Reserved22_23 :
    2;
UINT32 portintr :
    1;
UINT32 hcintr :
    1;
UINT32 ptxfempty :
    1;
UINT32 Reserved27 :
    1;
UINT32 conidstschng :
    1;
UINT32 disconnect :
    1;
UINT32 sessreqintr :
    1;
UINT32 wkupintr :
    1;
  }
  b;
} USB_OTG_GINTMSK_TypeDef ;

typedef union _USB_OTG_DSTS_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 suspsts :
    1;
UINT32 enumspd :
    2;
UINT32 errticerr :
    1;
UINT32 Reserved4_7:
    4;
UINT32 soffn :
    14;
UINT32 Reserved22_31 :
    10;
  }
  b;
} USB_OTG_DSTS_TypeDef ;

typedef union _USB_OTG_GINTSTS_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 cmod :
    1;
UINT32 mmis :
    1;
UINT32 otgint :
    1;
UINT32 sof :
    1;
UINT32 rxflvl :
    1;
UINT32 nptxfe :
    1;
UINT32 ginakeff :
    1;
UINT32 gonakeff :
    1;
UINT32 reserved8_9 :
    2;
UINT32 esusp :
    1;
UINT32 usbsusp :
    1;
UINT32 usbrst :
    1;
UINT32 enumdne :
    1;
UINT32 isoodrp :
    1;
UINT32 eopf :
    1;
UINT32 intimerrx :
    1;
UINT32 epmismatch :
    1;
UINT32 iepint:
    1;
UINT32 oepint :
    1;
UINT32 iisoixfr :
    1;
UINT32 incompisoout :
    1;
UINT32 reserved22_23 :
    2;
UINT32 hprtint : 1;
UINT32 hcintr : 1;
UINT32 ptxfe : 1;
UINT32 reserved27 : 1;
UINT32 cidschg : 1;
UINT32 discint : 1;
UINT32 sreqint : 1;
UINT32 wkupint : 1;
  }
  bits;
} USB_OTG_GINTSTS_TypeDef ;

typedef union _USB_OTG_GAHBCFG_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 glblintrmsk :
    1;
UINT32 hburstlen :
    4;
UINT32 dmaenable :
    1;
UINT32 Reserved :
    1;
UINT32 nptxfemplvl_txfemplvl :
    1;
UINT32 ptxfemplvl :
    1;
UINT32 Reserved9_31 :
    23;
  }
  b;
} USB_OTG_GAHBCFG_TypeDef ;

typedef union _USB_OTG_DCFG_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 devspd :
    2;
UINT32 nzstsouthshk :
    1;
UINT32 Reserved3 :
    1;
UINT32 devaddr :
    7;
UINT32 perfrint :
    2;
UINT32 Reserved13_17 :
    5;
UINT32 epmscnt :
    4;
  }
  b;
} USB_OTG_DCFG_TypeDef ;

typedef union _USB_OTG_FSIZ_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 startaddr : 16;
UINT32 depth : 16;
  }
  b;
} USB_OTG_FSIZ_TypeDef ;

typedef union _USB_OTG_PCGCCTL_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 stoppclk : 1;
UINT32 gatehclk : 1;
UINT32 Reserved : 30;
  }
  b;
} USB_OTG_PCGCCTL_TypeDef ;

typedef union _USB_OTG_GUSBCFG_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 toutcal :
    3;
UINT32 phyif :
    1;
UINT32 ulpi_utmi_sel :
    1;
UINT32 fsintf :
    1;
UINT32 physel :
    1;
UINT32 ddrsel :
    1;
UINT32 srpcap :
    1;
UINT32 hnpcap :
    1;
UINT32 usbtrdtim :
    4;
UINT32 nptxfrwnden :
    1;
UINT32 phylpwrclksel :
    1;
UINT32 otgutmifssel :
    1;
UINT32 ulpi_fsls :
    1;
UINT32 ulpi_auto_res :
    1;
UINT32 ulpi_clk_sus_m :
    1;
UINT32 ulpi_ext_vbus_drv :
    1;
UINT32 ulpi_int_vbus_indicator :
    1;
UINT32 term_sel_dl_pulse :
    1;
UINT32 Reserved :
    6;
UINT32 force_host :
    1;
UINT32 force_dev :
    1;
UINT32 corrupt_tx :
    1;
  }
  b;
} USB_OTG_GUSBCFG_TypeDef ;

typedef union _USB_OTG_DCTL_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 rmtwkupsig :
    1;
UINT32 sftdiscon :
    1;
UINT32 gnpinnaksts :
    1;
UINT32 goutnaksts :
    1;
UINT32 tstctl :
    3;
UINT32 sgnpinnak :
    1;
UINT32 cgnpinnak :
    1;
UINT32 sgoutnak :
    1;
UINT32 cgoutnak :
    1;
UINT32 Reserved :
    21;
  }
  b;
} USB_OTG_DCTL_TypeDef ;

typedef union _USB_OTG_DIEPINTn_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 xfercompl :
    1;
UINT32 epdisabled :
    1;
UINT32 ahberr :
    1;
UINT32 timeout :
    1;
UINT32 intktxfemp :
    1;
UINT32 intknepmis :
    1;
UINT32 inepnakeff :
    1;
UINT32 emptyintr :
    1;
UINT32 txfifoundrn :
    1;
UINT32 Reserved08_31 :
    23;
  }
  b;
} USB_OTG_DIEPINTn_TypeDef ;
typedef union _USB_OTG_DIEPINTn_TypeDef   USB_OTG_DIEPMSK_TypeDef ;

typedef union _USB_OTG_GRSTCTL_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 csftrst :
    1;
UINT32 hsftrst :
    1;
UINT32 hstfrm :
    1;
UINT32 intknqflsh :
    1;
UINT32 rxfflsh :
    1;
UINT32 txfflsh :
    1;
UINT32 txfnum :
    5;
UINT32 Reserved11_29 :
    19;
UINT32 dmareq :
    1;
UINT32 ahbidle :
    1;
  }
  b;
} USB_OTG_GRSTCTL_TypeDef ;

typedef union _USB_OTG_DRXSTS_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 epnum :
    4;
UINT32 bcnt :
    11;
UINT32 dpid :
    2;
UINT32 pktsts :
    4;
UINT32 fn :
    4;
UINT32 Reserved :
    7;
  }
  b;
} USB_OTG_DRXSTS_TypeDef ;

typedef union _USB_OTG_DTXFSTSn_TypeDef 
{
    UINT32 d32;
    struct
    {
        UINT32 txfspcavail : 16;
        UINT32 Reserved : 16;
    } b;
} USB_OTG_DTXFSTSn_TypeDef ;

typedef union _USB_OTG_DOEPINTn_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 xfercompl :
    1;
UINT32 epdisabled :
    1;
UINT32 ahberr :
    1;
UINT32 setup :
    1;
UINT32 Reserved04_31 :
    28;
  }
  b;
} USB_OTG_DOEPINTn_TypeDef ;
typedef union _USB_OTG_DOEPINTn_TypeDef   USB_OTG_DOEPMSK_TypeDef ;

typedef union _USB_OTG_GCCFG_TypeDef 
{
  UINT32 d32;
  struct
  {
UINT32 Reserved_in :
    16;
UINT32 pwdn :
    1;
UINT32 i2cifen :
    1;
UINT32 vbussensingA :
    1;
UINT32 vbussensingB :
    1;
UINT32 sofouten :
    1;
UINT32 disablevbussensing :
    1;
UINT32 Reserved_out :
    10;
  }
  b;
} USB_OTG_GCCFG_TypeDef ;

typedef struct _USBD_DCD_INT
{
  UINT8 (* DataOutStage) (USB_OTG_CORE_HANDLE *pdev, UINT8 epnum);
  UINT8 (* DataInStage)  (USB_OTG_CORE_HANDLE *pdev, UINT8 epnum);
  UINT8 (* SetupStage) (USB_OTG_CORE_HANDLE *pdev);
  //UINT8 (* SOF) (USB_OTG_CORE_HANDLE *pdev);
  UINT8 (* Reset) (USB_OTG_CORE_HANDLE *pdev);
  UINT8 (* Suspend) (USB_OTG_CORE_HANDLE *pdev);
  UINT8 (* Resume) (USB_OTG_CORE_HANDLE *pdev);
  //UINT8 (* IsoINIncomplete) (USB_OTG_CORE_HANDLE *pdev);
  //UINT8 (* IsoOUTIncomplete) (USB_OTG_CORE_HANDLE *pdev);  
  
  //UINT8 (* DevConnected) (USB_OTG_CORE_HANDLE *pdev);
  //UINT8 (* DevDisconnected) (USB_OTG_CORE_HANDLE *pdev);
} USBD_DCD_INT_cb_TypeDef;
extern USBD_DCD_INT_cb_TypeDef *USBD_DCD_INT_fops;

// Class-specific Callbacks
extern USBD_Class_cb_TypeDef USBD_NETMF_cb;
extern  USBD_Usr_cb_TypeDef USR_cb;

extern  UINT8 USBD_DeviceDesc  [USB_SIZ_DEVICE_DESC];
extern  UINT8 USBD_StrDesc[USB_MAX_STR_DESC_SIZ];
extern  UINT8 USBD_OtherSpeedCfgDesc[USB_LEN_CFG_DESC]; 
extern  UINT8 USBD_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC];
extern  UINT8 USBD_LangIDDesc[USB_SIZ_STRING_LANGID];
extern  USBD_DEVICE USR_desc; 

extern USB_CONTROLLER_STATE STM32_USB_ControllerState;

extern UINT8 USB_Rx_Buffer[USB_MAX_DATA_PACKET_SIZE];
extern UINT8 APP_Rx_Buffer[USB_MAX_DATA_PACKET_SIZE];

UINT32  STM32_USB_EP_Stall (USB_OTG_CORE_HANDLE *pdev, UINT8   epnum);
UINT32 STM32_USB_EP_Open(USB_OTG_CORE_HANDLE *pdev, UINT8 ep_addr, UINT16 ep_mps, UINT8 ep_type);
USBD_Status STM32_USB_CtlContinueRx(USB_OTG_CORE_HANDLE *pdev, UINT8 *pbuf, UINT16 len);
USBD_Status STM32_USB_CtlSendStatus(USB_OTG_CORE_HANDLE  *pdev);
void STM32_USB_EP0_OutStart(USB_OTG_CORE_HANDLE *pdev);
USBD_Status STM32_USB_CtlContinueSendData(USB_OTG_CORE_HANDLE *pdev, UINT8 *pbuf, UINT16 len);
USBD_Status STM32_USB_CtlReceiveStatus(USB_OTG_CORE_HANDLE  *pdev);
void STM32_USB_ParseSetupRequest( USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET  *req);
USBD_Status STM32_USB_StandardDeviceRequest(USB_OTG_CORE_HANDLE *pdev, USB_SETUP_PACKET  *req);
USBD_Status STM32_USB_StandardInterfaceRequest(USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET   *req);
USBD_Status STM32_USB_StandardEndpointRequest (USB_OTG_CORE_HANDLE  *pdev, USB_SETUP_PACKET   *req);
void STM32_USB_GetString(UINT8 *desc, UINT8 *unicode, UINT16 *len);

USB_OTG_STS STM32_USB_FlushTxFifo (USB_OTG_CORE_HANDLE *pdev , UINT32 num );
USB_OTG_STS STM32_USB_FlushRxFifo( USB_OTG_CORE_HANDLE *pdev );
#endif
