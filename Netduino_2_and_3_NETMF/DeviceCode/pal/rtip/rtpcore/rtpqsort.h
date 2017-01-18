/*
|  RTPQSORT.H - 
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

#ifndef __RTPQSORT_H__
#define __RTPQSORT_H__


#ifdef __cplusplus
extern "C" {
#endif

#ifndef rtp_qsort
typedef int (*RTP_QSORT_COMPARISON_FN)(const void *node1, const void *node2);
void rtp_qsort (void *head, unsigned long num, unsigned long size, RTP_QSORT_COMPARISON_FN compfunc);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __RTPQSORT_H__ */
