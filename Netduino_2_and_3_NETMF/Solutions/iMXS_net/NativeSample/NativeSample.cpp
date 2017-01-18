////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <AsyncProcCalls_decl.h>
#include <Tests.h>
#include "nativesample.h"

#ifdef TCPIP_LWIP 
void  Network_PostEvent(UINT32 evt, UINT32 data)
{
}
#endif

#define USE_TCP 1

//
// 
// To test OpenSSL, define TEST_OPEN_SOURCE_NETWORKSTACK and build with: OPENSSLLIBRARIES=true, e.g.:
// 'msbuild /p:OPENSSLLIBRARIES=true /p:platform=imxs_net Solutions\iMXS_net\NativeSample\NativeSample.proj'
//
//

//#define TEST_OPEN_SOURCE_NETWORKSTACK


#ifdef TEST_OPEN_SOURCE_NETWORKSTACK

#ifdef TCPIP_LWIP 
#include "ipv4\lwip\ip_addr.h"
#include "netif\etharp.h"
#include "loopback_lwip_driver.h"
#include "lwip\init.h"
#include "lwip\tcpip.h"
#include "lwip\opt.h"
#include "lwip\netif.h"
#include "lwip\tcp.h"
#include "lwip\udp.h"
#include "lwip\sockets.h"
#else
#include "rtp.h"
#include "Socket.h"
#endif

#include <tinyclr/ssl_functions.h>
#include <tinyclr/ssl_types.h>
#include <tinyclr/test/ssl_tests.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

char* my_cert = 
"-----BEGIN CERTIFICATE-----\n" \
"MIIDmzCCAoOgAwIBAgICAR4wDQYJKoZIhvcNAQEFBQAwgZMxCzAJBgNVBAYTAnVz\n"\
"MQswCQYDVQQIEwJ3YTEQMA4GA1UEBxMHUmVkbW9uZDESMBAGA1UEChMJTWljcm9z\n"\
"b2Z0MQ0wCwYDVQQLEwRTUE9UMR0wGwYDVQQDExREZXZpY2UuTWljcm9zb2Z0LkNv\n"\
"bTEjMCEGCSqGSIb3DQEJARYUYWxkZW5sQG1pY3Jvc29mdC5jb20wHhcNMDgwMzIx\n"\
"MjIzMjA4WhcNMDkwMzIxMjIzMjA4WjCBkzELMAkGA1UEBhMCdXMxCzAJBgNVBAgT\n"\
"AndhMRAwDgYDVQQHEwdyZWRtb25kMRIwEAYDVQQKEwltaWNyb3NvZnQxDTALBgNV\n"\
"BAsTBFNQT1QxHTAbBgNVBAMTFERldmljZS5NaWNyb3NvZnQuQ29tMSMwIQYJKoZI\n"\
"hvcNAQkBFhRhbGRlbmxAbWljcm9zb2Z0LmNvbTCBnzANBgkqhkiG9w0BAQEFAAOB\n"\
"jQAwgYkCgYEAuey3jRMPDJDdKpzuyKqygtg6NJGWHZGAncG2gEm+BDPI6qseyyw8\n"\
"UU0RzzQobV+G/2fuklCJ6Ge3HXMNqMGO1/HOpzEXDSpxcCcTCYQn+jPlHrSVNsgo\n"\
"CF26EMdGeGmXicEBFyni22mhqHCRHfg0aN22zfKbTc2YqzldY7vIMLUCAwEAAaN7\n"\
"MHkwCQYDVR0TBAIwADAsBglghkgBhvhCAQ0EHxYdT3BlblNTTCBHZW5lcmF0ZWQg\n"\
"Q2VydGlmaWNhdGUwHQYDVR0OBBYEFCSj1pyMxm/K7bjeQUARsxBmdQneMB8GA1Ud\n"\
"IwQYMBaAFNndm5RrijyuKlG2PX5OfJuTZDDjMA0GCSqGSIb3DQEBBQUAA4IBAQB+\n"\
"CKvvh0x5OjGe5cdVXaHCAGuYZRy+wJttNZxei12j8qXKS9wa3H9hax1OU03Flw0H\n"\
"UDdzQG6pi0FVASdVt39wCyfkEoe9NTcJ9xkDQqbuG2vfuWfyzg96Ms1qSim+yxUU\n"\
"f4MvZ94MmQmqSrbKzPwWfwecjI80zUDAKw6bbLOsWKhWliGM5vwr3Ebr34iSpQTp\n"\
"ltiI3kdtYE0wqqsb6p4vcVonDAWQIZ9JufJyjomW+rrKkrpVRaSYt7othlQWlHrP\n"\
"QZoZBQ4inpzlPWVE6Tz8YkxaBbTZGIfllGUj607d9+KXhGkVv5Gc+U/EYeugAfhO\n"\
"6HYWqnsXlsL1Knn70wE9\n"\
"-----END CERTIFICATE-----\n\0";


