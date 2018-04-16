////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) Oberon microsystems, Inc.
//
//  *** USB OTG Full Speed Host Mode Driver ***
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// todo:
// multiple controllers/ports
// multiple devices
// multiple interfaces
// dynamic channel allocation
// hub support
// suspend/resume
// power fail
// interrupt ep interval handling
// isochronous endpoints


#include <tinyhal.h>
#include <pal\com\usbhost\USBHost.h>
#include <pal\com\usbhost\USBHost_decl.h>

#ifdef STM32F4XX
#include "..\stm32f4xx.h"
#else
#include "..\stm32f2xx.h"
#endif

#include "usb_def.h"


void uart_printf( const char* text ) {
    int n = 0;
    while (text[n]) n++;
    USART_Flush( ConvertCOM_ComPort(USART_DEFAULT_PORT) );
    USART_Write( ConvertCOM_ComPort(USART_DEFAULT_PORT), text, n );
}
void print_hex(UINT32 val, UINT32 digits) {
    if (digits > 1) {
        print_hex(val >> 4, digits - 1);
        val &= 15;
    }
    if (val < 10) val += '0';
    else val += 'A' - 10;
    uart_printf((char*)&val);
}
void print_reg(UINT32 val) {
    print_hex(val, 8);
    uart_printf(" ");
}
void print_regs() {
    uart_printf("\r\n");
    uart_printf("GOTGCTL  GUSBCFG  GINTSTS  GCCFG    HCFG     HPRT\r\n");
    print_reg(OTG_FS->GOTGCTL);
    print_reg(OTG_FS->GUSBCFG);
    print_reg(OTG_FS->GINTSTS);
    print_reg(OTG_FS->GCCFG);
    if (OTG_FS->GINTSTS & 1) {
        print_reg(OTG_FS->HCFG);
        print_reg(OTG_FS->HPRT);
        print_reg(OTG_FS->HC[0].CHAR);
        print_reg(OTG_FS->HC[0].TSIZ);
    }
    uart_printf("\r\n");
}
void uart_init() {
        USART_Initialize( ConvertCOM_ComPort(USART_DEFAULT_PORT),
                          USART_DEFAULT_BAUDRATE, USART_PARITY_NONE, 8,
                          USART_STOP_BITS_ONE, USART_FLOW_NONE );
}
//#define USBH_Debug_Init uart_init()
//#define USBH_Debug(s)   uart_printf(s)
#define USBH_Debug_Init
#define USBH_Debug(s)

#ifdef DEBUG
#define _ASSERT(x) ASSERT(x)
#else
#define _ASSERT(x)
#endif


#if !defined(STM32F4_USBH_FS_USE_ID_PIN)
    #define STM32F4_USBH_FS_USE_ID_PIN 0
#endif
#if !defined(STM32F4_USBH_FS_USE_VB_PIN)
    #define STM32F4_USBH_FS_USE_VB_PIN 0
#endif
#if !defined(STM32F4_USBH_HS_USE_ID_PIN)
    #define STM32F4_USBH_HS_USE_ID_PIN 0
#endif
#if !defined(STM32F4_USBH_HS_USE_VB_PIN)
    #define STM32F4_USBH_HS_USE_VB_PIN 0
#endif

#if !defined(STM32F4_USBH_FS_POWER_PIN)
    #define STM32F4_USBH_FS_POWER_PIN GPIO_PIN_NONE
#endif
#if !defined(STM32F4_USBH_FS_POWER_LEVEL)
    #define STM32F4_USBH_FS_POWER_LEVEL 0 // active low
#endif
#if !defined(STM32F4_USBH_FS_FAULT_PIN)
    #define STM32F4_USBH_FS_FAULT_PIN GPIO_PIN_NONE
#endif
#if !defined(STM32F4_USBH_HS_POWER_PIN)
    #define STM32F4_USBH_HS_POWER_PIN GPIO_PIN_NONE
#endif
#if !defined(STM32F4_USBH_HS_POWER_LEVEL)
    #define STM32F4_USBH_HS_POWER_LEVEL 0 // active low
#endif
#if !defined(STM32F4_USBH_HS_FAULT_PIN)
    #define STM32F4_USBH_HS_FAULT_PIN GPIO_PIN_NONE
#endif


#define STM32F4_Num_CH_FS  8 // OTG FS supports 8 channels
#define STM32F4_Num_CH_HS 12 // OTG FS supports 12 channels


#if TOTAL_USBH_CONTROLLER == 2
    // USBH OTG Full Speed is controller 0
    // USBH OTG High Speed is controller 1
    
    #define STM32F4_USBH_REGS(c) (c ? OTG_HS : OTG_FS)
    
    #define STM32F4_USBH_IS_HS(c) c
    #define STM32F4_USBH_IDX(c) c
    #define STM32F4_USBH_FS_IDX 0
    #define STM32F4_USBH_HS_IDX 1
    
    #define STM32F4_USBH_DM_PIN(c) (c ? 30 : 11) // B14,A11
    #define STM32F4_USBH_DP_PIN(c) (c ? 31 : 12) // B15,A12
    #define STM32F4_USBH_VB_PIN(c) (c ? 29 :  9) // B13,A9
    #define STM32F4_USBH_ID_PIN(c) (c ? 28 : 10) // B12,A10
    
    #define STM32F4_USBH_USE_ID_PIN(c) (c ? STM32F4_USBH_HS_USE_ID_PIN : STM32F4_USBH_FS_USE_ID_PIN)
    #define STM32F4_USBH_USE_VB_PIN(c) (c ? STM32F4_USBH_HS_USE_VB_PIN : STM32F4_USBH_FS_USE_VB_PIN)
    #define STM32F4_USBH_POWER_PIN(c) (c ? STM32F4_USBH_HS_POWER_PIN : STM32F4_USBH_FS_POWER_PIN)
    #define STM32F4_USBH_POWER_LEVEL(c) (c ? STM32F4_USBH_HS_POWER_LEVEL : STM32F4_USBH_FS_POWER_LEVEL)
    #define STM32F4_USBH_FAULT_PIN(c) (c ? STM32F4_USBH_HS_FAULT_PIN : STM32F4_USBH_FS_FAULT_PIN)
    #define STM32F4_USBH_ALT_MODE(c) (GPIO_ALT_MODE)(c ? 0x2C2 : 0x2A2) // AF12,AF10, 50MHz
    
    #if USBH_MAX_QUEUES <= STM32F4_Num_CH_FS
        #define USBH_MAX_CH(c) USBH_MAX_QUEUES
        #define USBH_MAX_BUFFERS (2 * USBH_MAX_QUEUES - 2)
    #elif USBH_MAX_QUEUES <= STM32F4_Num_CH_HS
        #define USBH_MAX_CH(c) (c ? USBH_MAX_QUEUES : STM32F4_Num_CH_FS)
        #define USBH_MAX_BUFFERS (USBH_MAX_QUEUES + STM32F4_Num_CH_FS - 2)
    #else
        #define USBH_MAX_CH(c) (c ? STM32F4_Num_CH_HS : STM32F4_Num_CH_FS)
        #define USBH_MAX_BUFFERS (STM32F4_Num_CH_HS + STM32F4_Num_CH_FS - 2)
    #endif
    

