////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "sockets.h"

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PAL_Sockets_Driver"
#endif

Sockets_Driver g_Sockets_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

HAL_COMPLETION Sockets_Driver::s_DebuggerTimeoutCompletion;

//--//

SOCK_SOCKET SOCK_socket( INT32 family, INT32 type, INT32 protocol ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Socket( family, type, protocol, FALSE );
}
INT32 SOCK_bind( SOCK_SOCKET socket, const struct SOCK_sockaddr* address, INT32 addressLen  ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_bind( socket, address, addressLen  );
}
INT32 SOCK_connect(SOCK_SOCKET socket, const struct SOCK_sockaddr* address, INT32 addressLen) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Connect(socket, address, addressLen);
}
INT32 SOCK_send(SOCK_SOCKET socket, const char* buf, INT32 len, INT32 flags) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Send(socket, buf, len, flags);
}
INT32 SOCK_recv(SOCK_SOCKET socket, char* buf, INT32 len, INT32 flags)
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Recv(socket, buf, len, flags);
}
INT32 SOCK_close(SOCK_SOCKET socket)
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Close(socket);
}
INT32 SOCK_listen( SOCK_SOCKET socket, INT32 backlog )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Listen( socket, backlog );
}
SOCK_SOCKET SOCK_accept( SOCK_SOCKET socket, struct SOCK_sockaddr* address, INT32* addressLen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Accept( socket, address, addressLen, FALSE );
}
INT32 SOCK_shutdown( SOCK_SOCKET socket, INT32 how )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_shutdown( socket, how );
}
INT32 SOCK_getaddrinfo( const char* nodename, char* servname, const struct SOCK_addrinfo* hints, struct SOCK_addrinfo** res )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getaddrinfo( nodename, servname, hints, res );
}
void SOCK_freeaddrinfo( struct SOCK_addrinfo* ai )
{ 
    NATIVE_PROFILE_PAL_COM();
    HAL_SOCK_freeaddrinfo( ai );
}
INT32 SOCK_ioctl( SOCK_SOCKET socket, INT32 cmd, INT32* data )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_ioctl( socket, cmd, data );
}
INT32 SOCK_getlasterror()
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getlasterror();
}
INT32 SOCK_select( INT32 nfds, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* except, const struct SOCK_timeval* timeout )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Select( nfds, readfds, writefds, except, timeout );
}
INT32 SOCK_setsockopt( SOCK_SOCKET socket, INT32 level, INT32 optname, const char* optval, INT32  optlen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_setsockopt( socket, level, optname, optval, optlen );
}
INT32 SOCK_getsockopt( SOCK_SOCKET socket, INT32 level, INT32 optname,       char* optval, INT32* optlen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getsockopt( socket, level, optname, optval, optlen );
}
INT32 SOCK_getpeername( SOCK_SOCKET socket, struct SOCK_sockaddr* name, INT32* namelen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getpeername( socket, name, namelen );
}
INT32 SOCK_getsockname( SOCK_SOCKET socket, struct SOCK_sockaddr* name, INT32* namelen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_getsockname( socket, name, namelen );
}
INT32 SOCK_recvfrom( SOCK_SOCKET s, char* buf, INT32 len, INT32 flags, struct SOCK_sockaddr* from, INT32* fromlen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::RecvFrom( s, buf, len, flags, from, fromlen );
}
INT32 SOCK_sendto( SOCK_SOCKET s, const char* buf, INT32 len, INT32 flags, const struct SOCK_sockaddr* to, INT32 tolen )
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::SendTo( s, buf, len, flags, to, tolen );
}

BOOL Network_Initialize()
{
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Initialize( );
}

BOOL Network_Uninitialize()
{
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Uninitialize( );
}

void Sockets_Driver::CloseDebuggerSocket()
{
    GLOBAL_LOCK_SOCKETS(lock);

    if(g_Sockets_Driver.m_SocketDebugStream != SOCK_SOCKET_ERROR)
    {
        SOCK_close( g_Sockets_Driver.m_SocketDebugStream );
        
        g_Sockets_Driver.m_SocketDebugStream = SOCK_SOCKET_ERROR;

        g_Sockets_Driver.m_stateDebugSocket = DbgSock_Listening;

        if(s_DebuggerTimeoutCompletion.IsLinked())
        {
            s_DebuggerTimeoutCompletion.Abort();
        }
    }
}

void Sockets_Driver::OnDebuggerTimeout(void* arg)
{
    CloseDebuggerSocket();
}

BOOL  SOCKETS_Initialize( int ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();

    return Sockets_Driver::InitializeDbgListener( ComPortNum );
}
BOOL  SOCKETS_Uninitialize( int ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::UninitializeDbgListener( ComPortNum );
}

INT32 SOCKETS_Write( INT32 ComPortNum, const char* Data, size_t size )
{
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Write( ComPortNum, Data, size );
}

INT32 SOCKETS_Read( INT32 ComPortNum, char* Data, size_t size )
{
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::Read( ComPortNum, Data, size );
}

BOOL SOCKETS_Flush( INT32 ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();
    return TRUE;
}

BOOL SOCKETS_UpgradeToSsl( INT32 ComPortNum, const UINT8* pCACert, UINT32 caCertLen, const UINT8* pDeviceCert, UINT32 deviceCertLen, LPCSTR szTargetHost )
{
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}

BOOL SOCKETS_IsUsingSsl( INT32 ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}

void SOCKETS_CloseConnections()
{
    NATIVE_PROFILE_PAL_COM();
    Sockets_Driver::CloseConnections(FALSE);
}

BOOL SOCKETS_ProcessSocketActivity(SOCK_SOCKET signalSocket)
{
    NATIVE_PROFILE_PAL_COM();
    return Sockets_Driver::ProcessSocketActivity(signalSocket);
}

UINT32 SOCK_CONFIGURATION_GetAdapterCount()
{
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_CONFIGURATION_GetAdapterCount();
}

HRESULT SOCK_CONFIGURATION_LoadAdapterConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config )
{
    NATIVE_PROFILE_PAL_COM();
    return HAL_SOCK_CONFIGURATION_LoadAdapterConfiguration(interfaceIndex, config);
}
HRESULT SOCK_CONFIGURATION_UpdateAdapterConfiguration( UINT32 interfaceIndex, UINT32 updateFlags, SOCK_NetworkConfiguration* config )
{
    NATIVE_PROFILE_PAL_COM();
    HRESULT hr = S_OK;
    BOOL fDbg = FALSE;

    if(interfaceIndex >= NETWORK_INTERFACE_COUNT) 
    {
        return CLR_E_INVALID_PARAMETER;
    }

    const UINT32 c_reInitFlag = SOCK_NETWORKCONFIGURATION_UPDATE_DHCP       | 
                                SOCK_NETWORKCONFIGURATION_UPDATE_DHCP_RENEW | 
                                SOCK_NETWORKCONFIGURATION_UPDATE_MAC;

    const UINT32 c_uninitFlag = c_reInitFlag | SOCK_NETWORKCONFIGURATION_UPDATE_DHCP_RELEASE;

    if(0 != (updateFlags & c_uninitFlag))
    {
        fDbg = SOCKETS_Uninitialize(COM_SOCKET_DBG);
    }

    hr = HAL_SOCK_CONFIGURATION_UpdateAdapterConfiguration(interfaceIndex, updateFlags, config);

    if(SUCCEEDED(hr))
    {
        Sockets_Driver::SaveConfig(interfaceIndex, config);
    }
    else
    {
        // restore the network configuration
        HAL_SOCK_CONFIGURATION_UpdateAdapterConfiguration(interfaceIndex, updateFlags, &g_NetworkConfig.NetworkInterfaces[interfaceIndex]);
    }

    if(0 != (updateFlags & c_reInitFlag))
    {
        if(fDbg) SOCKETS_Initialize(COM_SOCKET_DBG);
    }

    return hr;
}

