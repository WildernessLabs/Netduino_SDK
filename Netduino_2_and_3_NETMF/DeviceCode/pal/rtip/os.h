/*                                                                      */
/* OS.H - defines for layer which interfaces with the porting layer     */
/*                                                                      */
/* EBS - RTIP                                                           */
/*                                                                      */
/* Copyright Peter Van Oudenaren , 1993                                 */
/* All rights reserved.                                                 */
/* This code may not be redistributed in source or linkable object form */
/* without the consent of its author.                                   */
/*                                                                      */
/*  Module description:                                                 */
/*                                                                      */
/*  This module consists of all the macros which interface to the       */
/*      operating systems porting layer.                                */

#ifndef __OS__
#define __OS__ 1

#include "rtpsignl.h"
#include "xnconf.h"

/* ********************************************************************   */
/*  [tbd] find some place to put these                                    */
#define KS_NUM_INTS       16
#define INCLUDE_TASK_ISRS 1

/* ********************************************************************   */
#if (0)
#define RTIP_INF          0xffffffff      /* infinite wait count */ 
#endif
#define RTIP_INF          0xffff          /* infinite wait count */
#define KS_INF            RTIP_INF

/* ********************************************************************   */
/* macro to access OS_LIST in entry which is at offset instead of         */
/* at the beginning of the structures (the offset are 0 for the first,    */
/* 1 for the second, etc)                                                 */
/* NOTE: offsets are 0, 1, 2 etc                                          */
#define POS_ENTRY_OFF(entry, offset) ((POS_LIST)entry + offset)

/* ********************************************************************   */
/* SIGNALS                                                                */
/* Define all signals bound to an interface (iface) - signal index
    0 = IP     - signalled when data packet sent to IP exchange
    1 = RARP   - signalled when rarp response has been received (and processed)
    2 = OUTPUT - signalled when output is complete
    3 = INTERRUPT - signalled when interrupt needs processing (processing
                    done at task layer instead of interrupt layer)
    4 = PPP    - signalled when PPP connection has become established (xn_lcp_open) */
#define IF_SIG_IP     		(0)
#define IF_SIG_RARP   		(1)
#define IF_SIG_INTERRUPT  	(2)
#define IF_SIG_PPP    		(3)

#define NUM_SIG_PER_IFACE    		(3+(INCLUDE_TASK_ISRS || INCLUDE_PPP))

/* Signal and exchange encoding 
    BIT NUMBERS         PURPOSE
    -----------			-------
    0-3 (0x000x)        Which signal or semaphore to use
    4-7 (0x00x0)        Which exchange to use 
    8   (0x0100)        Object is an iface structure
    9   (0x0200)        Object is a port structure
*/

// defines for the above encoding scheme
#define OS_HNDL_TO_SIGNAL(X) (RTP_UINT16) ((X) & 0x0f)        /* Signal index */
#define OS_HNDL_TO_EXCH(X)   (RTP_UINT16) ((((X) & 0xf0)>>4)) /* exchange index */

/* Define all signals bound to a port (socket) - signal index
    0  = READ   - signalled when data available for TCP. Also signaled when
                  UDP packet sent to input exchange 
    1  = WRITE  - signalled when data sent by TCP has been acked 
    2  = SENT   - if requested, ARP signals the port when a packet which 
                  was queued for address resolution is sent
    3+x = SELECT - If the port is in a socket select group on of the port's
                  select signal will be used to signal completion 
                  NOTE: x ranges from 0 to CFG_NUM_SELECT_P_SOCK-1
    3+CFG_NUM_SELECT_P_SOCK  = 
         DCU    - signalled when DCU is freed 
                  NOTE: only used if INCLUDE_NO_DCU_BLOCK
*/
/* NOTE: KS_PORT_SIGNAL_SELECT needs to be last   */
#define PO_SIG_READ   0
#define PO_SIG_WRITE  1
#define PO_SIG_PING   1
#define PO_SIG_SENT   2
#define PO_SIG_SELECT 3
#define PO_SIG_DCU    (3+CFG_NUM_SELECT_P_SOCK)

#define PO_EX_UDP  (0x00|PO_SIG_READ)
#define PO_EX_PING (0x00|PO_SIG_PING)

#define NUM_SIG_PER_PORT        (3+INCLUDE_NO_DCU_BLOCK+CFG_NUM_SELECT_P_SOCK)

/* ********************************************************************   */
RTP_EXTERN RTP_MUTEX tablesem;
RTP_EXTERN RTP_MUTEX udpsem;
RTP_EXTERN RTP_MUTEX tcpsem;
RTP_EXTERN RTP_MUTEX criticalsem;

