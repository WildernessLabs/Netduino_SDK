////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framework Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

#include "vect.h"
#include "..\include\iodefine.h"
#include "..\BlockStorage\USB\USB_BL.h"
#include "..\GPIO\SH7264_GPIO.h"
#include "..\I2C\SH7264_I2C.h"

extern void SH7264_TIMER_ISR( UINT32 timer );
extern void USART_TxISR(UINT32 port   );
extern void USART_RxISR( UINT32 port  );
extern void SH7264_I2C_ISR(int intEvent);

//--//
#pragma section INTTBL
void *INT_Vectors[] = {
// 4 Illegal code
    INT_Illegal_code,
// 5 Reserved
    Dummy,
// 6 Illegal slot
    INT_Illegal_slot,
// 7 Reserved
    Dummy,
// 8 Reserved
    Dummy,
// 9 CPU Address error
    INT_CPU_Address,
// 10 BUS error
    INT_DMAC_Address,
// 11 NMI
    INT_NMI,
// 12 Reserved
//  INT_User_Break,
    Dummy,
// 13 INT_FPU
    INT_FPU,
// 14 H-UDI
    INT_HUDI,
// 15 Bank Overflow
    INT_Bank_Overflow,
// 16 Bank Underflow
    INT_Bank_Underflow,
// 17 Divide by zero
    INT_Divide_by_Zero,
// 18 Divide Overflow
    INT_Divide_Overflow,
// 19 Reserved
    Dummy,
// 20 Reserved
    Dummy,
// 21 Reserved
    Dummy,
// 22 Reserved
    Dummy,
// 23 Reserved
    Dummy,
// 24 Reserved
    Dummy,
// 25 Reserved
    Dummy,
// 26 Reserved
    Dummy,
// 27 Reserved
    Dummy,
// 28 Reserved
    Dummy,
// 29 Reserved
    Dummy,
// 30 Reserved
    Dummy,
// 31 Reserved
    Dummy,
// 32 TRAPA (User Vecter)
    INT_TRAPA32,
// 33 TRAPA (User Vecter)
    INT_TRAPA33,
// 34 TRAPA (User Vecter)
    INT_TRAPA34,
// 35 TRAPA (User Vecter)
    INT_TRAPA35,
// 36 TRAPA (User Vecter)
    INT_TRAPA36,
// 37 TRAPA (User Vecter)
    INT_TRAPA37,
// 38 TRAPA (User Vecter)
    INT_TRAPA38,
// 39 TRAPA (User Vecter)
    INT_TRAPA39,
// 40 TRAPA (User Vecter)
    INT_TRAPA40,
// 41 TRAPA (User Vecter)
    INT_TRAPA41,
// 42 TRAPA (User Vecter)
    INT_TRAPA42,
// 43 TRAPA (User Vecter)
    INT_TRAPA43,
// 44 TRAPA (User Vecter)
    INT_TRAPA44,
// 45 TRAPA (User Vecter)
    INT_TRAPA45,
// 46 TRAPA (User Vecter)
    INT_TRAPA46,
// 47 TRAPA (User Vecter)
    INT_TRAPA47,
// 48 TRAPA (User Vecter)
    INT_TRAPA48,
// 49 TRAPA (User Vecter)
    INT_TRAPA49,
// 50 TRAPA (User Vecter)
    INT_TRAPA50,
// 51 TRAPA (User Vecter)
    INT_TRAPA51,
// 52 TRAPA (User Vecter)
    INT_TRAPA52,
// 53 TRAPA (User Vecter)
    INT_TRAPA53,
// 54 TRAPA (User Vecter)
    INT_TRAPA54,
// 55 TRAPA (User Vecter)
    INT_TRAPA55,
// 56 TRAPA (User Vecter)
    INT_TRAPA56,
// 57 TRAPA (User Vecter)
    INT_TRAPA57,
// 58 TRAPA (User Vecter)
    INT_TRAPA58,
// 59 TRAPA (User Vecter)
    INT_TRAPA59,
// 60 TRAPA (User Vecter)
    INT_TRAPA60,
// 61 TRAPA (User Vecter)
    INT_TRAPA61,
// 62 TRAPA (User Vecter)
    INT_TRAPA62,
// 63 TRAPA (User Vecter)
    INT_TRAPA63,
// 64 Interrupt IRQ0
    INT_IRQ0,
// 65 Interrupt IRQ1
    INT_IRQ1,
// 66 Interrupt IRQ2
    INT_IRQ2,
// 67 Interrupt IRQ3
    INT_IRQ3,
// 68 Interrupt IRQ4
    INT_IRQ4,
// 69 Interrupt IRQ5
    INT_IRQ5,
// 70 Interrupt IRQ6
    INT_IRQ6,
// 71 Interrupt IRQ7
    INT_IRQ7,
// 72 Reserved
    Dummy,
// 73 Reserved
    Dummy,
// 74 Reserved
    Dummy,
// 75 Reserved
    Dummy,
// 76 Reserved
    Dummy,
// 77 Reserved
    Dummy,
// 78 Reserved
    Dummy,
// 79 Reserved
    Dummy,
// 80 Interrupt PINT0
    INT_PINT0,
// 81 Interrupt PINT1
    INT_PINT1,
// 82 Interrupt PINT2
    INT_PINT2,
// 83 Interrupt PINT3
    INT_PINT3,
// 84 Interrupt PINT4
    INT_PINT4,
// 85 Interrupt PINT5
    INT_PINT5,
// 86 Interrupt PINT6
    INT_PINT6,
// 87 Interrupt PINT7
    INT_PINT7,
// 88 Reserved
    Dummy,
// 89 Reserved
    Dummy,
// 90 Reserved
    Dummy,
// 91 Reserved
    Dummy,
// 92 Reserved
    Dummy,
// 93 Reserved
    Dummy,
// 94 Reserved
    Dummy,
// 95 Reserved
    Dummy,
// 96 Reserved
    Dummy,
// 97 Reserved
    Dummy,
// 98 Reserved
    Dummy,
// 99 Reserved
    Dummy,
// 100 Reserved
    Dummy,
// 101 Reserved
    Dummy,
// 102 Reserved
    Dummy,
// 103 Reserved
    Dummy,
// 104 Reserved
    Dummy,
// 105 Reserved
    Dummy,
// 106 Reserved
    Dummy,
// 107 Reserved
    Dummy,
// 108 DMAC0 TEI0
//  INT_DMAC0_TEI0,
//  _tx_DMAO_CH0_ISR,
    Dummy,
// 109 DMAC0 HEI0
    INT_DMAC0_HEI0,
// 110 Reserved
    Dummy,
// 111 Reserved
    Dummy,
// 112 DMAC1 TEI1
//  _tx_DMAO_CH1_ISR,
    Dummy,
// 113 DMAC1 HEI1
    INT_DMAC1_HEI1,
// 114 Reserved
    Dummy,
// 115 Reserved
    Dummy,
// 116 DMAC2 TEI2
    //_tx_DMAO_CH2_ISR,
    Dummy,
// 117 DMAC2 HEI2
    INT_DMAC2_HEI2,
// 118 Reserved
    Dummy,
// 119 Reserved
    Dummy,
// 120 DMAC3 TEI3
    INT_DMAC3_TEI3,
// 121 DMAC3 HEI3
    INT_DMAC3_HEI3,
// 122 Reserved
    Dummy,
// 123 Reserved
    Dummy,
// 124 DMAC4 TEI4
    INT_DMAC4_TEI4,
// 125 DMAC4 HEI4
    INT_DMAC4_HEI4,
// 126 Reserved
    Dummy,
// 127 Reserved
    Dummy,
// 128 DMAC5 TEI5
    INT_DMAC5_TEI5,
// 129 DMAC5 HEI5
    INT_DMAC5_HEI5,
// 130 Reserved
    Dummy,
// 131 Reserved
    Dummy,
// 132 DMAC6 TEI6
    INT_DMAC6_TEI6,
// 133 DMAC6 HEI6
    INT_DMAC6_HEI6,
// 134 Reserved
    Dummy,
// 135 Reserved
    Dummy,
// 136 DMAC7 TEI7
    INT_DMAC7_TEI7,
// 137 DMAC7 HEI7
    INT_DMAC7_HEI7,
// 138 Reserved
    Dummy,
// 139 Reserved
    Dummy,
// 140 DMAC8 TEI8
    INT_DMAC8_TEI8,
// 141 DMAC8 HEI8
    INT_DMAC8_HEI8,
// 142 Reserved
    Dummy,
// 143 Reserved
    Dummy,
// 144 DMAC9 TEI9
    INT_DMAC9_TEI9,
// 145 DMAC9 HEI9
    INT_DMAC9_HEI9,
// 146 Reserved
    Dummy,
// 147 Reserved
    Dummy,
// 148 DMAC10 TEI10
    INT_DMAC10_TEI10,
// 149 DMAC10 HEI10
    INT_DMAC10_HEI10,
// 150 Reserved
    Dummy,
// 151 Reserved
    Dummy,
// 152 DMAC11 TEI11
    INT_DMAC11_TEI11,
// 153 DMAC11 HEI11
    INT_DMAC11_HEI11,
// 154 Reserved
    Dummy,
// 155 Reserved
    Dummy,
// 156 DMAC12 TEI12
    INT_DMAC12_TEI12,
// 157 DMAC12 HEI12
    INT_DMAC12_HEI12,
// 158 Reserved
    Dummy,
// 159 Reserved
    Dummy,
// 160 DMAC13 TEI13
    INT_DMAC13_TEI13,
// 161 DMAC13 HEI13
    INT_DMAC13_HEI13,
// 162 Reserved
    Dummy,
// 163 Reserved
    Dummy,
// 164 DMAC14 TEI14
    INT_DMAC14_TEI14,
// 165 DMAC14 HEI14
    INT_DMAC14_HEI14,
// 166 Reserved
    Dummy,
// 167 Reserved
    Dummy,
// 168 DMAC15 TEI15
    INT_DMAC15_TEI15,
// 169 DMAC15 HEI15
    INT_DMAC15_HEI15,
// 170 USB USBI
    INT_USB_USBI,
// 171 VDC3 VIVSYNCJ
    INT_VDC3_VIVSYNCJ,
// 172 VDC3 VBUFERRJ
    INT_VDC3_VBUFERR,
// 173 VDC3 VIFIELDE
    INT_VDC3_VIFIELDE,
// 174 VDC3 VOLINE
    INT_VDC3_VOLINE,
// 175 CMT CMT0 CMI0
    INT_CMT0_CMI0,
// 176 CMT CMT1 CMI1
    INT_CMT1_CMI1,
// 177 BSC CMI
    INT_BSC_CMI,
// 178 WDT
    INT_WDT_ITI,
// 179 MTU2 MTU0 TGI0A
    INT_MTU2_TGI0A,
// 180 MTU2 MTU0 TGI0B
    INT_MTU2_TGI0B,
// 181 MTU2 MTU0 TGI0C
    INT_MTU2_TGI0C,
// 182 MTU2 MTU0 TGI0D
    INT_MTU2_TGI0D,
// 183 MTU2 MTU0 TCI0V
    INT_MTU2_TCI0V,
// 184 MTU2 MTU0 TGI0E
    INT_MTU2_TGI0E,
// 185 MTU2 MTU0 TGI0F
    INT_MTU2_TGI0F,
// 186 MTU2 MTU1 TGI1A
    INT_MTU2_TGI1A,
// 187 MTU2 MTU1 TGI1B
    INT_MTU2_TGI1B,
// 188 MTU2 MTU1 TCI1V
    INT_MTU2_TCI1V,
// 189 MTU2 MTU1 TCI1U
    INT_MTU2_TCI1U,
// 190 MTU2 MTU2 TGI2A
    INT_MTU2_TGI2A,
// 191 MTU2 MTU2 TGI2B
    INT_MTU2_TGI2B,
// 192 MTU2 MTU2 TCI2V
    INT_MTU2_TCI2V,
// 193 MTU2 MTU2 TCI2U
    INT_MTU2_TCI2U,
// 194 MTU2 MTU3 TGI3A
    INT_MTU2_TGI3A,
// 195 MTU2 MTU3 TGI3B
    INT_MTU2_TGI3B,
// 196 MTU2 MTU3 TGI3C
    INT_MTU2_TGI3C,
// 197 MTU2 MTU3 TGI3D
    INT_MTU2_TGI3D,
// 198 MTU2 MTU3 TCI3V
    INT_MTU2_TCI3V,
// 199 MTU2 MTU4 TGI4A
    INT_MTU2_TGI4A,
// 200 MTU2 MTU4 TGI4B
    INT_MTU2_TGI4B,
// 201 MTU2 MTU4 TGI4C
    INT_MTU2_TGI4C,
// 202 MTU2 MTU4 TGI4D
    INT_MTU2_TGI4D,
// 203 MTU2 MTU4 TCI4V
    INT_MTU2_TCI4V,
// 204 PWM PWM1 
    INT_PWM1,
// 205 PWM PWM2
    INT_PWM2,
// 206 A/D ADI
    INT_AD_ADI,
// 207 SSI0 SSIF0
    INT_SSI0_SSIF0,
// 208 SSI0 SSIRXI0
    INT_SSI0_SSIRXI0,
// 209 SSI0 SSITXI0
    INT_SSI0_SSITXI0,
// 210 SSI1 SSII1
    INT_SSI1_SSII1,
// 211 SSI1 SSIRTI1
    INT_SSI1_SSIRTI1,
// 212 SSI2 SSII2
    INT_SSI2_SSII2,
// 213 SSI2 SSIRTI2
    INT_SSI2_SSIRTI2,
// 214 SSI3 SSII3
    INT_SSI3_SSII3,
// 215 SSI3 SSIRTI3
    INT_SSI3_SSIRTI3,
// 216 SPDIF SPDIFI
    INT_SPDIF_SPDIFI,
// 217 IIC3 IIC0 STPI0
    INT_IIC3_STPI0,
// 218 IIC3 IIC0 NAKI0
    INT_IIC3_NAKI0,
// 219 IIC3 IIC0 RXI0
    INT_IIC3_RXI0,
// 220 IIC3 IIC0 TXI0
    INT_IIC3_TXI0,
// 221 IIC3 IIC0 TEI0
    INT_IIC3_TEI0,
// 222 IIC3 IIC1 STPI1
    INT_IIC3_STPI1,
// 223 IIC3 IIC1 NAKI1
    INT_IIC3_NAKI1,
// 224 IIC3 IIC1 RXI1
    INT_IIC3_RXI1,
// 225 IIC3 IIC1 TXI1
    INT_IIC3_TXI1,
// 226 IIC3 IIC1 TEI1
    INT_IIC3_TEI1,
// 227 IIC3 IIC2 STPI2
    INT_IIC3_STPI2,
// 228 IIC3 IIC2 NAKI2
    INT_IIC3_NAKI2,
// 229 IIC3 IIC2 RXI2
    INT_IIC3_RXI2,
// 230 IIC3 IIC2 TXI2
    INT_IIC3_TXI2,
// 231 IIC3 IIC2 TEI2
    INT_IIC3_TEI2,
// 232 SCIF SCIF0 BRI0
    INT_SCIF0_BRI0,
// 233 SCIF SCIF0 ERI0
    INT_SCIF0_ERI0,
// 234 SCIF SCIF0 RXI0
    INT_SCIF0_RXI0,
// 235 SCIF SCIF0 TXI0
    INT_SCIF0_TXI0,
// 236 SCIF SCIF1 BRI1
    INT_SCIF1_BRI1,
// 237 SCIF SCIF1 ERI1
    INT_SCIF1_ERI1,
// 238 SCIF SCIF1 RXI1
    INT_SCIF1_RXI1,
// 239 SCIF SCIF1 TXI1
    INT_SCIF1_TXI1,
// 240 SCIF SCIF2 BRI2
    INT_SCIF2_BRI2,
// 241 SCIF SCIF2 ERI2
    INT_SCIF2_ERI2,
// 242 SCIF SCIF2 RXI2
    INT_SCIF2_RXI2,
// 243 SCIF SCIF2 TXI2
    INT_SCIF2_TXI2,
// 244 SCIF SCIF3 BRI3
    INT_SCIF3_BRI3,
// 245 SCIF SCIF3 ERI3
    INT_SCIF3_ERI3,
// 246 SCIF SCIF3 RXI3
    INT_SCIF3_RXI3,
// 247 SCIF SCIF3 TXI3
    INT_SCIF3_TXI3,
// 248 SCIF SCIF4 BRI4
    INT_SCIF4_BRI4,
// 249 SCIF SCIF4 ERI4
    INT_SCIF4_ERI4,
// 250 SCIF SCIF4 RXI4
    INT_SCIF4_RXI4,
// 251 SCIF SCIF4 TXI4
    INT_SCIF4_TXI4,
// 252 SCIF SCIF5 BRI5
    INT_SCIF5_BRI5,
// 253 SCIF SCIF5 ERI5
    INT_SCIF5_ERI5,
// 254 SCIF SCIF5 RXI5
    INT_SCIF5_RXI5,
// 255 SCIF SCIF5 TXI5
    INT_SCIF5_TXI5,
// 256 SCIF SCIF6 BRI6
    INT_SCIF6_BRI6,
// 257 SCIF SCIF6 ERI6
    INT_SCIF6_ERI6,
// 258 SCIF SCIF6 RXI6
    INT_SCIF6_RXI6,
// 259 SCIF SCIF6 TXI6
    INT_SCIF6_TXI6,
// 260 SCIF SCIF7 BRI7
    INT_SCIF7_BRI7,
// 261 SCIF SCIF7 ERI7
    INT_SCIF7_ERI7,
// 262 SCIF SCIF7 RXI7
    INT_SCIF7_RXI7,
// 263 SCIF SCIF7 TXI7
    INT_SCIF7_TXI7,
// 264 SIOF SIOFI
    INT_SIOF_SIOFI,
// 265 RSPI RSPI0 SPEI0
    INT_RSPI0_SPEI0,
// 266 RSPI RSPI0 SPRI0
    INT_RSPI0_SPRI0,
// 267 RSPI RSPI0 SPTI0
    INT_RSPI0_SPTI0,
// 268 RSPI RSPI1 SPEI1
    INT_RSPI1_SPEI1,
// 269 RSPI RSPI1 SPRI1
    INT_RSPI1_SPRI1,
// 270 RSPI RSPI1 SPTI1
    INT_RSPI1_SPTI1,
// 271 RCAN RCAN0 ERS0 
    INT_RCAN0_ERS0,
// 272 RCAN RCAN0 OVR0 
    INT_RCAN0_OVR0,
// 273 RCAN RCAN0 RM00 
    INT_RCAN0_RM00,
// 274 RCAN RCAN0 RM10 
    INT_RCAN0_RM10,
// 275 RCAN RCAN0 SLE0 
    INT_RCAN0_SLE0,
// 276 RCAN RCAN1 ERS1 
    INT_RCAN1_ERS1,
// 277 RCAN RCAN1 OVR1 
    INT_RCAN1_OVR1,
// 278 RCAN RCAN1 RM01 
    INT_RCAN1_RM01,
// 279 RCAN RCAN1 RM11 
    INT_RCAN1_RM11,
// 280 RCAN RCAN1 SLE1 
    INT_RCAN1_SLE1,
// 281 IEB IEB 
    INT_IEB_IEB,
// 282 ROMDEC ISY 
    INT_ROMDEC_ISY,
// 283 ROMDEC IERR 
    INT_ROMDEC_IERR,
// 284 ROMDEC ITARG 
    INT_ROMDEC_ITARG,
// 285 ROMDEC ISEC 
    INT_ROMDEC_ISEC,
// 286 ROMDEC IBUF 
    INT_ROMDEC_IBUF,
// 287 ROMDEC IREADY 
    INT_ROMDEC_IREADY,
// 288 FLCTL FLSTEI 
    INT_FLCTL_FLSTEI,
// 289 FLCTL FLTENDI 
    INT_FLCTL_FLTENDI,
// 290 FLCTL FLTREQ0I 
    INT_FLCTL_FLTREQ0I,
// 291 FLCTL FLTREQ1I 
    INT_FLCTL_FLTREQ1I,
// 292 SDHI SDHI3 
    INT_SDHI_SDHI3,
//    _sdhi_insert_remove_ISR,
// 293 SDHI SDHI0 
    INT_SDHI_SDHI0,
//    _sdhi_response_ISR,
// 294 SDHI SDHI1 
    INT_SDHI_SDHI1,
//    _sdhi_unknown_ISR,
// 295 Reserved
    Dummy,
// 296 RTC ARM 
    INT_RTC_ARM,
// 297 RTC PRD 
    INT_RTC_PRD,
// 298 RTC CUP 
    INT_RTC_CUP,
// 299 SRC SRC0 OVF0 
    INT_SRC0_OVF0,
// 300 SRC SRC0 UDF0 
    INT_SRC0_UDF0,
// 301 SRC SRC0 CEF0 
    INT_SRC0_CEF0,
// 302 SRC SRC0 ODFI0 
    INT_SRC0_ODFI0,
// 303 SRC SRC0 IDEI0 
    INT_SRC0_IDEI0,
// 304 SRC SRC1 OVF1 
    INT_SRC1_OVF1,
// 305 SRC SRC1 UDF1 
    INT_SRC1_UDF1,
// 306 SRC SRC1 CEF1 
    INT_SRC1_CEF1,
// 307 SRC SRC1 ODFI1 
    INT_SRC1_ODFI1,
// 308 SRC SRC1 IDEI1 
    INT_SRC1_IDEI1,
// 309 Reserved
    Dummy,
// 310 DCU DCUEI 
    INT_DCU_DCUEI,
// 311 DCU OFFI 
    INT_DCU_OFFI,
// 312 DCU IFEI 
    INT_DCU_IFEI
};

