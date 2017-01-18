////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RTIP_sockets.h"
#include "net_decl.h"
#include "xnconf.h"
#include "rtipconf.h"
#include "rtipapi.h"
#include "socket.h"
#include "pollos.h"
#include "bget.h"
#include "rtpnet.h"
#include "dhcpcapi.h"
#include "loopback_driver.h"
#include "rtipext.h"
#include "rtpnet.h"
/* JRT */
#include "rtpprint.h"

//--//
/* JRT */
extern int default_mcast_iface;

extern const HAL_CONFIG_BLOCK   g_NetworkConfigHeader;
extern LOOPBACK_Driver          g_LOOPBACK_Driver;
extern NETWORK_CONFIG           g_NetworkConfig;

//--//

#if defined(DEBUG)
#define DEBUG_HANDLE_SOCKET_ERROR(t,a) \
    int iErr = xn_getlasterror();                               \
    const char* textErr = xn_geterror_string(iErr);             \
    debug_printf( "%s failed! %d: %s\r\n", t, iErr, textErr )
#else
#define DEBUG_HANDLE_SOCKET_ERROR(t,a) 
#endif

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_RTIP_SOCKETS_Driver"
#endif

RTIP_SOCKETS_Driver g_RTIP_SOCKETS_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL RTIP_SOCKETS_Driver::Initialize()
{
    NATIVE_PROFILE_PAL_NETWORK();
    IFACE_INFO info;

    memset(g_RTIP_SOCKETS_Driver.m_interfaces, 0, sizeof(g_RTIP_SOCKETS_Driver.m_interfaces));

    /* Initialize the network stack   */
    if (rtp_net_init() != 0)
    {
        DEBUG_HANDLE_SOCKET_ERROR("rtp_net_init", TRUE);
        return FALSE;
    }

#if defined(NETWORK_USE_LOOPBACK)
    // Bind and Open the loopback driver
    g_LOOPBACK_Driver.Bind();
    
    if (g_LOOPBACK_Driver.Open() == SOCK_SOCKET_ERROR)
    {
        DEBUG_HANDLE_SOCKET_ERROR("loopback init", FALSE);
    }
#endif        

    for(int i=0; i<g_NetworkConfig.NetworkInterfaceCount; i++)
    {
        int interfaceNumber;
        
        SOCK_NetworkConfiguration *pNetCfg = &g_NetworkConfig.NetworkInterfaces[i];

        Network_Interface_Bind(i);

        interfaceNumber = Network_Interface_Open(i);
        
        if (interfaceNumber == SOCK_SOCKET_ERROR)
        {
            DEBUG_HANDLE_SOCKET_ERROR("Network init", FALSE);
            debug_printf("SocketError: %d\n", xn_getlasterror());
            continue;
        }

        g_RTIP_SOCKETS_Driver.m_interfaces[i].m_interfaceNumber = interfaceNumber;

        
        UpdateAdapterConfiguration(i, SOCK_NETWORKCONFIGURATION_UPDATE_DHCP | SOCK_NETWORKCONFIGURATION_UPDATE_DNS, pNetCfg);

        // default debugger interface
        if(0 == i)
        {
            // add multicast addresses to the routing table
            UINT32 mcast1 = SOCK_htonl(SOCK_DISCOVERY_MULTICAST_IPADDR);
            UINT32 mcast2 = SOCK_htonl(SOCK_DISCOVERY_MULTICAST_IPADDR_SND);
            UINT32 mask   = SOCK_htonl(SOCK_MAKE_IP_ADDR(255,255,255,255));
            
            if(SOCK_SOCKET_ERROR == xn_rt_add((RTP_PFCUINT8)&mcast1, (RTP_PFUINT8)&mask, (RTP_PFUINT8)0, RT_USEIFACEMETRIC, interfaceNumber, RT_INF))
            {
                DEBUG_HANDLE_SOCKET_ERROR("Multicast xn_rt_add (recv)", FALSE);
            }
            if(SOCK_SOCKET_ERROR == xn_rt_add((RTP_PFCUINT8)&mcast2, (RTP_PFUINT8)&mask, (RTP_PFUINT8)0, RT_USEIFACEMETRIC, interfaceNumber, RT_INF))
            {
                DEBUG_HANDLE_SOCKET_ERROR("Multicast xn_rt_add (send)", FALSE);
            }

            /* JRT - TBD call xn_ip_set_option */
            default_mcast_iface = interfaceNumber;
            
            xn_interface_info(interfaceNumber, &info );

            debug_printf( "ip address from interface info: %d.%d.%d.%d\r\n", (UINT32)info.my_ip_address[0], 
                                                                             (UINT32)info.my_ip_address[1],                                                                 
                                                                             (UINT32)info.my_ip_address[2], 
                                                                             (UINT32)info.my_ip_address[3] );
            
            debug_printf( "mac addrress from interface info: %x.%x.%x.%x.%x.%x\r\n", (UINT32)info.my_ethernet_address[0], 
                                                                                     (UINT32)info.my_ethernet_address[1],                                                                 
                                                                                     (UINT32)info.my_ethernet_address[2], 
                                                                                     (UINT32)info.my_ethernet_address[3],                                                            
                                                                                     (UINT32)info.my_ethernet_address[4], 
                                                                                     (UINT32)info.my_ethernet_address[5] );
        }
    }
    
    return TRUE;
}

