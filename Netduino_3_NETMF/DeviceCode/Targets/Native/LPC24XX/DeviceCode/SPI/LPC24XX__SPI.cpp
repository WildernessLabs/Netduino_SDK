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
#include "..\LPC24XX.h"

//--//

/***************************************************************************/

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_SPI_Driver"
#endif

LPC24XX_SPI_Driver g_LPC24XX_SPI_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//--//

BOOL CPU_SPI_Initialize()
{
    return LPC24XX_SPI_Driver::Initialize();
}

void CPU_SPI_Uninitialize()
{
    LPC24XX_SPI_Driver::Uninitialize();
}

UINT32 CPU_SPI_PortsCount()
{
   return LPC24XX_SPI::c_MAX_SPI;
}

void CPU_SPI_GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
{
    LPC24XX_SPI_Driver::GetPins(spi_mod,  msk, miso, mosi );
}

BOOL CPU_SPI_nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{
    return LPC24XX_SPI_Driver::nWrite16_nRead16( Configuration, Write16, WriteCount, Read16, ReadCount, ReadStartOffset );
}

BOOL CPU_SPI_nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{
    return LPC24XX_SPI_Driver::nWrite8_nRead8( Configuration, Write8, WriteCount, Read8, ReadCount, ReadStartOffset );
}

BOOL CPU_SPI_Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
    return LPC24XX_SPI_Driver::Xaction_Start( Configuration );
}

BOOL CPU_SPI_Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{
    return LPC24XX_SPI_Driver::Xaction_Stop( Configuration );
}

BOOL CPU_SPI_Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    return LPC24XX_SPI_Driver::Xaction_nWrite16_nRead16( Transaction );
}

BOOL CPU_SPI_Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    return LPC24XX_SPI_Driver::Xaction_nWrite8_nRead8( Transaction );
}


//--//

//---------------------------//

BOOL LPC24XX_SPI_Driver::Initialize()
{
    // allow peripheral control of pins
    CPU_GPIO_DisablePin( LPC24XX_SPI::c_SPI0_SCLK, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_3);
    CPU_GPIO_DisablePin( LPC24XX_SPI::c_SPI0_MOSI, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_3);            
    CPU_GPIO_DisablePin( LPC24XX_SPI::c_SPI0_MISO, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_3);

    return TRUE;
}


void LPC24XX_SPI_Driver::Uninitialize()
{
}



void LPC24XX_SPI_Driver::GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi  )
{
    switch (spi_mod)
    {
        case (LPC24XX_SPI::c_SPI0):
            msk  = LPC24XX_SPI::c_SPI0_SCLK;
            miso = LPC24XX_SPI::c_SPI0_MISO;
            mosi = LPC24XX_SPI::c_SPI0_MOSI;
            
            break;
        default:
            break;

    }

}

/***************************************************************************/

void LPC24XX_SPI_Driver::ISR( void* Param )
{
    ASSERT(0);
}

