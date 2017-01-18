////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <..\DeviceCode\Targets\Native\SH7264\DeviceCode\GPIO\SH7264_GPIO.h>

GPIO_PIN VirtualKey_GetPins( UINT32 virtualKey)
{
    GPIO_PIN pin;   

    switch (virtualKey)
    {
    case VK_MENU:
        pin = SH7264_GPIO::c_SW_K1;     //SW_02;
        break;
    case VK_SELECT:
        pin = SH7264_GPIO::c_SW_K2;     //SW_03;
        break;
    case VK_LEFT:
        pin = SH7264_GPIO::c_SW_K5;     //SW_06;
        break;
    case VK_RIGHT:
        pin = SH7264_GPIO::c_SW_K6;     //SW_07;
        break;
    case VK_UP:
        pin = SH7264_GPIO::c_SW_K9;      //SW_10;
        break;
    case VK_DOWN:
        pin = SH7264_GPIO::c_SW_K13;     //SW_14;
        break;
    case VK_HOME:
        pin = SH7264_GPIO::c_SW_K10;     //SW_11;
        break;
    case VK_BACK:
        pin = SH7264_GPIO::c_SW_K14;     //SW_15;
        break;    
    default:        
        pin = GPIO_PIN_NONE;
        break;
    }

   return pin;
}