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

#include <tinyhal.h>
#include "net_decl_lwip.h"
#include "LPC24XX_EMAC_lwip.h"

extern void lwip_interrupt_continuation( void );

/* ********************************************************************
   DEBUG AIDS
   ******************************************************************** */
#define DEBUG_LPC24XX_EMAC 0

extern BOOL ENET_PHY_lwip_init ( void );
extern BOOL ENET_PHY_lwip_get_MII_mode ( void );
extern UINT32 ENET_PHY_lwip_get_MDC_Clk_Div ( void );

//void LPC24XX_EMAC_setmcast ( PIFACE  pi );

void LPC24XX_EMAC_lwip_FastCopy ( UINT32 * dst, UINT32 * src, UINT32 len );

BOOL LPC24XX_EMAC_lwip_open ( struct netif *pNetIf )
{
    /* Initialiaze external ethernet Phy */
    if ( !LPC24XX_EMAC_lwip_init(pNetIf) )
    {
        return FALSE;
    }

    return TRUE;
}

void LPC24XX_EMAC_lwip_close ( struct netif *pNetIf )
{
    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC *)LPC24XX_EMAC::c_EMAC_Base;

    ENET.MAC_COMMAND  = 0;
    ENET.MAC_INTENABLE = 0;
    LPC24XX::SYSCON().PCONP &= ~(LPC24XX_SYSCON::ENABLE_ENET);
}

err_t LPC24XX_EMAC_lwip_xmit ( struct netif *pNetIf, struct pbuf *pPBuf )
{
    UINT32 bytes_out,idx;
    UINT32 * dst;
    UINT32 * src;
    
    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC *)LPC24XX_EMAC::c_EMAC_Base;
    
    /* Disable intrrupts */
    GLOBAL_LOCK(irq);
    
    if (!pNetIf || !pPBuf)
    {
        return ERR_ARG;
    }
    
    bytes_out = pPBuf->tot_len;
    if( bytes_out > ETH_MAX_FLEN )
    {
        debug_printf("LPC24XX_EMAC_xmit - Ehernet Frame truncated");
        bytes_out = ETH_MAX_FLEN;   /* What's the point of sending less data! */
    }
    idx = ENET.MAC_TXPRODUCEINDEX;
    /* Check if a Tx descriptor is available */
    if ( ENET.MAC_TXCONSUMEINDEX == (idx + 1) )
    {
        /* Tx descriptor not available */
        return ERR_IF;
    }
    dst = (UINT32 *)TX_DESC_PACKET(idx);
    while (pPBuf)
    {
        /* TODO - can LWIP word align input buffer ? If so we could use FastCopy */
        // src = (UINT32 *)pPBuf->payload;
        // LPC24XX_EMAC_lwip_FastCopy(dst, src, bytes_out);
        memcpy(dst, pPBuf->payload, pPBuf->len);
        pPBuf = pPBuf->next;
        dst += pPBuf->len;
    }
    
    TX_DESC_CTRL(idx) = bytes_out | TCTRL_LAST | TCTRL_INT;
    idx++;
    /* Reset the index if last descriptor */
    if (idx == NUM_TX_FRAG) 
    {
        idx = 0;
    }
    ENET.MAC_TXPRODUCEINDEX = idx;

    return ERR_OK;
}

