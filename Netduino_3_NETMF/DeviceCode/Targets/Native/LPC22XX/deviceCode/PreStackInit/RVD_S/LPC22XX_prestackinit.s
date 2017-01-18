;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (c) Microsoft Corporation.  All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    EXPORT  PreStackInit

    IMPORT  PreStackInit_Exit_Pointer

    IMPORT  BootstrapCode_Clocks

    IMPORT BootstrapCode_Pins

    IMPORT BootstrapCode_Ext_SRAM

    IMPORT BootstrapCode_Ext_Flash

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

    ldr     sp,=0x40004000

    bl      BootstrapCode_Clocks

    bl      BootstrapCode_Pins


    IF TargetLocation != "RAM"

    bl      BootstrapCode_Ext_SRAM

    ENDIF

    bl      BootstrapCode_Ext_Flash

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


