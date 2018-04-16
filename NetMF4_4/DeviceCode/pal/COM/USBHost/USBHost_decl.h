////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Oberon microsystems, Inc.
//
// USB Host Mode Declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRIVERS_USBH_DECL_H_
#define _DRIVERS_USBH_DECL_H_ 1

#include "USB_decl.h"

//--//

#ifndef USBH_MAX_QUEUES
#define USBH_MAX_QUEUES         16
#endif

//--//

// Endpoint error state
// Arguments to USBH_ControlCallback (result of transfer)
#define USBH_EP_OK              0
#define USBH_EP_STALL           1
#define USBH_EP_ERROR           2

// Return values from USBH_ControlCallback (next transfer)
#define USBH_TRANSFER_DONE      0
#define USBH_TRANSFER_IN        1
#define USBH_TRANSFER_OUT       2

//--//

// Special String IDs
#define USBH_ID_ManufacturerString   0x101
#define USBH_ID_ProductString        0x102
#define USBH_ID_SerialNumber         0x103

//--//

#define USBH_SETUP_COUNT          8  // length of setup packet

//--//

// configuration template
ADS_PACKED struct GNU_PACKED USB_DYNAMIC_CONFIGURATION {
    USB_DEVICE_DESCRIPTOR        device;
    USB_CONFIGURATION_DESCRIPTOR config;
    // more decriptors follow
};


struct USBH_CONTROLLER_STATE;

typedef void (*USBH_RESULT_CALLBACK)( int Controller, int result );

struct USBH_CONTROLLER_STATE
{
    /* ---------- These 5 entries must be the same as in USB_CONTROLLER_STATE ---------- */

    BOOL                                             Initialized;
    UINT8                                            CurrentState;
    UINT8                                            ControllerNum;
    UINT32                                           Event;

    const USB_DYNAMIC_CONFIGURATION*                 Configuration;
    
    /* ---------- Otherwise, USB_FindRecord & USB_NextEndpoint will not work ---------- */

    UINT8                                            Address;
    UINT8                                            DeviceState;
    UINT8                                            ConfigError;
    UINT16                                           ConfigSize;
    UINT16                                           ConfigOffs;
    
    /* Queues & MaxPacketSize must be initialized by the HAL */
    Hal_Queue_KnownSize<USB_PACKET64,USB_QUEUE_PACKET_COUNT> *Queues[USBH_MAX_QUEUES];
    UINT8                                            CurrentPacketOffset[USBH_MAX_QUEUES];
    UINT8                                            MaxPacketSize[USBH_MAX_QUEUES];
    BOOL                                             IsTxQueue[USBH_MAX_QUEUES];
    BOOL                                             IsRxActive[USBH_MAX_QUEUES];

    /* Arbitrarily as many streams as endpoints since that is the maximum number of streams
       necessary to represent the maximum number of endpoints */
    USB_STREAM_MAP                                   streams[USBH_MAX_QUEUES];

    //--//

    /* used for transferring packets between upper & lower */
    UINT8*                                           Data;
    UINT8                                            DataSize;
    
    /* USB status information, used in
       GET_STATUS, SET_FEATURE, CLEAR_FEATURE */
    UINT16                                           DeviceStatus;
    UINT16*                                          EndpointStatus;
    UINT8                                            EndpointCount;

    /* for helping out upper layer during callbacks */
    const USB_SETUP_PACKET*                          Request;
    UINT8                                            ControlError; // error repeat count
    UINT8*                                           ControlData;
    UINT16                                           ControlCount;
    USBH_RESULT_CALLBACK                             ResultCallback;
    volatile INT16                                   AsynchResult;

    /* delayed command processing */
    HAL_COMPLETION                                   timer;                            
};


struct USBH_DEVICE_STATE;

typedef void (*USBH_DEV_CALLBACK)( USBH_DEVICE_STATE* );

struct USBH_DEVICE_STATE
{
    USBH_CONTROLLER_STATE*                           Controller;

    const USB_DYNAMIC_CONFIGURATION*                 Configuration;

