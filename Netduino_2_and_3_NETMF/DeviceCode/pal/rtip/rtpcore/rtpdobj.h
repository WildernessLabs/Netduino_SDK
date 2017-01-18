/*
|  RTPDOBJ.H - Runtime Platform Directory Listing functions
| 
|  EBSnet - 
| 
|   $Author: sarah $
|   $Date: 2005/05/14 02:43:29 $
|   $Name:  $
|   $Revision: 1.1 $
| 
|  Copyright EBSnet Inc. , 2004
|  All rights reserved.
|  This code may not be redistributed in source or linkable object form
|  without the consent of its author.
*/ 

#ifndef __RTPDOBJ_H__
#define __RTPDOBJ_H__

#include "rtpdate.h"
#include "rtpfile.h"

#ifdef __cplusplus
extern "C" {
#endif

int  rtp_file_gfirst     (void ** dirobj, char * pattern);
int  rtp_file_gnext      (void * dirobj);
void rtp_file_gdone      (void * dirobj);
int  rtp_file_get_size   (void * dirobj, unsigned long * size);
int  rtp_file_get_attrib (void * dirobj, unsigned char * attributes);
int  rtp_file_get_name   (void * dirobj, char * name, int size);
int  rtp_file_get_time   (void * dirobj, RTP_DATE * adate, RTP_DATE * wdate, RTP_DATE * cdate, RTP_DATE * hdate);

#ifdef __cplusplus
}
#endif

#endif /* __RTPDOBJ_H__ */
