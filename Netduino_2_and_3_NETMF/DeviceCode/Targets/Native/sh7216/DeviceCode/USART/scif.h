/***************************************************************************
* Tabs 5 9
* The carets in the following lines will match up if tabs are set properly
*   ^   ^   ^   ^   ^   ^   <== tabs
*   ^   ^   ^   ^   ^   ^   <== spaces
***************************************************************************/

////////////////////////////////////////////////////////////////////////////
//  Author:         Brandon Wong
//  Module:         SCIF
//  Version:        0.00
//  File:           scif.h
//  Creation Date:  3/7/2005 6:13PM
//  Last Modified:  12/29/2009 7:00PM by Deepankar
//
//  Summary:        
//
//  Version:        Note:
//  --------        --------------------------------------------------------
//  1.0             Created to make the commsh3.c a little more flexible
////////////////////////////////////////////////////////////////////////////

#ifndef     __SCIF_H__
#define     __SCIF_H__ 1

    #include "types.h"
    
    // SCSMR
    #define SCIF_SMR_CA         (0x80)
    #define SCIF_SMR_CHR        (0x40)
    #define SCIF_SMR_PE         (0x20)
    #define SCIF_SMR_OE         (0x10)
    #define SCIF_SMR_STOP       (0x08)
    #define SCIF_SMR_CKS1       (0x02)
    #define SCIF_SMR_CKS0       (0x01)
    
    // SCSCR
    #define SCIF_SCR_TIE        (0x80)
    #define SCIF_SCR_RIE        (0x40)
    #define SCIF_SCR_TE         (0x20)
    #define SCIF_SCR_RE         (0x10)
    #define SCIF_SCR_MPIE       (0x08)
    #define SCIF_SCR_TEIE       (0x04)
    #define SCIF_SCR_CKE1       (0x02)
    #define SCIF_SCR_CKE0       (0x01)
    
    // SCSSR
    #define SCIF_SR_TDRE        (0x80)
    #define SCIF_SR_RDRF        (0x40)
    #define SCIF_SR_ORER        (0x20)
    #define SCIF_SR_FER         (0x10)
    #define SCIF_SR_PER         (0x08)
    #define SCIF_SR_TEND        (0x04)
    #define SCIF_SR_MPB         (0x02)
    #define SCIF_SR_MPBT        (0x01)
    
    // SCSPTR
    #define SCIF_PTR_EIO        (0x80)
    #define SCIF_PTR_SPB1IO     (0x08)
    #define SCIF_PTR_SPB1DT     (0x04)
    #define SCIF_PTR_SPB0DT     (0x01)
    

    typedef struct
    {
        vuint16_t smr;              // 0x00
        vuint16_t _reserved0;           // 0x02
        vuint8_t  brr;              // 0x04
        vuint8_t  _reserved1[3];        // 0x05 - 0x07
        vuint16_t scr;              // 0x08
        vuint16_t _reserved2;           // 0x0A
        vuint8_t  tdr;              // 0x0C
        vuint8_t  _reserved3[3];        // 0x0D - 0x0F
        vuint16_t sr;                   // 0x10
        vuint16_t _reserved4;           // 0x12
        vuint8_t  rdr;              // 0x14
        vuint8_t  _reserved5[3];        // 0x15 - 0x17
        vuint16_t fcr,              // 0x18
                  _reserved6,           // 0x1A
                  fdr,              // 0x1C
                  _reserved7,           // 0x1E
                  sptr,             // 0x20
                  _reserved8,           // 0x22
                  lsr;              // 0x24
//      vuint32_t * _reserved9[0x3F6]   // 0x26 - 0x0FFC
    }scif_t;

/*///////////////////////////////////////////////////////////////////////////*/
/* Baud rate generator macros*/
/*///////////////////////////////////////////////////////////////////////////*/

/*  CKS 1:0 :   00  01  10  11*/
/*----------------------------*/
/*      cks :   0.5 2   8   32*/

#define cks .5  // 2^(2*n-1).. with n = 0
#define MHZ         50

#define ckf ( (unsigned)((float)64*cks) )
//#define ckf ( (unsigned)((float)8*cks) )      // Biscayne Changed Multiplier from 64 to 8 - synchronous

#define BRR_CALC(bps) (  (unsigned)(  ( pClock / (ckf*bps) )-1  )   )

#define xtal ( (float)MHZ*1e6 )

#define pClock ((unsigned long)xtal)
//#define pClock ((unsigned long)(xtal/2))  // pclock is half the bus speed


#endif