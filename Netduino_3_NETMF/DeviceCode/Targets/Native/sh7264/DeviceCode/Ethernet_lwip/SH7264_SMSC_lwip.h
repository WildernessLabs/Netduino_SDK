////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SH7264_SMSC_LWIP_H_1
#define _SH7264_SMSC_LWIP_H_1 1

#include <tinyhal.h>
#include "SH7264_SMSC_lwip_adapter.h"
#include "types.h"

// TODO - what should this really be ?
#define ETHERSIZE  1514         /* maximum number of bytes in ETHERNET packet */


BOOL  SH7264_SMSC_lwip_open(struct netif *pNetIf);
void  SH7264_SMSC_lwip_close(struct netif *pNetIf);
err_t SH7264_SMSC_lwip_xmit(struct netif *pNetIf, struct pbuf *pPBuf);
void  SH7264_SMSC_lwip_recv(struct netif *pNetIf);
void  SH7264_SMSC_interrupt(void *param);
void  SH7264_SMSC_lwip_interrupt(void *param);


#define VL(x) (*(volatile unsigned long *)(x))
#define VS(x) (*(volatile unsigned short *)(x))
#define VB(x) (*(volatile unsigned char *)(x))


#define SMSC9218_BASE       0x28000000      


#define ICR1                            VS(0xFFFE0802)
#define IPR01                           VS(0xFFFE0818)
#define PECR0                           VS(0xFFFE388E)
#define PJCR0                           VS(0xFFFE390E)


#define GPT_INT_INTERVAL                500UL       /* 50mSec */

#define GPIO_CFG_LED1_DIS_ (GPIOCFG_GPIOBUF0|GPIOCFG_GPDIR0|GPIOCFG_GPIOD0)

#define LINKMODE_ANEG                   0x40UL
#define LINKMODE_ASYM_PAUSE             0x20UL
#define LINKMODE_SYM_PAUSE              0x10UL
#define LINKMODE_100_FD                 0x08UL
#define LINKMODE_100_HD                 0x04UL
#define LINKMODE_10_FD                  0x02UL
#define LINKMODE_10_HD                  0x01UL
#define LINKMODE_DEFAULT                0x7FUL

#define LINK_NO_LINK        (0UL)
#define LINK_10MPS_HALF     (1UL)
#define LINK_10MPS_FULL     (2UL)
#define LINK_100MPS_HALF    (3UL)
#define LINK_100MPS_FULL    (4UL)
#define LINK_AUTO_NEGOTIATE (5UL)

//
// SMSC9218 Directly Addressable Registers
//
#define RX_DATA_FIFO_PORT       VL(SMSC9218_BASE +  0x00)   // RX Data FIFO Port
#define TX_DATA_FIFO_PORT       VL(SMSC9218_BASE +  0x20)   // TX Data FIFO Port
#define RX_STS_FIFO_PORT        VL(SMSC9218_BASE +  0x40)   // RX Status FIFO Port
#define RX_STS_FIFO_PEEK        VL(SMSC9218_BASE +  0x44)   // RX Status FIFO Peek
#define TX_STS_FIFO_PORT        VL(SMSC9218_BASE +  0x48)   // TX Status FIFO Port
#define TX_STS_FIFO_PEEK        VL(SMSC9218_BASE +  0x4C)   // TX Status FIFO Peek
#define ID_REV                  VL(SMSC9218_BASE +  0x50)   // Chip IP and Revision
#define INT_CFG                 VL(SMSC9218_BASE +  0x54)   // Main Interrupt Configuration
#define INT_STS                 VL(SMSC9218_BASE +  0x58)   // Interrupt Status
#define INT_EN                  VL(SMSC9218_BASE +  0x5C)   // Interrupt Enable Register
#define BYTE_TEST               VL(SMSC9218_BASE +  0x64)   // Read-only byte order testing register
#define FIFO_INT                VL(SMSC9218_BASE +  0x68)   // FIFO Level Interrupts
#define RX_CFG                  VL(SMSC9218_BASE +  0x6C)   // Receive configuration
#define TX_CFG                  VL(SMSC9218_BASE +  0x70)   // Transmit Configuration
#define HW_CFG                  VL(SMSC9218_BASE +  0x74)   // Hardware Configuration
#define RX_DP_CTRL              VL(SMSC9218_BASE +  0x78)   // RX Datapath Control
#define RX_FIFO_INF             VL(SMSC9218_BASE +  0x7C)   // Receive FIFO Information
#define TX_FIFO_INF             VL(SMSC9218_BASE +  0x80)   // Transmit FIFO Information
#define PMT_CTRL                VL(SMSC9218_BASE +  0x84)   // Power Management Control
#define GPIO_CFG                VL(SMSC9218_BASE +  0x88)   // General Purpose IO Configuraion
#define GPT_CFG                 VL(SMSC9218_BASE +  0x8C)   // General Purpose Timer
#define GPT_CNT                 VL(SMSC9218_BASE +  0x90)   // General Purpose Timer Count
#define ENDIAN                  VL(SMSC9218_BASE +  0x98)   // ENDIAN
#define FREE_RUN                VL(SMSC9218_BASE +  0x9C)   // Free Run Counter
#define RX_DROP                 VL(SMSC9218_BASE +  0xA0)   // RX Dropped Frames Counter
#define MAC_CSR_CMD             VL(SMSC9218_BASE +  0xA4)   // used to access the MAC CSRs
#define MAC_CSR_DATA            VL(SMSC9218_BASE +  0xA8)   // used to access the MAC CSRs
#define AFC_CFG                 VL(SMSC9218_BASE +  0xAC)   // Automatic Flow Control
#define E2P_CMD                 VL(SMSC9218_BASE +  0xB0)   // used to access the EEPROM
#define E2P_DATA                VL(SMSC9218_BASE +  0xB4)   // used to access the EEPROM
#define WORD_SWAP               VL(SMSC9218_BASE +  0x98)   

