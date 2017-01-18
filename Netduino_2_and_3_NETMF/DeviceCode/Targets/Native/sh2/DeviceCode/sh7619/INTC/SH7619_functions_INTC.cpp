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
#include "..\iodefine.h"
//#include "..\Ethernet\SH7619_Ethernet.h"
#include "..\GPIO\SH7619_GPIO.h"

//--//

#pragma section INTTBL
extern void *INT_Vectors[] = {
// 4 Illegal code
    (void*) INT_Illegal_code,
// 5 Reserved
    (void*) Dummy,
// 6 Illegal slot
    (void*) INT_Illegal_slot,
// 7 Reserved
    (void*) Dummy,
// 8 Reserved
    (void*) Dummy,
// 9 CPU Address error
    (void*) INT_CPU_Address,
// 10 Reserved
    (void*) Dummy,
// 11 NMI
    (void*) INT_NMI,
// 12 User breakpoint trap
    (void*) INT_User_Break,
// 13 H-UDI
    (void*) INT_H_UDI,
// 14 Reserved
    (void*) Dummy,
// 15 Reserved
    (void*) Dummy,
// 16 Reserved
    (void*) Dummy,
// 17 Reserved
    (void*) Dummy,
// 18 Reserved
    (void*) Dummy,
// 19 Reserved
    (void*) Dummy,
// 20 Reserved
    (void*) Dummy,
// 21 Reserved
    (void*) Dummy,
// 22 Reserved
    (void*) Dummy,
// 23 Reserved
    (void*) Dummy,
// 24 Reserved
    (void*) Dummy,
// 25 Reserved
    (void*) Dummy,
// 26 Reserved
    (void*) Dummy,
// 27 Reserved
    (void*) Dummy,
// 28 Reserved
    (void*) Dummy,
// 29 Reserved
    (void*) Dummy,
// 30 Reserved
    (void*) Dummy,
// 31 Reserved
    (void*) Dummy,
// 32 TRAPA (User Vecter)
    (void*) INT_TRAPA32,
// 33 TRAPA (User Vecter)
    (void*) INT_TRAPA33,
// 34 TRAPA (User Vecter)
    (void*) INT_TRAPA34,
// 35 TRAPA (User Vecter)
    (void*) INT_TRAPA35,
// 36 TRAPA (User Vecter)
    (void*) INT_TRAPA36,
// 37 TRAPA (User Vecter)
    (void*) INT_TRAPA37,
// 38 TRAPA (User Vecter)
    (void*) INT_TRAPA38,
// 39 TRAPA (User Vecter)
    (void*) INT_TRAPA39,
// 40 TRAPA (User Vecter)
    (void*) INT_TRAPA40,
// 41 TRAPA (User Vecter)
    (void*) INT_TRAPA41,
// 42 TRAPA (User Vecter)
    (void*) INT_TRAPA42,
// 43 TRAPA (User Vecter)
    (void*) INT_TRAPA43,
// 44 TRAPA (User Vecter)
    (void*) INT_TRAPA44,
// 45 TRAPA (User Vecter)
    (void*) INT_TRAPA45,
// 46 TRAPA (User Vecter)
    (void*) INT_TRAPA46,
// 47 TRAPA (User Vecter)
    (void*) INT_TRAPA47,
// 48 TRAPA (User Vecter)
    (void*) INT_TRAPA48,
// 49 TRAPA (User Vecter)
    (void*) INT_TRAPA49,
// 50 TRAPA (User Vecter)
    (void*) INT_TRAPA50,
// 51 TRAPA (User Vecter)
    (void*) INT_TRAPA51,
// 52 TRAPA (User Vecter)
    (void*) INT_TRAPA52,
// 53 TRAPA (User Vecter)
    (void*) INT_TRAPA53,
// 54 TRAPA (User Vecter)
    (void*) INT_TRAPA54,
// 55 TRAPA (User Vecter)
    (void*) INT_TRAPA55,
// 56 TRAPA (User Vecter)
    (void*) INT_TRAPA56,
// 57 TRAPA (User Vecter)
    (void*) INT_TRAPA57,
// 58 TRAPA (User Vecter)
    (void*) INT_TRAPA58,
// 59 TRAPA (User Vecter)
    (void*) INT_TRAPA59,
// 60 TRAPA (User Vecter)
    (void*) INT_TRAPA60,
// 61 TRAPA (User Vecter)
    (void*) INT_TRAPA61,
// 62 TRAPA (User Vecter)
    (void*) INT_TRAPA62,
// 63 TRAPA (User Vecter)
    (void*) INT_TRAPA63,
// 64 Interrupt IRQ0
    (void*) INT_IRQ0,
// 65 Interrupt IRQ1
    (void*) INT_IRQ1,
// 66 Interrupt IRQ2
    (void*) INT_IRQ2,
// 67 Interrupt IRQ3
    (void*) INT_IRQ3,
// 68 Reserved
    (void*) Dummy,
// 69 Reserved
    (void*) Dummy,
// 70 Reserved
    (void*) Dummy,
// 71 Reserved
    (void*) Dummy,
// 72 Reserved
    (void*) Dummy,
// 73 Reserved
    (void*) Dummy,
// 74 Reserved
    (void*) Dummy,
// 75 Reserved
    (void*) Dummy,
// 76 Reserved
    (void*) Dummy,
// 77 Reserved
    (void*) Dummy,
// 78 Reserved
    (void*) Dummy,
// 79 Reserved
    (void*) Dummy,
// 80 Interrupt IRQ4
    (void*) INT_IRQ4,
// 81 Interrupt IRQ5
    (void*) INT_IRQ5,
// 82 Interrupt IRQ6
    (void*) INT_IRQ6,
// 83 Interrupt IRQ7
    (void*) INT_IRQ7,
// 84 WDT ITI
    (void*) INT_WDT_ITI,
// 85 EDMAC EINT0
    (void*) INT_EDMAC_EINT0,
// 86 CMT0 CMI_0
    (void*) INT_CMT0_CMI_0,
// 87 CMT1 CMI_1
    (void*) INT_CMT1_CMI_1,
// 88 SCIF_ERI_0
    (void*) INT_SCIF_ERI_0,
// 89 SCIF_RXI_0
    (void*) INT_SCIF_RXI_0,
// 90 SCIF_BRI_0
    (void*) INT_SCIF_BRI_0,
// 91 SCIF_TXI_0
    (void*) INT_SCIF_TXI_0,
// 92 SCIF_ERI_1
    (void*) INT_SCIF_ERI_1,
// 93 SCIF_RXI_1
    (void*) INT_SCIF_RXI_1,
// 94 SCIF_BRI_1
    (void*) INT_SCIF_BRI_1,
// 95 SCIF_TXI_1
    (void*) INT_SCIF_TXI_1,
// 96 SCIF_ERI_2
    (void*) INT_SCIF_ERI_2,
// 97 SCIF_RXI_2
    (void*) INT_SCIF_RXI_2,
// 98 SCIF_BRI_2
    (void*) INT_SCIF_BRI_2,
// 99 SCIF_TXI_2
    (void*) INT_SCIF_TXI_2,
// 100 HIF_HIFI
    (void*) INT_HIF_HIFI,
// 101 HIF_HIFBI
    (void*) INT_HIF_HIFBI,
// 102 Reserved
    (void*) Dummy,
// 103 Reserved
    (void*) Dummy,
// 104 DMAC DEI0
    (void*) INT_DMAC_DEI0,
// 105 DMAC DEI1
    (void*) INT_DMAC_DEI1,
// 106 DMAC DEI2
    (void*) INT_DMAC_DEI2,
// 107 DMAC DEI3
    (void*) INT_DMAC_DEI3,
// 108 SIOF SIOF1
    (void*) INT_SIOF_SIOF1,
// 109 Reserved
    (void*) Dummy,
// 110 Reserved
    (void*) Dummy,
// 111 Reserved
    (void*) Dummy,
// 112 Reserved
    (void*) Dummy,
// 113 Reserved
    (void*) Dummy,
// 114 Reserved
    (void*) Dummy,
// 115 Reserved
    (void*) Dummy,
// 116 Reserved
    (void*) Dummy,
// 117 Reserved
    (void*) Dummy,
// 118 Reserved
    (void*) Dummy,
// 119 Reserved
    (void*) Dummy,
// 120 Reserved
    (void*) Dummy,
// 121 Reserved
    (void*) Dummy,
// 122 Reserved
    (void*) Dummy,
// 123 Reserved
    (void*) Dummy,
// 124 Reserved
    (void*) Dummy,
// 125 Reserved
    (void*) Dummy,
// 126 Reserved
    (void*) Dummy,
// 127 Reserved
    (void*) Dummy,
// 128 Reserved
    (void*) Dummy,
// 129 Reserved
    (void*) Dummy,
// 130 Reserved
    (void*) Dummy,
// 131 Reserved
    (void*) Dummy,
// 132 Reserved
    (void*) Dummy,
// 133 Reserved
    (void*) Dummy,
// 134 Reserved
    (void*) Dummy,
// 135 Reserved
    (void*) Dummy,
// 136 Reserved
    (void*) Dummy,
// 137 Reserved
    (void*) Dummy,
// 138 Reserved
    (void*) Dummy,
// 139 Reserved
    (void*) Dummy,
// 140 Reserved
    (void*) Dummy,
// 141 Reserved
    (void*) Dummy,
// 142 Reserved
    (void*) Dummy,
// 143 Reserved
    (void*) Dummy,
// 144 Reserved
    (void*) Dummy,
// 145 Reserved
    (void*) Dummy,
// 146 Reserved
    (void*) Dummy,
// 147 Reserved
    (void*) Dummy,
// 148 Reserved
    (void*) Dummy,
// 149 Reserved
    (void*) Dummy,
// 150 Reserved
    (void*) Dummy,
// 151 Reserved
    (void*) Dummy,
// 152 Reserved
    (void*) Dummy,
// 153 Reserved
    (void*) Dummy,
// 154 Reserved
    (void*) Dummy,
// 155 Reserved
    (void*) Dummy,
// 156 Reserved
    (void*) Dummy,
// 157 Reserved
    (void*) Dummy,
// 158 Reserved
    (void*) Dummy,
// 159 Reserved
    (void*) Dummy,
// 160 Reserved
    (void*) Dummy,
// 161 Reserved
    (void*) Dummy,
// 162 Reserved
    (void*) Dummy,
// 163 Reserved
    (void*) Dummy,
// 164 Reserved
    (void*) Dummy,
// 165 Reserved
    (void*) Dummy,
// 166 Reserved
    (void*) Dummy,
// 167 Reserved
    (void*) Dummy,
// 168 Reserved
    (void*) Dummy,
// 169 Reserved
    (void*) Dummy,
// 170 Reserved
    (void*) Dummy,
// 171 Reserved
    (void*) Dummy,
// 172 Reserved
    (void*) Dummy,
// 173 Reserved
    (void*) Dummy,
// 174 Reserved
    (void*) Dummy,
// 175 Reserved
    (void*) Dummy,
// 176 Reserved
    (void*) Dummy,
// 177 Reserved
    (void*) Dummy,
// 178 Reserved
    (void*) Dummy,
// 179 Reserved
    (void*) Dummy,
// 180 Reserved
    (void*) Dummy,
// 181 Reserved
    (void*) Dummy,
// 182 Reserved
    (void*) Dummy,
// 183 Reserved
    (void*) Dummy,
// 184 Reserved
    (void*) Dummy,
// 185 Reserved
    (void*) Dummy,
// 186 Reserved
    (void*) Dummy,
// 187 Reserved
    (void*) Dummy,
// 188 Reserved
    (void*) Dummy,
// 189 Reserved
    (void*) Dummy,
// 190 Reserved
    (void*) Dummy,
// 191 Reserved
    (void*) Dummy,
// 192 Reserved
    (void*) Dummy,
// 193 Reserved
    (void*) Dummy,
// 194 Reserved
    (void*) Dummy,
// 195 Reserved
    (void*) Dummy,
// 196 Reserved
    (void*) Dummy,
// 197 Reserved
    (void*) Dummy,
// 198 Reserved
    (void*) Dummy,
// 199 Reserved
    (void*) Dummy,
// 200 Reserved
    (void*) Dummy,
// 201 Reserved
    (void*) Dummy,
// 202 Reserved
    (void*) Dummy,
// 203 Reserved
    (void*) Dummy,
// 204 Reserved
    (void*) Dummy,
// 205 Reserved
    (void*) Dummy,
// 206 Reserved
    (void*) Dummy,
// 207 Reserved
    (void*) Dummy,
// 208 Reserved
    (void*) Dummy,
// 209 Reserved
    (void*) Dummy,
// 210 Reserved
    (void*) Dummy,
// 211 Reserved
    (void*) Dummy,
// 212 Reserved
    (void*) Dummy,
// 213 Reserved
    (void*) Dummy,
// 214 Reserved
    (void*) Dummy,
// 215 Reserved
    (void*) Dummy,
// 216 Reserved
    (void*) Dummy,
// 217 Reserved
    (void*) Dummy,
// 218 Reserved
    (void*) Dummy,
// 219 Reserved
    (void*) Dummy,
// 220 Reserved
    (void*) Dummy,
// 221 Reserved
    (void*) Dummy,
// 222 Reserved
    (void*) Dummy,
// 223 Reserved
    (void*) Dummy,
// 224 Reserved
    (void*) Dummy,
// 225 Reserved
    (void*) Dummy,
// 226 Reserved
    (void*) Dummy,
// 227 Reserved
    (void*) Dummy,
// 228 Reserved
    (void*) Dummy,
// 229 Reserved
    (void*) Dummy,
// 230 Reserved
    (void*) Dummy,
// 231 Reserved
    (void*) Dummy,
// 232 Reserved
    (void*) Dummy,
// 233 Reserved
    (void*) Dummy,
// 234 Reserved
    (void*) Dummy,
// 235 Reserved
    (void*) Dummy,
// 236 Reserved
    (void*) Dummy,
// 237 Reserved
    (void*) Dummy,
// 238 Reserved
    (void*) Dummy,
// 239 Reserved
    (void*) Dummy,
// 240 Reserved
    (void*) Dummy,
// 241 Reserved
    (void*) Dummy,
// 242 Reserved
    (void*) Dummy,
// 243 Reserved
    (void*) Dummy,
// 244 Reserved
    (void*) Dummy,
// 245 Reserved
    (void*) Dummy,
// 246 Reserved
    (void*) Dummy,
// 247 Reserved
    (void*) Dummy,
// 248 Reserved
    (void*) Dummy,
// 249 Reserved
    (void*) Dummy,
// 250 Reserved
    (void*) Dummy,
// 251 Reserved
    (void*) Dummy,
// 252 Reserved
    (void*) Dummy,
// 253 Reserved
    (void*) Dummy,
// 254 Reserved
    (void*) Dummy,
// 255 Reserved
    (void*) Dummy
};

