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
/* ********************************************************************
   DEBUG AIDS
   ******************************************************************** */
#define DEBUG_SH7264EMAC 0

#ifndef NETWORK_INTERFACE_INDEX_SH7264EMAC
#define NETWORK_INTERFACE_INDEX_SH7264EMAC 0
#endif

#define BIT_EDIANDW(data) ((data >> 24) & 0x000000FF) | ((data << 24) & 0xFF000000) | ((data >> 8) & 0x0000FF00) | ((data << 8) & 0x00FF0000);

/**************************************************************************
 * LOCAL DEFINITIONS AND MACROS
 **************************************************************************/
#define     RX_BUFFER 0x0D000000
#define     ENET_MAX_MTU            (512+4) //PKTSIZE
#define     ENET_ADDR_LENGTH        6
#define     TX_TIMEOUT_COUNT        30      // waiting for TX_FIFO to drain

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

// Receive Transfer Descriptor buffer
//static RxTd rxTd; 
// Transmit Transfer Descriptor buffer
//static TxTd txTd; 

unsigned char protocol_type[] = {0x08, 0x06};
/*************************************************************************
 * GLOBAL DATA
 *************************************************************************/
 
static DWORD dwLinkMode;
static long f100RxEnWorkaroundDone;

BOOL IS_ARRAY_EMPTY(PKT_LIST x);
BOOL IS_ARRAY_FULL(PKT_LIST x);

RTP_BOOL SH7264_SMSC_SetupDevice(void);
BOOL Smsc9218_Xmit_Packet(unsigned char * txbp, int len);
DWORD Smsc9218_GetTxStatusCount();
void HandlerRxISR();
void RxFastForward(DWORD dwDWCount);
BOOL CardInitialize();
BOOL Smsc9218_Initialize();
BOOL CardSetup();
BOOL Smsc9218_InitializePhy();
void Smsc9218_InitializeTX();
void Smsc9218_InitializeRX(DWORD dwRXCFG, DWORD dwRXLvl);
void EnableSwFlowControlFD();
void CardStart();
BOOL Smsc9218_EstablishLink();
BOOL Smsc9218_AutoNegotiate();
DWORD Smsc9218_GetLinkMode();
void Smsc9218_WritePhy(unsigned short usPhyRegister, unsigned short usPhyData);
unsigned short Smsc9218_ReadPhy(unsigned short usPhyRegister);
void Smsc9218_WriteMac(unsigned short usMacRegister, unsigned long ulMacData);
unsigned long Smsc9218_ReadMac(unsigned short usMacRegister);
BOOL Smsc9218_WaitMACNoBusy();
BOOL Smsc9218_WaitMIINoBusy();
void Smsc9218_EnableInterrupt(DWORD dwMask);
void Smsc9218_DisableInterrupt(DWORD dwMask);
DWORD Smsc9218_GetInterruptStatus();
void Smsc9218_ClearInterruptStatus(DWORD dwMask);
void Smsc9218_InitializeInterrupts(DWORD dwRegINTCFG);
BOOL InitializeQueues();
DCU GetPktFromEmptyArray();
DCU GetPktFromFullArray();
BOOL PutPktToEmptyArray(DCU pPkt);
DCU PutPktToFullArray();
void DispatchSetPacketFilter();
void DispatchSetMulticastAddressList();
void EnableMacRxEn();
void DisableMacRxEn();
void Smsc9218SetMacFilter();
void UpdateFilterAndMacReg();

BOOL IS_ARRAY_EMPTY(PKT_LIST x)
{
    if(x.dwRdPtr == x.dwWrPtr)
        return TRUE;
    return FALSE;
}

BOOL IS_ARRAY_FULL(PKT_LIST x)
{
    if(x.dwRdPtr == ((x.dwWrPtr+1UL)%(DWORD)(MAX_RXPACKET_IN_LIST)))
        return TRUE;
    return FALSE;
}


void SH7264_SMSC_SwapEtherData(unsigned char* data, unsigned long leng)
{
    unsigned char temp;
    for(unsigned long i = 0; i < leng; i += 4)
    {
        temp = data[i];
        data[i] = data[i + 3];
        data[i + 3] = temp;
        
        temp = data[i + 1];
        data[i + 1] = data[i + 2];
        data[i + 2] = temp;
    }
}

BOOL CardInitialize()
{
    DWORD dwIdRev;
    
    hal_printf("+CardInitialize\r\n");

    if(!Smsc9218_Initialize()){
        return FALSE;
    }

    dwIdRev = ID_REV;
    switch (dwIdRev & 0xFFFF0000)
    {
        case 0x118A0000:
            hal_printf("Smsc9218 identified. ID_REV = 0x%08lX\r\n", dwIdRev);
            break;
        default:
            hal_printf("Can not Identify SMSC9218. (ID_REV = 0x%08x)\r\n", dwIdRev);
            return FALSE;
    }

    hal_printf("-CardInitialize\r\n");
    return TRUE;
}

BOOL Smsc9218_Initialize()
{
    DWORD dwReg;
    DWORD dwCnt;
    
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
        hal_printf("Smsc9218_Initialize: ERROR! soft reset.\r\n");
        return FALSE;
    }
    
    return TRUE;
}

BOOL CardSetup()
{
//  DWORD dwADDRL, dwADDRH;
    unsigned int MACAddr;

    hal_printf("+SMSC9218: CardSetup\r\n");

    Smsc9218_InitializeInterrupts(0);

    if (!Smsc9218_InitializePhy())
    {
        return FALSE;
    }

    Smsc9218_InitializeTX();

    Smsc9218_InitializeRX(0x00000200, 0); // PIO with 2-byte offset

    // Mac Initialize
    EnableSwFlowControlFD();

    // LED is ON. LED1(Speed), LED2(Link&Activity), LED3(Full-Duplex)
    GPIO_CFG = GPIOCFG_LED3_GPIO2_EN | GPIOCFG_LED2_GPIO1_EN | GPIOCFG_LED1_GPIO0_EN;

    //gmac_address[3] = {0x0000, 0x8736, 0xC469};
        
    //#define MALR_DUMMY_ADDR     0x0000e10c
    //#define MAHR_DUMMY_ADDR     0x000050ff
    
    //Smsc9218_WriteMac(ADDRL, 0x0ce10000);
    //Smsc9218_WriteMac(ADDRH, 0x0000ff50);

    Smsc9218_WriteMac(ADDRL, 0x36870000);
    Smsc9218_WriteMac(ADDRH, 0x000069C4);

    hal_printf("-SMSC9218: CardSetup\r\n");

    return TRUE;
}

BOOL Smsc9218_InitializePhy()
{
    DWORD dwCnt;
    unsigned short usPhy;
    
    //
    // Reset the PHY.
    //
    Smsc9218_WritePhy((unsigned short)PHY_CONTROL, (unsigned short)PHY_CTRL_RESET);
    dwCnt = 100000;
    do
    {
        ETHERNET_DELAY(10);
        usPhy = Smsc9218_ReadPhy(PHY_CONTROL);
        dwCnt--;
    } while ((dwCnt > 0) && (usPhy & PHY_CTRL_RESET));
    
    if (usPhy & PHY_CTRL_RESET)
    {
        hal_printf("Smsc9218_InitializePhy: ERROR! PHY reset failed.\r\n");
        return FALSE;
    }
    
    return TRUE;
}

void Smsc9218_InitializeTX()
{
    DWORD dwReg;

    // setup MAC for TX
    dwReg = Smsc9218_ReadMac(MAC_CR);
    dwReg |= MAC_TXEN;  // Transmitter enable
    Smsc9218_WriteMac(MAC_CR, dwReg);

    // setup Store and Forward
    dwReg = HW_CFG;
    dwReg &= (HWCFG_TX_FIF_SZ_MASK | 0xFFF);
    dwReg |= HWCFG_SF;
    HW_CFG = dwReg;
    dwReg = HW_CFG; //dummy read

    TX_CFG = TXCFG_TX_ON;   // Transmitter Enable

    FIFO_INT = 0xFF000000;
    Smsc9218_EnableInterrupt(INTEN_TDFU_INT_EN | INTEN_TDFO_INT_EN | INTEN_TDFA_INT_EN);
}

void Smsc9218_InitializeRX(DWORD dwRXCFG, DWORD dwRXLvl)
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
    Smsc9218_EnableInterrupt(INTEN_RSFL_INT_EN);
}

void EnableSwFlowControlFD()
{
    DWORD dwCnt = 1000;

    // Enable flow control.
    AFC_CFG = 0x006E374F;

    //Wait for busy bit to clear.
    while ((Smsc9218_ReadMac(FLOW) & MAC_FCBSY) && (dwCnt > 0))
    {
        ETHERNET_DELAY(1);
        dwCnt--;
    }

    // if(!dwCnt)
    // {
        // hal_printf("Timeout waiting flow busy bit.\r\n");
    // }

    // Set FLOW register in MAC to send out a pause frame.
    Smsc9218_WriteMac(FLOW, (MAC_FCEN | (0xFFFF<<16) | MAC_FCBSY));
}