HRESULT SOCK_CONFIGURATION_LoadConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config )
{
    NATIVE_PROFILE_PAL_COM();
    HRESULT hr = S_OK;

    if(interfaceIndex >= NETWORK_INTERFACE_COUNT || config == NULL) 
    {
        return CLR_E_INVALID_PARAMETER;
    }

    // load current DCHP settings
    hr = SOCK_CONFIGURATION_LoadAdapterConfiguration(interfaceIndex, config);

    return hr;
}

HRESULT SOCK_CONFIGURATION_LoadWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    NATIVE_PROFILE_PAL_COM();

    if (interfaceIndex >= WIRELESS_INTERFACE_COUNT)
    {
        return CLR_E_INVALID_PARAMETER;
    }

    Sockets_Driver::ApplyWirelessConfig();

    /// Hal version is given a chance if it wants to override stored predifned values.
    if (HAL_SOCK_CONFIGURATION_LoadWirelessConfiguration(interfaceIndex, wirelessConfig) != S_OK)
    {
        memcpy( wirelessConfig, &g_WirelessConfig.WirelessInterfaces[interfaceIndex], sizeof(SOCK_WirelessConfiguration) );
    }

    return S_OK;
}

HRESULT SOCK_CONFIGURATION_UpdateWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    NATIVE_PROFILE_PAL_COM();

    if (interfaceIndex >= WIRELESS_INTERFACE_COUNT)
    {
        return CLR_E_INVALID_PARAMETER;
    }

    memcpy( &g_WirelessConfig.WirelessInterfaces[interfaceIndex], wirelessConfig, sizeof(SOCK_WirelessConfiguration) );

    return S_OK;
}

HRESULT SOCK_CONFIGURATION_SaveAllWirelessConfigurations( )
{
    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_WirelessConfig.GetDriverName(), &g_WirelessConfig, sizeof(g_WirelessConfig), TRUE);

    return S_OK;
}


#define SOCKET_SHUTDOWN_READ         0
#define SOCKET_SHUTDOWN_WRITE        1
#define SOCKET_SHUTDOWN_READ_WRITE   2

#define ISSET_SOCKET_FLAG(x,y) ((y) == ((y) & (x).m_flags))
#define SET_SOCKET_FLAG(x,y)   (x).m_flags |= (y)
#define CLEAR_SOCKET_FLAG(x,y) (x).m_flags &= ~(y)

//-----------------------------------------------------------------------------
//
//  CloseConnections - close all connections with the option of leaving the debugger sockets open.
//  debugger sockets are left open on CLR reboot so that communication with the debugger is not
//  lost.
//
//-----------------------------------------------------------------------------
void Sockets_Driver::CloseConnections(BOOL fCloseDbg)
{
    NATIVE_PROFILE_PAL_COM();
    GLOBAL_LOCK_SOCKETS(lock);

    if(fCloseDbg)
    {
        // this has to be set if we are closing all debugger connections, or the thread will abort
        g_Sockets_Driver.m_fShuttingDown = TRUE;
    }

    // For some platforms (NetOS) we need to close the socket on a separate socket thread,
    // therefore, we set the close flag for each socket and then call SignalSocketThread 
    // which will wake up the socket thread to close the sockets.
    for( INT32 i=0; i<g_Sockets_Driver.m_cntSockets; i++ )
    {
        SOCK_SOCKET sock = g_Sockets_Driver.m_socketHandles[i].m_socket;
        
        if((sock != SOCK_SOCKET_ERROR) && (fCloseDbg || !ISSET_SOCKET_FLAG(g_Sockets_Driver.m_socketHandles[i], SocketRegisterMap::c_DebugSocket)))
        {
            SET_SOCKET_FLAG( g_Sockets_Driver.m_socketHandles[i], SocketRegisterMap::c_CloseSocket );
        }
    }

    HAL_SOCK_SignalSocketThread();
}

//--//

SOCK_SOCKET Sockets_Driver::Socket( INT32 family, INT32 type, INT32 protocol, BOOL fDebug )
{
    NATIVE_PROFILE_PAL_COM();
    SOCK_SOCKET ret = SOCK_SOCKET_ERROR;
    
    if(g_Sockets_Driver.m_cntSockets < (fDebug? SOCKETS_MAX_COUNT: SOCKETS_MAX_COUNT-1)) //leave one for the debugger
    {
        ret = HAL_SOCK_socket(family, type, protocol);

        if(ret != SOCK_SOCKET_ERROR)
        {
            // TODO: because we are somewhat constrained by the number of sockets we support
            // we may want to force sockets to have linger set to 0 to force cleanup when they are closed.
            //INT32 optVal = 0;
            
            //HAL_SOCK_setsockopt(ret, SOCK_SOL_SOCKET, SOCK_SOCKO_LINGER, (const char*)&optVal, sizeof(INT32));

            RegisterSocket(ret, (protocol != SOCK_IPPROTO_TCP), fDebug);
        }
    }

    return ret;
}
INT32 Sockets_Driver::Connect(SOCK_SOCKET socket, const struct SOCK_sockaddr* address, INT32 addressLen)
{
    NATIVE_PROFILE_PAL_COM();
    INT32 ret = HAL_SOCK_connect(socket, address, addressLen);

    // WARNING: the socket is selectable if the connect returns EWOULDBLOCK because the connection process has started,
    // the managed code portion polls for completion
    if((ret != SOCK_SOCKET_ERROR) || (HAL_SOCK_getlasterror() == SOCK_EWOULDBLOCK))
    {
        RegisterForSelect(socket);
    }
    return ret;
}
INT32 Sockets_Driver::Send(SOCK_SOCKET socket, const char* buf, INT32 len, INT32 flags) 
{
    NATIVE_PROFILE_PAL_COM();
    INT32 ret;

    if(SOCK_FD_ISSET(socket, &g_Sockets_Driver.m_fdsetExcept)) return SOCK_SOCKET_ERROR;

#if defined(SOCKETS_MAX_SEND_LENGTH)
    if((0 < SOCKETS_MAX_SEND_LENGTH) && (SOCKETS_MAX_SEND_LENGTH < len))
    {
        len = SOCKETS_MAX_SEND_LENGTH;
    }
#endif
    
    ret = HAL_SOCK_send(socket, buf, len, flags);

    ClearStatusBit(socket, &g_Sockets_Driver.m_fdsetWrite);

    return ret;
}
INT32 Sockets_Driver::Recv(SOCK_SOCKET socket, char* buf, INT32 len, INT32 flags)
{
    NATIVE_PROFILE_PAL_COM();
    char  c   = 0;
    INT32 ret = 0;
    
    if(SOCK_FD_ISSET(socket, &g_Sockets_Driver.m_fdsetExcept)) return SOCK_SOCKET_ERROR;
        
    ret = HAL_SOCK_recv(socket, buf, len, flags);

    // only clear the status bit if we have no more data to read
    if(HAL_SOCK_recv(socket, &c, sizeof(c),  SOCKET_READ_PEEK_OPTION) < 0)
    {
        ClearStatusBit(socket, &g_Sockets_Driver.m_fdsetRead);
    }
    
    return ret;
}

