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
#pragma arm section zidata = "g_PXA271_SPI_Driver"
#endif
PXA271_SPI_Driver g_PXA271_SPI_Driver;
PXA271_SPI_Driver::SPI_PORT_INFO PXA271_SPI_Driver::s_All_SPI_port[]=
{
    { 
        PXA271_SPI::c_SSP1_CLK,       
        PXA271_SPI::c_SSP1_TXD ,      
        PXA271_SPI::c_SSP1_RXD ,      
        PXA271_CLOCK_MANAGER::CKEN__SSP1,
        FALSE,
    },  

    { 
        PXA271_SPI::c_SSP2_CLK,       
        PXA271_SPI::c_SSP2_TXD ,      
        PXA271_SPI::c_SSP2_RXD ,      
        PXA271_CLOCK_MANAGER::CKEN__SSP2,
        FALSE,
    },                         

    { 
        PXA271_SPI::c_SSP3_CLK,       
        PXA271_SPI::c_SSP3_TXD ,      
        PXA271_SPI::c_SSP3_RXD ,      
        PXA271_CLOCK_MANAGER::CKEN__SSP3,
        FALSE,
    },          
};

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

//#define SPI_LOOP_BACK_PXA271

#ifdef SPI_LOOP_BACK_PXA271
#define SPI_LOOPBACK   0x4
#else
#define SPI_LOOPBACK   0x0
#endif
 
//--//
BOOL PXA271_SPI_Driver::Initialize()
{   
    //SPI 0 and SPI 1 is not initialized as we don't know what the connection is.Only SPI 2 is initialized as it is dedicated to the Zigbee chip. 
    CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[2].CLK_pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_3);
    CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[2].TXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_3);
    CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[2].RXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN,GPIO_ALT_MODE_3);
    return TRUE;
}


void PXA271_SPI_Driver::Uninitialize()
{    
    UINT32 index;
    
    // enable the Periperal clock for this device
    for(index=0;index<3;index++)
    {
        PXA271_SPI& SPI = PXA271::SPI(index);
        SPI.SSP_SSCR0 = 0;
        SPI.SSP_SSCR1 = 0;
        PXA271::CLKMNGR().CKEN &= ~g_PXA271_SPI_Driver.s_All_SPI_port[index].ClockIndex;    
    }   
}

/***************************************************************************/

void PXA271_SPI_Driver::ISR( void* Param )
{
    ASSERT(0);
}

BOOL PXA271_SPI_Driver::nWrite16_nRead16( const SPI_CONFIGURATION& Configuration, UINT16* Write16, INT32 WriteCount, UINT16* Read16, INT32 ReadCount, INT32 ReadStartOffset )
{
    if(g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].m_Enabled)
    {
        lcd_printf("\fSPI Xaction 1\r\n");
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

    return Xaction_Stop( Configuration );
}

BOOL PXA271_SPI_Driver::nWrite8_nRead8( const SPI_CONFIGURATION& Configuration, UINT8* Write8, INT32 WriteCount, UINT8* Read8, INT32 ReadCount, INT32 ReadStartOffset )
{   
    if(g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].m_Enabled)
    {
        lcd_printf("\fSPI Xaction 1\r\n");
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

BOOL PXA271_SPI_Driver::Xaction_Start( const SPI_CONFIGURATION& Configuration )
{
    
    if(!g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].m_Enabled)
    {
        g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].m_Enabled= TRUE;
        
        // enable the Periperal clock for this device
        PXA271::CLKMNGR().CKEN |= g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].ClockIndex; 
        
        PXA271_SPI& SPI = PXA271::SPI(Configuration.SPI_mod);       
        
        // make sure we didn't start one in the middle of an existing transaction
        if((0 != SPI.SSP_SSCR0)||(0 != SPI.SSP_SSCR1))
        {
            lcd_printf("\fSPI Collision 1\r\n");
            HARD_BREAKPOINT();  
            return FALSE;
        } 
           
        SPI.SSP_SSCR0= PXA271_SPI::ConfigurationControlReg0( Configuration );
        SPI.SSP_SSCR1= PXA271_SPI::ConfigurationControlReg1( Configuration );   
        
        #ifdef SPI_LOOP_BACK_PXA271
        SPI.SSP_SSCR1|= SPI_LOOPBACK;
        #endif
        SPI.SSP_SSCR0|= SPI.SSP_SSCR0__SSE;        
        
        //everything should be clean and idle
        ASSERT(!SPI.TransmitFifoNotEmpty());
        ASSERT( SPI.ReceiveFifoEmpty());
        ASSERT( SPI.ShiftBufferEmpty());      
        
        if(Configuration.SPI_mod ==0)
        {
            //allow peripheral control of pins;
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].CLK_pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_2);
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].TXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_2);  
        }
        else if(Configuration.SPI_mod ==1)
        {
            //allow peripheral control of pins;
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].CLK_pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_2);
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].TXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_2);  
        }
        else
        {
            //allow peripheral control of pins;
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].CLK_pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_3);
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].TXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_3);  
        }
        // first set CS active as soon as clock and data pins are in proper initial state
        if(Configuration.DeviceCS != PXA271_GPIO::c_Pin_None)
        {
            CPU_GPIO_EnableOutputPin(Configuration.DeviceCS, Configuration.CS_Active);
        }
        
        if(Configuration.SPI_mod ==0)
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].RXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN,GPIO_ALT_MODE_1);
        else if(Configuration.SPI_mod ==1)
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].RXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN,GPIO_ALT_MODE_2);
        else
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].RXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN,GPIO_ALT_MODE_3);
       
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


