////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "rtip.h"
#include "sock.h"
#include "rtipapi.h"
#include "rtpirq.h"
#include "rtp.h"
#include "rtpprint.h"
#include "rtpnet.h"

#include <tinyhal.h>
#include "SH7264_SMSC.h"

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

extern "C"
{
    extern void rtp_thrd_interrupt_continuation(int);
    extern void rtp_thrd_ip_continuation(int);
}   /* extern "C" */

RTP_BOOL    SH7264_SMSC_open       (PIFACE pi);
void        SH7264_SMSC_close      (PIFACE pi);
int         SH7264_SMSC_xmit       (PIFACE pi, DCU msg);
RTP_BOOL    SH7264_SMSC_xmit_done  (PIFACE pi, DCU msg, RTP_BOOL success);
void        SH7264_SMSC_recv       (PIFACE pi);
RTP_BOOL    SH7264_SMSC_statistics (PIFACE pi);
void        SH7264_SMSC_interrupt  (void *param);

/* ********************************************************************
   GLOBAL DATA
   ******************************************************************** */
EDEVTABLE RTP_FAR SH7264_mac_device = 
{
    SH7264_SMSC_open, 
    SH7264_SMSC_close, 
    SH7264_SMSC_xmit, 
    NULLP_FUNC,
    NULLP_FUNC, 
    SH7264_SMSC_statistics, 
    NULLP_FUNC, 
    SH7264_SMSC_DEVICE, 
    "SH7264_SMSC", 
    MINOR_0, 
    ETHER_IFACE, 
    SNMP_DEVICE_INFO(CFG_OID_SH7264_SMSC, CFG_SPEED_SH7264_SMSC)
    CFG_ETHER_MAX_MTU, 
    CFG_ETHER_MAX_MSS, 
    CFG_ETHER_MAX_WIN_IN, 
    CFG_ETHER_MAX_WIN_OUT, 
    IOADD(0), 
    EN(0), 
    EN(0)
};

extern  NETWORK_CONFIG              g_NetworkConfig;

struct SH7264_SMSC_SOFTC RTP_FAR SH7264_softc;

#define iface_to_softc(X) (PSH7264_SMSC_SOFTC) &SH7264_softc;
#define off_to_softc(X)   (PSH7264_SMSC_SOFTC) &SH7264_softc;

/*************************************************************************
 * GLOBAL DATA
 *************************************************************************/
static DWORD dwLinkMode;

void SMSC9218_Enable_Sw_Flow_Control_FD()
{
    DWORD dwCnt = 1000;

    // Enable flow control.
    AFC_CFG = 0x006E374F;

    //Wait for busy bit to clear.
    while ((SMSC9218_Read_Mac_Reg(FLOW) & MAC_FCBSY) && (dwCnt > 0))
    {
        ETHERNET_DELAY(1);
        dwCnt--;
    }

    // Set FLOW register in MAC to send out a pause frame.
    SMSC9218_Write_Mac_Reg(FLOW, (MAC_FCEN | (0xFFFF<<16) | MAC_FCBSY));
}

BOOL SMSC9218_Initialize_Phy()
{
    DWORD dwCnt;
    unsigned short usPhy;
    
    //
    // Reset the PHY.
    //
    SMSC9218_Write_Phy_Reg((unsigned short)PHY_CONTROL, (unsigned short)PHY_CTRL_RESET);
    dwCnt = 100000;
    do
    {
        ETHERNET_DELAY(10);
        usPhy = SMSC9218_Read_Phy_Reg(PHY_CONTROL);
        dwCnt--;
    } while ((dwCnt > 0) && (usPhy & PHY_CTRL_RESET));
    
    if (usPhy & PHY_CTRL_RESET)
    {
        debug_printf("PHY reset failed.\r\n");
        return FALSE;
    }
    
    return TRUE;
}

