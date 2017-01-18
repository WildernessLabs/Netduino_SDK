/* PORTSSLCFG.H  - RTSSL Configuration File

   EBSNet Inc. - SSL

   This code may not be redistributed in source or linkable object form
   without the consent of its author.

    Module description:
        This module contains constants that may be change to modify
        the build characteristics of SSL.                            __st__
*/

#ifndef __PORTSSLCFG__
#define __PORTSSLCFG__  1


/* --------------------------------------- */
/*     EBSNet Inc. RTSSL Windows Port      */
/* --------------------------------------- */


/* --------------------------------------- */
/*                 CIPHERS                 */
/* --------------------------------------- */

/* SUPPORTED */
#define INCLUDE_DES                 1
#define INCLUDE_RSA                 1
#define INCLUDE_RC2                 1
#define INCLUDE_RC4                 1
#define INCLUDE_RC5                 1
#define INCLUDE_IDEA                1
#define INCLUDE_CAST                1
#define INCLUDE_BF                  1
#define INCLUDE_DH                  1
#define INCLUDE_FORZETTA            1

/* NOT SUPPORTED */
#define INCLUDE_ONLY_DES_CBC        0
/* #define NO_DESCBCM       1 */

/* --------------------------------------- */
/*                 DIGESTS                 */
/* --------------------------------------- */
/* SUPPORTED */
#define INCLUDE_SHA                 1
#define INCLUDE_SHA0                1
#define INCLUDE_SHA1                1
#define INCLUDE_MD5                 1
#define INCLUDE_MD2                 1
#define INCLUDE_MD4                 1
#define INCLUDE_MDC2                (1 && INCLUDE_DES)
#define INCLUDE_DSA                 1
#define INCLUDE_RIPEMD              1

/* NOT SUPPORTED */

/* --------------------------------------- */
/*                   MAC                   */
/* --------------------------------------- */
/* SUPPORTED */
#define INCLUDE_HMAC                1

/* --------------------------------------- */
/*                  HASH                   */
/* --------------------------------------- */
/* SUPPORTED */
#define INCLUDE_LHASH               1

/* --------------------------------------- */
/*               SSL VERSION               */
/* --------------------------------------- */
/* SUPPORTED */
#define INCLUDE_TLS1                1
#define INCLUDE_SSL3                1
#define INCLUDE_SSL2                0

/* NOT SUPPORTED */

/* --------------------------------------- */
/*                 RTSSL API               */
/* --------------------------------------- */
#define INCLUDE_SSL_SRV             1
#define INCLUDE_SSL_CLI             1
#define INCLUDE_SSL_CONNECT         1
#define INCLUDE_SSL_ACCEPT          1
#define INCLUDE_SRV_SIDE_AUTH       1
#define INCLUDE_CLI_SIDE_AUTH       1
#define INCLUDE_SESSION_CACHING     0
#define INCLUDE_CERTS_BY_POINTERS   1
#define INCLUDE_ASYNC_STATES        1

/* --------------------------------------- */
/*            SSL TEST UTILITIES           */
/* --------------------------------------- */
#define INCLUDE_MONOLITH            0
#define INCLUDE_TEST_CRYPT          0

/* --------------------------------------- */
/*              MISCELLANEOUS              */
/* --------------------------------------- */
/* SUPPORTED */
#define INCLUDE_SOCK                    1
#define INCLUDE_FILE                    1
#define INCLUDE_BIO                     1
#define INCLUDE_STDIO                   1
#define INCLUDE_FP_API                  1   /* 0: you don't want 'FILE *' emulation/handling.                */
#define INCLUDE_OBJECT                  1   /* 0: all objects (OIDs) must be loaded by hand (see OBJ_DAT.C). */
#define INCLUDE_REDUCED_FILEPATH        1   /* 0: the full path is displayed in error messages.              */
#define INCLUDE_SSL_TCP_NAGLE           1   /* 0: to turn the Nagle algorithm off. Should be done at         */
                                            /*    application, but this is a quick fix to facilitate         */
                                            /*    Telnet-oriented behaviour.                                 */
/* NOT SUPPORTED */
#define INCLUDE_DES_ENC_READ            0
#define INCLUDE_DES_ENC_WRITE           0

#define INCLUDE_SSL_ERROR_STRINGS       0
#define INCLUDE_ERR                     0   /* 0: don't display error descriptions and file loc's            */
#define INCLUDE_PWD_ENTRY               0   /* 0: password for private keys is not supported                 */
#define INCLUDE_PWD_STATIC              0   /* 0: no static password CFG_KEY_PASSWORD                        */
#define INCLUDE_EXPORT_ONLY_CIPHERS     0   /* 0: if you don't wish to support export-only ciphers.          */
#define INCLUDE_CIPHER_DEBUG            1   /* 0: don't show which ciphers are available & compared by       */
                                            /*    the client and/or server.                                  */
