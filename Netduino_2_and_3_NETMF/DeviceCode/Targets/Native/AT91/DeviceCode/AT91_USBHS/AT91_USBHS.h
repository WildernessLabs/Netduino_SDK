////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// This file is part of the Microsoft .NET Micro Framerwork Porting Kit Code Samples and is unsupported. 
// Copyright (C) Microsoft Corporation. All rights reserved. Use of this sample source code is subject to 
// the terms of the Microsoft license agreement under which you licensed this sample source code. 
// 
// THIS SAMPLE CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AT91__USBHS_H_1
#define _AT91__USBHS_H_1   1

///////////////////////////////////////////////////////////////////////////////////////////////
// AT91_UDPHS
//
struct AT91_UDPHS_EPT {
	volatile UINT32	 UDPHS_EPTCFG; 	// UDPHS Endpoint Config Register
	volatile UINT32	 UDPHS_EPTCTLENB; 	// UDPHS Endpoint Control Enable Register
	volatile UINT32	 UDPHS_EPTCTLDIS; 	// UDPHS Endpoint Control Disable Register
	volatile UINT32	 UDPHS_EPTCTL; 	// UDPHS Endpoint Control Register
	volatile UINT32	 Reserved0[1]; 	// 
	volatile UINT32	 UDPHS_EPTSETSTA; 	// UDPHS Endpoint Set Status Register
	volatile UINT32	 UDPHS_EPTCLRSTA; 	// UDPHS Endpoint Clear Status Register
	volatile UINT32	 UDPHS_EPTSTA; 	// UDPHS Endpoint Status Register
};


///////////////////////////////////////////////////////////////////////////////////////////////

struct AT91_UDPHS_DMA {
	volatile UINT32	 UDPHS_DMANXTDSC; 	// UDPHS DMA Channel Next Descriptor Address
	volatile UINT32	 UDPHS_DMAADDRESS; 	// UDPHS DMA Channel Address Register
	volatile UINT32	 UDPHS_DMACONTROL; 	// UDPHS DMA Channel Control Register
	volatile UINT32	 UDPHS_DMASTATUS; 	// UDPHS DMA Channel Status Register
};

//////////////////////////////////////////////////////////////////////////////
struct AT91_UDPHS {
	volatile UINT32	 UDPHS_CTRL; 	// UDPHS Control Register
	volatile UINT32	 UDPHS_FNUM; 	// UDPHS Frame Number Register
	volatile UINT32	 Reserved0[2]; 	// 
	volatile UINT32	 UDPHS_IEN; 	// UDPHS Interrupt Enable Register
	volatile UINT32	 UDPHS_INTSTA; 	// UDPHS Interrupt Status Register
	volatile UINT32	 UDPHS_CLRINT; 	// UDPHS Clear Interrupt Register
	volatile UINT32	 UDPHS_EPTRST; 	// UDPHS Endpoints Reset Register
	volatile UINT32	 Reserved1[44]; 	// 
	volatile UINT32	 UDPHS_TSTSOFCNT; 	// UDPHS Test SOF Counter Register
	volatile UINT32	 UDPHS_TSTCNTA; 	// UDPHS Test A Counter Register
	volatile UINT32	 UDPHS_TSTCNTB; 	// UDPHS Test B Counter Register
	volatile UINT32	 UDPHS_TSTMODREG; 	// UDPHS Test Mode Register
	volatile UINT32	 UDPHS_TST; 	// UDPHS Test Register
	volatile UINT32	 Reserved2[2]; 	// 
	volatile UINT32	 UDPHS_RIPPADDRSIZE; 	// UDPHS PADDRSIZE Register
	volatile UINT32	 UDPHS_RIPNAME1; 	// UDPHS Name1 Register
	volatile UINT32	 UDPHS_RIPNAME2; 	// UDPHS Name2 Register
	volatile UINT32	 UDPHS_IPFEATURES; 	// UDPHS Features Register
	volatile UINT32	 UDPHS_IPVERSION; 	// UDPHS Version Register
	struct AT91_UDPHS_EPT	 UDPHS_EPT[16]; 	// UDPHS Endpoint struct
	struct AT91_UDPHS_DMA	 UDPHS_DMA[8]; 	// UDPHS DMA channel struct (not use [0])
};

