////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Oberon microsystems, Inc.
//
// USB Host Mode PAL Declarations
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PAL_USBH_H_
#define _PAL_USBH_H_ 1

//--//

#include "Tinyhal.h"
#include "USBHost_decl.h"
#include <pal\com\usb\USB.h>


//--//

extern USB_PACKET64* USBH_RxEnqueue( USBH_CONTROLLER_STATE* State, int queue, BOOL& DisableRx );
extern USB_PACKET64* USBH_TxDequeue( USBH_CONTROLLER_STATE* State, int queue, BOOL  Done      );

extern UINT8 USBH_ControlCallback  ( USBH_CONTROLLER_STATE* State, UINT8 result );
extern void  USBH_StateCallback    ( USBH_CONTROLLER_STATE* State );

//--//


void USBH_ClearQueues( USBH_CONTROLLER_STATE *State, BOOL ClrRxQueue, BOOL ClrTxQueue );

USB_PACKET64* USBH_RxEnqueue( int queue, BOOL& DisableRx );
USB_PACKET64* USBH_TxDequeue( int queue, BOOL  Done      );

UINT8 USBH_ControlCallback( USBH_CONTROLLER_STATE* State, UINT8 result );

void USBH_StateCallback(USBH_CONTROLLER_STATE *State);

BOOL USBH_AsynchRequest ( int Controller, const USB_SETUP_PACKET* request, BYTE* data, int length, USBH_RESULT_CALLBACK callback );


//--//

struct USBH_Driver
{
    static int  GetControllerCount();
    static BOOL Initialize  ( int Controller );
    static BOOL Uninitialize( int Controller );
    
    static const USB_DYNAMIC_CONFIGURATION * GetConfiguration( int Controller );
    static BOOL GetString   ( int Controller, int index, USBH_STRING_DESC* string );
    static BOOL ControlRequest ( int Controller, const USB_SETUP_PACKET* request, BYTE* data, int length );
    
    static int  ErrorState  ( int Controller, int ep );
    static BOOL ResetError  ( int Controller, int ep );

    static BOOL OpenStream  ( int stream, int writeEP, int readEP );
    static BOOL CloseStream ( int stream );

    static int  Write       ( int UsbStream, const char* Data, size_t size );
    static int  Read        ( int UsbStream, char*       Data, size_t size );
    static BOOL Flush       ( int UsbStream                                );
    static void DiscardData ( int UsbStream, BOOL fTx );

    static UINT32 GetEvent  ( int Controller, UINT32 Mask  );
    static UINT32 SetEvent  ( int Controller, UINT32 Event );
    static UINT32 ClearEvent( int Controller, UINT32 Event );
    static UINT8  GetStatus ( int Controller               );
};

//--//

#endif /* _PAL_USBH_H_ */
