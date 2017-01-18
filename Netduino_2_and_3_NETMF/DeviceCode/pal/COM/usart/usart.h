////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
// Portions Copyright (c) Secret Labs LLC.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _USART_H_
#define _USART_H_ 1

//--//
#include "Tinyhal.h"

struct USART_Driver
{
    static BOOL Initialize  ( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue );
    static BOOL Uninitialize( int ComPortNum                              );

    static int  Write( int ComPortNum, const char* Data, size_t size );
    static int  Read ( int ComPortNum, char*       Data, size_t size );
    static BOOL Flush( int ComPortNum                                );

    static BOOL AddCharToRxBuffer     ( int ComPortNum, char  c     );
    static BOOL RemoveCharFromTxBuffer( int ComPortNum, char& c     );
    static INT8 PowerSave             ( int ComPortNum, INT8 Enable );

    static void PrepareForClockStop();
    static void ClockStopFinished();

    static void SendXOFF( int ComPortNum, const UINT32 Flag );
    static void SendXON ( int ComPortNum, const UINT32 Flag );

    static void CloseAllPorts();

    static int  BytesInBuffer( int ComPortNum, BOOL fRx );
    static void DiscardBuffer( int ComPortNum, BOOL fRx );

#if defined(PLATFORM_ARM_Netduino2) || defined(PLATFORM_ARM_NetduinoPlus2) || defined(PLATFORM_ARM_NetduinoGo) || defined(PLATFORM_ARM_NetduinoShieldBase) || defined(PLATFORM_ARM_Netduino3) || defined(PLATFORM_ARM_Netduino3Ethernet) || defined(PLATFORM_ARM_Netduino3Wifi)
    static void SetDataEventRaised( int ComPortNum );
#endif

    static BOOL ConnectEventSink( int ComPortNum, int EventType, void *pContext, PFNUsartEvent pfnUsartEvtHandler, void **ppArg );
    static void SetEvent( int ComPortNum, unsigned int event );

};

#endif /* _H_USB_ */