struct AT91_UDPHS_EPTFIFO {
	volatile UINT32	 UDPHS_READEPT0[16384]; 	// FIFO Endpoint Data Register 0
	volatile UINT32	 UDPHS_READEPT1[16384]; 	// FIFO Endpoint Data Register 1
	volatile UINT32	 UDPHS_READEPT2[16384]; 	// FIFO Endpoint Data Register 2
	volatile UINT32	 UDPHS_READEPT3[16384]; 	// FIFO Endpoint Data Register 3
	volatile UINT32	 UDPHS_READEPT4[16384]; 	// FIFO Endpoint Data Register 4
	volatile UINT32	 UDPHS_READEPT5[16384]; 	// FIFO Endpoint Data Register 5
	volatile UINT32	 UDPHS_READEPT6[16384]; 	// FIFO Endpoint Data Register 6
	volatile UINT32	 UDPHS_READEPT7[16384]; 	// FIFO Endpoint Data Register 7
	volatile UINT32	 UDPHS_READEPT8[16384]; 	// FIFO Endpoint Data Register 8
	volatile UINT32	 UDPHS_READEPT9[16384]; 	// FIFO Endpoint Data Register 9
	volatile UINT32	 UDPHS_READEPTA[16384]; 	// FIFO Endpoint Data Register 10
	volatile UINT32	 UDPHS_READEPTB[16384]; 	// FIFO Endpoint Data Register 11
	volatile UINT32	 UDPHS_READEPTC[16384]; 	// FIFO Endpoint Data Register 12
	volatile UINT32	 UDPHS_READEPTD[16384]; 	// FIFO Endpoint Data Register 13
	volatile UINT32	 UDPHS_READEPTE[16384]; 	// FIFO Endpoint Data Register 14
	volatile UINT32	 UDPHS_READEPTF[16384]; 	// FIFO Endpoint Data Register 15
};

