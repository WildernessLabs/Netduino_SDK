////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include "..\Include\iodefine.h"

#ifndef _SH7264_TIME_H_1
#define _SH7264_TIME_H_1   1

//////////////////////////////////////////////////////////////////////////////
// SH7264_TIME_Driver
//
struct SH7264_TIME_Driver
{
    UINT64 m_lastRead;
    UINT64 m_nextCompare;
    

    static BOOL Initialize  ();
    static BOOL Uninitialize();
    static UINT64 CounterValue();
    static void SetCompareValue( UINT64 CompareValue );
    static INT64 TicksToTime( UINT64 Ticks );
    static INT64 CurrentTime();
    static void Sleep_uSec( UINT32 uSec );
    static void Sleep_uSec_Loop( UINT32 uSec );
    static void Compare_ISR( void* Param );
    static void Overflow_ISR( void* Param );
};

extern SH7264_TIME_Driver g_SH7264_TIME_Driver;
//
// SH7264_TIME_Driver
//////////////////////////////////////////////////////////////////////////////
#endif //_SH7264_TIME_H_1
