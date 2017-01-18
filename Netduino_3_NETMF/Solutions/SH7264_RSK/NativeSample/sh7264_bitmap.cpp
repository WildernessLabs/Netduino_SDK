#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SH7264_bitmap.h"

#ifndef BIG
#define BIG 0
#endif

#ifndef ENDIAN
#define ENDIAN  BIG
#endif

bitmap_file_header_t header;       /* File header */

glubyte_t* SH7264_BITMAP::bitmap_load(unsigned long mem,    /* I - Memory to load */
                                bitmap_info_t *info)        /* O - Bitmap information */
{   
    unsigned long   memadd;
    int             bitsize;      /* Size of bitmap */
    int             infosize;     /* Size of header information */
    short           bconvert = 0, bmp_type;

    memadd = mem;

    /* Check for BM reversed... */
    bmp_type = read_word(memadd);
    if ((bmp_type == BF_TYPE) || (bmp_type == BF_REVERSE_TYPE))
    {
        if (bmp_type == BF_REVERSE_TYPE)
            bconvert = 1;       
    }
    else
        return (NULL);

    /* Convert endian of bitmap header */
    if (bconvert)
        bitmap_convert_endian(memadd, 14);
        
    /*  Read bitmap header  */
    header.bfType      = read_word(memadd);
    memadd += 2;
    
    header.bfSize      = read_dword(memadd);
    memadd += 4;

    header.bfReserved1 = read_word(memadd);
    memadd += 2;

    header.bfReserved2 = read_word(memadd);
    memadd += 2;

    header.bfOffBits   = read_dword(memadd);
    memadd += 4;

    if (header.bfType != BF_TYPE) /* Check for BM reversed... */
    {
        /* Not a bitmap file - return 0... */
        return (NULL);
    }

    infosize = header.bfOffBits - 14 /* sizeof(bitmap_file_header_t)*/;
    
    /* Convert endian of bitmap Info         */
    if (bconvert)
        bitmap_convert_endian(memadd, infosize);
        
    info->bmiHeader.biSize = read_dword(memadd);
    memadd += 4;

    info->bmiHeader.biWidth = read_long(memadd);
    memadd += 4;

    info->bmiHeader.biHeight = read_long(memadd);
    memadd += 4;

    info->bmiHeader.biPlanes = read_word(memadd);
    memadd += 2;

    info->bmiHeader.biBitCount = read_word(memadd);
    memadd += 2;
    
    info->bmiHeader.biCompression = read_dword(memadd);
    memadd += 4;
    
    info->bmiHeader.biSizeImage = read_dword(memadd);
    memadd += 4;
    
    info->bmiHeader.biXPelsPerMeter = read_long(memadd);
    memadd += 4;

    info->bmiHeader.biYPelsPerMeter = read_long(memadd);
    memadd += 4;

    info->bmiHeader.biClrUsed = read_dword(memadd);
    memadd += 4;

    info->bmiHeader.biClrImportant = read_dword(memadd);
    memadd += 4;

    if (info->bmiHeader.biCompression != 0)
        return (NULL);

    if (infosize > 40)
    {
        if (bconvert)
            bitmap_convert_endian(memadd, infosize-40);
        
        memcpy(&info->bmiColors, (unsigned long *)memadd, infosize - 40);
        memadd += infosize - 40;
    }   
    /* Now that we have all the header info read in, allocate memory for *
     * the bitmap and read *it* in...                                    */
    if ((bitsize = info->bmiHeader.biSizeImage) == 0)
    {
        bitsize = (info->bmiHeader.biWidth *
                   info->bmiHeader.biBitCount + 7) / 8 *
               abs(info->bmiHeader.biHeight);
    }
    
    //Convert endian of bitmap data
    if ( bconvert && (info->bmiHeader.biBitCount != 24) )// && (info->bmiHeader.biPlanes)>8)
        bitmap_convert_endian(memadd, bitsize);
        
    /* OK, everything went fine - return the position of image data */
    return ((glubyte_t *)memadd);
}