#pragma section

extern void SH7619_TIMER_ISR( UINT32 timer );
extern void USART_TxISR( UINT32 port );
extern void USART_RxISR( UINT32 port );

#if defined(SH7619_ENABLE_TCPIP)
extern void SH7619_EDMAC_interrupt( void *param );
#endif
    
#pragma section IntPRG
// 4 Illegal code
void INT_Illegal_code(void){/* sleep(); */}
// 5 Reserved

// 6 Illegal slot
void INT_Illegal_slot(void){/* sleep(); */}
// 7 Reserved

// 8 Reserved

// 9 CPU Address error
void INT_CPU_Address(void){/* sleep(); */}
// 10 Reserved

// 11 NMI
void INT_NMI(void){/* sleep(); */}
// 12 User breakpoint trap
void INT_User_Break(void){/* sleep(); */}
// 13 Reserved
void INT_H_UDI(void){/* sleep(); */}
// 14 Reserved

// 15 Reserved

// 16 Reserved

// 17 Reserved

// 18 Reserved

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
//void INT_TRAPA32(void){/* sleep(); */}
void INT_TRAPA32(void){ SH7619_GPIO_Driver::ISR(); }
// 33 TRAPA (User Vecter)
void INT_TRAPA33(void){/* sleep(); */}
// 34 TRAPA (User Vecter)
void INT_TRAPA34(void){/* sleep(); */}
// 35 TRAPA (User Vecter)
void INT_TRAPA35(void){/* sleep(); */}
// 36 TRAPA (User Vecter)
void INT_TRAPA36(void){/* sleep(); */}
// 37 TRAPA (User Vecter)
void INT_TRAPA37(void){/* sleep(); */}
// 38 TRAPA (User Vecter)
void INT_TRAPA38(void){/* sleep(); */}
// 39 TRAPA (User Vecter)
void INT_TRAPA39(void){/* sleep(); */}
// 40 TRAPA (User Vecter)
void INT_TRAPA40(void){/* sleep(); */}
// 41 TRAPA (User Vecter)
void INT_TRAPA41(void){/* sleep(); */}
// 42 TRAPA (User Vecter)
void INT_TRAPA42(void){/* sleep(); */}
// 43 TRAPA (User Vecter)
void INT_TRAPA43(void){/* sleep(); */}
// 44 TRAPA (User Vecter)
void INT_TRAPA44(void){/* sleep(); */}
// 45 TRAPA (User Vecter)
void INT_TRAPA45(void){/* sleep(); */}
// 46 TRAPA (User Vecter)
void INT_TRAPA46(void){/* sleep(); */}
// 47 TRAPA (User Vecter)
void INT_TRAPA47(void){/* sleep(); */}
// 48 TRAPA (User Vecter)
void INT_TRAPA48(void){/* sleep(); */}
// 49 TRAPA (User Vecter)
void INT_TRAPA49(void){/* sleep(); */}
// 50 TRAPA (User Vecter)
void INT_TRAPA50(void){/* sleep(); */}
// 51 TRAPA (User Vecter)
void INT_TRAPA51(void){/* sleep(); */}
// 52 TRAPA (User Vecter)
void INT_TRAPA52(void){/* sleep(); */}
// 53 TRAPA (User Vecter)
void INT_TRAPA53(void){/* sleep(); */}
// 54 TRAPA (User Vecter)
void INT_TRAPA54(void){/* sleep(); */}
// 55 TRAPA (User Vecter)
void INT_TRAPA55(void){/* sleep(); */}
// 56 TRAPA (User Vecter)
void INT_TRAPA56(void){/* sleep(); */}
// 57 TRAPA (User Vecter)
void INT_TRAPA57(void){/* sleep(); */}
// 58 TRAPA (User Vecter)
void INT_TRAPA58(void){/* sleep(); */}
// 59 TRAPA (User Vecter)
void INT_TRAPA59(void){/* sleep(); */}
// 60 TRAPA (User Vecter)
void INT_TRAPA60(void){/* sleep(); */}
// 61 TRAPA (User Vecter)
void INT_TRAPA61(void){/* sleep(); */}
// 62 TRAPA (User Vecter)
void INT_TRAPA62(void){/* sleep(); */}
// 63 TRAPA (User Vecter)
void INT_TRAPA63(void){/* sleep(); */}
// 64 Interrupt IRQ0
void INT_IRQ0(void){/* sleep(); */}
// 65 Interrupt IRQ1
void INT_IRQ1(void){/* sleep(); */}
// 66 Interrupt IRQ2
void INT_IRQ2(void){/* sleep(); */}
// 67 Interrupt IRQ3
void INT_IRQ3(void){/* sleep(); */}

