////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AT91__USB_H_1
#define _AT91__USB_H_1   1


// USB driver
//

#include <..\pal\com\usb\USB.h>

struct AT91_USB_Driver
{
#if defined(PLATFORM_ARM_SAM7S256)
    // for SAM7S256 platform we only allow 3 endpoint: control, in and out
    static const UINT32 c_Used_Endpoints = 3;
#else
    static const UINT32 c_Used_Endpoints = 6;
#endif
    static const UINT32 c_default_ctrl_packet_size = 8;

    USB_CONTROLLER_STATE* pUsbControllerState;
    
#if defined(USB_REMOTE_WAKEUP)
    #define USB_MAX_REMOTE_WKUP_RETRY 5

    HAL_COMPLETION RemoteWKUP10msCompletion;
    HAL_COMPLETION RemoteWKUP100msEOPCompletion;
    UINT32         RemoteWkUpRetry;
#endif

    UINT8  ControlPacketBuffer[c_default_ctrl_packet_size];
    UINT16 EndpointStatus[c_Used_Endpoints];
    BOOL   TxRunning [USB_MAX_QUEUES];
    BOOL   TxNeedZLPS[USB_MAX_QUEUES];

    UINT8  PreviousDeviceState;
    UINT8  RxExpectedToggle[USB_MAX_QUEUES];
    BOOL   PinsProtected;
    BOOL   FirstDescriptorPacket;

#if defined(USB_METRIC_COUNTING)
    USB_PERFORMANCE_METRICS PerfMetrics;
#endif

    static UINT32    MAX_EP;

    //--//

    struct UDP_EPATTRIBUTE
    {
        UINT16 Dir_Type;
        UINT16 Payload;
        BOOL   DualBank;
        UINT32 dFlag;
    };
    
    static UDP_EPATTRIBUTE s_EpAttr[];

    //--//

    static USB_CONTROLLER_STATE * GetState( int Controller );

    static HRESULT Initialize( int Controller );

    static HRESULT Uninitialize( int Controller );

    static BOOL StartOutput( USB_CONTROLLER_STATE* State, int endpoint );

    static BOOL RxEnable( USB_CONTROLLER_STATE* State, int endpoint );

    static BOOL GetInterruptState();

    static BOOL ProtectPins( int Controller, BOOL On );

private:
    static void ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint );

    static void StartHardware();

    static void StopHardware ();

    static void TxPacket( USB_CONTROLLER_STATE* State, int endpoint );

    static void ControlNext();

    static void SuspendEvent();
    static void ResumeEvent ();
    static void ResetEvent  ();

    static void Global_ISR    ( void* Param );

    static void Endpoint_ISR    (UINT32 endpoint);

    static UINT32 PORT_TO_EP(UINT32 PortNo){ return (PortNo*2 +1);};
    static UINT32 EP_TO_PORT(UINT32 EP){ return (EP-1)>>2;};    

#if defined(USB_REMOTE_WAKEUP)
    static void RemoteWkUp_ISR    ( void* Param );
    static void RemoteWkUp_Process( void* Param );
#endif

    static void VBus_ISR    (GPIO_PIN Pin, BOOL PinState, void* Param);

};

extern AT91_USB_Driver g_AT91_USB_Driver;

struct AT91_USB_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    static LPCSTR GetDriverName() { return "AT91_USB"; }
};

extern AT91_USB_CONFIG* const g_pAT91_USB_Config;

//
// USB Driver
///////////////////////////////////////////////////////////////////////////////

#endif //_AT91__USB_H_1