#pragma section IntPRG
// 4 Illegal code
void INT_Illegal_code(void)
{
    /* sleep() */
}
// 5 Reserved

// 6 Illegal slot
void INT_Illegal_slot(void)
{
    /* sleep() */
}
// 7 Reserved

// 8 Reserved

// 9 CPU Address error
void INT_CPU_Address(void)
{
    /* sleep() */
}
// 10 BUS error
void INT_DMAC_Address(void)
{
    /* sleep() */
}
// 11 NMI
void INT_NMI(void)
{
    /* sleep() */
}
// 12 Reserved

// 13 INT_FPU
void INT_FPU(void)
{
    /* sleep() */
}
// 14 H-UDI
void INT_HUDI(void)
{
    /* sleep() */
}
// 15 Bank Overflow
void INT_Bank_Overflow(void)
{
    /* sleep() */
}
// 16 Bank Underflow
void INT_Bank_Underflow(void)
{
    /* sleep() */
}
// 17 Divide by zero
void INT_Divide_by_Zero(void)
{
    /* sleep() */
}
// 18 Divide Overflow
void INT_Divide_Overflow(void)
{
    /* sleep() */
}
// 19 Reserved

// 20 Reserved

// 21 Reserved

// 22 Reserved

// 23 Reserved

// 24 Reserved