void CardStart()
{
    BOOL    bRet;
    DWORD   dwTemp;

    hal_printf("+SMSC9218: CardStart\r\n");
    
    // we don't care about return value now.
    bRet = Smsc9218_EstablishLink();
    // if (pAdapter->smsc9218data.dwLinkMode != LINK_NO_LINK)
    // {
    // }
    
    
    // else 
    // {
        // if (pAdapter->CurrentPowerState == NdisDeviceStateD3) 
        // {                        
            // WriteDWord(pAdapter, PMT_CTRL_OFFSET, ReadDWord(pAdapter, PMT_CTRL_OFFSET) & ~PMTCTL_PM_MODE_MASK);
            // DelayUsingFreeRun(pAdapter, 64);
            // Smsc9218_WritePhy(pAdapter, PHY_CONTROL, Smsc9218_ReadPhy(pAdapter, PHY_CONTROL) & ~PHY_CTRL_PWRDWN);                
            // WriteDWord(pAdapter, INT_CFG_OFFSET, ReadDWord(pAdapter, INT_CFG_OFFSET) | INTCFG_IRQ_EN);   
        // }
    // }

    // dwTemp = GPT_CFG;
    // dwTemp &= ~0xFFFF;
    // dwTemp |= (GPT_INT_INTERVAL | GPTCFG_TIMER_EN);
    // GPT_CFG = dwTemp;    
    // Smsc9218_EnableInterrupt(INTEN_GPT_INT_EN);

    // Enable Rx Overrun Interrupt
    Smsc9218_EnableInterrupt(INTEN_RXDF_INT_EN);

    hal_printf("-SMSC9218: CardStart\r\n");
}

BOOL Smsc9218_EstablishLink()
{
    volatile WORD wReg;
    BOOL bRet=TRUE;
    DWORD dwReg;
    
    //
    // Enable Auto-Negotiation
    //
    Smsc9218_WritePhy(PHY_CONTROL, PHY_CTRL_ANENABLE);
    
    //
    // Set Auto-Negotiation-Advertise
    //
    wReg = Smsc9218_ReadPhy(PHY_ANADVERTISE);
    wReg &= ~(PHY_ANA_PAUSE_OP_MASK | PHY_ANA_SPEED_MASK);
    wReg |= PHY_ANA_ASYM_PAUSE | PHY_ANA_SYM_PAUSE | PHY_ANA_10HDX | PHY_ANA_10FDX | PHY_ANA_100HDX | PHY_ANA_100FDX;
    Smsc9218_WritePhy(PHY_ANADVERTISE, wReg);

    if(!Smsc9218_AutoNegotiate())
    {
        bRet = FALSE;
    } 

    //Clear interrupts
    wReg = Smsc9218_ReadPhy(PHY_INTERRUPTSRC);

    // Get link mode
    dwLinkMode = Smsc9218_GetLinkMode();

    dwReg = Smsc9218_ReadMac(MAC_CR);
    dwReg &= ~( MAC_RCVOWN | MAC_FDPX );
    switch(dwLinkMode){

    case LINK_NO_LINK:
        //TODO: consider auto linking to a specified link state.
        hal_printf("link is down.\r\n");
        break;

    case LINK_10MPS_HALF:
        hal_printf("10Base-T Half Duplex capable.\r\n");
        dwReg |= MAC_RCVOWN;
        break;

    case LINK_10MPS_FULL:
        hal_printf("10Base-T Full Duplex capable.\r\n");
        dwReg |= MAC_FDPX;
        break;

    case LINK_100MPS_HALF:
        hal_printf("100Base-TX Half Duplex capable.\r\n");
        dwReg |= MAC_RCVOWN;
        break;

    case LINK_100MPS_FULL:
        hal_printf("100Base-T Full Duplex capable.\r\n");
        dwReg |= MAC_FDPX;
        break;

    default:
        break;
    }
    Smsc9218_WriteMac(MAC_CR, dwReg);   

    return bRet;
}

BOOL Smsc9218_AutoNegotiate()
{
    DWORD dwCnt;
    unsigned short usPhy;

    hal_printf("Try Auto Negotiation.......\r\n");

    // Start AutoNegotiate
    Smsc9218_WritePhy(PHY_CONTROL, PHY_CTRL_ANENABLE | PHY_CTRL_ANRESTART);
    //Smsc9218_WritePhy(pAdapter, PHY_CONTROL, 0x1100 | PHY_CTRL_ANRESTART);

    //
    // Wait for the autonegotiation to complete
    //
    dwCnt = 100000;
    do
    {
        usPhy = Smsc9218_ReadPhy(PHY_STATUS);
        if (usPhy & (PHY_STAT_ANCOMPLETE | PHY_STAT_REMOTEFAULT))
        {
            break;
        }
        dwCnt--;
    }while(dwCnt > 0);
    
    if (usPhy & PHY_STAT_ANCOMPLETE)
    {
        hal_printf("Smsc9218_AutoNegotiate: Auto Negotiation done.\r\n");
    }
    else
    {
        if(dwCnt < 1)
        {
            hal_printf("time out.\r\n");
        }
        else
        {
            hal_printf("remote fault.\r\n");
        }
        hal_printf("Smsc9218_AutoNegotiate: Auto Negotiation Failed.\r\n");
        return FALSE;
    }
    return TRUE;
}

