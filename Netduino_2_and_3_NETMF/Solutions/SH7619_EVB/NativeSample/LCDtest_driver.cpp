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

/*
 *    Copyright (C) Renesas Technology America,Ltd. 2009  All rights reserved.
 */
 
#include <tinyhal.h>
#include "LCDtest.h"
#include "..\..\DeviceCode\Drivers\Display\HD66773R\HD66773R.h"

void LCDTest_Driver::LCD_Dot_Write (unsigned short *lcd_top_addr, unsigned short dot_x, unsigned short dot_y, unsigned short color)
{
    unsigned short  *addr;

    addr = (unsigned short *)((unsigned long)lcd_top_addr + (dot_y * LCD_ONELINE_OFFSET) + (dot_x * 2));
    *addr = color;
}

long LCDTest_Driver::LCD_Full_Write (unsigned short *lcd_top_addr, unsigned short color)
{
    int p_x, p_y;

    for (p_y = 0; p_y < DISP_YMAX; p_y++)
    {
        for (p_x = 0; p_x < DISP_XMAX; p_x++)
        {
            *lcd_top_addr++ = color;
        }
    }

    return (0);
}

long LCDTest_Driver::LCD_Rect_Write (unsigned short *lcd_top_addr, LCD_RECT data)
{
    int             p_x, p_y;
    long            ercd;
    unsigned short  *addr, *save_addr;

    if ((data.Left_top.p_x < 0) || (DISP_XMAX < data.Left_top.p_x) ||
        (data.Right_bottom.p_x <= 0 ) || (DISP_XMAX < data.Right_bottom.p_x) ||
        (data.Left_top.p_x == data.Right_bottom.p_x))
    {
        ercd = -1;
        goto EXIT;
    }
    if ((data.Left_top.p_y < 0) || (DISP_YMAX < data.Left_top.p_y) ||
        (data.Right_bottom.p_y <= 0 ) || (DISP_YMAX < data.Right_bottom.p_y) ||
        (data.Left_top.p_y == data.Right_bottom.p_y))
    {
        ercd = -1;
        goto EXIT;
    }

    UINT32 offset = (data.Left_top.p_y * LCD_ONELINE_OFFSET) + (data.Left_top.p_x * 2);

    if (offset > DISP_XMAX *DISP_YMAX*2)
    {
        ercd = -1;
        goto EXIT;
    }

    addr = (unsigned short *)((unsigned long)lcd_top_addr + offset); 
    save_addr = addr;

   
    for (p_y = data.Left_top.p_y; p_y < data.Right_bottom.p_y; p_y++)
    {
        for (p_x = data.Left_top.p_x ; p_x < data.Right_bottom.p_x; p_x++)
        {
            *addr++ = data.Color;
        }

        save_addr = (unsigned short *)((unsigned long)save_addr + LCD_ONELINE_OFFSET);
        addr = save_addr;

    }

    ercd = 0;

EXIT:
    return (ercd);
}

long LCDTest_Driver::LCD_Frame_Write (unsigned short *lcd_top_addr, LCD_RECT data)
{
    int             p_x, p_y;//, x_max, y_max;
    long            ercd;
    unsigned short  *addr, *save_addr;

    if ((data.Left_top.p_x < 0) || (DISP_XMAX < data.Left_top.p_x) ||
        (data.Right_bottom.p_x <= 0 ) || (DISP_XMAX < data.Right_bottom.p_x) ||
        (data.Left_top.p_x == data.Right_bottom.p_x))
    {
        ercd = -1;
        goto EXIT;
    }
    if ((data.Left_top.p_y < 0) || (DISP_YMAX < data.Left_top.p_y) ||
        (data.Right_bottom.p_y <= 0 ) || (DISP_YMAX < data.Right_bottom.p_y) ||
        (data.Left_top.p_y == data.Right_bottom.p_y))
    {
        ercd = -1;
        goto EXIT;
    }

    addr = (unsigned short *)((unsigned long)lcd_top_addr + (data.Left_top.p_y * LCD_ONELINE_OFFSET) + (data.Left_top.p_x * 2) );
    save_addr = addr;
    for (p_y = data.Left_top.p_y; p_y < data.Right_bottom.p_y; p_y++)
    {
        if ((p_y == data.Left_top.p_y) || (p_y == data.Right_bottom.p_y))
        {
            for (p_x = data.Left_top.p_x; p_x < data.Right_bottom.p_x; p_x++)
            {
                *addr++ = data.Color;
            }
        }
        else
        {
            *addr++ = data.Color;
            addr += ((data.Right_bottom.p_x - data.Left_top.p_x));
            *addr++ = data.Color;
        }
        save_addr = (unsigned short *)((unsigned long)save_addr + LCD_ONELINE_OFFSET);
        addr = save_addr;
    }
    ercd = 0;

EXIT:
    return (ercd);
}

