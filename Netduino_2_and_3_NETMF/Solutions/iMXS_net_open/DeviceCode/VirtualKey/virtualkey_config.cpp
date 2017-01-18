////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>


GPIO_PIN VirtualKey_GetPins(UINT32 virtualKey)
{
    GPIO_PIN pin;

    switch (virtualKey)
    {
    case VK_MENU:
        pin = MC9328MXL_GPIO::c_Port_B_15;
        break;
    case VK_SELECT:
        pin = MC9328MXL_GPIO::c_Port_B_08;
        break;
    case VK_UP:
        pin = MC9328MXL_GPIO::c_Port_B_11;
        break;
    case VK_DOWN:
        pin = MC9328MXL_GPIO::c_Port_B_10;
        break;
    case VK_LEFT:
        pin = MC9328MXL_GPIO::c_Port_B_16;
        break;
    case VK_RIGHT:
        pin = MC9328MXL_GPIO::c_Port_B_12;
        break;
    case VK_BACK:
        pin = MC9328MXL_GPIO::c_Port_B_17;
        break;
    case VK_HOME:
        pin = MC9328MXL_GPIO::c_Port_B_09;
        break;
    default:
        pin = GPIO_PIN_NONE;
        break;
    }
   
   return pin;
}

