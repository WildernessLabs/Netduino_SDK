////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#include "xnconf.h"
#include "dhcpconf.h"
#include "rtip.h"
#include "rtipapi.h"


extern "C"
{
int         xn_tcp_initialize( void ) { return 0; }
void        tcp_invalidate_sockets(RTP_PFUINT8 ip_addr) { }

/*
PTCPPORT    os_alloc_tcpport(void) { return 0; }
void        os_free_tcpport(PTCPPORT port) { }
*/
}


