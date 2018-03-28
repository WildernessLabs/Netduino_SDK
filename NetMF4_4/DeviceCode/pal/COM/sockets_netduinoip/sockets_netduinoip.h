////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SOCKETS_H_
#define _SOCKETS_H_ 1

//--//

#include "tinyhal.h"

//--//

extern NETWORK_CONFIG          g_NetworkConfig;
extern NETWORK_CONFIG_EXTENDED g_NetworkConfigExtended;
extern WIRELESS_CONFIG         g_WirelessConfig;

//--//

struct Sockets_NetduinoIP_Driver
{
    
    static BOOL  Initialize();

    static void SaveConfig(INT32 index, SOCK_NetworkConfiguration *cfg);
    static void SaveConfigExtended(INT32 index, SOCK_NetworkConfigurationExtended *cfg);

    static void ApplyWirelessConfig();

    static void ApplyConfig();    
    static void ApplyConfigExtended();    
    static void SaveWirelessConfig(INT32 index, SOCK_NetworkConfiguration *cfg);


    static BOOL           s_initialized;
    static BOOL           s_wirelessInitialized;
};

#endif //_SOCKETS_H_