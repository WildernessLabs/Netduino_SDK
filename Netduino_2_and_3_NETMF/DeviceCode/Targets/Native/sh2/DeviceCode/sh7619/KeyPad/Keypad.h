////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GPIO_KEYPAD_H_
#define _GPIO_KEYPAD_H_ 1


//--//

struct GPIO_KEYPAD_Driver
{
	static void Debounce_Delay (unsigned int time);
	static void KeyPad_Initialize (void);
	static char KeyPad_Check_Bit(unsigned int keybit);
	static int  KeyPad_Search(void);

	unsigned int key_data_tmp[2];
	char Keyscan_result;
	char Edge_flag;
	char Keyscan_Active;
};

extern GPIO_KEYPAD_Driver g_GPIO_KEYPAD_Driver;

 
//--//

#endif  //_GPIO_BUTTON_H_
