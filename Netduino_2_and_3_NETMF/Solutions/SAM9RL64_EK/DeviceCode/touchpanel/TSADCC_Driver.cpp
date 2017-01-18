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
#include "TSADCC_Driver.h"

#define NO_PIN 4

extern UINT8 c_TSADCC[];

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
    #pragma arm section zidata = "g_AT91_TSADCC_Driver"
#endif
struct AT91_TSADCC_Driver g_AT91_TSADCC_Driver;

static UINT32 dwIfdown = 0;

INT32 SamplesX[SAMPLE_NB];
INT32 SamplesY[SAMPLE_NB];


#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
    #pragma arm section zidata
#endif

BOOL AT91_TSADCC_Driver::Initialize(GPIO_INTERRUPT_SERVICE_ROUTINE touchIsrProc)
{
	UINT32 i;
	UINT32 dwValue;
	struct AT91S_TSADC *pTSADC = (struct AT91S_TSADC *)AT91C_BASE_TSADCC;
	AT91_PMC &pmc = AT91::PMC();
	
	/* Selected as TSADCC pins */
    for(i = 0; i < NO_PIN; i++)
    {
        CPU_GPIO_DisablePin( c_TSADCC[i], RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
    }
	
	pmc.EnablePeriphClock(AT91C_ID_TSADCC);
	
	dwValue = (SYSTEM_PERIPHERAL_CLOCK_HZ/(ADC_CLOCK * 2)) - 1;
	if(dwValue > 0X3F)	// Saturation
		dwValue = 0x3F;
	
	//debug_printf("dwValue :%d\r\n", dwValue);
	pTSADC->TSADC_MR = (AT91C_TSADC_TSAMOD_TS_ONLY_MODE |
					   AT91C_TSADC_PENDET |
					   (dwValue << 8) |
					   AT91C_TSADC_SLEEP |
					   STARTUP_TIME << 16 |
					   PEN_DEBOUNCE_TIME << 28);
					   
	pTSADC->TSADC_TSR = SAMPLE_AND_HOLD_TIME << 24;
	
	pTSADC->TSADC_TRGR = AT91C_TSADC_TRGMOD_NO_TRIGGER;
	
	pTSADC->TSADC_IER = AT91C_TSADC_PENCNT;
	
	CPU_INTC_ActivateInterrupt( AT91C_ID_TSADCC, TSADCC_ISR, NULL);
	
	for (i = 0; i<SAMPLE_NB; i++)
	{
		g_AT91_TSADCC_Driver.m_stiX_Samples[i] = 0;
		g_AT91_TSADCC_Driver.m_stiY_Samples[i] = 0;
	}
	g_AT91_TSADCC_Driver.m_sdwSampleIndex = 0;
	g_AT91_TSADCC_Driver.m_sdwSampleNo = 0;
	g_AT91_TSADCC_Driver.m_siX_SampleSum = 0;
	g_AT91_TSADCC_Driver.m_siY_SampleSum = 0;
	g_AT91_TSADCC_Driver.m_TipState = 0;
	g_AT91_TSADCC_Driver.m_UnCalX = 0;
	g_AT91_TSADCC_Driver.m_UnCalY = 0;
	
	g_AT91_TSADCC_Driver.m_touchIsrProc = touchIsrProc;
	
	//debug_printf("AT91_TSADCC_Driver::Initialize called\r\n");
	return TRUE;
}

BOOL AT91_TSADCC_Driver::Uninitialize()
{
	struct AT91S_TSADC *pTSADC = (struct AT91S_TSADC *)AT91C_BASE_TSADCC;
	
	AT91_PMC &pmc = AT91::PMC();
    
	CPU_INTC_DeactivateInterrupt( AT91C_ID_TSADCC);
	
	pmc.EnablePeriphClock(AT91C_ID_TSADCC);    
	
	
	pTSADC->TSADC_MR = 0;
	
	//debug_printf("AT91_TSADCC_Driver::Uninitialize called\r\n");
	return TRUE;
}

BOOL AT91_TSADCC_Driver::CalibrationPointGet(TOUCH_PANEL_CALIBRATION_POINT* point)
{
	INT32   cDisplayWidth  = point->cDisplayWidth;
    INT32   cDisplayHeight = point->cDisplayHeight;

    int CalibrationRadiusX = cDisplayWidth  / 20;
    int CalibrationRadiusY = cDisplayHeight / 20;

	
	//debug_printf("AT91_TSADCC_Driver::CalibrationPointGet called\r\n");
    switch (point -> PointNumber)
    {
    case    0:
        point->CalibrationX = cDisplayWidth  / 2;
        point->CalibrationY = cDisplayHeight / 2;
        break;

    case    1:
        point->CalibrationX = CalibrationRadiusX * 2;
        point->CalibrationY = CalibrationRadiusY * 2;
        break;

    case    2:
        point->CalibrationX = CalibrationRadiusX * 2;
        point->CalibrationY = cDisplayHeight - CalibrationRadiusY * 2;
        break;

    case    3:
        point->CalibrationX = cDisplayWidth  - CalibrationRadiusX * 2;
        point->CalibrationY = cDisplayHeight - CalibrationRadiusY * 2;
        break;

    case    4:
        point->CalibrationX = cDisplayWidth - CalibrationRadiusX * 2;
        point->CalibrationY = CalibrationRadiusY * 2;
        break;

    default:
        point->CalibrationX = cDisplayWidth  / 2;
        point->CalibrationY = cDisplayHeight / 2;
        
        return FALSE;
    }
    
    return TRUE;
}

void AT91_TSADCC_Driver::GetPoint(TOUCH_PANEL_SAMPLE_FLAGS* pTipState, int* pSource, int* pUnCalX, int* pUnCalY)
{
					
	INT32 iX_Average;
	INT32 iY_Average;

	*pSource = 0;
	// Sum the samples
	if (dwIfdown == 0)
	{
		g_AT91_TSADCC_Driver.m_TipState = TouchSampleValidFlag;
		*pTipState = g_AT91_TSADCC_Driver.m_TipState;
		return;
	}
	
	if (g_AT91_TSADCC_Driver.m_sdwSampleNo < MIN_GOOD_SAMPLES)
	{
		g_AT91_TSADCC_Driver.m_TipState = TouchSampleIgnore | TouchSampleDownFlag;
		*pTipState = g_AT91_TSADCC_Driver.m_TipState;
		return;
	}
	g_AT91_TSADCC_Driver.m_siX_SampleSum = 0;
	g_AT91_TSADCC_Driver.m_siY_SampleSum = 0;
	
	for (int i=0;i<g_AT91_TSADCC_Driver.m_sdwSampleNo;i++)
	{
		g_AT91_TSADCC_Driver.m_siX_SampleSum += g_AT91_TSADCC_Driver.m_stiX_Samples[i];
		g_AT91_TSADCC_Driver.m_siY_SampleSum += g_AT91_TSADCC_Driver.m_stiY_Samples[i];
	}
				
	// Calculate average
	iX_Average = g_AT91_TSADCC_Driver.m_siX_SampleSum / g_AT91_TSADCC_Driver.m_sdwSampleNo;
	iY_Average = g_AT91_TSADCC_Driver.m_siY_SampleSum / g_AT91_TSADCC_Driver.m_sdwSampleNo;

	memcpy(SamplesX, g_AT91_TSADCC_Driver.m_stiX_Samples, g_AT91_TSADCC_Driver.m_sdwSampleNo * sizeof(&SamplesX[0]));
	memcpy(SamplesY, g_AT91_TSADCC_Driver.m_stiY_Samples, g_AT91_TSADCC_Driver.m_sdwSampleNo * sizeof(&SamplesY[0]));
	// if filtering sample lists are ok ?
	if(	SamplesFilter(SamplesX, g_AT91_TSADCC_Driver.m_sdwSampleNo, g_AT91_TSADCC_Driver.m_sdwSampleNo/2, g_AT91_TSADCC_Driver.m_siX_SampleSum, &iX_Average)
		&&
		SamplesFilter(SamplesY, g_AT91_TSADCC_Driver.m_sdwSampleNo, g_AT91_TSADCC_Driver.m_sdwSampleNo/2, g_AT91_TSADCC_Driver.m_siY_SampleSum, &iY_Average)
		)
	{
						
		// Return averages with only good samples
		g_AT91_TSADCC_Driver.m_UnCalX = iX_Average;
		g_AT91_TSADCC_Driver.m_UnCalY = iY_Average;

		// Set flags
		if (dwIfdown == 1)
		{
			g_AT91_TSADCC_Driver.m_TipState = (TouchSampleValidFlag | TouchSampleDownFlag);
			dwIfdown = 2;
			
		}
		else
		{
			g_AT91_TSADCC_Driver.m_TipState = (TouchSampleValidFlag | TouchSamplePreviousDownFlag | TouchSampleDownFlag);
		}
						
	}
	else
	{
		// Set flags
		g_AT91_TSADCC_Driver.m_TipState = TouchSampleIgnore | TouchSampleDownFlag;
						
	}

	

	*pTipState = g_AT91_TSADCC_Driver.m_TipState;
	*pUnCalX = g_AT91_TSADCC_Driver.m_UnCalX;
	*pUnCalY = g_AT91_TSADCC_Driver.m_UnCalY;
	
}

HRESULT AT91_TSADCC_Driver::GetDeviceCaps(unsigned int iIndex, void* lpOutput)
{
	if ( lpOutput == NULL )
    {
        return FALSE;
    }
	
	switch ( iIndex )
    {
        case TOUCH_PANEL_SAMPLE_RATE_ID:
        {
            TOUCH_PANEL_SAMPLE_RATE  *pTSR  = (TOUCH_PANEL_SAMPLE_RATE*)lpOutput;    
            pTSR->SamplesPerSecondLow       = g_TouchPanel_Sampling_Settings.SampleRate.SamplesPerSecondLow;
            pTSR->SamplesPerSecondHigh      = g_TouchPanel_Sampling_Settings.SampleRate.SamplesPerSecondHigh;
            pTSR->CurrentSampleRateSetting  = g_TouchPanel_Sampling_Settings.SampleRate.CurrentSampleRateSetting;
            pTSR->MaxTimeForMoveEvent_ticks = g_TouchPanel_Sampling_Settings.SampleRate.MaxTimeForMoveEvent_ticks;
        }
		break;
	case TOUCH_PANEL_CALIBRATION_POINT_COUNT_ID:
        {
    		TOUCH_PANEL_CALIBRATION_POINT_COUNT *pTCPC = (TOUCH_PANEL_CALIBRATION_POINT_COUNT*)lpOutput;            

            pTCPC->flags              = 0;
            pTCPC->cCalibrationPoints = 5;
        }
		break;
	case TOUCH_PANEL_CALIBRATION_POINT_ID:
        {
		    return CalibrationPointGet((TOUCH_PANEL_CALIBRATION_POINT*) lpOutput);
        }
		break;
	default:
		return FALSE;
		break;
	}
	
	return TRUE;
}

BOOL AT91_TSADCC_Driver::SamplesFilter(INT32 tiSample[], const INT32 ciNbSamples, const INT32 ciMinNbGoodSamples, INT32 iSum, INT32 *piAverage)
{
	INT32 iDelta;
	INT32 iMaxDelta;
	INT32 iMaxDeltaIndex;
	BOOL bMaxDeltaFound;

	INT32 iMaxNbSamples = ciNbSamples;
	INT32 iAverage	  = *piAverage;

	// Filter aberrant values in sample list
	for(INT32 j=0 ; j<iMaxNbSamples ; j++)
	{
		iMaxDelta		= 0;
		iMaxDeltaIndex	= 0;
		bMaxDeltaFound	= FALSE;

		// Check sample list for the most aberrant value
		for(INT32 i=0 ; i<iMaxNbSamples ; i++)
		{
			// Calculate delta between sample and average
			iDelta = abs(tiSample[i] - iAverage);
			
			// Delta out of range ?
			if(iDelta > MAX_DELTA)
			{
				// New max delta ?
				if(iDelta > iMaxDelta)
				{
					iMaxDelta		= iDelta;	// Save value of aberrant sample
					iMaxDeltaIndex	= i;		// Save index of aberrant sample
					bMaxDeltaFound	= TRUE;
				}
			}
		}
		
		// Aberrant found ?
		if(bMaxDeltaFound)
		{
			// Decrement number of samples
			iMaxNbSamples--;

			// Remove abberant value from sum
			iSum -= tiSample[iMaxDeltaIndex];

			// Calculate new average
			iAverage = iSum / iMaxNbSamples;

			// Zap samples list if necessary
			if(iMaxDeltaIndex < iMaxNbSamples)
				memcpy(&tiSample[iMaxDeltaIndex], &tiSample[iMaxDeltaIndex + 1], (iMaxNbSamples-iMaxDeltaIndex) * sizeof(&tiSample[0]));
		}
		else
			break;	// No more aberrant !
			
		if (iMaxNbSamples < ciMinNbGoodSamples)
		{
			break;
		}
	}

	// Too bad samples ?
	if(iMaxNbSamples < ciMinNbGoodSamples)
		return FALSE;

	// Enough good samples !
	// Average with only good samples
	*piAverage = iAverage;

	return TRUE;
	
}

INT32 AT91_TSADCC_Driver::abs(INT32 input)
{
	INT32 dwRet;
	
	if (input >= 0)
	{
		dwRet = input;
	}
	else
	{
		dwRet = -input;
	}
	return dwRet;
}

void AT91_TSADCC_Driver::TSADCC_ISR(void* Param)
{
	UINT32 dwStatus;
	UINT32 dwInterruptPending;
	volatile struct AT91S_TSADC *pTSADC = (struct AT91S_TSADC *)AT91C_BASE_TSADCC;
	
//	static UINT32 debug_count = 0;
//	g_AT91_TSADCC_Driver.m_UnCalX = 0;
//    g_AT91_TSADCC_Driver.m_UnCalY = 0;
	
	// Read status and determine pending interrupt
	dwStatus = pTSADC->TSADC_SR;
	dwInterruptPending = dwStatus & pTSADC->TSADC_IMR;
//	//debug_printf("dwStatus: %x, dwInterruptPending: %x\r\n", dwStatus, dwInterruptPending);

//	debug_count ++;
	
	if(dwInterruptPending & AT91C_TSADC_NOCNT)
	{
		//debug_printf("AT91C_TSADC_NOCNT\r\n");
		// Disable End Of Conversion (X,Y) and NO Pen Contact interrupt
		pTSADC->TSADC_IDR = AT91C_TSADC_EOC3 | AT91C_TSADC_NOCNT;

		// Enable Pen Contact interrupt
		pTSADC->TSADC_IER = AT91C_TSADC_PENCNT;

		// Set trigger mode to none
		pTSADC->TSADC_TRGR = AT91C_TSADC_TRGMOD_NO_TRIGGER;

		// Set flags
		g_AT91_TSADCC_Driver.m_TipState = TouchSampleValidFlag;
		
		dwIfdown = 0;
		
		if (g_AT91_TSADCC_Driver.m_touchIsrProc != NULL)
		{
			g_AT91_TSADCC_Driver.m_touchIsrProc(GPIO_PIN_NONE, TRUE, NULL);
		}
	
	}
	else 
	{
		// Pen Contact interrupt (Pen Down) ?
		if(dwInterruptPending & AT91C_TSADC_PENCNT)
		{
			//debug_printf("AT91C_TSADC_PENCNT\r\n");
			// Disable Pen Contact interrupt
			pTSADC->TSADC_IDR = AT91C_TSADC_PENCNT;

			// Enable End Of Conversion (X,Y) and NO Pen Contact interrupt
			pTSADC->TSADC_IER = AT91C_TSADC_EOC3 | AT91C_TSADC_NOCNT;

			// Set trigger mode to periodic
			pTSADC->TSADC_TRGR = AT91C_TSADC_TRGMOD_PERIODIC_TRIGGER | (((ADC_CLOCK/(g_TouchPanel_Sampling_Settings.SampleRate.SamplesPerSecondLow*SAMPLE_NB)) - 1) << 16);
//			pTSADC->TSADC_TRGR = AT91C_TSADC_TRGMOD_PERIODIC_TRIGGER | ((0x3E7) << 16);
			
			//debug_printf("trigger: %x\r\n", (((ADC_CLOCK/(g_TouchPanel_Sampling_Settings.SampleRate.SamplesPerSecondLow*SAMPLE_NB)) - 1)));
			// Set flags
			g_AT91_TSADCC_Driver.m_TipState = TouchSampleIgnore | TouchSampleDownFlag; // Ignore because conversion is no yet available
			
			g_AT91_TSADCC_Driver.m_sdwSampleNo = 0;
			g_AT91_TSADCC_Driver.m_sdwSampleIndex = 0;
			g_AT91_TSADCC_Driver.m_siX_SampleSum = 0;
			g_AT91_TSADCC_Driver.m_siY_SampleSum = 0;
			g_AT91_TSADCC_Driver.m_UnCalX = 0;
			g_AT91_TSADCC_Driver.m_UnCalY = 0;
			
			dwIfdown = 1;
			if (g_AT91_TSADCC_Driver.m_touchIsrProc != NULL)
			{
				g_AT91_TSADCC_Driver.m_touchIsrProc(GPIO_PIN_NONE, FALSE, NULL);
			}
		}
		else
		{
			// End Of Conversion (X,Y) interrupt ?
			if(dwInterruptPending & AT91C_TSADC_EOC3)
			{
	//			//debug_printf("AT91C_TSADC_EOC3, mask: %x\r\n", dwInterruptPending);
				INT32 iX_Current = (pTSADC->TSADC_CDR1 << 10) / pTSADC->TSADC_CDR0;
				INT32 iY_Current = (pTSADC->TSADC_CDR3 << 10) / pTSADC->TSADC_CDR2;
				
				g_AT91_TSADCC_Driver.m_stiX_Samples[g_AT91_TSADCC_Driver.m_sdwSampleIndex] = iX_Current;
				g_AT91_TSADCC_Driver.m_stiY_Samples[g_AT91_TSADCC_Driver.m_sdwSampleIndex] = iY_Current;
				
				// Increment index
				g_AT91_TSADCC_Driver.m_sdwSampleIndex++;
				if (g_AT91_TSADCC_Driver.m_sdwSampleIndex >= SAMPLE_NB)
				{
					g_AT91_TSADCC_Driver.m_sdwSampleIndex = 0;
				}
				
				if (g_AT91_TSADCC_Driver.m_touchIsrProc != NULL && g_AT91_TSADCC_Driver.m_sdwSampleNo == (SAMPLE_NB - 1))
				{
	//				g_AT91_TSADCC_Driver.m_touchIsrProc(GPIO_PIN_NONE, TRUE, NULL);
				}
				if (g_AT91_TSADCC_Driver.m_sdwSampleNo < SAMPLE_NB)
				{
					g_AT91_TSADCC_Driver.m_sdwSampleNo++;
				}
				
				
				
				
			}
		}
	}

}

//--//

