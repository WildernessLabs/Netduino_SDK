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
#include "AT91_EMAC.h"
#include "dm9161.h"

//--//
/* ********************************************************************
   DEBUG AIDS
   ******************************************************************** */
#define DEBUG_AT91EMAC 0

#ifndef NETWORK_INTERFACE_INDEX_AT91EMAC
#define NETWORK_INTERFACE_INDEX_AT91EMAC 0
#endif

extern "C"
{
    extern void rtp_thrd_interrupt_continuation(int);
    extern void rtp_thrd_ip_continuation(int);
}   /* extern "C" */

RTP_BOOL    AT91_EMAC_open       (PIFACE pi);
void        AT91_EMAC_close      (PIFACE pi);
int         AT91_EMAC_xmit       (PIFACE pi, DCU msg);
RTP_BOOL    AT91_EMAC_xmit_done  (PIFACE pi, DCU msg, RTP_BOOL success);
void        AT91_EMAC_recv       (PIFACE pi);
RTP_BOOL    AT91_EMAC_statistics (PIFACE  pi);
void        AT91_EMAC_interrupt  (PIFACE  pi);

/* ********************************************************************
   GLOBAL DATA
   ******************************************************************** */
EDEVTABLE RTP_FAR at91_mac_device = 
{
    AT91_EMAC_open, 
    AT91_EMAC_close, 
    AT91_EMAC_xmit, 
    NULLP_FUNC,
    NULLP_FUNC, 
    AT91_EMAC_statistics, 
    NULLP_FUNC, 
    AT91EMAC_DEVICE, 
    "AT91_EMAC", 
    MINOR_0, 
    ETHER_IFACE, 
    SNMP_DEVICE_INFO(CFG_OID_AT91_EMAC, CFG_SPEED_AT91_EMAC)
    CFG_ETHER_MAX_MTU, 
    CFG_ETHER_MAX_MSS, 
    CFG_ETHER_MAX_WIN_IN, 
    CFG_ETHER_MAX_WIN_OUT, 
    IOADD(0), 
    EN(0), 
    EN(0)
};

extern  NETWORK_CONFIG              g_NetworkConfig;

struct AT91_EMAC_SOFTC RTP_FAR at91_softc;

#define iface_to_softc(X) (PAT91_EMAC_SOFTC) &at91_softc;
#define off_to_softc(X)   (PAT91_EMAC_SOFTC) &at91_softc;

#if defined(__GNUC__)
#define ATTRIBUTE_ALIGN_8 __attribute__ ((aligned (8)))
#else
#define ATTRIBUTE_ALIGN_8 __align(8)
#endif

// Receive Transfer Descriptor buffer
static volatile ATTRIBUTE_ALIGN_8 RxTd rxTd; 
// Transmit Transfer Descriptor buffer
static volatile ATTRIBUTE_ALIGN_8 TxTd txTd; 
// Receiving Buffer
static volatile ATTRIBUTE_ALIGN_8 UINT8 RxBuffer[RX_BUFFERS * EMAC_RX_UNITSIZE];
// Transmitting Buffer
static volatile ATTRIBUTE_ALIGN_8 UINT8 TxBuffer[TX_BUFFERS * EMAC_TX_UNITSIZE];


RTP_BOOL AT91_EMAC_SetupDevice(void);

