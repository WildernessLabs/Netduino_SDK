////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <tinyhal.h>

#ifdef TCPIP_LWIP 
void  Network_PostEvent(UINT32 evt, UINT32 data)
{
}
#endif

//#define TEST_DAC
//#define TIME_DAC_ISR
//#define TEST_JOYSTICK
//#define TEST_SST39WF

#if defined(TEST_DAC)
    
    #include "test_short.c"
    //#include "A.c"
    
    #if !defined(TEST_SAMPLES_NUM)
        #error    TEST_SAMPLES_NUM must be defined in test file
    #endif
    
    #define CUTOUT 1
    
    #define OUT_FREQ 8000        //8kHz output
    
    #if defined(TIME_DAC_ISR)
        extern UINT32 execStartTicks[TEST_SAMPLES_NUM];
        extern UINT32 timesRun;
        #include <math.h>
    #endif
    
    extern LPC24XX_DAC_Driver g_LPC24XX_DAC_Driver;
#endif

HAL_DECLARE_NULL_HEAP();

void resetDACISRTiming()
{
    #if defined(TIME_DAC_ISR)
    timesRun = 0;
    for(int i=0; i <TEST_SAMPLES_NUM; i++)
    {
        execStartTicks[i]=0;
    }
    #endif
}

UINT16 IRQ_INDEX_TEST[32];
UINT16 IRQ_TEST_OUTPUT[8192];
static int irq_count;

#if defined(TEST_JOYSTICK)
BOOL wait_joystick;
#endif

void hijackingISR(void* arg)
{
    UINT16* index = (UINT16*) arg;
    if(irq_count < 8192) IRQ_TEST_OUTPUT[irq_count] = *index;
    irq_count++;
}

BOOL hijackISRs()
{
    for(int irqIndex=0; irqIndex < 32; irqIndex++)
    {
        IRQ_INDEX_TEST[irqIndex] = irqIndex;
        if /*leave untouched ISRs needed for testing*/
        (
            irqIndex == LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer)||
            irqIndex == LPC24XX_TIMER::getIntNo(LPC24XX_Driver::c_SystemTime_Timer)/*||
            irqIndex == LPC24XX_VIC::c_IRQ_INDEX_UART2 ||
            irqIndex == LPC24XX_VIC::c_IRQ_INDEX_DBG_COM_RX*/||
            irqIndex == LPC24XX_VIC::c_IRQ_INDEX_DBG_COM_TX
        )
            break;
        if(!CPU_INTC_ActivateInterrupt( irqIndex , hijackingISR, (void*)&(IRQ_INDEX_TEST[irqIndex]) )) return FALSE;
    }
    return true;
}

BOOL restoreISRs()
{
    //CPU_INTC_ActivateInterrupt(LPC24XX_VIC::c_IRQ_INDEX_EMAC, LPC24XX_EMAC_interrupt, NULL);
    //CPU_INTC_ActivateInterrupt( LPC24XX_VIC::c_IRQ_INDEX_I2C0, &LPC24XX_I2C_Driver::ISR, NULL );
    //CPU_INTC_ActivateInterrupt( LPC24XX_TIMER::getIntNo(Timer) , LPC24XX_TIMER::ISR, ISR_Param );
    //CPU_INTC_ActivateInterrupt( LPC24XX_VIC::c_IRQ_INDEX_WDT, LPC24XX_WATCHDOG_Driver::ISR, NULL );
    //CPU_INTC_ActivateInterrupt( LPC24XX_VIC::c_IRQ_INDEX_DBG_COM_TX, LPC24XX_USART_Driver::UART_IntHandler,  (void *)3);  
    return true;
}