// 68 Reserved

// 69 Reserved

// 70 Reserved

// 71 Reserved

// 72 Reserved

// 73 Reserved 

// 74 Reserved

// 75 Reserved

// 76 Reserved

// 77 Reserved

// 78 Reserved

// 79 Reserved

// 80 Interrupt IRQ4
    void INT_IRQ4(void){/* sleep(); */}
// 81 Interrupt IRQ5
    void INT_IRQ5(void){hal_printf( "INT_IRQ5\r\n" );/* sleep(); */}
// 82 Interrupt IRQ6
    void INT_IRQ6(void){/* sleep(); */}
// 83 Interrupt IRQ7
    void INT_IRQ7(void){/* sleep(); */}
// 84 WDT ITI
    void INT_WDT_ITI(void){/* sleep(); */}
// 85 EDMAC EINT0
    void INT_EDMAC_EINT0(void){ 
/*#if defined (_DEBUG)
        ECSR0   = 0x00000007;
        //EESR0   = 0x47FF0F9F;
        EESR0   = 0x47DF0F9F;// & ~0x00200000;
#else   
        ECSR0   = 0xFFFFFFFF;
        //EESR0   = 0xFFFFFFFF;
        EESR0   = 0xFFDFFFFF;// & ~0x00200000;
#endif*/
        //SH7619_Network_Driver::Ether_Rcv();
        //hal_printf("\r\nEDMAC interrupt\r\n");
#if defined(SH7619_ENABLE_TCPIP)
        SH7619_EDMAC_interrupt((void *)0);
#endif
}
// 86 CMT0_CMI0
    void INT_CMT0_CMI_0(void)
    {
        SH7619_TIMER_ISR(0);
    }
