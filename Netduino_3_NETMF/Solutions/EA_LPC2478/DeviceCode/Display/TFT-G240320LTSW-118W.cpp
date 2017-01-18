//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include <tinyhal.h>
#include "TFT-G240320LTSW-118W.h"
#include "LPC24XX_LCDC.h"

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_G240320LTSW_Driver"
#endif

G240320LTSW_Driver g_G240320LTSW_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL G240320LTSW_Driver::Initialize()
{
     if ( g_G240320LTSW_Driver.m_initialized == TRUE) return TRUE;

    BOOL bRet = TRUE;

   // turn off everything first 

     // Turn off LCD (PC8)
    CPU_GPIO_EnableOutputPin( g_G240320LTSW_Config.LcdConfig.LcdEnable.Pin, !g_G240320LTSW_Config.LcdConfig.LcdEnable.ActiveState );

    LCD_Controller_Enable(FALSE);

    
    bRet = LCD_Controller_Initialize(g_G240320LTSW_Config.ControllerConfig);

    LPC24XX_LCDC_Driver::SetScreenBuffer(g_G240320LTSW_Config.LcdConfig.ScreenBuffer);

    Clear();
	
    LCD_Controller_Enable(TRUE);


    HAL_Time_Sleep_MicroSeconds(50000);

   
    Display_Init( g_G240320LTSW_Config.LcdConfig );


    // Turn on LCD
    CPU_GPIO_EnableOutputPin( g_G240320LTSW_Config.LcdConfig.LcdEnable.Pin, g_G240320LTSW_Config.LcdConfig.LcdEnable.ActiveState );

     g_G240320LTSW_Driver.m_initialized = TRUE;
    return bRet;
}

BOOL G240320LTSW_Driver::Uninitialize()
{    

    if ( g_G240320LTSW_Driver.m_initialized  == TRUE)
    {
          Clear();

          LCD_Controller_Enable(FALSE);

          // Turn off LCD (PC8)
    	   CPU_GPIO_EnableOutputPin( g_G240320LTSW_Config.LcdConfig.LcdEnable.Pin, !g_G240320LTSW_Config.LcdConfig.LcdEnable.ActiveState );
          g_G240320LTSW_Driver.m_initialized  = FALSE;
         return LCD_Controller_Uninitialize();
    }
    else
    {
    	 return TRUE;
    }
	
}

void G240320LTSW_Driver::PowerSave( BOOL On )
{
    return;
}

void G240320LTSW_Driver::Clear()
{
    GLOBAL_LOCK(irq);
    memset( LCD_GetFrameBuffer(), 0, SizeInBytes() );

    g_G240320LTSW_Driver.m_cursor = 0;
}

// Macro for retriving pixel value in 1-bit bitmaps
#define G240320LTSW_GETBIT(_x,_y,_data,_widthInWords) (((_data[((_x)/32) + (_y)*(_widthInWords)])>>((_x)%32)) & 0x1)
#define LCD_BITBLT_COLOR_0  0x0000
#define LCD_BITBLT_COLOR_1  0x9E79

void G240320LTSW_Driver::BitBlt( int width, int height, int widthInWords, UINT32 data[], BOOL fUseDelta )
{
    ASSERT(width  <= g_G240320LTSW_Config.ControllerConfig.Width );
    ASSERT(height <= g_G240320LTSW_Config.ControllerConfig.Height);

    UINT32* ScreenBuffer = LCD_GetFrameBuffer();

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x+=2)
        {
            UINT32 val = 0;
            if(G240320LTSW_GETBIT(x  ,y,data,widthInWords)) val |=          LCD_BITBLT_COLOR_1       ;
            else                                            val |=          LCD_BITBLT_COLOR_0       ;
            if(G240320LTSW_GETBIT(x+1,y,data,widthInWords)) val |= ((UINT32)LCD_BITBLT_COLOR_1) << 16;
            else                                            val |= ((UINT32)LCD_BITBLT_COLOR_0) << 16;

            ScreenBuffer[y*WidthInWords() + x/2] = val;
        }
    }
    
}