void SH7264_BITMAP::bitmap_rotate(unsigned short *bmp, unsigned short *lcdmem, int xsize, int ysize )
{
    int i, j;   
    unsigned short * mem = lcdmem;
                    
    for (i = 0; i < xsize; ++i)
        for (j = ysize-1; j >= 0; j--)
            *mem++ = (bmp[(xsize*j + i)]);
            
}

unsigned short SH7264_BITMAP::bitmap_16bits_copy(unsigned short *bmp, unsigned short *lcdbuf, 
                        int wsize, int hsize, int rotate, int insert, int swap)
{
    int i, j;   
    unsigned short * p = bmp, word;
    
    for (j = hsize-1; j >= 0; j--)
    {
        if (rotate) 
            p = bmp + (j * wsize);
        for (i = 0; i < wsize; i++)
        {
            word = *(p);
            if (swap) 
                word = ((word & 0xFF00)>>8)|((word & 0xFF)<<8);
            if (insert)
            {
                /* Convert RGB555 to RGB565 */
                *lcdbuf++ =  ((word & 0x7C00) << 1) 
                          |  ((word & 0x03E0) << 1)
                          |   (word & 0x1F);
            }
            else
            {
                *lcdbuf++ = *p;
            }
            p++;
        }   
    }   
    return (*(lcdbuf-1));
}

void SH7264_BITMAP::bitmap_24bits_copy (unsigned char *bmp, unsigned char *lcdmem, 
                         int wsize, int hsize, int invert )
{
    /* RRGGBB -> 00RRGGBB */
    int i, j;   
    unsigned char * p = bmp;

    for (j = hsize-1; j>=0; j--)
    {
        if (invert) p = bmp + (wsize * j)*3;
        for (i = 0; i < wsize; i++)
        {
            *lcdmem++ = p[0];
            *lcdmem++ = p[1];
            *lcdmem++ = p[2];
            p += 3;
        }
    }
}

void SH7264_BITMAP::bitmap_32bits_copy(unsigned long *bmp, unsigned long *lcdmem, 
                        int wsize, int hsize, int invert )
{
    int i,j;    
    unsigned long * p = bmp;        
    for (j = hsize-1; j>=0; j--)
    {
        if (invert) p = bmp + (wsize * j);
        for (i = 0; i < wsize; i++)
        {
            *lcdmem++ = (*p);
            p++;
        }
    }   
}

unsigned short SH7264_BITMAP::bitmap_convert_24bits_2_16bits(unsigned char * bitmap, unsigned short *lcdbuf, int wsize, int hsize )
{
    int pix, i, j;
    unsigned char blue, green, red;
    
    for (j = hsize-1; j >= 0; j--)
        for (i = 0; i < wsize; ++i)
        {
            pix = (wsize*j + i)*3;

            blue    = bitmap[pix];
            green   = bitmap[pix + 1];
            red     = bitmap[pix + 2];

            *lcdbuf++ = RGB24BIT16BIT(red, green, blue);
        }
    return (*(lcdbuf-1));
}

void SH7264_BITMAP::video_16bits_copy(unsigned short *bmp, unsigned short *vbuf, 
                        int wsize, int hsize, int invert, int insert, int swap)
{
    int i,j;    
    unsigned short * p = bmp, word;
    unsigned short * vi;
    
    /* Make top video data */
    vi = vbuf + (16 * wsize);
    for (j = 0; j < hsize; j += 2)
    {
        p = bmp + (j * wsize);
        for (i = 0; i < wsize; i++)
        {
            word = *(p);
            if (swap) word = ((word & 0xFF00)>>8)|((word & 0xFF)<<8);
            if (insert)
            {
                /* Convert RGB555 to RGB565 */
                word =  ((word & 0x7C00) << 1) 
                          | (((word & 0x03E0) << 1) | (0x20)) 
                          |   (word & 0x1F);
            }
            *vi++ = word;
            p++;
        }   
    }   

    /* Make bottom video data */
    vi = vbuf + (279 * wsize);
    for (j = 1; j < hsize; j += 2)
    {
        p = bmp + (j * wsize);
        for (i = 0; i < wsize; i++)
        {
            word = *(p);
            if (swap) word = ((word & 0xFF00)>>8)|((word & 0xFF)<<8);
            if (insert)
            {
                /* Convert RGB555 to RGB565 */
                word =  ((word & 0x7C00) << 1) 
                          | (((word & 0x03E0) << 1) | (0x20)) 
                          |   (word & 0x1F);
            }
            *vi++ = word;
            p++;
        }   
    }   
}

