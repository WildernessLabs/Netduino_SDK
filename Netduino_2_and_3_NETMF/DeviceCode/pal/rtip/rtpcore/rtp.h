 /*
 | RTP.H - Runtime Platform Services
 |
 |   UNIVERSAL CODE - DO NOT CHANGE
 |
 | EBSnet - RT-Platform
 |
 |  $Author: sarah $
 |  $Date: 2005/06/14 01:33:31 $
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

#ifndef __RTP_H__
#define __RTP_H__

/* ----------------------------------- */
/*  Shared definitions for the         */
/*  RT-Platform interface              */
/* ----------------------------------- */
/* FOR EXAMPLE: (RTPLATFORM_VERSION)
        0004 == 00.04 == 0.4
        0201 == 02.01 == 2.1
        0415 == 04.15 == 4.15
        1003 == 10.03 == 10.3
*/
#define RTPLATFORM_VERSION             0004 /* 0.4 */
#define RTP_TIMEOUT_INFINITE           -1

/* ----------------------------------- */
/* enables using code as DLL            */
/* NOTE: the same define in in socket.h */
#define SOCKAPI_
/*#define SOCKAPI_  __stdcall __export   */


/* ----------------------------------- */
/* Various platform-specific defines.  */
/* ----------------------------------- */
#ifndef RTP_HANDLE
typedef long RTP_HANDLE;
#endif

#include "rtpport.h"
#include "rtpenv.h"     // porting from rtpport directory
#include "rtptypes.h"
#include "rtputil.h"
#include "rtpstr.h"
#include "rtpscnv.h"
#include "rtpchar.h"
#include "rtpsignl.h"
#include "rtpdb.h"


#if !defined(__GNUC__)

// tbd - in RTIP
RTP_EXTERN int RTP_FAR in_irq;

#endif

/* *********************************************************************   */
#define TASKPRIO_NORMAL_INDEX  CFG_PRIO_INDEX_NORMAL
#define STACKSIZE_NORMAL_INDEX CFG_STACK_SIZE_NORMAL
#define STACKSIZE_BIG_INDEX    CFG_STACK_SIZE_BIG
#define STACKSIZE_HUGE_INDEX   CFG_STACK_SIZE_HUGE

// *******************************************************************
// constant definition - TBD - is this a porting issue?
// *******************************************************************
#if (defined(SEGMC16) || HIGHC386 || defined(RTPXGSH2) || defined(RTPXGSH3))
#define KS_CONSTANT
#define KS_GLOBAL_CONSTANT
#define KS_EXTERN_GLOBAL_CONSTANT extern

#else
/*
    KS_CONSTANT is mostly used for parameters and structure fields. The
    other _CONSTANT macros below are needed for global constants, since
    C++ handles these differently than C. In C++, a global constant can
    be treated like a #define, meaning the compiler does not allocate
    space for it, and only uses it locally in the module where it is
    defined. Any external references to it won't link since the symbol
    doesn't actually exist. In order to force the C++ compiler to allocate
    storage and make it public, the extern keyword is used. Normally,
    extern is used to indicate an external reference to something defined
    elsewhere, but for a C++ const it is used also in the definition.
    The compiler can tell it is the definition because of the assignment
    of the initial value, which the other extern references won't have.
    For example:
        extern const int a = 100;  is the definition.
        extern const int a;        is the extern reference.
*/
/*
    The first macro is mostly used for parameters and structure fields.
    The others were needed for global constants, since C++ handles these
    differently than C. In C++, a global constant can be treated like a
    #define, meaning the compiler does not allocate space for it, and
    only uses it locally in the module where it is defined. Any external
    references to it won't work if the compiler does that since the symbol
    doesn't exist. In order to force the compiler to allocate storage and
    make it public, the definition needs the extern keyword. Normally,
    extern is used to indicate that the statement it is used in is not
    a definition but just an external reference to something defined
    elsewhere, but not in this case. The compiler can tell it is the
    definition because of the assignment of the initial value, which the
    other extern references to it won't have. For example, extern int a = 100.
*/
#define KS_CONSTANT const
#if (__cplusplus)
#define KS_GLOBAL_CONSTANT extern const
#else
#define KS_GLOBAL_CONSTANT const
#endif
#define KS_EXTERN_GLOBAL_CONSTANT extern const
#endif

/* ----------------------------------- */
/* Callbacks to APPLICATIONc defines.  */
/* ----------------------------------- */
typedef void (*CB_WR_ERROR_STRING_FNC)(RTP_PFCCHAR in_string, RTP_BOOL from_interrupt);
typedef void (*CB_WR_INTERRUPT_STRING_FNC)(void);
typedef void (*CB_WR_LOG_STRING_FNC)(RTP_PFCCHAR in_string, RTP_BOOL from_interrupt);

#define CB_WR_ERROR_STRING(a, b)                                    \
    if (rtp_callbacks && rtp_callbacks->cb_wr_error_string_fnc)     \
        rtp_callbacks->cb_wr_error_string_fnc(a, b)
#define CB_WR_INTERRUPT_STRING()                                    \
    if (rtp_callbacks && rtp_callbacks->cb_wr_interrupt_string_fnc) \
        rtp_callbacks->cb_wr_interrupt_string_fnc()
#define CB_WR_LOG_STRING(a, b)                                      \
    if (rtp_callbacks && rtp_callbacks->cb_wr_error_string_fnc)     \
        rtp_callbacks->cb_wr_error_string_fnc(a, b)

/* *********************************************************************   */
typedef struct rtp_callbacks
{
    /* output routines for error logging   */
    CB_WR_ERROR_STRING_FNC      cb_wr_error_string_fnc;
    CB_WR_INTERRUPT_STRING_FNC  cb_wr_interrupt_string_fnc;
    CB_WR_LOG_STRING_FNC        cb_wr_log_string_fnc;
} RTP_CALLBACKS;

typedef RTP_CALLBACKS RTP_FAR * PRTP_CALLBACKS;


/* ********************************************************************   */
/* CALLBACKS                                                              */
/* ********************************************************************   */
extern PRTP_CALLBACKS rtp_callbacks;

/************************************************************************
 * API functions                                                        *
 ************************************************************************/

void rtp_register_callbacks(PRTP_CALLBACKS rtp_cb);

void rtp_not_yet_implemented (void);

#endif /*__RTP_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
