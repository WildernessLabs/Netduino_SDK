////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
//#include "..\Include\iodefine.h"

struct SH7264_TIMER_Driver
{
    static const UINT32 CLOCK_SOURCE_DIV8   = 0;
    static const UINT32 CLOCK_SOURCE_DIV32  = 1;
    static const UINT32 CLOCK_SOURCE_DIV128 = 2;
    static const UINT32 CLOCK_SOURCE_DIV512 = 3;

    static const UINT16 c_MaxTimerValue = 0xFFFF;

    static const UINT32 c_Timer_System  = 0;
    static const UINT32 c_Timer_Compare = 1;
    
    static BOOL Initialize( UINT32 timer, UINT32 clkSource, HAL_CALLBACK_FPN ISR, void* ISR_Param );
    static BOOL Uninitialize( UINT32 timer );
    static UINT16 ReadCompare( UINT32 timer );
    static UINT16 ReadCounter( UINT32 timer );
    static void ForceInterrupt( UINT32 timer );
    static void SetCompare( UINT32 timer, UINT16 val );

    struct TimerISR
    {
        HAL_CALLBACK_FPN         ISR;
        void*                    Arg;
        BOOL                     Configured;
        volatile struct st_cmt0 *pCMT;
    } m_timers[2];
};

extern SH7264_TIMER_Driver g_SH7264_TIMER_Driver;