BOOL LPC24XX_SPI_Driver::nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{

    if(g_LPC24XX_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        lcd_printf("\fSPI Xaction 1\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(Configuration.SPI_mod > LPC24XX_SPI::c_MAX_SPI)
    {
        lcd_printf("\fSPI wrong mod\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if (!Xaction_Start( Configuration )) return FALSE;

    {
        SPI_XACTION_16 Transaction;

        Transaction.Read16           = Read16;
        Transaction.ReadCount        = ReadCount;
        Transaction.ReadStartOffset  = ReadStartOffset;
        Transaction.Write16          = Write16;
        Transaction.WriteCount       = WriteCount;
        Transaction.SPI_mod          = Configuration.SPI_mod;
        Transaction.BusyPin          = Configuration.BusyPin;
        
        if(!Xaction_nWrite16_nRead16( Transaction )) return FALSE;
    }

    return Xaction_Stop( Configuration );
}

BOOL LPC24XX_SPI_Driver::nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{
    if(g_LPC24XX_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        lcd_printf("\fSPI Xaction 1\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(Configuration.SPI_mod > LPC24XX_SPI::c_MAX_SPI)
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

BOOL LPC24XX_SPI_Driver::Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
    if(!g_LPC24XX_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        g_LPC24XX_SPI_Driver.m_Enabled[Configuration.SPI_mod] = TRUE;

        UINT32 index = Configuration.SPI_mod;
        
        LPC24XX_SPI & SPI = LPC24XX::SPI(index);

        // first build the mode register
        SPI.SPCR = LPC24XX_SPI::ConfigurationToMode( Configuration );
    
        // Set SPI Clock
        SPI.SPCCR = LPC24XX_SPI::c_SPI_Clk_KHz / (Configuration.Clock_RateKHz);
        
        // first set CS active as soon as clock and data pins are in proper initial state
        if(Configuration.DeviceCS != LPC24XX_GPIO::c_Pin_None)
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


BOOL LPC24XX_SPI_Driver::Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{
    if(g_LPC24XX_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {        
        if(Configuration.CS_Hold_uSecs)
        {
            HAL_Time_Sleep_MicroSeconds_InterruptEnabled( Configuration.CS_Hold_uSecs );
        }
      
        // next, bring the CS to the proper inactive state
        if(Configuration.DeviceCS != LPC24XX_GPIO::c_Pin_None)
        {
            CPU_GPIO_SetPinState( Configuration.DeviceCS, !Configuration.CS_Active );
        }

        g_LPC24XX_SPI_Driver.m_Enabled[Configuration.SPI_mod] = FALSE;
    }
    else
    {
        lcd_printf("\fSPI Collision 4\r\n");
        HARD_BREAKPOINT();

        return FALSE;
    }

    return TRUE;
}


BOOL LPC24XX_SPI_Driver::Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    UINT16 Data16;

    if(!g_LPC24XX_SPI_Driver.m_Enabled[Transaction.SPI_mod])
    {
        lcd_printf("\fSPI Xaction OFF\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    LPC24XX_SPI& SPI = LPC24XX::SPI(Transaction.SPI_mod);

    // Enable 16 bit mode
    SPI.SPCR |= LPC24XX_SPI::CONTROLREG_BitEnable;

    UINT16* Write16         = Transaction.Write16;
    INT32   WriteCount      = Transaction.WriteCount;
    UINT16* Read16          = Transaction.Read16;
    INT32   ReadCount       = Transaction.ReadCount;
    INT32   ReadStartOffset = Transaction.ReadStartOffset;
    INT32   ReadTotal  = 0;

    ASSERT(WriteCount > 0);
    ASSERT(Write16);
    ASSERT(ReadCount == 0 || Read16);


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
        // Write Transmit Data
        SPI.SPDR = Write16[0];

        // repeat last write word for all subsequent reads
        if(WriteCount)
        {
            WriteCount--;
            Write16++;
        }

        // wait while the Transmission is in progress
        // No error checking as there is no mechanism to report errors
        while((SPI.SPSR & LPC24XX_SPI::SPIF_Mask) == 0x0)
        {
        }
        
        // Read recieved data
        Data16 = SPI.SPDR;

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

BOOL LPC24XX_SPI_Driver::Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    UINT8 Data8;

    if(!g_LPC24XX_SPI_Driver.m_Enabled[Transaction.SPI_mod])
    {
        lcd_printf("\fSPI Xaction OFF\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    LPC24XX_SPI& SPI = LPC24XX::SPI(Transaction.SPI_mod);

    // Enable 8 bit mode
    SPI.SPCR &= ~(LPC24XX_SPI::CONTROLREG_BitEnable);

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
    
    // Start transmission 
    while(loopCnt--)

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
        while((SPI.SPSR & LPC24XX_SPI::SPIF_Mask) == 0x0)
        {
        }
        
        // Read recieved data
        Data8 = SPI.SPDR;

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
