////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "net_decl_lwip.h"
#include "lwip\netif.h"
#include "netif\etharp.h"
#include "lwip\pbuf.h"

#include "AT91_EMAC_lwip.h"
#include "dm9161_lwip.h"

//--//
/* ********************************************************************
   DEBUG AIDS
   ******************************************************************** */
#define DEBUG_AT91EMAC 0

#ifndef NETWORK_INTERFACE_INDEX_AT91EMAC
#define NETWORK_INTERFACE_INDEX_AT91EMAC 0
#endif

extern void lwip_interrupt_continuation( void );

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


BOOL AT91_EMAC_LWIP_SetupDevice(struct netif *pNetIf);

BOOL AT91_EMAC_LWIP_open(struct netif *pNetIf)   
{
    return AT91_EMAC_LWIP_SetupDevice(pNetIf);
}

void AT91_EMAC_LWIP_close(struct netif *pNetIf)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    //disable MII
    emac.EMAC_USRIO = 0;
    
    // Disable interrupts
    emac.EMAC_IDR = ~0;

    // Disable TX & RX and more
    emac.EMAC_NCR = 0;

    // Disable clock
    AT91_PMC &pmc = AT91::PMC();
    pmc.DisablePeriphClock(AT91C_ID_EMAC);

    memset((void*)RxBuffer, 0, sizeof(RxBuffer));
    memset((void*)TxBuffer, 0, sizeof(TxBuffer));
    memset((void*)&rxTd   , 0, sizeof(rxTd    ));
    memset((void*)&txTd   , 0, sizeof(txTd    ));
}

void AT91_EMAC_LWIP_interrupt(struct netif *pNetIf)
{
    UINT32 emac_isr;
    UINT32 emac_rsr;
    UINT32 emac_tsr;

    AT91_EMAC &emac = AT91_EMAC::EMAC();
    

    emac_isr = emac.EMAC_ISR & emac.EMAC_IMR;
    emac_rsr = emac.EMAC_RSR;
    emac_tsr = emac.EMAC_TSR;
    
    /* Frame(s) received */
    if((emac_isr & AT91_EMAC::EMAC_RCOMP) || (emac_rsr & (AT91_EMAC::EMAC_REC | AT91_EMAC::EMAC_BNA)))
    {
        emac.EMAC_RSR = AT91_EMAC::EMAC_REC | AT91_EMAC::EMAC_BNA;
        lwip_interrupt_continuation();
    }

    /* a Frame is transmitted */
    if((emac_isr & AT91_EMAC::EMAC_TCOMP) || (emac_tsr & AT91_EMAC::EMAC_COMP))
    {
        volatile EmacTDescriptor *pTxTd;

        // clear TSR
        emac.EMAC_TSR = AT91_EMAC::EMAC_COMP;

        // Check the buffers
        while (CIRC_CNT(txTd.head, txTd.tail, TX_BUFFERS))
        {
            pTxTd = &txTd.td[txTd.tail];         

            // Exit if buffer has not been sent yet
            if (0 == (pTxTd->status & EMAC_TX_USED_BIT))            
                break;
            
            CIRC_INC( txTd.tail, TX_BUFFERS );
        }
    }
    
}