char* server_pem = 
"-----BEGIN CERTIFICATE-----\n"\
"MIIB6TCCAVICAQYwDQYJKoZIhvcNAQEEBQAwWzELMAkGA1UEBhMCQVUxEzARBgNV\n"\
"BAgTClF1ZWVuc2xhbmQxGjAYBgNVBAoTEUNyeXB0U29mdCBQdHkgTHRkMRswGQYD\n"\
"VQQDExJUZXN0IENBICgxMDI0IGJpdCkwHhcNMDAxMDE2MjIzMTAzWhcNMDMwMTE0\n"\
"MjIzMTAzWjBjMQswCQYDVQQGEwJBVTETMBEGA1UECBMKUXVlZW5zbGFuZDEaMBgG\n"\
"A1UEChMRQ3J5cHRTb2Z0IFB0eSBMdGQxIzAhBgNVBAMTGlNlcnZlciB0ZXN0IGNl\n"\
"cnQgKDUxMiBiaXQpMFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAJ+zw4Qnlf8SMVIP\n"\
"Fe9GEcStgOY2Ww/dgNdhjeD8ckUJNP5VZkVDTGiXav6ooKXfX3j/7tdkuD8Ey2//\n"\
"Kv7+ue0CAwEAATANBgkqhkiG9w0BAQQFAAOBgQCT0grFQeZaqYb5EYfk20XixZV4\n"\
"GmyAbXMftG1Eo7qGiMhYzRwGNWxEYojf5PZkYZXvSqZ/ZXHXa4g59jK/rJNnaVGM\n"\
"k+xIX8mxQvlV0n5O9PIha5BX5teZnkHKgL8aKKLKW1BK7YTngsfSzzaeame5iKfz\n"\
"itAE+OjGF+PFKbwX8Q==\n"\
"-----END CERTIFICATE-----\n"\
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIBPAIBAAJBAJ+zw4Qnlf8SMVIPFe9GEcStgOY2Ww/dgNdhjeD8ckUJNP5VZkVD\n"\
"TGiXav6ooKXfX3j/7tdkuD8Ey2//Kv7+ue0CAwEAAQJAN6W31vDEP2DjdqhzCDDu\n"\
"OA4NACqoiFqyblo7yc2tM4h4xMbC3Yx5UKMN9ZkCtX0gzrz6DyF47bdKcWBzNWCj\n"\
"gQIhANEoojVt7hq+SQ6MCN6FTAysGgQf56Q3TYoJMoWvdiXVAiEAw3e3rc+VJpOz\n"\
"rHuDo6bgpjUAAXM+v3fcpsfZSNO6V7kCIQCtbVjanpUwvZkMI9by02oUk9taki3b\n"\
"PzPfAfNPYAbCJQIhAJXNQDWyqwn/lGmR11cqY2y9nZ1+5w3yHGatLrcDnQHxAiEA\n"\
"vnlEGo8K85u+KwIOimM48ZG8oTk7iFdkqLJR1utT3aU=\n"\
"-----END RSA PRIVATE KEY-----\n"\
"-----BEGIN X509 CERTIFICATE-----\n"\
"MIICCDCCAXECAQAwDQYJKoZIhvcNAQEEBQAwTjELMAkGA1UEBhMCVVMxHzAdBgNV\n"\
"BAoUFkFUJlQgQmVsbCBMYWJvcmF0b3JpZXMxHjAcBgNVBAsUFVByb3RvdHlwZSBS\n"\
"ZXNlYXJjaCBDQTAeFw05NTA0MTMyMTA2NTZaFw05NzA0MTIyMTA2NTZaME4xCzAJ\n"\
"BgNVBAYTAlVTMR8wHQYDVQQKFBZBVCZUIEJlbGwgTGFib3JhdG9yaWVzMR4wHAYD\n"\
"VQQLFBVQcm90b3R5cGUgUmVzZWFyY2ggQ0EwgZwwDQYJKoZIhvcNAQEBBQADgYoA\n"\
"MIGGAoGAebOmgtSCl+wCYZc86UGYeTLY8cjmW2P0FN8ToT/u2pECCoFdrlycX0OR\n"\
"3wt0ZhpFXLVNeDnHwEE9veNUih7pCL2ZBFqoIoQkB1lZmXRiVtjGonz8BLm/qrFM\n"\
"YHb0lme/Ol+s118mwKVxnn6bSAeI/OXKhLaVdYZWk+aEaxEDkVkCAQ8wDQYJKoZI\n"\
"hvcNAQEEBQADgYEAAZMG14lZmZ8bahkaHaTV9dQf4p2FZiQTFwHP9ZyGsXPC+LT5\n"\
"dG5iTaRmyjNIJdPWohZDl97kAci79aBndvuEvRKOjLHs3WRGBIwERnAcnY9Mz8u/\n"\
"zIHK23PjYVxGGaZd669OJwD0CYyqH22HH9nFUGaoJdsv39ChW0NRdLE9+y8=\n"\
"-----END X509 CERTIFICATE-----\n\0";

