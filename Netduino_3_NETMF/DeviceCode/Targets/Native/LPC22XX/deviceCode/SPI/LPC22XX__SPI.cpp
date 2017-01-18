//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#include <tinyhal.h>
#include "..\LPC22XX.h"

//--//

/***************************************************************************/

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC22XX_SPI_Driver"
#endif

LPC22XX_SPI_Driver g_LPC22XX_SPI_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL CPU_SPI_Initialize()
{
    return LPC22XX_SPI_Driver::Initialize();
}

void CPU_SPI_Uninitialize()
{
    LPC22XX_SPI_Driver::Uninitialize();
}

UINT32 CPU_SPI_PortsCount()
{
   return LPC22XX_SPI::c_MAX_SPI;
}

void CPU_SPI_GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
{
    LPC22XX_SPI_Driver::GetPins(spi_mod,  msk, miso, mosi );
}

BOOL CPU_SPI_nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{
    return LPC22XX_SPI_Driver::nWrite16_nRead16( Configuration, Write16, WriteCount, Read16, ReadCount, ReadStartOffset );
}

BOOL CPU_SPI_nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{
    return LPC22XX_SPI_Driver::nWrite8_nRead8( Configuration, Write8, WriteCount, Read8, ReadCount, ReadStartOffset );
}

BOOL CPU_SPI_Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
    return LPC22XX_SPI_Driver::Xaction_Start( Configuration );
}

BOOL CPU_SPI_Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{
    return LPC22XX_SPI_Driver::Xaction_Stop( Configuration );
}

BOOL CPU_SPI_Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    return LPC22XX_SPI_Driver::Xaction_nWrite16_nRead16( Transaction );
}

BOOL CPU_SPI_Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    return LPC22XX_SPI_Driver::Xaction_nWrite8_nRead8( Transaction );
}


//--//

//---------------------------//