RTP_BOOL AT91_EMAC_open(PIFACE pi)   
{
    PAT91_EMAC_SOFTC sc = iface_to_softc(pi);
  
    if (!sc)
    {
        RTP_DEBUG_ERROR("AT91_EMAC_open: softc invalid!\r\n", NOVAR, 0, 0);
        set_errno(ENUMDEVICE);
        return(RTP_FALSE);
    }

    // Set Interface
    sc->iface = pi;

    int macLen = __min(g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_AT91EMAC].macAddressLen, sizeof(sc->mac_address));

    if(macLen > 0)
    {
        memcpy(&sc->mac_address[0], &g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_AT91EMAC].macAddressBuffer[0], macLen);
    }
    else
    {
        RTP_DEBUG_ERROR("Device initialize without MAC address!!!\r\n", NOVAR, 0, 0);
    }
    
    /* Now put in a dummy ethernet address */
    rtp_memcpy(&pi->addr.my_hw_addr[0], sc->mac_address, 6); // Get the ethernet address
    
    /* clear statistic information */
    sc->stats.packets_in     = 0L;
    sc->stats.packets_out    = 0L;
    sc->stats.bytes_in       = 0L;
    sc->stats.bytes_out      = 0L;
    sc->stats.errors_in      = 0L;
    sc->stats.errors_out     = 0L;    
    
    if(RTP_FALSE == AT91_EMAC_SetupDevice())
    {
        return RTP_FALSE;
    }
    
    rtp_irq_hook_interrupt( (RTP_PFVOID) pi, 
                            (RTP_IRQ_FN_POINTER)AT91_EMAC_recv, 
                            (RTP_IRQ_FN_POINTER) 0);
    
    return(RTP_TRUE);
}


void AT91_EMAC_close(PIFACE pi)                                /*__fn__*/
{
    ARGSUSED_PVOID( pi );  /*  keep compiler happy */
}

void AT91_EMAC_interrupt(void *param)
{
    PIFACE              pi;
    PAT91_EMAC_SOFTC     sc;

    UINT32 emac_isr;
    UINT32 emac_rsr;
    UINT32 emac_tsr;

    AT91_EMAC &emac = AT91_EMAC::EMAC();
    
    /* get pi structure based on the minor number   */
    sc = off_to_softc(0);
    
    pi = sc->iface;
    
    if (!pi)
    {
        return;
    }
    
    GLOBAL_LOCK(encIrq);

    emac_isr = emac.EMAC_ISR & emac.EMAC_IMR;
    emac_rsr = emac.EMAC_RSR;
    emac_tsr = emac.EMAC_TSR;
    
    /* Frame(s) received */
    if((emac_isr & AT91_EMAC::EMAC_RCOMP) || (emac_rsr & AT91_EMAC::EMAC_REC))
    {
        emac.EMAC_RSR |= emac_rsr;
        rtp_thrd_interrupt_continuation(pi->ctrl.index);
    }

    /* a Frame is transmitted */
    if((emac_isr & AT91_EMAC::EMAC_TCOMP) || (emac_tsr & AT91_EMAC::EMAC_COMP))
    {
        volatile EmacTDescriptor *pTxTd;

        emac.EMAC_TSR |= emac_tsr;

        // Check the buffers
        while (CIRC_CNT(txTd.head, txTd.tail, TX_BUFFERS))
        {
            pTxTd = txTd.td + txTd.tail;         

            // Exit if buffer has not been sent yet
            if (!(pTxTd->status & EMAC_TX_USED_BIT))            
                break;
            
            CIRC_INC( txTd.tail, TX_BUFFERS );
        }
        // signal IP task that xmit has completed
        rtp_net_invoke_output(pi, 1); 
    }
    
}