DWORD Smsc9218_GetLinkMode()
{
    WORD wReg, wRegANEG, wRegCtl, wRegADV, wRegAPA;
    DWORD dwReg, LinkMode = LINK_NO_LINK;
    
    //
    // Get link mode
    //
    wReg = Smsc9218_ReadPhy(PHY_STATUS);
    wRegCtl = Smsc9218_ReadPhy(PHY_CONTROL);
    if (wRegCtl & PHY_CTRL_ANENABLE)
    {
        if(wReg & PHY_STAT_LINK_UP)
        {
            wRegADV = Smsc9218_ReadPhy(PHY_ANADVERTISE);
            wRegAPA = Smsc9218_ReadPhy(PHY_ANPARTNER);
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
                    dwReg = Smsc9218_ReadMac(FLOW);
                    dwReg |= MAC_FCEN;
                    Smsc9218_WriteMac(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg |= AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
                else if(((wRegADV & 0x0C00) == 0x0C00) && ((wRegAPA & 0x0C00) == 0x0800))
                {
                    // Partner is Asymmetric Flow Control
                    dwReg = Smsc9218_ReadMac(FLOW);
                    dwReg |= MAC_FCEN;
                    Smsc9218_WriteMac(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg &= ~AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
                else
                {
                    // Disable Flow Control
                    dwReg = Smsc9218_ReadMac(FLOW);
                    dwReg &= ~MAC_FCEN;
                    Smsc9218_WriteMac(FLOW, dwReg);

                    dwReg = AFC_CFG;
                    dwReg &= ~AFCCFG_FCANY;
                    AFC_CFG = dwReg;
                }
            }
            else    //Half-Duplex
            {
                // Disable Flow Control
                dwReg = Smsc9218_ReadMac(FLOW);
                dwReg &= ~MAC_FCEN;
                Smsc9218_WriteMac(FLOW, dwReg);

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

void Smsc9218_WritePhy(unsigned short usPhyRegister, unsigned short usPhyData)
{
    // check busy
    if(Smsc9218_ReadMac(MII_ACC) & MAC_PHY_MIIBZY){
        hal_printf("Smsc9218_WritePhy: MII is BUSY.\r\n");
        return;
    }

    // set PHY data
    Smsc9218_WriteMac(MII_DATA, usPhyData);

    // set PHY write command
    Smsc9218_WriteMac(MII_ACC, MAC_PHY_ADDR_INTERNAL | MAC_PHY_MIIRINDA(usPhyRegister) | MAC_PHY_MIIWRITE);

    // wait for MII no BUSY
    if(!Smsc9218_WaitMIINoBusy())
    {
        hal_printf("Smsc9218_WritePhy: PHY Write Error.\r\n");
    }
}

unsigned short Smsc9218_ReadPhy(unsigned short usPhyRegister)
{
    WORD wReg=0xffff;

    // check busy
    if(Smsc9218_ReadMac(MII_ACC) & MAC_PHY_MIIBZY)
    {
        hal_printf("Smsc9218_ReadPhy: MII is BUSY.\r\n");
        return 0;
    }

    // set PHY read command
    Smsc9218_WriteMac(MII_ACC, MAC_PHY_ADDR_INTERNAL | MAC_PHY_MIIRINDA(usPhyRegister) | MAC_PHY_MIIREAD);

    // wait for MII no BUSY
    if(!Smsc9218_WaitMIINoBusy())
    {
        hal_printf("Smsc9218_ReadPhy: PHY Read Error.\r\n");
    }
    else
    {
        // read PHY register
        wReg = (WORD)Smsc9218_ReadMac(MII_DATA);
    }
    
    return wReg;
}

void Smsc9218_WriteMac(unsigned short usMacRegister, unsigned long ulMacData)
{
    // check busy
    if(MAC_CSR_CMD & MAC_CSR_BUSY)
    {
        hal_printf("Smsc9218_WriteMac: MAC is BUSY.\r\n");
        return;
    }

    // set MAC Data
    MAC_CSR_DATA = ulMacData;

    // set MAC CMD
    MAC_CSR_CMD = ((usMacRegister & MAC_CSR_ADDR_MASK) | MAC_CSR_BUSY | MAC_CSR_REQ_WRITE);

    // wait for MAC no BUSY
    if(!Smsc9218_WaitMACNoBusy())
    {
        hal_printf("Smsc9218_WriteMac: MAC Write Error.\r\n");
    }
}

unsigned long Smsc9218_ReadMac(unsigned short usMacRegister)
{
    DWORD dwReg=0xFFFFFFFF;

    // check busy
    if(MAC_CSR_CMD & MAC_CSR_BUSY)
    {
        hal_printf("Smsc9218_ReadMac: MAC is BUSY.\r\n");
        return dwReg;
    }

    // set MAC read command
    MAC_CSR_CMD = (usMacRegister & MAC_CSR_ADDR_MASK) | MAC_CSR_BUSY | MAC_CSR_REQ_READ;

    // wait for MAC no BUSY
    if(!Smsc9218_WaitMACNoBusy())
    {
        hal_printf("Smsc9218_ReadMac: MAC Read Error.\r\n");
    }
    else
    {
        // read MAC register
        dwReg = MAC_CSR_DATA;
    }

    return dwReg;
}

BOOL Smsc9218_WaitMACNoBusy()
{
    DWORD dwCnt;

    for(dwCnt = 0; dwCnt < 40; dwCnt++)
    {
        if(!(MAC_CSR_CMD & MAC_CSR_BUSY))
        {
            return TRUE;
        }
    }

    hal_printf("Smsc9218_WaitMACNoBusy: MAC BUSY.\r\n");

    return FALSE;
}

BOOL Smsc9218_WaitMIINoBusy()
{
    DWORD dwCnt;

    for(dwCnt = 0; dwCnt < 100; dwCnt++)
    {
        if(!(Smsc9218_ReadMac(MII_ACC) & MAC_PHY_MIIBZY))
        {
            return TRUE;
        }
    }

    hal_printf("Smsc9218_WaitMIINoBusy: PHY MII BUSY.\r\n");

    return FALSE;
}

void Smsc9218_EnableInterrupt(DWORD dwMask)
{
    DWORD dwReg;

    dwReg = INT_EN;
    dwReg |= dwMask | INTEN_PME_INT_EN;
    INT_EN = dwReg;
}

void Smsc9218_DisableInterrupt(DWORD dwMask)
{
    DWORD dwReg;

    dwReg = INT_EN;
    dwReg &= ~dwMask;
    INT_EN = dwReg;
}

DWORD Smsc9218_GetInterruptStatus()
{
    return INT_STS;
    //return 0xFFFFFFFF;
}

void Smsc9218_ClearInterruptStatus(DWORD dwMask)
{
    INT_STS = dwMask;
}

void Smsc9218_InitializeInterrupts(DWORD dwRegINTCFG)
{   
    INT_EN = 0;
    // hal_printf("\r\n INT_STS (Truoc): 0x%x", INT_STS);
    INT_STS = 0xFFFFFFFF;
    // hal_printf("\r\n INT_STS (Sau): 0x%x", INT_STS);
    dwRegINTCFG |= INTCFG_IRQ_EN;
    INT_CFG = dwRegINTCFG;
}

RTP_BOOL SH7264_SMSC_open(PIFACE pi)   
{
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

    int macLen = __min(g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264EMAC].macAddressLen, sizeof(sc->mac_address));

    if(macLen > 0)
    {
        memcpy(&sc->mac_address[0], &g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7264EMAC].macAddressBuffer[0], macLen);
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
    
    // hal_printf("\r\n INT_CFG : 0x%x", INT_CFG);
    // hal_printf("\r\n INT_EN : 0x%x", INT_EN);
    // hal_printf("\r\n INT_STS (before): 0x%x", int_sts);
    
    // a Frame is transmitted
    //if(int_sts & INTSTS_TSFL_INT)
    //{
        // hal_printf("\r\nTransmitted");
        // signal IP task that xmit has completed
//      rtp_net_invoke_output(iface, 1);
    //}

    // Frame(s) received
    if(int_sts & INTSTS_RSFL_INT)
    {
        // hal_printf("  I");
        Events_Set( SYSTEM_EVENT_FLAG_SOCKET ); 
        HandlerRxISR();
        rtp_thrd_interrupt_continuation(iface->ctrl.index);
        INT_STS = INTSTS_RSFL_INT;
    }
    
    // Clear interrupt status
    INT_STS = 0xFFFFFFFF;
    int_sts = INT_STS;
    // hal_printf("\r\n INT_STS (after): 0x%x", INT_STS);
}

void SH7264_SMSC_recv(PIFACE pi)
{
    //DCU           RxPacketArrayIndicate[MAX_RXPACKETS_IN_QUEUE];  // Packet array for indication
    DCU             pPacket;
    //DWORD             iy, numRxToIndicate;
    //DWORD         dwDeferedTxPktCount;
    
    GLOBAL_LOCK(encIrq);

    //numRxToIndicate = 0;
    
    // hal_printf("  R");
    
    while ((pPacket = GetPktFromFullArray()) != NULL)
    {
        // hal_printf("  H");
        rtp_net_invoke_input(pi, pPacket, pPacket->length);
        hal_printf("\r\nLength = %d\r\n\n", pPacket->length);
        
        /*unsigned char* recv_packet = (unsigned char*)DCUTODATA(pPacket);
        
        for(int i = 0; i < pPacket->length; i++)
        {
            hal_printf("%x  ", recv_packet[i]);
        }
                
        RxPacketArrayIndicate[numRxToIndicate] = pPacket;
        numRxToIndicate++;
        
        if (numRxToIndicate > (DWORD)MAX_RXPACKETS_IN_QUEUE)
        {
            break;
        }*/
    }

    // Indicate packets.
    /*
    if (numRxToIndicate)
    {       
        for (iy = 0; iy < numRxToIndicate; iy++)
        {
            PutPktToEmptyArray(RxPacketArrayIndicate[iy]);
        }
    }*/
}

void HandlerRxISR()
{       
    DWORD RxStatus, packet_len, dw;
    DWORD numPacketReceived;
    DWORD RxFFCountDW;
    DCU msg;
    unsigned char* recv_packet;
    unsigned long temp;
    unsigned long count;
    unsigned long len;
    int timeout;
    int i, tries = 0;

    dw = RX_FIFO_INF;
    numPacketReceived = (dw & 0x00FF0000) >> 16;

    while(numPacketReceived)
    {
        // Pop out the status DW.
        RxStatus = RX_STS_FIFO_PORT;
        packet_len = (RxStatus & 0x3FFF0000) >> 16;

        // If no error, make the transfer.
        // Should discard packet with length error
        if (!(RxStatus & RX_STS_ERROR) && 
            !(((RxStatus & RX_STS_LEN_ERROR) == RX_STS_LEN_ERROR) && 
              ((RxStatus & RX_STS_FRAME_TYPE) == 0)))
        {
            
            if ((msg = PutPktToFullArray()) != NULL)
            // {
                // register unsigned long *rxbpl    = (unsigned long *) RX_BUFFER;
                // unsigned char *rx_ptr            = (unsigned char *) RX_BUFFER;
                // int ndx;
                    
                // for (ndx = ((packet_len + 2 + 3) >> 2); ndx > 0; ndx--)
                // {
                    // temp = RX_DATA_FIFO_PORT;
                    // *rxbpl++ = BIT_EDIANDW(temp);
                // }
            //if ((msg = os_alloc_packet_input(packet_len, DRIVER_ALLOC)) != NULL)
            {
                // hal_printf("  A");
                // ReadFifo(RX_DATA_FIFO_PORT, 
                         // (DWORD *)(((DWORD)pucBufAddress) & ~3),   // There is 2-byte offset in buffer.
                         // (((packet_len + 2 + 3) >> 2)));

                unsigned long *rxbpl    = (unsigned long *) RX_BUFFER;
                unsigned char *rx_ptr   = (unsigned char *) RX_BUFFER;
                int ndx;
                    
                // Copy this packet to a NetRxPacket buffer
                for (ndx = ((packet_len + 2 + 3) >> 2); ndx > 0; ndx--)
                {
                    temp = RX_DATA_FIFO_PORT;
                    *rxbpl++ = BIT_EDIANDW(temp);
                }

                recv_packet = (unsigned char*)DCUTODATA(msg);
                msg->length = packet_len - 2;
                memcpy(recv_packet, rx_ptr + 2, packet_len - 2);
                
                // hal_printf("\r\n\n");
                // for(i = 0; i < packet_len; i++)
                // {
                    // hal_printf("  %x", recv_packet[i]);
                // }
                
                //rtp_net_invoke_input(pi, msg, msg->length);
                
                if (f100RxEnWorkaroundDone == 0)
                {
                    if (INT_STS & INTSTS_RXSTOP_INT)
                    {
                        UpdateFilterAndMacReg();
                        // Enable RX 
                        EnableMacRxEn();
                        // Wait for at least 64uSec after enabling MAC_CR_RXEN
                        ETHERNET_DELAY(64);
                        f100RxEnWorkaroundDone = 1;
                    }
                }               
            }
            else
            {
                // Out of buffer. Read off and discard the packet.
                RxFFCountDW = (packet_len + 2 + 3) >> 2;
                RxFastForward(RxFFCountDW);

                if (f100RxEnWorkaroundDone == 0)
                {
                    if (INT_STS & INTSTS_RXSTOP_INT)
                    {
                        UpdateFilterAndMacReg();
                        // Enable RX 
                        EnableMacRxEn();
                        // Wait for at least 64uSec after enabling MAC_CR_RXEN
                        ETHERNET_DELAY(64);
                        f100RxEnWorkaroundDone = 1;
                    }
                }
            }
        }
        else
        {
            //
            // Packet has error. Discard it and update stats.
            //
            RxFFCountDW = (packet_len + 2 + 3) >> 2;
            RxFastForward(RxFFCountDW);         
        }

        // Update numPacketReceived for loop.
        dw = RX_FIFO_INF;
        numPacketReceived = (dw & 0x00FF0000) >> 16;
    }
}

void RxFastForward(DWORD dwDWCount)
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

/*UINT16 Send_Packet(EmacTDescriptor *epTxTd, char* buf, UINT16 length)
{  
    //long i=0;
    UINT16 FrameLength = ((length <= EMAC_TX_UNITSIZE) ? length : EMAC_TX_UNITSIZE);

    if(epTxTd->status & ACT != 0)
        return 0;
    memcpy(epTxTd->TRBA, buf, FrameLength);
    epTxTd->TDRBL = FrameLength;

    return( FrameLength );
}*/

DWORD Smsc9218_GetTxStatusCount()
{
    DWORD dwReg;

    dwReg = TX_FIFO_INF;
    dwReg &= TXFIFO_TXSUSED_MASK;
    dwReg >>= 16;
    dwReg &= 0xFFFF;
    
    return dwReg;
}

BOOL sendToNet(unsigned char * txbp, int len)
{
    unsigned long tx_cmd_a, tx_cmd_b;
    int i, uiBufLen;
    unsigned long * txbpl = (unsigned long *)txbp;
    unsigned long temp;
    DWORD TxStatus;

    // tx_cmd_a = (((unsigned long)txbp & 0x3) << 16) | 0x00003000 | len;
    // tx_cmd_b = (((DWORD)len) << 16) | ((DWORD)len);//(lastTxTag << 16) | len;
    
    // TX_DATA_FIFO_PORT = tx_cmd_a;
    // TX_DATA_FIFO_PORT = tx_cmd_b;

    // for (i = (len+3)/sizeof(unsigned long); i > 0; i--)
    // {
        // temp = *txbpl++;
        // TX_DATA_FIFO_PORT = BIT_EDIANDW(temp);
    // }
    
    uiBufLen = (len + 3 + (((DWORD)txbpl) & 0x03)) >> 2;
    
    tx_cmd_a = (((unsigned long)txbpl & 0x3) << 16) | 0x00003000 | len;
    tx_cmd_b = (((DWORD)uiBufLen) << 16) | ((DWORD)uiBufLen);//(lastTxTag << 16) | len;

    // hal_printf("\r\nsendToNet: tx_cmd_a: 0x%0.8x tx_cmd_b: 0x%0.8x\r\n", tx_cmd_a, tx_cmd_b);

    TX_DATA_FIFO_PORT = tx_cmd_a;
    TX_DATA_FIFO_PORT = tx_cmd_b;

    for (i = uiBufLen; i > 0; i--)
    {
        temp = *txbpl++;
        TX_DATA_FIFO_PORT = BIT_EDIANDW(temp);
    }
    
    while (Smsc9218_GetTxStatusCount())
    {
        TxStatus = TX_STS_FIFO_PORT;
    }

    // TX_CFG = TXCFG_TX_ON;        // Enable transmitter
    return (TRUE);
}

BOOL Smsc9218_Xmit_Packet(unsigned char * txbp, int len)
{
    //PDWORD    pucBufAddress;
    //UINT  uiBufLen, uiPacketLength, uiBufCount, uiTemp;
    //volatile DWORD    dwTxCmdA, dwTxCmdB;
    //DWORD TxStatus;
    //BOOL  fFistNonZeroBuf = TRUE;
    //PNDIS_BUFFER pBuffer, pBufferStart;
    //const NDIS_BUFFER *pBufferLast = NULL;

    //
    // Transmit the buffers.
    //
    //dwTxCmdB = (((DWORD)len) << 16) | ((DWORD)len);

    //pBuffer = pBufferStart;
    
    if(sendToNet(txbp, len))
        return TRUE;
    return FALSE;
    

    //if (((uiBufCount*MAX_NUM_SEGMENTS) <= 86) || (uiBufCount <= 86))
    /*{
        DWORD  pkt_len = 0;
        PUCHAR pkt_buf = (PUCHAR)(pAdapter->TxTempPktBuf.pVAddr);

        do{
            NdisQueryBuffer (pBuffer, (PVOID*)&pucBufAddress, &uiBufLen);
            if (pBuffer && (uiBufLen == 0)){
                NdisGetNextBuffer (pBuffer, &pBuffer);
                continue;
            }

            if (pkt_len){
                const DWORD dwPktPtr = (DWORD)(pAdapter->TxTempPktBuf.pVAddr);

                // there is a buffer which is not sent last iteration
                NdisMoveMemory((PVOID)pkt_buf, (PVOID)pucBufAddress, uiBufLen);
                pkt_buf += uiBufLen;
                pkt_len += (DWORD)uiBufLen;

                if (pBuffer == pBufferLast){    //last non-zero buffer
                    dwTxCmdA=( ((dwPktPtr)<<16) |
                           TX_CMD_A_LAST_SEG | ((DWORD)pkt_len));

                    // Check if it is also the first buffer, i.e. single buffer packet.
                    if (fFistNonZeroBuf){   //first non-zero buffer
                        fFistNonZeroBuf = FALSE;
                        dwTxCmdA |= TX_CMD_A_FIRST_SEG;
                    }
                }
                else{
                    dwTxCmdA=(((dwPktPtr)<<16) | (pkt_len));

                    // Check if it is the first buffer.
                    if (fFistNonZeroBuf){   //first non-zero buffer
                        fFistNonZeroBuf = FALSE;
                        dwTxCmdA |= TX_CMD_A_FIRST_SEG;
                    }
                    else{
                        // Middle buffers should be 
                        // greater than or equal to 4bytes in length
                        if (pkt_len < 4){
                            // Do not transmit this buffer.
                            // This should be merged to next buffer
                            if (pBuffer){
                                NdisGetNextBuffer (pBuffer, &pBuffer);
                            }
                            continue;
                        }
                    }
                }

                AdapterSetCSR(TX_DATA_FIFO_PORT, dwTxCmdA);
                AdapterSetCSR(TX_DATA_FIFO_PORT, dwTxCmdB);
                Lan_WriteTxFifo(pAdapter,
                            (DWORD *)((dwPktPtr)&~0x03),
                            ((DWORD)pkt_len+3+(dwPktPtr&0x03))>>2);

                pkt_len = 0;
                pkt_buf = (PUCHAR)(pAdapter->TxTempPktBuf.pVAddr);
                if (pBuffer){
                    NdisGetNextBuffer (pBuffer, &pBuffer);
                }
                continue;
            }

            // Calculate dwTxCmdA
            if (pBuffer == pBufferLast){    //last non-zero buffer
                dwTxCmdA=( ((((DWORD)pucBufAddress)&0x03)<<16) | //DWORD alignment adjustment
                       TX_CMD_A_LAST_SEG | 
                       ((DWORD)uiBufLen));

                // Check if it is also the first buffer, i.e. single buffer packet.
                if (fFistNonZeroBuf){   //first non-zero buffer
                    fFistNonZeroBuf = FALSE;
                    dwTxCmdA |= TX_CMD_A_FIRST_SEG;
                }
            }
            else{
                dwTxCmdA=( ((((DWORD)pucBufAddress)&0x03)<<16) | //DWORD alignment adjustment
                       ((DWORD)uiBufLen) );

                // Check if it is the first buffer.
                if (fFistNonZeroBuf){   //first non-zero buffer
                    fFistNonZeroBuf = FALSE;
                    dwTxCmdA |= TX_CMD_A_FIRST_SEG;
                }
                else{
                    // Middle buffers should be 
                    // greater than or equal to 4bytes in length
                    if (uiBufLen < 4U)
                    {
                        NdisMoveMemory((PVOID)pkt_buf, (PVOID)pucBufAddress, uiBufLen);
                        pkt_buf += uiBufLen;
                        pkt_len += (DWORD)uiBufLen;
                        // Do not transmit this buffer.
                        // This should be merged to next buffer
                        if (pBuffer) 
                        {
                            NdisGetNextBuffer (pBuffer, &pBuffer);
                        }
                        continue;
                    }
                }
            }

            AdapterSetCSR(TX_DATA_FIFO_PORT, dwTxCmdA);
            AdapterSetCSR(TX_DATA_FIFO_PORT, dwTxCmdB);
            Lan_WriteTxFifo(pAdapter,
                        (DWORD *)(((DWORD)pucBufAddress)&~0x03),
                        ( (DWORD)uiBufLen+3+(((DWORD)pucBufAddress)&0x03) )>>2);

            if (pBuffer) {
                NdisGetNextBuffer (pBuffer, &pBuffer);
            }
            else {
            }

        } while (pBuffer);
    }
    /*else
    {
        // Packet is too fragmented. 
        // Copy the buffers to a driver buffer and xfer using the driver buffer.
        DWORD  pkt_len = 0;
        PUCHAR pkt_buf = (PUCHAR)(pAdapter->TxPktBuffer);
        do
        {
            NdisQueryBuffer (pBuffer, (PVOID*)&pucBufAddress, &uiBufLen);

            while (pBuffer && (uiBufLen == 0)){
                NdisGetNextBuffer (pBuffer, &pBuffer);
                NdisQueryBuffer (pBuffer, (PVOID*)&pucBufAddress, &uiBufLen);
            }

            NdisMoveMemory((PVOID)pkt_buf, (PVOID)pucBufAddress, uiBufLen);
            pkt_buf += uiBufLen;
            pkt_len += (DWORD)uiBufLen;

            if (pBuffer) {
                NdisGetNextBuffer(pBuffer, &pBuffer);
            }
        } while (pBuffer);

        if (pkt_len != (DWORD)uiPacketLength)
            RETAILMSG(1, (TEXT("pkt_len = %d, uiPacketLength = %d\r\n"), pkt_len, uiPacketLength));
        
        // Send it using PIO Mode

        // Calculate dwTxCmdA
        dwTxCmdA=( ((((DWORD)(pAdapter->TxPktBuffer))&0x03)<<16) | //DWORD alignment adjustment
                   TX_CMD_A_LAST_SEG | TX_CMD_A_FIRST_SEG |
                   (pkt_len) );

        AdapterSetCSR(TX_DATA_FIFO_PORT, dwTxCmdA);
        AdapterSetCSR(TX_DATA_FIFO_PORT, dwTxCmdB);
        Lan_WriteTxFifo(pAdapter,
                        (DWORD *)(((DWORD)(pAdapter->TxPktBuffer))&~0x03),
                        (pkt_len+3+(((DWORD)(pAdapter->TxPktBuffer))&0x03) )>>2);
    }*/

    // while (Smsc9218_GetTxStatusCount(pAdapter))
    // {
        // TxStatus = TX_STS_FIFO_PORT;
    // }

    // return TRUE;
}

int SH7264_SMSC_xmit(PIFACE pi, DCU msg)
{
    DWORD   TxDataFreeSpace, TxSpaceNeeded;
    BOOL    status = TRUE;
    
    RTP_UINT16 length = 0;
    unsigned char* send_packet;
    
    GLOBAL_LOCK(encIrq);
    
    // hal_printf("\r\n+SH7264_SMSC_xmit");

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

    if (length > (ETHERSIZE+4))
    {
        RTP_DEBUG_ERROR("xmit - length is too large, truncated", NOVAR, 0, 0);
        length = ETHERSIZE + 4;
    }

    send_packet = (unsigned char*)DCUTODATA(msg);
    
    // Read the actual data.
    TxDataFreeSpace = TX_FIFO_INF & TXFIFO_TDFREE_MASK;
    
    // if (BufferCount > 30), TxSpaceNeeded will be larger than
    // max available TX FIFO.
    // If then, Smsc9218XmitOnePacket() will handle the packet
    //  as PIO mode.
    // We can check as PIO mode here
    // TxSpaceNeeded = (DWORD)((packet_len + 3) & (~0x03)) + (BufferCount << 4);

    //if (TxSpaceNeeded < TxDataFreeSpace)
    {
        if(!Smsc9218_Xmit_Packet(send_packet, length))
            return -1;
    }
    
    // hal_printf("\r\n-SH7264_SMSC_xmit");
    
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
    UINT32 errCount = 0;

    //Setting port
    PECR0 |= 0x0030;
    PJCR0 |= 0x3000;
        
    //Enable interrupt
    ICR1 = 0;
    IPR01 |= 0x0D00;
    
    if(!SH7264_SMSC_Init())
        return RTP_FALSE;

    hal_printf("Waiting for getting IP address...\r\n");

    return RTP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//BOOL SH7264_SMSC_AutoNegotiate();
RTP_BOOL SH7264_SMSC_Init()
{
    WORD wd;
    
    hal_printf("+SH7264_SMSC_Init\r\n");
    
    if (!CardInitialize())
    {
        return RTP_FALSE;
    }

    Smsc9218_WritePhy(PHY_INTERRUPTMASK, Smsc9218_ReadPhy(PHY_INTERRUPTMASK) & ~0x0080);
        
    //Clear interrupts
    wd = Smsc9218_ReadPhy(PHY_INTERRUPTSRC);
        
    // Put PHY out from EnergyOn mode
    Smsc9218_WritePhy(PHY_MODE_CTLSTS, Smsc9218_ReadPhy(PHY_MODE_CTLSTS) & ~0x2000);

    // reset 9218 and restore packet filter & multicast list
    // Disable Global Interrupt
    INT_CFG = INT_CFG & ~INTCFG_IRQ_EN;

    // Clear Interrupt Status
    INT_STS = 0xFFFFFFFF;
    // Enable Global Interrupt
    INT_CFG |= INTCFG_IRQ_EN;
    
    // Disable PME interrupt & clear pending status
    /*Smsc9218_DisableInterrupt(INTEN_PME_INT_EN);
    INT_STS = INTSTS_PME_INT;

    // Clear pending status & Enable GPT interrupt
    INT_STS = INTSTS_GPT_INT;
    Smsc9218_EnableInterrupt(INTEN_GPT_INT_EN);*/
    
    if (CardSetup())
    {
        CardStart();
    }
    else
    {
        return RTP_FALSE;
    }
    
    //
    // Initialize queues
    //
    if (!InitializeQueues())
    {
        return RTP_FALSE;
    }
    
    DispatchSetPacketFilter();
    DispatchSetMulticastAddressList();
    
    hal_printf("-SH7264_SMSC_Init\r\n");
    
    return RTP_TRUE;
}

BOOL InitializeQueues()
{
    unsigned int cnt;
    DCU msg;
    // Initialize Tx queue
    // QUEUE_INIT(&(pAdapter->TxRdyToComplete));
    // QUEUE_INIT(&(pAdapter->TxWaitToSend));
    // QUEUE_INIT(&(pAdapter->TxDeferedPkt));

    // Initialize Rx queue
    // NdisAllocatePacketPool(&Status, &(pAdapter->hPacketPool), MAX_RXPACKETS_IN_QUEUE, PROTOCOL_RESERVED_LENGTH);
    // if (Status != NDIS_STATUS_SUCCESS){
        // return (Status);
    // }

    // for (cnt=0U; cnt<MAX_RXPACKETS_IN_QUEUE; cnt++){
        // NdisAllocatePacket(&Status, &(pAdapter->RxPacketArray[cnt]), pAdapter->hPacketPool);
        // if (Status != NDIS_STATUS_SUCCESS){
            // return (Status);
        // }
    // }

    // NdisAllocateBufferPool(&Status, &(pAdapter->hBufferPool), MAX_RXPACKETS_IN_QUEUE);
    // if (Status != NDIS_STATUS_SUCCESS){
        // return (Status);
    // }

    // pSharedMem = (PSMSC9218_SHAREDMEM)(pAdapter->pSharedMemVA);
    // for (cnt=0U; cnt<MAX_RXPACKETS_IN_QUEUE; cnt++){
        // NdisAllocateBuffer(&Status, 
                           // &(pAdapter->RxBufferArray[cnt]), 
                           // pAdapter->hBufferPool,
                           // (PVOID)((PUCHAR)(pSharedMem->RxBuffer[cnt])+2),   // PIO with 2-byte offset
                           // MAX_PACKET);
        // if (Status != NDIS_STATUS_SUCCESS){
            // return (Status);
        // }
    // }

    // for (cnt=0U; cnt<MAX_RXPACKETS_IN_QUEUE; cnt++)
    // {
        // NdisChainBufferAtBack((pAdapter->RxPacketArray[cnt]), (pAdapter->RxBufferArray[cnt]));
        // {
            // UINT ArraySize;
            // NDIS_PHYSICAL_ADDRESS_UNIT SegmentArray[MAX_NUM_SEGMENTS];

            // NdisMStartBufferPhysicalMapping(pAdapter->hMiniportAdapterHandle,
                    // pAdapter->RxBufferArray[cnt],
                    // 0, (BOOLEAN)TRUE, SegmentArray, &ArraySize);
            // if (ArraySize != 1U){
                // RETAILMSG(1, (TEXT("InitializeQueues: Error! ArraySize = %d\r\n"), ArraySize));
            // }
            // SET_PACKET_RESERVED_PA(pAdapter->RxPacketArray[cnt], 
                    // NdisGetPhysicalAddressLow(SegmentArray[0].PhysicalAddress));
        // }
    // }

    // Initialize EmptyPktArray
    EmptyPkt.dwRdPtr = 0;
    EmptyPkt.dwWrPtr = 0;
    for (cnt = 0; cnt < MAX_RXPACKETS_IN_QUEUE; cnt++)
    {       
        while (1)
        {
            msg = os_alloc_packet_input(512, DRIVER_ALLOC);
            if (msg)
            {
                break;
            }
        }
        EmptyPkt.dwPktArray[cnt] = msg;
    }
    EmptyPkt.dwRdPtr = 0;
    EmptyPkt.dwWrPtr = EmptyPkt.dwRdPtr + (DWORD)MAX_RXPACKETS_IN_QUEUE;

    // Initialize FullPktArray
    FullPkt.dwRdPtr = FullPkt.dwWrPtr = 0;

    // pAdapter->TxDPCNeeded = FALSE;
    // pAdapter->RxDPCNeeded = FALSE;
    // pAdapter->PhyDPCNeeded = FALSE;
    // pAdapter->SWDPCNeeded = FALSE;
    // pAdapter->RxOverRun = FALSE;
    
    f100RxEnWorkaroundDone = 1;

    return TRUE;
}

DCU GetPktFromEmptyArray(DCU    pPkt)
{
    //DCU   pPkt;

    if (!IS_ARRAY_EMPTY(EmptyPkt))
    {
        //pPkt = EmptyPkt.dwPktArray[EmptyPkt.dwRdPtr];
        //memcpy((char *)pPkt,(char *)EmptyPkt.dwPktArray[EmptyPkt.dwWrPtr], sizeof(DCU))
        INC_PTR(EmptyPkt.dwRdPtr);
        return pPkt;
    }
    else
    {
        return 0;
    }
}

DCU GetPktFromFullArray()
{
    //DCU   pPkt;
    unsigned int rpt;
    if (!IS_ARRAY_EMPTY(FullPkt))
    {
        //pPkt = FullPkt.dwPktArray[FullPkt.dwRdPtr];
        //memcopy((char *)pPkt,(char *)FullPkt.dwPktArray[FullPkt.dwWrPtr], sizeof(DCU))
        rpt = FullPkt.dwRdPtr;
        INC_PTR(FullPkt.dwRdPtr);
        return FullPkt.dwPktArray[rpt];
    }
    else
    {
        return (0);
    }
}

BOOL PutPktToEmptyArray(DCU pPkt)
{
    if (!IS_ARRAY_FULL(EmptyPkt))
    {
        //EmptyPkt.dwPktArray[EmptyPkt.dwWrPtr] = pPkt;
        //memcpy((char *)EmptyPkt.dwPktArray[EmptyPkt.dwWrPtr],(char *)pPkt, sizeof(DCU))
        INC_PTR(EmptyPkt.dwWrPtr);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

DCU PutPktToFullArray()
{
    unsigned int wpt;
    
    if (!IS_ARRAY_FULL(FullPkt))
    {
        //FullPkt.dwPktArray[FullPkt.dwWrPtr] = pPkt;
        //memcopy((char *)FullPkt.dwPktArray[FullPkt.dwWrPtr],(char *)pPkt, sizeof(DCU));
        wpt = FullPkt.dwWrPtr;
        INC_PTR(FullPkt.dwWrPtr);
        return FullPkt.dwPktArray[wpt];
    }
    else
    {
        return 0;
    }
}

void DispatchSetPacketFilter()
{
    DWORD   dwReg;
    
    if (dwLinkMode == LINK_10MPS_HALF)
    {
        DisableMacRxEn();
        Smsc9218SetMacFilter();
        EnableMacRxEn();
    }
    else if (dwLinkMode == LINK_10MPS_FULL)
    {
        // 10/FD
        // Disable RX 
        dwReg = Smsc9218_ReadMac(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            INT_STS = INTSTS_RXSTOP_INT;
            // Enable
            Smsc9218_EnableInterrupt(INTEN_RXSTOP_INT_EN);
            dwReg = Smsc9218_ReadMac(MAC_CR);
            dwReg &= ~MAC_RXEN;
            Smsc9218_WriteMac(MAC_CR, dwReg);
            return;
        }
        else
        {
            Smsc9218SetMacFilter();
            EnableMacRxEn();
        }
    }
    else if ((dwLinkMode == LINK_100MPS_FULL) || (dwLinkMode == LINK_100MPS_HALF))
    {
        dwReg = Smsc9218_ReadMac(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            // 100/FD/HD
            f100RxEnWorkaroundDone = 0;
            // Initiate SWINT
            Smsc9218_EnableInterrupt(INTEN_SW_INT_EN);
        }
        else
        {
            UpdateFilterAndMacReg();
            // Enable RX 
            EnableMacRxEn();
        }
    }
}

void DispatchSetMulticastAddressList()
{
    DWORD   dwReg;
    
    if (dwLinkMode == LINK_10MPS_HALF)
    {
        DisableMacRxEn();
        // Smsc9218_WriteMac(HASHL, pAdapter->ucNicMulticastRegs[0]);
        // Smsc9218_WriteMac(HASHH, pAdapter->ucNicMulticastRegs[1]);
        Smsc9218_WriteMac(HASHL, 0);
        Smsc9218_WriteMac(HASHH, 0);
        EnableMacRxEn();
    }
    else if (dwLinkMode == LINK_10MPS_FULL)
    {
        // 10/FD
        dwReg = Smsc9218_ReadMac(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            INT_STS = INTSTS_RXSTOP_INT;
            // Enable Interrupt
            Smsc9218_EnableInterrupt(INTEN_RXSTOP_INT_EN);

            dwReg = Smsc9218_ReadMac(MAC_CR);
            dwReg &= ~MAC_RXEN;
            Smsc9218_WriteMac(MAC_CR, dwReg);
        }
        else
        {
            // Smsc9218_WriteMac(HASHL, pAdapter->ucNicMulticastRegs[0]);
            // Smsc9218_WriteMac(HASHH, pAdapter->ucNicMulticastRegs[1]);
            Smsc9218_WriteMac(HASHL, 0);
            Smsc9218_WriteMac(HASHH, 0);

            // Enable RX_EN
            dwReg = Smsc9218_ReadMac(MAC_CR);
            dwReg |= MAC_RXEN;
            Smsc9218_WriteMac(MAC_CR, dwReg);
        }
    }
    else if ((dwLinkMode == LINK_100MPS_FULL) || (dwLinkMode == LINK_100MPS_HALF))
    {
        dwReg = Smsc9218_ReadMac(MAC_CR);
        if (dwReg & MAC_RXEN)
        {
            // 100/FD/HD
            f100RxEnWorkaroundDone = 0;         
            // Initiate SWINT Interrupt 
            Smsc9218_EnableInterrupt(INTEN_SW_INT_EN);
        }
        else
        {
            UpdateFilterAndMacReg();
            // Enable RX 
            EnableMacRxEn();
        }
    }
}

void EnableMacRxEn()
{
    volatile DWORD  dwReg;
    
    // Enable RX 
    dwReg = Smsc9218_ReadMac(MAC_CR);
    dwReg |= MAC_RXEN;
    Smsc9218_WriteMac(MAC_CR, dwReg);

    // Wait for at least 64uSec
    ETHERNET_DELAY(64);
}

void DisableMacRxEn()
{
    volatile DWORD  dwReg;
    volatile long   counter;
    
    dwReg = Smsc9218_ReadMac(MAC_CR);
    if (dwReg & MAC_RXEN)
    {
        INT_STS = INTSTS_RXSTOP_INT;

        dwReg = Smsc9218_ReadMac(MAC_CR);
        dwReg &= ~MAC_RXEN; //disable RX
        Smsc9218_WriteMac(MAC_CR, dwReg);

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

void Smsc9218SetMacFilter()
{
    DWORD   dwReg;
    
    dwReg = Smsc9218_ReadMac(MAC_CR);
    
    dwReg &= (~(MAC_MCPAS | MAC_PRMS | MAC_INVFILT | MAC_HO | MAC_HPFILT | MAC_BCAST));
    // if (pAdapter->ulPacketFilter & (DWORD)NDIS_PACKET_TYPE_ALL_MULTICAST)
    {
        dwReg |= MAC_MCPAS;
    }
    // if (pAdapter->ulPacketFilter & (DWORD)NDIS_PACKET_TYPE_PROMISCUOUS)
    {
        dwReg |= MAC_PRMS;
    }
    // if (pAdapter->ulPacketFilter & (DWORD)NDIS_PACKET_TYPE_MULTICAST)
    // {
        // dwReg |= MAC_HPFILT;
    // }
    Smsc9218_WriteMac(MAC_CR, dwReg);
}

void UpdateFilterAndMacReg()
{
    // Smsc9218_WriteMac(HASHL, pAdapter->ucNicMulticastRegs[0]);
    // Smsc9218_WriteMac(HASHH, pAdapter->ucNicMulticastRegs[1]);
    Smsc9218_WriteMac(HASHL, 0);
    Smsc9218_WriteMac(HASHH, 0);
    Smsc9218SetMacFilter();
}

void lan9218_udelay(unsigned long delta)    // Arg is really microseconds
{
    const unsigned long start = FREE_RUN;   // Start timing
    const unsigned long usec = delta * (25000000/1000000);

    // usec adjusted for 25MHz on-chip clock, 1 microsecond (1/1000000) scaling
    do {
        delta = FREE_RUN;
        if (delta >= start)
        delta = (delta - start);
        else
            delta = (delta - start) + 1;    // use 0x100000000, not 0xffffffff
    } while (delta < usec);
}

int MacBusy(int ReqTO)
{
    int timeout = ReqTO;
    int RetVal = FALSE;   // No timeout

    while (timeout--) 
    {
        if (!(MAC_CSR_CMD & MAC_CSR_BUSY)) 
        {
          return (RetVal);
        }
    }
    RetVal = TRUE;        // Timeout
    return (RetVal);
}

unsigned long GetMacReg(unsigned long Reg)
{
    unsigned long RegVal = 0xffffffff;

    if (MAC_CSR_CMD & MAC_CSR_BUSY)
    {
        hal_printf("\r\nGetMacReg: previous command not complete");
        return (RegVal);
    }

    MAC_CSR_CMD = MAC_RD_CMD(Reg);
    ETHERNET_DELAY(1);

    if (MacBusy(MAC_TIMEOUT) == TRUE) 
    {
        hal_printf("\r\nGetMacReg: timeout waiting for response from MAC");
        return (RegVal);
    }

    RegVal = MAC_CSR_DATA;

    return (RegVal);    
}

int SetMacReg(unsigned long Reg, unsigned long Value)
{
    int RetVal = FALSE;

    if (MAC_CSR_CMD & MAC_CSR_BUSY) 
    {
        hal_printf("\r\nSetMacReg: previous command not complete");
        return (RetVal);
    }

    MAC_CSR_DATA = Value;
    ETHERNET_DELAY(1);
    MAC_CSR_CMD = MAC_WR_CMD(Reg);
    ETHERNET_DELAY(1);

    if (MacBusy(MAC_TIMEOUT) == TRUE) 
    {
        hal_printf("\r\nSetMacReg: timeout waiting for response from MAC");
        return (RetVal);
    }
    
    RetVal = TRUE;
    return (RetVal);
}

int PhyBusy(int ReqTO)
{
    int timeout = ReqTO;
    int RetVal = FALSE;   // No timeout

    while (timeout--) 
    {
        if (!(GetMacReg(MII_ACC) & MAC_PHY_MIIBZY)) 
        {
            return (RetVal);
        }
    }

    RetVal = TRUE;        // Timeout
    return (RetVal);
}

unsigned short GetPhyReg(unsigned char Reg)
{
    unsigned short RegVal = 0xffff;

    if (GetMacReg(MII_ACC) & MAC_PHY_MIIBZY) 
    {
        hal_printf("\r\nGetPhyReg: MII busy");
        RegVal = 0;
        return (RegVal);
    }

    SetMacReg(MII_ACC, MAC_MII_RD_CMD((unsigned char)PHY_ADDR, Reg));
    ETHERNET_DELAY(1);

    if (PhyBusy(PHY_TIMEOUT) == TRUE) 
    {
        hal_printf("GetPhyReg: timeout waiting for MII command\r\n");
        return (RegVal);
    }

    RegVal = (unsigned short)GetMacReg(MII_DATA);
    return (RegVal);
}

int SetPhyReg(unsigned char Reg, unsigned short Value)
{
    int RetVal = FALSE;

    if (GetMacReg(MII_ACC) & MAC_PHY_MIIBZY) 
    {
        hal_printf("\r\nSetPhyReg: MII busy\r\n");
        return (RetVal);
    }

    SetMacReg(MII_DATA, Value);
    ETHERNET_DELAY(1);
    SetMacReg(MII_ACC, MAC_MII_WR_CMD((unsigned char)PHY_ADDR, Reg));
    ETHERNET_DELAY(1);

    if (PhyBusy(PHY_TIMEOUT) == TRUE) 
    {
        hal_printf("\r\nSetPhyReg: timeout waiting for MII command");
        return (RetVal);
    }
    RetVal = TRUE;
    return (RetVal);
}

/*void Init_Malloc()
{
    #define FIXED_HEAP 0x00600010   

    HEAP_TOP = (unsigned long)(CS3_TOP+FIXED_HEAP);  // CS3-SDRAM area
}

int* Malloc_Buf(int size)
{
    int *ptr;

    if((HEAP_TOP&0x0000000F) != 0x00000000) {
        // Aligns 16bytes boundary
        HEAP_TOP = (HEAP_TOP & 0xFFFFFFF0) + 0x00000010;
    }
    ptr = (int *)HEAP_TOP;
    HEAP_TOP = HEAP_TOP + (unsigned long)size;
    if((HEAP_TOP & 0x00000003) != 0x00000000) {
        // Aligns longword boundary
        HEAP_TOP = (HEAP_TOP & 0xFFFFFFFC) + 0x00000004;
    }

    return (ptr);
}

void Flush_All(PIFACE pi)
{
    UINT32 Index;
     
    ECMR0   = 0x00000000;
    EDRRR0  = 0x00000000;
    RMCR0   = 0x00000000;
    
    SH7264_SMSC_recv(pi);
    
    ECSR0   = 0x00000007;
    EESR0   = 0x47FF0F9F;
    TROCR0  = 0x00000000;
    CDCR0   = 0x00000000;
    LCCR0   = 0x00000000;
    CNDCR0  = 0x00000000;
    CEFCR0  = 0x00000000;
    FRECR0  = 0x00000000;
    TSFRCR0 = 0x00000000;
    TLFRCR0 = 0x00000000;
    RFCR0   = 0x00000000;
    MAFCR0  = 0x00000000;
    RMFCR0  = 0x00000000;
    
    // Setup the RX descriptors
    for(Index = 0; Index < RX_BUFFERS; Index++) 
    {
        rxTd.td[Index]->status = ACT;
        rxTd.td[Index]->TDRBL = EMAC_RX_UNITSIZE;
        rxTd.td[Index]->RDL = 0;        
    }
    rxTd.td[RX_BUFFERS - 1]->status |= DL;

    // Setup the TX descriptors
    for(Index = 0; Index < TX_BUFFERS; Index++) 
    {
        txTd.td[Index]->status = 0;
        txTd.td[Index]->TDRBL = 0;
        txTd.td[Index]->RDL = 0;        
    }
    txTd.td[TX_BUFFERS - 1]->status |= DL;

    RMCR0   = 0x00000001;
    //ECMR0   = 0x00030062;
    ECMR0   = 0x00070062;
    EDRRR0  = 0x00000001;
}

unsigned short Ether_Reg_Read( unsigned short reg_addr )
{
    volatile unsigned short data;
    data = PHYAccess( 0, PHY_ADDR, reg_addr, OPREAD, 0);

    return( data );
}

void Ether_Reg_Write( unsigned short reg_addr, unsigned short data )
{
    PHYAccess( 0, PHY_ADDR, reg_addr, OPWRITE, data);
}

vuint16_t PHYAccess(vuint8_t channel, vuint8_t PHYAdd, vuint8_t RegAdd, vuint8_t OPCode, vuint16_t wData)
{
    // Local variables
    int i;
    vuint16_t MGMTval, mdio;

    // Filter unused bits from input variables.
    PHYAdd &= 0x1F;
    RegAdd &= 0x1F;
    OPCode &= 0x03;
    
    // Output Preamble (32 '1's)
    for (i = 0; i < 32; i++)
        Write_MDO(channel, 1);

    // Output Start of Frame ('01')
    for (i = 0; i < 2; i++)
        Write_MDO(channel, i);

    // Output OPCode ('01' for write or '10' for Read)
    for (i = 1; i >= 0; i--)
        Write_MDO(channel, ((OPCode>>i) & 0x01));

    // Output PHY Address
    for (i = 4; i >= 0; i--)
        Write_MDO(channel, ((PHYAdd>>i) & 0x01));

    // Output Register Address
    for (i = 4; i >= 0; i--)
        Write_MDO(channel, ((RegAdd>>i) & 0x01));

    if (OPCode == OPREAD)
    {
        // Read Operation
        // Implement Turnaround ('Z0')  - not sure if needed...
        Write_MDO(channel, 0);

        // Read Data
        wData = 0;
        for (i = 15; i >= 0; i--)
            wData |= (Read_MDI(channel, 0) << i);

        // produce 1 clk idle state
        Read_MDI(channel, 0);

        return (wData);
    }
    else
    {
        // Write Operation
        // Implement Turnaround ('10')  - not sure if needed...
        for (i = 1; i >= 0; i--)
            Write_MDO(channel, ((2>>i) & 0x01));

        // Write Data
        for (i = 15; i >= 0; i--)
            Write_MDO(channel, ((wData>>i) & 0x01));

        // produce 1 clk idle state
        Read_MDI(channel, 0);

        return (1);
    }
}

void Write_MDO(vuint8_t channel, unsigned MGMTData)
{
    volatile int i   ;
    vuint32_t * pir_ptr;
    vuint32_t data = (MGMTData << ETHERC_MDO_SLB) & ETHERC_MDO_WRITE; // Shift data and make sure only 1 bit is set

    pir_ptr = (vuint32_t *) (&PIR0);

    // Clear PHY Interface Register
    *pir_ptr = 0;

    // Setup data time
    *pir_ptr = data | ETHERC_MMD;

    // 5 * clkfreq of CPU
    for (i = 0; i < 5; i ++)
    {
    }

    // Send Data
    *pir_ptr = data | ETHERC_MMD | ETHERC_MDC;

    // 5 * clkfreq of CPU
    for (i = 0; i < 5; i ++)
    {
    }

    // Hold Data Time
    *pir_ptr = data|ETHERC_MMD;
}

vuint32_t Read_MDI(vuint8_t channel, unsigned MGMTData)
{
    volatile int i   ;
    vuint32_t * pir_ptr;
    vuint32_t data = 0;

    pir_ptr = (vuint32_t *)(&PIR0);

    // Clear PHY Interface Register
    *pir_ptr = 0;

    *pir_ptr = ETHERC_MDC; 
     
    // Setup data time
    *pir_ptr = data;

    // 5 * clkfreq of CPU
    for (i = 0; i < 5; i ++)
    {
    }

    // Read Data
    data = *pir_ptr;                                           
    data &= ETHERC_MDI;         // Filter out all but input data
    data >>= ETHERC_MDI_SRB;    // shift data to the right
    *pir_ptr = 0x00000000;      // Clear PIR

    return data;
}

BOOL SH7264_SMSC_GetLinkSpeed(BOOL applySetting)
{
    BOOL rc = TRUE;

    return rc;
}

int get_initial_phymd(void)
{
    int mode;
    unsigned short sw3;

    PCCRH2 &= 0xFF00;
    PCIORH &= 0xfff0;

    sw3 = (~PCDRH) & 0x0007u;
    switch( sw3 ){
    case 0x0000:
        mode = HALF_10T_SET;
        break;
    case 0x0001:
        mode = HALF_100TX_SET;
        break;
    case 0x0002:
        mode = FULL_10T_SET;
        break;
    case 0x0003:
        mode = FULL_100TX_SET;
        break;
    case 0x0004:
    case 0x0005:
        mode = HALF_100TX_AUTONEGO_SET;
        break;
    case 0x0006:
    case 0x0007:
        mode = FULL_100TX_AUTONEGO_SET;
        break;
    default:
        mode = POWERDOWN_SET;
        break;
    }
    return mode;
}

int phy_read_linkmode( void )
{
    unsigned short reg0,reg4,reg5;
    unsigned short w16;
    int link = NEGO_FAIL;
    int my_able, pt_able;

    reg0 = Ether_Reg_Read(BASIC_MODE_CONTROL_REG);
    reg4 = Ether_Reg_Read(AN_ADVERTISEMENT_REG);
    reg5 = Ether_Reg_Read(AN_LINK_PARTNER_ABILITY_REG);

    if( !(reg0 & 0x1000) ) {
        w16 = reg0 & 0x2100;
        if( w16 == 0x2100 ) {
            link = FULL_TX;
        }
        else if( w16 == 0x2000 ) {
            link = HALF_TX;
        }
        else if( w16 == 0x0100 ) {
            link = FULL_10M;
        }
        else {
            link = HALF_10M;
        }
    }
    else {
        if( reg4 & 0x0100 ) {
            my_able = FULL_TX;
        }
        else if( reg4 & 0x0080 ) {
            my_able = HALF_TX;
        }
        else if( reg4 & 0x0040 ) {
            my_able = FULL_10M;
        }
        else {
            my_able = HALF_10M;
        }

        if( reg5 & 0x0100 ) {
            pt_able = FULL_TX;
        }
        else if( reg5 & 0x0080 ) {
            pt_able = HALF_TX;
        }
        else if( reg5 & 0x0040 ) {
            pt_able = FULL_10M;
        }
        else if( reg5 & 0x0020 ) {
            pt_able = HALF_10M;
        }
        else {
            pt_able = NEGO_FAIL;
        }

        link = my_able;
        if( my_able > pt_able )
        {
            link = pt_able;
        }
    }
    return link;
}

int PHY_Init(int mode)
{
    long i;
    unsigned short PHYIFCR_MODE;
    unsigned short  data;
    int tries;

    hal_printf("PHY_Init.\r\n"); 

    STBCR4 = 0x00; // Enable phyif

    tries = 0;
    while (PHYSR == 0x8000)
    {
        if(tries >= 500)
            break;
        for(i = 0; i < 500000; i++);
        tries++;
    }

    if(tries >= 500)
    {
        hal_printf("PHY Init Failed\r\n");
        return 0;
    }

    PCCRH2 = 0x0000; // MII inhibited
    PCCRL1 = 0x0000;
    PCCRL2 = 0xFF00; // _DUPLEX,_COL,_LINK,_SPED10

    PHYSMIR2 = 0x0000;
    PHYSMIR3 = 0x0000;
    PHYADDR = PHY_ADDR; // 16 bits

    PHYIFCR_MODE = 0xA007 | mode; //0xA007; // External Clock        
    //  PHYIFCR_MODE = WR_PHYIFCR+CLKSEL_I+MII_MODE_00+CO_ST_MODE_111;
    PHYCR = PHYIFCR_MODE; // - bww
    //REG_PHYIF_PHYIFCR = 0xA007;

    // Wait 100us before negating the phy module reset.
    // With assumption of wait loop takes 4cyc, 12500/4=3125 loops,
    // 100us = ca. 12500 Icyc@Iclk=125MHz.
    for (i = 1; i <= 3125; i++)
    {
    }

    PHYCR = PHYIFCR_MODE + 0x4000;

    // wait 20ms after module reset.
    // With assumption of wait loop takes 4cyc, 2,500,000/4=625000 loops,
    // 20ms = ca. 2,500,000 Icyc@Iclk=125MHz.
    for (i = 1; i <= 625000; i++)
    {
    }

    Ether_Reg_Write( BASIC_MODE_CONTROL_REG, 0x1000 );

    for (i = 1; i <= 10000; i++)
    {
    }
    data = 0x0000;
    tries = 0;
    while ((data & 0x0020) != 0x0020)
    {
        if(tries >= 500)
            break;
        for(i = 0; i < 500000; i++);        
        data = Ether_Reg_Read( BASIC_MODE_STATUS_REG );
        // 0x0020 : Auto-negotiation complete.      
        tries++;
    }

    if(tries >= 500)
    {
        hal_printf("PHY Init Failed\r\n");
        return 0;
    }
    hal_printf("PHY_Init...Done!\r\n"); 
    return 1;
}

void PHY_Initialize()
{   
    volatile int w,cnt = 0;
    int mode = 0;
    int volatile link, pre_link = -1;
    
    mode =  get_initial_phymd();
    
    PHY_Init(mode);
    
    do {
        for(w = LOOP_100us*10; w > 0; w--) {
            // 1ms wait
        }
        
        link = phy_read_linkmode();
        if( link == pre_link ) {
            cnt++;
        }
        else {
            pre_link = link;
            cnt = 0;
        }
    }while( cnt < 500 );
    
    if( link == FULL_TX || link == FULL_10M ) {     
        ECMR0 |= (1 << 1);  // DM = 1
    }
    else {
        ECMR0 &= ~(1 << 1);  // DM = 0
    }
}

UINT32 SH7264_SMSC_FindValidPhy()
{
    UINT32 rc = 0xFF;
    unsigned short value;

    for (int i = 0; i < 17; i++)
    {
        value = Ether_Reg_Read(phy_addr[i]);
        if (value != 0x0000 && value != 0xFFFF)
        {
            rc = value;
            break;
        }
    }

    if (rc != 0xFF)
    {
        hal_printf( "Valid PHY Found: %d\r\n", rc);
    }

    return rc;
}

BOOL SH7264_SMSC_AutoNegotiate()
{
    BOOL rc = TRUE;

    PHY_Initialize();
    hal_printf("SH7264_SMSC_AutoNegotiate \r\n");
    hal_printf("AutoNegotiate complete\r\n");

    return rc;
}
*/