void G240320LTSW_Driver::BitBltEx( int x, int y, int width, int height, UINT32 data[] )
{
    // Make sure the region being redrawn is completely inside the screen
    ASSERT((x >= 0) && ((x+width)  <= g_G240320LTSW_Config.ControllerConfig.Width));
    ASSERT((y >= 0) && ((y+height) <= g_G240320LTSW_Config.ControllerConfig.Height));

    // Adjust x and width so they start out word-aligned
    UINT32  leftAdjustment = x % PixelsPerWord();
    x     -= leftAdjustment;
    width += leftAdjustment;


    // Set the starting addresses and size based on the clipping region
    UINT32  firstPixelAdj = (y * WidthInWords()) + (x / PixelsPerWord());
    UINT32* destAddr      = LCD_GetFrameBuffer() + firstPixelAdj;
    UINT32* srcAddr       = data                                  + firstPixelAdj;
    UINT32  widthInBytes  = (width + (PixelsPerWord() - 1)) / PixelsPerWord() * 4;

    // Copy away
    for(int i = 0; i < height; i++)
    {
        memcpy(destAddr, srcAddr, widthInBytes);
        destAddr += WidthInWords();
        srcAddr  += WidthInWords();
    }
    
}

void G240320LTSW_Driver::WriteChar( unsigned char c, int row, int col )
{
    // convert to LCD pixel coordinates
    row *= Font_Height();
    col *= Font_Width();

    if(row > (g_G240320LTSW_Config.ControllerConfig.Height - Font_Height())) return;
    if(col > (g_G240320LTSW_Config.ControllerConfig.Width  - Font_Width() )) return;

    const UINT8* font = Font_GetGlyph( c );

    UINT32* ScreenBuffer = LCD_GetFrameBuffer();

    for(int y = 0; y < Font_Height(); y++)
    {
        for(int x = 0; x < Font_Width(); x+=2)
        {
            UINT32 val = 0;
            // the font data is mirrored
            if(G240320LTSW_GETBIT(Font_Width() -  x   ,y,font,1)) val |= 0x7fff;
            if(G240320LTSW_GETBIT(Font_Width() - (x+1),y,font,1)) val |= 0x7fff0000;
            
            ScreenBuffer[(row+y)*WidthInWords() + (col+x)/2] = val;
        }
    }
}

void G240320LTSW_Driver::WriteFormattedChar( unsigned char c )
{

     if ( g_G240320LTSW_Driver.m_initialized == FALSE) return;

    if(c < 32)
    {
        switch(c)
        {
        case '\b':                      /* backspace, clear previous char and move cursor back */
            if((g_G240320LTSW_Driver.m_cursor % TextColumns()) > 0)
            {
                g_G240320LTSW_Driver.m_cursor--;
                LCD_WriteChar( ' ', g_G240320LTSW_Driver.m_cursor / TextColumns(), g_G240320LTSW_Driver.m_cursor % TextColumns() );
            }
            break;

        case '\f':                      /* formfeed, clear screen and home cursor */
            //LCD_Clear();
            g_G240320LTSW_Driver.m_cursor = 0;
            break;

        case '\n':                      /* newline */
            g_G240320LTSW_Driver.m_cursor += TextColumns();
            g_G240320LTSW_Driver.m_cursor -= (g_G240320LTSW_Driver.m_cursor % TextColumns());
            break;

        case '\r':                      /* carriage return */
            g_G240320LTSW_Driver.m_cursor -= (g_G240320LTSW_Driver.m_cursor % TextColumns());
            break;

        case '\t':                      /* horizontal tab */
            g_G240320LTSW_Driver.m_cursor += (Font_TabWidth() - ((g_G240320LTSW_Driver.m_cursor % TextColumns()) % Font_TabWidth()));
            // deal with line wrap scenario
            if((g_G240320LTSW_Driver.m_cursor % TextColumns()) < Font_TabWidth())
            {
                // bring the cursor to start of line
                g_G240320LTSW_Driver.m_cursor -= (g_G240320LTSW_Driver.m_cursor % TextColumns());
            }
            break;

        case '\v':                      /* vertical tab */
            g_G240320LTSW_Driver.m_cursor += TextColumns();
            break;

        default:
            debug_printf("Unrecognized control character in LCD_WriteChar");
            break;
        }
    }
    else
    {
        LCD_WriteChar( c, g_G240320LTSW_Driver.m_cursor / TextColumns(), g_G240320LTSW_Driver.m_cursor % TextColumns() );
        g_G240320LTSW_Driver.m_cursor++;
    }

    if(g_G240320LTSW_Driver.m_cursor >= (TextColumns() * TextRows()))
    {
        g_G240320LTSW_Driver.m_cursor = 0;
    }
}

UINT32 G240320LTSW_Driver::PixelsPerWord()
{
    return ((8*sizeof(UINT32)) / g_G240320LTSW_Config.ControllerConfig.BitsPerPixel);
}

UINT32 G240320LTSW_Driver::TextRows()
{
    return (g_G240320LTSW_Config.ControllerConfig.Height / Font_Height());
}

UINT32 G240320LTSW_Driver::TextColumns()
{
    return (g_G240320LTSW_Config.ControllerConfig.Width / Font_Width());
}

