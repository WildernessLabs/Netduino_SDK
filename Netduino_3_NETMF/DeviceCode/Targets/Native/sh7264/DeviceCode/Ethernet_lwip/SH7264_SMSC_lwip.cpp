////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "net_decl_lwip.h"
#include "SH7264_SMSC_lwip.h"

extern void lwip_interrupt_continuation( void );

//--//
#define DEBUG_SH7264RSK 0

#ifndef NETWORK_INTERFACE_INDEX_SH7264RSK
#define NETWORK_INTERFACE_INDEX_SH7264RSK 0
#endif

#define BIT_EDIANDW(data) ((data >> 24) & 0x000000FF) | ((data << 24) & 0xFF000000) | ((data >> 8) & 0x0000FF00) | ((data << 8) & 0x00FF0000);

/**************************************************************************
 * LOCAL DEFINITIONS AND MACROS
 **************************************************************************/
#define     RX_BUFFER 0x0D000000

extern struct BlockStorageDevice  g_SH7264_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_BS_Config_SH7264_RSK;

BlockStorageDevice  * pBlockStorageDevice = &g_SH7264_BS;
BLOCK_CONFIG        * pBLOCK_CONFIG         = &g_BS_Config_SH7264_RSK;

/*************************************************************************
 * GLOBAL DATA
 *************************************************************************/
static DWORD dwLinkMode;

static DWORD dwIntEnSave;

void SMSC9218_lwip_Enable_Sw_Flow_Control_FD()
{
    DWORD dwCnt = 1000;

    // Enable flow control.
    AFC_CFG = 0x006E374F;

    //Wait for busy bit to clear.
    while ((SMSC9218_lwip_Read_Mac_Reg(FLOW) & MAC_FCBSY) && (dwCnt > 0))
    {
        ETHERNET_DELAY(1);
        dwCnt--;
    }

    // Set FLOW register in MAC to send out a pause frame.
    SMSC9218_lwip_Write_Mac_Reg(FLOW, (MAC_FCEN | (0xFFFF<<16) | MAC_FCBSY));
}

BOOL SMSC9218_lwip_Initialize_Phy()
{
    DWORD dwCnt;
    unsigned short usPhy;
    
    //
    // Reset the PHY.
    //
    SMSC9218_lwip_Write_Phy_Reg((unsigned short)PHY_CONTROL, (unsigned short)PHY_CTRL_RESET);
    dwCnt = 100000;
    do
    {
        ETHERNET_DELAY(10);
        usPhy = SMSC9218_lwip_Read_Phy_Reg(PHY_CONTROL);
        dwCnt--;
    } while ((dwCnt > 0) && (usPhy & PHY_CTRL_RESET));
    
    if (usPhy & PHY_CTRL_RESET)
    {
        debug_printf("PHY reset failed.\r\n");
        return FALSE;
    }
    
    return TRUE;
}

void SMSC9218_lwip_Initialize_TX()
{
    DWORD dwReg;

    // setup MAC for TX
    dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
    dwReg |= MAC_TXEN;  // Transmitter enable
    SMSC9218_lwip_Write_Mac_Reg(MAC_CR, dwReg);

    // setup Store and Forward
    dwReg = HW_CFG;
    dwReg &= (HWCFG_TX_FIF_SZ_MASK | 0xFFF);
    dwReg |= HWCFG_SF;
    HW_CFG = dwReg;
    dwReg = HW_CFG; //dummy read

    TX_CFG = TXCFG_TX_ON;   // Transmitter Enable

    FIFO_INT = 0xFF000000;
    SMSC9218_lwip_Enable_Interrupt(INTEN_TDFU_INT_EN | INTEN_TDFO_INT_EN | INTEN_TDFA_INT_EN);
}

void SMSC9218_lwip_Initialize_RX(DWORD dwRXCFG, DWORD dwRXLvl)
{
    DWORD dwReg;

    //set receive configuration
    RX_CFG = dwRXCFG;

    //set the interrupt levels to zero
    dwReg = FIFO_INT;
    dwReg &= 0xFFFF0000;
    dwReg |= (dwRXLvl & 0x0000FFFF);
    FIFO_INT = dwReg;

    //enable interrupt
    SMSC9218_lwip_Enable_Interrupt(INTEN_RSFL_INT_EN);
}

