/*                                                                      */
/* RTPDBAPI.H - ERROR LOGGING functions                                 */
/*                                                                      */
/* EBS - RTIP                                                           */
/*                                                                      */
/* Copyright Peter Van Oudenaren , 1993                                 */
/* All rights reserved.                                                 */
/* This code may not be redistributed in source or linkable object form */
/* without the consent of its author.                                   */
/*                                                                      */


#ifndef __RTIP_RTPDBAPI__
#define __RTIP_RTPDBAPI__ 1

/* ********************************************************************   */
/* YOU MIGHT WANT TO CHANGE VALUES IN THE FOLLOWING CONFIGUATION SECTION  */
/* ********************************************************************   */

/* ERROR LOGGING:                                                             */
/* Two macros RTP_DEBUG_ERROR and RTP_DEBUG_LOG are used throughout                   */
/* the code to log error and diagnostic messages.                             */
/* NOTE: ERROR LOGGING (RTP_DEBUG_ERROR) goes to the screen                       */
/*       and possiby out thru the RS232;  (it is also logged to RS232/FILE if */
/*       diagnostic messages are being logged or LOG_ERRS is set to 1).       */
/*       Diagnostic messages (RTP_DEBUG_LOG) are either sent out on RS232,        */
/*       written to a FILE (a:debug.out), or written to the SCREEN            */
/*       depending upon what IO_TYPE_FSR is set to                            */

/* Set error logging level such that:                                          */
/*   Level 0 = neither error messages or debug messages                        */
/*             i.e. set none of the calls to the RTP_DEBUG_ERROR                   */
/*             or RTP_DEBUG_LOG will be compiled in                                */
/*   Level 1 = error messages only (RTP_DEBUG_ERROR and RTP_DEBUG_ERROR_LOG),          */
/*             i.e. none of the calls to RTP_DEBUG_LOG will be compiled in         */
/*   Level 2 = error messages + application messages (DEBUG and RTP_DEBUG_LOG)     */
/*             NOTE: RTP_DEBUG_LOG calls in application are set to level 2         */
/*             NOTE: this level is very verbose and can change the timing      */
/*                   of RTIP                                                   */
/*   Level 3 = error messages + application messages + general flow of program */
/*             (RTP_DEBUG_ERROR and RTP_DEBUG_LOG)                                     */
/*             NOTE: RTP_DEBUG_LOG calls in application are set to level 2 and     */
/*                   RTP_DEBUG_LOG calls in RTIP are set to level 3                */
/*             NOTE: this level is very verbose and can change the timing      */
/*                   of RTIP                                                   */
/*   Level 4 = error messages + general flow of program +                      */
/*             more specific debug information (DEBUG and RTP_DEBUG_LOG)           */
/*             NOTE: RTP_DEBUG_LOG calls in application are set to level 2 and     */
/*                   RTP_DEBUG_LOG calls in RTIP are set to level 3                */
/*             NOTE: this level is very verbose and can change the timing      */
/*                   of RTIP                                                   */
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif


/* ********************************************************************   */
/* definitions for TYPE parameter                                         */
#define NOVAR        0       /* write comment only, i.e.val1 and val2 are invalid */
#define EBS_INT1     1       /* val1 is an integer */
#define EBS_INT2     2       /* val1 and val 2 are an integers */
#define LINT1        3       /* val1 is a long signed integer */
#define LINT2        4       /* val1 and val 2 are long signed integers */
#define DINT1        5       /* val1 is a long unsigned integer */
#define DINT2        6       /* val1 and val 2 are long unsigned integers */
#define STR1         7       /* val1 is a string */
#define STR2         8       /* val1 and val 2 are strings */
#define IPADDR       9       /* val1 is an IP address of type PFBYTE */
#define IPV6ADDR    10       /* val1 is an IP address of type PFBYTE */
#define ETHERADDR   11       /* val1 is an ethernet address */
#define PKT         12       /* val1 is an ethernet packet of type PFBYTE */

