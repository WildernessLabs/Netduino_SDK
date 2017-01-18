
#ifndef __RTPPRINT_H__
#define __RTPPRINT_H__

#include "rtp.h"

/************************************************************************
* Configuration Define                                                  *
*************************************************************************/

/************************************************************************
 * Native print library utility functions includes                      *
 *      printf      vprintf      va_list    va_end                      *
 *      sprintf     vsprintf     va_arg                                 *
 *      snprintf    vsnprintf    va_start                               *
 ************************************************************************/
 

/************************************************************************
* Set the following defines according to the protocol needs.            *
*************************************************************************/

/* -------------------------------------------- */
/*              Add printf support.             */
/* -------------------------------------------- */
#define RTP_INCLUDE_PRINTF                      1

/* -------------------------------------------- */
/*              Add sprintf support.            */
/* -------------------------------------------- */
#define RTP_INCLUDE_SPRINTF                     1


/************************************************************************
* Typedefs                                                              *
*************************************************************************/

/************************************************************************
* Macros                                                                *
*************************************************************************/

/* if defining 'RTP_USE_STDARG' causes compile problems, comment out the
   following line, and copy the definitions of va_list, va_start, va_arg, 
   and va_end into this header file from your compiler's stdarg.h */
/* #define RTP_USE_STDARG */

#ifdef RTP_USE_STDARG

#include <stdarg.h>

typedef va_list rtp_va_list;
#define rtp_va_start(ap, lastarg) va_start(ap, lastarg)
#define rtp_va_arg(ap, type)      va_arg(ap, type)
#define rtp_va_end(ap)            va_end(ap)

#else /* RTP_USE_STDARG */

/* This is just a common implementation of va_list.  It will probably
   NOT WORK on your compiler, if you are targeting something other than
   Intel x86.  These macros may need to be copied directly from your 
   compiler's stdarg.h file (see comment above on RTP_USE_STDARG) */

#define _RTPINTSIZEOF(n)          ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
typedef void * rtp_va_list;
#define rtp_va_start(ap, lastarg) ( ap = (rtp_va_list)((char*)&(lastarg) + _RTPINTSIZEOF(lastarg)) )
#define rtp_va_arg(ap, type)      ( (char*)ap += _RTPINTSIZEOF(type) , *((type*)((char*)ap - _RTPINTSIZEOF(type))) )
#define rtp_va_end(ap)            ( ap = (rtp_va_list)0 )

#endif /* RTP_USE_STDARG */


/************************************************************************/
/*      THERE IS NO NEED TO CHANGE ANYTHING BELOW THIS COMMENT          */
/************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************
* Prototypes                                                            *
*************************************************************************/

#if (RTP_INCLUDE_PRINTF)
#ifndef rtp_printf
/*----------------------------------------------------------------------
 rtp_printf - print formatted output
----------------------------------------------------------------------*/
int             rtp_printf     (const char * f, ...);
#endif


#ifndef rtp_vprintf
/*----------------------------------------------------------------------
 rtp_vprintf - print a list of formatted data to a string
----------------------------------------------------------------------*/
int             rtp_vprintf    (const char * f, rtp_va_list ap);
#endif
#endif /* INCLUDE_TC_PRINTF_SUPPORT */


#if (RTP_INCLUDE_SPRINTF)
#ifndef rtp_sprintf
/*----------------------------------------------------------------------
 rtp_sprintf - print formatted data to a string
----------------------------------------------------------------------*/
int             rtp_sprintf    (char * wh, const char * f, ...);
#endif


#ifndef rtp_snprintf
/*----------------------------------------------------------------------
 rtp_snprintf - print formatted data to a string
----------------------------------------------------------------------*/
int             rtp_snprintf   (char * wh, int bufflen, const char * f, ...);
#endif


#ifndef rtp_vsprintf
/*----------------------------------------------------------------------
 rtp_vsprintf - print a list of formatted data to a string
----------------------------------------------------------------------*/
int             rtp_vsprintf   (char * wh, const char * f, rtp_va_list ap);
#endif


#ifndef rtp_vsnprintf
/*----------------------------------------------------------------------
 rtp_vsnprintf - print a list of formatted data to a string
----------------------------------------------------------------------*/
int             rtp_vsnprintf  (char * wh, long bufflen, const char * f, rtp_va_list ap);
#endif
#endif /* INCLUDE_TC_SPRINTF_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif /*__RTPPRINT_H__*/

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
