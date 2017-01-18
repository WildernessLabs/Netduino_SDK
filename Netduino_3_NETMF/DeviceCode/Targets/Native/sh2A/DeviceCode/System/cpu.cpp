////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include <tinyhal.h>
#include <tinyhal.h>
 
/////////////////////////////////////////////////////////////////////

void InitCRuntime()
{
}

/////////////////////////////////////////////////////////////////////

void SystemState_Set  ( SYSTEM_STATE State )
{
}
void SystemState_Clear( SYSTEM_STATE State )
{
}
void SystemState_SetNoLock  ( SYSTEM_STATE State )
{
}
void SystemState_ClearNoLock( SYSTEM_STATE State )
{
}
BOOL SystemState_QueryNoLock( SYSTEM_STATE State )
{
    return FALSE;
}

void HAL_EnterBooterMode()
{
}

void HAL_UnReserveAllGpios()
{
    for(INT32 i = CPU_GPIO_GetPinCount()-1; i >=0; i--)
    {
        CPU_GPIO_ReservePin((GPIO_PIN)i, false);
    }
}

bool g_fDoNotUninitializeDebuggerPort = false;

void HAL_Initialize()
{
    HAL_CONTINUATION::InitializeList();
    HAL_COMPLETION  ::InitializeList();

    Events_Initialize();

    CPU_GPIO_Initialize();

    CPU_SPI_Initialize();

    HAL_Time_Initialize();

    Time_Initialize();

    ENABLE_INTERRUPTS();

    BlockStorageList::Initialize();

    BlockStorage_AddDevices();

    BlockStorageList::InitializeDevices();

    FS_Initialize();

    FileSystemVolumeList::Initialize();

    FS_AddVolumes();

    FileSystemVolumeList::InitializeVolumes();

    CPU_InitializeCommunication();

    LCD_Initialize();

    I2C_Initialize();

    PalEvent_Initialize();

    Gesture_Initialize();

    Ink_Initialize();

    TimeService_Initialize();
    

/*
    other drivers init
*/
}

static ON_SOFT_REBOOT_HANDLER s_rebootHandlers[5] = {NULL, NULL, NULL, NULL, NULL};

void HAL_AddSoftRebootHandler(ON_SOFT_REBOOT_HANDLER handler)
{
    for(int i=0; i<ARRAYSIZE(s_rebootHandlers); i++)
    {
        if(s_rebootHandlers[i] == NULL)
        {
            s_rebootHandlers[i] = handler;
            return;
        }
        else if(s_rebootHandlers[i] == handler)
        {
            return;
        }
    }
}


void HAL_Uninitialize()
{
/* 
    other driver uninit
*/
    int i;

    for(i=0; i<ARRAYSIZE(s_rebootHandlers); i++)
    {
        if(s_rebootHandlers[i] != NULL)
        {
            s_rebootHandlers[i]();
            return;
        }
    }    

    LCD_Uninitialize();
    
    I2C_Uninitialize();

    TimeService_UnInitialize();

    Ink_Uninitialize();

    Gesture_Uninitialize();

    PalEvent_Uninitialize();

    SOCKETS_CloseConnections();

    CPU_UninitializeCommunication();

    FileSystemVolumeList::UninitializeVolumes();

    BlockStorageList::UnInitializeDevices();

    USART_CloseAllPorts();

    CPU_SPI_Uninitialize();

    HAL_UnReserveAllGpios();

    CPU_GPIO_Uninitialize();

    Time_Uninitialize();

    HAL_Time_Uninitialize();

    DISABLE_INTERRUPTS();
    
    Events_Uninitialize();

    HAL_CONTINUATION::Uninitialize();
    HAL_COMPLETION  ::Uninitialize();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void HARD_Breakpoint()
{
    return;
}
/////////////////////////////////////////////////////////////////////




