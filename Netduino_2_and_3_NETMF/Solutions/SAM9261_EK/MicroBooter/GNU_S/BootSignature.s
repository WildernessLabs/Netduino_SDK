@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .global  BootEntry

    .extern  BootEntry
    .extern  IRQ_Handler

    .section i.BootSignature, "xa", %progbits
    
    .arm

    @ RESET
RESET_VECTOR:
    b  BootSignature

    @ UNDEF INSTR
UNDEF_VECTOR:
    b  ABORT_HANDLER

    @ SWI
SWI_VECTOR:
    b  ABORT_HANDLER

    @ PREFETCH ABORT
PREFETCH_VECTOR:
    b  ABORT_HANDLER

    @ DATA ABORT
DATA_VECTOR:
    b  ABORT_HANDLER

    @ unused
USED_VECTOR:
    .word  0x27000

    @ IRQ
IRQ_VECTOR:
    b  IRQ_HANDLER

    @ FIQ
FIQ_Handler:
    b  ABORT_HANDLER

IRQ_HANDLER:
    ldr  pc, IRQ_Handler_Addr

IRQ_Handler_Addr:
    .word  IRQ_Handler

ABORT_HANDLER:
    b ABORT_HANDLER

BootSignature:
_main:
    b BootEntry

BootEntryLoader:
    b BootEntry

    .ifdef COMPILE_THUMB
	  .thumb
    .endif

    .end

