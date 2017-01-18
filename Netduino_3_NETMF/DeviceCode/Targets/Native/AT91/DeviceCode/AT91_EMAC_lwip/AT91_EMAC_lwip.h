////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AT91_EMAC_LWIP_H_1
#define _AT91_EMAC_LWIP_H_1 1


BOOL  AT91_EMAC_LWIP_open(struct netif *pNetIf);
void  AT91_EMAC_LWIP_close(struct netif *pNetIf);
err_t AT91_EMAC_LWIP_xmit(struct netif *pNetIf, struct pbuf *pPBuf);
void  AT91_EMAC_LWIP_recv (struct netif *pNetIf);
BOOL  AT91_EMAC_LWIP_statistics(struct netif *pNetIf);
void  AT91_EMAC_LWIP_interrupt(struct netif *pNetIf);
void AT91_EMAC_AddMulticastAddr(UINT32 addr);

//////////////////////////////////////////////////////////////////////////////
// AT91_EMAC
//
struct AT91_EMAC {
    static const UINT32 c_Base = AT91C_BASE_EMAC;

    /****/ volatile UINT32 EMAC_NCR;      // Network Control Register
    static const    UINT32 EMAC_LB         = (0x1ul <<  0); // (EMAC) Loopback. Optional. When set, loopback signal is at high level.
    static const    UINT32 EMAC_LLB        = (0x1ul <<  1); // (EMAC) Loopback local. 
    static const    UINT32 EMAC_RE         = (0x1ul <<  2); // (EMAC) Receive enable. 
    static const    UINT32 EMAC_TE         = (0x1ul <<  3); // (EMAC) Transmit enable. 
    static const    UINT32 EMAC_MPE        = (0x1ul <<  4); // (EMAC) Management port enable. 
    static const    UINT32 EMAC_CLRSTAT    = (0x1ul <<  5); // (EMAC) Clear statistics registers. 
    static const    UINT32 EMAC_INCSTAT    = (0x1ul <<  6); // (EMAC) Increment statistics registers. 
    static const    UINT32 EMAC_WESTAT     = (0x1ul <<  7); // (EMAC) Write enable for statistics registers. 
    static const    UINT32 EMAC_BP         = (0x1ul <<  8); // (EMAC) Back pressure. 
    static const    UINT32 EMAC_TSTART     = (0x1ul <<  9); // (EMAC) Start Transmission. 
    static const    UINT32 EMAC_THALT      = (0x1ul << 10); // (EMAC) Transmission Halt. 
    static const    UINT32 EMAC_TPFR       = (0x1ul << 11); // (EMAC) Transmit pause frame 
    static const    UINT32 EMAC_TZQ        = (0x1ul << 12); // (EMAC) Transmit zero quantum pause frame

