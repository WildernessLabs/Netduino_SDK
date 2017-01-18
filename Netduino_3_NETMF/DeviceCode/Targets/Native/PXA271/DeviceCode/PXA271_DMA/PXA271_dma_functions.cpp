////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>

//--//
BOOL DMA_Initialize()
{
    return PXA271_DMA_Driver::Initialize();
}

BOOL DMA_Uninitialize()
{
    return PXA271_DMA_Driver::Uninitialize();
}

void DMA_MemCpy( void* dst, void*src, UINT32 size, BOOL async = FALSE )
{
    PXA271_DMA_Driver::MemCpy( dst, src, size, async );
}

void DMA_MemCpy2D( void* dst, void*src, UINT32 width, UINT32 height, UINT32 dataWidth, BOOL async = FALSE )
{
    PXA271_DMA_Driver::MemCpy2D( dst, src, width, height, dataWidth, async );
}

void DMA_StartDummyDMA()
{
    PXA271_DMA_Driver::StartDummyDMA();
}

