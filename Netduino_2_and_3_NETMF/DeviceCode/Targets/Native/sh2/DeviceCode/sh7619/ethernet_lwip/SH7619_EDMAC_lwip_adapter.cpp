////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include <tinyhal.h>
#include <tinyhal.h>
#include <tinyhal.h>
#include "net_decl_lwip.h"
#include "lwip\netif.h"
#include "lwip\tcp.h"
#include "lwip\tcpip.h"
#include "lwip\dhcp.h"
#include "lwip\pbuf.h"
#include "netif\etharp.h"

#include "SH7619_EDMAC_LWIP_Adapter.h"
#include "SH7619_EDMAC_LWIP.h"


SH7619_EDMAC_LWIP_Driver g_SH7619_EDMAC_LWIP_Driver;


struct netif          g_SH7619_EDMAC_NetIF;

HAL_CONTINUATION    InterruptTaskContinuation;
HAL_COMPLETION      LwipUpTimeCompletion;
static BOOL         LwipNetworkStatus = 0;
static UINT32       LwipLastIpAddress = 0;


// extern SH7619_EDMAC_LWIP_DRIVER_CONFIG g_SH7619_EDMAC_LWIP_Config;

extern NETWORK_CONFIG               g_NetworkConfig;



extern void SH7619_EDMAC_LWIP_interrupt(void *param);
//--//