void AT91_EMAC_LWIP_recv(struct netif *pNetIf)
{
    struct pbuf *pPBuf;
    UINT8       *dataRX;
    UINT16       frameLength;
    UINT16       bufferLength;
    UINT32       tmpIdx = rxTd.idx;
    BOOL         isFrame = FALSE;

    GLOBAL_LOCK(encIrq);

    volatile EmacTDescriptor *pRxTd = &rxTd.td[rxTd.idx];

    while ((pRxTd->addr & EMAC_RX_OWNERSHIP_BIT) == EMAC_RX_OWNERSHIP_BIT)
    {
        // A start of frame has been received
        if(pRxTd->status & EMAC_RX_SOF_BIT) 
        {
            // Skip previous fragment
            while (tmpIdx != rxTd.idx)
            {
                pRxTd = &rxTd.td[rxTd.idx];
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
            if (tmpIdx == rxTd.idx && 0 == (pRxTd->status & EMAC_RX_EOF_BIT))
            {
                debug_printf("Receive buffer is too small for the current frame!\r\n");
                do
                {
                    pRxTd = &rxTd.td[rxTd.idx];
                    pRxTd->addr &= ~(EMAC_RX_OWNERSHIP_BIT);
                    CIRC_INC(rxTd.idx, RX_BUFFERS);
                } while(tmpIdx != rxTd.idx);
            }
            // An end of frame has been received
            if(pRxTd->status & EMAC_RX_EOF_BIT)
            {
                // Frame size from the EMAC
                frameLength = (pRxTd->status & EMAC_LENGTH_FRAME);

                pPBuf = pbuf_alloc(PBUF_RAW, frameLength, PBUF_RAM);

                if (pPBuf)
                {
                    dataRX = (UINT8*)pPBuf->payload;
 
                    bufferLength = EMAC_RX_UNITSIZE;
                    // Get all the data
                    do
                    {                   
                        pRxTd = &rxTd.td[rxTd.idx];
                        if(bufferLength >= frameLength)
                        {
                            memcpy(dataRX, (void*)(pRxTd->addr & EMAC_ADDRESS_MASK), frameLength);
                        }
                        else
                        {
                            memcpy(dataRX, (void*)(pRxTd->addr & EMAC_ADDRESS_MASK), bufferLength);
                            frameLength -= bufferLength;
                            dataRX += bufferLength;
                        }
                    
                        pRxTd->addr &= ~(EMAC_RX_OWNERSHIP_BIT);
                        CIRC_INC(rxTd.idx, RX_BUFFERS);
                    }while (rxTd.idx != tmpIdx);


                    encIrq.Release();
                    // signal IP layer that a packet is on its exchange
                    pNetIf->input(pPBuf, pNetIf);
                    encIrq.Acquire();
                }                

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
        pRxTd = &rxTd.td[tmpIdx];
    }
}

err_t AT91_EMAC_LWIP_xmit(struct netif *pNetIf, struct pbuf *pPBuf)
{
    UINT16  length = 0;
    UINT8 *pDst;
    INT32 len;

    volatile EmacTDescriptor *pTxTd;

    GLOBAL_LOCK(encIrq);
    
    if (!pNetIf || !pPBuf)
    {
        return ERR_ARG;
    }

    length = pPBuf->tot_len;

    if(length == 0) return ERR_OK;

    
    /* First see if there is enough space in the remainder of the transmit buffer */
    if (CIRC_SPACE(txTd.head, txTd.tail, TX_BUFFERS) * EMAC_TX_UNITSIZE < length)
    {
        debug_printf("AT91_EMAC_LWIP_xmit: no space\r\n");
        AT91_EMAC &emac = AT91_EMAC::EMAC();
        // Now start to transmit if it is not already done
        emac.EMAC_NCR |= AT91_EMAC::EMAC_TSTART;
        return ERR_IF;
    }

    len = length;
    // Copy data to transmition buffer
    while (len > 0 && pPBuf != NULL)
    {
        int max = EMAC_TX_UNITSIZE;
        int offset = 0;

        // Pointers to the current TxTd
        pTxTd = &txTd.td[txTd.head];
        pDst = (UINT8*)pTxTd->addr;

        do
        {
            int bufLen = pPBuf->len - offset;
            BYTE *pTmp = (BYTE*)pPBuf->payload;

            if(bufLen > max) bufLen = max;

            memcpy(pDst, &pTmp[offset], bufLen);

            pDst += bufLen;
            max  -= bufLen;
            len  -= bufLen;
            
            if(bufLen + offset >= pPBuf->len)
            {
                pPBuf  = pPBuf->next;       
                offset = 0;
            }
            else
            {
                offset += bufLen;
            }

        } while( max > 0 && len > 0 && pPBuf != NULL);

        if (txTd.head == TX_BUFFERS-1)
        {
            pTxTd->status = (length & 0x7FF) | EMAC_TX_WRAP_BIT;
        }
        else
        {
            pTxTd->status = (length & 0x7FF);
        }
    
        // Driver manage the ring buffer
        CIRC_INC(txTd.head, TX_BUFFERS)
    }

    pTxTd->status |= EMAC_TX_LAST_BUFFER_BIT;

    AT91_EMAC &emac = AT91_EMAC::EMAC();

    // Now start to transmit if it is not already done
    emac.EMAC_NCR |= AT91_EMAC::EMAC_TSTART;

    return ERR_OK;

}

void AT91_EMAC_LWIP_Init(struct netif *pNetIf);

BOOL AT91_EMAC_LWIP_SetupDevice(struct netif *pNetIf)
{
    UINT32 errCount = 0;
    
    AT91_EMAC_LWIP_Init(pNetIf);

    if( !dm9161_lwip_mii_Init() )
    {
        debug_printf("PHY Initialize ERROR!\r\n");
        return FALSE;
    }

    if( !dm9161_lwip_AutoNegotiate() )
    {
        debug_printf("PHY AutoNegotiate ERROR!\r\n");
        return FALSE;
    }

    while(errCount < 1000 && !dm9161_lwip_GetLinkSpeed(TRUE))
    {
        errCount++;
    }
    debug_printf("Link detected 0x%X\r\n", errCount);

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void AT91_EMAC_AddMulticastAddr(UINT32 mcast)
{
    UINT32 hash = 0;
    UINT64 addr = mcast;
    AT91_EMAC &emac = AT91_EMAC::EMAC();
 
    addr = htonl(addr);
    addr &= 0x7FFFFF;
                    
    addr |=  0x01005E000000;

    hash |= (1 & ((addr >> 5) ^ (addr >> 11) ^ (addr >> 17) ^ (addr >> 23) ^ (addr >> 29) ^ (addr >> 35) ^ (addr >> 41) ^ (addr >> 47))) << 5;
    hash |= (1 & ((addr >> 4) ^ (addr >> 10) ^ (addr >> 16) ^ (addr >> 22) ^ (addr >> 28) ^ (addr >> 34) ^ (addr >> 40) ^ (addr >> 46))) << 4;
    hash |= (1 & ((addr >> 3) ^ (addr >>  9) ^ (addr >> 15) ^ (addr >> 21) ^ (addr >> 27) ^ (addr >> 33) ^ (addr >> 39) ^ (addr >> 45))) << 3;
    hash |= (1 & ((addr >> 2) ^ (addr >>  8) ^ (addr >> 14) ^ (addr >> 20) ^ (addr >> 26) ^ (addr >> 32) ^ (addr >> 38) ^ (addr >> 44))) << 2;
    hash |= (1 & ((addr >> 1) ^ (addr >>  7) ^ (addr >> 13) ^ (addr >> 19) ^ (addr >> 25) ^ (addr >> 31) ^ (addr >> 37) ^ (addr >> 43))) << 1;
    hash |= (1 & ((addr >> 0) ^ (addr >>  6) ^ (addr >> 12) ^ (addr >> 18) ^ (addr >> 24) ^ (addr >> 30) ^ (addr >> 36) ^ (addr >> 42))) << 0;

    if(hash < 32)
    {
        emac.EMAC_HRB |= 1u << hash;
    }
    else
    {
        emac.EMAC_HRT |= 1u << (hash - 32); 
    }
}

void AT91_EMAC_LWIP_Init(struct netif *pNetIf)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    UINT32 Index;
    UINT32 Address;
    UINT32 Status;

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
    emac.EMAC_SA1L = ( ((UINT32)pNetIf->hwaddr[3] << 24)
                     | ((UINT32)pNetIf->hwaddr[2] << 16)
                     | ((UINT32)pNetIf->hwaddr[1] << 8 )
                     |          pNetIf->hwaddr[0] );

    emac.EMAC_SA1H = ( ((UINT32)pNetIf->hwaddr[5] << 8 )
                     |          pNetIf->hwaddr[4] );

    // Initialize multicast - valid addresses will be set up when ADD_MEMBERSHIP sockopt is used.
    emac.EMAC_HRB = 0x00000000;
    emac.EMAC_HRT = 0x00000000; 

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
                                | AT91_EMAC::EMAC_TGO | AT91_EMAC::EMAC_BEX | AT91_EMAC::EMAC_COMP
                                | AT91_EMAC::EMAC_UND );

     // Clear Status
    Status = emac.EMAC_ISR;  

    // Don't copy FCS
    emac.EMAC_NCFGR |= (AT91_EMAC::EMAC_DRFCS | AT91_EMAC::EMAC_PAE | AT91_EMAC::EMAC_MTI);

    // Enable Rx and Tx, plus the stats register.
    emac.EMAC_NCR |= (AT91_EMAC::EMAC_TE | AT91_EMAC::EMAC_RE); // | AT91_EMAC::EMAC_WESTAT);

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


BOOL AT91_EMAC_LWIP_WaitPhy(UINT32 retry)
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
            debug_printf("PHY: TimeOut!\r\n");
            return FALSE;
        }
    }
    return TRUE;
}

