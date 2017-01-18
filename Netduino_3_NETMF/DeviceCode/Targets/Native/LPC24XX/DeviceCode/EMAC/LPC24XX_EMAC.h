//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#ifndef _LPC24XX_EMAC_H_
#define _LPC24XX_EMAC_H_ 1

#include <tinyhal.h>
#include "..\LPC24XX.h"
#include "LPC24xx_emac_driver.h"

//////////////////////////////////////////////////////////////////////////////
// Ethernet MAC
//
struct LPC24XX_EMAC
{
    static const UINT32 c_EMAC_Base     = 0xFFE00000;

    // EMAC pins
    static const UINT32 c_ENET_TXD0   = LPC24XX_GPIO::c_P1_00;
    static const UINT32 c_ENET_TXD1   = LPC24XX_GPIO::c_P1_01;
    static const UINT32 c_ENET_TXD2   = LPC24XX_GPIO::c_P1_02;
    static const UINT32 c_ENET_TXD3   = LPC24XX_GPIO::c_P1_03;
    static const UINT32 c_ENET_TX_EN  = LPC24XX_GPIO::c_P1_04;
    static const UINT32 c_ENET_TX_ER  = LPC24XX_GPIO::c_P1_05;
    static const UINT32 c_ENET_TX_CLK = LPC24XX_GPIO::c_P1_06;
    static const UINT32 c_ENET_COL    = LPC24XX_GPIO::c_P1_07;
    static const UINT32 c_ENET_CRS_DV = LPC24XX_GPIO::c_P1_08;
    static const UINT32 c_ENET_RXD0   = LPC24XX_GPIO::c_P1_09;
    static const UINT32 c_ENET_RXD1   = LPC24XX_GPIO::c_P1_10;
    static const UINT32 c_ENET_RXD2   = LPC24XX_GPIO::c_P1_11;
    static const UINT32 c_ENET_RXD3   = LPC24XX_GPIO::c_P1_12;
    static const UINT32 c_ENET_RX_DV  = LPC24XX_GPIO::c_P1_13;
    static const UINT32 c_ENET_RX_ER  = LPC24XX_GPIO::c_P1_14;
    static const UINT32 c_ENET_REF_CLK= LPC24XX_GPIO::c_P1_15;          
    static const UINT32 c_ENET_MDC    = LPC24XX_GPIO::c_P1_16;
    static const UINT32 c_ENET_MDIO   = LPC24XX_GPIO::c_P1_17;
           
    /****/ volatile UINT32 MAC_MAC1;             /* MAC config reg 1 */              
    /****/ volatile UINT32 MAC_MAC2;             /* MAC config reg 2 */
    /****/ volatile UINT32 MAC_IPGT;             /* b2b InterPacketGap reg */
    /****/ volatile UINT32 MAC_IPGR;             /* non b2b InterPacketGap reg */
    /****/ volatile UINT32 MAC_CLRT;             /* CoLlision window/ReTry reg */
    /****/ volatile UINT32 MAC_MAXF;             /* MAXimum Frame reg */
    /****/ volatile UINT32 MAC_SUPP;             /* PHY SUPPort reg */
    /****/ volatile UINT32 MAC_TEST;             /* TEST reg */
    /****/ volatile UINT32 MAC_MCFG;             /* MII Mgmt ConFiG reg */
    /****/ volatile UINT32 MAC_MCMD;             /* MII Mgmt CoMmanD reg */
    /****/ volatile UINT32 MAC_MADR;             /* MII Mgmt ADdRess reg */
    /****/ volatile UINT32 MAC_MWTD;             /* MII Mgmt WriTe Data reg (WO) */
    /****/ volatile UINT32 MAC_MRDD;             /* MII Mgmt ReaD Data reg (RO) */
    /****/ volatile UINT32 MAC_MIND;             /* MII Mgmt INDicators reg (RO) */

    /****/ volatile UINT32 dummy0[2];     // Filler to align next register address
                                                 
    /****/ volatile UINT32 MAC_SA0;              /* Station Address 0 reg */
    /****/ volatile UINT32 MAC_SA1;              /* Station Address 1 reg */
    /****/ volatile UINT32 MAC_SA2;              /* Station Address 2 reg */
                                                 
