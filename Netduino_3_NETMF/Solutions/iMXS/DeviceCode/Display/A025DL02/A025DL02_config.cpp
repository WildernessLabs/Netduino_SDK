////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <Drivers\Display\A025DL02\A025DL02.h>

//////////////////////////////////////////////////////////////////////////////


#if (HARDWARE_BOARD_TYPE >= HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2)
    #define SPI_A025DL02_CS                    MC9328MXL_SPI::c_SPI1_SS
    #define SPI_A025DL02_CS_ACTIVE             FALSE
    #define SPI_A025DL02_MSK_IDLE              FALSE
    #define SPI_A025DL02_MSK_SAMPLE_EDGE       TRUE
    #define SPI_A025DL02_16BIT_OP              TRUE
    #define SPI_A025DL02_CLOCK_RATE_KHZ        1500
    #define SPI_A025DL02_CS_SETUP_USEC         0
    #define SPI_A025DL02_CS_HOLD_USEC          0
    #define SPI_A025DL02_MODULE                MC9328MXL_SPI::c_SPI1
    #define SPI_A025DL02_BUSYPIN               GPIO_PIN_NONE
    #define SPI_A025DL02_BUSYPIN_ACTIVESTATE   FALSE
#else
    #define SPI_A025DL02_CS                    GPIO_PIN_NONE
    #define SPI_A025DL02_CS_ACTIVE             FALSE
    #define SPI_A025DL02_MSK_IDLE              FALSE
    #define SPI_A025DL02_MSK_SAMPLE_EDGE       TRUE
    #define SPI_A025DL02_16BIT_OP              TRUE
    #define SPI_A025DL02_CLOCK_RATE_KHZ        1000
    #define SPI_A025DL02_CS_SETUP_USEC         0
    #define SPI_A025DL02_CS_HOLD_USEC          0
    #define SPI_A025DL02_MODULE                0
    #define SPI_A025DL02_BUSYPIN               GPIO_PIN_NONE
    #define SPI_A025DL02_BUSYPIN_ACTIVESTATE   FALSE
#endif

//////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata = "g_A025DL02_SPI_Config"
#endif

SPI_CONFIGURATION g_A025DL02_SPI_Config =
{
    SPI_A025DL02_CS,
    SPI_A025DL02_CS_ACTIVE,
    SPI_A025DL02_MSK_IDLE,
    SPI_A025DL02_MSK_SAMPLE_EDGE,
    SPI_A025DL02_16BIT_OP,
    SPI_A025DL02_CLOCK_RATE_KHZ,
    SPI_A025DL02_CS_SETUP_USEC,
    SPI_A025DL02_CS_HOLD_USEC,
    SPI_A025DL02_MODULE,
    {
        SPI_A025DL02_BUSYPIN,
        SPI_A025DL02_BUSYPIN_ACTIVESTATE,
    },
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section rwdata
#endif
