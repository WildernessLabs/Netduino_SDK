////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

/***************************************************************************/

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_MC9328MXL_SPI_Driver"
#endif

MC9328MXL_SPI_Driver g_MC9328MXL_SPI_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

//#define DEBUG_SPI  1

#if defined(DEBUG_SPI)
#define SPI_DEBUG_PRINT(x)  hal_printf(x);
#define SPI_LOOP_BACK   1
#else
#define SPI_DEBUG_PRINT(x)
#define SPI_LOOP_BACK   0
#endif


//---------------------------//

BOOL MC9328MXL_SPI_Driver::Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();

    // Start by disabling the modules, since it does not happen on reset
    for (int i=0; i<MC9328MXL_SPI::c_MAX_SPI;i++)
    {
        if(g_MC9328MXL_SPI_Driver.m_initialized[i] == FALSE)
        {
            MC9328MXL::SPI(i).CONTROLREG &= (~MC9328MXL_SPI::CONTROLREG_SPIEN);     //Disable SPI Module, don't care the other bit.

            g_MC9328MXL_SPI_Driver.m_initialized[i] = TRUE;
        }
    }

    // set up the GPIOs, always set them in the right state, as some other peripheral could have stolen them
    
    // should not put the SPI pins to any state, as it is unknown what user has connected it to.
    // put pins as in input would be the safest state.
    // SPI 1
    CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI1_SCLK, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );        
    CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI1_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
    CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI1_MOSI, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );

    #if !defined(PLATFORM_ARM_MC9328MXS)
    // SPI 2 
    CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI2_SCLK, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );        
    CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI2_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
    CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI2_MOSI, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
    #endif
    
    return TRUE;
}


void MC9328MXL_SPI_Driver::Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    // Do not disable the Periperal clock, as it is used for LCD 
    for (int i=0; i<MC9328MXL_SPI::c_MAX_SPI;i++)
    {
        if(g_MC9328MXL_SPI_Driver.m_initialized[i] == TRUE)
        {
            MC9328MXL::SPI(i).CONTROLREG &= (~MC9328MXL_SPI::CONTROLREG_SPIEN);     //Disable SPI Module, don't care the other bit.

            g_MC9328MXL_SPI_Driver.m_initialized[i] = FALSE;
        }
    }
}

/***************************************************************************/

void MC9328MXL_SPI_Driver::ISR( void* Param )
{
    ASSERT(0);
}

