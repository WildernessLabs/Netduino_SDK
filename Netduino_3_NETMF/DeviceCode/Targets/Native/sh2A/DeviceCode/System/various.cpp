////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////#include "tinyhal.h"
#include "tinyhal.h"
#include <string.h>
   
////////////////////////////////////////////////////////////////////////////////////////////////////

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif


#if !defined(BUILD_RTM)

void debug_printf(char const* format, ... )
{

    char    buffer[256];
    va_list arg_ptr;

    va_start( arg_ptr, format );

   int len = hal_vsnprintf( buffer, sizeof(buffer)-1, format, arg_ptr );

    // flush existing characters
    DebuggerPort_Flush( HalSystemConfig.DebugTextPort );

    // write string
    DebuggerPort_Write( HalSystemConfig.DebugTextPort, buffer, len );

    // flush new characters
    DebuggerPort_Flush( HalSystemConfig.DebugTextPort );

    va_end( arg_ptr );


}


void lcd_printf(char const * format,...)
{	
    va_list arg_ptr;

    va_start( arg_ptr, format );

    hal_vfprintf( STREAM_LCD, format, arg_ptr );	
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
void abort(void)
{

}
#endif
void main(void)
{
    HAL_Initialize();
    ApplicationEntryPoint();
}


///////////////////////////////////////////////////////////////////////////////////////////


