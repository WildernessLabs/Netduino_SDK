////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <tinyhal.h>

#include "net_decl_lwip.h"
#include "SH7619_EDMAC_lwip.h"

#include "lwip\netif.h"
#include "lwip\pbuf.h"
#include "lwip\mem.h"



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


extern struct netif g_SH7619_EDMAC_NetIF;
extern void         lwip_interrupt_continuation( void );

extern NETWORK_CONFIG  g_NetworkConfig;


// Receive Transfer Descriptor buffer
static RxTd rxTd; 
// Transmit Transfer Descriptor buffer
static TxTd txTd; 


UINT32 SH7619_EDMAC_LWIP_AtoH(char c)
{
    UINT32 rc = 0xFF;

    if ( ( c >= '0' ) &&  (c <='9'))
            rc=(int)c-0x30;
    
    else if (( c >='a') && (c <='f')) 
        
            rc= c - 'a' + 0xa;
    else if (( c >='A') && (c <='F')) 
        
            rc= c - 'A' + 0xa;

    return rc;
}


void SH7619_EDMAC_interrupt( void *param )
{
    struct netif *pNetIf;
    pNetIf = &g_SH7619_EDMAC_NetIF;
    SH7619_EDMAC_LWIP_interrupt(pNetIf);
}


BOOL SH7619_EDMAC_LWIP_SetupDevice(struct netif *pNetIf);

BOOL SH7619_EDMAC_LWIP_open(struct netif *pNetIf)   
{
    return SH7619_EDMAC_LWIP_SetupDevice(pNetIf);
}


void SH7619_EDMAC_LWIP_close(struct netif *pNetIf)                                /*__fn__*/
{
}

void SH7619_EDMAC_LWIP_interrupt(struct netif *pNetIf)
{
    long eesr0_val;

    GLOBAL_LOCK(encIrq);

    Events_Set( SYSTEM_EVENT_FLAG_SOCKET );     
    eesr0_val = EESR0;  

    if((eesr0_val & 0x00010000) == 0x00010000)
    {       
        Flush_All(pNetIf);
        return;
    }

    // a Frame is transmitted
    if((eesr0_val & 0x00200000) == 0x00200000)
    {
        //signal IP task that xmit has completed

    }

    // Frame(s) received
    if((eesr0_val & 0x00040000) == 0x00040000)
    {
        lwip_interrupt_continuation();

    }

    ECSR0   = 0x00000007;
    EESR0   = 0x47FF0F9F;
}