#define INCLUDE_BN_LLONG                0   /* 0: we don't have 'long long'.                                 */
#define INCLUDE_PRINTF_FLOAT_SUPPORT    0   /* 0: don't include floating point printf routines.              */
#define INCLUDE_DISPLAY_WARNING         0   /* 0: if you don't want to display a warning when trying to      */
                                            /*    compile a file that has not been included above.           */
#define INCLUDE_ONLY31CHAR_SYMBOLS      0   /* 0: if platform supports more than 31 character symbols.       */
#define INCLUDE_CASE_INSENSITIVITY      0   /* 0: if linker is case sensitive.                               */

#if (!INCLUDE_STDIO)
#undef  INCLUDE_FP_API
#define INCLUDE_FP_API                  0
#endif

/* --------------------------------------- */
/*      RTSSL CONFIGURATION VARIABLES      */
/* --------------------------------------- */

/* ------------------------------------------------------------------------- */
/*  The maximum length for SSL records in bytes.  The default value uses a   */
/*  large amount of memory.  Suggest that this value is reduced to 1460      */
/*  bytes after the default is tested and confirmed.                         */
/*                                                                           */
/*  DEFAULT: 16384                                                           */
/* ------------------------------------------------------------------------- */
#define CFG_MAX_RECORD_LENGTH           16384

/* ------------------------------------------------------------------------- */
/*  The password used when the private certificate was created.  This is     */
/*  only needed iff INCLUDE_PWD_ENTRY and INCLUDE_PWD_STATIC are both set to */
/*  1 above.  If a static password is not used and INCLUDE_PWD_ENTRY is set  */
/*  to 1, communication with the user is assumed by way of the console.  If  */
/*  no console is available the code in source/read_pwd.c should be changed. */
/*                                                                           */
/*  DEFAULT: "StaticPassword"                                                */
/* ------------------------------------------------------------------------- */
#define CFG_KEY_PASSWORD                "StaticPassword"