#elif defined(STM32F4_USBH_HS)
    // use USBH OTG High Speed
    
    #define STM32F4_USBH_REGS(c) OTG_HS
    
    #define STM32F4_USBH_IS_HS(c) TRUE
    #define STM32F4_USBH_IDX(c) 0
    #define STM32F4_USBH_FS_IDX 0
    #define STM32F4_USBH_HS_IDX 0

    #define STM32F4_USBH_DM_PIN(c) 30 // B14
    #define STM32F4_USBH_DP_PIN(c) 31 // B15
    #define STM32F4_USBH_VB_PIN(c) 29 // B13
    #define STM32F4_USBH_ID_PIN(c) 28 // B12
    
    #define STM32F4_USBH_USE_ID_PIN(c) STM32F4_USBH_HS_USE_ID_PIN
    #define STM32F4_USBH_USE_VB_PIN(c) STM32F4_USBH_HS_USE_VB_PIN
    #define STM32F4_USBH_POWER_PIN(c) STM32F4_USBH_HS_POWER_PIN
    #define STM32F4_USBH_POWER_LEVEL(c) STM32F4_USBH_HS_POWER_LEVEL
    #define STM32F4_USBH_FAULT_PIN(c) STM32F4_USBH_HS_FAULT_PIN
    #define STM32F4_USBH_ALT_MODE(c) (GPIO_ALT_MODE)0x2C2; // AF12, 50MHz
    
    #if USBH_MAX_QUEUES <= STM32F4_Num_CH_HS
        #define USBH_MAX_CH(c) USBH_MAX_QUEUES
        #define USBH_MAX_BUFFERS (USBH_MAX_QUEUES - 1)
    #else
        #define USBH_MAX_CH(c) STM32F4_Num_CH_HS
        #define USBH_MAX_BUFFERS (STM32F4_Num_CH_HS - 1)
    #endif
    

#else
    // use OTG Full Speed
    
    #define STM32F4_USBH_REGS(c) OTG_FS
    
    #define STM32F4_USBH_IS_HS(c) FALSE
    #define STM32F4_USBH_IDX(c) 0
    #define STM32F4_USBH_FS_IDX 0
    #define STM32F4_USBH_HS_IDX 0
    
    #define STM32F4_USBH_DM_PIN(c) 11 // A11
    #define STM32F4_USBH_DP_PIN(c) 12 // A12
    #define STM32F4_USBH_VB_PIN(c)  9 // A9
    #define STM32F4_USBH_ID_PIN(c) 10 // A10
    
    #define STM32F4_USBH_USE_ID_PIN(c) STM32F4_USBH_FS_USE_ID_PIN
    #define STM32F4_USBH_USE_VB_PIN(c) STM32F4_USBH_FS_USE_VB_PIN
    #define STM32F4_USBH_POWER_PIN(c) STM32F4_USBH_FS_POWER_PIN
    #define STM32F4_USBH_POWER_LEVEL(c) STM32F4_USBH_FS_POWER_LEVEL
    #define STM32F4_USBH_FAULT_PIN(c) STM32F4_USBH_FS_FAULT_PIN
    #define STM32F4_USBH_ALT_MODE(c) (GPIO_ALT_MODE)0x2A2; // AF10, 50MHz
    
    #if USBH_MAX_QUEUES <= STM32F4_Num_CH_FS
        #define USBH_MAX_CH(c) USBH_MAX_QUEUES
        #define USBH_MAX_BUFFERS (USBH_MAX_QUEUES - 1)
    #else
        #define USBH_MAX_CH(c) STM32F4_Num_CH_FS
        #define USBH_MAX_BUFFERS (STM32F4_Num_CH_FS - 1)
    #endif
    
    
#endif


// FIFO sizes (in 32 bit words)
#define USBH_RXFIFO_SIZE  128 // 512 bytes
#define USBH_TXNFIFO_SIZE 128 // 512 bytes
#define USBH_TXPFIFO_SIZE  64 // 256 bytes


// State variables for one controller
typedef struct {
    USBH_CONTROLLER_STATE state;
    UINT16 endpointStatus[USBH_MAX_QUEUES];
    UINT8 epToChannel[16];  // per device
    HAL_COMPLETION timer; // delayed command processing
    BOOL initialized;
    BOOL pinsProtected;
    UINT16 errorCount[USBH_MAX_QUEUES];
} STM32F4_USBH_STATE;

/* State variables for the controllers */
static STM32F4_USBH_STATE STM32F4_USBH_ControllerState[TOTAL_USBH_CONTROLLER];

/* Queues for all data endpoints */
Hal_Queue_KnownSize<USB_PACKET64,USB_QUEUE_PACKET_COUNT> USBHQueueBuffers[USBH_MAX_BUFFERS];


void CPU_USBH_Initialize2( STM32F4_USBH_STATE *State );
void STM32F4_USBH_StartReset( STM32F4_USBH_STATE* State );
void STM32F4_USBH_StopRes( STM32F4_USBH_STATE* State );


/*
 * Initialize Channel Registers
 * type:
 *   OTG_HCCHAR_EPDIR   // direction IN
 *   OTG_HCCHAR_EPTYP_x // endpoint type
 */
