//
// DEBUGAPI.H - ERROR LOGGING functions
//
// EBS - RTIP
//
// Copyright Peter Van Oudenaren , 1993
// All rights reserved.
// This code may not be redistributed in source or linkable object form
// without the consent of its author.
//


#ifndef __RTIP_DEBUGAPI__
#define __RTIP_DEBUGAPI__ 1

#include "rtpdbapi.h"

/* ********************************************************************   */
#define PORTS_TCP   16       /* write port lists; val2 is invalid */
#define PORTS_TCP_NO_SEM 17  /* write port lists; val2 is invalid */
#define PORTS_UDP   18       /* write port lists; val1 and val2 are invalid */
#define PORTS_UDP_NO_SEM 19  /* write port lists; val1 and val2 are invalid */
#define PORTS_RAW   20       /* write port lists; val1 and val2 are invalid */
#define PORTS_PING  21       /* write port lists; val1 and val2 are invalid */
#define PORTS_ALL   22       /* write port lists; val1 and val2 are invalid */
#define RT_ENTRY    13       /* val1 is a routing table entry */
#define RT          14       /* val1, val2 not used (ROUTING TABLE) */
#define ARPC_ENTRY  11       /* val1 is an arp cache entry */

// RTIP_DEBUG_ERROR, RTIP_DEBUG_LOG, RTIP_DEBUG_ASSERT
// ********************************************************************
#if (DEBUG_LEVEL == 0)

	// define out the RTIP_DEBUG_ERRORs for nothing
	#define RTIP_DEBUG_ERROR(STR, TYPE, X1, X2)
	#define RTIP_DEBUG_LOG(STR, LEVEL, TYPE, X1, X2)
	#define RTIP_DEBUG_ASSERT(MUST_BE_TRUE,STR,TYPE,X1,X2)
	#define RTIP_DEBUG_ASSERT_LOG(MUST_BE_TRUE,STR,LEVEL,TYPE,X1,X2)

// ********************************************************************
#elif (DEBUG_LEVEL == 1)

	// define RTIP_DEBUG_ERRORs and define out RTIP_DEBUG_LOGs
	#define RTIP_DEBUG_ERROR(STR, TYPE, X1, X2)                                    \
		xn_rtip_debug_error((RTP_PFCCHAR)STR, TYPE, (RTP_UINT32)X1, (RTP_UINT32)X2)
	#define RTIP_DEBUG_LOG(STR, LEVEL, TYPE, X1, X2)

	/* make sure this macro acts as a 'single statement' when
	 * postfixed by a regular semicolon... */
	#define RTIP_DEBUG_ASSERT(MUST_BE_TRUE,STR,TYPE,X1,X2) \
		if (!(MUST_BE_TRUE)) RTIP_DEBUG_ERROR(STR,TYPE,X1,X2)
	#define RTIPDEBUG_ASSERT_LOG(MUST_BE_TRUE,STR,LEVEL,TYPE,X1,X2)

// ********************************************************************
#else // (DEBUG_LEVEL > 1)

	// define RTIP_DEBUG_ERRORs and RTIP_DEBUG_LOGs
	#define RTIP_DEBUG_ERROR(STR, TYPE, X1, X2)                                    \
		xn_rtip_debug_error((RTP_PFCCHAR)STR, TYPE, (RTP_UINT32)X1, (RTP_UINT32)X2)
	#define RTIP_DEBUG_LOG(STR, LEVEL, TYPE, X1, X2)                               \
		xn_rtip_debug_log((RTP_PFCCHAR)STR, LEVEL, TYPE, (RTP_UINT32)X1, (RTP_UINT32)X2)
	/* make sure this macro acts as a 'single statement' when
	 * postfixed by a regular semicolon... */
	#define RTIP_DEBUG_ASSERT(MUST_BE_TRUE,STR,TYPE,X1,X2)                           \
		do                                                                      \
		{                                                                       \
			if (!(MUST_BE_TRUE))                                                \
			{                                                                   \
				RTP_DEBUG_ERROR("Assertion failed:",STR1,MUST_BE_TRUE,0);          \
				RTIP_DEBUG_ERROR(STR,TYPE,X1,X2);                                    \
			}                                                                   \
		} while(0)
	/* make sure this macro acts as a 'single statement' when
	 * postfixed by a regular semicolon... */
	#define RTIP_DEBUG_ASSERT_LOG(MUST_BE_TRUE,STR,LEVEL,TYPE,X1,X2)                 \
		do                                                                      \
		{                                                                       \
			if (!(MUST_BE_TRUE))                                                \
			{                                                                   \
				RTP_DEBUG_LOG("Assertion failed:",LEVEL,STR1,MUST_BE_TRUE,0);      \
				RTIP_DEBUG_LOG(STR,LEVEL,TYPE,X1,X2);                                \
			}                                                                   \
		} while(0)

#endif

// ********************************************************************
// API
// ********************************************************************
#ifdef __cplusplus
extern "C" {
#endif


// ********************************************************************
void SOCKAPI_ xn_rtip_debug_error(RTP_PFCCHAR string, int type, RTP_UINT32 val1, RTP_UINT32 val2);
void SOCKAPI_ xn_rtip_debug_log(RTP_PFCCHAR string, int level, int type, RTP_UINT32 val1, RTP_UINT32 val2);

#ifdef __cplusplus
}
#endif

#endif	// __RTIP_DEBUGAPI__ 

