////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-- Stream --//

#define  STREAM__OUT       STREAM__SERIAL

//--  RamTest --//

#define  BUS_WIDTH         0x00008000
#define  RAMTestBase       0x08400000
#define  RAMTestSize       0x00010000
#define  ENDIANESS         LE_ENDIAN

//--  Serial Port  --//

#define  COMTestPort       COM1

//--  GPIO --//
    
#define  GPIOTestPin       GPIO_PIN_NONE

//-- SPI --//

#define  SPIChipSelect     GPIO_PIN_NONE
#define  SPIModule         GPIO_PIN_NONE

//-- Timer --//

#define  DisplayInterval    5
#define  TimerDuration      30

//--//



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

typedef struct lcd_bmp
{
    POINT           Left_top;
    long            X_Width;
    long            Y_Height;
    unsigned short  *data;
} LCD_BMP;


typedef struct lcd_rect
{
    POINT           Left_top;
    POINT           Right_bottom;
    unsigned short  Color;
} LCD_RECT;

struct data_rect
{
    long            l_x;
    long            l_y;
    long            r_x;
    long            r_y;
    unsigned short  color;
};
const unsigned short Color_Tbl[8] =
{
    RED,
    GREEN,
    BLUE,
    PINK,
    BLACK,
    YELLOW,
    CYAN,
    MAGENTA
};
const struct data_rect  VGA_SolidRect_data[9] =
{
    {   0,   0, 320, 240, RED     },
    { 320,   0, 640, 240, GREEN   },    
    {   0, 240, 320, 480, BLUE    },
    { 320, 240, 640, 480, BLACK   },
    { 160, 120, 480, 360, WHITE   },
    {   0,   0, 320, 240, YELLOW  },
    { 320,   0, 640, 240, CYAN    },
    {   0, 240, 320, 480, MAGENTA },
    { 320, 240, 640, 480, WHITE   },
};

const struct data_rect  QVGA_LANDSCAPE_SolidRect_data[9] =
{
    {   0,   0, 160, 120, RED     },
    { 160,   0, 320, 120, GREEN   },    
    {   0, 120, 160, 240, BLUE    },
    { 160, 120, 320, 240, BLACK   },
    {  80,  60, 240, 180, WHITE   },
    {   0,   0, 160, 120, YELLOW  },
    { 160,   0, 320, 120, CYAN    },
    {   0, 120, 160, 240, MAGENTA },
    { 160, 120, 320, 240, WHITE   },
};

const struct data_rect  QVGA_PORTRAIT_SolidRect_data[9] =
{
    {   0,   0, 120, 160, RED     },
    { 120,   0, 240, 160, GREEN   },    
    {   0, 160, 120, 320, BLUE    },
    { 120, 160, 240, 320, BLACK   },
    {  60,  80, 180, 240, WHITE   },
    {   0,   0, 120, 160, YELLOW  },
    { 120,   0, 240, 160, CYAN    },
    {   0, 160, 120, 320, MAGENTA },
    { 120, 160, 240, 320, WHITE   },
};

const struct data_rect  WQVGA_SolidRect_data[9] =
{
    {   0,   0, 240, 120, RED     },
    { 240,   0, 480, 120, GREEN   },    
    {   0, 120, 240, 240, BLUE    },
    { 240, 120, 480, 240, BLACK   },
    {  120, 60, 360, 180, WHITE   },
    {   0,   0, 240, 120, YELLOW  },
    { 240,   0, 480, 120, CYAN    },
    {   0, 120, 240, 240, MAGENTA },
    { 240, 120, 480, 240, WHITE   },
};


//--//



