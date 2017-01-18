 /*
 | RTPTYPES.H - Runtime Platform Services
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

#ifndef __RTPTYPES_H__
#define __RTPTYPES_H__

#if !defined(__RTIP_TYPES__)

#if  (defined(__BORLANDC__) || defined(_MSC_VER) || defined(__WATCOMC__))  
// ********************************************************************
/* BORLAND, MICROSOFT, WATCOM, and HIGHC                                  */
// ********************************************************************

/* Simplify test of if we're using microsoft 8.0 + in protected mode */
#define IS_MS_PM   0
#ifdef _MSC_VER
#undef IS_MS_PM
#define IS_MS_PM   ( (_MSC_VER >= 800) && (_M_IX86 >= 300) )
#endif

/* Simplify test of if we're using Borland + in protected mode */
#define IS_BCC_PM 0
#if (defined(__BORLANDC__))
#ifdef __WIN32__
#undef IS_BCC_PM
#define IS_BCC_PM 1
#endif
#endif

/* Simplify test of if we're using Watcom in protected mode */
#define IS_WC_PM 0
#if (defined(__WATCOMC__))
#ifdef __386__
#undef IS_WC_PM
#define IS_WC_PM 1
#endif
#endif

/* Simplify test of if we're using High C/C++ in protected mode   */
#define IS_HC_PM 0

#endif

// ********************************************************************
// RTP_FAR
// ********************************************************************
#define RTP_FAR

#if ( defined(__BORLANDC__) || defined(_MSC_VER) ||  defined(__WATCOMC__) )	// real mode
#undef RTP_FAR
#define RTP_FAR far
#endif

#if (IS_MS_PM || IS_BCC_PM || IS_WC_PM || IS_HC_PM || defined(MC68K) || MCF52XX || defined(DJGPP) || defined(SEG_IAR) || defined(RTPXGSH2) || defined(RTPXGSH3))
#undef RTP_FAR
#define RTP_FAR
#define far
#endif

// ********************************************************************
#define RTP_INLINE

// ********************************************************************
#define RTP_CONST  const
#define RTP_EXTERN extern

#define RTIP_STATIC static

typedef char                              RTP_INT8;
typedef RTP_CONST char                    RTP_CINT8;
typedef unsigned char                     RTP_UINT8;
typedef RTP_CONST unsigned char           RTP_CUINT8;

typedef short                             RTP_INT16;
typedef RTP_CONST short                   RTP_CINT16;
typedef unsigned short                    RTP_UINT16;
typedef RTP_CONST unsigned short          RTP_CUINT16;
typedef long                              RTP_INT32;
typedef RTP_CONST long                    RTP_CINT32;
typedef unsigned long                     RTP_UINT32;
typedef RTP_CONST unsigned long           RTP_CUINT32;
                                     
typedef char                              RTP_CHAR;
typedef RTP_CONST char                    RTP_CCHAR;
typedef unsigned char                     RTP_UCHAR;
typedef RTP_CONST unsigned char           RTP_CUCHAR;
                                     
typedef char RTP_FAR*                     RTP_PFINT8;
typedef unsigned char RTP_FAR*            RTP_PFUINT8;
typedef RTP_CONST unsigned char RTP_FAR*  RTP_PFCUINT8;
typedef short RTP_FAR*                    RTP_PFINT16;
typedef unsigned short RTP_FAR*           RTP_PFUINT16;
typedef RTP_CONST unsigned short RTP_FAR* RTP_PFCUINT16;
typedef long RTP_FAR*                     RTP_PFINT32;
typedef unsigned long RTP_FAR*            RTP_PFUINT32;
typedef RTP_CONST unsigned long RTP_FAR*  RTP_PFCUINT32;

typedef char RTP_FAR*                     RTP_PFCHAR;
typedef RTP_CONST char RTP_FAR*           RTP_PFCCHAR;
typedef unsigned char RTP_FAR*            RTP_PFUCHAR;
typedef RTP_CONST unsigned char RTP_FAR*  RTP_PFCUCHAR;

// SPR
typedef int RTP_FAR * 					  RTP_PFINT;
typedef long  RTP_FAR * 				  RTP_PFLONG;
typedef void  RTP_FAR * 				  RTP_PFVOID;
typedef RTP_CONST void  RTP_FAR * 		  RTP_PFCVOID;

typedef unsigned                      	  RTP_BOOL;
typedef RTP_BOOL RTP_FAR * 			      RTP_PFBOOL;

#define RTP_TRUE  1
#define RTP_FALSE 0

#endif
#endif /*__RTPTYPES_H__*/



/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */
