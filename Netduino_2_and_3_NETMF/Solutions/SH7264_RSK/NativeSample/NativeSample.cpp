////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */

#include <tinyhal.h>
#include <Tests.h>

#include "..\..\..\DeviceCode\Targets\Native\SH7264\DeviceCode\vdc3\SH7264_vdc3.h"

#include "nativesample.h"
#include "SH7264_bitmap.h"

#include "..\..\..\DeviceCode\Drivers\FS\FAT\FAT_FS.h"
#include "..\..\..\DeviceCode\Targets\Native\SH7264\DeviceCode\Include\SH7264_VDC3_decl.h"
#include "..\..\..\DeviceCode\Targets\Native\SH7264\DeviceCode\GPIO\SH7264_GPIO.h"
#include "..\..\..\DeviceCode\Targets\Native\SH7264\DeviceCode\Keypad\SH7264_Keypad.h"

//Added NOR flash memory test
extern struct BlockStorageDevice  g_SH7264_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_BS_Config_SH7264_RSK;
BlockStorageDevice  * pBlockStorageDevice_NOR = &g_SH7264_BS;
BLOCK_CONFIG        * pBLOCK_CONFIG_NOR         = &g_BS_Config_SH7264_RSK;

//Added SERIAL flash memory test
extern struct BlockStorageDevice  g_SerialFlash_Device;
extern struct IBlockStorageDevice g_SerialFlash_BL_DeviceTable;
extern struct BLOCK_CONFIG        g_SerialFlash_BL_Config;
BlockStorageDevice  * pBlockStorageDevice_SERIAL    = &g_SerialFlash_Device;
BLOCK_CONFIG        * pBLOCK_CONFIG_SERIAL          = &g_SerialFlash_BL_Config;




extern SH7264_Keypad_Driver g_SH7264_Keypad_Driver;

//VDC3 size
#define VDC3_VGA_WIDTH                  640
#define VDC3_VGA_HEIGHT                 480
#define VDC3_WQVGA_WIDTH                480
#define VDC3_WQVGA_HEIGHT               240
#define VDC3_QVGA_LANDSCAPE_WIDTH       320
#define VDC3_QVGA_LANDSCAPE_HEIGHT      240
#define VDC3_QVGA_PORTRAIT_WIDTH        240
#define VDC3_QVGA_PORTRAIT_HEIGHT       320

//--//



//HAL_DECLARE_NULL_HEAP();
HAL_DECLARE_CUSTOM_HEAP(SimpleHeap_Allocate, SimpleHeap_Release,SimpleHeap_ReAllocate);

void PostManagedEvent( UINT8 category, UINT8 subCategory, UINT16 data1, UINT32 data2 )
{
}

UINT32 handle;
WCHAR path[12] = {'\\', 't', 'e', 's', 't','1', '.', 't', 'x', 't', '\0'};
FileSystemVolume* pSDVolume;
FAT_LogicDisk* pSDLogicDisk;


void VDC3_Rect_Write (unsigned short *lcd_top_addr, LCD_RECT data)
{
    int             p_x, p_y;
    unsigned short  *addr, *save_addr;

    if ((data.Left_top.p_x < 0) || (g_SH7264VDC3_Config.Width < data.Left_top.p_x) ||
        (data.Right_bottom.p_x <= 0 ) || (g_SH7264VDC3_Config.Width < data.Right_bottom.p_x) ||
        (data.Left_top.p_x == data.Right_bottom.p_x))
    {
        goto EXIT;
    }
    if ((data.Left_top.p_y < 0) || (g_SH7264VDC3_Config.Height < data.Left_top.p_y) ||
        (data.Right_bottom.p_y <= 0 ) || (g_SH7264VDC3_Config.Height < data.Right_bottom.p_y) ||
        (data.Left_top.p_y == data.Right_bottom.p_y))
    {
        goto EXIT;
    }

    addr = (unsigned short *)((unsigned long)lcd_top_addr + (data.Left_top.p_y * 2*g_SH7264VDC3_Config.Width) + (data.Left_top.p_x * 2));
    save_addr = addr;
    for (p_y = data.Left_top.p_y; p_y < data.Right_bottom.p_y; p_y++)
    {
        for (p_x = data.Left_top.p_x ; p_x < data.Right_bottom.p_x; p_x++)
        {
            *addr++ = data.Color;
        }
        save_addr = (unsigned short *)((unsigned long)save_addr + (2*g_SH7264VDC3_Config.Width));
        addr = save_addr;
    }

EXIT:
    return;
}

