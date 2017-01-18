////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#undef  TRACE_ALWAYS

#define TRACE_ALWAYS               0x00000001

#undef  DEBUG_TRACE
#define DEBUG_TRACE (TRACE_ALWAYS)

//--//

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_MC9328MXL_DMA_Driver"
#endif

MC9328MXL_DMA_Driver g_MC9328MXL_DMA_Driver;

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif

extern UINT32 LCD_GetPixelClockDivider();

//--//

BOOL MC9328MXL_DMA_Driver::Initialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    MC9328MXL_DMA & DMA = MC9328MXL::DMA();

    // Disable all 10 channels
    for (int i = 0; i < 10; i++)
    {
        DMA.CH[i].CCR = MC9328MXL_DMA::CCR__CEN__Disable << MC9328MXL_DMA::CCR__CEN_shift;
    }

    // Turn on the DMA controller.
    DMA.DCR = MC9328MXL_DMA::DCR__DEN__Enable << MC9328MXL_DMA::DCR__DEN_shift;

    return TRUE;
}

BOOL MC9328MXL_DMA_Driver::Uninitialize()
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    MC9328MXL_DMA & DMA = MC9328MXL::DMA();

    // Turn on the DMA controller.
    DMA.DCR = MC9328MXL_DMA::DCR__DEN__Disable << MC9328MXL_DMA::DCR__DEN_shift;

    return TRUE;
}

// Note - dst and src have to be word-aligned
// TODO - this code is NOT thread-safe, it is possible for two threads calling MemCpy() issuing the DMA on the same 
//        channel. (i.e. Use this function in an ISR at your own risk)
//        Or we can alternatively consider to statically assign DMA channels.
// TODO - need to make sure the src data is pinned if async is TRUE (and unpinned it in the ISR for the GC)
void MC9328MXL_DMA_Driver::MemCpy(void* dst, void* src, UINT32 size, BOOL async = FALSE)
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    int channel = c_MemCpyChannelStart;
    MC9328MXL_DMA & DMA = MC9328MXL::DMA();

    // the channel is enabled and there's no complete status means it's in use
    while((DMA.CH[channel].CCR & DMA.CCR__CEN_mask) && (!(DMA.DISR & (1 << channel))))
    {
        if (channel == c_MemCpyChannelEnd) 
        {
            debug_printf( "WARNING: DMA_MemCpy() can't find an available channel.\r\n" );

            // We tried all the channels and can't find an available one, so just do a normal memcpy
            memcpy( dst, src, size );
            return;

            // Or alternatively, we can start the search over again
            // channel = c_MemCpyChannelStart;
        }
        else 
        { 
            // We move on to the next one
            channel--;
        }
    }

    DMA.DISR |= (1 << channel); // Remove the complete status, if there is one

    MC9328MXL_DMA::CHANNEL_REGISTERS & DMACH = DMA.CH[channel];

    {
        UINT32 val = 0;

        val |= MC9328MXL_DMA::CCR__DMOD__Linear         << MC9328MXL_DMA::CCR__DMOD_shift;
        val |= MC9328MXL_DMA::CCR__SMOD__Linear         << MC9328MXL_DMA::CCR__SMOD_shift;
        val |= MC9328MXL_DMA::CCR__MDIR__Inc            << MC9328MXL_DMA::CCR__MDIR_shift;
        val |= MC9328MXL_DMA::CCR__DSIZ__32_Bit_Port    << MC9328MXL_DMA::CCR__DSIZ_shift;
        val |= MC9328MXL_DMA::CCR__SSIZ__32_Bit_Port    << MC9328MXL_DMA::CCR__SSIZ_shift;
        val |= MC9328MXL_DMA::CCR__REN__Request_Disable << MC9328MXL_DMA::CCR__REN_shift;
        val |= MC9328MXL_DMA::CCR__RPT__Repeat_Disable  << MC9328MXL_DMA::CCR__RPT_shift;
        val |= MC9328MXL_DMA::CCR__CEN__Disable         << MC9328MXL_DMA::CCR__CEN_shift;

        DMACH.CCR = val;
    }

    if (CPU_IsMMUEnabled())
    {
        // DMA controller is unaware of the MMU translation, so we need to do it
        // TODO: make sure the entire range is in continguous region of physical memory
        dst = TranslateAddress( dst );
        src = TranslateAddress( src );
    }

    DMACH.DAR  = (UINT32)dst;
    DMACH.SAR  = (UINT32)src;
    DMACH.CNTR =         size;

    // Since DMA does not go through cache, we want to flush out all data that's still in the buffer
    CPU_DrainWriteBuffers();

    DMACH.CCR |= MC9328MXL_DMA::CCR__CEN__Enable << MC9328MXL_DMA::CCR__CEN_shift;

    if (!async)
    {
        UINT32 mask = (1 << channel);

        // Spin until the transfer is done
        while(!(DMA.DISR & mask));

        DMA.DISR |= 1 << channel; // Remove the complete status
        DMACH.CCR = MC9328MXL_DMA::CCR__CEN__Disable << MC9328MXL_DMA::CCR__CEN_mask; // Disable the channel
    }
}