// TX Buffer Format
// TX COMMAND A
#define TX_CMD_A_INTR_COMPLETE          0x80000000  // Interrupt on Completion
#define TX_CMD_A_BUFF_END_ALIGN_4       0x00000000  // Buffer End Alignment(4-byte alignment)
#define TX_CMD_A_BUFF_END_ALIGN_16      0x01000000  // Buffer End Alignment(16-byte alignment)
#define TX_CMD_A_BUFF_END_ALIGN_32      0x02000000  // Buffer End Alignment(32-byte alignment)
#define TX_CMD_A_DATA_START_OFFSET_MASK 0x001F0000  // Data Start Offset(bytes)
#define TX_CMD_A_FIRST_SEG              0x00002000  // First Segment
#define TX_CMD_A_LAST_SEG               0x00001000  // Last Segment
#define TX_CMD_A_BUFFER_SIZE_MASK       0x000007FF  // Buffer Size(bytes)

// TX COMMAND B
#define TX_CMD_B_PACKET_TAG_MASK        0xFFFF0000  // Packet Tag
#define TX_CMD_B_ADD_CRC_DISABLEK       0x00002000  // Add CRC Disable
#define TX_CMD_B_DISABLE_ETH_FRAME_PAD  0x00001000  // Disable Ethernet Frame Pazdding
#define TX_CMD_B_PACKET_LEN_MASK        0x000007FF  // Packet Length(bytes)

// RX Status FIFO Port
#define RX_STS_FILTERING_FAIL   0x40000000  // Filtering Fail
#define RX_STS_PAKCET_LEN_MASK  0x3FFF0000  // Packet Length
#define RX_STS_ERROR            0x00008000  // Error Status
#define RX_STS_BROADCAST_FRAME  0x00002000  // Broadcast Frame
#define RX_STS_LEN_ERROR        0x00001000  // Length Error
#define RX_STS_RUNT_FRAME       0x00000800  // Bunt Frame
#define RX_STS_MULTICAST_FRAME  0x00000400  // Multicast Frame
#define RX_STS_FRAME_TOO_LONG   0x00000080  // Frame Too Long
#define RX_STS_COLLISION_SEEN   0x00000040  // Collision Seen
#define RX_STS_FRAME_TYPE       0x00000020  // Frame Type
#define RX_STS_RECV_WATCHDOG    0x00000010  // Receive Watchdog time-out
#define RX_STS_MII_ERR          0x00000008  // MII Error
#define RX_STS_DRIBBLING_BIT    0x00000004  // Dribbling Bit
#define RX_STS_CRC_ERR          0x00000002  // CRC Error

// TX Status FIFO Port
#define TX_STS_PACKET_TAG_MASK      0xFFFF0000  // Packet TAG
#define TX_STS_ERROR                0x00008000  // Error Status
#define TX_STS_LOSS_CARRIER         0x00000800  // Loss of Carrier
#define TX_STS_NO_CARRIER           0x00000400  // No Carrier
#define TX_STS_LATE_COLLISION       0x00000200  // Late Collision
#define TX_STS_EXCESSIVE_COLLISION  0x00000200  // Excessive Collision
#define TX_STS_COLLISION_CNT_MASK   0x00000078  // Collision Count
#define TX_STS_EXCESSIVE_DEFERRAL   0x00000004  // Excessive Deferral
#define TX_STS_UINDERRUN_ERROR      0x00000002  // Underrun Error
#define TX_STS_DEFERRED             0x00000001  // Deferred