// 87 CMT1_CMI1
    void INT_CMT1_CMI_1(void)
    {
        SH7619_TIMER_ISR(1);
    }
// 88 SCIF_ERI_0
    void INT_SCIF_ERI_0(void){/* sleep(); */}
// 89 SCIF_RXI_0
    void INT_SCIF_RXI_0(void){
        USART_RxISR(0);
    }
// 90 SCIF_BRI_0
    void INT_SCIF_BRI_0(void){/* sleep(); */}
// 91 SCIF_TXI_0
    void INT_SCIF_TXI_0(void)
    {
        USART_TxISR(0);
    }       
    
// 92 SCIF_ERI_1
    void INT_SCIF_ERI_1(void){/* sleep(); */}
// 93 SCIF_RXI_1
    void INT_SCIF_RXI_1(void){
        USART_RxISR(1);
    }
// 94 SCIF_BRI_1
    void INT_SCIF_BRI_1(void){/* sleep(); */}
// 95 SCIF_TXI_1
    void INT_SCIF_TXI_1(void)
    {
        USART_TxISR(1);
    }       

// 96 SCIF_ERI_2
    void INT_SCIF_ERI_2(void){/* sleep(); */}
// 97 SCIF_RXI_2
    void INT_SCIF_RXI_2(void){
        USART_RxISR(2);
    }
