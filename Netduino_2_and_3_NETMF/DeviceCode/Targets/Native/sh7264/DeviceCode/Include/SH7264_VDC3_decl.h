/*
 *    Copyright (C) 2010 Renesas Electronics America Inc. All rights reserved.
 */

#ifndef _SH7264_VDC3_DECL_
#define _SH7264_VDC3_DECL_ 1

#include "..\vdc3\sh7264_vdc3.h"

//--//  Video Display Controller 3 //--//

BOOL VDC3_Initialize                  (unsigned short uWidth, unsigned short uHeight);
BOOL VDC3_Controller_Initialize         ( SH7264VDC3_CONTROLLER_CONFIG& config );
BOOL VDC3_Controller_Uninitialize       (                                   );
BOOL VDC3_Controller_Enable             ( BOOL fEnable                      );
unsigned short* VDC3_GetFrameBuffer     (                                   );

#endif //_SH7264_VDC3_DECL_