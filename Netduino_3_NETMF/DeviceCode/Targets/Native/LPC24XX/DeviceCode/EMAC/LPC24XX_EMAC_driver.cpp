#include <tinyhal.h>
//#include "net_decl.h"
#include "LPC24XX_EMAC_driver.h"

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_EMAC_Driver"
#endif

LPC24XX_EMAC_Driver          g_LPC24XX_EMAC_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

extern int xn_bind_lpc24xx_emac ( int minor_number );
extern void LPC24XX_EMAC_interrupt ( void *param );

BOOL Network_Interface_Bind(int index)
{
    return LPC24XX_EMAC_Driver::Bind();
}
int  Network_Interface_Open(int index)
{
    return LPC24XX_EMAC_Driver::Open();
}
BOOL Network_Interface_Close(int index)
{
    return LPC24XX_EMAC_Driver::Close();
}

int LPC24XX_EMAC_Driver::Open()
{
    int use_default_multicast = 1;
    
    memset(&g_LPC24XX_EMAC_Driver.m_currentDhcpSession, 0, sizeof(g_LPC24XX_EMAC_Driver.m_currentDhcpSession));

    /* Open the interface first */
    g_LPC24XX_EMAC_Driver.m_interfaceNumber = xn_interface_open_config (LPC24XX_EMAC_DEVICE, 
                                                                        0,              /*  minor_number        */
                                                                        0,              /*  ioaddress           */
                                                                        0,              /*  irq value           */
                                                                        0               /*  mem_address)        */
                                                    );
    
    if (g_LPC24XX_EMAC_Driver.m_interfaceNumber == -1)
    {
        return -1;    
    }

    if (xn_interface_opt(g_LPC24XX_EMAC_Driver.m_interfaceNumber, 
                        IO_DEFAULT_MCAST,
                        (RTP_PFCCHAR)&use_default_multicast,
                        sizeof(int)) == -1)
    {
        /* Failed to set the default multicast interface */
        debug_printf("LPC24XX_EMAC_Driver::Open: Failed to set the default multicast interface\r\n");
    }
    
       
    /* Enable the INTERRUPT */                            
    CPU_INTC_ActivateInterrupt(LPC24XX_VIC::c_IRQ_INDEX_EMAC, LPC24XX_EMAC_interrupt, NULL);

    return g_LPC24XX_EMAC_Driver.m_interfaceNumber;    
}

BOOL LPC24XX_EMAC_Driver::Close( )
{
    int retVal = -1;

    CPU_INTC_DeactivateInterrupt(LPC24XX_VIC::c_IRQ_INDEX_EMAC);

    retVal = xn_interface_close(g_LPC24XX_EMAC_Driver.m_interfaceNumber);
    
    
    if (retVal == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL  LPC24XX_EMAC_Driver::Bind  ( )
{
    int retVal;
    
    retVal = xn_bind_lpc24xx_emac(0);
    
    if (retVal != -1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