// -------- UDPHS_IEN : (UDPHS Offset: 0x10) UDPHS Interrupt Enable Register -------- 
#define AT91C_UDPHS_DET_SUSPD (0x1 <<  1) // (UDPHS) Suspend Interrupt Enable/Clear/Status
#define AT91C_UDPHS_MICRO_SOF (0x1 <<  2) // (UDPHS) Micro-SOF Interrupt Enable/Clear/Status
#define AT91C_UDPHS_IEN_SOF   (0x1 <<  3) // (UDPHS) SOF Interrupt Enable/Clear/Status
#define AT91C_UDPHS_ENDRESET  (0x1 <<  4) // (UDPHS) End Of Reset Interrupt Enable/Clear/Status
#define AT91C_UDPHS_WAKE_UP   (0x1 <<  5) // (UDPHS) Wake Up CPU Interrupt Enable/Clear/Status
#define AT91C_UDPHS_ENDOFRSM  (0x1 <<  6) // (UDPHS) End Of Resume Interrupt Enable/Clear/Status
#define AT91C_UDPHS_UPSTR_RES (0x1 <<  7) // (UDPHS) Upstream Resume Interrupt Enable/Clear/Status
#define AT91C_UDPHS_EPT_INT_0 (0x1 <<  8) // (UDPHS) Endpoint 0 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_1 (0x1 <<  9) // (UDPHS) Endpoint 1 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_2 (0x1 << 10) // (UDPHS) Endpoint 2 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_3 (0x1 << 11) // (UDPHS) Endpoint 3 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_4 (0x1 << 12) // (UDPHS) Endpoint 4 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_5 (0x1 << 13) // (UDPHS) Endpoint 5 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_6 (0x1 << 14) // (UDPHS) Endpoint 6 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_7 (0x1 << 15) // (UDPHS) Endpoint 7 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_8 (0x1 << 16) // (UDPHS) Endpoint 8 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_9 (0x1 << 17) // (UDPHS) Endpoint 9 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_10 (0x1 << 18) // (UDPHS) Endpoint 10 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_11 (0x1 << 19) // (UDPHS) Endpoint 11 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_12 (0x1 << 20) // (UDPHS) Endpoint 12 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_13 (0x1 << 21) // (UDPHS) Endpoint 13 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_14 (0x1 << 22) // (UDPHS) Endpoint 14 Interrupt Enable/Status
#define AT91C_UDPHS_EPT_INT_15 (0x1 << 23) // (UDPHS) Endpoint 15 Interrupt Enable/Status
#define AT91C_UDPHS_DMA_INT_1 (0x1 << 25) // (UDPHS) DMA Channel 1 Interrupt Enable/Status
#define AT91C_UDPHS_DMA_INT_2 (0x1 << 26) // (UDPHS) DMA Channel 2 Interrupt Enable/Status
#define AT91C_UDPHS_DMA_INT_3 (0x1 << 27) // (UDPHS) DMA Channel 3 Interrupt Enable/Status
#define AT91C_UDPHS_DMA_INT_4 (0x1 << 28) // (UDPHS) DMA Channel 4 Interrupt Enable/Status
#define AT91C_UDPHS_DMA_INT_5 (0x1 << 29) // (UDPHS) DMA Channel 5 Interrupt Enable/Status
#define AT91C_UDPHS_DMA_INT_6 (0x1 << 30) // (UDPHS) DMA Channel 6 Interrupt Enable/Status
#define AT91C_UDPHS_DMA_INT_7 (0x1 << 31) // (UDPHS) DMA Channel 7 Interrupt Enable/Status

// -------- UDPHS_EPTCFG : (UDPHS_EPT Offset: 0x0) UDPHS Endpoint Config Register -------- 
#define AT91C_UDPHS_EPT_SIZE  (0x7 <<  0) // (UDPHS_EPT) Endpoint Size
#define 	AT91C_UDPHS_EPT_SIZE_8                    (0x0) // (UDPHS_EPT)    8 bytes
#define 	AT91C_UDPHS_EPT_SIZE_16                   (0x1) // (UDPHS_EPT)   16 bytes
#define 	AT91C_UDPHS_EPT_SIZE_32                   (0x2) // (UDPHS_EPT)   32 bytes
#define 	AT91C_UDPHS_EPT_SIZE_64                   (0x3) // (UDPHS_EPT)   64 bytes
#define 	AT91C_UDPHS_EPT_SIZE_128                  (0x4) // (UDPHS_EPT)  128 bytes
#define 	AT91C_UDPHS_EPT_SIZE_256                  (0x5) // (UDPHS_EPT)  256 bytes
#define 	AT91C_UDPHS_EPT_SIZE_512                  (0x6) // (UDPHS_EPT)  512 bytes
#define 	AT91C_UDPHS_EPT_SIZE_1024                 (0x7) // (UDPHS_EPT) 1024 bytes
#define AT91C_UDPHS_EPT_DIR   (0x1 <<  3) // (UDPHS_EPT) Endpoint Direction 0:OUT, 1:IN
#define 	AT91C_UDPHS_EPT_DIR_OUT                  (0x0 <<  3) // (UDPHS_EPT) Direction OUT
#define 	AT91C_UDPHS_EPT_DIR_IN                   (0x1 <<  3) // (UDPHS_EPT) Direction IN
#define AT91C_UDPHS_EPT_TYPE  (0x3 <<  4) // (UDPHS_EPT) Endpoint Type
#define 	AT91C_UDPHS_EPT_TYPE_CTL_EPT              (0x0 <<  4) // (UDPHS_EPT) Control endpoint
#define 	AT91C_UDPHS_EPT_TYPE_ISO_EPT              (0x1 <<  4) // (UDPHS_EPT) Isochronous endpoint
#define 	AT91C_UDPHS_EPT_TYPE_BUL_EPT              (0x2 <<  4) // (UDPHS_EPT) Bulk endpoint
#define 	AT91C_UDPHS_EPT_TYPE_INT_EPT              (0x3 <<  4) // (UDPHS_EPT) Interrupt endpoint
#define AT91C_UDPHS_BK_NUMBER (0x3 <<  6) // (UDPHS_EPT) Number of Banks
#define 	AT91C_UDPHS_BK_NUMBER_0                    (0x0 <<  6) // (UDPHS_EPT) Zero Bank, the EndPoint is not mapped in memory
#define 	AT91C_UDPHS_BK_NUMBER_1                    (0x1 <<  6) // (UDPHS_EPT) One Bank (Bank0)
#define 	AT91C_UDPHS_BK_NUMBER_2                    (0x2 <<  6) // (UDPHS_EPT) Double bank (Ping-Pong : Bank0 / Bank1)
#define 	AT91C_UDPHS_BK_NUMBER_3                    (0x3 <<  6) // (UDPHS_EPT) Triple Bank (Bank0 / Bank1 / Bank2)
#define AT91C_UDPHS_NB_TRANS  (0x3 <<  8) // (UDPHS_EPT) Number Of Transaction per Micro-Frame (High-Bandwidth iso only)
#define AT91C_UDPHS_EPT_MAPD  ((UINT32)0x1 << 31) // (UDPHS_EPT) Endpoint Mapped (read only

