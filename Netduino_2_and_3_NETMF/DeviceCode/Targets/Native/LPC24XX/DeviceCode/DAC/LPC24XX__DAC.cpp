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

//#define TIME_DAC_ISR

#if defined(TIME_DAC_ISR)
    //this must match the one in the test sample stream
    #define TEST_SAMPLES_NUM 8000
    UINT32 execStartTicks[TEST_SAMPLES_NUM];
    UINT32 timesRun;
    #include <math.h>
#endif


//#define DECODER_TIMING 1
//#define DAC_STOP_TIMING 1
    
#if defined(DECODER_TIMING) || defined(DAC_STOP_TIMING)
    #define TIMED_FRAMES 150
#endif

//#define MONITOR_BUFFER_LEVEL 1

#if defined(MONITOR_BUFFER_LEVEL)
    UINT32 bufferLevel[80];
    UINT16 bufLevelMarkCount = 0;
    UINT32 bufferMonitorSampleCount = 0;
#endif

extern UINT16 framesDecoded;

#if defined(DECODER_TIMING)
    extern UINT16 decodingTimesMs[TIMED_FRAMES];
    extern BOOL decTimingAlreadyOutput;
#endif
    
#if defined(DAC_STOP_TIMING)
    extern UINT16 stopTimesUs[TIMED_FRAMES];
    extern BOOL dacTimingAlreadyOutput;
#endif
    
///////////////////////////////////////////////////////////////////////////////

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata = "g_LPC24XX_DAC_Driver"
#endif
    
#pragma arm section zidata = "SectionForInternalSRAM"
LPC24XX_DAC_Driver g_LPC24XX_DAC_Driver;
#pragma arm section zidata

#if defined(ADS_LINKER_BUG__NOT_ALL_UNUSED_VARIABLES_ARE_REMOVED)
#pragma arm section zidata
#endif


//--//

void LPC24XX_DAC_Driver::Initialize( UINT32 SampleFrequencyHz )
{
    //I'm leaving PCLK_DAC in PCLKSEL0 to the DEFAULT value (00)
    
    //SET PINSEL1 to DAC
    CPU_GPIO_DisablePin( LPC24XX_DAC::c_DAC_AOUT, RESISTOR_DISABLED, GPIO_ATTRIBUTE_NONE, GPIO_ALT_MODE_2 );
    
    //Init the SampleBuffer as empty.
    g_LPC24XX_DAC_Driver.nextFrameWrite = 0;
    g_LPC24XX_DAC_Driver.nextSampleRead = DAC_FRAME_BUFFER_SIZE_SAMPLES;
    g_LPC24XX_DAC_Driver.nextFrameRead = DAC_FRAME_BUFFERS_NUM;
    g_LPC24XX_DAC_Driver.SampleCount = 0;
    g_LPC24XX_DAC_Driver.FrameCount = 0;
    for(int i =0; i<DAC_FRAME_BUFFERS_NUM; i++)
        g_LPC24XX_DAC_Driver.SamplesInFrame[i]=0;
    
    //Set Sample output lenght
    UINT32 SampleTimeInUsecs = 1000000/SampleFrequencyHz;
    UINT32 CycleTimeInNanosecs = 1000000000/LPC24XX_DAC_PCLK;
    g_LPC24XX_DAC_Driver.SampleTimeInCycles = SampleTimeInUsecs * 1000 / CycleTimeInNanosecs;
    
    /*
    debug_printf("SampleTime= %dus, SYSTEM_CLOCK_HZ=%dHz (1 cycle = %dns)\r\n",SampleTimeInUsecs,SYSTEM_CLOCK_HZ,CycleTimeInNanosecs);
    debug_printf("SampleTime= %dcycles =  %dus * 1000 / %dns\r\n",g_LPC24XX_DAC_Driver.SampleTimeInCycles,SampleTimeInUsecs,CycleTimeInNanosecs);
    */
    
    LPC24XX_DAC& DAC = LPC24XX::DAC();
    //set the OUTPUT value at half scale
    DAC.DACR = 0x7FC0;
}