// Chip IP and Revision
#define IDREV_CHIPID_MASK       0xFFFF0000
#define IDREV_REVISION_MASK     0x0000FFFF

// Main Interrupt Configuration
#define INTCFG_INT_DEAS_DISABLE     0x00000000  // Interrupt Deassertion Interval
#define INTCFG_INT_DEAS_CLR         0x00004000  // Interrupt Deassertion Interval Clear
#define INTCFG_INT_DEAS_STS         0x00002000  // Interrupt Deassertion Status
#define INTCFG_IRQ_INT              0x00001000  // Master Interrupt
#define INTCFG_IRQ_EN               0x00000100  // IRQ Enable
#define INTCFG_IRQ_POL_ACTIVELOW    0x00000000  // IRQ Polarity
#define INTCFG_IRQ_POL_ACTIVEHIGH   0x00000010  // IRQ Polarity
#define INTCFG_IRQ_TYPE_OPENDRAIN   0x00000000  // IRQ Buffer Type(Open-drain buffer)
#define INTCFG_IRQ_TYPE_PUSHPULL    0x00000001  // IRQ Buffer Type(Push-Pull driver)

// Interrupt Status
#define INTSTS_SW_INT               0x80000000  // Software Interrupt
#define INTSTS_TXSTOP_INT           0x02000000  // TX Stopped
#define INTSTS_RXSTOP_INT           0x01000000  // RX Stopped
#define INTSTS_RXDFH_INT            0x00800000  // RX Dropped Frame Counter Halfway
#define INTSTS_TX_IOC_INT           0x00200000  // TX IOC Interrupt
#define INTSTS_RXD_INT              0x00100000  // RX DMA Interrupt
#define INTSTS_GPT_INT              0x00080000  // GP Timer
#define INTSTS_PHY_INT              0x00040000  // PHY
#define INTSTS_PME_INT              0x00020000  // Power Management Event Interrupt
#define INTSTS_TXSO_INT             0x00010000  // TX Status FIFO Overflow
#define INTSTS_RWT_INT              0x00008000  // Receive Watchdog Time-out
#define INTSTS_RXE_INT              0x00004000  // Receiver Error
#define INTSTS_TXE_INT              0x00002000  // Transmitter Error
#define INTSTS_TDFU_INT             0x00000800  // TX Data FIFO Underrun Interrupt
#define INTSTS_TDFO_INT             0x00000400  // TX Data FIFO FIFO Overrun Interrupt
#define INTSTS_TDFA_INT             0x00000200  // TX Data FIFO Available Interrupt
#define INTSTS_TSFF_INT             0x00000100  // TX Status FIFO Full Interrupt
#define INTSTS_TSFL_INT             0x00000080  // TX Status FIFO Level Interrupt
#define INTSTS_RXDF_INT             0x00000040  // RX Dropped Frame Interrupt
#define INTSTS_RDFL_INT             0x00000020  // RX Data FIFO Level Interrupt
#define INTSTS_RSFF_INT             0x00000010  // RX Status FIFO Full Interrupt
#define INTSTS_RSFL_INT             0x00000008  // RX Status FIFO Level Interrupt
#define INTSTS_GPIOX_INT_MASK       0x00000007  // GPIO

// Interrupt Enable Register
#define INTEN_SW_INT_EN             0x80000000  // Software Interrupt
#define INTEN_TXSTOP_INT_EN         0x02000000  // TX Stopped Interrupt
#define INTEN_RXSTOP_INT_EN         0x01000000  // RX Stopped Interrupt
#define INTEN_RXDFH_INT_EN          0x00800000  // RX Dropped Frame Counter Halfway Interrupt
#define INTEN_TIOC_INT_EN           0x00200000  // TX IOC Interrupt
#define INTEN_RXD_INT_EN            0x00100000  // RX DMA Interrupt
#define INTEN_GPT_INT_EN            0x00080000  // GP Timer
#define INTEN_PHY_INT_EN            0x00040000  // PHY
#define INTEN_PME_INT_EN            0x00020000  // Power Management Event Interrupt
#define INTEN_TXSO_INT_EN           0x00010000  // TX Status FIFO Overflow
#define INTEN_RWT_INT_EN            0x00008000  // Receive Watchdog Time-out
#define INTEN_RXE_INT_EN            0x00004000  // Receiver Error
#define INTEN_TXE_INT_EN            0x00002000  // Transmitter Error
#define INTEN_TDFU_INT_EN           0x00000800  // TX Data FIFO Underrun Interrupt
#define INTEN_TDFO_INT_EN           0x00000400  // TX Data FIFO FIFO Overrun Interrupt
#define INTEN_TDFA_INT_EN           0x00000200  // TX Data FIFO Available Interrupt
#define INTEN_TSFF_INT_EN           0x00000100  // TX Status FIFO Full Interrupt
#define INTEN_TSFL_INT_EN           0x00000080  // TX Status FIFO Level Interrupt
#define INTEN_RXDF_INT_EN           0x00000040  // RX Dropped Frame Interrupt
#define INTEN_RDFL_INT_EN           0x00000020  // RX Data FIFO Level Interrupt
#define INTEN_RSFF_INT_EN           0x00000010  // RX Status FIFO Full Interrupt
#define INTEN_RSFL_INT_EN           0x00000008  // RX Status FIFO Level Interrupt
#define INTEN_GPIOX_INT_EN_MASK     0x00000007  // GPIO