void SH7619_EDMAC_LWIP_recv(struct netif *pNetIf)
{

    struct pbuf *pPBuf;
    UINT8       *dataRX;

    UINT16              FrameLength = 0;
    long                leng;
    UINT16              BufferLength;
    UINT32              tmpIdx = rxTd.idx;
    char*               recv_packet;
    int                 tries = 100;
    BOOL                recv_error = FALSE;
    BOOL                mem_error = FALSE;
    EmacTDescriptor     *pRxTd;
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
        //  CIRC_INC(rxTd.idx, RX_BUFFERS);
        //  pRxTd = *(rxTd.td + rxTd.idx);
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
                        pPBuf = pbuf_alloc(PBUF_RAW, FrameLength, PBUF_RAM);

                        if(!pPBuf)
                            for(int i = 0; i < 5000; i++);      //delay
                    }while(!pPBuf && tries++ < 50);

                    if (pPBuf)
                    {
                        dataRX = (UINT8*)pPBuf->payload;
                        BufferLength = EMAC_RX_UNITSIZE;
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

                while (rxTd.idx != tmpIdx)
                {                   
                    pRxTd = *(rxTd.td + rxTd.idx);

                    if(mem_error == FALSE)
                    {
                        if(BufferLength >= FrameLength)
                        {
                            memcpy(dataRX, (void*)(pRxTd->TRBA ), FrameLength);
                            
                        }
                        else
                        {
                            memcpy(dataRX, (void*)(pRxTd->TRBA), BufferLength);
                            FrameLength -= BufferLength;
                            dataRX += BufferLength;
                        }
                    }

                    pRxTd->status |= ACT;
                    CIRC_INC(rxTd.idx, RX_BUFFERS);
                }
                
                // signal IP layer that a packet is on its exchange
                if(mem_error == FALSE)
                    pNetIf->input(pPBuf, pNetIf);

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

INT16 Send_Packet(EmacTDescriptor *epTxTd, char* buf, INT16 length)
{  
    //UINT16 TotalLength = length;
    INT16 FrameLength = ((length <= EMAC_TX_UNITSIZE) ? length : EMAC_TX_UNITSIZE);
 
    //while(TotalLength != 0)
    //{
        //FrameLength = ((TotalLength <= EMAC_TX_UNITSIZE) ? TotalLength : EMAC_TX_UNITSIZE);
        //TotalLength -= FrameLength;
        if(epTxTd->status & ACT != 0)
            return -1;
        memcpy(epTxTd->TRBA, buf, FrameLength);
        epTxTd->TDRBL = FrameLength;
    //}
    return( FrameLength );
}

void Frame_Xmit(struct pbuf *pPBuf, BOOL startPacket)
{  
    INT16  length = 0;
 
    EmacTDescriptor *pTxTd;
    
    INT16  count;
    long    flag = 0;
    
    length = pPBuf->len;
            
    if(startPacket)
        flag = FP1;
            
    for( count = 0 ; length > 0 ; length -= count )
    {  
        // Pointers to the current TxTd
        pTxTd = *(txTd.td + txTd.head);
        while( (count = Send_Packet(pTxTd, (char *)pPBuf->payload + (pPBuf->len - length), length)) < 0 ) {};
        if((count == length) && (pPBuf->tot_len == pPBuf->len))
        {
            flag |= FP0;
        }
        pTxTd->status = (pTxTd->status & DL) | flag | ACT;
        flag = 0;
 
        // Driver manage the ring buffer
        CIRC_INC(txTd.head, TX_BUFFERS)
    }
}

err_t SH7619_EDMAC_LWIP_xmit(struct netif *pNetIf, struct pbuf *pPBuf)
{
    INT16  length = 0;
    BOOL startPacket = TRUE;
 
    GLOBAL_LOCK(encIrq);
 
    if (!pNetIf || !pPBuf)
    {
        return ERR_ARG;
    }   
 
    length = pPBuf->tot_len;
 
    if (length > SH7619_EDMAC_MAX_FRAME_SIZE) // (ETHERSIZE+4))
    {
        debug_printf("xmit - length is too large, truncated \r\n" );
        length = SH7619_EDMAC_MAX_FRAME_SIZE; // ETHERSIZE+4;
    }
            
    while(length > 0)
    {
        Frame_Xmit(pPBuf, startPacket);
        startPacket = FALSE;
        length -= pPBuf->len;
        pPBuf = pPBuf->next;
    }
 
    /*for( count = 0 ; length > 0 ; length -= count )
    {  
        // Pointers to the current TxTd
        pTxTd = *(txTd.td + txTd.head);
        while( (count = Send_Packet(pTxTd, (char *)pPBuf->payload, pPBuf->len)) < 0 ) {};
 
        
        if( count == length )
        {
            flag |= FP0;
        }
        pTxTd->status = (pTxTd->status & DL) | flag | ACT;
        flag = 0;
 
        // Driver manage the ring buffer
        CIRC_INC(txTd.head, TX_BUFFERS)
 
        pPBuf = pPBuf->next;
 
    }*/
    
    // Now start to transmit if it is not already done
    if( EDTRR0 == 0x00000000 )
    {
        EDTRR0 = 0x00000001;
    }
 
    return ERR_OK;
 
}

BOOL SH7619_EDMAC_LWIP_SetupDevice(struct netif *pNetIf)
{
    UINT32 errCount = 0;

    SH7619_EDMAC_LWIP_Init();

    while(!SH7619_EDMAC_LWIP_GetLinkSpeed(TRUE))
    {
        errCount++;
    }
    hal_printf("Waiting for getting IP address...\r\n");

    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
void SH7619_EDMAC_LWIP_Init()
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

    SH7619_EDMAC_LWIP_AutoNegotiate();

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
        txTd.td[Index]->TDRBL = 0;
        txTd.td[Index]->RDL = 0;        
    }
    txTd.td[TX_BUFFERS - 1]->status |= DL;

    IPRC |= 0x0D00;
    EESIPR0 = 0x00250000;

    ECSR0   = 0x00000007;           /* clear all EtherC status  */
    EESR0   = 0x47FF0F9F;           /* clear all with write 1 */

    RDLAR0  = (long)(rxTd.td[0]);   /* initialaize Rx Descriptor List Address */
    TDLAR0  = (long)(txTd.td[0]);   /* initialaize Tx Descriptor List Address */
    TRSCER0 = 0x00000000;           /* copy-back status is RFE & TFE only */
    TFTR0   = 0x00000000;           /* threshold of Tx_FIFO */
    FDR0    = 0x00000101;           /* transmit fifo & receive fifo is 512 bytes */
    RMCR0   = 0x00000001;           /* receive function is normal mode(continued) */
    FCFTR0  = 0x00000001;

    APR0    = 0x00000002;
    IPGR0   = 0x00000002;

    mac = ((unsigned long)gmac_address[0] << 16) | (unsigned long)gmac_address[1];
    MAHR0 = mac;

    mac = (unsigned long)gmac_address[2];
    MALR0 = mac;

    ECSIPR0 = 0x00000017;                   /* disable EtherC status chenge interrupt */    

    RFLR0   = 1518;                         /* ether payload is 1500+ CRC;5EE   */
    ECMR0   = 0x00020000;                   /* not loopback */
    for( i = 0 ; i < 0x00010000 ; i++ );    /* wait */

    //ECMR0   = 0x00030062;                 /* disable MagicPacket, enable Tx,Rx full-duplex mode */ 
    ECMR0   = 0x00070062;                   /* disable MagicPacket, enable Tx,Rx full-duplex mode */ 
    EDRRR0  = 0x00000001;                   /* receive EDMAC start */
}


