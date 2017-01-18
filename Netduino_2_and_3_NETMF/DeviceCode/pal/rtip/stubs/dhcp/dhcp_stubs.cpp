////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "Dhcpcapi.h"
#include "dhcpext.h"

//--//

///////////////////////////////////////////////////////////////////////////////
// basic API functions

CFG_DHCPC_DATA cfg_dhcpc_data = 
{
    0,
    0,
};


INIT_FNCS dhcp_fnc =
{
    NULL,
    NULL,
};


ARP_CALLBACK_DHCP_FNC arp_callback_dhcp_fnc = NULL;


void init_dhcp(void)
{
    NATIVE_PROFILE_PAL_NETWORK();
}

void xn_dhcp_init_conf(PFDHCP_conf conf)
{
    NATIVE_PROFILE_PAL_NETWORK();
}

int xn_dhcp(int iface_no,PFDHCP_session session,PFDHCP_conf conf)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int xn_dhcp_inform(int iface_no, PFDHCP_session session, PFDHCP_conf conf)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int xn_dhcp_extend_lease(PFDHCP_session session,RTP_UINT32 lease_time)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int xn_dhcp_release(PFDHCP_session session)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

// basic API functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// API functions for advanced use of DHCP
void xn_dhcp_set_callback(DHCP_callback_fp fp)
{
    NATIVE_PROFILE_PAL_NETWORK();
}

int  xn_dhcp_get_op(PFDHCP_session session,RTP_UINT8 op_id,RTP_UINT8 bufsize,RTP_PFVOID vpdest)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

void xn_dhcp_set_conf_op(PFDHCP_conf conf,PFDHCP_cparam cpdata)
{
    NATIVE_PROFILE_PAL_NETWORK();
}
// API functions for advanced use of DHCP
///////////////////////////////////////////////////////////////////////////////