void SH7264_BITMAP::bitmap_convert_RGB565_YUV444(unsigned short * bmp, unsigned char * Ymem, unsigned char * UVmem, int wsize, int hsize)
{
    int i, j, m = 0, n = 0; 
    unsigned char R, G, B, Y, U, V, c;
    
    for (j = hsize-1; j>=0; j--)
    {
        for (i = 0; i < wsize; i++)
        {
            //Convert RGB555 to RGB888
            c = bmp[(wsize*j + i)];
            R = (c & 0x7C00) >> 7;
            G = (c & 0x03E0) >> 3; 
            B = (c & 0x1F) << 3;
            
            //Convert RGB888 to YUV444  
            Y = ( (  66 * R + 129 * G +  25 * B + 128) >> 8) +  16;
            U = ( ( -38 * R -  74 * G + 112 * B + 128) >> 8) + 128;
            V = ( ( 112 * R -  94 * G -  18 * B + 128) >> 8) + 128;
            Ymem[m++] = Y;
            UVmem[n] = U;
            UVmem[n+1] = V;
            n += 2;
        }   
    }   
}

void SH7264_BITMAP::bitmap_convert_RGB888_YUV444(unsigned char * p, unsigned char * Ymem, unsigned char * UVmem, int wsize, int hsize )
{   
    unsigned char * yp = Ymem;
    unsigned char * up = UVmem;
    unsigned char * vp = UVmem + 1;
    unsigned long * r2yuv = (unsigned long *)HEAP_AREA; 
    unsigned long * g2yuv = (unsigned long *)HEAP_AREA + 256*4; 
    unsigned long * b2yuv = (unsigned long *)HEAP_AREA + 256*4*2; 
    int x, y, u, v, i, j;
    unsigned int yuv ;
    
    for (x = 0; x < 256; ++x) {
        /* can't have overflow in this direction */
        y = (0.299 * x);
        u = (-0.1687 * x);
        u &= 0xff;
        v = (0.5 * x);
        r2yuv[x] = y | u << 10 | v << 20;

        y = (0.587 * x);
        u = (-0.3313 * x);
        u &= 0xff;
        v = (-0.4187 * x);
        v &= 0xff;
        g2yuv[x] = y | u << 10 | v << 20;

        y = (0.114 * x);
        u = (0.5 * x);
        v = (- 0.0813 * x);
        v &= 0xff;
        b2yuv[x] = y | u << 10 | v << 20;
    }
    
    for (j = hsize-1; j >=0 ; j--)
    {
        for (i = 0; i < wsize; i += 1)
        {
            yuv = r2yuv[p[(3*wsize*j + i*3 + 2)]];
            yuv += g2yuv[p[(3*wsize*j + i*3 + 1)]];
            yuv += b2yuv[p[(3*wsize*j + i*3)]];

            *yp++ = yuv;
            *up = (yuv >> 10) ^ 0x80;
            *vp = (yuv >> 20) ^ 0x80;
            up += 2;
            vp += 2;
        }
    }
}