BOOL MC9328MXL_SPI_Driver::nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    SPI_DEBUG_PRINT(" spi write 16 \r\n");

    if(g_MC9328MXL_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        lcd_printf("\fSPI Xaction 1\r\n");
        hal_printf("\fSPI Xaction 1\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(Configuration.SPI_mod > MC9328MXL_SPI::c_MAX_SPI)
    {
        lcd_printf("\fSPI wrong mod\r\n");
        hal_printf("\fSPI wrong mod\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(!Xaction_Start( Configuration )) return FALSE;
    {
        SPI_XACTION_16 Transaction;

        Transaction.Read16          = Read16;
        Transaction.ReadCount       = ReadCount;
        Transaction.ReadStartOffset = ReadStartOffset;
        Transaction.Write16         = Write16;
        Transaction.WriteCount      = WriteCount;
        Transaction.SPI_mod         = Configuration.SPI_mod;
        Transaction.BusyPin         = Configuration.BusyPin;

        if(!Xaction_nWrite16_nRead16( Transaction )) return FALSE;
    }

    SPI_DEBUG_PRINT(" B4 xs 16 \r\n");
    
    return Xaction_Stop( Configuration );
}

BOOL MC9328MXL_SPI_Driver::nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();

    SPI_DEBUG_PRINT(" spi write 8 \r\n");
    if(g_MC9328MXL_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        lcd_printf("\fSPI Xaction 1\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(Configuration.SPI_mod > MC9328MXL_SPI::c_MAX_SPI)
    {
        lcd_printf("\fSPI wrong mod\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(!Xaction_Start( Configuration )) return FALSE;

    {
        SPI_XACTION_8 Transaction;

        Transaction.Read8           = Read8;
        Transaction.ReadCount       = ReadCount;
        Transaction.ReadStartOffset = ReadStartOffset;
        Transaction.Write8          = Write8;
        Transaction.WriteCount      = WriteCount;
        Transaction.SPI_mod         = Configuration.SPI_mod;
        Transaction.BusyPin         = Configuration.BusyPin;
        
        if(!Xaction_nWrite8_nRead8( Transaction )) return FALSE;
    }

    return Xaction_Stop( Configuration );
}

//--//

BOOL MC9328MXL_SPI_Driver::Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    if(!g_MC9328MXL_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        g_MC9328MXL_SPI_Driver.m_Enabled[Configuration.SPI_mod] = TRUE;

        UINT32 index = Configuration.SPI_mod;
        
        MC9328MXL_SPI & SPI = MC9328MXL::SPI(index);

        // make sure we didn't start one in the middle of an existing transaction
        if( SPI.CONTROLREG & SPI.CONTROLREG_SPIEN ) 
        {
            lcd_printf("\fSPI Collision 1\r\n");
            hal_printf("\fSPI Collision 1\r\n");
            HARD_BREAKPOINT();

            return FALSE;
        }

        // first build the mode register
        SPI.CONTROLREG = MC9328MXL_SPI::ConfigurationToMode( Configuration );

        // LCD Controller needs to have Pulse mode enabled
        #if (HARDWARE_BOARD_TYPE >= HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2)
        if(Configuration.DeviceCS == MC9328MXL_SPI::c_SPI1_SS)
        {
            SPI.PERIODREG = 2;

            SPI.CONTROLREG |= MC9328MXL_SPI::CONTROLREG_SSCTL_PULSE;       // Pulse SS between bursts

	    // set the SSPOL to active lo as it is force at the ConfigurationMOde to not trigger SS for other SPI operation
            SPI.CONTROLREG &= (~MC9328MXL_SPI::CONTROLREG_SSPOL_HIGH);       // Pulse SS between bursts
            CPU_GPIO_DisablePin( MC9328MXL_SPI::c_SPI1_SS,RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);

        }
        #endif


        SPI.CONTROLREG |= MC9328MXL_SPI::CONTROLREG_SPIEN;

        #if (SPI_LOOP_BACK)
        // for spi testing
        SPI.TESTREG |= SPI.TESTREG_LBC;
        #endif    

        // everything should be clean and idle
        ASSERT( SPI.TransmitBufferEmpty());
        ASSERT( SPI.ReceiveBufferEmpty());
        ASSERT( SPI.ShiftBufferEmpty  ());

        // make sure not trigger the SS pin for the LCD when doing any SPI activity.
        // but we can have LCD SPI activity, if so, we want the SS be able to drive
        #if (HARDWARE_BOARD_TYPE >= HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2)
        if(Configuration.DeviceCS != MC9328MXL_SPI::c_SPI1_SS)
        {
            CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI1_SS, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
        }
        #endif

        #if defined(PLATFORM_ARM_MC9328MXS)
         // make sure that we don't trigger the SS pin for LCD  
        CPU_GPIO_DisablePin( MC9328MXL_SPI::c_SPI1_SCLK, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);
        CPU_GPIO_DisablePin( MC9328MXL_SPI::c_SPI1_MOSI, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);            
        #else  // MC9328MXL
        if (index == MC9328MXL_SPI::c_SPI1)
        {
            // allow peripheral control of pins
            CPU_GPIO_DisablePin( MC9328MXL_SPI::c_SPI1_SCLK, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);
            CPU_GPIO_DisablePin( MC9328MXL_SPI::c_SPI1_MOSI, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);            
        }
        else
        {
             // SPI2 need to set to Alternate function - AIN
            CPU_GPIO_DisablePin(MC9328MXL_SPI::c_SPI2_SCLK, RESISTOR_DISABLED, MC9328MXL_GPIO::DDIR__OUT, GPIO_ALT_MODE_1);
            CPU_GPIO_DisablePin(MC9328MXL_SPI::c_SPI2_MOSI, RESISTOR_DISABLED, MC9328MXL_GPIO::DDIR__OUT, GPIO_ALT_MODE_1);
        }
        #endif
        
        // first set CS active as soon as clock and data pins are in proper initial state
        if((Configuration.DeviceCS != MC9328MXL_GPIO::c_Pin_None) && (Configuration.DeviceCS != MC9328MXL_SPI::c_SPI1_SS))
        {
            CPU_GPIO_EnableOutputPin( Configuration.DeviceCS, Configuration.CS_Active );
        }

        #if defined(PLATFORM_ARM_MC9328MXS)
        CPU_GPIO_DisablePin( MC9328MXL_SPI::c_SPI1_MISO, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY );
        #else
        if (index == MC9328MXL_SPI::c_SPI1)
        {
           CPU_GPIO_DisablePin( MC9328MXL_SPI::c_SPI1_MISO, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY );
        }
        else
        {
            // set AOUT mode for MISO for SPI 2
            CPU_GPIO_DisablePin(MC9328MXL_SPI::c_SPI2_MISO, RESISTOR_DISABLED,MC9328MXL_GPIO::DDIR__IN ,GPIO_ALT_MODE_4);
            MC9328MXL::SC().FMCR |= MC9328MXL_SC::FMCR__SPI2_RXD_SEL;  
        }
        #endif

        if(Configuration.CS_Setup_uSecs)
        {
            HAL_Time_Sleep_MicroSeconds_InterruptEnabled( Configuration.CS_Setup_uSecs );
        }
    }
    else
    {
        lcd_printf( "\fSPI Collision 3\r\n" );
        HARD_BREAKPOINT();

        return FALSE;
    }

    return TRUE;
}


BOOL MC9328MXL_SPI_Driver::Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    if(g_MC9328MXL_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        UINT32 index = Configuration.SPI_mod;
        
        MC9328MXL_SPI & SPI = MC9328MXL::SPI(index);

        // we should have cleared the last TBF on the last RBF true setting
        // we should never bring CS inactive with the shifter busy
        ASSERT( SPI.TransmitBufferEmpty());
        ASSERT( SPI.ReceiveBufferEmpty ());
        ASSERT( SPI.ShiftBufferEmpty   ());

        if(Configuration.CS_Hold_uSecs)
        {
            HAL_Time_Sleep_MicroSeconds_InterruptEnabled( Configuration.CS_Hold_uSecs );
        }


          
        // avoid noise drawing excess power on a floating line by putting this in pulldown
        #if defined(PLATFORM_ARM_MC9328MXS)
        CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI1_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
        #else
        if (index == MC9328MXL_SPI::c_SPI1)
        {
            CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI1_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
        }
        else
        {
            CPU_GPIO_EnableInputPin( MC9328MXL_SPI::c_SPI2_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
        }
        #endif
        
        // next, bring the CS to the proper inactive state
        if((Configuration.DeviceCS != MC9328MXL_GPIO::c_Pin_None) && (Configuration.DeviceCS != MC9328MXL_SPI::c_SPI1_SS))
        {
            CPU_GPIO_SetPinState( Configuration.DeviceCS, !Configuration.CS_Active );
        }

        // make sure that we don't trigger the SS pin for LCD on the iMXS board, this may change if Freescale change their circuitry
        // can remove if not use on imxs platform
        // be safe, as LCD SPI access will use this function as well, set it back to Output 
        #if (HARDWARE_BOARD_TYPE >= HARDWARE_BOARD_i_MXS_DEMO_REV_V1_2)
        CPU_GPIO_EnableOutputPin( MC9328MXL_SPI::c_SPI1_SS, TRUE );        

        #endif        

        // put pins in output low state when not in use to avoid noise since clock stop and reset will cause these to become inputs
        #if defined(PLATFORM_ARM_MC9328MXS)
        CPU_GPIO_EnableOutputPin( MC9328MXL_SPI::c_SPI1_SCLK, FALSE );
        CPU_GPIO_EnableOutputPin( MC9328MXL_SPI::c_SPI1_MOSI, FALSE );
        #else
        if (index == MC9328MXL_SPI::c_SPI1)
        {
            CPU_GPIO_EnableOutputPin( MC9328MXL_SPI::c_SPI1_SCLK, FALSE );
            CPU_GPIO_EnableOutputPin( MC9328MXL_SPI::c_SPI1_MOSI, FALSE );
        }
        else
        {
            CPU_GPIO_EnableOutputPin( MC9328MXL_SPI::c_SPI2_SCLK, FALSE );
            CPU_GPIO_EnableOutputPin( MC9328MXL_SPI::c_SPI2_MOSI, FALSE );
        }
        #endif

        // off SPI module
        SPI.CONTROLREG &=  ~SPI.CONTROLREG_SPIEN;

        g_MC9328MXL_SPI_Driver.m_Enabled[Configuration.SPI_mod] = FALSE;
    }
    else
    {
        lcd_printf("\fSPI Collision 4\r\n");
        HARD_BREAKPOINT();

        return FALSE;
    }

    return TRUE;
}


BOOL MC9328MXL_SPI_Driver::Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    UINT16 Data16;

    if(!g_MC9328MXL_SPI_Driver.m_Enabled[ Transaction.SPI_mod ])
    {
        hal_printf("\fSPI Xaction OFF\r\n");
        lcd_printf("\fSPI Xaction OFF\r\n");
        ASSERT(FALSE);
        return FALSE;
    }

    SPI_DEBUG_PRINT("\fxwr16\r\n");
    MC9328MXL_SPI& SPI = MC9328MXL::SPI( Transaction.SPI_mod );

    UINT16* Write16         = Transaction.Write16;
    INT32   WriteCount      = Transaction.WriteCount;
    UINT16* Read16          = Transaction.Read16;
    INT32   ReadCount       = Transaction.ReadCount;
    INT32   ReadStartOffset = Transaction.ReadStartOffset;
    INT32   ReadTotal  = 0;

    // as master, we must always write something before reading or not
    if(WriteCount <= 0)                     { ASSERT(FALSE); return FALSE; }
    if(Write16 == NULL)                     { ASSERT(FALSE); return FALSE; }
    if((ReadCount > 0) && (Read16 == NULL)) { ASSERT(FALSE); return FALSE; }


    if(ReadCount)
    {
        ReadTotal = ReadCount + ReadStartOffset;    // we need to read as many bytes as the buffer is long, plus the offset at which we start
    }


    INT32 loopCnt= ReadTotal;

    // take the max of Read+offset or WrCnt
    if (loopCnt < WriteCount) 
        loopCnt = WriteCount;


    // we will use WriteCount to move in the Write16 array
    // so we do no want to go past the end when we will check for 
    // WriteCount to be bigger than zero
    WriteCount -= 1;

    while(loopCnt--)
    {
        SPI.TXDATAREG = Write16[0];
        SPI.CONTROLREG |= SPI.CONTROLREG_XCH;

        // wait while the transmit buffer is full and receive buffer is empty
        while(!SPI.TransmitBufferEmpty() || !SPI.ShiftBufferEmpty()|| SPI.ReceiveBufferEmpty());

        Data16 = SPI.RXDATAREG;

        // repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
            Write16++;
        }

        // only save data once we have reached ReadCount-1 portion of words
        ReadTotal--;
        if((ReadTotal>=0) && (ReadTotal < ReadCount))
        {

            Read16[0] = Data16;
            Read16++;
        }
       
     }            
        
    return TRUE;
}

BOOL MC9328MXL_SPI_Driver::Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    UINT8 Data8;
    
    if(!g_MC9328MXL_SPI_Driver.m_Enabled[ Transaction.SPI_mod ])
    {
        lcd_printf("\fSPI Xaction OFF\r\n");
        ASSERT(FALSE);
        return FALSE;
    }
    
    MC9328MXL_SPI& SPI = MC9328MXL::SPI( Transaction.SPI_mod );
    
    UINT8* Write8          = Transaction.Write8;
    INT32  WriteCount      = Transaction.WriteCount;
    UINT8* Read8           = Transaction.Read8;
    INT32  ReadCount       = Transaction.ReadCount;
    INT32  ReadStartOffset = Transaction.ReadStartOffset;
    INT32  ReadTotal        = 0;

    // as master, we must always write something before reading or not
    if(WriteCount <= 0)                    { ASSERT(FALSE); return FALSE; }
    if(Write8 == NULL)                     { ASSERT(FALSE); return FALSE; }
    if((ReadCount > 0) && (Read8 == NULL)) { ASSERT(FALSE); return FALSE; }   
    
    if(ReadCount)
    {
        ReadTotal = ReadCount + ReadStartOffset;    // we need to read as many bytes as the buffer is long, plus the offset at which we start
    }

    INT32 loopCnt= ReadTotal;

    // take the max of Read+offset or WrCnt
    if (loopCnt < WriteCount) 
        loopCnt = WriteCount;

    // we will use WriteCount to move in the Write8 array
    // so we do no want to go past the end when we will check for 
    // WriteCount to be bigger than zero
    WriteCount -= 1;
    
    // Start transmission 
    while(loopCnt--)
    {
        SPI.TXDATAREG = Write8[0];
        SPI.CONTROLREG |= SPI.CONTROLREG_XCH; 

        // wait while the transmit buffer is full and receive buffer is empty
        while(!SPI.TransmitBufferEmpty() || !SPI.ShiftBufferEmpty()|| SPI.ReceiveBufferEmpty());

        // reading clears the RBF bit and allows another transfer from the shift register
        Data8 = SPI.RXDATAREG;
        // repeat last write word for all subsequent reads

        if(WriteCount)
        {
            WriteCount--;
            Write8++;
        }

        // only save data once we have reached ReadCount-1 portion of words
        ReadTotal--;
        if((ReadTotal>=0) && (ReadTotal < ReadCount))
        {
            Read8[0] = Data8;
            Read8++;
        }
       
    }
    return TRUE;
}


void MC9328MXL_SPI_Driver::GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();

    switch (spi_mod)
    {
        case (MC9328MXL_SPI::c_SPI1):
            msk  = MC9328MXL_SPI::c_SPI1_SCLK;;
            miso = MC9328MXL_SPI::c_SPI1_MISO;
            mosi = MC9328MXL_SPI::c_SPI1_MOSI;
            
            break;
        case (MC9328MXL_SPI::c_SPI2):
            msk  = MC9328MXL_SPI::c_SPI2_SCLK;;
            miso = MC9328MXL_SPI::c_SPI2_MISO;
            mosi = MC9328MXL_SPI::c_SPI2_MOSI;

            break;
        default:
            break;

    }

}
