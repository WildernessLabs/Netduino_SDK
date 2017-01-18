////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include <tinyhal.h>

#include <tinyhal.h>
#include "net_decl_lwip.h"
#include "lwip\netif.h"
#include "lwip\tcp.h"
#include "lwip\tcpip.h"
#include "lwip\dhcp.h"
#include "lwip\pbuf.h"
#include "netif\etharp.h"

#include "SH7264_SMSC_lwip.h"
#include "SH7264_SMSC_lwip_adapter.h"

static struct netif            g_SH7264_SMSC_NetIF;

HAL_CONTINUATION    InterruptTaskContinuation;
HAL_COMPLETION      LwipUpTimeCompletion;
static BOOL         LwipNetworkStatus = 0;
static UINT32       LwipLastIpAddress = 0;

extern NETWORK_CONFIG g_NetworkConfig;

// -- //

void SH7264_SMSC_status_callback(struct netif *netif)
{
    if(LwipLastIpAddress != netif->ip_addr.addr)
    {
        Network_PostEvent( NETWORK_EVENT_TYPE_ADDRESS_CHANGED, 0 );
        LwipLastIpAddress = netif->ip_addr.addr;
    }
    
#if defined(_DEBUG)
    lcd_printf("\f\nLink Update: \n");
    lcd_printf("         IP: %d.%d.%d.%d\n", (netif->ip_addr.addr >> 24) & 0xFF, 
                                             (netif->ip_addr.addr >> 16) & 0xFF,
                                             (netif->ip_addr.addr >> 8) & 0xFF,
                                             (netif->ip_addr.addr >> 0) & 0xFF);
    lcd_printf("         GW: %d.%d.%d.%d\n", (netif->gw.addr >> 24) & 0xFF, 
                                             (netif->gw.addr >> 16) & 0xFF,
                                             (netif->gw.addr >> 8) & 0xFF,
                                             (netif->gw.addr >> 0) & 0xFF);

    debug_printf("\nLink Update: \r\n");
    debug_printf("         IP: %d.%d.%d.%d\r\n", (netif->ip_addr.addr >> 24) & 0xFF, 
                                             (netif->ip_addr.addr >> 16) & 0xFF,
                                             (netif->ip_addr.addr >> 8) & 0xFF,
                                             (netif->ip_addr.addr >> 0) & 0xFF);
    debug_printf("         GW: %d.%d.%d.%d\r\n", (netif->gw.addr >> 24) & 0xFF, 
                                             (netif->gw.addr >> 16) & 0xFF,
                                             (netif->gw.addr >> 8) & 0xFF,
                                             (netif->gw.addr >> 0) & 0xFF);
#endif
}

err_t SH7264_SMSC_ethhw_init( netif * myNetIf) 
{ 
    myNetIf->mtu = ETHERSIZE;

    /* ethhw_init() is user-defined */
    /* use ip_input instead of ethernet_input for non-ethernet hardware */
    /* (this function is assigned to netif.input and should be called by the hardware driver) */

    /* Assign the xmit routine to the stack's netif and call the driver's Open */

    myNetIf->output = etharp_output;
    myNetIf->linkoutput = SH7264_SMSC_lwip_xmit;
    myNetIf->status_callback = SH7264_SMSC_status_callback;

    SH7264_SMSC_lwip_open( myNetIf );

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

    BOOL status = (SMSC9218_lwip_Get_Link_Mode() != LINK_NO_LINK);

    if(status != LwipNetworkStatus)
    {
        struct netif* pNetIf = (struct netif*)arg;

        if(status)
        {
            SOCK_NetworkConfiguration *pNetCfg = &g_NetworkConfig.NetworkInterfaces[0];

            SMSC9218_lwip_Initialize_Link();
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
    InterruptTaskContinuation.InitializeCallback( (HAL_CALLBACK_FPN)SH7264_SMSC_lwip_recv, pNetIf );

    LwipUpTimeCompletion.InitializeForUserMode( (HAL_CALLBACK_FPN)lwip_network_uptime_completion, pNetIf );
    LwipUpTimeCompletion.EnqueueDelta64( 2000000 );
}

BOOL Network_Interface_Bind(int index)
{
    return SH7264_SMSC_LWIP_Driver::Bind();
}
int  Network_Interface_Open(int index)
{
    return SH7264_SMSC_LWIP_Driver::Open(index);
}
BOOL Network_Interface_Close(int index)
{
    return SH7264_SMSC_LWIP_Driver::Close();
}

int SH7264_SMSC_LWIP_Driver::Open(int index)
{
    /* Network interface variables */
    struct ip_addr ipaddr, subnetmask, gateway;
    struct netif *pNetIF;
    int len;
    const SOCK_NetworkConfiguration *iface;

    /* Apply network configuration */
    iface = &g_NetworkConfig.NetworkInterfaces[index];

    len = g_SH7264_SMSC_NetIF.hwaddr_len;
    if(len == 0 || iface->macAddressLen < len)
    {
        len = iface->macAddressLen;
        g_SH7264_SMSC_NetIF.hwaddr_len = len;
    }
    memcpy(g_SH7264_SMSC_NetIF.hwaddr, iface->macAddressBuffer, len);

    ipaddr.addr = iface->ipaddr;
    gateway.addr = iface->gateway;
    subnetmask.addr = iface->subnetmask;
 
    g_SH7264_SMSC_NetIF.flags |= NETIF_FLAG_IGMP;

    pNetIF = netif_add( &g_SH7264_SMSC_NetIF, &ipaddr, &subnetmask, &gateway, NULL, SH7264_SMSC_ethhw_init, ethernet_input );
    
    netif_set_default( pNetIF );
    LwipNetworkStatus = (SMSC9218_lwip_Get_Link_Mode() != LINK_NO_LINK);
    if (LwipNetworkStatus)
    {
        netif_set_up( pNetIF );
    }
    /* Initialize the continuation routine for the driver interrupt and receive */    
    InitContinuations( pNetIF );

    // Return LWIP's net interface number
    return g_SH7264_SMSC_NetIF.num;    

}

// -- //

BOOL SH7264_SMSC_LWIP_Driver::Close(void)
{
    LwipUpTimeCompletion.Abort();

    //CPU_INTC_DeactivateInterrupt(SH7264C_ID_EDMAC);
    CPU_INTC_DeactivateInterrupt(81);

    InterruptTaskContinuation.Abort();

    LwipNetworkStatus = 0;

    netif_set_down( &g_SH7264_SMSC_NetIF );
    netif_remove( &g_SH7264_SMSC_NetIF );

    SH7264_SMSC_lwip_close( &g_SH7264_SMSC_NetIF );
    
    memset( &g_SH7264_SMSC_NetIF, 0, sizeof g_SH7264_SMSC_NetIF);

    return TRUE;
}

BOOL SH7264_SMSC_LWIP_Driver::Bind(void)
{
    return TRUE;
}

