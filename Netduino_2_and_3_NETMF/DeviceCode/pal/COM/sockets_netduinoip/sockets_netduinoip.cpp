////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "sockets_netduinoip.h"

//--//

BOOL Network_Initialize()
{ 
    NATIVE_PROFILE_PAL_COM();
    return Sockets_NetduinoIP_Driver::Initialize( );
}

BOOL Network_Uninitialize()
{ 
    NATIVE_PROFILE_PAL_COM();
    return TRUE; 
}

BOOL SOCKETS_Initialize( int ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();
    return TRUE;
}

BOOL SOCKETS_Uninitialize( int ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();
    return TRUE;
}

int SOCKETS_Write( int ComPortNum, const char* Data, size_t size )
{ 
    NATIVE_PROFILE_PAL_COM();
    return 0; 
}

int SOCKETS_Read( int ComPortNum, char* Data, size_t size )
{ 
    NATIVE_PROFILE_PAL_COM();
    return 0; 
}

BOOL SOCKETS_Flush( int ComPortNum )
{ 
    NATIVE_PROFILE_PAL_COM();
    return FALSE; 
}

void SOCKETS_CloseConnections()
{
    NATIVE_PROFILE_PAL_COM();
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


BOOL SOCKETS_ProcessSocketActivity(SOCK_SOCKET signalSocket)
{
    NATIVE_PROFILE_PAL_COM();
    return TRUE;
}

UINT32 SOCK_CONFIGURATION_GetAdapterCount()
{
    NATIVE_PROFILE_PAL_COM();
    return NETWORK_INTERFACE_COUNT;
}

HRESULT SOCK_CONFIGURATION_LoadAdapterConfiguration( UINT32 interfaceIndex, SOCK_NetworkConfiguration* config )
{
    NATIVE_PROFILE_PAL_COM();

    if(interfaceIndex >= NETWORK_INTERFACE_COUNT) 
    {
        return CLR_E_INVALID_PARAMETER;
    }

    memcpy(config, &g_NetworkConfig.NetworkInterfaces[interfaceIndex], sizeof(g_NetworkConfig.NetworkInterfaces[interfaceIndex]));

    return S_OK;
}

HRESULT SOCK_CONFIGURATION_UpdateAdapterConfiguration( UINT32 interfaceIndex, UINT32 updateFlags, SOCK_NetworkConfiguration* config )
{ 
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
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

	Sockets_NetduinoIP_Driver::ApplyWirelessConfig();

    memcpy( wirelessConfig, &g_WirelessConfig.WirelessInterfaces[interfaceIndex], sizeof(SOCK_WirelessConfiguration) );

    return S_OK;
}

HRESULT SOCK_CONFIGURATION_UpdateWirelessConfiguration( UINT32 interfaceIndex, SOCK_WirelessConfiguration* wirelessConfig )
{
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}

HRESULT SOCK_CONFIGURATION_SaveAllWirelessConfigurations( )
{
    NATIVE_PROFILE_PAL_COM();
    return CLR_E_FAIL; 
}



//--//

void Sockets_NetduinoIP_Driver::SaveConfig(INT32 index, SOCK_NetworkConfiguration *cfg)
{
    NATIVE_PROFILE_PAL_COM();
    if(index >= NETWORK_INTERFACE_COUNT) return;

    if(cfg) 
    {
        memcpy( &g_NetworkConfig.NetworkInterfaces[index], cfg, sizeof(SOCK_NetworkConfiguration) );
    }

    // if the default config has not been applied yet: retrieve our MAC address from OTP
    UINT32 otpIndex = 0;
    UINT32 otpSectionIndex = 0;
    BOOL otpSectionIsAllOnes = FALSE;
    BOOL otpSectionIsAllZeroes = FALSE;
    BOOL otpMacAddressIsAllOnes = FALSE;
    if (!cfg && (index == 0))
    {			
			  BYTE* otpBuffer = (BYTE*)0x1FFF7800;
        {
            for (otpIndex = 0; otpIndex < 32; otpIndex += 8)
            {
                otpSectionIsAllOnes = TRUE;
                otpSectionIsAllZeroes = TRUE;
                for (otpSectionIndex = otpIndex; otpSectionIndex < otpIndex + 8; otpSectionIndex++)
                {
                    if (otpBuffer[otpSectionIndex] != 0xFF)
                        otpSectionIsAllOnes = FALSE;
                    if (otpBuffer[otpSectionIndex] != 0x00)
                        otpSectionIsAllZeroes = FALSE;
                }
                
                // if OTP section is all 0xFF, OTP is not used; break.
                if (otpSectionIsAllOnes)
                {
                    break;
                }
                // if OTP section is not empty, read out the MAC address
                if (!otpSectionIsAllZeroes)
                {
                    // verify that MAC address is not all 0xFF (unpopulated)
                    otpMacAddressIsAllOnes = TRUE;
                    for (otpSectionIndex = otpIndex + 2; otpSectionIndex < otpIndex + 8; otpSectionIndex++)
                    {
                        if (otpBuffer[otpSectionIndex] != 0xFF)
                            otpMacAddressIsAllOnes = FALSE;
                    }   
                    // if a MAC address is stored, read it in now.
                    if (!otpMacAddressIsAllOnes)
                    {
                        memcpy(&g_NetworkConfig.NetworkInterfaces[index].macAddressBuffer, (void*)&otpBuffer[otpIndex + 2], 6);
                    }
                    // we have already processed OTP; break out of our loop.
                    break;
                }
            }
        }
    }	
    
    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_NetworkConfig.GetDriverName(), &g_NetworkConfig, sizeof(g_NetworkConfig), TRUE);
}

