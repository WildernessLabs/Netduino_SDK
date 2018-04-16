////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  STM32 USB OTG Register Definitions
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define USB_OTG_NUM_CHANNELS            16


typedef struct
{
  // global registers
  __IO uint32_t GOTGCTL;
  __IO uint32_t GOTGINT;
  __IO uint32_t GAHBCFG;
  __IO uint32_t GUSBCFG;
  __IO uint32_t GRSTCTL;
  __IO uint32_t GINTSTS;
  __IO uint32_t GINTMSK;
  __IO uint32_t GRXSTSR;
  __IO uint32_t GRXSTSP;
  __IO uint32_t GRXFSIZ;
  union {
    __IO uint32_t HNPTXFSIZ;
    __IO uint32_t DIEPTXF0;
  };
  __IO uint32_t HNPTXSTS;
       uint32_t Res1[2];
  __IO uint32_t GCCFG;
  __IO uint32_t CID;
       uint32_t Res2[48];
  union {
    __IO uint32_t HPTXFSIZ;
    __IO uint32_t DIEPTXF[8];
  };
       uint32_t Res3[184];
  // host mode registers
  __IO uint32_t HCFG;
  __IO uint32_t HFIR;
  __IO uint32_t HFNUM;
       uint32_t Res4;
  __IO uint32_t HPTXSTS;
  __IO uint32_t HAINT;
  __IO uint32_t HAINTMSK;
       uint32_t Res5[9];
  __IO uint32_t HPRT;
       uint32_t Res6[47];
  struct {
    __IO uint32_t CHAR;
    __IO uint32_t SPLT;
    __IO uint32_t INT;
    __IO uint32_t INTMSK;
    __IO uint32_t TSIZ;
    __IO uint32_t TDMA;
         uint32_t Res8[2];
  } HC[USB_OTG_NUM_CHANNELS];
       uint32_t Res9[64];
  // device mode registers
  __IO uint32_t DCFG;
  __IO uint32_t DCTL;
  __IO uint32_t DSTS;
       uint32_t Res10;
  __IO uint32_t DIEPMSK;
  __IO uint32_t DOEPMSK;
  __IO uint32_t DAINT;
  __IO uint32_t DAINTMSK;
       uint32_t Res11[2];
  __IO uint32_t DVBUSDIS;
  __IO uint32_t DVBUSPULSE;
       uint32_t Res12;
  __IO uint32_t DIEPEMPMSK;
       uint32_t Res13[50];
  struct {
    __IO uint32_t CTL;
         uint32_t Res14;
    __IO uint32_t INT;
         uint32_t Res15;
    __IO uint32_t TSIZ;
         uint32_t Res16;
    __IO uint32_t TXFSTS;
         uint32_t Res17;
  } DIEP[USB_OTG_NUM_CHANNELS];
  struct {
    __IO uint32_t CTL;
         uint32_t Res18;
    __IO uint32_t INT;
         uint32_t Res19;
    __IO uint32_t TSIZ;
         uint32_t Res20[3];
  } DOEP[USB_OTG_NUM_CHANNELS];
       uint32_t Res21[64];
  // power and clock gating
  __IO uint32_t PCGCCTL;
       uint32_t Res22[127];
  // FIFO regions
  __IO uint32_t DFIFO[USB_OTG_NUM_CHANNELS][1024];
}
OTG_TypeDef;


#define OTG_FS_BASE           (0x50000000)
#define OTG_HS_BASE           (0x40040000)
#define OTG_FS                ((OTG_TypeDef *) OTG_FS_BASE)  
#define OTG_HS                ((OTG_TypeDef *) OTG_HS_BASE)  


#define OTG_GUSBCFG_PHYSEL    (1<<6)
#define OTG_GUSBCFG_HNPCAP    (1<<9)
#define OTG_GUSBCFG_PHYLPCS   (1<<15)
#define OTG_GUSBCFG_FHMOD     (1<<29)
#define OTG_GUSBCFG_FDMOD     (1<<30)