INT32 Sockets_Driver::Shutdown(SOCK_SOCKET sock, INT32 how)
{
    RegisterForSelect(sock, FALSE);

    return HAL_SOCK_shutdown(sock, how);
}

//-----------------------------------------------------------------------------
//
// Close - The close method marks a socket to be closed by the select thread.  Close is handled
// in this way because (at least for one implementation) the select method can not be set for a 
// closed socket.  Therfore in the select thread the socket is closed.
//
//-----------------------------------------------------------------------------
INT32 Sockets_Driver::Close(SOCK_SOCKET sock)
{
    NATIVE_PROFILE_PAL_COM();
    GLOBAL_LOCK_SOCKETS(x);

    RegisterForSelect(sock, FALSE);

    // For some platforms, we need to close the socket on a separate socket thread,
    // therefore, we set the close flag for each socket and then call SignalSocketThread 
    // which will wake up the socket thread to close the sockets.
    for(INT32 i=0; i<g_Sockets_Driver.m_cntSockets; i++)
    {
        if(g_Sockets_Driver.m_socketHandles[i].m_socket == sock)
        {
            SET_SOCKET_FLAG(g_Sockets_Driver.m_socketHandles[i], SocketRegisterMap::c_CloseSocket);
            break;
        }
    }

    HAL_SOCK_SignalSocketThread();

    return 0;        
}
INT32 Sockets_Driver::Listen( SOCK_SOCKET socket, INT32 backlog )
{
    NATIVE_PROFILE_PAL_COM();
    INT32 ret = HAL_SOCK_listen(socket, backlog);

    // register the socket for select now that we are in listen mode
    RegisterForSelect(socket);

    return ret;
}
SOCK_SOCKET Sockets_Driver::Accept( SOCK_SOCKET socket, struct SOCK_sockaddr* address, INT32* addressLen, BOOL fDebug )
{
    NATIVE_PROFILE_PAL_COM();
    SOCK_SOCKET ret = SOCK_SOCKET_ERROR;

    if(g_Sockets_Driver.m_cntSockets < (fDebug? SOCKETS_MAX_COUNT: SOCKETS_MAX_COUNT-1)) //leave one for the debugger
    {
        ret = HAL_SOCK_accept(socket, address, addressLen);

        if(ret != SOCK_SOCKET_ERROR)
        {
            RegisterSocket(ret, TRUE, fDebug);
        }    

        ClearStatusBit(socket, &g_Sockets_Driver.m_fdsetRead);
    }

    return ret;
}    

INT32 Sockets_Driver::SetSelectValues( SOCK_fd_set *reqSet, SOCK_fd_set *selectSet, BOOL fClearIfSet )
{
    NATIVE_PROFILE_PAL_COM();
    INT32 cnt = 0;
    INT32 i;

    if(reqSet != NULL)
    {
        for(i=0; i<reqSet->fd_count; i++ )
        {
            ///
            /// Check for invalid sockets first.  If any of the given sockets in reqSet are not valid sockets
            /// then we must return SOCKET_ERROR
            ///
            bool fErrs = true;
            for(int index = 0; index < g_Sockets_Driver.m_cntSockets; index++)
            {
                if(reqSet->fd_array[i] == g_Sockets_Driver.m_socketHandles[index].m_socket)
                {
                    fErrs = false;
                    break;
                }
            }

            if(fErrs) return SOCK_SOCKET_ERROR;

            if(SOCK_FD_ISSET(reqSet->fd_array[i], selectSet))
            {
                reqSet->fd_array[cnt] = reqSet->fd_array[i];

                if(fClearIfSet)
                {
                    SOCK_FD_CLR(reqSet->fd_array[i], selectSet);
                }

                cnt++;
            }
        }
        reqSet->fd_count = cnt;
    }

    return cnt;
}

// We maintain a current collection of the select results for all sockets.  This was done to support
// OS ports (NetOS).  We use the socket 'select' method as an asynchronous eventing system for socket activity.
INT32 Sockets_Driver::Select( INT32 nfds, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* exceptfds, const SOCK_timeval* timeout )
{
    NATIVE_PROFILE_PAL_COM();
    INT32 cnt = 0;
    INT32 val = 0;
    
    GLOBAL_LOCK_SOCKETS(lock);
  
    if(SOCK_SOCKET_ERROR == (val = SetSelectValues( readfds  , &g_Sockets_Driver.m_fdsetRead,   FALSE ))) return SOCK_SOCKET_ERROR;
    cnt += val;

    if(SOCK_SOCKET_ERROR == (val = SetSelectValues( writefds , &g_Sockets_Driver.m_fdsetWrite,  FALSE ))) return SOCK_SOCKET_ERROR;
    cnt += val;

    if(SOCK_SOCKET_ERROR == (val = SetSelectValues( exceptfds, &g_Sockets_Driver.m_fdsetExcept, TRUE  ))) return SOCK_SOCKET_ERROR;
    cnt += val;

    return cnt;
}
INT32 Sockets_Driver::RecvFrom( SOCK_SOCKET s, char* buf, INT32 len, INT32 flags, struct SOCK_sockaddr* from, INT32* fromlen )
{
    NATIVE_PROFILE_PAL_COM();
    char c = 0;
    INT32 ret = 0;

    if(SOCK_FD_ISSET(s, &g_Sockets_Driver.m_fdsetExcept)) return SOCK_SOCKET_ERROR;
        
    ret = HAL_SOCK_recvfrom(s, buf, len, flags, from, fromlen);

    // only clear the status bit if we have no more data to read
    if(HAL_SOCK_recvfrom(s, &c, sizeof(c),  SOCKET_READ_PEEK_OPTION, from, fromlen) < 0)
    {
        ClearStatusBit(s, &g_Sockets_Driver.m_fdsetRead);
    }

    return ret;
}
INT32 Sockets_Driver::SendTo( SOCK_SOCKET s, const char* buf, INT32 len, INT32 flags, const struct SOCK_sockaddr* to, INT32 tolen )
{
    NATIVE_PROFILE_PAL_COM();
    INT32 ret;

    if(SOCK_FD_ISSET(s, &g_Sockets_Driver.m_fdsetExcept)) return SOCK_SOCKET_ERROR;
    
#if defined(SOCKETS_MAX_SEND_LENGTH)
    if((0 < SOCKETS_MAX_SEND_LENGTH) && (SOCKETS_MAX_SEND_LENGTH < len))
    {
        len = SOCKETS_MAX_SEND_LENGTH;
    }
#endif
    
    ret = HAL_SOCK_sendto(s, buf, len, flags, to, tolen);

    ClearStatusBit(s, &g_Sockets_Driver.m_fdsetWrite);

    return ret;
}


//--//