// FIFO Level Interrupts
#define FIFOLVL_TXDAT_MASK  0xFF000000  // TX Data Available Level
#define FIFOLVL_TXSTS_MASK  0x00FF0000  // TX Status Level
#define FIFOLVL_RXSPC_MASK  0x0000FF00  // RX Space Available Level
#define FIFOLVL_RXSTS_MASK  0x000000FF  // RX Status Level

// Receive configuration
#define RXCFG_RX_END_ALIGN_MASK 0xC0000000  // RX End Alignment
#define RXCFG_RX_END_ALIGN_4        0x00000000  // RX End Alignment(4-byte alignment)
#define RXCFG_RX_END_ALIGN_16       0x40000000  // RX End Alignment(16-byte alignment)
#define RXCFG_RX_END_ALIGN_32       0x80000000  // RX End Alignment(32-byte alignment)
#define RXCFG_RX_DMA_CNT_MASK       0x0FFF0000  // RX DMA Count
#define RXCFG_RX_DUMP               0x00008000  // Force RX Discard
#define RXCFG_RXDOFF_MASK           0x00001F00  // RX Data Offset

// Transmit Configuration
#define TXCFG_TXS_DUMP              0x00008000  // Force TX Status Discard
#define TXCFG_TXD_DUMP              0x00004000  // Force TX Data Discard
#define TXCFG_TXSAO                 0x00000004  // TX Status Allow Overrun
#define TXCFG_TX_ON                 0x00000002  // Transmitter Enable
#define TXCFG_STOP_TX               0x00000001  // Stop Transmitter

// Hardware Configuration
#define HWCFG_TTM_100M              0x00000000  // Transmit Threshold Mode(100Mbps)
#define HWCFG_TTM_10M               0x00200000  // Transmit Threshold Mode(10Mbps)
#define HWCFG_SF                    0x00100000  // Store and Forward
#define HWCFG_TX_FIF_SZ_MASK        0x000F0000  // TX FIFO Size
#define HWCFG_TR_MASK               0x00003000  // Threshold Control Bits
#define HWCFG_TR_10M_012            0x00000000  //  Threshold 012h
#define HWCFG_TR_10M_018            0x00001000  //  Threshold 018h
#define HWCFG_TR_10M_020            0x00002000  //  Threshold 020h
#define HWCFG_TR_10M_028            0x00003000  //  Threshold 028h
#define HWCFG_TR_100M_020           0x00000000  //  Threshold 020h
#define HWCFG_TR_100M_040           0x00001000  //  Threshold 040h
#define HWCFG_TR_100M_080           0x00002000  //  Threshold 080h
#define HWCFG_TR_100M_100           0x00003000  //  Threshold 100h
#define HWCFG_MODE_16_BIT           0x00000000  // 16-bit Mode
#define HWCFG_MODE_32_BIT           0x00000004  // 32-bit Mode
#define HWCFG_SRST_TO               0x00000002  // Soft Reset Time-out
#define HWCFG_SRST                  0x00000001  // Soft Reset

// RX Datapath Control
#define RXDPCTL_RX_FFWD             0x80000000  // RX Data FIFO Fast Forward

// Receive FIFO Information
#define RXFIFO_RXSUSED_MASK         0x00FF0000  // RX Status FIFO Used Space
#define RXFIFO_RXDUSED_MASK         0x0000FFFF  // RX Data FIFO Used Space

// Transmit FIFO Information
#define TXFIFO_TXSUSED_MASK         0x00FF0000  // TX Status FIFO Used Space
#define TXFIFO_TDFREE_MASK          0x0000FFFF  // TX Data FIFO Free Space