    /****/ volatile UINT32 EMAC_NCFGR;    // Network Configuration Register
    static const    UINT32 EMAC_SPD           = (0x1ul <<  0); // (EMAC) Speed. 
    static const    UINT32 EMAC_FD            = (0x1ul <<  1); // (EMAC) Full duplex. 
    static const    UINT32 EMAC_JFRAME        = (0x1ul <<  3); // (EMAC) Jumbo Frames. 
    static const    UINT32 EMAC_CAF           = (0x1ul <<  4); // (EMAC) Copy all frames. 
    static const    UINT32 EMAC_NBC           = (0x1ul <<  5); // (EMAC) No broadcast. 
    static const    UINT32 EMAC_MTI           = (0x1ul <<  6); // (EMAC) Multicast hash event enable
    static const    UINT32 EMAC_UNI           = (0x1ul <<  7); // (EMAC) Unicast hash enable. 
    static const    UINT32 EMAC_BIG           = (0x1ul <<  8); // (EMAC) Receive 1522 bytes. 
    static const    UINT32 EMAC_EAE           = (0x1ul <<  9); // (EMAC) External address match enable. 
    static const    UINT32 EMAC_CLK           = (0x3ul << 10); // (EMAC) 
    static const    UINT32 EMAC_CLK_HCLK_8    = (0x0ul << 10); // (EMAC) HCLK divided by 8
    static const    UINT32 EMAC_CLK_HCLK_16   = (0x1ul << 10); // (EMAC) HCLK divided by 16
    static const    UINT32 EMAC_CLK_HCLK_32   = (0x2ul << 10); // (EMAC) HCLK divided by 32
    static const    UINT32 EMAC_CLK_HCLK_64   = (0x3ul << 10); // (EMAC) HCLK divided by 64
    static const    UINT32 EMAC_RTY           = (0x1ul << 12); // (EMAC) 
    static const    UINT32 EMAC_PAE           = (0x1ul << 13); // (EMAC) 
    static const    UINT32 EMAC_RBOF          = (0x3ul << 14); // (EMAC) 
    static const    UINT32 EMAC_RBOF_OFFSET_0 = (0x0ul << 14); // (EMAC) no offset from start of receive buffer
    static const    UINT32 EMAC_RBOF_OFFSET_1 = (0x1ul << 14); // (EMAC) one byte offset from start of receive buffer
    static const    UINT32 EMAC_RBOF_OFFSET_2 = (0x2ul << 14); // (EMAC) two bytes offset from start of receive buffer
    static const    UINT32 EMAC_RBOF_OFFSET_3 = (0x3ul << 14); // (EMAC) three bytes offset from start of receive buffer
    static const    UINT32 EMAC_RLCE          = (0x1ul << 16); // (EMAC) Receive Length field Checking Enable
    static const    UINT32 EMAC_DRFCS         = (0x1ul << 17); // (EMAC) Discard Receive FCS
    static const    UINT32 EMAC_EFRHD         = (0x1ul << 18); // (EMAC) 
    static const    UINT32 EMAC_IRXFCS        = (0x1ul << 19); // (EMAC) Ignore RX FCS

    /****/ volatile UINT32 EMAC_NSR;      // Network Status Register
    static const    UINT32 EMAC_LINKR      = (0x1ul <<  0); // (EMAC) 
    static const    UINT32 EMAC_MDIO       = (0x1ul <<  1); // (EMAC) 
    static const    UINT32 EMAC_IDLE       = (0x1ul <<  2); // (EMAC) 

    /****/ volatile UINT32 Reserved0[2];  

    /****/ volatile UINT32 EMAC_TSR;      // Transmit Status Register
    static const    UINT32 EMAC_UBR        = (0x1ul <<  0); // (EMAC) 
    static const    UINT32 EMAC_COL        = (0x1ul <<  1); // (EMAC) 
    static const    UINT32 EMAC_RLES       = (0x1ul <<  2); // (EMAC) 
    static const    UINT32 EMAC_TGO        = (0x1ul <<  3); // (EMAC) Transmit Go
    static const    UINT32 EMAC_BEX        = (0x1ul <<  4); // (EMAC) Buffers exhausted mid frame
    static const    UINT32 EMAC_COMP       = (0x1ul <<  5); // (EMAC) 
    static const    UINT32 EMAC_UND        = (0x1ul <<  6); // (EMAC) 

    /****/ volatile UINT32 EMAC_RBQP;     // Receive Buffer Queue Pointer

    /****/ volatile UINT32 EMAC_TBQP;     // Transmit Buffer Queue Pointer

    /****/ volatile UINT32 EMAC_RSR;      // Receive Status Register
    static const    UINT32 EMAC_BNA        = (0x1ul <<  0); // (EMAC) 
    static const    UINT32 EMAC_REC        = (0x1ul <<  1); // (EMAC) 
    static const    UINT32 EMAC_OVR        = (0x1ul <<  2); // (EMAC) 

