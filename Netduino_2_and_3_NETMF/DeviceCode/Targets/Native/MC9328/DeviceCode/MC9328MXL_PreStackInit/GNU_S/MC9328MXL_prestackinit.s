@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .global  PreStackInit

    .extern  PreStackInit_Exit_Pointer
    .extern  CPU_ARM9_BootstrapCode
    .extern  BootstrapCode_Clocks
    .extern  BootstrapCode_SDRAM
	  .extern  CPU_FlushCaches

    .section SectionForBootstrapOperations, "xa", %progbits

	  .arm

PreStackInit:

    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @
    @ The MC9328MXL system does not have any static memory,
    @ so we have to point the stack pointer to some control registers that can act as r/w memory.
    @

    ldr     sp,=0x00223040

	@ disable MMU
    mrc     p15, 0, r10, c1, c0, 0
    bic     r10, r10, #0x0001             @ Disable MMU
    mcr     p15, 0, r10, c1, c0, 0
    @ wait
    mrc     p15, 0, r10, c1, c0, 0
    nop

    mov     r0, #8
    mov     r1, #64
    bl      CPU_ARM9_FlushCaches_asm
    bl      CPU_ARM9_BootstrapCode
    bl      BootstrapCode_Clocks

    .ifndef TARGETLOCATION_RAM

    bl      BootstrapCode_SDRAM

    .endif

    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
    @ DO NOT CHANGE THE FOLLOWING CODE! we can not use pop to return because we 
    @ loaded the PC register to get here (since the stack has not been initialized).  
    @ Make sure the PreStackInit_Exit_Pointer is within range and 
    @ in the SectionForBootstrapOperations
    @ go back to the firstentry(_loader) code 
    @


PreStackEnd:
    B     PreStackInit_Exit_Pointer
    
    @
    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .ifdef COMPILE_THUMB
	  .thumb
    .endif

    .end