// Power Management Control
#define PMTCTL_PM_MODE_MASK         0x00003000  // Power Management Mode
#define PMTCTL_PM_MODE_D0           0x00000000  //  normal operation
#define PMTCTL_PM_MODE_D1           0x00001000  //  wake-up frame and magic packet detection are enabled
#define PMTCTL_PM_MODE_D2           0x00002000  //  can perform energy detect
#define PMTCTL_PHY_RST              0x00000400  // PHY Reset
#define PMTCTL_WOL_EN               0x00000200  // Wake-On-Lan Enable
#define PMTCTL_ED_EN                0x00000100  // Energy-Detect Enable
#define PMTCTL_PME_TYPE_OPENDRAIN   0x00000000  // PME Buffer Type(open-drain buffer)
#define PMTCTL_PME_TYPE_PUSHPULL    0x00000040  // PME Buffer Type(Push-Pull driver)
#define PMTCTL_WUPS_MASK            0x00000030  // WAKE-UP Status
#define PMTCTL_WUPS_NO              0x00000000  //  No wake-up event detected
#define PMTCTL_WUPS_ENERGY          0x00000010  //  Energy detected
#define PMTCTL_WUPS_WAKEUP          0x00000020  //  Wake-up frame or magic packet detected
#define PMTCTL_WUPS_MULTIEVENT      0x00000030  //  Indicates multiple events occurred
#define PMTCTL_PME_IND              0x00000008  // PME indication
#define PMTCTL_PME_POL_ACTIVEHIGH   0x00000004  // PME Polarity(active high)
#define PMTCTL_PME_POL_ACTIVELOW    0x00000000  // PME Polarity(active low)
#define PMTCTL_PME_EN               0x00000002  // 
#define PMTCTL_READY                0x00000001  // 

// General Purpose IO Configuraion
#define GPIOCFG_LED3_GPIO2_EN           0x40000000  // LED Enable(LED3/GPIO2)
#define GPIOCFG_LED2_GPIO1_EN           0x20000000  // LED Enable(LED2/GPIO1)
#define GPIOCFG_LED1_GPIO0_EN           0x10000000  // LED Enable(LED1/GPIO0)
#define GPIOCFG_GPIO2_INT_POL           0x04000000  // GPIO Interrupt Polarity 2
#define GPIOCFG_GPIO1_INT_POL           0x02000000  // GPIO Interrupt Polarity 1
#define GPIOCFG_GPIO0_INT_POL           0x01000000  // GPIO Interrupt Polarity 0
#define GPIOCFG_EEPR_EN_MASK            0x00700000  // EEPROM Enable    EEDIO FUNC  EECLK FUNC
#define GPIOCFG_EEPR_EN_EEDIO_EECLK     0x00000000  //                  EEDIO       EECLK
#define GPIOCFG_EEPR_EN_GPO3_GPO4       0x00100000  //                  GPO3        GPO4
#define GPIOCFG_EEPR_EN_GPO3_RX_DV      0x00300000  //                  GPO3        RX_DV
#define GPIOCFG_EEPR_EN_TX_EN_GPO4      0x00500000  //                  TX_EN       GPO4
#define GPIOCFG_EEPR_EN_TX_EN_RX_DV     0x00600000  //                  TX_EN       RX_DV
#define GPIOCFG_EEPR_EN_TX_CLK_RX_CLK   0x00700000  //                  TX_CLK      RX_CLK
#define GPIOCFG_GPIOBUF2                0x00040000  // GPIO Buffer Type 2
#define GPIOCFG_GPIOBUF1                0x00020000  // GPIO Buffer Type 1
#define GPIOCFG_GPIOBUF0                0x00010000  // GPIO Buffer Type 0
#define GPIOCFG_GPDIR2                  0x00000400  // GPIO Direction 2
#define GPIOCFG_GPDIR1                  0x00000200  // GPIO Direction 1
#define GPIOCFG_GPDIR0                  0x00000100  // GPIO Direction 0
#define GPIOCFG_GPOD4                   0x00000010  // GPIO Data 4
#define GPIOCFG_GPOD3                   0x00000008  // GPIO Data 3
#define GPIOCFG_GPIOD2                  0x00000004  // GPIO Data 2
#define GPIOCFG_GPIOD1                  0x00000002  // GPIO Data 1
#define GPIOCFG_GPIOD0                  0x00000001  // GPIO Data 0

// General Purpose Timer
#define GPTCFG_TIMER_EN                 0x20000000  // GP Timer Enable
#define GPTCFG_GPT_LOAD_MASK            0x0000FFFF  // General Purpose Timer Pre-Load

// General Purpose Timer Count
#define GPTCNT_GPT_CNT_MASK             0x0000FFFF  // General Purpose Timer Current Count

// MAC CSR Synchronizer Command Register
#define MAC_CSR_BUSY                    0x80000000
#define MAC_CSR_REQ_WRITE               0x00000000
#define MAC_CSR_REQ_READ                0x40000000
#define MAC_CSR_ADDR_MASK               0x000000FF

