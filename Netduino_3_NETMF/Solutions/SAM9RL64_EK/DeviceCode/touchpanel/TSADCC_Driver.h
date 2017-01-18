////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 

// *****************************************************************************
//              SOFTWARE API DEFINITION  FOR TSADCC 
// *****************************************************************************
#ifndef _AT91_TSADCC_H_1 
#define _AT91_TSADCC_H_1 		1
#include <CPU_GPIO_decl.h>

struct AT91S_TSADC {
	UINT32	 TSADC_CR; 	// Control Register
	UINT32	 TSADC_MR; 	// Mode Register
	UINT32	 TSADC_TRGR; 	// Trigger Register
	UINT32	 TSADC_TSR; 	// Touch Screen Register
	UINT32	 TSADC_CHER; 	// Channel Enable Register
	UINT32	 TSADC_CHDR; 	// Channel Disable Register
	UINT32	 TSADC_CHSR; 	// Channel Status Register
	UINT32	 TSADC_SR; 	// Status Register
	UINT32	 TSADC_LCDR; 	// Last Converted Register
	UINT32	 TSADC_IER; 	// Interrupt Enable Register
	UINT32	 TSADC_IDR; 	// Interrupt Disable Register
	UINT32	 TSADC_IMR; 	// Interrupt Mask Register
	UINT32	 TSADC_CDR0; 	// Channel Data Register 0
	UINT32	 TSADC_CDR1; 	// Channel Data Register 1
	UINT32	 TSADC_CDR2; 	// Channel Data Register 2
	UINT32	 TSADC_CDR3; 	// Channel Data Register 3
	UINT32	 TSADC_CDR4; 	// Channel Data Register 4
	UINT32	 TSADC_CDR5; 	// Channel Data Register 5
	UINT32	 TSADC_CDR6; 	// Channel Data Register 6
	UINT32	 TSADC_CDR7; 	// Channel Data Register 7
	UINT32	 Reserved0[44]; 	// 
	UINT32	 TSADC_RPR; 	// Receive Pointer Register
	UINT32	 TSADC_RCR; 	// Receive Counter Register
	UINT32	 TSADC_TPR; 	// Transmit Pointer Register
	UINT32	 TSADC_TCR; 	// Transmit Counter Register
	UINT32	 TSADC_RNPR; 	// Receive Next Pointer Register
	UINT32	 TSADC_RNCR; 	// Receive Next Counter Register
	UINT32	 TSADC_TNPR; 	// Transmit Next Pointer Register
	UINT32	 TSADC_TNCR; 	// Transmit Next Counter Register
	UINT32	 TSADC_PTCR; 	// PDC Transfer Control Register
	UINT32	 TSADC_PTSR; 	// PDC Transfer Status Register
};


