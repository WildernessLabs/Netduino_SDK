//
// APIRTSSL.H - RTSSL API
//
// EBSNet Inc. - RTSSL
//
// Copyright EBSnet Inc, 1998
// All rights reserved.
// This code may not be redistributed in source or linkable object form
// without the consent of its author.                            __st__
//


#ifndef __APIRTSSL__
#define __APIRTSSL__ 1


// ***************************************************************************
// ***************************************************************************
#define RTSSL_SECURE_CLIENT     0x01
#define RTSSL_SECURE_SERVER     0x02
#define ENABLE_RTSSLv2          0x04
#define ENABLE_RTSSLv3          0x08
#define ENABLE_RTTLSv1          0x10

// ***************************************************************************
// ***************************************************************************
#define RTSSL_VERIFY_NONE                   0x01
#define RTSSL_VERIFY_PEER                   0x02
#define RTSSL_VERIFY_FAIL_IF_NO_PEER_CERT   0x04
#define RTSSL_VERIFY_CLIENT_ONCE            0x08

typedef unsigned char * X509Certificate;

typedef struct
{
    X509Certificate cert;
    int size;
} RTCertificate;


typedef struct
{
    char     Issuer[256];
    char     Subject[256];
    RTP_DATE EffectiveDate;
    RTP_DATE ExpirationDate;   
} RTCertData;


// ***************************************************************************
// ***************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

void *          secure_new_context          (int sslMode);
int             secure_associate_ca_store   (void * sslctx, const RTCertificate* cert, int cert_count);
int             secure_server_init          (void * sslctxint, int sslverify, const RTCertificate* cert);
int             secure_client_init          (void * sslctx, int sslverify, const RTCertificate* cert);
void            secure_exit                 (   );

// ***************************************************************************
// ***************************************************************************

int             secure_accept               (RTP_HANDLE sock, void *sslctx, void **s);
int             secure_connect              (RTP_HANDLE sock, const char* szTargetHost, void *sslctx, void **s);
int             secure_reconnect            (RTP_HANDLE sock, void *sslctx, void **s);

// ***************************************************************************
// ***************************************************************************

int             secure_read         (void *ssl,char *buffer,unsigned int buflen, unsigned int blocking);
int             secure_read_pending (void *ssl);
int             secure_write        (void *ssl,char *buffer,unsigned int buflen, unsigned int blocking);
int             secure_closesocket  (RTP_HANDLE sd, void *s);
int             secure_sslexit      (void *sctx);

// ***************************************************************************
// ***************************************************************************

int             secure_sslshutdown  (void *s);
void            secure_sslfree      (void *s);
void            secure_sslctxfree   (void *sslctx);

// ***************************************************************************
// ***************************************************************************
int             secure_parse_certificate(const RTCertificate* cert, RTCertData* certData);
int	        secure_get_fd           (void *s);

#ifdef __cplusplus
}
#endif
// ***************************************************************************
// ***************************************************************************

#define		RTSSL_X509_V_OK					                            0
#define		RTSSL_X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT		            2
#define		RTSSL_X509_V_ERR_UNABLE_TO_GET_CRL			                3
#define		RTSSL_X509_V_ERR_UNABLE_TO_DECRYPT_CERT_SIGNATURE	        4
#define		RTSSL_X509_V_ERR_UNABLE_TO_DECRYPT_CRL_SIGNATURE	        5
#define		RTSSL_X509_V_ERR_UNABLE_TO_DECODE_ISSUER_PUBLIC_KEY	        6
#define		RTSSL_X509_V_ERR_CERT_SIGNATURE_FAILURE		                7
#define		RTSSL_X509_V_ERR_CRL_SIGNATURE_FAILURE		                8
#define		RTSSL_X509_V_ERR_CERT_NOT_YET_VALID			                9
#define		RTSSL_X509_V_ERR_CERT_HAS_EXPIRED			                10
#define		RTSSL_X509_V_ERR_CRL_NOT_YET_VALID			                11
#define		RTSSL_X509_V_ERR_CRL_HAS_EXPIRED			                12
#define		RTSSL_X509_V_ERR_ERROR_IN_CERT_NOT_BEFORE_FIELD	            13
#define		RTSSL_X509_V_ERR_ERROR_IN_CERT_NOT_AFTER_FIELD	            14
#define		RTSSL_X509_V_ERR_ERROR_IN_CRL_LAST_UPDATE_FIELD	            15
#define		RTSSL_X509_V_ERR_ERROR_IN_CRL_NEXT_UPDATE_FIELD	            16
#define		RTSSL_X509_V_ERR_OUT_OF_MEM				                    17
#define		RTSSL_X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT		        18
#define		RTSSL_X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN		            19
#define		RTSSL_X509_V_ERR_UNABLE_TO_GET_ISSUER_CERT_LOCALLY	        20
#define		RTSSL_X509_V_ERR_UNABLE_TO_VERIFY_LEAF_SIGNATURE	        21
#define		RTSSL_X509_V_ERR_CERT_CHAIN_TOO_LONG			            22
#define		RTSSL_X509_V_ERR_CERT_REVOKED				                23
#define		RTSSL_X509_V_ERR_INVALID_CA				                    24
#define		RTSSL_X509_V_ERR_PATH_LENGTH_EXCEEDED			            25
#define		RTSSL_X509_V_ERR_INVALID_PURPOSE			                26
#define		RTSSL_X509_V_ERR_CERT_UNTRUSTED			                    27
#define		RTSSL_X509_V_ERR_CERT_REJECTED			                    28
#define		RTSSL_X509_V_ERR_SUBJECT_ISSUER_MISMATCH		            29
#define		RTSSL_X509_V_ERR_AKID_SKID_MISMATCH			                30
#define		RTSSL_X509_V_ERR_AKID_ISSUER_SERIAL_MISMATCH		        31
#define		RTSSL_X509_V_ERR_KEYUSAGE_NO_CERTSIGN			            32
#define		RTSSL_X509_V_ERR_APPLICATION_VERIFICATION		            50

#define     RTSSL_NO_PEER_CERTIFICATE                                   100
#define     RTSSL_NO_VALID_PEER_NAME                                    101

#define     RTSSL_CTX_NOT_VALID                                         -1
#define     RTSSL_HANDSHAKE_FAILURE                                     -2
#define     RTSSL_WANT_READ                                             -3
#define     RTSSL_WANT_WRITE                                            -4
#define     RTSSL_WRONG_HOST_NAME                                       -5
#define     RTSSL_ERROR_NONE                                            0
#define     RTSSL_CONNECT_FINISHED                                      0
#define     RTSSL_ACCEPT_FINISHED                                       0
#endif // __APIRTSSL__
