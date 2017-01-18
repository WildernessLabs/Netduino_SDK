

/* SNTPCONF.H  - configuration file

   EBS - RTIP

   Copyright EBSNet Inc. , 1993
   All rights reserved.
   This code may not be redistributed in source or linkable object form
   without the consent of its author.

    Module description:
        This module contains constants that may be change to the modify
        the build characteristics of RTIP.

        NOTE: constants which start with _CFG are runtime configurable;
              (see confdata.c)
              constants which start with CFG are NOT runtime configurable
*/

#ifndef __SNTPCONF__
#define __SNTPCONF__ 1

/* ************************************************************************    */
/* ******************        TOD/SNTP                   *******************    */
/* ************************************************************************    */

// NOTE: _CFG_TOD_FREQ  is defined in rtipconf.h

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
// Interval in which time of day should be updated via SNTP
// This is used if INCLUDE_SNTP is 1. This value should be greater 
// or equal to _CFG_SNTP_TOD_FREQ.
// Units: seconds
// NOTE: this value should be greater than _CFG_TOD_FREQ
#define _CFG_SNTP_TOD_FREQ      360    // 6 minutes

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
// number of seconds to wait for response to SNTP request
// NOTE: this value should be less than _CFG_SNTP_TOD_FREQ
// Units: seconds
#define _CFG_SNTP_RCV_TMO       20    

/* YOU MIGHT WANT TO CHANGE THE FOLLOWING */
/* difference in hours of local time to Greenwich Mean Time */
/* NOTE: -5 = EST */
#define _CFG_GMT_DIFF           0    

/* maximum number of SNTP/NTP servers which can be passed to 
   xn_ebs_set_time_srv_list() */
#define CFG_SNTP_NUM_SERVERS    5

#endif /* __SNTPCONF__ */