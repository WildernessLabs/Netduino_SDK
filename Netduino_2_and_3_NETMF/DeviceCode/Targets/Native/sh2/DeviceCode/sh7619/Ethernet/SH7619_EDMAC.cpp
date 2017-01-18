////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "rtip.h"
#include "sock.h"
#include "rtipapi.h"
#include "rtpirq.h"
#include "rtp.h"
#include "rtpprint.h"
#include "rtpnet.h"

#include <tinyhal.h>
#include "SH7619_EDMAC.h"

//--//
/* ********************************************************************
   DEBUG AIDS
   ******************************************************************** */
#define DEBUG_SH7619EMAC 0

#ifndef NETWORK_INTERFACE_INDEX_SH7619EMAC
#define NETWORK_INTERFACE_INDEX_SH7619EMAC 0
#endif


#if defined(SH7619_EVB_8M_FLASH)
extern struct BlockStorageDevice  g_S29GL064A_16_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_S29GL064A_16_BS_Config;

BlockStorageDevice  * pBlockStorageDevice = &g_S29GL064A_16_BS;
BLOCK_CONFIG        * pBLOCK_CONFIG         = &g_S29GL064A_16_BS_Config;

#define MAC_address_area 0xA07F0000

#else
extern struct BlockStorageDevice  g_S29GL032A_16_BS;
extern struct IBlockStorageDevice g_AM29DL_16_BS_DeviceTable;
extern struct BLOCK_CONFIG        g_S29GL032A_16_BS_Config;

BlockStorageDevice  * pBlockStorageDevice = &g_S29GL032A_16_BS;
BLOCK_CONFIG        * pBLOCK_CONFIG         = &g_S29GL032A_16_BS_Config;

#define MAC_address_area 0xA03FE000

#endif


extern "C"
{
	extern void rtp_thrd_interrupt_continuation(int);
	extern void rtp_thrd_ip_continuation(int);
}   /* extern "C" */

RTP_BOOL    SH7619_EDMAC_open       (PIFACE pi);
void        SH7619_EDMAC_close      (PIFACE pi);
int         SH7619_EDMAC_xmit       (PIFACE pi, DCU msg);
RTP_BOOL    SH7619_EDMAC_xmit_done  (PIFACE pi, DCU msg, RTP_BOOL success);
void        SH7619_EDMAC_recv       (PIFACE pi);
RTP_BOOL    SH7619_EDMAC_statistics (PIFACE pi);
void        SH7619_EDMAC_interrupt  (void *param);

/* ********************************************************************
   GLOBAL DATA
   ******************************************************************** */
EDEVTABLE RTP_FAR sh7619_mac_device = 
{
	SH7619_EDMAC_open, 
	SH7619_EDMAC_close, 
	SH7619_EDMAC_xmit, 
	NULLP_FUNC,
	NULLP_FUNC, 
	SH7619_EDMAC_statistics, 
	NULLP_FUNC, 
	SH7619_EDMAC_DEVICE, 
	"SH7619_EDMAC", 
	MINOR_0, 
	ETHER_IFACE, 
	SNMP_DEVICE_INFO(CFG_OID_SH7619_EDMAC, CFG_SPEED_SH7619_EDMAC)
	CFG_ETHER_MAX_MTU, 
	CFG_ETHER_MAX_MSS, 
	CFG_ETHER_MAX_WIN_IN, 
	CFG_ETHER_MAX_WIN_OUT, 
	IOADD(0), 
	EN(0), 
	EN(0)
};

extern  NETWORK_CONFIG              g_NetworkConfig;

struct SH7619_EDMAC_SOFTC RTP_FAR sh7619_softc;

#define iface_to_softc(X) (PSH7619_EDMAC_SOFTC) &sh7619_softc;
#define off_to_softc(X)   (PSH7619_EDMAC_SOFTC) &sh7619_softc;

// Receive Transfer Descriptor buffer
static RxTd rxTd; 
// Transmit Transfer Descriptor buffer
static TxTd txTd; 

RTP_BOOL SH7619_EDMAC_SetupDevice(void);

UINT32 SH7619_EDMAC_AtoH(char c)
{
	UINT32 rc = 0xFF;
	
	switch (c) {
	    case '0':
	    case '1':
	    case '2':
	    case '3':
	    case '4':
	    case '5':
	    case '6':
	    case '7':
	    case '8':
	    case '9':
	        rc=(int)c-0x30;
        	break;

	    case 'a':
	    case 'A':
	        rc=0xa;
        break;

	    case 'b':
	    case 'B':
	        rc=0xb;
        break;

	    case 'c':
	    case 'C':
	        rc=0xc;
        break;

	    case 'd':
	    case 'D':
	        rc=0xd;
        break;
		
	    case 'e':
	    case 'E':
	        rc=0xe;
        break;

	    case 'f':
	    case 'F':
	        rc=0xf;
        break;

	    default:
	        rc=0xf;
        break;
	}

	return rc;
}

