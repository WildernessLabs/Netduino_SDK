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
        pin = SH7264_GPIO::c_SW_K1;     //SW_04;
        break;
    case VK_LEFT:
        pin = SH7264_GPIO::c_SW_K2;     //SW_05;
        break;
    case VK_UP:
        pin = SH7264_GPIO::c_SW_K3;     //SW_06;
        break;
    case VK_DOWN:
        pin = SH7264_GPIO::c_SW_K4;     //SW_07;
        break;
    case VK_SELECT:
        pin = SH7264_GPIO::c_SW_K5;     //SW_08;
        break;
    case VK_RIGHT:
        pin = SH7264_GPIO::c_SW_K6;     //SW_09;
        break;
    case VK_HOME:
        pin = SH7264_GPIO::c_SW_K7;     //SW_10;
        break;
    case VK_BACK:
        pin = SH7264_GPIO::c_SW_K8;     //SW_11;
        break;    
    default:        
        pin = GPIO_PIN_NONE;
        break;
    }

   return pin;
}