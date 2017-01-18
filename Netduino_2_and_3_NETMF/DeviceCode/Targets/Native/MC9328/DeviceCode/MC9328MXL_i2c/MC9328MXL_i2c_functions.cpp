////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>


BOOL I2C_Internal_Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    return MC9328MXL_I2C_Driver::Initialize();
}

BOOL I2C_Internal_Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    return MC9328MXL_I2C_Driver::Uninitialize();
}

void I2C_Internal_XActionStart( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    MC9328MXL_I2C_Driver::MasterXAction_Start( xAction, repeatedStart );
}

void I2C_Internal_XActionStop()
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    MC9328MXL_I2C_Driver::MasterXAction_Stop();
}

void I2C_Internal_GetClockRate( UINT32 rateKhz, UINT8& clockRate, UINT8& clockRate2)
{
    NATIVE_PROFILE_HAL_PROCESSOR_I2C();
    clockRate = MC9328MXL_I2C_Driver::GetClockRate( rateKhz );
    //clockrate2 not use.
    clockRate2 = 0;
}

void I2C_Internal_GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
    MC9328MXL_I2C_Driver::GetPins( scl, sda);
    return;
}

