////////////////////////////////////////////////////////////////////////////
//  Author:     Brandon Wong
//  Module:     Base Registers
//  Version:        0.00
//  File:       types.h
//  Date:       1/21/02 11:17AM
//
//  Summary:        typedefs for types
////////////////////////////////////////////////////////////////////////////

#ifndef __TYPES_H__
#define __TYPES_H__ 1

//  #define MISMATCH_TYPE 1
//  #include "b_variables.h"

    #if 0
        typedef int                 int32_t;
        typedef short                   int16_t;
        typedef char                    int8_t;

        typedef unsigned int            uint32_t;
        typedef unsigned short          uint16_t;
        typedef unsigned char           uint8_t;
    #endif
    
//  typedef unsigned long long int  uint64_t;
//  typedef double      uint64_t;
    
    typedef unsigned int            uint32_t;
    typedef unsigned short          uint16_t;
    typedef unsigned char           uint8_t;

    typedef volatile int            vint32_t;
    typedef volatile short          vint16_t;
    typedef volatile char           vint8_t;

    typedef volatile unsigned int   vuint32_t;
    typedef volatile unsigned short vuint16_t;
    typedef volatile unsigned char  vuint8_t;

    typedef union
    {
        uint32_t    dword;
        uint16_t    word[2];
        uint8_t     byte[4];
    } converter_t;

//  #define FALSE 0
//  #define TRUE 1

//  typedef uint8_t BOOL;
    
    

#endif