// 25 Reserved

// 26 Reserved

// 27 Reserved

// 28 Reserved

// 29 Reserved

// 30 Reserved

// 31 Reserved

// 32 TRAPA (User Vecter)
void INT_TRAPA32(void)
{
    SH7264_GPIO_Driver::ISR();
}
// 33 TRAPA (User Vecter)
void INT_TRAPA33(void)
{
    /* sleep() */
}
// 34 TRAPA (User Vecter)
void INT_TRAPA34(void)
{
    /* sleep() */
}
// 35 TRAPA (User Vecter)
void INT_TRAPA35(void)
{
    /* sleep() */
}
// 36 TRAPA (User Vecter)
void INT_TRAPA36(void)
{
    /* sleep() */
}
// 37 TRAPA (User Vecter)
void INT_TRAPA37(void)
{
    /* sleep() */
}
// 38 TRAPA (User Vecter)
void INT_TRAPA38(void)
{
    /* sleep() */
}
// 39 TRAPA (User Vecter)
void INT_TRAPA39(void)
{
    /* sleep() */
}
// 40 TRAPA (User Vecter)
void INT_TRAPA40(void)
{
    /* sleep() */
}
// 41 TRAPA (User Vecter)
void INT_TRAPA41(void)
{
    /* sleep() */
}
// 42 TRAPA (User Vecter)
void INT_TRAPA42(void)
{
    /* sleep() */
}
// 43 TRAPA (User Vecter)
void INT_TRAPA43(void)
{
    /* sleep() */
}
// 44 TRAPA (User Vecter)
void INT_TRAPA44(void)
{
    /* sleep() */
}
// 45 TRAPA (User Vecter)
void INT_TRAPA45(void)
{
    /* sleep() */
}
// 46 TRAPA (User Vecter)
void INT_TRAPA46(void)
{
    /* sleep() */
}
// 47 TRAPA (User Vecter)
void INT_TRAPA47(void)
{
    /* sleep() */
}
// 48 TRAPA (User Vecter)
void INT_TRAPA48(void)
{
    /* sleep() */
}
// 49 TRAPA (User Vecter)
void INT_TRAPA49(void)
{
    /* sleep() */
}
// 50 TRAPA (User Vecter)
void INT_TRAPA50(void)
{
    /* sleep() */
}
// 51 TRAPA (User Vecter)
void INT_TRAPA51(void)
{
    /* sleep() */
}
// 52 TRAPA (User Vecter)
void INT_TRAPA52(void)
{
    /* sleep() */
}
// 53 TRAPA (User Vecter)
void INT_TRAPA53(void)
{
    /* sleep() */
}
// 54 TRAPA (User Vecter)
void INT_TRAPA54(void)
{
    /* sleep() */
}
// 55 TRAPA (User Vecter)
void INT_TRAPA55(void)
{
    /* sleep() */
}
// 56 TRAPA (User Vecter)
void INT_TRAPA56(void)
{
    /* sleep() */
}
// 57 TRAPA (User Vecter)
void INT_TRAPA57(void)
{
    /* sleep() */
}
// 58 TRAPA (User Vecter)
void INT_TRAPA58(void)
{
    /* sleep() */
}
// 59 TRAPA (User Vecter)
void INT_TRAPA59(void)
{
    /* sleep() */
}
// 60 TRAPA (User Vecter)
void INT_TRAPA60(void)
{
    /* sleep() */
}
// 61 TRAPA (User Vecter)
void INT_TRAPA61(void)
{
    /* sleep() */
}
// 62 TRAPA (User Vecter)
void INT_TRAPA62(void)
{
    /* sleep() */
}
// 63 TRAPA (User Vecter)
void INT_TRAPA63(void)
{
    /* sleep() */
}
// 64 Interrupt IRQ0
void INT_IRQ0(void)
{
#ifdef SH7264_TOUCH_PANEL
	extern void SH7264_TOUCH_ISR(void);
	SH7264_TOUCH_ISR();
#endif
    /* sleep() */
}
// 65 Interrupt IRQ1
void INT_IRQ1(void)
{
#ifdef SH7264_RSK_ETHERNET  
    extern void SH7264_SMSC_interrupt(void *param);
    SH7264_SMSC_interrupt(NULL);
#endif
    /* sleep() */
}
// 66 Interrupt IRQ2
void INT_IRQ2(void)
{
    /* sleep() */
}
// 67 Interrupt IRQ3
void INT_IRQ3(void)
{
    /* sleep() */
}
// 68 Interrupt IRQ4
void INT_IRQ4(void)
{
    /* sleep() */
}
// 69 Interrupt IRQ5
void INT_IRQ5(void)
{
    /* sleep() */
}
// 70 Interrupt IRQ6
void INT_IRQ6(void)
{
    /* sleep() */
}
// 71 Interrupt IRQ7
void INT_IRQ7(void)
{
    /* sleep() */
}
// 72 Reserved

