////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <../../../../DeviceCode/Targets/Native/sh2/DeviceCode/sh7619/GPIO/SH7619_GPIO.h>

#define AppDefined1              273
#define AppDefined2              274
#define AppDefined3              275
#define AppDefined4              276
#define AppDefined5              277
#define AppDefined6              278

GPIO_PIN VirtualKey_GetPins(UINT32 virtualKey)
{
    GPIO_PIN pin;

    switch (virtualKey)
    {
    case VK_MENU:
        pin = SH7619_GPIO::c_Key_6;  //Key6(Fn)
        break;
    case VK_SELECT:
        pin = SH7619_GPIO::c_Key_5;  //Key5(OK)
        break;
    case VK_UP:
        pin = SH7619_GPIO::c_Key_1;  //Key1(UP)
        break;
    case VK_DOWN:
        pin = SH7619_GPIO::c_Key_2;  //Key2(DOWN)
        break;
    case VK_LEFT:
        pin = SH7619_GPIO::c_Key_3;  //Key3(LEFT)
        break;
    case VK_RIGHT:
        pin = SH7619_GPIO::c_Key_4;  //Key4(RIGHT)
        break;
    case VK_BACK:
        pin = SH7619_GPIO::c_Key_22;  //Key22(BACK)
        break;
    case VK_HOME:
        pin = SH7619_GPIO::c_Key_23;  //Key23(STOP)
        break;
//For Washmashine demo
	case AppDefined1:
        pin = SH7619_GPIO::PC00;  //On/Off
        break;
	case AppDefined2:
        pin = SH7619_GPIO::PC03;  //Water In
        break;
    case AppDefined3:
        pin = SH7619_GPIO::PC09;  //Wash
        break;
    case AppDefined4:
        pin = SH7619_GPIO::PC02;  //Water Out
        break;
    case AppDefined5:
        pin = SH7619_GPIO::PC01;  //Spin
        break;
    case AppDefined6:
        pin = SH7619_GPIO::PC11;  //Start
        break;
//For Washmashine demo
    default:
        pin = GPIO_PIN_NONE;
        break;
    }
   
   return pin;
}