// -------- TSADC_CR : (TSADC Offset: 0x0) Control Register -------- 
#define AT91C_TSADC_SWRST     (0x1 <<  0) // (TSADC) Software Reset
#define AT91C_TSADC_START     (0x1 <<  1) // (TSADC) Start Conversion
// -------- TSADC_MR : (TSADC Offset: 0x4) Mode Register -------- 
#define AT91C_TSADC_TSAMOD    (0x3 <<  0) // (TSADC) Touch Screen ADC Mode
#define 	AT91C_TSADC_TSAMOD_ADC_ONLY_MODE        (0x0) // (TSADC) ADC Mode
#define 	AT91C_TSADC_TSAMOD_TS_ONLY_MODE         (0x1) // (TSADC) Touch Screen Only Mode
#define AT91C_TSADC_LOWRES    (0x1 <<  4) // (TSADC) ADC Resolution
#define AT91C_TSADC_SLEEP     (0x1 <<  5) // (TSADC) Sleep Mode
#define AT91C_TSADC_PENDET    (0x1 <<  6) // (TSADC) Pen Detect Selection
#define AT91C_TSADC_PRESSURE  (0x1 <<  7) // (TSADC) Pressure Measurement Selection
#define AT91C_TSADC_PRESCAL   (0x3F <<  8) // (TSADC) Prescaler Rate Selection
#define AT91C_TSADC_STARTUP   (0x7F << 16) // (TSADC) Startup Time
#define AT91C_TSADC_SHTIM     (0xF << 24) // (TSADC) Sample and Hold Time for ADC Channels
#define AT91C_TSADC_PENDBC    (0xF << 28) // (TSADC) Pen Detect Debouncing Period
// -------- TSADC_TRGR : (TSADC Offset: 0x8) Trigger Register -------- 
#define AT91C_TSADC_TRGMOD    (0x7 <<  0) // (TSADC) Trigger Mode
#define 	AT91C_TSADC_TRGMOD_NO_TRIGGER           (0x0) // (TSADC) No Trigger
#define 	AT91C_TSADC_TRGMOD_EXTERNAL_TRIGGER_RE  (0x1) // (TSADC) External Trigger Rising Edge
#define 	AT91C_TSADC_TRGMOD_EXTERNAL_TRIGGER_FE  (0x2) // (TSADC) External Trigger Falling Edge
#define 	AT91C_TSADC_TRGMOD_EXTERNAL_TRIGGER_AE  (0x3) // (TSADC) External Trigger Any Edge
#define 	AT91C_TSADC_TRGMOD_PENDET_TRIGGER       (0x4) // (TSADC) Pen Detect Trigger (only if PENDET is set and in Touch Screen mode only)
#define 	AT91C_TSADC_TRGMOD_PERIODIC_TRIGGER     (0x5) // (TSADC) Periodic Trigger (wrt TRGPER)
#define 	AT91C_TSADC_TRGMOD_CONT_TRIGGER         (0x6) // (TSADC) Continuous Trigger
#define AT91C_TSADC_TRGPER    (0xFFFF << 16) // (TSADC) Trigger Period
// -------- TSADC_TSR : (TSADC Offset: 0xc) Touch Screen Register -------- 
#define AT91C_TSADC_TSSHTIM   (0xF << 24) // (TSADC) Sample and Hold Time for Touch Screen Channels
// -------- TSADC_CHER : (TSADC Offset: 0x10) Channel Enable Register -------- 
#define AT91C_TSADC_CHENA0    (0x1 <<  0) // (TSADC) Channel 0 Enable
#define AT91C_TSADC_CHENA1    (0x1 <<  1) // (TSADC) Channel 1 Enable
#define AT91C_TSADC_CHENA2    (0x1 <<  2) // (TSADC) Channel 2 Enable
#define AT91C_TSADC_CHENA3    (0x1 <<  3) // (TSADC) Channel 3 Enable
#define AT91C_TSADC_CHENA4    (0x1 <<  4) // (TSADC) Channel 4 Enable
#define AT91C_TSADC_CHENA5    (0x1 <<  5) // (TSADC) Channel 5 Enable
#define AT91C_TSADC_CHENA6    (0x1 <<  6) // (TSADC) Channel 6 Enable
#define AT91C_TSADC_CHENA7    (0x1 <<  7) // (TSADC) Channel 7 Enable
// -------- TSADC_CHDR : (TSADC Offset: 0x14) Channel Disable Register -------- 
#define AT91C_TSADC_CHDIS0    (0x1 <<  0) // (TSADC) Channel 0 Disable
#define AT91C_TSADC_CHDIS1    (0x1 <<  1) // (TSADC) Channel 1 Disable
#define AT91C_TSADC_CHDIS2    (0x1 <<  2) // (TSADC) Channel 2 Disable
#define AT91C_TSADC_CHDIS3    (0x1 <<  3) // (TSADC) Channel 3 Disable
#define AT91C_TSADC_CHDIS4    (0x1 <<  4) // (TSADC) Channel 4 Disable
#define AT91C_TSADC_CHDIS5    (0x1 <<  5) // (TSADC) Channel 5 Disable
#define AT91C_TSADC_CHDIS6    (0x1 <<  6) // (TSADC) Channel 6 Disable
#define AT91C_TSADC_CHDIS7    (0x1 <<  7) // (TSADC) Channel 7 Disable
// -------- TSADC_CHSR : (TSADC Offset: 0x18) Channel Status Register -------- 
#define AT91C_TSADC_CHS0      (0x1 <<  0) // (TSADC) Channel 0 Status
#define AT91C_TSADC_CHS1      (0x1 <<  1) // (TSADC) Channel 1 Status
#define AT91C_TSADC_CHS2      (0x1 <<  2) // (TSADC) Channel 2 Status
#define AT91C_TSADC_CHS3      (0x1 <<  3) // (TSADC) Channel 3 Status
#define AT91C_TSADC_CHS4      (0x1 <<  4) // (TSADC) Channel 4 Status
#define AT91C_TSADC_CHS5      (0x1 <<  5) // (TSADC) Channel 5 Status
#define AT91C_TSADC_CHS6      (0x1 <<  6) // (TSADC) Channel 6 Status
#define AT91C_TSADC_CHS7      (0x1 <<  7) // (TSADC) Channel 7 Status
// -------- TSADC_SR : (TSADC Offset: 0x1c) Status Register -------- 
#define AT91C_TSADC_EOC0      (0x1 <<  0) // (TSADC) Channel 0 End Of Conversion
#define AT91C_TSADC_EOC1      (0x1 <<  1) // (TSADC) Channel 1 End Of Conversion
#define AT91C_TSADC_EOC2      (0x1 <<  2) // (TSADC) Channel 2 End Of Conversion
#define AT91C_TSADC_EOC3      (0x1 <<  3) // (TSADC) Channel 3 End Of Conversion
#define AT91C_TSADC_EOC4      (0x1 <<  4) // (TSADC) Channel 4 End Of Conversion
#define AT91C_TSADC_EOC5      (0x1 <<  5) // (TSADC) Channel 5 End Of Conversion
#define AT91C_TSADC_EOC6      (0x1 <<  6) // (TSADC) Channel 6 End Of Conversion
#define AT91C_TSADC_EOC7      (0x1 <<  7) // (TSADC) Channel 7 End Of Conversion
#define AT91C_TSADC_OVRE0     (0x1 <<  8) // (TSADC) Channel 0 Overrun Error
#define AT91C_TSADC_OVRE1     (0x1 <<  9) // (TSADC) Channel 1 Overrun Error
#define AT91C_TSADC_OVRE2     (0x1 << 10) // (TSADC) Channel 2 Overrun Error
#define AT91C_TSADC_OVRE3     (0x1 << 11) // (TSADC) Channel 3 Overrun Error
#define AT91C_TSADC_OVRE4     (0x1 << 12) // (TSADC) Channel 4 Overrun Error
#define AT91C_TSADC_OVRE5     (0x1 << 13) // (TSADC) Channel 5 Overrun Error
#define AT91C_TSADC_OVRE6     (0x1 << 14) // (TSADC) Channel 6 Overrun Error
#define AT91C_TSADC_OVRE7     (0x1 << 15) // (TSADC) Channel 7 Overrun Error
#define AT91C_TSADC_DRDY      (0x1 << 16) // (TSADC) Data Ready
#define AT91C_TSADC_GOVRE     (0x1 << 17) // (TSADC) General Overrun Error
#define AT91C_TSADC_ENDRX     (0x1 << 18) // (TSADC) End of RX Buffer
#define AT91C_TSADC_RXBUFF    (0x1 << 19) // (TSADC) RX Buffer Full
#define AT91C_TSADC_PENCNT    (0x1 << 20) // (TSADC) Pen Contact
#define AT91C_TSADC_NOCNT     (0x1 << 21) // (TSADC) No Contact
// -------- TSADC_LCDR : (TSADC Offset: 0x20) Last Converted Data Register -------- 
#define AT91C_TSADC_LDATA     (0x3FF <<  0) // (TSADC) Last Converted Data
// -------- TSADC_IER : (TSADC Offset: 0x24) Interrupt Enable Register -------- 
#define AT91C_TSADC_IENAEOC0  (0x1 <<  0) // (TSADC) Channel 0 End Of Conversion Interrupt Enable
#define AT91C_TSADC_IENAEOC1  (0x1 <<  1) // (TSADC) Channel 1 End Of Conversion Interrupt Enable
#define AT91C_TSADC_IENAEOC2  (0x1 <<  2) // (TSADC) Channel 2 End Of Conversion Interrupt Enable
#define AT91C_TSADC_IENAEOC3  (0x1 <<  3) // (TSADC) Channel 3 End Of Conversion Interrupt Enable
#define AT91C_TSADC_IENAEOC4  (0x1 <<  4) // (TSADC) Channel 4 End Of Conversion Interrupt Enable
#define AT91C_TSADC_IENAEOC5  (0x1 <<  5) // (TSADC) Channel 5 End Of Conversion Interrupt Enable
#define AT91C_TSADC_IENAEOC6  (0x1 <<  6) // (TSADC) Channel 6 End Of Conversion Interrupt Enable
#define AT91C_TSADC_IENAEOC7  (0x1 <<  7) // (TSADC) Channel 7 End Of Conversion Interrupt Enable
#define AT91C_TSADC_IENAOVRE0 (0x1 <<  8) // (TSADC) Channel 0 Overrun Error Interrupt Enable
#define AT91C_TSADC_IENAOVRE1 (0x1 <<  9) // (TSADC) Channel 1 Overrun Error Interrupt Enable
#define AT91C_TSADC_IENAOVRE2 (0x1 << 10) // (TSADC) Channel 2 Overrun Error Interrupt Enable
#define AT91C_TSADC_IENAOVRE3 (0x1 << 11) // (TSADC) Channel 3 Overrun Error Interrupt Enable
#define AT91C_TSADC_IENAOVRE4 (0x1 << 12) // (TSADC) Channel 4 Overrun Error Interrupt Enable
#define AT91C_TSADC_IENAOVRE5 (0x1 << 13) // (TSADC) Channel 5 Overrun Error Interrupt Enable
#define AT91C_TSADC_IENAOVRE6 (0x1 << 14) // (TSADC) Channel 6 Overrun Error Interrupt Enable
#define AT91C_TSADC_IENAOVRE7 (0x1 << 15) // (TSADC) Channel 7 Overrun Error Interrupt Enable
#define AT91C_TSADC_IENADRDY  (0x1 << 16) // (TSADC) Data Ready Interrupt Enable
#define AT91C_TSADC_IENAGOVRE (0x1 << 17) // (TSADC) General Overrun Error Interrupt Enable
#define AT91C_TSADC_IENAENDRX (0x1 << 18) // (TSADC) End of RX Buffer Interrupt Enable
#define AT91C_TSADC_IENARXBUFF (0x1 << 19) // (TSADC) RX Buffer Full Interrupt Enable
#define AT91C_TSADC_IENAPENCNT (0x1 << 20) // (TSADC) Pen Contact Interrupt Enable
#define AT91C_TSADC_IENANOCNT (0x1 << 21) // (TSADC) No Contact Interrupt Enable
// -------- TSADC_IDR : (TSADC Offset: 0x28) Interrupt Disable Register -------- 
#define AT91C_TSADC_IDISEOC0  (0x1 <<  0) // (TSADC) Channel 0 End Of Conversion Interrupt Disable
#define AT91C_TSADC_IDISEOC1  (0x1 <<  1) // (TSADC) Channel 1 End Of Conversion Interrupt Disable
#define AT91C_TSADC_IDISEOC2  (0x1 <<  2) // (TSADC) Channel 2 End Of Conversion Interrupt Disable
#define AT91C_TSADC_IDISEOC3  (0x1 <<  3) // (TSADC) Channel 3 End Of Conversion Interrupt Disable
#define AT91C_TSADC_IDISEOC4  (0x1 <<  4) // (TSADC) Channel 4 End Of Conversion Interrupt Disable
#define AT91C_TSADC_IDISEOC5  (0x1 <<  5) // (TSADC) Channel 5 End Of Conversion Interrupt Disable
#define AT91C_TSADC_IDISEOC6  (0x1 <<  6) // (TSADC) Channel 6 End Of Conversion Interrupt Disable
#define AT91C_TSADC_IDISEOC7  (0x1 <<  7) // (TSADC) Channel 7 End Of Conversion Interrupt Disable
#define AT91C_TSADC_IDISOVRE0 (0x1 <<  8) // (TSADC) Channel 0 Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISOVRE1 (0x1 <<  9) // (TSADC) Channel 1 Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISOVRE2 (0x1 << 10) // (TSADC) Channel 2 Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISOVRE3 (0x1 << 11) // (TSADC) Channel 3 Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISOVRE4 (0x1 << 12) // (TSADC) Channel 4 Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISOVRE5 (0x1 << 13) // (TSADC) Channel 5 Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISOVRE6 (0x1 << 14) // (TSADC) Channel 6 Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISOVRE7 (0x1 << 15) // (TSADC) Channel 7 Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISDRDY  (0x1 << 16) // (TSADC) Data Ready Interrupt Disable
#define AT91C_TSADC_IDISGOVRE (0x1 << 17) // (TSADC) General Overrun Error Interrupt Disable
#define AT91C_TSADC_IDISENDRX (0x1 << 18) // (TSADC) End of RX Buffer Interrupt Disable
#define AT91C_TSADC_IDISRXBUFF (0x1 << 19) // (TSADC) RX Buffer Full Interrupt Disable
#define AT91C_TSADC_IDISPENCNT (0x1 << 20) // (TSADC) Pen Contact Interrupt Disable
#define AT91C_TSADC_IDISNOCNT (0x1 << 21) // (TSADC) No Contact Interrupt Disable
// -------- TSADC_IMR : (TSADC Offset: 0x2c) Interrupt Mask Register -------- 
#define AT91C_TSADC_IMSKEOC0  (0x1 <<  0) // (TSADC) Channel 0 End Of Conversion Interrupt Mask
#define AT91C_TSADC_IMSKEOC1  (0x1 <<  1) // (TSADC) Channel 1 End Of Conversion Interrupt Mask
#define AT91C_TSADC_IMSKEOC2  (0x1 <<  2) // (TSADC) Channel 2 End Of Conversion Interrupt Mask
#define AT91C_TSADC_IMSKEOC3  (0x1 <<  3) // (TSADC) Channel 3 End Of Conversion Interrupt Mask
#define AT91C_TSADC_IMSKEOC4  (0x1 <<  4) // (TSADC) Channel 4 End Of Conversion Interrupt Mask
#define AT91C_TSADC_IMSKEOC5  (0x1 <<  5) // (TSADC) Channel 5 End Of Conversion Interrupt Mask
#define AT91C_TSADC_IMSKEOC6  (0x1 <<  6) // (TSADC) Channel 6 End Of Conversion Interrupt Mask
#define AT91C_TSADC_IMSKEOC7  (0x1 <<  7) // (TSADC) Channel 7 End Of Conversion Interrupt Mask
#define AT91C_TSADC_IMSKOVRE0 (0x1 <<  8) // (TSADC) Channel 0 Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKOVRE1 (0x1 <<  9) // (TSADC) Channel 1 Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKOVRE2 (0x1 << 10) // (TSADC) Channel 2 Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKOVRE3 (0x1 << 11) // (TSADC) Channel 3 Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKOVRE4 (0x1 << 12) // (TSADC) Channel 4 Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKOVRE5 (0x1 << 13) // (TSADC) Channel 5 Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKOVRE6 (0x1 << 14) // (TSADC) Channel 6 Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKOVRE7 (0x1 << 15) // (TSADC) Channel 7 Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKDRDY  (0x1 << 16) // (TSADC) Data Ready Interrupt Mask
#define AT91C_TSADC_IMSKGOVRE (0x1 << 17) // (TSADC) General Overrun Error Interrupt Mask
#define AT91C_TSADC_IMSKENDRX (0x1 << 18) // (TSADC) End of RX Buffer Interrupt Mask
#define AT91C_TSADC_IMSKRXBUFF (0x1 << 19) // (TSADC) RX Buffer Full Interrupt Mask
#define AT91C_TSADC_IMSKPENCNT (0x1 << 20) // (TSADC) Pen Contact Interrupt Mask
#define AT91C_TSADC_IMSKNOCNT (0x1 << 21) // (TSADC) No Contact Interrupt Mask
// -------- TSADC_CDR0 : (TSADC Offset: 0x30) Channel 0 Data Register -------- 
#define AT91C_TSADC_DATA0     (0x3FF <<  0) // (TSADC) Channel 0 Data
// -------- TSADC_CDR1 : (TSADC Offset: 0x34) Channel 1 Data Register -------- 
#define AT91C_TSADC_DATA1     (0x3FF <<  0) // (TSADC) Channel 1 Data
// -------- TSADC_CDR2 : (TSADC Offset: 0x38) Channel 2 Data Register -------- 
#define AT91C_TSADC_DATA2     (0x3FF <<  0) // (TSADC) Channel 2 Data
// -------- TSADC_CDR3 : (TSADC Offset: 0x3c) Channel 3 Data Register -------- 
#define AT91C_TSADC_DATA3     (0x3FF <<  0) // (TSADC) Channel 3 Data
// -------- TSADC_CDR4 : (TSADC Offset: 0x40) Channel 4 Data Register -------- 
#define AT91C_TSADC_DATA4     (0x3FF <<  0) // (TSADC) Channel 4 Data
// -------- TSADC_CDR5 : (TSADC Offset: 0x44) Channel 5 Data Register -------- 
#define AT91C_TSADC_DATA5     (0x3FF <<  0) // (TSADC) Channel 5 Data
// -------- TSADC_CDR6 : (TSADC Offset: 0x48) Channel 6 Data Register -------- 
#define AT91C_TSADC_DATA6     (0x3FF <<  0) // (TSADC) Channel 6 Data
// -------- TSADC_CDR7 : (TSADC Offset: 0x4c) Channel 7 Data Register -------- 
#define AT91C_TSADC_DATA7     (0x3FF <<  0) // (TSADC) Channel 7 Data

