////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include <tinyhal.h>

#include <tinyhal.h>
#include <PAL\COM\Sockets_LWIP\sockets_lwip.h>
#include "net_decl_lwip.h"
#include "lwip\netif.h"
#include "lwip\tcp.h"
#include "lwip\tcpip.h"
#include "lwip\dhcp.h"
#include "lwip\pbuf.h"
#include "netif\etharp.h"

#include "AT91_EMAC_LWIP_adapter.h"
#include "AT91_EMAC_lwip.h"
#include "dm9161_lwip.h"

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_AT91_EMAC_LWIP_Driver"
#endif

static struct netif          g_AT91_EMAC_NetIF;

HAL_CONTINUATION    InterruptTaskContinuation;
HAL_COMPLETION      LwipUpTimeCompletion;
static UINT32       LwipNetworkStatus = 0;
static UINT32       LwipLastIpAddress = 0;

#define LWIP_STATUS_LinkUp         0x01
#define LWIP_STATUS_MultiCastInit  0x02

#define LWIP_STATUS_setorclear(x, b) LwipNetworkStatus  =  (b) ? (LwipNetworkStatus |= (x)) : (LwipNetworkStatus & ~(x))
#define LWIP_STATUS_set(x)           LwipNetworkStatus |=  (x);
#define LWIP_STATUS_clear(x)         LwipNetworkStatus &= ~(x);
#define LWIP_STATUS_isset(x)       ((LwipNetworkStatus &   (x)) != 0)

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

extern AT91_EMAC_LWIP_DRIVER_CONFIG g_AT91_EMAC_LWIP_Config;
extern NETWORK_CONFIG               g_NetworkConfig;

// -- //

void AT91_EMAC__status_callback(struct netif *netif)
{
    if(LwipLastIpAddress != netif->ip_addr.addr)
    {
        Network_PostEvent( NETWORK_EVENT_TYPE_ADDRESS_CHANGED, 0 );
        LwipLastIpAddress = netif->ip_addr.addr;
    }

#if !defined(BUILD_RTM)
    lcd_printf("\f\n\n\n\n\n\nLink Update: \n");
    lcd_printf("         IP: %d.%d.%d.%d\n", (netif->ip_addr.addr >>  0) & 0xFF, 
                                             (netif->ip_addr.addr >>  8) & 0xFF,
                                             (netif->ip_addr.addr >> 16) & 0xFF,
                                             (netif->ip_addr.addr >> 24) & 0xFF);
    lcd_printf("         GW: %d.%d.%d.%d\n", (netif->gw.addr >>  0) & 0xFF, 
                                             (netif->gw.addr >>  8) & 0xFF,
                                             (netif->gw.addr >> 16) & 0xFF,
                                             (netif->gw.addr >> 24) & 0xFF);
    debug_printf("\nLink Update: \r\n");
    debug_printf("         IP: %d.%d.%d.%d\r\n", (netif->ip_addr.addr >>  0) & 0xFF, 
                                             (netif->ip_addr.addr >>  8) & 0xFF,
                                             (netif->ip_addr.addr >> 16) & 0xFF,
                                             (netif->ip_addr.addr >> 24) & 0xFF);
    debug_printf("         GW: %d.%d.%d.%d\r\n", (netif->gw.addr >>  0) & 0xFF, 
                                             (netif->gw.addr >>  8) & 0xFF,
                                             (netif->gw.addr >> 16) & 0xFF,
                                             (netif->gw.addr >> 24) & 0xFF);
#endif
}


err_t AT91_igmp_mac_filter( struct netif *netif, struct ip_addr *group, u8_t action)
{
   if(action == IGMP_ADD_MAC_FILTER)
   {
       AT91_EMAC_AddMulticastAddr(group->addr);
   }
   return ERR_OK;
}

err_t AT91_EMAC_ethhw_init(struct netif *myNetIf) 
{ 
    myNetIf->mtu = EMAC_TX_UNITSIZE;

    /* ethhw_init() is user-defined */
    /* use ip_input instead of ethernet_input for non-ethernet hardware */
    /* (this function is assigned to netif.input and should be called by the hardware driver) */

    /* Assign the xmit routine to the stack's netif and call the driver's Open */

    myNetIf->output = etharp_output;
    myNetIf->linkoutput = AT91_EMAC_LWIP_xmit;
    myNetIf->status_callback = AT91_EMAC__status_callback;

    myNetIf->igmp_mac_filter = AT91_igmp_mac_filter;

    AT91_EMAC_LWIP_open( myNetIf );

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

    BOOL status = dm9161_lwip_GetLinkStatus( ) != 0;

    if(!status)
    {
        status = dm9161_lwip_GetLinkStatus( ) != 0;
    }

    if(status != LWIP_STATUS_isset(LWIP_STATUS_LinkUp) )
    {
        struct netif* pNetIf = (struct netif*)arg;

        if(status)
        {
            tcpip_timeout(1000, (sys_timeout_handler)netif_set_link_up, (void*)pNetIf);
            tcpip_timeout(1000, (sys_timeout_handler)netif_set_up, (void*)pNetIf);

            Network_PostEvent( NETWORK_EVENT_TYPE__AVAILABILITY_CHANGED, NETWORK_EVENT_FLAGS_IS_AVAILABLE );
        }
        else
        {
            tcpip_callback((sys_timeout_handler)netif_set_link_down, (void*)pNetIf);
            tcpip_callback((sys_timeout_handler)netif_set_down, (void*)pNetIf);

            Network_PostEvent( NETWORK_EVENT_TYPE__AVAILABILITY_CHANGED, 0);
        }

        Events_Set(SYSTEM_EVENT_FLAG_SOCKET);
        Events_Set(SYSTEM_EVENT_FLAG_NETWORK);

        LWIP_STATUS_setorclear( LWIP_STATUS_LinkUp, status );
    }

    if(LwipLastIpAddress != 0 && !LWIP_STATUS_isset(LWIP_STATUS_MultiCastInit))
    {
        LWIP_STATUS_set(LWIP_STATUS_MultiCastInit);
        Sockets_LWIP_Driver::InitializeMulticastDiscovery();
    }
    

    LwipUpTimeCompletion.EnqueueDelta64( 2000000 );
}