void VDC3_Frame_Write (unsigned short *lcd_top_addr, LCD_RECT data)
{
    int             p_x, p_y;
    unsigned short  *addr, *save_addr;

    if ((data.Left_top.p_x < 0) || (g_SH7264VDC3_Config.Width < data.Left_top.p_x) ||
        (data.Right_bottom.p_x <= 0 ) || (g_SH7264VDC3_Config.Width < data.Right_bottom.p_x) ||
        (data.Left_top.p_x == data.Right_bottom.p_x))
    {
        goto EXIT;
    }
    if ((data.Left_top.p_y < 0) || (g_SH7264VDC3_Config.Height < data.Left_top.p_y) ||
        (data.Right_bottom.p_y <= 0 ) || (g_SH7264VDC3_Config.Height < data.Right_bottom.p_y) ||
        (data.Left_top.p_y == data.Right_bottom.p_y))
    {
        goto EXIT;
    }

    addr = (unsigned short *)((unsigned long)lcd_top_addr + (data.Left_top.p_y * 2* g_SH7264VDC3_Config.Width) + (data.Left_top.p_x * 2) );
    save_addr = addr;
    for (p_y = data.Left_top.p_y; p_y <= data.Right_bottom.p_y; p_y++)
    {
        if ((p_y == data.Left_top.p_y) || (p_y == data.Right_bottom.p_y))
        {
            for (p_x = data.Left_top.p_x; p_x <= data.Right_bottom.p_x; p_x++)
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
        save_addr = (unsigned short *)((unsigned long)save_addr + (2* g_SH7264VDC3_Config.Width));
        addr = save_addr;
    }

EXIT:
    return;
}

void VDC3_Sleep(unsigned int nTime)
{
    volatile int    i;
    for (i = 0; i < nTime; i++);
}

void VDC3_SolidRectangles_Drawing()
{
    LCD_RECT    rect;
    int         i;

    SH7264_VDC3_Driver::VDC3_ClearScreen(g_SH7264VDC3_Config.Width, g_SH7264VDC3_Config.Height);
    
    Buffer = (unsigned short * )VDC3_GRA1_BASE_ADDRESS;

    for( i = 0 ; i < 9 ; i++ )
    {
        if (g_SH7264VDC3_Config.Width == VDC3_PANEL_VGA_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_VGA_HEIGHT)
        {
            rect.Left_top.p_x     = VGA_SolidRect_data[i].l_x;
            rect.Left_top.p_y     = VGA_SolidRect_data[i].l_y;
            rect.Right_bottom.p_x = VGA_SolidRect_data[i].r_x;
            rect.Right_bottom.p_y = VGA_SolidRect_data[i].r_y;
            rect.Color            = VGA_SolidRect_data[i].color;
            
            VDC3_Sleep(0x1000);
        }
        else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_QVGA_LANDSCAPE_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_QVGA_LANDSCAPE_HEIGHT)
        {
            rect.Left_top.p_x     = QVGA_LANDSCAPE_SolidRect_data[i].l_x;
            rect.Left_top.p_y     = QVGA_LANDSCAPE_SolidRect_data[i].l_y;
            rect.Right_bottom.p_x = QVGA_LANDSCAPE_SolidRect_data[i].r_x;
            rect.Right_bottom.p_y = QVGA_LANDSCAPE_SolidRect_data[i].r_y;
            rect.Color            = QVGA_LANDSCAPE_SolidRect_data[i].color;
            
            VDC3_Sleep(0x20000);
        }
        else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_QVGA_PORTRAIT_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_QVGA_PORTRAIT_HEIGHT)  
        {
            rect.Left_top.p_x     = QVGA_PORTRAIT_SolidRect_data[i].l_x;
            rect.Left_top.p_y     = QVGA_PORTRAIT_SolidRect_data[i].l_y;
            rect.Right_bottom.p_x = QVGA_PORTRAIT_SolidRect_data[i].r_x;
            rect.Right_bottom.p_y = QVGA_PORTRAIT_SolidRect_data[i].r_y;
            rect.Color            = QVGA_PORTRAIT_SolidRect_data[i].color;
            
            VDC3_Sleep(0x20000);
        }
        else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_WQVGA_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_WQVGA_HEIGHT)
        {
            rect.Left_top.p_x     = WQVGA_SolidRect_data[i].l_x;
            rect.Left_top.p_y     = WQVGA_SolidRect_data[i].l_y;
            rect.Right_bottom.p_x = WQVGA_SolidRect_data[i].r_x;
            rect.Right_bottom.p_y = WQVGA_SolidRect_data[i].r_y;
            rect.Color            = WQVGA_SolidRect_data[i].color;
            
            VDC3_Sleep(0x6000);
        }       
        
        VDC3_Rect_Write ((unsigned short *)Buffer, rect);

        //Start displaying graphic
        SH7264_VDC3_Driver::VDC3_Display_Start(0, 1, 0);        
    }
}

void VDC3_FrameRectangles_Drawing(int option)
{
    LCD_RECT    rect;
    int         i, ix, iy, dx, dy;
    int         Rx_size, Ry_size, color;
    int         nNumRect;
    
    //Clear screen
    SH7264_VDC3_Driver::VDC3_ClearScreen(g_SH7264VDC3_Config.Width, g_SH7264VDC3_Config.Height);
    
    color   = 0;
    Rx_size = 20;
    Ry_size = 20;
    dx      = 16;
    dy      = 16;
    ix      = (g_SH7264VDC3_Config.Width - Rx_size) ;
    iy      = (g_SH7264VDC3_Config.Height - Ry_size) ;

    rect.Left_top.p_x     = ix;
    rect.Left_top.p_y     = iy;
    rect.Right_bottom.p_x = ix + Rx_size;
    rect.Right_bottom.p_y = iy + Ry_size;
    rect.Color            = Color_Tbl[color];
    
    if (g_SH7264VDC3_Config.Width == VDC3_PANEL_VGA_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_VGA_HEIGHT)
        nNumRect = VGA_NUM_RECT;
    else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_QVGA_LANDSCAPE_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_QVGA_LANDSCAPE_HEIGHT)    
        nNumRect = QVGA_LANDSCAPE_NUM_RECT;
    else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_QVGA_PORTRAIT_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_QVGA_PORTRAIT_HEIGHT)  
        nNumRect = QVGA_PORTRAIT_NUM_RECT;  
    else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_WQVGA_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_WQVGA_HEIGHT)  
        nNumRect = WQVGA_NUM_RECT;
        
    for (i = 0; i < nNumRect; i++)
    {
        Buffer = (unsigned short * )VDC3_GRA1_BASE_ADDRESS;
        if (option == 0)
        {
            VDC3_Frame_Write ((unsigned short *)Buffer, rect);
        }
        else
        {           
            VDC3_Rect_Write ((unsigned short *)Buffer, rect);   
        }       
        SH7264_VDC3_Driver::VDC3_Display_Start(0, 1, 0);
        
        if (g_SH7264VDC3_Config.Width == VDC3_PANEL_VGA_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_VGA_HEIGHT)
            VDC3_Sleep(0x2000);
        else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_QVGA_LANDSCAPE_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_QVGA_LANDSCAPE_HEIGHT)    
            VDC3_Sleep(0x5000);
        else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_QVGA_PORTRAIT_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_QVGA_PORTRAIT_HEIGHT)
            VDC3_Sleep(0x5000); 
        else if (g_SH7264VDC3_Config.Width == VDC3_PANEL_WQVGA_WIDTH && g_SH7264VDC3_Config.Height == VDC3_PANEL_WQVGA_HEIGHT)  
            VDC3_Sleep(0x3000);
            
        rect.Left_top.p_x     += dx;
        rect.Left_top.p_y     += dy;
        rect.Right_bottom.p_x += dx;
        rect.Right_bottom.p_y += dy;
        
        if (rect.Left_top.p_x < 0)
        {
            dx          = -dx;
            color      += 1;
            if (color > 7)
            {
                color = 0;
            }
            rect.Color             = Color_Tbl[color];
            rect.Right_bottom.p_x -= rect.Left_top.p_x;
            rect.Right_bottom.p_x += 0;
            rect.Left_top.p_x      = 0;
        }
        if (rect.Left_top.p_y < 0)
        {
            dy          = -dy;
            color      += 1;
            if (color > 7)
            {
                color = 0;
            }
            rect.Color             = Color_Tbl[color];
            rect.Right_bottom.p_y -= rect.Left_top.p_y;
            rect.Right_bottom.p_y += 0;
            rect.Left_top.p_y      = 0;
        }
        if (rect.Right_bottom.p_x > g_SH7264VDC3_Config.Width)
        {
            dx          = -dx;
            color      += 1;
            if (color > 7)
            {
                color = 0;
            }
            rect.Color             = Color_Tbl[color];
            rect.Left_top.p_x     -= rect.Right_bottom.p_x;
            rect.Left_top.p_x     += g_SH7264VDC3_Config.Width;
            rect.Right_bottom.p_x  = g_SH7264VDC3_Config.Width;
        }
        if (rect.Right_bottom.p_y > g_SH7264VDC3_Config.Height)
        {
            dy          = -dy;
            color      += 1;
            if (color > 7)
            {
                color = 0;
            }
            rect.Color             = Color_Tbl[color];
            rect.Left_top.p_y     -= rect.Right_bottom.p_y;
            rect.Left_top.p_y     += g_SH7264VDC3_Config.Height;
            rect.Right_bottom.p_y  = g_SH7264VDC3_Config.Height;
        }
    }
}


