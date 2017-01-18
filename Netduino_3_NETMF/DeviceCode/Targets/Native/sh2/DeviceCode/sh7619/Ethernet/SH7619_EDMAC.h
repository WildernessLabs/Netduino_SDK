////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SH7619_EDMAC_H_1
#define _SH7619_EDMAC_H_1 1

#include <tinyhal.h>
#include "rtip.h"
#include "SH7619_EDMAC_adapter.h"
#include "..\USART\types.h"
//#include "..\iodefine.h"

//////////////////////////////////////////////////////////////////////////////
// SH7619_EDMAC
//

#define LOOP_100us			2000

#define SH7619C_PHY_ADDR	31

enum link{
	NEGO_FAIL = 0, HALF_10M, FULL_10M, HALF_TX, FULL_TX,
};

enum init_mode{
	HALF_10T_SET   			= 0,
	FULL_10T_SET   			= 1,
	HALF_100TX_SET 			= 2,
	FULL_100TX_SET 			= 3,
	HALF_100TX_AUTONEGO_SET = 4,
	POWERDOWN_SET  			= 6,
	FULL_100TX_AUTONEGO_SET = 7,
};


/* SH7619 CONFIGURATION */
#define     CFG_NUM_SH7619            NETWORK_INTERFACE_COUNT                                    
#define     CFG_MAX_PACKETS_PROCESSED   10       /* The maximum number of packets to process in one shot */

#define     SH7619_FULL_DUPLEX        0           /* DON'T CHANGE adds support for full duplex */
#define     SH7619_MAXIMUM_FRAME_SIZE 1530        /* maximum frame sizes to be transmitted */
                                    
//Ether
static unsigned short   gmac_address[3] = {0x1A0E, 0x0FF8, 0x06F3};

#define ACT     0x80000000
#define DL      0x40000000
#define FP1     0x20000000
#define FP0     0x10000000
#define FE      0x08000000

#define RFOVER  0x00000200
#define RMAF    0x00000080
#define RRF     0x00000010
#define RTLF    0x00000008
#define RTSF    0x00000004
#define PRE     0x00000002
#define CERF    0x00000001

#define ITF     0x00000010
#define CND     0x00000008
#define DLC     0x00000004
#define CD      0x00000002
#define TRO     0x00000001

/****************************************************/

static unsigned long HEAP_TOP; // for memory alloc

#define VL(x) (*(volatile unsigned long *)(x))
#define VS(x) (*(volatile unsigned short *)(x))
#define VB(x) (*(volatile unsigned char *)(x))

#ifndef BIT
    #define BIT(n)  (1<<(n))
#endif

/* Base Address &   Modules Included */
#define CPU_BASE            0xF8000000  
#define PFC_BASE            0xF8050000  

#define PHY_BASE            0xF8490000
#define BSC_BASE            0xF8FD0000          
#define EDMAC0_BASE         0xFB000000
#define ETHERC0_BASE        0xFB000100

/* INTC Registers */
#define ICR0                VS(0xF8140000)
#define IRQCR               VS(0xF8140002)
#define IRQSR               VS(0xF8140004)

#define IPRA                VS(0xF8140006)  /* Interrupt Priority Registers */
#define IPRB                VS(0xF8140008)
#define IPRC                VS(0xF8080000)
#define IPRD                VS(0xF8080002)
#define IPRE                VS(0xF8080004)
#define IPRF                VS(0xF8080006)
#define IPRG                VS(0xF8080008)

/* CPG - PHY */
#define MCLKCR_R            VB(0xF80A000C)
#define MCLKCR_W            VS(0xF80A000C)

/* CCR Register */
#define CCR                 VL(0xFFFFFFEC)
#define CCR2                VL(0xA40000B0)
#define CCR3                VL(0xA40000B4)

/* CCR Values */
#define CCR_CF                  0x08
#define CCR_CB                  0x04
#define CCR_WT                  0x02
#define CCR_CE                  0x01

/* Clock Pulse Generator */
#define FRQCR               VS(CPU_BASE+0x0015FF80)

