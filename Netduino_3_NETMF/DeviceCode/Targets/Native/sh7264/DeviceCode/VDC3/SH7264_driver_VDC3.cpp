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
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#include <tinyhal.h>
#include "SH7264_VDC3.h"
#include "SH7264_QVGA_LCD.h"


BOOL SH7264_VDC3_Driver::Initialize(SH7264VDC3_CONTROLLER_CONFIG& config)
{   
    //Select Pins for VDC3 in Video Display Mode
    PORT.PGCR5.WORD = (PORT.PGCR5.WORD & 0xFFF0) | 0x1;
    PORT.PGCR4.WORD = 0x1111;
    PORT.PGCR3.WORD = 0x1111;
    PORT.PGCR2.WORD = 0x1111;
    PORT.PGCR1.WORD = 0x1111;
    PORT.PGCR0.WORD = 0x1110;
    PORT.PGCR7.WORD = 0x5a01;

    //Select Pins for VDC3 in Video Decording Mode
    PORT.PECR1.WORD = 0033; 
    PORT.PFCR2.WORD = (PORT.PFCR2.WORD & 0xFFF0) | 0x0003;  
    PORT.PFCR1.WORD = 0x3333;
    PORT.PFCR0.WORD = 0x3333;

    //Enable supply clock to VDC3
    CPG.STBCR7.BYTE &= ~(0x10);
    
    VDC3_Panel_Initialize(g_SH7264VDC3_Config.Width, g_SH7264VDC3_Config.Height);       /* Init LCD/CRT panel */
        
    // Initializes graphic block 1 with alpha control area = video size 
    VDC3_Display_Initialize(VDC3_GRA1_BASE_ADDRESS );
    
    //Initialize DAC
#ifdef SH7264_RSK_QVGA_LCD  
    SH7264_QVGA_LCD_Driver::QVGA_LCD_DAC_Initialize();  
#endif
    
    Buffer = (unsigned short * )VDC3_GRA1_BASE_ADDRESS;
    
    VDC3_ClearScreen(g_SH7264VDC3_Config.Width, g_SH7264VDC3_Config.Height);
    
    Buffer = (unsigned short * )VDC3_GRA1_BASE_ADDRESS;
                               
    return TRUE;
}

void SH7264_VDC3_Driver::VDC3_ClearScreen(unsigned short width, unsigned short height)
{
    int p_x, p_y;
    
    for (p_y = 0; p_y < width; p_y++)
    {
        for (p_x = 0; p_x < height; p_x++)
        {
            *Buffer++ = WHITE;
        }
    }
    
    VDC3_Display_Start(0, 1, 0);
}

BOOL SH7264_VDC3_Driver::Uninitialize()
{   
    // Reset Registers
    VDC3.VIDEO_INT_CNT.LONG = 0x00000000;
    VDC3.PANEL_CLKSEL.LONG  = 0x00000000;
    VDC3.GROPEWH1.LONG      = 0x00000000;
    VDC3.GROPEDPHV1.LONG    = 0x00000000;
    VDC3.GROPEWH2.LONG      = 0x00000000;
    VDC3.GROPEDPHV2.LONG    = 0x00000000;
    VDC3.GROPCRKY0_1.LONG   = 0x00000000;
    VDC3.GROPCRKY0_2.LONG   = 0x00000000;
    VDC3.VIDEO_MODE.LONG    = 0x00000000;   

    //Disable the Alpha control area and Chroma-Keying
    VDC3.GROPEDPA1.LONG &= ~VDC3_GROPEDPA_AEN
                        &  ~VDC3_GROPEDPA_WE;
    VDC3.GROPCRKY0_1.LONG = VDC3.GROPCRKY0_1.LONG & ~(1<<16);
    VDC3.GROPEDPA2.LONG &= ~VDC3_GROPEDPA_AEN
                        &  ~VDC3_GROPEDPA_WE;
    VDC3.GROPCRKY0_2.LONG = VDC3.GROPCRKY0_2.LONG & ~(1<<16);

    //Disable graphic blocks display
    VDC3.GRCMEN1.LONG   = 0x00000000;
    VDC3.GRCMEN2.LONG   = 0x00000000;
    VDC3.SGMODE.LONG    = 0x00000000;

    //Stop VDC3 intterrupt
    INTC.IPR10.WORD = INTC.IPR10.WORD & 0xF0FF;
    VDC3.SGINTCNT.LONG      =  0x00000000;
        
    return TRUE;
}