BOOL RTIP_SOCKETS_Driver::Uninitialize()
{
    NATIVE_PROFILE_PAL_NETWORK();
    const int c_exitTimeout = 1; // secs
    int exitRetries         = 3; 

    bool fEnabled = INTERRUPTS_ENABLED_STATE();

    if(!fEnabled) ENABLE_INTERRUPTS();

    while(exitRetries--)
    {
        if(!xn_wait_pkts_output( RTP_TRUE, c_exitTimeout * rtp_get_ticks_p_sec() )) break;

        while(HAL_CONTINUATION::Dequeue_And_Execute());
    }

    if(!fEnabled) DISABLE_INTERRUPTS();

#if defined(NETWORK_USE_LOOPBACK)
    // close the loopback driver
    g_LOOPBACK_Driver.Close();
#endif        

    for(int i=0; i<g_NetworkConfig.NetworkInterfaceCount; i++)
    {
    
        UpdateAdapterConfiguration(i, SOCK_NETWORKCONFIGURATION_UPDATE_DHCP_RELEASE, &g_NetworkConfig.NetworkInterfaces[i]);

        Network_Interface_Close(i);
    }
    
    rtp_net_exit();

    return TRUE;
}


SOCK_SOCKET RTIP_SOCKETS_Driver::Socket( int family, int type, int protocol ) 
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    SOCK_SOCKET socketHandle;
    int         nativeType;
    int         nativeFamily; 
    
    
    switch(type)
    {
        case SOCK_SOCK_STREAM:
            nativeType = RTP_NET_SOCK_STREAM;
            break;
        case SOCK_SOCK_DGRAM:   
            nativeType = RTP_NET_SOCK_DGRAM;
            break;
        case SOCK_SOCK_RAW:    
            nativeType = RTP_NET_SOCK_RAW;
            break;
        default:
            nativeType = 0;
            break;
    }
    
    switch (family)
    {
        case SOCK_AF_INET:
            nativeFamily = RTP_NET_AF_INET;
            break;
        default:
            nativeFamily = 0;
            break;
    }
    
    rtp_net_socket((RTP_SOCKET *)&socketHandle, nativeFamily, nativeType, protocol);
    
    return socketHandle;

}

int RTIP_SOCKETS_Driver::Bind( SOCK_SOCKET socket, const SOCK_sockaddr* address, int addressLen  ) 
{ 
    NATIVE_PROFILE_PAL_NETWORK();
 
    int             ret;
    unsigned char  *ipAddr;
    int             port;
    
    
    port    = ((SOCK_sockaddr_in *) address)->sin_port;
    ipAddr  = (unsigned char *) &(((SOCK_sockaddr_in *) address)->sin_addr);
    
    ret = rtp_net_bind ((RTP_HANDLE) socket, 
                           ipAddr, 
                           port, 
                           RTP_NET_TYPE_IPV4);
    return ret; 
}

int RTIP_SOCKETS_Driver::Connect(SOCK_SOCKET socket, const SOCK_sockaddr* address, int addressLen) 
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int ret;
    unsigned char * ipAddr;
    int             port;
    
    ipAddr = (unsigned char *) &(((SOCK_sockaddr_in *) address)->sin_addr);
    port   = ((SOCK_sockaddr_in *) address)->sin_port;
    
    ret = rtp_net_connect ((RTP_HANDLE) socket,
                           ipAddr,
                           port, 
                           RTP_NET_TYPE_IPV4);
    
    return ret;

}

int RTIP_SOCKETS_Driver::Send(SOCK_SOCKET socket, const char* buf, int len, int flags) 
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int ret;
    
    
    ret = (int) rtp_net_send ((RTP_HANDLE) socket, 
                        (const unsigned char *) buf, 
                        len, flags);
                        
    return ret;
}


int RTIP_SOCKETS_Driver::Recv(SOCK_SOCKET socket, char* buf, int len, int flags)
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int ret;
    int     nativeFlag;
    
    switch (flags)
    {
        case SOCKET_READ_PEEK_OPTION:
            nativeFlag = RTP_NET_MSG_PEEK;
            break;
        default:
            nativeFlag = flags;
            break;
    }
    
    ret = (int) rtp_net_recv ((RTP_HANDLE) socket, 
                               (unsigned char *) buf, 
                                len, nativeFlag);
                                
    return ret;
}

int RTIP_SOCKETS_Driver::Close(SOCK_SOCKET socket)
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int ret;
    
    ret = rtp_net_closesocket((RTP_HANDLE) socket);
    
    return ret;
}

