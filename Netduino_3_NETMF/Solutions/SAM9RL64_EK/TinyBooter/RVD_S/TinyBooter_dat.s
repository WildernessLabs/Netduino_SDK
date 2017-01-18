; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 
;  
;  
;  This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
;  Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
;  the terms of the Microsoft license agreement under which you licensed this sample source code. 
;  
;  THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
;  INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
;  
;  
; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; ; 

    AREA |.text|, CODE, READONLY

    ; has to keep it as ARM code, otherwise the the label TinyBooterCompressed_Dat and TinyBooterCompressed_Dat_End are 1 word shift.
    ; ARM directive is only valid for ARM/THUMB processor, but not CORTEX 
    IF :DEF:COMPILE_ARM :LOR: :DEF:COMPILE_THUMB
    ARM
    ENDIF

    EXPORT  TinyBooterCompressed_Dat
    IMPORT  EntryPoint


TinyBooterCompressed_Dat
    INCBIN TinyBooter_Compressed.dat
TinyBooterCompressed_Dat_End

    END
