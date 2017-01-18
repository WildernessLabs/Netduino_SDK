#include <tinyclr/ssl_functions.h>
#include <openssl/ssl.h>
#include <openssl.h>

static const int  c_MaxSslDataSize = 1460; 

int ssl_write_internal( int sd, const char* data, size_t size )
{
    SSL *ssl = (SSL*)SOCKET_DRIVER.GetSocketSslData(sd);

    if(ssl == NULL || ssl == (void*)SSL_SOCKET_ATTEMPTED_CONNECT)
    {
        return SOCK_SOCKET_ERROR;
    }
    
    if(size > c_MaxSslDataSize)
    {
        size = c_MaxSslDataSize;
    }
    
    int sent = SSL_write( ssl, data, size ); 

    int err = SSL_get_error(ssl,sent);

    if(err == SSL_ERROR_WANT_WRITE)
    {
#ifndef TCPIP_LWIP
        SOCKET_DRIVER.ClearStatusBitsForSocket( sd, TRUE );
        return 0;
#endif
    }

#ifndef TCPIP_LWIP
    SOCKET_DRIVER.ClearStatusBitsForSocket( sd, TRUE );
#endif

    return sent;
}

