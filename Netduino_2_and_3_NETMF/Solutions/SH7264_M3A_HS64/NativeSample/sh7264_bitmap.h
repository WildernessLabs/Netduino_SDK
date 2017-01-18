#ifndef __SH7264_BITMAP_H__
#define __SH7264_BITMAP_H__

#define RGB24BIT16BIT(r,g,b) ((b>>3) + ((g>>2) << 5) + ((r>>3) << 11))  
#define RGB24BIT16BIT2(r,g,b)(((r & 0xF8) << 8)|((g & 0xFC) << 3)|((b & 0xF8) >> 3))
#define RGB15BIT16BIT(rgb)((rgb & 0xFF80) << 1)|(rgb & 0x7F)

#define HEAP_AREA       0x09A00000

typedef struct                       /**** BMP file header structure ****/
{
    unsigned short bfType;           /* Magic number for file */
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} bitmap_file_header_t;

#define BF_TYPE         0x4D42       /* "MB" */
#define BF_REVERSE_TYPE 0x424D       /* "BM" */

typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} bitmap_info_header_t;

/*
 * Constants for the biCompression field...
 */

#define BI_RGB       0             /* No compression - straight BGR data */
#define BI_RLE8      1             /* 8-bit run-length compression */
#define BI_RLE4      2             /* 4-bit run-length compression */
#define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

typedef struct                       /**** Colormap entry structure ****/
{
    unsigned char  rgb_blue;          /* Blue value */
    unsigned char  rgb_green;         /* Green value */
    unsigned char  rgb_ged;           /* Red value */
    unsigned char  rgb_reserved;      /* Reserved */
} rgb_quad_t;

typedef struct                       /**** Bitmap information structure ****/
{
    bitmap_info_header_t    bmiHeader;      /* Image header */
    rgb_quad_t              bmiColors[256]; /* Image colormap */
} bitmap_info_t;

typedef struct
{
    unsigned char blue,
                  green,
                  red;
}color_t;

typedef struct _JPEG
{
    short soi;
    char identifier[5];
    short version;
    char units;
    short Xdensity;
    short Ydensity;
    char Xthumbnail;
    char Ythumbnail;
    char * RGB;
}jpeg_header_t;

typedef unsigned char glubyte_t;

struct SH7264_BITMAP
{
    static glubyte_t *bitmap_load(unsigned long mem, bitmap_info_t *info);
    static unsigned short bitmap_16bits_copy(unsigned short *bmp, unsigned short *lcdmem, 
                            int wsize, int hsize, int invert, int insert, int swap);
    static void bitmap_24bits_copy (unsigned char *bmp, unsigned char *lcdmem, 
                             int wsize, int hsize, int invert );
    static void bitmap_32bits_copy (unsigned long *bmp, unsigned long *lcdmem, 
                             int wsize, int hsize, int invert );
    
    static void bitmap_convert_RGB565_YUV444(unsigned short * bmp, unsigned char * Ymem, 
                                      unsigned char * UVmem, int wsize, int hsize);
    static void bitmap_convert_RGB888_YUV444(unsigned char * p, unsigned char * Ymem, 
                                      unsigned char * UVmem, int wsize, int hsize );
    static void bitmap_convert_RGB555_YUV444(unsigned short * p, unsigned char * Ymem, 
                                      unsigned char * UVmem, int wsize, int hsize);
    static void bitmap_convert_RGB888_YUV422(unsigned char * p, unsigned char * Ymem, 
                                      unsigned char * UVmem, int wsize, int hsize);
    static void bitmap_convert_RGB555_YUV422(unsigned short * p, unsigned char * Ymem, 
                                      unsigned char * UVmem, int wsize, int hsize);
    static void bitmap_convert_RGB888_YUV420(unsigned char * p, unsigned char * Ymem, 
                                      unsigned char * UVmem, int wsize, int hsize);
    static void bitmap_convert_RGB555_YUV420(unsigned short * p, unsigned char * Ymem, 
                                      unsigned char * UVmem, int wsize, int hsize);
    static unsigned short read_word(unsigned long mem);
    static unsigned long read_dword(unsigned long mem);
    static long read_long(unsigned long mem);
    static void bitmap_convert_endian(unsigned long memadd, int size);
    static unsigned short short_color(unsigned long long_color);
    static void draw_line(int line, int w, unsigned short color, unsigned short *data);
    static void draw_line2(int line, int l, int r, int w, unsigned short color, unsigned short *data);
    static void draw_vline(int col, int w, int h, unsigned short color, unsigned short *data);
    static void draw_vline2(int col, int t, int b, int w, unsigned short color, unsigned short *data);  
    static int delete_lines(int line, int w, int h, int num_line, unsigned short *buf);
    static void draw_buffer(int line, int col, int w, int h, unsigned short * buf);
    static void draw_multi_rectangle(int w, int h, unsigned short * buf);
    static void read_string(char dest, char src, int size);
    static void bitmap_rotate(unsigned short *bmp, unsigned short *lcdmem, int xsize, int ysize );
    static unsigned short bitmap_convert_24bits_2_16bits(unsigned char * bitmap, unsigned short *lcdbuf, int wsize, int hsize );
    static void video_16bits_copy(unsigned short *bmp, unsigned short *vbuf, 
                                    int wsize, int hsize, int invert, int insert, int swap);
};                            
                        
#endif /* __SH7264_BITMAP_H__ */
