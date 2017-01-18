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

#include <tinyhal.h>

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_ADS7843Config"
#endif

#define TOUCH_POINT_BUFFER_SIZE            200
#define TOUCH_POINT_RUNNINGAVG_BUFFER_SIZE 4

TOUCH_SPI_CONFIGURATION g_ADS7843_Config = { {AT91_GPIO_Driver::PA28, false, false, false, false, 2000, 1, 1, AT91_SPI_Driver::SPI_0, { AT91_GPIO_Driver::PA11, TRUE }}, AT91_GPIO_Driver::PC2};
TOUCH_PANEL_CalibrationData g_TouchPanel_Calibration_Config = {-192, -256, 48000, 64000, -2800, -2800};
TOUCH_PANEL_SamplingSettings g_TouchPanel_Sampling_Settings = {2, 2, 80*80, FALSE, {50, 200, 100, TOUCH_PANEL_SAMPLE_MS_TO_TICKS(70)}};

TouchPoint g_PAL_TouchPointBuffer[TOUCH_POINT_BUFFER_SIZE];
UINT32     g_PAL_TouchPointBufferSize = TOUCH_POINT_BUFFER_SIZE;
TOUCH_PANEL_Point g_PAL_RunningAvg_Buffer[TOUCH_POINT_RUNNINGAVG_BUFFER_SIZE];
UINT32     g_PAL_RunningAvg_Buffer_Size = TOUCH_POINT_RUNNINGAVG_BUFFER_SIZE;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