    /****/ volatile UINT32 dummy1[45];     // Filler to align next register address
                                                 
    /****/ volatile UINT32 MAC_COMMAND;          /* Command reg */
    /****/ volatile UINT32 MAC_STATUS;           /* Status reg (RO) */
    /****/ volatile UINT32 MAC_RXDESCRIPTOR;     /* Rx descriptor base address reg */
    /****/ volatile UINT32 MAC_RXSTATUS;         /* Rx status base address reg */
    /****/ volatile UINT32 MAC_RXDESCRIPTORNUM;  /* Rx number of descriptors reg */
    /****/ volatile UINT32 MAC_RXPRODUCEINDEX;   /* Rx produce index reg (RO) */
    /****/ volatile UINT32 MAC_RXCONSUMEINDEX;   /* Rx consume index reg */
    /****/ volatile UINT32 MAC_TXDESCRIPTOR;     /* Tx descriptor base address reg */
    /****/ volatile UINT32 MAC_TXSTATUS;         /* Tx status base address reg */
    /****/ volatile UINT32 MAC_TXDESCRIPTORNUM;  /* Tx number of descriptors reg */
    /****/ volatile UINT32 MAC_TXPRODUCEINDEX;   /* Tx produce index reg */
    /****/ volatile UINT32 MAC_TXCONSUMEINDEX;   /* Tx consume index reg (RO) */

    /****/ volatile UINT32 dummy2[10];     // Filler to align next register address

    /****/ volatile UINT32 MAC_TSV0;             /* Tx status vector 0 reg (RO) */
    /****/ volatile UINT32 MAC_TSV1;             /* Tx status vector 1 reg (RO) */
    /****/ volatile UINT32 MAC_RSV;              /* Rx status vector reg (RO) */

    /****/ volatile UINT32 dummy3[3];     // Filler to align next register address

    /****/ volatile UINT32 MAC_FLOWCONTROLCNT;   /* Flow control counter reg */
    /****/ volatile UINT32 MAC_FLOWCONTROLSTS;   /* Flow control status reg */

    /****/ volatile UINT32 dummy4[34];     // Filler to align next register address

    /****/ volatile UINT32 MAC_RXFILTERCTRL;     /* Rx filter ctrl reg */
    /****/ volatile UINT32 MAC_RXFILTERWOLSTS;   /* Rx filter WoL status reg (RO) */
    /****/ volatile UINT32 MAC_RXFILTERWOLCLR;   /* Rx filter WoL clear reg (WO) */

    /****/ volatile UINT32 dummy5[1];     // Filler to align next register address
                                                 
    /****/ volatile UINT32 MAC_HASHFILTERL;      /* Hash filter LSBs reg */
    /****/ volatile UINT32 MAC_HASHFILTERH;      /* Hash filter MSBs reg */

    /****/ volatile UINT32 dummy6[882];     // Filler to align next register address

    /****/ volatile UINT32 MAC_INTSTATUS;        /* Interrupt status reg (RO) */
    /****/ volatile UINT32 MAC_INTENABLE;        /* Interrupt enable reg  */
    /****/ volatile UINT32 MAC_INTCLEAR;         /* Interrupt clear reg (WO) */
    /****/ volatile UINT32 MAC_INTSET;           /* Interrupt set reg (WO) */

    /****/ volatile UINT32 dummy7[1];     // Filler to align next register address

    /****/ volatile UINT32 MAC_POWERDOWN;        /* Power-down reg */
    /****/ volatile UINT32 MAC_MODULEID;         /* Module ID reg (RO) */            
};                                               
//
// Ethernet MAC
//////////////////////////////////////////////////////////////////////////////

/* EMAC Memory Buffer configuration for 16K Ethernet RAM. */
#define NUM_RX_FRAG         7           /* Num.of RX Fragments 4*1536= 10.5kB */
#define NUM_TX_FRAG         3           /* Num.of TX Fragments 3*1536=  4.5kB */
#define ETH_FRAG_SIZE       1536        /* Packet Fragment size 1536 Bytes   */

