@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

    .global PreStackInit

    .extern PreStackInit_Exit_Pointer
    .extern BootstrapCode_Clocks
    .extern BootstrapCode_Pins
    .extern BootstrapCode_Ext_SRAM
    .extern BootstrapCode_Ext_Flash

    .section SectionForBootstrapOperations, "xa", %progbits

	.arm

PreStackInit:

    @*************************************************************************
    @@
    @@     Change SP back to Internal RAM. EMC is not yet initialized

    ldr     sp,=0x40004000

    bl      BootstrapCode_Clocks

    bl      BootstrapCode_Pins


    .ifnc TargetLocation, "RAM"

    bl      BootstrapCode_Ext_SRAM

    .endif

    bl      BootstrapCode_Ext_Flash

    @*************************************************************************
    @ DO NOT CHANGE THE FOLLOWING CODE! we can not use pop to return because we 
    @ loaded the PC register to get here (since the stack has not been initialized).  
    @ Make sure the PreStackInit_Exit_Pointer is within range and 
    @ in the SectionForBootstrapOperations
    @ go back to the firstentry(_loader) code 
    @

PreStackEnd:
    b    PreStackInit_Exit_Pointer

    @
    @**************************************************************************

	.ifdef COMPILE_THUMB
	.thumb
	.endif

    .end


