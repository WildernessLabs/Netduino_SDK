////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CLRSTARTUP_H__
#define __CLRSTARTUP_H__

/////////////////////////////////////////////////////////////////////////////

#include <TinyCLR_Runtime.h>
#include <TinyCLR_Checks.h>
#include <TinyCLR_Diagnostics.h>
#include <TinyCLR_Graphics.h>
#include <TinyCLR_Hardware.h>
#include <TinyCLR_Application.h>

#include <TinyCLR_Version.h>

#include <CorLib_Native.h>
#include <SPOT_Native.h>
#include <SPOT_Graphics_Native.h>
#include <SPOT_Hardware_Native.h>
#include <SPOT_Net_Native.h>
#include <SPOT_Net_Security_Native.h>

/////////////////////////////////////////////////////////////////////////////

struct CLR_EmbeddedTB
{
    bool Initialize();
    static CLR_UINT32 GetMinutesLeft();
    
private:
    static const CLR_UINT32 c_MaxDWORDs              = 270;  // enough bits for 30 days in 5 minute increments (32 bits per dword)
    static const CLR_UINT32 c_MaxIntervals           = c_MaxDWORDs * 8 * sizeof(FLASH_WORD);
    static const CLR_UINT32 c_CompletionInterval_sec = 5*60; // 5 min per bit interval

    HAL_COMPLETION m_completion;
    CLR_UINT32     m_bitsFlipped;
    static UINT32  m_configPhysicalAddr;

    static void OnUpdateTB( void *arg );
    static bool UpdateBitField( CLR_UINT32& bitsUsed );
    static bool FindIndex( CLR_UINT32& index, CLR_UINT8& offset, CLR_UINT32& totalIncrementsUsed );
};

/////////////////////////////////////////////////////////////////////////////

#endif /* __CLRSTARTUP_H__ */
