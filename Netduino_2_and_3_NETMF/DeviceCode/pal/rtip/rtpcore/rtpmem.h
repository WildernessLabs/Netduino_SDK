 /*
 | RTPMEM.H - Runtime Platform Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform 
 |
 |  $Author: sarah $
 |  $Date: 2005/05/14 17:35:21 $
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

#ifndef __RTPMEM_H__
#define __RTPMEM_H__

#include "rtp.h"

#ifdef RTP_MEM_RTP_DEBUG_LOG_SIZE
#ifndef RTP_TRACK_LOCAL_MEMORY
#define RTP_TRACK_LOCAL_MEMORY
#endif
#endif

/************************************************************************
 * If RTP_TRACK_LOCAL_MEMORY is turned on, the debug implementaion of   *
 * the memory system will be implemented.  This provides debug          *
 * information otherwise not available by most platforms.               *
 ************************************************************************/
 
/* -------------------------------------------- */
/* Enable this flag to include memory tracking. */
/* -------------------------------------------- */
/* #define RTP_TRACK_LOCAL_MEMORY */


/************************************************************************
 * Memory System API
 ************************************************************************/

/************************************************************************
 * Memory System API which should be
 * used via the macros indicated below.
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
void * rtp_malloc  (unsigned long size);
void * rtp_calloc  (unsigned long num, unsigned long size);
void * rtp_realloc (void * ptr, unsigned long size);
void   rtp_free    (void * ptr);
 
#ifdef __cplusplus
}
#endif


#ifdef RTP_TRACK_LOCAL_MEMORY
#include "rtpmemdb.h"
#else


#define rtp_debug_print_mem_usage(logFile,mode)

#endif /* !RTP_TRACK_LOCAL_MEMORY */


#endif /* __RTPMEM_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