void STM32F4_USBH_InitChannel (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State,
                               UINT32 ch, UINT32 ep, UINT32 type)
{
    OTG->HC[ch].INT = 0xFFFFFFFF; // clear pending interrupts
    
    ((STM32F4_USBH_STATE*)State)->epToChannel[ep] = ch;
    
    OTG->HC[ch].INTMSK = OTG_HCINTMSK_XFRCM | OTG_HCINTMSK_STALLM | OTG_HCINTMSK_NAKM
                       | OTG_HCINTMSK_DTERRM | OTG_HCINTMSK_TXERRM
                       | OTG_HCINTMSK_BBERRM | OTG_HCINTMSK_FRMORM;
    type |= OTG_HCCHAR_MCNT_1         // 1 transaction per frame (periodic)
          | ep << 11                  // endpoint
          | State->Address << 22      // device address
          | State->MaxPacketSize[ch]; // max packet size
    if ((OTG->HPRT & OTG_HPRT_PSPD) == OTG_HPRT_PSPD_L) { // low speed
        type |= OTG_HCCHAR_LSDEV;
    }
    OTG->HC[ch].CHAR = type;
    OTG->HC[ch].TSIZ = ep ? OTG_HCTSIZ_DPID_D0 : OTG_HCTSIZ_DPID_D1;
    OTG->HAINTMSK |= (1 << ch);  // enable channel interrupt
}

// Halt a channel
void STM32F4_USBH_HaltChannel (OTG_TypeDef* OTG, UINT32 ch)
{
    OTG->HC[ch].INTMSK |= OTG_HCINTMSK_CHHM; // enable halted interrupt

    UINT32 chch = OTG->HC[ch].CHAR; // channel characteristics
    
    // get relevant queue status
    UINT32 qStatus;
    if (chch & OTG_HCCHAR_EPTYP_P) { // periodic endpoint
        qStatus = OTG->HPTXSTS;
    } else { // non-periodic endpoint
        qStatus = OTG->HNPTXSTS;
    }
    if (qStatus & OTG_HPTXSTS_PTQXSAV) { // queue space available
        OTG->HC[ch].CHAR = chch | (OTG_HCCHAR_CHENA | OTG_HCCHAR_CHDIS); // disable
    } else {
        OTG->HC[ch].CHAR = chch & ~OTG_HCCHAR_CHENA | OTG_HCCHAR_CHDIS; // flush
        // what exactly is the difference ?
    }
}

// Disable all channels
void STM32F4_USBH_Driver_DisableChannels (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State)
{

    for (int ch = 0; ch < USBH_MAX_QUEUES; ch++) {
        if (OTG->HC[ch].CHAR & OTG_HCCHAR_CHENA) {
            ((STM32F4_USBH_STATE*)State)->errorCount[ch] = 5;
            STM32F4_USBH_HaltChannel(OTG, ch);
        }
    }
    
}


/*
 * Resume Event Interrupt Handler
 */
void STM32F4_USBH_Driver_ResumeEvent (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State)
{
    // Device requested resume
    // resume bit set automatically
    
    STM32F4_USBH_STATE* state = (STM32F4_USBH_STATE*) State;
    
    // setup timer to clear resume bit after 20ms
    state->timer.Abort();
    state->timer.InitializeForISR((HAL_CALLBACK_FPN)STM32F4_USBH_StopRes, State);
    state->timer.EnqueueDelta(20000);
    
    State->DeviceState = USB_DEVICE_STATE_CONFIGURED;
    USBH_StateCallback( State );
}

/*
 * Device Attached Interrupt Handler
 */
void STM32F4_USBH_Driver_AttachedEvent (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State)
{
    OTG->HAINTMSK = 0;
    
    // flush fifos
    while (!(OTG->GRSTCTL & OTG_GRSTCTL_AHBIDL)); // wait for AHB idle
    OTG->GRSTCTL |= OTG_GRSTCTL_TXFFLSH | OTG_GRSTCTL_TXF_ALL; // flush tx
    while (OTG->GRSTCTL & OTG_GRSTCTL_TXFFLSH); // wait for completion
    OTG->GRSTCTL |= OTG_GRSTCTL_RXFFLSH; // flush rx
    while (OTG->GRSTCTL & OTG_GRSTCTL_RXFFLSH); // wait for completion
    
    State->DeviceState = USB_DEVICE_STATE_ATTACHED;
    USBH_StateCallback( State );
}

/*
 * Device Detached Interrupt Handler
 */
void STM32F4_USBH_Driver_DetachedEvent (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State)
{
    STM32F4_USBH_Driver_DisableChannels(OTG, State); // disable all channels
    
    State->DeviceState = USB_DEVICE_STATE_DETACHED;
    USBH_StateCallback( State );
}

/*
 * Device Enabled Interrupt Handler
 */
void STM32F4_USBH_Driver_EnabledEvent (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State)
{
    // initialize channel 0: ep0 control
    STM32F4_USBH_InitChannel(OTG, State, 0, 0, OTG_HCCHAR_EPTYP_C);
    
    State->DeviceState = USB_DEVICE_STATE_DEFAULT;
    USBH_StateCallback( State );
}

/*
 * Device Disabled Interrupt Handler (Port Babble Error)
 */
void STM32F4_USBH_Driver_DisabledEvent (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State)
{
    STM32F4_USBH_Driver_DisableChannels(OTG, State); // disable all channels
    
    UINT8 error = State->ConfigError;
    if (++error < 3) {
        // set state to Attached to re-initialize device
        State->DeviceState = USB_DEVICE_STATE_ATTACHED;
    } else {
        // give up and set state to Unusable
        State->DeviceState = USB_DEVICE_STATE_UNINITIALIZED;
    }
    USBH_StateCallback(State);
    State->ConfigError = error;
}

/*
 * Data Received Interrupt Handler
 */
void STM32F4_USBH_Driver_RX_Int (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State,
                                 UINT32 ch, UINT32 count)
{
    ASSERT_IRQ_MUST_BE_OFF();
    
    UINT32* pd;
    
    UINT32 chch = OTG->HC[ch].CHAR; // channel characteristics
    UINT32 ep = chch >> 11 & 0xF;   // endpoint
    

    if (ep == 0) { // control endpoint
        USBH_Debug(count ? "c" : "z");
        
        pd = (UINT32*)State->Data;
        State->DataSize = count;
    } else { // data endpoint
        USBH_Debug("r");
        
        BOOL full;
        USB_PACKET64* Packet64 = USBH_RxEnqueue( State, ep, full );
        if (Packet64 == NULL) {  // should not happen
            USBH_Debug("?");
            _ASSERT(0);
        }
        pd = (UINT32*)Packet64->Buffer;
        Packet64->Size = count;
    }

    // read data
    volatile UINT32* ps = OTG->DFIFO[ch];
    for (int c = count; c > 0; c -= 4) {
        *pd++ = *ps;
    }
        
    // data handling & Rx reenabling delayed to transfer completed interrupt
}