//-----------------------------------------------------------------------------
//
//  MulticastDiscoveryRespond - Respond to a NETMF multicast message.  This is
//  a simple discovery mechanism that uses multicast sockets over UDP.  The
//  client will send a message to the ip address defined by
//     SOCK_DISCOVERY_MULTICAST_IPADDR 
//  over port
//     SOCK_DISCOVERY_MULTICAST_PORT
//  If the message contains the sting "NETMF", then a response is sent with the
//  the data structure SOCK_discoveryinfo (which includes the ip address and mac
//  address of the device).
//
//-----------------------------------------------------------------------------
void Sockets_Driver::MulticastDiscoveryRespond()
{
    NATIVE_PROFILE_PAL_COM();
    SOCK_sockaddr from;

    char data[64];
    INT32 fromlen = sizeof(from);

    // intentionally call the HAL recvfrom so as not to invoke the signalnotifythread
    INT32 len = HAL_SOCK_recvfrom( g_Sockets_Driver.m_multicastSocket, data, sizeof(data), 0, &from, &fromlen );

    if(len > 0)
    {
        UINT32 idx = 0;
        BOOL fFound = FALSE;
        const char* c_Signature = SOCK_DISCOVERY_MULTICAST_TOKEN;
        INT32 sigLen = hal_strlen_s(c_Signature);

        // search for discovery token
        while(idx <= (len-sigLen))
        {
            if(0 == memcmp( &data[idx], c_Signature, sigLen))
            {
                fFound = TRUE;
                break;
            }
            idx++;
        }
        
        if(fFound)
        {
            SOCK_NetworkConfiguration current;
            SOCK_discoveryinfo info;
            SOCK_sockaddr_in sockAddr;
            SOCK_sockaddr_in sockAddrMulticast;
            INT32 opt = 64;
            SOCK_SOCKET sock;
            INT32 nonblocking = 1;

            // Load is required here because the g_NetworkConfig contains only the static ip address (not DHCP)
            HAL_SOCK_CONFIGURATION_LoadAdapterConfiguration(0, &current);

            info.ipaddr        = current.ipaddr;
            info.macAddressLen = current.macAddressLen;
            memcpy( &info.macAddressBuffer[0], &current.macAddressBuffer[0], current.macAddressLen );

            sock = Socket(SOCK_AF_INET, SOCK_SOCK_DGRAM, SOCK_IPPROTO_UDP, FALSE);
            SOCKET_CHECK_RESULT(sock);

            memset( &sockAddr, 0, sizeof(sockAddr) );
            sockAddr.sin_family           = SOCK_AF_INET;
            sockAddr.sin_port             = SOCK_htons(0);
            sockAddr.sin_addr.S_un.S_addr = info.ipaddr;

            memset( &sockAddrMulticast, 0, sizeof(sockAddrMulticast) );
            sockAddrMulticast.sin_family           = SOCK_AF_INET;
            sockAddrMulticast.sin_port             = SOCK_htons(SOCK_DISCOVERY_MULTICAST_PORT);
            sockAddrMulticast.sin_addr.S_un.S_addr = SOCK_htonl(SOCK_DISCOVERY_MULTICAST_IPADDR_SND);

            SOCKET_CHECK_RESULT(HAL_SOCK_ioctl(sock, SOCK_FIONBIO, &nonblocking));
            SOCKET_CHECK_RESULT(HAL_SOCK_setsockopt(sock, SOCK_IPPROTO_IP, SOCK_IPO_MULTICAST_TTL, (const char *) &opt, sizeof(opt)));
            SOCKET_CHECK_RESULT(HAL_SOCK_bind(sock,  (SOCK_sockaddr*)&sockAddr, sizeof(sockAddr)));
            // send a multicast socket back to the caller
            SOCKET_CHECK_RESULT(SendTo(sock, (const char*)&info, sizeof(info), 0, (SOCK_sockaddr*)&sockAddrMulticast, sizeof(sockAddrMulticast)));

            SOCK_close(sock);

            SOCKET_CLEANUP();

            if(sock != SOCK_SOCKET_ERROR)
            {
                SOCK_close(sock);

                debug_printf("MULTICAST RESP SOCKET_ERROR: %d\r\n", HAL_SOCK_getlasterror() );
            }

            SOCKET_CHECK_EXIT_NORETURN();            
        }
    }
}

//-----------------------------------------------------------------------------
//
// WaitForEventThread - socket activity monitor thread function
//   This method is used as a monitor thread for the socket subsystem.  It waits
//   for activity by calling the select method for each open (selectable) socket.
//   In order to prevent spinning when a socket is in an evented state, we update
//   a global fd_set for read/write/except.  Then when a caller invokes the 
//   select method it will use the global state rather than calling select.
//
//-----------------------------------------------------------------------------
BOOL Sockets_Driver::ProcessSocketActivity(SOCK_SOCKET signalSocket)
{
    NATIVE_PROFILE_PAL_COM();
    SOCK_fd_set rCpy;
    SOCK_fd_set wCpy;
    SOCK_fd_set eCpy;

    SOCK_timeval timeout;

    timeout.tv_sec  = 0;
    timeout.tv_usec = 0;

    INT32 sel = 0;
    bool fSockClosed = false;

    if(signalSocket != SOCK_SOCKET_ERROR) SOCK_FD_SET( signalSocket, &g_Sockets_Driver.m_nextReadSet );
    
    // always select multicast socket
    if(g_Sockets_Driver.m_multicastSocket != SOCK_SOCKET_ERROR) SOCK_FD_SET( g_Sockets_Driver.m_multicastSocket, &g_Sockets_Driver.m_nextReadSet );

    // NULL timeout uses largest ulong value, thus creating a blocking call
    sel = HAL_SOCK_select( SOCK_FD_SETSIZE, &g_Sockets_Driver.m_nextReadSet, &g_Sockets_Driver.m_nextWriteSet, &g_Sockets_Driver.m_nextExcptSet, &timeout);

    if(SOCK_SOCKET_ERROR == sel)
    {
        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextReadSet);
        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextWriteSet);
        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextExcptSet);
    }
    // always process (incase there are any closed sockets)
    {
        // the update lock mutex is used to safely modify the global
        GLOBAL_LOCK_SOCKETS(lock);

        // clear out the temp fd_set variables
        SOCK_FD_ZERO(&rCpy);  
        SOCK_FD_ZERO(&wCpy); 
        SOCK_FD_ZERO(&eCpy); 

        // loop through each open socket and determine which ones need to be set
        // for the next select call.  Note sockets that already have events in the
        // global state do not need (AND SHOULDN'T) to be added to the next select call.
        for(INT32 i=0; i<g_Sockets_Driver.m_cntSockets; i++)
        {
            if(ISSET_SOCKET_FLAG(g_Sockets_Driver.m_socketHandles[i], SocketRegisterMap::c_CloseSocket))
            {
                SOCK_SOCKET s = g_Sockets_Driver.m_socketHandles[i].m_socket;

                if(g_Sockets_Driver.m_socketHandles[i].m_sslData == NULL)
                {
                    // g_Sockets_Driver.m_socketHandles[i] is no longer valid after this call
                    UnregisterSocket(i); 

                    if(SOCK_SOCKET_ERROR == HAL_SOCK_close(s))
                    {
                        debug_printf("ERROR: HAL_SOCK_close: %d\r\n", HAL_SOCK_getlasterror());
                    }

                }
                // c_SSL_DATA_SOCKET_CLOSED_BY_SSL indicates the socket already closed by SSL
                else
                {

                    // SSL_CloseSocket calls UnregisterSocket
                    SSL_CloseSocket(s);
                }

                fSockClosed = true;

                i--; continue; // Unregister socket collapses the array, so try again with previous 'i' value                    
            }

            // sockets must be in a connected or listening state before they can be added to the
            // select method
            if(!ISSET_SOCKET_FLAG(g_Sockets_Driver.m_socketHandles[i], SocketRegisterMap::c_SelectableSocket))
            {
                continue;
            }

            SOCK_SOCKET sock = g_Sockets_Driver.m_socketHandles[i].m_socket;

            
            if(sel > 0)
            {
                // update the global fd_set variables if we have activity on any socket (DO THIS BEFORE UPDATING THE NEXT SELECT SETS)
                if(SOCK_FD_ISSET(sock, &g_Sockets_Driver.m_nextReadSet))
                {
                    SOCK_FD_SET( sock, &g_Sockets_Driver.m_fdsetRead );
                }
                if(SOCK_FD_ISSET(sock, &g_Sockets_Driver.m_nextWriteSet))
                {
                    SOCK_FD_SET( sock, &g_Sockets_Driver.m_fdsetWrite );
                }
                if(SOCK_FD_ISSET(sock, &g_Sockets_Driver.m_nextExcptSet))
                {
                    SOCK_FD_SET( sock, &g_Sockets_Driver.m_fdsetExcept );
                }
            }
            // only update the select sockets if they don't exist in the global fd_set variables
            // MAKE SURE THIS IS DONE AFTER UPDATING THE GLOBAL STATE (above)
            if(!SOCK_FD_ISSET(sock, &g_Sockets_Driver.m_fdsetRead ))
            {
                SOCK_FD_SET( sock, &rCpy );
            }
            if(!SOCK_FD_ISSET(sock, &g_Sockets_Driver.m_fdsetWrite ))
            {
                SOCK_FD_SET( sock, &wCpy );
            }
            if(!SOCK_FD_ISSET(sock, &g_Sockets_Driver.m_fdsetExcept ))
            {
                SOCK_FD_SET( sock, &eCpy );
            }
        }

        if(sel > 0 && SOCK_FD_ISSET( g_Sockets_Driver.m_multicastSocket, &g_Sockets_Driver.m_nextReadSet ))
        {
            MulticastDiscoveryRespond();
            sel--;
        }
        
        // copy over the temp fd_set variables
        memcpy( &g_Sockets_Driver.m_nextReadSet , &rCpy, sizeof(rCpy) );
        memcpy( &g_Sockets_Driver.m_nextWriteSet, &wCpy, sizeof(wCpy) );
        memcpy( &g_Sockets_Driver.m_nextExcptSet, &eCpy, sizeof(eCpy) );

        if((sel > 0 || fSockClosed) && !g_Sockets_Driver.m_fShuttingDown)
        {
            HAL_SOCK_EventsSet( SOCKET_EVENT_FLAG_SOCKET );
        }
    }

    return TRUE;
}

