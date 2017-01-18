/************************************************************/
/*  FileName    : SH7264_VDC3.h                             */
/*  FUNC        : HD66773 Register Header File              */
/************************************************************/

/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */
 
#ifndef _SH7264_VDC3_H_
#define _SH7264_VDC3_H_

//#include "..\Include\iodefine.h"

//#define CRT_MONITOR 0

#ifdef CRT_MONITOR
    #define LEFT_STARTING_POINT 85
#else
    #define LEFT_STARTING_POINT 0
#endif

#define CAPTURE_BANK0   (0x1C0A0000)

#ifdef USE_CACHE
#define DISPLAY_BANK0   (0x1C000000)
#define DISPLAY_BANK1   (0x1C000000 + (VIDEO_INPUT_WIDTH*(VIDEO_INPUT_HEIGHT)*2))
#else
#define DISPLAY_BANK0   (0x3C000000)
#define DISPLAY_BANK1   (0x3C000000 + (VIDEO_INPUT_WIDTH*(VIDEO_INPUT_HEIGHT)*2))
#endif

/* Buffers Addresses */
//#define VDC3_GRA1_BASE_ADDRESS  0x3C000000
#define VDC3_GRA1_BASE_ADDRESS	0x3C0DA800  //QVGA size (25800)

#define VDC3_GRA2_BASE_ADDRESS  0x0C900000
#define VDC3_GRA22_BASE_ADDRESS 0x0CA00000
#define VDC3_VIDEO_BASE_ADDRESS CAPTURE_BANK0

/* panel size */
#define VDC3_PANEL_VGA_WIDTH                640
#define VDC3_PANEL_VGA_HEIGHT               480
#define VDC3_PANEL_WQVGA_WIDTH              480
#define VDC3_PANEL_WQVGA_HEIGHT             240
#define VDC3_PANEL_QVGA_LANDSCAPE_WIDTH     320
#define VDC3_PANEL_QVGA_LANDSCAPE_HEIGHT    240
#define VDC3_PANEL_QVGA_PORTRAIT_WIDTH      240
#define VDC3_PANEL_QVGA_PORTRAIT_HEIGHT     320

/* Constants of GRCMEN */
#define VDC3_GRCMEN_WE              (1 << 31)
#define VDC3_GRCMEN_DEN             (1 << 1)
#define VDC3_GRCMEN_VEN             (1 << 0)

#define VDC3_GROPEDPA_WE            (1 << 7)
#define VDC3_GROPEDPA_AEN           (1 << 0)

/* Constants of Sync Signal Control Register (SYNCNT)*/
#define VDC3_SYNCNT_RGB_TIM         (1 << 24)
#define VDC3_SYNCNT_VSYNC_TIM       (1 << 19)
#define VDC3_SYNCNT_HSYNC_TIM       (1 << 18)
#define VDC3_SYNCNT_DE_TIM          (1 << 17)
#define VDC3_SYNCNT_M_DISP_TIM      (1 << 16)
#define VDC3_SYNCNT_VSYNC_TYPE      (1 << 3)
#define VDC3_SYNCNT_HSYNC_TYPE      (1 << 2)
#define VDC3_SYNCNT_DE_TYPE         (1 << 1)
#define VDC3_SYNCNT_M_DISP_TYPE     (1 << 0)

/* Constants of PANEL_CLKSEL */
#define VDC3_PANEL_CLKSEL_ICKEN     (1 << 12)
#define VDC3_PANEL_CLKSEL_ICKSEL    (1 << 13)

/* Constants of GRCBUSCNT1 */
#define VDC3_GRCBUSCNT1_BURST_MODE  (1 << 8)
#define VDC3_GRCBUSCNT1_BUS_FORMAT  (1 << 4)
#define VDC3_GRCBUSCNT1_ENDIAN      (1 << 0)

// Colors
#define RED     (0xF800)
#define GREEN   (0x07E0)
#define BLUE    (0x001F)
#define WHITE   (0xFFFF)
#define BLACK   (0x0000)
#define YELLOW  (0xFFE0)
#define CYAN    (0x07FF)
#define MAGENTA (0xF81F)
#define PINK    (0xFE80)


static unsigned short * Buffer = (unsigned short * )VDC3_GRA1_BASE_ADDRESS;


#define VGA_NUM_RECT                987
#define QVGA_LANDSCAPE_NUM_RECT     253
#define QVGA_PORTRAIT_NUM_RECT      253
#define WQVGA_NUM_RECT              407


struct SH7264VDC3_CONTROLLER_CONFIG
{
    UINT32 Width;
    UINT32 Height;
 
    BOOL EnableAlpha;
    BOOL EnableChromaKey;
    BOOL EnableTFT;
    BOOL ClockSelectEnable;

    UINT8 PixelClockDivider;
    UINT8 BusWidth;
    UINT8 BitsPerPixel;
    UINT8 Orientation;

    UINT32 LcdDataEdge;      // falling edge = 1     

    UINT8 LcdVSyncEdge;     // falling edge = 1
    UINT8 LcdHSyncEdge;     // falling edge = 1
    UINT8 LcdDeEdge;        // falling edge = 1
    UINT8 LcdMDispEdge;     // falling edge = 1

    UINT8 LcdVSyncType;     // Inverted = 1; No inverted = 0
    UINT8 LcdHSyncType;     // Inverted = 1; No inverted = 0
    UINT8 LcdDeType;        // Inverted = 1; No inverted = 0
    UINT8 LcdMDispType;     // Inverted = 1; No inverted = 0

    UINT8  HorizontalSyncStart;
    UINT8  HorizontalSyncEnd;
    UINT16 HorizontalBlankingInterval;


    UINT8  VerticalSyncStart;
    UINT8  VerticalSyncEnd;
    UINT16 VerticalBlankingInterval;

    UINT16 DE_Start_H;
    UINT16 DE_Start_V;

    UINT16 AcLineNum;
    UINT16 GraVsyncTim;

//    UINT32 GraWidth;
//    UINT32 GraHeight;
    UINT16 GraHpos;
    UINT16 GraVpos;


    UINT16 AlphaWidth;
    UINT16 AlphaHeight;
    UINT16 AlphaHpos;
    UINT16 AlphaVpos;

    UINT16 RepColor;
     
};


extern SH7264VDC3_CONTROLLER_CONFIG g_SH7264VDC3_Config;

struct SH7264_VDC3_Driver
{
    static BOOL Initialize(SH7264VDC3_CONTROLLER_CONFIG& config);
    static BOOL Uninitialize();
    static BOOL Enable( BOOL fEnable );
    static unsigned short* GetFrameBuffer();    
    static void VDC3_Panel_Initialize(unsigned short panel_width, unsigned short panel_height);
    static void VDC3_Display_Initialize(unsigned long gra_buf);                                  
    static void VDC3_Display_Start(int video_ena, int gra1_ena, int gra2_ena); 
    static void VDC3_ClearScreen(unsigned short width, unsigned short height);      
};

#endif //_SH7264_VDC3_H_