// Read value stored in a PHY register
BOOL AT91_EMAC_LWIP_ReadPhy(UINT32 phy_addr, UINT32 address, UINT32 *value, UINT32 retry)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();
    
    emac.EMAC_MAN = (AT91_EMAC::EMAC_SOF & (0x01 << 30))
                              | (AT91_EMAC::EMAC_CODE & (2 << 16))
                              | (AT91_EMAC::EMAC_RW & (2 << 28))
                              | (AT91_EMAC::EMAC_PHYA & ((phy_addr & 0x1f) << 23))
                              | (AT91_EMAC::EMAC_REGA & (address << 18));

   if(AT91_EMAC_LWIP_WaitPhy(retry))
   {
        *value = (emac.EMAC_MAN & 0x0000ffff);
        return TRUE;
   }
   else
        return FALSE;       
}

BOOL AT91_EMAC_LWIP_WritePhy(UINT32 phy_addr, UINT32 address, UINT32 value, UINT32 retry)
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    emac.EMAC_MAN = (AT91_EMAC::EMAC_SOF & (0x01 << 30))
                              | (AT91_EMAC::EMAC_CODE & (2 << 16))
                              | (AT91_EMAC::EMAC_RW & (1 << 28))
                              | (AT91_EMAC::EMAC_PHYA & ((phy_addr & 0x1f) << 23))
                              | (AT91_EMAC::EMAC_REGA & (address << 18))
                              | (AT91_EMAC::EMAC_DATA & value) ;

    return AT91_EMAC_LWIP_WaitPhy(retry);
}

BOOL AT91_EMAC_LWIP_SetMdcClock(UINT32 mck)
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

void AT91_EMAC_LWIP_EnableMdio()
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();
    emac.EMAC_NCR |= AT91_EMAC::EMAC_MPE;
}

void AT91_EMAC_LWIP_DisableMdio()
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();
    emac.EMAC_NCR &= ~AT91_EMAC::EMAC_MPE;
}

//-----------------------------------------------------------------------------
// speed        TRUE for 100M, FALSE for 10M
// fullduplex   TRUE for Full Duplex mode
//-----------------------------------------------------------------------------
void AT91_EMAC_LWIP_SetLinkSpeed(BOOL speed, BOOL fullduplex)
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

void AT91_EMAC_LWIP_EnableMII()
{
    AT91_EMAC &emac = AT91_EMAC::EMAC();

    emac.EMAC_USRIO = AT91_EMAC::EMAC_CLKEN;
}