// Automatic Flow Control
#define AFCCFG_AFC_HI_MASK              0x00FF0000  // Automatic Flow Control High Level
#define AFCCFG_AFC_LO_MASK              0x0000FF00  // Automatic Flow Control Low Level
#define AFCCFG_BACK_DUR_MASK            0x000000F0  // Backpressure Duration
                                                    // 100Mbs Mode
#define AFCCFG_BACK_DUR_100M_600        0x000000F0  //  600us
#define AFCCFG_BACK_DUR_100M_550        0x000000E0  //  550us
#define AFCCFG_BACK_DUR_100M_500        0x000000D0  //  500us
#define AFCCFG_BACK_DUR_100M_450        0x000000C0  //  450us
#define AFCCFG_BACK_DUR_100M_400        0x000000B0  //  400us
#define AFCCFG_BACK_DUR_100M_350        0x000000A0  //  350us
#define AFCCFG_BACK_DUR_100M_300        0x00000090  //  300us
#define AFCCFG_BACK_DUR_100M_250        0x00000080  //  250us
#define AFCCFG_BACK_DUR_100M_200        0x00000070  //  200us
#define AFCCFG_BACK_DUR_100M_150        0x00000060  //  150us
#define AFCCFG_BACK_DUR_100M_100        0x00000050  //  100us
#define AFCCFG_BACK_DUR_100M_50         0x00000040  //  50us
#define AFCCFG_BACK_DUR_100M_25         0x00000030  //  25us
#define AFCCFG_BACK_DUR_100M_15         0x00000020  //  15us
#define AFCCFG_BACK_DUR_100M_10         0x00000010  //  10us
#define AFCCFG_BACK_DUR_100M_5          0x00000000  //  5us
                                                    // 10Mbs Mode
#define AFCCFG_BACK_DUR_10M_602_2       0x000000F0  //  602.2us
#define AFCCFG_BACK_DUR_10M_552_2       0x000000E0  //  552.2us
#define AFCCFG_BACK_DUR_10M_502_2       0x000000D0  //  502.2us
#define AFCCFG_BACK_DUR_10M_452_2       0x000000C0  //  452.2us
#define AFCCFG_BACK_DUR_10M_402_2       0x000000B0  //  402.2us
#define AFCCFG_BACK_DUR_10M_352_2       0x000000A0  //  352.2us
#define AFCCFG_BACK_DUR_10M_302_2       0x00000090  //  302.2us
#define AFCCFG_BACK_DUR_10M_252_2       0x00000080  //  252.2us
#define AFCCFG_BACK_DUR_10M_202_2       0x00000070  //  202.2us
#define AFCCFG_BACK_DUR_10M_152_2       0x00000060  //  152.2us
#define AFCCFG_BACK_DUR_10M_102_2       0x00000050  //  102.2us
#define AFCCFG_BACK_DUR_10M_52_2        0x00000040  //  52.2us
#define AFCCFG_BACK_DUR_10M_27_2        0x00000030  //  27.2us
#define AFCCFG_BACK_DUR_10M_17_2        0x00000020  //  17.2us
#define AFCCFG_BACK_DUR_10M_12_2        0x00000010  //  12.2us
#define AFCCFG_BACK_DUR_10M_7_2         0x00000000  //  7.2us
#define AFCCFG_FCMULT                   0x00000008  // Flow Control on Multicast Frame
#define AFCCFG_FCBRD                    0x00000004  // Flow Control on Broadcast Frame
#define AFCCFG_FCADD                    0x00000002  // Flow Control on Address Decode
#define AFCCFG_FCANY                    0x00000001  // Flow Control on Any Frame

//
// SMSC9218 MAC Control and Status Registers
//
#define MAC_CR      1   // MAC Control Register
#define ADDRH       2   // MAC Address High(upper 16 bits)
#define ADDRL       3   // MAC Address Low(lower 32 bits)
#define HASHH       4   // Multicast Hash Table High(upper 32 bits)
#define HASHL       5   // Multicast Hash Table Low(lower 32 bits)
#define MII_ACC     6   // MII Access
#define MII_DATA    7   // MII Data
#define FLOW        8   // Flow Control
#define VLAN1       9   // VLAN1 Tag
#define VLAN2       10  // VLAN2 Tag
#define WUFF        11  // Wake-up Frame Filter
#define WUCSR       12  // Wake-up Status and Control