void LPC24XX_EMAC_lwip_recv( struct netif *pNetIf )
{
    UINT32 idx, bytes_in;
    struct pbuf *pPBuf;
    void *dst, *src;
    
    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC *)LPC24XX_EMAC::c_EMAC_Base;

    /* Disable intrrupts */
    GLOBAL_LOCK(irq);
    
    while ( ENET.MAC_RXCONSUMEINDEX != ENET.MAC_RXPRODUCEINDEX )
    {
        /* Get the index of the received ethernet frame */
        idx = ENET.MAC_RXCONSUMEINDEX;
        if ( RX_STAT_INFO(idx) & RX_FRAME_ERROR )
        {
            /* Received ethernet frame has an error */
            //lpc24xx_emac_stats.errors_in++;
        }
        else
        {
            /* Ethernet Frame FCS is not part of the data and the frame size is - 1 encoded */
            bytes_in = ( RX_STAT_INFO(idx) & RX_DESC_STATUS_SIZE ) - 3;
            
            pPBuf = pbuf_alloc(PBUF_RAW, bytes_in, PBUF_RAM);

            if( pPBuf )
            {
                src = (void*)RX_DESC_PACKET(idx);
                dst = pPBuf->payload;
                memcpy(dst, src, bytes_in);
                /* Tell IP layer that a packet has arrived */
                pNetIf->input( pPBuf, pNetIf );
                /* Update statistics */
                //lpc24xx_emac_stats.packets_in++;
                //lpc24xx_emac_stats.bytes_in += bytes_in;
            }
            else
            {
                debug_printf("LPC24XX_EMAC_recv: pbuf_alloc failed");
                //lpc24xx_emac_stats.packets_lost++;
            }    
        }
        idx++;
        /* Reset the index if last descriptor */
        if (idx == NUM_RX_FRAG) 
        {    
             idx = 0;
        }
        /* Mark RX descriptor as available */
        ENET.MAC_RXCONSUMEINDEX = idx;
    }
}

void LPC24XX_EMAC_lwip_interrupt( struct netif *pNetIf )
{
    UINT32 intstatus;

    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC *)LPC24XX_EMAC::c_EMAC_Base;

    /* Disable intrrupts */
    GLOBAL_LOCK(irq);
    
    intstatus = ENET.MAC_INTSTATUS;
    
    if ( !( intstatus & ( INT_RX_OVERRUN | INT_TX_UNDERRUN ) ) )
    {
        if ( pNetIf )
        {
            if( intstatus & INT_RX_DONE )
            {
                /* Ethernet frame received */
                lwip_interrupt_continuation( );
                /* Clear Rx Done Interrupt */
                ENET.MAC_INTCLEAR = INT_RX_DONE;
            }
            if( intstatus & INT_TX_DONE )
            {
                /* Clear Tx Done Interrupt */
                ENET.MAC_INTCLEAR = INT_TX_DONE;
            }
        }
        else
        {
            /* Invalid netif structure, clear the interrupts and return */
            ENET.MAC_INTCLEAR = INT_RX_DONE | INT_TX_DONE;
            return;
        }
    }
    else
    {
        if ( intstatus & INT_RX_OVERRUN )
        {
            /* Fatal Rx Error Reset RX hardware */
            ENET.MAC_COMMAND |= CR_RX_RES;
            /* Clear Rx overrun Interrupt */
            ENET.MAC_INTCLEAR = INT_RX_OVERRUN;
            //lpc24xx_emac_stats.rx_other_errors++;
        }
        if ( intstatus & INT_TX_UNDERRUN )
        {
            /* Fatal Tx Error Reset TX hardware */
            ENET.MAC_COMMAND |= CR_TX_RES;
            /* Clear Tx underrun Interrupt */
            ENET.MAC_INTCLEAR = INT_TX_UNDERRUN;
            //lpc24xx_emac_stats.tx_other_errors++;
        }
        HAL_Time_Sleep_MicroSeconds(1);
    }
}

#if 0
void LPC24XX_EMAC_setmcast ( PIFACE  pi )
{
    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC *)LPC24XX_EMAC::c_EMAC_Base;
    /* Receive Multicast Packets */
    ENET.MAC_RXFILTERCTRL |= RFC_MCAST_EN;
}
#endif

