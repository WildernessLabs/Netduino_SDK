////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include "tinyhal.h"
#include "..\..\vdc3\sh7264_vdc3.h"

#ifndef _DRIVERS_DISPLAY_HD66773R_H_
#define _DRIVERS_DISPLAY_HD66773R_H_ 1

struct SH7264VDC3_SERIALBUS_CONFIG
{
    GPIO_FLAG EnablePin;  // HD66773R_LCD_ON_PORT, 1 );
    GPIO_FLAG ResetPin;   // HD66773R_LRST_PORT  , 0 );
    GPIO_FLAG LoadPin;    // HD66773R_LOAD_PORT  , 1 );
    GPIO_FLAG SclPin;     // HD66773R_SCL_PORT   , 0 );
    GPIO_FLAG SdaPin;     // HD66773R_SDA_PORT   , 0 );

    UINT32 StartupTime;   // #define HD66773R_TIME_POWER_STABLE     10000
    UINT32 SdaSetupTime;  // #define HD66773R_TIME_SDA_SETUP        2          // Time after presenting data until rising clock edge.
    UINT32 SdaHoldTime;   // #define HD66773R_TIME_SDA_HOLD         2          // Time after rising clock edge to data change.
    UINT32 CmdholdTime;   // #define HD66773R_TIME_MIN_CMD_HOLD_OFF 2          // Minimum spacing between commands on serial bus.
    UINT32 LoadSetupTime; // #define HD66773R_TIME_LOAD_SETUP       2          // Time between asserting LOAD and first bit.
    UINT32 LoadHoldTime;  // #define HD66773R_TIME_LOAD_HOLD        2
};

struct SH7264VDC3_Driver
{
    UINT32 m_cursor;

    static BOOL Initialize();
    static BOOL Uninitialize();
    static void PowerSave( BOOL On );
    static void Clear();
    static void BitBltEx( int x, int y, int width, int height, UINT32 data[] );
    static void BitBlt( int width, int height, int widthInWords, UINT32 data[], BOOL fUseDelta );
    static void WriteChar( unsigned char c, int row, int col );
    static void WriteFormattedChar( unsigned char c );

private:
    static void WriteRawByte( UINT8 data );
    static void WriteCmdByte( UINT8 addr, UINT8 data );
    static UINT32 PixelsPerWord();
    static UINT32 TextRows();
    static UINT32 TextColumns();
    static UINT32 WidthInWords();
    static UINT32 SizeInWords();
    static UINT32 SizeInBytes();
};

extern SH7264VDC3_Driver g_SH7264VDC3_Driver;
extern SH7264VDC3_CONTROLLER_CONFIG g_SH7264VDC3_Config;

#endif  // _DRIVERS_DISPLAY_HD66773R_H_