BOOL Bitmap_File_Load(LPCSTR nameSpace, UINT32 nameSpaceLength, LPCWSTR path, unsigned char * buf, int lenght)
{
    UINT32 handle;  
    FileSystemVolume* pSDVolume = NULL;
    int bytesRead;
    
    pSDVolume = FileSystemVolumeList::FindVolume(nameSpace, nameSpaceLength);
    handle = 0;
    
    if(pSDVolume != NULL)
    {
        //Open bitmap file
        FAT_FS_Driver::Open(&pSDVolume->m_volumeId, path, &handle);
        if(!handle)
        {
            return FALSE;
        }
        
        //do
        //{
        //  bytesRead = 0;
        //  FAT_FS_Driver::Read( handle, buff, 1024, &bytesRead );
        //  for(int i = 0; i < bytesRead; i += 2)
        //  {
        //      //*((volatile unsigned long *)mem) = (buff[i] << 24) | (buff[i + 1] << 16) | (buff[i + 2] << 8) | (buff[i + 3] << 0);
        //      *((volatile unsigned short *)mem) = (buff[i] << 0) | (buff[i + 1] << 8);
        //      mem += 2;
        //  }
        //}while(bytesRead == 1024);
        
        FAT_FS_Driver::Read( handle, buf, lenght, &bytesRead );
        
        FAT_FS_Driver::Close(handle);       
    }
    else
    {
        return FALSE;
    }
    
    return TRUE;
}

void VDC3_Graphic_Bitmap_Display(int opt, LPCSTR nameSpace)
{
    bitmap_info_t bitmap_info;  /* Bitmap information */
    glubyte_t *bitmap_bits;     /* Bitmap data */
    unsigned short * bmp_buff;
    int lenght;
    
    WCHAR path1[64] = {'\\', 'A', 'u', 't', 'u', 'm', 'n', '_', '6', '4', '0', 'x', '4', '8', '0', '_', '1', '6', 'b', 'i', 't', '.', 'b', 'm', 'p', '\0'};
    WCHAR path2[64] = {'\\', 'A', 'u', 't', 'u', 'm', 'n', '_', '3', '2', '0', 'x', '2', '4', '0', '_', '1', '6', 'b', 'i', 't', '.', 'b', 'm', 'p', '\0'};
    WCHAR path3[64] = {'\\', 'A', 'u', 't', 'u', 'm', 'n', '_', '2', '4', '0', 'x', '3', '2', '0', '_', '1', '6', 'b', 'i', 't', '.', 'b', 'm', 'p', '\0'};
    WCHAR path4[64] = {'\\', 'A', 'u', 't', 'u', 'm', 'n', '_', '4', '8', '0', 'x', '2', '4', '0', '_', '1', '6', 'b', 'i', 't', '.', 'b', 'm', 'p', '\0'};
    
    BOOL result;

    SH7264_VDC3_Driver::VDC3_ClearScreen(g_SH7264VDC3_Config.Width, g_SH7264VDC3_Config.Height);
    
    switch(opt)
    {
        case 0:
            bmp_buff = (unsigned short*)private_malloc(640*480*2+54);
            lenght = 640*480*2+54;
            break;
        case 1:
            bmp_buff = (unsigned short*)private_malloc(320*240*2+54);
            lenght = 320*240*2+54;
            break;
        case 2:
            bmp_buff = (unsigned short*)private_malloc(320*240*2+54);
            lenght = 320*240*2+54;
            break;
        case 3:
            bmp_buff = (unsigned short*)private_malloc(480*240*2+54);
            lenght = 480*240*2+54;
            break;
        default:
            bmp_buff = (unsigned short*)private_malloc(640*480*2+54);
            lenght = 640*480*2+54;
    }
    
    hal_printf("\r\n-This test will display only graphic on block 1.");
    hal_printf("\r\n-Download a bitmap to %08x.",bmp_buff);
        
    switch(opt)
    {
        case 0:
            result = Bitmap_File_Load(nameSpace, 1, path1, (unsigned char *)bmp_buff, lenght);//0x0C040000);
            break;
        case 1:
            result = Bitmap_File_Load(nameSpace, 1, path2, (unsigned char *)bmp_buff, lenght);//0x0C040000);
            break;
        case 2:
            result = Bitmap_File_Load(nameSpace, 1, path3, (unsigned char *)bmp_buff, lenght);//0x0C040000);
            break;
        case 3:
            result = Bitmap_File_Load(nameSpace, 1, path4, (unsigned char *)bmp_buff, lenght);//0x0C040000);
            break;
        default:
            result = Bitmap_File_Load(nameSpace, 1, path1, (unsigned char *)bmp_buff, lenght);//0x0C040000);
    }
        
    if(!result)
    {
        switch(opt)
        {
            case 0:
                hal_printf("\r\n-Cannot find the Autumn_640x480_16bit.bmp bitmap file on the SD card/USB flash.");
                break;
            case 1:
                hal_printf("\r\n-Cannot find the Autumn_320x240_16bit.bmp bitmap file on the SD card/USB flash.");
                break;
            case 2:
                hal_printf("\r\n-Cannot find the Autumn_240x320_16bit.bmp bitmap file on the SD card/USB flash.");
                break;
            case 3:
                hal_printf("\r\n-Cannot find the Autumn_480x240_16bit.bmp bitmap file on the SD card/USB flash.");
                break;
            default:
                hal_printf("\r\n-Cannot find the Autumn_640x480_16bit.bmp bitmap file on the SD card/USB flash.");
        }       
        return;
    }

    bitmap_bits = SH7264_BITMAP::bitmap_load((unsigned long)bmp_buff/*0x0C040000*/, &bitmap_info);
        
    if(bitmap_bits != NULL)
    {
        {
            if (bitmap_info.bmiHeader.biBitCount == 16)
            {
                SH7264_BITMAP::bitmap_16bits_copy((unsigned short *)bitmap_bits, 
                                   (unsigned short *)VDC3_GRA1_BASE_ADDRESS,
                                    bitmap_info.bmiHeader.biWidth, 
                                    bitmap_info.bmiHeader.biHeight, 1, 1, 0);   
            }
            else if (bitmap_info.bmiHeader.biBitCount == 24)
            {
                SH7264_BITMAP::bitmap_convert_24bits_2_16bits((unsigned char *)bitmap_bits, 
                                               (unsigned short *)VDC3_GRA1_BASE_ADDRESS,
                                                bitmap_info.bmiHeader.biWidth, 
                                                bitmap_info.bmiHeader.biHeight);                
            }
        }
    }
    else
    {
        hal_printf("\r\nNo bitmap was found at %08x.\r\n",bmp_buff);
    }
    if(bmp_buff)
        private_free(bmp_buff);
    
    //Start displaying graphic
    SH7264_VDC3_Driver::VDC3_Display_Start(0, 1, 0);
    return;
}