//--//

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

void Flush_All(struct netif *pNetIf)
{
    UINT32 Index;
     
    ECMR0   = 0x00000000;
    EDRRR0  = 0x00000000;
    RMCR0   = 0x00000000;
    
    SH7619_EDMAC_LWIP_recv(pNetIf);
    
    ECSR0   = 0x00000007;
    EESR0   = 0x47FF0F9F;
    TROCR0  = 0x00000000;
    CDCR0   = 0x00000000;
    LCCR0   = 0x00000000;
    CNDCR0  = 0x00000000;
    CEFCR0  = 0x00000000;
    FRECR0  = 0x00000000;
    TSFRCR0 = 0x00000000;
    TLFRCR0 = 0x00000000;
    RFCR0   = 0x00000000;
    MAFCR0  = 0x00000000;
    RMFCR0  = 0x00000000;
    
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
        txTd.td[Index]->TDRBL = 0;
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
    
    mode =  get_initial_phymd();
    
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

BOOL SH7619_EDMAC_LWIP_AutoNegotiate()
{
    BOOL rc = TRUE;

    PHY_Initialize();
    hal_printf("SH7619_EDMAC_LWIP_AutoNegotiate \r\n");
    hal_printf("AutoNegotiate complete\r\n");

    return rc;
}



UINT32 SH7619_EDMAC_LWIP_FindValidPhy()
{
    UINT32 rc = INVALID_PHY_ADDRESS;
    UINT16 value;

    for (int i = 0; i < 17; i++)
    {
        value = Ether_Reg_Read(phy_addr[i]);
        if (value != 0x0000 && value != 0xFFFF)
        {
            rc = value;
            break;
        }
    }

    if (rc != INVALID_PHY_ADDRESS)
    {
//      hal_printf( "Valid PHY Found: %d\r\n", rc);
    }

    return rc;
}



BOOL SH7619_EDMAC_LWIP_GetLinkSpeed(BOOL applySetting)
{
    BOOL rc = TRUE;

    return rc;
}

BOOL SH7619_EDMAC_LWIP_GetLinkStatus(void)
{

#if 0
    UINT16 cnt = 5;
    UINT16 value;

    do
    {
        value = Ether_Reg_Read(BASIC_MODE_STATUS_REG);
    }while (cnt --);

    if (value & STATUS_LINK_STATUS )
        return TRUE;
    return FALSE;

#else
// if PSR is connected

    UINT32 value;
    value = PSR0 ;

    if (value)
        return TRUE;
    else
        return FALSE;

#endif 

}