void LPC24XX_DAC_Driver::Uninitialize()
{
    // Disable the timer in case it is running
    g_LPC24XX_DAC_Driver.Off();
    
    //Reset the SampleBuffer as empty.
    g_LPC24XX_DAC_Driver.nextFrameWrite = 0;
    g_LPC24XX_DAC_Driver.nextSampleRead = DAC_FRAME_BUFFER_SIZE_SAMPLES;
    g_LPC24XX_DAC_Driver.nextFrameRead = DAC_FRAME_BUFFERS_NUM;
    g_LPC24XX_DAC_Driver.SampleCount = 0;
    g_LPC24XX_DAC_Driver.FrameCount = 0;
        
    //We check this to make sure the driver is correctly initialized
    g_LPC24XX_DAC_Driver.SampleTimeInCycles = 0;
    
#if defined(DECODER_TIMING)
        
        if((framesDecoded >= TIMED_FRAMES)&&(!decTimingAlreadyOutput))
        {
            debug_printf("\r\n=====DECODER_TIMING=====\r\n");
            for(int i = 0; i < TIMED_FRAMES; i++)
                debug_printf("%u\r\n", decodingTimesMs[i]);
            debug_printf("========================\r\n");
            decTimingAlreadyOutput = true;
#if !defined(DAC_STOP_TIMING)
            return;
#endif
        }
#endif
#if defined(DAC_STOP_TIMING)
        if((framesDecoded >= TIMED_FRAMES)&&(!dacTimingAlreadyOutput))
        {
            debug_printf("\r\n=====DAC_STOP_TIMING=====\r\n");
            for(int i = 0; i < TIMED_FRAMES; i++)
                debug_printf("%u\r\n", stopTimesUs[i]);
            debug_printf("========================\r\n");
            dacTimingAlreadyOutput = true;
            return;
        }
#endif
}

BOOL LPC24XX_DAC_Driver::On()
{
    if(g_LPC24XX_DAC_Driver.SampleTimeInCycles == 0) return FALSE;
    
    ASSERT(LPC24XX_DAC::Timer < LPC24XX_TIMER_Driver::c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_LPC24XX_TIMER_Driver.m_configured[LPC24XX_DAC::Timer] == TRUE) return FALSE;

    //--//
    
    //rise IRQ priority to the highest one. this guarantees delay < execution time of the longest ISR in the system
    LPC24XX_VIC& VIC = LPC24XX::VIC();    
    VIC.VECTPRIORITY[LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer)] = 0;

    if(!CPU_INTC_ActivateInterrupt( LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer) , LPC24XX_DAC_Driver::ISR, NULL )) return FALSE;
    

    LPC24XX_TIMER& TIMER = LPC24XX::TIMER(LPC24XX_DAC::Timer);

    //Reset and Enable timer
    TIMER.TCR = 0x00000002;
    TIMER.TCR = LPC24XX_TIMER::TCR_TEN;
    
    //set prescaler to 0, we only need very short time intervals
    TIMER.PR = 0x00000000;
    
    //load MATCH REGISTER with TARGET time (expressed in cycles, since PR=0)
    TIMER.MR0 = g_LPC24XX_DAC_Driver.SampleTimeInCycles;
        
    //set MatchControlRegister to generate INTR (bit 0) on MR0 == TC and reset timer (bit 1) immediately 
    //this way the IRQ will be generated every X us, regardless of when the ISR for the last one is actually executed
    
    TIMER.MCR = 0x00000003; 
    
    //--//

    g_LPC24XX_TIMER_Driver.m_configured[LPC24XX_DAC::Timer] = TRUE;

    return TRUE;
}