BOOL Sockets_Driver::InitializeDbgListener( int ComPortNum )
{   
    NATIVE_PROFILE_PAL_COM();
    SOCK_sockaddr_in sockAddr;
    INT32 nonblocking = 1;
    INT32 optval = 1;
    INT32 optLinger = 0;
    SOCKET_CHECK_ENTER();

    if(ComPortNum != ConvertCOM_SockPort(COM_SOCKET_DBG)) return FALSE;

    if(g_Sockets_Driver.m_SocketDebugListener != SOCK_SOCKET_ERROR) return TRUE;

    s_DebuggerTimeoutCompletion.InitializeForUserMode(OnDebuggerTimeout);

    GLOBAL_LOCK_SOCKETS(sock);

    //-- debug api socket --//

    g_Sockets_Driver.m_SocketDebugListener = Socket( SOCK_AF_INET, SOCK_SOCK_STREAM, SOCK_IPPROTO_TCP, TRUE );
    SOCKET_CHECK_RESULT( g_Sockets_Driver.m_SocketDebugListener );

    memset( &sockAddr, 0, sizeof(sockAddr) );
    sockAddr.sin_family             = SOCK_AF_INET;
    sockAddr.sin_port               = SOCK_htons(DEBUG_SOCKET_PORT);
    sockAddr.sin_addr.S_un.S_addr   = SOCK_htonl(SOCK_INADDR_ANY);

    SOCKET_CHECK_RESULT( SOCK_ioctl(g_Sockets_Driver.m_SocketDebugListener, SOCK_FIONBIO, &nonblocking) );

    SOCKET_CHECK_RESULT( HAL_SOCK_setsockopt( g_Sockets_Driver.m_SocketDebugListener, SOCK_IPPROTO_TCP, SOCK_TCP_NODELAY, (char*)&optval, sizeof(optval) ) );

    SOCKET_CHECK_RESULT( HAL_SOCK_setsockopt(g_Sockets_Driver.m_SocketDebugListener, SOCK_SOL_SOCKET, SOCK_SOCKO_LINGER, (const char*)&optLinger, sizeof(INT32)) );

    SOCKET_CHECK_RESULT( SOCK_bind( g_Sockets_Driver.m_SocketDebugListener,  (SOCK_sockaddr*)&sockAddr, sizeof(sockAddr) ) );

    SOCKET_CHECK_RESULT( SOCK_listen( g_Sockets_Driver.m_SocketDebugListener, 1 ) );

    g_Sockets_Driver.m_stateDebugSocket = DbgSock_Listening;

    Sockets_Driver::InitializeMulticastDiscovery();

    SOCKET_CLEANUP();

    if(g_Sockets_Driver.m_SocketDebugListener != SOCK_SOCKET_ERROR)
    {
        SOCK_close(g_Sockets_Driver.m_SocketDebugListener);
        g_Sockets_Driver.m_SocketDebugListener = SOCK_SOCKET_ERROR;

        debug_printf("DBGLISTENER SOCKET_ERROR: %d\r\n", HAL_SOCK_getlasterror() );
    }

    SOCKET_CHECK_EXIT_BOOL_CLEANUP();
}

BOOL Sockets_Driver::UninitializeDbgListener( int ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();

    if(ComPortNum != ConvertCOM_SockPort(COM_SOCKET_DBG)) return FALSE;

    g_Sockets_Driver.m_stateDebugSocket = DbgSock_Uninitialized;
    
    GLOBAL_LOCK_SOCKETS(lock);

    CloseDebuggerSocket();

    if(g_Sockets_Driver.m_SocketDebugListener != SOCK_SOCKET_ERROR)
    {
        SOCK_close( g_Sockets_Driver.m_SocketDebugListener );
        g_Sockets_Driver.m_SocketDebugListener = SOCK_SOCKET_ERROR;
    }

    return TRUE;
}

void Sockets_Driver::SaveConfig(INT32 index, SOCK_NetworkConfiguration *cfg)
{
    NATIVE_PROFILE_PAL_COM();
    if(index >= NETWORK_INTERFACE_COUNT) return;

    if(cfg) 
    {
        memcpy( &g_NetworkConfig.NetworkInterfaces[index], cfg, sizeof(SOCK_NetworkConfiguration) );
    }
    
    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_NetworkConfig.GetDriverName(), &g_NetworkConfig, sizeof(g_NetworkConfig), TRUE);
}

void Sockets_Driver::ApplyConfig()
{
    NATIVE_PROFILE_PAL_COM();
    if(!HAL_CONFIG_BLOCK::ApplyConfig( g_NetworkConfig.GetDriverName(), &g_NetworkConfig, sizeof(g_NetworkConfig) ))
    {
        // save to the dynamic config section so that MFDeploy will be able to get the configuration.
        SaveConfig(0, NULL);            
    }
}

