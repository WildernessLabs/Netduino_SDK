 /*
 | RTPTIME.C - Runtime Platform Timing Services
 |
 |   PORTED TO THE EBSnet Inc RTIP TCP/IP PLATFORM
 |
 | EBSnet - RT-Platform
 |
 |  $Author: yohannes $
 |  $Date: 2005/06/14 16:06:52 $
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



/************************************************************************
* Headers
************************************************************************/
#include "rtp.h"
#include "rtptime.h"
#include "rtpdate.h"
#include <tinyhal.h>
#include "tinyhal_types.h"

/************************************************************************
* Defines
************************************************************************/

/************************************************************************
* Types
************************************************************************/

/************************************************************************
* Data
************************************************************************/

/************************************************************************
* Macros
************************************************************************/

/************************************************************************
* Function Prototypes
************************************************************************/
unsigned long rtp_get_ticks_p_sec (void);

/************************************************************************
* Function Bodies
************************************************************************/

/*----------------------------------------------------------------------*
                         rtp_get_system_msec
 *----------------------------------------------------------------------*/
unsigned long rtp_get_system_msec (void)
{
    NATIVE_PROFILE_PAL_NETWORK();
    UINT64  currentTime;
    
    /* JRT */
    currentTime = HAL_Time_CurrentTime() / 10000;
    
    return ((unsigned long)(currentTime));
    
}


/*----------------------------------------------------------------------*
                         rtp_get_system_sec
 *----------------------------------------------------------------------*/
unsigned long rtp_get_system_sec ()
{
    NATIVE_PROFILE_PAL_NETWORK();
    
    return((unsigned long) (rtp_get_system_msec() /1000));  
}

/*----------------------------------------------------------------------*
                         rtp_get_system_sec
 *----------------------------------------------------------------------*/
unsigned long rtp_get_ticks_p_sec ()
{
    NATIVE_PROFILE_PAL_NETWORK();
    return CPU_TicksPerSecond();
}

int rtp_get_gmt_date (RTP_DATE * gmDate, unsigned long utcSec)
{
    NATIVE_PROFILE_PAL_NETWORK();

    SSL_GetTime( (DATE_TIME_INFO*) gmDate );
    
    return (1);
}

/* ----------------------------------- */
/*             END OF FILE             */
/* ----------------------------------- */