BOOL LPC24XX_EMAC_lwip_init ( struct netif *pNetIf )
{
    UINT32 i;
    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC *)LPC24XX_EMAC::c_EMAC_Base;

    /* Power Up the EMAC controller. */
    LPC24XX::SYSCON().PCONP |= LPC24XX_SYSCON::ENABLE_ENET;
    
    /* Connect EMAC pins */
    LPC24XX_EMAC_lwip_setpins( ENET_PHY_lwip_get_MII_mode() );
    
    /* Reset EMAC */
    ENET.MAC_MAC1 = MAC1_RES_TX | MAC1_RES_MCS_TX | MAC1_RES_RX | MAC1_RES_MCS_RX |
                    MAC1_SIM_RES | MAC1_SOFT_RES;
    ENET.MAC_COMMAND = CR_REG_RES | CR_TX_RES | CR_RX_RES;
    HAL_Time_Sleep_MicroSeconds(1);
    
      /* Initialize MAC control registers. */
    ENET.MAC_MAC1 = 0;
    ENET.MAC_MAC2 = MAC2_CRC_EN | MAC2_PAD_EN;
    ENET.MAC_MAXF = ETH_MAX_FLEN;
    ENET.MAC_CLRT = CLRT_DEF;
    ENET.MAC_IPGR = IPGR_DEF;

    if ( !ENET_PHY_lwip_get_MII_mode() )
    {
        /* Enable RMII mode in MAC command register */
        ENET.MAC_COMMAND = ENET.MAC_COMMAND | CR_RMII;
    }

    /* Reset MII Management hardware */
    ENET.MAC_MCFG = MCFG_RES_MII;
    HAL_Time_Sleep_MicroSeconds(1);
    
    /* Set MDC Clock divider */
    ENET.MAC_MCFG = ENET_PHY_lwip_get_MDC_Clk_Div() & MCFG_CLK_SEL;

    /* Initialiaze external ethernet Phy */
    if ( !ENET_PHY_lwip_init() )
    {
        return FALSE;
    }

    /* Set the Ethernet MAC Address registers */
    ENET.MAC_SA2 = (pNetIf->hwaddr[1] << 8) | pNetIf->hwaddr[0];
    ENET.MAC_SA1 = (pNetIf->hwaddr[3] << 8) | pNetIf->hwaddr[2];
    ENET.MAC_SA0 = (pNetIf->hwaddr[5] << 8) | pNetIf->hwaddr[4];
    
    /* Setup the Rx DMA Descriptors */
    for (i = 0; i < NUM_RX_FRAG; i++)
    {
        RX_DESC_PACKET(i)  = RX_BUF(i);
        RX_DESC_CTRL(i)    = RCTRL_INT | (ETH_FRAG_SIZE-1);
        RX_STAT_INFO(i)    = 0;
        RX_STAT_HASHCRC(i) = 0;
    }

    /* Set the EMAC Rx Descriptor Registers. */
    ENET.MAC_RXDESCRIPTOR    = RX_DESC_BASE;
    ENET.MAC_RXSTATUS        = RX_STAT_BASE;
    ENET.MAC_RXDESCRIPTORNUM = NUM_RX_FRAG-1;
    ENET.MAC_RXCONSUMEINDEX  = 0;

    /* Setup the Tx DMA Descriptors */
    for (i = 0; i < NUM_TX_FRAG; i++) 
    {
        TX_DESC_PACKET(i) = TX_BUF(i);
        TX_DESC_CTRL(i)   = 0;
        TX_STAT_INFO(i)   = 0;
    }

    /* Set the EMAC Tx Descriptor Registers. */
    ENET.MAC_TXDESCRIPTOR    = TX_DESC_BASE;
    ENET.MAC_TXSTATUS        = TX_STAT_BASE;
    ENET.MAC_TXDESCRIPTORNUM = NUM_TX_FRAG-1;
    ENET.MAC_TXPRODUCEINDEX  = 0;
    
    /* Receive Broadcast and Perfect Match Packets */
    ENET.MAC_RXFILTERCTRL = RFC_PERFECT_EN | RFC_BCAST_EN;
    
    /* Enable EMAC interrupts. */
    ENET.MAC_INTENABLE = INT_RX_DONE | INT_TX_DONE | INT_TX_UNDERRUN | INT_RX_OVERRUN;

    /* Reset all interrupts */
    ENET.MAC_INTCLEAR  = 0xFFFF;

    /* Enable receive and transmit */
    ENET.MAC_COMMAND  |= (CR_RX_EN | CR_TX_EN);
    ENET.MAC_MAC1     |= MAC1_REC_EN;
    
    return TRUE;
}

