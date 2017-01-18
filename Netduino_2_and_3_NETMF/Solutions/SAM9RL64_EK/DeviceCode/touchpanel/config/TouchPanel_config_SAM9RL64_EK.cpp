////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
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
#pragma arm section rwdata = "g_TSC2046Config"
#endif

#define TOUCH_POINT_BUFFER_SIZE            200
#define TOUCH_POINT_RUNNINGAVG_BUFFER_SIZE 4

UINT8 c_TSADCC[] = {AT91_GPIO_Driver::PA17, AT91_GPIO_Driver::PA18, AT91_GPIO_Driver::PA19, AT91_GPIO_Driver::PA20};
//TOUCH_PANEL_CalibrationData g_TSC2046_Calibration_Config = {-256, -192, 390400, 319296, -1984, -1920};
//TOUCH_PANEL_CalibrationData g_TouchPanel_Calibration_Config = {1, 1, 0, 0, 1, 1};
TOUCH_PANEL_CalibrationData g_TouchPanel_Calibration_Config = {-192, -256, 14448, 252000, -710, 763};
TOUCH_PANEL_SamplingSettings g_TouchPanel_Sampling_Settings = {1, 2, 400, FALSE, {50, 200, 150, TOUCH_PANEL_SAMPLE_MS_TO_TICKS(50)}};

TouchPoint g_PAL_TouchPointBuffer[TOUCH_POINT_BUFFER_SIZE];
UINT32     g_PAL_TouchPointBufferSize = TOUCH_POINT_BUFFER_SIZE;
TOUCH_PANEL_Point g_PAL_RunningAvg_Buffer[TOUCH_POINT_RUNNINGAVG_BUFFER_SIZE];
UINT32     g_PAL_RunningAvg_Buffer_Size = TOUCH_POINT_RUNNINGAVG_BUFFER_SIZE;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
