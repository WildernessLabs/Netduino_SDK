/*******************************************************************************
*   DISCLAIMER
*
*   This software is supplied by Renesas Electronics Corp. and is only 
*   intended for use with Renesas products. No other uses are authorized.
*
*   This software is owned by Renesas Electronics Corp. and is protected under 
*   all applicable laws, including copyright laws.
*
*   THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
*   REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
*   INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
*   PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
*   DISCLAIMED.
*
*   TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
*   TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
*   FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
*   FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
*   AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
*   Renesas reserves the right, without notice, to make changes to this 
*   software and to discontinue the availability of this software.
*   By using this software, you agree to the additional terms and 
*   conditions found by accessing the following link: 
*   http://www.renesas.com/disclaimer
********************************************************************************
*   Copyright (C) 2010 Renesas Electronics America Inc. All Rights Reserved.
*""FILE COMMENT""*********** Technical reference data **************************
*   System Name : SH7216 Sample Program
*   File Name   : vecttbl.c
*   Abstract    : Initialization for Vector Table
*   Version     : 0.01.00
*   Device      : SH7216
*   Tool-Chain  : High-performance Embedded Workshop (Ver.4.05.01).
*               : C/C++ compiler package for the SuperH RISC engine family
*               :                             (Ver.9.03 Release00).
*   OS          : None
*   H/W Platform: R0K572167 (CPU board)
*   Description : 
********************************************************************************
*   History     : Mar.30,2009 Ver.0.01.00  
*""FILE COMMENT END""**********************************************************/
#include "vect.h"

#pragma section VECTTBL
void *RESET_Vectors[] = {
//;<<VECTOR DATA START (POWER ON RESET)>>
// 0 Power On Reset PC
    (void*) PowerON_Reset_PC,
//;<<VECTOR DATA END (POWER ON RESET)>>
// 1 Power On Reset SP
    __secend("S")
};

/* End of File */