BOOL SMSC9218_lwip_Initialize_Link()
{
    volatile WORD wReg;
    BOOL bRet=TRUE;
    DWORD dwReg;
    
    // Enable Auto-Negotiation
    
    SMSC9218_lwip_Write_Phy_Reg(PHY_CONTROL, PHY_CTRL_ANENABLE);
    
    // Set Auto-Negotiation-Advertise
    wReg = SMSC9218_lwip_Read_Phy_Reg(PHY_ANADVERTISE);
    wReg &= ~(PHY_ANA_PAUSE_OP_MASK | PHY_ANA_SPEED_MASK);
    wReg |= PHY_ANA_ASYM_PAUSE | PHY_ANA_SYM_PAUSE | PHY_ANA_10HDX | PHY_ANA_10FDX | PHY_ANA_100HDX | PHY_ANA_100FDX;
    SMSC9218_lwip_Write_Phy_Reg(PHY_ANADVERTISE, wReg);

    if(!SMSC9218_lwip_Auto_Negotiate())
    {
        bRet = FALSE;
    } 

    //Clear interrupts
    wReg = SMSC9218_lwip_Read_Phy_Reg(PHY_INTERRUPTSRC);

    // Get link mode
    dwLinkMode = SMSC9218_lwip_Get_Link_Mode();

    dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
    dwReg &= ~( MAC_RCVOWN | MAC_FDPX );
    switch(dwLinkMode){

    case LINK_NO_LINK:
        //TODO: consider auto linking to a specified link state.
        debug_printf("link is down.\r\n");
        break;

    case LINK_10MPS_HALF:
      debug_printf("10Base-T Half Duplex capable.\r\n");
        dwReg |= MAC_RCVOWN;
        break;

    case LINK_10MPS_FULL:
      debug_printf("10Base-T Full Duplex capable.\r\n");
        dwReg |= MAC_FDPX;
        break;

    case LINK_100MPS_HALF:
      debug_printf("100Base-TX Half Duplex capable.\r\n");
        dwReg |= MAC_RCVOWN;
        break;

    case LINK_100MPS_FULL:
      debug_printf("100Base-T Full Duplex capable.\r\n");
        dwReg |= MAC_FDPX;
        break;

    default:
        break;
    }
    SMSC9218_lwip_Write_Mac_Reg(MAC_CR, dwReg);  

    return bRet;
}

BOOL SMSC9218_lwip_Auto_Negotiate()
{
    DWORD dwCnt;
    unsigned short usPhy;

    debug_printf("Try Auto Negotiation.......\r\n");

    // Start AutoNegotiate
    SMSC9218_lwip_Write_Phy_Reg(PHY_CONTROL, PHY_CTRL_ANENABLE | PHY_CTRL_ANRESTART);
    
    //
    // Wait for the autonegotiation to complete
    //
    dwCnt = 100000;
    do
    {
        usPhy = SMSC9218_lwip_Read_Phy_Reg(PHY_STATUS);
        if (usPhy & (PHY_STAT_ANCOMPLETE | PHY_STAT_REMOTEFAULT))
        {
            break;
        }
        dwCnt--;
    }while(dwCnt > 0);
    
    if (usPhy & PHY_STAT_ANCOMPLETE)
    {
        debug_printf("Auto Negotiation done.\r\n");
    }
    else
    {       
        debug_printf("Auto Negotiation Failed.\r\n");
        return FALSE;
    }
    return TRUE;
}

