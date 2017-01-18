////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_PXA271_DMA_Driver"
#endif

PXA271_DMA_Driver g_PXA271_DMA_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL PXA271_DMA_Driver::Initialize()
{
    PXA271_DMA & DMA = PXA271::DMA();


    return TRUE;
}

BOOL PXA271_DMA_Driver::Uninitialize()
{
    PXA271_DMA & DMA = PXA271::DMA();


    return TRUE;
}

void PXA271_DMA_Driver::MemCpy(void* dst, void* src, UINT32 size, BOOL async = FALSE)
{
    PXA271_DMA & DMA = PXA271::DMA();
}

void PXA271_DMA_Driver::MemCpy2D( void *dst, void*src, UINT32 width, UINT32 height, UINT32 dataWidth, BOOL async = FALSE )
{
    PXA271_DMA & DMA = PXA271::DMA();
}

void PXA271_DMA_Driver::StartDummyDMA()
{
}

// Translate from virtual address to physical address
void* PXA271_DMA_Driver::TranslateAddress( void* virtualAddress )
{
    return NULL;
}