void SMSC9218_Initialize_TX()
{
    DWORD dwReg;

    // setup MAC for TX
    dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
    dwReg |= MAC_TXEN;  // Transmitter enable
    SMSC9218_Write_Mac_Reg(MAC_CR, dwReg);

    // setup Store and Forward
    dwReg = HW_CFG;
    dwReg &= (HWCFG_TX_FIF_SZ_MASK | 0xFFF);
    dwReg |= HWCFG_SF;
    HW_CFG = dwReg;
    dwReg = HW_CFG; //dummy read

    TX_CFG = TXCFG_TX_ON;   // Transmitter Enable

    FIFO_INT = 0xFF000000;
    SMSC9218_Enable_Interrupt(INTEN_TDFU_INT_EN | INTEN_TDFO_INT_EN | INTEN_TDFA_INT_EN);
}

void SMSC9218_Initialize_RX(DWORD dwRXCFG, DWORD dwRXLvl)
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
    SMSC9218_Enable_Interrupt(INTEN_RSFL_INT_EN);
}

BOOL SMSC9218_Initialize_Link()
{
    volatile WORD wReg;
    BOOL bRet=TRUE;
    DWORD dwReg;
    
    // Enable Auto-Negotiation
    
    SMSC9218_Write_Phy_Reg(PHY_CONTROL, PHY_CTRL_ANENABLE);
    
    // Set Auto-Negotiation-Advertise
    wReg = SMSC9218_Read_Phy_Reg(PHY_ANADVERTISE);
    wReg &= ~(PHY_ANA_PAUSE_OP_MASK | PHY_ANA_SPEED_MASK);
    wReg |= PHY_ANA_ASYM_PAUSE | PHY_ANA_SYM_PAUSE | PHY_ANA_10HDX | PHY_ANA_10FDX | PHY_ANA_100HDX | PHY_ANA_100FDX;
    SMSC9218_Write_Phy_Reg(PHY_ANADVERTISE, wReg);

    if(!SMSC9218_Auto_Negotiate())
    {
        bRet = FALSE;
    } 

    //Clear interrupts
    wReg = SMSC9218_Read_Phy_Reg(PHY_INTERRUPTSRC);

    // Get link mode
    dwLinkMode = SMSC9218_Get_Link_Mode();

    dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
    dwReg &= ~( MAC_RCVOWN | MAC_FDPX );
    switch(dwLinkMode){

    case LINK_NO_LINK:
        //TODO: consider auto linking to a specified link state.
        debug_printf("link is down.\r\n");
        break;

    case LINK_10MPS_HALF:
//      debug_printf("10Base-T Half Duplex capable.\r\n");
        dwReg |= MAC_RCVOWN;
        break;

    case LINK_10MPS_FULL:
//      debug_printf("10Base-T Full Duplex capable.\r\n");
        dwReg |= MAC_FDPX;
        break;

    case LINK_100MPS_HALF:
//      debug_printf("100Base-TX Half Duplex capable.\r\n");
        dwReg |= MAC_RCVOWN;
        break;

    case LINK_100MPS_FULL:
//      debug_printf("100Base-T Full Duplex capable.\r\n");
        dwReg |= MAC_FDPX;
        break;

    default:
        break;
    }
    SMSC9218_Write_Mac_Reg(MAC_CR, dwReg);  

    return bRet;
}

