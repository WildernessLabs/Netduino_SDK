////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

/***************************************************************************/

BOOL PXA271_EIM_Driver::Memory_ReadOnly( const CPU_MEMORY_CONFIG& CPUMemoryConfig, BOOL ReadOnly )
{
	return true;
}

void PXA271_EIM_Driver::MemoryConfig_To_Control( const CPU_MEMORY_CONFIG& CPUMemoryConfig, UINT32& CS_L, UINT32& CS_U )
{
}

void PXA271_EIM_Driver::ConfigMemoryBlock( const CPU_MEMORY_CONFIG& CPUMemoryConfig )
{
}