// 98 SCIF_BRI_2
    void INT_SCIF_BRI_2(void){/* sleep(); */}
// 99 SCIF_TXI_2
    void INT_SCIF_TXI_2(void)
    {
        USART_TxISR(2);
    }       
// 100 HIF_HIFI
    void INT_HIF_HIFI(void){/* sleep(); */}
// 101 HIF_HIFBI
    void INT_HIF_HIFBI(void){/* sleep(); */}

// 102 Reserved

// 103 Reserved

// 104 DMAC DEI0
    void INT_DMAC_DEI0(void){/* sleep(); */}
// 105 DMAC DEI1
    void INT_DMAC_DEI1(void){/* sleep(); */}
// 106 DMAC DEI2
    void INT_DMAC_DEI2(void){/* sleep(); */}
// 107 DMAC DEI3
    void INT_DMAC_DEI3(void){/* sleep(); */}
// 108 SIOF SIOF1
    void INT_SIOF_SIOF1(void){/* sleep(); */}
// 109 Reserved

// 110 Reserved

// 111 Reserved

// 112 Reserved

// 113 Reserved

// 114 Reserved

// 115 Reserved

// 116 Reserved

// 117 Reserved

// 118 Reserved

// 119 Reserved

// 120 Reserved

// 121 Reserved