BOOL SMSC9218_Auto_Negotiate()
{
    DWORD dwCnt;
    unsigned short usPhy;

    debug_printf("Try Auto Negotiation.......\r\n");

    // Start AutoNegotiate
    SMSC9218_Write_Phy_Reg(PHY_CONTROL, PHY_CTRL_ANENABLE | PHY_CTRL_ANRESTART);
    
    //
    // Wait for the autonegotiation to complete
    //
    dwCnt = 100000;
    do
    {
        usPhy = SMSC9218_Read_Phy_Reg(PHY_STATUS);
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

DWORD SMSC9218_Get_Link_Mode()
{
    WORD wReg, wRegANEG, wRegCtl, wRegADV, wRegAPA;
    DWORD dwReg, LinkMode = LINK_NO_LINK;

    // Get link mode
    wReg = SMSC9218_Read_Phy_Reg(PHY_STATUS);
    wRegCtl = SMSC9218_Read_Phy_Reg(PHY_CONTROL);
    if (wRegCtl & PHY_CTRL_ANENABLE)
    {
        if(wReg & PHY_STAT_LINK_UP)
        {
            wRegADV = SMSC9218_Read_Phy_Reg(PHY_ANADVERTISE);
            wRegAPA = SMSC9218_Read_Phy_Reg(PHY_ANPARTNER);
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
                    dwReg = SMSC9218_Read_Mac_Reg(FLOW);
                    dwReg |= MAC_FCEN;
                    SMSC9218_Write_Mac_Reg(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg |= AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
                else if(((wRegADV & 0x0C00) == 0x0C00) && ((wRegAPA & 0x0C00) == 0x0800))
                {
                    // Partner is Asymmetric Flow Control
                    dwReg = SMSC9218_Read_Mac_Reg(FLOW);
                    dwReg |= MAC_FCEN;
                    SMSC9218_Write_Mac_Reg(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg &= ~AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
                else
                {
                    // Disable Flow Control
                    dwReg = SMSC9218_Read_Mac_Reg(FLOW);
                    dwReg &= ~MAC_FCEN;
                    SMSC9218_Write_Mac_Reg(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg &= ~AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
            }
            else    //Half-Duplex
            {
                // Disable Flow Control
                dwReg = SMSC9218_Read_Mac_Reg(FLOW);
                dwReg &= ~MAC_FCEN;
                SMSC9218_Write_Mac_Reg(FLOW, dwReg);

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

void SMSC9218_Write_Phy_Reg(unsigned short PhyReg, unsigned short PhyData)
{
    // check busy
    if(SMSC9218_Read_Mac_Reg(MII_ACC) & MAC_PHY_MIIBZY){
        debug_printf("SMSC9218_Write_Phy_Reg: MII is BUSY.\r\n");
        return;
    }

    // set PHY data
    SMSC9218_Write_Mac_Reg(MII_DATA, PhyData);

    // set PHY write command
    SMSC9218_Write_Mac_Reg(MII_ACC, MAC_PHY_ADDR_INTERNAL | MAC_PHY_MIIRINDA(PhyReg) | MAC_PHY_MIIWRITE);

    // wait for MII no BUSY
    if(!SMSC9218_Wait_MII_No_Busy())
    {
        debug_printf("SMSC9218_Write_Phy_Reg: PHY Write Error.\r\n");
    }
}

WORD SMSC9218_Read_Phy_Reg(WORD PhyReg)
{
    WORD wReg=0xffff;

    // check busy
    if(SMSC9218_Read_Mac_Reg(MII_ACC) & MAC_PHY_MIIBZY)
    {
        debug_printf("SMSC9218_Read_Phy_Reg: MII is BUSY.\r\n");
        return 0;
    }

    // set PHY read command
    SMSC9218_Write_Mac_Reg(MII_ACC, MAC_PHY_ADDR_INTERNAL | MAC_PHY_MIIRINDA(PhyReg) | MAC_PHY_MIIREAD);

    // wait for MII no BUSY
    if(!SMSC9218_Wait_MII_No_Busy())
    {
        debug_printf("SMSC9218_Read_Phy_Reg: PHY Read Error.\r\n");
    }
    else
    {
        // read PHY register
        wReg = (WORD)SMSC9218_Read_Mac_Reg(MII_DATA);
    }
    
    return wReg;
}

void SMSC9218_Write_Mac_Reg(WORD usMacRegister, unsigned long ulMacData)
{
    // check busy
    if(MAC_CSR_CMD & MAC_CSR_BUSY)
    {
        debug_printf("SMSC9218_Write_Mac_Reg: MAC is BUSY.\r\n");
        return;
    }

    // set MAC Data
    MAC_CSR_DATA = ulMacData;

    // set MAC CMD
    MAC_CSR_CMD = ((usMacRegister & MAC_CSR_ADDR_MASK) | MAC_CSR_BUSY | MAC_CSR_REQ_WRITE);

    // wait for MAC no BUSY
    if(!SMSC9218_Wait_MAC_No_Busy())
    {
        debug_printf("SMSC9218_Write_Mac_Reg: MAC Write Error.\r\n");
    }
}

unsigned long SMSC9218_Read_Mac_Reg(WORD usMacRegister)
{
    DWORD dwReg=0xFFFFFFFF;

    // check busy
    if(MAC_CSR_CMD & MAC_CSR_BUSY)
    {
        debug_printf("SMSC9218_Read_Mac_Reg: MAC is BUSY.\r\n");
        return dwReg;
    }

    // set MAC read command
    MAC_CSR_CMD = (usMacRegister & MAC_CSR_ADDR_MASK) | MAC_CSR_BUSY | MAC_CSR_REQ_READ;

    // wait for MAC no BUSY
    if(!SMSC9218_Wait_MAC_No_Busy())
    {
        debug_printf("SMSC9218_Read_Mac_Reg: MAC Read Error.\r\n");
    }
    else
    {
        // read MAC register
        dwReg = MAC_CSR_DATA;
    }

    return dwReg;
}

BOOL SMSC9218_Wait_MAC_No_Busy()
{
    DWORD dwCnt;

    for(dwCnt = 0; dwCnt < 40; dwCnt++)
    {
        if(!(MAC_CSR_CMD & MAC_CSR_BUSY))
        {
            return TRUE;
        }
    }

    debug_printf("SMSC9218_Wait_MAC_No_Busy: MAC BUSY.\r\n");

    return FALSE;
}

BOOL SMSC9218_Wait_MII_No_Busy()
{
    DWORD dwCnt;

    for(dwCnt = 0; dwCnt < 100; dwCnt++)
    {
        if(!(SMSC9218_Read_Mac_Reg(MII_ACC) & MAC_PHY_MIIBZY))
        {
            return TRUE;
        }
    }

    debug_printf("SMSC9218_Wait_MII_No_Busy: PHY MII BUSY.\r\n");

    return FALSE;
}

void SMSC9218_Enable_Interrupt(DWORD dwMask)
{
    DWORD dwReg;

    dwReg = INT_EN;
    dwReg |= dwMask | INTEN_PME_INT_EN;
    INT_EN = dwReg;
}

void SMSC9218_Set_Mac_Filter_Reg()
{
    DWORD   dwReg;
    
    dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);  
    dwReg &= (~(MAC_MCPAS | MAC_PRMS | MAC_INVFILT | MAC_HO | MAC_HPFILT | MAC_BCAST));
    dwReg |= MAC_MCPAS | MAC_PRMS;  
    SMSC9218_Write_Mac_Reg(MAC_CR, dwReg);
}

RTP_BOOL SH7264_SMSC_open(PIFACE pi)   
{
    DWORD mac[2], dwCnt, dwReg, dw; 
    PSH7264_SMSC_SOFTC sc = iface_to_softc(pi);

    if (!sc)
    {
        RTP_DEBUG_ERROR("SH7264_SMSC_open: softc invalid!\r\n", NOVAR, 0, 0);
        set_errno(ENUMDEVICE);
        return(RTP_FALSE);
    }

    // Set Interface
    sc->iface = pi;
    iface = pi;

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
    
    //Read Mac address from EEPROM          
    mac[0]  = BIT_EDIANDW(SMSC9218_Read_Mac_Reg(ADDRL));
    mac[1]  = BIT_EDIANDW(SMSC9218_Read_Mac_Reg(ADDRH));
    
    int macLen = __min(g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264RSK].macAddressLen, sizeof(sc->mac_address));
    
    dw = (DWORD)&g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264RSK].macAddressBuffer[0];
    if (((dw >= 0x00000000) && (dw <= 0x03000000)) || ((dw >= 0x20000000) && (dw <= 0x23000000)))
    {
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
        
    // Now put in a dummy ethernet address
    rtp_memcpy(&pi->addr.my_hw_addr[0], sc->mac_address, 6); // Get the ethernet address
    
    // clear statistic information
    sc->stats.packets_in     = 0L;
    sc->stats.packets_out    = 0L;
    sc->stats.bytes_in       = 0L;
    sc->stats.bytes_out      = 0L;
    sc->stats.errors_in      = 0L;
    sc->stats.errors_out     = 0L;    

    if(RTP_FALSE == SH7264_SMSC_SetupDevice())
    {
        return RTP_FALSE;
    }
    
    rtp_irq_hook_interrupt( (RTP_PFVOID) pi, 
                            (RTP_IRQ_FN_POINTER)SH7264_SMSC_recv, 
                            (RTP_IRQ_FN_POINTER) 0);

    return(RTP_TRUE);
}

void SH7264_SMSC_close(PIFACE pi)                          
{
    ARGSUSED_PVOID( pi );  //  keep compiler happy
}

void SH7264_SMSC_interrupt(void *param)
{   
    long int_sts;
    
    GLOBAL_LOCK(encIrq);
    
    int_sts = INT_STS;
    
    // Frame(s) received
    if(int_sts & INTSTS_RSFL_INT)
    {
        Events_Set( SYSTEM_EVENT_FLAG_SOCKET ); 
        SMSC9218_RX_ISR();
        rtp_thrd_interrupt_continuation(iface->ctrl.index);
        INT_STS = INTSTS_RSFL_INT;
    }
    
    // Clear interrupt status
    INT_STS = 0xFFFFFFFF;
}

void SH7264_SMSC_recv(PIFACE pi)
{
    GLOBAL_LOCK(encIrq);
    
    if (!pi)
    {
        return;
    }
    
    SMSC9218_RX_ISR();
}

void SMSC9218_RX_ISR()
{       
    DWORD RxStatus, packet_len, dw;
    DWORD numPacketReceived;
    DWORD RxFFCountDW;
    DCU msg;
    unsigned char* recv_packet;
    unsigned long temp;
    int tries = 0;

    dw = RX_FIFO_INF;
    numPacketReceived = (dw & 0x00FF0000) >> 16;

    while(numPacketReceived)
    {
        tries = 0;
        RxStatus = RX_STS_FIFO_PORT;
        packet_len = (RxStatus & 0x3FFF0000) >> 16;

        if (!(RxStatus & RX_STS_ERROR) && 
            !(((RxStatus & RX_STS_LEN_ERROR) == RX_STS_LEN_ERROR) && 
            ((RxStatus & RX_STS_FRAME_TYPE) == 0)))
        {   
            do
            {
                msg = os_alloc_packet_input(packet_len - 2, DRIVER_ALLOC);
                if(!msg)
                    for(int i = 0; i < 500; i++);       //delay
            }while(!msg && tries++ < 50);
                    
            if(msg)
            {
                unsigned long *rxbpl    = (unsigned long *) RX_BUFFER;
                unsigned char *rx_ptr   = (unsigned char *) RX_BUFFER;
                int ndx;
                    
                for (ndx = ((packet_len + 2 + 3) >> 2); ndx > 0; ndx--)
                {
                    temp = RX_DATA_FIFO_PORT;
                    *rxbpl++ = BIT_EDIANDW(temp);
                }

                recv_packet = (unsigned char*)DCUTODATA(msg);
                msg->length = packet_len - 2;
                memcpy(recv_packet, rx_ptr + 2, packet_len - 2);
                rtp_net_invoke_input(iface, msg, msg->length);              
            }
            else
            {
                // Read off and discard the packet.
                RxFFCountDW = (packet_len + 2 + 3) >> 2;
                SMSC9218_RX_Fast_Forward(RxFFCountDW);
            }
        }
        else
        {
            // Packet has error. Discard it and update stats.
            RxFFCountDW = (packet_len + 2 + 3) >> 2;
            SMSC9218_RX_Fast_Forward(RxFFCountDW);          
        }

        dw = RX_FIFO_INF;
        numPacketReceived = (dw & 0x00FF0000) >> 16;
    }
}

void SMSC9218_RX_Fast_Forward(DWORD dwDWCount)
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

DWORD SMSC9218_Get_TX_Status_Count()
{
    DWORD dwReg;

    dwReg = TX_FIFO_INF;
    dwReg &= TXFIFO_TXSUSED_MASK;
    dwReg >>= 16;
    dwReg &= 0xFFFF;
    
    return dwReg;
}


void SMSC9218_Set_Packet_Filter_Reg()
{
    DWORD   dwReg;
    
    if (dwLinkMode == LINK_10MPS_HALF)
    {
        SMSC9218_Disable_Mac_RX();
        SMSC9218_Set_Mac_Filter_Reg();
        SMSC9218_Enable_Mac_RX();
    }
    else if (dwLinkMode == LINK_10MPS_FULL)
    {
        // 10/FD
        // Disable RX 
        dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            INT_STS = INTSTS_RXSTOP_INT;
            // Enable
            SMSC9218_Enable_Interrupt(INTEN_RXSTOP_INT_EN);
            dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
            dwReg &= ~MAC_RXEN;
            SMSC9218_Write_Mac_Reg(MAC_CR, dwReg);
            return;
        }
        else
        {
            SMSC9218_Set_Mac_Filter_Reg();
            SMSC9218_Enable_Mac_RX();
        }
    }
    else if ((dwLinkMode == LINK_100MPS_FULL) || (dwLinkMode == LINK_100MPS_HALF))
    {
        dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            // 100/FD/HD
            // Initiate SWINT
            SMSC9218_Enable_Interrupt(INTEN_SW_INT_EN);
        }
        else
        {
            SMSC9218_Write_Mac_Reg(HASHL, 0);
            SMSC9218_Write_Mac_Reg(HASHH, 0);
            SMSC9218_Set_Mac_Filter_Reg();
            // Enable RX 
            SMSC9218_Enable_Mac_RX();
        }
    }
}

void SMSC9218_Set_Multicast_Address_List()
{
    DWORD   dwReg;
    
    if (dwLinkMode == LINK_10MPS_HALF)
    {
        SMSC9218_Disable_Mac_RX();
        SMSC9218_Write_Mac_Reg(HASHL, 0);
        SMSC9218_Write_Mac_Reg(HASHH, 0);
        SMSC9218_Enable_Mac_RX();
    }
    else if (dwLinkMode == LINK_10MPS_FULL)
    {
        // 10/FD
        dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            INT_STS = INTSTS_RXSTOP_INT;
            // Enable Interrupt
            SMSC9218_Enable_Interrupt(INTEN_RXSTOP_INT_EN);

            dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
            dwReg &= ~MAC_RXEN;
            SMSC9218_Write_Mac_Reg(MAC_CR, dwReg);
        }
        else
        {
            SMSC9218_Write_Mac_Reg(HASHL, 0);
            SMSC9218_Write_Mac_Reg(HASHH, 0);

            // Enable RX_EN
            dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
            dwReg |= MAC_RXEN;
            SMSC9218_Write_Mac_Reg(MAC_CR, dwReg);
        }
    }
    else if ((dwLinkMode == LINK_100MPS_FULL) || (dwLinkMode == LINK_100MPS_HALF))
    {
        dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            // 100/FD/HD
            // Initiate SWINT Interrupt 
            SMSC9218_Enable_Interrupt(INTEN_SW_INT_EN);
        }
        else
        {
            SMSC9218_Write_Mac_Reg(HASHL, 0);
            SMSC9218_Write_Mac_Reg(HASHH, 0);
            SMSC9218_Set_Mac_Filter_Reg();
            // Enable RX 
            SMSC9218_Enable_Mac_RX();
        }
    }
}

void SMSC9218_Enable_Mac_RX()
{
    volatile DWORD  dwReg;
    
    // Enable RX 
    dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
    dwReg |= MAC_RXEN;
    SMSC9218_Write_Mac_Reg(MAC_CR, dwReg);

    // Wait for at least 64uSec
    ETHERNET_DELAY(64);
}

void SMSC9218_Disable_Mac_RX()
{
    volatile DWORD  dwReg;
    volatile long   counter;
    
    dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
    if (dwReg & MAC_RXEN)
    {
        INT_STS = INTSTS_RXSTOP_INT;

        dwReg = SMSC9218_Read_Mac_Reg(MAC_CR);
        dwReg &= ~MAC_RXEN; //disable RX
        SMSC9218_Write_Mac_Reg(MAC_CR, dwReg);

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

BOOL Xmit_Packet(unsigned char * txbp, int len)
{
    volatile DWORD  dwTxCmdA, dwTxCmdB;
    
    GLOBAL_LOCK(encIrq);
    
    dwTxCmdB = (((DWORD)len) << 16) | ((DWORD)len);
    dwTxCmdA=(((((DWORD)txbp) & 0x03) << 16) | TX_CMD_A_FIRST_SEG | TX_CMD_A_LAST_SEG | ((DWORD)len));

    TX_DATA_FIFO_PORT = dwTxCmdA;
    TX_DATA_FIFO_PORT = dwTxCmdB;
    SMSC9218_Write_TX_FIFO((DWORD *)(((DWORD)txbp) & ~0x03), ((DWORD)len + 3 + (((DWORD)txbp) & 0x03)) >> 2);

    while (SMSC9218_Get_TX_Status_Count())
    {
        DWORD TxStatus = TX_STS_FIFO_PORT;
    }

    return TRUE;
}

void SMSC9218_Write_TX_FIFO(DWORD * pdwBuf, DWORD dwDwordCount)
{
    unsigned long temp;
    
    while (dwDwordCount)
    {
        temp = *pdwBuf++;
        TX_DATA_FIFO_PORT = BIT_EDIANDW(temp);
        dwDwordCount--;
    }
}

int SH7264_SMSC_xmit(PIFACE pi, DCU msg)
{
    RTP_UINT16 length = 0;
    unsigned char* send_packet;
    
    GLOBAL_LOCK(encIrq);
    
    if (!pi)
    {
        return (-1);
    }
    
    // Wake 9218 up if it is sleeping
    if (!(PMT_CTRL & PMTCTL_READY))
    {
        BYTE_TEST = 0xFFFFFFFF;
    }
    
    length = msg->length;

    if (length > (ETHERSIZE + 4))
    {
        RTP_DEBUG_ERROR("xmit - length is too large, truncated", NOVAR, 0, 0);
        length = ETHERSIZE + 4;
    }

    send_packet = (unsigned char*)DCUTODATA(msg);
    
    if(!Xmit_Packet(send_packet, length))
            return -1;
    
    rtp_net_invoke_output(pi, 1);
        
    return 0;   
}

RTP_BOOL SH7264_SMSC_xmit_done  (PIFACE pi, DCU msg, RTP_BOOL success)
{
    PSH7264_SMSC_SOFTC   sc;

    ARGSUSED_PVOID(msg);

    if(!pi)
    {
        return (RTP_FALSE);
    }

    sc = iface_to_softc(pi);
    if(!sc)
        return(RTP_TRUE);

    if(!success)
    {
        sc->stats.errors_out++;
        sc->stats.tx_other_errors++;
    }
    else
    {
        // Update total number of successfully transmitted packets.
        sc->stats.packets_out++;
        sc->stats.bytes_out += msg->length;
    }
    return(RTP_TRUE);
}

RTP_BOOL SH7264_SMSC_statistics(PIFACE pi)                       
{
#if (!INCLUDE_KEEP_STATS)
    ARGSUSED_PVOID(pi)
#endif

    if (!pi)
    {
        return RTP_FALSE;
    }

    UPDATE_SET_INFO(pi, interface_packets_in, SH7264emac_packets_in)
    UPDATE_SET_INFO(pi, interface_packets_out, SH7264emac_packets_out)
    UPDATE_SET_INFO(pi, interface_bytes_in, SH7264emac_bytes_in)
    UPDATE_SET_INFO(pi, interface_bytes_out, SH7264emac_bytes_out)
    UPDATE_SET_INFO(pi, interface_errors_in, SH7264emac_errors_in)
    UPDATE_SET_INFO(pi, interface_errors_out, SH7264emac_errors_out)
    UPDATE_SET_INFO(pi, interface_packets_lost, 0L)
    return(RTP_TRUE);
}

int xn_bind_SH7264_mac(int minor_number)
{
    return(xn_device_table_add(SH7264_mac_device.device_id,
                                minor_number, 
                                SH7264_mac_device.iface_type,
                                SH7264_mac_device.device_name,
                                SNMP_DEVICE_INFO(SH7264_mac_device.media_mib, 
                                                    SH7264_mac_device.speed)                         
                                (DEV_OPEN)SH7264_mac_device.open,
                                (DEV_CLOSE)SH7264_mac_device.close,
                                (DEV_XMIT)SH7264_mac_device.xmit,
                                (DEV_XMIT_DONE)SH7264_mac_device.xmit_done,
                                (DEV_PROCESS_INTERRUPTS)SH7264_mac_device.proc_interrupts,
                                (DEV_STATS)SH7264_mac_device.statistics,
                                (DEV_SETMCAST)SH7264_mac_device.setmcast));
}

RTP_BOOL SH7264_SMSC_SetupDevice()
{
    DWORD mac;
/*  
    if (WORD_SWAP != 0xFFFFFFFF)    //Need at least one read from LAN chip before writing to WORD_SWAP register
    {
        WORD_SWAP = 0xFFFFFFFF;
    }
    
    ETHERNET_DELAY(10000);
    
    //Setting port
    PECR0 |= 0x0030;
    PJCR0 |= 0x3000;
*/  
    //Enable interrupt
    ICR1 = 0;
    IPR01 |= 0x0D00;
    
    mac  = SMSC9218_Read_Mac_Reg(ADDRL);
    mac  = SMSC9218_Read_Mac_Reg(ADDRH);
    
    if(!SH7264_SMSC_Init())
        return RTP_FALSE;

    debug_printf("Waiting for getting IP address...\r\n");

    return RTP_TRUE;
}

RTP_BOOL SH7264_SMSC_Init()
{
    DWORD dwReg, dwCnt, dwIdRev, mac;
    WORD wd;
    
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
        return RTP_FALSE;
    }

    SMSC9218_Write_Phy_Reg(PHY_INTERRUPTMASK, SMSC9218_Read_Phy_Reg(PHY_INTERRUPTMASK) & ~0x0080);
        
    //Clear interrupts
    wd = SMSC9218_Read_Phy_Reg(PHY_INTERRUPTSRC);
        
    // Put PHY out from EnergyOn mode
    SMSC9218_Write_Phy_Reg(PHY_MODE_CTLSTS, SMSC9218_Read_Phy_Reg(PHY_MODE_CTLSTS) & ~0x2000);

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
            return RTP_FALSE;
    }

    //Initialize Interrupts
    dwReg = 0;
    INT_EN = 0;
    INT_STS = 0xFFFFFFFF;
    dwReg |= INTCFG_IRQ_EN;
    INT_CFG = dwReg;

    if (!SMSC9218_Initialize_Phy())
    {
        return RTP_FALSE;
    }

    SMSC9218_Initialize_TX();

    SMSC9218_Initialize_RX(0x00000200, 0); // PIO with 2-byte offset

    // Mac Initialize
    SMSC9218_Enable_Sw_Flow_Control_FD();

    // LED is ON. LED1(Speed), LED2(Link&Activity), LED3(Full-Duplex)
    GPIO_CFG = GPIOCFG_LED3_GPIO2_EN | GPIOCFG_LED2_GPIO1_EN | GPIOCFG_LED1_GPIO0_EN;
    
    mac = ((gmac_address[1] & 0x00FF) << 24) | ((gmac_address[1] & 0xFF00) << 8) |
          ((gmac_address[0] & 0x00FF) << 8) | ((gmac_address[0] & 0xFF00) >> 8);
    SMSC9218_Write_Mac_Reg(ADDRL, mac);
    
    mac = ((gmac_address[2] & 0x00FF) << 8) | ((gmac_address[2] & 0xFF00) >> 8);
    SMSC9218_Write_Mac_Reg(ADDRH, mac);

    SMSC9218_Initialize_Link();
    
    // Enable Rx Overrun Interrupt
    SMSC9218_Enable_Interrupt(INTEN_RXDF_INT_EN);
    
    SMSC9218_Set_Packet_Filter_Reg();
    SMSC9218_Set_Multicast_Address_List();
    
    return RTP_TRUE;
}