/*
 * Data Transmit Interrupt Handler
 */
void STM32F4_USBH_Driver_TX_Int (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State, UINT32 ch)
{
    ASSERT_IRQ_MUST_BE_OFF();
    
    // get channel characteristics
    UINT32 chch = OTG->HC[ch].CHAR & ~(OTG_HCCHAR_CHENA | OTG_HCCHAR_CHDIS);
    UINT32 ep = (chch >> 11) & 0xF;  // endpoint

    // get relevant queue status
    UINT32 qStatus;
    if (chch & OTG_HCCHAR_EPTYP_P) { // periodic endpoint
        qStatus = OTG->HPTXSTS;
    } else { // non-periodic endpoint
        qStatus = OTG->HNPTXSTS;
    }
    
    if (qStatus & OTG_HPTXSTS_PTQXSAV                     // queue space available
        && (qStatus & OTG_HPTXSTS_PTXFSAVL) >= 64 >> 2) { // fifo space available
        UINT32* ps = NULL;
        UINT32 count;

        if (ep == 0) { // control endpoint
            ps = (UINT32*)State->Data;
            count = State->DataSize;
            if (count == 0xFF) { // setup packet
                ps = (UINT32*)State->Request;
                count = USBH_SETUP_COUNT;
            }
            USBH_Debug(count ? "x" : "n");
        } else if (State->Queues[ep] != NULL && State->IsTxQueue[ep]) { // Tx data endpoint
            USB_PACKET64* Packet64 = USBH_TxDequeue( State, ep, FALSE ); // do not pop!
            if (Packet64) {  // data to send
                ps = (UINT32*)Packet64->Buffer;
                count = Packet64->Size;
                
                USBH_Debug("s");
            }
        }
        
        if (ps) { // data to send
            // enable channel
            OTG->HC[ch].TSIZ = OTG->HC[ch].TSIZ & OTG_HCTSIZ_DPID
                               | OTG_HCTSIZ_PKTCNT_1 | count;
            OTG->HC[ch].CHAR = chch | OTG_HCCHAR_CHENA;
            
            // write data
            volatile UINT32* pd = OTG->DFIFO[ch];
            for (int c = count; c > 0; c -= 4) {
                *pd = *ps++;
            }
        }
    } else {
        // schedule transfer later !!!
        // use OTG_GINTSTS_[N]PTXFE
    }
}

/*
 * Channel Interrupt Handler
 */
void STM32F4_USBH_Driver_Ch_Int (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State, UINT32 ch)
{
    ASSERT_IRQ_MUST_BE_OFF();
    
    // get channel characteristics
    UINT32 chch = OTG->HC[ch].CHAR & ~(OTG_HCCHAR_CHENA | OTG_HCCHAR_CHDIS);
    UINT32 ep = (chch >> 11) & 0xF;  // endpoint
    
    UINT32 intPend = OTG->HC[ch].INT; // pending interrupts
    OTG->HC[ch].INT = intPend; // clear interrupts
/*    
    // only needed for multi transaction requests
    if (intPend & OTG_HCINT_ACK) { // ACK
        USBH_Debug(":");
        ((STM32F4_USBH_STATE*)State)->errorCount[ch] = 0;
    }
*/
    if (intPend & OTG_HCINT_XFRC) { // transaction completed
        USBH_Debug(">");
        ((STM32F4_USBH_STATE*)State)->errorCount[ch] = 0;
        // confirm data
        if (ep) { // stream data
            State->EndpointStatus[ep] = USBH_EP_OK;
            if (!(chch & OTG_HCCHAR_EPDIR)) { // direction OUT
                USBH_TxDequeue( State, ep, TRUE ); // advance queue pointer
            }
            // next transaction started below
        } else { // control channel
            UINT8 res = USBH_ControlCallback(State, USBH_EP_OK);
            if (res == USBH_TRANSFER_DONE) {
                return;
            } else {
                UINT8 actual = chch & OTG_HCCHAR_EPDIR ? USBH_TRANSFER_IN : USBH_TRANSFER_OUT;
                if (res != actual) { // direction change
                    chch ^= OTG_HCCHAR_EPDIR;
                    OTG->HC[ch].CHAR = chch;
                    OTG->HC[ch].TSIZ = OTG_HCTSIZ_DPID_D1; // start with data1
                }
                // next transaction started below
            }
        }
    } else if (intPend & OTG_HCINT_STALL) { // Stall
        USBH_Debug("#");
        ((STM32F4_USBH_STATE*)State)->errorCount[ch] = 4;
        STM32F4_USBH_HaltChannel(OTG, ch); // halt channel
        return;
    } else if (intPend & (OTG_HCINT_NAK | OTG_HCINT_DTERR)) { // NAK
        if (intPend & OTG_HCINT_DTERR) USBH_Debug(",");
        ((STM32F4_USBH_STATE*)State)->errorCount[ch] = 0;
        if (chch & OTG_HCCHAR_EPDIR && !(chch & OTG_HCCHAR_EPTYP_P)) { // bulk/ctrl IN
            if (OTG->HPRT & OTG_HPRT_PENA) {
                OTG->HC[ch].CHAR = chch | OTG_HCCHAR_CHENA; // reschedule IN transaction
            }
        } else { // interrupt or OUT
            STM32F4_USBH_HaltChannel(OTG, ch); // halt channel
        }
        return;
    } else if (intPend & (OTG_HCINT_TXERR | OTG_HCINT_BBERR | OTG_HCINT_FRMOR)) {
        USBH_Debug("!");
        if (intPend & OTG_HCINT_BBERR) USBH_Debug("b");
        if (intPend & OTG_HCINT_FRMOR) USBH_Debug("f");
        ((STM32F4_USBH_STATE*)State)->errorCount[ch]++;
        STM32F4_USBH_HaltChannel(OTG, ch); // halt channel
        return;
    } else if (intPend & OTG_HCINT_CHH) { // channel halted
        USBH_Debug("h");
        OTG->HC[ch].INTMSK &= ~OTG_HCINTMSK_CHHM; // disable halted interrupt
        switch (((STM32F4_USBH_STATE*)State)->errorCount[ch]) {
        case 3: // error after 3 retries
            if (ep == 0) USBH_ControlCallback(State, USBH_EP_ERROR);
            else State->EndpointStatus[ep] = USBH_EP_ERROR;
            return;
        case 4: // STALL
            if (ep == 0) USBH_ControlCallback(State, USBH_EP_STALL);
            else State->EndpointStatus[ep] = USBH_EP_STALL;
            return;
        case 5: // detached / port error
            return;
        default: // NAK or error retry
            // next transaction started below
        } 
    } else {
        return;
    }
    
    if (!(OTG->HPRT & OTG_HPRT_PENA)) { // device disconnected
        return;
    }
    
    // Start new transaction
    
    if (chch & OTG_HCCHAR_EPTYP_P) { // periodic channel
        chch ^= OTG_HCCHAR_ODDFRM; // toggle even/odd frame
        OTG->HC[ch].CHAR = chch;
    }
    
    if (chch & OTG_HCCHAR_EPDIR) { // direction IN 
    
        if (ep != 0 && State->Queues[ep]->IsFull()) { // no receive buffer
            return;
        }
        
        USBH_Debug("$");
        
        // check queue space ???
        
        // enable channel
        OTG->HC[ch].TSIZ = OTG->HC[ch].TSIZ & OTG_HCTSIZ_DPID
                           | OTG_HCTSIZ_PKTCNT_1 | State->MaxPacketSize[ep];
        OTG->HC[ch].CHAR = chch | OTG_HCCHAR_CHENA; // start IN transaction
        
    } else { // direction OUT
        STM32F4_USBH_Driver_TX_Int(OTG, State, ch);
    }
}