// 73 Reserved

// 74 Reserved

// 75 Reserved

// 76 Reserved

// 77 Reserved

// 78 Reserved

// 79 Reserved

// 80 Interrupt PINT0
void INT_PINT0(void)
{
    /* sleep() */
}
// 81 Interrupt PINT1
void INT_PINT1(void)
{
    /* sleep() */
}
// 82 Interrupt PINT2
void INT_PINT2(void)
{
    /* sleep() */
}
// 83 Interrupt PINT3
void INT_PINT3(void)
{
    /* sleep() */
}
// 84 Interrupt PINT4
void INT_PINT4(void)
{
    /* sleep() */
}
// 85 Interrupt PINT5
void INT_PINT5(void)
{
    /* sleep() */
}
// 86 Interrupt PINT6
void INT_PINT6(void)
{
    /* sleep() */
}
// 87 Interrupt PINT7
void INT_PINT7(void)
{
    /* sleep() */
}
// 88 Reserved

// 89 Reserved

// 90 Reserved

// 91 Reserved

// 92 Reserved

// 93 Reserved

// 94 Reserved

// 95 Reserved

// 96 Reserved

// 97 Reserved

// 98 Reserved

// 99 Reserved

// 100 Reserved

// 101 Reserved

// 102 Reserved

// 103 Reserved

