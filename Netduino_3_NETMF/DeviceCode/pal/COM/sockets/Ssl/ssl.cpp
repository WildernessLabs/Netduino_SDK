////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ssl.h"

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_SSL_Driver"
#endif

SSL_Driver g_SSL_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

#define SSL_SOCKET_ATTEMPTED_CONNECT -1

//--//

BOOL SSL_Initialize()
{
    NATIVE_PROFILE_PAL_COM();
    memset(&g_SSL_Driver, 0, sizeof(g_SSL_Driver));

    return TRUE;
}

BOOL SSL_Uninitialize()
{
    NATIVE_PROFILE_PAL_COM();
    BOOL result = TRUE;
    
    for(int i = 0; i<ARRAYSIZE(g_SSL_Driver.m_sslContextArray); i++)
    {
        if(g_SSL_Driver.m_sslContextArray[i].SslContext != NULL)
        {
            SSL_ExitContext( i );
        }
    }

    secure_exit();
        
    g_SSL_Driver.m_sslContextCount = 0;
    
    return result;
}

static BOOL SSL_GenericInit( INT32 sslMode, INT32 sslVerify, const char* certificate, INT32 cert_len, INT32& sslContextHandle, BOOL isServer )
{
    NATIVE_PROFILE_PAL_COM();
    
    void *         ctx         = NULL;
    RTCertificate* rtCert      = NULL;
    int            sslCtxIndex = -1;

    if(g_SSL_Driver.m_sslContextCount >= ARRAYSIZE(g_SSL_Driver.m_sslContextArray))
    {
        return FALSE;
    }

    for(int i=0; i<ARRAYSIZE(g_SSL_Driver.m_sslContextArray); i++)
    { 
        if(g_SSL_Driver.m_sslContextArray[i].SslContext == NULL)
        {
            sslCtxIndex = i; 
           
            break;
        }
    }

    if(sslCtxIndex == -1) return FALSE;

    ctx = secure_new_context((isServer ? RTSSL_SECURE_SERVER : RTSSL_SECURE_CLIENT) | sslMode);
    
    if(NULL != ctx)
    {
        int ret;

        rtCert = &g_SSL_Driver.m_sslContextArray[sslCtxIndex].m_certificates[0];

        //always increment the cert count (even if no personal cert) because CA certs are offset by one
        g_SSL_Driver.m_sslContextArray[sslCtxIndex].m_certificateCount = 1;

        if(certificate != NULL)
        {
            // first cert is always the devices outbound certificate
            rtCert->cert = (X509Certificate)certificate;
            rtCert->size = cert_len;
        }
        else
        {
            rtCert->cert = NULL;
            rtCert->size = 0;
        }

        if(isServer)
        {
            ret = secure_server_init(ctx, sslVerify, rtCert );
        }
        else
        {
            ret = secure_client_init(ctx, sslVerify, rtCert );
        }

        if(ret == 1)
        {
            g_SSL_Driver.m_sslContextArray[sslCtxIndex].SslContext = ctx;
            g_SSL_Driver.m_sslContextCount ++;

            sslContextHandle = sslCtxIndex;
        }
        else
        {
            secure_sslexit(ctx);
            ctx = NULL;
        }            
    }

    if(ctx == NULL)
    {
        rtCert->cert = NULL;
        rtCert->size = 0;
    }
        
    return (ctx != NULL);     
}

void SSL_GetTime(DATE_TIME_INFO* pdt)
{
    NATIVE_PROFILE_PAL_COM();
    if(pdt == NULL) return;
    
    if(g_SSL_Driver.m_pfnGetTimeFuncPtr != NULL)
    {
        g_SSL_Driver.m_pfnGetTimeFuncPtr(pdt);
    }
    else
    {
       pdt->year     = 2009;
       pdt->month    = 1;
       pdt->day      = 1;
       pdt->hour     = 0;
       pdt->minute   = 0;
       pdt->second   = 0;
       pdt->msec     = 0;
       pdt->dlsTime  = 0;
       pdt->tzOffset = -8 * 60 * 60;
        
    }
}