/* Power-Down Modes */
#define STBCR               VB(CPU_BASE+0x0015FF82)
#define STBCR2              VB(CPU_BASE+0x0015FF88)
#define STBCR3              VB(CPU_BASE+0x000A0000)
#define STBCR4              VB(CPU_BASE+0x000A0004)
#define STBCR5              VB(CPU_BASE+0x000A0010)

#define PADRH           VS(PFC_BASE+0x00)
#define PAIORH          VS(PFC_BASE+0x04)
#define PACRH1          VS(PFC_BASE+0x08)
#define PACRH2          VS(PFC_BASE+0x0A)
#define PBDRL           VS(PFC_BASE+0x12)
#define PBIORL          VS(PFC_BASE+0x16)
#define PBCRL1          VS(PFC_BASE+0x1C)
#define PBCRL2          VS(PFC_BASE+0x1E)
#define PCDRH           VS(PFC_BASE+0x20)
#define PCDRL           VS(PFC_BASE+0x22)
#define PCIORH          VS(PFC_BASE+0x24)
#define PCIORL          VS(PFC_BASE+0x26)
#define PCCRH2          VS(PFC_BASE+0x2A)
#define PCCRL1          VS(PFC_BASE+0x2C)
#define PCCRL2          VS(PFC_BASE+0x2E)
#define PDDRL           VS(PFC_BASE+0x32)
#define PDIORL          VS(PFC_BASE+0x36)
#define PDCRL2          VS(PFC_BASE+0x3E)
#define PEDRH           VS(PFC_BASE+0x40)
#define PEDRL           VS(PFC_BASE+0x42)
#define PEIORH          VS(PFC_BASE+0x44)
#define PEIORL          VS(PFC_BASE+0x46)
#define PECRH1          VS(PFC_BASE+0x48)
#define PECRH2          VS(PFC_BASE+0x4A)
#define PECRL1          VS(PFC_BASE+0x4C)
#define PECRL2          VS(PFC_BASE+0x4E)

/* 7710/7619 Ether Module Definitions */
#ifdef ETHERC0_BASE
	/* EtherC (MAC-0) */
	#define ECMR0           VL(ETHERC0_BASE+0x60)
	#define ECSR0           VL(ETHERC0_BASE+0x64)
	#define ECSIPR0         VL(ETHERC0_BASE+0x68)
	#define PIR0            VL(ETHERC0_BASE+0x6C)
	#define MAHR0           VL(ETHERC0_BASE+0x70)
	#define MALR0           VL(ETHERC0_BASE+0x74)
	#define RFLR0           VL(ETHERC0_BASE+0x78)
	#define PSR0            VL(ETHERC0_BASE+0x7C)
	#define TROCR0          VL(ETHERC0_BASE+0x80)
	#define CDCR0           VL(ETHERC0_BASE+0x84)
	#define LCCR0           VL(ETHERC0_BASE+0x88)
	#define CNDCR0          VL(ETHERC0_BASE+0x8C)
	#define CEFCR0          VL(ETHERC0_BASE+0x94)
	#define FRECR0          VL(ETHERC0_BASE+0x98)
	#define TSFRCR0         VL(ETHERC0_BASE+0x9C)
	#define TLFRCR0         VL(ETHERC0_BASE+0xA0)
	#define RFCR0           VL(ETHERC0_BASE+0xA4)
	#define MAFCR0          VL(ETHERC0_BASE+0xA8)
	#define IPGR0           VL(ETHERC0_BASE+0xB4)
	#define APR0            VL(ETHERC0_BASE+0xB8)
	#define MPR0            VL(ETHERC0_BASE+0xBC)
	#define TPAUSER0        VL(ETHERC0_BASE+0xC4)
#endif