// 104 Reserved

// 105 Reserved

// 106 Reserved

// 107 Reserved

// 108 DMAC0 TEI0
//void INT_DMAC0_TEI0(void)
//void _tx_DMAO_CH0_ISR(void)
//{
//  /* sleep() */
//}
// 109 DMAC0 HEI0
void INT_DMAC0_HEI0(void)
{
    /* sleep() */
}
// 110 Reserved

// 111 Reserved

// 112 DMAC1 TEI1
//void INT_DMAC1_TEI1(void)
//void _tx_DMAO_CH1_ISR(void)
//{
    /* sleep() */
//}

// 113 DMAC1 HEI1
void INT_DMAC1_HEI1(void)
{
    /* sleep() */
}
// 114 Reserved

// 115 Reserved

// 116 DMAC2 TEI2
//void INT_DMAC2_TEI2(void)
//void _tx_DMAO_CH2_ISR(void)
//{
    /* sleep() */
//}
// 117 DMAC2 HEI2
void INT_DMAC2_HEI2(void)
{
    /* sleep() */
}
// 118 Reserved

// 119 Reserved

// 120 DMAC3 TEI3
void INT_DMAC3_TEI3(void)
//void _tx_DMAO_CH3_ISR(void)
{
    /* sleep() */
}

// 121 DMAC3 HEI3
void INT_DMAC3_HEI3(void)
{
    /* sleep() */
}
// 122 Reserved

// 123 Reserved

// 124 DMAC4 TEI4
void INT_DMAC4_TEI4(void)
{
    /* sleep() */
}
// 125 DMAC4 HEI4
void INT_DMAC4_HEI4(void)
{
    /* sleep() */
}
// 126 Reserved

// 127 Reserved

// 128 DMAC5 TEI5
void INT_DMAC5_TEI5(void)
{
    /* sleep() */
}
// 129 DMAC5 HEI5
void INT_DMAC5_HEI5(void)
{
    /* sleep() */
}
// 130 Reserved

// 131 Reserved

// 132 DMAC6 TEI6
void INT_DMAC6_TEI6(void)
{
    /* sleep() */
}
// 133 DMAC6 HEI6
void INT_DMAC6_HEI6(void)
{
    /* sleep() */
}
// 134 Reserved

// 135 Reserved

// 136 DMAC7 TEI7
void INT_DMAC7_TEI7(void)
{
    /* sleep() */
}
// 137 DMAC7 HEI7
void INT_DMAC7_HEI7(void)
{
    /* sleep() */
}
// 138 Reserved

// 139 Reserved

// 140 DMAC8 TEI8
void INT_DMAC8_TEI8(void)
{
    /* sleep() */
}
// 141 DMAC8 HEI8
void INT_DMAC8_HEI8(void)
{
    /* sleep() */
}
// 142 Reserved

// 143 Reserved

// 144 DMAC9 TEI9
void INT_DMAC9_TEI9(void)
{
    /* sleep() */
}
// 145 DMAC9 HEI9
void INT_DMAC9_HEI9(void)
{
    /* sleep() */
}
// 146 Reserved

// 147 Reserved

// 148 DMAC10 TEI10
void INT_DMAC10_TEI10(void)
{
    /* sleep() */
}
// 149 DMAC10 HEI10
void INT_DMAC10_HEI10(void)
{
    /* sleep() */
}
// 150 Reserved

// 151 Reserved

// 152 DMAC11 TEI11
void INT_DMAC11_TEI11(void)
{
    /* sleep() */
}
// 153 DMAC11 HEI11
void INT_DMAC11_HEI11(void)
{
    /* sleep() */
}
// 154 Reserved

