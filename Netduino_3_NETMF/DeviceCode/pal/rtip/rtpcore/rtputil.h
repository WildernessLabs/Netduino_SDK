 /*
 | RTPUTIL.H - Runtime Platform Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform 
 |
 |  $Author: sarah $
 |  $Date: 2005/06/14 01:33:31 $
 |  $Name:  $
 |  $Revision: 1.2 $
 |
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

#ifndef __RTPUTIL_H__
#define __RTPUTIL_H__

#include "rtp.h"

/************************************************************************
 * Defines
 ************************************************************************/
#ifdef RTP_LONGWORD_ALLIGNED
/* Move and compare 4 bytes */
#define tc_mv4(A,B,C)  rtp_memcpy((A), (B), 4)
#define tc_cmp4(A,B,C) (!rtp_memcmp((A), (B), 4))

#else
#ifndef tc_mv4	/* _yi_ */
#define tc_mv4(A,B,C)        (*((RTP_PFUINT32)(A))  =  *((RTP_PFUINT32)(B)))
#endif
#ifndef tc_cmp4 /* _yi_ */
#define tc_cmp4(A,B,C)       (*((RTP_PFUINT32)(A)) ==  *((RTP_PFUINT32)(B)))
#endif
#endif

 
/************************************************************************
 * Typedefs
 ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
 * Utility API                                                          *
 ************************************************************************/
short           rtp_net_htons     (short i);
short           rtp_net_ntohs     (short i);
long            rtp_net_htonl     (long i);
long            rtp_net_ntohl     (long i);
                                  
unsigned long   rtp_net_wtol      (unsigned short* w);
void            rtp_net_ltow      (unsigned short* w, unsigned long i);

RTP_UINT32  byte_to_long(RTP_PFUINT32 pd);
void        long_to_bytes(RTP_PFUINT32 pd, RTP_UINT32 d);

#ifdef __cplusplus
}
#endif

#endif /* __RTPUTIL_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
