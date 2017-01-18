////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

void CPU_FlushCaches()
{
    UINT32 reg = 0;
#ifdef __GNUC__
	asm("MCR p15, 0, %0, c7,  c6, 0" :: "r" (reg));
	asm("MCR p15, 0, %0, c7, c10, 4" :: "r" (reg));
	asm("MCR p15, 0, %0, c7,  c5, 0" :: "r" (reg));
#else
    __asm
    {
        mcr p15, 0, reg, c7,  c6, 0 // invalidate DCache (Write through)
        mcr p15, 0, reg, c7, c10, 4 // Drain write buffer
        mcr p15, 0, reg, c7,  c5, 0 // invalidate Icache
    }
#endif
//  tci_loop:
//  mrc p15, 0, pc, c7, c14, 3 // test clean & invalidate DCache
//  bne tci_loop
}

void CPU_DrainWriteBuffers()
{
    UINT32  reg = 0;        

#ifdef __GNUC__
	asm("MCR p15, 0, %0, c7, c10, 4" :: "r" (reg));
#else
    __asm
    {
        mcr     p15, 0, reg, c7, c10, 4 // Drain Write Buffers.
    }
#endif
}

void CPU_InvalidateCaches()
{
    UINT32 reg = 0;

#ifdef __GNUC__
	asm("MCR p15, 0, %0, c7, c7, 0" :: "r" (reg));
#else
    __asm
    {
        mcr     p15, 0, reg, c7, c7 , 0 // Invalidate caches.
    }
#endif
}

void CPU_EnableCaches()
{
    UINT32 reg;

    CPU_InvalidateCaches();

#ifdef __GNUC__
	asm("MRC p15, 0, %0, c1, c0, 0" : "=r" (reg));
	asm("ORR %0, %0, #0x1000"       : "=r" (reg) : "r" (reg));
	asm("ORR %0, %0, #0x0004"       : "=r" (reg) : "r" (reg));
	asm("MCR p15, 0, %0, c1, c0, 0" :            : "r" (reg));
#else
    __asm
    {
        mrc     p15, 0, reg, c1, c0, 0
        orr     reg, reg, #0x1000             // Enable ICache
        orr     reg, reg, #0x0004             // Enable DCache
        mcr     p15, 0, reg, c1, c0, 0
    }
#endif
}

void CPU_DisableCaches()
{
    UINT32 reg;

#ifdef __GNUC__
	asm("MRC p15, 0, %0, c1, c0, 0" : "=r" (reg));
	asm("BIC %0, %0, #0x1000"       : "=r" (reg) : "r" (reg));
	asm("BIC %0, %0, #0x0004"       : "=r" (reg) : "r" (reg));
	asm("MCR p15, 0, %0, c1, c0, 0" :            : "r" (reg));
#else
    __asm
    {
        mrc     p15, 0, reg, c1, c0, 0
        bic     reg, reg, #0x1000             // Disable ICache
        bic     reg, reg, #0x0004             // Disable DCache
        mcr     p15, 0, reg, c1, c0, 0
    }
#endif

    CPU_FlushCaches();
}

//--//

template <typename T> void CPU_InvalidateAddress( T* address )
{
    UINT32 reg = 0;

#ifdef __GNUC__
	asm("MCR p15, 0, %0, c7, c10, 4" :: "r" (reg));
	asm("MCR p15, 0, %0, c7,  c5, 1" :: "r" (address));
	asm("MCR p15, 0, %0, c7,  c6, 1" :: "r" (address));
#else
	__asm
    {
        mcr     p15, 0, reg    , c7, c10, 4        // Drain Write Buffers.
        mcr     p15, 0, address, c7,  c5, 1        // Invalidate ICache.
        mcr     p15, 0, address, c7,  c6, 1        // Invalidate DCache.
    }
#endif
}

//--//

size_t CPU_GetCachableAddress( size_t address )
{
    return address;
}

//--//

size_t CPU_GetUncachableAddress( size_t address )
{
    return address;
}