void VDC3_Load_Bitmap_App(LPCSTR nameSpace)
{
    VDC3_Graphic_Bitmap_Display(0, nameSpace);
}

void VDC3_QVGA_Load_Bitmap_App(int opt, LPCSTR nameSpace)
{
    
    switch(opt)
    {
        case 0:
            VDC3_Graphic_Bitmap_Display(1,nameSpace);
            break;
        case 1:
            VDC3_Graphic_Bitmap_Display(2,nameSpace);
            break;
        default:
            VDC3_Graphic_Bitmap_Display(1,nameSpace);
    }
}

void VDC3_WQVGA_Load_Bitmap_App(LPCSTR nameSpace)
{
    VDC3_Graphic_Bitmap_Display(3, nameSpace);
}

 //--//





BOOL USBFlashOpenFile()
{
    pSDVolume = NULL;
    pSDLogicDisk = NULL;
        
    if(!pSDVolume)
        pSDVolume = FileSystemVolumeList::FindVolume("U", 1);
    handle = 0;
    
    if(pSDVolume != NULL)
    {
        //Open/Create file
        FAT_FS_Driver::Open(&pSDVolume->m_volumeId, (LPCWSTR)path, &handle);
        if(!handle)
        {
            hal_printf("USBFlash - Open File: Cannot open\\create file\r\n");           
            return FALSE;
        }
    }
    else
    {
            hal_printf("USBFlash - Open File: Volume not found\r\n");
            return FALSE;
    }

    return TRUE;
}

BOOL USBFlashOpenFileTest()
{
    BOOL result;

    hal_printf("\r\nUSBFlash - Open File Test: Opening\\Creating file... Please do not remove the card\r\n");
    result = USBFlashOpenFile();    
    
    if(!result)
    {
        hal_printf("USBFlash - Open File Test: FAILED\r\n");
        return FALSE;
    }
    
    FAT_FS_Driver::Close(handle);
    hal_printf("USBFlash - Open File Test: SUCCESS\r\n");

    return TRUE;
}

BOOL USBFlashReadFileTest()
{
    char buff[128] = "";
    int bytesReadWrite = 0;
    BOOL result;
    HRESULT hresult;
    
    hal_printf("\r\nUSBFlash - Read File Test: Reading file... Please do not remove the card\r\n");
    result = USBFlashOpenFile();    
    
    if(result)
    {
        hresult = FAT_FS_Driver::Read( handle, (BYTE*)buff, 128, &bytesReadWrite );
        buff[bytesReadWrite] = '\0';
        
        if(!hresult)
        {
            if(bytesReadWrite <= 1)
            {
                hal_printf("USBFlash - Read File Test: File empty\r\n");
            }
            else
            {
                hal_printf("USBFlash - Read File Test: Read data: %s\r\n", buff);
            }
        }
        else
        {
            hal_printf("USBFlash - Read File Test: FAILED\r\n");            
            return FALSE;
        }
    }
    else
    {
        hal_printf("USBFlash - Read File Test: FAILED\r\n");
        return FALSE;
    }
    
    FAT_FS_Driver::Close(handle);
    hal_printf("USBFlash - Read File Test: SUCCESS\r\n");
    
    return TRUE;
}

BOOL USBFlashWriteFileTest()
{
    char buffer[1024] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ-abcdefghijklmnopqrstuvwxyz";
    int bytesReadWrite = 0;
    BOOL result;
    
    hal_printf("\r\nUSBFlash - Write File Test: Writing file... Please do not remove the card\r\n");
    result = USBFlashOpenFile();
    
    if(result)
    {
        bytesReadWrite = 0;
        result = FAT_FS_Driver::Write( handle, (BYTE*)buffer, 53, &bytesReadWrite );            
        if(bytesReadWrite != 53)
        {
            hal_printf("USBFlash - Write File Test: FAILED\r\n");
            FAT_FS_Driver::Close(handle);
            return FALSE;
        }

        FAT_FS_Driver::Close(handle);
        hal_printf("USBFlash - Write File Test: SUCCESS\r\n");      
    }
    else
    {
        hal_printf("USBFlash - Write File Test: FAILED\r\n");
        return FALSE;
    }   
    
    return TRUE;
}

BOOL USBFlashDeleteFileTest()
{
    HRESULT hresult;
    
    hal_printf("\r\nUSBFlash - Delete File Test: Deleting file... Please do not remove the card\r\n");
    
    if(!pSDVolume)
        pSDVolume = FileSystemVolumeList::FindVolume("U", 1);
    
    if(pSDVolume != NULL)
    {
        if(!handle)
            FAT_FS_Driver::Close(handle);
        hresult = FAT_FS_Driver::Delete(&pSDVolume->m_volumeId, (LPCWSTR)path);
        if(hresult == CLR_E_FILE_IO)
        {
            hal_printf("USBFlash - Delete File Test: Invalid file name\r\n");
            hal_printf("USBFlash - Delete File Test: FAILED\r\n");
            return FALSE;
        }
        else if(hresult == CLR_E_FILE_NOT_FOUND)
        {
            hal_printf("USBFlash - Delete File Test: File not found\r\n");
            hal_printf("USBFlash - Delete File Test: FAILED\r\n");
            return FALSE;
        }
        else if(hresult == CLR_E_FILE_NOT_FOUND)
        {
            hal_printf("USBFlash - Delete File Test: File not found\r\n");
            hal_printf("USBFlash - Delete File Test: FAILED\r\n");
            return FALSE;
        }
        else if(hresult == CLR_E_UNAUTHORIZED_ACCESS)
        {
            hal_printf("USBFlash - Delete File Test: Cannot delete file: Access is denied\r\n");
            hal_printf("USBFlash - Delete File Test: FAILED\r\n");
            return FALSE;
        }

        if(!pSDVolume)
            pSDVolume = FileSystemVolumeList::FindVolume("U", 1);
        
        if(pSDVolume)
            hresult = FAT_FS_Driver::FlushAll(&pSDVolume->m_volumeId);
    }
    else
    {
        hal_printf("USBFlash - Delete File Test: Volume not found\r\n");
        hal_printf("USBFlash - Delete File Test: FAILED\r\n");
        return FALSE;
    }
    hal_printf("USBFlash - Delete File Test: SUCCESS\r\n");
    
    return TRUE;
}