    /* USB hardware information */
    UINT8                                            Address;
    UINT8                                            DeviceState;
    UINT8                                            ConfigurationNum;

    UINT8                                            MaxPacketSize[USBH_MAX_QUEUES];
    
    /* used for transferring packets between upper & lower */
    UINT8*                                           Data;
    UINT8                                            DataSize;
    USB_SETUP_PACKET*                                Request;

    /* USB status information, used in
       GET_STATUS, SET_FEATURE, CLEAR_FEATURE */
    UINT16                                           DeviceStatus;
    UINT16*                                          EndpointStatus;
    UINT8                                            EndpointCount;


    /* callback function for getting next packet */
//    USBH_DEV_CALLBACK                                DataCallback;

    /* for helping out upper layer during callbacks */
    UINT8*                                           ResidualData;
    UINT16                                           ResidualCount;
    UINT16                                           Expected;
    USBH_DEV_CALLBACK                                ResultCallback;

};


struct USBH_STRING_DESC
{
    UINT8 bLength;                // length in bytes
    UINT8 bDescriptorType;        // = USB_STRING_DESCRIPTOR_TYPE (3)
    USB_STRING_CHAR string[126];  // Unicode, not zero terminated
};

//--//

int    USBH_GetControllerCount();
BOOL   USBH_Initialize  ( int Controller                               );
BOOL   USBH_Uninitialize( int Controller                               );
const USB_DYNAMIC_CONFIGURATION* USBH_GetConfiguration( int Controller );
BOOL   USBH_GetString   ( int Controller, int index, USBH_STRING_DESC* string );
BOOL   USBH_ControlRequest ( int Controller, const USB_SETUP_PACKET* request, BYTE* data, int length );

int    USBH_ErrorState  ( int Controller,      int ep                  );
BOOL   USBH_ResetError  ( int Controller,      int ep                  );

BOOL   USBH_OpenStream  ( int UsbStream,       int writeEP, int readEP );
BOOL   USBH_CloseStream ( int UsbStream                                );
int    USBH_Write       ( int UsbStream, const char* Data, size_t size );
int    USBH_Read        ( int UsbStream,       char* Data, size_t size );
BOOL   USBH_Flush       ( int UsbStream                                );
void   USBH_DiscardData ( int UsbStream,       BOOL fTx                );

UINT32 USBH_GetEvent    ( int Controller,      UINT32 Mask             );
UINT32 USBH_SetEvent    ( int Controller,      UINT32 Event            );
UINT32 USBH_ClearEvent  ( int Controller,      UINT32 Event            );
UINT8  USBH_GetStatus   ( int Controller                               );

/*
int USBH_AddStandardTarget( int Controller, int Driver, int class, int subclass, int protocol )
int USBH_AddVendorTarget( int Controller, int Driver, int vid, int pid )
void USBH_SetEventHandlers( int Controller, CALLBACK attached, CALLBACK detached);
*/

//--//

USBH_CONTROLLER_STATE *CPU_USBH_GetState ( int Controller          );
HRESULT        CPU_USBH_Initialize       ( int Controller          );
HRESULT        CPU_USBH_Uninitialize     ( int Controller          );
void           CPU_USBH_SetControl       ( USBH_CONTROLLER_STATE* State );
void           CPU_USBH_PortReset        ( USBH_CONTROLLER_STATE* State );
BOOL           CPU_USBH_StartSetup       ( USBH_CONTROLLER_STATE* State );
BOOL           CPU_USBH_StartOutput      ( USBH_CONTROLLER_STATE* State, int endpoint );
BOOL           CPU_USBH_RxEnable         ( USBH_CONTROLLER_STATE* State, int endpoint );
void           CPU_USBH_OpenChannel      ( USBH_CONTROLLER_STATE* State, const USB_ENDPOINT_DESCRIPTOR* pEp );
void           CPU_USBH_Suspend          ( USBH_CONTROLLER_STATE* State );
void           CPU_USBH_Resume           ( USBH_CONTROLLER_STATE* State );
BOOL           CPU_USBH_ProtectPins      ( int Controller, BOOL On );

//--//

#endif // _DRIVERS_USBH_DECL_H_