RTP_BOOL SH7619_EDMAC_open(PIFACE pi)   
{
	PSH7619_EDMAC_SOFTC sc = iface_to_softc(pi);
    char c[1],Buffer[17];
	int i;
    UINT32 Events;
	BYTE Buff[6];

	if (!sc)
	{
		RTP_DEBUG_ERROR("SH7619_EDMAC_open: softc invalid!\r\n", NOVAR, 0, 0);
		set_errno(ENUMDEVICE);
		return(RTP_FALSE);
	}

	// Set Interface
	sc->iface = pi;
	iface = pi;

	int macLen = __min(g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressLen, sizeof(sc->mac_address));

	if(macLen > 0)
	{

	    int addr = MAC_address_area;
		for(i=0; i<macLen; i++) g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[i] = *(volatile char *)(addr+i);
	
        debug_printf( "MAC Address: %x.%x.%x.%x.%x.%x\r\n", (UINT8)g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[0], 
                                                             (UINT8)g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[1],
                                                             (UINT8)g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[2],
                                                             (UINT8)g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[3],
                                                             (UINT8)g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[4],
                                                             (UINT8)g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[5]);
	    debug_printf( "Do you need to change MAC Address? If yes, press 'Enter' key\r\n" );
		c[0] = 0x0;
    	for (i=0; i<0xff; i++){
			DebuggerPort_Read( HalSystemConfig.DebugTextPort, c, 1);
			if (c[0] == 0x0d){
			    hal_printf( "new MAC Address :" );
				while(c[0] == 0x0d) DebuggerPort_Read( HalSystemConfig.DebugTextPort, c, 1);
				for(i=0; i<17; ){
		            Events = Events_WaitForEvents( SYSTEM_EVENT_FLAG_COM_IN, 100 );
		            if(Events & SYSTEM_EVENT_FLAG_COM_IN){
						Events_Clear( SYSTEM_EVENT_FLAG_COM_IN );
						DebuggerPort_Read( HalSystemConfig.DebugTextPort, c, 1);
						Buffer[i] = c[0];
						i++;
					}
				}
				for (i=0; i<17; i++) hal_printf( "%c",Buffer[i] );
				hal_printf( "\r\n");

				for(i=0; i<macLen; i++) {
					Buff[i]=SH7619_EDMAC_AtoH(Buffer[i*3])*0x10;
					Buff[i]+=SH7619_EDMAC_AtoH(Buffer[i*3+1]);
				}

				for(i=0; i<macLen; i++) {
					g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[i]=Buff[i];
				}


				debug_printf( "Updating...\r\n" );
				g_AM29DL_16_BS_DeviceTable.InitializeDevice(pBLOCK_CONFIG);
				g_AM29DL_16_BS_DeviceTable.EraseBlock(pBLOCK_CONFIG,MAC_address_area);
				g_AM29DL_16_BS_DeviceTable.Write(pBLOCK_CONFIG,MAC_address_area,macLen,Buff,0x0);
				debug_printf( "Done\r\n" );

				i=0x100;
			}
		}
		memcpy(&sc->mac_address[0], &g_NetworkConfig.NetworkInterfaces[NETWORK_INTERFACE_INDEX_SH7619EMAC].macAddressBuffer[0], macLen);
	}
	else
	{
		RTP_DEBUG_ERROR("Device initialize without MAC address!!!\r\n", NOVAR, 0, 0);
}
    
	/* Now put in a dummy ethernet address */
	rtp_memcpy(&pi->addr.my_hw_addr[0], sc->mac_address, 6); // Get the ethernet address
    
	/* clear statistic information */
	sc->stats.packets_in     = 0L;
	sc->stats.packets_out    = 0L;
	sc->stats.bytes_in       = 0L;
	sc->stats.bytes_out      = 0L;
	sc->stats.errors_in      = 0L;
	sc->stats.errors_out     = 0L;    

	if(RTP_FALSE == SH7619_EDMAC_SetupDevice())
	{
		return RTP_FALSE;
	}
    
	rtp_irq_hook_interrupt( (RTP_PFVOID) pi, 
							(RTP_IRQ_FN_POINTER)SH7619_EDMAC_recv, 
							(RTP_IRQ_FN_POINTER) 0);

	return(RTP_TRUE);
}

