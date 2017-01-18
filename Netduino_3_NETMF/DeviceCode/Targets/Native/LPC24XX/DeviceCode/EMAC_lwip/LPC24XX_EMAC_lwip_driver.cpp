#include <tinyhal.h>
#include "net_decl_lwip.h"
#include "lwip\netif.h"
#include "lwip\tcp.h"
#include "lwip\tcpip.h"
#include "lwip\dhcp.h"
#include "lwip\pbuf.h"
#include "netif\etharp.h"

#include "LPC24XX_EMAC_lwip.h"
#include "LPC24XX_EMAC_lwip_driver.h"

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_EMAC_LWIP_Driver"
#endif

static LPC24XX_EMAC_LWIP_Driver g_LPC24XX_EMAC_LWIP_Driver;
static struct netif             g_LPC24XX_EMAC_NetIF;

HAL_CONTINUATION    InterruptTaskContinuation;
HAL_COMPLETION      LwipUpTimeCompletion;
static BOOL         LwipNetworkStatus = 0;
static UINT32       LwipLastIpAddress = 0;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

extern NETWORK_CONFIG g_NetworkConfig;

extern BOOL ENET_PHY_lwip_get_link_status( void );
extern BOOL ENET_PHY_lwip_set_link_speed( void );

void lpc24xx_status_callback(struct netif *netif)
{
    if(LwipLastIpAddress != netif->ip_addr.addr)
    {
        Network_PostEvent( NETWORK_EVENT_TYPE_ADDRESS_CHANGED, 0 );
        LwipLastIpAddress = netif->ip_addr.addr;
    }
    
#if defined(_DEBUG)
    lcd_printf("\f\nLink Update: \n");
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

err_t   lpc24xx_ethhw_init( netif * myNetIf) 
{ 
    myNetIf->mtu = ETH_MAX_FLEN;

    /* ethhw_init() is user-defined */
    /* use ip_input instead of ethernet_input for non-ethernet hardware */
    /* (this function is assigned to netif.input and should be called by the hardware driver) */

    /* Assign the xmit routine to the stack's netif and call the driver's Open */

    myNetIf->output = etharp_output;
    myNetIf->linkoutput = LPC24XX_EMAC_lwip_xmit;
    myNetIf->status_callback = lpc24xx_status_callback;

    LPC24XX_EMAC_lwip_open( myNetIf );

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

    BOOL status = ENET_PHY_lwip_get_link_status( );

    if(status != LwipNetworkStatus)
    {
        struct netif* pNetIf = (struct netif*)arg;

        if(status)
        {
            SOCK_NetworkConfiguration *pNetCfg = &g_NetworkConfig.NetworkInterfaces[0];

            ENET_PHY_lwip_set_link_speed( );
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
    InterruptTaskContinuation.InitializeCallback( (HAL_CALLBACK_FPN)LPC24XX_EMAC_lwip_recv, pNetIf );

    LwipUpTimeCompletion.InitializeForUserMode( (HAL_CALLBACK_FPN)lwip_network_uptime_completion, pNetIf );
    LwipUpTimeCompletion.EnqueueDelta64( 2000000 );
}

BOOL Network_Interface_Bind(int index)
{
    return LPC24XX_EMAC_LWIP_Driver::Bind();
}
int  Network_Interface_Open(int index)
{
    return LPC24XX_EMAC_LWIP_Driver::Open(index);
}
BOOL Network_Interface_Close(int index)
{
    return LPC24XX_EMAC_LWIP_Driver::Close();
}

int LPC24XX_EMAC_LWIP_Driver::Open(int index)
{    
    /* Network interface variables */
    struct ip_addr ipaddr, subnetmask, gateway;
    struct netif *pNetIF;
    int len;
    const SOCK_NetworkConfiguration *iface;

    /* Apply network configuration */
    iface = &g_NetworkConfig.NetworkInterfaces[index];

    len = g_LPC24XX_EMAC_NetIF.hwaddr_len;
    if(len == 0 || iface->macAddressLen < len)
    {
        len = iface->macAddressLen;
        g_LPC24XX_EMAC_NetIF.hwaddr_len = len;
    }
    memcpy(g_LPC24XX_EMAC_NetIF.hwaddr, iface->macAddressBuffer, len);

    ipaddr.addr = iface->ipaddr;
    gateway.addr = iface->gateway;
    subnetmask.addr = iface->subnetmask;

    pNetIF = netif_add( &g_LPC24XX_EMAC_NetIF, &ipaddr, &subnetmask, &gateway, NULL, lpc24xx_ethhw_init, ethernet_input );
       
    /* Enable the INTERRUPT */                            
    CPU_INTC_ActivateInterrupt(LPC24XX_VIC::c_IRQ_INDEX_EMAC, (HAL_CALLBACK_FPN)LPC24XX_EMAC_lwip_interrupt, &g_LPC24XX_EMAC_NetIF);

    netif_set_default( pNetIF );
    LwipNetworkStatus = ENET_PHY_lwip_get_link_status( );
    if (LwipNetworkStatus)
    {
        netif_set_up( pNetIF );
    }
    /* Initialize the continuation routine for the driver interrupt and receive */    
    InitContinuations( pNetIF );

    // Return LWIP's net interface number
    return g_LPC24XX_EMAC_NetIF.num;    
}

BOOL LPC24XX_EMAC_LWIP_Driver::Close( )
{
    LwipUpTimeCompletion.Abort();

    /* Disable the interrupt */
    CPU_INTC_DeactivateInterrupt(LPC24XX_VIC::c_IRQ_INDEX_EMAC);

    InterruptTaskContinuation.Abort();

    LwipNetworkStatus = 0;

    netif_set_down( &g_LPC24XX_EMAC_NetIF );
    netif_remove( &g_LPC24XX_EMAC_NetIF );

    LPC24XX_EMAC_lwip_close( &g_LPC24XX_EMAC_NetIF );
    
    memset( &g_LPC24XX_EMAC_NetIF, 0, sizeof g_LPC24XX_EMAC_NetIF);

    return TRUE;
}

BOOL  LPC24XX_EMAC_LWIP_Driver::Bind  ( )
{
    return TRUE;
}