/* ------------------------------------------------------------------------- */
/*  These control the desired cipher lists that are to be used during any    */
/*  given SSL session.  CFG_SRV_CIPHER_LIST and CFG_CLI_CIPHER_LIST are used */
/*  to create independent cipher lists for a secure server and a secure      */
/*  client respectively.  Please be aware of the fact that by default        */
/*  support for Anonymous Diffie-Hellman is not preset (ADH is used by many  */
/*  SSL servers that do not have a certificate).  In other words, if you     */
/*  ever want to connect to certificateless SSL servers, both server and     */
/*  client need to have their ADH cipher suites loaded or a connect failure  */
/*  will result with the 'no common cipher' failure notice.                  */
/*  To set up a cipher list the ciphers specified may be any one of the      */
/*  available alieases indicated in the table.  If any of the alaises are    */
/*  miss-typed, or are not supported due to build configurations there will  */
/*  be no error unless no cipher is found to populate the SSL context and    */
/*  the SSL objects that make use of it.                                     */
/*  ________________________________________________________________________ */
/*  CODE_________DESCRIPTION________________________________________________ */
/*  LOW          56 and 64 bit ciphers not including export ciphers.         */
/*  MEDIUM       128 bit ciphers.                                            */
/*  HIGH         >128 bit ciphers.                                           */
/*  EXP          40 and 56 bit export ciphers.                               */
/*  EXPORT       40 and 56 bit export ciphers.                               */
/*  EXPORT40     40 bit export ciphers.                                      */
/*  EXPORT56     56 bit export ciphers.                                      */
/*  aNULL        Ciphers with no authentication.                             */
/*  eNULL        Ciphers with no encryption.                                 */
/*  NULL         Ciphers with no encryption.                                 */
/*  kRSA         Ciphers that make use of the RSA key exchange.              */
/*  kEDH         Ciphers that make use of the ephemeral DH key exchange.     */
/*  aRSA         Ciphers that make use of RSA keys for their certificates.   */
/*  aDSS         Ciphers that make use of DSS keys for their certificates.   */
/*  DSS          Ciphers that make use of DSS keys for their certificates.   */
/*  DH           Ciphers that make use of DH.                                */
/*  ADH          Ciphers that make use of only anonymous DH.                 */
/*  RSA          Ciphers that make use of the RSA key exchange.              */
/*  DES          Ciphers that make use of DES.                               */
/*  3DES         Ciphers that make use of Triple DES.                        */
/*  RC4          Ciphers that make use of RC4.                               */
/*  RC2          Ciphers that make use of RC2.                               */
/*  IDEA         Ciphers that make use of IDEA.                              */
/*  MD5          Ciphers that make use of MD5.                               */
/*  SHA1         Ciphers that make use of SHA1.                              */
/*  SHA          Ciphers that make use of SHA1.                              */
/*  SSLv2        Ciphers that make use of SSLv2.                             */
/*  SSLv3        Ciphers that make use of SSLv3.                             */
/*  TLSv1        Ciphers that make use of TLSv1.                             */
/*  ALL          All the ciphers except for those aliased by eNULL.          */
/*  DEFAULT      Sets the cipher list to SSL_DEFAULT_CIPHER_LIST defined in  */
/*               include/openssl/ssl.h to "ALL:!ADH:RC4+RSA:+SSLv2:@STRENGTH"*/
/*               SSL_DEFAULT_CIPHER_LIST should be left unchanged, as it is  */
/*               used to set an initial cipher list whith in every new       */
/*               SSL context.  Set the following to DEFAULT if you want to   */
/*               leave the cipher list unchanged.                            */
/*                                                                           */
/*  (The DEFAULT explained. Select ALL ciphers, then remove all ADH ciphers. */
/*   Move the RC4 AND RSA ciphers to the lower end of the selection priority */
/*   list --reorder--.  Mak sure all SSLv2 ciphers are added too.  Sort the  */
/*   whole active cipher list based on STRENGTH --key length--.)             */
/*  ________________________________________________________________________ */
/*                                                                           */
/*                                                                           */
/*  NOTE: There is a delimiter and addition that may alter the               */
/*        functioanlity of the above aliases:                                */
/*        __________________________________________________________________ */
/*        :      Delimiter.                                                  */
/*        +      Used to logically AND two algorithms together.              */
/*               example: RC4+RSA  (any cipher containing both RC4 AND RSA)  */
/*        __________________________________________________________________ */
/*                                                                           */
/*                                                                           */
/*  NOTE: There are a few prefixes that are used to alter the                */
/*        functionality of the above aliases:                                */
/*        __________________________________________________________________ */
/*        PREFIX_DESCRIPTION________________________________________________ */
/*        -      Removed from the active list.                               */
/*        +      Moves existing ciphers already added to the list to the     */
/*               end of the list.                                            */
/*        !      Removed from the active list and cannot be added by a later */
/*               alias within this string.                                   */
/*        __________________________________________________________________ */
/*                                                                           */
/*                                                                           */
/*  NOTE: There is a command that can be added to order the list:            */
/*        __________________________________________________________________ */
/*        @STRENGTH     Sorts the active cipher list in order of cipher      */
/*                      key length (strength).                               */
/*        __________________________________________________________________ */
/*                                                                           */
/*                                                                           */
/*  DEFAULT: "ALL:!LOW:!EXP:!ADH:@STRENGTH"                                  */
/*  DEFAULT: "ALL:!LOW:!EXP:@STRENGTH"                                       */
/* ------------------------------------------------------------------------- */
#define CFG_SRV_CIPHER_LIST             "All:!LOW:MEDIUM:HIGH:!DES:!DSS:!RC5:!MD5:@STRENGTH"
#define CFG_CLI_CIPHER_LIST             "All:!LOW:MEDIUM:HIGH:!DES:!DSS:!RC5:!MD5:@STRENGTH"//:!LOW:!EXP:@STRENGTH" //ALL:!3DES:!DES:!SHA:@STRENGTH"

/* ------------------------------------------------------------------------- */
/*  The server CA file and client CA file are used to authenticate those     */
/*  peers which send a certificate to the corresponding application.  These  */
/*  configurations are only relevant when CFG_SRV_VERIFY and CFG_CLI_VERIFY  */
/*  are set to something other than SSL_VERIFY_NONE.  This means that the    */
/*  client or server need to know who the peer is requiring their            */
/*  certificate.  The value of this flag should be a string representing the */
/*  name of the file (that is trusted) that can be accessed using a file     */
/*  system.  If no file system is present the RTSSL Memory Files System      */
/*  must be used.  The current client default configuration is dependent on  */
/*  the RTSSL MFS and the INCLUDE_VSIGN_TEST being turned on.                */
/*                                                                           */
/*  DEFAULT: NULL                                                            */
/*  DEFAULT: NULL                                                            */
/* ------------------------------------------------------------------------- */
#define CFG_SRV_CA_FILE                 NULL
#define CFG_CLI_CA_FILE                 NULL 

/* ------------------------------------------------------------------------- */
/*  This represents a path to a directory containing a set of hashed trusted */
/*  CAs.  When a certificate is being checked for authentication from a peer */
/*  it is first checked against the already loaded CFG_XXX_CA_FILE and then  */
/*  against this directory.  Each file in this directory is not loaded until */
/*  that time.  The RTSSL MFS does not support directories and therefor the  */
/*  following must be set to NULL unless another file system is present and  */
/*  has been added to the RTSSL porting files.                               */
/*                                                                           */
/*  DEFAULT: NULL                                                            */
/*  DEFAULT: NULL                                                            */
/* ------------------------------------------------------------------------- */
#define CFG_SRV_CA_PATH                 NULL
#define CFG_CLI_CA_PATH                 NULL