BOOL LPC24XX_DAC_Driver::Off()
{
    if(g_LPC24XX_DAC_Driver.SampleTimeInCycles == 0) return FALSE;
      
    ASSERT(LPC24XX_DAC::Timer < LPC24XX_TIMER_Driver::c_MaxTimers);

    GLOBAL_LOCK(irq);

    if(g_LPC24XX_TIMER_Driver.m_configured[LPC24XX_DAC::Timer] == FALSE) return FALSE;

    //--//
    
    //reset IRQ priority
    LPC24XX_VIC& VIC = LPC24XX::VIC();    
    VIC.VECTPRIORITY[LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer)] = 0xF;
    
    if(!CPU_INTC_DeactivateInterrupt( LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer) )) return FALSE;

    LPC24XX_TIMER& TIMER = LPC24XX::TIMER( LPC24XX_DAC::Timer );

    //Reset timer
    TIMER.TCR = 0x2;
    //Reset Match0 and Match Control regs
    TIMER.MR0 = 0x0;
    TIMER.MCR = 0x0;
    // disable Timer
    TIMER.TCR = 0;

    //--//

    
    g_LPC24XX_TIMER_Driver.m_configured[LPC24XX_DAC::Timer] = FALSE;

    #if defined(TIME_DAC_ISR)
    
    UINT16 maxSamplesUsed = 6400;
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
    
    #if defined(MONITOR_BUFFER_LEVEL)
    for(bufLevelMarkCount = 0; bufLevelMarkCount<80; bufLevelMarkCount++)
        debug_printf("%u\r\n",bufferLevel[bufLevelMarkCount]);
    bufLevelMarkCount = 0;
    #endif
    return TRUE;
}

//return whether the DAC is converting or not
BOOL LPC24XX_DAC_Driver::IsEnabled()
{
    return g_LPC24XX_TIMER_Driver.m_configured[LPC24XX_DAC::Timer];
}

//Samples in the buffer
UINT32 LPC24XX_DAC_Driver::GetBufferLevel()
{
    return g_LPC24XX_DAC_Driver.SampleCount;
}

//Max SampleBuffer Capacity (in 16bit samples)
UINT32 LPC24XX_DAC_Driver::GetBufferCapacity()
{
    return DAC_FRAME_BUFFER_SIZE_SAMPLES*DAC_FRAME_BUFFERS_NUM;
}

//FRAMES in the buffer
UINT32 LPC24XX_DAC_Driver::GetFramesLeft()
{
    return g_LPC24XX_DAC_Driver.FrameCount;
}
    
//Max SampleBuffer Capacity (in FRAMES)
UINT32 LPC24XX_DAC_Driver::GetBufferFrameCapacity()
{
    return DAC_FRAME_BUFFERS_NUM;
}
    
//return false if there was not enough space left
BOOL LPC24XX_DAC_Driver::AddFrame(short const * const Samples, UINT32 const SamplesNum)
{
    if(g_LPC24XX_DAC_Driver.SampleTimeInCycles == 0)
        return FALSE;
    
    if(g_LPC24XX_DAC_Driver.FrameCount >= DAC_FRAME_BUFFERS_NUM)
        return FALSE;
    
    if(SamplesNum > DAC_FRAME_BUFFER_SIZE_SAMPLES)
        return FALSE;

    
    if(g_LPC24XX_DAC_Driver.nextFrameWrite==DAC_FRAME_BUFFERS_NUM)
            g_LPC24XX_DAC_Driver.nextFrameWrite = 0;
    UINT32 nextFrameWriteOffset = g_LPC24XX_DAC_Driver.nextFrameWrite*DAC_FRAME_BUFFER_SIZE_SAMPLES;
    
    
    for(int i=0; i < SamplesNum; i++)
    {
        g_LPC24XX_DAC_Driver.SamplesBuffer[nextFrameWriteOffset+i] = Samples[i];
    }
    
    g_LPC24XX_DAC_Driver.SamplesInFrame[g_LPC24XX_DAC_Driver.nextFrameWrite] = SamplesNum;
    g_LPC24XX_DAC_Driver.nextFrameWrite++;
    
    //disable output timer IRQ
    BOOL wasEnabled = CPU_INTC_InterruptDisable( LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer));
    
    g_LPC24XX_DAC_Driver.FrameCount++;
    g_LPC24XX_DAC_Driver.SampleCount+=SamplesNum;
    
    //re-enable output timer IRQ
    if(wasEnabled)
        CPU_INTC_InterruptEnable( LPC24XX_TIMER::getIntNo(LPC24XX_DAC::Timer));
    
    //debug_printf("SampleCount: %d\r\n", g_LPC24XX_DAC_Driver.SampleCount);
    //debug_printf("FrameCount: %d\r\n", g_LPC24XX_DAC_Driver.FrameCount);
    
    //debug_printf("nextFrameWrite: %d\r\n", g_LPC24XX_DAC_Driver.nextFrameWrite);
    //debug_printf("nextFrameRead: %d\r\n", g_LPC24XX_DAC_Driver.nextFrameRead);
    //for (int i = 0 ; i < DAC_FRAME_BUFFERS_NUM; i++)
    //{
    //    debug_printf("SamplesInFrame[%d]: %d\r\n", i, g_LPC24XX_DAC_Driver.SamplesInFrame[i]);
    //}
    
    return TRUE;
}