void displayRunTestResults()
{
    #if defined(TIME_DAC_ISR)
    UINT16 maxSamplesUsed = TEST_SAMPLES_NUM/5;
    long double avgInterTicks = 0.0;
    long double sigmaInterTicks = 0.0;
    double tickTimeMicroSecs = 1000000.0/CPU_TicksPerSecond();
    debug_printf("Tick length = %eus\r\n",tickTimeMicroSecs);
    for(int i = 0; i < maxSamplesUsed-1; i++)
    {
        /*if(i<100)
        {
            debug_printf("execStartTicks[%u]=%u\r\n",i+1,execStartTicks[i+1]);
            debug_printf("-\r\nexecStartTicks[%u]=%u\r\n",i,execStartTicks[i]);
            debug_printf("=\r\n%u\r\n\r\n",(execStartTicks[i+1] - execStartTicks[i]));
        }*/
        avgInterTicks += (execStartTicks[i+1]-execStartTicks[i]);
        //debug_printf("avgInterTicks = %e\r\n",avgInterTicks);
    }
    avgInterTicks=avgInterTicks/(maxSamplesUsed-1);
    debug_printf("Avg Time between ISR executions = %eus\r\n",avgInterTicks*tickTimeMicroSecs);
    
    for(int i = 0; i < maxSamplesUsed; i++)
    {
        sigmaInterTicks += pow((execStartTicks[i+1] - execStartTicks[i])-avgInterTicks,2);
    }
    sigmaInterTicks = sqrt(sigmaInterTicks/maxSamplesUsed);
    debug_printf("Std Deviation in Times = %eus\r\n",sigmaInterTicks*tickTimeMicroSecs);
    #endif
}

void csvRunTestResults()
{
    #if defined(TIME_DAC_ISR)
    UINT16 maxSamplesUsed = TEST_SAMPLES_NUM;
    
    double tickTimeMicroSecs = 1000000.0/CPU_TicksPerSecond();
    debug_printf("\r\n\r\n==========================================\r\n");
    for(int i = 0; i < maxSamplesUsed; i++)
    {
        debug_printf("%f,%f\r\n",execStartTicks[i]*tickTimeMicroSecs,(execStartTicks[i+1]-execStartTicks[i])*tickTimeMicroSecs);
        //debug_printf("%f\r\n",execStartTicks[i]*tickTimeMicroSecs);
    }
    debug_printf("\r\n\r\n==========================================\r\n");
    #endif
}

void joystickISR(GPIO_PIN Pin, BOOL PinState, void* Param)
{
#if defined(TEST_JOYSTICK)
    
    debug_printf("JOYSTICK PRESSED\r\nPIN\t= %u\r\nSTATE\t= %u\r\n", Pin, PinState);
    //wait_joystick = false;
#endif
}
    