void SH7264_BITMAP::bitmap_convert_RGB555_YUV444(unsigned short * p, unsigned char * Ymem, unsigned char * UVmem, int wsize, int hsize)
{   
    unsigned char * yp = Ymem;
    unsigned char * up = UVmem;
    unsigned char * vp = UVmem + 1;
    unsigned long * r2yuv = (unsigned long *)HEAP_AREA; 
    unsigned long * g2yuv = (unsigned long *)HEAP_AREA + 256*4; 
    unsigned long * b2yuv = (unsigned long *)HEAP_AREA + 256*4*2; 
    int x, y, u, v, i, j;
    unsigned int yuv ;
    unsigned char r, g, b;
    
    for (x = 0; x < 256; ++x) {
        /* can't have overflow in this direction */
        y = (0.299 * x);
        u = (-0.1687 * x);
        u &= 0xff;
        v = (0.5 * x);
        r2yuv[x] = y | u << 10 | v << 20;

        y = (0.587 * x);
        u = (-0.3313 * x);
        u &= 0xff;
        v = (-0.4187 * x);
        v &= 0xff;
        g2yuv[x] = y | u << 10 | v << 20;

        y = (0.114 * x);
        u = (0.5 * x);
        v = (- 0.0813 * x);
        v &= 0xff;
        b2yuv[x] = y | u << 10 | v << 20;
    }
    
    for (j = hsize-1; j >=0 ; j--)
    {
        for (i = 0; i < wsize; i++)
        {
            r = (((p[(wsize*j + i)]) >> 7) & 0xf8);
            g = (((p[(wsize*j + i)]) >> 2) & 0xf8);
            b = (((p[(wsize*j + i)]) << 3) & 0xf8);
            
            yuv = r2yuv[r];
            yuv += g2yuv[g];
            yuv += b2yuv[b];

            *yp++ = yuv;
            *up = (yuv >> 10) ^ 0x80;
            *vp = (yuv >> 20) ^ 0x80;
            
            up += 2;
            vp += 2;
        }
    }
}

void SH7264_BITMAP::bitmap_convert_RGB888_YUV422(unsigned char * p, unsigned char * Ymem, unsigned char * UVmem, int wsize, int hsize)
{   
    unsigned char * yp = Ymem;
    unsigned char * up = UVmem;
    unsigned char * vp = UVmem + 1;
    unsigned long * r2yuv = (unsigned long *)HEAP_AREA; 
    unsigned long * g2yuv = (unsigned long *)HEAP_AREA + 255*4; 
    unsigned long * b2yuv = (unsigned long *)HEAP_AREA + 255*4*2; 
    int x, y, u, v, i, j;
    unsigned int yuv ;
    
    for (x = 0; x < 256; ++x) {
        /* can't have overflow in this direction */
        y = (0.299 * x);
        u = (-0.1687 * x);
        u &= 0xff;
        v = (0.5 * x);
        r2yuv[x] = y | u << 10 | v << 20;

        y = (0.587 * x);
        u = (-0.3313 * x);
        u &= 0xff;
        v = (-0.4187 * x);
        v &= 0xff;
        g2yuv[x] = y | u << 10 | v << 20;

        y = (0.114 * x);
        u = (0.5 * x);
        v = (- 0.0813 * x);
        v &= 0xff;
        b2yuv[x] = y | u << 10 | v << 20;
    }
    
    for (j = hsize-1; j >=0 ; j--)
    {
        for (i = 0; i < wsize; i += 2)
        {
            yuv = r2yuv[p[(3*wsize*j + i*3 + 2)]];
            yuv += g2yuv[p[(3*wsize*j + i*3 + 1)]];
            yuv += b2yuv[p[(3*wsize*j + i*3)]];

            *yp++ = yuv;
            *up = (yuv >> 10) ^ 0x80;
            *vp = (yuv >> 20) ^ 0x80;
            up += 2;
            vp += 2;

            yuv = r2yuv[p[(3*wsize*j + (i+1)*3 + 2)]];
            yuv += g2yuv[p[(3*wsize*j + (i+1)*3 + 1)]];
            yuv += b2yuv[p[(3*wsize*j + (i+1)*3)]];

            *yp++ = yuv;
        }
    }
}