int RTIP_SOCKETS_Driver::Listen(   SOCK_SOCKET socket, 
                                        int backlog )
{
    int ret;
    
    ret = rtp_net_listen ((RTP_HANDLE) socket, 
                            backlog);
    
    return ret;
}

SOCK_SOCKET RTIP_SOCKETS_Driver::Accept(    SOCK_SOCKET socket, 
                                            SOCK_sockaddr* address, 
                                            int* addressLen )
{ 
    
    SOCK_SOCKET     ret = SOCK_SOCKET_ERROR;
    int             type = RTP_NET_TYPE_IPV4;

    unsigned long   addr = address == NULL? 0: ((SOCK_sockaddr_in *) address)->sin_addr.S_un.S_addr;
    int             port = address == NULL? 0: ((SOCK_sockaddr_in *) address)->sin_port;
    
    rtp_net_accept ((RTP_SOCKET *) &ret, 
                    (RTP_SOCKET) socket,
                    (unsigned char*)&addr,
                    &port, 
                    &type);

    if(address)
    {
        ((SOCK_sockaddr_in *) address)->sin_addr.S_un.S_addr = addr;
        ((SOCK_sockaddr_in *) address)->sin_port             = port;
        ((SOCK_sockaddr_in *) address)->sin_family           = RTP_NET_AF_INET;
    }
    
    return ret;
}

int RTIP_SOCKETS_Driver::Shutdown( SOCK_SOCKET socket, 
                                        int how )
{
    int ret;
    
    ret = rtp_net_shutdown ((RTP_HANDLE) socket, how);
    
    return ret;
}

int RTIP_SOCKETS_Driver::GetAddrInfo(  const char* nodename, 
                                            char* servname, 
                                            const SOCK_addrinfo* hints, 
                                            SOCK_addrinfo** res )
{ 
    if(res == NULL || nodename == NULL) 
    {
        set_errno(EINVAL);
        return SOCK_SOCKET_ERROR;
    }


    *res = NULL;

    PFHOSTENT pHost = NULL;
    struct hostentext localHost;

    if(nodename[0] == '\0')
    {
        IFACE_INFO info;

        if(SOCK_SOCKET_ERROR != xn_interface_info(g_RTIP_SOCKETS_Driver.m_interfaces[0].m_interfaceNumber, &info ))
        {
            memset(&localHost, 0, sizeof(localHost));

            localHost.ip_addr.s_un.S_addr = *(UINT32*)info.my_ip_address;

            pHost = &localHost;
        }
    }
    else
    {
        // this method will be called to get both IP address from name and name from IP address, so nodename
        // can either be "www.xxx.com" or "xxx.xxx.xxx.xxx".  Therefore we will need to first get the IP bytes
        // gethostbyname will do this for either name or IP format, then we will need to get the name
        // by calling gethostbyaddr to get the host name.

        // first call is to get the IP bytes from string version (name or IP)
        pHost = gethostbyname((RTP_PFCHAR)nodename);

        // second call to get the host name
        if(pHost != NULL)
        {
            pHost = gethostbyaddr((RTP_PFCHAR)&pHost->ip_addr.s_un.S_addr, sizeof(UINT32), PF_INET);

            if(!pHost)
            {
                DEBUG_HANDLE_SOCKET_ERROR( "gethostbyaddr", FALSE );
            }
        }
        else
        {
            DEBUG_HANDLE_SOCKET_ERROR( "gethostbyname", FALSE );

        }
    }

    if(pHost)
    {
        SOCK_addrinfo* pAddrInfo = (SOCK_addrinfo*) private_malloc(sizeof(SOCK_addrinfo));

        if(pAddrInfo)
        {
            memset(pAddrInfo, 0, sizeof(SOCK_addrinfo));

            pAddrInfo->ai_family = RTP_NET_AF_INET;

            SOCK_sockaddr_in *pSockaddr = (SOCK_sockaddr_in*) private_malloc(sizeof(SOCK_sockaddr_in));

            if(pSockaddr)
            {
                memset(pSockaddr, 0, sizeof(SOCK_sockaddr_in));
                
                pSockaddr->sin_addr.S_un.S_addr = pHost->ip_addr.s_un.S_addr;
                pSockaddr->sin_family           = RTP_NET_AF_INET;

                pAddrInfo->ai_addr    = (SOCK_sockaddr*)pSockaddr;
                pAddrInfo->ai_addrlen = sizeof(SOCK_sockaddr_in);
            }

            if(pHost->sz_name != NULL)
            {
                int len = hal_strlen_s(pHost->sz_name);

                if(len > 0)
                {
                    int buffer_size = sizeof(char) * len + 1;
                    
                    pAddrInfo->ai_canonname = (char*) private_malloc(buffer_size);

                    if(pAddrInfo->ai_canonname)
                    {
                        hal_strcpy_s(pAddrInfo->ai_canonname, buffer_size, pHost->sz_name);
                    }
                }
            }
            
            *res = pAddrInfo;
        }
    }
    
    return (*res == NULL? SOCK_SOCKET_ERROR: 0);
}