void SH7619_EDMAC_close(PIFACE pi)                          
{
	ARGSUSED_PVOID( pi );  /*  keep compiler happy */
}

void SH7619_EDMAC_interrupt(void *param)
{
	long eesr0_val;

	GLOBAL_LOCK(encIrq);

	Events_Set( SYSTEM_EVENT_FLAG_SOCKET );    	
	eesr0_val = EESR0;	

	if((eesr0_val & 0x00010000) == 0x00010000)
	{		
		Flush_All(iface);
		return;
	}

	// a Frame is transmitted
	if((eesr0_val & 0x00200000) == 0x00200000)
	{
		//signal IP task that xmit has completed
		rtp_net_invoke_output(iface, 1); 
	}

	// Frame(s) received
	if((eesr0_val & 0x00040000) == 0x00040000)
	{
		rtp_thrd_interrupt_continuation(iface->ctrl.index);
	}

	ECSR0   = 0x00000007;
	EESR0   = 0x47FF0F9F;
}

void SH7619_EDMAC_recv(PIFACE pi)
{
	DCU               	msg;
	RTP_UINT16         	FrameLength = 0;
	long 				leng;
	UINT16             	BufferLength;
	UINT32  			tmpIdx = rxTd.idx;
	char*				recv_packet;
	int					tries = 100;
	BOOL				recv_error = FALSE;
	BOOL				mem_error = FALSE;
	EmacTDescriptor 	*pRxTd;
	BOOL isFrame = FALSE;
	
	GLOBAL_LOCK(encIrq);

	pRxTd = *(rxTd.td + rxTd.idx);

	while ((pRxTd->status & ACT) != 0 && tries-- > 0)
	{
		for(int i = 0; i < 500; i++);
	}

	if(tries <= 0)
	{
		//while ((pRxTd->status & ACT) != 0 && tries++ < RX_BUFFERS)
		//{
		//	CIRC_INC(rxTd.idx, RX_BUFFERS);
		//	pRxTd = *(rxTd.td + rxTd.idx);
		//}
		//tmpIdx = rxTd.idx;
		return;
	}

	while ((pRxTd->status & ACT) == 0)
	{
		leng = pRxTd->RDL;
		// A start of frame has been received
		if((pRxTd->status & FP1) != 0) 
		{
			// Skip previous fragment
			while (tmpIdx != rxTd.idx)
			{
				pRxTd = *(rxTd.td + rxTd.idx);
				pRxTd->status |= ACT;
				CIRC_INC(rxTd.idx, RX_BUFFERS);
			}

			FrameLength = 0;

			// Start to gather buffers in a frame
			isFrame = TRUE;
		}

		// Increment the pointer
		CIRC_INC(tmpIdx, RX_BUFFERS);

		if(isFrame)
		{
			if (tmpIdx == rxTd.idx) 
			{
				hal_printf("Receive buffer is too small for the current frame!\r\n");
				do
				{
					//FrameLength += pRxTd->RDL;
					pRxTd = *(rxTd.td + rxTd.idx);
					pRxTd->status |= ACT;
					CIRC_INC(rxTd.idx, RX_BUFFERS);
				} while(tmpIdx != rxTd.idx);
			}

			if((pRxTd->status & FE) != 0) 
			{
				recv_error = TRUE;
			}

			FrameLength += leng;

			// An end of frame has been received
			if((pRxTd->status & FP0) != 0)
			{
				tries = 0;

				if(recv_error == FALSE)
				{				
					do
					{
						msg = os_alloc_packet_input(FrameLength, DRIVER_ALLOC);
						if(!msg)
							for(int i = 0; i < 5000; i++);		//delay
					}while(!msg && tries++ < 50);

					if (msg)
					{
						recv_packet = (char*)DCUTODATA(msg);
						msg->length = FrameLength;
					}
					else
					{
						mem_error = TRUE;
					}
				}
				else
				{
					mem_error = TRUE;
				}

				BufferLength = EMAC_RX_UNITSIZE;
				// Get all the data
				while (rxTd.idx != tmpIdx)
				{                   
					pRxTd = *(rxTd.td + rxTd.idx);

					if(mem_error == FALSE)
					{
						if(BufferLength >= FrameLength)
						{
							memcpy(recv_packet, (void*)(pRxTd->TRBA), FrameLength);
						}
						else
						{
							memcpy(recv_packet, (void*)(pRxTd->TRBA), BufferLength);
							FrameLength -= BufferLength;
							recv_packet += BufferLength;
						}
					}

					pRxTd->status |= ACT;
					CIRC_INC(rxTd.idx, RX_BUFFERS);
				}
                
				// signal IP layer that a packet is on its exchange
				if(mem_error == FALSE)
					rtp_net_invoke_input(pi, msg, msg->length);

				// Prepare for the next Frame
				isFrame = FALSE;
				recv_error = FALSE;
				mem_error = FALSE;
			}
		}// if(isFrame) ends
		else
		{
			pRxTd->status |= ACT;
			rxTd.idx = tmpIdx;		   
		}

		// Process the next buffer
		pRxTd = *(rxTd.td + tmpIdx);
	}
}

