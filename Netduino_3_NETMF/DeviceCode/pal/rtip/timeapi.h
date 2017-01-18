/*                                                                      */
/* TIMEAPI.H - TOD functions                                            */
/*                                                                      */
/* EBS - RTIP                                                           */
/*                                                                      */
/* Copyright EBSNet Inc. , 1993                                         */
/* All rights reserved.                                                 */
/* This code may not be redistributed in source or linkable object form */
/* without the consent of its author.                                   */
/*                                                                      */
/*  Module description:                                                 */
/*      This module contains TOD declarations                           */

#ifndef __TIMEAPI__
#define __TIMEAPI__

// ********************************************************************
/* 40-bit date value */

/* ********************************************************************   */
/* 40-bit date value                                                      */
typedef struct _ebs_timeval
{
    long  second;
    int   year;
} EBSTIME;
typedef EBSTIME RTP_FAR *PEBSTIME;

/* ********************************************************************   */
/* maximum length of string which can be written by xn_ebs_print_time()   */
#define EBS_MAX_TIME_LEN 50 

/* *********************************************************************   */
/* RUN TIME CONFIGURATION                                                  */
/* *********************************************************************   */
#if (INCLUDE_RUN_TIME_CONFIG)
#if (!KS_SUPPORTS_TIME || INCLUDE_SNTP) /* tbd - spr */
typedef struct cfg_sntp_data
{
    int tod_freq;

#if (INCLUDE_SNTP)
    /* Interval in which time of day should be updated via SNTP         */
    /* This is used if INCLUDE_SNTP is 1. This value should be greater  */
    /* or equal to CFG_SNTP_TOD_FREQ.                                   */
    /* Units: seconds                                                   */
    /* NOTE: this value should be greater than CFG_TOD_FREQ             */
    int sntp_tod_freq;

    /* number of seconds to wait for response to SNTP request   */
    /* NOTE: this value should be less than CFG_SNTP_TOD_FREQ   */
    /* Units: seconds                                           */
    int sntp_rcv_tmo;

    /* difference in hours of local time to Greenwich Mean Time   */
    /* NOTE: -5 = EST                                             */
    int gmt_diff;
#endif
} CFG_SNTP_DATA;
#endif
#endif /* INCLUDE_RUN_TIME_CONFIG */

#if (INCLUDE_RUN_TIME_CONFIG)
#define CFG_TOD_FREQ        cfg_sntp_data.tod_freq
#define CFG_SNTP_TOD_FREQ   cfg_sntp_data.sntp_tod_freq
#define CFG_SNTP_RCV_TMO    cfg_sntp_data.sntp_rcv_tmo
#define CFG_GMT_DIFF        cfg_sntp_data.gmt_diff
#else
#define CFG_TOD_FREQ        _CFG_TOD_FREQ
#define CFG_SNTP_TOD_FREQ   _CFG_SNTP_TOD_FREQ
#define CFG_SNTP_RCV_TMO    _CFG_SNTP_RCV_TMO
#define CFG_GMT_DIFF        _CFG_GMT_DIFF
#endif

/* ********************************************************************   */
#ifdef __cplusplus
extern "C" {
#endif

/* ********************************************************************   */
void  tc_convert_time_to_ebs(int date, int time, PEBSTIME ptime);
void  tc_convert_ebs_to_time(PEBSTIME ptime, int *year, int *month, int *day,
                          int *hour, int *minute, int *second);
int   ebs_parse_time(PEBSTIME ptime, char *str);
int   ebs_compare_time(PEBSTIME ptime1, PEBSTIME ptime2);
void  ebs_set_time_forever(PEBSTIME ptime);
void  ebs_add_seconds(PEBSTIME ptime, long seconds);
long  ebs_get_seconds_in_year(int year);
long  ebs_time_difference(PEBSTIME pearlier, PEBSTIME plater);

/* ********************************************************************   */
/* API                                                                    */

/* maximum string which can be written by xn_ebs_print_time   */
#define EBS_MAX_DATE_STR_LEN    34

int  xn_ebs_set_time(RTP_PFUINT8 ip_sntp_server, int year, int month, int day, 
                     int hour, int minute, int second);
int xn_ebs_set_time_srv_list(RTP_PFUINT8 ip_sntp_server, int num_servers,
                             int year, int month, int day, 
                             int hour, int minute, int second);
int  xn_ebs_get_system_time(PEBSTIME ptime);
int  xn_ebs_print_time(char *str, PEBSTIME ptime, int style);

#ifdef __cplusplus
}
#endif

#endif  /* TIMEAPI */


/*  EOF: TIMEAPI.H   */