void RTIP_SOCKETS_Driver::FreeAddrInfo( SOCK_addrinfo* ai )
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    if(ai == NULL) return;

    if(ai->ai_canonname)
    {
        private_free(ai->ai_canonname);
        ai->ai_canonname = NULL;
    }

    if(ai->ai_addr)
    {
        private_free(ai->ai_addr);
        ai->ai_addr = NULL;    
    }
    
    private_free(ai);
}

int RTIP_SOCKETS_Driver::Ioctl( SOCK_SOCKET socket, int cmd, int* data )
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int ret = SOCK_SOCKET_ERROR;
    int blocking = !(*data);
    int nToRead;
    
    switch( cmd )
    {
        case SOCK_FIONBIO:
            ret = rtp_net_setblocking((RTP_SOCKET) socket, 
                                        (unsigned int) blocking);
            break; 
        case SOCK_FIONREAD:
            ret = rtp_net_getntoread ((RTP_SOCKET) socket, 
                                        (unsigned long *) &nToRead);
            *data = nToRead;
            
            if (ret != -1)
            {
                return nToRead;
            }
            break; 
        default:
            ret = 0;
            break;
    }
    
    return ret;
}

int RTIP_SOCKETS_Driver::GetLastError()
{
    NATIVE_PROFILE_PAL_NETWORK();
    int err = 0;
    
    err = rtp_net_getlasterror();

    return GetNativeError(err);
}

int RTIP_SOCKETS_Driver::Select( int nfds, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* exceptfds, const SOCK_timeval* timeout )
{
    NATIVE_PROFILE_PAL_NETWORK();
    int ret;
    
    ret = rtp_net_select ((  RTP_FD_SET  *)   readfds,
                            (RTP_FD_SET  *)   writefds,
                            (RTP_FD_SET  *)   exceptfds,
                            (struct timeval*) timeout);

    if(ret == SOCK_SOCKET_ERROR)
    {
        DEBUG_HANDLE_SOCKET_ERROR("Select failed", FALSE);
    }
    
    return ret;
}

int RTIP_SOCKETS_Driver::SetSockOpt( SOCK_SOCKET socket, int level, int optname, const char* optval, int  optlen )
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int ret;
    int nativeLevel;
    int nativeOptionName;
    int nativeIntValue;
    char *pNativeOptionValue = (char*)optval;
    
    switch(level)
    {
        case SOCK_IPPROTO_IP:
            nativeLevel = RTP_NET_IPROTO_IP;
            nativeOptionName = GetNativeIPOption(optname);
            break;
        case SOCK_IPPROTO_TCP:    
        case SOCK_IPPROTO_UDP: 
        case SOCK_IPPROTO_ICMP:
        case SOCK_IPPROTO_IGMP:
        case SOCK_IPPROTO_IPV4:
        case SOCK_SOL_SOCKET:
            nativeLevel      = RTP_NET_SOL_SOCKET;
            nativeOptionName = GetNativeSockOption(level, optname);            

            switch(optname)
            {
                case SOCK_SOCKO_EXCLUSIVEADDRESSUSE:
                case SOCK_SOCKO_NOCHECKSUM:
                case SOCK_SOCKO_DONTLINGER:
                    nativeIntValue     = !*(int*)optval;
                    pNativeOptionValue = (char*)&nativeIntValue;
                    break;
                default:
                    break;
            }
            break;
        default:
            nativeLevel         = 0;
            nativeOptionName    = 0;
            break;
    }
    
    ret = rtp_net_setsockoopt((RTP_HANDLE) socket, 
                nativeLevel,
                nativeOptionName, 
                pNativeOptionValue,
                optlen);

    return ret;
    
}

int RTIP_SOCKETS_Driver::GetSockOpt( SOCK_SOCKET socket, int level, int optname, char* optval, int* optlen )
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int         ret;
    int         nativeLevel;
    int         nativeOptionName;
    
    switch(level)
    {
        case SOCK_IPPROTO_IP:
            nativeLevel = RTP_NET_IPROTO_IP;
            nativeOptionName = GetNativeIPOption(optname);
            break;
        case SOCK_IPPROTO_TCP:    
        case SOCK_IPPROTO_UDP: 
        case SOCK_IPPROTO_ICMP:
        case SOCK_IPPROTO_IGMP:
        case SOCK_IPPROTO_IPV4:
        case SOCK_SOL_SOCKET:
            nativeLevel         = RTP_NET_SOL_SOCKET;
            nativeOptionName    = GetNativeSockOption(level, optname);
            break;
        default:
            nativeLevel         = 0;
            nativeOptionName    = 0;
            break;
    }
    


    ret = rtp_net_getsockopt((RTP_HANDLE)socket, 
                            nativeLevel, 
                            nativeOptionName,     
                            optval, 
                            optlen);

    if(SOCK_SOCKET_ERROR != ret)
    {
        switch(optname)
        {
            case SOCK_SOCKO_NOCHECKSUM:
            case SOCK_SOCKO_EXCLUSIVEADDRESSUSE:
                *((int *) optval) = !*(int*)optval;
                break;
            default:
                break;
        }
    }

    return ret; 
}