DWORD SMSC9218_lwip_Get_Link_Mode()
{
    WORD wReg, wRegANEG, wRegCtl, wRegADV, wRegAPA;
    DWORD dwReg, LinkMode = LINK_NO_LINK;

    // Get link mode
    wReg = SMSC9218_lwip_Read_Phy_Reg(PHY_STATUS);
    wRegCtl = SMSC9218_lwip_Read_Phy_Reg(PHY_CONTROL);
    if (wRegCtl & PHY_CTRL_ANENABLE)
    {
        if(wReg & PHY_STAT_LINK_UP)
        {
            wRegADV = SMSC9218_lwip_Read_Phy_Reg(PHY_ANADVERTISE);
            wRegAPA = SMSC9218_lwip_Read_Phy_Reg(PHY_ANPARTNER);
            wRegANEG = wRegADV & wRegAPA;

            if(wRegANEG & PHY_ANLPA_100FDX)
            {
                LinkMode = LINK_100MPS_FULL;
            }
            else if(wRegANEG & PHY_ANLPA_100HDX)
            {
                LinkMode = LINK_100MPS_HALF;
            }
            else if(wRegANEG & PHY_ANLPA_10FDX)
            {
                LinkMode = LINK_10MPS_FULL;
            }
            else if(wRegANEG & PHY_ANLPA_10HDX)
            {
                LinkMode = LINK_10MPS_HALF;
            }

            // check Flow Control
            if(wRegANEG & (PHY_ANLPA_100FDX | PHY_ANLPA_10FDX)) //Full-Duplex
            {
                if(wRegANEG & 0x0400)
                {
                    // both support Symmetric Flow Control
                    dwReg = SMSC9218_lwip_Read_Mac_Reg(FLOW);
                    dwReg |= MAC_FCEN;
                    SMSC9218_lwip_Write_Mac_Reg(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg |= AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
                else if(((wRegADV & 0x0C00) == 0x0C00) && ((wRegAPA & 0x0C00) == 0x0800))
                {
                    // Partner is Asymmetric Flow Control
                    dwReg = SMSC9218_lwip_Read_Mac_Reg(FLOW);
                    dwReg |= MAC_FCEN;
                    SMSC9218_lwip_Write_Mac_Reg(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg &= ~AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
                else
                {
                    // Disable Flow Control
                    dwReg = SMSC9218_lwip_Read_Mac_Reg(FLOW);
                    dwReg &= ~MAC_FCEN;
                    SMSC9218_lwip_Write_Mac_Reg(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg &= ~AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
            }
            else    //Half-Duplex
            {
                // Disable Flow Control
                dwReg = SMSC9218_lwip_Read_Mac_Reg(FLOW);
                dwReg &= ~MAC_FCEN;
                SMSC9218_lwip_Write_Mac_Reg(FLOW, dwReg);

                dwReg = AFC_CFG;
                dwReg &= ~AFCCFG_FCANY;
                AFC_CFG = dwReg;
            }
        }
    }
    else
    {
        if(wReg & PHY_STAT_LINK_UP)
        {
            if (wRegCtl & PHY_CTRL_SPEED)
            {
                if(wRegCtl & PHY_CTRL_DUPLEX)
                {
                    LinkMode = LINK_100MPS_FULL;
                } 
                else
                {
                    LinkMode = LINK_100MPS_HALF;
                }
            }
            else
            {
                if(wRegCtl & PHY_CTRL_DUPLEX)
                {
                    LinkMode = LINK_10MPS_FULL;
                } 
                else
                {
                    LinkMode = LINK_10MPS_HALF;
                }
            }
        }
    }

    return LinkMode;
}

void SMSC9218_lwip_Write_Phy_Reg(unsigned short PhyReg, unsigned short PhyData)
{
    // check busy
    if(SMSC9218_lwip_Read_Mac_Reg(MII_ACC) & MAC_PHY_MIIBZY){
        debug_printf("SMSC9218_lwip_Write_Phy_Reg: MII is BUSY.\r\n");
        return;
    }

    // set PHY data
    SMSC9218_lwip_Write_Mac_Reg(MII_DATA, PhyData);

    // set PHY write command
    SMSC9218_lwip_Write_Mac_Reg(MII_ACC, MAC_PHY_ADDR_INTERNAL | MAC_PHY_MIIRINDA(PhyReg) | MAC_PHY_MIIWRITE);

    // wait for MII no BUSY
    if(!SMSC9218_lwip_Wait_MII_No_Busy())
    {
        debug_printf("SMSC9218_lwip_Write_Phy_Reg: PHY Write Error.\r\n");
    }
}

WORD SMSC9218_lwip_Read_Phy_Reg(WORD PhyReg)
{
    WORD wReg=0xffff;

    // check busy
    if(SMSC9218_lwip_Read_Mac_Reg(MII_ACC) & MAC_PHY_MIIBZY)
    {
        debug_printf("SMSC9218_lwip_Read_Phy_Reg: MII is BUSY.\r\n");
        return 0;
    }

    // set PHY read command
    SMSC9218_lwip_Write_Mac_Reg(MII_ACC, MAC_PHY_ADDR_INTERNAL | MAC_PHY_MIIRINDA(PhyReg) | MAC_PHY_MIIREAD);

    // wait for MII no BUSY
    if(!SMSC9218_lwip_Wait_MII_No_Busy())
    {
        debug_printf("SMSC9218_lwip_Read_Phy_Reg: PHY Read Error.\r\n");
    }
    else
    {
        // read PHY register
        wReg = (WORD)SMSC9218_lwip_Read_Mac_Reg(MII_DATA);
    }
    
    return wReg;
}

void SMSC9218_lwip_Write_Mac_Reg(WORD usMacRegister, unsigned long ulMacData)
{
    // check busy
    if(MAC_CSR_CMD & MAC_CSR_BUSY)
    {
        debug_printf("SMSC9218_lwip_Write_Mac_Reg: MAC is BUSY.\r\n");
        return;
    }

    // set MAC Data
    MAC_CSR_DATA = ulMacData;

    // set MAC CMD
    MAC_CSR_CMD = ((usMacRegister & MAC_CSR_ADDR_MASK) | MAC_CSR_BUSY | MAC_CSR_REQ_WRITE);

    // wait for MAC no BUSY
    if(!SMSC9218_lwip_Wait_MAC_No_Busy())
    {
        debug_printf("SMSC9218_lwip_Write_Mac_Reg: MAC Write Error.\r\n");
    }
}

unsigned long SMSC9218_lwip_Read_Mac_Reg(WORD usMacRegister)
{
    DWORD dwReg=0xFFFFFFFF;

    // check busy
    if(MAC_CSR_CMD & MAC_CSR_BUSY)
    {
        debug_printf("SMSC9218_lwip_Read_Mac_Reg: MAC is BUSY.\r\n");
        return dwReg;
    }

    // set MAC read command
    MAC_CSR_CMD = (usMacRegister & MAC_CSR_ADDR_MASK) | MAC_CSR_BUSY | MAC_CSR_REQ_READ;

    // wait for MAC no BUSY
    if(!SMSC9218_lwip_Wait_MAC_No_Busy())
    {
        debug_printf("SMSC9218_lwip_Read_Mac_Reg: MAC Read Error.\r\n");
    }
    else
    {
        // read MAC register
        dwReg = MAC_CSR_DATA;
    }

    return dwReg;
}

BOOL SMSC9218_lwip_Wait_MAC_No_Busy()
{
    DWORD dwCnt;

    for(dwCnt = 0; dwCnt < 40; dwCnt++)
    {
        if(!(MAC_CSR_CMD & MAC_CSR_BUSY))
        {
            return TRUE;
        }
    }

    debug_printf("SMSC9218_lwip_Wait_MAC_No_Busy: MAC BUSY.\r\n");

    return FALSE;
}

BOOL SMSC9218_lwip_Wait_MII_No_Busy()
{
    DWORD dwCnt;

    for(dwCnt = 0; dwCnt < 100; dwCnt++)
    {
        if(!(SMSC9218_lwip_Read_Mac_Reg(MII_ACC) & MAC_PHY_MIIBZY))
        {
            return TRUE;
        }
    }

    debug_printf("SMSC9218_lwip_Wait_MII_No_Busy: PHY MII BUSY.\r\n");

    return FALSE;
}

void SMSC9218_lwip_Enable_Interrupt(DWORD dwMask)
{
    DWORD dwReg;

    dwReg = INT_EN;
    dwReg |= dwMask | INTEN_PME_INT_EN;
    INT_EN = dwReg;
}

void SMSC9218_lwip_Set_Mac_Filter_Reg()
{
    DWORD   dwReg;
    
    dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);  
    dwReg &= (~(MAC_MCPAS | MAC_PRMS | MAC_INVFILT | MAC_HO | MAC_HPFILT | MAC_BCAST));
    dwReg |= MAC_MCPAS | MAC_PRMS;  
    SMSC9218_lwip_Write_Mac_Reg(MAC_CR, dwReg);
}

BOOL SH7264_SMSC_lwip_open(struct netif *pNetIf)   
{
    DWORD dwAddrH, dwAddrL, dwCnt, dwReg, dw; 

    debug_printf("SH7264_SMSC_lwip_open\r\n");

    //Setting port
    PJCR0 |= 0x3300;
    
    if (WORD_SWAP != 0xFFFFFFFF)    //Need at least one read from LAN chip before writing to WORD_SWAP register
    {
        WORD_SWAP = 0xFFFFFFFF;
    }
    
    ETHERNET_DELAY(10000);

    // soft reset
    HW_CFG = HWCFG_SRST;
    
    dwCnt = 100000;
    do
    {
        ETHERNET_DELAY(10);
        dwReg = HW_CFG;
        dwCnt--;
    }while ((dwCnt > 0) && (dwReg & HWCFG_SRST));
    
    if (dwReg & HWCFG_SRST)
    {
        debug_printf("SH7264_SMSC_open: Error in Soft reset.\r\n");
        return;
    }

    ETHERNET_DELAY(10000);

    DWORD dwTest = E2P_CMD;

    //Read Mac address from EEPROM          
    dwAddrL = SMSC9218_lwip_Read_Mac_Reg(ADDRL);
    dwAddrH = SMSC9218_lwip_Read_Mac_Reg(ADDRH);

    // Copy to NetIF
    pNetIf->hwaddr[0] = (dwAddrL >>  0) & 0xFF;
    pNetIf->hwaddr[1] = (dwAddrL >>  8) & 0xFF;
    pNetIf->hwaddr[2] = (dwAddrL >> 16) & 0xFF;
    pNetIf->hwaddr[3] = (dwAddrL >> 24) & 0xFF;
    pNetIf->hwaddr[4] = (dwAddrH >>  0) & 0xFF;
    pNetIf->hwaddr[5] = (dwAddrH >>  8) & 0xFF;
    
    debug_printf("EEPROM MAC: %08X %08X\r\n",dwAddrL,dwAddrH);
        
// What is this actually doing ?
#if 0
    //int macLen = __min(g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264RSK].macAddressLen, sizeof(sc->mac_address));
    
    dw = (DWORD)&g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264RSK].macAddressBuffer[0];
    if (((dw >= 0x00000000) && (dw <= 0x03000000)) || ((dw >= 0x20000000) && (dw <= 0x23000000)))
    {
        debug_printf("Writing MAC to block storage...\r\n");
        g_AM29DL_16_BS_DeviceTable.InitializeDevice(pBLOCK_CONFIG);
        g_AM29DL_16_BS_DeviceTable.Write(pBLOCK_CONFIG,(DWORD)&g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264RSK].macAddressBuffer[0],6,(BYTE *)mac,TRUE);
    }
    else
    {
        memcpy((void *)&g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264RSK].macAddressBuffer[0], (void *)&mac[0], 6);
    }

    if(macLen > 0)
    {
        memcpy(&sc->mac_address[0], &g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264RSK].macAddressBuffer[0], macLen);
    }
    else
    {
        RTP_DEBUG_ERROR("Device initialize without MAC address!!!\r\n", NOVAR, 0, 0);
    }
#endif

    if (!SH7264_SMSC_lwip_SetupDevice(pNetIf))
    {
        return FALSE;
    }
    
    return TRUE;
}

void SH7264_SMSC_lwip_close(struct netif *pNetIf)                          
{
}

// Hardwired call from INTC, param always NULL...
void SH7264_SMSC_interrupt( void *param )
{
    SH7264_SMSC_lwip_interrupt(param);
}

void SH7264_SMSC_lwip_interrupt(void *param)
{   
    long int_sts;
    
    GLOBAL_LOCK(encIrq);
    
    int_sts = INT_STS;
    
    // Frame(s) received
    if(int_sts & INTSTS_RSFL_INT)
    {
        // disable interrupts until continuation has run
        // not doing this results in being swamped with interrupts
        INT_CFG = INT_CFG & ~INTCFG_IRQ_EN;

        lwip_interrupt_continuation();
        INT_STS = INTSTS_RSFL_INT;
    }
    
    // Clear interrupt status
    INT_STS = 0xFFFFFFFF;
}

void SH7264_SMSC_lwip_recv(struct netif *pNetIf)
{
    DWORD RxStatus, packet_len, dw;
    DWORD numPacketReceived;
    DWORD RxFFCountDW;
    struct pbuf *pPBuf;
    unsigned long temp;

    GLOBAL_LOCK(encIrq);

    dw = RX_FIFO_INF;
    numPacketReceived = (dw & 0x00FF0000) >> 16;

    //debug_printf("recv %d packets\r\n",numPacketReceived);

    while(numPacketReceived)
    {
        RxStatus = RX_STS_FIFO_PORT;
        packet_len = (RxStatus & 0x3FFF0000) >> 16;

        if (!(RxStatus & RX_STS_ERROR) && 
            !(((RxStatus & RX_STS_LEN_ERROR) == RX_STS_LEN_ERROR) && 
            ((RxStatus & RX_STS_FRAME_TYPE) == 0)))
        {   
            pPBuf = pbuf_alloc(PBUF_RAW, packet_len - 2, PBUF_RAM);
                    
            if(pPBuf)
            {
                unsigned long *rxbpl    = (unsigned long *) RX_BUFFER;
                unsigned char *rx_ptr   = (unsigned char *) RX_BUFFER;
                int ndx;
                    
                for (ndx = ((packet_len + 2 + 3) >> 2); ndx > 0; ndx--)
                {
                    temp = RX_DATA_FIFO_PORT;
                    *rxbpl++ = BIT_EDIANDW(temp);
                }

                memcpy(pPBuf->payload, rx_ptr + 2, packet_len - 2);
                pNetIf->input( pPBuf, pNetIf );
            }
            else
            {
                debug_printf("SH7264_SMSC_lwip_recv: pbuf_alloc failed, discarding data");
                // Read off and discard the packet.
                RxFFCountDW = (packet_len + 2 + 3) >> 2;
                SMSC9218_lwip_RX_Fast_Forward(RxFFCountDW);
            }
        }
        else
        {
            // Packet has error. Discard it and update stats.
            RxFFCountDW = (packet_len + 2 + 3) >> 2;
            SMSC9218_lwip_RX_Fast_Forward(RxFFCountDW);          
        }

        dw = RX_FIFO_INF;
        numPacketReceived = (dw & 0x00FF0000) >> 16;
    }

    // re-enable interupts
    INT_CFG |= INTCFG_IRQ_EN;

}

void SMSC9218_lwip_RX_Fast_Forward(DWORD dwDWCount)
{
    DWORD dwCnt;

    RX_DP_CTRL = RXDPCTL_RX_FFWD;

    // dummy read required by spec.
    dwCnt = BYTE_TEST;

    dwCnt = 10000;
    while ((RX_DP_CTRL & RXDPCTL_RX_FFWD) && (dwCnt > 0))
    {
        ETHERNET_DELAY(1);
        dwCnt--;
    }

    if(dwCnt == 0)
    {
    }
}

DWORD SMSC9218_lwip_Get_TX_Status_Count()
{
    DWORD dwReg;

    dwReg = TX_FIFO_INF;
    dwReg &= TXFIFO_TXSUSED_MASK;
    dwReg >>= 16;
    dwReg &= 0xFFFF;
    
    return dwReg;
}

void SMSC9218_lwip_Set_Packet_Filter_Reg()
{
    DWORD   dwReg;
    
    if (dwLinkMode == LINK_10MPS_HALF)
    {
        SMSC9218_lwip_Disable_Mac_RX();
        SMSC9218_lwip_Set_Mac_Filter_Reg();
        SMSC9218_lwip_Enable_Mac_RX();
    }
    else if (dwLinkMode == LINK_10MPS_FULL)
    {
        // 10/FD
        // Disable RX 
        dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            INT_STS = INTSTS_RXSTOP_INT;
            // Enable
            SMSC9218_lwip_Enable_Interrupt(INTEN_RXSTOP_INT_EN);
            dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
            dwReg &= ~MAC_RXEN;
            SMSC9218_lwip_Write_Mac_Reg(MAC_CR, dwReg);
            return;
        }
        else
        {
            SMSC9218_lwip_Set_Mac_Filter_Reg();
            SMSC9218_lwip_Enable_Mac_RX();
        }
    }
    else if ((dwLinkMode == LINK_100MPS_FULL) || (dwLinkMode == LINK_100MPS_HALF))
    {
        dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            // 100/FD/HD
            // Initiate SWINT
            SMSC9218_lwip_Enable_Interrupt(INTEN_SW_INT_EN);
        }
        else
        {
            SMSC9218_lwip_Write_Mac_Reg(HASHL, 0);
            SMSC9218_lwip_Write_Mac_Reg(HASHH, 0);
            SMSC9218_lwip_Set_Mac_Filter_Reg();
            // Enable RX 
            SMSC9218_lwip_Enable_Mac_RX();
        }
    }
}