#define OTG_GCCFG_PWRDWN      (1<<16)
#define OTG_GCCFG_VBUSASEN    (1<<18)
#define OTG_GCCFG_VBUSBSEN    (1<<19)
#define OTG_GCCFG_SOFOUTEN    (1<<20)
#define OTG_GCCFG_NOVBUSSENS  (1<<21)

#define OTG_GAHBCFG_GINTMSK   (1<<0)
#define OTG_GAHBCFG_TXFELVL   (1<<7)
#define OTG_GAHBCFG_PTXFELVL  (1<<8)

#define OTG_GINTSTS_MMIS      (1<<1)
#define OTG_GINTSTS_OTGINT    (1<<2)
#define OTG_GINTSTS_SOF       (1<<3)
#define OTG_GINTSTS_RXFLVL    (1<<4)
#define OTG_GINTSTS_NPTXFE    (1<<5)
#define OTG_GINTSTS_USBSUSP   (1<<11)
#define OTG_GINTSTS_USBRST    (1<<12)
#define OTG_GINTSTS_ENUMDNE   (1<<13)
#define OTG_GINTSTS_IEPINT    (1<<18)
#define OTG_GINTSTS_OEPINT    (1<<19)
#define OTG_GINTSTS_HPRTINT   (1<<24)
#define OTG_GINTSTS_HCINT     (1<<25)
#define OTG_GINTSTS_PTXFE     (1<<26)
#define OTG_GINTSTS_DISCINT   (1<<29)
#define OTG_GINTSTS_SRQINT    (1<<30)
#define OTG_GINTSTS_WKUPINT   (1U<<31)

#define OTG_GINTMSK_MMISM     (1<<1)
#define OTG_GINTMSK_OTGINT    (1<<2)
#define OTG_GINTMSK_SOFM      (1<<3)
#define OTG_GINTMSK_RXFLVLM   (1<<4)
#define OTG_GINTMSK_NPTXFEM   (1<<5)
#define OTG_GINTMSK_USBSUSPM  (1<<11)
#define OTG_GINTMSK_USBRST    (1<<12)
#define OTG_GINTMSK_ENUMDNEM  (1<<13)
#define OTG_GINTMSK_IEPINT    (1<<18)
#define OTG_GINTMSK_OEPINT    (1<<19)
#define OTG_GINTMSK_HPRTIM    (1<<24)
#define OTG_GINTMSK_HCIM      (1<<25)
#define OTG_GINTMSK_PTXFEM    (1<<26)
#define OTG_GINTMSK_DISCINT   (1<<29)
#define OTG_GINTMSK_SRQIM     (1<<30)
#define OTG_GINTMSK_WUIM      (1U<<31)

#define OTG_GOTGINT_DBCDNE    (1<<19)

#define OTG_GRSTCTL_CSRST     (1<<0)
#define OTG_GRSTCTL_HSRST     (1<<1)
#define OTG_GRSTCTL_FCRST     (1<<2)
#define OTG_GRSTCTL_RXFFLSH   (1<<4)
#define OTG_GRSTCTL_TXFFLSH   (1<<5)
#define OTG_GRSTCTL_TXFNUM    (0x1F<<6)
#define OTG_GRSTCTL_TXF_ALL   (0x10<<6)
#define OTG_GRSTCTL_AHBIDL    (1U<<31)

#define OTG_GRXSTSP_EPNUM     (0x0F<<0)
#define OTG_GRXSTSP_CHNUM     (0x0F<<0)
#define OTG_GRXSTSP_BCNT      (0x7FF<<4)
#define OTG_GRXSTSP_DPID      (0x03<<15)
#define OTG_GRXSTSP_PKTSTS    (0x0F<<17)
#define OTG_GRXSTSP_PKTSTS_GN (0x01<<17) // global OUT NAK
#define OTG_GRXSTSP_PKTSTS_PR (0x02<<17) // packet received
#define OTG_GRXSTSP_PKTSTS_DC (0x03<<17) // data transaction completed
#define OTG_GRXSTSP_PKTSTS_SC (0x04<<17) // setup stage completed
#define OTG_GRXSTSP_PKTSTS_TE (0x05<<17) // toggle error
#define OTG_GRXSTSP_PKTSTS_SR (0x06<<17) // setup data received
#define OTG_GRXSTSP_PKTSTS_CH (0x07<<17) // channel haltet
#define OTG_GRXSTSP_FRMNUM    (0x0F<<21)