// Note - since the chip allows only 1 2D DMA transfer at one time, the call (even if async = TRUE) may potentially block 
//        until the previous 2D DMA transfer is completed.
//        Also, dst and src have to be word-aligned
// TODO - this code is NOT thread-safe, it is possible for two threads calling MemCpy2D() issuing the DMA on the same 
//        channel. (i.e. Use this function in an ISR at your own risk) 
//        Or we can alternatively consider to statically assign DMA channels.
// TODO - need to make sure the src data is pinned if async is TRUE (and unpinned it in the ISR for the GC)
void MC9328MXL_DMA_Driver::MemCpy2D( void *dst, void*src, UINT32 width, UINT32 height, UINT32 dataWidth, BOOL async = FALSE )
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    MC9328MXL_DMA & DMA = MC9328MXL::DMA();
    MC9328MXL_DMA::CHANNEL_REGISTERS & DMACH = DMA.CH[c_MemCpy2DChannel];

    UINT32 count = 0;

    // spin until the previous 2D transfer is complete
    while((DMACH.CCR & DMA.CCR__CEN_mask) && (!(DMA.DISR & (1 << c_MemCpy2DChannel))))
    {
        if (count++ > c_MemCpy2DWaitTimeOut)
        {
            debug_printf( "WARNING: DMA_MemCpy2D() timed out.\r\n" );
            
            // Do the copying the old-fashioned way
            for (int h = 0; h < height; h++)
            {
                memcpy( dst, src, width );

                dst = ((UINT8*)dst) + dataWidth;
                src = ((UINT8*)src) + dataWidth;
            }

            return;
        }
    }
    
    DMA.DISR |= 1 << c_MemCpy2DChannel; // Remove the complete status, if there is one

    {
        UINT32 val = 0;

        val |= MC9328MXL_DMA::CCR__DMOD__2D             << MC9328MXL_DMA::CCR__DMOD_shift;
        val |= MC9328MXL_DMA::CCR__SMOD__2D             << MC9328MXL_DMA::CCR__SMOD_shift;
        val |= MC9328MXL_DMA::CCR__MDIR__Inc            << MC9328MXL_DMA::CCR__MDIR_shift;
        val |= MC9328MXL_DMA::CCR__MSEL__2D_Memory_A    << MC9328MXL_DMA::CCR__MSEL_shift;
        val |= MC9328MXL_DMA::CCR__DSIZ__32_Bit_Port    << MC9328MXL_DMA::CCR__DSIZ_shift;
        val |= MC9328MXL_DMA::CCR__SSIZ__32_Bit_Port    << MC9328MXL_DMA::CCR__SSIZ_shift;
        val |= MC9328MXL_DMA::CCR__REN__Request_Disable << MC9328MXL_DMA::CCR__REN_shift;
        val |= MC9328MXL_DMA::CCR__RPT__Repeat_Disable  << MC9328MXL_DMA::CCR__RPT_shift;
        val |= MC9328MXL_DMA::CCR__CEN__Disable         << MC9328MXL_DMA::CCR__CEN_shift;

        DMACH.CCR = val;
    }

    if (CPU_IsMMUEnabled())
    {
        // DMA controller is unaware of the MMU translation, so we need to do it
        // TODO: make sure the entire range is in continguous region of physical memory
        dst = TranslateAddress( dst );
        src = TranslateAddress( src );
    }

    DMACH.DAR  = (UINT32)dst;
    DMACH.SAR  = (UINT32)src;

    DMA.WSRA = dataWidth & MC9328MXL_DMA::WSRA__WS_mask;
    DMA.XSRA = width     & MC9328MXL_DMA::XSRA__XS_mask;
    DMA.YSRA = height    & MC9328MXL_DMA::YSRA__YS_mask;

    // Since DMA does not go through cache, we want to flush out all data that's still in the buffer
    CPU_DrainWriteBuffers();

    DMACH.CCR |= MC9328MXL_DMA::CCR__CEN__Enable << MC9328MXL_DMA::CCR__CEN_shift;

    if (!async)
    {
        UINT32 mask = (1 << c_MemCpy2DChannel);

        // Spin until the transfer is done (TODO: is there a better way to spin???)
        while(!(DMA.DISR & mask));

        DMACH.CCR = MC9328MXL_DMA::CCR__CEN__Disable << MC9328MXL_DMA::CCR__CEN_mask; // Disable the channel
    }
}


