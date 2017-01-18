/* rsaref_err.h */
/* ====================================================================
 * Copyright (c) 1998-2001 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#ifndef HEADER_RSAREF_ERR_H
#define HEADER_RSAREF_ERR_H

#ifdef  __cplusplus
extern "C" {
#endif

/* BEGIN ERROR CODES */
/* The following lines are auto generated by the script mkerr.pl. Any changes
 * made after this point may be overwritten when the script is next run.
 */
static void ERR_load_RSAREF_strings(void);
static void ERR_unload_RSAREF_strings(void);
static void ERR_RSAREF_error(int function, int reason, char *file, int line);
#define RSAREFerr(f,r) ERR_RSAREF_error((f),(r),__FILE__,__LINE__)
/* Error codes for the RSAREF functions. */

/* Function codes. */
#define RSAREF_F_BNREF_MOD_EXP				 100
#define RSAREF_F_CIPHER_DES_CBC_CODE			 112
#define RSAREF_F_RSAREF_BN2BIN				 101
#define RSAREF_F_RSAREF_MOD_EXP				 102
#define RSAREF_F_RSAREF_PRIVATE_DECRYPT			 103
#define RSAREF_F_RSAREF_PRIVATE_ENCRYPT			 104
#define RSAREF_F_RSAREF_PUBLIC_DECRYPT			 105
#define RSAREF_F_RSAREF_PUBLIC_ENCRYPT			 106
#define RSAREF_F_RSA_BN2BIN				 107
#define RSAREF_F_RSA_PRIVATE_DECRYPT			 108
#define RSAREF_F_RSA_PRIVATE_ENCRYPT			 109
#define RSAREF_F_RSA_PUBLIC_DECRYPT			 110
#define RSAREF_F_RSA_PUBLIC_ENCRYPT			 111

/* Reason codes. */
#define RSAREF_R_CONTENT_ENCODING			 100
#define RSAREF_R_DATA					 101
#define RSAREF_R_DIGEST_ALGORITHM			 102
#define RSAREF_R_ENCODING				 103
#define RSAREF_R_ENCRYPTION_ALGORITHM			 104
#define RSAREF_R_KEY					 105
#define RSAREF_R_KEY_ENCODING				 106
#define RSAREF_R_LEN					 107
#define RSAREF_R_LENGTH_NOT_BLOCK_ALIGNED		 114
#define RSAREF_R_MODULUS_LEN				 108
#define RSAREF_R_NEED_RANDOM				 109
#define RSAREF_R_PRIVATE_KEY				 110
#define RSAREF_R_PUBLIC_KEY				 111
#define RSAREF_R_SIGNATURE				 112
#define RSAREF_R_SIGNATURE_ENCODING			 113
#define RSAREF_R_UNKNOWN_FAULT				 115

#ifdef  __cplusplus
}
#endif
#endif