#ifdef  EDMAC0_BASE
	/* E-DMAC 0 */
	#define EDMR0           VL(EDMAC0_BASE+0x00)
	#define EDTRR0          VL(EDMAC0_BASE+0x04)
	#define EDRRR0          VL(EDMAC0_BASE+0x08)
	#define TDLAR0          VL(EDMAC0_BASE+0x0C)
	#define RDLAR0          VL(EDMAC0_BASE+0x10)
	#define EESR0           VL(EDMAC0_BASE+0x14)
	#define EESIPR0         VL(EDMAC0_BASE+0x18)
	#define TRSCER0         VL(EDMAC0_BASE+0x1C)
	#define RMFCR0          VL(EDMAC0_BASE+0x20)
	#define TFTR0           VL(EDMAC0_BASE+0x24)
	#define FDR0            VL(EDMAC0_BASE+0x28)
	#define RMCR0           VL(EDMAC0_BASE+0x2C)
	#define EDOCR0          VL(EDMAC0_BASE+0x30)
	#define FCFTR0          VL(EDMAC0_BASE+0x34)
	#define RPADIR0         VL(EDMAC0_BASE+0x38)
	#define TRIMD0          VL(EDMAC0_BASE+0x3C)
	#define RBWAR0          VL(EDMAC0_BASE+0x40)
	#define RDFAR0          VL(EDMAC0_BASE+0x44)
	#define TBRAR0          VL(EDMAC0_BASE+0x4C)
	#define TDFAR0          VL(EDMAC0_BASE+0x50)
#endif

#ifdef PHY_BASE
	#define PHYCR           VS(PHY_BASE+0x00)
	#define PHYSMIR2        VS(PHY_BASE+0x04)
	#define PHYSMIR3        VS(PHY_BASE+0x08)
	#define PHYADDR         VS(PHY_BASE+0x0C)
	#define PHYSR           VS(PHY_BASE+0x10)
#endif


/*//////////////////////////////////////////////////////////////////////////*/
/* Status register bits*/
/*//////////////////////////////////////////////////////////////////////////*/
#define IMASK 0x000000f0L /* interrupt mask level field*/
#define SR_BL BIT(28)   /* exception/interrupt block bit*/

#define BASIC_MODE_CONTROL_REG              (0x0000)
#define BASIC_MODE_STATUS_REG               (0x0001)
#define PHY_IDENTIFIER1_REG                 (0x0002)
#define PHY_IDENTIFIER2_REG                 (0x0003)
#define AN_ADVERTISEMENT_REG                (0x0004)
#define AN_LINK_PARTNER_ABILITY_REG         (0x0005)
#define AN_EXPANSION_REG                    (0x0006)
#define NWAY_SETUP_REG                      (0x0010)
#define LPBACK_BYPASS_RX_ERR_MASK_REG       (0x0011)
#define RX_ER_COUNTER_REG                   (0x0012)
#define BPS10M_NETIF_CONFIGURATION_REG      (0x0013)
#define PHY1_1_REG                          (0x0014)
#define PHY1_2_REG                          (0x0015)
#define PHY2_REG                            (0x0016)
#define TWISTER1_REG                        (0x0017)
#define TWISTER2_REG                        (0x0018)
#define TEST_REG                            (0x0019)

#define PHY_ST      (0x0001)
#define PHY_READ    (0x0002)
#define PHY_WRITE   (0x0001)
#define PHY_ADDR    (0x0001)

static unsigned short   rtl8201bl_data[17];
static unsigned short   phy_addr[17] =
{
	BASIC_MODE_CONTROL_REG,
	BASIC_MODE_STATUS_REG,
	PHY_IDENTIFIER1_REG,
	PHY_IDENTIFIER2_REG,
	AN_ADVERTISEMENT_REG,
	AN_LINK_PARTNER_ABILITY_REG,
	AN_EXPANSION_REG,
	NWAY_SETUP_REG,
	LPBACK_BYPASS_RX_ERR_MASK_REG,
	RX_ER_COUNTER_REG,
	BPS10M_NETIF_CONFIGURATION_REG,
	PHY1_1_REG,
	PHY1_2_REG,
	PHY2_REG,
	TWISTER1_REG,
	TWISTER2_REG,
	TEST_REG
};

// URAM
#define URAM_TOP 0xE55FC000
#define URAM_END 0xE5600000

// CS3
#define CS3_TOP 0xAC000000