    /****/ volatile UINT32 EMAC_ISR;      // Interrupt Status Register
    static const    UINT32 EMAC_MFD        = (0x1ul <<  0); // (EMAC) 
    static const    UINT32 EMAC_RCOMP      = (0x1ul <<  1); // (EMAC) 
    static const    UINT32 EMAC_RXUBR      = (0x1ul <<  2); // (EMAC) 
    static const    UINT32 EMAC_TXUBR      = (0x1ul <<  3); // (EMAC) 
    static const    UINT32 EMAC_TUNDR      = (0x1ul <<  4); // (EMAC) 
    static const    UINT32 EMAC_RLEX       = (0x1ul <<  5); // (EMAC) 
    static const    UINT32 EMAC_TXERR      = (0x1ul <<  6); // (EMAC) 
    static const    UINT32 EMAC_TCOMP      = (0x1ul <<  7); // (EMAC) 
    static const    UINT32 EMAC_LINK       = (0x1ul <<  9); // (EMAC) 
    static const    UINT32 EMAC_ROVR       = (0x1ul << 10); // (EMAC) 
    static const    UINT32 EMAC_HRESP      = (0x1ul << 11); // (EMAC) 
    static const    UINT32 EMAC_PFRE       = (0x1ul << 12); // (EMAC) 
    static const    UINT32 EMAC_PTZ        = (0x1ul << 13); // (EMAC) 

    /****/ volatile UINT32 EMAC_IER;      // Interrupt Enable Register

    /****/ volatile UINT32 EMAC_IDR;      // Interrupt Disable Register

    /****/ volatile UINT32 EMAC_IMR;      // Interrupt Mask Register

    /****/ volatile UINT32 EMAC_MAN;      // PHY Maintenance Register
    static const    UINT32 EMAC_DATA       = (0xFFFFul <<  0); // (EMAC) 
    static const    UINT32 EMAC_CODE       = (0x03ul << 16);   // (EMAC) 
    static const    UINT32 EMAC_REGA       = (0x1Ful << 18);   // (EMAC) 
    static const    UINT32 EMAC_PHYA       = (0x1Ful << 23);   // (EMAC) 
    static const    UINT32 EMAC_RW         = (0x03ul << 28);   // (EMAC) 
    static const    UINT32 EMAC_SOF        = (0x03ul << 30);   // (EMAC) 

    /****/ volatile UINT32 EMAC_PTR;      // Pause Time Register

    /****/ volatile UINT32 EMAC_PFR;      // Pause Frames received Register

    /****/ volatile UINT32 EMAC_FTO;      // Frames Transmitted OK Register

    /****/ volatile UINT32 EMAC_SCF;      // Single Collision Frame Register

    /****/ volatile UINT32 EMAC_MCF;      // Multiple Collision Frame Register

    /****/ volatile UINT32 EMAC_FRO;      // Frames Received OK Register

    /****/ volatile UINT32 EMAC_FCSE;     // Frame Check Sequence Error Register

    /****/ volatile UINT32 EMAC_ALE;      // Alignment Error Register

    /****/ volatile UINT32 EMAC_DTF;      // Deferred Transmission Frame Register

    /****/ volatile UINT32 EMAC_LCOL;     // Late Collision Register

    /****/ volatile UINT32 EMAC_ECOL;     // Excessive Collision Register

    /****/ volatile UINT32 EMAC_TUND;     // Transmit Underrun Error Register

    /****/ volatile UINT32 EMAC_CSE;      // Carrier Sense Error Register

    /****/ volatile UINT32 EMAC_RRE;      // Receive Ressource Error Register

    /****/ volatile UINT32 EMAC_ROV;      // Receive Overrun Errors Register

    /****/ volatile UINT32 EMAC_RSE;      // Receive Symbol Errors Register

    /****/ volatile UINT32 EMAC_ELE;      // Excessive Length Errors Register

    /****/ volatile UINT32 EMAC_RJA;      // Receive Jabbers Register

    /****/ volatile UINT32 EMAC_USF;      // Undersize Frames Register