/*
 * Main Interrupt Handler
 */
void STM32F4_USBH_Driver_Interrupt (OTG_TypeDef* OTG, USBH_CONTROLLER_STATE* State)
{
    INTERRUPT_START;

    UINT32 intPend = OTG->GINTSTS; // get pending bits
    
    while (intPend & OTG_GINTSTS_RXFLVL) { // RxFifo non empty
        UINT32 status = OTG->GRXSTSP; // read and pop status word from fifo
        int ch = status & OTG_GRXSTSP_CHNUM;
        int count = (status & OTG_GRXSTSP_BCNT) >> 4;
        status &= OTG_GRXSTSP_PKTSTS;
        if (status == OTG_GRXSTSP_PKTSTS_PR) { // data received
            STM32F4_USBH_Driver_RX_Int(OTG, State, ch, count);
/*
        } else if (status == OTG_GRXSTSP_PKTSTS_DC) { // in transaction completed
            USBH_Debug("<");
        } else if (status == OTG_GRXSTSP_PKTSTS_CH) { // halt
            USBH_Debug("H");
        } else { // OTG_GRXSTSP_PKTSTS_TE  toggle error
            USBH_Debug("g");
*/
        }
        intPend = OTG->GINTSTS; // update pending bits
    }
    
    if (intPend & OTG_GINTSTS_HCINT) { // host channel interrupt
        UINT32 bits = OTG->HAINT & 0xFFFF; // pending channels
        int ch = 0;
        while (bits) {
            if (bits & 1) STM32F4_USBH_Driver_Ch_Int(OTG, State, ch);
            ch++;
            bits >>= 1;
        }         
    }
    if (intPend & OTG_GINTSTS_HPRTINT) { // host port interrupt
        UINT32 hprt = OTG->HPRT;
/*        
        if (hprt & OTG_HPRT_POCCHNG) { // overcurrent
            USBH_Debug("O");
        }
*/
        if (hprt & OTG_HPRT_PENCHNG) {
            if (hprt & OTG_HPRT_PENA) { // enabled
                USBH_Debug("E");
                STM32F4_USBH_Driver_EnabledEvent(OTG, State);
            } else { // disabled (detached, overcurrent, or babble)
                if (hprt & OTG_HPRT_POCA) { // overcurrent
                    USBH_Debug("o");
                } else if (!(hprt & OTG_HPRT_PCSTS)) { // detached
                    if (!(intPend & OTG_GINTSTS_DISCINT)) USBH_Debug("d");
                } else { // babble error
                    USBH_Debug("B");
                    // re-enable port
                    STM32F4_USBH_Driver_DisabledEvent(OTG, State);
                }
            }
        }
        if (hprt & OTG_HPRT_PCDET) { // attach detected
            USBH_Debug("A");
            STM32F4_USBH_Driver_AttachedEvent(OTG, State);
        }
        // clear interrupt bits but preserve enabled state!
        OTG->HPRT = hprt & ~OTG_HPRT_PENA;
    }
    
    if (intPend & OTG_GINTSTS_SOF) { // start of frame
        // schedule interrupt requests !!!
        OTG->GINTSTS = OTG_GINTSTS_SOF; // clear interrupt
    }
    if (intPend & OTG_GINTSTS_DISCINT) { // detached
        if (!(OTG->HPRT & OTG_HPRT_PCSTS)) {
            USBH_Debug("D");
            STM32F4_USBH_Driver_DetachedEvent(OTG, State);
        }
        OTG->GINTSTS = OTG_GINTSTS_DISCINT; // clear interrupt
    }
    if (intPend & OTG_GINTSTS_WKUPINT) { // wakeup
        USBH_Debug("R");
        STM32F4_USBH_Driver_ResumeEvent(OTG, State);
        OTG->GINTSTS = OTG_GINTSTS_WKUPINT; // clear interrupt
    }
    
/*
    if (intPend & OTG_GINTSTS_PTXFE) { // periodic Tx Fifo empty
        // write packet to fifo and queue
        // or
        // disable interrupt
    }
    if (intPend & OTG_GINTSTS_NPTXFE) { // non-periodic Tx Fifo empty
        // write packet to fifo and queue
        // or
        // disable interrupt
    }
*/    
    
    INTERRUPT_END;
}

/*
 * OTG FS Interrupt Handler
 */
void STM32F4_USBH_Driver_FS_Interrupt (void* param)
{
    STM32F4_USBH_Driver_Interrupt(OTG_FS, &STM32F4_USBH_ControllerState[STM32F4_USBH_FS_IDX].state);
}

/*
 * OTG HS Interrupt Handler
 */
void STM32F4_USBH_Driver_HS_Interrupt (void* param)
{
    STM32F4_USBH_Driver_Interrupt(OTG_HS, &STM32F4_USBH_ControllerState[STM32F4_USBH_HS_IDX].state);
}

/*
 * Power Fault Interrupt Handler
 */
void STM32F4_USBH_Driver_Fault_Interrupt (GPIO_PIN Pin, BOOL PinState, void* Param)
{
    INTERRUPT_START;
    USBH_CONTROLLER_STATE* State = (USBH_CONTROLLER_STATE*)Param;
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);

    // disable bus power
