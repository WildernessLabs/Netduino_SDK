////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//
BOOL I2C_Initialize()
{
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}

BOOL I2C_Uninitialize()
{
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}

BOOL I2C_Enqueue( I2C_HAL_XACTION* xAction )
{
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}

void I2C_Cancel( I2C_HAL_XACTION* xAction, bool signal )
{
    NATIVE_PROFILE_PAL_COM();
}

void I2C_InitializeTransaction( I2C_HAL_XACTION* xAction, I2C_USER_CONFIGURATION& config, I2C_HAL_XACTION_UNIT** xActions, size_t numXActions )
{
    NATIVE_PROFILE_PAL_COM();
}

void I2C_InitializeTransactionUnit( I2C_HAL_XACTION_UNIT* xActionUnit, I2C_WORD* src, I2C_WORD* dst, size_t size, BOOL fRead )
{
    NATIVE_PROFILE_PAL_COM();
}

//--//

void I2C_XAction_SetState( I2C_HAL_XACTION* xAction, UINT8 state )
{
    NATIVE_PROFILE_PAL_COM();
}

UINT8 I2C_XAction_GetState( I2C_HAL_XACTION* xAction )
{
    NATIVE_PROFILE_PAL_COM();
    return 0;
}

BOOL I2C_XAction_CheckState( I2C_HAL_XACTION* xAction, UINT8 state )
{
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}

size_t I2C_XAction_TransactedBytes( I2C_HAL_XACTION* xAction )
{
    NATIVE_PROFILE_PAL_COM();
    return 0;
}

void I2C_XActionUnit_CopyBuffer( I2C_HAL_XACTION_UNIT* xActionUnit, I2C_WORD* data, size_t length )
{
    NATIVE_PROFILE_PAL_COM();
}

BOOL I2C_XActionUnit_IsRead( I2C_HAL_XACTION_UNIT* xActionUnit )
{
    NATIVE_PROFILE_PAL_COM();
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////


