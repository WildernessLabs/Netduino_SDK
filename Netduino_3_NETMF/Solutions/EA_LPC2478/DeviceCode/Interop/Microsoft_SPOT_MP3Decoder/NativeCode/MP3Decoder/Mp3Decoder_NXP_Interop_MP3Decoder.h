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


#ifndef _MP3DECODER_NXP_INTEROP_MP3DECODER_H_
#define _MP3DECODER_NXP_INTEROP_MP3DECODER_H_

#include <mp3common.h>
#include <DeviceCode\LPC24XX.h>

//just a random number among those NOT in TinyCLR_ErrorCodes.h, the PK docs only say to use "any other value except the ones already used". clear, uh?
#define CLR_E_GENERIC_EXCEPTION 	1234

//update the DAC driver if this changes. I didn't include this .h in the DAC driver as I don't want it to be dependent on the MP3 decoder
#define MAX_DECODED_FRAME_SIZE 		0x900

namespace NXP
{
    namespace Interop
    {
        struct MP3Decoder
        {
            // Helper Functions to access fields of managed object
            static UNSUPPORTED_TYPE& Get_inBuffer( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_UNSUPPORTED_TYPE( pMngObj, Library_Mp3Decoder_NXP_Interop_MP3Decoder::FIELD__inBuffer ); }

            static INT32& Get_bytesLeft( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_Mp3Decoder_NXP_Interop_MP3Decoder::FIELD__bytesLeft ); }

            static INT32& Get_readIndex( CLR_RT_HeapBlock* pMngObj )    { return Interop_Marshal_GetField_INT32( pMngObj, Library_Mp3Decoder_NXP_Interop_MP3Decoder::FIELD__readIndex ); }

            // Declaration of stubs. These functions are implemented by Interop code developers
            static void FindNextFrameStart( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 inBuffer, HRESULT &hr );
            static void NativeMP3Decode( CLR_RT_HeapBlock* pMngObj, CLR_RT_TypedArray_UINT8 inBuffer, INT16 * framesDecodedAtomically, HRESULT &hr );
            static void InitHelixDecoder( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
            static void UninitHelixDecoder( CLR_RT_HeapBlock* pMngObj, HRESULT &hr );
			
			/////////////////////
			//support variables//
			/////////////////////
			
			// Helix Decoder Handle
			static HMP3Decoder hMP3Decoder;
			
			//aux mp3 info
			static MP3FrameInfo lastMP3FrameInfo;
			
			/////////////////////
			//support methods  //
			/////////////////////
			
			static void UpdateDACIndices(UINT32 outputSamps, UINT16 framesDecodedThisRun);
        };
    }
}
#endif  //_MP3DECODER_NXP_INTEROP_MP3DECODER_H_