// 122 Reserved

// 123 Reserved

// 124 Reserved

// 125 Reserved

// 126 Reserved

// 127 Reserved

// 128 Reserved

// 129 Reserved

// 130 Reserved

// 131 Reserved

// 132 Reserved

// 133 Reserved

// 134 Reserved

// 135 Reserved

// 136 Reserved

// 137 Reserved

// 138 Reserved

// 139 Reserved

// 140 Reserved

// 141 Reserved

// 142 Reserved

// 143 Reserved

// 144 Reserved

// 145 Reserved

// 146 Reserved

// 147 Reserved

// 148 Reserved

// 149 Reserved

// 150 Reserved

// 151 Reserved

// 152 Reserved

// 153 Reserved

// 154 Reserved

// 155 Reserved

// 156 Reserved

// 157 Reserved

// 158 Reserved

// 159 Reserved

// 160 Reserved

// 161 Reserved

// 162 Reserved

// 163 Reserved

// 164 Reserved

// 165 Reserved

// 166 Reserved

// 167 Reserved

// 168 Reserved

// 169 Reserved

// 170 Reserved

// 171 Reserved

// 172 Reserved

// 173 Reserved

// 174 Reserved

// 175 Reserved

// 176 Reserved

// 177 Reserved

// 178 Reserved