/* ------------------------------------------------------------------------- */
/*  These are used when INCLUDE_SRV_SIDE_AUTH and INCLUDE_CLI_SIDE_AUTH are  */
/*  set to one respectively.  This indicates that the server or client       */
/*  intend to authenticate themselves to their peer.  The configuration      */
/*  value should represent the file name containing the        				 */
/*  server/client certificate that will be used for self authentication.     */
/*                                                                           */
/*  DEFAULT: NULL                                                            */
/*  DEFAULT: NULL                                                            */
/* ------------------------------------------------------------------------- */
#define CFG_SRV_CERT_FILE               NULL
#define CFG_CLI_CERT_FILE               NULL

/* ------------------------------------------------------------------------- */
/*  If this is NULL, the CFG_SRV_CERT_FILE or the CFG_CLI_CERT_FILE are      */
/*  assumed to containg the private key.  Otherwise these should be the file */
/*  name which contains the private key for the certificate.  This private   */
/*  key is compaired with the public key of the certificate to assure its    */
/*  validity.                                                                */
/*                                                                           */
/*  DEFAULT: NULL                                                            */
/*  DEFAULT: NULL                                                            */
/* ------------------------------------------------------------------------- */
#define CFG_SRV_PRIV_KEY                NULL
#define CFG_CLI_PRIV_KEY                NULL

/* ------------------------------------------------------------------------- */
/*  These are used to set the period of time that the secure_accept() and    */
/*  the secure_connect() will wait until the socket is ready respectively.   */
/*  The configured values are messured in seconds.                           */
/*                                                                           */
/*  NOTE: Depending on the implementation of select at the porting layer,    */
/*        generally if secure_accept() or secure_connect() are called in     */
/*        blocking mode, these timeout values will be ignored and the        */
/*        select will return immediately with a successful value.            */
/*                                                                           */
/*  DEFAULT: 10                                                              */
/*  DEFAULT: 10                                                              */
/* ------------------------------------------------------------------------- */
#define CFG_ACCEPT_SELECT_TMO           10
#define CFG_CONNECT_SELECT_TMO          10

/* ------------------------------------------------------------------------- */
/*  If client side caching is turned on, secure_reconnect can be used to     */
/*  to connect to sessions that are already saved.  In such cases, rtssl     */
/*  keeps track of multiple sessions using a link list.  Use the following   */
/*  macro to set the maximum number of cacheable sessions.  This value has   */
/*  to be equal or greater than 1                                            */
/* ------------------------------------------------------------------------- */
#if (INCLUDE_SESSION_CACHING)
#define CFG_MAX_CACHED_SESSIONS         10
#endif

/* ------------------------------------------------------------------------- */
/*                           ENVIRONMENT SPECIFIC                            */
/* ------------------------------------------------------------------------- */

/* --------------------------------------- */
/*  16 if running on a TCP/IP stack that   */
/*  supports IPv6.                         */
/*  4 if running on a TCP/IP stack that    */
/*  supports only IPv4.                    */  
/* --------------------------------------- */
#define RTSSL_IP_ALEN                      4

/* --------------------------------------- */
/* Add the appropriate flag to the project */
/* #define B_ENDIAN                        */
/* #define L_ENDIAN                        */
/* --------------------------------------- */
#define L_ENDIAN

/* --------------------------------------- */
/*    DON'T CHANGE THE FOLLOWING FLAGS.    */
/* --------------------------------------- */

/* --------------------------------------- */
/*  A non-fatal error includes a function  */
/*  call has returned a value that is not  */
/*  necessarily an error.  For instance,   */
/*  a socket that is set for non-blocking  */
/*  mode and an rtp_net_accept() call      */
/*  returns immediately with an error      */
/*  indicating that it would block or that */
/*  the operation is in progress.          */
/* --------------------------------------- */
#define RTSSL_NONFATAL_ERROR               (-2)

#define MS_STATIC
#define MS_FAR
#define THREADS

#define EBS_FILE_STRUCTURE                 1

/* --------------------------------------- */
/*      CONFIGURE MEMORY FILE SYSTEM       */
/* --------------------------------------- */
#include <openssl/rtsslmfscfg.h>

/* --------------------------------------- */
/*         SET UP MAPPING FOR PORT         */
/* --------------------------------------- */
#include <openssl/mapssl.h>

/* --------------------------------------- */
/*               END OF FILE               */
/* --------------------------------------- */
#endif /* __PORTSSLCFG__ */