UINT16 Send_Packet(EmacTDescriptor *epTxTd, char* buf, UINT16 length)
{  
	//long i=0;
	UINT16 FrameLength = ((length <= EMAC_TX_UNITSIZE) ? length : EMAC_TX_UNITSIZE);

	if(epTxTd->status & ACT != 0)
		return -1;
	memcpy(epTxTd->TRBA, buf, FrameLength);
	epTxTd->RDL = FrameLength;

	return( FrameLength );
}

int SH7619_EDMAC_xmit(PIFACE pi, DCU msg)
{
	RTP_UINT16              length = 0;
	EmacTDescriptor *pTxTd;
	char* send_packet;
	UINT16  count;
	long    flag = FP1;
	UINT16  size = length;

	GLOBAL_LOCK(encIrq);

	if (!pi)
	{
		return (-1);
	}   

	length = msg->length;

	if (length > (ETHERSIZE+4))
	{
		RTP_DEBUG_ERROR("xmit - length is too large, truncated", NOVAR, 0, 0);
		length = ETHERSIZE+4;         /* what a terriable hack! */
	}

	send_packet = (char*)DCUTODATA(msg);

	for( count = 0 ; length > 0 ; length -= count )
	{  
    	// Pointers to the current TxTd
    	pTxTd = *(txTd.td + txTd.head);
		while( (count = Send_Packet(pTxTd, send_packet, length)) < 0 );
		if( count == length )
		{
			flag |= FP0;
		}
		pTxTd->status = (pTxTd->status & DL) | flag | ACT;
		flag = 0;

		// Driver manage the ring buffer
		CIRC_INC(txTd.head, TX_BUFFERS)

		send_packet += count;
	}

	// Now start to transmit if it is not already done
	if( EDTRR0 == 0x00000000 )
	{
		EDTRR0 = 0x00000001;
	}

	return (0);
}

RTP_BOOL SH7619_EDMAC_xmit_done  (PIFACE pi, DCU msg, RTP_BOOL success)
{
	PSH7619_EDMAC_SOFTC   sc;

	ARGSUSED_PVOID(msg);

	if(!pi)
	{
		return (RTP_FALSE);
	}

	sc = iface_to_softc(pi);
	if(!sc)
		return(RTP_TRUE);

	if(!success)
	{
		sc->stats.errors_out++;
		sc->stats.tx_other_errors++;
	}
	else
	{
		/* Update total number of successfully transmitted packets. */
		sc->stats.packets_out++;
		sc->stats.bytes_out += msg->length;
	}
	return(RTP_TRUE);
}

RTP_BOOL SH7619_EDMAC_statistics(PIFACE pi)                       
{
#if (!INCLUDE_KEEP_STATS)
	ARGSUSED_PVOID(pi)
#endif

	if (!pi)
	{
		return RTP_FALSE;
	}

	UPDATE_SET_INFO(pi, interface_packets_in, sh7619emac_packets_in)
	UPDATE_SET_INFO(pi, interface_packets_out, sh7619emac_packets_out)
	UPDATE_SET_INFO(pi, interface_bytes_in, sh7619emac_bytes_in)
	UPDATE_SET_INFO(pi, interface_bytes_out, sh7619emac_bytes_out)
	UPDATE_SET_INFO(pi, interface_errors_in, sh7619emac_errors_in)
	UPDATE_SET_INFO(pi, interface_errors_out, sh7619emac_errors_out)
	UPDATE_SET_INFO(pi, interface_packets_lost, 0L)
	return(RTP_TRUE);
}