    /****/ volatile UINT32 EMAC_STE;      // SQE Test Error Register

    /****/ volatile UINT32 EMAC_RLE;      // Receive Length Field Mismatch Register

    /****/ volatile UINT32 EMAC_TPF;      // Transmitted Pause Frames Register

    /****/ volatile UINT32 EMAC_HRB;      // Hash Address Bottom[31:0]

    /****/ volatile UINT32 EMAC_HRT;      // Hash Address Top[63:32]

    /****/ volatile UINT32 EMAC_SA1L;     // Specific Address 1 Bottom, First 4 bytes

    /****/ volatile UINT32 EMAC_SA1H;     // Specific Address 1 Top, Last 2 bytes

    /****/ volatile UINT32 EMAC_SA2L;     // Specific Address 2 Bottom, First 4 bytes

    /****/ volatile UINT32 EMAC_SA2H;     // Specific Address 2 Top, Last 2 bytes

    /****/ volatile UINT32 EMAC_SA3L;     // Specific Address 3 Bottom, First 4 bytes

    /****/ volatile UINT32 EMAC_SA3H;     // Specific Address 3 Top, Last 2 bytes

    /****/ volatile UINT32 EMAC_SA4L;     // Specific Address 4 Bottom, First 4 bytes

    /****/ volatile UINT32 EMAC_SA4H;     // Specific Address 4 Top, Last 2 bytes

    /****/ volatile UINT32 EMAC_TID;      // Type ID Checking Register

    /****/ volatile UINT32 EMAC_TPQ;      // Transmit Pause Quantum Register

    /****/ volatile UINT32 EMAC_USRIO;    // USER Input/Output Register
    static const    UINT32 EMAC_RMII       = (0x1ul <<  0);       // (EMAC) Reduce MII

    static const    UINT32 EMAC_CLKEN      = (0x1ul <<  1);       // (EMAC) Clock Enable

    /****/ volatile UINT32 EMAC_WOL;      // Wake On LAN Register
    static const    UINT32 EMAC_IP         = (0xFFFFul <<  0);    // (EMAC) ARP request IP address

    static const    UINT32 EMAC_MAG        = (0x1ul << 16);       // (EMAC) Magic packet event enable

    static const    UINT32 EMAC_ARP        = (0x1ul << 17);       // (EMAC) ARP request event enable

    static const    UINT32 EMAC_SA1        = (0x1ul << 18);       // (EMAC) Specific address register 1 event enable

    /****/ volatile UINT32 Reserved1[13];  

    /****/ volatile UINT32 EMAC_REV;      // Revision Register
    static const    UINT32 EMAC_REVREF     = (0xFFFFUL <<  0);    // (EMAC) 
    static const    UINT32 EMAC_PARTREF    = (0xFFFFUL << 16);    // (EMAC) 

    static AT91_EMAC &EMAC() { return *(AT91_EMAC     *)(size_t)(c_Base); }

};

//
// AT91_EMAC
//////////////////////////////////////////////////////////////////////////////


// Callback functions type
typedef void (*EMAC_TxCallback)(unsigned int status);
typedef void (*EMAC_RxCallback)(unsigned int status);
typedef void (*EMAC_WakeupCallback)(void);

//----------------------------------------------------------------------------
// Data Structures for Transfer Buffers
//----------------------------------------------------------------------------
// Number of buffer for RX, be carreful: MUST be 2^n
#define RX_BUFFERS  32
// Number of buffer for TX, be carreful: MUST be 2^n
#define TX_BUFFERS   8

// Buffer Size
#define EMAC_RX_UNITSIZE             128    /// Fixed size for RX buffer
#define EMAC_TX_UNITSIZE            1518    /// Size for ETH frame length

// Definitions used by EMAC Descriptors
#define EMAC_ADDRESS_MASK   ((UINT32)0xFFFFFFFC)
#define EMAC_LENGTH_FRAME   ((UINT32)0x0FFF)    /// Length of frame mask

