;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Copyright (c) Microsoft Corporation.  All rights reserved.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;*****************************************************************************

;    IMPORT  UNDEF_SubHandler
;    IMPORT  ABORTP_SubHandler
;    IMPORT  ABORTD_SubHandler  

    IMPORT  IRQ_Handler
    IMPORT  EntryPoint
    IMPORT  BootEntry

    EXPORT  BootEntryLoader

;    EXPORT  ARM_Vectors

;*****************************************************************************

    AREA |.text|, CODE, READONLY

    ; ARM directive is only valid for ARM/THUMB processor, but not CORTEX 
    IF :DEF:COMPILE_ARM :LOR: :DEF:COMPILE_THUMB
    ARM
    ENDIF
    
    ; RESET
RESET_VECTOR
    b  SKIP_VECTORS

    ; UNDEF INSTR
UNDEF_VECTOR
    b  ABORT_VECTOR

    ; SWI
SWI_VECTOR
    b  ABORT_VECTOR

    ; PREFETCH ABORT
PREFETCH_VECTOR
    b  ABORT_VECTOR

    ; DATA ABORT
DATA_VECTOR
    b  ABORT_VECTOR

    ; unused
USED_VECTOR
    DCD     0x27000

    ; IRQ
IRQ_VECTOR
    b  IRQ_HANDLER

FIQ_Handler
    b  ABORT_VECTOR
    
ABORT_VECTOR
    b ABORT_VECTOR

IRQ_HANDLER
    ldr  pc, IRQ_Handler_Addr

IRQ_Handler_Addr
    dcd  IRQ_Handler

_main
SKIP_VECTORS
    b EntryPoint

BootEntryLoader
    b  BootEntry

;*****************************************************************************

    IF :DEF:COMPILE_THUMB  
    THUMB
    ENDIF

    END