void AT91_EMAC_recv(PIFACE pi)
{
    DCU                msg;
    RTP_PFUINT8        dataTX;
    RTP_UINT16         FrameLength;
    UINT16             BufferLength;
    UINT32  tmpIdx = rxTd.idx;

    BOOL isFrame = FALSE;

    GLOBAL_LOCK(encIrq);

    volatile EmacTDescriptor *pRxTd = rxTd.td + rxTd.idx;

    while ((pRxTd->addr & EMAC_RX_OWNERSHIP_BIT) == EMAC_RX_OWNERSHIP_BIT)
    {
        // A start of frame has been received
        if(pRxTd->status & EMAC_RX_SOF_BIT) 
        {
            // Skip previous fragment
            while (tmpIdx != rxTd.idx)
            {
                pRxTd = rxTd.td + rxTd.idx;
                pRxTd->addr &= ~(EMAC_RX_OWNERSHIP_BIT);
                CIRC_INC(rxTd.idx, RX_BUFFERS);
            }
            
            // Start to gather buffers in a frame
            isFrame = TRUE;
        }

        // Increment the pointer
        CIRC_INC(tmpIdx, RX_BUFFERS);

        if(isFrame)
        {
            if (tmpIdx == rxTd.idx) 
            {
                debug_printf("Receive buffer is too small for the current frame!\r\n");
                do
                {
                    pRxTd = rxTd.td + rxTd.idx;
                    pRxTd->addr &= ~(EMAC_RX_OWNERSHIP_BIT);
                    CIRC_INC(rxTd.idx, RX_BUFFERS);
                } while(tmpIdx != rxTd.idx);
            }
            // An end of frame has been received
            if(pRxTd->status & EMAC_RX_EOF_BIT)
            {
                // Frame size from the EMAC
                FrameLength = (pRxTd->status & EMAC_LENGTH_FRAME);

                msg = os_alloc_packet_input(FrameLength, DRIVER_ALLOC);

                if (msg)
                {
                    dataTX = (RTP_PFUINT8)DCUTODATA(msg);
                    msg->length = FrameLength;                    
                }

                BufferLength = EMAC_RX_UNITSIZE;
                 // Get all the data
                while (rxTd.idx != tmpIdx)
                {                   
                    pRxTd = rxTd.td + rxTd.idx;
                    if(BufferLength >= FrameLength)                    
                        memcpy(dataTX, (void*)(pRxTd->addr & EMAC_ADDRESS_MASK), FrameLength);
                    else
                    {
                        memcpy(dataTX, (void*)(pRxTd->addr & EMAC_ADDRESS_MASK), BufferLength);
                        FrameLength -= BufferLength;
                        dataTX += BufferLength;
                    }
                    
                    pRxTd->addr &= ~(EMAC_RX_OWNERSHIP_BIT);
                    CIRC_INC(rxTd.idx, RX_BUFFERS);
                }
                
                 // signal IP layer that a packet is on its exchange
                 rtp_net_invoke_input(pi, msg, msg->length);
/*
                 // maybe more packets left re-queue the continuation
                 rtp_thrd_interrupt_continuation(pi->ctrl.index);
                 
                 return;
*/
                 // Prepare for the next Frame
                 isFrame = FALSE;
            }
        }// if(isFrame) ends
        else
        {
           pRxTd->addr &= ~(EMAC_RX_OWNERSHIP_BIT);
           rxTd.idx = tmpIdx;
        }
        
        // Process the next buffer
        pRxTd = rxTd.td + tmpIdx;
    }
}

int AT91_EMAC_xmit(PIFACE pi, DCU msg)
{
    RTP_UINT16              length = 0;
    RTP_PFUINT16            dataTX;

    volatile EmacTDescriptor *pTxTd;

    GLOBAL_LOCK(encIrq);
    
    if (!pi)
    {
        return (-1);
    }
     
    length = msg->length;
/*    
    if (length < ETHER_MIN_LEN)
    {
        length = ETHER_MIN_LEN;
    }
*/    
    if (length > (ETHERSIZE+4))
    {
        RTP_DEBUG_ERROR("xmit - length is too large, truncated", NOVAR, 0, 0);
        length = ETHERSIZE+4;         /* what a terriable hack! */
    }
    
    dataTX = (RTP_PFUINT16)DCUTODATA(msg);

    /* First see if there is enough space in the remainder of the transmit buffer */
    if (CIRC_SPACE(txTd.head, txTd.tail, TX_BUFFERS) == 0)
    {
        return (-1);
    }

    // Pointers to the current TxTd
    pTxTd = txTd.td + txTd.head;

    // Setup/Copy data to transmition buffer
    if (length != 0)
    {
        memcpy((void *)pTxTd->addr, dataTX, length);
    }

    if (txTd.head == TX_BUFFERS-1) 
            pTxTd->status = (length & EMAC_LENGTH_FRAME) | EMAC_TX_LAST_BUFFER_BIT | EMAC_TX_WRAP_BIT;
    else
        pTxTd->status = (length & EMAC_LENGTH_FRAME) | EMAC_TX_LAST_BUFFER_BIT;

    // Driver manage the ring buffer
    CIRC_INC(txTd.head, TX_BUFFERS)

    AT91_EMAC &emac = AT91_EMAC::EMAC();

    // Now start to transmit if it is not already done
    emac.EMAC_NCR |= AT91_EMAC::EMAC_TSTART;

    return (0);

}