void SH7264_BITMAP::bitmap_convert_RGB555_YUV422(unsigned short * p, unsigned char * Ymem, unsigned char * UVmem, int wsize, int hsize)
{   
    unsigned char * yp = Ymem;
    unsigned char * up = UVmem;
    unsigned char * vp = UVmem + 1;
    unsigned long * r2yuv = (unsigned long *)HEAP_AREA; 
    unsigned long * g2yuv = (unsigned long *)HEAP_AREA + 255*4; 
    unsigned long * b2yuv = (unsigned long *)HEAP_AREA + 255*4*2; 
    int x, y, u, v, i, j;
    unsigned int yuv ;
    unsigned char r, g, b;
    
    for (x = 0; x < 256; ++x) {
        /* can't have overflow in this direction */
        y = (0.299 * x);
        u = (-0.1687 * x);
        u &= 0xff;
        v = (0.5 * x);
        r2yuv[x] = y | u << 10 | v << 20;

        y = (0.587 * x);
        u = (-0.3313 * x);
        u &= 0xff;
        v = (-0.4187 * x);
        v &= 0xff;
        g2yuv[x] = y | u << 10 | v << 20;

        y = (0.114 * x);
        u = (0.5 * x);
        v = (- 0.0813 * x);
        v &= 0xff;
        b2yuv[x] = y | u << 10 | v << 20;
    }
    
    for (j = hsize-1; j >=0 ; j--)
    {
        for (i = 0; i < wsize; i += 2)
        {
            r = (((p[(wsize*j + i)]) >> 7) & 0xf8);
            g = (((p[(wsize*j + i)]) >> 2) & 0xf8);
            b = (((p[(wsize*j + i)]) << 3) & 0xf8);
            
            yuv = r2yuv[r];
            yuv += g2yuv[g];
            yuv += b2yuv[b];

            *yp++ = yuv;
            *up = (yuv >> 10) ^ 0x80;
            *vp = (yuv >> 20) ^ 0x80;
            
            up += 2;
            vp += 2;
            
            r = (((p[(wsize*j + i + 1)]) >> 7) & 0xf8);
            g = (((p[(wsize*j + i + 1)]) >> 2) & 0xf8);
            b = (((p[(wsize*j + i + 1)]) << 3) & 0xf8);

            yuv = r2yuv[r];
            yuv += g2yuv[g];
            yuv += b2yuv[b];

            *yp++ = yuv;
        }
    }
}

void SH7264_BITMAP::bitmap_convert_RGB888_YUV420(unsigned char * p, unsigned char * Ymem, unsigned char * UVmem, int wsize, int hsize)
{   
    unsigned char * yp = Ymem;
    unsigned char * up = UVmem;
    unsigned char * vp = UVmem + 1;
    unsigned long * r2yuv = (unsigned long *)HEAP_AREA; 
    unsigned long * g2yuv = (unsigned long *)HEAP_AREA + 255*4; 
    unsigned long * b2yuv = (unsigned long *)HEAP_AREA + 255*4*2; 
    int x, y, u, v, i, j, next_line;
    unsigned int yuv ;
    
    for (x = 0; x < 256; ++x) {
        /* can't have overflow in this direction */
        y = (0.299 * x);
        u = (-0.1687 * x);
        u &= 0xff;
        v = (0.5 * x);
        r2yuv[x] = y | u << 10 | v << 20;

        y = (0.587 * x);
        u = (-0.3313 * x);
        u &= 0xff;
        v = (-0.4187 * x);
        v &= 0xff;
        g2yuv[x] = y | u << 10 | v << 20;

        y = (0.114 * x);
        u = (0.5 * x);
        v = (- 0.0813 * x);
        v &= 0xff;
        b2yuv[x] = y | u << 10 | v << 20;
    }
    
    next_line = wsize * 24 / 8 ;//24 bit
    
    for (j = hsize-1; j >=0 ; j--)
    {
        for (i = 0; i < wsize; i += 2)
        {
            yuv = r2yuv[p[(3*wsize*j + i*3 + 2)]];
            yuv += g2yuv[p[(3*wsize*j + i*3 + 1)]];
            yuv += b2yuv[p[(3*wsize*j + i*3)]];
            
            // get Y, U, and V from RGB(x,y)
            yp[0] = yuv;
            *up = (yuv >> 10) ^ 0x80;
            *vp = (yuv >> 20) ^ 0x80;
            up += 2;
            vp += 2;

            // get only Y from RGB(x,y+1)
            yuv = r2yuv[p[(3*wsize*j + i*3) + next_line + 2]];
            yuv += g2yuv[p[(3*wsize*j + i*3) + next_line + 1]];
            yuv += b2yuv[p[(3*wsize*j + i*3) + next_line + 0]];
            yp[wsize] = yuv;
            ++yp;

            // get only Y from RGB(x+1,y)
            yuv = r2yuv[p[(3*wsize*j + (i+1)*3) + 2]];
            yuv += g2yuv[p[(3*wsize*j + (i+1)*3) + 1]];
            yuv += b2yuv[p[(3*wsize*j + (i+1)*3)]];
            yp[0] = yuv;

            // get only Y from RGB(x+1,y+1)
            yuv = r2yuv[p[(3*wsize*j + (i+1)*3) + next_line + 2]];
            yuv += g2yuv[p[(3*wsize*j + (i+1)*3) + next_line + 1]];
            yuv += b2yuv[p[(3*wsize*j + (i+1)*3) + next_line]];
            yp[wsize] = yuv;
            ++yp;
        }
    }
}