// -------- UDPHS_EPTCTLENB : (UDPHS_EPT Offset: 0x4) UDPHS Endpoint Control Enable Register -------- 
#define AT91C_UDPHS_EPT_ENABL (0x1 <<  0) // (UDPHS_EPT) Endpoint Enable
#define AT91C_UDPHS_AUTO_VALID (0x1 <<  1) // (UDPHS_EPT) Packet Auto-Valid Enable/Disable
#define AT91C_UDPHS_INTDIS_DMA (0x1 <<  3) // (UDPHS_EPT) Endpoint Interrupts DMA Request Enable/Disable
#define AT91C_UDPHS_NYET_DIS  (0x1 <<  4) // (UDPHS_EPT) NYET Enable/Disable
#define AT91C_UDPHS_DATAX_RX  (0x1 <<  6) // (UDPHS_EPT) DATAx Interrupt Enable/Disable
#define AT91C_UDPHS_MDATA_RX  (0x1 <<  7) // (UDPHS_EPT) MDATA Interrupt Enabled/Disable
#define AT91C_UDPHS_ERR_OVFLW (0x1 <<  8) // (UDPHS_EPT) OverFlow Error Interrupt Enable/Disable/Status
#define AT91C_UDPHS_RX_BK_RDY (0x1 <<  9) // (UDPHS_EPT) Received OUT Data
#define AT91C_UDPHS_TX_COMPLT (0x1 << 10) // (UDPHS_EPT) Transmitted IN Data Complete Interrupt Enable/Disable or Transmitted IN Data Complete (clear)
#define AT91C_UDPHS_ERR_TRANS (0x1 << 11) // (UDPHS_EPT) Transaction Error Interrupt Enable/Disable
#define AT91C_UDPHS_TX_PK_RDY (0x1 << 11) // (UDPHS_EPT) TX Packet Ready Interrupt Enable/Disable
#define AT91C_UDPHS_RX_SETUP  (0x1 << 12) // (UDPHS_EPT) Received SETUP Interrupt Enable/Disable
#define AT91C_UDPHS_ERR_FL_ISO (0x1 << 12) // (UDPHS_EPT) Error Flow Clear/Interrupt Enable/Disable
#define AT91C_UDPHS_STALL_SNT (0x1 << 13) // (UDPHS_EPT) Stall Sent Clear
#define AT91C_UDPHS_ERR_CRISO (0x1 << 13) // (UDPHS_EPT) CRC error / Error NB Trans / Interrupt Enable/Disable
#define AT91C_UDPHS_NAK_IN    (0x1 << 14) // (UDPHS_EPT) NAKIN ERROR FLUSH / Clear / Interrupt Enable/Disable
#define AT91C_UDPHS_NAK_OUT   (0x1 << 15) // (UDPHS_EPT) NAKOUT / Clear / Interrupt Enable/Disable
#define AT91C_UDPHS_BUSY_BANK (0x1 << 18) // (UDPHS_EPT) Busy Bank Interrupt Enable/Disable
#define AT91C_UDPHS_SHRT_PCKT (0x1 << 31) // (UDPHS_EPT) Short Packet / Interrupt Enable/Disable
// -------- UDPHS_CTRL : (UDPHS Offset: 0x0) UDPHS Control Register -------- 
#define AT91C_UDPHS_DEV_ADDR  (0x7F <<  0) // (UDPHS) UDPHS Address
#define AT91C_UDPHS_FADDR_EN  (0x1 <<  7) // (UDPHS) Function Address Enable
#define AT91C_UDPHS_EN_UDPHS  (0x1 <<  8) // (UDPHS) UDPHS Enable
#define AT91C_UDPHS_DETACH    (0x1 <<  9) // (UDPHS) Detach Command
#define AT91C_UDPHS_REWAKEUP  (0x1 << 10) // (UDPHS) Send Remote Wake Up
#define AT91C_UDPHS_PULLD_DIS (0x1 << 11) // (UDPHS) PullDown Disable