void ApplicationEntryPoint()
{
    
#if defined(TEST_DAC)
    UINT32 FramesNum = g_LPC24XX_DAC_Driver.GetBufferFrameCapacity();
    if (DAC_FRAME_BUFFERS_NUM!=FramesNum)
    {
        debug_printf( "Error, BufferFrameCapacity != DAC_FRAME_BUFFERS_NUM: %d != %d.\r\n", FramesNum, DAC_FRAME_BUFFERS_NUM );
    }
        
    UINT32 nextInFrameOffset=0;
    UINT16 frameLength = MAX_DECODED_FRAME_SIZE/2;
    short* frameSignedStart = NULL;

    
    LPC24XX_VIC& VIC = LPC24XX::VIC();
    
    /*debug_printf("VIC INTRSEL = 0x%08x\r\n", VIC.INTRSEL);
    VIC.INTRSEL |= 1 << LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer);
    debug_printf("new VIC INTRSEL = 0x%08x\r\n", VIC.INTRSEL);*/
    
    VIC.VECTPRIORITY[LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer)] = 0;
        
    for(int i= 0; i< 32; i++)
    {
        debug_printf("PRIO INTR%02d = %d \r\n", i,VIC.VECTPRIORITY[i]);
    }
    
        
    debug_printf( "Init DAC, 8kHz output.\r\n" );
    g_LPC24XX_DAC_Driver.Initialize(OUT_FREQ); 
    


    
    debug_printf( "BUFFER PRE-FILL TEST.\r\n" );
    debug_printf( "Adding frames to the DAC driver buffer: " );


    debug_printf("total frames to be added = %d\r\n", TEST_SAMPLES_NUM/MAX_DECODED_FRAME_SIZE-CUTOUT);
    debug_printf("DAC frame buffers available = %d\r\n", DAC_FRAME_BUFFERS_NUM);
    if(DAC_FRAME_BUFFERS_NUM<(TEST_SAMPLES_NUM/MAX_DECODED_FRAME_SIZE-CUTOUT))
        debug_printf("ONLY THE FIRST %d FRAMES OF THE SAMPLE WILL BE PLAYED.\r\n", DAC_FRAME_BUFFERS_NUM);
    
    while(nextInFrameOffset+(MAX_DECODED_FRAME_SIZE*CUTOUT) < TEST_SAMPLES_NUM)
    {
        //if(i%(1024*256)) continue;
        
            
        frameSignedStart = (short*)(bin_data+nextInFrameOffset);
            
        if(g_LPC24XX_DAC_Driver.AddFrame(frameSignedStart, frameLength))
        {
            debug_printf( "     done.\r\n" );
            nextInFrameOffset+=MAX_DECODED_FRAME_SIZE;
        }
        else
        {
            debug_printf( "Buffer full, starting playout.\r\n");
            break;
        }
    }

    resetDACISRTiming();
    
        
    debug_printf( "DAC.On() in 2 seconds\r\n");
    Events_WaitForEvents( 0, 2000 );
    
    if(!hijackISRs())
        return;
    
    if(g_LPC24XX_DAC_Driver.On())
    {
        //debug_printf( "Done. 2sec wait.\r\n" ); don't output to avoid adding serial activity during the test
    }
    else
    {
        debug_printf( "FAILED.\r\n" );
    }
    
    while(g_LPC24XX_DAC_Driver.GetBufferLevel()>0)
    {
        //debug_printf("Samples left: %d\r\n", g_LPC24XX_DAC_Driver.GetBufferLevel());
        //debug_printf("Frames left:  %d\r\n", g_LPC24XX_DAC_Driver.GetFramesLeft());
    }
    
    //stop logging interrupts before starting to output again
    
    int finalIrqCount = irq_count;
    irq_count = 8192;
    
    Events_WaitForEvents( 0, 5000 );
    
    if(!restoreISRs())
        return;
    
    debug_printf("%d frames left.\r\n", g_LPC24XX_DAC_Driver.GetFramesLeft());
    debug_printf("Final IRQ count = %u\r\n", finalIrqCount);
    debug_printf( "BUFFER PRE-FILL TEST OVER.\r\n");
    
    displayRunTestResults();
    debug_printf("CSV DATA OUTPUT FOLLOWS\r\n");
    //csvRunTestResults();
    
    
    
    debug_printf("\r\nPARALLEL BUFFER FILL TEST\r\n" );
    
    
    Events_WaitForEvents( 0, 3000 );
    
    debug_printf( "DAC.Off()\r\n");
    if(g_LPC24XX_DAC_Driver.Off())
    {
        debug_printf( "Done.\r\n" );
    }
    else
    {
        debug_printf( "FAILED.\r\n" );
    }
    
    debug_printf( "Uninit DAC\r\n");
    g_LPC24XX_DAC_Driver.Uninitialize();
    debug_printf( "Done.\r\n");
    
    debug_printf( "Init DAC, 8kHz output.\r\n" );
    g_LPC24XX_DAC_Driver.Initialize(OUT_FREQ); 
    
    resetDACISRTiming();
    
    debug_printf( "DAC.On() in 2 seconds\r\n");
    Events_WaitForEvents( 0, 2000 );
    if(g_LPC24XX_DAC_Driver.On())
    {
        //debug_printf( "Done.\r\n" );
    }
    else
    {
        debug_printf( "FAILED.\r\n" );
    }
    
    debug_printf( "Adding frames to the DAC driver buffer: " );

    nextInFrameOffset=0;
    
    debug_printf("total frames to be added = %d\r\n", TEST_SAMPLES_NUM/MAX_DECODED_FRAME_SIZE-CUTOUT);
    
    //FILL JUST ONCE 
    while(nextInFrameOffset+(MAX_DECODED_FRAME_SIZE*CUTOUT) < TEST_SAMPLES_NUM)
    {
        //if(i%(1024*256)) continue;
        
            
        frameSignedStart = (short*)(bin_data+nextInFrameOffset);
            
        if(g_LPC24XX_DAC_Driver.AddFrame(frameSignedStart, frameLength))
        {
            debug_printf( "     done.\r\n" );
            nextInFrameOffset+=MAX_DECODED_FRAME_SIZE;
        }
        else
        {
            //debug_printf( "FAIL.\r\n");
        }
    }    
    
    while(g_LPC24XX_DAC_Driver.GetBufferLevel()>0)
    {
        //debug_printf("Samples left: %d\r\n", g_LPC24XX_DAC_Driver.GetBufferLevel());
        //debug_printf("Frames left:  %d\r\n", g_LPC24XX_DAC_Driver.GetFramesLeft());
    }
    
    Events_WaitForEvents( 0, 3000 );
    
    displayRunTestResults();
    
    debug_printf("CSV DATA OUTPUT FOLLOWS\r\n");
    csvRunTestResults();
    
    /*CONTINUOUS REFILL with samples
    while(true)
    {
        //if(i%(1024*256)) continue;
        
            
        frameSignedStart = (short*)(bin_data+nextInFrameOffset);
            
        if(g_LPC24XX_DAC_Driver.AddFrame(frameSignedStart, frameLength))
        {
            //debug_printf( "     done.\r\n" );
            nextInFrameOffset+=MAX_DECODED_FRAME_SIZE;
            if(nextInFrameOffset+(MAX_DECODED_FRAME_SIZE*CUTOUT)>=TEST_SAMPLES_NUM)
                nextInFrameOffset = 0;
        }
        else
        {
            //debug_printf( "FAIL.\r\n");
        }
        debug_printf("Samples left: %d\r\n", g_LPC24XX_DAC_Driver.GetBufferLevel());
        debug_printf("Frames left:  %d\r\n", g_LPC24XX_DAC_Driver.GetFramesLeft());
    }*///end continuous refill

        
    debug_printf("%d frames left.\r\n", g_LPC24XX_DAC_Driver.GetFramesLeft());
    debug_printf( "PARALLEL BUFFER FILL TEST OVER.\r\n\r\n" );
    
    
    //Events_WaitForEvents( 0, 10000 );
        
    debug_printf( "DAC.Off()\r\n");
    if(g_LPC24XX_DAC_Driver.Off())
    {
        debug_printf( "Done.\r\n" );
    }
    else
    {
        debug_printf( "FAILED.\r\n" );
    }
    
    debug_printf( "Uninit DAC()\r\n");
    g_LPC24XX_DAC_Driver.Uninitialize();
    debug_printf( "Done.\r\n");
    
