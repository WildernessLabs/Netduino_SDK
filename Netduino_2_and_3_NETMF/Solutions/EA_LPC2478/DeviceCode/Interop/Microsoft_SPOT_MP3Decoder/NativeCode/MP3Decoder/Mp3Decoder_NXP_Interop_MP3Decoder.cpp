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
// use without further testing or modification. 
//-----------------------------------------------------------------------------


#include "Mp3Decoder.h"
#include "Mp3Decoder_NXP_Interop_MP3Decoder.h"

#include "coder.h"

//#define DECODER_DBG 1
//#define DECODER_TIMING 1
//#define DAC_STOP_TIMING 1


#if defined(DECODER_TIMING) || defined(DAC_STOP_TIMING)
	#define TIMED_FRAMES 150
#endif

UINT16 framesDecoded = 0;

#if defined(DECODER_TIMING)
	UINT16 decodingTimesMs[TIMED_FRAMES];
	BOOL decTimingAlreadyOutput = false;
#endif
	
#if defined(DAC_STOP_TIMING)
	UINT16 stopTimesUs[TIMED_FRAMES];
	BOOL dacTimingAlreadyOutput = false;
#endif

using namespace NXP::Interop;

extern LPC24XX_DAC_Driver g_LPC24XX_DAC_Driver;

HMP3Decoder MP3Decoder::hMP3Decoder;

#pragma arm section zidata = "SectionForInternalSRAM"
unsigned char static_mp3DecInfo[sizeof(MP3DecInfo)];
unsigned char static_fh[sizeof(FrameHeader)];
unsigned char static_si[sizeof(SideInfo)];
unsigned char static_sfi[sizeof(ScaleFactorInfo)];
unsigned char static_hi[sizeof(HuffmanInfo)];
unsigned char static_di[sizeof(DequantInfo)];
unsigned char static_mi[sizeof(IMDCTInfo)];
unsigned char static_sbi[sizeof(SubbandInfo)];
#pragma arm section zidata

// we use the DAC input buffer directly as the output one for the PCM samples: there is no need 
// to synch on g_LPC24XX_DAC_Driver.SamplesBuffer (only written to by this code) or nextFrameWrite (only written to in code that executes serially in our respect).
// However, we need to disable interrupts from the TIMER used by the DAC to access the SampleCount and FrameCount values as the DAC ISR writes to it.

MP3FrameInfo MP3Decoder::lastMP3FrameInfo = { 0, 0, 0, 0, 0, 0, 0};


void MP3Decoder::FindNextFrameStart( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 managedInBuffer, HRESULT &hr )
{
	UINT8* inBuffer = managedInBuffer.GetBuffer();
	INT32& bytesLeft = Get_bytesLeft(pMngObj);
	INT32& readIndex = Get_readIndex(pMngObj);
	
	UINT8* readAddress = inBuffer + readIndex;
	INT32 offset = MP3FindSyncWord(readAddress, bytesLeft);
		if (offset < 0) {
			hr = CLR_E_INDEX_OUT_OF_RANGE;
			return;
		}
		readIndex += offset;
		bytesLeft -= offset;
	
}

void MP3Decoder::UpdateDACIndices(UINT32 outputSamps, UINT16 framesDecodedThisRun)
{
	//disable IRQs altogether. Disabling just the TIMER takes more cycles and requires a global lock anyway. Uneconomical for such a short time.
	GLOBAL_LOCK(irq);
	//Update DAC buffer state (only ISR-critical part: Sample and Frame count)
	g_LPC24XX_DAC_Driver.SampleCount += outputSamps;
	g_LPC24XX_DAC_Driver.FrameCount  += framesDecodedThisRun;
}
				