static const char *mon[12]=
    { 
    "Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"
    };

void Test_Time()
{
    LCD_Clear();
    lcd_printf("Testing SSL Time functions...\n");
	SYSTEMTIME systime;

	INT64 getlocaltime = Time_GetLocalTime();
	TINYCLR_SSL_PRINTF("Time_GetLocalTime(): %d\n", getlocaltime);
	Time_ToSystemTime(getlocaltime,&systime);
	INT64 fromsystemtime = Time_FromSystemTime(&systime);
	TINYCLR_SSL_PRINTF("Time_FromSystemTime: %d\n", fromsystemtime);

	time_t time = TINYCLR_SSL_TIME(NULL);
	TINYCLR_SSL_PRINTF("TINYCLR_SSL_TIME in ms: %d\n", (INT64)time);
	struct tm *local =	TINYCLR_SSL_LOCALTIME(&time);
	TINYCLR_SSL_PRINTF("Local Time: %s %2d %02d:%02d:%02d %d\n",
		mon[local->tm_mon-1],
		local->tm_mday,
		local->tm_hour,
		local->tm_min,
		local->tm_sec,
		local->tm_year);
	
	struct tm *gmtime =	TINYCLR_SSL_GMTIME(&time);
	TINYCLR_SSL_PRINTF("GM Time: %s %2d %02d:%02d:%02d %d\n",
		mon[gmtime->tm_mon],
		gmtime->tm_mday,
		gmtime->tm_hour,
		gmtime->tm_min,
		gmtime->tm_sec,
		gmtime->tm_year);
}

void Test_OpenSSL_ParseCertificate()
{
    LCD_Clear();
    lcd_printf("Testing SSL_ParseCertificate...\n");
    X509CertData* x509CertData = (X509CertData*)OPENSSL_malloc(sizeof(X509CertData));
    SSL_ParseCertificate( my_cert, TINYCLR_SSL_STRLEN(my_cert), NULL, x509CertData );
    
    OPENSSL_free(x509CertData);
}

