////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

///////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_MC9328MXL_USART_Config"
#endif

MC9328MXL_USART_CONFIG __section(rwdata) g_MC9328MXL_USART_Config =
{
    { TRUE }, // HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    TRUE,       // UINT8         TxProtectInput;
    TRUE,       // UINT8         RxProtectInput;
    TRUE,       // UINT8         CTSProtectInput;
    TRUE,       // UINT8         RTSProtectInput;
    TXPROTECTRESISTOR,    // GPIO_RESISTOR TxProtectResistor;
    RXPROTECTRESISTOR,    // GPIO_RESISTOR RxProtectResistor;
    CTSPROTECTRESISTOR,   // GPIO_RESISTOR CTSProtectResistor;
    RTSPROTECTRESISTOR,   // GPIO_RESISTOR RTSProtectResistor;
    FALSE, // UINT8         TxProtectOutputValue;
    FALSE, // UINT8         RxProtectOutputValue;
    FALSE, // UINT8         CTSProtectOutputValue;
    FALSE  // UINT8         RTSProtectOutputValue;
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif



