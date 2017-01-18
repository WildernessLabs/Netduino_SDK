//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
//-----------------------------------------------------------------------------


#include "DAC.h"
#include "DAC_NXP_Interop_DAC.h"

using namespace NXP::Interop;

extern LPC24XX_DAC_Driver g_LPC24XX_DAC_Driver;

void DAC::Initialize( CLR_RT_HeapBlock* pMngObj, INT16 outputFrequency, HRESULT &hr )
{
	g_LPC24XX_DAC_Driver.Initialize(outputFrequency);
}

void DAC::Uninitialize( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	g_LPC24XX_DAC_Driver.Uninitialize();
}

void DAC::On( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if(!g_LPC24XX_DAC_Driver.On())
		hr = CLR_E_INVALID_OPERATION;
}

void DAC::Off( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if(!g_LPC24XX_DAC_Driver.Off())
		hr = CLR_E_INVALID_OPERATION;
}

INT8 DAC::get_Enabled( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_LPC24XX_DAC_Driver.IsEnabled();
}

void DAC::set_Enabled( CLR_RT_HeapBlock* pMngObj, INT8 On, HRESULT &hr )
{
	if(On)
	{
		if(!g_LPC24XX_DAC_Driver.On())
		hr = CLR_E_INVALID_OPERATION;
	}
	else
	{
		if(!g_LPC24XX_DAC_Driver.Off())
		hr = CLR_E_INVALID_OPERATION;
	}
}

void DAC::AddFrame( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_INT16 Samples, INT32 SamplesNum, HRESULT &hr )
{
	if(!g_LPC24XX_DAC_Driver.AddFrame(/*short const * const */Samples.GetBuffer(), /*UINT32 const*/ SamplesNum))
	{
		if(g_LPC24XX_DAC_Driver.SampleTimeInCycles == 0)
			hr = CLR_E_INVALID_OPERATION;
		else
			hr = CLR_E_OUT_OF_MEMORY;
	}
}

void DAC::AddFrame( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 Samples, INT32 SamplesNum, HRESULT &hr )
{
	if(!g_LPC24XX_DAC_Driver.AddFrame(/*short const * const */(short*)Samples.GetBuffer(), /*UINT32 const*/ SamplesNum))
	{
		if(g_LPC24XX_DAC_Driver.SampleTimeInCycles == 0)
			hr = CLR_E_INVALID_OPERATION;
		else
			hr = CLR_E_OUT_OF_MEMORY;
	}
}

INT32 DAC::GetBufferLevel( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_LPC24XX_DAC_Driver.GetBufferLevel();
}

INT32 DAC::GetFramesLeft( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
    return g_LPC24XX_DAC_Driver.GetFramesLeft();
}

UINT32 DAC::GetBufferFrameCapacity( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	return g_LPC24XX_DAC_Driver.GetBufferFrameCapacity();
}

