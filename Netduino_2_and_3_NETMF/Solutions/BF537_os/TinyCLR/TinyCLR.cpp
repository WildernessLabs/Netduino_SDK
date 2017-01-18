////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyclr_application.h"
#include "tinyhal.h"

void ApplicationEntryPoint()
{
    CLR_SETTINGS clrSettings;

    InitCRuntime();

    CPU_Initialize();

    Time_Initialize();

    HAL_Initialize();
    
    // CLR entry point 
    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

    ClrStartup( clrSettings );

    HAL_Uninitialize();

    Time_Uninitialize();

#if !defined(BUILD_RTM)
    debug_printf( "Exiting.\r\n" );
    CPU_Halt();
#else
    CPU_Reset();
#endif

}

BOOL Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}

