////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "Spot_InteropSample_Native.h"
#include "Spot_InteropSample_Native_Microsoft_SPOT_Interop_TestCallback.h"

using namespace Microsoft::SPOT::Interop;


HRESULT Library_Spot_InteropSample_Native_Microsoft_SPOT_Interop_TestCallback::GenerateInterrupt___STATIC__VOID( CLR_RT_StackFrame& stack )
{
    TINYCLR_HEADER(); hr = S_OK;
    {
        TestCallback::GenerateInterrupt( hr );
        TINYCLR_CHECK_HRESULT( hr );
    }
    TINYCLR_NOCLEANUP();
}