void LPC24XX_EMAC_lwip_setpins ( BOOL mode )
{
    /* Connect pins to EMAC controller */
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_TXD0, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_TXD1, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_TX_EN, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_CRS_DV, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_RXD0, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_RXD1, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_RX_ER, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_REF_CLK, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );

    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_MDC, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_MDIO, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    
    if (mode)
    {
    /* Additional pins for MII mode */
        CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_TXD2, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
        CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_TXD3, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
        CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_TX_ER, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
        CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_TX_CLK, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
        CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_COL, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
        CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_RXD2, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
        CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_RXD3, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
        CPU_GPIO_DisablePin( LPC24XX_EMAC::c_ENET_RX_DV, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_1 );
    }
}

BOOL LPC24XX_EMAC_lwip_WritePhy ( UINT32 phy_addr, UINT32 phy_reg_addr, UINT32 value, UINT32 retry )
{
    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC    *)LPC24XX_EMAC::c_EMAC_Base;

    ENET.MAC_MADR = phy_addr | phy_reg_addr;
    ENET.MAC_MWTD = value;

    /* Wait for the command completion */
    if (retry != 0)
    {
        while( (ENET.MAC_MIND & MIND_BUSY) )
        {
            retry--;
            if( retry == 0)
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

BOOL LPC24XX_EMAC_lwip_ReadPhy ( UINT32 phy_addr, UINT32 phy_reg_addr, UINT32 *value, UINT32 retry )
{
    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC    *)LPC24XX_EMAC::c_EMAC_Base;

    ENET.MAC_MADR = phy_addr | phy_reg_addr;
    ENET.MAC_MCMD = MCMD_READ;

    /* Wait for the command completion */
    if (retry != 0)
    {
        while( (ENET.MAC_MIND & MIND_BUSY) )
        {
            retry--;
            if( retry == 0)
            {
                ENET.MAC_MCMD = 0;
                return FALSE;
            }
        }
    }
    ENET.MAC_MCMD = 0;
    *value = ENET.MAC_MRDD;
    return TRUE;
}

void LPC24XX_EMAC_lwip_SetLinkSpeed ( BOOL speed, BOOL fullduplex )
{
    LPC24XX_EMAC & ENET = *(LPC24XX_EMAC *)LPC24XX_EMAC::c_EMAC_Base;
    
    if ( speed )
    {
        /* 100 Mbit */
        ENET.MAC_SUPP = SUPP_SPEED;
        debug_printf("Ethernet Link 100M/");
    }
    else
    {
        /* 10 Mbit */
        ENET.MAC_SUPP = 0;
        debug_printf("Ethernet Link 10M/");
    }
    if ( fullduplex )
    {
        /* Full Duplex */
        ENET.MAC_MAC2    |= MAC2_FULL_DUP;
        ENET.MAC_COMMAND |= CR_FULL_DUP;
        ENET.MAC_IPGT     = IPGT_FULL_DUP;
        debug_printf("Full Duplex.\r\n");
    }
    else
    {
        /* Half Duplex */
        ENET.MAC_IPGT = IPGT_HALF_DUP;
        debug_printf("Half Duplex.\r\n");
    }
}

void LPC24XX_EMAC_lwip_FastCopy ( UINT32 * dst, UINT32 * src, UINT32 len )
{
    UINT32 i;
    UINT8 * src8;
    UINT8 * dst8;

    for(i = 0; i< len / 4; i++)
    {
        *dst = *src;
        src++;
        dst++;
    }
    if( len % 4 )
    {
        src8 = (UINT8 *)src;
        dst8 = (UINT8 *)dst;
        for(i = 0; i< len % 4; i++)
        {
            *dst8 = *src8;
            src8++;
            dst8++;
        }
    }
}
