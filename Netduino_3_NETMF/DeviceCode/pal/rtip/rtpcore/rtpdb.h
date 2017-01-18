/*                                                                      */
/* RTPDB.H - DISPLAY functions                                          */
/*                                                                      */
/* EBS - RTIP                                                           */
/*                                                                      */
/* Copyright Peter Van Oudenaren , 1993                                 */
/* All rights reserved.                                                 */
/* This code may not be redistributed in source or linkable object form */
/* without the consent of its author.                                   */
/*                                                                      */


#ifndef __RTIP_RTPDB__
#define __RTIP_RTPDB__ 1

#include "rtp.h"
#include "rtpdbapi.h"

/* ********************************************************************   */
/* END OF CONFIGUATION SECTION                                            */
/* ********************************************************************   */

#define LEVEL_1  1
#define LEVEL_2  2
#define LEVEL_3  3
#define LEVEL_4  4

/* ********************************************************************   */
/* NOTE: if 1 than can call says directly and always will be displayed;   */
/*       if 0 then direct calls will be compiled out                      */
/*       NOTE: this controls whether output from special keys in demo     */
/*             display results                                            */

#ifdef __cplusplus
extern "C" {
#endif

#if (DEBUG_LEVEL > 0)
    void say(int error_type, RTP_PFINT8 comment);
    void say_hex(int error_type, RTP_PFINT8 comment, RTP_PFINT8 p, int len);
    void say_ip_addr(int error_type, RTP_PFINT8 comment,RTP_PFUINT8 addr);
    void say_ether_addr(int error_type, RTP_PFINT8 comment, RTP_PFUINT8 addr);
    void say_int(int error_type, RTP_PFINT8 comment, int val);
    void say_int2(int error_type, RTP_PFINT8 comment, int val1, int val2);
    void say_lint(int error_type, RTP_PFINT8 comment, long val1);
    void say_lint2(int error_type, RTP_PFINT8 comment, long val1, long val2);
    void say_dint(int error_type, RTP_PFINT8 comment, RTP_INT32 val1);
    void say_dint2(int error_type, RTP_PFINT8 comment, RTP_INT32 val1, RTP_INT32 val2);
    void say_str(int error_type, RTP_PFINT8 comment, RTP_PFINT8 val1);
    void say_str2(int error_type, RTP_PFINT8 comment, RTP_PFINT8 val1, RTP_PFINT8 val2);
#endif

#ifdef __cplusplus
}
#endif


#if (INCLUDE_PROFILING)
/* ********************************************************************   */
/* Profiling package -
   This is an execution profiling package. It is used internally to
   profile code sections.
   Two macros: PROFILE_ENTER(TOKEN) and PROFILE_EXIT(TOKEN) are defined. If
   profiling is turned off they do nothing. If they are on they note the
   start time and end time for the section being profiled. The routine
   dump_profile() dumps the duration for each call. This profiling package
   does not do histogram analysis. It only logs the duretion of the last
   pass through the code being profiled.. so it must be used carefully.

   Currently the only platform that supports profiling is the NET186 board.
*/

struct timerecord
{
	RTP_UINT32 start_micro;  /* Value of the 0 to 1000 clock at start */
	RTP_UINT32 end_micro;  /* Value of the interrupt clock at start */
};

struct namerecord
{
    int token;
    char *name;
};


extern RTP_UINT32 tick_every_10000_tenth_micros;

#define PROF_NOW (RTP_UINT32) (tick_every_10000_tenth_micros +  (RTP_UINT32) (INWORD((TMR2_COUNT))))

#define PROFILE_ENTER(TOKEN) profile_time_array[TOKEN].start_micro = PROF_NOW;
#define PROFILE_EXIT(TOKEN) profile_time_array[TOKEN].end_micro = PROF_NOW;

void dump_profile(void);

#else

#define PROFILE_ENTER(TOKEN)
#define PROFILE_EXIT(TOKEN)


#endif

/* Define ID's for regions baing profiled. Each token maps to an entry in
   the profile_time_array[] (see rtpdb.c). Each token must also have an
   entry in the profile_name_array[] (see rtpdb.c)
*/
#define PROF_TOKEN_IP_INTERPRET 0
#define PROF_N_RECORDS 1

#if (INCLUDE_PROFILING)

extern struct timerecord profile_time_array[PROF_N_RECORDS];

#endif


#endif  /* RTPDB */

