/*
|  RTPROT.H - 
| 
|  EBSnet - 
| 
|   $Author: shane $
|   $Date: 2004/11/09 22:03:27 $
|   $Name:  $
|   $Revision: 1.2 $
| 
|  Copyright EBSnet Inc. , 2004
|  All rights reserved.
|  This code may not be redistributed in source or linkable object form
|  without the consent of its author.
*/ 

#ifndef __RTPROT_H__
#define __RTPROT_H__


#include "rtptypes.h"

#ifdef __cplusplus
extern "C" {
#endif

unsigned long   rtp_lrotl    (unsigned long, int);
unsigned long   rtp_lrotr    (unsigned long, int);

#ifdef __cplusplus
}
#endif

#endif /* __RTPROT_H__ */
