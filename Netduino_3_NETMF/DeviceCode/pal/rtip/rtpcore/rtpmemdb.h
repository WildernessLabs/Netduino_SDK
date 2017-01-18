 /*
 | RTPMEMDB.H - Runtime Platform Services
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

#ifndef __RTPMEMDB_H__
#define __RTPMEMDB_H__

#include "rtp.h"

#ifdef RTP_MEM_RTP_DEBUG_LOG_SIZE
#ifndef RTP_TRACK_LOCAL_MEMORY
#define RTP_TRACK_LOCAL_MEMORY
#endif
#endif

#ifdef RTP_TRACK_LOCAL_MEMORY
/************************************************************************
 * If RTP_TRACK_LOCAL_MEMORY is turned on, the debug implementaion of   *
 * the memory system will be implemented.  This provides debug          *
 * information otherwise not available by most platforms.  Review the   *
 * explanations for the following defines and set accordingly.          *
 ************************************************************************/

/* -------------------------------------------- */
/*  Prepad the memory allocated, with a         */
/*  RTP_MEM_PREPAD_BYTE (defined in rtpmemdb.c) */
/*  RTP_MEM_BUFFER_PREPAD times.    (default 0) */
/* -------------------------------------------- */
#define RTP_MEM_BUFFER_PREPAD        0

/* -------------------------------------------- */
/*  Pad the memory allocated, with a            */
/*  RTP_MEM_GUARD_BYTE (defined in rtpmemdb.c)  */
/*  RTP_MEM_BUFFER_GAURD times.     (default 0) */
/* -------------------------------------------- */
#define RTP_MEM_BUFFER_GUARD         0

/* -------------------------------------------- */
/*  Add an identifier string of                 */
/*  RTP_MEM_BLOCK_ID_BYTE of length             */
/*  RTP_MEM_BUFFER_ID_SIZE to every             */
/*  RTPMemBlockHeader so to be sure that the    */
/*  header was added for every memory alloc.    */
/*                                  (default 0) */
/* -------------------------------------------- */
#define RTP_MEM_BUFFER_ID_SIZE       0

/* -------------------------------------------- */
/*  Add a prepad to the RTPMemBlockHeader       */
/*  using RTP_MEM_BLOCK_PREPAD_BYTE(s) (defined */
/*  in rtpmemdb.c).                 (default 0) */
/* -------------------------------------------- */
#define RTP_MEM_BLOCK_PREPAD_SIZE    0

/* -------------------------------------------- */
/*  When more than giMemBase has been used,     */
/*  randomly deny malloc requests for torture   */
/*  purposes.                       (default 0) */
/* -------------------------------------------- */
#ifndef RTP_MEM_RESTRAIN
#define RTP_MEM_RESTRAIN             0
#endif

/* -------------------------------------------- */
/*  To enable this flag, support for file       */
/*  access must be enabled.         (default 0) */
/* -------------------------------------------- */
#define RTP_MEM_CHECK_BLOCK_LIST     0

/* -------------------------------------------- */
/*  To enable this flag, support for file       */
/*  access must be enabled, and printf support  */
/*  must be available.              (default 0) */
/* -------------------------------------------- */
#define RTP_MEM_PRINT_LEAKS          1

/* -------------------------------------------- */
/*  If RTP_MEM_PRINT_LEAKS is enabled the       */
/*  following sets the size of the buffer used  */
/*  to output to the log file passed to         */
/*  rtp_PrintMemLeaks().          (default 512) */
/* -------------------------------------------- */
#define RTP_MEM_REPORT_BUF_SIZE      512


/************************************************************************
 * Typedefs
 ************************************************************************/
typedef void * (* RTP_MALLOC_FN)(unsigned long);
typedef void * (* RTP_CALLOC_FN)(unsigned long, unsigned long);
typedef void * (* RTP_REALLOC_FN)(void *, unsigned long);
typedef void   (* RTP_FREE_FN)(void *);

/************************************************************************
 * Memory System Debug API which should be
 * used via the macros indicated below.
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
 
void * _rtp_debug_malloc  (unsigned long size, RTP_MALLOC_FN mallocMethod, const char *file, long line_num);
void * _rtp_debug_calloc  (unsigned long num, unsigned long size, RTP_CALLOC_FN callocMethod, const char* file, long line_num);
void * _rtp_debug_realloc (void * ptr, unsigned long size, RTP_REALLOC_FN reallocMethod, const char *file, long line_num);
void   _rtp_debug_free    (void * ptr, RTP_FREE_FN freeMethod, const char *file, long line_num);

void _rtp_debug_check_mem               (const char *logFile, const char* mode);
void _rtp_debug_print_mem_usage         (const char *logFile, const char* mode);
void _rtp_debug_set_high_water_callback (void (*fn)(void *), void* ctx);
void _rtp_debug_print_log               (const char *logFile, const char* mode);

unsigned long rtp_debug_get_mem_used (void);

#ifdef __cplusplus
}
#endif

/************************************************************************
 * Memory System Debug API macros
 ************************************************************************/
#define rtp_malloc(size)       _rtp_debug_malloc(size, _rtp_malloc, __FILE__, __LINE__)
#define rtp_calloc(num, size)  _rtp_debug_calloc(num, size, _rtp_calloc, __FILE__, __LINE__)
#define rtp_realloc(ptr, size) _rtp_debug_realloc(ptr, size, _rtp_realloc, __FILE__, __LINE__)
#define rtp_free(ptr)          _rtp_debug_free(ptr, _rtp_free, __FILE__, __LINE__)

#if (RTP_MEM_CHECK_BLOCK_LIST)
#define rtp_debug_check_mem(X,M)       _rtp_debug_check_mem(X,M)
#else
#define rtp_debug_check_mem(X,M)
#endif /* RTP_MEM_CHECK_BLOCK_LIST */

#ifdef RTP_MEM_PRINT_LEAKS
#define rtp_debug_print_mem_usage(X,M) _rtp_debug_print_mem_usage(X,M)
#else
#define rtp_debug_print_mem_usage(X,M)
#endif /* RTP_MEM_PRINT_LEAKS */

#else

/* Memory debugging turned off */

#define _rtp_debug_malloc(S,M,F,L)    M(S)
#define _rtp_debug_calloc(N,S,C,F,L)  C(N,S)
#define _rtp_debug_realloc(P,S,R,F,L) R(P,S)
#define _rtp_debug_free(P,D,F,L)      D(P)

#endif /* !RTP_TRACK_LOCAL_MEMORY */

#endif /*__RTPMEMDB_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