// 155 Reserved

// 156 DMAC12 TEI12
void INT_DMAC12_TEI12(void)
{
    /* sleep() */
}
// 157 DMAC12 HEI12
void INT_DMAC12_HEI12(void)
{
    /* sleep() */
}
// 158 Reserved

// 159 Reserved

// 160 DMAC13 TEI13
void INT_DMAC13_TEI13(void)
{
    /* sleep() */
}
// 161 DMAC13 HEI13
void INT_DMAC13_HEI13(void)
{
    /* sleep() */
}
// 162 Reserved

// 163 Reserved

// 164 DMAC14 TEI14
void INT_DMAC14_TEI14(void)
{
    /* sleep() */
}
// 165 DMAC14 HEI14
void INT_DMAC14_HEI14(void)
{
    /* sleep() */
}
// 166 Reserved

// 167 Reserved

// 168 DMAC15 TEI15
void INT_DMAC15_TEI15(void)
{
    /* sleep() */
}
// 169 DMAC15 HEI15
void INT_DMAC15_HEI15(void)
{
    /* sleep() */
}
// 170 USB USBI
void INT_USB_USBI(void)
{
    USB_BS_Driver::CPU_USB_Global_ISR(NULL);
    /* sleep() */
}
// 171 VDC3 VIVSYNCJ
void INT_VDC3_VIVSYNCJ(void)
{
    /* sleep() */
}
// 172 VDC3 VBUFERRJ
void INT_VDC3_VBUFERR(void)
{
    /* sleep() */
}
// 173 VDC3 VIFIELDE
void INT_VDC3_VIFIELDE(void)
{
    /* sleep() */
}
// 174 VDC3 VOLINE
void INT_VDC3_VOLINE(void)
{
    /* sleep() */
}
// 175 CMT CMT0 CMI0
void INT_CMT0_CMI0(void)
{
    SH7264_TIMER_ISR(0);
    /* sleep() */
}
// 176 CMT CMT1 CMI1
void INT_CMT1_CMI1(void)
{
    SH7264_TIMER_ISR(1);
    /* sleep() */
}
// 177 BSC CMI
void INT_BSC_CMI(void)
{
    /* sleep() */
}
// 178 WDT
void INT_WDT_ITI(void)
{
    /* sleep() */
}
// 179 MTU2 MTU0 TGI0A
void INT_MTU2_TGI0A(void)
{
    /* sleep() */
}
// 180 MTU2 MTU0 TGI0B
void INT_MTU2_TGI0B(void)
{
    /* sleep() */
}
// 181 MTU2 MTU0 TGI0C
void INT_MTU2_TGI0C(void)
{
    /* sleep() */
}
// 182 MTU2 MTU0 TGI0D
void INT_MTU2_TGI0D(void)
{
    /* sleep() */
}
// 183 MTU2 MTU0 TCI0V
void INT_MTU2_TCI0V(void)
{
    /* sleep() */
}
// 184 MTU2 MTU0 TGI0E
void INT_MTU2_TGI0E(void)
{
    /* sleep() */
}
// 185 MTU2 MTU0 TGI0F
void INT_MTU2_TGI0F(void)
{
    /* sleep() */
}
// 186 MTU2 MTU1 TGI1A
void INT_MTU2_TGI1A(void)
{
    /* sleep() */
}
// 187 MTU2 MTU1 TGI1B
void INT_MTU2_TGI1B(void)
{
    /* sleep() */
}
// 188 MTU2 MTU1 TCI1V
void INT_MTU2_TCI1V(void)
{
    /* sleep() */
}
// 189 MTU2 MTU1 TCI1U
void INT_MTU2_TCI1U(void)
{
    /* sleep() */
}
// 190 MTU2 MTU2 TGI2A
void INT_MTU2_TGI2A(void)
{
    /* sleep() */
}
// 191 MTU2 MTU2 TGI2B
void INT_MTU2_TGI2B(void)
{
    /* sleep() */
}
// 192 MTU2 MTU2 TCI2V
void INT_MTU2_TCI2V(void)
{
    /* sleep() */
}
// 193 MTU2 MTU2 TCI2U
void INT_MTU2_TCI2U(void)
{
    /* sleep() */
}
// 194 MTU2 MTU3 TGI3A
void INT_MTU2_TGI3A(void)
{
    /* sleep() */
}
// 195 MTU2 MTU3 TGI3B
void INT_MTU2_TGI3B(void)
{
    /* sleep() */
}
// 196 MTU2 MTU3 TGI3C
void INT_MTU2_TGI3C(void)
{
    /* sleep() */
}
// 197 MTU2 MTU3 TGI3D
void INT_MTU2_TGI3D(void)
{
    /* sleep() */
}
// 198 MTU2 MTU3 TCI3V
void INT_MTU2_TCI3V(void)
{
    /* sleep() */
}
// 199 MTU2 MTU4 TGI4A
void INT_MTU2_TGI4A(void)
{
    /* sleep() */
}
// 200 MTU2 MTU4 TGI4B
void INT_MTU2_TGI4B(void)
{
    /* sleep() */
}
// 201 MTU2 MTU4 TGI4C
void INT_MTU2_TGI4C(void)
{
    /* sleep() */
}
// 202 MTU2 MTU4 TGI4D
void INT_MTU2_TGI4D(void)
{
    /* sleep() */
}
// 203 MTU2 MTU4 TCI4V
void INT_MTU2_TCI4V(void)
{
    /* sleep() */
}
// 204 PWM PWM1 
void INT_PWM1(void)
{
    /* sleep() */
}
// 205 PWM PWM2
void INT_PWM2(void)
{
    /* sleep() */
}
// 206 A/D ADI
void INT_AD_ADI(void)
{
    /* sleep() */
}
// 207 SSI0 SSIF0
void INT_SSI0_SSIF0(void)
{
    /* sleep() */
}
// 208 SSI0 SSIRXI0
void INT_SSI0_SSIRXI0(void)
{
    /* sleep() */
}
// 209 SSI0 SSITXI0
void INT_SSI0_SSITXI0(void)
{
    /* sleep() */
}
// 210 SSI1 SSII1
void INT_SSI1_SSII1(void)
{
    /* sleep() */
}
// 211 SSI1 SSIRTI1
void INT_SSI1_SSIRTI1(void)
{
    /* sleep() */
}
// 212 SSI2 SSII2
void INT_SSI2_SSII2(void)
{
    /* sleep() */
}
// 213 SSI2 SSIRTI2
void INT_SSI2_SSIRTI2(void)
{
    /* sleep() */
}
// 214 SSI3 SSII3
void INT_SSI3_SSII3(void)
{
    /* sleep() */
}
// 215 SSI3 SSIRTI3
void INT_SSI3_SSIRTI3(void)
{
    /* sleep() */
}
// 216 SPDIF SPDIFI
void INT_SPDIF_SPDIFI(void)
{
    /* sleep() */
}
// 217 IIC3 IIC0 STPI0
void INT_IIC3_STPI0(void)
{
    /* sleep() */
}
// 218 IIC3 IIC0 NAKI0
void INT_IIC3_NAKI0(void)
{
    /* sleep() */
}
// 219 IIC3 IIC0 RXI0
void INT_IIC3_RXI0(void)
{
    /* sleep() */
}
// 220 IIC3 IIC0 TXI0
void INT_IIC3_TXI0(void)
{
    /* sleep() */
}
// 221 IIC3 IIC0 TEI0
void INT_IIC3_TEI0(void)
{
    /* sleep() */
}
// 222 IIC3 IIC1 STPI1
void INT_IIC3_STPI1(void)
{
    /* sleep() */
}
// 223 IIC3 IIC1 NAKI1
void INT_IIC3_NAKI1(void)
{
    /* sleep() */
}
// 224 IIC3 IIC1 RXI1
void INT_IIC3_RXI1(void)
{
    /* sleep() */
}
// 225 IIC3 IIC1 TXI1
void INT_IIC3_TXI1(void)
{
    /* sleep() */
}
// 226 IIC3 IIC1 TEI1
void INT_IIC3_TEI1(void)
{
    /* sleep() */
}
// 227 IIC3 IIC2 STPI2
void INT_IIC3_STPI2(void)
{
    /* sleep() */
}
// 228 IIC3 IIC2 NAKI2
void INT_IIC3_NAKI2(void)
{
    /* sleep() */
}
// 229 IIC3 IIC2 RXI2
void INT_IIC3_RXI2(void)
{
    /* sleep() */
}
// 230 IIC3 IIC2 TXI2
void INT_IIC3_TXI2(void)
{
    /* sleep() */
}
// 231 IIC3 IIC2 TEI2
void INT_IIC3_TEI2(void)
{
    /* sleep() */
}
// 232 SCIF SCIF0 BRI0
void INT_SCIF0_BRI0(void)
{
    /* sleep() */
}
// 233 SCIF SCIF0 ERI0
void INT_SCIF0_ERI0(void)
{
    /* sleep() */
}
// 234 SCIF SCIF0 RXI0
void INT_SCIF0_RXI0(void)
{
    USART_RxISR(0);
}
// 235 SCIF SCIF0 TXI0
void INT_SCIF0_TXI0(void)
{
        USART_TxISR(0);
}
// 236 SCIF SCIF1 BRI1
void INT_SCIF1_BRI1(void)
{
    /* sleep() */
}
// 237 SCIF SCIF1 ERI1
void INT_SCIF1_ERI1(void)
{
    /* sleep() */
}
// 238 SCIF SCIF1 RXI1
void INT_SCIF1_RXI1(void)
{
    USART_RxISR(1);
}
// 239 SCIF SCIF1 TXI1
void INT_SCIF1_TXI1(void)
{
        USART_TxISR(1);
}
// 240 SCIF SCIF2 BRI2
void INT_SCIF2_BRI2(void)
{
    /* sleep() */
}
// 241 SCIF SCIF2 ERI2
void INT_SCIF2_ERI2(void)
{
    /* sleep() */
}
// 242 SCIF SCIF2 RXI2
void INT_SCIF2_RXI2(void)
{
    USART_RxISR(2);
}
// 243 SCIF SCIF2 TXI2
void INT_SCIF2_TXI2(void)
{
        USART_TxISR(2);
}
// 244 SCIF SCIF3 BRI3
void INT_SCIF3_BRI3(void)
{
    /* sleep() */
}
// 245 SCIF SCIF3 ERI3
void INT_SCIF3_ERI3(void)
{
    /* sleep() */
}
// 246 SCIF SCIF3 RXI3
void INT_SCIF3_RXI3(void)
{
    USART_RxISR(3);
}
// 247 SCIF SCIF3 TXI3
void INT_SCIF3_TXI3(void)
{
        USART_TxISR(3);
}
// 248 SCIF SCIF4 BRI4
void INT_SCIF4_BRI4(void)
{
    /* sleep() */
}
// 249 SCIF SCIF4 ERI4
void INT_SCIF4_ERI4(void)
{
    /* sleep() */
}
// 250 SCIF SCIF4 RXI4
void INT_SCIF4_RXI4(void)
{
    /* sleep() */
}
// 251 SCIF SCIF4 TXI4
void INT_SCIF4_TXI4(void)
{
    /* sleep() */
}
// 252 SCIF SCIF5 BRI5
void INT_SCIF5_BRI5(void)
{
    /* sleep() */
}
// 253 SCIF SCIF5 ERI5
void INT_SCIF5_ERI5(void)
{
    /* sleep() */
}
// 254 SCIF SCIF5 RXI5
void INT_SCIF5_RXI5(void)
{
    /* sleep() */
}
// 255 SCIF SCIF5 TXI5
void INT_SCIF5_TXI5(void)
{
    /* sleep() */
}
// 256 SCIF SCIF6 BRI6
void INT_SCIF6_BRI6(void)
{
    /* sleep() */
}
// 257 SCIF SCIF6 ERI6
void INT_SCIF6_ERI6(void)
{
    /* sleep() */
}
// 258 SCIF SCIF6 RXI6
void INT_SCIF6_RXI6(void)
{
    /* sleep() */
}
// 259 SCIF SCIF6 TXI6
void INT_SCIF6_TXI6(void)
{
    /* sleep() */
}
// 260 SCIF SCIF7 BRI7
void INT_SCIF7_BRI7(void)
{
    /* sleep() */
}
// 261 SCIF SCIF7 ERI7
void INT_SCIF7_ERI7(void)
{
    /* sleep() */
}
// 262 SCIF SCIF7 RXI7
void INT_SCIF7_RXI7(void)
{
    /* sleep() */
}
// 263 SCIF SCIF7 TXI7
void INT_SCIF7_TXI7(void)
{
    /* sleep() */
}
// 264 SIOF SIOFI
void INT_SIOF_SIOFI(void)
{
    /* sleep() */
}
// 265 RSPI RSPI0 SPEI0
void INT_RSPI0_SPEI0(void)
{
    /* sleep() */
}
// 266 RSPI RSPI0 SPRI0
void INT_RSPI0_SPRI0(void)
{
    /* sleep() */
}
// 267 RSPI RSPI0 SPTI0
void INT_RSPI0_SPTI0(void)
{
    /* sleep() */
}
// 268 RSPI RSPI1 SPEI1
void INT_RSPI1_SPEI1(void)
{
    /* sleep() */
}
// 269 RSPI RSPI1 SPRI1
void INT_RSPI1_SPRI1(void)
{
    /* sleep() */
}
// 270 RSPI RSPI1 SPTI1
void INT_RSPI1_SPTI1(void)
{
    /* sleep() */
}
// 271 RCAN RCAN0 ERS0 
void INT_RCAN0_ERS0(void)
{
    /* sleep() */
}
// 272 RCAN RCAN0 OVR0 
void INT_RCAN0_OVR0(void)
{
    /* sleep() */
}
// 273 RCAN RCAN0 RM00 
void INT_RCAN0_RM00(void)
{
    /* sleep() */
}
// 274 RCAN RCAN0 RM10 
void INT_RCAN0_RM10(void)
{
    /* sleep() */
}
// 275 RCAN RCAN0 SLE0 
void INT_RCAN0_SLE0(void)
{
    /* sleep() */
}
// 276 RCAN RCAN1 ERS1 
void INT_RCAN1_ERS1(void)
{
    /* sleep() */
}
// 277 RCAN RCAN1 OVR1 
void INT_RCAN1_OVR1(void)
{
    /* sleep() */
}
// 278 RCAN RCAN1 RM01 
void INT_RCAN1_RM01(void)
{
    /* sleep() */
}
// 279 RCAN RCAN1 RM11 
void INT_RCAN1_RM11(void)
{
    /* sleep() */
}
// 280 RCAN RCAN1 SLE1 
void INT_RCAN1_SLE1(void)
{
    /* sleep() */
}
// 281 IEB IEB 
void INT_IEB_IEB(void)
{
    /* sleep() */
}
// 282 ROMDEC ISY 
void INT_ROMDEC_ISY(void)
{
    /* sleep() */
}
// 283 ROMDEC IERR 
void INT_ROMDEC_IERR(void)
{
    /* sleep() */
}
// 284 ROMDEC ITARG 
void INT_ROMDEC_ITARG(void)
{
    /* sleep() */
}
// 285 ROMDEC ISEC 
void INT_ROMDEC_ISEC(void)
{
    /* sleep() */
}
// 286 ROMDEC IBUF 
void INT_ROMDEC_IBUF(void)
{
    /* sleep() */
}
// 287 ROMDEC IREADY 
void INT_ROMDEC_IREADY(void)
{
    /* sleep() */
}
// 288 FLCTL FLSTEI 
void INT_FLCTL_FLSTEI(void)
{
    /* sleep() */
}
// 289 FLCTL FLTENDI 
void INT_FLCTL_FLTENDI(void)
{
    /* sleep() */
}
// 290 FLCTL FLTREQ0I 
void INT_FLCTL_FLTREQ0I(void)
{
    /* sleep() */
}
// 291 FLCTL FLTREQ1I 
void INT_FLCTL_FLTREQ1I(void)
{
    /* sleep() */
}
// 292 SDHI SDHI3 
//void _sdhi_insert_remove_ISR(void)
void INT_SDHI_SDHI3(void)
{
    /* sleep() */
}
// 293 SDHI SDHI0 
//void _sdhi_response_ISR(void)
void INT_SDHI_SDHI0(void)
{
    /* sleep() */
}
// 294 SDHI SDHI1 
//void sdhi_unknown_ISR(void)
//{
//}
void INT_SDHI_SDHI1(void)
{
    /* sleep() */
}