//    if (STM32F4_USBH_POWER_PIN(Controller) != GPIO_PIN_NONE) {
//            CPU_GPIO_DisablePin(STM32F4_USBH_POWER_PIN(Controller),
//                            RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);
//    }

    // reset power and enable bits, preserve interrupts
//    OTG->HPRT &= ~(OTG_HPRT_PPWR | OTG_HPRT_POCCHNG | OTG_HPRT_PENCHNG | OTG_HPRT_PCDET);
    
    USBH_Debug("F");
    
    INTERRUPT_END;
}

USBH_CONTROLLER_STATE * CPU_USBH_GetState( int Controller )
{
    if ((UINT32)Controller >= TOTAL_USBH_CONTROLLER) return NULL;
    return &STM32F4_USBH_ControllerState[STM32F4_USBH_IDX(Controller)].state;
}

HRESULT CPU_USBH_Initialize( int Controller )
{
    USBH_Debug_Init;

    if ((UINT32)Controller >= TOTAL_USBH_CONTROLLER) return S_FALSE;
    
    STM32F4_USBH_STATE *State = &STM32F4_USBH_ControllerState[STM32F4_USBH_IDX(Controller)];
    
    if (State->initialized) return S_OK;
    State->initialized = TRUE;
    
    // enable USB clock
    if (STM32F4_USBH_IS_HS(Controller)) { // HS on AHB1
        RCC->AHB1ENR |= RCC_AHB1ENR_OTGHSEN;
        // this is needed to enable the FS phy clock when the CPU is sleeping
        RCC->AHB1LPENR &= ~RCC_AHB1LPENR_OTGHSULPILPEN;
    } else { // FS on AHB2
        RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
    }
    
    // setup pins
    State->pinsProtected = TRUE;
    CPU_USBH_ProtectPins(Controller, FALSE);
    
    // handle power fault pin
    if (STM32F4_USBH_FAULT_PIN(Controller) != GPIO_PIN_NONE) {
        CPU_GPIO_EnableInputPin2(STM32F4_USBH_FAULT_PIN(Controller), FALSE,
            STM32F4_USBH_Driver_Fault_Interrupt, State, GPIO_INT_EDGE_LOW, RESISTOR_PULLUP);
    }
    
    // setup usb state variables
    State->state.EndpointStatus = State->endpointStatus;
    State->state.EndpointCount  = USBH_MAX_CH(Controller);
    
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(Controller);

    GLOBAL_LOCK(irq);
    
    OTG->GAHBCFG = 0; // global interrupt disable
    OTG->GUSBCFG |= OTG_GUSBCFG_PHYSEL;  // internal PHY

    int n;
    do {
        while (!(OTG->GRSTCTL & OTG_GRSTCTL_AHBIDL)); // wait for AHB idle
        OTG->GRSTCTL |= OTG_GRSTCTL_CSRST; // start core soft reset
        n = 100000;
        while (OTG->GRSTCTL & OTG_GRSTCTL_CSRST && --n); // wait for reset completed or timeout
    } while(n == 0); // repeat reset if wait timed out

    OTG->GCCFG  |= OTG_GCCFG_VBUSASEN       // A device Vbus sensing
                 | OTG_GCCFG_PWRDWN;        // transceiver enabled
    
    // no automatic device - host switch
    if (STM32F4_USBH_USE_VB_PIN(Controller) == 0) { // no Vbus pin
        OTG->GCCFG |= OTG_GCCFG_NOVBUSSENS; // disable vbus sense
    }
    OTG->GUSBCFG |= OTG_GUSBCFG_FHMOD;      // force host mode
                

    // enable bus power driver
    if (STM32F4_USBH_POWER_PIN(Controller) != GPIO_PIN_NONE) {
        CPU_GPIO_EnableOutputPin(STM32F4_USBH_POWER_PIN(Controller), STM32F4_USBH_POWER_LEVEL(Controller));
    }
    
    // Continue initialization after 200ms power up delay
    State->timer.Abort();
    State->timer.InitializeForISR((HAL_CALLBACK_FPN)CPU_USBH_Initialize2, State);
    State->timer.EnqueueDelta(200000);
    
    return S_OK;
}

// Second (delayed) part of initialization
void CPU_USBH_Initialize2( STM32F4_USBH_STATE *State )
{
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);

    GLOBAL_LOCK(irq);
    
    OTG->HPRT |= OTG_HPRT_PPWR; // set bus power, reset enable & interrupts
    OTG->PCGCCTL = 0;
    
    // configure Fifos
    OTG->GRXFSIZ = USBH_RXFIFO_SIZE; // Rx Fifo
    OTG->HNPTXFSIZ = (USBH_TXNFIFO_SIZE << 16) | USBH_RXFIFO_SIZE; // NP Tx Fifo
    OTG->HPTXFSIZ = (USBH_TXPFIFO_SIZE << 16) | USBH_RXFIFO_SIZE + USBH_TXNFIFO_SIZE; // P Tx Fifo

    // configure interrupts
    OTG->HAINTMSK = 0;
    OTG->GINTSTS = 0xFFFFFFFF; // clear all interrupts
    OTG->GINTMSK = OTG_GINTMSK_RXFLVLM | OTG_GINTMSK_SOFM | OTG_GINTMSK_WUIM
                 | OTG_GINTMSK_HCIM | OTG_GINTMSK_HPRTIM | OTG_GINTMSK_DISCINT;
/*
                 | OTG_GINTMSK_NPTXFEM | OTG_GINTMSK_PTXFEM; // tx fifo empty interrupts
                 | OTG_GINTMSK_SRQIM | OTG_GINTMSK_CIDSCHG;  // id status change 
*/                
    // set interrupt handlers
    if (STM32F4_USBH_IS_HS(State->ControllerNum)) { // HS
        CPU_INTC_ActivateInterrupt(OTG_HS_IRQn,      STM32F4_USBH_Driver_HS_Interrupt, 0);
        CPU_INTC_ActivateInterrupt(OTG_HS_WKUP_IRQn, STM32F4_USBH_Driver_HS_Interrupt, 0);
    } else { // FS
        CPU_INTC_ActivateInterrupt(OTG_FS_IRQn,      STM32F4_USBH_Driver_FS_Interrupt, 0);
        CPU_INTC_ActivateInterrupt(OTG_FS_WKUP_IRQn, STM32F4_USBH_Driver_FS_Interrupt, 0);
    }
    
    OTG->GAHBCFG |= OTG_GAHBCFG_GINTMSK; // global interrupt enable
    
    USBH_Debug("*");
}