RTP_BOOL AT91_EMAC_xmit_done  (PIFACE pi, DCU msg, RTP_BOOL success)
{
    PAT91_EMAC_SOFTC   sc;

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
        /* Update total number of successfully transmitted packets. */
        sc->stats.packets_out++;
        sc->stats.bytes_out += msg->length;
    }
    return(RTP_TRUE);
}

RTP_BOOL AT91_EMAC_statistics(PIFACE pi)                       /*__fn__*/
{
#if (!INCLUDE_KEEP_STATS)
    ARGSUSED_PVOID(pi)
#endif

    if (!pi)
    {
        return RTP_FALSE;
    }
    
    UPDATE_SET_INFO(pi, interface_packets_in, at91emac_packets_in)
    UPDATE_SET_INFO(pi, interface_packets_out, at91emac_packets_out)
    UPDATE_SET_INFO(pi, interface_bytes_in, at91emac_bytes_in)
    UPDATE_SET_INFO(pi, interface_bytes_out, at91emac_bytes_out)
    UPDATE_SET_INFO(pi, interface_errors_in, at91emac_errors_in)
    UPDATE_SET_INFO(pi, interface_errors_out, at91emac_errors_out)
    UPDATE_SET_INFO(pi, interface_packets_lost, 0L)
    return(RTP_TRUE);
}

int xn_bind_at91_mac(int minor_number)
{
    return(xn_device_table_add(at91_mac_device.device_id,
                        minor_number, 
                        at91_mac_device.iface_type,
                        at91_mac_device.device_name,
                        SNMP_DEVICE_INFO(at91_mac_device.media_mib, 
                                         at91_mac_device.speed)                         
                        (DEV_OPEN)at91_mac_device.open,
                        (DEV_CLOSE)at91_mac_device.close,
                        (DEV_XMIT)at91_mac_device.xmit,
                        (DEV_XMIT_DONE)at91_mac_device.xmit_done,
                        (DEV_PROCESS_INTERRUPTS)at91_mac_device.proc_interrupts,
                        (DEV_STATS)at91_mac_device.statistics,
                        (DEV_SETMCAST)at91_mac_device.setmcast));
}

void AT91_EMAC_Init();

