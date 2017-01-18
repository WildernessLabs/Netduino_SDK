;-----------------------------------------------------------------------------
; Software that is described herein is for illustrative purposes only  
; which provides customers with programming information regarding the  
; products. This software is supplied "AS IS" without any warranties.  
; NXP Semiconductors assumes no responsibility or liability for the 
; use of the software, conveys no license or title under any patent, 
; copyright, or mask work right to the product. NXP Semiconductors 
; reserves the right to make changes in the software without 
; notification. NXP Semiconductors also make no representation or 
; warranty that such application will be suitable for the specified 
; use without further testing or modification. 
;-----------------------------------------------------------------------------

    EXPORT  PreStackInit

    IMPORT  PreStackInit_Exit_Pointer
    IMPORT  platform_prestackinit

    PRESERVE8

    AREA SectionForBootstrapOperations, CODE, READONLY

    ; ARM directive is only valid for ARM/THUMB processor, but not CORTEX 
    IF :DEF:COMPILE_ARM :LOR: :DEF:COMPILE_THUMB  
    ARM
    ENDIF

PreStackInit

    ;*************************************************************************
    ;;
    ;;     Change SP back to Internal RAM. EMC is not yet initialized
    ldr     sp,=0x40008000
    bl      platform_prestackinit

    ;*************************************************************************
    ; DO NOT CHANGE THE FOLLOWING CODE! we can not use pop to return because we 
    ; loaded the PC register to get here (since the stack has not been initialized).  
    ; Make sure the PreStackInit_Exit_Pointer is within range and 
    ; in the SectionForBootstrapOperations
    ; go back to the firstentry(_loader) code 
    ;

PreStackEnd
    B    PreStackInit_Exit_Pointer

    ;
    ;**************************************************************************

    IF :DEF:COMPILE_THUMB  
    THUMB
    ENDIF

    END