// -------- UDPHS_IPFEATURES : (UDPHS Offset: 0xf8) UDPHS Features Register -------- 
#define AT91C_UDPHS_EPT_NBR_MAX (0xF <<  0) // (UDPHS) Max Number of Endpoints
#define AT91C_UDPHS_DMA_CHANNEL_NBR (0x7 <<  4) // (UDPHS) Number of DMA Channels
#define AT91C_UDPHS_DMA_B_SIZ (0x1 <<  7) // (UDPHS) DMA Buffer Size
#define AT91C_UDPHS_DMA_FIFO_WORD_DEPTH (0xF <<  8) // (UDPHS) DMA FIFO Depth in words
#define AT91C_UDPHS_FIFO_MAX_SIZE (0x7 << 12) // (UDPHS) DPRAM size
#define AT91C_UDPHS_BW_DPRAM  (0x1 << 15) // (UDPHS) DPRAM byte write capability
#define AT91C_UDPHS_DATAB16_8 (0x1 << 16) // (UDPHS) UTMI DataBus16_8
#define AT91C_UDPHS_ISO_EPT_1 (0x1 << 17) // (UDPHS) Endpoint 1 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_2 (0x1 << 18) // (UDPHS) Endpoint 2 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_3 (0x1 << 19) // (UDPHS) Endpoint 3 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_4 (0x1 << 20) // (UDPHS) Endpoint 4 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_5 (0x1 << 21) // (UDPHS) Endpoint 5 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_6 (0x1 << 22) // (UDPHS) Endpoint 6 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_7 (0x1 << 23) // (UDPHS) Endpoint 7 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_8 (0x1 << 24) // (UDPHS) Endpoint 8 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_9 (0x1 << 25) // (UDPHS) Endpoint 9 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_10 (0x1 << 26) // (UDPHS) Endpoint 10 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_11 (0x1 << 27) // (UDPHS) Endpoint 11 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_12 (0x1 << 28) // (UDPHS) Endpoint 12 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_13 (0x1 << 29) // (UDPHS) Endpoint 13 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_14 (0x1 << 30) // (UDPHS) Endpoint 14 High Bandwidth Isochronous Capability
#define AT91C_UDPHS_ISO_EPT_15 (0x1 << 31) // (UDPHS) Endpoint 15 High Bandwidth Isochronous Capability
//////////////////////////////////////////////////////////////////////////////