HRESULT CPU_USBH_Uninitialize( int Controller )
{
    if (STM32F4_USBH_IS_HS(Controller)) { // HS
        CPU_INTC_DeactivateInterrupt(OTG_HS_WKUP_IRQn);
        CPU_INTC_DeactivateInterrupt(OTG_HS_IRQn);
    } else { // FS
        CPU_INTC_DeactivateInterrupt(OTG_FS_WKUP_IRQn);
        CPU_INTC_DeactivateInterrupt(OTG_FS_IRQn);
    }
    
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(Controller);
    
    GLOBAL_LOCK(irq);
    
    OTG->HPRT &= ~OTG_HPRT_PPWR; // reset power, enable & interrupt bits
    
    // disable power fault interrupt
    if (STM32F4_USBH_FAULT_PIN(Controller) != GPIO_PIN_NONE) {
        CPU_GPIO_DisablePin(STM32F4_USBH_FAULT_PIN(Controller),
                            RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);
    }
    
    // disable bus power driver
    if (STM32F4_USBH_POWER_PIN(Controller) != GPIO_PIN_NONE) {
        CPU_GPIO_DisablePin(STM32F4_USBH_POWER_PIN(Controller),
                            RESISTOR_DISABLED, 0, GPIO_ALT_PRIMARY);
    }

    CPU_USBH_ProtectPins(Controller, TRUE);
    
    // disable USB clock
    if (STM32F4_USBH_IS_HS(Controller)) { // HS on AHB1
        RCC->AHB1ENR &= ~RCC_AHB1ENR_OTGHSEN;
    } else { // FS on AHB2
        RCC->AHB2ENR &= ~RCC_AHB2ENR_OTGFSEN;
    }
    
    STM32F4_USBH_ControllerState[STM32F4_USBH_IDX(Controller)].initialized = FALSE;

    return S_OK;
}

/* 
 * Reset device address & control channel packet size
 * according to the changes in the controller state
 */
void CPU_USBH_SetControl (USBH_CONTROLLER_STATE* State)
{
    if (State == NULL) return;

    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);
    
    // control channel only
    OTG->HC[0].CHAR = OTG->HC[0].CHAR
                      & ~(OTG_HCCHAR_CHENA | OTG_HCCHAR_CHDIS | OTG_HCCHAR_DAD)
                      | (State->Address << 22)
                      | State->MaxPacketSize[0];
}

/*
 * Open a Channel for the given endpoint
 */
void CPU_USBH_OpenChannel (USBH_CONTROLLER_STATE* State, const USB_ENDPOINT_DESCRIPTOR* pEp)
{
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);
    
    USBH_Debug("l");

    UINT32 ep = pEp->bEndpointAddress & 0x0F;
    UINT32 ch = ep; // !!!
    State->Queues[ep] = &USBHQueueBuffers[ep - 1];
    State->MaxPacketSize[ep] = pEp->wMaxPacketSize;
    BOOL isTx = TRUE;
    UINT32 type = pEp->bmAttributes << 18; // OTG_HCCHAR_EPTYP_x
    if (pEp->bEndpointAddress & USB_ENDPOINT_DIRECTION_IN) {
        isTx = FALSE;
        type |= OTG_HCCHAR_EPDIR;
    }
    State->IsTxQueue[ep] = isTx;
    State->EndpointStatus[ep] = USBH_EP_OK;
    STM32F4_USBH_InitChannel(OTG, State, ch, ep, type);
}

/*
 * Start a control channel transfer by sending the initial Setup packet
 */
BOOL CPU_USBH_StartSetup( USBH_CONTROLLER_STATE* State )
{
    if (State == NULL) return FALSE;

    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);

    USBH_Debug("C");

    GLOBAL_LOCK(irq);
    
    ((STM32F4_USBH_STATE*)State)->errorCount[0] = 0;
    
    // setup packet on endpoint 0 OUT channel
    OTG->HC[0].TSIZ = OTG_HCTSIZ_DPID_MS | OTG_HCTSIZ_PKTCNT_1 | USBH_SETUP_COUNT;
    OTG->HC[0].CHAR = OTG->HC[0].CHAR & ~(OTG_HCCHAR_CHDIS | OTG_HCCHAR_EPDIR)
                      | OTG_HCCHAR_CHENA; // enable channel
    
    // Write packet to fifo
    *OTG->DFIFO[0] = ((UINT32*)State->Request)[0];
    *OTG->DFIFO[0] = ((UINT32*)State->Request)[1];
    
    State->DataSize = 0xFF; // Setup packet marker (for error repeat)
    
    return TRUE;
}

BOOL CPU_USBH_StartOutput( USBH_CONTROLLER_STATE* State, int ep )
{
    if (State == NULL || ep >= State->EndpointCount) return FALSE;

    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);

    USBH_Debug("t");
    
    GLOBAL_LOCK(irq);

    // If endpoint is not an output
    if( State->Queues[ep] == NULL || !State->IsTxQueue[ep] )
        return FALSE;

    UINT32 ch = ((STM32F4_USBH_STATE*)State)->epToChannel[ep];
    
    if ((OTG->HC[ch].CHAR & (OTG_HCCHAR_CHDIS | OTG_HCCHAR_CHENA)) == 0) {
        // reset error count
        ((STM32F4_USBH_STATE*)State)->errorCount[ch] = 0;
        // write first packet
        STM32F4_USBH_Driver_TX_Int(OTG, State, ch);
    }
    
    return TRUE;
}

