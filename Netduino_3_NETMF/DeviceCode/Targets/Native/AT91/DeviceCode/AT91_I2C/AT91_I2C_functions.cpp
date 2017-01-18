////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <cores\arm\include\cpu.h>
#include <tinyhal.h>

//--//

BOOL I2C_Internal_Initialize()
{
    return AT91_I2C_Driver::Initialize();
}

BOOL I2C_Internal_Uninitialize()
{
    return AT91_I2C_Driver::Uninitialize();
}

void I2C_Internal_XActionStart( I2C_HAL_XACTION* xAction, bool repeatedStart )
{
    AT91_I2C_Driver::MasterXAction_Start( xAction, repeatedStart );
}

void I2C_Internal_XActionStop()
{
    AT91_I2C_Driver::MasterXAction_Stop();
}

void I2C_Internal_GetClockRate( UINT32 rateKhz, UINT8& clockRate, UINT8& clockRate2)
{
    AT91_I2C_Driver::GetClockRate( rateKhz, clockRate, clockRate2 );
}

void I2C_Internal_GetPins(GPIO_PIN& scl, GPIO_PIN& sda)
{
    return AT91_I2C_Driver::GetPins( scl, sda);
}