#define OTG_HNPTXSTS_NPTXFSAV 0x0000FFFF // np fifo space available
#define OTG_HNPTXSTS_NPTQXSAV 0x00FF0000 // np queue space available
#define OTG_HNPTXSTS_NPTXQTOP 0x7F000000 // top of np tx queue

#define OTG_HPTXSTS_PTXFSAVL  0x0000FFFF // p fifo space available
#define OTG_HPTXSTS_PTQXSAV   0x00FF0000 // p queue space available
#define OTG_HPTXSTS_PTXQTOP   0x7F000000 // top of p tx queue

#define OTG_HCFG_FSLSPCS      (3<<0)
#define OTG_HCFG_FSLSPCS48    (1<<0)
#define OTG_HCFG_FSLSPCS6     (2<<0)
#define OTG_HCFG_FSLSS        (1<<2)

#define OTG_HPRT_PSPD         (3<<17) // port speed
#define OTG_HPRT_PSPD_F       (1<<17) // full speed
#define OTG_HPRT_PSPD_L       (2<<17) // low speed
#define OTG_HPRT_PTCTL        (0xF<<13) // test control
#define OTG_HPRT_PPWR         (1<<12) // port power
#define OTG_HPRT_PLSTS        (3<<10) // line status
#define OTG_HPRT_PLSTS_P      (1<<10) // P line status
#define OTG_HPRT_PLSTS_M      (1<<11) // M line status
#define OTG_HPRT_PRST         (1<<8)  // port reset
#define OTG_HPRT_PSUSP        (1<<7)  // suspend
#define OTG_HPRT_PRES         (1<<6)  // resume
#define OTG_HPRT_POCCHNG      (1<<5)  // overcurrent changed
#define OTG_HPRT_POCA         (1<<4)  // overcurrent active
#define OTG_HPRT_PENCHNG      (1<<3)  // port enable changed
#define OTG_HPRT_PENA         (1<<2)  // port enabled  // rc_w1!
#define OTG_HPRT_PCDET        (1<<1)  // connect detected
#define OTG_HPRT_PCSTS        (1<<0)  // connected status

#define OTG_HCCHAR_MPSIZ      (0x7FF<<0) // max packet size
#define OTG_HCCHAR_EPNUM      (0xF<<11)  // endpoint number
#define OTG_HCCHAR_EPDIR      (1<<15)    // direction IN
#define OTG_HCCHAR_LSDEV      (1<<17)    // low speed device
#define OTG_HCCHAR_EPTYP      (3<<18)    // endpoint type
#define OTG_HCCHAR_EPTYP_C    (0<<18)    //  control
#define OTG_HCCHAR_EPTYP_S    (1<<18)    //  isochronous
#define OTG_HCCHAR_EPTYP_B    (2<<18)    //  bulk
#define OTG_HCCHAR_EPTYP_I    (3<<18)    //  interrupt
#define OTG_HCCHAR_EPTYP_P    (1<<18)    //  periodic types
#define OTG_HCCHAR_MCNT       (3<<20)    // multicount
#define OTG_HCCHAR_MCNT_1     (1<<20)    //  1 transaction
#define OTG_HCCHAR_MCNT_2     (2<<20)    //  2 transaction
#define OTG_HCCHAR_MCNT_3     (3<<20)    //  3 transaction
#define OTG_HCCHAR_DAD        (0x7F<<22) // device address
#define OTG_HCCHAR_ODDFRM     (1<<29)    // odd frame transfer
#define OTG_HCCHAR_CHDIS      (1<<30)    // channel disable
#define OTG_HCCHAR_CHENA      (1U<<31)   // channel enable