void SH7264_BITMAP::bitmap_convert_RGB555_YUV420(unsigned short * p, unsigned char * Ymem, unsigned char * UVmem, int wsize, int hsize)
{   
    unsigned char * yp = Ymem;
    unsigned char * up = UVmem;
    unsigned char * vp = UVmem + 1;
    unsigned long * r2yuv = (unsigned long *)HEAP_AREA; 
    unsigned long * g2yuv = (unsigned long *)HEAP_AREA + 255*4; 
    unsigned long * b2yuv = (unsigned long *)HEAP_AREA + 255*4*2; 
    int x, y, u, v, i, j, next_line;
    unsigned int yuv ;
    unsigned char r, g, b;
    
    for (x = 0; x < 256; ++x) {
        /* can't have overflow in this direction */
        y = (0.299 * x);
        u = (-0.1687 * x);
        u &= 0xff;
        v = (0.5 * x);
        r2yuv[x] = y | u << 10 | v << 20;

        y = (0.587 * x);
        u = (-0.3313 * x);
        u &= 0xff;
        v = (-0.4187 * x);
        v &= 0xff;
        g2yuv[x] = y | u << 10 | v << 20;

        y = (0.114 * x);
        u = (0.5 * x);
        v = (- 0.0813 * x);
        v &= 0xff;
        b2yuv[x] = y | u << 10 | v << 20;
    }

    next_line = wsize * 16 / 8 ;//16 bit
    
    for (j = hsize-1; j >= 0 ; j--)
    {
        for (i = 0; i < wsize; i += 2)
        {
            // get Y, U, and V from RGB(x,y)
            r = (((p[(wsize*j + i)]) >> 7) & 0xf8);
            g = (((p[(wsize*j + i)]) >> 2) & 0xf8);
            b = (((p[(wsize*j + i)]) << 3) & 0xf8);
            
            yuv = r2yuv[r];
            yuv += g2yuv[g];
            yuv += b2yuv[b];
            
            yp[0] = yuv;
            *up = (yuv >> 10) ^ 0x80;
            *vp = (yuv >> 20) ^ 0x80;
            up += 2;
            vp += 2;

            // get only Y from RGB(x,y+1)
            r = (((p[wsize*j + next_line + i]) >> 7) & 0xf8);
            g = (((p[wsize*j + next_line + i]) >> 2) & 0xf8);
            b = (((p[wsize*j + next_line + i]) << 3) & 0xf8);
            
            yuv = r2yuv[r];
            yuv += g2yuv[g];
            yuv += b2yuv[b];
            yp[wsize] = yuv;
            ++yp;

            // get only Y from RGB(x+1,y)
            r = (((p[wsize*j + i + 1]) >> 7) & 0xf8);
            g = (((p[wsize*j + i + 1]) >> 2) & 0xf8);
            b = (((p[wsize*j + i + 1]) << 3) & 0xf8);

            yuv = r2yuv[r];
            yuv += g2yuv[g];
            yuv += b2yuv[b];
            yp[0] = yuv;

            // get only Y from RGB(x+1,y+1)
            r = (((p[wsize*j + next_line + i + 1]) >> 7) & 0xf8);
            g = (((p[wsize*j + next_line + i + 1]) >> 2) & 0xf8);
            b = (((p[wsize*j + next_line + i + 1]) << 3) & 0xf8);

            yuv = r2yuv[r];
            yuv += g2yuv[g];
            yuv += b2yuv[b];
            yp[wsize] = yuv;
            ++yp;
        }
    }
}

unsigned short SH7264_BITMAP::short_color(unsigned long long_color)
{
    short color = long_color & 0xf80000 >> 11 
                | long_color & 0x00fc00 >> 6
                | long_color & 0x0000f8 >> 3; 
                
    return(color);
}