void SSL_RegisterTimeCallback(SSL_DATE_TIME_FUNC pfn)
{
    NATIVE_PROFILE_PAL_COM();
    g_SSL_Driver.m_pfnGetTimeFuncPtr = pfn;
}

BOOL SSL_ServerInit( INT32 sslMode, INT32 sslVerify, const char* certificate, INT32 cert_len, const char* szCertPwd, INT32& sslContextHandle )
{
    NATIVE_PROFILE_PAL_COM();
    return SSL_GenericInit( sslMode, sslVerify, certificate, cert_len, sslContextHandle, TRUE );
}

BOOL SSL_ClientInit( INT32 sslMode, INT32 sslVerify, const char* certificate, INT32 cert_len, const char* szCertPwd, INT32& sslContextHandle )
{ 
    NATIVE_PROFILE_PAL_COM();
    return SSL_GenericInit( sslMode, sslVerify, certificate, cert_len, sslContextHandle, FALSE );
}

BOOL SSL_AddCertificateAuthority( int sslContextHandle, const char* certificate, int cert_len, const char* szCertPwd )
{
    NATIVE_PROFILE_PAL_COM();
    SSL_Driver::SSL_Context *ctx;
    RTCertificate *rtCert;

    if((sslContextHandle >= ARRAYSIZE(g_SSL_Driver.m_sslContextArray)) || 
       (sslContextHandle < 0) || 
       (g_SSL_Driver.m_sslContextArray[sslContextHandle].m_certificateCount >= SSL_Driver::c_MaxCertificatesPerContext))
    {
        return FALSE;
    }

    ctx = &g_SSL_Driver.m_sslContextArray[sslContextHandle];

    rtCert = &ctx->m_certificates[ctx->m_certificateCount];
    ctx->m_certificateCount++;
    
    rtCert->cert = (X509Certificate)certificate;
    rtCert->size = cert_len;

    return TRUE;    
}

void SSL_ClearCertificateAuthority( int sslContextHandle )
{
    NATIVE_PROFILE_PAL_COM();

    if((sslContextHandle >= ARRAYSIZE(g_SSL_Driver.m_sslContextArray)) || (sslContextHandle < 0))
    {
        return;
    }

    memset(&g_SSL_Driver.m_sslContextArray[sslContextHandle].m_certificates[0], 0, sizeof(g_SSL_Driver.m_sslContextArray[sslContextHandle].m_certificates));
    g_SSL_Driver.m_sslContextArray[sslContextHandle].m_certificateCount = 0;
}

BOOL SSL_ExitContext( INT32 sslContextHandle )
{ 
    NATIVE_PROFILE_PAL_COM();
    BOOL ret;
    
    if((sslContextHandle >= ARRAYSIZE(g_SSL_Driver.m_sslContextArray)) || (sslContextHandle < 0) || (g_SSL_Driver.m_sslContextArray[sslContextHandle].SslContext == NULL))
    {
        return FALSE;
    }

    ret = (0 == secure_sslexit(g_SSL_Driver.m_sslContextArray[sslContextHandle].SslContext));

    memset(&g_SSL_Driver.m_sslContextArray[sslContextHandle], 0, sizeof(g_SSL_Driver.m_sslContextArray[sslContextHandle]));

    g_SSL_Driver.m_sslContextCount --;

    return ret;
}

