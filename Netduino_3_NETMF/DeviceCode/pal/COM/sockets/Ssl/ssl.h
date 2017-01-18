////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SSL_H_
#define _SSL_H_ 1

//--//

#include "tinyhal.h"
#include <rtipapi.h>
#include <apirtssl.h>
#include <sockets.h>

//--//

struct SSL_Driver
{
    static const int c_MaxSslContexts = SOCKETS_MAX_COUNT;
    static const int c_MaxCertificatesPerContext = 10;

    //--//

    struct SSL_Context
    {
        void* SslContext;

        RTCertificate m_certificates[c_MaxCertificatesPerContext];
        INT32         m_certificateCount;
    };

    SSL_Context        m_sslContextArray[c_MaxSslContexts];
    INT32              m_sslContextCount;   
    SSL_DATE_TIME_FUNC m_pfnGetTimeFuncPtr;

};

extern SSL_Driver g_SSL_Driver;

#endif //_SSL_H_