BOOL LPC22XX_SPI_Driver::Initialize()
{
    // allow peripheral control of pins
    CPU_GPIO_DisablePin( LPC22XX_SPI::c_SPI0_SCLK, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
    CPU_GPIO_DisablePin( LPC22XX_SPI::c_SPI0_MOSI, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);            
    CPU_GPIO_DisablePin( LPC22XX_SPI::c_SPI0_MISO, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
    CPU_GPIO_DisablePin( LPC22XX_SPI::c_SPI0_SSEL, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);

    CPU_GPIO_DisablePin(LPC22XX_SPI::c_SPI1_SCLK, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);
    CPU_GPIO_DisablePin(LPC22XX_SPI::c_SPI1_MOSI, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);
    CPU_GPIO_DisablePin(LPC22XX_SPI::c_SPI1_MISO, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);
    CPU_GPIO_DisablePin(LPC22XX_SPI::c_SPI1_SSEL, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);
    return TRUE;
}


void LPC22XX_SPI_Driver::Uninitialize()
{
}



void LPC22XX_SPI_Driver::GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi  )
{
    switch (spi_mod)
    {
        case (LPC22XX_SPI::c_SPI0):
            msk  = LPC22XX_SPI::c_SPI0_SCLK;
            miso = LPC22XX_SPI::c_SPI0_MISO;
            mosi = LPC22XX_SPI::c_SPI0_MOSI;
            
            break;
        case (LPC22XX_SPI::c_SPI1):
            msk  = LPC22XX_SPI::c_SPI1_SCLK;
            miso = LPC22XX_SPI::c_SPI1_MISO;
            mosi = LPC22XX_SPI::c_SPI1_MOSI;

            break;
        default:
            break;

    }

}

/***************************************************************************/

void LPC22XX_SPI_Driver::ISR( void* Param )
{
    ASSERT(0);
}

BOOL LPC22XX_SPI_Driver::nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{

        lcd_printf("\fSPI Peripheral does not support 16 bit transfer\r\n");
        hal_printf("\fSPI Peripheral does not support 16 bit transfer\r\n");
        HARD_BREAKPOINT();
        return FALSE;
}

BOOL LPC22XX_SPI_Driver::nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{
    if(g_LPC22XX_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        lcd_printf("\fSPI Xaction 1\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(Configuration.SPI_mod > LPC22XX_SPI::c_MAX_SPI)
    {
        lcd_printf("\fSPI wrong mod\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if (!Xaction_Start( Configuration )) return FALSE;

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

BOOL LPC22XX_SPI_Driver::Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
    if(!g_LPC22XX_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        g_LPC22XX_SPI_Driver.m_Enabled[Configuration.SPI_mod] = TRUE;

        UINT32 index = Configuration.SPI_mod;
        
        LPC22XX_SPI & SPI = LPC22XX::SPI(index);

        // first build the mode register
        SPI.SPCR = LPC22XX_SPI::ConfigurationToMode( Configuration );
    
        // Set SPI Clock
        SPI.SPCCR = LPC22XX_SPI::c_SPI_Clk_KHz / (Configuration.Clock_RateKHz);
        
        // first set CS active as soon as clock and data pins are in proper initial state
        if(Configuration.DeviceCS != LPC22XX_GPIO::c_Pin_None)
        {
            CPU_GPIO_EnableOutputPin( Configuration.DeviceCS, Configuration.CS_Active );
        }

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


BOOL LPC22XX_SPI_Driver::Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{
    if(g_LPC22XX_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {        
        if(Configuration.CS_Hold_uSecs)
        {
            HAL_Time_Sleep_MicroSeconds_InterruptEnabled( Configuration.CS_Hold_uSecs );
        }
      
        // next, bring the CS to the proper inactive state
        if(Configuration.DeviceCS != LPC22XX_GPIO::c_Pin_None)
        {
            CPU_GPIO_SetPinState( Configuration.DeviceCS, !Configuration.CS_Active );
        }

        g_LPC22XX_SPI_Driver.m_Enabled[Configuration.SPI_mod] = FALSE;
    }
    else
    {
        lcd_printf("\fSPI Collision 4\r\n");
        HARD_BREAKPOINT();

        return FALSE;
    }

    return TRUE;
}


BOOL LPC22XX_SPI_Driver::Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
        lcd_printf("\fSPI Peripheral does not support 16 bit transfer\r\n");
        hal_printf("\fSPI Peripheral does not support 16 bit transfer\r\n");
        HARD_BREAKPOINT();

        return FALSE;
}

BOOL LPC22XX_SPI_Driver::Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    INT32 loop;
    UINT8 Data8;

    if(!g_LPC22XX_SPI_Driver.m_Enabled[Transaction.SPI_mod])
    {
        lcd_printf("\fSPI Xaction OFF\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    LPC22XX_SPI& SPI = LPC22XX::SPI(Transaction.SPI_mod);

    UINT8* Write8          = Transaction.Write8;
    INT32  WriteCount      = Transaction.WriteCount;
    UINT8* Read8           = Transaction.Read8;
    INT32  ReadCount       = Transaction.ReadCount;
    INT32  ReadStartOffset = Transaction.ReadStartOffset;
    INT32  ReadTotal        = 0;

    ASSERT(WriteCount > 0);
    ASSERT(Write8);
    ASSERT(ReadCount == 0 || Read8);

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

    while (loopCnt--)
    {
        // Write Transmit Data
        SPI.SPDR = Write8[0];

        // repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
            Write8++;
        }

        // wait while the Transmission is in progress
        // No error checking as there is no mechanism to report errors
        while((SPI.SPSR & LPC22XX_SPI::SPIF_Mask) == 0x0)
        {
        }
        
        // Read recieved data
        Data8 = SPI.SPDR;

        // Throw away read data until ReadStartOffset is reached 
        ReadTotal--;
        if((ReadTotal>=0) && (ReadTotal < ReadCount))
        {
            Read8[0] = Data8;
            Read8++;
        }

    }

    return TRUE;
}