// 295 Reserved

// 296 RTC ARM 
void INT_RTC_ARM(void)
{
    /* sleep() */
}
// 297 RTC PRD 
void INT_RTC_PRD(void)
{
    /* sleep() */
}
// 298 RTC CUP 
void INT_RTC_CUP(void)
{
    /* sleep() */
}
// 299 SRC SRC0 OVF0 
void INT_SRC0_OVF0(void)
{
    /* sleep() */
}
// 300 SRC SRC0 UDF0 
void INT_SRC0_UDF0(void)
{
    /* sleep() */
}
// 301 SRC SRC0 CEF0 
void INT_SRC0_CEF0(void)
{
    /* sleep() */
}
// 302 SRC SRC0 ODFI0 
void INT_SRC0_ODFI0(void)
{
    /* sleep() */
}
// 303 SRC SRC0 IDEI0 
void INT_SRC0_IDEI0(void)
{
    /* sleep() */
}
// 304 SRC SRC1 OVF1 
void INT_SRC1_OVF1(void)
{
    /* sleep() */
}
// 305 SRC SRC1 UDF1 
void INT_SRC1_UDF1(void)
{
    /* sleep() */
}
// 306 SRC SRC1 CEF1 
void INT_SRC1_CEF1(void)
{
    /* sleep() */
}
// 307 SRC SRC1 ODFI1 
void INT_SRC1_ODFI1(void)
{
    /* sleep() */
}
// 308 SRC SRC1 IDEI1 
void INT_SRC1_IDEI1(void)
{
    /* sleep() */
}
// 309 Reserved