#if (INCLUDE_WEB_BROWSER && defined(PEGRTIP))
/* TBDTBD - Move this later   */
#define MAX_GUI_TASKS         4     /* three demo tasks + PegTask */
#define NUM_PEG_EVENTS (MAX_GUI_TASKS + 1)
#define PEG_EV_MSGFREE 0
#define PEG_EV_CONSOLE MAX_GUI_TASKS

#define PEG_RES_MSG     0
#define PEG_RES_TMR     1
#define PEG_RES_SCREEN  2
#define NUM_PEG_SEM 3

#ifdef __cplusplus
RTP_EXTERN "C" {
#endif
RTP_EXTERN RTP_SEMAPHORE ks_peg_sig[NUM_PEG_EVENTS];
RTP_EXTERN RTP_MUTEX     ks_peg_sem[NUM_PEG_SEM];
#ifdef __cplusplus
}
#endif
#endif		/* WEB BROWSER && PEG */

/* ********************************************************************   */
/* SEMAPHORES                                                             */
/* ********************************************************************   */
/* called by rest of rtip   */
#if (!INCLUDE_TRK_PKTS || POLLOS)
#define OS_CLAIM_TABLE(X)            (rtp_sig_mutex_claim(tablesem)==0)?1:0;
#define OS_RELEASE_TABLE()           rtp_sig_mutex_release(tablesem);
#define OS_CLAIM_UDP(X)              (rtp_sig_mutex_claim(udpsem)==0)?1:0;
#define OS_RELEASE_UDP()             rtp_sig_mutex_release(udpsem);
#define OS_CLAIM_TCP(X)              (rtp_sig_mutex_claim(tcpsem)==0)?1:0;
#define OS_RELEASE_TCP()             rtp_sig_mutex_release(tcpsem);
#define OS_CLAIM_IFACE(PI,X)         (rtp_sig_mutex_claim(PI->ctrl.ifsem)==0)?1:0;
#define OS_RELEASE_IFACE(PI)         rtp_sig_mutex_release(PI->ctrl.ifsem);
#define OS_RTIP_ENTER_CRITICAL(X)    (rtp_sig_mutex_claim(criticalsem)==0)?1:0;
#define OS_RTIP_EXIT_CRITICAL()      rtp_sig_mutex_release(criticalsem);

#else

/* parameters for sem_no   */
#define TABLE_CLAIM     0
#define UDP_CLAIM       1
#define TCP_CLAIM       2
#define CRIT_CLAIM      3
#define MEMFILE_CLAIM   4	// in vfile.h
#define IFACE_CLAIM     5   /* PUT IFACE LAST SINCE MIGHT BE MORE THAN ONE */

void track_sem_pend(int who, int sem_no, int offset);
void track_sem_claim(int who, int sem_no, int offset);
void track_sem_rel(int sem_no, int offset);
#define OS_CLAIM_TABLE(X)            {track_sem_pend(X, TABLE_CLAIM, 0);    \
                                      (rtp_sig_mutex_claim(tablesem)==0)?1:0;        \
                                      track_sem_claim(X, TABLE_CLAIM, 0);}
#define OS_RELEASE_TABLE()           {track_sem_rel(TABLE_CLAIM, 0);        \
                                      rtp_sig_mutex_release(tablesem);}
#define OS_CLAIM_UDP(X)              {track_sem_pend(X, UDP_CLAIM, 0);     \
                                      (rtp_sig_mutex_claim(udpsem)==0)?1:0;         \
                                      track_sem_claim(X, UDP_CLAIM, 0);}
#define OS_RELEASE_UDP()             {track_sem_rel(UDP_CLAIM, 0);            \
                                      rtp_sig_mutex_release(udpsem);}
#define OS_CLAIM_TCP(X)              {track_sem_pend(X, TCP_CLAIM, 0);        \
                                      (rtp_sig_mutex_claim(tcpsem)==0)?1:0;            \
                                      track_sem_claim(X, TCP_CLAIM, 0);}
#define OS_RELEASE_TCP()             {track_sem_rel(TCP_CLAIM, 0);            \
                                      rtp_sig_mutex_release(tcpsem);}

#define OS_CLAIM_IFACE(PI,X)         {track_sem_pend(X, IFACE_CLAIM, PI->ctrl.index);      \
                                      (rtp_sig_mutex_claim(PI->ctrl.ifsem)==0)?1:0;    \
                                      track_sem_claim(X, IFACE_CLAIM, PI->ctrl.index);}
#define OS_RELEASE_IFACE(PI)         {track_sem_rel(IFACE_CLAIM, PI->ctrl.index);          \
                                      rtp_sig_mutex_release(PI->ctrl.ifsem);}
