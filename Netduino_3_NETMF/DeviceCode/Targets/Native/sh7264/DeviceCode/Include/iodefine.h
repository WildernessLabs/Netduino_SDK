/*""FILE COMMENT""*************************************************************
*
*       System Name : SH7264 
*       File Name   : iodefine.h
*       Version     : 0.06.00
*       Contents    : SH7264 IO define file 
*       Model       : M3A-HS64
*       CPU         : SH7264
*       Compiler    : SHC9.2.0.0
*       OS          : none
*
*       note        :
*                     <注意事項>
*                     本サンプルプログラムはすべて参考資料であり、
*                     その動作を保証するものではありません。
*                     本サンプルプログラムはお客様のソフトウエア開発時の
*                     技術参考資料としてご利用ください。
*
*       The information described here may contain technical inaccuracies or
*       typographical errors. Renesas Technology Corporation and Renesas Solutions
*       assume no responsibility for any damage, liability, or other loss rising 
*       from these inaccuracies or errors.
*
*       Copyright (C) 2008 Renesas Technology Corp. All Rights Reserved
*       AND Renesas Solutions Corp. All Rights Reserved
*
*       history   : 2008.06.02 ver.0.00.00 
*                 : 2008.06.17 ver.0.01.00 
*                 : 2008.06.18 ver.0.02.00 
*                 : 2008.06.19 ver.0.03.00 
*                 : 2008.08.04 ver.0.04.00 
*                 : 2008.10.01 ver.0.05.00  DMAC bitfield Modified
*                 : 2008.10.02 ver.0.06.00  DMAC bitfield Modified
*""FILE COMMENT END""*********************************************************/
#ifndef _IODEFINE_H_
#define _IODEFINE_H_

