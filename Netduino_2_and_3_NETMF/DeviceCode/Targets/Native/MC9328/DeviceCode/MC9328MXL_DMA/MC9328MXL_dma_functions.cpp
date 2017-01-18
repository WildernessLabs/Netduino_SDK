////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>



//--//

BOOL DMA_Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    return MC9328MXL_DMA_Driver::Initialize();
}

BOOL DMA_Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    return MC9328MXL_DMA_Driver::Uninitialize();
}

void DMA_MemCpy( void* dst, void*src, UINT32 size, BOOL async = FALSE )
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    MC9328MXL_DMA_Driver::MemCpy( dst, src, size, async );
}

void DMA_MemCpy2D( void* dst, void*src, UINT32 width, UINT32 height, UINT32 dataWidth, BOOL async = FALSE )
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    MC9328MXL_DMA_Driver::MemCpy2D( dst, src, width, height, dataWidth, async );
}

void DMA_StartDummyDMA()
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    MC9328MXL_DMA_Driver::StartDummyDMA();
}