void Sockets_Driver::ApplyWirelessConfig()
{
    NATIVE_PROFILE_PAL_COM();

    if(!s_wirelessInitialized)
    {
        if(!HAL_CONFIG_BLOCK::ApplyConfig( g_WirelessConfig.GetDriverName(), &g_WirelessConfig, sizeof(g_WirelessConfig) ))
        {
            SaveWirelessConfig(0, NULL);
        }
        s_wirelessInitialized = TRUE;
    }
}

void Sockets_Driver::SaveWirelessConfig(INT32 index, SOCK_NetworkConfiguration *cfg)
{
    NATIVE_PROFILE_PAL_COM();
    if(index >= WIRELESS_INTERFACE_COUNT) return;
    
    if(cfg) 
    {
        memcpy( &g_WirelessConfig.WirelessInterfaces[index], cfg, sizeof(SOCK_WirelessConfiguration) );
    }
    
    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_WirelessConfig.GetDriverName(), &g_WirelessConfig, sizeof(g_WirelessConfig), TRUE);    
}


//-----------------------------------------------------------------------------
//
//  InitializeMulticastDiscovery - Initialize the NETMF discovery service for
//  sockets.  We use multicast sockets to create the discovery mechanism.   
//
//-----------------------------------------------------------------------------
BOOL Sockets_Driver::InitializeMulticastDiscovery()
{
    NATIVE_PROFILE_PAL_COM();
    SOCKET_CHECK_ENTER(); 
    SOCK_sockaddr_in sockAddr;
    INT32 nonblocking = 1;

    if(g_Sockets_Driver.s_discoveryInitialized) return TRUE;

    // set up discovery socket to list to defined discovery port for any ip address
    memset( &sockAddr, 0, sizeof(sockAddr) );
    sockAddr.sin_family           = SOCK_AF_INET;
    sockAddr.sin_port             = SOCK_htons(SOCK_DISCOVERY_MULTICAST_PORT);
    sockAddr.sin_addr.S_un.S_addr = SOCK_htonl(SOCK_INADDR_ANY);

    // UDP socket is easier in our scenario because it isn't session based
    g_Sockets_Driver.m_multicastSocket = Socket( SOCK_AF_INET, SOCK_SOCK_DGRAM, SOCK_IPPROTO_UDP, TRUE );
    SOCKET_CHECK_RESULT( g_Sockets_Driver.m_multicastSocket );

    // set sock option for multicast
    SOCK_ip_mreq multicast;
    multicast.imr_multiaddr.S_un.S_addr = SOCK_htonl(SOCK_DISCOVERY_MULTICAST_IPADDR);
    multicast.imr_interface.S_un.S_addr = SOCK_htonl(SOCK_INADDR_ANY);

    SOCKET_CHECK_RESULT( SOCK_ioctl(g_Sockets_Driver.m_multicastSocket, SOCK_FIONBIO, &nonblocking) );

    SOCKET_CHECK_RESULT( SOCK_setsockopt( g_Sockets_Driver.m_multicastSocket, SOCK_IPPROTO_IP, SOCK_IPO_ADD_MEMBERSHIP, (const char*)&multicast, sizeof(multicast) ) );

    SOCKET_CHECK_RESULT( SOCK_bind( g_Sockets_Driver.m_multicastSocket, (SOCK_sockaddr*)&sockAddr, sizeof(sockAddr) ) );

    g_Sockets_Driver.s_discoveryInitialized = TRUE;

    SOCKET_CLEANUP()

    if(g_Sockets_Driver.m_multicastSocket != SOCK_SOCKET_ERROR)
    {
        SOCK_close(g_Sockets_Driver.m_multicastSocket);
        g_Sockets_Driver.m_multicastSocket = SOCK_SOCKET_ERROR;

        debug_printf("MULTICAST SOCKET_ERROR: %d\r\n", HAL_SOCK_getlasterror() );
    }

    SOCKET_CHECK_EXIT_BOOL_CLEANUP();
}

BOOL Sockets_Driver::Initialize()
{
    NATIVE_PROFILE_PAL_COM();
    SOCKET_CHECK_ENTER(); 

    
    if(!s_initialized)
    {
        g_Sockets_Driver.m_fShuttingDown = FALSE;

        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextReadSet);  
        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextWriteSet); 
        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextExcptSet); 

        SOCK_FD_ZERO(&g_Sockets_Driver.m_fdsetRead);
        SOCK_FD_ZERO(&g_Sockets_Driver.m_fdsetWrite);
        SOCK_FD_ZERO(&g_Sockets_Driver.m_fdsetExcept);

        g_Sockets_Driver.m_cntSockets = 0;

        for( INT32 i=0; i<SOCKETS_MAX_COUNT; i++ )
        {
            g_Sockets_Driver.m_socketHandles[i].m_socket  = SOCK_SOCKET_ERROR;
            g_Sockets_Driver.m_socketHandles[i].m_flags   = 0;
            g_Sockets_Driver.m_socketHandles[i].m_sslData = NULL;
        }

        g_Sockets_Driver.m_multicastSocket     = SOCK_SOCKET_ERROR;
        g_Sockets_Driver.m_SocketDebugStream   = SOCK_SOCKET_ERROR;
        g_Sockets_Driver.m_SocketDebugListener = SOCK_SOCKET_ERROR;

        g_Sockets_Driver.m_stateDebugSocket = DbgSock_Uninitialized;

        ApplyConfig();        
        ApplyWirelessConfig();
        
        SOCKET_CHECK_BOOL( HAL_SOCK_Initialize() );

        if(!InitializeMulticastDiscovery())
        {
            debug_printf("MULTICAST discovery failed\r\n");
        }

        HAL_SOCK_EventsSet(SOCKET_EVENT_FLAG_SOCKETS_READY);

        //SOCKET_CHECK_BOOL( InitializeDbgListener() );

        SSL_Initialize();

        s_initialized = TRUE;
    }
     
    
    SOCKET_CHECK_EXIT_BOOL();
}

BOOL Sockets_Driver::Uninitialize( )
{
    NATIVE_PROFILE_PAL_COM();
    BOOL ret = TRUE;
   
    if(s_initialized)
    {
        g_Sockets_Driver.m_stateDebugSocket = DbgSock_Uninitialized;

        // close all connections (including debugger sockets)
        CloseConnections(TRUE);

        SSL_Uninitialize();
    
        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextReadSet);  
        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextWriteSet); 
        SOCK_FD_ZERO(&g_Sockets_Driver.m_nextExcptSet); 

        SOCK_FD_ZERO(&g_Sockets_Driver.m_fdsetRead);
        SOCK_FD_ZERO(&g_Sockets_Driver.m_fdsetWrite);
        SOCK_FD_ZERO(&g_Sockets_Driver.m_fdsetExcept);

        g_Sockets_Driver.m_cntSockets = 0;

        for( INT32 i=0; i<SOCKETS_MAX_COUNT; i++ )
        {
            g_Sockets_Driver.m_socketHandles[i].m_socket  = SOCK_SOCKET_ERROR;
            g_Sockets_Driver.m_socketHandles[i].m_flags   = 0;
            g_Sockets_Driver.m_socketHandles[i].m_sslData = NULL;
        }

        g_Sockets_Driver.m_multicastSocket     = SOCK_SOCKET_ERROR;
        g_Sockets_Driver.m_SocketDebugStream   = SOCK_SOCKET_ERROR;
        g_Sockets_Driver.m_SocketDebugListener = SOCK_SOCKET_ERROR;

        ret = HAL_SOCK_Uninitialize();

        s_initialized          = FALSE;
        s_discoveryInitialized = FALSE;
        s_wirelessInitialized  = FALSE;
    }
   
    
    return ret;
}