BOOL PXA271_SPI_Driver::Xaction_Stop( const SPI_CONFIGURATION& Configuration )
{       
    if(g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].m_Enabled)
    {
        PXA271_SPI& SPI = PXA271::SPI(Configuration.SPI_mod);

        // we should have cleared the last TBF on the last RBF true setting
        // we should never bring CS inactive with the shifter busy
        ASSERT(!SPI.TransmitFifoNotEmpty());
        ASSERT( SPI.ReceiveFifoEmpty());
        ASSERT( SPI.ShiftBufferEmpty());

        if(Configuration.CS_Hold_uSecs)
        {
            HAL_Time_Sleep_MicroSeconds_InterruptEnabled( Configuration.CS_Hold_uSecs );
        }

         // avoid noise drawing excess power on a floating line by putting this in pulldown
        if(Configuration.SPI_mod ==0)
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].RXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN,GPIO_ALT_MODE_1);
        else if(Configuration.SPI_mod ==1)
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].RXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN,GPIO_ALT_MODE_2);
        else
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].RXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_IN,GPIO_ALT_MODE_3);
         // next, bring the CS to the proper inactive state
        if(Configuration.DeviceCS != PXA271_GPIO::c_Pin_None)
        {
            CPU_GPIO_EnableOutputPin(Configuration.DeviceCS, !Configuration.CS_Active);
        }

         // put pins in output low state when not in use to avoid noise since clock stop and reset will cause these to become inputs
        if(Configuration.SPI_mod ==0)
        {
            //allow peripheral control of pins;
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].CLK_pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_2);
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].TXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_2);  
        }
        else if(Configuration.SPI_mod ==1)
        {
            //allow peripheral control of pins;
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].CLK_pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_2);
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].TXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_2);  
        }
        else
        {
            //allow peripheral control of pins;
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].CLK_pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_3);
            CPU_GPIO_DisablePin( g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].TXD_Pin, RESISTOR_DISABLED, PXA271_GPIO::GPDR__DIR_OUT,GPIO_ALT_MODE_3);  
        }
        // disable spi bus so no new operations start
        SPI.SSP_SSCR0 = 0;
        SPI.SSP_SSCR1 = 0;
        PXA271::CLKMNGR().CKEN &= ~g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].ClockIndex;     

        g_PXA271_SPI_Driver.s_All_SPI_port[Configuration.SPI_mod].m_Enabled = FALSE;
    }
    else
    {
        lcd_printf("\fSPI Collision 4\r\n");
        HARD_BREAKPOINT();
        return FALSE;
    }

    return TRUE;
}


BOOL PXA271_SPI_Driver::Xaction_nWrite16_nRead16( SPI_XACTION_16& Transaction )
{
    UINT16 Data16;

    if(!g_PXA271_SPI_Driver.s_All_SPI_port[Transaction.SPI_mod].m_Enabled)
    {
        lcd_printf("\fSPI Xaction OFF\r\n");
        ASSERT(FALSE);
        return FALSE;
    }
    
    PXA271_SPI& SPI = PXA271::SPI(Transaction.SPI_mod);

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
   
    //DelayValue = SPI_Configuration->ClockDivisor;

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

    while(loopCnt--)
    {
        // writing sets the TBF bit again
        SPI.SSP_SSDR = Write16[0];

        // wait while the transmit buffer is full and receive buffer is empty
        while(SPI.TransmitFifoNotEmpty() || !SPI.ShiftBufferEmpty() || SPI.ReceiveFifoEmpty());

        // reading clears the RBF bit and allows another transfer from the shift register
        Data16 = SPI.SSP_SSDR;

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

BOOL PXA271_SPI_Driver::Xaction_nWrite8_nRead8( SPI_XACTION_8& Transaction )
{
    UINT8 Data8;
    
    if(!g_PXA271_SPI_Driver.s_All_SPI_port[Transaction.SPI_mod].m_Enabled)
    {
        lcd_printf("\fSPI Xaction OFF\r\n");
        ASSERT(FALSE);
        return FALSE;
    }

    PXA271_SPI& SPI = PXA271::SPI(Transaction.SPI_mod);

    UINT8* Write8          = Transaction.Write8;
    INT32  WriteCount      = Transaction.WriteCount;
    UINT8* Read8           = Transaction.Read8;
    INT32  ReadCount       = Transaction.ReadCount;
    INT32  ReadStartOffset = Transaction.ReadStartOffset;
    INT32  ReadTotal  = 0;

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
   
    while(loopCnt--)
    {
        // writing sets the TBF bit again
        SPI.SSP_SSDR= Write8[0];

        // wait while the transmit buffer is full and receive buffer is empty
        while(SPI.TransmitFifoNotEmpty() || !SPI.ShiftBufferEmpty() || SPI.ReceiveFifoEmpty());
        
        // reading clears the RBF bit and allows another transfer from the shift register
        Data8 = SPI.SSP_SSDR;
        
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

void PXA271_SPI_Driver::GetPins( UINT32 spi_mod, GPIO_PIN& msk, GPIO_PIN& miso, GPIO_PIN& mosi )
{
    switch (spi_mod)
    {
        case (PXA271_SPI::c_SPI1):
            msk  = PXA271_SPI::c_SSP1_CLK;;
            miso = PXA271_SPI::c_SSP1_TXD;
            mosi = PXA271_SPI::c_SSP1_RXD;
            
            break;
        case (PXA271_SPI::c_SPI2):
            msk  = PXA271_SPI::c_SSP2_CLK;;
            miso = PXA271_SPI::c_SSP2_TXD;
            mosi = PXA271_SPI::c_SSP2_RXD;

            break;
        case (PXA271_SPI::c_SPI3):
            msk  = PXA271_SPI::c_SSP3_CLK;;
            miso = PXA271_SPI::c_SSP3_TXD;
            mosi = PXA271_SPI::c_SSP3_RXD;

            break;
            
        default:
            break;

    }
}