int RTIP_SOCKETS_Driver::GetPeerName( SOCK_SOCKET socket, SOCK_sockaddr* name, int* namelen )
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int             ret;
    unsigned char * ipAddr;
    int             port;
    int             type = RTP_NET_TYPE_IPV4;
    
    ipAddr = (unsigned char *)&(((SOCK_sockaddr_in *) name)->sin_addr);
        
    ret =  rtp_net_getpeername ((RTP_HANDLE) socket, 
                                ipAddr ,
                                &port, 
                                &type);

    ((SOCK_sockaddr_in *) name)->sin_family = RTP_NET_AF_INET;
    ((SOCK_sockaddr_in *) name)->sin_port   = port;
    
    return ret;
}

int RTIP_SOCKETS_Driver::GetSockName( SOCK_SOCKET socket, SOCK_sockaddr* name, int* namelen )
{ 
    NATIVE_PROFILE_PAL_NETWORK();

    int         ret;
    int         port;
    int         type = RTP_NET_TYPE_IPV4;
    
    
    ret = rtp_net_getsockname ((RTP_HANDLE) socket,
                                (unsigned char *) &((SOCK_sockaddr_in *) name)->sin_addr,
                                &port,
                                &type);
    
    ((SOCK_sockaddr_in *) name)->sin_family = RTP_NET_AF_INET;
    ((SOCK_sockaddr_in *) name)->sin_port   = port;
    
    return ret; 
}

int RTIP_SOCKETS_Driver::RecvFrom( SOCK_SOCKET s, char* buf, int len, int flags, SOCK_sockaddr* from, int* fromlen )
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int             ret;
    int             type;
    int             port;
    UINT32          ipAddr;
    int             nativeFlag;
    
    type = RTP_NET_TYPE_IPV4;
    
    switch (flags)
    {
        case SOCKET_READ_PEEK_OPTION:
            nativeFlag = RTP_NET_MSG_PEEK;
            break;
        default:
            nativeFlag = flags;
            break;
    }
    if(from == NULL)
    {
        ret = (int ) rtp_net_recvfrom ((RTP_HANDLE) s,
                             (unsigned char *)buf, 
                             len,
                             NULL,
                             NULL, 
                             &type,
                             nativeFlag);
    }
    else
    {
        port    = ((SOCK_sockaddr_in *) from)->sin_port;
        ipAddr  = ((SOCK_sockaddr_in *) from)->sin_addr.S_un.S_addr;
    
        ret = (int) rtp_net_recvfrom ((RTP_HANDLE) s,
                             (unsigned char *)buf, 
                             len,
                             (unsigned char*)&ipAddr, 
                             &port,
                             &type, 
                             nativeFlag);

        if (SOCK_SOCKET_ERROR != ret)
        {
            ((SOCK_sockaddr_in *) from)->sin_port             = port;
            ((SOCK_sockaddr_in *) from)->sin_family           = RTP_NET_AF_INET;
            ((SOCK_sockaddr_in *) from)->sin_addr.S_un.S_addr = ipAddr;
        }
    }
        
    return ret; 
}

int RTIP_SOCKETS_Driver::SendTo( SOCK_SOCKET s, const char* buf, int len, int flags, const SOCK_sockaddr* to, int tolen )
{ 
    NATIVE_PROFILE_PAL_NETWORK();
    int    ret;
    UINT32 ipAddr;
    int    port;

    if(to == NULL)
    {
        set_errno(EINVAL);
        return SOCK_SOCKET_ERROR;
    }
    
    ipAddr = ((SOCK_sockaddr_in *) to)->sin_addr.S_un.S_addr;
    port   = ((SOCK_sockaddr_in *) to)->sin_port;

    ret = rtp_net_sendto ((RTP_HANDLE) s,
                                (const unsigned char *) buf, 
                                len,
                                (unsigned char *)&ipAddr,
                                port, 
                                RTP_NET_TYPE_IPV4,
                                flags);

    return ret;
}

UINT32 RTIP_SOCKETS_Driver::GetAdapterCount()
{
    NATIVE_PROFILE_PAL_NETWORK();
    return NETWORK_INTERFACE_COUNT;
}

HRESULT RTIP_SOCKETS_Driver::LoadAdapterConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config )
{
    NATIVE_PROFILE_PAL_NETWORK();
    if(interfaceIndex >= NETWORK_INTERFACE_COUNT) 
    {
        return CLR_E_INVALID_PARAMETER;
    }

    memcpy(config, &g_NetworkConfig.NetworkInterfaces[interfaceIndex], sizeof(g_NetworkConfig.NetworkInterfaces[interfaceIndex]));

    if(config->flags & SOCK_NETWORKCONFIGURATION_FLAGS_DHCP)
    {
        IFACE_INFO info;
        
        if(SOCK_SOCKET_ERROR != xn_interface_info(g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_interfaceNumber, &info ))
        {
            config->ipaddr     = *(UINT32*)info.my_ip_address;
            config->subnetmask = *(UINT32*)info.ip_mask;
            config->gateway    = *(UINT32*)info.gateways[0];
            config->dnsServer1 = *(UINT32*)info.dns_servers[0];
            config->dnsServer2 = *(UINT32*)info.dns_servers[1];
        }
        else
        {
            config->ipaddr     = 0;
            config->subnetmask = 0;
            config->gateway    = 0;
        }
    }
    
    
    return S_OK;
}