void Sockets_NetduinoIP_Driver::ApplyConfig()
{
    NATIVE_PROFILE_PAL_COM();
    if(!HAL_CONFIG_BLOCK::ApplyConfig( g_NetworkConfig.GetDriverName(), &g_NetworkConfig, sizeof(g_NetworkConfig) ))
    {
        // save to the dynamic config section so that MFDeploy will be able to get the configuration.
        SaveConfig(0, NULL);            
    }
}

void Sockets_NetduinoIP_Driver::SaveConfigExtended(INT32 index, SOCK_NetworkConfigurationExtended *cfg)
{
    NATIVE_PROFILE_PAL_COM();
    if(index >= NETWORK_INTERFACE_COUNT) return;

    if(cfg) 
    {
        memcpy( &g_NetworkConfigExtended.NetworkInterfaces[index], cfg, sizeof(SOCK_NetworkConfigurationExtended) );
    }

    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_NetworkConfigExtended.GetDriverName(), &g_NetworkConfigExtended, sizeof(g_NetworkConfigExtended), TRUE);
}

void Sockets_NetduinoIP_Driver::ApplyConfigExtended()
{
    NATIVE_PROFILE_PAL_COM();
    if(!HAL_CONFIG_BLOCK::ApplyConfig( g_NetworkConfigExtended.GetDriverName(), &g_NetworkConfigExtended, sizeof(g_NetworkConfigExtended) ))
    {
        // save to the dynamic config section so that MFDeploy will be able to get the configuration.
        SaveConfigExtended(0, NULL);            
    }
}

void Sockets_NetduinoIP_Driver::ApplyWirelessConfig()
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

void Sockets_NetduinoIP_Driver::SaveWirelessConfig(INT32 index, SOCK_NetworkConfiguration *cfg)
{
    NATIVE_PROFILE_PAL_COM();
    if(index >= WIRELESS_INTERFACE_COUNT) return;
    
    if(cfg) 
    {
        memcpy( &g_WirelessConfig.WirelessInterfaces[index], cfg, sizeof(SOCK_WirelessConfiguration) );
    }
    
    HAL_CONFIG_BLOCK::UpdateBlockWithName(g_WirelessConfig.GetDriverName(), &g_WirelessConfig, sizeof(g_WirelessConfig), TRUE);    
}


//-//--//--//
    
BOOL Sockets_NetduinoIP_Driver::Initialize()
{
    NATIVE_PROFILE_PAL_COM();

    if(!s_initialized)
    {
        ApplyConfig();        
		ApplyConfigExtended();
        ApplyWirelessConfig();
        
        s_initialized = TRUE;
    }
    
    return s_initialized;
}

int SOCK_socket( int family, int type, int protocol ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_bind( int socket, const struct SOCK_sockaddr* address, int addressLen  ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_connect(int socket, const struct SOCK_sockaddr* address, int addressLen) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_send(int socket, const char* buf, int len, int flags) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_recv(int socket, char* buf, int len, int timeout) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_close(int socket) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_listen( int socket, int backlog ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_accept( int socket, struct SOCK_sockaddr* address, int* addressLen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_shutdown( int socket, int how ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getaddrinfo(  const char* nodename, char* servname, const struct SOCK_addrinfo* hints, struct SOCK_addrinfo** res ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

void SOCK_freeaddrinfo( struct SOCK_addrinfo* ai )
{
    NATIVE_PROFILE_PAL_COM();
}
    

int SOCK_ioctl( int socket, int cmd, int* data ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getlasterror() 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_select( int socket, SOCK_fd_set* readfds, SOCK_fd_set* writefds, SOCK_fd_set* except, const struct SOCK_timeval* timeout ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_setsockopt( int socket, int level, int optname, const char* optval, int  optlen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getsockopt( int socket, int level, int optname,       char* optval, int* optlen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getpeername( int socket, struct SOCK_sockaddr* name, int* namelen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_getsockname( int socket, struct SOCK_sockaddr* name, int* namelen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_recvfrom( int s, char* buf, int len, int flags, struct SOCK_sockaddr* from, int* fromlen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

int SOCK_sendto( int s, const char* buf, int len, int flags, const struct SOCK_sockaddr* to, int tolen ) 
{ 
    NATIVE_PROFILE_PAL_COM();
    return SOCK_SOCKET_ERROR; 
}

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_Sockets_NetduinoIP_Driver"
#endif

BOOL Sockets_NetduinoIP_Driver::s_initialized=FALSE;
BOOL Sockets_NetduinoIP_Driver::s_wirelessInitialized=FALSE;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

