////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <rtipapi.h>
#include <apirtssl.h>

extern "C"
{
void RAND_seed(const void *buf, int num)
{
}
}

void* secure_new_context(int sslMode)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return NULL;
}

int secure_server_init( void * sslctx, int sslverify, const RTCertificate* cert )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_client_init( void * sslctx, int sslverify, const RTCertificate* cert )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_associate_ca_store( void * sslctx, const RTCertificate* cert, int cert_count )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_accept( RTP_HANDLE sock, void *sslctx, void **s )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 2;
}

int secure_connect( RTP_HANDLE sock, const char* szTargetHost, void *sslctx, void **s )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_reconnect( RTP_HANDLE sock, void *sslctx, void **s )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_read( void *ssl,char *buffer,unsigned int buflen, unsigned int blocking )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_read_pending (void *ssl)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}


int secure_write( void *ssl,char *buffer,unsigned int buflen, unsigned int blocking )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_closesocket( RTP_HANDLE sd, void *s )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_sslexit( void *sctx )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

int secure_sslshutdown( void *s )
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

void secure_sslfree( void *s )
{
    NATIVE_PROFILE_PAL_NETWORK();
}

void secure_sslctxfree( void *sslctx )
{
    NATIVE_PROFILE_PAL_NETWORK();
}

int secure_parse_certificate(const RTCertificate* cert, RTCertData* certData)
{
    NATIVE_PROFILE_PAL_NETWORK();
    return 0;
}

void secure_exit( )
{
}