void Keypad_Test(void)
{
    int ret = 0;
    char Data[1];
    Data[1] = '\0'; 

    hal_printf("\r\n\nPress any key on keypad of the board to see its I.D.");
    hal_printf("\r\nTo EXIT test mode, press ESC key on the computer keyboard!");
    hal_printf("\r\n"); 
    
    while(1)
    {
        UINT16 sw_code = g_SH7264_Keypad_Driver.Key_Press();
        UINT16 sw_offset = SH7264_GPIO::c_sw_key_offset;

        sw_code -= sw_offset;
        
        switch(sw_code)
        {
        case 0:
            hal_printf("\r\nKey pressed: SW2\n");
            break;          
        case 1:
            hal_printf("\r\nKey pressed: SW3\n");
            break;          
        case 2:
            hal_printf("\r\nKey pressed: SW4\n");
            break;  
        case 3:
            hal_printf("\r\nKey pressed: SW5\n");
            break;
        case 4:
            hal_printf("\r\nKey pressed: SW6\n");
            break;
        case 5:
            hal_printf("\r\nKey pressed: SW7\n");
            break;
        case 6:
            hal_printf("\r\nKey pressed: SW8\n");
            break;      
        case 7:
            hal_printf("\r\nKey pressed: SW9\n");
            break;                  
        case 8:
            hal_printf("\r\nKey pressed: SW10\n");
            break;                  
        case 9:
            hal_printf("\r\nKey pressed: SW11\n");
            break;                      
        case 10:
            hal_printf("\r\nKey pressed: SW12\n");
            break;                      
        case 11:
            hal_printf("\r\nKey pressed: SW13\n");
            break;                  
        case 12:
            hal_printf("\r\nKey pressed: SW14\n");
            break;                          
        case 13:
            hal_printf("\r\nKey pressed: SW15\n");
            break;                      
        case 14:
            hal_printf("\r\nKey pressed: SW16\n");
            break;                      
        case 15:
            hal_printf("\r\nKey pressed: SW17\n");
            break;
        }       
        
        UINT32 Events = Events_WaitForEvents( SYSTEM_EVENT_FLAG_COM_IN, 50 );
                        
        if(Events & SYSTEM_EVENT_FLAG_COM_IN)           
        {       
            Events_Clear( SYSTEM_EVENT_FLAG_COM_IN );
            ret = DebuggerPort_Read( HalSystemConfig.DebugTextPort, Data, 1 );
            if(ret)
            {
                if(Data[0] == 0x1B)             //0x1B: ESC key
                {
                    goto End;
                }
            }
        }
    }
End:
    return; 
}