#pragma arm section code = "SectionForInternalFLASH"
//Output one sample when the timer goes off.
void LPC24XX_DAC_Driver::ISR(void* Param)
{    
#if defined(TIME_DAC_ISR)
    if(timesRun<TEST_SAMPLES_NUM)
    {
        execStartTicks[timesRun] = /*(UINT32)*/Time_CurrentTicks();
        timesRun++;
    }
#endif

#if defined(MONITOR_BUFFER_LEVEL)
    bufferMonitorSampleCount++;
    if(!(bufferMonitorSampleCount%2000)) /*mark the buf lvl 4 times a sec @ 8kHz*/
    {
        bufferLevel[bufLevelMarkCount] =(g_LPC24XX_DAC_Driver.FrameCount*100)/DAC_FRAME_BUFFERS_NUM;
        if(++bufLevelMarkCount==80)
            bufLevelMarkCount=0;
    }
#endif

    
    LPC24XX_TIMER& TIMER = LPC24XX::TIMER(LPC24XX_DAC::Timer);
        
    //Reset match interrupt
    TIMER.IR = LPC24XX_TIMER::MR0_RESET;
    
    LPC24XX_DAC& DAC = LPC24XX::DAC();
    
    if(g_LPC24XX_DAC_Driver.SampleCount>0)
    {
        if((g_LPC24XX_DAC_Driver.nextSampleRead>=g_LPC24XX_DAC_Driver.SamplesInFrame[g_LPC24XX_DAC_Driver.nextFrameRead])
            ||(g_LPC24XX_DAC_Driver.nextSampleRead>=DAC_FRAME_BUFFER_SIZE_SAMPLES))    //if we reached the end of frame buffer
        {
            g_LPC24XX_DAC_Driver.nextSampleRead = 0;
            g_LPC24XX_DAC_Driver.nextFrameRead++;                                                //jump to the next frame
            g_LPC24XX_DAC_Driver.FrameCount--;
            if(g_LPC24XX_DAC_Driver.nextFrameRead>=DAC_FRAME_BUFFERS_NUM)                            //if we reached the last frame
                g_LPC24XX_DAC_Driver.nextFrameRead = 0;                                                //jump back to the first one
            //debug_printf("\r\nFrame %u:\r\n", (g_LPC24XX_DAC_Driver.nextFrameRead>=DAC_FRAME_BUFFERS_NUM)?0:g_LPC24XX_DAC_Driver.nextFrameRead);
        }
        
        
            
        // switch to unsigned
        // zero out the reserved bits
        // add the bias bit 
        // write to DACR
        unsigned short sample = 0x7FFF+(signed long)g_LPC24XX_DAC_Driver.SamplesBuffer[g_LPC24XX_DAC_Driver.nextFrameRead*DAC_FRAME_BUFFER_SIZE_SAMPLES+g_LPC24XX_DAC_Driver.nextSampleRead];
        DAC.DACR = DAC.TRADE_SPEED_FOR_POWER | (DAC.VALUE_MASK & (sample));
        
        g_LPC24XX_DAC_Driver.SampleCount--;
        g_LPC24XX_DAC_Driver.nextSampleRead++;
    }
    else
    {
        //buffer empty, zero out the output
        DAC.DACR = 0x7FC0;
    }
}
#pragma arm section code