#define AT91C_UDPHS_BYTE_COUNT (0x7FF << 20) // (UDPHS_EPT) UDPHS Byte Count
// USB driver
//

#include <..\pal\com\usb\USB.h>

struct AT91_USBHS_Driver
{
    static const UINT32 c_Used_Endpoints           = 6;
    static const UINT32 c_default_ctrl_packet_size = 8;

	USB_CONTROLLER_STATE*		pUsbControllerState;
    
#if defined(USB_REMOTE_WAKEUP)
	#define USB_MAX_REMOTE_WKUP_RETRY 5

	HAL_COMPLETION		RemoteWKUP10msCompletion;
	HAL_COMPLETION		RemoteWKUP100msEOPCompletion;
	UINT32			RemoteWkUpRetry;
#endif

	UINT8			ControlPacketBuffer[c_default_ctrl_packet_size];
	UINT16			EndpointStatus[c_Used_Endpoints];
	BOOL			TxRunning [USB_MAX_QUEUES];
	BOOL			TxNeedZLPS[USB_MAX_QUEUES];

	UINT8			PreviousDeviceState;
	UINT8			RxExpectedToggle[USB_MAX_QUEUES];
	BOOL			PinsProtected;
	BOOL			FirstDescriptorPacket;

#if defined(USB_METRIC_COUNTING)
	USB_PERFORMANCE_METRICS PerfMetrics;
#endif

	static UINT32	MAX_EP;

	//--//

	struct UDP_EPATTRIBUTE
	{
		UINT16		Dir_Type;
		UINT16		Payload;
		BOOL		DualBank;
		UINT32		dFlag;
	};
	
	static UDP_EPATTRIBUTE s_EpAttr[];

	//--//

    static USB_CONTROLLER_STATE * GetState( int Controller );

	static HRESULT Initialize( int Controller );

	static HRESULT Uninitialize( int Controller );

	static BOOL StartOutput( USB_CONTROLLER_STATE* State, int endpoint );

	static BOOL RxEnable( USB_CONTROLLER_STATE* State, int endpoint );

	static BOOL GetInterruptState();

	static BOOL ProtectPins( int Controller, BOOL On );

private:
	static void ClearTxQueue( USB_CONTROLLER_STATE* State, int endpoint );

	static void StartHardware();

	static void StopHardware ();

	static void TxPacket( USB_CONTROLLER_STATE* State, int endpoint );

	static void ControlNext();

	static void SuspendEvent();
	static void ResumeEvent ();
	static void ResetEvent  ();

	static void Global_ISR	( void* Param );

	static void Endpoint_ISR	(UINT32 endpoint);

	static UINT32 PORT_TO_EP(UINT32 PortNo){ return (PortNo*2 +1);};
	static UINT32 EP_TO_PORT(UINT32 EP){ return (EP-1)>>2;};    

#if defined(USB_REMOTE_WAKEUP)
	static void RemoteWkUp_ISR    ( void* Param );
	static void RemoteWkUp_Process( void* Param );
#endif

	static void VBus_ISR	(GPIO_PIN Pin, BOOL PinState, void* Param);

};

extern AT91_USBHS_Driver g_AT91_USBHS_Driver;

struct AT91_USBHS_CONFIG
{
    HAL_DRIVER_CONFIG_HEADER Header;

    //--//

    static LPCSTR GetDriverName() { return "AT91_USB"; }
};

extern AT91_USBHS_CONFIG* const g_pAT91_USBHS_Config;

//
// USB Driver
///////////////////////////////////////////////////////////////////////////////

#endif //_AT91__USBHS_H_1