#define OTG_HCINT_XFRC        (1<<0)  // transfer completed
#define OTG_HCINT_CHH         (1<<1)  // channel halted
#define OTG_HCINT_STALL       (1<<3)  // stall received
#define OTG_HCINT_NAK         (1<<4)  // nak received
#define OTG_HCINT_ACK         (1<<5)  // ack received/transmitted
#define OTG_HCINT_TXERR       (1<<7)  // transaction error
#define OTG_HCINT_BBERR       (1<<8)  // babble error
#define OTG_HCINT_FRMOR       (1<<9)  // frame overrun
#define OTG_HCINT_DTERR       (1<<10) // data toggle error

#define OTG_HCINTMSK_XFRCM    (1<<0)  // transfer completed
#define OTG_HCINTMSK_CHHM     (1<<1)  // channel halted
#define OTG_HCINTMSK_STALLM   (1<<3)  // stall received
#define OTG_HCINTMSK_NAKM     (1<<4)  // nak received
#define OTG_HCINTMSK_ACKM     (1<<5)  // ack received/transmitted
#define OTG_HCINTMSK_TXERRM   (1<<7)  // transaction error
#define OTG_HCINTMSK_BBERRM   (1<<8)  // babble error
#define OTG_HCINTMSK_FRMORM   (1<<9)  // frame overrun
#define OTG_HCINTMSK_DTERRM   (1<<10) // data toggle error

#define OTG_HCTSIZ_XFRSIZ     (0x7FFFF<<0) // transfer size
#define OTG_HCTSIZ_XFRSIZ_1   (1<<0)
#define OTG_HCTSIZ_PKTCNT     (0x3FF<<19) // packet count
#define OTG_HCTSIZ_PKTCNT_1   (1<<19)
#define OTG_HCTSIZ_DPID       (3<<29) // data pid
#define OTG_HCTSIZ_DPID_D0    (0<<29) //  data0
#define OTG_HCTSIZ_DPID_D2    (1<<29) //  data2
#define OTG_HCTSIZ_DPID_D1    (2<<29) //  data1
#define OTG_HCTSIZ_DPID_MS    (3<<29) //  mdata/setup

#define OTG_DCFG_DSPD         (0x3<<0)
#define OTG_DCFG_DAD          (0x7F<<4)

#define OTG_DCTL_RWUSIG       (1<<0)
#define OTG_DCTL_SDIS         (1<<1)
#define OTG_DCTL_POPRGDNE     (1<<11)

#define OTG_DIEPMSK_XFRCM     (1<<0) // transfer completed
#define OTG_DIEPMSK_TOM       (1<<3) // timeout

#define OTG_DOEPMSK_XFRCM     (1<<0) // transfer completed
#define OTG_DOEPMSK_STUPM     (1<<3) // setup phase done

#define OTG_DIEPINT_XFRC      (1<<0) // transfer completed
#define OTG_DIEPINT_TOC       (1<<3) // timeout

#define OTG_DOEPINT_XFRC      (1<<0) // transfer completed
#define OTG_DOEPINT_STUP      (1<<3) // setup phase done

#define OTG_DIEPCTL_USBAEP    (1<<15)
#define OTG_DIEPCTL_STALL     (1<<21)
#define OTG_DIEPCTL_CNAK      (1<<26)
#define OTG_DIEPCTL_SNAK      (1<<27)
#define OTG_DIEPCTL_SD0PID    (1<<28)
#define OTG_DIEPCTL_EPDIS     (1<<30)
#define OTG_DIEPCTL_EPENA     (1U<<31)

#define OTG_DOEPCTL_USBAEP    (1<<15)
#define OTG_DOEPCTL_STALL     (1<<21)
#define OTG_DOEPCTL_CNAK      (1<<26)
#define OTG_DOEPCTL_SNAK      (1<<27)
#define OTG_DOEPCTL_SD0PID    (1<<28)
#define OTG_DOEPCTL_EPDIS     (1<<30)
#define OTG_DOEPCTL_EPENA     (1U<<31)

#define OTG_DIEPTSIZ_PKTCNT   (3<<19)
#define OTG_DIEPTSIZ_PKTCNT_1 (1<<19)

#define OTG_DOEPTSIZ_PKTCNT   (3<<19)
#define OTG_DOEPTSIZ_PKTCNT_1 (1<<19)
#define OTG_DOEPTSIZ_STUPCNT  (3<<29)
