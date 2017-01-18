 /*
 | RTPPORT.H - Target specific setup and configuration parameters
 |
 |
 |   PORTED TO THE Segger embOS simulation for Windows 32 bit system
 |                 and Visual C++.
 |
 |  $Author: sarah $
 |  $Date: 2005/05/20 16:38:48 $
 |  $Name:  $
 |  $Revision: 1.3 $
 |
 | Copyright EBSnet Inc. , 2003
 | All rights reserved.
 | This code may not be redistributed in source or linkable object form
 | without the consent of its author.
 |
 | Module description:
 |  [tbd]
*/

#ifndef __RTPPORT_H__
#define __RTPPORT_H__

#define RTP_INCLUDE_IRQ
#define RTP_INCLUDE_KERN
//#define RTP_INCLUDE_MEM
#define RTP_INCLUDE_SIGNL
#define RTP_INCLUDE_TERM
#define RTP_INCLUDE_THRD
#define RTP_INCLUDE_TIME


/* YOU MIGHT WANT TO CHANGE THE FOLLOWING                    */
/* Uncomment to include callbacks to application to allow POLLOS */
/* to yield to kernel when it has nothing to do; the         */
/* callback routines are cb_pollos_yield_to_rtos() and       */
/* cb_pollos_signal_to_rtos()                                */
/* This define is useful if running POLLOS as a task in a    */
/* multitasking kernel                                       */
//#define INCLUDE_POLLOS_COOPERATIVE
#define POLLOS  1

/************************************************************************
* Target Specific Headers
************************************************************************/

// EMBOS header

#include "bget.h"

/************************************************************************
* PORTING DEFINES
**********************************************************************/
// comment in or out the following two defines if needed
#define RTP_LITTLE_ENDIAN 1
#define RTP_LONGWORD_ALLIGNED 

/************************************************************************
* RTPIRQ.C - Runtime Platform IRQ Services
************************************************************************/
#ifdef RTP_INCLUDE_IRQ
#include "rtpirq.h"

#endif /* RTP_INCLUDE_IRQ */

/************************************************************************
* RTPKERN.C - Runtime Platform Kernel Services
************************************************************************/
#ifdef RTP_INCLUDE_KERN
#include "rtpkern.h"

#endif /* RTP_INCLUDE_KERN */

/************************************************************************
* RTPMEM.C - Runtime Platform Memory System Services
************************************************************************/
#ifdef RTP_INCLUDE_MEM
#include "rtpmem.h"

#endif /* RTP_INCLUDE_MEM */

/************************************************************************
* RTPSIGNL.C - Runtime Platform Semaphore and Mutex Services
************************************************************************/
#ifdef RTP_INCLUDE_SIGNL
#include "rtpsignl.h"

/* The following may be added to the API but are left here for now.
   This section provides a dynamic allocation scheme for signals and mutexes */
#ifdef RTP_INC_DYNAMIC_SIGNAL
typedef struct s_RTP_SIGNAL_ALLOC RTP_SIGNAL_ALLOC;
struct s_RTP_SIGNAL_ALLOC {
    void* (*AllocFn) (unsigned long size);
    void  (*FreeFn)  (void * ptr);
};
int  rtp_sig_semaphore_alloc_dynamic (RTP_SEMAPHORE *newSem, const char *name, 
                                       RTP_SIGNAL_ALLOC *funcptr);
#endif /* RTP_INC_DYNAMIC_SIGNAL */

#ifdef RTP_INC_DYNAMIC_MUTEX
typedef struct s_RTP_MUTEX_ALLOC RTP_MUTEX_ALLOC;
struct s_RTP_MUTEX_ALLOC {
    void* (*AllocFn) (unsigned long size);
    void  (*FreeFn)  (void * ptr);
};
int  rtp_sig_mutex_alloc_dynamic (RTP_MUTEX *newMutex, const char *name, 
                                       RTP_MUTEX_ALLOC *funcptr);
#endif /* RTP_INC_DYNAMIC_MUTEX */
/* end dynamic allocation scheme */


#endif /* RTP_INCLUDE_SIGNL */

/************************************************************************
* RTPTERM.C - Runtime Platform Terminal I/O Services
************************************************************************/
#ifdef RTP_INCLUDE_TERM
#include "rtpterm.h"
#include "rtpstr.h"
#endif /* RTP_INCLUDE_TERM */

/************************************************************************
* RTPTHRD.C - Runtime Platform Thread Services
************************************************************************/
#ifdef RTP_INCLUDE_THRD
#include "rtpthrd.h"

#define RTP_CFG_N_STACKS_HUGE      0
#define RTP_CFG_SIZESTACK_HUGE     0x8000
#define RTP_CFG_N_STACKS_BIG       2
#define RTP_CFG_SIZESTACK_BIG      0x8000
#define RTP_CFG_N_STACKS_NORMAL    8
#define RTP_CFG_SIZESTACK_NORMAL   0x4000
#define RTP_CFG_N_STACKS_SMALL     4
#define RTP_CFG_SIZESTACK_SMALL    0x2000
#define RTP_CFG_N_STACKS_TINY      2
#define RTP_CFG_SIZESTACK_TINY     0x200

#endif /* RTP_INCLUDE_THRD */

/************************************************************************
* RTPTIME.C - Runtime Platform Timing Services
************************************************************************/
#ifdef RTP_INCLUDE_TIME
#include "rtptime.h"

#endif /* RTP_INCLUDE_TIME */

/************************************************************************
************************************************************************/
#endif /* __RTPPORT_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