// StartDummyDMA()
// This routine was written to compensate for a BUG in the MC9328MXL and MC9328MXS that causes the cache
// DMA to hang onto the bus.  The result of this bug is that after a cache line fill, it is a long time
// before the LCDC FIFO DMA can gain control of the bus to make its necessary periodic data transfer to
// keep the LCD display refreshed.  The result is a flashing or flickering display due to LCD FIFO underrun.
// It turns out that performing a brief DMA using the DMA controller can shake loose the cache DMA's hold
// on the bus.  This routine, therefore, implements a periodic brief DMA to free up the bus for the LCDC
// FIFO DMA transfers.  It is not necessary for any other function.
void MC9328MXL_DMA_Driver::StartDummyDMA()
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    StartDummyDMAChannel(c_DummyDMAChannel_10);
    StartDummyDMAChannel(c_DummyDMAChannel_5);    
}

void MC9328MXL_DMA_Driver::StartDummyDMAChannel(int DMAChannel)
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    UINT32 dmaBufferAddr = 0x0010001C; // The address of the bootstrap instruction buffer. (at most 3 cycles to complete a r/w operation)

    MC9328MXL_DMA::CHANNEL_REGISTERS & DMACH = MC9328MXL::DMA().CH[DMAChannel];

    // Set up DMA channel control parameters.
    {
        UINT32 val = 0;

        val |= MC9328MXL_DMA::CCR__DMOD__Linear         << MC9328MXL_DMA::CCR__DMOD_shift;  // Destination is linear mode
        val |= MC9328MXL_DMA::CCR__SMOD__Linear         << MC9328MXL_DMA::CCR__SMOD_shift;  // Source is linear mode
        val |= MC9328MXL_DMA::CCR__MDIR__Inc            << MC9328MXL_DMA::CCR__MDIR_shift;  // Increment addresses
        val |= MC9328MXL_DMA::CCR__DSIZ__32_Bit_Port    << MC9328MXL_DMA::CCR__DSIZ_shift;  // Destination is 32 bit port
        val |= MC9328MXL_DMA::CCR__SSIZ__32_Bit_Port    << MC9328MXL_DMA::CCR__SSIZ_shift;  // Source is 32 bit port
        val |= MC9328MXL_DMA::CCR__REN__Request_Disable << MC9328MXL_DMA::CCR__REN_shift;   // User CEN to start DMA operation
        val |= MC9328MXL_DMA::CCR__RPT__Repeat_Enable   << MC9328MXL_DMA::CCR__RPT_shift;   // Auto repeat
        val |= MC9328MXL_DMA::CCR__CEN__Disable         << MC9328MXL_DMA::CCR__CEN_shift;   // Disable DMA channel (for now).
        
        DMACH.CCR = val;
    }

    // Setup the dummy DMA buffer.
    DMACH.SAR  = dmaBufferAddr;
    DMACH.DAR  = dmaBufferAddr;
    DMACH.CNTR = 4; // size of the transfer

    // Set channel burst length--number of bytes to move per burst.
    DMACH.BLR = 4;


    // Set bus utilization -- number of system clocks to wait between bursts.
    // Set the burst to occur every 6 words (this is just a ballpark figure) This
    // should be often enough that if the display just misses one burst, it is
    // not in dire straights by the time another comes around.  It is also not so
    // often that it begins to negatively affect system performance.
    //
    // 6 words equates to 12 pixels.  One pixel time = (DIV/96MHz) and one DM
    // interval = (1/96MHz).  So to give a DMA every 12 pixels that's
    // 12*(pixel time)/(DMA interval) which is just 12 * DIV where DIV is the
    // pixel clock divider - since the LCD clock & DMA clock have the same
    // base frequency.
    {
        UINT32 cycles;

        cycles = LCD_GetPixelClockDivider();
        if (cycles != LCD_NO_PIXEL_CLOCK_DIVIDER)
        {
           cycles =(cycles +1) *12;
        }
        else
        {
            // Legacy code - if you are using this, you probably aren't looking closely
            // enough at this issue...
            // Set bus utilization -- number of system clocks to wait between bursts
            // Our max LCD latency is 10 words (DMACR__TM == 10), which is 20 pixels.
            // Our pixel clock is 3.31 Mhz (96Mhz / 29). 
            // So we have to insert 1 DMA cycle every 20 / 3.31 Mhz = 6.04 usec
            // At 96Mhz, that is 580 cycles.
            // Subtract the Max CPU Burst (16 cycles) and the max cycles it take to r/w the buffer (3), 
            // we have Max DMA Bus utilization = 580 - 16 - 3 = 561 cycles
            cycles = 561;
        }

        
        DMACH.RTORBUCR = cycles;
    }
    
    // Start the DMA channel.
    DMACH.CCR |= MC9328MXL_DMA::CCR__CEN__Enable << MC9328MXL_DMA::CCR__CEN_shift;
}