// 310 DCU DCUEI 
void INT_DCU_DCUEI(void)
{
    /* sleep() */
}
// 311 DCU OFFI 
void INT_DCU_OFFI(void)
{
    /* sleep() */
}
// 312 DCU IFEI 
void INT_DCU_IFEI(void)
{
    /* sleep() */
}

void Dummy(void)
{
    /* sleep(); */
}

void __irq IRQ_Handler()
{
}

void CPU_INTC_Initialize()
{
}

BOOL CPU_INTC_ActivateInterrupt( UINT32 Irq_Index, HAL_CALLBACK_FPN ISR, void* ISR_Param )
{
    // figure out the interrupt
    HAL_CALLBACK* IsrVector;
    
    IsrVector = (HAL_CALLBACK *)INT_Vectors[Irq_Index];

    if(!IsrVector) 
        return FALSE;
        
    {
        // set the vector
        IsrVector->Initialize( ISR, ISR_Param ); 
    }

    return TRUE;
}

BOOL CPU_INTC_DeactivateInterrupt( UINT32 Irq_Index )
{
    return FALSE;
}

BOOL CPU_INTC_InterruptEnable( UINT32 Irq_Index )
{
    return FALSE;
}

BOOL CPU_INTC_InterruptDisable( UINT32 Irq_Index )
{
    return FALSE;
}

BOOL CPU_INTC_InterruptEnableState( UINT32 Irq_Index )
{
    return FALSE;
}

BOOL CPU_INTC_InterruptState( UINT32 Irq_Index )
{
    return FALSE;
}