void InitContinuations( struct netif *pNetIf )
{
    InterruptTaskContinuation.InitializeCallback( (HAL_CALLBACK_FPN)AT91_EMAC_LWIP_recv, &g_AT91_EMAC_NetIF );

    LwipUpTimeCompletion.InitializeForUserMode( (HAL_CALLBACK_FPN)lwip_network_uptime_completion, pNetIf );
    LwipUpTimeCompletion.EnqueueDelta64( 2000000 );
}

BOOL Network_Interface_Bind(int index)
{
    return AT91_EMAC_LWIP_Driver::Bind();
}
int  Network_Interface_Open(int index)
{
    return AT91_EMAC_LWIP_Driver::Open(index);
}
BOOL Network_Interface_Close(int index)
{
    return AT91_EMAC_LWIP_Driver::Close();
}

int AT91_EMAC_LWIP_Driver::Open(int index)
{
    /* Network interface variables */
    struct ip_addr ipaddr, subnetmask, gateway;
    struct netif *pNetIF;
    int len;
    const SOCK_NetworkConfiguration *iface;

    /* Apply network configuration */
    iface = &g_NetworkConfig.NetworkInterfaces[index];

    if(0 == (iface->flags & SOCK_NETWORKCONFIGURATION_FLAGS_DHCP))
    {
        ipaddr.addr     = iface->ipaddr;
        gateway.addr    = iface->gateway;
        subnetmask.addr = iface->subnetmask;
    }
    else
    {
        /* Set network address variables - this will be set by either DHCP or when the configuration is applied */
        IP4_ADDR(&gateway, 0,0,0,0);
        IP4_ADDR(&ipaddr, 0,0,0,0);
        IP4_ADDR(&subnetmask, 255,255,255,0);
    }

    len = g_AT91_EMAC_NetIF.hwaddr_len;

    if(len == 0 || iface->macAddressLen < len)
    {
        len = iface->macAddressLen;
        g_AT91_EMAC_NetIF.hwaddr_len = len;
    }

    memcpy(g_AT91_EMAC_NetIF.hwaddr, iface->macAddressBuffer, len);

    // PHY Power Up
    CPU_GPIO_EnableOutputPin(g_AT91_EMAC_LWIP_Config.PHY_PD_GPIO_Pin, FALSE);

    // Enable Interrupt
    CPU_INTC_ActivateInterrupt(AT91C_ID_EMAC, (HAL_CALLBACK_FPN)AT91_EMAC_LWIP_interrupt, &g_AT91_EMAC_NetIF);

    g_AT91_EMAC_NetIF.flags = NETIF_FLAG_IGMP | NETIF_FLAG_BROADCAST;

    pNetIF = netif_add( &g_AT91_EMAC_NetIF, &ipaddr, &subnetmask, &gateway, NULL, AT91_EMAC_ethhw_init, ethernet_input );

    netif_set_default( pNetIF );

    LWIP_STATUS_setorclear( LWIP_STATUS_LinkUp, 0 != dm9161_lwip_GetLinkStatus( ) );

    if (LWIP_STATUS_isset(LWIP_STATUS_LinkUp))
    {
        netif_set_link_up( pNetIF );
        netif_set_up     ( pNetIF );

        Network_PostEvent( NETWORK_EVENT_TYPE__AVAILABILITY_CHANGED, NETWORK_EVENT_FLAGS_IS_AVAILABLE );
    }

    /* Initialize the continuation routine for the driver interrupt and receive */    
    InitContinuations( pNetIF );

    return g_AT91_EMAC_NetIF.num;
}

// -- //

BOOL AT91_EMAC_LWIP_Driver::Close(void)
{
    LwipUpTimeCompletion.Abort();

    netif_set_down( &g_AT91_EMAC_NetIF );
    netif_remove( &g_AT91_EMAC_NetIF );

    CPU_INTC_DeactivateInterrupt(AT91C_ID_EMAC);

    LwipNetworkStatus = 0;

    AT91_EMAC_LWIP_close( &g_AT91_EMAC_NetIF );

    CPU_GPIO_SetPinState(g_AT91_EMAC_LWIP_Config.PHY_PD_GPIO_Pin, TRUE);

    InterruptTaskContinuation.Abort();

    LwipLastIpAddress = 0;    
    
    memset( &g_AT91_EMAC_NetIF, 0, sizeof g_AT91_EMAC_NetIF );

    return TRUE;
}

BOOL AT91_EMAC_LWIP_Driver::Bind(void)
{
    return TRUE;
}