void SH7619_EDMAC__status_callback(struct netif *netif)
{
    if(LwipLastIpAddress != netif->ip_addr.addr)
    {
        Network_PostEvent( NETWORK_EVENT_TYPE_ADDRESS_CHANGED, 0 );
        LwipLastIpAddress = netif->ip_addr.addr;
    }


    #if defined(DEBUG)
/*    
    lcd_printf("\f\n\n\n\n\n\nLink Update: \n");
    lcd_printf("         IP: %d.%d.%d.%d\n", (netif->ip_addr.addr >>  0) & 0xFF, 
                                             (netif->ip_addr.addr >>  8) & 0xFF,
                                             (netif->ip_addr.addr >> 16) & 0xFF,
                                             (netif->ip_addr.addr >> 24) & 0xFF);
    lcd_printf("         GW: %d.%d.%d.%d\n", (netif->gw.addr >>  0) & 0xFF, 
                                             (netif->gw.addr >>  8) & 0xFF,
                                             (netif->gw.addr >> 16) & 0xFF,
                                             (netif->gw.addr >> 24) & 0xFF);
*/
    hal_printf("\nLink Update: \r\n");
    hal_printf("         IP: %d.%d.%d.%d\r\n", (netif->ip_addr.addr >> 24) & 0xFF, 
                                             (netif->ip_addr.addr >>  16) & 0xFF,
                                             (netif->ip_addr.addr >> 8) & 0xFF,
                                             (netif->ip_addr.addr >> 0) & 0xFF);
    hal_printf("         GW: %d.%d.%d.%d\r\n", (netif->gw.addr >>  24) & 0xFF, 
                                             (netif->gw.addr >>  16) & 0xFF,
                                             (netif->gw.addr >> 8 & 0xFF,
                                             (netif->gw.addr >> 0) & 0xFF);

    #endif
}
err_t SH7619_EDMAC_ethhw_init(struct netif *myNetIf) 
{ 
    myNetIf->mtu = SH7619_EDMAC_MAX_FRAME_SIZE;

    /* ethhw_init() is user-defined */
    /* use ip_input instead of ethernet_input for non-ethernet hardware */
    /* (this function is assigned to netif.input and should be called by the hardware driver) */

    /* Assign the xmit routine to the stack's netif and call the driver's Open */

    myNetIf->output = etharp_output;
    myNetIf->linkoutput = SH7619_EDMAC_LWIP_xmit;
    myNetIf->status_callback = SH7619_EDMAC__status_callback;

    SH7619_EDMAC_LWIP_open( myNetIf );

    return 0; 
}

void lwip_interrupt_continuation( void )
{
    NATIVE_PROFILE_PAL_NETWORK();
    GLOBAL_LOCK(irq);
    
    if(!InterruptTaskContinuation.IsLinked())
    {
        InterruptTaskContinuation.Enqueue();
    }
}


void lwip_network_uptime_completion(void *arg)
{
    NATIVE_PROFILE_PAL_NETWORK();

    BOOL status = SH7619_EDMAC_LWIP_GetLinkStatus( );

    if(status != LwipNetworkStatus)
    {
        struct netif* pNetIf = (struct netif*)arg;

        if(status)
        {
            SOCK_NetworkConfiguration *pNetCfg = &g_NetworkConfig.NetworkInterfaces[0];

            SH7619_EDMAC_LWIP_AutoNegotiate( );
            netif_set_up( pNetIf );

            if(pNetCfg->flags & SOCK_NETWORKCONFIGURATION_FLAGS_DHCP)
            {
                dhcp_start( pNetIf );
            }
            Network_PostEvent( NETWORK_EVENT_TYPE__AVAILABILITY_CHANGED, NETWORK_EVENT_FLAGS_IS_AVAILABLE );
        }
        else
        {
            netif_set_down( (struct netif*)arg );
            Network_PostEvent( NETWORK_EVENT_TYPE__AVAILABILITY_CHANGED, 0);
        }

        LwipNetworkStatus = status;
    }

    LwipUpTimeCompletion.EnqueueDelta64( 2000000 );
}



void InitContinuations( struct netif *pNetIf )
{
 
    InterruptTaskContinuation.InitializeCallback( (HAL_CALLBACK_FPN)SH7619_EDMAC_LWIP_recv, pNetIf );
    LwipUpTimeCompletion.InitializeForUserMode( (HAL_CALLBACK_FPN)lwip_network_uptime_completion, pNetIf );
    LwipUpTimeCompletion.EnqueueDelta64( 2000000 );

}
// -- //

BOOL Network_Interface_Bind(int index)
{
    return SH7619_EDMAC_LWIP_Driver::Bind();
}
int  Network_Interface_Open(int index)
{
    return SH7619_EDMAC_LWIP_Driver::Open(index);
}
BOOL Network_Interface_Close(int index)
{
    return SH7619_EDMAC_LWIP_Driver::Close();
}

int SH7619_EDMAC_LWIP_Driver::Open(int index)
{
    /* Network interface variables */
    struct ip_addr ipaddr, subnetmask, gateway;
    struct netif *pNetIF;
    int len;
    const SOCK_NetworkConfiguration *iface;

    /* Clear DHCP information */

// --//

    /* Apply network configuration */
    iface = &g_NetworkConfig.NetworkInterfaces[index];

    len = g_SH7619_EDMAC_NetIF.hwaddr_len;
    if(len == 0 || iface->macAddressLen < len)
    {
        len = iface->macAddressLen;
        g_SH7619_EDMAC_NetIF.hwaddr_len = len;
    }
    memcpy(g_SH7619_EDMAC_NetIF.hwaddr, iface->macAddressBuffer, len);

    ipaddr.addr     = iface->ipaddr;
    gateway.addr    = iface->gateway;
    subnetmask.addr = iface->subnetmask;

    // PHY Power Up
  //  CPU_GPIO_EnableOutputPin(g_AT91_EMAC_LWIP_Config.PHY_PD_GPIO_Pin, FALSE);

    // Enable Interrupt
 //   CPU_INTC_ActivateInterrupt(AT91C_ID_EMAC, (HAL_CALLBACK_FPN)AT91_EMAC_LWIP_interrupt, &g_AT91_EMAC_NetIF);

    g_SH7619_EDMAC_NetIF.flags |= NETIF_FLAG_IGMP;
 
    pNetIF = netif_add( &g_SH7619_EDMAC_NetIF, &ipaddr, &subnetmask, &gateway, NULL, SH7619_EDMAC_ethhw_init, ethernet_input );

    netif_set_default( pNetIF );
    LwipNetworkStatus = SH7619_EDMAC_LWIP_GetLinkStatus();
    if (LwipNetworkStatus)
    {
        netif_set_up( pNetIF );
    }
    /* Initialize the continuation routine for the driver interrupt and receive */    
    InitContinuations( pNetIF );

    return g_SH7619_EDMAC_NetIF.num;
}

// -- //

BOOL SH7619_EDMAC_LWIP_Driver::Close(void)
{
    int retVal = -1;

    LwipUpTimeCompletion.Abort();

    //CPU_INTC_DeactivateInterrupt(SH7619C_ID_EDMAC);
	CPU_INTC_DeactivateInterrupt(81);
    InterruptTaskContinuation.Abort();

    LwipNetworkStatus = 0;


    netif_set_down( &g_SH7619_EDMAC_NetIF );
    netif_remove( &g_SH7619_EDMAC_NetIF );

    SH7619_EDMAC_LWIP_close( &g_SH7619_EDMAC_NetIF );
    
    memset( &g_SH7619_EDMAC_NetIF, 0, sizeof g_SH7619_EDMAC_NetIF);

    return TRUE;

    
}

BOOL SH7619_EDMAC_LWIP_Driver::Bind(void)
{
    return TRUE;
}

