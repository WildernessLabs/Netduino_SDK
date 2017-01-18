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

#include "tinyhal.h"

//--//

#ifndef _DRIVERS_DISPLAY_G240320LTSW_H_
#define _DRIVERS_DISPLAY_G240320LTSW_H_ 1


//////////////////////////////////////////////////////////////////////////////
struct G240320LTSW_LCD_CONFIG
{
    UINT32 * ScreenBuffer;
    GPIO_PIN Data_or_Cmd;
    GPIO_FLAG LcdEnable;
    SPI_CONFIGURATION   SPI_Config;
};

struct G240320LTSW_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    G240320LTSW_LCD_CONFIG LcdConfig;

    DISPLAY_CONTROLLER_CONFIG ControllerConfig;

    static LPCSTR GetDriverName() { return "G240320LTSW"; }
};

extern G240320LTSW_CONFIG g_G240320LTSW_Config;

//////////////////////////////////////////////////////////////////////////////

struct G240320LTSW_Driver
{
    //--//

    UINT32 m_cursor;
    UINT32 m_initialized;

    //--//
    
    static BOOL Initialize();

    static BOOL Uninitialize();

    static void PowerSave( BOOL On );

    static void Clear();

    static void BitBltEx( int x, int y, int width, int height, UINT32 data[] );

    static void BitBlt( int width, int height, int widthInWords, UINT32 data[], BOOL fUseDelta );

    static void WriteChar         ( unsigned char c, int row, int col );
    
    static void WriteFormattedChar( unsigned char c                   );

	static void   Display_Init( G240320LTSW_LCD_CONFIG& LcdConfig );
	
private:
    static UINT32 PixelsPerWord();
    static UINT32 TextRows();
    static UINT32 TextColumns();
    static UINT32 WidthInWords();
    static UINT32 SizeInWords();
    static UINT32 SizeInBytes();
    static void   Write_SPI ( G240320LTSW_LCD_CONFIG& LcdConfig, UINT16 addr, UINT16 data );
};

extern G240320LTSW_Driver g_G240320LTSW_Driver;


#endif  // _DRIVERS_DISPLAY_G240320LTSW_H_