int xn_bind_sh7619_mac(int minor_number)
{
	return(xn_device_table_add(sh7619_mac_device.device_id,
								minor_number, 
								sh7619_mac_device.iface_type,
								sh7619_mac_device.device_name,
								SNMP_DEVICE_INFO(sh7619_mac_device.media_mib, 
													sh7619_mac_device.speed)                         
								(DEV_OPEN)sh7619_mac_device.open,
								(DEV_CLOSE)sh7619_mac_device.close,
								(DEV_XMIT)sh7619_mac_device.xmit,
								(DEV_XMIT_DONE)sh7619_mac_device.xmit_done,
								(DEV_PROCESS_INTERRUPTS)sh7619_mac_device.proc_interrupts,
								(DEV_STATS)sh7619_mac_device.statistics,
								(DEV_SETMCAST)sh7619_mac_device.setmcast));
}

RTP_BOOL SH7619_EDMAC_SetupDevice()
{
	UINT32 errCount = 0;

	SH7619_EDMAC_Init();

	while(!SH7619_EDMAC_GetLinkSpeed(TRUE))
	{
		errCount++;
	}
	hal_printf("Waiting for getting IP address...\r\n");

	return RTP_TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL SH7619_EDMAC_AutoNegotiate();
void SH7619_EDMAC_Init()
{
	UINT32 Index;
	UINT32 Address;    
	long            i;
	unsigned long   mac;
	int s;
	int tries;

	tries = 0;
	EDMR0 = 0x00000001; /* reset EDMAC  */
	while(EDMR0 != 0x00000000) {
		if(tries >= 50)
			break;
		for(i = 0; i < 500000; i++);
		tries++;
	}

	if(tries >= 50)
	{
		hal_printf("Ethernet Init Failed.\r\n");
		return;
	}

	SH7619_EDMAC_AutoNegotiate();

	s = 16;
	Init_Malloc();

	// Clear buffer index
	rxTd.idx = 0;
	CIRC_CLEAR(&txTd);

	// Setup the RX descriptors.
	for(Index = 0; Index < RX_BUFFERS; Index++) 
	{
		rxTd.td[Index] = (EmacTDescriptor*)Malloc_Buf(s);
	}
    
	for(Index = 0; Index < RX_BUFFERS; Index++) 
	{
		rxTd.td[Index]->TRBA = (char*)Malloc_Buf(EMAC_RX_UNITSIZE);
		rxTd.td[Index]->status = ACT;
		rxTd.td[Index]->TDRBL = EMAC_RX_UNITSIZE;
		rxTd.td[Index]->RDL = 0;        
	}
	rxTd.td[RX_BUFFERS - 1]->status |= DL;

	// Setup the TX descriptors.
	for(Index = 0; Index < TX_BUFFERS; Index++)
	{
		txTd.td[Index] = (EmacTDescriptor*)Malloc_Buf(s);// * EMAC_TX_UNITSIZE]);
	}
    
	for(Index = 0; Index < TX_BUFFERS; Index++) 
	{
		txTd.td[Index]->TRBA = (char*)Malloc_Buf(EMAC_TX_UNITSIZE);
		txTd.td[Index]->status = 0;
		txTd.td[Index]->TDRBL = EMAC_TX_UNITSIZE;
		txTd.td[Index]->RDL = 0;        
	}
	txTd.td[TX_BUFFERS - 1]->status |= DL;

	IPRC |= 0x0D00;
	EESIPR0 = 0x00250000;

	ECSR0   = 0x00000007;			/* clear all EtherC status  */
	EESR0   = 0x47FF0F9F;			/* clear all with write 1 */

	RDLAR0  = (long)(rxTd.td[0]);  	/* initialaize Rx Descriptor List Address */
	TDLAR0  = (long)(txTd.td[0]);  	/* initialaize Tx Descriptor List Address */
	TRSCER0 = 0x00000000;           /* copy-back status is RFE & TFE only */
	TFTR0   = 0x00000000;           /* threshold of Tx_FIFO */
	FDR0    = 0x00000101;           /* transmit fifo & receive fifo is 512 bytes */
	RMCR0   = 0x00000001;           /* receive function is normal mode(continued) */
	FCFTR0  = 0x00000001;

	APR0	= 0x00000002;
	IPGR0	= 0x00000002;

	mac = ((unsigned long)gmac_address[0] << 16) | (unsigned long)gmac_address[1];
	MAHR0 = mac;

	mac = (unsigned long)gmac_address[2];
	MALR0 = mac;

	ECSIPR0 = 0x00000017;                   /* disable EtherC status chenge interrupt */    

	RFLR0   = 1518;                         /* ether payload is 1500+ CRC;5EE   */
	ECMR0   = 0x00020000;                   /* not loopback */
	for( i = 0 ; i < 0x00010000 ; i++ );    /* wait */

	//ECMR0   = 0x00030062;					/* disable MagicPacket, enable Tx,Rx full-duplex mode */ 
	ECMR0   = 0x00070062;                   /* disable MagicPacket, enable Tx,Rx full-duplex mode */ 
	EDRRR0  = 0x00000001;                   /* receive EDMAC start */
}

void Init_Malloc()
{
	#define FIXED_HEAP 0x00600010   

	HEAP_TOP = (unsigned long)(CS3_TOP+FIXED_HEAP);  // CS3-SDRAM area
}

int* Malloc_Buf(int size)
{
	int *ptr;

	if((HEAP_TOP&0x0000000F) != 0x00000000) {
		// Aligns 16bytes boundary
		HEAP_TOP = (HEAP_TOP & 0xFFFFFFF0) + 0x00000010;
	}
	ptr = (int *)HEAP_TOP;
	HEAP_TOP = HEAP_TOP + (unsigned long)size;
	if((HEAP_TOP & 0x00000003) != 0x00000000) {
		// Aligns longword boundary
		HEAP_TOP = (HEAP_TOP & 0xFFFFFFFC) + 0x00000004;
	}

	return (ptr);
}

void Flush_All(PIFACE pi)
{
	UINT32 Index;
	 
	ECMR0   = 0x00000000;
	EDRRR0  = 0x00000000;
	RMCR0   = 0x00000000;
	
	SH7619_EDMAC_recv(pi);
	
	ECSR0   = 0x00000007;
	EESR0   = 0x47FF0F9F;
	TROCR0	= 0x00000000;
	CDCR0	= 0x00000000;
	LCCR0	= 0x00000000;
	CNDCR0	= 0x00000000;
	CEFCR0	= 0x00000000;
	FRECR0	= 0x00000000;
	TSFRCR0	= 0x00000000;
	TLFRCR0	= 0x00000000;
	RFCR0	= 0x00000000;
	MAFCR0	= 0x00000000;
	RMFCR0	= 0x00000000;
	
	// Setup the RX descriptors
	for(Index = 0; Index < RX_BUFFERS; Index++) 
	{
		rxTd.td[Index]->status = ACT;
		rxTd.td[Index]->TDRBL = EMAC_RX_UNITSIZE;
		rxTd.td[Index]->RDL = 0;        
	}
	rxTd.td[RX_BUFFERS - 1]->status |= DL;

	// Setup the TX descriptors
	for(Index = 0; Index < TX_BUFFERS; Index++) 
	{
		txTd.td[Index]->status = 0;
		txTd.td[Index]->TDRBL = EMAC_TX_UNITSIZE;
		txTd.td[Index]->RDL = 0;        
	}
	txTd.td[TX_BUFFERS - 1]->status |= DL;

	RMCR0   = 0x00000001;
	//ECMR0   = 0x00030062;
	ECMR0   = 0x00070062;
	EDRRR0  = 0x00000001;
}

unsigned short Ether_Reg_Read( unsigned short reg_addr )
{
	volatile unsigned short data;
	data = PHYAccess( 0, PHY_ADDR, reg_addr, OPREAD, 0);

	return( data );
}

void Ether_Reg_Write( unsigned short reg_addr, unsigned short data )
{
	PHYAccess( 0, PHY_ADDR, reg_addr, OPWRITE, data);
}

vuint16_t PHYAccess(vuint8_t channel, vuint8_t PHYAdd, vuint8_t RegAdd, vuint8_t OPCode, vuint16_t wData)
{
	// Local variables
	int i;
	vuint16_t MGMTval, mdio;


	// Filter unused bits from input variables.
	PHYAdd &= 0x1F;
	RegAdd &= 0x1F;
	OPCode &= 0x03;
	
	// Output Preamble (32 '1's)
	for (i = 0; i < 32; i++)
		Write_MDO(channel, 1);

	// Output Start of Frame ('01')
	for (i = 0; i < 2; i++)
		Write_MDO(channel, i);

	// Output OPCode ('01' for write or '10' for Read)
	for (i = 1; i >= 0; i--)
		Write_MDO(channel, ((OPCode>>i) & 0x01));

	// Output PHY Address
	for (i = 4; i >= 0; i--)
		Write_MDO(channel, ((PHYAdd>>i) & 0x01));

	// Output Register Address
	for (i = 4; i >= 0; i--)
		Write_MDO(channel, ((RegAdd>>i) & 0x01));

	if (OPCode == OPREAD)
	{
		// Read Operation
		// Implement Turnaround ('Z0')  - not sure if needed...
		Write_MDO(channel, 0);

		// Read Data
		wData = 0;
		for (i = 15; i >= 0; i--)
			wData |= (Read_MDI(channel, 0) << i);

		// produce 1 clk idle state
		Read_MDI(channel, 0);

		return (wData);
	}
	else
	{
		// Write Operation
		// Implement Turnaround ('10')  - not sure if needed...
		for (i = 1; i >= 0; i--)
			Write_MDO(channel, ((2>>i) & 0x01));

		// Write Data
		for (i = 15; i >= 0; i--)
			Write_MDO(channel, ((wData>>i) & 0x01));

		// produce 1 clk idle state
		Read_MDI(channel, 0);

		return (1);
	}
}

void Write_MDO(vuint8_t channel, unsigned MGMTData)
{
	volatile int i   ;
	vuint32_t * pir_ptr;
	vuint32_t data = (MGMTData << ETHERC_MDO_SLB) & ETHERC_MDO_WRITE; // Shift data and make sure only 1 bit is set

	pir_ptr = (vuint32_t *) (&PIR0);

	// Clear PHY Interface Register
	*pir_ptr = 0;

	// Setup data time
	*pir_ptr = data | ETHERC_MMD;

	// 5 * clkfreq of CPU
	for (i = 0; i < 5; i ++)
	{
	}

	// Send Data
	*pir_ptr = data | ETHERC_MMD | ETHERC_MDC;

	// 5 * clkfreq of CPU
	for (i = 0; i < 5; i ++)
	{
	}

	// Hold Data Time
	*pir_ptr = data|ETHERC_MMD;
}

vuint32_t Read_MDI(vuint8_t channel, unsigned MGMTData)
{
	volatile int i   ;
	vuint32_t * pir_ptr;
	vuint32_t data = 0;

	pir_ptr = (vuint32_t *)(&PIR0);

	// Clear PHY Interface Register
	*pir_ptr = 0;

	*pir_ptr = ETHERC_MDC; 
	 
	// Setup data time
	*pir_ptr = data;

	// 5 * clkfreq of CPU
	for (i = 0; i < 5; i ++)
	{
	}

	// Read Data
	data = *pir_ptr;                                           
	data &= ETHERC_MDI;         // Filter out all but input data
	data >>= ETHERC_MDI_SRB;    // shift data to the right
	*pir_ptr = 0x00000000;      // Clear PIR

	return data;
}

BOOL SH7619_EDMAC_GetLinkSpeed(BOOL applySetting)
{
	BOOL rc = TRUE;

	return rc;
}

int get_initial_phymd(void)
{
	int mode;
	unsigned short sw3;

	PCCRH2 &= 0xFF00;
	PCIORH &= 0xfff0;

	sw3 = (~PCDRH) & 0x0007u;
	switch( sw3 ){
	case 0x0000:
		mode = HALF_10T_SET;
		break;
	case 0x0001:
		mode = HALF_100TX_SET;
		break;
	case 0x0002:
		mode = FULL_10T_SET;
		break;
	case 0x0003:
		mode = FULL_100TX_SET;
		break;
	case 0x0004:
	case 0x0005:
		mode = HALF_100TX_AUTONEGO_SET;
		break;
	case 0x0006:
	case 0x0007:
		mode = FULL_100TX_AUTONEGO_SET;
		break;
	default:
		mode = POWERDOWN_SET;
		break;
	}
	return mode;
}

int phy_read_linkmode( void )
{
	unsigned short reg0,reg4,reg5;
	unsigned short w16;
	int link = NEGO_FAIL;
	int my_able, pt_able;

	reg0 = Ether_Reg_Read(BASIC_MODE_CONTROL_REG);
	reg4 = Ether_Reg_Read(AN_ADVERTISEMENT_REG);
	reg5 = Ether_Reg_Read(AN_LINK_PARTNER_ABILITY_REG);

	if( !(reg0 & 0x1000) ) {
		w16 = reg0 & 0x2100;
		if( w16 == 0x2100 ) {
			link = FULL_TX;
		}
		else if( w16 == 0x2000 ) {
			link = HALF_TX;
		}
		else if( w16 == 0x0100 ) {
			link = FULL_10M;
		}
		else {
			link = HALF_10M;
		}
	}
	else {
		if( reg4 & 0x0100 ) {
			my_able = FULL_TX;
		}
		else if( reg4 & 0x0080 ) {
			my_able = HALF_TX;
		}
		else if( reg4 & 0x0040 ) {
			my_able = FULL_10M;
		}
		else {
			my_able = HALF_10M;
		}

		if( reg5 & 0x0100 ) {
			pt_able = FULL_TX;
		}
		else if( reg5 & 0x0080 ) {
			pt_able = HALF_TX;
		}
		else if( reg5 & 0x0040 ) {
			pt_able = FULL_10M;
		}
		else if( reg5 & 0x0020 ) {
			pt_able = HALF_10M;
		}
		else {
			pt_able = NEGO_FAIL;
		}

		link = my_able;
		if( my_able > pt_able )
		{
			link = pt_able;
		}
	}
	return link;
}

int PHY_Init(int mode)
{
	long i;
	unsigned short PHYIFCR_MODE;
	unsigned short  data;
	int tries;

	hal_printf("PHY_Init.\r\n"); 

	STBCR4 = 0x00; /* Enable phyif */

	tries = 0;
	while (PHYSR == 0x8000)
	{
		if(tries >= 500)
			break;
		for(i = 0; i < 500000; i++);
		tries++;
	}

	if(tries >= 500)
	{
		hal_printf("PHY Init Failed\r\n");
		return 0;
	}

	PCCRH2 = 0x0000; /* MII inhibited   */
	PCCRL1 = 0x0000;
	PCCRL2 = 0xFF00; /* _DUPLEX,_COL,_LINK,_SPED10 */

	PHYSMIR2 = 0x0000;
	PHYSMIR3 = 0x0000;
	PHYADDR = PHY_ADDR; /* 16 bits */

	PHYIFCR_MODE = 0xA007 | mode; //0xA007; // External Clock        
	//  PHYIFCR_MODE = WR_PHYIFCR+CLKSEL_I+MII_MODE_00+CO_ST_MODE_111;
	PHYCR = PHYIFCR_MODE; // - bww
	//REG_PHYIF_PHYIFCR = 0xA007;

	// Wait 100us before negating the phy module reset.
	// With assumption of wait loop takes 4cyc, 12500/4=3125 loops,
	// 100us = ca. 12500 Icyc@Iclk=125MHz.
	for (i = 1; i <= 3125; i++)
	{
	}

	PHYCR = PHYIFCR_MODE + 0x4000;

	// wait 20ms after module reset.
	// With assumption of wait loop takes 4cyc, 2,500,000/4=625000 loops,
	// 20ms = ca. 2,500,000 Icyc@Iclk=125MHz.
	for (i = 1; i <= 625000; i++)
	{
	}

	Ether_Reg_Write( BASIC_MODE_CONTROL_REG, 0x1000 );

	for (i = 1; i <= 10000; i++)
	{
	}
	data = 0x0000;
	tries = 0;
	while ((data & 0x0020) != 0x0020)
	{
		if(tries >= 200)
			break;
		for(i = 0; i < 500000; i++);        
	    data = Ether_Reg_Read( BASIC_MODE_STATUS_REG );
	    // 0x0020 : Auto-negotiation complete.      
	    tries++;
	}

	if(tries >= 200)
	{
	    hal_printf("PHY Init Failed\r\n");
	    return 0;
	}
	hal_printf("PHY_Init...Done!\r\n"); 
	return 1;
}

void PHY_Initialize()
{	
	volatile int w,cnt = 0;
	int mode = 0;
	int volatile link, pre_link = -1;
	
	mode = 	get_initial_phymd();
	
	PHY_Init(mode);
	
	do {
		for(w = LOOP_100us*10; w > 0; w--) {
			/* 1ms wait */
		}
		
		link = phy_read_linkmode();
		if( link == pre_link ) {
			cnt++;
		}
		else {
			pre_link = link;
			cnt = 0;
		}
	}while( cnt < 500 );
	
	if( link == FULL_TX || link == FULL_10M ) {		
		ECMR0 |= (1 << 1);  // DM = 1
	}
	else {
		ECMR0 &= ~(1 << 1);  // DM = 0
	}
}

UINT32 SH7619_EDMAC_FindValidPhy()
{
	UINT32 rc = 0xFF;
	unsigned short value;

	for (int i = 0; i < 17; i++)
	{
		value = Ether_Reg_Read(phy_addr[i]);
		if (value != 0x0000 && value != 0xFFFF)
		{
			rc = value;
			break;
		}
	}

	if (rc != 0xFF)
	{
		hal_printf( "Valid PHY Found: %d\r\n", rc);
	}

	return rc;
}

BOOL SH7619_EDMAC_AutoNegotiate()
{
	BOOL rc = TRUE;

	PHY_Initialize();
	hal_printf("SH7619_EDMAC_AutoNegotiate \r\n");
	hal_printf("AutoNegotiate complete\r\n");

	return rc;
}

