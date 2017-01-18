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

 #include "LPC24XX_LCDC.h"

//--//

BOOL LCD_Controller_Initialize(DISPLAY_CONTROLLER_CONFIG& config)
{
    return LPC24XX_LCDC_Driver::Initialize( config );
}

BOOL LCD_Controller_Uninitialize()
{
    return LPC24XX_LCDC_Driver::Uninitialize();
}

BOOL LCD_Controller_Enable(BOOL fEnable)
{
    return LPC24XX_LCDC_Driver::Enable( fEnable );
}

UINT32* LCD_GetFrameBuffer()
{
    return LPC24XX_LCDC_Driver::GetScreenBuffer();
}