#define ETHERC_MDI          0x08
#define ETHERC_MDI_SRB      3       // Shift Right Bits by 3
#define ETHERC_MDO_WRITE    0x04
#define ETHERC_MDO_SLB      2       // Shift Left Bits by 2
#define ETHERC_MMD          0x02
#define ETHERC_MDC          0x01


///////////////////////////////////////////////////////////////////////////////
// PHY management
///////////////////////////////////////////////////////////////////////////////

#define OPWRITE 0x01
#define OPREAD 0x02

///////////////////////////////////////////////////////////////////////////////
// PHY Registers
///////////////////////////////////////////////////////////////////////////////

#define PHY_CTRL    0x00        // PHY Control
#define PHY_STAT    0x01        // PHY Status
#define PHY_ID1     0x02        // PHY ID # 1
#define PHY_ID2     0x03        // PHY ID # 2
#define PHY_ANEGAD  0x04        // Auto Negotiation Advertisement
#define PHY_ANEGREC 0x05        // Auto Negotiation Remote End Capacity
#define PHY_CFG1    0x10        // Configuration 1
#define PHY_CFG2    0x11        // Configuration 2
#define PHY_STATOUT 0x12        // Status Output
#define PHY_MASK    0x13        // Mask

//static char *(ptr_rxbuf[ENTRY]);        /* receive data buffer */
//static char *(ptr_txbuf[ENTRY]);        /* transmit data buffer */

struct SH7619_EDMAC_SOFTC
{
	PIFACE          iface;
	struct          ether_statistics stats;
	RTP_UINT8       mac_address[6];
};
typedef struct SH7619_EDMAC_SOFTC RTP_FAR * PSH7619_EDMAC_SOFTC;

PIFACE              iface;

static void Init_Malloc                 (                                               );
static int* Malloc_Buf                  ( int size                                      );
static int  Ether_PFC_Init              (                                               );
static int  Ether_Init                  (                                               );
static unsigned short Ether_Reg_Read    ( unsigned short reg_addr                       );
static void Ether_Reg_Write             ( unsigned short reg_addr, unsigned short data  );
static void Write_MDO                   ( vuint8_t channel, unsigned MGMTData           );
static vuint32_t Read_MDI               ( vuint8_t channel, unsigned MGMTData           );
static vuint16_t PHYAccess              ( vuint8_t channel, vuint8_t PHYAdd, vuint8_t RegAdd, vuint8_t OPCode, vuint16_t wData  );
static void Flush_All					( PIFACE pi										);
static void SH7619_EDMAC_Init			(												);
static BOOL SH7619_EDMAC_GetLinkSpeed	( BOOL applySetting								);

/////////////////////////////// EDMAC define ////////////////////////////
// Callback functions type
typedef void (*EMAC_TxCallback)(unsigned int status);
typedef void (*EMAC_RxCallback)(unsigned int status);
typedef void (*EMAC_WakeupCallback)(void);

// Number of buffer for RX
#define RX_BUFFERS  2048
// Number of buffer for TX
#define TX_BUFFERS  64

// Buffer Size
#define EMAC_RX_UNITSIZE            512     /// Fixed size for RX buffer
#define EMAC_TX_UNITSIZE            512    /// Size for ETH frame length

// Definitions used by EMAC Descriptors
#define EMAC_ADDRESS_MASK   ((UINT32)0xFFFFFFFC)
#define EMAC_LENGTH_FRAME   ((UINT32)0x0FFF)    /// Length of frame mask

// Describes the type and attribute of Transfer descriptor.
typedef struct _EmacTDescriptor {
	//UINT32 addr;
	UINT32 status;
	UINT16 TDRBL;
	UINT16 RDL;
	char* TRBA;
} EmacTDescriptor, *PEmacTDescriptor;

typedef struct {
	PEmacTDescriptor td[RX_BUFFERS];
	UINT16 idx;
} RxTd;

typedef struct {
	PEmacTDescriptor td[TX_BUFFERS];
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
#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

// Return space available, 0..size-1
// We always leave one free char as a completely full buffer 
// has head == tail, which is the same as empty
#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

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

//////////////////////// end ///////////////////////////////////////////
    
#endif