#define OS_RTIP_ENTER_CRITICAL(X)        {track_sem_pend(X, CRIT_CLAIM, 0);       \
                                      (rtp_sig_mutex_claim(criticalsem)==0)?1:0;       \
                                      track_sem_claim(X, CRIT_CLAIM, 0);}
#define OS_RTIP_EXIT_CRITICAL()              {track_sem_rel(CRIT_CLAIM, 0);           \
                                      rtp_sig_mutex_release(criticalsem);}
#endif /* INCLUDE_TRK_PKTS || POLLOS */

/* ********************************************************************   */
/* SIGNALS                                                                */
/* ********************************************************************   */

#define IFSIG(PI, SIG_NO) (PI)->ctrl.ifsig[(SIG_NO)]

/* map iface signals to kernel specific   */
#define OS_IFACE_SIGNAL_BIND(SIGNAL, PI)
#define OS_IFACE_SIGNAL_SET(SIGNAL, PI)                                     \
        rtp_sig_semaphore_signal(IFSIG((PI), SIGNAL))
#define OS_IFACE_SIGNAL_CLEAR(SIGNAL, PI)                                   \
        rtp_sig_semaphore_clear(IFSIG((PI), SIGNAL))
#define OS_IFACE_SIGNAL_TEST(SIGNAL, PI, WAIT_COUNT)                        \
        (rtp_sig_semaphore_wait_timed(IFSIG((PI), SIGNAL), (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0

/* map port signals to kernel specific   */
#define OS_PORT_SIGNAL_BIND(SIGNAL, PORT)
#define OS_PORT_SIGNAL_SET(SIGNAL, PORT)              \
        rtp_sig_semaphore_signal(((PANYPORT) PORT)->portsig[SIGNAL])
#define OS_PORT_SIGNAL_CLEAR(SIGNAL, PORT)            \
        rtp_sig_semaphore_clear(((PANYPORT) PORT)->portsig[SIGNAL])
#define OS_PORT_SIGNAL_TEST(SIGNAL, PORT, WAIT_COUNT) \
        (rtp_sig_semaphore_wait_timed(((PANYPORT) PORT)->portsig[SIGNAL], (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0

/* ********************************************************************   */
#define OS_TEST_RARP_SIGNAL(PI,WAIT_COUNT)  \
	(rtp_sig_semaphore_wait_timed(IFSIG((PI), IF_SIG_RARP), (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0

#define OS_BIND_RARP_SIGNAL(PI)          
#define OS_CLEAR_RARP_SIGNAL(PI)            \
    rtp_sig_semaphore_clear(IFSIG((PI), IF_SIG_RARP))
#define OS_SET_RARP_SIGNAL(PI)              \
	rtp_sig_semaphore_signal(IFSIG((PI), IF_SIG_RARP))

#define OS_TEST_INTERRUPT_SIGNAL(PI, WAIT_COUNT)    \
	(rtp_sig_semaphore_wait_timed(IFSIG((PI), IF_SIG_INTERRUPT), (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0

#define OS_BIND_INTERRUPT_SIGNAL(PI)                
#define OS_CLEAR_INTERRUPT_SIGNAL(PI)               \
    rtp_sig_semaphore_clear(IFSIG((PI), IF_SIG_INTERRUPT))
#define OS_SET_INTERRUPT_SIGNAL(PI)             \
	rtp_sig_semaphore_signal(IFSIG((PI), IF_SIG_INTERRUPT))

#define OS_TEST_IP_SIGNAL(PI, WAIT_COUNT)   \
    (rtp_sig_semaphore_wait_timed(IFSIG((PI), IF_SIG_IP), (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0
#define OS_BIND_IP_SIGNAL(PI)               
#define OS_CLEAR_IP_SIGNAL(PI)              \
	rtp_sig_semaphore_clear(IFSIG((PI), IF_SIG_IP))
#define OS_SET_IP_SIGNAL(PI)                \
	rtp_sig_semaphore_signal(IFSIG((PI), IF_SIG_IP))

#if (INCLUDE_PPP)
#define OS_TEST_PPP_SIGNAL(PI, WAIT_COUNT)  \
    (rtp_sig_semaphore_wait_timed(IFSIG((PI), IF_SIG_PPP), (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0
#define OS_BIND_PPP_SIGNAL(PI)  
#define OS_CLEAR_PPP_SIGNAL(PI) \
    rtp_sig_semaphore_clear(IFSIG((PI), IF_SIG_PPP))
#define OS_SET_PPP_SIGNAL(PI)   \
	rtp_sig_semaphore_signal(IFSIG((PI), IF_SIG_PPP))
#endif

// PORT SIGNALS
#define OS_TEST_SENT_SIGNAL(PORT, WAIT_COUNT)   \
    (rtp_sig_semaphore_wait_timed(((PANYPORT) PORT)->portsig[PO_SIG_SENT], (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0
#define OS_BIND_SENT_SIGNAL(PORT)  
#define OS_CLEAR_SENT_SIGNAL(PORT)  \
	rtp_sig_semaphore_clear(((PANYPORT) PORT)->portsig[PO_SIG_SENT])

#define OS_TEST_WRITE_SIGNAL(PORT, WAIT_COUNT)  \
    (rtp_sig_semaphore_wait_timed(((PANYPORT) PORT)->portsig[PO_SIG_WRITE], (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0
#define OS_BIND_WRITE_SIGNAL(PORT)             
#define OS_CLEAR_WRITE_SIGNAL(PORT)             \
	rtp_sig_semaphore_clear(((PANYPORT) PORT)->portsig[PO_SIG_WRITE])

#define OS_TEST_READ_SIGNAL(PORT, WAIT_COUNT)   \
    (rtp_sig_semaphore_wait_timed(((PANYPORT) PORT)->portsig[PO_SIG_READ], (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0
#define OS_BIND_READ_SIGNAL(PORT)             
#define OS_CLEAR_READ_SIGNAL(PORT)              \
	rtp_sig_semaphore_clear(((PANYPORT) PORT)->portsig[PO_SIG_READ])

#define OS_TEST_SELECT_SIGNAL(PORT, i, WAIT_COUNT)  \
    (rtp_sig_semaphore_wait_timed(((PANYPORT) PORT)->portsig[PO_SIG_SELECT+i], (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:(1000*(WAIT_COUNT/rtp_get_ticks_p_sec())))==0)?1:0
#define OS_BIND_SELECT_SIGNAL(PORT, i)              
#define OS_CLEAR_SELECT_SIGNAL(PORT, i)         \
	rtp_sig_semaphore_clear(((PANYPORT) PORT)->portsig[PO_SIG_SELECT+i])
#if (INCLUDE_NO_DCU_BLOCK)
#define OS_TEST_DCU_SIGNAL(PORT, WAIT_COUNT)    \
    (rtp_sig_semaphore_wait_timed(((PANYPORT) PORT)->portsig[PO_SIG_DCU], (WAIT_COUNT==RTP_TIMEOUT_INFINITE)?-1:WAIT_COUNT)==0)?1:0
#define OS_BIND_DCU_SIGNAL(PORT)                
#define OS_CLEAR_DCU_SIGNAL(PORT)               \
	rtp_sig_semaphore_clear(((PANYPORT) PORT)->portsig[PO_SIG_DCU])
#define OS_SET_DCU_SIGNAL(PORT)                 \
	rtp_sig_semaphore_signal(((PANYPORT) PORT)->portsig[PO_SIG_DCU])
#endif  /* INCLUDE_NO_DCU_BLOCK */

/* ********************************************************************   */
/* EXCHANGES                                                              */
/* ********************************************************************   */
/* Note: at this point exchanges are arrays of 1 element so offset        */
/*       0 is always used                                                 */
#define EXCHANGE_OFF 0

/* IP EXCHANGE   */
#define OS_CLEAR_IP_EXCHG(PI)                               \
    os_iface_exchange_clear(IF_SIG_IP, (RTP_PFVOID) (PI))               
#define OS_SNDX_IP_EXCHG(PI, MSG)                           \
    os_iface_exchange_send(IF_SIG_IP, (RTP_PFVOID) (PI), (MSG));        

/* Note: these procedures are functions in os_.c not macros...
**  void os_clear_udpapp_exchg(PUDPPORT port) 
**  void os_sndx_udpapp_exchg(PUDPPORT port, DCU msg)
**  DCU os_rcvx_udpapp_exchg(PUDPPORT pport, RTP_PFUINT16 wait_count)
*/

#define OS_BIND_UDPAPP_EXCHG(PORT)  \
    os_port_exchange_bind(PO_SIG_READ, (RTP_PFVOID) (PORT))

/* PING EXCHANGE   */
#define OS_CLEAR_PING_EXCHG(PORT)                   \
    os_port_exchange_clear(PO_SIG_PING, (RTP_PFVOID) (PORT));   
#define OS_BIND_PING_EXCHG(PORT)                    \
    os_port_exchange_bind(PO_SIG_PING, (RTP_PFVOID) (PORT))
#define OS_SNDX_PING_EXCHG(PORT, MSG)                   \
    os_port_exchange_send(PO_SIG_PING, (RTP_PFVOID) (PORT), (MSG));    
#define OS_RCVX_PING_EXCHG(PORT, WAIT_COUNT)                \
    os_port_exchange_rcv(PO_SIG_PING, (RTP_PFVOID) (PORT), (WAIT_COUNT));   

#endif /* __OS__ */