HRESULT RTIP_SOCKETS_Driver::LoadWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    /// Load wireless specific settings if any. You must return S_OK, otherwise default values will be
    /// loaded by PAL.

    return CLR_E_FAIL;
}

HRESULT RTIP_SOCKETS_Driver::UpdateAdapterConfiguration( UINT32 interfaceIndex, UINT32 updateFlags, SOCK_NetworkConfiguration* config )
{
    NATIVE_PROFILE_PAL_NETWORK();
    if(interfaceIndex >= NETWORK_INTERFACE_COUNT) 
    {
        return CLR_E_INVALID_PARAMETER;
    }
    
    BOOL fEnableDhcp = (0 != (config->flags & SOCK_NETWORKCONFIGURATION_FLAGS_DHCP));

    if(0 != (updateFlags & SOCK_NETWORKCONFIGURATION_UPDATE_DHCP))
    {
        if(fEnableDhcp)
        {           
            DHCP_conf    reg_conf;

            memset(&reg_conf, 0, sizeof(reg_conf));
            memset(&g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_currentDhcpSession, 0, sizeof(DHCP_session));

            xn_dhcp_init_conf(&reg_conf);

            g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_currentDhcpSession.packet_style = DHCP_MICROSOFT;

            if (SOCK_SOCKET_ERROR == xn_dhcp(g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_interfaceNumber, &g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_currentDhcpSession, &reg_conf))
            { 
                DEBUG_HANDLE_SOCKET_ERROR("update cfg: xn_dhcp", FALSE); 
                return CLR_E_FAIL;
            }
        }
        else
        {
            if(SOCK_SOCKET_ERROR == rtp_net_set_ip(g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_interfaceNumber, (RTP_PFUINT8)&config->ipaddr, (RTP_PFUINT8)&config->subnetmask))
            {
                DEBUG_HANDLE_SOCKET_ERROR("update cfg: rtp_net_set_ip", FALSE);
                return CLR_E_FAIL;
            }

            UINT32 destMask = SOCK_MAKE_IP_ADDR_LITTLEEND(0,0,0,0);

            if(SOCK_SOCKET_ERROR == xn_rt_add((RTP_PFUINT8)RT_DEFAULT, (RTP_PFUINT8)&destMask, (RTP_PFUINT8)&config->gateway, RT_USEIFACEMETRIC, g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_interfaceNumber, RT_INF))
            {
                DEBUG_HANDLE_SOCKET_ERROR("update cfg: xn_rt_add", FALSE);
                return FALSE;
            }
        }
    }

    if(fEnableDhcp)
    {
        if(0 != (updateFlags & SOCK_NETWORKCONFIGURATION_UPDATE_DHCP_RELEASE))
        {
            if(g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_currentDhcpSession.client_ip != 0)
            {
                if(SOCK_SOCKET_ERROR == xn_dhcp_release(&g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_currentDhcpSession))
                {
                    DEBUG_HANDLE_SOCKET_ERROR("update cfg: xn_dhcp_release", FALSE);
                    /*return CLR_E_FAIL;*/
                }
            }

            memset(&g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_currentDhcpSession, 0, sizeof(DHCP_session));
        }
        if(0 != (updateFlags & SOCK_NETWORKCONFIGURATION_UPDATE_DHCP_RENEW))
        {
            DHCP_conf reg_conf;

            xn_dhcp_init_conf(&reg_conf);

            g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_currentDhcpSession.packet_style = DHCP_MICROSOFT;

            if (SOCK_SOCKET_ERROR == xn_dhcp(g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_interfaceNumber, &g_RTIP_SOCKETS_Driver.m_interfaces[interfaceIndex].m_currentDhcpSession, &reg_conf))
            {
                DEBUG_HANDLE_SOCKET_ERROR("update cfg: xn_dhcp", FALSE);
                return CLR_E_FAIL;
            }
        }
    }
    // when using DHCP do not use the static settings
    else if(0 != (updateFlags & SOCK_NETWORKCONFIGURATION_UPDATE_DNS))
    {
        if(config->dnsServer1 != 0 || config->dnsServer2 != 0)
        {
            xn_set_server_list( NULL, 0);
            
            // add the DNS servers of the config 
            if( config->dnsServer1 != 0)
            {
                RTP_PFCUINT8 dns1 = (RTP_PFCUINT8)&(config->dnsServer1);
                
                if (SOCK_SOCKET_ERROR == xn_add_dns_server( dns1 ))
                {
                    DEBUG_HANDLE_SOCKET_ERROR("update cfg: xn_add_dns_server(1)", FALSE);
                }
            }
            if( config->dnsServer2 != 0)
            {
                RTP_PFCUINT8 dns2 = (RTP_PFCUINT8)&(config->dnsServer2);

                if (SOCK_SOCKET_ERROR == xn_add_dns_server( dns2 ))
                {
                    DEBUG_HANDLE_SOCKET_ERROR("update cfg: xn_add_dns_server(2)", FALSE);
                }       
            }
        }
    }

    if(0 != (updateFlags & SOCK_NETWORKCONFIGURATION_UPDATE_MAC))
    {
        // mac address requires stack re-init
        Network_Uninitialize();
        Network_Initialize();
    }

    if(0 != (config->flags & SOCK_NETWORKCONFIGURATION_FLAGS_DYNAMIC_DNS))
    {
        // the rtip stack doesn't support dynamic dns
        return CLR_E_NOT_SUPPORTED;
    }
    
    return S_OK;
}

