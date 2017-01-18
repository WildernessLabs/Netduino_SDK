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

BOOL MC9328MXL_EIM_Driver::Memory_ReadOnly( const CPU_MEMORY_CONFIG& CPUMemoryConfig, BOOL ReadOnly )
{
    NATIVE_PROFILE_HAL_PROCESSOR_EBIU();
    // lock this non-atomic update to the control register
    GLOBAL_LOCK(irq);

    MC9328MXL_EIM::DEVICE& dev = MC9328MXL::EIM().Device[CPUMemoryConfig.ChipSelect];

    UINT32 Control = dev.CS_L;

    BOOL Prev_ReadOnly = (Control & MC9328MXL_EIM::DEVICE::CS_L__WP) != 0;

    if(ReadOnly) Control |=  MC9328MXL_EIM::DEVICE::CS_L__WP;
    else         Control &= ~MC9328MXL_EIM::DEVICE::CS_L__WP;

    dev.CS_L = Control;

    return Prev_ReadOnly;
}

void MC9328MXL_EIM_Driver::MemoryConfig_To_Control( const CPU_MEMORY_CONFIG& CPUMemoryConfig, UINT32& CS_L, UINT32& CS_U )
{
    NATIVE_PROFILE_HAL_PROCESSOR_EBIU();
    //
    // assuming the Burst and page mode are not used in appl.
    //


    // do the upper word of the control word, the write extra wait state to 0
    {
        UINT32 val = 0;

        val |= MC9328MXL_EIM::DEVICE::CS_U__WSC__set( CPUMemoryConfig.WaitStates    );
        val |= MC9328MXL_EIM::DEVICE::CS_U__EDC__set( CPUMemoryConfig.ReleaseCounts );

        // cleared the needed field to zero, do not change the other field. Not all the non-used fields are set to 0
        CS_U = (CS_U & (~MC9328MXL_EIM::DEVICE::CS_U__NEEDED_CONTROL)) | val;
    }

    // lower control word, use upper control 16 bit to determine which session it is.
    {
        UINT32 val = 0;

        UINT32 bitWidth = CPUMemoryConfig.BitWidth & 0xFFFF;
        UINT32 bitOrder = CPUMemoryConfig.BitWidth >>    16;

        switch(bitWidth)
        {
        case 8:
            switch(bitOrder)
            {
            case 0 : bitOrder = MC9328MXL_EIM::DEVICE::CS_L__DSZ__UU8; break;
            case 1 : bitOrder = MC9328MXL_EIM::DEVICE::CS_L__DSZ__UL8; break;
            case 2 : bitOrder = MC9328MXL_EIM::DEVICE::CS_L__DSZ__LU8; break;
            case 3 : bitOrder = MC9328MXL_EIM::DEVICE::CS_L__DSZ__LL8; break;
            default: ASSERT(0)                                       ; break;
            }
            break;

        case 16:
            switch(bitOrder)
            {
            case 0 : bitOrder = MC9328MXL_EIM::DEVICE::CS_L__DSZ__U16; break;
            case 1 : bitOrder = MC9328MXL_EIM::DEVICE::CS_L__DSZ__L16; break;
            default: ASSERT(0)                                       ; break;
            }
            break;

        case 32:
            switch(bitOrder)
            {
            case 0 : bitOrder = MC9328MXL_EIM::DEVICE::CS_L__DSZ__32; break;
            default: ASSERT(0)                                      ; break;
            }
            break;

        default: ASSERT(0); break;
        }

        val |= MC9328MXL_EIM::DEVICE::CS_L__DSZ__set( bitOrder );

        if(CPUMemoryConfig.ReadOnly)
        {
            val |= MC9328MXL_EIM::DEVICE::CS_L__WP;     // Disables all writes to the device. This can be used to protect FLASH memory from inadvertent erasure.
        }

        if(!CPUMemoryConfig.ByteSignalsForRead)
        {
            val |= MC9328MXL_EIM::DEVICE::CS_L__EBC; // Determines whether the XBE[3:0] signals are used for writes only or for both reads and writes.
                                                     // 1: XBE[3:0] are used for writes only. They are byte write enables.
                                                     // 0: XBE[3:0] are used for BOTH reads and writes.
        }

        // enable the memory mapper
        val |= MC9328MXL_EIM::DEVICE::CS_L__CSEN;

        CS_L = (CS_L & (~MC9328MXL_EIM::DEVICE::CS_L__NEEDED_CONTROL)) | val;
    }
}

void MC9328MXL_EIM_Driver::ConfigMemoryBlock( const CPU_MEMORY_CONFIG& CPUMemoryConfig )
{
    NATIVE_PROFILE_HAL_PROCESSOR_EBIU();
    MC9328MXL_EIM::DEVICE& dev = MC9328MXL::EIM().Device[CPUMemoryConfig.ChipSelect];

    // Note: CS0, has to use supervisor mode, other CS ok to use normal mode, but we work in superiisor mode already.

    UINT32 CS_L = dev.CS_L;
    UINT32 CS_U = dev.CS_U;

    MemoryConfig_To_Control( CPUMemoryConfig, CS_L, CS_U );

    dev.CS_L = CS_L;
    dev.CS_U = CS_U;

    // enable chip select
    // if CS, other than CS0, have to disable A[24} primary GPIO function.
}