#endif


#if defined(TEST_JOYSTICK)
    extern LPC24XX_GPIO_Driver g_LPC24XX_GPIO_Driver;
    
    wait_joystick = true;
    
    for(UINT32 pin = LPC24XX_GPIO::c_P2_22; pin < LPC24XX_GPIO::c_P2_28; pin++)
    {
        if(pin == LPC24XX_GPIO::c_P2_24)
            continue;
        if(!g_LPC24XX_GPIO_Driver.EnableInputPin( pin, false,  joystickISR, NULL, GPIO_INT_EDGE_HIGH, (GPIO_RESISTOR)2 ))
        {
            debug_printf("Cannot enable pin %u as INPUT pin.\r\n", pin);
            exit(1);
        }
        debug_printf("Enabled pin %u as INPUT pin.\r\n", pin);
    }
    
    while(wait_joystick){};
    
#endif
    
#if    defined(TEST_SST39WF)
    
    while(1)
    {
        lcd_printf  ( "Hello, world from the LCD!\r\n" );
        hal_printf  ( "Hello, world from the HAL!\r\n" );
        debug_printf( "Hello, world from the debug intf!\r\n" );
        if(BlockStorageList::GetNumDevices() != 1)
        {
            debug_printf( "%d Block Devices present!\r\n", BlockStorageList::GetNumDevices() );
            break;
        }
        
        BlockStorageDevice* SST = BlockStorageList::GetFirstDevice();
        if(SST == NULL)
        {
            debug_printf( "GetFirstDevice failed.\r\n" );
            break;
        }
        
        const BlockDeviceInfo* SSTInfo = SST->GetDeviceInfo();
        if(SSTInfo == NULL)
        {
            debug_printf( "GetDeviceInfo failed.\r\n" );
            break;
        }
        
        debug_printf( "NumRegions in BSDevice: %d\r\n", SSTInfo->NumRegions);
        
        ByteAddress PhyAddress = (ByteAddress) 0xC0FFEEEE;
        
        SectorAddress SectAddress = 0xC0FFEEEE;
        UINT32 RangeIndex;
        UINT32 RegionIndex;
        
        const BlockRegionInfo *pBlockRegionInfo;
        SST->FindForBlockUsage(    /*UINT32*/ BlockRange::BLOCKTYPE_DEPLOYMENT , 
                                  PhyAddress , RegionIndex, RangeIndex );
        if(PhyAddress == 0xC0FFEEEE)
        {
            debug_printf( "FindForBlockUsage failed.\r\n" );
            break;
        }
                            

        debug_printf( "Sector 0x%08x physical address: 0x%08x\r\n", SectAddress, PhyAddress);
        
        BYTE pSectorBuf[] = {0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF};
            
        //ERASE before writing!
        if(!SST->IsBlockErased(PhyAddress, 0x1000))
        {
            debug_printf( "Erasing block " );
            if(!SST->EraseBlock(SectAddress))
            {
                debug_printf( "failed.\r\n" );
                break;
            }
            debug_printf( "successful.\r\n" );
        }
        
        if(SST->Write(/*UINT32*/ PhyAddress, /*UINT32 NumOfBytes*/ 16, /*BYTE* */ pSectorBuf, /*SectorMetadata* */ FALSE))
            debug_printf( "Correctly written 16 bytes to Sector 0x%08x\r\n", SectAddress);

        Events_WaitForEvents( 0, 2000 );
    }