BOOL SH7264_VDC3_Driver::Enable(BOOL fEnable)
{
    if(fEnable)
    {
        VDC3_Display_Start(0, 1, 0);
    }
    else
    {
        VDC3_Display_Start(0, 0, 0);
    }

    return TRUE;
}

unsigned short* SH7264_VDC3_Driver::GetFrameBuffer()
{
    return Buffer;  
}

void SH7264_VDC3_Driver::VDC3_Display_Initialize(unsigned long gra_buf)
{

    //Settings for Starting Graphic Display of Block 1
    VDC3.GRCBUSCNT1.LONG = VDC3.GRCBUSCNT1.LONG & ~(VDC3_GRCBUSCNT1_BUS_FORMAT | VDC3_GRCBUSCNT1_ENDIAN);
    
    VDC3.GROPSADR1.LONG = gra_buf;

    
    VDC3.GROPSOFST1.LONG = (UINT32)( 2* g_SH7264VDC3_Config.Width);
    VDC3.GROPSWH1.LONG = ((UINT32)(g_SH7264VDC3_Config.Height << 16)) | g_SH7264VDC3_Config.Width;


    VDC3.GROPDPHV1.LONG = (g_SH7264VDC3_Config.GraVpos << 16) | g_SH7264VDC3_Config.GraHpos;
        
    //Settings for Alpha Control Area Display
    if (g_SH7264VDC3_Config.EnableAlpha == TRUE)
    {
        VDC3.GROPEWH1.LONG = (g_SH7264VDC3_Config.AlphaHeight << 16) | g_SH7264VDC3_Config.AlphaWidth;
        VDC3.GROPEDPHV1.LONG = (g_SH7264VDC3_Config.AlphaVpos << 16) | g_SH7264VDC3_Config.AlphaHpos;
        VDC3.GROPEDPA1.LONG = ((VDC3.GROPEDPA1.LONG & 0x000000FF) | 0x00FF0100) & (~0x0000006);
    }

    // Enables or disables alpha control 
    VDC3.GROPEDPA1.LONG = (VDC3.GROPEDPA1.LONG & ~0x00000001) | g_SH7264VDC3_Config.EnableAlpha;
        
    //Settings for Chroma-Key Control
    if (g_SH7264VDC3_Config.EnableChromaKey ==TRUE)
    {
        VDC3.GROPCRKY0_1.LONG |= g_SH7264VDC3_Config.RepColor | (1 << 16);
        VDC3.GROPCRKY1_1.LONG =  (VDC3.GROPCRKY1_1.LONG & 0xFF00FFFF) |  g_SH7264VDC3_Config.RepColor;
    }
    
    //Enables or disables chroma-key processing
    VDC3.GROPCRKY0_1.LONG = (VDC3.GROPCRKY0_1.LONG & ~(1 << 16)) | (g_SH7264VDC3_Config.EnableChromaKey << 16);
    
    //Color for Outside of Graphic Image Area = Black color
    VDC3.GROPBASERGB1.LONG = VDC3.GROPBASERGB1.LONG & 0xFFFF0000;
    
}