void SMSC9218_lwip_Set_Multicast_Address_List()
{
    DWORD   dwReg;
    
    if (dwLinkMode == LINK_10MPS_HALF)
    {
        SMSC9218_lwip_Disable_Mac_RX();
        SMSC9218_lwip_Write_Mac_Reg(HASHL, 0);
        SMSC9218_lwip_Write_Mac_Reg(HASHH, 0);
        SMSC9218_lwip_Enable_Mac_RX();
    }
    else if (dwLinkMode == LINK_10MPS_FULL)
    {
        // 10/FD
        dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            INT_STS = INTSTS_RXSTOP_INT;
            // Enable Interrupt
            SMSC9218_lwip_Enable_Interrupt(INTEN_RXSTOP_INT_EN);

            dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
            dwReg &= ~MAC_RXEN;
            SMSC9218_lwip_Write_Mac_Reg(MAC_CR, dwReg);
        }
        else
        {
            SMSC9218_lwip_Write_Mac_Reg(HASHL, 0);
            SMSC9218_lwip_Write_Mac_Reg(HASHH, 0);

            // Enable RX_EN
            dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
            dwReg |= MAC_RXEN;
            SMSC9218_lwip_Write_Mac_Reg(MAC_CR, dwReg);
        }
    }
    else if ((dwLinkMode == LINK_100MPS_FULL) || (dwLinkMode == LINK_100MPS_HALF))
    {
        dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            // 100/FD/HD
            // Initiate SWINT Interrupt 
            SMSC9218_lwip_Enable_Interrupt(INTEN_SW_INT_EN);
        }
        else
        {
            SMSC9218_lwip_Write_Mac_Reg(HASHL, 0);
            SMSC9218_lwip_Write_Mac_Reg(HASHH, 0);
            SMSC9218_lwip_Set_Mac_Filter_Reg();
            // Enable RX 
            SMSC9218_lwip_Enable_Mac_RX();
        }
    }
}