unsigned short SH7264_BITMAP::read_word(unsigned long mem)
{
    unsigned short data;
    
    data = *((volatile unsigned short *)mem);
    return data;
}

unsigned long SH7264_BITMAP::read_dword(unsigned long mem)
{
    unsigned long data;
    
    memcpy(&data, (unsigned long *)mem, 4);
    if (ENDIAN == BIG)
        data = ((data & 0xFFFF0000)>>16) | ((data & 0xFFFF)<<16);   
    return data;
}

long SH7264_BITMAP::read_long(unsigned long mem)
{
    long data;
    
    memcpy(&data, (unsigned long *)mem, 4);
    
    if (ENDIAN == BIG)
        data = ((data & 0xFFFF0000)>>16) | ((data & 0xFFFF)<<16);   
    return data;
}

void SH7264_BITMAP::bitmap_convert_endian(unsigned long memadd, int size)
{
    int i;
    volatile unsigned short * mem = (volatile unsigned short *)memadd;
    unsigned short word;
    
    for (i=0; i<size/2; i++)
    {
        word = *(mem);
        word =  ((word & 0xFF00)>>8)|((word & 0xFF)<<8);
        *(mem) = word;
        mem += 1;
    }   
}

void SH7264_BITMAP::draw_line(int line, int w, unsigned short color, unsigned short *data)
{
    int j;
    for (j = 0; j < w; j++)
    {
        *(data + (line * w) + j) = color;
    }
}

void SH7264_BITMAP::draw_line2(int line, int l, int r, int w, unsigned short color, unsigned short *data)
{
    int j;
    for (j = l; j < r; j++)
    {
        *(data + (line * w) + j) = color;
    }
}

void SH7264_BITMAP::draw_vline(int col, int w, int h, unsigned short color, unsigned short *data)
{
    int j;
    for (j = 0; j < h; j++)
    {
        *(data + (w * j) + col) = color;
    }
}

void SH7264_BITMAP::draw_vline2(int col, int t, int b, int w, unsigned short color, unsigned short *data)
{
    int j;
    for (j = t; j < b; j++)
    {
        *(data + (w * j) + col) = color;
    }
}

int SH7264_BITMAP::delete_lines(int line, int w, int h, int num_line, unsigned short *buf)
{
    int i = line, k = 0;
    
    while(k < num_line)
    {
        draw_line(i, w, 0x0000, buf);//Delete H-line
        i = i + 1;
        if (i == h) i = 0;
        k++;
    }
    return i;
}

void SH7264_BITMAP::draw_buffer(int line, int col, int w, int h, unsigned short * buf)
{   
    int i;
    
    i = line - 2;
    if(i < 0) i = i + h;
    
    draw_line(i, w, 0x0000, buf);//Delete H-line

    i = col - 2;
    if(i < 0) i = i + w;

    draw_vline(i, w, h, 0x0000, buf);//Delete V-line
    
    draw_vline(col, w, h, 0xFFFF, buf); //White color
    draw_line(line, w, 0xF800, buf); //Red color    
}

void SH7264_BITMAP::draw_multi_rectangle(int w, int h, unsigned short * buf)
{
    int i, j = 0;
    unsigned short r, g, b, y;
    int h1 = h - 1, w1 = w - 1;
    
    for (i = 0; i < 10; i++)
    {
        if((i % 2) == 0)
        {
            r = (0xF800) & (0xF800<<j); /* Red color */
            g = (0x07E0) & (0x07E0<<j); /* Green color */
            b = (0x001F) & (0x001F<<j); /* Bue color */
            y = (0xFFE0) & (0xFFE0<<j); /* Yellow color */
            j += 1;
        }
        else
        {
            r = g = b = y = 0x0000;
        }
        draw_line2(i, i, w - i, w, r, buf); /* Top line */
        draw_line2(h1 - i, i, w - i, w, y, buf); /* Bottom line */
        draw_vline2(i , i, h1 - i, w, g, buf); /* Left line */
        draw_vline2(w1 - i, i, h1 - i, w, b, buf); /* Right line */
    }
}