INT32 SSL_Accept( SOCK_SOCKET socket, INT32 sslContextHandle )
{ 
    NATIVE_PROFILE_PAL_COM();
    void *sslData = g_Sockets_Driver.GetSocketSslData(socket);
    SSL_Driver::SSL_Context *ctx;
    INT32 ret = 0;
    
    if((sslContextHandle >= ARRAYSIZE(g_SSL_Driver.m_sslContextArray)) || (sslContextHandle < 0))
    {
        return SOCK_SOCKET_ERROR;
    }

    ctx = &g_SSL_Driver.m_sslContextArray[sslContextHandle];

    // WARNING - SSL_Accept is asynchronous and will be called multiple times for 1 connection, therefore
    // we only want to set the CA store on the first call (when sslData == NULL)
    //
    // The first certificate is the device's outbound certificate
    if( sslData == NULL && ctx->m_certificateCount > 1)
    {
        if(-1 == secure_associate_ca_store( ctx->SslContext, &ctx->m_certificates[1], ctx->m_certificateCount - 1 ))
        {
            return SOCK_SOCKET_ERROR;
        }
    }

    if(sslData == (void*)SSL_SOCKET_ATTEMPTED_CONNECT)
    {
        sslData = NULL;
    }
    

    ret = secure_accept( socket, ctx->SslContext, &sslData );

    switch(ret)
    {
        case RTSSL_WANT_READ:
            ret = SOCK_EWOULDBLOCK;
            Sockets_Driver::ClearStatusBitsForSocket( socket, FALSE );        
            break;

        case RTSSL_WANT_WRITE:
            ret = SOCK_TRY_AGAIN;
            Sockets_Driver::ClearStatusBitsForSocket( socket, TRUE );        
            break;

        case 0:
            Sockets_Driver::ClearStatusBitsForSocket( socket, FALSE ); 
            break;
    }

    if(sslData != NULL)
    {
        g_Sockets_Driver.SetSocketSslData(socket, sslData);
    }
    // make sure we call secure_close_socket if we attempted a connect    
    else
    {
        g_Sockets_Driver.SetSocketSslData(socket, (void*)SSL_SOCKET_ATTEMPTED_CONNECT);
    }

    Sockets_Driver::ClearStatusBitsForSocket( socket, FALSE );
    Sockets_Driver::ClearStatusBitsForSocket( socket, TRUE );

    return ret; 
}

INT32 SSL_Connect( SOCK_SOCKET socket, const char* szTargetHost, INT32 sslContextHandle )
{ 
    NATIVE_PROFILE_PAL_COM();
    void *sslData = g_Sockets_Driver.GetSocketSslData(socket);
    SSL_Driver::SSL_Context *ctx;
    int ret = 0;
    
    if((sslContextHandle >= ARRAYSIZE(g_SSL_Driver.m_sslContextArray)) || (sslContextHandle < 0))
    {
        return SOCK_SOCKET_ERROR;
    }

    ctx = &g_SSL_Driver.m_sslContextArray[sslContextHandle];

    // WARNING - SSL_Connect is asynchronous and will be called multiple times for 1 connection, therefore
    // we only want to set the CA store on the first call (when sslData == NULL)
    //
    // The first certificate is the device's outbound certificate
    if(sslData == NULL && ctx->m_certificateCount > 1)
    {
        if(-1 == secure_associate_ca_store( ctx->SslContext, &ctx->m_certificates[1], ctx->m_certificateCount - 1 ))
        {
            return SOCK_SOCKET_ERROR;
        }
    }

    if(sslData == (void*)SSL_SOCKET_ATTEMPTED_CONNECT)
    {
        sslData = NULL;
    }
    
    ret = secure_connect( socket, szTargetHost, ctx->SslContext, &sslData );

    if(ret == RTSSL_WANT_READ)
    {
        ret = SOCK_EWOULDBLOCK;

        Sockets_Driver::ClearStatusBitsForSocket( socket, FALSE );        
    }

    if(ret == RTSSL_WANT_WRITE)
    {
        ret = SOCK_TRY_AGAIN;

        Sockets_Driver::ClearStatusBitsForSocket( socket, TRUE );        
    }

    if(sslData != NULL)
    {
        g_Sockets_Driver.SetSocketSslData(socket, sslData);
    }
    // make sure we call secure_close_socket if we attempted a connect    
    else
    {
        g_Sockets_Driver.SetSocketSslData(socket, (void*)SSL_SOCKET_ATTEMPTED_CONNECT);
    }

    Sockets_Driver::ClearStatusBitsForSocket( socket, FALSE );
    Sockets_Driver::ClearStatusBitsForSocket( socket, TRUE );

    return ret; 
}

static const int  c_MaxSslDataSize = 1460; 

