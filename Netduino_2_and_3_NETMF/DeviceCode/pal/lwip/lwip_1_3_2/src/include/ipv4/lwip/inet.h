/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __LWIP_INET_H__
#define __LWIP_INET_H__

#include "lwip\opt.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch\bpstruct.h"
#endif


//[MSCHANGE] - in_addr needs to be packed because sockaddr_in is packed 
// This is required for to assure proper alignment when casting from sockaddr 
// to sockaddr_in 
/* For compatibility with BSD code */
PACK_STRUCT_BEGIN
struct in_addr {
  PACK_STRUCT_FIELD(u32_t s_addr);
} PACK_STRUCT_STRUCT;
PACK_STRUCT_END

#ifdef PACK_STRUCT_USE_INCLUDES
#  include "arch\epstruct.h"
#endif


#define INADDR_NONE         ((u32_t)0xffffffffUL)  /* 255.255.255.255 */
#define INADDR_LOOPBACK     ((u32_t)0x7f000001UL)  /* 127.0.0.1 */
#define INADDR_ANY          ((u32_t)0x00000000UL)  /* 0.0.0.0 */
#define INADDR_BROADCAST    ((u32_t)0xffffffffUL)  /* 255.255.255.255 */

u32_t inet_addr(const char *cp);
int inet_aton(const char *cp, struct in_addr *addr);
char *inet_ntoa(struct in_addr addr); /* returns ptr to static buffer; not reentrant! */

#ifdef htons
#undef htons
#endif /* htons */
#ifdef htonl
#undef htonl
#endif /* htonl */
#ifdef ntohs
#undef ntohs
#endif /* ntohs */
#ifdef ntohl
#undef ntohl
#endif /* ntohl */

#ifndef LWIP_PLATFORM_BYTESWAP
#define LWIP_PLATFORM_BYTESWAP 0
#endif

// [MS_CHANGE] - changed LITTLE_ENDIAN to LWIP_LITTLE_ENDIAN
#if BYTE_ORDER == LWIP_BIG_ENDIAN
#define htons(x) (x)
#define ntohs(x) (x)
#define htonl(x) (x)
#define ntohl(x) (x)
// [MS_CHANGE] - changed LITTLE_ENDIAN to LWIP_LITTLE_ENDIAN
#else /* BYTE_ORDER != LWIP_BIG_ENDIAN */
#ifdef LWIP_PREFIX_BYTEORDER_FUNCS
/* workaround for naming collisions on some platforms */
#define htons lwip_htons
#define ntohs lwip_ntohs
#define htonl lwip_htonl
#define ntohl lwip_ntohl
#endif /* LWIP_PREFIX_BYTEORDER_FUNCS */
#if LWIP_PLATFORM_BYTESWAP
#define htons(x) LWIP_PLATFORM_HTONS(x)
#define ntohs(x) LWIP_PLATFORM_HTONS(x)
#define htonl(x) LWIP_PLATFORM_HTONL(x)
#define ntohl(x) LWIP_PLATFORM_HTONL(x)
#else /* LWIP_PLATFORM_BYTESWAP */
u16_t htons(u16_t x);
u16_t ntohs(u16_t x);
u32_t htonl(u32_t x);
u32_t ntohl(u32_t x);
#endif /* LWIP_PLATFORM_BYTESWAP */

// [MS_CHANGE] - changed LITTLE_ENDIAN to LWIP_LITTLE_ENDIAN
#endif /* BYTE_ORDER == LWIP_BIG_ENDIAN */

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_INET_H__ */
