@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .global  PreStackInit
    .global  Remap

    .extern  PreStackInit_Exit_Pointer

    .section SectionForBootstrapOperations, "xa", %progbits

	.arm

PreStackInit:
    ldr     r0, =Remap
    bx      r0 
    
    @*************************************************************************
    @@ The SAM7X boots on internal FLASH at adress 0x00000000
    @@ We have remap FLASH @ 0x00100000 and internal SRAM @ 0x0000000 

Remap:
    ldr     r0,=0x00000000
    ldr     r1,=0xdeadbeef
    str     r1,[r0]
    ldr     r2,[r0]
    cmp     r1, r2
    beq     PreStackEnd    
    ldr     r0,=0xFFFFFF00
    ldr     r1,=1
    str     r1,[r0]

    @*************************************************************************
    @ DO NOT CHANGE THE FOLLOWING CODE! we can not use pop to return because we 
    @ loaded the PC register to get here (since the stack has not been initialized).  
    @ Make sure the PreStackInit_Exit_Pointer is within range and 
    @ in the SectionForBootstrapOperations
    @ go back to the firstentry(_loader) code 
    @
PreStackEnd:
    b       PreStackInit_Exit_Pointer       

    @
    @*************************************************************************

	.ifdef COMPILE_THUMB
	.thumb
	.endif

    .end


