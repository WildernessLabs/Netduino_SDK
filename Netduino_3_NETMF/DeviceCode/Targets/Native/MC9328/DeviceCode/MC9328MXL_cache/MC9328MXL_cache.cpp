////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>

#pragma arm section code = "SectionForBootstrapOperations"

void __section(SectionForBootstrapOperations) CPU_FlushCaches()
{
    CPU_ARM9_FlushCaches(8,64);
}

//--//

// OTHER CACHE FUNCTIONS COME FROM ARM9 common implemenation

//--//

size_t __section(SectionForBootstrapOperations) CPU_GetCachableAddress( size_t address )
{
    return address & ~MC9328MXL::c_UncachableMask;
}

//--//

size_t __section(SectionForBootstrapOperations) CPU_GetUncachableAddress( size_t address )
{

    // only the flash range is uncachable
    // Note have to use the Constant for Flash memroy base and size, 
    // as the MMU will use this function as well,without startingthe MMU,
    // it is not yet ready to access RW area data.
    if(CPU_IsMMUEnabled())
    {
        return address | MC9328MXL::c_UncachableMask;
    }
    else
    {
        return address & ~MC9328MXL::c_UncachableMask;
    }
}

#pragma arm section code 