void SMSC9218_lwip_Enable_Mac_RX()
{
    volatile DWORD  dwReg;
    
    // Enable RX 
    dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
    dwReg |= MAC_RXEN;
    SMSC9218_lwip_Write_Mac_Reg(MAC_CR, dwReg);

    // Wait for at least 64uSec
    ETHERNET_DELAY(64);
}

void SMSC9218_lwip_Disable_Mac_RX()
{
    volatile DWORD  dwReg;
    volatile long   counter;
    
    dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
    if (dwReg & MAC_RXEN)
    {
        INT_STS = INTSTS_RXSTOP_INT;

        dwReg = SMSC9218_lwip_Read_Mac_Reg(MAC_CR);
        dwReg &= ~MAC_RXEN; //disable RX
        SMSC9218_lwip_Write_Mac_Reg(MAC_CR, dwReg);

        counter = (long)FREE_RUN;
        while (!((dwReg = INT_STS) & INTSTS_RXSTOP_INT))
        {
            if (((long)FREE_RUN - counter) > (25 * 2000))
            {
                break;
            }
        }

        INT_STS = INTSTS_RXSTOP_INT;
    }
}

void SMSC9218_lwip_Write_TX_FIFO(DWORD * pdwBuf, DWORD dwDwordCount)
{
    DWORD temp;
    
    while (dwDwordCount)
    {
        temp = *pdwBuf++;
        TX_DATA_FIFO_PORT = BIT_EDIANDW(temp);
        dwDwordCount--;
    }
}

