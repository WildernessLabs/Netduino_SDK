////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RTIP_SOCKETS_H_
#define _RTIP_SOCKETS_H_

#include <tinyhal.h>
#include <dhcpcapi.h>

//////////////////////////////////////////////////////////////////////////////
// SOCKET driver
// 

struct RTIP_DRIVER_INTERFACE_DATA
{
    int          m_interfaceNumber;
    DHCP_session m_currentDhcpSession;    
};


struct RTIP_SOCKETS_Driver
{
    static BOOL Initialize();
    
    static BOOL Uninitialize();
    
    static SOCK_SOCKET Socket( int family, int type, int protocol );
    
    static int Bind( SOCK_SOCKET socket, const SOCK_sockaddr* address, int addressLen  );

    static int Connect(SOCK_SOCKET socket, const SOCK_sockaddr* address, int addressLen);

    static int Send(SOCK_SOCKET socket, const char* buf, int len, int flags);

    static int Recv(SOCK_SOCKET socket, char* buf, int len, int flags);

    static int Close(SOCK_SOCKET socket);

    static int Listen(SOCK_SOCKET socket, int backlog );

    static SOCK_SOCKET Accept(SOCK_SOCKET socket, SOCK_sockaddr* address, int* addressLen );


    static int Shutdown(SOCK_SOCKET socket, int how );

    static int GetAddrInfo(const char* nodename, 
                  char* servname, 
                  const SOCK_addrinfo* hints, 
                  SOCK_addrinfo** res );

    static void FreeAddrInfo(SOCK_addrinfo* ai );

    static int Ioctl(SOCK_SOCKET socket, int cmd, int* data );

    static int GetLastError();

    static int GetNativeError( int error );

    static int GetNativeSockOption (int level, int optname);

    static int GetNativeIPOption (int optname);

    static int Select(int nfds, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* exceptfds, const SOCK_timeval* timeout );

    static int SetSockOpt(SOCK_SOCKET socket, int level, int optname, const char* optval, int  optlen );

    static int GetSockOpt(SOCK_SOCKET socket, int level, int optname, char* optval, int* optlen );

    static int GetPeerName(SOCK_SOCKET socket, SOCK_sockaddr* name, int* namelen );
    
    static int GetSockName(SOCK_SOCKET socket, SOCK_sockaddr* name, int* namelen );

    static int RecvFrom(SOCK_SOCKET s, char* buf, int len, int flags, SOCK_sockaddr* from, int* fromlen );
    
    static int SendTo(SOCK_SOCKET s, const char* buf, int len, int flags, const SOCK_sockaddr* to, int tolen );

    static UINT32 GetAdapterCount();

    static HRESULT LoadAdapterConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config );
    
    static HRESULT UpdateAdapterConfiguration( UINT32 interfaceIndex, UINT32 updateFlags, SOCK_NetworkConfiguration* config );

    static HRESULT LoadWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig );


private:
    RTIP_DRIVER_INTERFACE_DATA m_interfaces[NETWORK_INTERFACE_COUNT];
};

//
// SOCKET driver
/////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _RTIP_SOCKETS_H_ 1