// MAC Control Register
#define MAC_RXALL           0x80000000  // Receive All Mode
#define MAC_RCVOWN          0x00800000  // Disable Receive Own
#define MAC_LOOPBK          0x00200000  // Loopback operation Mode
#define MAC_FDPX            0x00100000  // Full Duplex Mode
#define MAC_MCPAS           0x00080000  // Pass All Multicast
#define MAC_PRMS            0x00040000  // Promiscuous Mode
#define MAC_INVFILT         0x00020000  // Inverse filtering
#define MAC_PASSBAD         0x00010000  // Pass Bad Frames
#define MAC_HO              0x00008000  // hash Only Filtering Mode
#define MAC_HPFILT          0x00002000  // hash/Perfect Filtering Mode
#define MAC_LCOLL           0x00001000  // Late Collision Control
#define MAC_BCAST           0x00000800  // Disable Broadcast Frames
#define MAC_DISRTY          0x00000400  // Disable Retry
#define MAC_PADSTR          0x00000100  // Automatic Pad Stripping
#define MAC_BOLMT_MASK      0x000000C0  // BackOff Limit
#define MAC_BOLMT_10        0x00000000  //  LFSR Counter 10
#define MAC_BOLMT_8         0x00000040  //  LFSR Counter 8
#define MAC_BOLMT_4         0x00000080  //  LFSR Counter 4
#define MAC_BOLMT_1         0x000000C0  //  LFSR Counter 1
#define MAC_DFCHK           0x00000020  // Deferral Check
#define MAC_TXEN            0x00000008  // Transmitter enable
#define MAC_RXEN            0x00000004  // Receiver Enable

// MAC Address High
#define MAC_ADDRH_MASK      0x0000FFFF  // Physical Address upper 16-bit

// MII Access
#define MAC_PHY_ADDR_MASK       0x0000F800  // PHY Address
#define MAC_PHY_ADDR_LSHIFT     11
#define MAC_PHY_ADDR_INTERNAL   0x00000800  // PHY Address(Internal MII)
#define MAC_PHY_MIIRINDA_MASK   0x000007C0  // MII Register Index
#define MAC_PHY_MIIRINDA_LSHIFT 6
#define MAC_PHY_MIIRINDA(r)     ((r & 0x0000001F)<<MAC_PHY_MIIRINDA_LSHIFT)
#define MAC_PHY_MIIWRITE        0x00000002  // MII Write
#define MAC_PHY_MIIREAD         0x00000000  // MII Read
#define MAC_PHY_MIIBZY          0x00000001  // MII Busy

// MII Data
#define MAC_PHY_DATA_MASK       0x000000FF  // MII Data

// Flow Control
#define MAC_FCPT_MASK           0xFFFF0000  // Pause Time
#define MAC_FCPASS              0x00000003  // Pass Control Frames
#define MAC_FCEN                0x00000002  // Flow Control Enable
#define MAC_FCBSY               0x00000001  // Flow Control Busy

// Wake-up Status and Control
#define MAC_GUE                 0x00000200  // Global Unicast Enable
#define MAC_WUFR                0x00000040  // Remote Wake-up Frame Received
#define MAC_MPR                 0x00000020  // Magic Packet Received
#define MAC_WUEN                0x00000004  // Wake-Up Frame enabled
#define MAC_WPEN                0x00000002  // Magic Packet Enable

//
// SMSC9218 PHY Registers
//
#define PHY_CONTROL         0       // Basic Control Register
#define PHY_STATUS          1       // Basic Status Register
#define PHY_ID0             2       // PHY Identifier 1
#define PHY_ID1             3       // PHY Identifier 2
#define PHY_ANADVERTISE     4       // Auto-Negotiation Advertisement Register
#define PHY_ANPARTNER       5       // Auto-Negotiation Link Partner Ability Register
#define PHY_ANEXPANSION     6       // Auto-Negotiation Expansion Register
#define PHY_MODE_CTLSTS     17      // Mode Control/Status Register
#define PHY_SP_MODES        18      // Special Modes Register
#define PHY_SP_CTLSTS       27      // Special Control/Status Indications
#define PHY_INTERRUPTSRC    29      // Interrupt Source Register
#define PHY_INTERRUPTMASK   30      // Interrupt Mask Register
#define PHY_PHY_SP_CTLSTS   31      // PHY Special Control/Status Register

// Basic Control Register
#define PHY_CTRL_RESET      0x8000
#define PHY_CTRL_LOOPBACK   0x4000
#define PHY_CTRL_SPEED      0x2000
#define PHY_CTRL_ANENABLE   0x1000
#define PHY_CTRL_PWRDWN     0x0800
#define PHY_CTRL_ISOLATION  0x0400
#define PHY_CTRL_ANRESTART  0x0200
#define PHY_CTRL_DUPLEX     0x0100

// Basic Status Register
#define PHY_STAT_LINK_UP        0x0004
#define PHY_STAT_LINK_DOWN      0x0000
#define PHY_STAT_ANCOMPLETE     0x0020
#define PHY_STAT_REMOTEFAULT    0x0010
#define PHY_STAT_ANABILITY      0x0008