// 179 Reserved

// 180 Reserved

// 181 Reserved

// 182 Reserved

// 183 Reserved

// 184 Reserved

// 185 Reserved

// 186 Reserved

// 187 Reserved

// 188 Reserved

// 189 Reserved

// 190 Reserved

// 191 Reserved

// 192 Reserved

// 193 Reserved

// 194 Reserved

// 195 Reserved

// 196 Reserved

// 197 Reserved

// 198 Reserved

// 199 Reserved

// 200 Reserved

// 201 Reserved

// 202 Reserved

// 203 Reserved

// 204 Reserved

// 205 Reserved

// 206 Reserved

// 207 Reserved

// 208 Reserved

// 209 Reserved

// 210 Reserved

// 211 Reserved

// 212 Reserved

// 213 Reserved

// 214 Reserved

// 215 Reserved

// 216 Reserved

// 217 Reserved

// 218 Reserved

// 219 Reserved

// 220 Reserved

// 221 Reserved

// 222 Reserved

// 223 Reserved

// 224 Reserved

// 225 Reserved

// 226 Reserved

// 227 Reserved

// 228 Reserved

// 229 Reserved

// 230 Reserved

// 231 Reserved

// 232 Reserved

// 233 Reserved

// 234 Reserved

// 235 Reserved

// 236 Reserved

// 237 Reserved

// 238 Reserved

// 239 Reserved

// 240 Reserved

// 241 Reserved

// 242 Reserved

// 243 Reserved

// 244 Reserved

// 245 Reserved

// 246 Reserved

// 247 Reserved

// 248 Reserved

// 249 Reserved

// 250 Reserved

// 251 Reserved

// 252 Reserved

// 253 Reserved

// 254 Reserved

// 255 Reserved

// Dummy
void Dummy(void){/* sleep(); */}

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