#define ETH_MAX_FLEN        1536        /* Max. Ethernet Frame Size          */

/* EMAC variables located in 16K Ethernet SRAM */
#define RX_DESC_BASE        0x7FE00000
#define RX_STAT_BASE        (RX_DESC_BASE + NUM_RX_FRAG*8)
#define TX_DESC_BASE        (RX_STAT_BASE + NUM_RX_FRAG*8)
#define TX_STAT_BASE        (TX_DESC_BASE + NUM_TX_FRAG*8)
#define RX_BUF_BASE         (TX_STAT_BASE + NUM_TX_FRAG*4)
#define TX_BUF_BASE         (RX_BUF_BASE  + NUM_RX_FRAG*ETH_FRAG_SIZE)

/* RX Descriptor Control Word */
#define RCTRL_SIZE          0x000007FF  /* Buffer size mask                  */
#define RCTRL_INT           0x80000000  /* Generate RxDone Interrupt         */

/* TX Descriptor Control Word */
#define TCTRL_SIZE          0x000007FF  /* Size of data buffer in bytes      */
#define TCTRL_LAST          0x40000000  /* Last Descriptor for TX Frame      */
#define TCTRL_INT           0x80000000  /* Generate TxDone Interrupt         */

/* RX and TX descriptor and status definitions. */
#define RX_DESC_PACKET(i)   (*(unsigned int *)(RX_DESC_BASE   + 8*i))
#define RX_DESC_CTRL(i)     (*(unsigned int *)(RX_DESC_BASE+4 + 8*i))
#define RX_STAT_INFO(i)     (*(unsigned int *)(RX_STAT_BASE   + 8*i))
#define RX_STAT_HASHCRC(i)  (*(unsigned int *)(RX_STAT_BASE+4 + 8*i))
#define TX_DESC_PACKET(i)   (*(unsigned int *)(TX_DESC_BASE   + 8*i))
#define TX_DESC_CTRL(i)     (*(unsigned int *)(TX_DESC_BASE+4 + 8*i))
#define TX_STAT_INFO(i)     (*(unsigned int *)(TX_STAT_BASE   + 4*i))
#define RX_BUF(i)           (RX_BUF_BASE + ETH_FRAG_SIZE*i)
#define TX_BUF(i)           (TX_BUF_BASE + ETH_FRAG_SIZE*i)

#define RX_DESC_STATUS_SIZE     0x000007FF  /* Data size in bytes */
#define RX_DESC_STATUS_FAILFLT  0x00100000  /* RX Filter Failed   */
#define RX_DESC_STATUS_CRCERR   0x00800000  /* CRC Error in Frame */
#define RX_DESC_STATUS_LENERR   0x02000000  /* Length Error  */
#define RX_DESC_STATUS_OVERRUN  0x10000000  /* Receive overrun */
#define RX_DESC_STATUS_NODESC   0x20000000  /* No new Descriptor available */

#define RX_FRAME_ERROR (RX_DESC_STATUS_FAILFLT | RX_DESC_STATUS_CRCERR | RX_DESC_STATUS_LENERR | RX_DESC_STATUS_OVERRUN | RX_DESC_STATUS_NODESC)

/* Interrupt Status/Enable/Clear/Set Registers */
#define INT_RX_OVERRUN      0x00000001  /* Overrun Error in RX Queue         */
#define INT_RX_DONE         0x00000008  /* Receive Done                      */
#define INT_TX_UNDERRUN     0x00000010  /* Transmit Underrun                 */
#define INT_TX_DONE         0x00000080  /* Transmit Done                     */

/* Command Register */
#define CR_RX_EN            0x00000001  /* Enable Receive                    */
#define CR_TX_EN            0x00000002  /* Enable Transmit                   */
#define CR_REG_RES          0x00000008  /* Reset Host Registers              */
#define CR_TX_RES           0x00000010  /* Reset Transmit Datapath           */
#define CR_RX_RES           0x00000020  /* Reset Receive Datapath            */
#define CR_PASS_RUNT_FRM    0x00000040  /* Pass Runt Frames                  */
#define CR_RMII             0x00000200  /* Reduced MII Interface             */
#define CR_FULL_DUP         0x00000400  /* Full Duplex                       */

