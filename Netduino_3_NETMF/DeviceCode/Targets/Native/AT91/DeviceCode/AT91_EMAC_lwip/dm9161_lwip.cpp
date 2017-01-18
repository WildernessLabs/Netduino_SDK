#include <tinyhal.h>
#include "net_decl_lwip.h"
#include "lwip\netif.h"
#include "AT91_EMAC_lwip.h"
#include "dm9161_lwip.h"

#define INVALID_PHY_ADDR 0xFF

static UINT32 g_phyAddress = INVALID_PHY_ADDR;

BOOL dm9161_lwip_mii_Init()
{
    UINT32 pin;

    g_phyAddress = INVALID_PHY_ADDR;

    for(pin = 0; pin < ARRAYSIZE(c_EMAC_MII); pin++)
    {
        CPU_GPIO_DisablePin( c_EMAC_MII[pin], RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
    }

    return AT91_EMAC_LWIP_SetMdcClock(SYSTEM_PERIPHERAL_CLOCK_HZ);
}

UINT32 dm9161_lwip_FindValidPhy()
{
    UINT32 phyAddress = AT91C_PHY_ADDR;
    UINT32 retryMax = 1000;    // At least 1, should not be 0
    UINT32 value;
    UINT32 rc = INVALID_PHY_ADDR;
    UINT32 cnt = 32;

    do
    {
        AT91_EMAC_LWIP_ReadPhy(phyAddress, DM9161_BMSR, &value, retryMax);
        if (value != 0x0000 && value != 0xFFFF)
        {
            rc = phyAddress;
            break;
        }
        phyAddress = (phyAddress + 1) & 0x1F;

    }while(--cnt);

    if (rc != INVALID_PHY_ADDR)
    {
        debug_printf( "Valid PHY Found: %d\r\n", rc);
    }

    return rc;
}

BOOL dm9161_lwip_AutoNegotiate()
{
    UINT32 retryCount;
    UINT32 retryMax = 100000;    // At least 1, should not be 0
    UINT32 value;
    UINT32 phyAnar;
    UINT32 phyAnalpar;
    BOOL rc = TRUE;

    debug_printf("DM9161_AutoNegotiate\r\n");

    AT91_EMAC_LWIP_EnableMdio();

    g_phyAddress = dm9161_lwip_FindValidPhy();

    if (g_phyAddress == INVALID_PHY_ADDR)
    {
        debug_printf("Error: PHY Access fail\r\n");
        rc = FALSE;
        goto AutoNegotiateExit;
    }

    rc = AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_PHYID1, &value, retryMax);
    if (!rc)
    {
        debug_printf("Error: EMAC_ReadPhy Id1\r\n");
        goto AutoNegotiateExit;
    }

    rc = AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_PHYID2, &phyAnar, retryMax);
    if (!rc)
    {
        debug_printf("Error: EMAC_ReadPhy Id2\r\n");
        goto AutoNegotiateExit;
    }

    value = (value << 6) | ((phyAnar >> 10) & DM9161_LSB_MASK);
    if( value == DM9161_PHYID1_OUI )
    {
        debug_printf("PHY: Vendor Number Model = 0x%X\r\n", ((phyAnar >> 4) & 0x3F));
        debug_printf("PHY: Model Revision Number = 0x%X\r\n", (phyAnar & 0x7));
    }

    // Setup control register
    rc  = AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMCR, &value, retryMax)|
          AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMCR, &value, retryMax);
    if (!rc)
        goto AutoNegotiateExit;
    
    value &= ~DM9161_AUTONEG;   // Remove autonegotiation enable
    value |=  DM9161_ISOLATE;   // Electrically isolate PHY
    rc = AT91_EMAC_LWIP_WritePhy(g_phyAddress, DM9161_BMCR, value, retryMax);
    if (!rc)
        goto AutoNegotiateExit;

    // Set the Auto_negotiation Advertisement Register
    // MII advertising for Next page
    // 100BaseTxFD and HD, 10BaseTFD and HD, IEEE 802.3
    phyAnar = DM9161_NP | DM9161_TX_FDX | DM9161_TX_HDX |
              DM9161_10_FDX | DM9161_10_HDX | DM9161_AN_IEEE_802_3;
    rc = AT91_EMAC_LWIP_WritePhy(g_phyAddress, DM9161_ANAR, phyAnar, retryMax);
    if (!rc)
        goto AutoNegotiateExit;

    // Read & modify control register
    rc  = AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMCR, &value, retryMax)|
          AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMCR, &value, retryMax);
    if (!rc)
        goto AutoNegotiateExit;

    value |= DM9161_SPEED_SELECT | DM9161_AUTONEG | DM9161_DUPLEX_MODE;
    rc = AT91_EMAC_LWIP_WritePhy(g_phyAddress, DM9161_BMCR, value, retryMax);
    if (!rc)
        goto AutoNegotiateExit;
    
    // Restart Auto_negotiation
    value |=  DM9161_RESTART_AUTONEG;
    value &= ~DM9161_ISOLATE;
    rc = AT91_EMAC_LWIP_WritePhy(g_phyAddress, DM9161_BMCR, value, retryMax);
    if (!rc)
        goto AutoNegotiateExit;

    // Check AutoNegotiate complete
    retryCount = 1000;
    while (retryCount--)
    {
        rc  = AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMSR, &value, retryMax)|
              AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMSR, &value, retryMax);
        if (!rc)
        {
            debug_printf("Error: AutoNegociate failed\r\n");
            goto AutoNegotiateExit;
        }
        // Done successfully
        if (value & DM9161_AUTONEG_COMP)
        {
            debug_printf("AutoNegotiate complete\r\n");
            break;
        }
        // Timeout check
        if (retryCount == 0)
        {
            AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_DSCSR   , &value, retryMax);
            debug_printf("DM9161_DSCSR = 0x%X\r\n", value);
            AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMCR    , &value, retryMax);
            debug_printf("DM9161_BMCR = 0x%X\r\n", value);
            AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_DSCR    , &value, retryMax);
            debug_printf("DM9161_DSCR = 0x%X\r\n", value);
            
            rc = FALSE;
            debug_printf("Error: AutoNegotiate TimeOut\r\n");
            goto AutoNegotiateExit;
        }
    }

    // Get the AutoNeg Link partner base page
    rc  = AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_ANLPAR, &phyAnalpar, retryMax)|
          AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_ANLPAR, &phyAnalpar, retryMax);
    if (rc == FALSE)
        goto AutoNegotiateExit;

    // Setup the EMAC link speed
    if ((phyAnar & phyAnalpar) & DM9161_TX_FDX)
    {
        // set MII for 100BaseTX and Full Duplex
        AT91_EMAC_LWIP_SetLinkSpeed(TRUE, TRUE);
    }
    else if ((phyAnar & phyAnalpar) & DM9161_10_FDX)
    {
        // set MII for 10BaseT and Full Duplex
        AT91_EMAC_LWIP_SetLinkSpeed(FALSE, TRUE);
    }
    else if ((phyAnar & phyAnalpar) & DM9161_TX_HDX)
    {
        // set MII for 100BaseTX and half Duplex
        AT91_EMAC_LWIP_SetLinkSpeed(TRUE, FALSE);
    }
    else if ((phyAnar & phyAnalpar) & DM9161_10_HDX)
    {
        // set MII for 10BaseT and half Duplex
        AT91_EMAC_LWIP_SetLinkSpeed(FALSE, FALSE);
    }

    // Setup EMAC mode
    AT91_EMAC_LWIP_EnableMII();