static UINT64 s_lastWrite = 0;

//-----------------------------------------------------------------------------
//
//  Write - The Write method will write data to the debugger stream socket (if a connection is active).
//  In addition if the write fails for a reason other than EWOULDBLOCK then we should shutdown
//  the debugger stream socket and change to the listening state
//
//-----------------------------------------------------------------------------
INT32 Sockets_Driver::Write( int ComPortNum, const char* Data, size_t size )
{
    NATIVE_PROFILE_PAL_COM();
    INT32 ret;

    if(ComPortNum != ConvertCOM_SockPort(COM_SOCKET_DBG)) return SOCK_SOCKET_ERROR;

    if((g_Sockets_Driver.m_stateDebugSocket  != DbgSock_Connected) ||
       (g_Sockets_Driver.m_SocketDebugStream == SOCK_SOCKET_ERROR)) 
    {
        return SOCK_SOCKET_ERROR;
    }

    // the network stack (RTIP) can get overloaded with "hal_printf" messages which causes
    // the communication link to be killed (by sending a QUENCH message).  We have to add
    // an artificial delay here if the back-to-back writes are too close.
    UINT64 tics = HAL_Time_CurrentTicks();
    UINT64 cmp = (CPU_MillisecondsToTicks((UINT32)5) / 10) + 1; // at least 500 uSec
    if(((tics - s_lastWrite) < cmp))
    {
        // we can't sleep if the interrupts are off, so return
        if(!INTERRUPTS_ENABLED_STATE()) return 0;
        
        Events_WaitForEvents(0, 5);
    }

    ret = SOCK_send( g_Sockets_Driver.m_SocketDebugStream, Data, size, 0 );

    if(ret < 0) 
    {
        INT32 err = HAL_SOCK_getlasterror();

        // debugger stream is no longer active, change to listening state
        if(err != SOCK_EWOULDBLOCK)
        {
            CloseDebuggerSocket();
        }
        else
        {   
            ret = 0;
        }
    }
    else
    {
        s_lastWrite = HAL_Time_CurrentTicks();
    }

    return ret;
}

//-----------------------------------------------------------------------------
//
//  Read - the Read method performs two duties: first to read data from the debug stream; and 
//  second to manage the debugger connection state.  The initial state is to be listening for debug
//  connections, and therefore each read performs an accept to see if there is a pending connection.
//  Once a connection is made the state changes to connected and the debugger stream socket 
//  is read for each Read call.  During the Connected state an Accept is still called so that no other 
//  connections will be handled.  If the debugger is still receiving data on the debugger stream, then
//  new connections will be closed immediately.  If the debugger stream socket has been closed then
//  the state return to the listening state (unless there is an pending connection in which case the 
//  pending connection becomes the new debugger stream).
//
//-----------------------------------------------------------------------------
INT32 Sockets_Driver::Read( int ComPortNum, char* Data, size_t size )
{
    NATIVE_PROFILE_PAL_COM();
    SOCK_SOCKET sock;
    SOCK_sockaddr_in addr;
    INT32 len = sizeof(addr);
    INT32 ret = 0;

    if(ComPortNum != ConvertCOM_SockPort(COM_SOCKET_DBG)) return 0;

    if(g_Sockets_Driver.m_stateDebugSocket == DbgSock_Uninitialized) return 0;

    memset(&addr, 0, sizeof(addr));

    // if we are connected, then read from the debug stream
    if(g_Sockets_Driver.m_stateDebugSocket == DbgSock_Connected)
    {
        ret = SOCK_recv(g_Sockets_Driver.m_SocketDebugStream, Data, size, 0);

        // return value of zero indicates a shutdown of the socket.  Also we shutdown for any error except
        // ewouldblock.  If either of these happens, then we go back to the listening state
        if((ret == 0) || (ret == SOCK_SOCKET_ERROR && HAL_SOCK_getlasterror() != SOCK_EWOULDBLOCK))
        {
            CloseDebuggerSocket();
        }
        else if(ret != SOCK_SOCKET_ERROR && s_DebuggerTimeoutCompletion.IsLinked())
        {
            // we either got data or the socket was closed, so kill the timeout
            s_DebuggerTimeoutCompletion.Abort();
        }
    }

    // we always perform an accept so that we handle pending connections
    // if we alread are connected and the debug stream socket is still active, then we immediately close
    // the pending connection
    sock = Accept( g_Sockets_Driver.m_SocketDebugListener, (SOCK_sockaddr*)&addr, &len, TRUE );

    if(SOCK_SOCKET_ERROR != sock)
    {
        INT32 nonblocking = 1;
        BOOL  optval      = 1;
        INT32 optLinger   = 0;

        GLOBAL_LOCK_SOCKETS(lock);

        // if we are already in the connected state, then verify that the debugger stream socket is still active
        if(DbgSock_Connected == g_Sockets_Driver.m_stateDebugSocket)
        {
            // the debugger stream is still active, so shutdown the pending connection
            HAL_SOCK_setsockopt( sock, SOCK_SOL_SOCKET, SOCK_SOCKO_LINGER, (const char*)&optLinger, sizeof(INT32) );
            SOCK_close(sock);

            // set timeout since another connection is trying to use us.
            if(!s_DebuggerTimeoutCompletion.IsLinked())
            {
                s_DebuggerTimeoutCompletion.EnqueueDelta(5000000); // 5 seconds
            }
        }
        else // we are in the listening state, so accept the pending connection and update the state
        {
            g_Sockets_Driver.m_SocketDebugStream = sock;
            SOCKET_CHECK_RESULT( SOCK_ioctl(g_Sockets_Driver.m_SocketDebugStream, SOCK_FIONBIO, &nonblocking) );
            SOCKET_CHECK_RESULT( HAL_SOCK_setsockopt( g_Sockets_Driver.m_SocketDebugStream, SOCK_IPPROTO_TCP, SOCK_TCP_NODELAY, (char*)&optval, sizeof(optval) ) );
            SOCKET_CHECK_RESULT( HAL_SOCK_setsockopt( g_Sockets_Driver.m_SocketDebugStream, SOCK_SOL_SOCKET, SOCK_SOCKO_LINGER, (const char*)&optLinger, sizeof(INT32) ) );

            g_Sockets_Driver.m_stateDebugSocket = DbgSock_Connected;

            SOCKET_CLEANUP()

            if(g_Sockets_Driver.m_SocketDebugStream != SOCK_SOCKET_ERROR)
            {
                SOCK_close(g_Sockets_Driver.m_SocketDebugStream);
                g_Sockets_Driver.m_SocketDebugStream = SOCK_SOCKET_ERROR;

                debug_printf("DBGSTREAM SOCKET_ERROR: %d\r\n", HAL_SOCK_getlasterror() );
            }

            SOCKET_CHECK_EXIT_NORETURN();
        }
        
        HAL_SOCK_EventsSet( SOCKET_EVENT_FLAG_SOCKET );
    }


    if(ret < 0)
    {  
        ret = 0;
    }

    return ret;
}