/* MAC Configuration Register 1 */
#define MAC1_REC_EN         0x00000001  /* Receive Enable                    */
#define MAC1_PASS_ALL       0x00000002  /* Pass All Receive Frames           */
#define MAC1_RES_TX         0x00000100  /* Reset TX Logic                    */
#define MAC1_RES_MCS_TX     0x00000200  /* Reset MAC TX Control Sublayer     */
#define MAC1_RES_RX         0x00000400  /* Reset RX Logic                    */
#define MAC1_RES_MCS_RX     0x00000800  /* Reset MAC RX Control Sublayer     */
#define MAC1_SIM_RES        0x00004000  /* Simulation Reset                  */
#define MAC1_SOFT_RES       0x00008000  /* Soft Reset MAC                    */

/* MAC Configuration Register 2 */
#define MAC2_FULL_DUP       0x00000001  /* Full Duplex Mode                  */
#define MAC2_FRM_LEN_CHK    0x00000002  /* Frame Length Checking             */
#define MAC2_CRC_EN         0x00000010  /* Append CRC to every Frame         */
#define MAC2_PAD_EN         0x00000020  /* Pad all Short Frames              */

/* Receive Filter Control Register */
#define RFC_BCAST_EN        0x00000002  /* Accept Broadcast Frames Enable    */
#define RFC_MCAST_EN        0x00000004  /* Accept Multicast Frames Enable    */
#define RFC_PERFECT_EN      0x00000020  /* Accept Perfect Match Enable       */

/* PHY Support Register */
#define SUPP_SPEED          0x00000100  /* Reduced MII Logic Current Speed   */

/* MII Management Configuration Register */
#define MCFG_SCAN_INC       0x00000001  /* Scan Increment PHY Address        */
#define MCFG_SUPP_PREAM     0x00000002  /* Suppress Preamble                 */
#define MCFG_CLK_SEL        0x0000001C  /* Clock Select Mask                 */
#define MCFG_RES_MII        0x00008000  /* Reset MII Management Hardware     */
#define MDC_CLK_DIV_4       0x00000000
#define MDC_CLK_DIV_6       0x00000008
#define MDC_CLK_DIV_8       0x0000000C
#define MDC_CLK_DIV_10      0x00000010
#define MDC_CLK_DIV_14      0x00000014
#define MDC_CLK_DIV_20      0x00000018
#define MDC_CLK_DIV_28      0x0000001C

/* Non Back-to-Back Inter-Packet-Gap Register */
#define IPGR_DEF            0x00000012  /* Recommended value                 */

/* Collision Window/Retry Register */
#define CLRT_DEF            0x0000370F  /* Default value                     */

/* Back-to-Back Inter-Packet-Gap Register */
#define IPGT_FULL_DUP       0x00000015  /* Recommended value for Full Duplex */
#define IPGT_HALF_DUP       0x00000012  /* Recommended value for Half Duplex */

/* MII Management Configuration Register */
#define MCFG_CLK_SEL        0x0000001C  /* Clock Select Mask                 */
#define MCFG_RES_MII        0x00008000  /* Reset MII Management Hardware     */

/* MII Management Command Register */
#define MCMD_READ           0x00000001  /* MII Read                          */

/* MII Management Address Register */
#define MADR_REG_ADR        0x0000001F  /* MII Register Address Mask         */
#define MADR_PHY_ADR        0x00001F00  /* PHY Address Mask                  */

/* MII Management Indicators Register */
#define MIND_BUSY           0x00000001  /* MII is Busy                       */

/* Function Prototypes */
BOOL LPC24XX_EMAC_ReadPhy(UINT32 phy_addr, UINT32 phy_reg_addr, UINT32 *value, UINT32 retry);

BOOL LPC24XX_EMAC_WritePhy(UINT32 phy_addr, UINT32 phy_reg_addr, UINT32 value, UINT32 retry);

void LPC24XX_EMAC_SetLinkSpeed(BOOL speed, BOOL fullduplex);

#endif