#endif        //TEST_SST39WF

#if defined(TEST_PWM)

    PWM_Initialize(PWM_CHANNEL_0);

    // NOTE: on the EA_LPC2478 board the first pin we will return is the 11th pin on the left side from the top of the J1 connector
    GPIO_PIN pin = PWM_GetPinForChannel( PWM_CHANNEL_0 );

    // from 90% to 2/3, to 50%, to 1/3 to 10% 
    float dc[5] = { 0.9, 0.666, 0.5, 0.333, 0.1 };
    UINT32 period1 = 1000; // 1Kxz
    for(UINT32 idx = 0; idx < 5; ++idx)
    {
        UINT32 duration1 = (UINT32)((float)period1 * dc[idx]);
        
        PWM_ApplyConfiguration( PWM_CHANNEL_0, pin, period1, duration1, FALSE);
        PWM_Start             ( PWM_CHANNEL_0, pin );

        // 2 secs, then change
        HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1 * 1000 * 1000);
        //Events_WaitForEvents( 0, 2 * 1000);
        
        PWM_Stop              ( PWM_CHANNEL_0, pin );
    }

    // from 10Khz to 1Khz, 50% duty cycle
    for(UINT32 period = 10000; period >= 1000; period -= 1000)
    {
        UINT32 duration = period / 2;
        
        PWM_ApplyConfiguration( PWM_CHANNEL_0, pin, period, duration, FALSE);
        PWM_Start             ( PWM_CHANNEL_0, pin );

        // 2 secs, then change
        HAL_Time_Sleep_MicroSeconds_InterruptEnabled(1 * 1000 * 1000);
        //Events_WaitForEvents( 0, 2 * 1000);
        
        PWM_Stop              ( PWM_CHANNEL_0, pin );
    }
    
    PWM_Uninitialize(PWM_CHANNEL_0);

#endif // TEST_PWM

    while(1)
    {
        lcd_printf  ( "Hello, world!\r\n" );
        hal_printf  ( "Hello, world!\r\n" );
        debug_printf( "Hello, world!\r\n" );

        
        Events_WaitForEvents( 0, 1000 );
    }

}
