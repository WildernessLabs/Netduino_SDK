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

#include "LPC24XX_EMAC_lwip.h"
#include "DP83848_lwip.h"

static BOOL PhyReset ( void )
{
    UINT32 tout,regv;

    if( !LPC24XX_EMAC_lwip_WritePhy( DP83848C_DEF_ADR, PHY_REG_BMCR, BMCR_RESET, MII_WR_TOUT ) )
    {
        return( FALSE );
    }
    
    /* Wait 3 uS (as per the PHY spec) before accessing PHY registers */
    HAL_Time_Sleep_MicroSeconds(3);
    /* Wait for the hardware reset to end */
    for (tout = 0; tout < MII_WR_TOUT; tout++) 
    {
        if( !LPC24XX_EMAC_lwip_ReadPhy( DP83848C_DEF_ADR, PHY_REG_BMCR, &regv, MII_RD_TOUT ) )
        {
            return( FALSE );
        }
        if (!(regv & BMCR_RESET)) 
        {
            /* Reset complete */
            return( TRUE );
        }
    }
    return( FALSE );
}

static BOOL PhyVerifyId ( void )
{
    UINT32 id1,id2;

    if( !LPC24XX_EMAC_lwip_ReadPhy( DP83848C_DEF_ADR, PHY_REG_IDR1, &id1, MII_RD_TOUT ) )
    {
        return( FALSE );
    }
    if( !LPC24XX_EMAC_lwip_ReadPhy( DP83848C_DEF_ADR, PHY_REG_IDR2, &id2, MII_RD_TOUT ) )
    {
        return( FALSE );
    }
    
    if (((id1 << 16) | (id2 & 0xFFF0)) != DP83848C_ID)
    {
        return( FALSE );
    }
    return (TRUE);
}

static BOOL PhyAutoNegotiate (void )
{
    UINT32 tout,regv;
    
    /* Start Autonegotiation */
    if( !LPC24XX_EMAC_lwip_WritePhy( DP83848C_DEF_ADR, PHY_REG_BMCR, PHY_AUTO_NEG, MII_WR_TOUT ) )
    {
        return( FALSE );
    }
    /* Autonegotiation may take up to 3 seconds */
    for ( tout = 0; tout < 7; tout++)
    {
        HAL_Time_Sleep_MicroSeconds(500000);
        if( !LPC24XX_EMAC_lwip_ReadPhy( DP83848C_DEF_ADR, PHY_REG_BMSR, &regv, MII_RD_TOUT ) )
        {
            return( FALSE );
        }
        if ( regv & BMSR_AUTO_DONE ) 
        {
            /* Autonegotiation complete */
            return (TRUE);
        }
    }
    return (FALSE);
}

static BOOL PhySetLinkSpeed (void )
{
    UINT32 regv;
    BOOL full_duplex, mbit_100;

    /* Use Autonegotiation to set the link speed. */
    if ( !PhyAutoNegotiate() )
    {
        debug_printf("Warning: Ethernet PHY Link autonegotiation failure.\r\n");
        mbit_100 = FALSE;
        full_duplex = TRUE;
    }
    else
    {
        /* Read link Status */
        if( !LPC24XX_EMAC_lwip_ReadPhy( DP83848C_DEF_ADR, PHY_REG_BMSR, &regv, MII_RD_TOUT ) )
        {
            return( FALSE );
        }
        /* Configure Full/Half Duplex mode. */
        if (regv & STS_FULL_DUP) 
        {
            /* Full Duplex mode */
            full_duplex = TRUE;
        }
        else 
        {
            /* Half duplex mode. */
            full_duplex = FALSE;
        }
        /* Configure 100MBit/10MBit mode. */
        if (regv & STS_10_MBIT) 
        {
            /* 10 MBit */
            mbit_100 = FALSE;
        }
        else 
        {
            /* 100 MBit */
            mbit_100 = TRUE;
        }
    }
    LPC24XX_EMAC_lwip_SetLinkSpeed( mbit_100, full_duplex );
    return( TRUE );
}

BOOL ENET_PHY_lwip_init ( void )
{
    /* Reset PHY */
    if ( !PhyReset() )
    {
        debug_printf("Error: Phy reset\r\n");        
        return( FALSE );
    }
    
    /* Verify PHY ID */
    if ( !PhyVerifyId() )
    {
        debug_printf("Error: Invalid Phy Id\r\n");        
        return( FALSE );
    }

    /* Set the link speed. */
    if ( !PhySetLinkSpeed() )
    {
        debug_printf("Error: Link speed failure\r\n");        
        return( FALSE );
    }
    return( TRUE );
}

UINT32 ENET_PHY_lwip_get_MDC_Clk_Div(void)
{
    /* Divide value used for the MDC Clock */
    /* DP83848 Can work up to 25 MHz */
    return ( MDC_CLK_DIV_4 );
}

BOOL ENET_PHY_lwip_get_MII_mode ( void )
{
    // Indicate RMII mode */
    return ( FALSE );
}
BOOL ENET_PHY_lwip_get_link_status( void )
{
    UINT32 sts;
    BOOL link_status = FALSE;

    if( LPC24XX_EMAC_lwip_ReadPhy( DP83848C_DEF_ADR, PHY_REG_STS, &sts, MII_RD_TOUT ) )
    {
        link_status = (sts & STS_LINK_ON);
    }
    return link_status;
}

BOOL ENET_PHY_lwip_set_link_speed( void )
{
    return PhySetLinkSpeed( );
}