int RTIP_SOCKETS_Driver::GetNativeSockOption (int level, int optname)
{
    NATIVE_PROFILE_PAL_NETWORK();
    int nativeOptionName;
    
    switch(optname)
    {
        case SOCK_SOCKO_LINGER:    
        case SOCK_SOCKO_DONTLINGER:
            nativeOptionName = RTP_NET_SO_LINGER;
            break;
        case SOCK_SOCKO_SENDTIMEOUT:          
            nativeOptionName = RTP_NET_SO_SEND_TIMEO;
            break;
        case SOCK_SOCKO_RECEIVETIMEOUT:       
            nativeOptionName = RTP_NET_SO_RCV_TIMEO;
            break;
        case SOCK_SOCKO_EXCLUSIVEADDRESSUSE: 
        case SOCK_SOCKO_REUSEADDRESS:         
            nativeOptionName = RTP_NET_SO_REUSEADDR;
            break;
        case SOCK_SOCKO_KEEPALIVE:  
            nativeOptionName = RTP_NET_SO_KEEPALIVE;
            break;
        case SOCK_SOCKO_ERROR:                  
            nativeOptionName = RTP_NET_SO_ERROR;
            break;
        case SOCK_SOCKO_BROADCAST:              
            nativeOptionName = RTP_NET_IP_OPTS_BROADCAST;
            break;
        case SOCK_SOCKO_RECEIVEBUFFER:
            // for UDP level we use the UDP buffer size
            if(level == SOCK_IPPROTO_UDP || level == SOCK_SOL_SOCKET)
            {
                nativeOptionName =  RTP_NET_SO_RECEIVE_BUFFER;
            }
            // for tcp we use the TCP window size
            // for socket level we use the tcp window size as well, assuming arbitrarily the socket is a tcp socket
            else if(level == SOCK_IPPROTO_TCP) 
            {
                nativeOptionName =  RTP_NET_SO_RCVBUF;
            }
            else
            {
                nativeOptionName = 0;
            }
            break;
        case SOCK_SOCKO_SENDBUFFER:
            // for UDP level we use the UDP buffer size
            if(level == SOCK_IPPROTO_UDP || level == SOCK_SOL_SOCKET)
            {
                nativeOptionName =  RTP_NET_SO_SEND_BUFFER;
            }
            // for tcp we use the TCP window size
            // for socket level we use the tcp window size as well, assuming arbitrarily the socket is a tcp socket
            else if(level == SOCK_IPPROTO_TCP)
            {
                nativeOptionName =  RTP_NET_SO_SNDBUF;
            }
            else
            {
                nativeOptionName = 0;
            }
            break;
        case SOCK_SOCKO_NOCHECKSUM:
            if (level == SOCK_IPPROTO_UDP)
            {
                nativeOptionName = RTP_NET_SO_UDPCKSUM_IN;
            }
            else if (level == SOCK_IPPROTO_TCP)
            {
                nativeOptionName = RTP_NET_SO_NAGLE;
            }
            else
            { 
                nativeOptionName = 0;
            }
            break;    
        case SOCK_SOCKO_ACCEPTCONNECTION:
            nativeOptionName = RTP_NET_SO_ACCEPTCON;
            break;
        case SOCK_SOCKO_TYPE:
            nativeOptionName = RTP_NET_SO_TYPE;
            break;
            
        case SOCK_SOCKO_USELOOPBACK:            //don't support     
        case SOCK_SOCKO_DONTROUTE:              //don't support
        case SOCK_SOCKO_OUTOFBANDINLINE:        //don't support
        case SOCK_SOCKO_SENDLOWWATER:           //don't support
        case SOCK_SOCKO_RECEIVELOWWATER:        //don't dupport
        case SOCK_SOCKO_MAXCONNECTIONS:         //don't support
            nativeOptionName = 0;
            break;
        default:
            nativeOptionName = 0;
            break;
            
    }
    return nativeOptionName;
}