void MP3Decoder::NativeMP3Decode( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 managedInBuffer, INT16 * framesDecodedAtomically, HRESULT &hr )
{
#if defined(DECODER_DBG)
	debug_printf("=============NativeMP3Decode ====================== fDA=%d\r\n", *framesDecodedAtomically);
#endif
	
	UINT32 samplesDecodedThisRun = 0;
	UINT16 framesDecodedThisRun = 0;
		
	if(hMP3Decoder==0)
	{
		hr = CLR_E_NULL_REFERENCE;
		return;
	}
	
	UINT8* inBuffer = managedInBuffer.GetBuffer();
	INT32& bytesLeft = Get_bytesLeft(pMngObj);
	INT32& readIndex = Get_readIndex(pMngObj);
	
	//BOOL wasEnabled = false;
#if defined(DECODER_DBG)	
	debug_printf("bytesLeft =%d\r\n", bytesLeft);
	debug_printf("readIndex =%d\r\n", readIndex);
#endif
	for(; *framesDecodedAtomically > 0; (*framesDecodedAtomically)--)
	{
#if defined(DECODER_DBG)
		debug_printf("fDA =%d\r\n", *framesDecodedAtomically);
#endif
		//Check whether the DAC buffer is full
		//NOTE: no need to disable IRQs, as the ISR only DECREASES frame count and we only read FrameCount. 
		//		The worse that can happen is thinking the DAC buffer is full when we could actually add 
		//		one more frame. It's not worth disabling sound output.
		if(g_LPC24XX_DAC_Driver.FrameCount >= DAC_FRAME_BUFFERS_NUM)
		{
			//hr = CLR_E_OUT_OF_MEMORY; // don't generate an exception for buffer full, useless. just going on will let it empty.
			return;
		}
#if defined(DECODER_DBG)		
		debug_printf("Frame Buffers =%d/%d\r\n", DAC_FRAME_BUFFERS_NUM - g_LPC24XX_DAC_Driver.FrameCount, DAC_FRAME_BUFFERS_NUM);
#endif
		//look for next Frame in MP3 Stream
		MP3Decoder::FindNextFrameStart( pMngObj, managedInBuffer, hr );
		
		if(hr == CLR_E_INDEX_OUT_OF_RANGE) //we reached the end of the input buffer
		{
			hr = S_OK;
			*framesDecodedAtomically = -1;
			return;
		}
		
		//calculate read address for the decoder
		UINT8* readAddress = inBuffer + readIndex;
		
		//update nextFrameWrite
		if(g_LPC24XX_DAC_Driver.nextFrameWrite == DAC_FRAME_BUFFERS_NUM)
			g_LPC24XX_DAC_Driver.nextFrameWrite = 0;
		
		//Actual decoding call
		/*debug_printf("\tMP3Decode(hMP3Decoder =0x%x, readAddr=0x%x, bytesLeft=0x%x,outbuf=0x%x,0)\r\n"
					,hMP3Decoder
					,readAddress
					,bytesLeft
					,(short*)(g_LPC24XX_DAC_Driver.SamplesBuffer+(g_LPC24XX_DAC_Driver.nextFrameWrite * DAC_FRAME_BUFFER_SIZE_SAMPLES))
					);*/
#if defined(DECODER_TIMING)					
		volatile INT32 decStartTime = Time_CurrentTicks();
#endif		
		INT32 err = MP3Decode(hMP3Decoder, &readAddress, &bytesLeft, (short*)(g_LPC24XX_DAC_Driver.SamplesBuffer+(g_LPC24XX_DAC_Driver.nextFrameWrite * DAC_FRAME_BUFFER_SIZE_SAMPLES)), 0);
		framesDecoded++;
#if defined(DECODER_TIMING)		
		volatile INT32 decEndTime = Time_CurrentTicks();
		if(framesDecoded < TIMED_FRAMES)
			decodingTimesMs[framesDecoded] = (decEndTime-decStartTime)*(1000000.0/CPU_TicksPerSecond())/1000;
		//debug_printf("Decode took %u ticks (%fms)\r\n",decEndTime-decStartTime,(decEndTime-decStartTime)*(1000000.0/CPU_TicksPerSecond())/1000);
#endif			
		//update readIndex. bytesLeft was updated by helix itself
		readIndex = (INT32) (readAddress - inBuffer);
#if defined(DECODER_DBG)			
		debug_printf("\tMP3Decode()'d, err = %d\r\n", err);
#endif
		switch(err)
		{
			case ERR_MP3_NONE:
				//get last FrameSampleSize from the decoder.
				MP3GetLastFrameInfo(hMP3Decoder, &lastMP3FrameInfo);
				/*debug_printf("FrameInfo:\r\n\tbps=%d\r\n\tnChans=%d\r\n\tHz=%d\r\n\tsample=%dbit\r\n\toutSamps=%d\r\n\tlayer %d, v.%d\r\n",
				lastMP3FrameInfo.bitrate,
				lastMP3FrameInfo.nChans,
				lastMP3FrameInfo.samprate,
				lastMP3FrameInfo.bitsPerSample,
				lastMP3FrameInfo.outputSamps,
				lastMP3FrameInfo.layer,
				lastMP3FrameInfo.version);*/
			
				samplesDecodedThisRun += lastMP3FrameInfo.outputSamps;
				framesDecodedThisRun++;
				
				
				//Update DAC buffer state (only parts not written to by the ISR: Samples in last framebuffer used, nextFrameWrite)
				g_LPC24XX_DAC_Driver.SamplesInFrame[g_LPC24XX_DAC_Driver.nextFrameWrite] = lastMP3FrameInfo.outputSamps;
				//debug_printf("\r\nWF:%u\r\n", g_LPC24XX_DAC_Driver.nextFrameWrite);
				g_LPC24XX_DAC_Driver.nextFrameWrite++;

#if defined(DECODER_DBG)
				debug_printf(
					"New DAC Buffer state:\r\n\tSampleCount=%d\r\n\tnextFrameWrite=%d\r\n\tFrameCount=%d\r\n\tLastFrameLen=%d\r\n",
					g_LPC24XX_DAC_Driver.SampleCount,
					g_LPC24XX_DAC_Driver.nextFrameWrite,
					g_LPC24XX_DAC_Driver.FrameCount,
					g_LPC24XX_DAC_Driver.SamplesInFrame[g_LPC24XX_DAC_Driver.nextFrameWrite-1]);
#endif				
				break;
			case ERR_MP3_MAINDATA_UNDERFLOW:		/////////////////TODOOOOO. DISCERN between INDATA and MAINDATA underflow!!!! //////////////
				hr = CLR_E_NOTIMPL;
				return;
			case ERR_MP3_FREE_BITRATE_SYNC:
				hr = CLR_E_NOTIMPL;
				return;
			case ERR_MP3_INDATA_UNDERFLOW:
				hr = CLR_E_NOTIMPL;
				return;
			default:
				hr = CLR_E_NOTIMPL;
				return;
		}

	}
	
#if defined(DAC_STOP_TIMING)				
	volatile INT32 dacEndTime = 0;				
	volatile INT32 dacStartTime = Time_CurrentTicks();
#endif
	//Update DAC buffer state (only ISR-critical part: Sample and Frame count)
	//this method disables output timer IRQ. We have to, in order to avoid corruption of the DAC buffer indices.
	MP3Decoder::UpdateDACIndices(samplesDecodedThisRun, framesDecodedThisRun);
	
#if defined(DAC_STOP_TIMING)				
	dacEndTime = Time_CurrentTicks();
	
	if(framesDecoded < TIMED_FRAMES)
		stopTimesUs[framesDecoded] = (dacEndTime-dacStartTime)*(1000000.0/CPU_TicksPerSecond());
#endif
}

void MP3Decoder::InitHelixDecoder( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{	
	if(hMP3Decoder!=0)
	{
		hr = CLR_E_GENERIC_EXCEPTION;
		return;
	}
	if ( (hMP3Decoder = MP3InitDecoder()) == 0 )
		hr = CLR_E_GENERIC_EXCEPTION;
	
	// g_LPC24XX_DAC_Driver.Initialize(8000);
	// g_LPC24XX_DAC_Driver.On();
}

void MP3Decoder::UninitHelixDecoder( CLR_RT_HeapBlock* pMngObj, HRESULT &hr )
{
	if(hMP3Decoder==0)
	{
		hr = CLR_E_NULL_REFERENCE;
		return;
	}
	MP3FreeDecoder(hMP3Decoder);
	hMP3Decoder = 0;
	//g_LPC24XX_DAC_Driver.Uninitialize();
}