long LCDTest_Driver::LCD_Bmp_Write (unsigned short *lcd_top_addr, LCD_BMP data)
{
    int             p_x, p_y;//, x_max, y_max;
    long            ercd;
    unsigned short  *addr, *save_addr, *color;

    if ((data.Left_top.p_x < 0) || (data.Left_top.p_y < 0) )
    {
        ercd = -1;
        goto EXIT;
    }
    if ((DISP_XMAX < (data.Left_top.p_x + data.X_Width)) ||
        (DISP_YMAX < (data.Left_top.p_y + data.Y_Height)))
    {
        ercd = -1;
        goto EXIT;
    }

    color = (unsigned short *)(data.data);
    addr = (unsigned short *)((unsigned long)lcd_top_addr + (data.Left_top.p_y * LCD_ONELINE_OFFSET) + (data.Left_top.p_x * 2));
    save_addr = addr;
    for (p_y = data.Left_top.p_y; p_y < (data.Left_top.p_y + data.Y_Height); p_y++)
    {
        for (p_x = data.Left_top.p_x; p_x < (data.Left_top.p_x + data.X_Width); p_x++)
        {
            *addr++ = *color++;
        }
        save_addr = (unsigned short *)((long)save_addr + LCD_ONELINE_OFFSET);
        addr = save_addr;
    }

    ercd = 0;

EXIT:
    return (ercd);
}

void LCDTest_Driver::wait (void)
{
    volatile int    i;
    for (i = 0; i < 0x00200000; i++);
}

void LCDTest_Driver::LCD_Sample1 (void)
{
    LCD_RECT    rect;
    int         i;
    
    LCD_Full_Write (HD66773R_Driver::GetScreenBuffer(), WHITE);
    HD66773R_Driver::Data_Trans (HD66773R_Driver::GetScreenBuffer());

    for( i = 0 ; i < 9 ; i++ )
    {
        rect.Left_top.p_x     = Sample1_data[i].l_x;
        rect.Left_top.p_y     = Sample1_data[i].l_y;
        rect.Right_bottom.p_x = Sample1_data[i].r_x;
        rect.Right_bottom.p_y = Sample1_data[i].r_y;
        rect.Color            = Sample1_data[i].color;
    
        LCD_Rect_Write (HD66773R_Driver::GetScreenBuffer(), rect);
        HD66773R_Driver::Data_Trans (HD66773R_Driver::GetScreenBuffer());
        wait();
    }
}

void LCDTest_Driver::LCD_Sample2 (int option)
{
    LCD_RECT    rect;
    int         i, ix, iy, dx, dy;
    int         Rx_size, Ry_size, color;

    color   = 0;
    Rx_size = 10;
    Ry_size = 10;
    dx      = 2;
    dy      = 3;
    ix      = (DISP_XMAX - Rx_size) ;
    iy      = (DISP_YMAX - Ry_size) ;

    rect.Left_top.p_x     = ix;
    rect.Left_top.p_y     = iy;
    rect.Right_bottom.p_x = ix + Rx_size;
    rect.Right_bottom.p_y = iy + Ry_size;
    rect.Color            = Color_Tbl[color];

    LCD_Full_Write (HD66773R_Driver::GetScreenBuffer(), WHITE);
    HD66773R_Driver::Data_Trans (HD66773R_Driver::GetScreenBuffer());
    
    for (i = 0; i < 1000; i++)
    {
        if (option == 0)
        {
            LCD_Frame_Write (HD66773R_Driver::GetScreenBuffer(), rect);
        }
        else
        {
            LCD_Rect_Write (HD66773R_Driver::GetScreenBuffer(), rect);
        }
        HD66773R_Driver::Data_Trans (HD66773R_Driver::GetScreenBuffer());

        rect.Left_top.p_x     += dx;
        rect.Left_top.p_y     += dy;
        rect.Right_bottom.p_x += dx;
        rect.Right_bottom.p_y += dy;
        if (rect.Left_top.p_x < DISP_XMIN)
        {
            dx          = -dx;
            color      += 1;
            if (color > 7)
            {
                color = 0;
            }
            rect.Color             = Color_Tbl[color];
            rect.Right_bottom.p_x -= rect.Left_top.p_x;
            rect.Right_bottom.p_x += DISP_XMIN;
            rect.Left_top.p_x      = DISP_XMIN;
        }
        if (rect.Left_top.p_y < DISP_YMIN)
        {
            dy          = -dy;
            color      += 1;
            if (color > 7)
            {
                color = 0;
            }
            rect.Color             = Color_Tbl[color];
            rect.Right_bottom.p_y -= rect.Left_top.p_y;
            rect.Right_bottom.p_y += DISP_YMIN;
            rect.Left_top.p_y      = DISP_YMIN;
        }
        if (rect.Right_bottom.p_x > DISP_XMAX)
        {
            dx          = -dx;
            color      += 1;
            if (color > 7)
            {
                color = 0;
            }
            rect.Color             = Color_Tbl[color];
            rect.Left_top.p_x     -= rect.Right_bottom.p_x;
            rect.Left_top.p_x     += DISP_XMAX;
            rect.Right_bottom.p_x  = DISP_XMAX;
        }
        if (rect.Right_bottom.p_y > DISP_YMAX)
        {
            dy          = -dy;
            color      += 1;
            if (color > 7)
            {
                color = 0;
            }
            rect.Color             = Color_Tbl[color];
            rect.Left_top.p_y     -= rect.Right_bottom.p_y;
            rect.Left_top.p_y     += DISP_YMAX;
            rect.Right_bottom.p_y  = DISP_YMAX;
        }
    }
}