RTP_BOOL AT91_EMAC_SetupDevice()
{
    UINT32 errCount = 0;
    
    AT91_EMAC_Init();

    if( !dm9161_mii_Init() )
    {
        RTP_DEBUG_ERROR("PHY Initialize ERROR!\r\n", NOVAR, 0, 0);
        return RTP_FALSE;
    }

    if( !dm9161_AutoNegotiate() )
    {
        RTP_DEBUG_ERROR("PHY AutoNegotiate ERROR!\r\n", NOVAR, 0, 0);
        return RTP_FALSE;
    }

    while(!dm9161_GetLinkSpeed(TRUE))
    {
        errCount++;
    }
    debug_printf("Link detected 0x%X\r\n", errCount);

    return RTP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void AT91_EMAC_Init()
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    UINT32 Index;
    UINT32 Address;
    UINT32 Status;

    PAT91_EMAC_SOFTC     sc = off_to_softc(0);

    AT91_PMC &pmc = AT91::PMC();
    pmc.EnablePeriphClock(AT91C_ID_EMAC);

    // Disable TX & RX and more
    emac.EMAC_NCR = 0;

    // Disable interrupts
    emac.EMAC_IDR = ~0;

    // Clear buffer index
    rxTd.idx = 0;
    CIRC_CLEAR(&txTd);

    // Setup the RX descriptors.
    for(Index = 0; Index < RX_BUFFERS; Index++) 
    {
        Address = (UINT32)(&(RxBuffer[Index * EMAC_RX_UNITSIZE]));
        // Remove EMAC_RX_OWNERSHIP_BIT and EMAC_RX_WRAP_BIT
        rxTd.td[Index].addr = Address & EMAC_ADDRESS_MASK;
        rxTd.td[Index].status = 0;
    }
    rxTd.td[RX_BUFFERS - 1].addr |= EMAC_RX_WRAP_BIT;

    // Setup the TX descriptors.
    for(Index = 0; Index < TX_BUFFERS; Index++)
    {
        Address = (UINT32)(&(TxBuffer[Index * EMAC_TX_UNITSIZE]));
        txTd.td[Index].addr = Address;
        txTd.td[Index].status = (UINT32)EMAC_TX_USED_BIT;
    }
    txTd.td[TX_BUFFERS - 1].status |= EMAC_TX_WRAP_BIT;

    // Set the MAC address
    emac.EMAC_SA1L = ( ((UINT32)sc->mac_address[3] << 24)
                                     | ((UINT32)sc->mac_address[2] << 16)
                                     | ((UINT32)sc->mac_address[1] << 8 )
                                     |                sc->mac_address[0] );

    emac.EMAC_SA1H = ( ((UINT32)sc->mac_address[5] << 8 )
                                     |                sc->mac_address[4] );
   
    // Now setup the descriptors
    // Receive Buffer Queue Pointer Register
    emac.EMAC_RBQP = (UINT32) (rxTd.td);
    // Transmit Buffer Queue Pointer Register
    emac.EMAC_TBQP = (UINT32) (txTd.td);

    emac.EMAC_NCR = AT91_EMAC::EMAC_CLRSTAT;

    // Clear all status bits in the receive status register.
    emac.EMAC_RSR = (AT91_EMAC::EMAC_OVR | AT91_EMAC::EMAC_REC | AT91_EMAC::EMAC_BNA);

    // Clear all status bits in the transmit status register
    emac.EMAC_TSR = ( AT91_EMAC::EMAC_UBR | AT91_EMAC::EMAC_COL | AT91_EMAC::EMAC_RLES
                                | AT91_EMAC::EMAC_BEX | AT91_EMAC::EMAC_COMP
                                | AT91_EMAC::EMAC_UND );

     // Clear Status
    Status = emac.EMAC_ISR;  

    // Don't copy FCS
    emac.EMAC_NCFGR |= (AT91_EMAC::EMAC_CAF | AT91_EMAC::EMAC_DRFCS | AT91_EMAC::EMAC_PAE);

    // Enable Rx and Tx, plus the stats register.
    emac.EMAC_NCR |= (AT91_EMAC::EMAC_TE | AT91_EMAC::EMAC_RE | AT91_EMAC::EMAC_WESTAT);

    // Setup the interrupts for TX (and errors)
    emac.EMAC_IER = AT91_EMAC::EMAC_RXUBR
                              | AT91_EMAC::EMAC_TUNDR
                              | AT91_EMAC::EMAC_RLEX
                              | AT91_EMAC::EMAC_TXERR
                              | AT91_EMAC::EMAC_TCOMP
                              | AT91_EMAC::EMAC_ROVR
                              | AT91_EMAC::EMAC_HRESP
                              | AT91_EMAC::EMAC_RCOMP;
}


BOOL AT91_EMAC_WaitPhy(UINT32 retry)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    // Wait until IDLE bit in Network Status register is cleared or timeout
    while( !(emac.EMAC_NSR & AT91_EMAC::EMAC_IDLE) )
    {
        if(retry != 0)
        {
            retry--;
            continue;
        }
        else
        {
            RTP_DEBUG_ERROR("PHY: TimeOut!\r\n", NOVAR, 0, 0);
            return FALSE;
        }
    }
    return TRUE;
}