#define SAMPLE_NB                  10
#define ADC_CLOCK                  1000000
#define STARTUP_TIME               127ul
#define PEN_DEBOUNCE_TIME          9ul
#define SAMPLE_AND_HOLD_TIME       15ul
#define MIN_GOOD_SAMPLES           (SAMPLE_NB/2 + SAMPLE_NB%2)
#define MAX_DELTA                  1
//////////////////////////////////////////////////////////////////////////////
// AT91_TSADCC_Driver
//
struct AT91_TSADCC_Driver
{
	static BOOL Initialize(GPIO_INTERRUPT_SERVICE_ROUTINE touchIsrProc);
	static BOOL Uninitialize();
	static void GetPoint(TOUCH_PANEL_SAMPLE_FLAGS* pTipState, int* pSource, int* pUnCalX, int* pUnCalY);
	static HRESULT GetDeviceCaps(unsigned int iIndex, void* lpOutput);
	static BOOL CalibrationPointGet(TOUCH_PANEL_CALIBRATION_POINT* point);
	static BOOL SamplesFilter(INT32 tiSample[], const INT32 ciNbSamples, const INT32 ciMinNbGoodSamples, INT32 iSum, INT32 *piAverage);
	static void TSADCC_ISR(void* Param );
	static INT32 abs(INT32 input);
	GPIO_INTERRUPT_SERVICE_ROUTINE m_touchIsrProc;
	
	TOUCH_PANEL_SAMPLE_FLAGS m_TipState;
	int m_UnCalX;
	int m_UnCalY;
	
	INT32 m_stiX_Samples[SAMPLE_NB];
	INT32 m_stiY_Samples[SAMPLE_NB];
	UINT32 m_sdwSampleIndex;
	UINT32 m_sdwSampleNo;
	INT32 m_siX_SampleSum;
	INT32 m_siY_SampleSum;
	
};
#endif