err_t SH7264_SMSC_lwip_xmit(struct netif *pNetIf, struct pbuf *pPBuf)
{
    UINT16 length = 0;
    unsigned char* send_packet;
    struct pbuf *pThisPBuf = pPBuf;
    
    GLOBAL_LOCK(encIrq);
    
    if (!pNetIf || !pPBuf)
    {
        return ERR_ARG;
    }
        
    // Wake 9218 up if it is sleeping
    if (!(PMT_CTRL & PMTCTL_READY))
    {
        BYTE_TEST = 0xFFFFFFFF;
    }
    
    length = pPBuf->tot_len;
    DWORD dwReg = TX_FIFO_INF;

/*
    if (length > (ETHERSIZE + 4))
    {
        debug_printf("xmit - length is too large, truncated");
        length = ETHERSIZE + 4;
    }
*/

    // Used to be Xmit_Packet function, now handles multiple buffer per packet
    {
        volatile DWORD dwTxCmdA, dwTxCmdB;

        // TX Command B: Packet Tag - Packet Length
        // Same for all buffers
        // Use length as packet tag (31:16), is that OK ?
        dwTxCmdB = (length << 16) | length;

        while (pThisPBuf)
        {
            // TX Command A: Buffer Byte Offset - Buffer Flags, Buffer Length
            dwTxCmdA = ((((DWORD)pThisPBuf->payload) & 0x03) << 16) | pThisPBuf->len;

            // First buffer of packet ?
            if (pThisPBuf == pPBuf)
            {
                dwTxCmdA |= TX_CMD_A_FIRST_SEG;
            }

            // Last buffer of packet ?
            if (NULL == pThisPBuf->next)
            {
                dwTxCmdA |= TX_CMD_A_LAST_SEG;
            }

            // Write TX Command DWORDS
            TX_DATA_FIFO_PORT = dwTxCmdA;
            TX_DATA_FIFO_PORT = dwTxCmdB;

            // Write data to FIFO
            SMSC9218_lwip_Write_TX_FIFO((DWORD *)(((DWORD)pThisPBuf->payload) & ~0x03), (pThisPBuf->len + 3 + (((DWORD)pThisPBuf->payload) & 0x03)) >> 2);

            pThisPBuf = pThisPBuf->next;
        }
        
        while (SMSC9218_lwip_Get_TX_Status_Count())
        {
            DWORD TxStatus = TX_STS_FIFO_PORT;
        }
    }
        
    return ERR_OK;   
}