INT32 SSL_Write( SOCK_SOCKET socket, const char* Data, size_t size  )
{ 
    NATIVE_PROFILE_PAL_COM();
    void *sslData = g_Sockets_Driver.GetSocketSslData(socket);
    INT32 sent = 0;

    if(sslData == NULL || sslData == (void*)SSL_SOCKET_ATTEMPTED_CONNECT)
    {
        return SOCK_SOCKET_ERROR;
    }

    if(size > c_MaxSslDataSize)
    {
        size = c_MaxSslDataSize;
    }
    
    sent = secure_write( sslData, (char*)Data, size, 0 ); 

    if(sent == RTSSL_WANT_WRITE)
    {
        Sockets_Driver::ClearStatusBitsForSocket( socket, TRUE );
        return 0;
    }

    Sockets_Driver::ClearStatusBitsForSocket( socket, TRUE );

    return sent;
}

INT32 SSL_Read( SOCK_SOCKET socket, char* Data, size_t size )
{ 
    NATIVE_PROFILE_PAL_COM();

    void *sslData = g_Sockets_Driver.GetSocketSslData(socket);
    INT32 recv = 0;

    if(sslData == NULL || sslData == (void*)SSL_SOCKET_ATTEMPTED_CONNECT)
    {
        return SOCK_SOCKET_ERROR;
    }
    
    recv = secure_read( sslData, Data, size, 0 ); 

    if(recv == RTSSL_WANT_READ)
    {
        Sockets_Driver::ClearStatusBitsForSocket( socket, FALSE );
        return SSL_RESULT__WOULD_BLOCK;
    }

    if(SSL_DataAvailable(socket) <= 0)
    {
        Sockets_Driver::ClearStatusBitsForSocket( socket, FALSE );
    }

    return recv;
}

INT32 SSL_CloseSocket( SOCK_SOCKET socket )
{
    NATIVE_PROFILE_PAL_COM();
    INT32 ret = 0;
    INT32 socketIndex = -1;

    GLOBAL_LOCK_SOCKETS(x);

    void *sslData = g_Sockets_Driver.GetSocketSslData(socket, socketIndex);

    if((sslData != NULL) && (socketIndex != -1))
    {
        g_Sockets_Driver.SetSocketSslData(socket, NULL);

        g_Sockets_Driver.UnregisterSocket( socketIndex );

        ret = secure_closesocket( socket, (sslData == (void*)SSL_SOCKET_ATTEMPTED_CONNECT ? NULL : sslData) );

        HAL_SOCK_SignalSocketThread();
    }
    else // ssl connect never opened or failed to connect (no error)
    {
        SOCK_close(socket);
    }

    return ret;
}

BOOL SSL_ParseCertificate( const char* certificate, size_t certLength, const char* szPwd, X509CertData* certData )
{
    NATIVE_PROFILE_PAL_COM();
    RTCertificate rt_cert;
    int ret;

    rt_cert.cert = (X509Certificate)certificate;
    rt_cert.size = certLength;

    ret = secure_parse_certificate( (const RTCertificate *)&rt_cert, (RTCertData*)certData );

    return (-1 != ret);
}

INT32 SSL_DataAvailable( SOCK_SOCKET socket )
{
    SOCK_fd_set  fdSock;
    SOCK_timeval to;
    INT32 avail = 0;
    void *sslData = g_Sockets_Driver.GetSocketSslData(socket);

    if(sslData == NULL  || sslData == (void*)SSL_SOCKET_ATTEMPTED_CONNECT) return -1;

    to.tv_sec = 0;
    to.tv_usec = 0;

    fdSock.fd_count= 1;
    fdSock.fd_array[0] = socket;

    // WARNING!!!! secure_read_pending will block if there is no data in the queue
    if(SOCK_select(SOCK_FD_SETSIZE, &fdSock, NULL, NULL, &to) > 0)
    {
        avail = secure_read_pending( sslData );

        // the socket is selected, so there must be some data (maybe a close socket)
        if(avail == 0) avail = 1;
    }

    return avail;    
}

