////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#undef TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

/***************************************************************************/

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_AT91_SPI_Driver"
#endif 

AT91_SPI_Driver g_AT91_SPI_Driver;

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

BOOL AT91_SPI_Driver::Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();

    // Power Management Controller
    AT91_PMC &pmc = AT91::PMC();


    // Start by disabling the modules, since it does not happen on reset
    for(int i = 0; i < AT91_SPI::c_MAX_SPI; i++)
    {
        if(g_AT91_SPI_Driver.m_initialized[i] == FALSE)
        {
            AT91_SPI &spi = AT91::SPI(i);

            spi.SPI_CR |= AT91_SPI::SPI_CR_DISABLE_SPI; //Disable SPI Module, don't care the other bit.

            g_AT91_SPI_Driver.m_initialized[i] = TRUE;
        } 
	

    }

    pmc.EnablePeriphClock(AT91C_ID_SPI0);

#if (AT91C_MAX_SPI == 2)
    pmc.EnablePeriphClock(AT91C_ID_SPI1);
#endif

    // set up the GPIOs, always set them in the right state, as some other peripheral could have stolen them

    // should not put the SPI pins to any state, as it is unknown what user has connected it to.
    // put pins as in input would be the safest state.
    // SPI 0
    CPU_GPIO_EnableInputPin(AT91_SPI0_SCLK, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableInputPin(AT91_SPI0_MOSI, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableInputPin(AT91_SPI0_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    
#if (AT91C_MAX_SPI == 2)
    // SPI 1 
    CPU_GPIO_EnableInputPin(AT91_SPI1_SCLK, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableInputPin(AT91_SPI1_MOSI, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
    CPU_GPIO_EnableInputPin(AT91_SPI1_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);
#endif
    return TRUE;
}


void AT91_SPI_Driver::Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    // Do not disable the Periperal clock, as it is used for LCD 
    for(int i = 0; i < AT91_SPI::c_MAX_SPI; i++)
    {
        if(g_AT91_SPI_Driver.m_initialized[i] == TRUE)
        {
            AT91_SPI &spi = AT91::SPI(i);

            spi.SPI_CR |= AT91_SPI::SPI_CR_DISABLE_SPI; //Disable SPI Module, don't care the other bit.

            g_AT91_SPI_Driver.m_initialized[i] = FALSE;
        }
    }
	
    AT91_PMC &pmc = AT91::PMC();
	
    pmc.DisablePeriphClock(AT91C_ID_SPI0);

#if (AT91C_MAX_SPI == 2)
     pmc.DisablePeriphClock(AT91C_ID_SPI1);
#endif


	   
}

/***************************************************************************/

void AT91_SPI_Driver::ISR(void *Param)
{

    debug_printf("error ISR\r\n");
    ASSERT(0);
}

BOOL AT91_SPI_Driver::nWrite16_nRead16(const SPI_CONFIGURATION &Configuration, UINT16 *Write16, INT32 WriteCount, UINT16 *Read16, INT32 ReadCount, INT32 ReadStartOffset)
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    
    
    SPI_DEBUG_PRINT(" spi write 16 \r\n");
    
    if(g_AT91_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        lcd_printf("\fSPI Xaction 1\r\n");
        hal_printf("\fSPI Xaction 1\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(Configuration.SPI_mod > AT91_SPI::c_MAX_SPI)
    {
        lcd_printf("\fSPI wrong mod\r\n");
        hal_printf("\fSPI wrong mod\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    if(!Xaction_Start(Configuration))
        return FALSE;
    {
        SPI_XACTION_16 Transaction;

        Transaction.Read16 = Read16;
        Transaction.ReadCount = ReadCount;
        Transaction.ReadStartOffset = ReadStartOffset;
        Transaction.Write16 = Write16;
        Transaction.WriteCount = WriteCount;
        Transaction.SPI_mod = Configuration.SPI_mod;
        Transaction.BusyPin = Configuration.BusyPin;

        if(!Xaction_nWrite16_nRead16(Transaction))
            return FALSE;
    }

    SPI_DEBUG_PRINT(" B4 xs 16 \r\n");

    return Xaction_Stop(Configuration);
}

BOOL AT91_SPI_Driver::nWrite8_nRead8(const SPI_CONFIGURATION &Configuration, UINT8 *Write8, INT32 WriteCount, UINT8 *Read8, INT32 ReadCount, INT32 ReadStartOffset)
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();

   
    SPI_DEBUG_PRINT(" spi write 8 \r\n");
    if(g_AT91_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        lcd_printf("\fSPI Xaction 1\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }
    
    if(Configuration.SPI_mod > AT91_SPI::c_MAX_SPI)
    {
        lcd_printf("\fSPI wrong mod\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }
    
    if(!Xaction_Start(Configuration))
        return FALSE;

    
    {
        SPI_XACTION_8 Transaction;

        Transaction.Read8 = Read8;
        Transaction.ReadCount = ReadCount;
        Transaction.ReadStartOffset = ReadStartOffset;
        Transaction.Write8 = Write8;
        Transaction.WriteCount = WriteCount;
        Transaction.SPI_mod = Configuration.SPI_mod;
        Transaction.BusyPin = Configuration.BusyPin;

        if(!Xaction_nWrite8_nRead8(Transaction))
            return FALSE;
    }
    
    
    return Xaction_Stop(Configuration);
}

//--//

BOOL AT91_SPI_Driver::Xaction_Start(const SPI_CONFIGURATION &Configuration)
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    
    if(!g_AT91_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        g_AT91_SPI_Driver.m_Enabled[Configuration.SPI_mod] = TRUE;

        UINT32 index = Configuration.SPI_mod;

        
        AT91_SPI &spi = AT91::SPI(index);
        
        // make sure we didn't start one in the middle of an existing transaction
        /* if( spi.SPI_SR & AT91_SPI::SPI_SR_SPIENS ) 
        {
        lcd_printf("\fSPI Collision 1\r\n");
        hal_printf("\fSPI Collision 1\r\n");
        HARD_BREAKPOINT();

        return FALSE;
        }*/
        
        if(Configuration.SPI_mod == 0)
        {
            //Alterate GPIO pins to periphal pins
            CPU_GPIO_DisablePin(AT91_SPI0_SCLK, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
            CPU_GPIO_DisablePin(AT91_SPI0_MOSI, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
            CPU_GPIO_DisablePin(AT91_SPI0_MISO, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
//            if (Configuration.DeviceCS == AT91_SPI0_NSS)
//                CPU_GPIO_DisablePin(AT91_SPI0_NSS, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
            
        }
#if defined(PLATFORM_ARM_SAM9261_ANY)        
        else
        {
            //Alterate GPIO pins to periphal pins
            CPU_GPIO_DisablePin(AT91_SPI1_SCLK, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
            CPU_GPIO_DisablePin(AT91_SPI1_MOSI, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
            CPU_GPIO_DisablePin(AT91_SPI1_MISO, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);

// we don't implement the NSS/CS function in th SPI core.
//            if (Configuration.DeviceCS == AT91_SPI1_NSS)
//                CPU_GPIO_DisablePin(AT91_SPI1_NSS, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_1);
        }
#elif defined(PLATFORM_ARM_SAM7X_ANY) 
        else
        {         
   //Alterate GPIO pins to periphal pins
            CPU_GPIO_DisablePin(AT91_SPI1_SCLK, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);
            CPU_GPIO_DisablePin(AT91_SPI1_MOSI, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);
            CPU_GPIO_DisablePin(AT91_SPI1_MISO, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);

//            if (Configuration.DeviceCS == AT91_SPI1_NSS)
//                CPU_GPIO_DisablePin(AT91_SPI1_NSS, RESISTOR_DISABLED, 0, GPIO_ALT_MODE_2);
        }
#else
        else
        {
            lcd_printf("Invalid SPI port\r\n");
            return FALSE;
        }
#endif
        
#if defined(PLATFORM_ARM_SAM9261_ANY)
        // first build the mode register
        spi.SPI_MR = AT91_SPI::SPI_MR_MSTR | AT91_SPI::SPI_MR_CS1| AT91_SPI::SPI_MR_MODFDIS ;
        spi.SPI_CSR1 = AT91_SPI::ConfigurationToCSR(Configuration);
        
#else
        // first build the mode register
        spi.SPI_MR = AT91_SPI::SPI_MR_MSTR | AT91_SPI::SPI_MR_CS0 | AT91_SPI::SPI_MR_MODFDIS ;
        spi.SPI_CSR0 = AT91_SPI::ConfigurationToCSR(Configuration);
        
#endif        

        
        spi.SPI_CR |= AT91_SPI::SPI_CR_ENABLE_SPI;
        
        
        // first set CS active as soon as clock and data pins are in proper initial state
#if (AT91C_MAX_SPI == 2)
        if((Configuration.DeviceCS != GPIO_PIN_NONE) && (Configuration.DeviceCS != AT91_SPI0_NSS) && (Configuration.DeviceCS != AT91_SPI1_NSS))
#else
        if((Configuration.DeviceCS != GPIO_PIN_NONE) && (Configuration.DeviceCS != AT91_SPI0_NSS))
#endif        
        {
            CPU_GPIO_EnableOutputPin(Configuration.DeviceCS, Configuration.CS_Active);
        }
        
        if(Configuration.CS_Setup_uSecs)
        {
            HAL_Time_Sleep_MicroSeconds_InterruptEnabled(Configuration.CS_Setup_uSecs);
        }
        
        if(Configuration.BusyPin.Pin != GPIO_PIN_NONE)
        {
            CPU_GPIO_DisablePin( Configuration.BusyPin.Pin, RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY );
        }
    }
    else
    {
        lcd_printf("\fSPI Collision 3\r\n");
        //  HARD_BREAKPOINT();

        return FALSE;
    }

    return TRUE;
}


BOOL AT91_SPI_Driver::Xaction_Stop(const SPI_CONFIGURATION &Configuration)
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    if(g_AT91_SPI_Driver.m_Enabled[Configuration.SPI_mod])
    {
        UINT32 index = Configuration.SPI_mod;

        AT91_SPI &spi = AT91::SPI(index);

        if(Configuration.CS_Hold_uSecs)
        {
            HAL_Time_Sleep_MicroSeconds_InterruptEnabled(Configuration.CS_Hold_uSecs);
        }

//        // avoid noise drawing excess power on a floating line by putting this in pulldown
//        CPU_GPIO_EnableInputPin(AT91_SPI0_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);

        if(Configuration.SPI_mod == 0)
        {
            // avoid noise drawing excess power on a floating line by putting this in pulldown
            CPU_GPIO_EnableInputPin(AT91_SPI0_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);

            // put pins in output low state when not in use to avoid noise since clock stop and reset will cause these to become inputs
            CPU_GPIO_EnableOutputPin(AT91_SPI0_SCLK, FALSE);
            CPU_GPIO_EnableOutputPin(AT91_SPI0_MOSI, FALSE);
//            if (Configuration.DeviceCS == AT91_SPI0_NSS)
//               CPU_GPIO_EnableOutputPin(AT91_SPI0_NSS, TRUE);
        }
#if (AT91C_MAX_SPI == 2)
        else
        {
            // avoid noise drawing excess power on a floating line by putting this in pulldown
            CPU_GPIO_EnableInputPin(AT91_SPI1_MISO, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP);

            // put pins in output low state when not in use to avoid noise since clock stop and reset will cause these to become inputs
            CPU_GPIO_EnableOutputPin(AT91_SPI1_SCLK, FALSE);
            CPU_GPIO_EnableOutputPin(AT91_SPI1_MOSI, FALSE);
//            if (Configuration.DeviceCS == AT91_SPI1_NSS)
//                CPU_GPIO_EnableOutputPin(AT91_SPI1_NSS, TRUE);
        }
#else
        else
        {
            lcd_printf("Invalid SPI Port\r\n");
            return FALSE;
        }
#endif
        // next, bring the CS to the proper inactive state
#if (AT91C_MAX_SPI == 2)
        if((Configuration.DeviceCS != GPIO_PIN_NONE) && (Configuration.DeviceCS != AT91_SPI0_NSS) && (Configuration.DeviceCS != AT91_SPI1_NSS))
#else
        if((Configuration.DeviceCS != GPIO_PIN_NONE) && (Configuration.DeviceCS != AT91_SPI0_NSS))
#endif        
        {
            CPU_GPIO_SetPinState(Configuration.DeviceCS, !Configuration.CS_Active);
        }

        if(Configuration.BusyPin.Pin != GPIO_PIN_NONE)
        {
            CPU_GPIO_EnableInputPin( Configuration.BusyPin.Pin, FALSE, NULL, GPIO_INT_NONE, RESISTOR_PULLUP );
        }

        // off SPI module
        spi.SPI_CR |= AT91_SPI::SPI_CR_DISABLE_SPI;

        g_AT91_SPI_Driver.m_Enabled[Configuration.SPI_mod] = FALSE;
    }
    else
    {
        lcd_printf("\fSPI Collision 4\r\n");
        HARD_BREAKPOINT();

        return FALSE;
    }
    return TRUE;
}


BOOL AT91_SPI_Driver::Xaction_nWrite16_nRead16(SPI_XACTION_16 &Transaction)
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    UINT16 Data16;

    if(!g_AT91_SPI_Driver.m_Enabled[Transaction.SPI_mod])
    {
        hal_printf("\fSPI Xaction OFF\r\n");
        lcd_printf("\fSPI Xaction OFF\r\n");
        ASSERT(FALSE);
        return FALSE;
    }

    SPI_DEBUG_PRINT("\fxwr16\r\n");

    AT91_SPI &spi = AT91::SPI(Transaction.SPI_mod);


    UINT16  *Write16    = Transaction.Write16;
    INT32   WriteCount  = Transaction.WriteCount;
    UINT16  *Read16     = Transaction.Read16;
    INT32   ReadCount   = Transaction.ReadCount;
    INT32   ReadStartOffset = Transaction.ReadStartOffset;
    INT32   ReadTotal   = 0;


    // as master, we must always write something before reading or not
    if(WriteCount <= 0)
    {
        ASSERT(FALSE);
        return FALSE;
    }
    if(Write16 == NULL)
    {
        ASSERT(FALSE);
        return FALSE;
    }
    if((ReadCount > 0) && (Read16 == NULL))
    {
        ASSERT(FALSE);
        return FALSE;
    }

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
        spi.SPI_TDR = Write16[0];

        // wait while the transmit buffer is empty
        while(!spi.TransmitBufferEmpty(spi));

        if(Transaction.BusyPin.Pin != GPIO_PIN_NONE) 
        {
            while(CPU_GPIO_GetPinState(Transaction.BusyPin.Pin) == Transaction.BusyPin.ActiveState);
        }

        Data16 = spi.SPI_RDR;

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

BOOL AT91_SPI_Driver::Xaction_nWrite8_nRead8(SPI_XACTION_8 &Transaction)
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();
    UINT8 Data8;

    if(!g_AT91_SPI_Driver.m_Enabled[Transaction.SPI_mod])
    {
        lcd_printf("\fSPI Xaction OFF\r\n");
        ASSERT(FALSE);
        return FALSE;
    }

    AT91_SPI &spi = AT91::SPI(Transaction.SPI_mod);

    UINT8   *Write8 = Transaction.Write8;
    INT32   WriteCount = Transaction.WriteCount;
    UINT8   *Read8 = Transaction.Read8;
    INT32   ReadCount = Transaction.ReadCount;
    INT32   ReadStartOffset = Transaction.ReadStartOffset;
    INT32   ReadTotal        = 0;


    // as master, we must always write something before reading or not
    if(WriteCount <= 0)
    {
        debug_printf("WriteCount <= 0\r\n");
        ASSERT(FALSE);
        return FALSE;
    }
    if(Write8 == NULL)
    {
        debug_printf("Write8 == NULL\r\n");
        ASSERT(FALSE);
        return FALSE;
    }
    if((ReadCount > 0) && (Read8 == NULL))
    {
        debug_printf("(ReadCount > 0) && (Read8 == NULL)\r\n");
        ASSERT(FALSE);
        return FALSE;
    }

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

        spi.SPI_TDR = Write8[0];

        // wait while the transmit buffer is empty
        while(!spi.TransmitBufferEmpty(spi));

        if(Transaction.BusyPin.Pin != GPIO_PIN_NONE) 
        {
            while(CPU_GPIO_GetPinState(Transaction.BusyPin.Pin) == Transaction.BusyPin.ActiveState);
        }

        // reading clears the RBF bit and allows another transfer from the shift register
        Data8 = spi.SPI_RDR;

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

void AT91_SPI_Driver::GetPins(UINT32 spi_mod, GPIO_PIN &msk, GPIO_PIN &miso, GPIO_PIN &mosi)
{
    NATIVE_PROFILE_HAL_PROCESSOR_SPI();

    switch(spi_mod)
    {
    case 0:
        msk = AT91_SPI0_SCLK;
        miso = AT91_SPI0_MISO;
        mosi = AT91_SPI0_MOSI;

        break;
#if (AT91C_MAX_SPI == 2)
    case 1:
        msk = AT91_SPI1_SCLK;
        miso = AT91_SPI1_MISO;
        mosi = AT91_SPI1_MOSI;

        break;
#endif        
    default:
        break;

    }

}
/***************************************************************************/