BOOL SH7264_SMSC_lwip_SetupDevice(struct netif *pNetIf)
{
    DWORD mac;

    debug_printf("SH7264_SMSC_lwip_SetupDevice\r\n");

    //Enable interrupt
    ICR1 = 0;
    IPR01 |= 0x0D00;

    if(!SH7264_SMSC_lwip_Init(pNetIf))
        return FALSE;

    return TRUE;
}

BOOL SH7264_SMSC_lwip_Init(struct netif *pNetIf)
{
    DWORD dwReg, dwCnt, dwIdRev, mac;
    WORD wd;
    
    debug_printf("SH7264_SMSC_lwip_Init\r\n");

    // soft reset
    HW_CFG = HWCFG_SRST;
    
    dwCnt = 100000;
    do
    {
        ETHERNET_DELAY(10);
        dwReg = HW_CFG;
        dwCnt--;
    }while ((dwCnt > 0) && (dwReg & HWCFG_SRST));
    
    if (dwReg & HWCFG_SRST)
    {
        debug_printf("SMSC9218 Initialize: Error in Soft reset.\r\n");
        return FALSE;
    }

    SMSC9218_lwip_Write_Phy_Reg(PHY_INTERRUPTMASK, SMSC9218_lwip_Read_Phy_Reg(PHY_INTERRUPTMASK) & ~0x0080);
        
    //Clear interrupts
    wd = SMSC9218_lwip_Read_Phy_Reg(PHY_INTERRUPTSRC);
        
    // Put PHY out from EnergyOn mode
    SMSC9218_lwip_Write_Phy_Reg(PHY_MODE_CTLSTS, SMSC9218_lwip_Read_Phy_Reg(PHY_MODE_CTLSTS) & ~0x2000);

    // reset 9218 and restore packet filter & multicast list
    // Disable Global Interrupt
    INT_CFG = INT_CFG & ~INTCFG_IRQ_EN;

    // Clear Interrupt Status
    INT_STS = 0xFFFFFFFF;
    // Enable Global Interrupt
    INT_CFG |= INTCFG_IRQ_EN;
    
    dwIdRev = ID_REV;

    switch (dwIdRev & 0xFFFF0000)
    {
        case 0x118A0000:
            debug_printf("SMSC9218 identified. ID_REV = 0x%08lX\r\n", dwIdRev);
            break;
        default:
            debug_printf("Cannot Identify SMSC9218. (ID_REV = 0x%08x)\r\n", dwIdRev);
            return FALSE;
    }

    //Initialize Interrupts
    dwReg = 0;
    INT_EN = 0;
    INT_STS = 0xFFFFFFFF;
    dwReg |= INTCFG_IRQ_EN;
    INT_CFG = dwReg;

    if (!SMSC9218_lwip_Initialize_Phy())
    {
        return FALSE;
    }

    SMSC9218_lwip_Initialize_TX();

    SMSC9218_lwip_Initialize_RX(0x00000200, 0); // PIO with 2-byte offset

    // Mac Initialize
    SMSC9218_lwip_Enable_Sw_Flow_Control_FD();

    // LED is ON. LED1(Speed), LED2(Link&Activity), LED3(Full-Duplex)
    GPIO_CFG = GPIOCFG_LED3_GPIO2_EN | GPIOCFG_LED2_GPIO1_EN | GPIOCFG_LED1_GPIO0_EN;
    
    mac = (pNetIf->hwaddr[3] << 24) | (pNetIf->hwaddr[2]  << 16) |
          (pNetIf->hwaddr[1] << 8) | pNetIf->hwaddr[0];
    SMSC9218_lwip_Write_Mac_Reg(ADDRL, mac);
    debug_printf("Set MAC ADDRL -> 0x%X\r\n",mac);
    mac = (pNetIf->hwaddr[5] << 8) | pNetIf->hwaddr[4];
    SMSC9218_lwip_Write_Mac_Reg(ADDRH, mac);
    debug_printf("Set MAC ADDRH -> 0x%X\r\n",mac);

    SMSC9218_lwip_Initialize_Link();
    
    // Enable Rx Overrun Interrupt
    SMSC9218_lwip_Enable_Interrupt(INTEN_RXDF_INT_EN);
    
    SMSC9218_lwip_Set_Packet_Filter_Reg();
    SMSC9218_lwip_Set_Multicast_Address_List();
    
    return TRUE;
}