void ApplicationEntryPoint()
{
    char Data[1], previousChar = '\0', c[1];
    int ret = 0, TestItem = 0;
    BOOL validCommand = FALSE;
    UINT32 Events;
    
    // Some tests to allocate memory. Initialize simple heap for it. 
    UINT8* BaseAddress;
    UINT32 SizeInBytes;

    HeapLocation( BaseAddress,    SizeInBytes );    
    SimpleHeap_Initialize( BaseAddress, SizeInBytes );
    
    Data[1] = '\0';
    
    lcd_printf( "\n\nSH7264 RSK .NET Micro Framework\r\n" );
    lcd_printf( "Renesas Electronics America Inc.\r\n" );
            
    //Serial TEST
    hal_printf( "\r\nSH7264 RSK .NET Micro Framework\r\n" );
    hal_printf( "Renesas Electronics America Inc.\r\n" );       
    hal_printf( ">" );
        
    NATIVE_PROFILE_PAL_EVENTS();
    CPU_USART_RxBufferFullInterruptEnable( ConvertCOM_ComPort(HalSystemConfig.DebugTextPort), TRUE );


        do
        {

            Events = Events_WaitForEvents( SYSTEM_EVENT_FLAG_COM_IN, 100 );

            if(Events & SYSTEM_EVENT_FLAG_COM_IN)           
            {
                Events_Clear( SYSTEM_EVENT_FLAG_COM_IN );
                ret = DebuggerPort_Read( HalSystemConfig.DebugTextPort, Data, 1 );
                if(ret)
                {                   
                    switch(TestItem)
                    {
                        case 0:     //None
                            if(Data[0] == 0xD)              //0xD: ENTER key
                            {                               
                                hal_printf( "\r\n" );
                                if(validCommand)
                                {
                                    TestItem = 1;           //Enter test
                                    validCommand = FALSE;
                                    previousChar = '\0';
                                    hal_printf( "\r\nTest Menu:\r\n" );
                                    hal_printf( "   [V]DC3 Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );                                      
                                    hal_printf( "   [U]SB Flash Tests\r\n" );
                                    hal_printf( "   [S]erial flash memory Tests\r\n" );  //Added Serial flash memory test
                                    hal_printf( "   [N]or flash memory Tests\r\n" );  //Added Nor flash memory test                                 
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                }
                                else if(previousChar == 'I')
                                {
                                    hal_printf("** Invalid Selection\r\n>");
                                    previousChar = '\0';
                                }
                                else
                                    hal_printf( ">" );
                            }
                            else
                            {
                                hal_printf( Data );
                                if(Data[0] == 'd' || Data[0] == 'D')
                                {
                                    if(previousChar == '\0')
                                        previousChar = 'd';
                                    else
                                    {
                                        validCommand = FALSE;
                                        previousChar = 'I';     //Invalid character input
                                    }
                                }
                                else if(Data[0] == 't' || Data[0] == 'T')
                                {
                                    if(previousChar == 'd')
                                    {
                                        previousChar = 't';
                                        validCommand = TRUE;
                                    }
                                    else
                                    {
                                        validCommand = FALSE;
                                        previousChar = 'I';     //Invalid character input
                                    }
                                }
                                else
                                {
                                    validCommand = FALSE;
                                    previousChar = 'I';         //Invalid character input
                                }
                            }
                            break;
                        case 1:     //Enter test
                            hal_printf( Data );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 0;
                                    hal_printf( "\r\n\n>" );
                                    break;
                                case 'V':
                                case 'v':
                                    TestItem = 2;
                                    hal_printf( "\r\n\nVDC3 Menu:\r\n" );
                                    hal_printf( "   [V]GA Tests \r\n" );
                                    hal_printf( "   QVGA [L]andscape Tests \r\n" );
                                    hal_printf( "   QVGA [P]ortrait Tests \r\n" );
                                    hal_printf( "   [W]QVGA Tests \r\n" );                                  
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case 'K':
                                case 'k':
                                    Keypad_Test();
                                    hal_printf( "\r\n\nTest Menu:\r\n" );
                                    hal_printf( "   [V]DC3 Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );                                  
                                    hal_printf( "   [U]SB Flash Tests\r\n" );
                                    hal_printf( "   [S]erial flash memory Tests\r\n" );
                                    hal_printf( "   [N]or flash memory Tests\r\n" );  //Added Nor flash memory test                                 
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;                              
                                case 'U':
                                case 'u':
                                    TestItem = 7;
                                    hal_printf( "\r\n\nUSB Flash Menu:\r\n" );
                                    hal_printf( "   [O]pen File Test\r\n" );
                                    hal_printf( "   [R]ead File Test\r\n" );
                                    hal_printf( "   [W]rite File Test\r\n" );
                                    hal_printf( "   [D]elete File Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;                              
                                case 'S':
                                case 's':
                                    TestItem = 8;
                                    hal_printf( "\r\n\nSerial Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0xF0000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case 'N':
                                case 'n':
                                    TestItem = 9;                                    
                                    hal_printf( "\r\n\nNOR Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0x20080000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );                                  
                                    hal_printf( "Your choice: " );
                                    break;                              
                                default:
                                    hal_printf( "\r\n\nTest Menu:\r\n" );
                                    hal_printf( "   [V]DC3 Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );      
                                    hal_printf( "   [U]SB Flash Tests\r\n" );
                                    hal_printf( "   [S]erial flash memory Tests\r\n" );
                                    hal_printf( "   [N]or flash memory Tests\r\n" );  //Added Nor flash memory test                                 
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        case 2:     //VDC3 test
                            hal_printf( Data );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 1;
                                    hal_printf( "\r\n\nTest Menu:\r\n" );
                                    hal_printf( "   [V]DC3 Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );
                                    hal_printf( "   [U]SB Flash Tests\r\n" );
                                    hal_printf( "   [S]erial flash memory Tests\r\n" );
                                    hal_printf( "   [N]or flash memory Tests\r\n" );  //Added Nor flash memory test                                 
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case 'v':
                                case 'V':       
                                    TestItem = 3;                            
                                    VDC3_Initialize(VDC3_VGA_WIDTH, VDC3_VGA_HEIGHT);
                                    hal_printf( "\r\n\nVDC3 VGA Menu:\r\n" );
                                    hal_printf( "   1: Display colorful rectangles\r\n" );
                                    hal_printf( "   2: Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: Load Bitmap\r\n" );                                  
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );                              
                                    break;
                                case 'l':
                                case 'L':
                                    VDC3_Initialize(VDC3_QVGA_LANDSCAPE_WIDTH, VDC3_QVGA_LANDSCAPE_HEIGHT);
                                    TestItem = 4;
                                    hal_printf( "\r\n\nVDC3 QVGA Landscape Menu:\r\n" );
                                    hal_printf( "   1: QVGA Landscape Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Landscape Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Landscape Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Landscape Load Bitmap\r\n" );                                   
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );  
                                    break;
                                case 'p':
                                case 'P':
                                    VDC3_Initialize(VDC3_QVGA_PORTRAIT_WIDTH, VDC3_QVGA_PORTRAIT_HEIGHT);
                                    TestItem = 5;
                                    hal_printf( "\r\n\nVDC3 QVGA Portrait Menu:\r\n" );
                                    hal_printf( "   1: QVGA Portrait Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Portrait Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Portrait Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Portrait Load Bitmap\r\n" );                                    
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );  
                                    break;
                                case 'w':
                                case 'W':
                                    VDC3_Initialize(VDC3_WQVGA_WIDTH, VDC3_WQVGA_HEIGHT);
                                    TestItem = 6;
                                    hal_printf( "\r\n\nVDC3 WQVGA Menu:\r\n" );
                                    hal_printf( "   1: WQVGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: WQVGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: WQVGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: WQVGA Load Bitmap\r\n" );                                    
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );              
                                    break;
                                default:    
                                    hal_printf( "\r\n\nVDC3 Menu:\r\n" );
                                    hal_printf( "   [V]GA Tests \r\n" );
                                    hal_printf( "   QVGA [L]andscape Tests \r\n" );
                                    hal_printf( "   QVGA [P]ortrait Tests \r\n" );
                                    hal_printf( "   [W]QVGA Tests \r\n" );                                  
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        case 3:
                            hal_printf( Data );
                            switch (Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 2;
                                    hal_printf( "\r\n\nVDC3 Menu:\r\n" );
                                    hal_printf( "   [V]GA Tests \r\n" );
                                    hal_printf( "   QVGA [L]andscape Tests \r\n" );
                                    hal_printf( "   QVGA [P]ortrait Tests \r\n" );
                                    hal_printf( "   [W]QVGA Tests \r\n" );                                  
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;          
                                case '1':
                                    VDC3_SolidRectangles_Drawing();
                                    hal_printf( "\r\n\nVDC3 VGA Menu:\r\n" );
                                    hal_printf( "   1: VGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: VGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: VGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: VGA Load Bitmap\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
                                    VDC3_FrameRectangles_Drawing(0);
                                    hal_printf( "\r\n\nVDC3 VGA Menu:\r\n" );
                                    hal_printf( "   1: VGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: VGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: VGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: VGA Load Bitmap\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '3':
                                    VDC3_FrameRectangles_Drawing(1);
                                    hal_printf( "\r\n\nVDC3 VGA Menu:\r\n" );
                                    hal_printf( "   1: VGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: VGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: VGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: VGA Load Bitmap\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '4':
                                    hal_printf( "\n\rSelect to display from [U]SB flash: " );
                                    while(TRUE) {
                                        DebuggerPort_Read( HalSystemConfig.DebugTextPort, c, 1 )== 0;
                                        if (c[0] != 0) break;
                                    }
                                    if (c[0] == 'u' || c[0] == 'U')
                                        VDC3_Load_Bitmap_App("U");
                                    c[0] = 0;
                                    hal_printf( "\r\n\nVDC3 VGA Menu:\r\n" );
                                    hal_printf( "   1: VGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: VGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: VGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: VGA Load Bitmap\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                default:
                                    hal_printf( "\r\n\nVDC3 VGA Menu:\r\n" );
                                    hal_printf( "   1: VGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: VGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: VGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: VGA Load Bitmap\r\n" );                                  
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        case 4:
                            hal_printf( Data );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 2;
                                    hal_printf( "\r\n\nVDC3 Menu:\r\n" );
                                    hal_printf( "   [V]GA Tests \r\n" );
                                    hal_printf( "   QVGA [L]andscape Tests \r\n" );
                                    hal_printf( "   QVGA [P]ortrait Tests \r\n" );
                                    hal_printf( "   [W]QVGA Tests \r\n" );                              
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '1':
                                    VDC3_SolidRectangles_Drawing();
                                    hal_printf( "\r\n\nVDC3 QVGA Landscape Menu:\r\n" );
                                    hal_printf( "   1: QVGA Landscape Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Landscape Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Landscape Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Landscape Load Bitmap\r\n" );                                   
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
                                    VDC3_FrameRectangles_Drawing(0);
                                    hal_printf( "\r\n\nVDC3 QVGA Landscape Menu:\r\n" );
                                    hal_printf( "   1: QVGA Landscape Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Landscape Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Landscape Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Landscape Load Bitmap\r\n" );                                       
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '3':
                                    VDC3_FrameRectangles_Drawing(1);
                                    hal_printf( "\r\n\nVDC3 QVGA Landscape Menu:\r\n" );
                                    hal_printf( "   1: QVGA Landscape Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Landscape Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Landscape Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Landscape Load Bitmap\r\n" );                                       
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '4':
                                    hal_printf( "\n\rSelect to display from [U]SB flash: " );
                                    while(TRUE) {
                                        DebuggerPort_Read( HalSystemConfig.DebugTextPort, c, 1 )== 0;
                                        if (c[0] != 0) break;
                                    }
                                    if (c[0] == 'u' || c[0] == 'U')
                                        VDC3_QVGA_Load_Bitmap_App(0, "U");
                                    c[0] = 0;                                   
                                    hal_printf( "\r\n\nVDC3 QVGA Landscape Menu:\r\n" );
                                    hal_printf( "   1: QVGA Landscape Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Landscape Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Landscape Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Landscape Load Bitmap\r\n" );                                       
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;  
                                default:
                                    hal_printf( "\r\n\nVDC3 QVGA Landscape Menu:\r\n" );
                                    hal_printf( "   1: QVGA Landscape Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Landscape Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Landscape Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Landscape Load Bitmap\r\n" );                                       
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        case 5:
                            hal_printf( Data );
                            switch (Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 2;
                                    hal_printf( "\r\n\nVDC3 Menu:\r\n" );
                                    hal_printf( "   [V]GA Tests \r\n" );
                                    hal_printf( "   QVGA [L]andscape Tests \r\n" );
                                    hal_printf( "   QVGA [P]ortrait Tests \r\n" );
                                    hal_printf( "   [W]QVGA Tests \r\n" );                              
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;          
                                case '1':
                                    VDC3_SolidRectangles_Drawing();
                                    hal_printf( "\r\n\nVDC3 QVGA Portrait Menu:\r\n" );
                                    hal_printf( "   1: QVGA Portrait Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Portrait Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Portrait Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Portrait Load Bitmap\r\n" );                                    
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
                                    VDC3_FrameRectangles_Drawing(0);
                                    hal_printf( "\r\n\nVDC3 QVGA Portrait Menu:\r\n" );
                                    hal_printf( "   1: QVGA Portrait Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Portrait Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Portrait Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Portrait Load Bitmap\r\n" );                                    
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '3':
                                    VDC3_FrameRectangles_Drawing(1);
                                    hal_printf( "\r\n\nVDC3 QVGA Portrait Menu:\r\n" );
                                    hal_printf( "   1: QVGA Portrait Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Portrait Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Portrait Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Portrait Load Bitmap\r\n" );                                    
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '4':
                                    hal_printf( "\n\rSelect to display from [U]SB flash: " );
                                    while(TRUE) {
                                        DebuggerPort_Read( HalSystemConfig.DebugTextPort, c, 1 )== 0;
                                        if (c[0] != 0) break;
                                    }
                                    if (c[0] == 'u' || c[0] == 'U')
                                        VDC3_QVGA_Load_Bitmap_App(1, "U");
                                    c[0] = 0;
                                    //VDC3_QVGA_Load_Bitmap_App(1, "S");
                                    hal_printf( "\r\n\nVDC3 QVGA Portrait Menu:\r\n" );
                                    hal_printf( "   1: QVGA Portrait Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Portrait Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Portrait Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Portrait Load Bitmap\r\n" );                                    
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;  
                                default:
                                    hal_printf( "\r\n\nVDC3 QVGA Portrait Menu:\r\n" );
                                    hal_printf( "   1: QVGA Portrait Display colorful rectangles\r\n" );
                                    hal_printf( "   2: QVGA Portrait Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: QVGA Portrait Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: QVGA Portrait Load Bitmap\r\n" );                                    
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        case 6:
                            hal_printf( Data );
                            switch (Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 2;
                                    hal_printf( "\r\n\nVDC3 Menu:\r\n" );
                                    hal_printf( "   [V]GA Tests \r\n" );
                                    hal_printf( "   QVGA [L]andscape Tests \r\n" );
                                    hal_printf( "   QVGA [P]ortrait Tests \r\n" );
                                    hal_printf( "   [W]QVGA Tests \r\n" );                  
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;          
                                case '1':
                                    VDC3_SolidRectangles_Drawing();
                                    hal_printf( "\r\n\nVDC3 WQVGA Menu:\r\n" );
                                    hal_printf( "   1: WQVGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: WQVGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: WQVGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: WQVGA Load Bitmap\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
                                    VDC3_FrameRectangles_Drawing(0);
                                    hal_printf( "\r\n\nVDC3 WQVGA Menu:\r\n" );
                                    hal_printf( "   1: WQVGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: WQVGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: WQVGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: WQVGA Load Bitmap\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '3':
                                    VDC3_FrameRectangles_Drawing(1);
                                    hal_printf( "\r\n\nVDC3 WQVGA Menu:\r\n" );
                                    hal_printf( "   1: WQVGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: WQVGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: WQVGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: WQVGA Load Bitmap\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '4':
                                    hal_printf( "\n\rSelect to display from [U]SB flash: " );
                                    while(TRUE) {
                                        DebuggerPort_Read( HalSystemConfig.DebugTextPort, c, 1 )== 0;
                                        if (c[0] != 0) break;
                                    }
                                    if (c[0] == 'u' || c[0] == 'U')
                                        VDC3_WQVGA_Load_Bitmap_App("U");
                                    c[0] = 0;                               
                                    hal_printf( "\r\n\nVDC3 WQVGA Menu:\r\n" );
                                    hal_printf( "   1: WQVGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: WQVGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: WQVGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: WQVGA Load Bitmap\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                default:
                                    hal_printf( "\r\n\nVDC3 WQVGA Menu:\r\n" );
                                    hal_printf( "   1: WQVGA Display colorful rectangles\r\n" );
                                    hal_printf( "   2: WQVGA Colorful wireframe rectangle\r\n" );
                                    hal_printf( "   3: WQVGA Colorful solid rectangle\r\n" );
                                    hal_printf( "   4: WQVGA Load Bitmap\r\n" );            
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;                      
                        case 7:     //USB Flash test
                            hal_printf( Data );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 1;                                   
                                    hal_printf( "\r\nTest Menu:\r\n" );
                                    hal_printf( "   [V]DC3 Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );
                                    hal_printf( "   [U]SB Flash Tests\r\n" );
                                    hal_printf( "   [S]erial flash memory Tests\r\n" );
                                    hal_printf( "   [N]or flash memory Tests\r\n" );  //Added Nor flash memory test                                 
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case 'O':
                                case 'o':
                                    USBFlashOpenFileTest();
                                    hal_printf( "\r\n\nUSB Flash Menu:\r\n" );
                                    hal_printf( "   [O]pen File Test\r\n" );
                                    hal_printf( "   [R]ead File Test\r\n" );
                                    hal_printf( "   [W]rite File Test\r\n" );
                                    hal_printf( "   [D]elete File Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case 'R':
                                case 'r':
                                    USBFlashReadFileTest();
                                    hal_printf( "\r\n\nUSB Flash Menu:\r\n" );
                                    hal_printf( "   [O]pen File Test\r\n" );
                                    hal_printf( "   [R]ead File Test\r\n" );
                                    hal_printf( "   [W]rite File Test\r\n" );
                                    hal_printf( "   [D]elete File Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case 'W':
                                case 'w':
                                    USBFlashWriteFileTest();
                                    hal_printf( "\r\n\nUSB Flash Menu:\r\n" );
                                    hal_printf( "   [O]pen File Test\r\n" );
                                    hal_printf( "   [R]ead File Test\r\n" );
                                    hal_printf( "   [W]rite File Test\r\n" );
                                    hal_printf( "   [D]elete File Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case 'D':
                                case 'd':
                                    USBFlashDeleteFileTest();
                                    hal_printf( "\r\n\nUSB Flash Menu:\r\n" );
                                    hal_printf( "   [O]pen File Test\r\n" );
                                    hal_printf( "   [R]ead File Test\r\n" );
                                    hal_printf( "   [W]rite File Test\r\n" );
                                    hal_printf( "   [D]elete File Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                default:
                                    hal_printf( "\r\n\nUSB Flash Menu:\r\n" );
                                    hal_printf( "   [O]pen File Test\r\n" );
                                    hal_printf( "   [R]ead File Test\r\n" );
                                    hal_printf( "   [W]rite File Test\r\n" );
                                    hal_printf( "   [D]elete File Test\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;                      
                        //Added Serial Flash memory test
                        case 8:     
                            hal_printf( Data );
                            hal_printf( "\r\n" );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 1;
                                    hal_printf( "\r\nTest Menu:\r\n" );
                                    hal_printf( "   [V]DC3 Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );
                                    hal_printf( "   [U]SB Flash Tests\r\n" );
                                    hal_printf( "   [S]erial flash memory Tests\r\n" );
                                    hal_printf( "   [N]or flash memory Tests\r\n" );  //Added Nor flash memory test                                 
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '1':
                                    g_SerialFlash_BL_DeviceTable.InitializeDevice(pBLOCK_CONFIG_SERIAL);
                                    g_SerialFlash_BL_DeviceTable.EraseBlock(pBLOCK_CONFIG_SERIAL,0xF0000);
                                    hal_printf( "\r\n\nSerial Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0xF0000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
                                    BYTE Buff[0x10];
                                    for(int i=0; i<0x10; i++) Buff[i]=i;
                                    g_SerialFlash_BL_DeviceTable.InitializeDevice(pBLOCK_CONFIG_SERIAL);
                                    g_SerialFlash_BL_DeviceTable.Write(pBLOCK_CONFIG_SERIAL,0xF0000,0x10,Buff,0x0);
                                    hal_printf( "\r\n\nSerial Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0xF0000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '3':
                                    BYTE rBuff[0x10];                                   
                                    g_SerialFlash_BL_DeviceTable.InitializeDevice(pBLOCK_CONFIG_SERIAL);
                                    g_SerialFlash_BL_DeviceTable.Read(pBLOCK_CONFIG_SERIAL,0xF0000,0x10,rBuff);
                                    hal_printf( "Read Data =" );
                                    for(int i=0; i<0x10; i++) hal_printf( " 0x%02x", rBuff[i] );
                                    hal_printf( "\r\n" );
                                    hal_printf( "\r\n\nSerial Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0xF0000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                default:
                                    hal_printf( "\r\n\nSerial Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0xF0000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0xF0000 to 0xF000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;
                        //Added NOR Flash memory test
                        case 9:     
                            hal_printf( Data );
                            hal_printf( "\r\n" );
                            switch(Data[0])
                            {
                                case 'X':
                                case 'x':
                                    TestItem = 1;
                                    hal_printf( "\r\nTest Menu:\r\n" );
                                    hal_printf( "   [V]DC3 Tests\r\n" );
                                    hal_printf( "   [K]eypad Tests\r\n" );                                  
                                    hal_printf( "   [U]SB Flash Tests\r\n" );
                                    hal_printf( "   [S]erial flash memory Tests\r\n" );
                                    hal_printf( "   [N]or flash memory Tests\r\n" );  //Added Nor flash memory test                                 
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '1':
                                    g_AM29DL_16_BS_DeviceTable.InitializeDevice(pBLOCK_CONFIG_NOR);
                                    g_AM29DL_16_BS_DeviceTable.EraseBlock(pBLOCK_CONFIG_NOR,0x20080000);
                                    hal_printf( "\r\n\nNOR Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0x20080000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '2':
                                    BYTE Buff[0x10];
                                    for(int i=0; i<0x10; i++) Buff[i]=i;
                                    g_AM29DL_16_BS_DeviceTable.InitializeDevice(pBLOCK_CONFIG_NOR);
                                    g_AM29DL_16_BS_DeviceTable.Write(pBLOCK_CONFIG_NOR,0x20080000,0x10,Buff,0x0);
                                    hal_printf( "\r\n\nNOR Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0x20080000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                                case '3':
                                    BYTE rBuff[0x10];                                   
                                    g_AM29DL_16_BS_DeviceTable.InitializeDevice(pBLOCK_CONFIG_NOR);
                                    g_AM29DL_16_BS_DeviceTable.Read(pBLOCK_CONFIG_NOR,0x20080000,0x10,rBuff);
                                    hal_printf( "Read Data =" );
                                    for(int i=0; i<0x10; i++) hal_printf( " 0x%02x", rBuff[i] );
                                    hal_printf( "\r\n" );
                                    hal_printf( "\r\n\nNOR Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0x20080000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;          
                                default:
                                    hal_printf( "\r\n\nNOR Flash memory Menu:\r\n" );
                                    hal_printf( "   [1]Erase block (Address 0x20080000)\r\n" );
                                    hal_printf( "   [2]Write block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   [3]Read block (Address 0x20080000 to 0x2008000F)\r\n" );
                                    hal_printf( "   E[x]it\r\n" );
                                    hal_printf( "Your choice: " );
                                    break;
                            }
                            break;  
                    }
                }
            }           
        } while(TRUE);
}