// receive buffer descriptor bits
#define EMAC_RX_OWNERSHIP_BIT   (1ul <<  0)
#define EMAC_RX_WRAP_BIT        (1ul <<  1)
#define EMAC_RX_SOF_BIT         (1ul << 14)
#define EMAC_RX_EOF_BIT         (1ul << 15)

// Transmit buffer descriptor bits
#define EMAC_TX_LAST_BUFFER_BIT (1ul << 15)
#define EMAC_TX_WRAP_BIT        (1ul << 30)
#define EMAC_TX_USED_BIT        (1ul << 31)

// Describes the type and attribute of Transfer descriptor.
typedef struct _EmacTDescriptor {
    UINT32 addr;
    UINT32 status;
} EmacTDescriptor, *PEmacTDescriptor;

typedef struct {
   EmacTDescriptor td[RX_BUFFERS];
   UINT16 idx;
} RxTd;

typedef struct {
   EmacTDescriptor td[TX_BUFFERS];
   EMAC_TxCallback txCb[TX_BUFFERS];    /// Callback function to be invoked once TD has been processed
   EMAC_WakeupCallback wakeupCb;        /// Callback function to be invoked once several TD have been released
   UINT16 wakeupThreshold; /// Number of free TD before wakeupCb is invoked
   UINT16 head;            /// Circular buffer head pointer incremented by the upper layer (buffer to be sent)
   UINT16 tail;            /// Circular buffer head pointer incremented by the IT handler (buffer sent)
} TxTd;

// The MAC can support frame lengths up to 1536 bytes.
#define EMAC_FRAME_LENTGH_MAX       1536

//-----------------------------------------------------------------------------
// Circular buffer management
//-----------------------------------------------------------------------------
// Return count in buffer
#define CIRC_CNT(head,tail,size) \
  ((head) >= (tail) ? ((head) - (tail)) : ((size) - (tail) + (head)))

// Return space available, 0..size-1
// We always leave one free char as a completely full buffer 
// has head == tail, which is the same as empty
#define CIRC_SPACE(head,tail,size) ((size) - CIRC_CNT((head),(tail),(size)))

// Return count up to the end of the buffer.  
// Carefully avoid accessing head and tail more than once,
// so they can change underneath us without returning inconsistent results
#define CIRC_CNT_TO_END(head,tail,size) \
   ({int end = (size) - (tail); \
     int n = ((head) + end) & ((size)-1); \
     n < end ? n : end;})

// Return space available up to the end of the buffer
#define CIRC_SPACE_TO_END(head,tail,size) \
   ({int end = (size) - 1 - (head); \
     int n = (end + (tail)) & ((size)-1); \
     n <= end ? n : end+1;})

// Increment head or tail
#define CIRC_INC(headortail,size) \
        headortail++;             \
        if(headortail >= size) {  \
            headortail = 0;       \
        }

#define CIRC_EMPTY(circ)     ((circ)->head == (circ)->tail)
#define CIRC_CLEAR(circ)     ((circ)->head = (circ)->tail = 0)


/* Function Prototypes */
void AT91_EMAC_LWIP_Init(struct netif *pNetIf);

void AT91_EMAC_LWIP_EnableMdio(void);

void AT91_EMAC_LWIP_DisableMdio(void);

void AT91_EMAC_LWIP_SetLinkSpeed(BOOL speed, BOOL fullduplex);

void AT91_EMAC_LWIP_EnableMII(void);

BOOL AT91_EMAC_LWIP_SetMdcClock(UINT32 mck);

BOOL AT91_EMAC_LWIP_ReadPhy(UINT32 phy_addr, UINT32 address, UINT32 *value, UINT32 retry);

BOOL AT91_EMAC_LWIP_WritePhy(UINT32 phy_addr, UINT32 address, UINT32 value, UINT32 retry);

BOOL AT91_EMAC_LWIP_SetMCast(struct netif *pNetIf);

#endif