void Test_OpenSSL_Crypto()
{
    LCD_Clear();

    int retval = 0;
    // run OpenSSL test suite
    lcd_printf("\nTesting BF...\n");
    retval = ssl_test_bf(0,NULL); //blowfish
    LCD_Clear();
    lcd_printf("\nTesting BN...\n");
    retval = ssl_test_bn(0,NULL); //BN
    LCD_Clear();
    lcd_printf("\nTesting EXP...\n");
    retval = ssl_test_exp(0,NULL); //EXP
    LCD_Clear();
    lcd_printf("\nTesting CAST...\n");
    retval = ssl_test_cast(0,NULL); //CAST
    LCD_Clear();
    lcd_printf("\nTesting DES...\n");
    retval = ssl_test_des(0,NULL); //DES
    LCD_Clear();
    lcd_printf("\nTesting DH...\n");
    retval = ssl_test_dh(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting DSA...\n");
    retval =  ssl_test_dsa(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting EC...\n");
    retval =  ssl_test_ec(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting ECDH...\n");
    retval =  ssl_test_ecdh(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting ECDSA...\n");
    retval =  ssl_test_ecdsa(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting ENGINE...\n");
    retval =  ssl_test_engine(0,NULL);
    LCD_Clear();
    //lcd_printf("\nTesting EVP...\n");
    //retval =  ssl_test_evp(0,NULL); // redo test to use memory instead of file
    LCD_Clear();
    lcd_printf("\nTesting HMAC...\n");
    retval =  ssl_test_hmac(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting IDEA...\n");
    retval =  ssl_test_idea(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting MD2...\n");
    retval =  ssl_test_md2(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting MD4...\n");
    retval =  ssl_test_md4(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting MD5...\n");
    retval =  ssl_test_md5(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting PQUEUE...\n");
    retval =  ssl_test_pqueue(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting RAND...\n");
    retval =  ssl_test_rand(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting RC2...\n");
    retval =  ssl_test_rc2(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting RC4...\n");
    retval =  ssl_test_rc4(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting RC5...\n");
    retval =  ssl_test_rc5(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting RIPEMD...\n");
    retval =  ssl_test_ripemd(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting RSA...\n");
    retval =  ssl_test_rsa(0,NULL);
    //LCD_Clear();
    //lcd_printf("\nTesting SHA...\n");
    //retval =  ssl_test_sha(0,NULL);
    //LCD_Clear();
    //lcd_printf("\nTesting SHA1...\n");
    //retval =  ssl_test_sha1(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting SHA256...\n");
    retval =  ssl_test_sha256(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting SHA512...\n");
    retval =  ssl_test_sha512(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting WHRLPOOL...\n");
    retval =  ssl_test_whrlpool(0,NULL);
    LCD_Clear();
    lcd_printf("\nTesting X509v3...\n");
    retval =  ssl_test_x509v3(0,NULL);

    LCD_Clear();
    lcd_printf("\nTest Suite for OpenSSL Crypto ibraries completed...\n");    
}

void Test_OpenSSL_ClientServerAuth()
{
    LCD_Clear();
    lcd_printf("Testing SSL Client/Server negotiations...\n");
	int err;
	int client_numbytes, server_numbytes;
	int listen_sd;
	int server_sd;
	int client_sd;
	struct TINYCLR_SSL_SOCKADDR_IN sa_serv;
	struct TINYCLR_SSL_SOCKADDR_IN sa_cli;
	size_t client_len;
	SSL_CTX* server_ctx = NULL;
	SSL*     server_ssl = NULL;
	X509*    server_cert = NULL;
	SSL_CTX* client_ctx = NULL;
	SSL*     client_ssl = NULL;
	X509*    client_cert = NULL;
	char*    str = NULL;
	char     client_buf [256];
	char	 server_buf [256];
	SSL_METHOD *server_meth = NULL;
	SSL_METHOD *client_meth = NULL;
	BIO *cert = NULL;
	X509 *x = NULL;
	EVP_PKEY *pkey = NULL;

	// SSL preliminaries. 
	// create client ssl
	client_meth = (SSL_METHOD*)SSLv3_client_method();
	client_ctx = SSL_CTX_new (client_meth);
	if (!client_ctx) goto cleanup;

	server_meth = (SSL_METHOD*)SSLv3_server_method();
	server_ctx = SSL_CTX_new (server_meth);
	if (!server_ctx) goto cleanup;

	
	if ((cert=BIO_new(BIO_s_mem())) == NULL)
	{
		TINYCLR_SSL_PRINTF("Unable to create new BIO");
		goto cleanup;
	}
	BIO_puts(cert,server_pem);
	x=PEM_read_bio_X509_AUX(cert, NULL, 0, NULL);
	
	pkey=PEM_read_bio_PrivateKey(cert,NULL,
		server_ctx->default_passwd_callback,server_ctx->default_passwd_callback_userdata);
	//if (SSL_CTX_use_certificate_file(server_ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
	if (SSL_CTX_use_certificate(server_ctx, x) <= 0) 
	{ 
		TINYCLR_SSL_PRINTF("Use certifcate chain file failed");
		goto cleanup;
	}
	if (SSL_CTX_use_PrivateKey(server_ctx, pkey) <= 0) 
	{
		TINYCLR_SSL_PRINTF("Unable to use Private Key");
		goto cleanup;
	}

	if (!SSL_CTX_check_private_key(server_ctx)) {
		TINYCLR_SSL_PRINTF("Private key does not match the certificate public key\n");
		goto cleanup;
	}

	//if (SSL_CTX_set_cipher_list(server_ctx, );

	// ----------------------------------------------- 
	// Prepare TCP socket for receiving connections

	listen_sd = TINYCLR_SSL_SOCKET (AF_INET, SOCK_STREAM, IPPROTO_TCP);   
	if (listen_sd < 0) goto cleanup;

	// set it to non-blocking
	int nonblock = 1;
	err = TINYCLR_SSL_IOCTL(listen_sd,SOCK_FIONBIO,&nonblock);
	if (err < 0)
	{
		int wsa = TINYCLR_SSL_GETLASTSOCKETERROR();
		TINYCLR_SSL_PRINTF("Nonblocking call failed for server: %d.\n", wsa);
		goto cleanup;
	}

	TINYCLR_SSL_MEMSET(&sa_serv, '\0', sizeof(sa_serv));
	sa_serv.sin_family      		= AF_INET;
	sa_serv.sin_addr.S_un.S_addr 	= inet_addr("127.0.0.1");
	sa_serv.sin_port        		= TINYCLR_SSL_HTONS (1111);          /* Server Port number */

	TINYCLR_SSL_PRINTF("Binding to %d...\n", TINYCLR_SSL_NTOHS(sa_serv.sin_port));
	err = TINYCLR_SSL_BIND(listen_sd, (struct TINYCLR_SSL_SOCKADDR*) &sa_serv,
	     sizeof (sa_serv));                   

	if (err < 0)
	{
		int wsa = TINYCLR_SSL_GETLASTSOCKETERROR();
		TINYCLR_SSL_PRINTF("Bind Socket error %d\n", wsa);
		goto cleanup;
	}

	TINYCLR_SSL_PRINTF("Listening...\n");
	/* Receive a TCP connection. */     
	err = TINYCLR_SSL_LISTEN (listen_sd, 5);                    
	if (err < 0) 
	{
		int wsa = TINYCLR_SSL_GETLASTSOCKETERROR();
		TINYCLR_SSL_PRINTF("Listen Socket error %d\n", wsa);
		goto cleanup;
	}

	// create a client socket
	client_sd = TINYCLR_SSL_SOCKET (AF_INET, SOCK_STREAM, IPPROTO_TCP);   
	if (client_sd < 0) goto cleanup;

	// set it to non-blocking
	err = TINYCLR_SSL_IOCTL(client_sd,SOCK_FIONBIO,&nonblock);
	if (err < 0)
	{
		int wsa = TINYCLR_SSL_GETLASTSOCKETERROR();
		TINYCLR_SSL_PRINTF("Nonblocking call failed for client: %d.\n", wsa);
		goto cleanup;
	}
	// Set up a tcp connection for client: Bind, Connect
	err == TINYCLR_SSL_CONNECT( client_sd, (const struct TINYCLR_SSL_SOCKADDR*)&sa_serv, sizeof(sa_serv));
	if (err < 0)
	{
		int wsa = TINYCLR_SSL_GETLASTSOCKETERROR();
		TINYCLR_SSL_PRINTF("Client connect failed with: %d.\n", wsa);
	}

	
	client_len = sizeof(sa_cli);
	char nodename[128] = "";
	char servname[128] = "";
	SOCK_addrinfo hints;
	SOCK_addrinfo *res = NULL;
	
	TINYCLR_SSL_MEMSET(&hints, '\0', sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

	SOCK_getaddrinfo(nodename,servname,&hints,&res);

	SOCK_addrinfo *ptr = NULL;
	for (ptr=res; ptr!=NULL; ptr=ptr->ai_next)
	{	
		struct sockaddr_in *ip = (struct sockaddr_in*) ptr->ai_addr;
		TINYCLR_SSL_PRINTF("Accepting connections on...%s:%d\n", 
			inet_ntoa(ip->sin_addr), 1111 );
	}

	int counter=0;
	do
	{
		server_sd = TINYCLR_SSL_ACCEPT (listen_sd, (struct TINYCLR_SSL_SOCKADDR*) &sa_cli, (int*)&client_len);
        Events_WaitForEvents(0,2000);
		TINYCLR_SSL_PRINTF("Accept again %d:%d\n", TINYCLR_SSL_GETLASTSOCKETERROR(), counter++);
	} while (server_sd == -1);
	
	TINYCLR_SSL_CLOSESOCKET (listen_sd);

	TINYCLR_SSL_PRINTF ("Connection from %lx, port %x\n",
	sa_cli.sin_addr.S_un.S_addr, sa_cli.sin_port);


	// connections are completed between server & client
	// now lets do the SSL negotiations
	// create server ssl
	server_ssl = SSL_new(server_ctx);
	if (server_ssl == NULL) goto cleanup;
	SSL_set_fd (server_ssl, server_sd);

	//Create server bio and set as non-blocking
	BIO* server_bio = BIO_new(BIO_s_socket());
	if (server_bio == NULL) goto cleanup;
	//CHK_NULL(bio);
	BIO_set_nbio(server_bio,1);
	BIO_set_fd(server_bio, server_sd, BIO_NOCLOSE);
	SSL_set_bio(server_ssl,server_bio,server_bio);

	// create client ssl & connect
	client_ssl = SSL_new(client_ctx);
	if (client_ssl == NULL) goto cleanup;
	SSL_set_fd(client_ssl, client_sd);

	//Create client bio and set as non-blocking
	BIO* client_bio = BIO_new(BIO_s_socket());
	if (client_bio == NULL) goto cleanup;
	BIO_set_nbio(client_bio,1);
	BIO_set_fd(client_bio, client_sd, BIO_NOCLOSE);
	SSL_set_bio(client_ssl,client_bio,client_bio);

	
	
	// loop until server accepts ssl client connect
	int ssl_err =0;
	do
	{
		err = SSL_connect(client_ssl);
		if (err <= 0) 
		{
			ssl_err = SSL_get_error(client_ssl,err);
			TINYCLR_SSL_PRINTF("SSL_Connect error: %d\n", ssl_err);
		}
        Events_WaitForEvents(0,1000);
		err = SSL_accept (server_ssl);
		if (err <= 0) 
		{
			ssl_err = SSL_get_error(server_ssl, err);
			TINYCLR_SSL_PRINTF("SSL_Accept error: %d\n", ssl_err);
		}
        Events_WaitForEvents(0,1000);
	} while (err != 1);

	//Get the cipher - opt
	TINYCLR_SSL_PRINTF("SSL connection using %s\n", SSL_get_cipher (server_ssl));

	//Get client's certificate (note: beware of dynamic allocation) - opt
	client_cert = SSL_get_peer_certificate (server_ssl);
	if (client_cert != NULL) 
	{
		TINYCLR_SSL_PRINTF("Client certificate:\n");

		str = X509_NAME_oneline (X509_get_subject_name (client_cert), 0, 0);
		if (str == NULL) goto cleanup;

		TINYCLR_SSL_PRINTF("subject: %s\n", str);
		OPENSSL_free (str);

		str = X509_NAME_oneline (X509_get_issuer_name  (client_cert), 0, 0);
		if (str == NULL) goto cleanup;
		TINYCLR_SSL_PRINTF("issuer: %s\n", str);
		OPENSSL_free (str);

		//We could do all sorts of certificate verification stuff here before
		//   deallocating the certificate.

		X509_free (client_cert);
	} 
	else
		TINYCLR_SSL_PRINTF("Client does not have certificate.\n");
	
	//Get server's certificate (note: beware of dynamic allocation) - opt
	server_cert = SSL_get_peer_certificate (client_ssl);
	if (server_cert != NULL) 
	{
		TINYCLR_SSL_PRINTF("Server certificate:\n");

		str = X509_NAME_oneline (X509_get_subject_name (server_cert), 0, 0);
		if (str == NULL) goto cleanup;

		TINYCLR_SSL_PRINTF("subject: %s\n", str);
		OPENSSL_free (str);

		str = X509_NAME_oneline (X509_get_issuer_name  (server_cert), 0, 0);
		if (str == NULL) goto cleanup;
		TINYCLR_SSL_PRINTF("issuer: %s\n", str);
		OPENSSL_free (str);

		/* We could do all sorts of certificate verification stuff here before
		   deallocating the certificate. */

		X509_free (server_cert);
	} 
	else
		TINYCLR_SSL_PRINTF("Server with no certificate?!?!?.\n");

	do
	{

		// DATA EXCHANGE - Receive message and send reply. 
		err = SSL_write(client_ssl,"Hello World!",TINYCLR_SSL_STRLEN("Hello World!"));
		if (err <= 0) ssl_err = SSL_get_error(client_ssl, err);
        Events_WaitForEvents(0,1000);
		server_numbytes= SSL_read (server_ssl, server_buf, sizeof(server_buf) - 1);                   
		if (server_numbytes <= 0) 
			ssl_err = SSL_get_error(server_ssl, server_numbytes);
		else
			server_buf[server_numbytes] = '\0';
        Events_WaitForEvents(0,1000);

		err = SSL_write (server_ssl, "I hear you.", TINYCLR_SSL_STRLEN("I hear you."));  
		if (err <= 0) ssl_err = SSL_get_error(server_ssl, err);
        Events_WaitForEvents(0,1000);

		client_numbytes= SSL_read(client_ssl, client_buf, sizeof(client_buf) -1);
		if (client_numbytes <= 0) 
			ssl_err = SSL_get_error(client_ssl, client_numbytes);
		else
			client_buf[client_numbytes] = '\0';
        Events_WaitForEvents(0,1000);
		
	} while (err <= 0);

	TINYCLR_SSL_PRINTF("Server got %d chars:'%s'\n", server_numbytes, server_buf);
	TINYCLR_SSL_PRINTF("Client go %d chars:'%s'\n", client_numbytes, client_buf);
	/* Clean up. */

	cleanup:
	if (pkey) EVP_PKEY_free(pkey);
	if (cert) BIO_free(cert);
	if (x) X509_free(x);
	TINYCLR_SSL_CLOSESOCKET(server_sd);
	if (server_ssl) SSL_shutdown(server_ssl);
	if (server_ssl) SSL_free (server_ssl);
	server_ssl = NULL;
	if (server_ctx) SSL_CTX_free(server_ctx);
	server_ctx = NULL;
	TINYCLR_SSL_CLOSESOCKET(client_sd);
	if (client_ssl) SSL_shutdown(client_ssl);
	if (client_ssl) SSL_free (client_ssl);
	client_ssl = NULL;
	if (client_ctx) SSL_CTX_free(client_ctx);
	client_ctx = NULL;
}

//--//


#ifdef TCPIP_LWIP 

extern LOOPBACK_LWIP_Driver g_LOOPBACK_LWIP_Driver;

#if defined(USE_TCP)
const static char HtmlText[] =
"<html> \
<head><title>A test page</tile></head> \
<body> \
This is a small test page. \
</body> \
</html>";

static char recvBuff[100];

#else // Use UDP
const static char HelloText[] = " Hello UDP!!! ";
#endif
void Test_lwIP()
{
    int server;
    int client;   
    int acceptSock;
    int nonblock;
    int recvd;

    struct sockaddr_in addr, acceptAddr;
    socklen_t len;

    // tcp interface variables 
    struct ip_addr IpAddrLB;
    
    IP4_ADDR( &IpAddrLB, 127,0,0,1); //Loopback  

    addr.sin_family = AF_INET;
    addr.sin_port = 8080;
    addr.sin_addr.s_addr = *(u32_t*)&IpAddrLB;
    addr.sin_len = sizeof(struct sockaddr_in);

    server = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    client = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    nonblock = 1;

    if(server == -1 || client == -1) lcd_printf("Sockets were not created!\n");

    if(-1 == lwip_ioctl(server, FIONBIO, (void*)&nonblock)) lcd_printf("Error ioctl 1\n");
    if(-1 == lwip_ioctl(client, FIONBIO, (void*)&nonblock)) lcd_printf("Error ioctl 2\n");

    len = sizeof(addr);

    if(-1 == lwip_bind( server, (const struct sockaddr*)&addr, len ))lcd_printf("Error bind\n");

    if(-1 == lwip_listen( server, 2 )) lcd_printf("Error listen\n");

   / / Set up a tcp connection for client: Bind, Connect

    if(-1 == lwip_connect( client, (const struct sockaddr*)&addr, len )) lcd_printf("Error connect\n");

    // Provide Accept with the callback when a new connection arrives

    acceptAddr = addr;
    acceptAddr.sin_addr.s_addr = 0;

    Events_WaitForEvents(0, 100);

    do
    {
        acceptSock = lwip_accept( server, (struct sockaddr*)&acceptAddr, &len );
        Events_WaitForEvents(0,2000);
    }
    while(acceptSock == -1);

    while(true)
    {
        LCD_Clear();
        lcd_printf("Sending data...\n");
        if(-1 == lwip_send(client, HtmlText, sizeof(HtmlText), 0)) lcd_printf("SEND FAILED %d\n", errno);
        

        lcd_printf("Try to recv...\n");
        do
        {
            recvd = lwip_recv(acceptSock, (void*)recvBuff, sizeof(recvBuff), 0);

            if(recvd == -1)
            {
                lcd_printf("Receive ERROR: %d\r", errno);
                Events_WaitForEvents(0, 5000);
            }
        }
        while(recvd == -1);

        recvBuff[recvd] = 0;

        lcd_printf( "data (%d): %s\n", recvd, recvBuff );

        Events_WaitForEvents(0, 3000);
    }

}*/
#endif

#endif  // TEST_OPEN_SOURCE_NETWORKSTACK


//--//
//--//
//--//

#if !defined(TEST_OPEN_SOURCE_NETWORKSTACK)

HAL_DECLARE_NULL_HEAP();

void ApplicationEntryPoint()
{
    while(true)
    {
        hal_printf("Hello World!\n");

        Events_WaitForEvents(0, 3000);
    }
}

#else

HAL_DECLARE_CUSTOM_HEAP( SimpleHeap_Allocate, SimpleHeap_Release, SimpleHeap_ReAllocate );

void ApplicationEntryPoint()
{
#ifdef TCPIP_LWIP 
    Test_lwIP();
#endif
    Test_Time();
    Test_OpenSSL_ParseCertificate();    
    Test_OpenSSL_Crypto();    
	Test_OpenSSL_ClientServerAuth();
}

#endif