struct st_cpg {                                 /* struct CPG   */
       union {                                  /* FRQCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 CKOSEL:1;            /*   CKOSEL     */
                    UINT16 :1;                  /*              */
                    UINT16 CKOEN:2;             /*   CKOEN      */
                    UINT16 :6;                  /*              */
                    UINT16 IFC:1;               /*   IFC        */
                    UINT16 :2;                  /*              */
                    UINT16 PFC:3;               /*   PFC        */
                    } BIT;                      /*              */
             } FRQCR;                           /*              */
       UINT8 wk0[2];                           /*              */
       union {                                  /* STBCR1       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 STBY:1;              /*   STBY       */
                    UINT8 DEEP:1;              /*   DEEP       */
                    UINT8 :6;                  /*              */
                    } BIT;                      /*              */
             } STBCR1;                          /*              */
       UINT8 wk1[3];                           /*              */
       union {                                  /* STBCR2       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 MSTP10:1;            /*   MSTP10     */
                    UINT8 :1;                  /*              */
                    UINT8 MSTP8:1;             /*   MSTP8      */
                    UINT8 MSTP7:1;             /*   MSTP7      */
                    UINT8 :4;                  /*              */
                    } BIT;                      /*              */
             } STBCR2;                          /*              */
       UINT8 wk2[999];                         /*              */
       union {                                  /* SYSCR1       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 RAME3:1;             /*   RAME3      */
                    UINT8 RAME2:1;             /*   RAME2      */
                    UINT8 RAME1:1;             /*   RAME1      */
                    UINT8 RAME0:1;             /*   RAME0      */
                    } BIT;                      /*              */
             } SYSCR1;                          /*              */
       UINT8 wk3[3];                           /*              */
       union {                                  /* SYSCR2       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 RAMWE3:1;            /*   RAMWE3     */
                    UINT8 RAMWE2:1;            /*   RAMWE2     */
                    UINT8 RAMWE1:1;            /*   RAMWE1     */
                    UINT8 RAMWE0:1;            /*   RAMWE0     */
                    } BIT;                      /*              */
             } SYSCR2;                          /*              */
       UINT8 wk4[3];                           /*              */
       union {                                  /* STBCR3       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 HIZ:1;               /*   HIZ        */
                    UINT8 MSTP36:1;            /*   MSTP36     */
                    UINT8 MSTP35:1;            /*   MSTP35     */
                    UINT8 MSTP34:1;            /*   MSTP34     */
                    UINT8 MSTP33:1;            /*   MSTP33     */
                    UINT8 MSTP32:1;            /*   MSTP32     */
                    UINT8 :1;                  /*              */
                    UINT8 MSTP30:1;            /*   MSTP30     */
                    } BIT;                      /*              */
             } STBCR3;                          /*              */
       UINT8 wk5[3];                           /*              */
       union {                                  /* STBCR4       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 MSTP47:1;            /*   MSTP47     */
                    UINT8 MSTP46:1;            /*   MSTP46     */
                    UINT8 MSTP45:1;            /*   MSTP45     */
                    UINT8 MSTP44:1;            /*   MSTP44     */
                    UINT8 MSTP43:1;            /*   MSTP43     */
                    UINT8 MSTP42:1;            /*   MSTP42     */
                    UINT8 MSTP41:1;            /*   MSTP41     */
                    UINT8 MSTP40:1;            /*   MSTP40     */
                    } BIT;                      /*              */
             } STBCR4;                          /*              */
       UINT8 wk6[3];                           /*              */
       union {                                  /* STBCR5       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 MSTP57:1;            /*   MSTP57     */
                    UINT8 MSTP56:1;            /*   MSTP56     */
                    UINT8 MSTP55:1;            /*   MSTP55     */
                    UINT8 :1;                  /*              */
                    UINT8 MSTP53:1;            /*   MSTP53     */
                    UINT8 MSTP52:1;            /*   MSTP52     */
                    UINT8 MSTP51:1;            /*   MSTP51     */
                    UINT8 MSTP50:1;            /*   MSTP50     */
                    } BIT;                      /*              */
             } STBCR5;                          /*              */
       UINT8 wk7[3];                           /*              */
       union {                                  /* STBCR6       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 MSTP67:1;            /*   MSTP67     */
                    UINT8 MSTP66:1;            /*   MSTP66     */
                    UINT8 MSTP65:1;            /*   MSTP65     */
                    UINT8 MSTP64:1;            /*   MSTP64     */
                    UINT8 MSTP63:1;            /*   MSTP63     */
                    UINT8 MSTP62:1;            /*   MSTP62     */
                    UINT8 MSTP61:1;            /*   MSTP61     */
                    UINT8 MSTP60:1;            /*   MSTP60     */
                    } BIT;                      /*              */
             } STBCR6;                          /*              */
       UINT8 wk8[3];                           /*              */
       union {                                  /* STBCR7       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 MSTP77:1;            /*   MSTP77     */
                    UINT8 MSTP76:1;            /*   MSTP76     */
                    UINT8 :1;                  /*              */
                    UINT8 MSTP74:1;            /*   MSTP74     */
                    UINT8 :1;                  /*              */
                    UINT8 MSTP72:1;            /*   MSTP72     */
                    UINT8 :1;                  /*              */
                    UINT8 MSTP70:1;            /*   MSTP70     */
                    } BIT;                      /*              */
             } STBCR7;                          /*              */
       UINT8 wk9[3];                           /*              */
       union {                                  /* STBCR8       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 MSTP87:1;            /*   MSTP87     */
                    UINT8 :6;                  /*              */
                    UINT8 MSTP80:1;            /*   MSTP80     */
                    } BIT;                      /*              */
             } STBCR8;                          /*              */
       UINT8 wk10[3];                          /*              */
       union {                                  /* SYSCR3       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 VRAME5:1;            /*   VRAME5     */
                    UINT8 VRAME4:1;            /*   VRAME4     */
                    UINT8 VRAME3:1;            /*   VRAME3     */
                    UINT8 VRAME2:1;            /*   VRAME2     */
                    UINT8 VRAME1:1;            /*   VRAME1     */
                    UINT8 VRAME0:1;            /*   VRAME0     */
                    } BIT;                      /*              */
             } SYSCR3;                          /*              */
       UINT8 wk11[3];                          /*              */
       union {                                  /* SYSCR4       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 VRAMWE5:1;           /*   VRAMWE5    */
                    UINT8 VRAMWE4:1;           /*   VRAMWE4    */
                    UINT8 VRAMWE3:1;           /*   VRAMWE3    */
                    UINT8 VRAMWE2:1;           /*   VRAMWE2    */
                    UINT8 VRAMWE1:1;           /*   VRAMWE1    */
                    UINT8 VRAMWE0:1;           /*   VRAMWE0    */
                    } BIT;                      /*              */
             } SYSCR4;                          /*              */
       UINT8 wk12[11];                         /*              */
       union {                                  /* SWRSTCR      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 AXTALE:1;            /*   AXTALE     */
                    UINT8 :2;                  /*              */
                    UINT8 IEBSRST:1;           /*   IEBSRST    */
                    UINT8 SSIF3SRST:1;         /*   SSIF3SRST  */
                    UINT8 SSIF2SRST:1;         /*   SSIF2SRST  */
                    UINT8 SSIF1SRST:1;         /*   SSIF1SRST  */
                    UINT8 SSIF0SRST:1;         /*   SSIF0SRST  */
                    } BIT;                      /*              */
             } SWRSTCR;                         /*              */
       UINT8 wk13[25551];                      /*              */
       union {                                  /* RRAMKP       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 RRAMKP1:1;           /*   RRAMKP1    */
                    UINT8 RRAMKP0:1;           /*   RRAMKP0    */
                    } BIT;                      /*              */
             } RRAMKP;                          /*              */
       UINT8 wk14[1];                          /*              */
       union {                                  /* DSCTR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 EBUSKEEPE:1;         /*   EBUSKEEPE  */
                    UINT8 RAMBOOT:1;           /*   RAMBOOT    */
                    UINT8 :6;                  /*              */
                    } BIT;                      /*              */
             } DSCTR;                           /*              */
       UINT8 wk15[1];                          /*              */
       union {                                  /* DSSSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :9;                  /*              */
                    UINT16 RTCAR:1;             /*   RTCAR      */
                    UINT16 PC8:1;               /*   PC8        */
                    UINT16 PC7:1;               /*   PC7        */
                    UINT16 PC6:1;               /*   PC6        */
                    UINT16 PC5:1;               /*   PC5        */
                    UINT16 PJ3:1;               /*   PJ3        */
                    UINT16 PJ1:1;               /*   PJ1        */
                    } BIT;                      /*              */
             } DSSSR;                           /*              */
       union {                                  /* DSESR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :7;                  /*              */
                    UINT16 NMIE:1;              /*   NMIE       */
                    UINT16 :2;                  /*              */
                    UINT16 PC8E:1;              /*   PC8E       */
                    UINT16 PC7E:1;              /*   PC7E       */
                    UINT16 PC6E:1;              /*   PC6E       */
                    UINT16 PC5E:1;              /*   PC5E       */
                    UINT16 PJ3E:1;              /*   PJ3E       */
                    UINT16 PJ1E:1;              /*   PJ1E       */
                    } BIT;                      /*              */
             } DSESR;                           /*              */
       union {                                  /* DSFR         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 IOKEEP:1;            /*   IOKEEP     */
                    UINT16 :6;                  /*              */
                    UINT16 NMIF:1;              /*   NMIF       */
                    UINT16 :1;                  /*              */
                    UINT16 RTCARF:1;            /*   RTCARF     */
                    UINT16 PC8F:1;              /*   PC8F       */
                    UINT16 PC7F:1;              /*   PC7F       */
                    UINT16 PC6F:1;              /*   PC6F       */
                    UINT16 PC5F:1;              /*   PC5F       */
                    UINT16 PJ3F:1;              /*   PJ3F       */
                    UINT16 PJ1F:1;              /*   PJ1F       */
                    } BIT;                      /*              */
             } DSFR;                            /*              */
       UINT8 wk16[6];                          /*              */
       union {                                  /* XTALCTR      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 GAIN:1;              /*   GAIN       */
                    } BIT;                      /*              */
             } XTALCTR;                         /*              */
};                                              /*              */
struct st_intc {                                /* struct INTC  */
       union {                                  /* ICR0         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 NMIL:1;              /*   NMIL       */
                    UINT16 :6;                  /*              */
                    UINT16 NMIE:1;              /*   NMIE       */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } ICR0;                            /*              */
       union {                                  /* ICR1         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 IRQ71S:1;            /*   IRQ71S     */
                    UINT16 IRQ70S:1;            /*   IRQ70S     */
                    UINT16 IRQ61S:1;            /*   IRQ61S     */
                    UINT16 IRQ60S:1;            /*   IRQ60S     */
                    UINT16 IRQ51S:1;            /*   IRQ51S     */
                    UINT16 IRQ50S:1;            /*   IRQ50S     */
                    UINT16 IRQ41S:1;            /*   IRQ41S     */
                    UINT16 IRQ40S:1;            /*   IRQ40S     */
                    UINT16 IRQ31S:1;            /*   IRQ31S     */
                    UINT16 IRQ30S:1;            /*   IRQ30S     */
                    UINT16 IRQ21S:1;            /*   IRQ21S     */
                    UINT16 IRQ20S:1;            /*   IRQ20S     */
                    UINT16 IRQ11S:1;            /*   IRQ11S     */
                    UINT16 IRQ10S:1;            /*   IRQ10S     */
                    UINT16 IRQ01S:1;            /*   IRQ01S     */
                    UINT16 IRQ00S:1;            /*   IRQ00S     */
                    } BIT;                      /*              */
             } ICR1;                            /*              */
       union {                                  /* ICR2         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 PINT7S:1;            /*   PINT7S     */
                    UINT16 PINT6S:1;            /*   PINT6S     */
                    UINT16 PINT5S:1;            /*   PINT5S     */
                    UINT16 PINT4S:1;            /*   PINT4S     */
                    UINT16 PINT3S:1;            /*   PINT3S     */
                    UINT16 PINT2S:1;            /*   PINT2S     */
                    UINT16 PINT1S:1;            /*   PINT1S     */
                    UINT16 PINT0S:1;            /*   PINT0S     */
                    } BIT;                      /*              */
             } ICR2;                            /*              */
       union {                                  /* IRQRR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 IRQ7F:1;             /*   IRQ7F      */
                    UINT16 IRQ6F:1;             /*   IRQ6F      */
                    UINT16 IRQ5F:1;             /*   IRQ5F      */
                    UINT16 IRQ4F:1;             /*   IRQ4F      */
                    UINT16 IRQ3F:1;             /*   IRQ3F      */
                    UINT16 IRQ2F:1;             /*   IRQ2F      */
                    UINT16 IRQ1F:1;             /*   IRQ1F      */
                    UINT16 IRQ0F:1;             /*   IRQ0F      */
                    } BIT;                      /*              */
             } IRQRR;                           /*              */
       union {                                  /* PINTER       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 PINT7E:1;            /*   PINT7E     */
                    UINT16 PINT6E:1;            /*   PINT6E     */
                    UINT16 PINT5E:1;            /*   PINT5E     */
                    UINT16 PINT4E:1;            /*   PINT4E     */
                    UINT16 PINT3E:1;            /*   PINT3E     */
                    UINT16 PINT2E:1;            /*   PINT2E     */
                    UINT16 PINT1E:1;            /*   PINT1E     */
                    UINT16 PINT0E:1;            /*   PINT0E     */
                    } BIT;                      /*              */
             } PINTER;                          /*              */
       union {                                  /* PIRR         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 PINT7R:1;            /*   PINT7R     */
                    UINT16 PINT6R:1;            /*   PINT6R     */
                    UINT16 PINT5R:1;            /*   PINT5R     */
                    UINT16 PINT4R:1;            /*   PINT4R     */
                    UINT16 PINT3R:1;            /*   PINT3R     */
                    UINT16 PINT2R:1;            /*   PINT2R     */
                    UINT16 PINT1R:1;            /*   PINT1R     */
                    UINT16 PINT0R:1;            /*   PINT0R     */
                    } BIT;                      /*              */
             } PIRR;                            /*              */
       union {                                  /* IBCR         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 E15:1;               /*   E15        */
                    UINT16 E14:1;               /*   E14        */
                    UINT16 E13:1;               /*   E13        */
                    UINT16 E12:1;               /*   E12        */
                    UINT16 E11:1;               /*   E11        */
                    UINT16 E10:1;               /*   E10        */
                    UINT16 E9:1;                /*   E9         */
                    UINT16 E8:1;                /*   E8         */
                    UINT16 E7:1;                /*   E7         */
                    UINT16 E6:1;                /*   E6         */
                    UINT16 E5:1;                /*   E5         */
                    UINT16 E4:1;                /*   E4         */
                    UINT16 E3:1;                /*   E3         */
                    UINT16 E2:1;                /*   E2         */
                    UINT16 E1:1;                /*   E1         */
                    UINT16 :1;                  /*              */
                    } BIT;                      /*              */
             } IBCR;                            /*              */
       union {                                  /* IBNR         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BE:2;                /*   BE         */
                    UINT16 BOVE:1;              /*   BOVE       */
                    UINT16 :9;                  /*              */
                    UINT16 BN:4;                /*   BN         */
                    } BIT;                      /*              */
             } IBNR;                            /*              */
       UINT8 wk0[8];                           /*              */
       union {                                  /* IPR01        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _IRQ0:4;             /*   _IRQ0      */
                    UINT16 _IRQ1:4;             /*   _IRQ1      */
                    UINT16 _IRQ2:4;             /*   _IRQ2      */
                    UINT16 _IRQ3:4;             /*   _IRQ3      */
                    } BIT;                      /*              */
             } IPR01;                           /*              */
       union {                                  /* IPR02        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _IRQ4:4;             /*   _IRQ4      */
                    UINT16 _IRQ5:4;             /*   _IRQ5      */
                    UINT16 _IRQ6:4;             /*   _IRQ6      */
                    UINT16 _IRQ7:4;             /*   _IRQ7      */
                    } BIT;                      /*              */
             } IPR02;                           /*              */
       UINT8 wk1[4];                           /*              */
       union {                                  /* IPR05        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _PINT:4;             /*   _IRQ4      */
                    UINT16 :12;                 /*              */
                    } BIT;                      /*              */
             } IPR05;                           /*              */
       UINT8 wk2[990];                         /*              */
       union {                                  /* IPR06        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _DMAC0:4;            /*   _DMAC0     */
                    UINT16 _DMAC1:4;            /*   _DMAC1     */
                    UINT16 _DMAC2:4;            /*   _DMAC2     */
                    UINT16 _DMAC3:4;            /*   _DMAC3     */
                    } BIT;                      /*              */
             } IPR06;                           /*              */
       union {                                  /* IPR07        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _DMAC4:4;            /*   _DMAC4     */
                    UINT16 _DMAC5:4;            /*   _DMAC5     */
                    UINT16 _DMAC6:4;            /*   _DMAC6     */
                    UINT16 _DMAC7:4;            /*   _DMAC7     */
                    } BIT;                      /*              */
             } IPR07;                           /*              */
       union {                                  /* IPR08        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _DMAC8:4;            /*   _DMAC8     */
                    UINT16 _DMAC9:4;            /*   _DMAC9     */
                    UINT16 _DMAC10:4;           /*   _DMAC10    */
                    UINT16 _DMAC11:4;           /*   _DMAC11    */
                    } BIT;                      /*              */
             } IPR08;                           /*              */
       union {                                  /* IPR09        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _DMAC12:4;           /*   _DMAC12    */
                    UINT16 _DMAC13:4;           /*   _DMAC13    */
                    UINT16 _DMAC14:4;           /*   _DMAC14    */
                    UINT16 _DMAC15:4;           /*   _DMAC15    */
                    } BIT;                      /*              */
             } IPR09;                           /*              */
       union {                                  /* IPR10        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _USB:4;              /*   _USB       */
                    UINT16 _VDC3:4;             /*   _VDC3      */
                    UINT16 _CMT0:4;             /*   _CMT0      */
                    UINT16 _CMT1:4;             /*   _CMT1      */
                    } BIT;                      /*              */
             } IPR10;                           /*              */
       union {                                  /* IPR11        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _BSC:4;              /*   _BSC       */
                    UINT16 _WDT:4;              /*   _WDT       */
                    UINT16 _MTU00:4;            /*   _MTU00     */
                    UINT16 _MTU01:4;            /*   _MTU01     */
                    } BIT;                      /*              */
             } IPR11;                           /*              */
       union {                                  /* IPR12        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _MTU10:4;            /*   _MTU10     */
                    UINT16 _MTU11:4;            /*   _MTU11     */
                    UINT16 _MTU20:4;            /*   _MTU20     */
                    UINT16 _MTU21:4;            /*   _MTU21     */
                    } BIT;                      /*              */
             } IPR12;                           /*              */
       union {                                  /* IPR13        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _MTU30:4;            /*   _MTU30     */
                    UINT16 _MTU31:4;            /*   _MTU31     */
                    UINT16 _MTU40:4;            /*   _MTU40     */
                    UINT16 _MTU41:4;            /*   _MTU41     */
                    } BIT;                      /*              */
             } IPR13;                           /*              */
       union {                                  /* IPR14        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _PWM1:4;             /*   _PWM1      */
                    UINT16 _PWM2:4;             /*   _PWM2      */
                    UINT16 _ADC:4;              /*   _ADC       */
                    UINT16 :4;                  /*              */
                    } BIT;                      /*              */
             } IPR14;                           /*              */
       union {                                  /* IPR15        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _SSI0:4;             /*   _SSI0      */
                    UINT16 _SSI1:4;             /*   _SSI1      */
                    UINT16 _SSI2:4;             /*   _SSI2      */
                    UINT16 _SSI3:4;             /*   _SSI3      */
                    } BIT;                      /*              */
             } IPR15;                           /*              */
       union {                                  /* IPR16        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _SPDIF:4;            /*   _SPDIF     */
                    UINT16 _IIC30:4;            /*   _IIC30     */
                    UINT16 _IIC31:4;            /*   _IIC31     */
                    UINT16 _IIC32:4;            /*   _IIC32     */
                    } BIT;                      /*              */
             } IPR16;                           /*              */
       union {                                  /* IPR17        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _SCIF0:4;            /*   _SCIF0     */
                    UINT16 _SCIF1:4;            /*   _SCIF1     */
                    UINT16 _SCIF2:4;            /*   _SCIF2     */
                    UINT16 _SCIF3:4;            /*   _SCIF3     */
                    } BIT;                      /*              */
             } IPR17;                           /*              */
       union {                                  /* IPR18        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _SCIF4:4;            /*   _SCIF4     */
                    UINT16 _SCIF5:4;            /*   _SCIF5     */
                    UINT16 _SCIF6:4;            /*   _SCIF6     */
                    UINT16 _SCIF7:4;            /*   _SCIF7     */
                    } BIT;                      /*              */
             } IPR18;                           /*              */
       union {                                  /* IPR19        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _SIOF:4;             /*   _SIOF      */
                    UINT16 :4;                  /*              */
                    UINT16 _RSPI0:4;            /*   _RSPI0     */
                    UINT16 _RSPI1:4;            /*   _RSPI1     */
                    } BIT;                      /*              */
             } IPR19;                           /*              */
       union {                                  /* IPR20        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _RCAN0:4;            /*   _RCAN0     */
                    UINT16 _RCAN1:4;            /*   _RCAN1     */
                    UINT16 _IEB:4;              /*   _IEB       */
                    UINT16 _ROMDEC:4;           /*   _ROMDEC    */
                    } BIT;                      /*              */
             } IPR20;                           /*              */
       union {                                  /* IPR21        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _FLCTL:4;            /*   _FLCTL     */
                    UINT16 _SDHI:4;             /*   _SDHI      */
                    UINT16 _RTC:4;              /*   _RTC       */
                    UINT16 :4;                  /*              */
                    } BIT;                      /*              */
             } IPR21;                           /*              */
       union {                                  /* IPR22        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 _SRC0:4;             /*   _SRC0      */
                    UINT16 _SRC1:4;             /*   _SRC1      */
                    UINT16 :4;                  /*              */
                    UINT16 _DCU:4;              /*   _DCU       */
                    } BIT;                      /*              */
             } IPR22;                           /*              */
};                                              /*              */
struct st_ccnt {                                /* struct CCNT  */
       union {                                  /* CCR1         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :20;                /*              */
                    UINT32 ICF:1;              /*   ICF        */
                    UINT32 :2;                 /*              */
                    UINT32 ICE:1;              /*   ICE        */
                    UINT32 :4;                 /*              */
                    UINT32 OCF:1;              /*   OCF        */
                    UINT32 :1;                 /*              */
                    UINT32 WT:1;               /*   WT         */
                    UINT32 OCE:1;              /*   OCE        */
                    } BIT;                      /*              */
             } CCR1;                            /*              */
       union {                                  /* CCR2         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :15;                /*              */
                    UINT32 LE:1;               /*   LE         */
                    UINT32 :6;                 /*              */
                    UINT32 W3LOAD:1;           /*   W3LOAD     */
                    UINT32 W3LOCK:1;           /*   W3LOCK     */
                    UINT32 :6;                 /*              */
                    UINT32 W2LOAD:1;           /*   W2LOAD     */
                    UINT32 W2LOCK:1;           /*   W2LOCK     */
                    } BIT;                      /*              */
             } CCR2;                            /*              */
};
union CSnBCR{                                   /* CSnBCR       */
      UINT32 LONG;                             /*  Long Access */
      struct {                                  /*  Bit Access  */
             UINT32 :1;                        /*              */
             UINT32 IWW:3;                     /*   IWW        */
             UINT32 IWRWD:3;                   /*   IWRWD      */
             UINT32 IWRWS:3;                   /*   IWRWS      */
             UINT32 IWRRD:3;                   /*   IWRRD      */
             UINT32 IWRRS:3;                   /*   IWRRS      */
             UINT32 :1;                        /*              */
             UINT32 TYPE:3;                    /*   TYPE       */
             UINT32 ENDIAN:1;                  /*   ENDIAN     */
             UINT32 BSZ:2;                     /*   BSZ        */
             UINT32 :9;                        /*              */
             } BIT;                             /*              */
};                                              /*              */
struct st_bsc {                                 /* struct BSC   */
       union {                                  /* CMNCR        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :20;                /*              */
                    UINT32 BLOCK:1;            /*   BLOCK      */
                    UINT32 DPRTY:2;            /*   DPRTY      */
                    UINT32 DMAIW:3;            /*   DMAIW      */
                    UINT32 DMAIWA:1;           /*   DMAIWA     */
                    UINT32 :3;                 /*              */
                    UINT32 HIZMEM:1;           /*   HIZMEM     */
                    UINT32 HIZCNT:1;           /*   HIZCNT     */
                    } BIT;                      /*              */
             } CMNCR;                           /*              */
       union CSnBCR CS0BCR;                     /* CS0BCR       */
       union CSnBCR CS1BCR;                     /* CS1BCR       */
       union CSnBCR CS2BCR;                     /* CS2BCR       */
       union CSnBCR CS3BCR;                     /* CS3BCR       */
       union CSnBCR CS4BCR;                     /* CS4BCR       */
       union CSnBCR CS5BCR;                     /* CS5BCR       */
       union CSnBCR CS6BCR;                     /* CS6BCR       */
       
       UINT8 wk0[8];                           /*              */
       union {                                  /* CS0WCR       */
             union {                            /* CS0WCR(NORMAL) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :11;         /*              */
                           UINT32 BAS:1;       /*   BAS        */
                           UINT32 :7;          /*              */
                           UINT32 SW:2;        /*   SW         */
                           UINT32 WR:4;        /*   WR         */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :4;          /*              */
                           UINT32 HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
             union {                            /* CS0WCR(BROM_ASY) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :10;         /*              */
                           UINT32 BST:2;       /*   BST        */
                           UINT32 :2;          /*              */
                           UINT32 BW:2;        /*   BW         */
                           UINT32 :5;          /*              */
                           UINT32 W:4;         /*   W          */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :6;          /*              */
                           } BIT;               /*              */
                    } BROM_ASY;                 /*              */
             union {                            /* CS0WCR(BROM_SY) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :14;         /*              */
                           UINT32 BW:2;        /*   BW         */
                           UINT32 :5;          /*              */
                           UINT32 W:4;         /*   W          */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :6;          /*              */
                           } BIT;               /*              */
                    } BROM_SY;                  /*              */
             } CS0WCR;
       union {                                  /* CS1WCR       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :11;                /*              */
                    UINT32 BAS:1;              /*   BAS        */
                    UINT32 :1;                 /*              */
                    UINT32 WW:3;               /*   WW         */
                    UINT32 :3;                 /*              */
                    UINT32 SW:2;               /*   SW         */
                    UINT32 WR:4;               /*   WR         */
                    UINT32 WM:1;               /*   WM         */
                    UINT32 :4;                 /*              */
                    UINT32 HW:2;               /*   HW         */
                    } BIT;                      /*              */
             } CS1WCR;                          /*              */
       union {                                  /* CS2WCR       */
             union {                            /* CS2WCR(NORMAL) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :11;         /*              */
                           UINT32 BAS:1;       /*   BAS        */
                           UINT32 :9;          /*              */
                           UINT32 WR:4;        /*   WR         */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :6;          /*              */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
             union {                            /* CS2WCR(SDRAM) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                          /*  Bit Access  */
                           UINT32 :23;         /*              */
                           UINT32 A2CL:2;      /*   A2CL       */
                           UINT32 :7;          /*              */
                           } BIT;               /*              */
                    } SDRAM;                    /*              */
             } CS2WCR;                          /*              */
       union {                                  /* CS3WCR       */
             union {                            /* CS3WCR(NORMAL) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :11;         /*              */
                           UINT32 BAS:1;       /*   BAS        */
                           UINT32 :9;          /*              */
                           UINT32 WR:4;        /*   WR         */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :6;          /*              */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
             union {                            /* CS3WCR(SDRAM) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :17;         /*              */
                           UINT32 WTRP:2;      /*   WTRP       */
                           UINT32 :1;          /*              */
                           UINT32 WTRCD:2;     /*   WTRCD      */
                           UINT32 :1;          /*              */
                           UINT32 A3CL:2;      /*   A3CL       */
                           UINT32 :2;          /*              */
                           UINT32 TRWL:2;      /*   TRWL       */
                           UINT32 :1;          /*              */
                           UINT32 WTRC:2;      /*   WTRC       */
                           } BIT;               /*              */
                    } SDRAM;                    /*              */
             } CS3WCR;                          /*              */
       union {                                  /* CS4WCR       */
              union {                           /* CS4WCR(NORMAL) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :11;         /*              */
                           UINT32 BAS:1;       /*   BAS        */
                           UINT32 :1;          /*              */
                           UINT32 WW:3;        /*   WW         */
                           UINT32 :3;          /*              */
                           UINT32 SW:2;        /*   SW         */
                           UINT32 WR:4;        /*   WR         */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :4;          /*              */
                           UINT32 HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
              union {                           /* CS4WCR(BROM_ASY) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :10;         /*              */
                           UINT32 BST:2;       /*   BST        */
                           UINT32 :2;          /*              */
                           UINT32 BW:2;        /*   BW         */
                           UINT32 :3;          /*              */
                           UINT32 SW:2;        /*   SW         */
                           UINT32 W:4;         /*   W          */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :4;          /*              */
                           UINT32 HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } BROM_ASY;                 /*              */
             } CS4WCR;                          /*              */
       union {                                  /* CS5WCR       */
              union {                           /* CS5WCR(NORMAL) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :10;         /*              */
                           UINT32 SZSEL:1;     /*   SZSEL      */
                           UINT32 MPXWBAS:1;   /*   MPXW/BAS   */
                           UINT32 :1;          /*              */
                           UINT32 WW:3;        /*   WW         */
                           UINT32 :3;          /*              */
                           UINT32 SW:2;        /*   SW         */
                           UINT32 WR:4;        /*   WR         */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :4;          /*              */
                           UINT32 HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
              union {                           /* CS5WCR(PCMCIA) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :10;         /*              */
                           UINT32 SA:2;        /*   SA         */
                           UINT32 :5;          /*              */
                           UINT32 TED:4;       /*   TED        */
                           UINT32 PCW:4;       /*   PCW        */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :2;          /*              */
                           UINT32 TEH:4;       /*   TEH        */
                           } BIT;               /*              */
                    } PCMCIA;                   /*              */
             } CS5WCR;                          /*              */
       union {                                  /* CS6WCR       */
              union {                           /* CS6WCR(NORMAL) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :11;         /*              */
                           UINT32 BAS:1;       /*   BAS        */
                           UINT32 :7;          /*              */
                           UINT32 SW:2;        /*   SW         */
                           UINT32 WR:4;        /*   WR         */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :4;          /*              */
                           UINT32 HW:2;        /*   HW         */
                           } BIT;               /*              */
                    } NORMAL;                   /*              */
              union {                           /* CS6WCR(PCMCIA) */
                    UINT32 LONG;               /*  Long Access */
                    struct {                    /*  Bit Access  */
                           UINT32 :10;         /*              */
                           UINT32 SA:2;        /*   SA         */
                           UINT32 :5;          /*              */
                           UINT32 TED:4;       /*   TED        */
                           UINT32 PCW:4;       /*   PCW        */
                           UINT32 WM:1;        /*   WM         */
                           UINT32 :2;          /*              */
                           UINT32 TEH:4;       /*   TEH        */
                           } BIT;               /*              */
                    } PCMCIA;                   /*              */
             } CS6WCR;                          /*              */
       UINT8 wk1[8];                           /*              */
       union {                                  /* SDCR         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :11;                /*              */
                    UINT32 A2ROW:2;            /*   A2ROW      */
                    UINT32 :1;                 /*              */
                    UINT32 A2COL:2;            /*   A2COL      */
                    UINT32 :2;                 /*              */
                    UINT32 DEEP:1;             /*   DEEP       */
                    UINT32 SLOW:1;             /*   SLOW       */
                    UINT32 RFSH:1;             /*   RFSH       */
                    UINT32 RMODE:1;            /*   RMODE      */
                    UINT32 PDOWN:1;            /*   PDOWN      */
                    UINT32 BACTV:1;            /*   BACTV      */
                    UINT32 :3;                 /*              */
                    UINT32 A3ROW:2;            /*   A3ROW      */
                    UINT32 :1;                 /*              */
                    UINT32 A3COL:2;            /*   A3COL      */
                    } BIT;                      /*              */
             } SDCR;                            /*              */
       union {                                  /* RTCSR        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :24;                /*              */
                    UINT32 CMF:1;              /*   CMF        */
                    UINT32 CMIE:1;             /*   CMIE       */
                    UINT32 CKS:3;              /*   CKS        */
                    UINT32 RRC:3;              /*   RRC        */
                    } BIT;                      /*              */
             } RTCSR;                           /*              */
       UINT32 RTCNT;                           /* RTCNT        */
       UINT32 RTCOR;                           /* RTCOR        */
};                                              /*              */
struct st_dmac {                                /* struct DMAC  */
       union {                                  /* SAR0         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR0;                            /*              */
       union {                                  /* DAR0         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR0;                            /*              */
       union {                                  /* DMATCR0      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR0;                         /*              */
       union {                                  /* CHCR0        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 DO:1;                /*   DO         */
                    UINT8 TL:1;                /*   TL         */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 AM:1;                /*   AM         */
                    UINT8 AL:1;                /*   AL         */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 DL:1;                /*   DL         */
                    UINT8 DS:1;                /*   DS         */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR0;                           /*              */
       union {                                  /* SAR1         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR1;                            /*              */
       union {                                  /* DAR1         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR1;                            /*              */
       union {                                  /* DMATCR1      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR1;                         /*              */
       union {                                  /* CHCR1        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 DO:1;                /*   DO         */
                    UINT8 TL:1;                /*   TL         */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 AM:1;                /*   AM         */
                    UINT8 AL:1;                /*   AL         */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 DL:1;                /*   DL         */
                    UINT8 DS:1;                /*   DS         */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR1;                           /*              */
       union {                                  /* SAR2         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR2;                            /*              */
       union {                                  /* DAR2         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR2;                            /*              */
       union {                                  /* DMATCR2      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR2;                         /*              */
       union {                                  /* CHCR2        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR2;                           /*              */
       union {                                  /* SAR3         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR3;                            /*              */
       union {                                  /* DAR3         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR3;                            /*              */
       union {                                  /* DMATCR3      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR3;                         /*              */
       union {                                  /* CHCR3        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR3;                           /*              */
       union {                                  /* SAR4         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR4;                            /*              */
       union {                                  /* DAR4         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR4;                            /*              */
       union {                                  /* DMATCR4      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR4;                         /*              */
       union {                                  /* CHCR4        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR4;                           /*              */
       union {                                  /* SAR5         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR5;                            /*              */
       union {                                  /* DAR5         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR5;                            /*              */
       union {                                  /* DMATCR5      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR5;                         /*              */
       union {                                  /* CHCR5        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR5;                           /*              */
       union {                                  /* SAR6         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR6;                            /*              */
       union {                                  /* DAR6         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR6;                            /*              */
       union {                                  /* DMATCR6      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR6;                         /*              */
       union {                                  /* CHCR6        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR6;                           /*              */
       union {                                  /* SAR7         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR7;                            /*              */
       union {                                  /* DAR7         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR7;                            /*              */
       union {                                  /* DMATCR7      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR7;                         /*              */
       union {                                  /* CHCR7        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR7;                           /*              */
       union {                                  /* SAR8         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR8;                            /*              */
       union {                                  /* DAR8         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR8;                            /*              */
       union {                                  /* DMATCR8      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR8;                         /*              */
       union {                                  /* CHCR8        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR8;                           /*              */
       union {                                  /* SAR9         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR9;                            /*              */
       union {                                  /* DAR9         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR9;                            /*              */
       union {                                  /* DMATCR9      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR9;                         /*              */
       union {                                  /* CHCR9        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR9;                           /*              */
       union {                                  /* SAR10        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR10;                           /*              */
       union {                                  /* DAR10        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR10;                           /*              */
       union {                                  /* DMATCR10     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR10;                        /*              */
       union {                                  /* CHCR10       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR10;                          /*              */
       union {                                  /* SAR11        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR11;                           /*              */
       union {                                  /* DAR11        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR11;                           /*              */
       union {                                  /* DMATCR11     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR11;                        /*              */
       union {                                  /* CHCR11       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR11;                          /*              */
       union {                                  /* SAR12        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR12;                           /*              */
       union {                                  /* DAR12        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR12;                           /*              */
       union {                                  /* DMATCR12     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR12;                        /*              */
       union {                                  /* CHCR12       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR12;                          /*              */
       union {                                  /* SAR13        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR13;                           /*              */
       union {                                  /* DAR13        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR13;                           /*              */
       union {                                  /* DMATCR13     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR13;                        /*              */
       union {                                  /* CHCR13       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR13;                          /*              */
       union {                                  /* SAR14        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR14;                           /*              */
       union {                                  /* DAR14        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR14;                           /*              */
       union {                                  /* DMATCR14     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR14;                        /*              */
       union {                                  /* CHCR14       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR14;                          /*              */
       union {                                  /* SAR15        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SAR15;                           /*              */
       union {                                  /* DAR15        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DAR15;                           /*              */
       union {                                  /* DMATCR15     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } DMATCR15;                        /*              */
       union {                                  /* CHCR15       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 TC:1;                /*   TC         */
                    UINT8 :1;                  /*              */
                    UINT8 RLDSAR:1;            /*   RLDSAR     */
                    UINT8 RLDDAR:1;            /*   RLDDAR     */
                    UINT8 :1;                  /*              */
                    UINT8 DAF:1;               /*   DAF        */
                    UINT8 SAF:1;               /*   SAF        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TEMASK:1;            /*   TEMASK     */
                    UINT8 HE:1;                /*   HE         */
                    UINT8 HIE:1;               /*   HIE        */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 DM:2;                /*   DM         */
                    UINT8 SM:2;                /*   SM         */
                    UINT8 RS:4;                /*   RS         */
                    UINT8 :1;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 TB:1;                /*   TB         */
                    UINT8 TS:2;                /*   TS         */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 TE:1;                /*   TE         */
                    UINT8 DE:1;                /*   DE         */
                    } BIT;                      /*              */
             } CHCR15;                          /*              */
       union {                                  /* RSAR0        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR0;                           /*              */
       union {                                  /* RDAR0        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR0;                           /*              */
       union {                                  /* RDMATCR0     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR0;                        /*              */
       UINT8 wk0[4];                           /*              */
       union {                                  /* RSAR1        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR1;                           /*              */
       union {                                  /* RDAR1        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR1;                           /*              */
       union {                                  /* RDMATCR1     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR1;                        /*              */
       UINT8 wk1[4];                           /*              */
       union {                                  /* RSAR2        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR2;                           /*              */
       union {                                  /* RDAR2        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR2;                           /*              */
       union {                                  /* RDMATCR2     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR2;                        /*              */
       UINT8 wk2[4];                           /*              */
       union {                                  /* RSAR3        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR3;                           /*              */
       union {                                  /* RDAR3        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR3;                           /*              */
       union {                                  /* RDMATCR3     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR3;                        /*              */
       UINT8 wk3[4];                           /*              */
       union {                                  /* RSAR4        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR4;                           /*              */
       union {                                  /* RDAR4        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR4;                           /*              */
       union {                                  /* RDMATCR4     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR4;                        /*              */
       UINT8 wk4[4];                           /*              */
       union {                                  /* RSAR5        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR5;                           /*              */
       union {                                  /* RDAR5        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR5;                           /*              */
       union {                                  /* RDMATCR5     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR5;                        /*              */
       UINT8 wk5[4];                           /*              */
       union {                                  /* RSAR6        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR6;                           /*              */
       union {                                  /* RDAR6        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR6;                           /*              */
       union {                                  /* RDMATCR6     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR6;                        /*              */
       UINT8 wk6[4];                           /*              */
       union {                                  /* RSAR7        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR7;                           /*              */
       union {                                  /* RDAR7        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR7;                           /*              */
       union {                                  /* RDMATCR7     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR7;                        /*              */
       UINT8 wk7[4];                           /*              */
       union {                                  /* RSAR8        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR8;                           /*              */
       union {                                  /* RDAR8        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR8;                           /*              */
       union {                                  /* RDMATCR8     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR8;                        /*              */
       UINT8 wk8[4];                           /*              */
       union {                                  /* RSAR9        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR9;                           /*              */
       union {                                  /* RDAR9        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR9;                           /*              */
       union {                                  /* RDMATCR9     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR9;                        /*              */
       UINT8 wk9[4];                           /*              */
       union {                                  /* RSAR10       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR10;                          /*              */
       union {                                  /* RDAR10       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR10;                          /*              */
       union {                                  /* RDMATCR10    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR10;                       /*              */
       UINT8 wk10[4];                          /*              */
       union {                                  /* RSAR11       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR11;                          /*              */
       union {                                  /* RDAR11       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR11;                          /*              */
       union {                                  /* RDMATCR11    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR11;                       /*              */
       UINT8 wk11[4];                          /*              */
       union {                                  /* RSAR12       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR12;                          /*              */
       union {                                  /* RDAR12       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR12;                          /*              */
       union {                                  /* RDMATCR12    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR12;                       /*              */
       UINT8 wk12[4];                          /*              */
       union {                                  /* RSAR13       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR13;                          /*              */
       union {                                  /* RDAR13       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR13;                          /*              */
       union {                                  /* RDMATCR13    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR13;                       /*              */
       UINT8 wk13[4];                          /*              */
       union {                                  /* RSAR14       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR14;                          /*              */
       union {                                  /* RDAR14       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR14;                          /*              */
       union {                                  /* RDMATCR14    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR14;                       /*              */
       UINT8 wk14[4];                          /*              */
       union {                                  /* RSAR15       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RSAR15;                          /*              */
       union {                                  /* RDAR15       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDAR15;                          /*              */
       union {                                  /* RDMATCR15    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } RDMATCR15;                       /*              */
       UINT8 wk15[4];                          /*              */
       union {                                  /* DMAOR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 :2;                  /*              */
                    UINT16 CMS:2;               /*   CMS        */
                    UINT16 :2;                  /*              */
                    UINT16 PR:2;                /*   PR         */
                    UINT16 :5;                  /*              */
                    UINT16 AE:1;                /*   AE         */
                    UINT16 NMIF:1;              /*   NMIF       */
                    UINT16 DME:1;               /*   DME        */
                    } BIT;                      /*              */
             } DMAOR;                           /*              */
       UINT8 wk16[254];                        /*              */
       union {                                  /* DMARS0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT16 CH1:8;               /*   CH1        */
                    UINT16 CH0:8;               /*   CH0        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 CH1MID:6;            /*   CH1MID     */
                    UINT16 CH1RID:2;            /*   CH1RID     */
                    UINT16 CH0MID:6;            /*   CH0MID     */
                    UINT16 CH0RID:2;            /*   CH0RID     */
                    } BIT;                      /*              */
             } DMARS0;                          /*              */
       UINT8 wk17[2];                          /*              */
       union {                                  /* DMARS1       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT16 CH3:8;               /*   CH3        */
                    UINT16 CH2:8;               /*   CH2        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 CH3MID:6;            /*   CH3ID     */
                    UINT16 CH3RID:2;            /*   CH3RID     */
                    UINT16 CH2MID:6;            /*   CH2MID     */
                    UINT16 CH2RID:2;            /*   CH2RID     */
                    } BIT;                      /*              */
             } DMARS1;                          /*              */
       UINT8 wk18[2];                          /*              */
       union {                                  /* DMARS2       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT16 CH5:8;               /*   CH5        */
                    UINT16 CH4:8;               /*   CH4        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 CH5MID:6;            /*   CH5MID     */
                    UINT16 CH5RID:2;            /*   CH5RID     */
                    UINT16 CH4MID:6;            /*   CH4MID     */
                    UINT16 CH4RID:2;            /*   CH4RID     */
                    } BIT;                      /*              */
             } DMARS2;                          /*              */
       UINT8 wk19[2];                          /*              */
       union {                                  /* DMARS3       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT16 CH7:8;               /*   CH7        */
                    UINT16 CH6:8;               /*   CH6        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 CH7MID:6;            /*   CH7MID     */
                    UINT16 CH7RID:2;            /*   CH7RID     */
                    UINT16 CH6MID:6;            /*   CH6MID     */
                    UINT16 CH6RID:2;            /*   CH6RID     */
                    } BIT;                      /*              */
             } DMARS3;                          /*              */
       UINT8 wk20[2];                          /*              */
       union {                                  /* DMARS4       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT16 CH9:8;               /*   CH9        */
                    UINT16 CH8:8;               /*   CH8        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 CH9MID:6;            /*   CH9MID     */
                    UINT16 CH9RID:2;            /*   CH9RID     */
                    UINT16 CH8MID:6;            /*   CH8MID     */
                    UINT16 CH8RID:2;            /*   CH8RID     */
                    } BIT;                      /*              */
             } DMARS4;                          /*              */
       UINT8 wk21[2];                          /*              */
       union {                                  /* DMARS5       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT16 CH11:8;              /*   CH11       */
                    UINT16 CH10:8;              /*   CH10       */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 CH11MID:6;           /*   CH11MID    */
                    UINT16 CH11RID:2;           /*   CH11RID    */
                    UINT16 CH10MID:6;           /*   CH10MID    */
                    UINT16 CH10RID:2;           /*   CH10RID    */
                    } BIT;                      /*              */
             } DMARS5;                          /*              */
       UINT8 wk22[2];                          /*              */
       union {                                  /* DMARS6       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT16 CH13:8;              /*   CH13       */
                    UINT16 CH12:8;              /*   CH12       */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 CH13MID:6;           /*   CH13MID    */
                    UINT16 CH13RID:2;           /*   CH13RID    */
                    UINT16 CH12MID:6;           /*   CH12MID    */
                    UINT16 CH12RID:2;           /*   CH12RID    */
                    } BIT;                      /*              */
             } DMARS6;                          /*              */
       UINT8 wk23[2];                          /*              */
       union {                                  /* DMARS7       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT16 CH15:8;              /*   CH15       */
                    UINT16 CH14:8;              /*   CH14       */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 CH15MID:6;           /*   CH15MID    */
                    UINT16 CH15RID:2;           /*   CH15RID    */
                    UINT16 CH14MID:6;           /*   CH14MID    */
                    UINT16 CH14RID:2;           /*   CH14RID    */
                    } BIT;                      /*              */
             } DMARS7;                          /*              */
};                                              /*              */
struct st_mtu2 {                                /* struct MTU2  */
       union {                                  /* TCR_2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 CCLR:2;              /*   CCLR       */
                    UINT8 CKEG:2;              /*   CKEG       */
                    UINT8 TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_2;                           /*              */
       union {                                  /* TMDR_2       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_2;                          /*              */
       union {                                  /* TIOR_2       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOB:4;               /*   IOB        */
                    UINT8 IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIOR_2;                          /*              */
       UINT8 wk0[1];                           /*              */
       union {                                  /* TIER_2       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TTGE:1;              /*   TTGE       */
                    UINT8 :1;                  /*              */
                    UINT8 TCIEU:1;             /*   TCIEU      */
                    UINT8 TCIEV:1;             /*   TCIEV      */
                    UINT8 :2;                  /*              */
                    UINT8 TGIEB:1;             /*   TGIEB      */
                    UINT8 TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_2;                          /*              */
       union {                                  /* TSR_2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TCFD:1;              /*   TCFD       */
                    UINT8 :1;                  /*              */
                    UINT8 TCFU:1;              /*   TCFU       */
                    UINT8 TCFV:1;              /*   TCFV       */
                    UINT8 TGFD:1;              /*   TGFD       */
                    UINT8 TGFC:1;              /*   TGFC       */
                    UINT8 TGFB:1;              /*   TGFB       */
                    UINT8 TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_2;                           /*              */
       UINT16 TCNT_2;                           /* TCNT_2       */
       UINT16 TGRA_2;                           /* TGRA_2       */
       UINT16 TGRB_2;                           /* TGRB_2       */
       UINT8 wk1[500];                         /*              */
       union {                                  /* TCR_3        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 CCLR:3;              /*   CCLR       */
                    UINT8 CKEG:2;              /*   CKEG       */
                    UINT8 TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_3;                           /*              */
       union {                                  /* TCR_4        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 CCLR:3;              /*   CCLR       */
                    UINT8 CKEG:2;              /*   CKEG       */
                    UINT8 TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_4;                           /*              */
       union {                                  /* TMDR_3       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 BFB:1;               /*   BFB        */
                    UINT8 BFA:1;               /*   BFA        */
                    UINT8 MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_3;                          /*              */
       union {                                  /* TMDR_4       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 BFB:1;               /*   BFB        */
                    UINT8 BFA:1;               /*   BFA        */
                    UINT8 MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_4;                          /*              */
       union {                                  /* TIORH_3      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOB:4;               /*   IOB        */
                    UINT8 IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIORH_3;                         /*              */
       union {                                  /* TIORL_3      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOD:4;               /*   IOD        */
                    UINT8 IOC:4;               /*   IOC        */
                    } BIT;                      /*              */
             } TIORL_3;                         /*              */
       union {                                  /* TIORH_4      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOB:4;               /*   IOB        */
                    UINT8 IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIORH_4;                         /*              */
       union {                                  /* TIORL_4      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOD:4;               /*   IOD        */
                    UINT8 IOC:4;               /*   IOC        */
                    } BIT;                      /*              */
             } TIORL_4;                         /*              */
       union {                                  /* TIER_3       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TTGE:1;              /*   TTGE       */
                    UINT8 :2;                  /*              */
                    UINT8 TCIEV:1;             /*   TCIEV      */
                    UINT8 TGIED:1;             /*   TGIED      */
                    UINT8 TGIEC:1;             /*   TGIEC      */
                    UINT8 TGIEB:1;             /*   TGIEB      */
                    UINT8 TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_3;                          /*              */
       union {                                  /* TIER_4       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TTGE:1;              /*   TTGE       */
                    UINT8 TTGE2:1;             /*   TTGE2      */
                    UINT8 :1;                  /*              */
                    UINT8 TCIEV:1;             /*   TCIEV      */
                    UINT8 TGIED:1;             /*   TGIED      */
                    UINT8 TGIEC:1;             /*   TGIEC      */
                    UINT8 TGIEB:1;             /*   TGIEB      */
                    UINT8 TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_4;                          /*              */
       union {                                  /* TOER         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 OE4D:1;              /*   OE4D       */
                    UINT8 OE4C:1;              /*   OE4C       */
                    UINT8 OE3D:1;              /*   OE3D       */
                    UINT8 OE4B:1;              /*   OE4B       */
                    UINT8 OE4A:1;              /*   OE4A       */
                    UINT8 OE3B:1;              /*   OE3B       */
                    } BIT;                      /*              */
             } TOER;                            /*              */
       UINT8 wk2[2];                           /*              */
       union {                                  /* TGCR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 BDC:1;               /*   BDC        */
                    UINT8 N:1;                 /*   N          */
                    UINT8 P:1;                 /*   P          */
                    UINT8 FB:1;                /*   FB         */
                    UINT8 WF:1;                /*   WF         */
                    UINT8 VF:1;                /*   VF         */
                    UINT8 UF:1;                /*   UF         */
                    } BIT;                      /*              */
             } TGCR;                            /*              */
       union {                                  /* TOCR1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 PSYE:1;              /*   PSYE       */
                    UINT8 :2;                  /*              */
                    UINT8 TOCL:1;              /*   TOCL       */
                    UINT8 TOCS:1;              /*   TOCS       */
                    UINT8 OLSN:1;              /*   OLSN       */
                    UINT8 OLSP:1;              /*   OLSP       */
                    } BIT;                      /*              */
             } TOCR1;                           /*              */
       union {                                  /* TOCR2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 BF:2;                /*   BF         */
                    UINT8 OLS3N:1;             /*   OLS3N      */
                    UINT8 OLS3P:1;             /*   OLS3P      */
                    UINT8 OLS2N:1;             /*   OLS2N      */
                    UINT8 OLS2P:1;             /*   OLS2P      */
                    UINT8 OLS1N:1;             /*   OLS1N      */
                    UINT8 OLS1P:1;             /*   OLS1P      */
                    } BIT;                      /*              */
             } TOCR2;                           /*              */
       UINT16 TCNT_3;                           /* TCNT_3       */
       UINT16 TCNT_4;                           /* TCNT_4       */
       UINT16 TCDR;                             /* TCDR         */
       UINT16 TDDR;                             /* TDDR         */
       UINT16 TGRA_3;                           /* TGRA_3       */
       UINT16 TGRB_3;                           /* TGRB_3       */
       UINT16 TGRA_4;                           /* TGRA_4       */
       UINT16 TGRB_4;                           /* TGRB_4       */
       UINT16 TCNTS;                            /* TCNTS        */
       UINT16 TCBR;                             /* TCBR         */
       UINT16 TGRC_3;                           /* TGRC_3       */
       UINT16 TGRD_3;                           /* TGRD_3       */
       UINT16 TGRC_4;                           /* TGRC_4       */
       UINT16 TGRD_4;                           /* TGRD_4       */
       union {                                  /* TSR_3        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TCFD:1;              /*   TCFD       */
                    UINT8 :2;                  /*              */
                    UINT8 TCFV:1;              /*   TCFV       */
                    UINT8 TGFD:1;              /*   TGFD       */
                    UINT8 TGFC:1;              /*   TGFC       */
                    UINT8 TGFB:1;              /*   TGFB       */
                    UINT8 TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_3;                           /*              */
       union {                                  /* TSR_4        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TCFD:1;              /*   TCFD       */
                    UINT8 :2;                  /*              */
                    UINT8 TCFV:1;              /*   TCFV       */
                    UINT8 TGFD:1;              /*   TGFD       */
                    UINT8 TGFC:1;              /*   TGFC       */
                    UINT8 TGFB:1;              /*   TGFB       */
                    UINT8 TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_4;                           /*              */
       UINT8 wk3[2];                           /*              */
       union {                                  /* TITCR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 T3AEN:1;             /*   T3AEN      */
                    UINT8 _3ACOR:3;            /*   _3ACOR     */
                    UINT8 T4VEN:1;             /*   T4VEN      */
                    UINT8 _4VCOR:3;            /*   _4VCOR     */
                    } BIT;                      /*              */
             } TITCR;                           /*              */
       union {                                  /* TITCNT       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 _3ACNT:3;            /*   _3ACNT     */
                    UINT8 :1;                  /*              */
                    UINT8 _4VCNT:3;            /*   _4VCNT     */
                    } BIT;                      /*              */
             } TITCNT;                          /*              */
       union {                                  /* TBTER        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 BTE:2;               /*   BTE        */
                    } BIT;                      /*              */
             } TBTER;                           /*              */
       UINT8 wk4[1];                           /*              */
       union {                                  /* TDER         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 TDER:1;              /*   TDER       */
                    } BIT;                      /*              */
             } TDER;                            /*              */
       UINT8 wk5[1];                           /*              */
       union {                                  /* TOLBR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 OLS3N:1;             /*   OLS3N      */
                    UINT8 OLS3P:1;             /*   OLS3P      */
                    UINT8 OLS2N:1;             /*   OLS2N      */
                    UINT8 OLS2P:1;             /*   OLS2P      */
                    UINT8 OLS1N:1;             /*   OLS1N      */
                    UINT8 OLS1P:1;             /*   OLS1P      */
                    } BIT;                      /*              */
             } TOLBR;                           /*              */
       UINT8 wk6[1];                           /*              */
       union {                                  /* TBTM_3       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 TTSB:1;              /*   TTSB       */
                    UINT8 TTSA:1;              /*   TTSA       */
                    } BIT;                      /*              */
             } TBTM_3;                          /*              */
       union {                                  /* TBTM_4       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 TTSB:1;              /*   TTSB       */
                    UINT8 TTSA:1;              /*   TTSA       */
                    } BIT;                      /*              */
             } TBTM_4;                          /*              */
       UINT8 wk7[6];                           /*              */
       union {                                  /* TADCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BF:2;                /*   BF         */
                    UINT16 :6;                  /*              */
                    UINT16 UT4AE:1;             /*   UT4AE      */
                    UINT16 DT4AE:1;             /*   DT4AE      */
                    UINT16 UT4BE:1;             /*   UT4BE      */
                    UINT16 DT4BE:1;             /*   DT4BE      */
                    UINT16 ITA3AE:1;            /*   ITA3AE     */
                    UINT16 ITA4VE:1;            /*   ITA4VE     */
                    UINT16 ITB3AE:1;            /*   ITB3AE     */
                    UINT16 ITB4VE:1;            /*   ITB4VE     */
                    } BIT;                      /*              */
             } TADCR;                           /*              */
       UINT8 wk8[2];                           /*              */
       UINT16 TADCORA_4;                        /* TADCORA_4    */
       UINT16 TADCORB_4;                        /* TADCORB_4    */
       UINT16 TADCOBRA_4;                       /* TADCOBRA_4   */
       UINT16 TADCOBRB_4;                       /* TADCOBRB_4   */
       UINT8 wk9[20];                          /*              */
       union {                                  /* TWCR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 CCE:1;               /*   CCE        */
                    UINT8 :6;                  /*              */
                    UINT8 WRE:1;               /*   WRE        */
                    } BIT;                      /*              */
             } TWCR;                            /*              */
       UINT8 wk10[31];                         /*              */
       union {                                  /* TSTR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 CST4:1;              /*   CST4       */
                    UINT8 CST3:1;              /*   CST3       */
                    UINT8 :3;                  /*              */
                    UINT8 CST2:1;              /*   CST2       */
                    UINT8 CST1:1;              /*   CST1       */
                    UINT8 CST0:1;              /*   CST0       */
                    } BIT;                      /*              */
             } TSTR;                            /*              */
       union {                                  /* TSYR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 SYNC4:1;             /*   SYNC4      */
                    UINT8 SYNC3:1;             /*   SYNC3      */
                    UINT8 :3;                  /*              */
                    UINT8 SYNC2:1;             /*   SYNC2      */
                    UINT8 SYNC1:1;             /*   SYNC1      */
                    UINT8 SYNC0:1;             /*   SYNC0      */
                    } BIT;                      /*              */
             } TSYR;                            /*              */
       UINT8 wk11[2];                          /*              */
       union {                                  /* TRWER        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 RWE:1;               /*   RWE        */
                    } BIT;                      /*              */
             } TRWER;                           /*              */
       UINT8 wk12[123];                        /*              */
       union {                                  /* TCR_0        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 CCLR:3;              /*   CCLR       */
                    UINT8 CKEG:2;              /*   CKEG       */
                    UINT8 TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_0;                           /*              */
       union {                                  /* TMDR_0       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 BFE:1;               /*   BFE        */
                    UINT8 BFB:1;               /*   BFB        */
                    UINT8 BFA:1;               /*   BFA        */
                    UINT8 MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_0;                          /*              */
       union {                                  /* TIORH_0      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOB:4;               /*   IOB        */
                    UINT8 IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIORH_0;                         /*              */
       union {                                  /* TIORL_0      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOD:4;               /*   IOD        */
                    UINT8 IOC:4;               /*   IOC        */
                    } BIT;                      /*              */
             } TIORL_0;                         /*              */
       union {                                  /* TIER_0       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TTGE:1;              /*   TTGE       */
                    UINT8 :2;                  /*              */
                    UINT8 TCIEV:1;             /*   TCIEV      */
                    UINT8 TGIED:1;             /*   TGIED      */
                    UINT8 TGIEC:1;             /*   TGIEC      */
                    UINT8 TGIEB:1;             /*   TGIEB      */
                    UINT8 TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_0;                          /*              */
       union {                                  /* TSR_0        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TCFD:1;              /*   TCFD       */
                    UINT8 :2;                  /*              */
                    UINT8 TCFV:1;              /*   TCFV       */
                    UINT8 TGFD:1;              /*   TGFD       */
                    UINT8 TGFC:1;              /*   TGFC       */
                    UINT8 TGFB:1;              /*   TGFB       */
                    UINT8 TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_0;                           /*              */
       UINT16 TCNT_0;                           /* TCNT_0       */
       UINT16 TGRA_0;                           /* TGRA_0       */
       UINT16 TGRB_0;                           /* TGRB_0       */
       UINT16 TGRC_0;                           /* TGRC_0       */
       UINT16 TGRD_0;                           /* TGRD_0       */
       UINT8 wk13[16];                         /*              */
       UINT16 TGRE_0;                           /* TGRE_0       */
       UINT16 TGRF_0;                           /* TGRF_0       */
       union {                                  /* TIER2_0      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TTGE2:1;             /*   TTGE2      */
                    UINT8 :5;                  /*              */
                    UINT8 TGIEF:1;             /*   TGIEF      */
                    UINT8 TGIEE:1;             /*   TGIEE      */
                    } BIT;                      /*              */
             } TIER2_0;                         /*              */
       union {                                  /* TSR2_0       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 TGFF:1;              /*   TGFF       */
                    UINT8 TGFE:1;              /*   TGFE       */
                    } BIT;                      /*              */
             } TSR2_0;                          /*              */
       union {                                  /* TBTM_0       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 TTSE:1;              /*   TTSE       */
                    UINT8 TTSB:1;              /*   TTSB       */
                    UINT8 TTSA:1;              /*   TTSA       */
                    } BIT;                      /*              */
             } TBTM_0;                          /*              */
       UINT8 wk14[89];                         /*              */
       union {                                  /* TCR_1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 CCLR:2;              /*   CCLR       */
                    UINT8 CKEG:2;              /*   CKEG       */
                    UINT8 TPSC:3;              /*   TPSC       */
                    } BIT;                      /*              */
             } TCR_1;                           /*              */
       union {                                  /* TMDR_1       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 MD:4;                /*   MD         */
                    } BIT;                      /*              */
             } TMDR_1;                          /*              */
       union {                                  /* TIOR_1       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOB:4;               /*   IOB        */
                    UINT8 IOA:4;               /*   IOA        */
                    } BIT;                      /*              */
             } TIOR_1;                          /*              */
       UINT8 wk15[1];                          /*              */
       union {                                  /* TIER_1       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TTGE:1;              /*   TTGE       */
                    UINT8 :1;                  /*              */
                    UINT8 TCIEU:1;             /*   TCIEU      */
                    UINT8 TCIEV:1;             /*   TCIEV      */
                    UINT8 :2;                  /*              */
                    UINT8 TGIEB:1;             /*   TGIEB      */
                    UINT8 TGIEA:1;             /*   TGIEA      */
                    } BIT;                      /*              */
             } TIER_1;                          /*              */
       union {                                  /* TSR_1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TCFD:1;              /*   TCFD       */
                    UINT8 :1;                  /*              */
                    UINT8 TCFU:1;              /*   TCFU       */
                    UINT8 TCFV:1;              /*   TCFV       */
                    UINT8 TGFD:1;              /*   TGFD       */
                    UINT8 TGFC:1;              /*   TGFC       */
                    UINT8 TGFB:1;              /*   TGFB       */
                    UINT8 TGFA:1;              /*   TGFA       */
                    } BIT;                      /*              */
             } TSR_1;                           /*              */
       UINT16 TCNT_1;                           /* TCNT_1       */
       UINT16 TGRA_1;                           /* TGRA_1       */
       UINT16 TGRB_1;                           /* TGRB_1       */
       UINT8 wk16[4];                          /*              */
       union {                                  /* TICCR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 I2BE:1;              /*   I2BE       */
                    UINT8 I2AE:1;              /*   I2AE       */
                    UINT8 I1BE:1;              /*   I1BE       */
                    UINT8 I1AE:1;              /*   I1AE       */
                    } BIT;                      /*              */
             } TICCR;                           /*              */
};                                              /*              */

#if 0
struct st_cmt {                                 /* struct CMT   */
       union {                                  /* CMSTR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :14;                 /*              */
                    UINT16 STR1:1;              /*   STR1       */
                    UINT16 STR0:1;              /*   STR0       */
                    } BIT;                      /*              */
             } CMSTR;                           /*              */
       union {                                  /* CMCSR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 CMF:1;               /*   CMF        */
                    UINT16 CMIE:1;              /*   CMIE       */
                    UINT16 :4;                  /*              */
                    UINT16 CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } CMCSR0;                          /*              */
       UINT16 CMCNT0;                           /* CMCNT0       */
       UINT16 CMCOR0;                           /* CMCOR0       */
       union {                                  /* CMCSR1       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 CMF:1;               /*   CMF        */
                    UINT16 CMIE:1;              /*   CMIE       */
                    UINT16 :4;                  /*              */
                    UINT16 CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } CMCSR1;                          /*              */
       UINT16 CMCNT1;                           /* CMCNT1       */
       UINT16 CMCOR1;                           /* CMCOR1       */
};   
#endif

struct st_cmt {                                         /* struct CMT   */
              union {                                   /* CMSTR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :8;        /*              */
                           unsigned char     :6;        /*              */
                           unsigned char STR1:1;        /*    STR1      */
                           unsigned char STR0:1;        /*    STR0      */
                           }       BIT;                 /*              */
                    }           CMSTR;                  /*              */
};                                                      /*              */
struct st_cmt0 {                                        /* struct CMT0  */
               union {                                  /* CMCSR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char     :8;       /*              */
                            unsigned char CMF :1;       /*    CMF       */
                            unsigned char CMIE:1;       /*    CMIE      */
                            unsigned char     :4;       /*              */
                            unsigned char CKS :2;       /*    CKS       */
                            }       BIT;                /*              */
                     }          CMCSR;                  /*              */
               unsigned short   CMCNT;                  /* CMCNT        */
               unsigned short   CMCOR;                  /* CMCOR        */
};                                                      /*              */
                                           /*              */
struct st_wdt {                                 /* struct WDT   */
       union {                                  /* WTCSR        */
             UINT16 WORD;                       /*  word Access */
             struct {                           /*  Bit Access  */
                    UINT8 IOVF:1;              /*   IOVF       */
                    UINT8 WTIT:1;              /*   WT/IT      */
                    UINT8 TME:1;               /*   TME        */
                    UINT8 :2;                  /*              */
                    UINT8 CKS:3;               /*   CKS        */
                    } BIT;                      /*              */
             } WTCSR;                           /*              */
       union {                                  /* WTCNT        */
             UINT16 WORD;                       /*  word Access */
             struct {                           /*  Bit Access  */
                    UINT8 TCNT:8;              /*   CKS        */
                    } BIT;                      /*              */
             } WTCNT;                           /*              */
       union {                                  /* WRCSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT8 WOVF:1;              /*   WOVF       */
                    UINT8 RSTE:1;              /*   RSTE       */
                    UINT8 RSTS:1;              /*   RSTS       */
                    UINT8 :5;                  /*              */
                    } BIT;                      /*              */
             } WRCSR;                           /*              */
};                                              /*              */
struct st_rtc {                                 /* struct RTC   */
       union {                                  /* R64CNT       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 _1Hz:1;              /*   _1Hz       */
                    UINT8 _2Hz:1;              /*   _2Hz       */
                    UINT8 _4Hz:1;              /*   _4Hz       */
                    UINT8 _8Hz:1;              /*   _8Hz       */
                    UINT8 _16Hz:1;             /*   _16Hz      */
                    UINT8 _32Hz:1;             /*   _32Hz      */
                    UINT8 _64Hz:1;             /*   _64Hz      */
                    } BIT;                      /*              */
             } R64CNT;                          /*              */
       UINT8 wk0[1];                           /*              */
       UINT8 RSECCNT;                          /* RSECCNT      */
       UINT8 wk1[1];                           /*              */
       UINT8 RMINCNT;                          /* RMINCNT      */
       UINT8 wk2[1];                           /*              */
       UINT8 RHRCNT;                           /* RHRCNT       */
       UINT8 wk3[1];                           /*              */
       UINT8 RWKCNT;                           /* RWKCNT       */
       UINT8 wk4[1];                           /*              */
       UINT8 RDAYCNT;                          /* RDAYCNT      */
       UINT8 wk5[1];                           /*              */
       UINT8 RMONCNT;                          /* RMONCNT      */
       UINT8 wk6[1];                           /*              */
       UINT16 RYRCNT;                           /* RYRCNT       */
       UINT8 RSECAR;                           /* RSECAR       */
       UINT8 wk7[1];                           /*              */
       UINT8 RMINAR;                           /* RMINAR       */
       UINT8 wk8[1];                           /*              */
       UINT8 RHRAR;                            /* RHRAR        */
       UINT8 wk9[1];                           /*              */
       UINT8 RWKAR;                            /* RWKAR        */
       UINT8 wk10[1];                          /*              */
       UINT8 RDAYAR;                           /* RDAYAR       */
       UINT8 wk11[1];                          /*              */
       UINT8 RMONAR;                           /* RMONAR       */
       UINT8 wk12[1];                          /*              */
       union {                                  /* RCR1         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 CF:1;                /*   CF         */
                    UINT8 :2;                  /*              */
                    UINT8 CIE:1;               /*   CIE        */
                    UINT8 AIE:1;               /*   AIE        */
                    UINT8 :2;                  /*              */
                    UINT8 AF:1;                /*   AF         */
                    } BIT;                      /*              */
             } RCR1;                            /*              */
       UINT8 wk13[1];                          /*              */
       union {                                  /* RCR2         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 PEF:1;               /*   PEF        */
                    UINT8 PES:3;               /*   PES        */
                    UINT8 RTCEN:1;             /*   RTCEN      */
                    UINT8 ADJ:1;               /*   ADJ        */
                    UINT8 RESET:1;             /*   RESET      */
                    UINT8 START:1;             /*   START      */
                    } BIT;                      /*              */
             } RCR2;                            /*              */
       UINT8 wk14[1];                          /*              */
       UINT16 RYRAR;                            /* RYRAR        */
       UINT8 wk15[2];                          /*              */
       union {                                  /* RCR3         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ENB:1;               /*   ENB        */
                    UINT8 :7;                  /*              */
                    } BIT;                      /*              */
             } RCR3;                            /*              */
       UINT8 wk16[1];                          /*              */
       union {                                  /* RCR5         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 RCKSEL:1;            /*   RCKSEL     */
                    } BIT;                      /*              */
             } RCR5;                            /*              */
       UINT8 wk17[2];                          /*              */
       UINT8 wk18[1];                          /*              */
       union {                                  /* RFRH         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :15;                 /*              */
                    UINT16 RFC16:1;             /*   RFC[16]    */
                    } BIT;                      /*              */
             } RFRH;                            /*              */
       union {                                  /* RFRL         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 RFC15:1;             /*   RFC[15]    */
                    UINT16 RFC14:1;             /*   RFC[14]    */
                    UINT16 RFC13:1;             /*   RFC[13]    */
                    UINT16 RFC12:1;             /*   RFC[12]    */
                    UINT16 RFC11:1;             /*   RFC[11]    */
                    UINT16 RFC10:1;             /*   RFC[10]    */
                    UINT16 RFC9:1;              /*   RFC[9]     */
                    UINT16 RFC8:1;              /*   RFC[8]     */
                    UINT16 RFC7:1;              /*   RFC[7]     */
                    UINT16 RFC6:1;              /*   RFC[6]     */
                    UINT16 RFC5:1;              /*   RFC[5]     */
                    UINT16 RFC4:1;              /*   RFC[4]     */
                    UINT16 RFC3:1;              /*   RFC[3]     */
                    UINT16 RFC2:1;              /*   RFC[2]     */
                    UINT16 RFC1:1;              /*   RFC[1]     */
                    UINT16 RFC0:1;              /*   RFC[0]     */
                    } BIT;                      /*              */
             } RFRL;                            /*              */
};                                              /*              */
struct st_scif02 {                              /* struct SCIF  */
       union {                                  /* SCSMR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 CA:1;                /*   C/A        */
                    UINT16 CHR:1;               /*   CHR        */
                    UINT16 PE:1;                /*   PE         */
                    UINT16 OE:1;                /*   O/E        */
                    UINT16 STOP:1;              /*   STOP       */
                    UINT16 :1;                  /*              */
                    UINT16 CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } SCSMR;                           /*              */
       UINT8 wk0[2];                           /*              */
       UINT8 SCBRR;                            /* SCBRR        */
       UINT8 wk1[3];                           /*              */
       union {                                  /* SCSCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 TIE:1;               /*   TIE        */
                    UINT16 RIE:1;               /*   RIE        */
                    UINT16 TE:1;                /*   TE         */
                    UINT16 RE:1;                /*   RE         */
                    UINT16 REIE:1;              /*   REIE       */
                    UINT16 :1;                  /*              */
                    UINT16 CKE:2;               /*   CKE        */
                    } BIT;                      /*              */
             } SCSCR;                           /*              */
       UINT8 wk2[2];                           /*              */
       UINT8 SCFTDR;                           /* SCFTDR       */
       UINT8 wk3[3];                           /*              */
       union {                                  /* SCFSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 PERN:4;              /*   PERN       */
                    UINT16 FERN:4;              /*   FERN       */
                    UINT16 ER:1;                /*   ER         */
                    UINT16 TEND:1;              /*   TEND       */
                    UINT16 TDFE:1;              /*   TDFE       */
                    UINT16 BRK:1;               /*   BRK        */
                    UINT16 FER:1;               /*   FER        */
                    UINT16 PER:1;               /*   PER        */
                    UINT16 RDF:1;               /*   RDF        */
                    UINT16 DR:1;                /*   DR         */
                    } BIT;                      /*              */
             } SCFSR;                           /*              */
       UINT8 wk4[2];                           /*              */
       UINT8 SCFRDR;                           /* SCFRDR       */
       UINT8 wk5[3];                           /*              */
       union {                                  /* SCFCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :5;                  /*              */
                    UINT16 RSTRG:3;             /*   RSTRG      */
                    UINT16 RTRG:2;              /*   RTRG       */
                    UINT16 TTRG:2;              /*   TTRG       */
                    UINT16 MCE:1;               /*   MCE        */
                    UINT16 TFRST:1;             /*   TFRST      */
                    UINT16 RFRST:1;             /*   RFRST      */
                    UINT16 LOOP:1;              /*   LOOP       */
                    } BIT;                      /*              */
             } SCFCR;                           /*              */
       UINT8 wk6[2];                           /*              */
       union {                                  /* SCFDR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 T:5;                 /*   T          */
                    UINT16 :3;                  /*              */
                    UINT16 R:5;                 /*   R          */
                    } BIT;                      /*              */
             } SCFDR;                           /*              */
       UINT8 wk7[2];                           /*              */
       union {                                  /* SCSPTR       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :12;                 /*              */
                    UINT16 SCKIO:1;             /*   SCKIO      */
                    UINT16 SCKDT:1;             /*   SCKDT      */
                    UINT16 SPB2IO:1;            /*   SPB2IO     */
                    UINT16 SPB2DT:1;            /*   SPB2DT     */
                    } BIT;                      /*              */
             } SCSPTR;                          /*              */
       UINT8 wk8[2];                           /*              */
       union {                                  /* SCLSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :15;                 /*              */
                    UINT16 ORER:1;              /*   ORER       */
                    } BIT;                      /*              */
             } SCLSR;                           /*              */
       UINT8 wk9[2];                           /*              */
       union {                                  /* SCEMR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 BGDM:1;              /*   BGDM       */
                    UINT16 :6;                  /*              */
                    UINT16 ABCS:1;              /*   ABCS       */
                    } BIT;                      /*              */
             } SCEMR;                           /*              */
};                                              /*              */
struct st_scif4567 {                            /* struct SCIF  */
       union {                                  /* SCSMR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 CA:1;                /*   C/A        */
                    UINT16 CHR:1;               /*   CHR        */
                    UINT16 PE:1;                /*   PE         */
                    UINT16 OE:1;                /*   O/E        */
                    UINT16 STOP:1;              /*   STOP       */
                    UINT16 :1;                  /*              */
                    UINT16 CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } SCSMR;                           /*              */
       UINT8 wk0[2];                           /*              */
       UINT8 SCBRR;                            /* SCBRR        */
       UINT8 wk1[3];                           /*              */
       union {                                  /* SCSCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 TIE:1;               /*   TIE        */
                    UINT16 RIE:1;               /*   RIE        */
                    UINT16 TE:1;                /*   TE         */
                    UINT16 RE:1;                /*   RE         */
                    UINT16 REIE:1;              /*   REIE       */
                    UINT16 :1;                  /*              */
                    UINT16 CKE:2;               /*   CKE        */
                    } BIT;                      /*              */
             } SCSCR;                           /*              */
       UINT8 wk2[2];                           /*              */
       UINT8 SCFTDR;                           /* SCFTDR       */
       UINT8 wk3[3];                           /*              */
       union {                                  /* SCFSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 PERN:4;              /*   PERN       */
                    UINT16 FERN:4;              /*   FERN       */
                    UINT16 ER:1;                /*   ER         */
                    UINT16 TEND:1;              /*   TEND       */
                    UINT16 TDFE:1;              /*   TDFE       */
                    UINT16 BRK:1;               /*   BRK        */
                    UINT16 FER:1;               /*   FER        */
                    UINT16 PER:1;               /*   PER        */
                    UINT16 RDF:1;               /*   RDF        */
                    UINT16 DR:1;                /*   DR         */
                    } BIT;                      /*              */
             } SCFSR;                           /*              */
       UINT8 wk4[2];                           /*              */
       UINT8 SCFRDR;                           /* SCFRDR       */
       UINT8 wk5[3];                           /*              */
       union {                                  /* SCFCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :5;                  /*              */
                    UINT16 RSTRG:3;             /*   RSTRG      */
                    UINT16 RTRG:2;              /*   RTRG       */
                    UINT16 TTRG:2;              /*   TTRG       */
                    UINT16 MCE:1;               /*   MCE        */
                    UINT16 TFRST:1;             /*   TFRST      */
                    UINT16 RFRST:1;             /*   RFRST      */
                    UINT16 LOOP:1;              /*   LOOP       */
                    } BIT;                      /*              */
             } SCFCR;                           /*              */
       UINT8 wk6[2];                           /*              */
       union {                                  /* SCFDR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 T:5;                 /*   T          */
                    UINT16 :3;                  /*              */
                    UINT16 R:5;                 /*   R          */
                    } BIT;                      /*              */
             } SCFDR;                           /*              */
       UINT8 wk7[2];                           /*              */
       union {                                  /* SCSPTR       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :14;                 /*              */
                    UINT16 SPB2IO:1;            /*   SPB2IO     */
                    UINT16 SPB2DT:1;            /*   SPB2DT     */
                    } BIT;                      /*              */
             } SCSPTR;                          /*              */
       UINT8 wk8[2];                           /*              */
       union {                                  /* SCLSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :15;                 /*              */
                    UINT16 ORER:1;              /*   ORER       */
                    } BIT;                      /*              */
             } SCLSR;                           /*              */
       UINT8 wk9[2];                           /*              */
       union {                                  /* SCEMR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 BGDM:1;              /*   BGDM       */
                    UINT16 :6;                  /*              */
                    UINT16 ABCS:1;              /*   ABCS       */
                    } BIT;                      /*              */
             } SCEMR;                           /*              */
};                                              /*              */
struct st_scif13 {                              /* struct SCIF  */
       union {                                  /* SCSMR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 CA:1;                /*   C/A        */
                    UINT16 CHR:1;               /*   CHR        */
                    UINT16 PE:1;                /*   PE         */
                    UINT16 OE:1;                /*   O/E        */
                    UINT16 STOP:1;              /*   STOP       */
                    UINT16 :1;                  /*              */
                    UINT16 CKS:2;               /*   CKS        */
                    } BIT;                      /*              */
             } SCSMR;                           /*              */
       UINT8 wk0[2];                           /*              */
       UINT8 SCBRR;                            /* SCBRR        */
       UINT8 wk1[3];                           /*              */
       union {                                  /* SCSCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 TIE:1;               /*   TIE        */
                    UINT16 RIE:1;               /*   RIE        */
                    UINT16 TE:1;                /*   TE         */
                    UINT16 RE:1;                /*   RE         */
                    UINT16 REIE:1;              /*   REIE       */
                    UINT16 :1;                  /*              */
                    UINT16 CKE:2;               /*   CKE        */
                    } BIT;                      /*              */
             } SCSCR  ;                         /*              */
       UINT8 wk2[2];                           /*              */
       UINT8 SCFTDR;                           /* SCFTDR       */
       UINT8 wk3[3];                           /*              */
       union {                                  /* SCFSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 PERN:4;              /*   PERN       */
                    UINT16 FERN:4;              /*   FERN       */
                    UINT16 ER:1;                /*   ER         */
                    UINT16 TEND:1;              /*   TEND       */
                    UINT16 TDFE:1;              /*   TDFE       */
                    UINT16 BRK:1;               /*   BRK        */
                    UINT16 FER:1;               /*   FER        */
                    UINT16 PER:1;               /*   PER        */
                    UINT16 RDF:1;               /*   RDF        */
                    UINT16 DR:1;                /*   DR         */
                    } BIT;                      /*              */
             } SCFSR  ;                         /*              */
       UINT8 wk4[2];                           /*              */
       UINT8 SCFRDR;                           /* SCFRDR       */
       UINT8 wk5[3];                           /*              */
       union {                                  /* SCFCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :5;                  /*              */
                    UINT16 RSTRG:3;             /*   RSTRG      */
                    UINT16 RTRG:2;              /*   RTRG       */
                    UINT16 TTRG:2;              /*   TTRG       */
                    UINT16 MCE:1;               /*   MCE        */
                    UINT16 TFRST:1;             /*   TFRST      */
                    UINT16 RFRST:1;             /*   RFRST      */
                    UINT16 LOOP:1;              /*   LOOP       */
                    } BIT;                      /*              */
             } SCFCR;                           /*              */
       UINT8 wk6[2];                           /*              */
       union {                                  /* SCFDR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 T:5;                 /*   T          */
                    UINT16 :3;                  /*              */
                    UINT16 R:5;                 /*   R          */
                    } BIT;                      /*              */
             } SCFDR;                           /*              */
       UINT8 wk7[2];                           /*              */
       union {                                  /* SCSPTR       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 RTSIO:1;             /*   RTSIO      */
                    UINT16 RTSDT:1;             /*   RTSDT      */
                    UINT16 CTSIO:1;             /*   CTSIO      */
                    UINT16 CTSDT:1;             /*   CTSDT      */
                    UINT16 SCKIO:1;             /*   SCKIO      */
                    UINT16 SCKDT:1;             /*   SCKDT      */
                    UINT16 SPB2IO:1;            /*   SPB2IO     */
                    UINT16 SPB2DT:1;            /*   SPB2DT     */
                    } BIT;                      /*              */
             } SCSPTR;                          /*              */
       UINT8 wk8[2];                           /*              */
       union {                                  /* SCLSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :15;                 /*              */
                    UINT16 ORER:1;              /*   ORER       */
                    } BIT;                      /*              */
             } SCLSR;                           /*              */
       UINT8 wk9[2];                           /*              */
       union {                                  /* SCEMR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 BGDM:1;              /*   BGDM       */
                    UINT16 :6;                  /*              */
                    UINT16 ABCS:1;              /*   ABCS       */
                    } BIT;                      /*              */
             } SCEMR;                           /*              */
};                                              /*              */

struct st_rspi {                                /* struct RSPI  */
       union {                                  /* SPCR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 SPRIE:1;             /*   SPRIE      */
                    UINT8 SPE:1;               /*   SPE        */
                    UINT8 SPTIE:1;             /*   SPTIE      */
                    UINT8 SPEIE:1;             /*   SPEIE      */
                    UINT8 MSTR:1;              /*   MSTR       */
                    UINT8 MODFEN:1;            /*   MODFEN     */
                    UINT8 FWMM:1;              /*   FWMM       */
                    UINT8 :1;                  /*              */
                    } BIT;                      /*              */
             } SPCR;                            /*              */
       union {                                  /* SSLP         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 SSL0P:1;             /*   SSL0P      */
                    } BIT;                      /*              */
             } SSLP;                            /*              */
       union {                                  /* SPPCR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 MOIFE:1;             /*   MOIFE      */
                    UINT8 MOIFV:1;             /*   MOIFV      */
                    UINT8 :3;                  /*              */
                    UINT8 SPLP:1;              /*   SPLP       */
                    } BIT;                      /*              */
             } SPPCR;                           /*              */
       union {                                  /* SPSR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 SPRF:1;              /*   SPRF       */
                    UINT8 TEND:1;              /*   TEND       */
                    UINT8 SPTEF:1;             /*   SPTEF      */
                    UINT8 :2;                  /*              */
                    UINT8 MODF:1;              /*   MODF       */
                    UINT8 :1;                  /*              */
                    UINT8 OVRF:1;              /*   OVRF       */
                    } BIT;                      /*              */
             } SPSR;                            /*              */
       union {                                  /* SPDR         */
             UINT32 LONG;                      /*  Long Access */
             UINT16 WORD;                       /*  Word Access */
             UINT8 BYTE;                       /*  Byte Access */
             } SPDR;                            /*              */
       union {                                  /* SPSCR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 SPSLN:2;             /*   SPSLN      */
                    } BIT;                      /*              */
             } SPSCR;                           /*              */
       union {                                  /* SPSSR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 SPCP:2;              /*   SPCP       */
                    } BIT;                      /*              */
             } SPSSR;                           /*              */
       union {                                  /* SPBR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 SPR:8;               /*   SPR        */
                    } BIT;                      /*              */
             } SPBR;                            /*              */
       union {                                  /* SPDCR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TXDMY:1;             /*   TXDMY      */
                    UINT8 SPLW:2;              /*   SPLW       */
                    UINT8 :5;                  /*              */
                    } BIT;                      /*              */
             } SPDCR;                           /*              */
       union {                                  /* SPCKD        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 SCKDL:3;             /*   SCKDL      */
                    } BIT;                      /*              */
             } SPCKD;                           /*              */
       union {                                  /* SSLND        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 SLNDL:3;             /*   SLNDL      */
                    } BIT;                      /*              */
             } SSLND;                           /*              */
       union {                                  /* SPND         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 SPNDL:3;             /*   SPNDL      */
                    } BIT;                      /*              */
             } SPND;                            /*              */
       UINT8 wk0[1];                           /*              */
       union {                                  /* SPCMD0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 SCKDEN:1;            /*   SCKDEN     */
                    UINT16 SLNDEN:1;            /*   SLNDEN     */
                    UINT16 SPNDEN:1;            /*   SPNDEN     */
                    UINT16 LSBF:1;              /*   LSBF       */
                    UINT16 SPB:4;               /*   SPB        */
                    UINT16 SSLKP:1;             /*   SSLKP      */
                    UINT16 :3;                  /*              */
                    UINT16 BRDV:2;              /*   BRDV       */
                    UINT16 CPOL:1;              /*   CPOL       */
                    UINT16 CPHA:1;              /*   CPHA       */
                    } BIT;                      /*              */
             } SPCMD0;                          /*              */
       union {                                  /* SPCMD1       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 SCKDEN:1;            /*   SCKDEN     */
                    UINT16 SLNDEN:1;            /*   SLNDEN     */
                    UINT16 SPNDEN:1;            /*   SPNDEN     */
                    UINT16 LSBF:1;              /*   LSBF       */
                    UINT16 SPB:4;               /*   SPB        */
                    UINT16 SSLKP:1;             /*   SSLKP      */
                    UINT16 :3;                  /*              */
                    UINT16 BRDV:2;              /*   BRDV       */
                    UINT16 CPOL:1;              /*   CPOL       */
                    UINT16 CPHA:1;              /*   CPHA       */
                    } BIT;                      /*              */
             } SPCMD1 ;                         /*              */
       union {                                  /* SPCMD2       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 SCKDEN:1;            /*   SCKDEN     */
                    UINT16 SLNDEN:1;            /*   SLNDEN     */
                    UINT16 SPNDEN:1;            /*   SPNDEN     */
                    UINT16 LSBF:1;              /*   LSBF       */
                    UINT16 SPB:4;               /*   SPB        */
                    UINT16 SSLKP:1;             /*   SSLKP      */
                    UINT16 :3;                  /*              */
                    UINT16 BRDV:2;              /*   BRDV       */
                    UINT16 CPOL:1;              /*   CPOL       */
                    UINT16 CPHA:1;              /*   CPHA       */
                    } BIT;                      /*              */
             } SPCMD2 ;                         /*              */
       union {                                  /* SPCMD3       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 SCKDEN:1;            /*   SCKDEN     */
                    UINT16 SLNDEN:1;            /*   SLNDEN     */
                    UINT16 SPNDEN:1;            /*   SPNDEN     */
                    UINT16 LSBF:1;              /*   LSBF       */
                    UINT16 SPB:4;               /*   SPB        */
                    UINT16 SSLKP:1;             /*   SSLKP      */
                    UINT16 :3;                  /*              */
                    UINT16 BRDV:2;              /*   BRDV       */
                    UINT16 CPOL:1;              /*   CPOL       */
                    UINT16 CPHA:1;              /*   CPHA       */
                    } BIT;                      /*              */
             } SPCMD3 ;                         /*              */
       UINT8 wk1[8];                           /*              */
       union {                                  /* SPBFCR       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TXRST:1;             /*   TXRST      */
                    UINT8 RXRST:1;             /*   RXRST      */
                    UINT8 TXTRG:2;             /*   TXTRG      */
                    UINT8 :1;                  /*              */
                    UINT8 RXTRG:3;             /*   RXTRG      */
                    } BIT;                      /*              */
             } SPBFCR;                          /*              */
       UINT8 wk2[1];                           /*              */
       union {                                  /* SPBFDR       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :4;                  /*              */
                    UINT16 T:4 ;                /*   T          */
                    UINT16 :2;                  /*              */
                    UINT16 R:6;                 /*   R          */
                    } BIT;                      /*              */
             } SPBFDR;                          /*              */
};                                              /*              */
struct st_iic3 {                                /* struct IIC3  */
       union {                                  /* ICCR1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ICE:1;               /*   ICE        */
                    UINT8 RCVD:1;              /*   RCVD       */
                    UINT8 MST:1;               /*   MST        */
                    UINT8 TRS:1;               /*   TRS        */
                    UINT8 CKS:4;               /*   CKS        */
                    } BIT;                      /*              */
             } ICCR1;                           /*              */
       union {                                  /* ICCR2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 BBSY:1;              /*   BBSY       */
                    UINT8 SCP:1;               /*   SCP        */
                    UINT8 SDAO:1;              /*   SDAO       */
                    UINT8 SDAOP:1;             /*   SDAOP      */
                    UINT8 SCLO:1;              /*   SCLO       */
                    UINT8 :1;                  /*              */
                    UINT8 IICRST:1;            /*   IICRST     */
                    UINT8 :1;                  /*              */
                    } BIT;                      /*              */
             } ICCR2;                           /*              */
       union {                                  /* ICMR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 MLS:1;               /*   MLS        */
                    UINT8 :3;                  /*              */
                    UINT8 BCWP:1;              /*   BCWP       */
                    UINT8 BC:3;                /*   BC         */
                    } BIT;                      /*              */
             } ICMR;                            /*              */
       union {                                  /* ICIER        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TIE:1;               /*   TIE        */
                    UINT8 TEIE:1;              /*   TEIE       */
                    UINT8 RIE:1;               /*   RIE        */
                    UINT8 NAKIE:1;             /*   NAKIE      */
                    UINT8 STIE:1;              /*   STIE       */
                    UINT8 ACKE:1;              /*   ACKE       */
                    UINT8 ACKBR:1;             /*   ACKBR      */
                    UINT8 ACKBT:1;             /*   ACKBT      */
                    } BIT;                      /*              */
             } ICIER;                           /*              */
       union {                                  /* ICSR         */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 TDRE:1;              /*   TDRE       */
                    UINT8 TEND:1;              /*   TEND       */
                    UINT8 RDRF:1;              /*   RDRF       */
                    UINT8 NACKF:1;             /*   NACKF      */
                    UINT8 STOP:1;              /*   STOP       */
                    UINT8 ALOVE:1;             /*   AL/OVE     */
                    UINT8 AAS:1;               /*   AAS        */
                    UINT8 ADZ:1;               /*   ADZ        */
                    } BIT;                      /*              */
             } ICSR;                            /*              */
       union {                                  /* SAR          */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 SVA:7;               /*   SVA        */
                    UINT8 FS:1;                /*   FS         */
                    } BIT;                      /*              */
             } SAR;                             /*              */
       UINT8 ICDRT;                            /* ICDRT        */
       UINT8 ICDRR;                            /* ICDRR        */
       union {                                  /* NF2CYC       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 PRS:1;               /*   PRS        */
                    UINT8 NF2CYC:1;            /*   NF2CYC     */
                    } BIT;                      /*              */
             } NF2CYC;                          /*              */
};                                              /*              */
struct st_ssif {                                /* struct SSIF  */
       union {                                  /* SSICR        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :1;                 /*              */
                    UINT32 CKS:1;              /*   CKS        */
                    UINT32 TUIEN:1;            /*   TUIEN      */
                    UINT32 TOIEN:1;            /*   TOIEN      */
                    UINT32 RUIEN:1;            /*   RUIEN      */
                    UINT32 ROIEN:1;            /*   ROIEN      */
                    UINT32 IIEN:1;             /*   IIEN       */
                    UINT32 :1;                 /*              */
                    UINT32 CHNL:2;             /*   CHNL       */
                    UINT32 DWL:3;              /*   DWL        */
                    UINT32 SWL:3;              /*   SWL        */
                    UINT32 SCKD:1;             /*   SCKD       */
                    UINT32 SWSD:1;             /*   SWSD       */
                    UINT32 SCKP:1;             /*   SCKP       */
                    UINT32 SWSP:1;             /*   SWSP       */
                    UINT32 SPDP:1;             /*   SPDP       */
                    UINT32 SDTA:1;             /*   SDTA       */
                    UINT32 PDTA:1;             /*   PDTA       */
                    UINT32 DEL:1;              /*   DEL        */
                    UINT32 CKDV:4;             /*   CKDV       */
                    UINT32 MUEN:1;             /*   MUEN       */
                    UINT32 :1;                 /*              */
                    UINT32 TEN:1;              /*   TEN        */
                    UINT32 REN:1;              /*   REN        */
                    } BIT;                      /*              */
             } SSICR;                           /*              */
       union {                                  /* SSISR        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :2;                 /*              */
                    UINT32 TUIRQ:1;            /*   TUIRQ      */
                    UINT32 TOIRQ:1;            /*   TOIRQ      */
                    UINT32 RUIRQ:1;            /*   RUIRQ      */
                    UINT32 ROIRQ:1;            /*   ROIRQ      */
                    UINT32 IIRQ:1;             /*   IIRQ       */
                    UINT32 :18;                /*              */
                    UINT32 TCHNO:2;            /*   TCHNO      */
                    UINT32 TSWNO:1;            /*   TSWNO      */
                    UINT32 RCHNO:2;            /*   RCHNO      */
                    UINT32 RSWNO:1;            /*   RSWNO      */
                    UINT32 IDST:1;             /*   IDST       */
                    } BIT;                      /*              */
             } SSISR;                           /*              */
       UINT8 wk0[8];                           /*              */
       union {                                  /* SSIFCR       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :24;                /*              */
                    UINT32 TTRG:2;             /*   TTRG       */
                    UINT32 RTRG:2;             /*   RTRG       */
                    UINT32 TIE:1;              /*   TIE        */
                    UINT32 RIE:1;              /*   RIE        */
                    UINT32 TFRST:1;            /*   TFRST      */
                    UINT32 RFRST:1;            /*   RFRST      */
                    } BIT;                      /*              */
             } SSIFCR;                          /*              */
       union {                                  /* SSIFSR       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :4;                 /*              */
                    UINT32 TDC:4;              /*   TDC        */
                    UINT32 :7;                 /*              */
                    UINT32 TDE:1;              /*   TDE        */
                    UINT32 :4;                 /*              */
                    UINT32 RDC:4;              /*   RDC        */
                    UINT32 :7;                 /*              */
                    UINT32 RDF:1;              /*   RDF        */
                    } BIT;                      /*              */
             } SSIFSR;                          /*              */
       UINT32 SSIFTDR;                         /* SSIFTDR      */
       UINT32 SSIFRDR;                         /* SSIFRDR      */
};                                              /*              */
struct st_siof {                                /* struct SIOF  */
       union {                                  /* SIMDR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TRMD:2;              /*   TRMD       */
                    UINT16 SYNCAT:1;            /*   SYNCAT     */
                    UINT16 REDG:1;              /*   REDG       */
                    UINT16 FL:4;                /*   FL         */
                    UINT16 TXDIZ:1;             /*   TXDIZ      */
                    UINT16 :1;                  /*              */
                    UINT16 SYNCAC:1;            /*   SYNCAC     */
                    UINT16 SYNCDL:1;            /*   SYNCDL     */
                    UINT16 :4;                  /*              */
                    } BIT;                      /*              */
             } SIMDR;                           /*              */
       union {                                  /* SISCR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 MSSEL:1;             /*   MSSEL      */
                    UINT16 :2;                  /*              */
                    UINT16 BRPS:5;              /*   BRPS       */
                    UINT16 :5;                  /*              */
                    UINT16 BRDV:3;              /*   BRDV       */
                    } BIT;                      /*              */
             } SISCR;                           /*              */
       union {                                  /* SITDAR       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TDLE:1;              /*   TDLE       */
                    UINT16 :3;                  /*              */
                    UINT16 TDLA:4;              /*   TDLA       */
                    UINT16 TDRE:1;              /*   TDRE       */
                    UINT16 TLREP:1;             /*   TLREP      */
                    UINT16 :2;                  /*              */
                    UINT16 TDRA:4;              /*   TDRA       */
                    } BIT;                      /*              */
             } SITDAR;                          /*              */
       union {                                  /* SIRDAR       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 RDLE:1;              /*   RDLE       */
                    UINT16 :3;                  /*              */
                    UINT16 RDLA:4;              /*   RDLA       */
                    UINT16 RDRE:1;              /*   RDRE       */
                    UINT16 :3;                  /*              */
                    UINT16 RDRA:4;              /*   RDRA       */
                    } BIT;                      /*              */
             } SIRDAR;                          /*              */
       UINT8 wk0[4];                           /*              */
       union {                                  /* SICTR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 SCKE:1;              /*   SCKE       */
                    UINT16 FSE:1;               /*   FSE        */
                    UINT16 :4;                  /*              */
                    UINT16 TXE:1;               /*   TXE        */
                    UINT16 RXE:1;               /*   RXE        */
                    UINT16 :6;                  /*              */
                    UINT16 TXRST:1;             /*   TXRST      */
                    UINT16 RXRST:1;             /*   RXRST      */
                    } BIT;                      /*              */
             } SICTR;                           /*              */
       UINT8 wk1[2];                           /*              */
       union {                                  /* SIFCTR       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TFWM:3;              /*   TFWM       */
                    UINT16 TFUA:5 ;             /*   TFUA       */
                    UINT16 RFWM:3;              /*   RFWM       */
                    UINT16 RFUA:5;              /*   RFUA       */
                    } BIT;                      /*              */
             } SIFCTR;                          /*              */
       UINT8 wk2[2];                           /*              */
       union {                                  /* SISTR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :2;                  /*              */
                    UINT16 TFEMP:1;             /*   TFEMP      */
                    UINT16 TDREQ:1;             /*   TDREQ      */
                    UINT16 :2;                  /*              */
                    UINT16 RFFUL:1 ;            /*   RFFUL      */
                    UINT16 RDREQ:1;             /*   RDREQ      */
                    UINT16 :3;                  /*              */
                    UINT16 FSERRE:1;            /*   FSERRE     */
                    UINT16 TFOVFE:1;            /*   TFOVFE     */
                    UINT16 TFUDFE:1;            /*   TFUDFE     */
                    UINT16 RFUDFE:1;            /*   RFUDFE     */
                    UINT16 RFOVFE:1;            /*   RFOVFE     */
                    } BIT;                      /*              */
             } SISTR;                           /*              */
       union {                                  /* SIIER        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TDMAE:1;             /*   TDMAE      */
                    UINT16 :1;                  /*              */
                    UINT16 TFEMPE:1;            /*   TFEMPE     */
                    UINT16 TDREQE:1;            /*   TDREQE     */
                    UINT16 RDMAE:1;             /*   RDMAE      */
                    UINT16 :1;                  /*              */
                    UINT16 RFFULE:1;            /*   RFFULE     */
                    UINT16 RDREQE:1;            /*   RDREQE     */
                    UINT16 :2;                  /*              */
                    UINT16 SAERRE:1;            /*   SASERRE    */
                    UINT16 FSERRE:1;            /*   FSERRE     */
                    UINT16 TFOVFE:1;            /*   TFOVFE     */
                    UINT16 TFUDFE:1;            /*   TFUDFE     */
                    UINT16 RFUDFE:1;            /*   RFUDFE     */
                    UINT16 RFOVFE:1;            /*   RFOVFE     */
                    } BIT;                      /*              */
             } SIIER;                           /*              */
       UINT8 wk3[8];                           /*              */
       union {                                  /* SITDR        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 SITDL:16;            /*   SITDL      */
                    UINT16 SITDR:16;            /*   SITDR      */
                    } BIT;                      /*              */
             } SITDR;                           /*              */
       union {                                  /* SIRDR        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT16 SIRDL:16;            /*   SIRDL      */
                    UINT16 SIRDR:16;            /*   SIRDR      */
                    } BIT;                      /*              */
             } SIRDR;                           /*              */
};
union un_mb3116{                                /* MB31-MB16    */
      UINT16 WORD;                              /*  Word Access */
      struct {                                  /*  Bit Access  */
             UINT16 MB31:1;                     /*   MB31       */
             UINT16 MB30:1;                     /*   MB30       */
             UINT16 MB29:1;                     /*   MB29       */
             UINT16 MB28:1;                     /*   MB28       */
             UINT16 MB27:1;                     /*   MB27       */
             UINT16 MB26:1;                     /*   MB26       */
             UINT16 MB25:1;                     /*   MB25       */
             UINT16 MB24:1;                     /*   MB24       */
             UINT16 MB23:1;                     /*   MB23       */
             UINT16 MB22:1;                     /*   MB22       */
             UINT16 MB21:1;                     /*   MB21       */
             UINT16 MB20:1;                     /*   MB20       */
             UINT16 MB19:1;                     /*   MB19       */
             UINT16 MB18:1;                     /*   MB18       */
             UINT16 MB17:1;                     /*   MB17       */
             UINT16 MB16:1;                     /*   MB16       */
             } BIT;                             /*              */
};
union un_mb15_0{                                /* MB15-MB0     */
      UINT16 WORD;                              /*  Word Access */
      struct {                                  /*  Bit Access  */
             UINT16 MB15:1;                     /*   MB15       */
             UINT16 MB14:1;                     /*   MB14       */
             UINT16 MB13:1;                     /*   MB13       */
             UINT16 MB12:1;                     /*   MB12       */
             UINT16 MB11:1;                     /*   MB11       */
             UINT16 MB10:1;                     /*   MB10       */
             UINT16 MB9:1;                      /*   MB9        */
             UINT16 MB8:1;                      /*   MB8        */
             UINT16 MB7:1;                      /*   MB7        */
             UINT16 MB6:1;                      /*   MB6        */
             UINT16 MB5:1;                      /*   MB5        */
             UINT16 MB4:1;                      /*   MB4        */
             UINT16 MB3:1;                      /*   MB3        */
             UINT16 MB2:1;                      /*   MB2        */
             UINT16 MB1:1;                      /*   MB1        */
             UINT16 MB0:1;                      /*   MB0        */
             } BIT;                             /*              */
};
union un_mb15_1{                                /* MB15-MB1     */
      UINT16 WORD;                              /*  Word Access */
      struct {                                  /*  Bit Access  */
             UINT16 MB15:1;                     /*   MB15       */
             UINT16 MB14:1;                     /*   MB14       */
             UINT16 MB13:1;                     /*   MB13       */
             UINT16 MB12:1;                     /*   MB12       */
             UINT16 MB11:1;                     /*   MB11       */
             UINT16 MB10:1;                     /*   MB10       */
             UINT16 MB9:1;                      /*   MB9        */
             UINT16 MB8:1;                      /*   MB8        */
             UINT16 MB7:1;                      /*   MB7        */
             UINT16 MB6:1;                      /*   MB6        */
             UINT16 MB5:1;                      /*   MB5        */
             UINT16 MB4:1;                      /*   MB4        */
             UINT16 MB3:1;                      /*   MB3        */
             UINT16 MB2:1;                      /*   MB2        */
             UINT16 MB1:1;                      /*   MB1        */
             UINT16 :1;                         /*              */
             } BIT;                             /*              */
};
struct st_rcan {                                /* struct RCAN  */
       union {                                  /* MCR          */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 IDR  :1;             /*   IDR        */
                    UINT16 AHBO :1;             /*   AHBO       */
                    UINT16      :3;             /*              */
                    UINT16 TST  :3;             /*   TST        */
                    UINT16 AWM  :1;             /*   AWM        */
                    UINT16 HTBO :1;             /*   HTBO       */
                    UINT16 SLPM :1;             /*   SLPM       */
                    UINT16      :2;             /*              */
                    UINT16 MTP  :1;             /*   MTP        */
                    UINT16 HLTRQ:1;             /*   HLTRQ      */
                    UINT16 RSTRQ:1;             /*   RSTRQ      */
                    } BIT;                      /*              */
             } MCR;                             /*              */
       union {                                  /* GSR          */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :10;                 /*              */
                    UINT16 EPS:1;               /*   EPS        */
                    UINT16 HSS:1;               /*   HSS        */
                    UINT16 RS:1;                /*   RS         */
                    UINT16 MTPF:1;              /*   MTPF       */
                    UINT16 TRWF:1;              /*   TRWF       */
                    UINT16 BOF:1;               /*   BOF        */
                    } BIT;                      /*              */
             } GSR;                             /*              */
       union {                                  /* BCR1         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TSG1:4;              /*   TSG1       */
                    UINT16 :1;                  /*              */
                    UINT16 TSG2:3;              /*   TSG2       */
                    UINT16 :2;                  /*              */
                    UINT16 SJW:2;               /*   SJW        */
                    UINT16 :3;                  /*              */
                    UINT16 BSP:1;               /*   BSP        */
                    } BIT;                      /*              */
             } BCR1;                            /*              */
       union {                                  /* BCR0         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :8;                  /*              */
                    UINT16 BRP:8;               /*   BRP        */
                    } BIT;                      /*              */
             } BCR0;                            /*              */
       union {                                  /* IRR          */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TCMI1 :1;            /*   TCMI1      */
                    UINT16 TCMI0 :1;            /*   TCMI0      */
                    UINT16 TOI   :1;            /*   TOI        */
                    UINT16 BASMIF:1;            /*   BASMIF     */
                    UINT16 TCMI2 :1;            /*   TCMI2      */
                    UINT16 SNSMI :1;            /*   SNSMI      */
                    UINT16 MOOIF :1;            /*   MOOIF      */
                    UINT16 MBEIF :1;            /*   MBEIF      */
                    UINT16 OLF   :1;            /*   OLF        */
                    UINT16 BOFIF :1;            /*   BOFIF      */
                    UINT16 EPIF  :1;            /*   EPIF       */
                    UINT16 RECWIF:1;            /*   RECWIF     */
                    UINT16 TECWIF:1;            /*   TECWIF     */
                    UINT16 RFRIF :1;            /*   RFRIF      */
                    UINT16 DFRIF :1;            /*   DFRIF      */
                    UINT16 RSTIF :1;            /*   RSTIF      */
                    } BIT;                      /*              */
             } IRR;                             /*              */
       union {                                  /* IMR          */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TCMI1M:1;            /*   TCMI1M     */
                    UINT16 TCMI0M:1;            /*   TCMI0M     */
                    UINT16 TOIM  :1;            /*   TOIM       */
                    UINT16 BASMIM:1;            /*   BASMIM     */
                    UINT16 TCMI2M:1;            /*   TCMI2M     */
                    UINT16 SNSMIM:1;            /*   SNSMIM     */
                    UINT16 MOOIM :1;            /*   MOOIM      */
                    UINT16 MBEIM :1;            /*   MBEIM      */
                    UINT16 OLFM  :1;            /*   OLFM       */
                    UINT16 BOFIM :1;            /*   BOFIM      */
                    UINT16 EPIM  :1;            /*   EPIM       */
                    UINT16 RECWIM:1;            /*   RECWIM     */
                    UINT16 TECWIM:1;            /*   TECWIM     */
                    UINT16 RFRIM :1;            /*   RFRIM      */
                    UINT16 DFRIM :1;            /*   DFRIM      */
                    UINT16 RSTIM :1;            /*   RSTIM      */
                    } BIT;                      /*              */
             } IMR;                             /*              */
       union {                                  /* TEC_REC      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TEC:8;               /*   TEC        */
                    UINT16 REC:8;               /*   REC        */
                    } BIT;                      /*              */
             } TEC_REC  ;                       /*              */
       UINT8 wk0[18];                          /*              */
       union un_mb3116 TXPR1;                   /* TXPR1        */
       union un_mb15_1 TXPR0;                   /* TXPR0        */
       UINT8 wk1[4];                           /*              */
       union un_mb3116 TXCR1;                   /* TXCR1        */
       union un_mb15_1 TXCR0;                   /* TXCR0        */
       UINT8 wk2[4];                           /*              */
       union un_mb3116 TXACK1;                  /* TXACK1       */
       union un_mb15_1 TXACK0;                  /* TXACK0       */
       UINT8 wk3[4];                           /*              */
       union un_mb3116 ABACK1;                  /* ABACK1       */
       union un_mb15_1 ABACK0;                  /* ABACK0       */
       UINT8 wk4[4];                           /*              */
       union un_mb3116 RXPR1;                   /* RXPR1        */
       union un_mb15_0 RXPR0;                   /* RXPR0        */
       UINT8 wk5[4];                           /*              */
       union un_mb3116 RFPR1;                   /* RFPR1        */
       union un_mb15_0 RFPR0;                   /* RFPR0        */
       UINT8 wk6[4];                           /*              */
       union un_mb3116 MBIMR1;                  /* MBIMR1       */
       union un_mb15_0 MBIMR0;                  /* MBIMR0       */
       UINT8 wk7[4];                           /*              */
       union un_mb3116 UMSR1;                   /* UMSR1        */
       union un_mb15_0 UMSR0;                   /* UMSR0        */
       UINT8 wk8[36];                          /*              */
       union {                                  /* TTCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TE:1;                /*   TE         */
                    UINT16 TS:1;                /*   TS         */
                    UINT16 CANC :1;             /*   CANC       */
                    UINT16 CME2:1;              /*   CME2       */
                    UINT16 CME1:1;              /*   CME1       */
                    UINT16 CME0:1;              /*   CME0       */
                    UINT16 :3;                  /*              */
                    UINT16 TCSC:1;              /*   TCSC       */
                    UINT16 TPSC :6;             /*   TPSC       */
                    } BIT;                      /*              */
             } TTCR0;                           /*              */
       UINT8 wk9[2];                           /*              */
       union {                                  /* CMAX_TEW     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :5;                  /*              */
                    UINT16 CMAX:3;              /*   CMAX       */
                    UINT16 :4;                  /*              */
                    UINT16 TEW:4 ;              /*   TEW        */
                    } BIT;                      /*              */
             } CMAX_TEW;                        /*              */
       union {                                  /* RFTROFF      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 RFTROFF:8;           /*   RFTROFF    */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } RFTROFF;                         /*              */
       union {                                  /* TSR          */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :11;                 /*              */
                    UINT16 SNSM:1;              /*   SNSM       */
                    UINT16 TCMF2:1;             /*   TCMF2      */
                    UINT16 TCMF1:1;             /*   TCMF1      */
                    UINT16 TCMF0:1;             /*   TCMF0      */
                    UINT16 TO_NGR_ME:1;         /*   TO_NGR_ME  */
                    } BIT;                      /*              */
             } TSR;                             /*              */
       union {                                  /* CCR          */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :10;                 /*              */
                    UINT16 CCR:6;               /*   CCR        */
                    } BIT;                      /*              */
             } CCR;                             /*              */
       union {                                  /* TCNTR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TCNTR:16;            /*   TCNTR      */
                    } BIT;                      /*              */
             } TCNTR;                           /*              */
       UINT8 wk10[2];                          /*              */
       union {                                  /* CYCTR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 CYCTR:16;            /*   CYCTR      */
                    } BIT;                      /*              */
             } CYCTR  ;                         /*              */
       UINT8 wk11[2];                          /*              */
       union {                                  /* RFMK         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 RFMK:16;             /*   RFMK       */
                    } BIT;                      /*              */
             } RFMK;                            /*              */
       UINT8 wk12[2];                          /*              */
       union {                                  /* TCMR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TCMR0:16;            /*   TCMR0      */
                    } BIT;                      /*              */
             } TCMR0;                           /*              */
       UINT8 wk13[2];                          /*              */
       union {                                  /* TCMR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TCMR1:16;            /*   TCMR1      */
                    } BIT;                      /*              */
             } TCMR1;                           /*              */
       UINT8 wk14[2];                          /*              */
       union {                                  /* TCMR2        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TCMR2:16;            /*   TCMR2      */
                    } BIT;                      /*              */
             } TCMR2;                           /*              */
       UINT8 wk15[2];                          /*              */
       union {                                  /* TTTSEL       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 MB30:1;              /*   MB30       */
                    UINT16 MB29:1;              /*   MB29       */
                    UINT16 MB28:1;              /*   MB28       */
                    UINT16 MB27:1;              /*   MB27       */
                    UINT16 MB26:1;              /*   MB26       */
                    UINT16 MB25:1;              /*   MB25       */
                    UINT16 MB24:1;              /*   MB24       */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } TTTSEL;                          /*              */
       UINT8 wk16[90];                         /*              */
       struct {
             union {                            /* CONTROL0     */
                   UINT32 LONG;                /*  Long Access */
                   struct {                     /*  Word Access */
                          UINT16 H;             /*   High       */
                          UINT16 L;             /*   Low        */
                          } WORD;               /*              */
                   struct {                     /*  Bit Access  */
                          UINT32 IDE:1;        /*   IDE        */
                          UINT32 RTR:1;        /*   RTR        */
                          UINT32 :1;           /*              */
                          UINT32 STDID:11;     /*   STDID      */
                          UINT32 EXTID:18;     /*   EXTID      */
                          } BIT;                /*              */
                   } CONTROL0;                  /*              */
             union {                            /* LAFM         */
                   UINT32 LONG;                /*  Long Access */
                   struct {                     /*  Word Access */
                          UINT16 H;             /*   High       */
                          UINT16 L;             /*   Low        */
                          } WORD;               /*              */
                   struct {                     /*  Bit Access  */
                          UINT32 IDE:1;        /*   IDE        */
                          UINT32 :2;           /*              */
                          UINT32 STDID_LAFM:11;/*   STDID_LAFM */
                          UINT32 EXTID_LAFM:18;/*   EXTID_LAFM */
                          } BIT;                /*              */
                   } LAFM;                      /*              */
             UINT8 MSG_DATA[8];                /* MSG_DATA     */
             union {                            /* CONTROL1     */
                   UINT16 WORD;                 /*  Word Access */
                   struct {                     /*  Byte Access */
                          UINT8 H;             /*   High       */
                          UINT8 L;             /*   Low        */
                          } BYTE;               /*              */
                   struct {                     /*  Bit Access  */
                          UINT16 :2;            /*              */
                          UINT16 NMC:1;         /*   NMC        */
                          UINT16 ATX:1;         /*   ATX        */
                          UINT16 DART:1;        /*   DART       */
                          UINT16 MBC:3;         /*   MBC        */
                          UINT16 :4;            /*              */
                          UINT16 DLC:4;         /*   DLC        */
                          } BIT;                /*              */
                   } CONTROL1;                  /*              */
             UINT16 TIMESTAMP;                  /* TIMESTAMP    */
             UINT16 TTT;                        /* TTT          */
             union {                            /* TTC          */
                   UINT16 WORD;                 /*  Word Access */
                   struct {                     /*  Bit Access  */
                          UINT16 TTW:2;         /*   TTW        */
                          UINT16 Offset:6;      /*   Offset     */
                          UINT16 :5;            /*              */
                          UINT16 rep_factor:3;  /*   rep_factor */
                          } BIT;                /*              */
                   } TTC;                       /*              */
             UINT8 wk[8];                      /*              */
       } MB[32];                                /*              */
};
struct st_ieb {                                 /* struct IEB   */
       union {                                  /* IECTR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 IOL:1;               /*   IOL        */
                    UINT8 DEE:1;               /*   DEE        */
                    UINT8 :1;                  /*              */
                    UINT8 RE:1;                /*   RE         */
                    UINT8 :3;                  /*              */
                    } BIT;                      /*              */
             } IECTR;                           /*              */
       union {                                  /* IECMR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 CMD:3;               /*   CMD        */
                    } BIT;                      /*              */
             } IECMR;                           /*              */
       union {                                  /* IEMCR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 SS:1;                /*   SS         */
                    UINT8 RN:3;                /*   RN         */
                    UINT8 CTL:4;               /*   CTL        */
                    } BIT;                      /*              */
             } IEMCR;                           /*              */
       union {                                  /* IEAR1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IARL4:4;             /*   IARL4      */
                    UINT8 IMD:2;               /*   IMD        */
                    UINT8 :1;                  /*              */
                    UINT8 STE:1;               /*   STE        */
                    } BIT;                      /*              */
             } IEAR1;                           /*              */
       union {                                  /* IEAR2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IARU8:8;             /*   IARU8      */
                    } BIT;                      /*              */
             } IEAR2;                           /*              */
       union {                                  /* IESA1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ISAL4:4;             /*   ISAL4      */
                    UINT8 :4;                  /*              */
                    } BIT;                      /*              */
             } IESA1;                           /*              */
       union {                                  /* IESA2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ISAU8:8;             /*   ISAU8      */
                    } BIT;                      /*              */
             } IESA2;                           /*              */
       union {                                  /* IETBFL       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IBFL:8;              /*   IBFL       */
                    } BIT;                      /*              */
             } IETBFL;                          /*              */
       UINT8 wk0[1];                           /*              */
       union {                                  /* IEMA1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IMAL4:4;             /*   IMAL4      */
                    UINT8 :4;                  /*              */
                    } BIT;                      /*              */
             } IEMA1;                           /*              */
       union {                                  /* IEMA2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 IMAU8:8;             /*   IMAU8      */
                    } BIT;                      /*              */
             } IEMA2;                           /*              */
       union {                                  /* IERCTL       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 RCTL:4;              /*   RCTL       */
                    } BIT;                      /*              */
             } IERCTL;                          /*              */
       union {                                  /* IERBFL       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 RBFL:8;              /*   RBFL       */
                    } BIT;                      /*              */
             } IERBFL;                          /*              */
       UINT8 wk1[1];                           /*              */
       union {                                  /* IELA1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ILAL8:8;             /*   ILAL8      */
                    } BIT;                      /*              */
             } IELA1;                           /*              */
       union {                                  /* IELA2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 ILAU4:4;             /*   ILAU4      */
                    } BIT;                      /*              */
             } IELA2;                           /*              */
       union {                                  /* IEFLG        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 CMX:1;               /*   CMX        */
                    UINT8 MRQ:1;               /*   MRQ        */
                    UINT8 SRQ:1;               /*   SRQ        */
                    UINT8 SRE:1;               /*   SRE        */
                    UINT8 LCK:1;               /*   LCK        */
                    UINT8 :1;                  /*              */
                    UINT8 RSS:1;               /*   RSS        */
                    UINT8 GG:1;                /*   GG         */
                    } BIT;                      /*              */
             } IEFLG;                           /*              */
       union {                                  /* IETSR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 TXS:1;               /*   TXS        */
                    UINT8 TXF:1;               /*   TXF        */
                    UINT8 :1;                  /*              */
                    UINT8 TXEAL:1;             /*   TXEAL      */
                    UINT8 TXETTME:1;           /*   TXETTME    */
                    UINT8 TXERO:1;             /*   TXERO      */
                    UINT8 TXEACK:1;            /*   TXEACK     */
                    } BIT;                      /*              */
             } IETSR;                           /*              */
       union {                                  /* IEIET        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 TXSE:1;              /*   TXSE       */
                    UINT8 TXFE:1;              /*   TXFE       */
                    UINT8 :1;                  /*              */
                    UINT8 TXEALE:1;            /*   TXEALE     */
                    UINT8 TXETTMEE:1;          /*   TXETTMEE   */
                    UINT8 TXEROE:1;            /*   TXEROE     */
                    UINT8 TXEACKE:1;           /*   TXEACKE    */
                    } BIT;                      /*              */
             } IEIET;                           /*              */
       UINT8 wk2[1];                           /*              */
       union {                                  /* IERSR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 RXBSY:1;             /*   RXBSY      */
                    UINT8 RXS:1;               /*   RXS        */
                    UINT8 RXF:1;               /*   RXF        */
                    UINT8 RXEDE:1;             /*   RXEDE      */
                    UINT8 RXEOVE:1;            /*   RXEOVE     */
                    UINT8 RXERTME:1;           /*   RXERTME    */
                    UINT8 RXEDLE:1;            /*   RXEDLE     */
                    UINT8 RXEPE:1;             /*   RXEPE      */
                    } BIT;                      /*              */
             } IERSR;                           /*              */
       union {                                  /* IEIER        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 RXBSYE:1;            /*   RXBSYE     */
                    UINT8 RXSE:1;              /*   RXSE       */
                    UINT8 RXFE:1;              /*   RXFE       */
                    UINT8 RXEDEE:1;            /*   RXEDEE     */
                    UINT8 RXEOVEE:1;           /*   RXEOVEE    */
                    UINT8 RXERTMEE:1;          /*   RXERTMEE   */
                    UINT8 RXEDLEE:1;           /*   RXEDLEE    */
                    UINT8 RXEPEE:1;            /*   RXEPEE     */
                    } BIT;                      /*              */
             } IEIER;                           /*              */
       UINT8 wk3[2];                           /*              */
       union {                                  /* IECKSR       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 CKS3:1;              /*   CKS3       */
                    UINT8 :1;                  /*              */
                    UINT8 CKS:3;               /*   CKS        */
                    } BIT;                      /*              */
             } IECKSR;                          /*              */
       UINT8 wk4[231];                         /*              */
       UINT8 IETB001;                          /* IETB001      */
       UINT8 IETB002;                          /* IETB002      */
       UINT8 IETB003;                          /* IETB003      */
       UINT8 IETB004;                          /* IETB004      */
       UINT8 IETB005;                          /* IETB005      */
       UINT8 IETB006;                          /* IETB006      */
       UINT8 IETB007;                          /* IETB007      */
       UINT8 IETB008;                          /* IETB008      */
       UINT8 IETB009;                          /* IETB009      */
       UINT8 IETB010;                          /* IETB010      */
       UINT8 IETB011;                          /* IETB011      */
       UINT8 IETB012;                          /* IETB012      */
       UINT8 IETB013;                          /* IETB013      */
       UINT8 IETB014;                          /* IETB014      */
       UINT8 IETB015;                          /* IETB015      */
       UINT8 IETB016;                          /* IETB016      */
       UINT8 IETB017;                          /* IETB017      */
       UINT8 IETB018;                          /* IETB018      */
       UINT8 IETB019;                          /* IETB019      */
       UINT8 IETB020;                          /* IETB020      */
       UINT8 IETB021;                          /* IETB021      */
       UINT8 IETB022;                          /* IETB022      */
       UINT8 IETB023;                          /* IETB023      */
       UINT8 IETB024;                          /* IETB024      */
       UINT8 IETB025;                          /* IETB025      */
       UINT8 IETB026;                          /* IETB026      */
       UINT8 IETB027;                          /* IETB027      */
       UINT8 IETB028;                          /* IETB028      */
       UINT8 IETB029;                          /* IETB029      */
       UINT8 IETB030;                          /* IETB030      */
       UINT8 IETB031;                          /* IETB031      */
       UINT8 IETB032;                          /* IETB032      */
       UINT8 IETB033;                          /* IETB033      */
       UINT8 IETB034;                          /* IETB034      */
       UINT8 IETB035;                          /* IETB035      */
       UINT8 IETB036;                          /* IETB036      */
       UINT8 IETB037;                          /* IETB037      */
       UINT8 IETB038;                          /* IETB038      */
       UINT8 IETB039;                          /* IETB039      */
       UINT8 IETB040;                          /* IETB040      */
       UINT8 IETB041;                          /* IETB041      */
       UINT8 IETB042;                          /* IETB042      */
       UINT8 IETB043;                          /* IETB043      */
       UINT8 IETB044;                          /* IETB044      */
       UINT8 IETB045;                          /* IETB045      */
       UINT8 IETB046;                          /* IETB046      */
       UINT8 IETB047;                          /* IETB047      */
       UINT8 IETB048;                          /* IETB048      */
       UINT8 IETB049;                          /* IETB049      */
       UINT8 IETB050;                          /* IETB050      */
       UINT8 IETB051;                          /* IETB051      */
       UINT8 IETB052;                          /* IETB052      */
       UINT8 IETB053;                          /* IETB053      */
       UINT8 IETB054;                          /* IETB054      */
       UINT8 IETB055;                          /* IETB055      */
       UINT8 IETB056;                          /* IETB056      */
       UINT8 IETB057;                          /* IETB057      */
       UINT8 IETB058;                          /* IETB058      */
       UINT8 IETB059;                          /* IETB059      */
       UINT8 IETB060;                          /* IETB060      */
       UINT8 IETB061;                          /* IETB061      */
       UINT8 IETB062;                          /* IETB062      */
       UINT8 IETB063;                          /* IETB063      */
       UINT8 IETB064;                          /* IETB064      */
       UINT8 IETB065;                          /* IETB065      */
       UINT8 IETB066;                          /* IETB066      */
       UINT8 IETB067;                          /* IETB067      */
       UINT8 IETB068;                          /* IETB068      */
       UINT8 IETB069;                          /* IETB069      */
       UINT8 IETB070;                          /* IETB070      */
       UINT8 IETB071;                          /* IETB071      */
       UINT8 IETB072;                          /* IETB072      */
       UINT8 IETB073;                          /* IETB073      */
       UINT8 IETB074;                          /* IETB074      */
       UINT8 IETB075;                          /* IETB075      */
       UINT8 IETB076;                          /* IETB076      */
       UINT8 IETB077;                          /* IETB077      */
       UINT8 IETB078;                          /* IETB078      */
       UINT8 IETB079;                          /* IETB079      */
       UINT8 IETB080;                          /* IETB080      */
       UINT8 IETB081;                          /* IETB081      */
       UINT8 IETB082;                          /* IETB082      */
       UINT8 IETB083;                          /* IETB083      */
       UINT8 IETB084;                          /* IETB084      */
       UINT8 IETB085;                          /* IETB085      */
       UINT8 IETB086;                          /* IETB086      */
       UINT8 IETB087;                          /* IETB087      */
       UINT8 IETB088;                          /* IETB088      */
       UINT8 IETB089;                          /* IETB089      */
       UINT8 IETB090;                          /* IETB090      */
       UINT8 IETB091;                          /* IETB091      */
       UINT8 IETB092;                          /* IETB092      */
       UINT8 IETB093;                          /* IETB093      */
       UINT8 IETB094;                          /* IETB094      */
       UINT8 IETB095;                          /* IETB095      */
       UINT8 IETB096;                          /* IETB096      */
       UINT8 IETB097;                          /* IETB097      */
       UINT8 IETB098;                          /* IETB098      */
       UINT8 IETB099;                          /* IETB099      */
       UINT8 IETB100;                          /* IETB100      */
       UINT8 IETB101;                          /* IETB101      */
       UINT8 IETB102;                          /* IETB102      */
       UINT8 IETB103;                          /* IETB103      */
       UINT8 IETB104;                          /* IETB104      */
       UINT8 IETB105;                          /* IETB105      */
       UINT8 IETB106;                          /* IETB106      */
       UINT8 IETB107;                          /* IETB107      */
       UINT8 IETB108;                          /* IETB108      */
       UINT8 IETB109;                          /* IETB109      */
       UINT8 IETB110;                          /* IETB110      */
       UINT8 IETB111;                          /* IETB111      */
       UINT8 IETB112;                          /* IETB112      */
       UINT8 IETB113;                          /* IETB113      */
       UINT8 IETB114;                          /* IETB114      */
       UINT8 IETB115;                          /* IETB115      */
       UINT8 IETB116;                          /* IETB116      */
       UINT8 IETB117;                          /* IETB117      */
       UINT8 IETB118;                          /* IETB118      */
       UINT8 IETB119;                          /* IETB119      */
       UINT8 IETB120;                          /* IETB120      */
       UINT8 IETB121;                          /* IETB121      */
       UINT8 IETB122;                          /* IETB122      */
       UINT8 IETB123;                          /* IETB123      */
       UINT8 IETB124;                          /* IETB124      */
       UINT8 IETB125;                          /* IETB125      */
       UINT8 IETB126;                          /* IETB126      */
       UINT8 IETB127;                          /* IETB127      */
       UINT8 IETB128;                          /* IETB128      */
       UINT8 wk5[128];                         /*              */
       UINT8 IERB001;                          /* IERB001      */
       UINT8 IERB002;                          /* IERB002      */
       UINT8 IERB003;                          /* IERB003      */
       UINT8 IERB004;                          /* IERB004      */
       UINT8 IERB005;                          /* IERB005      */
       UINT8 IERB006;                          /* IERB006      */
       UINT8 IERB007;                          /* IERB007      */
       UINT8 IERB008;                          /* IERB008      */
       UINT8 IERB009;                          /* IERB009      */
       UINT8 IERB010;                          /* IERB010      */
       UINT8 IERB011;                          /* IERB011      */
       UINT8 IERB012;                          /* IERB012      */
       UINT8 IERB013;                          /* IERB013      */
       UINT8 IERB014;                          /* IERB014      */
       UINT8 IERB015;                          /* IERB015      */
       UINT8 IERB016;                          /* IERB016      */
       UINT8 IERB017;                          /* IERB017      */
       UINT8 IERB018;                          /* IERB018      */
       UINT8 IERB019;                          /* IERB019      */
       UINT8 IERB020;                          /* IERB020      */
       UINT8 IERB021;                          /* IERB021      */
       UINT8 IERB022;                          /* IERB022      */
       UINT8 IERB023;                          /* IERB023      */
       UINT8 IERB024;                          /* IERB024      */
       UINT8 IERB025;                          /* IERB025      */
       UINT8 IERB026;                          /* IERB026      */
       UINT8 IERB027;                          /* IERB027      */
       UINT8 IERB028;                          /* IERB028      */
       UINT8 IERB029;                          /* IERB029      */
       UINT8 IERB030;                          /* IERB030      */
       UINT8 IERB031;                          /* IERB031      */
       UINT8 IERB032;                          /* IERB032      */
       UINT8 IERB033;                          /* IERB033      */
       UINT8 IERB034;                          /* IERB034      */
       UINT8 IERB035;                          /* IERB035      */
       UINT8 IERB036;                          /* IERB036      */
       UINT8 IERB037;                          /* IERB037      */
       UINT8 IERB038;                          /* IERB038      */
       UINT8 IERB039;                          /* IERB039      */
       UINT8 IERB040;                          /* IERB040      */
       UINT8 IERB041;                          /* IERB041      */
       UINT8 IERB042;                          /* IERB042      */
       UINT8 IERB043;                          /* IERB043      */
       UINT8 IERB044;                          /* IERB044      */
       UINT8 IERB045;                          /* IERB045      */
       UINT8 IERB046;                          /* IERB046      */
       UINT8 IERB047;                          /* IERB047      */
       UINT8 IERB048;                          /* IERB048      */
       UINT8 IERB049;                          /* IERB049      */
       UINT8 IERB050;                          /* IERB050      */
       UINT8 IERB051;                          /* IERB051      */
       UINT8 IERB052;                          /* IERB052      */
       UINT8 IERB053;                          /* IERB053      */
       UINT8 IERB054;                          /* IERB054      */
       UINT8 IERB055;                          /* IERB055      */
       UINT8 IERB056;                          /* IERB056      */
       UINT8 IERB057;                          /* IERB057      */
       UINT8 IERB058;                          /* IERB058      */
       UINT8 IERB059;                          /* IERB059      */
       UINT8 IERB060;                          /* IERB060      */
       UINT8 IERB061;                          /* IERB061      */
       UINT8 IERB062;                          /* IERB062      */
       UINT8 IERB063;                          /* IERB063      */
       UINT8 IERB064;                          /* IERB064      */
       UINT8 IERB065;                          /* IERB065      */
       UINT8 IERB066;                          /* IERB066      */
       UINT8 IERB067;                          /* IERB067      */
       UINT8 IERB068;                          /* IERB068      */
       UINT8 IERB069;                          /* IERB069      */
       UINT8 IERB070;                          /* IERB070      */
       UINT8 IERB071;                          /* IERB071      */
       UINT8 IERB072;                          /* IERB072      */
       UINT8 IERB073;                          /* IERB073      */
       UINT8 IERB074;                          /* IERB074      */
       UINT8 IERB075;                          /* IERB075      */
       UINT8 IERB076;                          /* IERB076      */
       UINT8 IERB077;                          /* IERB077      */
       UINT8 IERB078;                          /* IERB078      */
       UINT8 IERB079;                          /* IERB079      */
       UINT8 IERB080;                          /* IERB080      */
       UINT8 IERB081;                          /* IERB081      */
       UINT8 IERB082;                          /* IERB082      */
       UINT8 IERB083;                          /* IERB083      */
       UINT8 IERB084;                          /* IERB084      */
       UINT8 IERB085;                          /* IERB085      */
       UINT8 IERB086;                          /* IERB086      */
       UINT8 IERB087;                          /* IERB087      */
       UINT8 IERB088;                          /* IERB088      */
       UINT8 IERB089;                          /* IERB089      */
       UINT8 IERB090;                          /* IERB090      */
       UINT8 IERB091;                          /* IERB091      */
       UINT8 IERB092;                          /* IERB092      */
       UINT8 IERB093;                          /* IERB093      */
       UINT8 IERB094;                          /* IERB094      */
       UINT8 IERB095;                          /* IERB095      */
       UINT8 IERB096;                          /* IERB096      */
       UINT8 IERB097;                          /* IERB097      */
       UINT8 IERB098;                          /* IERB098      */
       UINT8 IERB099;                          /* IERB099      */
       UINT8 IERB100;                          /* IERB100      */
       UINT8 IERB101;                          /* IERB101      */
       UINT8 IERB102;                          /* IERB102      */
       UINT8 IERB103;                          /* IERB103      */
       UINT8 IERB104;                          /* IERB104      */
       UINT8 IERB105;                          /* IERB105      */
       UINT8 IERB106;                          /* IERB106      */
       UINT8 IERB107;                          /* IERB107      */
       UINT8 IERB108;                          /* IERB108      */
       UINT8 IERB109;                          /* IERB109      */
       UINT8 IERB110;                          /* IERB110      */
       UINT8 IERB111;                          /* IERB111      */
       UINT8 IERB112;                          /* IERB112      */
       UINT8 IERB113;                          /* IERB113      */
       UINT8 IERB114;                          /* IERB114      */
       UINT8 IERB115;                          /* IERB115      */
       UINT8 IERB116;                          /* IERB116      */
       UINT8 IERB117;                          /* IERB117      */
       UINT8 IERB118;                          /* IERB118      */
       UINT8 IERB119;                          /* IERB119      */
       UINT8 IERB120;                          /* IERB120      */
       UINT8 IERB121;                          /* IERB121      */
       UINT8 IERB122;                          /* IERB122      */
       UINT8 IERB123;                          /* IERB123      */
       UINT8 IERB124;                          /* IERB124      */
       UINT8 IERB125;                          /* IERB125      */
       UINT8 IERB126;                          /* IERB126      */
       UINT8 IERB127;                          /* IERB127      */
       UINT8 IERB128;                          /* IERB128      */
};                                              /*              */
struct st_spdif {                               /* struct SPDIF */
       UINT32 TLCA;                            /* TLCA         */
       UINT32 TRCA;                            /* TRCA         */
       union {                                  /* TLCS         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :2;                 /*              */
                    UINT32 CLAC:2;             /*   CLAC       */
                    UINT32 FS:4;               /*   FS         */
                    UINT32 CHNO:4;             /*   CHNO       */
                    UINT32 SRCNO:4;            /*   SRCNO      */
                    UINT32 CATCD:8;            /*   CATCD      */
                    UINT32 :2;                 /*              */
                    UINT32 CTL:5;              /*   CTL        */
                    UINT32 :1;                 /*              */
                    } BIT;                      /*              */
             } TLCS;                            /*              */
       union {                                  /* TRCS         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :2;                 /*              */
                    UINT32 CLAC:2;             /*   CLAC       */
                    UINT32 FS:4;               /*   FS         */
                    UINT32 CHNO:4;             /*   CHNO       */
                    UINT32 SRCNO:4;            /*   SRCNO      */
                    UINT32 CATCD:8;            /*   CATCD      */
                    UINT32 :2;                 /*              */
                    UINT32 CTL:5;              /*   CTL        */
                    UINT32 :1;                 /*              */
                    } BIT;                      /*              */
             } TRCS;                            /*              */
       UINT32 TUI;                             /* TUI          */
       UINT32 RLCA;                            /* RLCA         */
       UINT32 RRCA;                            /* RRCA         */
       union {                                  /* RLCS         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :2;                 /*              */
                    UINT32 CLAC:2;             /*   CLAC       */
                    UINT32 FS:4;               /*   FS         */
                    UINT32 CHNO:4;             /*   CHNO       */
                    UINT32 SRCNO:4;            /*   SRCNO      */
                    UINT32 CATCD:8;            /*   CATCD      */
                    UINT32 :2;                 /*              */
                    UINT32 CTL:5;              /*   CTL        */
                    UINT32 :1;                 /*              */
                    } BIT;                      /*              */
             } RLCS;                            /*              */
       union {                                  /* RRCS         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :2;                 /*              */
                    UINT32 CLAC:2;             /*   CLAC       */
                    UINT32 FS:4;               /*   FS         */
                    UINT32 CHNO:4;             /*   CHNO       */
                    UINT32 SRCNO:4;            /*   SRCNO      */
                    UINT32 CATCD:8;            /*   CATCD      */
                    UINT32 :2;                 /*              */
                    UINT32 CTL:5;              /*   CTL        */
                    UINT32 :1;                 /*              */
                    } BIT;                      /*              */
             } RRCS;                            /*              */
       UINT32 RUI;                             /* RUI          */
       union {                                  /* CTRL         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :3;                 /*              */
                    UINT32 CKS:1;              /*   CKS        */
                    UINT32 :1;                 /*              */
                    UINT32 PB:1;               /*   PB         */
                    UINT32 RASS:2;             /*   RASS       */
                    UINT32 TASS:2;             /*   TASS       */
                    UINT32 RDE:1;              /*   RDE        */
                    UINT32 TDE:1;              /*   TDE        */
                    UINT32 NCSI:1;             /*   NCSI       */
                    UINT32 AOS:1;              /*   AOS        */
                    UINT32 RME:1;              /*   RME        */
                    UINT32 TME:1;              /*   TME        */
                    UINT32 REIE:1;             /*   REIE       */
                    UINT32 TEIE:1;             /*   TEIE       */
                    UINT32 UBOI:1;             /*   UBOI       */
                    UINT32 UBUI:1;             /*   UBUI       */
                    UINT32 CREI:1;             /*   CREI       */
                    UINT32 PAEI:1;             /*   PAEI       */
                    UINT32 PREI:1;             /*   PREI       */
                    UINT32 CSEI:1;             /*   CSEI       */
                    UINT32 ABOI:1;             /*   ABOI       */
                    UINT32 ABUI:1;             /*   ABUI       */
                    UINT32 RUII:1;             /*   RUII       */
                    UINT32 TUII:1;             /*   TUII       */
                    UINT32 RCSI:1;             /*   RCSI       */
                    UINT32 RCBI:1;             /*   RCBI       */
                    UINT32 TCSI:1;             /*   TCSI       */
                    UINT32 TCBI:1;             /*   TCBI       */
                    } BIT;                      /*              */
             } CTRL;                            /*              */
       union {                                  /* STAT         */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :15;                /*              */
                    UINT32 CMD:1;              /*   CMD        */
                    UINT32 RIS:1;              /*   RIS        */
                    UINT32 TIS:1;              /*   TIS        */
                    UINT32 UBO:1;              /*   UBO        */
                    UINT32 UBU:1;              /*   UBU        */
                    UINT32 CE:1;               /*   CE         */
                    UINT32 PARE:1;             /*   PARE       */
                    UINT32 PREE:1;             /*   PREE       */
                    UINT32 CSE:1;              /*   CSE        */
                    UINT32 ABO:1;              /*   ABO        */
                    UINT32 ABU:1;              /*   ABU        */
                    UINT32 RUIR:1;             /*   RUIR       */
                    UINT32 TUIR:1;             /*   TUIR       */
                    UINT32 CSRX:1;             /*   CSRX       */
                    UINT32 CBRX:1;             /*   CBRX       */
                    UINT32 CSTX:1;             /*   CSTX       */
                    UINT32 CBTX:1;             /*   CBTX       */
                    } BIT;                      /*              */
             } STAT;                            /*              */
       UINT32 TDAD;                            /* TDAD         */
       UINT32 RDAD;                            /* RDAD         */
};                                              /*              */
struct st_romdec {                              /* struct ROMDEC */
       union {                                  /* CROMEN       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 SUBC_EN:1;           /*   SUBC_EN    */
                    UINT8 CROM_EN:1;           /*   CROM_EN    */
                    UINT8 CROM_STP:1;          /*   CROM_STP   */
                    UINT8 :5;                  /*              */
                    } BIT;                      /*              */
             } CROMEN;                          /*              */
       union {                                  /* CROMSY0      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 SY_AUT:1;            /*   SY_AUT     */
                    UINT8 SY_IEN:1;            /*   SY_IEN     */
                    UINT8 SY_DEN:1;            /*   SY_DEN     */
                    UINT8 :5;                  /*              */
                    } BIT;                      /*              */
             } CROMSY0;                         /*              */
       union {                                  /* CROMCTL0     */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 MD_DESC:1;           /*   MD_DESC    */
                    UINT8 :1;                  /*              */
                    UINT8 MD_AUTO:1;           /*   MD_AUTO    */
                    UINT8 MD_AUTOS1:1;         /*   MD_AUTOS1  */
                    UINT8 MD_AUTOS2:1;         /*   MD_AUTOS2  */
                    UINT8 MD_SEC:3;            /*   MD_SEC     */
                    } BIT;                      /*              */
             } CROMCTL0;                        /*              */
       union {                                  /* CROMCTL1     */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 M2F2EDC:1;           /*   M2F2EDC    */
                    UINT8 MD_DEC:3;            /*   MD_DEC     */
                    UINT8 :2;                  /*              */
                    UINT8 MD_PQREP:2;          /*   MD_PQREP   */
                    } BIT;                      /*              */
             } CROMCTL1;                        /*              */
       UINT8 wk0[1];                           /*              */
       union {                                  /* CROMCTL3     */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 STP_ECC:1;           /*   STP_ECC    */
                    UINT8 STP_EDC:1;           /*   STP_EDC    */
                    UINT8 :1;                  /*              */
                    UINT8 STP_MD:1;            /*   STP_MD     */
                    UINT8 STP_MIN:1;           /*   STP_MIN    */
                    UINT8 :3;                  /*              */
                    } BIT;                      /*              */
             } CROMCTL3;                        /*              */
       union {                                  /* CROMCTL4     */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 LINKOFF:1;           /*   LINKOFF    */
                    UINT8 LINK2:1;             /*   LINK2      */
                    UINT8 :1;                  /*              */
                    UINT8 EROSEL:1;            /*   EROSEL     */
                    UINT8 NO_ECC:1;            /*   NO_ECC     */
                    UINT8 :3;                  /*              */
                    } BIT;                      /*              */
             } CROMCTL4;                        /*              */
       union {                                  /* CROMCTL5     */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 MSF_LBA_SEL:1;       /*   MSF_LBA_SEL */
                    } BIT;                      /*              */
             } CROMCTL5;                        /*              */
       union {                                  /* CROMST0      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 ST_SYIL:1;           /*   ST_SYIL    */
                    UINT8 ST_SYNO:1;           /*   ST_SYNO    */
                    UINT8 ST_BLKS:1;           /*   ST_BLKS    */
                    UINT8 ST_BLKL:1;           /*   ST_BLKL    */
                    UINT8 ST_SECS:1;           /*   ST_SECS    */
                    UINT8 ST_SECL:1;           /*   ST_SECL    */
                    } BIT;                      /*              */
             } CROMST0;                         /*              */
       union {                                  /* CROMST1      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 ER2_HEAD0:1;         /*   ER2_HEAD0  */
                    UINT8 ER2_HEAD1:1;         /*   ER2_HEAD1  */
                    UINT8 ER2_HEAD2:1;         /*   ER2_HEAD2  */
                    UINT8 ER2_HEAD3:1;         /*   ER2_HEAD3  */
                    } BIT;                      /*              */
             } CROMST1;                         /*              */
       UINT8 wk1[1];                           /*              */
       union {                                  /* CROMST3      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ER2_SHEAD0:1;        /*   ER2_SHEAD0 */
                    UINT8 ER2_SHEAD1:1;        /*   ER2_SHEAD1 */
                    UINT8 ER2_SHEAD2:1;        /*   ER2_SHEAD2 */
                    UINT8 ER2_SHEAD3:1;        /*   ER2_SHEAD3 */
                    UINT8 ER2_SHEAD4:1;        /*   ER2_SHEAD4 */
                    UINT8 ER2_SHEAD5:1;        /*   ER2_SHEAD5 */
                    UINT8 ER2_SHEAD6:1;        /*   ER2_SHEAD6 */
                    UINT8 ER2_SHEAD7:1;        /*   ER2_SHEAD7 */
                    } BIT;                      /*              */
             } CROMST3;                         /*              */
       union {                                  /* CROMST4      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 NG_MD:1;             /*   NG_MD      */
                    UINT8 NG_MDCMP1:1;         /*   NG_MDCMP1  */
                    UINT8 NG_MDCMP2:1;         /*   NG_MDCMP2  */
                    UINT8 NG_MDCMP3:1;         /*   NG_MDCMP3  */
                    UINT8 NG_MDCMP4:1;         /*   NG_MDCMP4  */
                    UINT8 NG_MDDEF:1;          /*   NG_MDDEF   */
                    UINT8 NG_MDTIM1:1;         /*   NG_MDTIM1  */
                    UINT8 NG_MDTIM2:1;         /*   NG_MDTIM2  */
                    } BIT;                      /*              */
             } CROMST4;                         /*              */
       union {                                  /* CROMST5      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ST_AMD:3;            /*   ST_AMD     */
                    UINT8 ST_MDX:1;            /*   ST_MDX     */
                    UINT8 LINK_ON:1;           /*   LINK_ON    */
                    UINT8 LINK_DET:1;          /*   LINK_DET   */
                    UINT8 LINK_SDET:1;         /*   LINK_SDET  */
                    UINT8 LINK_OUT1:1;         /*   LINK_OUT1  */
                    } BIT;                      /*              */
             } CROMST5;                         /*              */
       union {                                  /* CROMST6      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ST_ERR:1;            /*   ST_ERR     */
                    UINT8 :1;                  /*              */
                    UINT8 ST_ECCABT:1;         /*   ST_ECCABT  */
                    UINT8 ST_ECCNG:1;          /*   ST_ECCNG   */
                    UINT8 ST_ECCP:1;           /*   ST_ECCP    */
                    UINT8 ST_ECCQ:1;           /*   ST_ECCQ    */
                    UINT8 ST_EDC1:1;           /*   ST_EDC1    */
                    UINT8 ST_EDC2:1;           /*   ST_EDC2    */
                    } BIT;                      /*              */
             } CROMST6;                         /*              */
       UINT8 wk2[5];                           /*              */
       union {                                  /* CBUFST0      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 BUF_REF:1;           /*   BUF_REF    */
                    UINT8 BUF_ACT:1;           /*   BUF_ACT    */
                    UINT8 :6;                  /*              */
                    } BIT;                      /*              */
             } CBUFST0;                         /*              */
       union {                                  /* CBUFST1      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 BUF_ECC:1;           /*   BUF_ECC    */
                    UINT8 BUF_EDC:1;           /*   BUF_EDC    */
                    UINT8 :1;                  /*              */
                    UINT8 BUF_MD:1;            /*   BUF_MD     */
                    UINT8 BUF_MIN:1;           /*   BUF_MIN    */
                    UINT8 :3;                  /*              */
                    } BIT;                      /*              */
             } CBUFST1;                         /*              */
       union {                                  /* CBUFST2      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 BUF_NG:1;            /*   BUF_NG     */
                    UINT8 :7;                  /*              */
                    } BIT;                      /*              */
             } CBUFST2;                         /*              */
       UINT8 wk3[1];                           /*              */
       UINT8 HEAD00;                           /* HEAD00       */
       UINT8 HEAD01;                           /* HEAD01       */
       UINT8 HEAD02;                           /* HEAD02       */
       UINT8 HEAD03;                           /* HEAD03       */
       UINT8 SHEAD00;                          /* SHEAD00      */
       UINT8 SHEAD01;                          /* SHEAD01      */
       UINT8 SHEAD02;                          /* SHEAD02      */
       UINT8 SHEAD03;                          /* SHEAD03      */
       UINT8 SHEAD04;                          /* SHEAD04      */
       UINT8 SHEAD05;                          /* SHEAD05      */
       UINT8 SHEAD06;                          /* SHEAD06      */
       UINT8 SHEAD07;                          /* SHEAD07      */
       UINT8 HEAD20;                           /* HEAD20       */
       UINT8 HEAD21;                           /* HEAD21       */
       UINT8 HEAD22;                           /* HEAD22       */
       UINT8 HEAD23;                           /* HEAD23       */
       UINT8 SHEAD20;                          /* SHEAD20      */
       UINT8 SHEAD21;                          /* SHEAD21      */
       UINT8 SHEAD22;                          /* SHEAD22      */
       UINT8 SHEAD23;                          /* SHEAD23      */
       UINT8 SHEAD24;                          /* SHEAD24      */
       UINT8 SHEAD25;                          /* SHEAD25      */
       UINT8 SHEAD26;                          /* SHEAD26      */
       UINT8 SHEAD27;                          /* SHEAD27      */       
       UINT8 wk4[16];                          /*              */
       union {                                  /* CBUFCTL0     */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 CBUF_AUT:1;          /*   CBUF_AUT   */
                    UINT8 CBUF_EN:1;           /*   CBUF_EN    */
                    UINT8 CBUF_LINK:1;         /*   CBUF_LINK  */
                    UINT8 CBUF_MD:2;           /*   CBUF_MD    */
                    UINT8 CBUF_TS:1;           /*   CBUF_TS    */
                    UINT8 CBUF_Q:1;            /*   CBUF_Q     */
                    UINT8 :1;                  /*              */
                    } BIT;                      /*              */
             } CBUFCTL0;                        /*              */
       UINT8 CBUFCTL1;                         /* CBUFCTL1     */
       UINT8 CBUFCTL2;                         /* CBUFCTL2     */
       UINT8 CBUFCTL3;                         /* CBUFCTL3     */
       UINT8 wk5[1];                           /*              */
       union {                                  /* CROMST0M     */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 ST_SYILM:1;          /*   ST_SYILM   */
                    UINT8 ST_SYNOM:1;          /*   ST_SYNOM   */
                    UINT8 ST_BLKSM:1;          /*   ST_BLKSM   */
                    UINT8 ST_BLKLM:1;          /*   ST_BLKLM   */
                    UINT8 ST_SECSM:1;          /*   ST_SECSM   */
                    UINT8 ST_SECLM:1;          /*   ST_SECLM   */
                    } BIT;                      /*              */
             } CROMST0M;                        /*              */
       UINT8 wk6[186];                         /*              */
       union {                                  /* ROMDECRST    */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 LOGICRST:1;          /*   LOGICRST   */
                    UINT8 RAMRST:1;            /*   RAMRST     */
                    UINT8 :6;                  /*              */
                    } BIT;                      /*              */
             } ROMDECRST;                       /*              */
       union {                                  /* RSTSTAT      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 RAMCLRST:1;          /*   RAMCLRST   */
                    UINT8 :7;                  /*              */
                    } BIT;                      /*              */
             } RSTSTAT;                         /*              */
       union {                                  /* SSI          */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 BYTEND:1;            /*   BYTEND     */
                    UINT8 BITEND:1;            /*   BITEND     */
                    UINT8 BUFEND0:2;           /*   BUFEND0    */
                    UINT8 BUFEND1:2;           /*   BUFEND1    */
                    UINT8 :2;                  /*              */
                    } BIT;                      /*              */
             } SSI;                             /*              */
       UINT8 wk7[5];                           /*              */
       union {                                  /* INTHOLD      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 ISEC:1;              /*   ISEC       */
                    UINT8 ITARG:1;             /*   ITARG      */
                    UINT8 ISY:1;               /*   ISY        */
                    UINT8 IERR:1;              /*   IERR       */
                    UINT8 IBUF:1;              /*   IBUF       */
                    UINT8 IREADY:1;            /*   IREADY     */
                    UINT8 :2;                  /*              */
                    } BIT;                      /*              */
             } INTHOLD;                         /*              */
       union {                                  /* INHINT       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 INHISEC:1;           /*   INHISEC    */
                    UINT8 INHITARG:1;          /*   INHITARG   */
                    UINT8 INHISY:1;            /*   INHISY     */
                    UINT8 INHIERR:1;           /*   INHIERR    */
                    UINT8 INHIBUF:1;           /*   INHIBUF    */
                    UINT8 INHIREADY:1;         /*   INHIREADY  */
                    UINT8 PREINHREQDM:1;       /*   PREINHREQDM */
                    UINT8 PREINHIREADY:1;      /*   PREINHIREADY */
                    } BIT;                      /*              */
             } INHINT;                          /*              */
       UINT8 wk8[246];                         /*              */
       UINT32 STRMDIN;                         /* STRMDIN      */
       UINT16 STRMDOUT;                         /* STRMDOUT     */
};                                              /*              */
struct st_adc {                                 /* struct ADC   */
       UINT16 ADDRA;                            /* ADDRA        */
       UINT16 ADDRB;                            /* ADDRB        */
       UINT16 ADDRC;                            /* ADDRC        */
       UINT16 ADDRD;                            /* ADDRD        */
       UINT16 ADDRE;                            /* ADDRE        */
       UINT16 ADDRF;                            /* ADDRF        */
       UINT16 ADDRG;                            /* ADDRG        */
       UINT16 ADDRH;                            /* ADDRH        */
       UINT8 wk0[16];                          /*              */
       union {                                  /* ADCSR        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 ADF:1;               /*   ADF        */
                    UINT16 ADIE:1;              /*   ADIE       */
                    UINT16 ADST:1;              /*   ADST       */
                    UINT16 TRGS:4;              /*   TRGS       */
                    UINT16 CKS:3;               /*   CKS        */
                    UINT16 MDS:3;               /*   MDS        */
                    UINT16 CH:3;                /*   CH         */
                    } BIT;                      /*              */
             } ADCSR;                           /*              */
};                                              /*              */
struct st_flctl {                               /* struct FLCTL */
       union {                                  /* FLCMNCR      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 ECCPOS2:1;          /*   ECCPOS[2]  */
                    UINT32 _4ECCCNTEN:1;       /*   _4ECCCNTEN */
                    UINT32 _4ECCEN:1;          /*   _4ECCEN    */
                    UINT32 _4ECCCORRECT:1;     /*   _4ECCCORRECT */
                    UINT32 BUSYON:1;           /*   BUSYON     */
                    UINT32 :2;                 /*              */
                    UINT32 SNAND:1;            /*   SNAND      */
                    UINT32 QTSEL:1;            /*   QTSEL      */
                    UINT32 :1;                 /*              */
                    UINT32 FCKSEL:1;           /*   FCKSEL     */
                    UINT32 :1;                 /*              */
                    UINT32 ECCPOS:2;           /*   ECCPOS     */
                    UINT32 ACM:2;              /*   ACM        */
                    UINT32 NANDWF:1;           /*   NANDWF     */
                    UINT32 :5;                 /*              */
                    UINT32 CE:1;               /*   CE         */
                    UINT32 :2;                 /*              */
                    UINT32 :1;                 /*              */
                    } BIT;                      /*              */
             } FLCMNCR;                         /*              */
       union {                                  /* FLCMDCR      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 ADRCNT2:1;          /*   ADRCNT2    */
                    UINT32 SCTCNT_:4;          /*   SCTCNT     */
                    UINT32 ADRMD:1;            /*   ADRMD      */
                    UINT32 CDSRC:1;            /*   CDSRC      */
                    UINT32 DOSR:1;             /*   DOSR       */
                    UINT32 :2;                 /*              */
                    UINT32 SELRW:1;            /*   SELRW      */
                    UINT32 DOADR:1;            /*   DOADR      */
                    UINT32 ADRCNT:2;           /*   ADRCNT     */
                    UINT32 DOCMD2:1;           /*   DOCMD2     */
                    UINT32 DOCMD1:1;           /*   DOCMD1     */
                    UINT32 SCTCNT:16;          /*   SCTCNT     */
                    } BIT;                      /*              */
             } FLCMDCR;                         /*              */
       union {                                  /* FLCMCDR      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :16;                /*              */
                    UINT32 CMD2:8;             /*   CMD2       */
                    UINT32 CMD1:8;             /*   CMD1       */
                    } BIT;                      /*              */
             } FLCMCDR;                         /*              */
       UINT32  FLADR;                          /* FLADR        */
       union {                                  /* FLDATAR      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 DT4:8;              /*   DT4        */
                    UINT32 DT3:8;              /*   DT3        */
                    UINT32 DT2:8;              /*   DT2        */
                    UINT32 DT1:8;              /*   DT1        */
                    } BIT;                      /*              */
             } FLDATAR;                         /*              */
       union {                                  /* FLDTCNTR     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 ECFLW:8;            /*   ECFLW      */
                    UINT32 DTFLW:8;            /*   DTFLW      */
                    UINT32 :4;                 /*              */
                    UINT32 DTCNT:12;           /*   DTCNT      */
                    } BIT;                      /*              */
             } FLDTCNTR;                        /*              */
       union {                                  /* FLINTDMACR   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 _4ECEINTE:1;        /*   _4ECEINTE  */
                    UINT32 ECERINTE:1;         /*   ECERINTE   */
                    UINT32 :2;                 /*              */
                    UINT32 FIFOTRG:2;          /*   FIFOTRG    */
                    UINT32 AC1CLR:1;           /*   AC1CLR     */
                    UINT32 AC0CLR:1;           /*   AC0CLR     */
                    UINT32 DREQ1EN:1;          /*   DREQ1EN    */
                    UINT32 DREQ0EN:1;          /*   DREQ0EN    */
                    UINT32 :6;                 /*              */
                    UINT32 ECERB:1;            /*   ECERB      */
                    UINT32 STERB:1;            /*   STERB      */
                    UINT32 BTOERB:1;           /*   BTOERB     */
                    UINT32 TRREQF1:1;          /*   TRREQF1    */
                    UINT32 TRREQF0:1;          /*   TRREQF0    */
                    UINT32 STERINTE:1;         /*   STERINTE   */
                    UINT32 RBERINTE:1;         /*   RBERINTE   */
                    UINT32 TEINTE:1;           /*   TEINTE     */
                    UINT32 TRINTE1:1;          /*   TRINTE1    */
                    UINT32 TRINTE0:1;          /*   TRINTE0    */
                    } BIT;                      /*              */
             } FLINTDMACR;                      /*              */
       union {                                  /* FLBSYTMR     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :12;                /*              */
                    UINT32 RBTMOUT:20;         /*   RBTMOUT    */
                    } BIT;                      /*              */
             } FLBSYTMR;                        /*              */
       union {                                  /* FLBSYCNT     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 STAT:8;             /*   STAT       */
                    UINT32 :4;                 /*              */
                    UINT32 RBTIMCNT:20;        /*   RBTIMCNT   */
                    } BIT;                      /*              */
             } FLBSYCNT;                        /*              */
       UINT8 wk0[8];                           /*              */
       union {                                  /* FLTRCR       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 TRSTAT:1;            /*   TRSTAT     */
                    UINT8 TREND:1;             /*   TREND      */
                    UINT8 TRSTRT:1;            /*   TRSTRT     */
                    } BIT;                      /*              */
             } FLTRCR;                          /*              */
       UINT8 wk1[11];                          /*              */
       union {                                  /* FLHOLDCR     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :31;                /*              */
                    UINT32 HOLDEN:1;           /*   HOLDEN     */
                    } BIT;                      /*              */
             } FLHOLDCR;                        /*              */
       union {                                  /* FLADR2       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :24;                /*              */
                    UINT32 ADR5:8;             /*   ADR5       */
                    } BIT;                      /*              */
             } FLADR2;                          /*              */
       UINT8 wk2[16];                          /*              */
       UINT32 FLDTFIFO;                        /* FLDTFIFO     */
       UINT8 wk3[12];                          /*              */
       UINT32 FLECFIFO;                        /* FLECFIFO     */
       UINT8 wk4[28];                          /*              */
       union {                                  /* FL4ECCRES1   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 LOC1:10;            /*   LOC1       */
                    UINT32 :6;                 /*              */
                    UINT32 PAT1:10;            /*   PAT1       */
                    } BIT;                      /*              */
             } FL4ECCRES1;                      /*              */
       union {                                  /* FL4ECCRES2   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 LOC2:10;            /*   LOC2       */
                    UINT32 :6;                 /*              */
                    UINT32 PAT2:10;            /*   PAT2       */
                    } BIT;                      /*              */
             } FL4ECCRES2;                      /*              */
       union {                                  /* FL4ECCRES3   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 LOC3:10;            /*   LOC3       */
                    UINT32 :6;                 /*              */
                    UINT32 PAT3:10;            /*   PAT3       */
                    } BIT;                      /*              */
             } FL4ECCRES3;                      /*              */
       union {                                  /* FL4ECCRES4   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 LOC4:10;            /*   LOC4       */
                    UINT32 :6;                 /*              */
                    UINT32 PAT4:10;            /*   PAT4       */
                    } BIT;                      /*              */
             } FL4ECCRES4;                      /*              */
       union {                                  /* FL4ECCCR     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :29;                /*              */
                    UINT32 _4ECCFA:1;          /*   _4ECCFA    */
                    UINT32 _4ECCEND:1;         /*   _4ECCEND   */
                    UINT32 _4ECCEXST:1;        /*   _4ECCEXST  */
                    } BIT;                      /*              */
             } FL4ECCCR;                        /*              */
       union {                                  /* FL4ECCCNT    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Bit Access  */
                    UINT32 :5;                 /*              */
                    UINT32 ERRCNT:11;          /*   ERRCNT     */
                    UINT32 :13;                /*              */
                    UINT32 ERRMAX:3;           /*   ERRMAX     */
                    } BIT;                      /*              */
             } FL4ECCCNT;                       /*              */
};                                              /*              */
struct st_usb {                                 /* struct USB   */
       union {                                  /* SYSCFG       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :5;                  /*              */
                    UINT16 SCKE:1;              /*   SCKE       */
                    UINT16 :2;                  /*              */
                    UINT16 HSE:1;               /*   HSE        */
                    UINT16 DCFM:1;              /*   DCFM       */
                    UINT16 DRPD:1;              /*   DRPD       */
                    UINT16 DPRPU:1;             /*   DPRPU      */
                    UINT16 :3;                  /*              */
                    UINT16 USBE:1;              /*   USBE       */
                    } BIT;                      /*              */
             } SYSCFG;                          /*              */
       union {                                  /* BUSWAIT      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :12;                 /*              */
                    UINT16 BWAIT:4;             /*   BWAIT      */
                    } BIT;                      /*              */
             } BUSWAIT;                         /*              */
       union {                                  /* SYSSTS       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :14;                 /*              */
                    UINT16 LNST:2;              /*   LNST       */
                    } BIT;                      /*              */
             } SYSSTS;                          /*              */
       UINT8 wk0[2];                           /*              */
       union {                                  /* DVSTCTR      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :7;                  /*              */
                    UINT16 WKUP:1;              /*   WKUP       */
                    UINT16 RWUPE:1;             /*   RWUPE      */
                    UINT16 USBRST:1;            /*   USBRST     */
                    UINT16 RESUME:1;            /*   RESUME     */
                    UINT16 UACT:1;              /*   UACT       */
                    UINT16 :1;                  /*              */
                    UINT16 RHST:3;              /*   RHST       */
                    } BIT;                      /*              */
             } DVSTCTR;                         /*              */
       UINT8 wk1[2];                           /*              */
       union {                                  /* TESTMODE     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :12;                 /*              */
                    UINT16 UTST:4;              /*   UTST       */
                    } BIT;                      /*              */
             } TESTMODE;                        /*              */
       UINT8 wk2[2];                           /*              */
       union {                                  /* D0FBCFG      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :11;                 /*              */
                    UINT16 TENDE:1;             /*   TENDE      */
                    UINT16 :4;                  /*              */
                    } BIT;                      /*              */
             } D0FBCFG;                         /*              */
       union {                                  /* D1FBCFG      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :11;                 /*              */
                    UINT16 TENDE:1;             /*   TENDE      */
                    UINT16 :4;                  /*              */
                    } BIT;                      /*              */
             } D1FBCFG;                         /*              */
       union {                                  /* CFIFO        */
             UINT32 LONG;                      /*  Long Access */
             UINT16 WORD;                       /*  Word Access */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT32 FIFOPORT:32;        /*   FIFOPORT   */
                    } BIT;                      /*              */
             } CFIFO;                           /*              */
       union {                                  /* D0FIFO       */
             UINT32 LONG;                      /*  Long Access */
             UINT16 WORD;                       /*  Word Access */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT32 FIFOPORT:32;        /*   FIFOPORT   */
                    } BIT;                      /*              */
             } D0FIFO;                          /*              */
       union {                                  /* D1FIFO       */
             UINT32 LONG;                      /*  Long Access */
             UINT16 WORD;                       /*  Word Access */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT32 FIFOPORT:32;        /*   FIFOPORT   */
                    } BIT;                      /*              */
             } D1FIFO;                          /*              */
       union {                                  /* CFIFOSEL     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 RCNT:1;              /*   RCNT       */
                    UINT16 REW:1;               /*   REW        */
                    UINT16 :2;                  /*              */
                    UINT16 MBW:2;               /*   MBW        */
                    UINT16 :1;                  /*              */
                    UINT16 BIGEND:1;            /*   BIGEND     */
                    UINT16 :2;                  /*              */
                    UINT16 ISEL:1;              /*   ISEL       */
                    UINT16 :1;                  /*              */
                    UINT16 CURPIPE:4;           /*   CURPIPE    */
                    } BIT;                      /*              */
             } CFIFOSEL;                        /*              */
       union {                                  /* CFIFOCTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BVAL:1;              /*   BVAL       */
                    UINT16 BCLR:1;              /*   BCLR       */
                    UINT16 FRDY:1;              /*   FRDY       */
                    UINT16 :1;                  /*              */
                    UINT16 DTLN:12;             /*   DTLN       */
                    } BIT;                      /*              */
             } CFIFOCTR;                        /*              */
       UINT8 wk3[4];                           /*              */
       union {                                  /* D0FIFOSEL    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 RCNT:1;              /*   RCNT       */
                    UINT16 REW:1;               /*   REW        */
                    UINT16 DCLRM:1;             /*   DCLRM      */
                    UINT16 DREQE:1;             /*   DREQE      */
                    UINT16 MBW:2;               /*   MBW        */
                    UINT16 :1;                  /*              */
                    UINT16 BIGEND:1;            /*   BIGEND     */
                    UINT16 :4;                  /*              */
                    UINT16 CURPIPE:4;           /*   CURPIPE    */
                    } BIT;                      /*              */
             } D0FIFOSEL;                       /*              */
       union {                                  /* D0FIFOCTR    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BVAL:1;              /*   BVAL       */
                    UINT16 BCLR:1;              /*   BCLR       */
                    UINT16 FRDY:1;              /*   FRDY       */
                    UINT16 :1;                  /*              */
                    UINT16 DTLN:12;             /*   DTLN       */
                    } BIT;                      /*              */
             } D0FIFOCTR;                       /*              */
       union {                                  /* D1FIFOSEL    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 RCNT:1;              /*   RCNT       */
                    UINT16 REW:1;               /*   REW        */
                    UINT16 DCLRM:1;             /*   DCLRM      */
                    UINT16 DREQE:1;             /*   DREQE      */
                    UINT16 MBW:2;               /*   MBW        */
                    UINT16 :1;                  /*              */
                    UINT16 BIGEND:1;            /*   BIGEND     */
                    UINT16 :4;                  /*              */
                    UINT16 CURPIPE:4;           /*   CURPIPE    */
                    } BIT;                      /*              */
             } D1FIFOSEL;                       /*              */
       union {                                  /* D1FIFOCTR    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BVAL:1;              /*   BVAL       */
                    UINT16 BCLR:1;              /*   BCLR       */
                    UINT16 FRDY:1;              /*   FRDY       */
                    UINT16 :1;                  /*              */
                    UINT16 DTLN:12;             /*   DTLN       */
                    } BIT;                      /*              */
             } D1FIFOCTR;                       /*              */
       union {                                  /* INTENB0      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 VBSE:1;              /*   VBSE       */
                    UINT16 RSME:1;              /*   RSME       */
                    UINT16 SOFE:1;              /*   SOFE       */
                    UINT16 DVSE:1;              /*   DVSE       */
                    UINT16 CTRE:1;              /*   CTRE       */
                    UINT16 BEMPE:1;             /*   BEMPE      */
                    UINT16 NRDYE:1;             /*   NRDYE      */
                    UINT16 BRDYE:1;             /*   BRDYE      */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } INTENB0;                         /*              */
       union {                                  /* INTENB1      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 BCHGE:1;             /*   BCHGE      */
                    UINT16 :1;                  /*              */
                    UINT16 DTCHE:1;             /*   DTCHE      */
                    UINT16 ATTCHE:1;            /*   ATTCHE     */
                    UINT16 :4;                  /*              */
                    UINT16 EOFERRE:1;           /*   EOFERRE    */
                    UINT16 SIGNE:1;             /*   SIGNE      */
                    UINT16 SACKE:1;             /*   SACKE      */
                    UINT16 :4;                  /*              */
                    } BIT;                      /*              */
             } INTENB1;                         /*              */
       UINT8 wk4[2];                           /*              */
       union {                                  /* BRDYENB      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 PIPE9BRDYE:1;        /*   PIPE9BRDYE */
                    UINT16 PIPE8BRDYE:1;        /*   PIPE8BRDYE */
                    UINT16 PIPE7BRDYE:1;        /*   PIPE7BRDYE */
                    UINT16 PIPE6BRDYE:1;        /*   PIPE6BRDYE */
                    UINT16 PIPE5BRDYE:1;        /*   PIPE5BRDYE */
                    UINT16 PIPE4BRDYE:1;        /*   PIPE4BRDYE */
                    UINT16 PIPE3BRDYE:1;        /*   PIPE3BRDYE */
                    UINT16 PIPE2BRDYE:1;        /*   PIPE2BRDYE */
                    UINT16 PIPE1BRDYE:1;        /*   PIPE1BRDYE */
                    UINT16 PIPE0BRDYE:1;        /*   PIPE0BRDYE */
                    } BIT;                      /*              */
             } BRDYENB;                         /*              */
       union {                                  /* NRDYENB      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 PIPE9NRDYE:1;        /*   PIPE9NRDYE */
                    UINT16 PIPE8NRDYE:1;        /*   PIPE8NRDYE */
                    UINT16 PIPE7NRDYE:1;        /*   PIPE7NRDYE */
                    UINT16 PIPE6NRDYE:1;        /*   PIPE6NRDYE */
                    UINT16 PIPE5NRDYE:1;        /*   PIPE5NRDYE */
                    UINT16 PIPE4NRDYE:1;        /*   PIPE4NRDYE */
                    UINT16 PIPE3NRDYE:1;        /*   PIPE3NRDYE */
                    UINT16 PIPE2NRDYE:1;        /*   PIPE2NRDYE */
                    UINT16 PIPE1NRDYE:1;        /*   PIPE1NRDYE */
                    UINT16 PIPE0NRDYE:1;        /*   PIPE0NRDYE */
                    } BIT;                      /*              */
             } NRDYENB;                         /*              */
       union {                                  /* BEMPENB      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 PIPE9BEMPE:1;        /*   PIPE9BEMPE */
                    UINT16 PIPE8BEMPE:1;        /*   PIPE8BEMPE */
                    UINT16 PIPE7BEMPE:1;        /*   PIPE7BEMPE */
                    UINT16 PIPE6BEMPE:1;        /*   PIPE6BEMPE */
                    UINT16 PIPE5BEMPE:1;        /*   PIPE5BEMPE */
                    UINT16 PIPE4BEMPE:1;        /*   PIPE4BEMPE */
                    UINT16 PIPE3BEMPE:1;        /*   PIPE3BEMPE */
                    UINT16 PIPE2BEMPE:1;        /*   PIPE2BEMPE */
                    UINT16 PIPE1BEMPE:1;        /*   PIPE1BEMPE */
                    UINT16 PIPE0BEMPE:1;        /*   PIPE0BEMPE */
                    } BIT;                      /*              */
             } BEMPENB;                         /*              */
       union {                                  /* SOFCFG       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :7;                  /*              */
                    UINT16 TRNENSEL:1;          /*   TRNENSEL   */
                    UINT16 :1;                  /*              */
                    UINT16 BRDYM:1;             /*   BRDYM      */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } SOFCFG;                          /*              */
       UINT8 wk5[2];                           /*              */
       union {                                  /* INTSTS0      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 VBINT:1;             /*   VBINT      */
                    UINT16 RESM:1;              /*   RESM       */
                    UINT16 SOFR:1;              /*   SOFR       */
                    UINT16 DVST:1;              /*   DVST       */
                    UINT16 CTRT:1;              /*   CTRT       */
                    UINT16 BEMP:1;              /*   BEMP       */
                    UINT16 NRDY:1;              /*   NRDY       */
                    UINT16 BRDY:1;              /*   BRDY       */
                    UINT16 VBSTS:1;             /*   VBSTS      */
                    UINT16 DVSQ:3;              /*   DVSQ       */
                    UINT16 VALID:1;             /*   VALID      */
                    UINT16 CTSQ:3;              /*   CTSQ       */
                    } BIT;                      /*              */
             } INTSTS0;                         /*              */
       union {                                  /* INTSTS1      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 BCHG:1;              /*   BCHG       */
                    UINT16 :1;                  /*              */
                    UINT16 DTCH:1;              /*   DTCH       */
                    UINT16 ATTCH:1;             /*   ATTCH      */
                    UINT16 :4;                  /*              */
                    UINT16 EOFERR:1;            /*   EOFERR     */
                    UINT16 SIGN:1;              /*   SIGN       */
                    UINT16 SACK:1;              /*   SACK       */
                    UINT16 :4;                  /*              */
                    } BIT;                      /*              */
             } INTSTS1;                         /*              */
       UINT8 wk6[2];                           /*              */
       union {                                  /* BRDYSTS      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 PIPE9BRDY:1;         /*   PIPE9BRDY  */
                    UINT16 PIPE8BRDY:1;         /*   PIPE8BRDY  */
                    UINT16 PIPE7BRDY:1;         /*   PIPE7BRDY  */
                    UINT16 PIPE6BRDY:1;         /*   PIPE6BRDY  */
                    UINT16 PIPE5BRDY:1;         /*   PIPE5BRDY  */
                    UINT16 PIPE4BRDY:1;         /*   PIPE4BRDY  */
                    UINT16 PIPE3BRDY:1;         /*   PIPE3BRDY  */
                    UINT16 PIPE2BRDY:1;         /*   PIPE2BRDY  */
                    UINT16 PIPE1BRDY:1;         /*   PIPE1BRDY  */
                    UINT16 PIPE0BRDY:1;         /*   PIPE0BRDY  */
                    } BIT;                      /*              */
             } BRDYSTS;                         /*              */
       union {                                  /* NRDYSTS      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 PIPE9NRDY:1;         /*   PIPE9NRDY  */
                    UINT16 PIPE8NRDY:1;         /*   PIPE8NRDY  */
                    UINT16 PIPE7NRDY:1;         /*   PIPE7NRDY  */
                    UINT16 PIPE6NRDY:1;         /*   PIPE6NRDY  */
                    UINT16 PIPE5NRDY:1;         /*   PIPE5NRDY  */
                    UINT16 PIPE4NRDY:1;         /*   PIPE4NRDY  */
                    UINT16 PIPE3NRDY:1;         /*   PIPE3NRDY  */
                    UINT16 PIPE2NRDY:1;         /*   PIPE2NRDY  */
                    UINT16 PIPE1NRDY:1;         /*   PIPE1NRDY  */
                    UINT16 PIPE0NRDY:1;         /*   PIPE0NRDY  */
                    } BIT;                      /*              */
             } NRDYSTS;                         /*              */
       union {                                  /* BEMPSTS      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 PIPE9BEMP:1;         /*   PIPE9BEMP  */
                    UINT16 PIPE8BEMP:1;         /*   PIPE8BEMP  */
                    UINT16 PIPE7BEMP:1;         /*   PIPE7BEMP  */
                    UINT16 PIPE6BEMP:1;         /*   PIPE6BEMP  */
                    UINT16 PIPE5BEMP:1;         /*   PIPE5BEMP  */
                    UINT16 PIPE4BEMP:1;         /*   PIPE4BEMP  */
                    UINT16 PIPE3BEMP:1;         /*   PIPE3BEMP  */
                    UINT16 PIPE2BEMP:1;         /*   PIPE2BEMP  */
                    UINT16 PIPE1BEMP:1;         /*   PIPE1BEMP  */
                    UINT16 PIPE0BEMP:1;         /*   PIPE0BEMP  */
                    } BIT;                      /*              */
             } BEMPSTS;                         /*              */
       union {                                  /* FRMNUM       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 OVRN:1;              /*   OVRN       */
                    UINT16 CRCE:1;              /*   CRCE       */
                    UINT16 :3;                  /*              */
                    UINT16 FRNM:11;             /*   FRNM       */
                    } BIT;                      /*              */
             } FRMNUM;                          /*              */
       union {                                  /* UFRMNUM      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :13;                 /*              */
                    UINT16 UFRNM:3;             /*   UFRNM      */
                    } BIT;                      /*              */
             } UFRMNUM;                         /*              */
       union {                                  /* USBADDR      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :9;                  /*              */
                    UINT16 USBADDR:7;           /*   USBADDR    */
                    } BIT;                      /*              */
             } USBADDR;                         /*              */
       UINT8 wk7[2];                           /*              */
       union {                                  /* USBREQ       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BREQUEST:8;          /*   BREQUEST   */
                    UINT16 BMREQUESTTYPE:8;     /*   BMREQUESTTYPE */
                    } BIT;                      /*              */
             } USBREQ;                          /*              */
       UINT16 USBVAL;                           /* USBVAL       */
       UINT16 USBINDX;                          /* USBINDX      */
       UINT16 USBLENG;                          /* USBLENG      */
       union {                                  /* DCPCFG       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :11;                 /*              */
                    UINT16 DIR:1;               /*   DIR        */
                    UINT16 :4;                  /*              */
                    } BIT;                      /*              */
             } DCPCFG;                          /*              */
       union {                                  /* DCPMAXP      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 DEVSEL:4;            /*   DEVSEL     */
                    UINT16 :5;                  /*              */
                    UINT16 MXPS:7;              /*   MXPS       */
                    } BIT;                      /*              */
             } DCPMAXP;                         /*              */
       union {                                  /* DCPCTR       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 SUREQ:1;             /*   SUREQ      */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 SUREQCLR:1;          /*   SUREQCLR   */
                    UINT16 :2;                  /*              */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 PINGE:1;             /*   PINGE      */
                    UINT16 :1;                  /*              */
                    UINT16 CCPL:1;              /*   CCPL       */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } DCPCTR;                          /*              */
       UINT8 wk8[2];                           /*              */
       union {                                  /* PIPESEL      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :12;                 /*              */
                    UINT16 PIPESEL:4;           /*   PIPESEL    */
                    } BIT;                      /*              */
             } PIPESEL;                         /*              */
       UINT8 wk9[2];                           /*              */
       union {                                  /* PIPECFG      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TYPE:2;              /*   TYPE       */
                    UINT16 :3;                  /*              */
                    UINT16 BFRE:1;              /*   BFRE       */
                    UINT16 DBLB:1;              /*   DBLB       */
                    UINT16 CNTMD:1;             /*   CNTMD      */
                    UINT16 SHTNAK:1;            /*   SHTNAK     */
                    UINT16 :2;                  /*              */
                    UINT16 DIR:1;               /*   DIR        */
                    UINT16 EPNUM:4;             /*   EPNUM      */
                    } BIT;                      /*              */
             } PIPECFG;                         /*              */
       union {                                  /* PIPEBUF      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 BUFSIZE:5;           /*   BUFSIZE    */
                    UINT16 :3;                  /*              */
                    UINT16 BUFNMB:7;            /*   BUFNMB     */
                    } BIT;                      /*              */
             } PIPEBUF;                         /*              */
       union {                                  /* PIPEMAXP     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 DEVSEL:4;            /*   DEVSEL     */
                    UINT16 :1;                  /*              */
                    UINT16 MXPS:11;             /*   MXPS       */
                    } BIT;                      /*              */
             } PIPEMAXP;                        /*              */
       union {                                  /* PIPEPERI     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 IFIS:1;              /*   IFIS       */
                    UINT16 :9;                  /*              */
                    UINT16 IITV:3;              /*   IITV       */
                    } BIT;                      /*              */
             } PIPEPERI;                        /*              */
       union {                                  /* PIPE1CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 INBUFM:1;            /*   INBUFM     */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :1;                  /*              */
                    UINT16 ATREPM:1;            /*   ATREPM     */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE1CTR;                        /*              */
       union {                                  /* PIPE2CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 INBUFM:1;            /*   INBUFM     */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :1;                  /*              */
                    UINT16 ATREPM:1;            /*   ATREPM     */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE2CTR;                        /*              */
       union {                                  /* PIPE3CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 INBUFM:1;            /*   INBUFM     */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :1;                  /*              */
                    UINT16 ATREPM:1;            /*   ATREPM     */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE3CTR;                        /*              */
       union {                                  /* PIPE4CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 INBUFM:1;            /*   INBUFM     */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :1;                  /*              */
                    UINT16 ATREPM:1;            /*   ATREPM     */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE4CTR;                        /*              */
       union {                                  /* PIPE5CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 INBUFM:1;            /*   INBUFM     */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :1;                  /*              */
                    UINT16 ATREPM:1;            /*   ATREPM     */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE5CTR;                        /*              */
       union {                                  /* PIPE6CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 :1;                  /*              */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :2;                  /*              */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE6CTR;                        /*              */
       union {                                  /* PIPE7CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 :1;                  /*              */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :2;                  /*              */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE7CTR;                        /*              */
       union {                                  /* PIPE8CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 :1;                  /*              */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :2;                  /*              */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE8CTR;                        /*              */
       union {                                  /* PIPE9CTR     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 BSTS:1;              /*   BSTS       */
                    UINT16 :1;                  /*              */
                    UINT16 CSCLR:1;             /*   CSCLR      */
                    UINT16 CSSTS:1;             /*   CSSTS      */
                    UINT16 :2;                  /*              */
                    UINT16 ACLRM:1;             /*   ACLRM      */
                    UINT16 SQCLR:1;             /*   SQCLR      */
                    UINT16 SQSET:1;             /*   SQSET      */
                    UINT16 SQMON:1;             /*   SQMON      */
                    UINT16 PBUSY:1;             /*   PBUSY      */
                    UINT16 :3;                  /*              */
                    UINT16 PID:2;               /*   PID        */
                    } BIT;                      /*              */
             } PIPE9CTR;                        /*              */
       UINT8 wk10[14];                         /*              */
       union {                                  /* PIPE1TRE     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 TRENB:1;             /*   TRENB      */
                    UINT16 TRCLR:1;             /*   TRCLR      */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE1TRE;                        /*              */
       UINT16 PIPE1TRN;                         /* PIPE1TRN     */
       union {                                  /* PIPE2TRE     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 TRENB:1;             /*   TRENB      */
                    UINT16 TRCLR:1;             /*   TRCLR      */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE2TRE;                        /*              */
       UINT16 PIPE2TRN;                         /* PIPE2TRN     */
       union {                                  /* PIPE3TRE     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 TRENB:1;             /*   TRENB      */
                    UINT16 TRCLR:1;             /*   TRCLR      */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE3TRE;                        /*              */
       UINT16 PIPE3TRN;                         /* PIPE3TRN     */
       union {                                  /* PIPE4TRE     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 TRENB:1;             /*   TRENB      */
                    UINT16 TRCLR:1;             /*   TRCLR      */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE4TRE;                        /*              */
       UINT16 PIPE4TRN;                         /* PIPE4TRN     */
       union {                                  /* PIPE5TRE     */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 TRENB:1;             /*   TRENB      */
                    UINT16 TRCLR:1;             /*   TRCLR      */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } PIPE5TRE;                        /*              */
       UINT16 PIPE5TRN;                         /* PIPE5TRN     */
       UINT8 wk11[30];                         /*              */
        union {                                 /* USBACSWR1    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit  Access */
                    UINT16 :6;                  /*  ATEST31-26  */
                    UINT16 PREENA :1;           /*  PREENA0     */
                    UINT16 :9;                  /*  ATEST24-16  */
                    } BIT;
             } USBACSWR1;
       UINT8 wk12[12];                         /*              */
       union {                                  /* DEVADD0      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD0;                         /*              */
       union {                                  /* DEVADD1      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD1;                         /*              */
       union {                                  /* DEVADD2      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD2;                         /*              */
       union {                                  /* DEVADD3      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD3;                         /*              */
       union {                                  /* DEVADD4      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD4;                         /*              */
       union {                                  /* DEVADD5      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD5;                         /*              */
       union {                                  /* DEVADD6      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD6;                         /*              */
       union {                                  /* DEVADD7      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD7;                         /*              */
       union {                                  /* DEVADD8      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD8;                         /*              */
       union {                                  /* DEVADD9      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADD9;                         /*              */
       union {                                  /* DEVADDA      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :1;                  /*              */
                    UINT16 UPPHUB:4;            /*   UPPHUB     */
                    UINT16 HUBPORT:3;           /*   HUBPORT    */
                    UINT16 USBSPD:2;            /*   USBSPD     */
                    UINT16 :6;                  /*              */
                    } BIT;                      /*              */
             } DEVADDA;                         /*              */
};                                              /*              */
struct st_vdc3 {                                /* struct VDC3  */
       union {                                  /* VIDEO_MODE   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {
                    UINT32 :2;
                    UINT32 RGB565:1;
                    UINT32 INV_CbCr:1;
                    UINT32 :6;
                    UINT32 BURST_MODE_DISP:1;
                    UINT32 BURST_MODE_MAIN:1;
                    UINT32 :2;
                    UINT32 ENDIAN_DISP:1;
                    UINT32 ENDIAN_MAIN:1;
                    UINT32 :1;
                    UINT32 SEL_EXSYNC:1;
                    UINT32 SEL_656601:1;
                    UINT32 SEL_525625:1;
                    UINT32 :7;
                    UINT32 VIDEO_MODE:1;
                    UINT32 :2;
                    UINT32 VIDEO_DISP_EXE:1;
                    UINT32 VIDEO_MAIN_EXE:1;   
                    } BIT;
             } VIDEO_MODE;                      /*              */
       union {                                  /* VIDEO_INT_CNT */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :3;                 /*              */
                    UINT32 INT_V_EN:1;         /*   INT_V_EN   */
                    UINT32 :3;                 /*              */
                    UINT32 INT_F_EN:1;         /*   INT_F_EN   */
                    UINT32 :3;                 /*              */
                    UINT32 INT_UF_EN:1;        /*   INT_UF_EN  */
                    UINT32 :3;                 /*              */
                    UINT32 INT_OF_EN:1;        /*   INT_OF_EN  */
                    UINT32 :3;                 /*              */
                    UINT32 V_PERIOD:1;         /*   V_PERIOD   */
                    UINT32 :3;                 /*              */
                    UINT32 F_END:1;            /*   F_END      */
                    UINT32 :3;                 /*              */
                    UINT32 UNDER_FLOW:1;       /*   UNDER_FLOW */
                    UINT32 :3;                 /*              */
                    UINT32 OVER_FLOW:1;        /*   OVER_FLOW  */
                    } BIT;                      /*              */
             } VIDEO_INT_CNT;                   /*              */
       union {                                  /* VIDEO_TIM_CNT */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 FIELD_SKEW:10;      /*   FIELD_SKEW */
                    UINT32 :10;                /*              */
                    UINT32 VSYNC_TYP:1;        /*   VSYNC_TYP  */
                    UINT32 HSYNC_TYP:1;        /*   HSYNC_TYP  */
                    UINT32 :1;                 /*              */
                    UINT32 VSYNC_TIM:1;        /*   VSYNC_TIM  */
                    UINT32 HSYNC_TIM:1;        /*   HSYNC_TIM  */
                    UINT32 VIDEO_TIM:1;        /*   VIDEO_TIM  */
                    } BIT;                      /*              */
             } VIDEO_TIM_CNT;                   /*              */
       UINT8 wk0[244];                         /*              */
       union {                                  /* VIDEO_SIZE   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :7;                 /*              */
                    UINT32 VIDEO_HEIGHT:9;     /*   VIDEO_HEIGHT */
                    UINT32 :6;                 /*              */
                    UINT32 VIDEO_WIDTH:10;     /*   VIDEO_WIDTH */
                    } BIT;                      /*              */
             } VIDEO_SIZE;                      /*              */
       union {                                  /* VIDEO_VSTART */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :7;                 /*              */
                    UINT32 VIDEO_VSTART_TOP:9; /*   VIDEO_VSTART_TOP */
                    UINT32 :7;                 /*              */
                    UINT32 VIDEO_VSTART_BTM:9; /*   VIDEO_VSTART_BTM */
                    } BIT;                      /*              */
             } VIDEO_VSTART;                    /*              */
       union {                                  /* VIDEO_HSTART */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :23;                /*              */
                    UINT32 VIDEO_HSTART:9;     /*   VIDEO_HSTART */
                    } BIT;                      /*              */
             } VIDEO_HSTART;                    /*              */
       union {                                  /* VIDEO_VSYNC_TIM1 */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 VIDEO_VSYNC_START1_TOP:10;/*   VIDEO_VSYNC_START1_TOP */
                    UINT32 :6;                 /*              */
                    UINT32 VIDEO_VSYNC_START1_BTM:10;/*   VIDEO_VSYNC_START1_BTM */
                    } BIT;                      /*              */
             } VIDEO_VSYNC_TIM1;                /*              */
       union {                                  /* VIDEO_SAVE_NUM */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 FIELD_NUM:10;       /*   FIELD_NUM */
                    UINT32 :6;                 /*              */
                    UINT32 FIELD_SAVE_NUM:10;  /*   FIELD_SAVE_NUM */
                    } BIT;                      /*              */
             } VIDEO_SAVE_NUM;                  /*              */
       union {                                  /* VIDEO_IMAGE_CNT */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 Contrast:8;         /*   Contrast   */
                    UINT32 Bright:8 ;          /*   Bright     */
                    UINT32 :6;                 /*              */
                    UINT32 CLIP_Y:1;           /*   CLIP_Y     */
                    UINT32 CLIP_C:1;           /*   CLKP_C     */
                    UINT32 :1;                 /*              */
                    UINT32 SUB_SCALE_V:1;      /*   SUB_SCALE_V */
                    UINT32 SCALE_V:2;          /*   SCALE_V    */
                    UINT32 :1;                 /*              */
                    UINT32 SUB_SCALE_H:1;      /*   SUB_SCALE_H */
                    UINT32 SCALE_H:2;          /*   SCALE_H    */
                    } BIT;                      /*              */
             } VIDEO_IMAGE_CNT;                 /*              */
       union {                                  /* VIDEO_BASEADR */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             } VIDEO_BASEADR;                    /*              */
       union {                                  /* VIDEO_LINE_OFFSET */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             } VIDEO_LINE_OFFSET;               /*              */
       union {                                  /* VIDEO_FIELD_OFFSET */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             } VIDEO_FIELD_OFFSET;              /*              */
       union {                                  /* VIDEO_LINEBUFF_NUM */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :23;                /*              */
                    UINT32 VIDEO_LINEBUFF_NUM:9;/*   VIDEO_LINEBUFF_NUM */
                    } BIT;                      /*              */
             } VIDEO_LINEBUFF_NUM;              /*              */
       union {                                  /* VIDEO_DISP_SIZE */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :7;                 /*              */
                    UINT32 VIDEO_DISP_HEIGHT:9;/*   VIDEO_DISP_HEIGHT */
                    UINT32 :6;                 /*              */
                    UINT32 VIDEO_DISP_WIDTH:10;/*   VIDEO_DISP_WIDTH */
                    } BIT;                      /*              */
             } VIDEO_DISP_SIZE;                 /*              */
       union {                                  /* VIDEO_DISP_HSTART */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :22;                /*              */
                    UINT32 VIDEO_DISP_HSTART:10;/*   VIDEO_DISP_HSTART */
                    } BIT;                      /*              */
             } VIDEO_DISP_HSTART;               /*              */
       UINT8 wk1[1744];                        /*              */
       union {                                  /* GRCMEN1      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 WE:1;               /*   WE         */
                    UINT32 :29;                /*              */
                    UINT32 DEN:1;              /*   DEN        */
                    UINT32 VEN:1;              /*   VEN        */
                    } BIT;                      /*              */
             } GRCMEN1;                         /*              */
       union {                                  /* GRCBUSCNT1   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :23;                /*              */
                    UINT32 BURST_MODE:1;       /*   BURST_MODE */
                    UINT32 :3;                 /*              */
                    UINT32 BUS_FORMAT:1;       /*   BUS_FORMAT */
                    UINT32 :3;                 /*              */
                    UINT32 ENDIAN:1;           /*   ENDIAN     */
                    } BIT;                      /*              */
             } GRCBUSCNT1;                      /*              */
       union {                                  /* GRCINTCNT1   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :15;                /*              */
                    UINT32 INT_UF_EN:1;        /*   INT_UF_EN  */
                    UINT32 :15;                /*              */
                    UINT32 UNDER_FLOW:1;       /*   UNDER_FLOW */
                    } BIT;                      /*              */
             } GRCINTCNT1;                      /*              */
       UINT8 wk2[764];                         /*              */
       union {                                  /* GROPSADR1    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :3;                 /*              */
                    UINT32 GROPSADR:29;        /*   GROPSADR   */
                    } BIT;                      /*              */
             } GROPSADR1;                       /*              */
       union {                                  /* GROPSWH1     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 GROPSH:10;          /*   GROPSH     */
                    UINT32 :6;                 /*              */
                    UINT32 GROPSW:10;          /*   GROPSW     */
                    } BIT;                      /*              */
             } GROPSWH1;                        /*              */
       union {                                  /* GROPSOFST1   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :3;                 /*              */
                    UINT32 GROPSOFST:29;       /*   GROPSOFST  */
                    } BIT;                      /*              */
             } GROPSOFST1;                      /*              */
       union {                                  /* GROPDPHV1    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 GROPDPV:10;         /*   GROPDPV    */
                    UINT32 :6;                 /*              */
                    UINT32 GROPDPH:10;         /*   GROPDPH    */
                    } BIT;                      /*              */
             } GROPDPHV1;                       /*              */
       union {                                  /* GROPEWH1     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 GROPEH:10;          /*   GROPEH     */
                    UINT32 :6;                 /*              */
                    UINT32 GROPEW:10;          /*   GROPEW     */
                    } BIT;                      /*              */
             } GROPEWH1;                        /*              */
       union {                                  /* GROPEDPHV1   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 GROPEDPV:10;        /*   GROPEDPV   */
                    UINT32 :6;                 /*              */
                    UINT32 GROPEDPH:10;        /*   GROPEDPH   */
                    } BIT;                      /*              */
             } GROPEDPHV1;                      /*              */
       union {                                  /* GROPEDPA1    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 DEFA:8;             /*   DEFA       */
                    UINT32 ACOEF:8;            /*   ACOEF      */
                    UINT32 ARATE:8;            /*   ARATE      */
                    UINT32 WE:1;               /*   WE         */
                    UINT32 :2;                 /*              */
                    UINT32 AST:1;              /*   AST        */
                    UINT32 :1;                 /*              */
                    UINT32 AMOD:2;             /*   AMOD       */
                    UINT32 AEN:1;              /*   AEN        */
                    } BIT;                      /*              */
             } GROPEDPA1;                       /*              */
       union {                                  /* GROPCRKY0_1  */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :15;                /*              */
                    UINT32 CKEN:1;             /*   CKEN       */
                    UINT32 CROMAKR:5;          /*   CROMAKR    */
                    UINT32 CROMAKG:6;          /*   CROMAKG    */
                    UINT32 CROMAKB:5;          /*   CROMAKB    */
                    } BIT;                      /*              */
             } GROPCRKY0_1;                     /*              */
       union {                                  /* GROPCRKY1_1  */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :8;                 /*              */
                    UINT32 ALPHA:8;            /*   ALPHA      */
                    UINT32 R:5;                /*   R          */
                    UINT32 G:6;                /*   G          */
                    UINT32 B:5;                /*   B          */
                    } BIT;                      /*              */
             } GROPCRKY1_1;                     /*              */
       union {                                  /* GROPBASERGB1 */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :16;                /*              */
                    UINT32 BASE_R:5;           /*   BASE_R     */
                    UINT32 BASE_G:6;           /*   BASE_G     */
                    UINT32 BASE_B:5;           /*   BASE_B     */
                    } BIT;                      /*              */
             } GROPBASERGB1;                    /*              */
       UINT8 wk3[1232];                        /*              */
       union {                                  /* GRCMEN2      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 WE:1;               /*   WE         */
                    UINT32 :29;                /*              */
                    UINT32 DEN:1;              /*   DEN        */
                    UINT32 VEN:1;              /*   VEN        */
                    } BIT;                      /*              */
             } GRCMEN2;                         /*              */
       union {                                  /* GRCBUSCNT2   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :23;                /*              */
                    UINT32 BURST_MODE:1;       /*   BURST_MODE */
                    UINT32 :3;                 /*              */
                    UINT32 BUS_FORMAT:1;       /*   BUS_FORMAT */
                    UINT32 :3;                 /*              */
                    UINT32 ENDIAN:1;           /*   ENDIAN     */
                    } BIT;                      /*              */
             } GRCBUSCNT2;                      /*              */
       union {                                  /* GRCINTCNT2   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :15;                /*              */
                    UINT32 INT_UF_EN:1;        /*   INT_UF_EN  */
                    UINT32 :15;                /*              */
                    UINT32 UNDER_FLOW:1;       /*   UNDER_FLOW */
                    } BIT;                      /*              */
             } GRCINTCNT2;                      /*              */
       UINT8 wk4[764];                         /*              */
       union {                                  /* GROPSADR2    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :3;                 /*              */
                    UINT32 GROPSADR:29;        /*   GROPSADR   */
                    } BIT;                      /*              */
             } GROPSADR2;                       /*              */
       union {                                  /* GROPSWH2     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 GROPSH:10;          /*   GROPSH     */
                    UINT32 :6;                 /*              */
                    UINT32 GROPSW:10;          /*   GROPSW     */
                    } BIT;                      /*              */
             } GROPSWH2;                        /*              */
       union {                                  /* GROPSOFST2   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :3;                 /*              */
                    UINT32 GROPSOFST:29;       /*   GROPSOFST  */
                    } BIT;                      /*              */
             } GROPSOFST2;                      /*              */
       union {                                  /* GROPDPHV2    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 GROPDPV:10;         /*   GROPDPV    */
                    UINT32 :6;                 /*              */
                    UINT32 GROPDPH:10;         /*   GROPDPH    */
                    } BIT;                      /*              */
             } GROPDPHV2;                       /*              */
       union {                                  /* GROPEWH2     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 GROPEH:10;          /*   GROPEH     */
                    UINT32 :6;                 /*              */
                    UINT32 GROPEW:10;          /*   GROPEW     */
                    } BIT;                      /*              */
             } GROPEWH2;                        /*              */
       union {                                  /* GROPEDPHV2   */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 GROPEDPV:10;        /*   GROPEDPV   */
                    UINT32 :6;                 /*              */
                    UINT32 GROPEDPH:10;        /*   GROPEDPH   */
                    } BIT;                      /*              */
             } GROPEDPHV2;                      /*              */
       union {                                  /* GROPEDPA2    */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 DEFA:8;             /*   DEFA       */
                    UINT32 ACOEF:8;            /*   ACOEF      */
                    UINT32 ARATE:8;            /*   ARATE      */
                    UINT32 WE:1;               /*   WE         */
                    UINT32 :2;                 /*              */
                    UINT32 AST:1;              /*   AST        */
                    UINT32 :1;                 /*              */
                    UINT32 AMOD:2;             /*   AMOD       */
                    UINT32 AEN:1;              /*   AEN        */
                    } BIT;                      /*              */
             } GROPEDPA2;                       /*              */
       union {                                  /* GROPCRKY0_2  */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :15;                /*              */
                    UINT32 CKEN:1;             /*   CKEN       */
                    UINT32 CROMAKR:5;          /*   CROMAKR    */
                    UINT32 CROMAKG:6;          /*   CROMAKG    */
                    UINT32 CROMAKB:5;          /*   CROMAKB    */
                    } BIT;                      /*              */
             } GROPCRKY0_2;                     /*              */
       union {                                  /* GROPCRKY1_2  */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :8;                 /*              */
                    UINT32 ALPHA:8;            /*   ALPHA      */
                    UINT32 R:5;                /*   R          */
                    UINT32 G:6;                /*   G          */
                    UINT32 B:5;                /*   B          */
                    } BIT;                      /*              */
             } GROPCRKY1_2;                     /*              */
       union {                                  /* GROPBASERGB2 */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :16;                /*              */
                    UINT32 BASE_R:5;           /*   BASE_R     */
                    UINT32 BASE_G:6;           /*   BASE_G     */
                    UINT32 BASE_B:5;           /*   BASE_B     */
                    } BIT;                      /*              */
             } GROPBASERGB2;                    /*              */
       UINT8 wk5[1232];                        /*              */
       union {                                  /* SGMODE       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :15;                /*              */
                    UINT32 EX_SYNC_MODE:1;     /*   EX_SYNC_MODE */
                    UINT32 :15;                /*              */
                    UINT32 RGB565:1;           /*   RGB565     */
                    } BIT;                      /*              */
             } SGMODE;                          /*              */
       union {                                  /* SGINTCNT     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :2;                 /*              */
                    UINT32 INT_LINE_NUM:10;    /*   INT_LINE_NUM */
                    UINT32 :3;                 /*              */
                    UINT32 INT_LINE_EN:1;      /*   INT_LINE_EN */
                    UINT32 :15;                /*              */
                    UINT32 LINE_STATUS:1;      /*   LINE_STATUS */
                    } BIT;                      /*              */
             } SGINTCNT;                        /*              */
       union {                                  /* SYNCNT       */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :7;                 /*              */
                    UINT32 RGB_TIM:1;          /*   RGB_TIM    */
                    UINT32 :4;                 /*              */
                    UINT32 VSYNC_TIM:1;        /*   VSYNC_TIM  */
                    UINT32 HSYNC_TIM:1;        /*   HSYNC_TIM  */
                    UINT32 DE_TIM:1;           /*   DE_TIM     */
                    UINT32 M_DISP_TIM:1;       /*   M_DISP_TM  */
                    UINT32 :12;                /*              */
                    UINT32 VSYNC_TYPE:1;       /*   VSYNC_TYPE */
                    UINT32 HSYNC_TYPE:1;       /*   HSYNC_TYPE */
                    UINT32 DE_TYPE:1;          /*   DE_TYPE    */
                    UINT32 M_DISP_TYPE:1;      /*   M_DISP_TYPE */
                    } BIT;                      /*              */
             } SYNCNT;                          /*              */
       union {                                  /* PANEL_CLKSEL */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :18;                /*              */
                    UINT32 ICKSEL:1;           /*   ICKSEL     */
                    UINT32 ICKEN:1;            /*   ICKEN      */
                    UINT32 :6;                 /*              */
                    UINT32 DCDR:6;             /*   DCDR       */
                    } BIT;                      /*              */
             } PANEL_CLKSEL;                    /*              */
       UINT8 wk6[240];                         /*              */
       union {                                  /* SYN_SIZE     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 SYN_HEIGHT:10;      /*   SYN_HEIGHT */
                    UINT32 :5;                 /*              */
                    UINT32 SYN_WIDTH:11;       /*   SYN_WIDTH  */
                    } BIT;                      /*              */
             } SYN_SIZE;                        /*              */
       union {                                  /* PANEL_VSYNC_TIM */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 VSYNC_START:10;     /*   VSYNC_START */
                    UINT32 :6;                 /*              */
                    UINT32 VSYNC_END:10;       /*   VSYNC_END  */
                    } BIT;                      /*              */
             } PANEL_VSYNC_TIM;                 /*              */
       union {                                  /* PANEL_HSYNC_TIM */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :5;                 /*              */
                    UINT32 HSYNC_START:11;     /*   HSYNC_START */
                    UINT32 :5;                 /*              */
                    UINT32 HSYNC_END:11;       /*   HSYNC_END  */
                    } BIT;                      /*              */
             } PANEL_HSYNC_TIM;                 /*              */
       union {                                  /* VIDEO_VSYNC_TIM2 */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :22;                /*              */
                    UINT32 VIDEO_VSYNC_START2:10;/*   VIDEO_VSYNC_START2 */
                    } BIT;                      /*              */
             } VIDEO_VSYNC_TIM2;                /*              */
       union {                                  /* GRA_VSYNC_TIM */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :22;                /*              */
                    UINT32 GRA_VSYNC_START:10; /*   GRA_VSYNC_START */
                    } BIT;                      /*              */
             } GRA_VSYNC_TIM;                   /*              */
       union {                                  /* AC_LINE_NUM  */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :27;                /*              */
                    UINT32 AC_LINE_NUM:5;      /*   AC_LINE_NUM */
                    } BIT;                      /*              */
             } AC_LINE_NUM;                     /*              */
       UINT8 wk7[8];                           /*              */
       union {                                  /* DE_SIZE      */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 DE_HEIGHT:10;       /*   DE_HEIGHT  */
                    UINT32 :5;                 /*              */
                    UINT32 DE_WIDTH:11;        /*   DE_WIDTH   */
                    } BIT;                      /*              */
             } DE_SIZE;                         /*              */
       union {                                  /* DE_START     */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             struct {                           /*  Byte Access */
                    UINT8 HH;                  /*   High, High */
                    UINT8 HL;                  /*   High, Low  */
                    UINT8 LH;                  /*   Low, High  */
                    UINT8 LL;                  /*   Low, Low   */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT32 :6;                 /*              */
                    UINT32 DE_START_V:10;      /*   DE_START_V */
                    UINT32 :5;                 /*              */
                    UINT32 DE_START_H:11;      /*   DE_START_H */
                    } BIT;                      /*              */
             } DE_START;                        /*              */
};
struct st_src0 {                                /* struct SRC0  */
       union {                                  /* SRCID        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SRCID;                           /*              */
       union {                                  /* SRCOD        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SRCOD;                           /*              */
       union {                                  /* SRCIDCTRL    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 IED:1;               /*   IED        */
                    UINT16 IEN:1;               /*   IEN        */
                    UINT16 :6;                  /*              */
                    UINT16 IFTRG:2;             /*   IFTRG      */
                    } BIT;                      /*              */
             } SRCIDCTRL;                       /*              */
       union {                                  /* SRCODCTRL    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :5;                  /*              */
                    UINT16 OCH:1;               /*   OCH        */
                    UINT16 OED:1;               /*   OED        */
                    UINT16 OEN:1;               /*   OEN        */
                    UINT16 :6;                  /*              */
                    UINT16 OFTRG:2;             /*   OFTRG      */
                    } BIT;                      /*              */
             } SRCODCTRL;                       /*              */
       union {                                  /* SRCCTRL      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :2;                  /*              */
                    UINT16 CEEN:1;              /*   CEEN       */
                    UINT16 SRCEN:1;             /*   SRCEN      */
                    UINT16 UDEN:1;              /*   UDEN       */
                    UINT16 OVEN:1;              /*   OVEN       */
                    UINT16 FL:1;                /*   FL         */
                    UINT16 CL:1;                /*   CL         */
                    UINT16 IFS:4;               /*   IFS        */
                    UINT16 :3;                  /*              */
                    UINT16 OFS:1;               /*   OFS        */
                    } BIT;                      /*              */
             } SRCCTRL;                         /*              */
       union {                                  /* SRCSTAT      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 OFDN:5;              /*   OFDN       */
                    UINT16 IFDN:4;              /*   IFDN       */
                    UINT16 :1;                  /*              */
                    UINT16 CEF:1;               /*   CEF        */
                    UINT16 FLF:1;               /*   FLF        */
                    UINT16 UDF:1;               /*   UDF        */
                    UINT16 OVF:1;               /*   OVF        */
                    UINT16 IINT:1;              /*   IINT       */
                    UINT16 OINT:1;              /*   OINT       */
                    } BIT;                      /*              */
             } SRCSTAT;                         /*              */
};                                              /*              */
struct st_src1 {                                /* struct SRC1  */
       union {                                  /* SRCID        */
             UINT16 WORD;                       /*  Word Access */
             } SRCID;                           /*              */
       UINT8 wk0[2];                           /*              */
       union {                                  /* SRCOD        */
             UINT32 LONG;                      /*  Long Access */
             struct {                           /*  Word Access */
                    UINT16 H;                   /*   High       */
                    UINT16 L;                   /*   Low        */
                    } WORD;                     /*              */
             } SRCOD;                           /*              */
       union {                                  /* SRCIDCTRL    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :6;                  /*              */
                    UINT16 IED:1;               /*   IED        */
                    UINT16 IEN:1;               /*   IEN        */
                    UINT16 :6;                  /*              */
                    UINT16 IFTRG:2;             /*   IFTRG      */
                    } BIT;                      /*              */
             } SRCIDCTRL;                       /*              */
       union {                                  /* SRCODCTRL    */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :5;                  /*              */
                    UINT16 OCH:1;               /*   OCH        */
                    UINT16 OED:1;               /*   OED        */
                    UINT16 OEN:1;               /*   OEN        */
                    UINT16 :6;                  /*              */
                    UINT16 OFTRG:2;             /*   OFTRG      */
                    } BIT;                      /*              */
             } SRCODCTRL;                       /*              */
       union {                                  /* SRCCTRL      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :2;                  /*              */
                    UINT16 CEEN:1;              /*   CEEN       */
                    UINT16 SRCEN:1;             /*   SRCEN      */
                    UINT16 UDEN:1;              /*   UDEN       */
                    UINT16 OVEN:1;              /*   OVEN       */
                    UINT16 FL:1;                /*   FL         */
                    UINT16 CL:1;                /*   CL         */
                    UINT16 :7;                  /*              */
                    UINT16 OFS:1;               /*   OFS        */
                    } BIT;                      /*              */
             } SRCCTRL;                         /*              */
       union {                                  /* SRCSTAT      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :2;                  /*              */
                    UINT16 OFDN:3;              /*   OFDN       */
                    UINT16 IFDN:4;              /*   IFDN       */
                    UINT16 :1;                  /*              */
                    UINT16 CEF:1;               /*   CEF        */
                    UINT16 FLF:1;               /*   FLF        */
                    UINT16 UDF:1;               /*   UDF        */
                    UINT16 OVF:1;               /*   OVF        */
                    UINT16 IINT:1;              /*   IINT       */
                    UINT16 OINT:1;              /*   OINT       */
                    } BIT;                      /*              */
             } SRCSTAT;                         /*              */
};                                              /*              */
struct st_dcu {                                 /* struct DCU   */
       union {                                  /* DCUCR        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 DCUE:1;              /*   DCUE       */
                    UINT8 DCUEEN:1;            /*   DCUEEN     */
                    UINT8 IFEN:1;              /*   IFEN       */
                    UINT8 OFEN:1;              /*   OFEN       */
                    UINT8 IDTRG:2;             /*   IDTRG      */
                    UINT8 ODTRG:2;             /*   ODTRG      */
                    } BIT;                      /*              */
             } DCUCR;                           /*              */
       union {                                  /* DCUISR       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 IDFE:1;              /*   IDFE       */
                    UINT8 IFOVF:1;             /*   IFOVF      */
                    UINT8 OFUDF:1;             /*   OFUDF      */
                    UINT8 IFEMP:1;             /*   IFEMP      */
                    UINT8 OFFUL:1;             /*   OFFUL      */
                    } BIT;                      /*              */
             } DCUISR;                          /*              */
       UINT8 wk0[2];                           /*              */
       union {                                  /* DCUID        */
             UINT32 LONG;                      /*  Long Access */
             UINT16 WORD;                       /*  Word Access */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT32 ID:32;              /*   ID         */
                    } BIT;                      /*              */
             } DCUID;                           /*              */
       union {                                  /* DCUOD        */
             UINT32 LONG;                      /*  Long Access */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT32 OD:32;              /*   OD         */
                    } BIT;                      /*              */
             } DCUOD;                           /*              */
       union {                                  /* DCUIFSR      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 IFVD:5;              /*   IFVD       */
                    } BIT;                      /*              */
             } DCUIFSR;                         /*              */
       union {                                  /* DCUOFSR      */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 OFVD:6;              /*   OFVD       */
                    } BIT;                      /*              */
             } DCUOFSR;                         /*              */
};                                              /*              */
struct st_gpio {                                /* struct GPIO  */
       union {                                  /* PAIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :4;                  /*              */
                    UINT8 PA3IOR:1;            /*   PA3IOR     */
                    UINT8 PA2IOR:1;            /*   PA2IOR     */
                    UINT8 PA1IOR:1;            /*   PA1IOR     */
                    UINT8 PA0IOR:1;            /*   PA0IOR     */
                    } BIT;                      /*              */
             } PAIOR0;                          /*              */
       union {                                  /* PADR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 PA3DR:1;             /*   PA3DR      */
                    UINT8 :7;                  /*              */
                    UINT8 PA2DR:1;             /*   PA2DR      */
                    } BIT;                      /*              */
             } PADR1;                           /*              */
       union {                                  /* PADR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 PA1DR:1;             /*   PA1DR      */
                    UINT8 :7;                  /*              */
                    UINT8 PA0DR:1;             /*   PA0DR      */
                    } BIT;                      /*              */
             } PADR0;                           /*              */
       UINT8 wk0[2];                           /*              */
       union {                                  /* PAPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :4;                  /*              */
                    UINT8 PA3PR:1;             /*   PA3PR      */
                    UINT8 PA2PR:1;             /*   PA2PR      */
                    UINT8 PA1PR:1;             /*   PA1PR      */
                    UINT8 PA0PR:1;             /*   PA0PR      */
                    } BIT;                      /*              */
             } PAPR0;                           /*              */
       UINT8 wk1[8];                           /*              */
       union {                                  /* PBCR5        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :6;                  /*              */
                    UINT8 PB22MD:2 ;           /*   PB22MD     */
                    UINT8 :3;                  /*              */
                    UINT8 PB21MD:1;            /*   PB21MD     */
                    UINT8 :3;                  /*              */
                    UINT8 PB20MD:1;            /*   PB20MD     */
                    } BIT;                      /*              */
             } PBCR5;                           /*              */
       union {                                  /* PBCR4        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PB19MD:2;            /*   PB19MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PB18MD:2;            /*   PB18MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PB17MD:2;            /*   PB17MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PB16MD:2;            /*   PB16MD     */
                    } BIT;                      /*              */
             } PBCR4;                           /*              */
       union {                                  /* PBCR3        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PB15MD:2;            /*   PB15MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PB14MD:2;            /*   PB14MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PB13MD:2;            /*   PB13MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PB12MD:2;            /*   PB12MD     */
                    } BIT;                      /*              */
             } PBCR3;                           /*              */
       union {                                  /* PBCR2        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PB11MD:2;            /*   PB11MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PB10MD:2;            /*   PB10MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PB9MD:2;             /*   PB9MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PB8MD:2;             /*   PB8MD      */
                    } BIT;                      /*              */
             } PBCR2;                           /*              */
       union {                                  /* PBCR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PB7MD:2;             /*   PB7MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PB6MD:2;             /*   PB6MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PB5MD:2;             /*   PB5MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PB4MD:2;             /*   PB4MD      */
                    } BIT;                      /*              */
             } PBCR1;                           /*              */
       union {                                  /* PBCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 PB3MD:1;             /*   PB3MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PB2MD:1;             /*   PB2MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PB1MD:1;             /*   PB1MD      */
                    UINT8 :4;                  /*              */
                    } BIT;                      /*              */
             } PBCR0;                           /*              */
       union {                                  /* PBIOR1       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 PB22IOR:1;           /*   PB22IOR    */
                    UINT8 PB21IOR:1;           /*   PB21IOR    */
                    UINT8 PB20IOR:1;           /*   PB20IOR    */
                    UINT8 PB19IOR:1;           /*   PB19IOR    */
                    UINT8 PB18IOR:1;           /*   PB18IOR    */
                    UINT8 PB17IOR:1;           /*   PB17IOR    */
                    UINT8 PB16IOR:1;           /*   PB16IOR    */
                    } BIT;                      /*              */
             } PBIOR1;                          /*              */
       union {                                  /* PBIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PB15IOR:1;           /*   PB15IOR    */
                    UINT8 PB14IOR:1;           /*   PB14IOR    */
                    UINT8 PB13IOR:1;           /*   PB13IOR    */
                    UINT8 PB12IOR:1;           /*   PB12IOR    */
                    UINT8 PB11IOR:1;           /*   PB11IOR    */
                    UINT8 PB10IOR:1;           /*   PB10IOR    */
                    UINT8 PB9IOR:1;            /*   PB9IOR     */
                    UINT8 PB8IOR:1;            /*   PB8IOR     */
                    UINT8 PB7IOR:1;            /*   PB7IOR     */
                    UINT8 PB6IOR:1;            /*   PB6IOR     */
                    UINT8 PB5IOR:1;            /*   PB5IOR     */
                    UINT8 PB4IOR:1;            /*   PB4IOR     */
                    UINT8 PB3IOR:1;            /*   PB3IOR     */
                    UINT8 PB2IOR:1;            /*   PB2IOR     */
                    UINT8 PB1IOR:1;            /*   PB1IOR     */
                    UINT8 :1;                  /*              */
                    } BIT;                      /*              */
             } PBIOR0;                          /*              */
       union {                                  /* PBDR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 PB22DR:1;            /*   PB22DR     */
                    UINT8 PB21DR:1;            /*   PB21DR     */
                    UINT8 PB20DR:1;            /*   PB20DR     */
                    UINT8 PB19DR:1;            /*   PB19DR     */
                    UINT8 PB18DR:1;            /*   PB18DR     */
                    UINT8 PB17DR:1;            /*   PB17DR     */
                    UINT8 PB16DR:1;            /*   PB16DR     */
                    } BIT;                      /*              */
             } PBDR1;                           /*              */
       union {                                  /* PBDR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PB15DR:1;            /*   PB15DR     */
                    UINT8 PB14DR:1;            /*   PB14DR     */
                    UINT8 PB13DR:1;            /*   PB13DR     */
                    UINT8 PB12DR:1;            /*   PB12DR     */
                    UINT8 PB11DR:1;            /*   PB11DR     */
                    UINT8 PB10DR:1;            /*   PB10DR     */
                    UINT8 PB9DR:1;             /*   PB9DR      */
                    UINT8 PB8DR:1;             /*   PB8DR      */
                    UINT8 PB7DR:1;             /*   PB7DR      */
                    UINT8 PB6DR:1;             /*   PB6DR      */
                    UINT8 PB5DR:1;             /*   PB5DR      */
                    UINT8 PB4DR:1;             /*   PB4DR      */
                    UINT8 PB3DR:1;             /*   PB3DR      */
                    UINT8 PB2DR:1;             /*   PB2DR      */
                    UINT8 PB1DR:1;             /*   PB1DR      */
                    UINT8 :1;                  /*              */
                    } BIT;                      /*              */
             } PBDR0;                           /*              */
       union {                                  /* PBPR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :1;                  /*              */
                    UINT8 PB22PR:1;            /*   PB22PR     */
                    UINT8 PB21PR:1;            /*   PB21PR     */
                    UINT8 PB20PR:1;            /*   PB20PR     */
                    UINT8 PB19PR:1;            /*   PB19PR     */
                    UINT8 PB18PR:1;            /*   PB18PR     */
                    UINT8 PB17PR:1;            /*   PB17PR     */
                    UINT8 PB16PR:1;            /*   PB16PR     */
                    } BIT;                      /*              */
             } PBPR1;                           /*              */
       union {                                  /* PBPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PB15PR:1;            /*   PB15PR     */
                    UINT8 PB14PR:1;            /*   PB14PR     */
                    UINT8 PB13PR:1;            /*   PB13PR     */
                    UINT8 PB12PR:1;            /*   PB12PR     */
                    UINT8 PB11PR:1;            /*   PB11PR     */
                    UINT8 PB10PR:1;            /*   PB10PR     */
                    UINT8 PB9PR:1;             /*   PB9PR      */
                    UINT8 PB8PR:1;             /*   PB8PR      */
                    UINT8 PB7PR:1;             /*   PB7PR      */
                    UINT8 PB6PR:1;             /*   PB6PR      */
                    UINT8 PB5PR:1;             /*   PB5PR      */
                    UINT8 PB4PR:1;             /*   PB4PR      */
                    UINT8 PB3PR:1;             /*   PB3PR      */
                    UINT8 PB2PR:1;             /*   PB2PR      */
                    UINT8 PB1PR:1;             /*   PB1PR      */
                    UINT8 :1;                  /*              */
                    } BIT;                      /*              */
             } PBPR0;                           /*              */
       UINT8 wk2[14];                          /*              */
       union {                                  /* PCCR2        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 PC10MD:1;            /*   PC10MD     */
                    UINT8 :3;                  /*              */
                    UINT8 PC9MD:1;             /*   PC9MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PC8MD:2;             /*   PC8MD      */
                    } BIT;                      /*              */
             } PCCR2;                           /*              */
       union {                                  /* PCCR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PC7MD:2;             /*   PC7MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PC6MD:2;             /*   PC6MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PC5MD:2;             /*   PC5MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PC4MD:1;             /*   PC4MD      */
                    } BIT;                      /*              */
             } PCCR1;                           /*              */
       union {                                  /* PCCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 PC3MD:1;             /*   PC3MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PC2MD:1;             /*   PC2MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PC1MD:1;             /*   PC1MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PC0MD:1;             /*   PC0MD      */
                    } BIT;                      /*              */
             } PCCR0;                           /*              */
       UINT8 wk3[2];                           /*              */
       union {                                  /* PCIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 PC10IOR:1;           /*   PC10IOR    */
                    UINT8 PC9IOR:1;            /*   PC9IOR     */
                    UINT8 PC8IOR:1;            /*   PC8IOR     */
                    UINT8 PC7IOR:1;            /*   PC7IOR     */
                    UINT8 PC6IOR:1;            /*   PC6IOR     */
                    UINT8 PC5IOR:1;            /*   PC5IOR     */
                    UINT8 PC4IOR:1;            /*   PC4IOR     */
                    UINT8 PC3IOR:1;            /*   PC3IOR     */
                    UINT8 PC2IOR:1;            /*   PC2IOR     */
                    UINT8 PC1IOR:1;            /*   PC1IOR     */
                    UINT8 PC0IOR:1;            /*   PC0IOR     */
                    } BIT;                      /*              */
             } PCIOR0;                          /*              */
       UINT8 wk4[2];                           /*              */
       union {                                  /* PCDR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 PC10DR:1;            /*   PC10DR     */
                    UINT8 PC9DR:1;             /*   PC9DR      */
                    UINT8 PC8DR:1;             /*   PC8DR      */
                    UINT8 PC7DR:1;             /*   PC7DR      */
                    UINT8 PC6DR:1;             /*   PC6DR      */
                    UINT8 PC5DR:1;             /*   PC5DR      */
                    UINT8 PC4DR:1;             /*   PC4DR      */
                    UINT8 PC3DR:1;             /*   PC3DR      */
                    UINT8 PC2DR:1;             /*   PC2DR      */
                    UINT8 PC1DR:1;             /*   PC1DR      */
                    UINT8 PC0DR:1;             /*   PC0DR      */
                    } BIT;                      /*              */
             } PCDR0;                           /*              */
       UINT8 wk5[2];                           /*              */
       union {                                  /* PCPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :5;                  /*              */
                    UINT8 PC10PR:1;            /*   PC10PR     */
                    UINT8 PC9PR:1;             /*   PC9PR      */
                    UINT8 PC8PR:1;             /*   PC8PR      */
                    UINT8 PC7PR:1;             /*   PC7PR      */
                    UINT8 PC6PR:1;             /*   PC6PR      */
                    UINT8 PC5PR:1;             /*   PC5PR      */
                    UINT8 PC4PR:1;             /*   PC4PR      */
                    UINT8 PC3PR:1;             /*   PC3PR      */
                    UINT8 PC2PR:1;             /*   PC2PR      */
                    UINT8 PC1PR:1;             /*   PC1PR      */
                    UINT8 PC0PR:1;             /*   PC0PR      */
                    } BIT;                      /*              */
             } PCPR0;                           /*              */
       UINT8 wk6[12];                          /*              */
       union {                                  /* PDCR3        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PD15MD:2;            /*   PD15MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PD14MD:2;            /*   PD14MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PD13MD:2;            /*   PD13MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PD12MD:2;            /*   PD12MD     */
                    } BIT;                      /*              */
             } PDCR3;                           /*              */
       union {                                  /* PDCR2        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PD11MD:2;            /*   PD11MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PD10MD:2;            /*   PD10MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PD9MD:2;             /*   PD9MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PD8MD:2;             /*   PD8MD      */
                    } BIT;                      /*              */
             } PDCR2;                           /*              */
       union {                                  /* PDCR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PD7MD:2;             /*   PD7MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PD6MD:2;             /*   PD6MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PD5MD:2;             /*   PD5MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PD4MD:2;             /*   PD4MD      */
                    } BIT;                      /*              */
             } PDCR1;                           /*              */
       union {                                  /* PDCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PD3MD:2;             /*   PD3MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PD2MD:2;             /*   PD2MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PD1MD:2;             /*   PD1MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PD0MD:2;             /*   PD0MD      */
                    } BIT;                      /*              */
             } PDCR0;                           /*              */
       UINT8 wk7[2];                           /*              */
       union {                                  /* PDIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PD15IOR:1;           /*   PD15IOR    */
                    UINT8 PD14IOR:1;           /*   PD14IOR    */
                    UINT8 PD13IOR:1;           /*   PD13IOR    */
                    UINT8 PD12IOR:1;           /*   PD12IOR    */
                    UINT8 PD11IOR:1;           /*   PD11IOR    */
                    UINT8 PD10IOR:1;           /*   PD10IOR    */
                    UINT8 PD9IOR:1;            /*   PD9IOR     */
                    UINT8 PD8IOR:1;            /*   PD8IOR     */
                    UINT8 PD7IOR:1;            /*   PD7IOR     */
                    UINT8 PD6IOR:1;            /*   PD6IOR     */
                    UINT8 PD5IOR:1;            /*   PD5IOR     */
                    UINT8 PD4IOR:1;            /*   PD4IOR     */
                    UINT8 PD3IOR:1;            /*   PD3IOR     */
                    UINT8 PD2IOR:1;            /*   PD2IOR     */
                    UINT8 PD1IOR:1;            /*   PD1IOR     */
                    UINT8 PD0IOR:1;            /*   PD0IOR     */
                    } BIT;                      /*              */
             } PDIOR0;                          /*              */
       UINT8 wk8[2];                           /*              */
       union {                                  /* PDDR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PD15DR:1;            /*   PD15DR     */
                    UINT8 PD14DR:1;            /*   PD14DR     */
                    UINT8 PD13DR:1;            /*   PD13DR     */
                    UINT8 PD12DR:1;            /*   PD12DR     */
                    UINT8 PD11DR:1;            /*   PD11DR     */
                    UINT8 PD10DR:1;            /*   PD10DR     */
                    UINT8 PD9DR:1;             /*   PD9DR      */
                    UINT8 PD8DR:1;             /*   PD8DR      */
                    UINT8 PD7DR:1;             /*   PD7DR      */
                    UINT8 PD6DR:1;             /*   PD6DR      */
                    UINT8 PD5DR:1;             /*   PD5DR      */
                    UINT8 PD4DR:1;             /*   PD4DR      */
                    UINT8 PD3DR:1;             /*   PD3DR      */
                    UINT8 PD2DR:1;             /*   PD2DR      */
                    UINT8 PD1DR:1;             /*   PD1DR      */
                    UINT8 PD0DR:1;             /*   PD0DR      */
                    } BIT;                      /*              */
             } PDDR0;                           /*              */
       UINT8 wk9[2];                           /*              */
       union {                                  /* PDPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PD15PR:1;            /*   PD15PR     */
                    UINT8 PD14PR:1;            /*   PD14PR     */
                    UINT8 PD13PR:1;            /*   PD13PR     */
                    UINT8 PD12PR:1;            /*   PD12PR     */
                    UINT8 PD11PR:1;            /*   PD11PR     */
                    UINT8 PD10PR:1;            /*   PD10PR     */
                    UINT8 PD9PR:1;             /*   PD9PR      */
                    UINT8 PD8PR:1;             /*   PD8PR      */
                    UINT8 PD7PR:1;             /*   PD7PR      */
                    UINT8 PD6PR:1;             /*   PD6PR      */
                    UINT8 PD5PR:1;             /*   PD5PR      */
                    UINT8 PD4PR:1;             /*   PD4PR      */
                    UINT8 PD3PR:1;             /*   PD3PR      */
                    UINT8 PD2PR:1;             /*   PD2PR      */
                    UINT8 PD1PR:1;             /*   PD1PR      */
                    UINT8 PD0PR:1;             /*   PD0PR      */
                    } BIT;                      /*              */
             } PDPR0;                           /*              */
       UINT8 wk10[16];                         /*              */
       union {                                  /* PECR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :2;                  /*              */
                    UINT8 PE5MD:2;             /*   PE5MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PE4MD:2;             /*   PE4MD      */
                    } BIT;                      /*              */
             } PECR1;                           /*              */
       union {                                  /* PECR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PE3MD:2;             /*   PE3MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PE2MD:2;             /*   PE2MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PE1MD:3;             /*   PE1MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PE0MD:2;             /*   PE0MD      */
                    } BIT;                      /*              */
             } PECR0;                           /*              */
       UINT8 wk11[2];                          /*              */
       union {                                  /* PEIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :2;                  /*              */
                    UINT8 PE5IOR:1;            /*   PE5IOR     */
                    UINT8 PE4IOR:1;            /*   PE4IOR     */
                    UINT8 PE3IOR:1;            /*   PE3IOR     */
                    UINT8 PE2IOR:1;            /*   PE2IOR     */
                    UINT8 PE1IOR:1;            /*   PE1IOR     */
                    UINT8 PE0IOR:1;            /*   PE0IOR     */
                    } BIT;                      /*              */
             } PEIOR0;                          /*              */
       UINT8 wk12[2];                          /*              */
       union {                                  /* PEDR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :2;                  /*              */
                    UINT8 PE5DR:1;             /*   PE5DR      */
                    UINT8 PE4DR:1;             /*   PE4DR      */
                    UINT8 PE3DR:1;             /*   PE3DR      */
                    UINT8 PE2DR:1;             /*   PE2DR      */
                    UINT8 PE1DR:1;             /*   PE1DR      */
                    UINT8 PE0DR:1;             /*   PE0DR      */
                    } BIT;                      /*              */
             } PEDR0;                           /*              */
       UINT8 wk13[2];                          /*              */
       union {                                  /* PEPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :2;                  /*              */
                    UINT8 PE5PR:1;             /*   PE5PR      */
                    UINT8 PE4PR:1;             /*   PE4PR      */
                    UINT8 PE3PR:1;             /*   PE3PR      */
                    UINT8 PE2PR:1;             /*   PE2PR      */
                    UINT8 PE1PR:1;             /*   PE1PR      */
                    UINT8 PE0PR:1;             /*   PE0PR      */
                    } BIT;                      /*              */
             } PEPR0;                           /*              */
       UINT8 wk14[12];                         /*              */
       union {                                  /* PFCR3        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :5;                  /*              */
                    UINT8 PF12MD:3;            /*   PF12MD     */
                    } BIT;                      /*              */
             } PFCR3;                           /*              */
       union {                                  /* PFCR2        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 PF11MD:3;            /*   PF11MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PF10MD:3;            /*   PF10MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PF9MD:3;             /*   PF9MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PF8MD:2;             /*   PF8MD      */
                    } BIT;                      /*              */
             } PFCR2;                           /*              */
       union {                                  /* PFCR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 PF7MD:3;             /*   PF7MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PF6MD:3;             /*   PF6MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PF5MD:3;             /*   PF5MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PF4MD:3;             /*   PF4MD      */
                    } BIT;                      /*              */
             } PFCR1;                           /*              */
       union {                                  /* PFCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 PF3MD:3;             /*   PF3MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PF2MD:3;             /*   PF2MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PF1MD:3;             /*   PF1MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PF0MD:3;             /*   PF0MD      */
                    } BIT;                      /*              */
             } PFCR0;                           /*              */
       UINT8 wk15[2];                          /*              */
       union {                                  /* PFIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 PF12IOR:1;           /*   PF12IOR    */
                    UINT8 PF11IOR:1;           /*   PF11IOR    */
                    UINT8 PF10IOR:1;           /*   PF10IOR    */
                    UINT8 PF9IOR:1;            /*   PF9IOR     */
                    UINT8 PF8IOR:1;            /*   PF8IOR     */
                    UINT8 PF7IOR:1;            /*   PF7IOR     */
                    UINT8 PF6IOR:1;            /*   PF6IOR     */
                    UINT8 PF5IOR:1;            /*   PF5IOR     */
                    UINT8 PF4IOR:1;            /*   PF4IOR     */
                    UINT8 PF3IOR:1;            /*   PF3IOR     */
                    UINT8 PF2IOR:1;            /*   PF2IOR     */
                    UINT8 PF1IOR:1;            /*   PF1IOR     */
                    UINT8 PF0IOR:1;            /*   PF0IOR     */
                    } BIT;                      /*              */
             } PFIOR0;                          /*              */
       UINT8 wk16[2];                          /*              */
       union {                                  /* PFDR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 PF12DR:1;            /*   PF12DR     */
                    UINT8 PF11DR:1;            /*   PF11DR     */
                    UINT8 PF10DR:1;            /*   PF10DR     */
                    UINT8 PF9DR:1;             /*   PF9DR      */
                    UINT8 PF8DR:1;             /*   PF8DR      */
                    UINT8 PF7DR:1;             /*   PF7DR      */
                    UINT8 PF6DR:1;             /*   PF6DR      */
                    UINT8 PF5DR:1;             /*   PF5DR      */
                    UINT8 PF4DR:1;             /*   PF4DR      */
                    UINT8 PF3DR:1;             /*   PF3DR      */
                    UINT8 PF2DR:1;             /*   PF2DR      */
                    UINT8 PF1DR:1;             /*   PF1DR      */
                    UINT8 PF0DR:1;             /*   PF0DR      */
                    } BIT;                      /*              */
             } PFDR0;                           /*              */
       UINT8 wk17[2];                          /*              */
       union {                                  /* PFPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 PF12PR:1;            /*   PF12PR     */
                    UINT8 PF11PR:1;            /*   PF11PR     */
                    UINT8 PF10PR:1;            /*   PF10PR     */
                    UINT8 PF9PR:1;             /*   PF9PR      */
                    UINT8 PF8PR:1;             /*   PF8PR      */
                    UINT8 PF7PR:1;             /*   PF7PR      */
                    UINT8 PF6PR:1;             /*   PF6PR      */
                    UINT8 PF5PR:1;             /*   PF5PR      */
                    UINT8 PF4PR:1;             /*   PF4PR      */
                    UINT8 PF3PR:1;             /*   PF3PR      */
                    UINT8 PF2PR:1;             /*   PF2PR      */
                    UINT8 PF1PR:1;             /*   PF1PR      */
                    UINT8 PF0PR:1;             /*   PF0PR      */
                    } BIT;                      /*              */
             } PFPR0;                           /*              */
       UINT8 wk18[4];                          /*              */
       union {                                  /* PGCR7        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :5;                  /*              */
                    UINT8 PG0MD:3;             /*   PG0MD      */
                    } BIT;                      /*              */
             } PGCR7;                           /*              */
       union {                                  /* PGCR6        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 :6;                  /*              */
                    UINT8 PG24MD:2;            /*   PG24MD     */
                    } BIT;                      /*              */
             } PGCR6;                           /*              */
       union {                                  /* PGCR5        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PG23MD:2;            /*   PG23MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PG22MD:2;            /*   PG22MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PG21MD:2;            /*   PG21MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG20MD:3;            /*   PG20MD     */
                    } BIT;                      /*              */
             } PGCR5;                           /*              */
       union {                                  /* PGCR4        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 PG19MD:3;            /*   PG19MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG18MD:3;            /*   PG18MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG17MD:3;            /*   PG17MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG16MD:3;            /*   PG16MD     */
                    } BIT;                      /*              */
             } PGCR4;                           /*              */
       union {                                  /* PGCR3        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 PG15MD:3;            /*   PG15MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG14MD:3;            /*   PG14MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG13MD:3;            /*   PG13MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG12MD:3;            /*   PG12MD     */
                    } BIT;                      /*              */
             } PGCR3;                           /*              */
       union {                                  /* PGCR2        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :1;                  /*              */
                    UINT8 PG11MD:3;            /*   PG11MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG10MD:3;            /*   PG10MD     */
                    UINT8 :1;                  /*              */
                    UINT8 PG9MD:3;             /*   PG9MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PG8MD:3;             /*   PG8MD      */
                    } BIT;                      /*              */
             } PGCR2;                           /*              */
       union {                                  /* PGCR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PG7MD:2;             /*   PG7MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PG6MD:2;             /*   PG6MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PG5MD:2;             /*   PG5MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PG4MD:2;             /*   PG4MD      */
                    } BIT;                      /*              */
             } PGCR1;                           /*              */
       union {                                  /* PGCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PG3MD:2;             /*   PG3MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PG2MD:2;             /*   PG2MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PG1MD:2;             /*   PG1MD      */
                    UINT8 :4;                  /*              */
                    } BIT;                      /*              */
             } PGCR0;                           /*              */
       union {                                  /* PGIOR1       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 PG24IOR:1;           /*   PG24IOR    */
                    UINT8 PG23IOR:1;           /*   PG23IOR    */
                    UINT8 PG22IOR:1;           /*   PG22IOR    */
                    UINT8 PG21IOR:1;           /*   PG21IOR    */
                    UINT8 PG20IOR:1;           /*   PG20IOR    */
                    UINT8 PG19IOR:1;           /*   PG19IOR    */
                    UINT8 PG18IOR:1;           /*   PG18IOR    */
                    UINT8 PG17IOR:1;           /*   PG17IOR    */
                    UINT8 PG16IOR:1;           /*   PG16IOR    */
                    } BIT;                      /*              */
             } PGIOR1;                          /*              */
       union {                                  /* PGIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PG15IOR:1;           /*   PG15IOR    */
                    UINT8 PG14IOR:1;           /*   PG14IOR    */
                    UINT8 PG13IOR:1;           /*   PG13IOR    */
                    UINT8 PG12IOR:1;           /*   PG12IOR    */
                    UINT8 PG11IOR:1;           /*   PG11IOR    */
                    UINT8 PG10IOR:1;           /*   PG10IOR    */
                    UINT8 PG9IOR:1;            /*   PG9IOR     */
                    UINT8 PG8IOR:1;            /*   PG8IOR     */
                    UINT8 PG7IOR:1;            /*   PG7IOR     */
                    UINT8 PG6IOR:1;            /*   PG6IOR     */
                    UINT8 PG5IOR:1;            /*   PG5IOR     */
                    UINT8 PG4IOR:1;            /*   PG4IOR     */
                    UINT8 PG3IOR:1;            /*   PG3IOR     */
                    UINT8 PG2IOR:1;            /*   PG2IOR     */
                    UINT8 PG1IOR:1;            /*   PG1IOR     */
                    UINT8 PG0IOR:1;            /*   PG0IOR     */
                    } BIT;                      /*              */
             } PGIOR0;                          /*              */
       union {                                  /* PGDR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 PG24DR:1;            /*   PG24DR     */
                    UINT8 PG23DR:1;            /*   PG23DR     */
                    UINT8 PG22DR:1;            /*   PG22DR     */
                    UINT8 PG21DR:1;            /*   PG21DR     */
                    UINT8 PG20DR:1;            /*   PG20DR     */
                    UINT8 PG19DR:1;            /*   PG19DR     */
                    UINT8 PG18DR:1;            /*   PG18DR     */
                    UINT8 PG17DR:1;            /*   PG17DR     */
                    UINT8 PG16DR:1;            /*   PG16DR     */
                    } BIT;                      /*              */
             } PGDR1;                           /*              */
       union {                                  /* PGDR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PG15DR:1;            /*   PG15DR     */
                    UINT8 PG14DR:1;            /*   PG14DR     */
                    UINT8 PG13DR:1;            /*   PG13DR     */
                    UINT8 PG12DR:1;            /*   PG12DR     */
                    UINT8 PG11DR:1;            /*   PG11DR     */
                    UINT8 PG10DR:1;            /*   PG10DR     */
                    UINT8 PG9DR:1;             /*   PG9DR      */
                    UINT8 PG8DR:1;             /*   PG8DR      */
                    UINT8 PG7DR:1;             /*   PG7DR      */
                    UINT8 PG6DR:1;             /*   PG6DR      */
                    UINT8 PG5DR:1;             /*   PG5DR      */
                    UINT8 PG4DR:1;             /*   PG4DR      */
                    UINT8 PG3DR:1;             /*   PG3DR      */
                    UINT8 PG2DR:1;             /*   PG2DR      */
                    UINT8 PG1DR:1;             /*   PG1DR      */
                    UINT8 PG0DR:1;             /*   PG0DR      */
                    } BIT;                      /*              */
             } PGDR0;                           /*              */
       union {                                  /* PGPR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :7;                  /*              */
                    UINT8 PG24PR:1;            /*   PG24PR     */
                    UINT8 PG23PR:1;            /*   PG23PR     */
                    UINT8 PG22PR:1;            /*   PG22PR     */
                    UINT8 PG21PR:1;            /*   PG21PR     */
                    UINT8 PG20PR:1;            /*   PG20PR     */
                    UINT8 PG19PR:1;            /*   PG19PR     */
                    UINT8 PG18PR:1;            /*   PG18PR     */
                    UINT8 PG17PR:1;            /*   PG17PR     */
                    UINT8 PG16PR:1;            /*   PG16PR     */
                    } BIT;                      /*              */
             } PGPR1;                           /*              */
       union {                                  /* PGPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 PG15PR:1;            /*   PG15PR     */
                    UINT8 PG14PR:1;            /*   PG14PR     */
                    UINT8 PG13PR:1;            /*   PG13PR     */
                    UINT8 PG12PR:1;            /*   PG12PR     */
                    UINT8 PG11PR:1;            /*   PG11PR     */
                    UINT8 PG10PR:1;            /*   PG10PR     */
                    UINT8 PG9PR:1;             /*   PG9PR      */
                    UINT8 PG8PR:1;             /*   PG8PR      */
                    UINT8 PG7PR:1;             /*   PG7PR      */
                    UINT8 PG6PR:1;             /*   PG6PR      */
                    UINT8 PG5PR:1;             /*   PG5PR      */
                    UINT8 PG4PR:1;             /*   PG4PR      */
                    UINT8 PG3PR:1;             /*   PG3PR      */
                    UINT8 PG2PR:1;             /*   PG2PR      */
                    UINT8 PG1PR:1;             /*   PG1PR      */
                    UINT8 PG0PR:1;             /*   PG0PR      */
                    } BIT;                      /*              */
             } PGPR0;                           /*              */
       UINT8 wk19[16];                         /*              */
       union {                                  /* PHCR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 PH7MD:1;             /*   PH7MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PH6MD:1;             /*   PH6MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PH5MD:1;             /*   PH5MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PH4MD:1;             /*   PH4MD      */
                    } BIT;                      /*              */
             } PHCR1;                           /*              */
       union {                                  /* PHCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :3;                  /*              */
                    UINT8 PH3MD:1;             /*   PH3MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PH2MD:1;             /*   PH2MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PH1MD:1;             /*   PH1MD      */
                    UINT8 :3;                  /*              */
                    UINT8 PH0MD:1;             /*   PH0MD      */
                    } BIT;                      /*              */
             } PHCR0;                           /*              */
       UINT8 wk20[10];                         /*              */
       union {                                  /* PHPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :8;                  /*              */
                    UINT8 PH7PR:1;             /*   PH7PR      */
                    UINT8 PH6PR:1;             /*   PH6PR      */
                    UINT8 PH5PR:1;             /*   PH5PR      */
                    UINT8 PH4PR:1;             /*   PH4PR      */
                    UINT8 PH3PR:1;             /*   PH3PR      */
                    UINT8 PH2PR:1;             /*   PH2PR      */
                    UINT8 PH1PR:1;             /*   PH1PR      */
                    UINT8 PH0PR:1;             /*   PH0PR      */
                    } BIT;                      /*              */
             } PHPR0;                           /*              */
       UINT8 wk21[14];                         /*              */
       union {                                  /* PJCR2        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PJ11MD:2;            /*   PJ11MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PJ10MD:2;            /*   PJ10MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PJ9MD:2;             /*   PJ9MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PJ8MD:2;             /*   PJ8MD      */
                    } BIT;                      /*              */
             } PJCR2;                           /*              */
       union {                                  /* PJCR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PJ7MD:2;             /*   PJ7MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PJ6MD:2;             /*   PJ6MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PJ5MD:2;             /*   PJ5MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PJ4MD:2;             /*   PJ4MD      */
                    } BIT;                      /*              */
             } PJCR1;                           /*              */
       union {                                  /* PJCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PJ3MD:2;             /*   PJ3MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PJ2MD:3;             /*   PJ2MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PJ1MD:3;             /*   PJ1MD      */
                    UINT8 :1;                  /*              */
                    UINT8 PJ0MD:3;             /*   PJ0MD      */
                    } BIT;                      /*              */
             } PJCR0;                           /*              */
       UINT8 wk22[2];                          /*              */
       union {                                  /* PJIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 PJ11IOR:1;           /*   PJ11IOR    */
                    UINT8 PJ10IOR:1;           /*   PJ10IOR    */
                    UINT8 PJ9IOR:1;            /*   PJ9IOR     */
                    UINT8 PJ8IOR:1;            /*   PJ8IOR     */
                    UINT8 PJ7IOR:1;            /*   PJ7IOR     */
                    UINT8 PJ6IOR:1;            /*   PJ6IOR     */
                    UINT8 PJ5IOR:1;            /*   PJ5IOR     */
                    UINT8 PJ4IOR:1;            /*   PJ4IOR     */
                    UINT8 PJ3IOR:1;            /*   PJ3IOR     */
                    UINT8 PJ2IOR:1;            /*   PJ2IOR     */
                    UINT8 PJ1IOR:1;            /*   PJ1IOR     */
                    UINT8 PJ0IOR:1;            /*   PJ0IOR     */
                    } BIT;                      /*              */
             } PJIOR0;                          /*              */
       UINT8 wk23[2];                          /*              */
       union {                                  /* PJDR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 PJ11DR:1;            /*   PJ11DR     */
                    UINT8 PJ10DR:1;            /*   PJ10DR     */
                    UINT8 PJ9DR:1;             /*   PJ9DR      */
                    UINT8 PJ8DR:1;             /*   PJ8DR      */
                    UINT8 PJ7DR:1;             /*   PJ7DR      */
                    UINT8 PJ6DR:1;             /*   PJ6DR      */
                    UINT8 PJ5DR:1;             /*   PJ5DR      */
                    UINT8 PJ4DR:1;             /*   PJ4DR      */
                    UINT8 PJ3DR:1;             /*   PJ3DR      */
                    UINT8 PJ2DR:1;             /*   PJ2DR      */
                    UINT8 PJ1DR:1;             /*   PJ1DR      */
                    UINT8 PJ0DR:1;             /*   PJ0DR      */
                    } BIT;                      /*              */
             } PJDR0;                           /*              */
       UINT8 wk24[2];                          /*              */
       union {                                  /* PJPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 PJ11PR:1;            /*   PJ11PR     */
                    UINT8 PJ10PR:1;            /*   PJ10PR     */
                    UINT8 PJ9PR:1;             /*   PJ9PR      */
                    UINT8 PJ8PR:1;             /*   PJ8PR      */
                    UINT8 PJ7PR:1;             /*   PJ7PR      */
                    UINT8 PJ6PR:1;             /*   PJ6PR      */
                    UINT8 PJ5PR:1;             /*   PJ5PR      */
                    UINT8 PJ4PR:1;             /*   PJ4PR      */
                    UINT8 PJ3PR:1;             /*   PJ3PR      */
                    UINT8 PJ2PR:1;             /*   PJ2PR      */
                    UINT8 PJ1PR:1;             /*   PJ1PR      */
                    UINT8 PJ0PR:1;             /*   PJ0PR      */
                    } BIT;                      /*              */
             } PJPR0;                           /*              */
       UINT8 wk25[14];                         /*              */
       union {                                  /* PKCR2        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PK11MD:2;            /*   PK11MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PK10MD:2;            /*   PK10MD     */
                    UINT8 :2;                  /*              */
                    UINT8 PK9MD:2;             /*   PK9MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PK8MD:2;             /*   PK8MD      */
                    } BIT;                      /*              */
             } PKCR2;                           /*              */
       union {                                  /* PKCR1        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PK7MD:2;             /*   PK7MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PK6MD:2;             /*   PK6MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PK5MD:2;             /*   PK5MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PK4MD:2;             /*   PK4MD      */
                    } BIT;                      /*              */
             } PKCR1;                           /*              */
       union {                                  /* PKCR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 PK3MD:2;             /*   PK3MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PK2MD:2;             /*   PK2MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PK1MD:2;             /*   PK1MD      */
                    UINT8 :2;                  /*              */
                    UINT8 PK0MD:2;             /*   PK0MD      */
                    } BIT;                      /*              */
             } PKCR0;                           /*              */
       UINT8 wk26[2];                          /*              */
       union {                                  /* PKIOR0       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 PK11IOR:1;           /*   PK11IOR    */
                    UINT8 PK10IOR:1;           /*   PK10IOR    */
                    UINT8 PK9IOR:1;            /*   PK9IOR     */
                    UINT8 PK8IOR:1;            /*   PK8IOR     */
                    UINT8 PK7IOR:1;            /*   PK7IOR     */
                    UINT8 PK6IOR:1;            /*   PK6IOR     */
                    UINT8 PK5IOR:1;            /*   PK5IOR     */
                    UINT8 PK4IOR:1;            /*   PK4IOR     */
                    UINT8 PK3IOR:1;            /*   PK3IOR     */
                    UINT8 PK2IOR:1;            /*   PK2IOR     */
                    UINT8 PK1IOR:1;            /*   PK1IOR     */
                    UINT8 PK0IOR:1;            /*   PK0IOR     */
                    } BIT;                      /*              */
             } PKIOR0;                          /*              */
       UINT8 wk27[2];                          /*              */
       union {                                  /* PKDR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 PK11DR:1;            /*   PK11DR     */
                    UINT8 PK10DR:1;            /*   PK10DR     */
                    UINT8 PK9DR:1;             /*   PK9DR      */
                    UINT8 PK8DR:1;             /*   PK8DR      */
                    UINT8 PK7DR:1;             /*   PK7DR      */
                    UINT8 PK6DR:1;             /*   PK6DR      */
                    UINT8 PK5DR:1;             /*   PK5DR      */
                    UINT8 PK4DR:1;             /*   PK4DR      */
                    UINT8 PK3DR:1;             /*   PK3DR      */
                    UINT8 PK2DR:1;             /*   PK2DR      */
                    UINT8 PK1DR:1;             /*   PK1DR      */
                    UINT8 PK0DR:1;             /*   PK0DR      */
                    } BIT;                      /*              */
             } PKDR0;                           /*              */
       UINT8 wk28[2];                          /*              */
       union {                                  /* PKPR0        */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Byte Access */
                    UINT8 H;                   /*   High       */
                    UINT8 L;                   /*   Low        */
                    } BYTE;                     /*              */
             struct {                           /*  Bit Access  */
                    UINT8 :4;                  /*              */
                    UINT8 PK11PR:1;            /*   PK11PR     */
                    UINT8 PK10PR:1;            /*   PK10PR     */
                    UINT8 PK9PR:1;             /*   PK9PR      */
                    UINT8 PK8PR:1;             /*   PK8PR      */
                    UINT8 PK7PR:1;             /*   PK7PR      */
                    UINT8 PK6PR:1;             /*   PK6PR      */
                    UINT8 PK5PR:1;             /*   PK5PR      */
                    UINT8 PK4PR:1;             /*   PK4PR      */
                    UINT8 PK3PR:1;             /*   PK3PR      */
                    UINT8 PK2PR:1;             /*   PK2PR      */
                    UINT8 PK1PR:1;             /*   PK1PR      */
                    UINT8 PK0PR:1;             /*   PK0PR      */
                    } BIT;                      /*              */
             } PKPR0;                           /*              */
};                                              /*              */
struct st_hudi {                                /* struct HUDI  */
       union {                                  /* SDIR         */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 TI:8;                /*   TI         */
                    UINT16 :8;                  /*              */
                    } BIT;                      /*              */
             } SDIR;                            /*              */
};                                              /*              */
struct st_pwm {                                 /* struct PWM   */
       union {                                  /* PWBTCR       */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 BTC2G:1;             /*   BTC2G      */
                    UINT8 BTC2E:1;             /*   BTC2E      */
                    UINT8 BTC2C:1;             /*   BTC2C      */
                    UINT8 BTC2A:1;             /*   BTC2A      */
                    UINT8 BTC1G:1;             /*   BTC1G      */
                    UINT8 BTC1E:1;             /*   BTC1E      */
                    UINT8 BTC1C:1;             /*   BTC1C      */
                    UINT8 BTC1A:1;             /*   BTC1A      */
                    } BIT;                      /*              */
             } PWBTCR;                          /*              */
       UINT8 wk0[217];                         /*              */
       union {                                  /* PWCR1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 CMF:1;               /*   CMF        */
                    UINT8 CST:1;               /*   CST        */
                    UINT8 CKS:3;               /*   CKS        */
                    } BIT;                      /*              */
             } PWCR1;                           /*              */
       UINT8 wk1[3];                           /*              */
       union {                                  /* PWPR1        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 OPS1H:1;             /*   OPS1H      */
                    UINT8 OPS1G:1;             /*   OPS1G      */
                    UINT8 OPS1F:1;             /*   OPS1F      */
                    UINT8 OPS1E:1;             /*   OPS1E      */
                    UINT8 OPS1D:1;             /*   OPS1D      */
                    UINT8 OPS1C:1;             /*   OPS1C      */
                    UINT8 OPS1B:1;             /*   OPS1B      */
                    UINT8 OPS1A:1;             /*   OPS1A      */
                    } BIT;                      /*              */
             } PWPR1;                           /*              */
       UINT8 wk2[1];                           /*              */
       union {                                  /* PWCYR1       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 PWCY15:1;            /*   PWCY15     */
                    UINT16 PWCY14:1;            /*   PWCY14     */
                    UINT16 PWCY13:1;            /*   PWCY13     */
                    UINT16 PWCY12:1;            /*   PWCY12     */
                    UINT16 PWCY11:1;            /*   PWCY11     */
                    UINT16 PWCY10:1;            /*   PWCY10     */
                    UINT16 PWCY9:1;             /*   PWCY9      */
                    UINT16 PWCY8:1;             /*   PWCY8      */
                    UINT16 PWCY7:1;             /*   PWCY7      */
                    UINT16 PWCY6:1;             /*   PWCY6      */
                    UINT16 PWCY5:1;             /*   PWCY5      */
                    UINT16 PWCY4:1;             /*   PWCY4      */
                    UINT16 PWCY3:1;             /*   PWCY3      */
                    UINT16 PWCY2:1;             /*   PWCY2      */
                    UINT16 PWCY1:1;             /*   PWCY1      */
                    UINT16 PWCY0:1;             /*   PWCY0      */
                    } BIT;                      /*              */
             } PWCYR1;                          /*              */
       union {                                  /* PWBFR1A      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 OTS:1;               /*   OTS        */
                    UINT16 :2;                  /*              */
                    UINT16 DT9:1;               /*   DT9        */
                    UINT16 DT8:1;               /*   DT8        */
                    UINT16 DT7:1;               /*   DT7        */
                    UINT16 DT6:1;               /*   DT6        */
                    UINT16 DT5:1;               /*   DT5        */
                    UINT16 DT4:1;               /*   DT4        */
                    UINT16 DT3:1;               /*   DT3        */
                    UINT16 DT2:1;               /*   DT2        */
                    UINT16 DT1:1;               /*   DT1        */
                    UINT16 DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR1A;                         /*              */
       union {                                  /* PWBFR1C      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 OTS:1;               /*   OTS        */
                    UINT16 :2;                  /*              */
                    UINT16 DT9:1;               /*   DT9        */
                    UINT16 DT8:1;               /*   DT8        */
                    UINT16 DT7:1;               /*   DT7        */
                    UINT16 DT6:1;               /*   DT6        */
                    UINT16 DT5:1;               /*   DT5        */
                    UINT16 DT4:1;               /*   DT4        */
                    UINT16 DT3:1;               /*   DT3        */
                    UINT16 DT2:1;               /*   DT2        */
                    UINT16 DT1:1;               /*   DT1        */
                    UINT16 DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR1C;                         /*              */
       union {                                  /* PWBFR1E      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 OTS:1;               /*   OTS        */
                    UINT16 :2;                  /*              */
                    UINT16 DT9:1;               /*   DT9        */
                    UINT16 DT8:1;               /*   DT8        */
                    UINT16 DT7:1;               /*   DT7        */
                    UINT16 DT6:1;               /*   DT6        */
                    UINT16 DT5:1;               /*   DT5        */
                    UINT16 DT4:1;               /*   DT4        */
                    UINT16 DT3:1;               /*   DT3        */
                    UINT16 DT2:1;               /*   DT2        */
                    UINT16 DT1:1;               /*   DT1        */
                    UINT16 DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR1E;                         /*              */
       union {                                  /* PWBFR1G      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 OTS:1;               /*   OTS        */
                    UINT16 :2;                  /*              */
                    UINT16 DT9:1;               /*   DT9        */
                    UINT16 DT8:1;               /*   DT8        */
                    UINT16 DT7:1;               /*   DT7        */
                    UINT16 DT6:1;               /*   DT6        */
                    UINT16 DT5:1;               /*   DT5        */
                    UINT16 DT4:1;               /*   DT4        */
                    UINT16 DT3:1;               /*   DT3        */
                    UINT16 DT2:1;               /*   DT2        */
                    UINT16 DT1:1;               /*   DT1        */
                    UINT16 DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR1G;                         /*              */
       union {                                  /* PWCR2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 :2;                  /*              */
                    UINT8 IE:1;                /*   IE         */
                    UINT8 CMF:1;               /*   CMF        */
                    UINT8 CST:1;               /*   CST        */
                    UINT8 CKS:3;               /*   CKS        */
                    } BIT;                      /*              */
             } PWCR2;                           /*              */
       UINT8 wk3[3];                           /*              */
       union {                                  /* PWPR2        */
             UINT8 BYTE;                       /*  Byte Access */
             struct {                           /*  Bit Access  */
                    UINT8 OPS2H:1;             /*   OPS2H      */
                    UINT8 OPS2G:1;             /*   OPS2G      */
                    UINT8 OPS2F:1;             /*   OPS2F      */
                    UINT8 OPS2E:1;             /*   OPS2E      */
                    UINT8 OPS2D:1;             /*   OPS2D      */
                    UINT8 OPS2C:1;             /*   OPS2C      */
                    UINT8 OPS2B:1;             /*   OPS2B      */
                    UINT8 OPS2A:1;             /*   OPS2A      */
                    } BIT;                      /*              */
             } PWPR2;                           /*              */
       UINT8 wk4[1];                           /*              */
       union {                                  /* PWCYR2       */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 PWCY15:1;            /*   PWCY15     */
                    UINT16 PWCY14:1;            /*   PWCY14     */
                    UINT16 PWCY13:1;            /*   PWCY13     */
                    UINT16 PWCY12:1;            /*   PWCY12     */
                    UINT16 PWCY11:1;            /*   PWCY11     */
                    UINT16 PWCY10:1;            /*   PWCY10     */
                    UINT16 PWCY9:1;             /*   PWCY9      */
                    UINT16 PWCY8:1;             /*   PWCY8      */
                    UINT16 PWCY7:1;             /*   PWCY7      */
                    UINT16 PWCY6:1;             /*   PWCY6      */
                    UINT16 PWCY5:1;             /*   PWCY5      */
                    UINT16 PWCY4:1;             /*   PWCY4      */
                    UINT16 PWCY3:1;             /*   PWCY3      */
                    UINT16 PWCY2:1;             /*   PWCY2      */
                    UINT16 PWCY1:1;             /*   PWCY1      */
                    UINT16 PWCY0:1;             /*   PWCY0      */
                    } BIT;                      /*              */
             } PWCYR2;                          /*              */
       union {                                  /* PWBFR2A      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 OTS:1;               /*   OTS        */
                    UINT16 :2;                  /*              */
                    UINT16 DT9:1;               /*   DT9        */
                    UINT16 DT8:1;               /*   DT8        */
                    UINT16 DT7:1;               /*   DT7        */
                    UINT16 DT6:1;               /*   DT6        */
                    UINT16 DT5:1;               /*   DT5        */
                    UINT16 DT4:1;               /*   DT4        */
                    UINT16 DT3:1;               /*   DT3        */
                    UINT16 DT2:1;               /*   DT2        */
                    UINT16 DT1:1;               /*   DT1        */
                    UINT16 DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR2A;                         /*              */
       union {                                  /* PWBFR2C      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 OTS:1;               /*   OTS        */
                    UINT16 :2;                  /*              */
                    UINT16 DT9:1;               /*   DT9        */
                    UINT16 DT8:1;               /*   DT8        */
                    UINT16 DT7:1;               /*   DT7        */
                    UINT16 DT6:1;               /*   DT6        */
                    UINT16 DT5:1;               /*   DT5        */
                    UINT16 DT4:1;               /*   DT4        */
                    UINT16 DT3:1;               /*   DT3        */
                    UINT16 DT2:1;               /*   DT2        */
                    UINT16 DT1:1;               /*   DT1        */
                    UINT16 DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR2C;                         /*              */
       union {                                  /* PWBFR2E      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 OTS:1;               /*   OTS        */
                    UINT16 :2;                  /*              */
                    UINT16 DT9:1;               /*   DT9        */
                    UINT16 DT8:1;               /*   DT8        */
                    UINT16 DT7:1;               /*   DT7        */
                    UINT16 DT6:1;               /*   DT6        */
                    UINT16 DT5:1;               /*   DT5        */
                    UINT16 DT4:1;               /*   DT4        */
                    UINT16 DT3:1;               /*   DT3        */
                    UINT16 DT2:1;               /*   DT2        */
                    UINT16 DT1:1;               /*   DT1        */
                    UINT16 DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR2E;                         /*              */
       union {                                  /* PWBFR2G      */
             UINT16 WORD;                       /*  Word Access */
             struct {                           /*  Bit Access  */
                    UINT16 :3;                  /*              */
                    UINT16 OTS:1;               /*   OTS        */
                    UINT16 :2;                  /*              */
                    UINT16 DT9:1;               /*   DT9        */
                    UINT16 DT8:1;               /*   DT8        */
                    UINT16 DT7:1;               /*   DT7        */
                    UINT16 DT6:1;               /*   DT6        */
                    UINT16 DT5:1;               /*   DT5        */
                    UINT16 DT4:1;               /*   DT4        */
                    UINT16 DT3:1;               /*   DT3        */
                    UINT16 DT2:1;               /*   DT2        */
                    UINT16 DT1:1;               /*   DT1        */
                    UINT16 DT0:1;               /*   DT0        */
                    } BIT;                      /*              */
             } PWBFR2G;                         /*              */
};                                              /*              */
#define CPG (*(volatile struct st_cpg *)0xFFFE0010)    /* CPG Address  */
#define INTC (*(volatile struct st_intc *)0xFFFE0800)  /* INTC Address */
#define CCNT (*(volatile struct st_ccnt *)0xFFFC1000)  /* CCNT Address */
#define BSC (*(volatile struct st_bsc *)0xFFFC0000)    /* BSC Address  */
#define DMAC (*(volatile struct st_dmac *)0xFFFE1000)  /* DMAC Address */
#define MTU2 (*(volatile struct st_mtu2 *)0xFFFE4000)  /* MTU2 Address */
#define CMT (*(volatile struct st_cmt *)0xFFFEC000)    /* CMT Address  */
#define CMT0 (*(volatile struct st_cmt0 *)0xFFFEC002)    /* CMT Address  */
#define CMT1 (*(volatile struct st_cmt0 *)0xFFFEC008)    /* CMT Address  */
#define WDT (*(volatile struct st_wdt *)0xFFFE0000)    /* WDT Address  */
#define RTC (*(volatile struct st_rtc *)0xFFFE6000)    /* RTC Address  */
#define SCIF0 (*(volatile struct st_scif02 *)0xFFFE8000)/* SCIF0 Address */
#define SCIF1 (*(volatile struct st_scif13 *)0xFFFE8800)/* SCIF1 Address */
#define SCIF2 (*(volatile struct st_scif02 *)0xFFFE9000)/* SCIF2 Address */
#define SCIF3 (*(volatile struct st_scif13 *)0xFFFE9800)/* SCIF3 Address */
#define SCIF4 (*(volatile struct st_scif4567 *)0xFFFEA000)/* SCIF4 Address */
#define SCIF5 (*(volatile struct st_scif4567 *)0xFFFEA800)/* SCIF5 Address */
#define SCIF6 (*(volatile struct st_scif4567 *)0xFFFEB000)/* SCIF6 Address */
#define SCIF7 (*(volatile struct st_scif4567 *)0xFFFEB800)/* SCIF7 Address */
#define RSPI0 (*(volatile struct st_rspi *)0xFFFF8000)  /* RSPI0 Address */
#define RSPI1 (*(volatile struct st_rspi *)0xFFFF8800)  /* RSPI1 Address */
#define IIC3_0 (*(volatile struct st_iic3 *)0xFFFEE000)/* IIC3_0 Address */
#define IIC3_1 (*(volatile struct st_iic3 *)0xFFFEE400)/* IIC3_1 Address */
#define IIC3_2 (*(volatile struct st_iic3 *)0xFFFEE800)/* IIC3_2 Address */
#define SSIF0 (*(volatile struct st_ssif *)0xFFFF0000)/* SSIF0 Address */
#define SSIF1 (*(volatile struct st_ssif *)0xFFFF0800)/* SSIF1 Address */
#define SSIF2 (*(volatile struct st_ssif *)0xFFFF1000)/* SSIF2 Address */
#define SSIF3 (*(volatile struct st_ssif *)0xFFFF1800)/* SSIF3 Address */
#define SIOF (*(volatile struct st_siof *)0xFFFF4800)  /* SIOF Address */
#define RCAN0 (*(volatile struct st_rcan *)0xFFFE5000) /* RCAN0 Address */
#define RCAN1 (*(volatile struct st_rcan *)0xFFFE5800) /* RCAN1 Address */
#define IEB (*(volatile struct st_ieb *)0xFFFEF000)    /* IEB Address  */
#define SPDIF (*(volatile struct st_spdif *)0xFFFF5800)/* SPDIF Address */
#define ROMDEC (*(volatile struct st_romdec *)0xFFFF9000)/* ROMDEC Address */
#define ADC (*(volatile struct st_adc *)0xFFFF9800)    /* ADC Address  */
#define FLCTL (*(volatile struct st_flctl *)0xFFFF4000)/* FLCTL Address */
#define USB (*(volatile struct st_usb *)0xFFFFC000)    /* USB Address  */
#define VDC3 (*(volatile struct st_vdc3 *)0xFFFF2000)  /* VDC3 Address */
#define SRC0 (*(volatile struct st_src0 *)0xFFFE7000)  /* SRC0 Address */
#define SRC1 (*(volatile struct st_src1 *)0xFFFE7800)  /* SRC1 Address */
#define DCU (*(volatile struct st_dcu *)0xFFFF5000)    /* DCU Address  */
#define PORT (*(volatile struct st_gpio *)0xFFFE3812)  /* GPIO Address */
#define HUDI (*(volatile struct st_hudi *)0xFFFE2000)  /* HUDI Address */
#define PWM (*(volatile struct st_pwm *)0xFFFEF406)    /* PWM Address  */

#endif /* _IODEFINE_H_ */