/* RTP_DEBUG_ERROR, RTP_DEBUG_LOG, RTP_DEBUG_ASSERT                                 */
/* ******************************************************************** */
#if (DEBUG_LEVEL == 0)

    /* define out the RTP_DEBUG_ERRORs for nothing   */
    #define RTP_DEBUG_ERROR(STR, TYPE, X1, X2)
    #define RTP_DEBUG_LOG(STR, LEVEL, TYPE, X1, X2)
    #define RTP_DEBUG_ASSERT(MUST_BE_TRUE,STR,TYPE,X1,X2)
    #define RTP_DEBUG_ASSERT_LOG(MUST_BE_TRUE,STR,LEVEL,TYPE,X1,X2)

/* ********************************************************************   */
#elif (DEBUG_LEVEL == 1)

    /* define RTP_DEBUG_ERRORs and define out RTP_DEBUG_LOGs   */
    #define RTP_DEBUG_ERROR(STR, TYPE, X1, X2)                                    \
        xn_rtp_debug_error((RTP_PFINT8)STR, TYPE, (RTP_INT32)X1, (RTP_INT32)X2)
    #define RTP_DEBUG_LOG(STR, LEVEL, TYPE, X1, X2)\
	/* make sure this macro acts as a 'single statement' when
     * postfixed by a regular semicolon... */
    #define RTP_DEBUG_ASSERT(MUST_BE_TRUE,STR,TYPE,X1,X2) \
        if (!(MUST_BE_TRUE)) RTP_DEBUG_ERROR(STR,TYPE,X1,X2)
    #define RTP_DEBUG_ASSERT_LOG(MUST_BE_TRUE,STR,LEVEL,TYPE,X1,X2)


/* ********************************************************************   */
#else /* (DEBUG_LEVEL > 1) */

    /* define RTP_DEBUG_ERRORs and RTP_DEBUG_LOGs   */
    #define RTP_DEBUG_ERROR(STR, TYPE, X1, X2)                                    \
        xn_rtp_debug_error((RTP_PFINT8)STR, TYPE, (RTP_INT32)X1, (RTP_INT32)X2)
    #define RTP_DEBUG_LOG(STR, LEVEL, TYPE, X1, X2)                               \
        xn_rtp_debug_log(STR, LEVEL, TYPE, (RTP_UINT32)X1, (RTP_UINT32)X2)
    /* make sure this macro acts as a 'single statement' when
     * postfixed by a regular semicolon... */
    #define RTP_DEBUG_ASSERT(MUST_BE_TRUE,STR,TYPE,X1,X2)                           \
        do                                                                      \
        {                                                                       \
            if (!(MUST_BE_TRUE))                                                \
            {                                                                   \
                RTP_DEBUG_ERROR("Assertion failed:",STR1,#MUST_BE_TRUE,0);          \
                RTP_DEBUG_ERROR(STR,TYPE,X1,X2);                                    \
            }                                                                   \
        } while(0)

    /* make sure this macro acts as a 'single statement' when
     * postfixed by a regular semicolon... */
    #define RTP_DEBUG_ASSERT_LOG(MUST_BE_TRUE,STR,LEVEL,TYPE,X1,X2)                 \
        do                                                                      \
        {                                                                       \
            if (!(MUST_BE_TRUE))                                                \
            {                                                                   \
                RTP_DEBUG_LOG("Assertion failed:",LEVEL,STR1,#MUST_BE_TRUE,0);      \
                RTP_DEBUG_LOG(STR,LEVEL,TYPE,X1,X2);                                \
            }                                                                   \
        } while(0)

#endif

/* ********************************************************************   */
/* API                                                                    */
/* ********************************************************************   */
#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************   */
void xn_rtp_debug_error(RTP_PFINT8 string, int type, RTP_INT32 val1, RTP_INT32 val2);
void xn_rtp_debug_log(RTP_PFCCHAR comment, int level, int type, RTP_UINT32 val1, RTP_UINT32 val2);

#ifdef __cplusplus
}
#endif

#endif  /* __RTIP_RTPDBAPI__  */

