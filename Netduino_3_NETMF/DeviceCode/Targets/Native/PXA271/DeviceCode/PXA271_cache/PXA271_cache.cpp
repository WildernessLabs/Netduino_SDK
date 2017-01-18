////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>


#pragma arm section code = "SectionForBootstrapOperations"

//--//

void __section(SectionForBootstrapOperations) CPU_FlushCaches()
{
    CPU_ARM9_FlushCaches(32, 32);
}


//--//
size_t __section(SectionForBootstrapOperations) CPU_GetCachableAddress( size_t address )
{
    return address & ~PXA271::c_UncachableMask;
}

//--//

size_t __section(SectionForBootstrapOperations) CPU_GetUncachableAddress( size_t address )
{
    if(CPU_IsMMUEnabled())
    {
        return address | PXA271::c_UncachableMask;
    }
    else
    {
        return address & ~PXA271::c_UncachableMask;
    }
}

#pragma arm section code