// Read value stored in a PHY register
BOOL AT91_EMAC_ReadPhy(UINT32 phy_addr, UINT32 address, UINT32 *value, UINT32 retry)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();
    
    emac.EMAC_MAN = (AT91_EMAC::EMAC_SOF & (0x01 << 30))
                              | (AT91_EMAC::EMAC_CODE & (2 << 16))
                              | (AT91_EMAC::EMAC_RW & (2 << 28))
                              | (AT91_EMAC::EMAC_PHYA & ((phy_addr & 0x1f) << 23))
                              | (AT91_EMAC::EMAC_REGA & (address << 18));

   if(AT91_EMAC_WaitPhy(retry))
   {
        *value = (emac.EMAC_MAN & 0x0000ffff);
        return TRUE;
   }
   else
        return FALSE;       
}

BOOL AT91_EMAC_WritePhy(UINT32 phy_addr, UINT32 address, UINT32 value, UINT32 retry)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    emac.EMAC_MAN = (AT91_EMAC::EMAC_SOF & (0x01 << 30))
                              | (AT91_EMAC::EMAC_CODE & (2 << 16))
                              | (AT91_EMAC::EMAC_RW & (1 << 28))
                              | (AT91_EMAC::EMAC_PHYA & ((phy_addr & 0x1f) << 23))
                              | (AT91_EMAC::EMAC_REGA & (address << 18))
                              | (AT91_EMAC::EMAC_DATA & value) ;

    return AT91_EMAC_WaitPhy(retry);
}

BOOL AT91_EMAC_SetMdcClock(UINT32 mck)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    UINT32 clock_dividor;

    if (mck <= 20000000)
        clock_dividor = AT91_EMAC::EMAC_CLK_HCLK_8;          /// MDC clock = MCK/8

    else if (mck <= 40000000)
        clock_dividor = AT91_EMAC::EMAC_CLK_HCLK_16;         /// MDC clock = MCK/16
    
    else if (mck <= 80000000) 
        clock_dividor = AT91_EMAC::EMAC_CLK_HCLK_32;         /// MDC clock = MCK/32
    
    else if (mck <= 160000000)
        clock_dividor = AT91_EMAC::EMAC_CLK_HCLK_64;         /// MDC clock = MCK/64
    else 
    {
        debug_printf("Error: No valid MDC clock.\r\n");
        return FALSE;
    }
    
    emac.EMAC_NCFGR = (emac.EMAC_NCFGR & (~AT91_EMAC::EMAC_CLK)) | clock_dividor;
    return TRUE;
}

void AT91_EMAC_EnableMdio()
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();
    emac.EMAC_NCR |= AT91_EMAC::EMAC_MPE;
}

void AT91_EMAC_DisableMdio()
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();
    emac.EMAC_NCR &= ~AT91_EMAC::EMAC_MPE;
}

//-----------------------------------------------------------------------------
// speed        TRUE for 100M, FALSE for 10M
// fullduplex   TRUE for Full Duplex mode
//-----------------------------------------------------------------------------
void AT91_EMAC_SetLinkSpeed(BOOL speed, BOOL fullduplex)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();
    UINT32 ncfgr;

    ncfgr = emac.EMAC_NCFGR;
    ncfgr &= ~(AT91_EMAC::EMAC_SPD | AT91_EMAC::EMAC_FD);

    if (speed)
        ncfgr |= AT91_EMAC::EMAC_SPD;
   
    if (fullduplex)
        ncfgr |= AT91_EMAC::EMAC_FD;
    
    emac.EMAC_NCFGR = ncfgr;
}

void AT91_EMAC_EnableMII()
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    emac.EMAC_USRIO = AT91_EMAC::EMAC_CLKEN;
}