BOOL CPU_USBH_RxEnable( USBH_CONTROLLER_STATE* State, int ep )
{

    // If this is not a legal Rx queue
    if( State == NULL || State->Queues[ep] == NULL || State->IsTxQueue[ep] )
        return FALSE;
        
    // If endpoint is halted
    if (State->EndpointStatus[ep] != USBH_EP_OK) {
        return FALSE;
    }

    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);

    GLOBAL_LOCK(irq);
    
    // enable Rx
    UINT32 ch = ((STM32F4_USBH_STATE*)State)->epToChannel[ep];
    UINT32 chch = OTG->HC[ch].CHAR;
    if ((chch & (OTG_HCCHAR_CHDIS | OTG_HCCHAR_CHENA)) == 0) { // channel not in use
    
        USBH_Debug("e");
        // reset error count
        ((STM32F4_USBH_STATE*)State)->errorCount[ch] = 0;
        // enable channel
        // check queue space ???
        OTG->HC[ch].TSIZ = OTG->HC[ch].TSIZ & OTG_HCTSIZ_DPID
                           | OTG_HCTSIZ_PKTCNT_1 | State->MaxPacketSize[ep];
        if (chch & OTG_HCCHAR_EPTYP_P) { // periodic channel
            chch |= OTG_HCCHAR_ODDFRM; // set ODDFRM
            chch ^= (OTG->HFNUM & 1) << 29; // clear ODDFRM if next frame is even
        }
        OTG->HC[ch].CHAR = chch | OTG_HCCHAR_CHENA; // start IN poll
    }
    
    return TRUE;
}

void CPU_USBH_PortReset( USBH_CONTROLLER_STATE* State )
{
    STM32F4_USBH_STATE* state = (STM32F4_USBH_STATE*) State;
    
    // setup timer to start reset after 10ms
    state->timer.Abort();
    state->timer.InitializeForISR((HAL_CALLBACK_FPN)STM32F4_USBH_StartReset, State);
    state->timer.EnqueueDelta(10000);
}

// set bus speed and start reset signaling
void STM32F4_USBH_StartReset( STM32F4_USBH_STATE* State )
{
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->state.ControllerNum);
    
    // preserve interrupt and enable bits
    UINT32 hprt = OTG->HPRT & ~(OTG_HPRT_POCCHNG | OTG_HPRT_PENCHNG | OTG_HPRT_PCDET | OTG_HPRT_PENA);

    // configure bus speed based on pin states
    if ((hprt & OTG_HPRT_PSPD) == OTG_HPRT_PSPD_L) { // low speed (1.5MBit)
        USBH_Debug("L");
        OTG->HFIR = 6000; // frame interval (1ms @ 6MHz)
        OTG->HCFG = OTG->HCFG & ~OTG_HCFG_FSLSPCS | (OTG_HCFG_FSLSS | OTG_HCFG_FSLSPCS6);
    } else { // full speed (12MBit)
        OTG->HFIR = 48000; // frame interval (1ms @ 48MHz)
        OTG->HCFG = OTG->HCFG & ~OTG_HCFG_FSLSPCS | (OTG_HCFG_FSLSS | OTG_HCFG_FSLSPCS48);
    }
    
    USBH_Debug("T");

    // start bus reset signaling
    OTG->HPRT = hprt | OTG_HPRT_PRST;
    
    // setup timer to clear reset bit after 15ms
    State->timer.Abort();
    State->timer.InitializeForISR((HAL_CALLBACK_FPN)STM32F4_USBH_StopRes, State);
    State->timer.EnqueueDelta(15000);
}

// Stop reset or resume signaling
void STM32F4_USBH_StopRes( STM32F4_USBH_STATE* State )
{
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->state.ControllerNum);
    
    // clear reset/resume and preserve interrupt and enable bits
    OTG->HPRT &= ~(OTG_HPRT_POCCHNG | OTG_HPRT_PENCHNG | OTG_HPRT_PCDET
                   | OTG_HPRT_PENA | OTG_HPRT_PRST | OTG_HPRT_PRES);
}

void CPU_USBH_Suspend( USBH_CONTROLLER_STATE* State )
{
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);
    
    // preserve interrupt and enable bits
    UINT32 hprt = OTG->HPRT & ~(OTG_HPRT_POCCHNG | OTG_HPRT_PENCHNG | OTG_HPRT_PCDET | OTG_HPRT_PENA);
    OTG->HPRT = hprt | OTG_HPRT_PSUSP; // set suspend bit
    
    // suspend bit cleared automatically
    
    State->DeviceState = USB_DEVICE_STATE_SUSPENDED;
    USBH_StateCallback( State );
}

void CPU_USBH_Resume( USBH_CONTROLLER_STATE* State )
{
    STM32F4_USBH_STATE* state = (STM32F4_USBH_STATE*) State;
    OTG_TypeDef* OTG = STM32F4_USBH_REGS(State->ControllerNum);
    
    // preserve interrupt and enable bits
    UINT32 hprt = OTG->HPRT & ~(OTG_HPRT_POCCHNG | OTG_HPRT_PENCHNG | OTG_HPRT_PCDET | OTG_HPRT_PENA);
    OTG->HPRT = hprt | OTG_HPRT_PRES; // set resume bit
    
    // setup timer to clear resume bit after 20ms
    state->timer.Abort();
    state->timer.InitializeForISR((HAL_CALLBACK_FPN)STM32F4_USBH_StopRes, State);
    state->timer.EnqueueDelta(20000);

    State->DeviceState = USB_DEVICE_STATE_CONFIGURED;
    USBH_StateCallback( State );
}


BOOL CPU_USBH_ProtectPins( int Controller, BOOL On )
{
    STM32F4_USBH_STATE *State = &STM32F4_USBH_ControllerState[STM32F4_USBH_IDX(Controller)];

    if (!State->initialized) return FALSE;  // not yet initialized

    OTG_TypeDef* OTG = STM32F4_USBH_REGS(Controller);

    GLOBAL_LOCK(irq);

    if (On) {
        if (!State->pinsProtected) {

            USBH_Debug("+");

            State->pinsProtected = TRUE;

            // clear USB Txbuffer
            for(int ep = 1; ep < State->state.EndpointCount; ep++) {
                if (State->state.Queues[ep] && State->state.IsTxQueue[ep]) {
                    while (USBH_TxDequeue( &State->state, ep, TRUE ) != NULL);  // clear TX queue
                }
            }
        }
    } else {
        if (State->pinsProtected) {

            USBH_Debug("-");

            State->pinsProtected = FALSE;

            GPIO_ALT_MODE altMode = STM32F4_USBH_ALT_MODE(Controller);
            CPU_GPIO_DisablePin(STM32F4_USBH_DM_PIN(Controller), RESISTOR_DISABLED, 0, altMode);
            CPU_GPIO_DisablePin(STM32F4_USBH_DP_PIN(Controller), RESISTOR_DISABLED, 0, altMode);
            if (STM32F4_USBH_USE_ID_PIN(Controller)) {
                CPU_GPIO_DisablePin(STM32F4_USBH_ID_PIN(Controller), RESISTOR_DISABLED, 0, altMode);
            }
        }
    }

    return TRUE;
}
