 /*
 | RTPTHRD.H - Runtime Platform Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform 
 |
 |  $Author: sarah $
 |  $Date: 2005/05/14 02:43:30 $
 |  $Name:  $
 |  $Revision: 1.1 $
 |
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

#ifndef __RTPTHRD_H__
#define __RTPTHRD_H__

/************************************************************************
 * Portable thread services including:
 *   initialization and reinitialization of thread services
 *   thread create(and start)/exit(and delete)
 *   thread info (thread handle/index, user data)
 *   thread control (sleep, yield)
 ************************************************************************/
#include "rtp.h"

/* *********************************************************************   */
/* Compile Time configuration for number of contexts to allocate		   */
/* *********************************************************************   */

#define MAX_CONTEXT (32 + 9 + 5)
					 /*	9 for tc_timer, main, ping_client 		
					 		select1_test, select2_test, 
							tcp_server_main, tcp_client_main, 
							udp_server_main, udp_client_main	*/
					/* 5 for application 						*/

/* *********************************************************************   */
typedef struct rtp_system_user
{
    int                     rtip_errno; /* current errno value for the task */
    RTP_PFVOID              udata0;     /* pointers for context blocks or */
    RTP_PFVOID              udata1;     /* other user data. We have up to */
    RTP_PFVOID              udata2;     /* four available */
    RTP_PFVOID              udata3;     /*  */
#if (INCLUDE_TERMMAN)
    RTP_PFVOID              terminal_context; /* Points to the current terminal */
                                          /* context for this task;    */
                                          /* the type is PTERMCONTEXT  */
                
#endif
//#if (INCLUDE_MFS && INCLUDE_SUBDIRS)
    /* current working directory; MFS never changes the cwd but just   */
    /* keeps track of it; it does all commands on the full path        */
    RTP_PFCHAR              mfcwd;
//#endif
} RTP_SYSTEM_USER;

typedef struct rtp_system_user RTP_FAR *PRTP_SYSTEM_USER;	/* MOVED FROM RTIPAPI.H */ /* _YI_ */


typedef struct _context_data        
{                                   
    int RTP_FAR     staticNo;       
    RTP_SYSTEM_USER     sysUser;      
} CONTEXT_DATA;  /* _YI_ 3/3/2005 */


typedef RTP_HANDLE RTP_THREAD;

typedef RTP_HANDLE RTP_REENTRANT_THREAD;
typedef RTP_HANDLE RTP_THREADP;

typedef RTP_HANDLE RTP_STACK;
typedef RTP_HANDLE RTP_STACKP;

typedef void (* RTP_ENTRY_POINT_FN)(void *);
 
#define CFG_NUM_THREADS         32
#define CFG_NAME_SIZE           32


#define CFG_STACK_SIZE_HUGE     4
#define CFG_STACK_SIZE_BIG      3
#define CFG_STACK_SIZE_NORMAL   2
#define CFG_STACK_SIZE_SMALL    1
#define CFG_STACK_SIZE_TINY     0

#define CFG_PRIO_INDEX_HIGHEST 3
#define CFG_PRIO_INDEX_HIGH    2
#define CFG_PRIO_INDEX_NORMAL  1

#define rtp_thread_init  rtp_threads_init
#define rtp_thread_exit  rtp_threads_shutdown
#define rtp_threads_exit rtp_threads_shutdown


/************************************************************************
 * Thread services                                                      *
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
 
int  rtp_threads_init               (void);
void rtp_threads_shutdown           (void);

int  rtp_threads_pool_init          (int nThread, RTP_THREADP resourceBlock);
int  rtp_threads_stacks_pool_init   (int nStack, int stackSizeIndex,
                                     int stackSize,
                                     RTP_STACKP resourceBlock);

int  rtp_thread_spawn               (RTP_THREAD  * newThread,
                                     RTP_ENTRY_POINT_FN entryPoint,
                                     const char  * name, 
                                     int           stackSizeIndex, 
                                     int           priorityIndex, 
                                     void        * userData);

int  rtp_thread_kill                (RTP_THREAD  * newThread,
                                     RTP_ENTRY_POINT_FN entryPoint,
                                     const char  * name, 
                                     int           stackSizeIndex, 
                                     int           priorityIndex, 
                                     void        * userData);
                          
int  rtp_thread_handle              (RTP_THREAD  * currentThread);

int  rtp_thread_user_data           (void ** userData);
int  rtp_thread_user_data_by_handle (RTP_THREAD handle, 
                                     void ** userData);
                                     
int  rtp_thread_name                (char ** name);
int  rtp_thread_name_by_handle      (RTP_THREAD handle, 
                                     char ** name);

void rtp_thread_sleep               (long msecs);
void rtp_thread_yield               (void);

#ifdef __cplusplus
}
#endif

#endif /* __RTPTHRD_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