int RTIP_SOCKETS_Driver::GetNativeIPOption (int optname)
{
    NATIVE_PROFILE_PAL_NETWORK();
    int nativeOptionName;
    
    switch(optname)
    {
        case SOCK_IPO_MULTICAST_IF:    
            nativeOptionName = RTP_NET_IP_OPTS_MULTICAST_IF;
            break;         
        case SOCK_IPO_MULTICAST_TTL:   
            nativeOptionName = RTP_NET_IP_OPTS_MULTICAST_TTL;
            break;         
        case SOCK_IPO_MULTICAST_LOOP:
            nativeOptionName = RTP_NET_IP_OPTS_MULTICAST_LOOP;
            break;
        case SOCK_IPO_ADD_MEMBERSHIP:  
            nativeOptionName = RTP_NET_IP_OPTS_ADD_MEMBERSHIP;
            break;         
        case SOCK_IPO_DROP_MEMBERSHIP: 
            nativeOptionName = RTP_NET_IP_OPTS_DROP_MEMBERSHIP;
            break;         
        case SOCK_IPO_IP_DONTFRAGMENT:
            nativeOptionName = RTP_NET_IP_OPTS_DONTFRAG;
            break;
        case SOCK_IPO_TTL:
            nativeOptionName = RTP_NET_SO_IP_TTL;
            break;
        default:
            nativeOptionName = 0;
            break;
    }   
    
    return nativeOptionName;
}   

int RTIP_SOCKETS_Driver::GetNativeError ( int error )
{
    NATIVE_PROFILE_PAL_NETWORK();
    int ret;
    
    switch(error)
    {
        case RTP_NET_EADDRNOTAVAIL:  
            ret = SOCK_EADDRNOTAVAIL;
            break;     
        case RTP_NET_EADDRINUSE:          
            ret = SOCK_EADDRINUSE;
            break;
        case RTP_NET_EAFNOSUPPORT: 
            ret = SOCK_EAFNOSUPPORT;
            break;       
        case RTP_NET_ECONNREFUSED: 
            ret = SOCK_ECONNREFUSED;
            break;       
        case RTP_NET_EDESTADDREQ: 
            ret = SOCK_EDESTADDRREQ;
            break;        
        case RTP_NET_EDESTUNREACH: 
            ret = SOCK_EHOSTUNREACH; 
            break;       
        case RTP_NET_EFAULT: 
            ret = SOCK_EFAULT;
            break;             
        case RTP_NET_EINVAL: 
            ret = SOCK_EINVAL;
            break;             
        case RTP_NET_EISCONN: 
            ret = SOCK_EISCONN;
            break;            
        case RTP_NET_EMFILE: 
            ret = SOCK_EMFILE;
            break;             
        case RTP_NET_ENETDOWN: 
            ret = SOCK_ENETDOWN;
            break;          
        case RTP_NET_ENETUNREACH: 
            ret = SOCK_ENETUNREACH;
            break;        
        case RTP_NET_ENOPROTOOPT: 
            ret = SOCK_ENOPROTOOPT;
            break;        
        case RTP_NET_ENOTCONN: 
            ret = SOCK_ENOTCONN;
            break;          
        case RTP_NET_ENOTINITIALIZED: 
            ret = SOCK_NOTINITIALISED;
            break;    
        case RTP_NET_ENOTSOCK: 
            ret = SOCK_ENOTSOCK;
            break;          
        case RTP_NET_EOPNOTSUPPORT: 
            ret = SOCK_EOPNOTSUPP;
            break;      
        case RTP_NET_ESHUTDOWN: 
            ret = SOCK_ESHUTDOWN;
            break;          
        case RTP_NET_ETIMEDOUT: 
            ret = SOCK_ETIMEDOUT;
            break;   
        case RTP_NET_EWOULDBLOCK:
        case RTP_NET_EOUTPUTFULL:
        case RTP_NET_ENOPKTS:
            ret = SOCK_EWOULDBLOCK;   
            break;                    
        case RTP_NET_EMSGSIZE: 
            ret = SOCK_EMSGSIZE;
            break;           
        case RTP_NET_ENAME_TOO_LONG: 
            ret = SOCK_ENAMETOOLONG;
            break;    
        case RTP_NET_ETRYAGAIN: 
            ret = SOCK_TRY_AGAIN;
            break;          
        case RTP_NET_ENODATA: 
            ret = SOCK_NO_DATA;
            break;            
        case RTP_NET_EPROTONOSUPPORT:
            ret = SOCK_EPROTONOSUPPORT;
            break;
        case RTP_NET_EPFNOSUPPORT:
            ret = SOCK_EPFNOSUPPORT;
            break;
        case RTP_NET_ENORESPONSE:
            ret = SOCK_HOST_NOT_FOUND;
            break;
        case RTP_NET_EACCES:
            ret = SOCK_EACCES;
            break;
        case RTP_NET_EBADRESP:
            ret = SOCK_TRY_AGAIN;
            break;                                
        case RTP_NET_ETNOSUPPORT:
            ret = SOCK_EPROTONOSUPPORT;
            break;
        default:
            ret = error;
            break;
    } 
    
    return (ret);   
}

