@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Copyright (c) Microsoft Corporation.  All rights reserved.
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.syntax unified

	.global IRQ_LOCK_Release_asm
	.global IRQ_LOCK_Probe_asm
	.global IRQ_LOCK_GetState_asm
	.global IRQ_LOCK_ForceDisabled_asm
	.global IRQ_LOCK_ForceEnabled_asm
	.global IRQ_LOCK_Disable_asm
	.global IRQ_LOCK_Restore_asm

    .section   SectionForFlashOperations,  "xa", %progbits

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

IRQ_LOCK_Release_asm:
    mrs     r0, xPSR
    bic     r1, r0, #0x80
    msr     xPSR, r1
    
    mov     pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

IRQ_LOCK_Probe_asm:
    mrs		r0, xPSR
    bic		r1, r0, #0x80
    msr		xPSR, r1
    msr		xPSR, r0

    mov     pc, lr

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

IRQ_LOCK_GetState_asm:
    @mrs		r0, xPSR
    @mvn		r0, r0
    @and		r0, r0, #0x80
    
    @mov     pc, lr
    
    MRS      r0,PRIMASK
    MRS      r1,IPSR
    EOR      r0,r0,#1
    CBZ      r1,L2
    MOV      r0,#0
L2:  BX       lr    
    
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  
IRQ_LOCK_ForceDisabled_asm: 
    @mrs		r0, xPSR
    @orr		r1, r0, #0x80
    @msr		xPSR, r1
    @mvn		r0, r0
    @and		r0, r0, #0x80
  
    @mov     pc, lr    
    
    MRS      r0,PRIMASK
    CPSID    i
    EOR      r0,r0,#1
    BX       lr    

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

IRQ_LOCK_ForceEnabled_asm:
    @mrs		r0, xPSR
    @bic		r1, r0, #0x80
    @msr		xPSR, r1
    @mvn		r0, r0
    @and		r0, r0, #0x80
  
    @mov     pc, lr   
    
    MRS      r0,PRIMASK
    CPSIE    i
    EOR      r0,r0,#1
    BX       lr    
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	    
IRQ_LOCK_Disable_asm:
    @mrs		r0, xPSR
    @orr		r1, r0, #0x80
    @msr		xPSR, r1

    @mov     pc, lr
    
    CPSID    i
    BX       lr    
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

IRQ_LOCK_Restore_asm:
    mrs	r0, xPSR
    bic	r0, r0, #0x80
    msr	xPSR, r0

    mov pc, lr   
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.end
	
	
	
	
