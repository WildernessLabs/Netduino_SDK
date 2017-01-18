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


static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_Mp3Decoder_NXP_Interop_MP3Decoder::FindNextFrameStart___VOID__SZARRAY_U1,
    Library_Mp3Decoder_NXP_Interop_MP3Decoder::NativeMP3Decode___VOID__SZARRAY_U1__BYREF_I2,
    Library_Mp3Decoder_NXP_Interop_MP3Decoder::InitHelixDecoder___VOID,
    Library_Mp3Decoder_NXP_Interop_MP3Decoder::UninitHelixDecoder___VOID,
    NULL,
    NULL,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Mp3Decoder =
{
    "Mp3Decoder", 
    0x7059ABF8,
    method_lookup
};