// Translate from virtual address to physical address
void* MC9328MXL_DMA_Driver::TranslateAddress( void* virtualAddress )
{
    NATIVE_PROFILE_HAL_PROCESSOR_DMA();
    UINT32* TTB;

    #if defined(COMPILE_ARM) || defined(COMPILE_THUMB)
		#if defined(__GNUC__)
			asm("MRC	p15, 0, %0, c2, c0, 0" : "=r" (TTB));
		#else
        __asm
        {
            mrc     p15, 0, TTB, c2, c0, 0 // Retrieve the TTB address location from CP15
        }
		#endif
    #elif defined(COMPILE_THUMB2)
    // to be fill in???
    #endif
    

    UINT32 L1Entry = *ARM9_MMU::GetL1Entry( TTB, (UINT32) virtualAddress );

    if ((L1Entry & 0x3) == 0x2) // The Entry is a section descriptor
    {
        return (void*) ((L1Entry & 0xFFF00000) | (((UINT32)virtualAddress) & 0xFFFFF));
    }
    else
    {
        lcd_printf( "Invalid address: %08x\r\n", (UINT32)virtualAddress );
        debug_printf( "Invalid address: %08x\r\n", (UINT32)virtualAddress );

        CPU_Halt();
 
        return (void*) -1; //INVALID_ADDRESS
    }
}