// Auto-Negotiation Advertisement Register
#define PHY_ANA_PAUSE_OP_MASK   0x0C00
#define PHY_ANA_ASYM_PAUSE      0x0800
#define PHY_ANA_SYM_PAUSE       0x0400
#define PHY_ANA_100FDX          0x0100
#define PHY_ANA_100HDX          0x0080
#define PHY_ANA_10FDX           0x0040
#define PHY_ANA_10HDX           0x0020
#define PHY_ANA_SPEED_MASK      0x01E0

// Auto-Negotiation Link Partner Ability Register
#define PHY_ANLPA_100FDX        0x0100
#define PHY_ANLPA_100HDX        0x0080
#define PHY_ANLPA_10FDX         0x0040
#define PHY_ANLPA_10HDX         0x0020


// MAHR - MAC Adress High register
#define MALR_DUMMY_ADDR     0x0000e10c
                   
// MAHR - MAC Adress Low register
#define MAHR_DUMMY_ADDR     0x000050ff

#define     ETHERNET_DELAY(n)   { {   \
                            int _i = n; \
                            do { \
                                    volatile unsigned long _temp; \
                                    _temp = BYTE_TEST; \
                            } while (--_i); \
                        } }

#define     MAC_RD_CMD(Reg)         ( (Reg & 0x000000FF) | \
                                     (MAC_CSR_BUSY | MAC_CSR_REQ_READ) )
                                     
#define     MAC_WR_CMD(Reg)         ((Reg & 0x000000FF) | \
                                     (MAC_CSR_BUSY))

#define     MAC_MII_RD_CMD(Addr,Reg)    (((Addr & 0x1f) << 11) | \
                                         ((Reg & 0x1f)) << 6)
                                         
#define     MAC_MII_WR_CMD(Addr,Reg)    (((Addr & 0x1f) << 11) | \
                                         ((Reg & 0x1f) << 6) | \
                                         MAC_PHY_MIIWRITE)



#define SH7264_SMSC_DEVICE      51
                                         
enum link
{
    NEGO_FAIL = 0, HALF_10M, FULL_10M, HALF_TX, FULL_TX,
};

enum init_mode
{
    HALF_10T_SET            = 0,
    FULL_10T_SET            = 1,
    HALF_100TX_SET          = 2,
    FULL_100TX_SET          = 3,
    HALF_100TX_AUTONEGO_SET = 4,
    POWERDOWN_SET           = 6,
    FULL_100TX_AUTONEGO_SET = 7,
};

static BOOL SH7264_SMSC_lwip_Init(struct netif *pNetIf);
static BOOL SH7264_SMSC_lwip_SetupDevice(struct netif *pNetIf);
static BOOL Smsc9218_lwip_Xmit_Packet(unsigned char * txbp, int len);
static void SMSC9218_lwip_Write_TX_FIFO(DWORD * pdwBuf, DWORD dwDwordCount);
static DWORD SMSC9218_lwip_Get_TX_Status_Count();
static void SMSC9218_lwip_RX_ISR();
static void SMSC9218_lwip_RX_Fast_Forward(DWORD dwDWCount);
static BOOL SMSC9218_lwip_Initialize_Phy();
static void SMSC9218_lwip_Initialize_TX();
static void SMSC9218_lwip_Initialize_RX(DWORD dwRXCFG, DWORD dwRXLvl);
static void SMSC9218_lwip_Enable_Sw_Flow_Control_FD();
BOOL SMSC9218_lwip_Initialize_Link();
static BOOL SMSC9218_lwip_Auto_Negotiate();
DWORD SMSC9218_lwip_Get_Link_Mode();
static void SMSC9218_lwip_Write_Phy_Reg(unsigned short usPhyRegister, unsigned short usPhyData);
static unsigned short SMSC9218_lwip_Read_Phy_Reg(unsigned short usPhyRegister);
static void SMSC9218_lwip_Write_Mac_Reg(unsigned short usMacRegister, unsigned long ulMacData);
static unsigned long SMSC9218_lwip_Read_Mac_Reg(unsigned short usMacRegister);
static BOOL SMSC9218_lwip_Wait_MAC_No_Busy();
static BOOL SMSC9218_lwip_Wait_MII_No_Busy();
static void SMSC9218_lwip_Enable_Interrupt(DWORD dwMask);
static void SMSC9218_lwip_Set_Mac_Filter_Reg();
static void SMSC9218_lwip_Set_Packet_Filter_Reg();
static void SMSC9218_lwip_Set_Multicast_Address_List();
static void SMSC9218_lwip_Enable_Mac_RX();
static void SMSC9218_lwip_Disable_Mac_RX();

#endif