void SH7264_VDC3_Driver::VDC3_Panel_Initialize(unsigned short panel_width, unsigned short panel_height)
{       

    if (g_SH7264VDC3_Config.EnableTFT == TRUE)
    {
        // lcd mode
        VDC3.PANEL_CLKSEL.LONG = VDC3.PANEL_CLKSEL.LONG  & ~VDC3_PANEL_CLKSEL_ICKEN;

    }
    else 
    {
        // CRT mode 
        VDC3.PANEL_CLKSEL.LONG = VDC3.PANEL_CLKSEL.LONG & (~VDC3_PANEL_CLKSEL_ICKEN);
       
   }
   
    UINT32 val =0;

    if (g_SH7264VDC3_Config.LcdDataEdge ) val |= VDC3_SYNCNT_RGB_TIM;
    if (g_SH7264VDC3_Config.LcdVSyncEdge) val |= VDC3_SYNCNT_VSYNC_TIM;
    if (g_SH7264VDC3_Config.LcdHSyncEdge) val |= VDC3_SYNCNT_HSYNC_TIM;
    if (g_SH7264VDC3_Config.LcdDeEdge)    val |= VDC3_SYNCNT_DE_TIM;
    if (g_SH7264VDC3_Config.LcdMDispEdge) val |= VDC3_SYNCNT_M_DISP_TIM;

    if (g_SH7264VDC3_Config.LcdVSyncType) val |= VDC3_SYNCNT_VSYNC_TYPE;
    if (g_SH7264VDC3_Config.LcdHSyncType) val |= VDC3_SYNCNT_HSYNC_TYPE;
    if (g_SH7264VDC3_Config.LcdDeType)    val |= VDC3_SYNCNT_DE_TYPE;
    if (g_SH7264VDC3_Config.LcdMDispType) val |= VDC3_SYNCNT_M_DISP_TYPE;       

    VDC3.SYNCNT.LONG = val;


    val = VDC3.PANEL_CLKSEL.LONG ;
    if (g_SH7264VDC3_Config.ClockSelectEnable) val |= (VDC3_PANEL_CLKSEL_ICKSEL | g_SH7264VDC3_Config.PixelClockDivider);

    VDC3.PANEL_CLKSEL.LONG = val;

    VDC3.SYN_SIZE.LONG = (g_SH7264VDC3_Config.VerticalBlankingInterval <<16) | g_SH7264VDC3_Config.HorizontalBlankingInterval;

    VDC3.PANEL_VSYNC_TIM.LONG = (g_SH7264VDC3_Config.VerticalSyncStart<<16)  | g_SH7264VDC3_Config.VerticalSyncEnd;
    VDC3.PANEL_HSYNC_TIM.LONG = (g_SH7264VDC3_Config.HorizontalSyncStart<<16)| g_SH7264VDC3_Config.HorizontalSyncEnd;

    VDC3.DE_SIZE.LONG = (panel_height << 16) | panel_width;
    VDC3.DE_START.LONG = (g_SH7264VDC3_Config.DE_Start_V <<16)| g_SH7264VDC3_Config.DE_Start_H;
    VDC3.AC_LINE_NUM.LONG = g_SH7264VDC3_Config.AcLineNum;
    
    VDC3.GRA_VSYNC_TIM.LONG = ((VDC3.DE_START.LONG & 0x03FF0000) >> 16) + g_SH7264VDC3_Config.GraVpos - 1;
    
}

void SH7264_VDC3_Driver::VDC3_Display_Start(int video_ena, int gra1_ena, int gra2_ena)
{   
    if(gra1_ena)
    {
        //Enables graphics display
        VDC3.GRCMEN1.LONG |= VDC3_GRCMEN_DEN;
    }
        
    if(gra2_ena)
    {   
        // Enables graphics display
        VDC3.GRCMEN2.LONG |= VDC3_GRCMEN_DEN;
    }

    // Begin Transfer
    VDC3.GRCMEN1.LONG |= VDC3_GRCMEN_WE
                      |  VDC3_GRCMEN_VEN;
    VDC3.GRCMEN2.LONG |= VDC3_GRCMEN_WE
                      |  VDC3_GRCMEN_VEN;                 

    if (video_ena)
    {
        //Starts video supply to the graphics blocks
        VDC3.VIDEO_MODE.LONG |= 0x2;
    }

    VDC3.GROPEDPA1.LONG |= 0x00000080;
    VDC3.GROPEDPA2.LONG |= 0x00000080;

    // Panel clock Enable
    VDC3.PANEL_CLKSEL.LONG |= VDC3_PANEL_CLKSEL_ICKEN;
}