//-----------------------------------------------------------------------------
//
//  ClearStatusBit - this method is used to remove the given socket from the given set
//  It is only used with the global read,write,except sets.  When the operation is complete
//  the select thread is notified so that it can update the select state.
//
//-----------------------------------------------------------------------------
void Sockets_Driver::ClearStatusBit( SOCK_SOCKET sock, SOCK_fd_set *fdset )
{
    NATIVE_PROFILE_PAL_COM();
    GLOBAL_LOCK_SOCKETS(lock);

    if(SOCK_FD_ISSET(sock, fdset))
    {
        SOCK_FD_CLR( sock, fdset );
    }
    HAL_SOCK_SignalSocketThread();
}

void Sockets_Driver::ClearStatusBitsForSocket(SOCK_SOCKET sock, BOOL fWrite)
{
    GLOBAL_LOCK_SOCKETS(lock);

    if(fWrite && SOCK_FD_ISSET( sock, &g_Sockets_Driver.m_fdsetWrite ))
    {
        SOCK_FD_CLR( sock, &g_Sockets_Driver.m_fdsetWrite );
    }
    if(!fWrite && SOCK_FD_ISSET( sock, &g_Sockets_Driver.m_fdsetRead ))
    {
        SOCK_FD_CLR( sock, &g_Sockets_Driver.m_fdsetRead );
    }
    if(SOCK_FD_ISSET( sock, &g_Sockets_Driver.m_fdsetExcept ))
    {
        SOCK_FD_CLR( sock, &g_Sockets_Driver.m_fdsetExcept );
    }
    HAL_SOCK_SignalSocketThread();
}

//-----------------------------------------------------------------------------
//
//  RegisterForSelect - this method is used to register a socket for selection
//    or ethernet activity.  A socket can only be included in the select if it 
//    is in a connected or listening state.
//
//-----------------------------------------------------------------------------
void Sockets_Driver::RegisterForSelect( SOCK_SOCKET sock, BOOL isSelectable )
{
    NATIVE_PROFILE_PAL_COM();
    if(sock == SOCK_SOCKET_ERROR) 
    {
        ASSERT(FALSE);
        return;
    }

    GLOBAL_LOCK_SOCKETS(lock);

    for(INT32 i=0; i<g_Sockets_Driver.m_cntSockets; i++)
    {
        if(sock == g_Sockets_Driver.m_socketHandles[i].m_socket)
        {
            if(isSelectable)
            {
                SET_SOCKET_FLAG(g_Sockets_Driver.m_socketHandles[i], SocketRegisterMap::c_SelectableSocket);
            }
            else
            {
                CLEAR_SOCKET_FLAG(g_Sockets_Driver.m_socketHandles[i], SocketRegisterMap::c_SelectableSocket);
            }
            break;
        }
    }

    if(isSelectable)
    {
        SOCK_FD_SET(sock, &g_Sockets_Driver.m_nextReadSet);
        SOCK_FD_SET(sock, &g_Sockets_Driver.m_nextWriteSet);
        SOCK_FD_SET(sock, &g_Sockets_Driver.m_nextExcptSet);
    }
    else
    {
        SOCK_FD_CLR(sock, &g_Sockets_Driver.m_fdsetRead  ); 
        SOCK_FD_CLR(sock, &g_Sockets_Driver.m_fdsetWrite ); 
        SOCK_FD_CLR(sock, &g_Sockets_Driver.m_fdsetExcept); 

        SOCK_FD_CLR(sock, &g_Sockets_Driver.m_nextReadSet );
        SOCK_FD_CLR(sock, &g_Sockets_Driver.m_nextWriteSet);
        SOCK_FD_CLR(sock, &g_Sockets_Driver.m_nextExcptSet);
    }

    // notify select thread to include socket in select call
    HAL_SOCK_SignalSocketThread();
}

//-----------------------------------------------------------------------------
//
//  RegisterSocket - socket tracking.  This method is used to track sockets
//    opened by this driver.  It does not include the fake socket.  Register
//    should be called after a socket is opened (via socket or accept methods).
//    The selectable parameter should only be true if the socket is in the 
//    listening or connected state
//
//-----------------------------------------------------------------------------
void Sockets_Driver::RegisterSocket( SOCK_SOCKET sock, BOOL selectable, BOOL fDebug )
{
    NATIVE_PROFILE_PAL_COM();
    if(sock == SOCK_SOCKET_ERROR)
    {
        ASSERT(FALSE);
        return;
    }

    if(g_Sockets_Driver.m_cntSockets >= SOCKETS_MAX_COUNT) return;

    GLOBAL_LOCK_SOCKETS(lock);

    g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets].m_socket  = sock;
    g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets].m_flags   = 0;
    g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets].m_sslData = NULL;

    if(selectable) SET_SOCKET_FLAG(g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets], SocketRegisterMap::c_SelectableSocket );
    if(fDebug    ) SET_SOCKET_FLAG(g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets], SocketRegisterMap::c_DebugSocket      );

    g_Sockets_Driver.m_cntSockets++;

    if(selectable)
    {
        SOCK_FD_SET(sock, &g_Sockets_Driver.m_nextReadSet );
        SOCK_FD_SET(sock, &g_Sockets_Driver.m_nextWriteSet);
        SOCK_FD_SET(sock, &g_Sockets_Driver.m_nextExcptSet);
        
        HAL_SOCK_SignalSocketThread();
    }
}

//-----------------------------------------------------------------------------
//
//  UnregisterSocket - No longer track a given socket for clean up and selection.
//    This method should only be called immediately before closing down a socket.
//
//-----------------------------------------------------------------------------
void Sockets_Driver::UnregisterSocket( INT32 index )
{
    NATIVE_PROFILE_PAL_COM();
    GLOBAL_LOCK_SOCKETS(lock);

    if(index < 0 || index >= g_Sockets_Driver.m_cntSockets) 
    {
        ASSERT(FALSE);
        return;
    }

    SOCK_SOCKET sock = g_Sockets_Driver.m_socketHandles[index].m_socket;

    SOCK_FD_CLR( sock, &g_Sockets_Driver.m_fdsetRead   ); 
    SOCK_FD_CLR( sock, &g_Sockets_Driver.m_fdsetWrite  ); 
    SOCK_FD_CLR( sock, &g_Sockets_Driver.m_fdsetExcept ); 

    SOCK_FD_CLR( sock, &g_Sockets_Driver.m_nextReadSet  ); 
    SOCK_FD_CLR( sock, &g_Sockets_Driver.m_nextWriteSet ); 
    SOCK_FD_CLR( sock, &g_Sockets_Driver.m_nextExcptSet ); 

    g_Sockets_Driver.m_cntSockets--;

    if(index != g_Sockets_Driver.m_cntSockets)
    {
        memcpy( &g_Sockets_Driver.m_socketHandles[index], &g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets], sizeof(g_Sockets_Driver.m_socketHandles[index]) );
    }

    g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets].m_socket = SOCK_SOCKET_ERROR;
    g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets].m_flags  = 0;
    g_Sockets_Driver.m_socketHandles[g_Sockets_Driver.m_cntSockets].m_socket = NULL;
}

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_SOCKETS_Driver"
#endif

BOOL Sockets_Driver::s_initialized=FALSE;
BOOL Sockets_Driver::s_wirelessInitialized=FALSE;
BOOL Sockets_Driver::s_discoveryInitialized=FALSE;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

