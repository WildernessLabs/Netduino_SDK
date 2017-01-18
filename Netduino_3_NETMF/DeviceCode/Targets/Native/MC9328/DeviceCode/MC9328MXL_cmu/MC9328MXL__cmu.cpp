////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>


void MC9328MXL_Clock_Driver::ClockDivisor( UINT32 peripheral, UINT32 divisor )
{
    NATIVE_PROFILE_HAL_PROCESSOR_CMU();
    MC9328MXL_CMU& CMU = MC9328MXL::CMU();

    UINT32 PCDR = CMU.PCDR;

    switch(peripheral)
    {
    case MC9328MXL_Clock_Driver::PERCLK1: PCDR = (PCDR & ~MC9328MXL_CMU::PCDR__PCLK_DIV1_mask) | ((divisor << MC9328MXL_CMU::PCDR__PCLK_DIV1_shift) & MC9328MXL_CMU::PCDR__PCLK_DIV1_mask); break;
    case MC9328MXL_Clock_Driver::PERCLK2: PCDR = (PCDR & ~MC9328MXL_CMU::PCDR__PCLK_DIV2_mask) | ((divisor << MC9328MXL_CMU::PCDR__PCLK_DIV2_shift) & MC9328MXL_CMU::PCDR__PCLK_DIV2_mask); break;
    case MC9328MXL_Clock_Driver::PERCLK3: PCDR = (PCDR & ~MC9328MXL_CMU::PCDR__PCLK_DIV3_mask) | ((divisor << MC9328MXL_CMU::PCDR__PCLK_DIV3_shift) & MC9328MXL_CMU::PCDR__PCLK_DIV3_mask); break;
    default                             : ASSERT(FALSE);                                                                                                                                    break;
    }

    CMU.PCDR = PCDR;
}

UINT32 MC9328MXL_Clock_Driver::ReadClockDivisor( UINT32 peripheral )
{
    NATIVE_PROFILE_HAL_PROCESSOR_CMU();
    MC9328MXL_CMU& CMU = MC9328MXL::CMU();

    UINT32 PCDR = CMU.PCDR;
    UINT32 clk;

    switch(peripheral)
    {
    case MC9328MXL_Clock_Driver::PERCLK1: clk = (PCDR & MC9328MXL_CMU::PCDR__PCLK_DIV1_mask) >> MC9328MXL_CMU::PCDR__PCLK_DIV1_shift; break;
    case MC9328MXL_Clock_Driver::PERCLK2: clk = (PCDR & MC9328MXL_CMU::PCDR__PCLK_DIV2_mask) >> MC9328MXL_CMU::PCDR__PCLK_DIV2_shift; break;
    case MC9328MXL_Clock_Driver::PERCLK3: clk = (PCDR & MC9328MXL_CMU::PCDR__PCLK_DIV3_mask) >> MC9328MXL_CMU::PCDR__PCLK_DIV3_shift; break;
    default                             : clk = 0; ASSERT(FALSE)                                                                             ; break;
    }

    return clk;
}

///////////////////////////////////////////////////////////////////////////////



