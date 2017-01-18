/************************************************************/
/*  FileName    : SH7619_LCD.h                              */
/*  FUNC        : HD66773 Register Header File              */
/************************************************************/

/*
 *    Copyright (C) Renesas Technology America,Ltd. 2009  All rights reserved.
 */
 
#ifndef _SH7619_LCD_H_
#define _SH7619_LCD_H_

// LCD Size Defines
#define PHYSICAL_XMAX 132
#define PHYSICAL_YMAX 176

#define DISP_XMIN   (0) 
#define DISP_XMAX   (128)  
#define DISP_YMIN   (0) 
#define DISP_YMAX   (160)  

#define LCD_ONELINE_OFFSET  (DISP_XMAX *2)

// Colors
#define RED     (0xF800)
#define GREEN   (0x07E0)
#define BLUE    (0x001F)
#define WHITE   (0xFFFF)
#define BLACK   (0x0000)
#define YELLOW  (0xFFE0)
#define CYAN    (0x07FF)
#define MAGENTA (0xF81F)


// Drawing Structures
typedef struct point
{
    short   p_x;
    short   p_y;
} POINT;

typedef struct lcd_dot
{
    POINT           point;
    unsigned short  Color;
} LCD_DOT;

typedef struct lcd_rect
{
    POINT           Left_top;
    POINT           Right_bottom;
    unsigned short  Color;
} LCD_RECT;

typedef struct lcd_bmp
{
    POINT           Left_top;
    short           X_Width;
    short           Y_Height;
    unsigned short  *data;
} LCD_BMP;


const unsigned short    Color_Tbl[8] =
{
    RED,
    GREEN,
    BLUE,
    WHITE,
    BLACK,
    YELLOW,
    CYAN,
    MAGENTA
};

struct data_rect
{
    short           l_x;
    short           l_y;
    short           r_x;
    short           r_y;
    unsigned short  color;
};

const struct data_rect  Sample1_data[9] =
{
    {  0,  0,  64,  80, RED     },
    {  0, 80,  64, 160, GREEN   },
    { 64,  0, 128,  80, BLUE    },
    { 64, 80, 128, 160, BLACK   },
    { 32, 40,  96, 120, WHITE   },
    {  0,  0,  64,  80, YELLOW  },
    {  0, 80,  64, 160, CYAN    },
    { 64,  0, 128,  80, MAGENTA },
    { 64, 80, 128, 160, WHITE   },
};


// HD6673R Register Definitions
#define OSCILLATION_REG         (0x0000)
#define DRVR_OUTPUT_CTRL_REG    (0x0001)
#define LCD_DRV_CTRL_REG        (0x0002)
#define PWR1_CTRL_REG           (0x0003)
#define PWR2_CTRL_REG           (0x0004)
#define ENTRY_MODE_REG          (0x0005)
#define COMPARE_REG             (0x0006)
#define DISP_CTRL_REG           (0x0007)

#define FRAME_CYC_CTRL_REG      (0x000B)
#define PWR3_CTRL_REG           (0x000C)
#define PWR4_CTRL_REG           (0x000D)
#define PWR5_CTRL_REG           (0x000E)
#define GATE_SCAN_POS_REG       (0x000F)

#define V_SCROLL_CTRL_REG       (0x0011)

#define DISPLAY1_DRV_POS_REG    (0x0014)
#define DISPLAY2_DRV_POS_REG    (0x0015)
#define H_RAM_ADDR_POS_REG      (0x0016)
#define V_RAM_ADDR_POS_REG      (0x0017)

#define RAM_WR_DATA_MASK_REG    (0x0020)
#define RAM_ADDR_SET_REG        (0x0021)
#define RAM_WR_DATA_REG         (0x0022)
#define RAM_RD_DATA_REG         (0x0022)

#define G_CTRL1_REG             (0x0030)
#define G_CTRL2_REG             (0x0031)
#define G_CTRL3_REG             (0x0032)
#define G_CTRL4_REG             (0x0033)
#define G_CTRL5_REG             (0x0034)
#define G_CTRL6_REG             (0x0035)
#define G_CTRL7_REG             (0x0036)
#define G_CTRL8_REG             (0x0037)
#define G_CTRL9_REG             (0x003A)
#define G_CTRL10_REG            (0x003B)

struct LCDTest_Driver
{
    
    //Draw functions
    static void LCD_Dot_Write (unsigned short *lcd_top_addr, unsigned short dot_x, unsigned short dot_y, unsigned short color);
    static long LCD_Full_Write (unsigned short *lcd_top_addr, unsigned short color);
    static long LCD_Rect_Write (unsigned short *lcd_top_addr, LCD_RECT data);
    static long LCD_Frame_Write (unsigned short *lcd_top_addr, LCD_RECT data);
    static long LCD_Bmp_Write (unsigned short *lcd_top_addr, LCD_BMP data);
    static void LCD_Sample1(void);
    static void LCD_Sample2(int option);
    static void LCD_Sample3(void);
    static void wait(void);    
};


//extern LCDTest_Driver gLCDTest_Driver;
#endif