UINT32 G240320LTSW_Driver::WidthInWords()
{
    return ((g_G240320LTSW_Config.ControllerConfig.Width + (PixelsPerWord() - 1)) / PixelsPerWord());
}

UINT32 G240320LTSW_Driver::SizeInWords()
{
    return (WidthInWords() * g_G240320LTSW_Config.ControllerConfig.Height);
}

UINT32 G240320LTSW_Driver::SizeInBytes()
{
    return (SizeInWords() * sizeof(UINT32));
}

void G240320LTSW_Driver::Display_Init( G240320LTSW_LCD_CONFIG& LcdConfig )
{
    Write_SPI(LcdConfig, 0x00, 0x0001);
    HAL_Time_Sleep_MicroSeconds(20000);

    Write_SPI(LcdConfig, 0x03, 0xA2A4);
    Write_SPI(LcdConfig, 0x0C, 0x0004);
    Write_SPI(LcdConfig, 0x0D, 0x0308);
    Write_SPI(LcdConfig, 0x0E, 0x3000);
    HAL_Time_Sleep_MicroSeconds(50000);

    Write_SPI(LcdConfig, 0x1E, 0x00AF);

    Write_SPI(LcdConfig, 0x01, 0x693f);

    Write_SPI(LcdConfig, 0x02, 0x0600);
    Write_SPI(LcdConfig, 0x10, 0x0000);
    Write_SPI(LcdConfig, 0x07, 0x0233);
    Write_SPI(LcdConfig, 0x0B, 0x0039);
    Write_SPI(LcdConfig, 0x0F, 0x0000);
    HAL_Time_Sleep_MicroSeconds(50000);
    
    Write_SPI(LcdConfig, 0x30, 0x0707);
    Write_SPI(LcdConfig, 0x31, 0x0204);
    Write_SPI(LcdConfig, 0x32, 0x0204);
    Write_SPI(LcdConfig, 0x33, 0x0502);
    Write_SPI(LcdConfig, 0x34, 0x0507);
    Write_SPI(LcdConfig, 0x35, 0x0204);
    Write_SPI(LcdConfig, 0x36, 0x0204);
    Write_SPI(LcdConfig, 0x37, 0x0502);
    Write_SPI(LcdConfig, 0x3A, 0x0302);
    Write_SPI(LcdConfig, 0x3B, 0x0302);
    Write_SPI(LcdConfig, 0x23, 0x0000);
    Write_SPI(LcdConfig, 0x24, 0x0000);
    Write_SPI(LcdConfig, 0x48, 0x0000);
    Write_SPI(LcdConfig, 0x49, 0x013F);
    Write_SPI(LcdConfig, 0x4A, 0x0000);
    Write_SPI(LcdConfig, 0x4B, 0x0000);
    Write_SPI(LcdConfig, 0x41, 0x0000);
    Write_SPI(LcdConfig, 0x42, 0x0000);
    Write_SPI(LcdConfig, 0x44, 0xEF00);
    Write_SPI(LcdConfig, 0x45, 0x0000);
    Write_SPI(LcdConfig, 0x46, 0x013F);
    HAL_Time_Sleep_MicroSeconds(50000);
  
    Write_SPI(LcdConfig, 0x44, 0xEF00);
    Write_SPI(LcdConfig, 0x45, 0x0000);
    Write_SPI(LcdConfig, 0x4E, 0x0000);
    Write_SPI(LcdConfig, 0x4F, 0x0000);
    Write_SPI(LcdConfig, 0x46, 0x013F);
}
  
void G240320LTSW_Driver::Write_SPI ( G240320LTSW_LCD_CONFIG& LcdConfig, UINT16 addr, UINT16 data )
{
    UINT8 SPIData[2];

    CPU_GPIO_EnableOutputPin(LcdConfig.Data_or_Cmd, FALSE);
  
    SPIData[0] = 0x0;
    SPIData[1] = addr & 0xff;
    CPU_SPI_nWrite8_nRead8 (LcdConfig.SPI_Config, SPIData, 2, 0, 0, 0);

    CPU_GPIO_SetPinState(LcdConfig.Data_or_Cmd, TRUE);
    SPIData[0] = data >> 8;
    SPIData[1] = data & 0xff;
    CPU_SPI_nWrite8_nRead8 (LcdConfig.SPI_Config, SPIData, 2, 0, 0, 0);

    CPU_GPIO_SetPinState(LcdConfig.Data_or_Cmd, FALSE);
    SPIData[0] = 0x0;
    SPIData[1] = 0x22;
    CPU_SPI_nWrite8_nRead8 (LcdConfig.SPI_Config, SPIData, 2, 0, 0, 0);
}
