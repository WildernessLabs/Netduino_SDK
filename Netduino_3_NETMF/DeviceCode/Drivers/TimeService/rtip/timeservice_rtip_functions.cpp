////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"
#include "sntpapi.h"

//--//

INT32 HAL_TIMESERVICE_GetTimeFromSNTPServer(UINT8* serverIP, SYSTEMTIME* systemTime)
{
    EBS_SYS_TIME ebs_time;
    int r = xn_sntp_get_time(&ebs_time, serverIP, SNTP_VERSION_3);

    systemTime->wYear = ebs_time.year;
    systemTime->wMonth = ebs_time.month;
    systemTime->wDay = ebs_time.day;
    systemTime->wHour = ebs_time.hour;
    systemTime->wMinute = ebs_time.minute;
    systemTime->wSecond = ebs_time.second;
    systemTime->wMilliseconds = ebs_time.msec;

    return r;
}

INT32 HAL_TIMESERVICE_GetTimeFromSNTPServerList(UINT8* serverIP, INT32 serverNum, SYSTEMTIME* systemTime)
{
    EBS_SYS_TIME ebs_time;
    int r = xn_sntp_get_time_srv_list(&ebs_time, serverIP, serverNum, SNTP_VERSION_3);

    systemTime->wYear = ebs_time.year;
    systemTime->wMonth = ebs_time.month;
    systemTime->wDay = ebs_time.day;
    systemTime->wHour = ebs_time.hour;
    systemTime->wMinute = ebs_time.minute;
    systemTime->wSecond = ebs_time.second;
    systemTime->wMilliseconds = ebs_time.msec;

    return r;
}