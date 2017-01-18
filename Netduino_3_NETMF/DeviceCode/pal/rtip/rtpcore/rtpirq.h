 /*
 | RTPIRQ.H - Runtime Platform IRQ Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform 
 |
 |  $Author: sarah $
 |  $Date: 2005/05/14 02:43:29 $
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

#ifndef __RTPIRQ_H__
#define __RTPIRQ_H__

#include "rtp.h"

/************************************************************************
 * Type definitions
 ************************************************************************/
typedef RTP_HANDLE RTP_IRQCNTXT;

/************************************************************************
 * Kernel API                                                           *
 ************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/* RTIP calls calls interrupts as normal 'C' functions. Code in 
   os_port.c converts the isr layer to 'C' and calls the device specific
   interrupt service routine */
typedef void (*RTP_IRQ_FN_POINTER)(void *);

RTP_IRQCNTXT rtp_irq_push_disable (void);
void         rtp_irq_pop          (RTP_IRQCNTXT previousLevel);
void         rtp_irq_disable      (void);
void         rtp_irq_enable       (void);
RTP_BOOL     rtp_irq_hook_interrupt(RTP_PFVOID piface,
   						  RTP_IRQ_FN_POINTER c_strategy,
						  RTP_IRQ_FN_POINTER c_interrupt);

#ifdef __cplusplus
}
#endif

#endif /* __RTPIRQ_H__ */

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
