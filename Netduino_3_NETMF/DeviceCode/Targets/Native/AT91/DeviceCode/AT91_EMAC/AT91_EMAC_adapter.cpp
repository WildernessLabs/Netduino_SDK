////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include <tinyhal.h>

#include <tinyhal.h>
#include "AT91_EMAC_Adapter.h"

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_AT91_EMAC_Driver"
#endif

AT91_EMAC_Driver g_AT91_EMAC_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

extern AT91_EMAC_DRIVER_CONFIG g_AT91_EMAC_Config;

extern void AT91_EMAC_interrupt(void *param);
// -- //

BOOL Network_Interface_Bind(int index)
{
    return AT91_EMAC_Driver::Bind();
}
int  Network_Interface_Open(int index)
{
    return AT91_EMAC_Driver::Open();
}
BOOL Network_Interface_Close(int index)
{
    return AT91_EMAC_Driver::Close();
}

int AT91_EMAC_Driver::Open()
{
    int use_default_multicast;

    memset(&g_AT91_EMAC_Driver.m_currentDhcpSession, 0, sizeof(g_AT91_EMAC_Driver.m_currentDhcpSession));

    // PHY Power Up
    CPU_GPIO_EnableOutputPin(g_AT91_EMAC_Config.PHY_PD_GPIO_Pin, FALSE);

     /* Open the interface first */
    g_AT91_EMAC_Driver.m_interfaceNumber = xn_interface_open_config(AT91EMAC_DEVICE, 
                                                    0,              /*  minor_number        */
                                                    0,              /*  ioaddress           */
                                                    0,              /*  irq value           */
                                                    0               /*  mem_address)        */
                                                    );

    if (g_AT91_EMAC_Driver.m_interfaceNumber == -1)
    {
        return -1;    
    }
    
    use_default_multicast = 1;
    if (xn_interface_opt(g_AT91_EMAC_Driver.m_interfaceNumber, 
                        IO_DEFAULT_MCAST,
                        (RTP_PFCCHAR)&use_default_multicast,
                        sizeof(int)) == -1)
    {
        /* Failed to set the default multicast interface */
        debug_printf("EMAC: Failed to set the default multicast interface\r\n");
    }

    CPU_INTC_ActivateInterrupt(AT91C_ID_EMAC, AT91_EMAC_interrupt, NULL);

    return g_AT91_EMAC_Driver.m_interfaceNumber;
}

// -- //

BOOL AT91_EMAC_Driver::Close(void)
{
    int retVal = -1;

    CPU_INTC_DeactivateInterrupt(AT91C_ID_EMAC);

    /* JRT - changed interface number from 0 */
    retVal = xn_interface_close(g_AT91_EMAC_Driver.m_interfaceNumber);
    
    
    if (retVal == 0)
    {
        /* JRT - Wait not necessary since just did a HARD_CLOSE above
        xn_wait_pkts_output(RTP_TRUE, 60);
        */
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL AT91_EMAC_Driver::Bind(void)
{
    int retVal;
    
    retVal = xn_bind_at91_mac(0);

    if (retVal != -1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