AutoNegotiateExit:
    AT91_EMAC_LWIP_DisableMdio();
    return rc;
}

BOOL dm9161_lwip_GetLinkSpeed(BOOL applySetting)
{
    UINT32 retryMax = 100000;    // At least 1, should not be 0
    UINT32 stat1;
    UINT32 stat2;
    BOOL rc = TRUE;

    AT91_EMAC_LWIP_EnableMdio();
    
    rc  = AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMSR, &stat1, retryMax)|
    AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMSR, &stat1, retryMax);
    if (!rc)
        goto GetLinkSpeedExit;

    if ((stat1 & DM9161_LINK_STATUS) == 0)
    {
        rc = FALSE;
        goto GetLinkSpeedExit;
    }

    if (!applySetting)
        goto GetLinkSpeedExit;
    
    // Re-configure Link speed
    rc  = AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_DSCSR, &stat2, retryMax)|
    AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_DSCSR, &stat2, retryMax);
    if (!rc)
        goto GetLinkSpeedExit;

    if ((stat1 & DM9161_100BASE_TX_FD) && (stat2 & DM9161_100FDX))
    {
        // set Emac for 100BaseTX and Full Duplex
        AT91_EMAC_LWIP_SetLinkSpeed(TRUE, TRUE);
    }

    if ((stat1 & DM9161_10BASE_T_FD) && (stat2 & DM9161_10FDX)) 
    {
        // set MII for 10BaseT and Full Duplex
        AT91_EMAC_LWIP_SetLinkSpeed(FALSE, TRUE);
    }

    if ((stat1 & DM9161_100BASE_T4_HD) && (stat2 & DM9161_100HDX))
    {
        // set MII for 100BaseTX and Half Duplex
        AT91_EMAC_LWIP_SetLinkSpeed(TRUE, FALSE);
    }

    if ((stat1 & DM9161_10BASE_T_HD) && (stat2 & DM9161_10HDX))
    {
        // set MII for 10BaseT and Half Duplex
        AT91_EMAC_LWIP_SetLinkSpeed(FALSE, FALSE);
    }

    debug_printf("DM9161_GetLinkSpeed passed\r\n");

GetLinkSpeedExit:
    AT91_EMAC_LWIP_DisableMdio();
    return rc;
}

BOOL dm9161_lwip_GetLinkStatus(void)
{
    UINT32 bmsr = 0;
    UINT32 retryMax = 100000;
    BOOL link_status = FALSE;

    if (g_phyAddress == INVALID_PHY_ADDR)
    {
        g_phyAddress == dm9161_lwip_FindValidPhy( );
    }

    AT91_EMAC_LWIP_EnableMdio();

    if (AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMSR, &bmsr, retryMax) ||
        AT91_EMAC_LWIP_ReadPhy(g_phyAddress, DM9161_BMSR, &bmsr, retryMax))
    {
        link_status = (bmsr & DM9161_LINK_STATUS);
    }

    AT91_EMAC_LWIP_DisableMdio();

    return link_status;
}


