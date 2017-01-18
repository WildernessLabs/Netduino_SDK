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

#include "tinyhal.h"
#include "TSADCC_Driver.h"
//--//

BOOL    TOUCH_PANEL_Driver_Enable(GPIO_INTERRUPT_SERVICE_ROUTINE touchIsrProc)
{
    return AT91_TSADCC_Driver::Initialize(touchIsrProc);
}

BOOL    TOUCH_PANEL_Driver_Disable()
{
    return AT91_TSADCC_Driver::Uninitialize();
}

void    TOUCH_PANEL_Driver_GetPoint( TOUCH_PANEL_SAMPLE_FLAGS* pTipState, int* pSource, int* pUnCalX, int* pUnCalY)
{
//	AT91_TSADCC_Driver::GetPoint(pTipState, pUnCalX, pUnCalY);
AT91_TSADCC_Driver::GetPoint(pTipState, pSource, pUnCalY, pUnCalX);
}

HRESULT TOUCH_PANEL_Driver_GetDeviceCaps(unsigned int iIndex, void* lpOutput)
{
    return AT91_TSADCC_Driver::GetDeviceCaps(iIndex, lpOutput);
    // return E_NOTIMPL;
}

BOOL HAL_TOUCH_PANEL_Enable( GPIO_INTERRUPT_SERVICE_ROUTINE touchIsrProc )
{
	return TOUCH_PANEL_Driver_Enable(touchIsrProc);
}
BOOL HAL_TOUCH_PANEL_Disable ()
{
	return TOUCH_PANEL_Driver_Disable();
}
void HAL_TOUCH_PANEL_GetPoint (
         TOUCH_PANEL_SAMPLE_FLAGS* pTipState,
		 int* pSource,
         int* pUnCalX,
         int* pUnCalY
)
{
	TOUCH_PANEL_Driver_GetPoint(pTipState, pSource, pUnCalX, pUnCalY);
}
HRESULT HAL_TOUCH_PANEL_GetDeviceCaps (
         unsigned int iIndex,
         void* lpOutput
)
{
	return TOUCH_PANEL_Driver_GetDeviceCaps(iIndex, lpOutput);
}
