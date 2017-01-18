////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

#include "xnconf.h"
#include "dhcpconf.h"
#include "rtip.h"
#include "rtipapi.h"

///////////////////////////////////////////////////////////////////////////////
// basic API functions

int default_mcast_iface;

//--//

void SOCKAPI_       proc_fd_clr(int socket, PFDSET fd) { }
int  SOCKAPI_       proc_fd_isset(int socket, PFDSET fd) { return 0; }
void SOCKAPI_       proc_fd_zero(PFDSET fd) { }


SOCKET SOCKAPI_     socket(int, int, int) { return 0; }
int  SOCKAPI_       accept(int, PSOCKADDR, RTP_PFINT) { return 0; }
int  SOCKAPI_       bind(int, PCSOCKADDR, int) { return 0; }
int  SOCKAPI_       closesocket(SOCKET socket) { return 0; }
int  SOCKAPI_       connect(SOCKET, PCSOCKADDR, int) { return 0; }
int  SOCKAPI_       ioctlsocket(SOCKET socket, long cmd, unsigned long RTP_FAR *argp) { return 0; }
int  SOCKAPI_       getpeername(SOCKET, PSOCKADDR, RTP_PFINT) { return 0; }
int  SOCKAPI_       getsockname(SOCKET, PSOCKADDR, RTP_PFINT) { return 0; }
int  SOCKAPI_       getsockopt(SOCKET socket, int level, int option_name,
                                RTP_PFCHAR option_value, RTP_PFINT optionlen) { return 0; }
int  SOCKAPI_       listen(SOCKET, int) { return 0; }
int  SOCKAPI_       recv(SOCKET, RTP_PFCHAR, int, int) { return 0; }
int  SOCKAPI_       recvfrom(SOCKET, RTP_PFCHAR, int, int, PSOCKADDR, RTP_PFINT) { return 0; }
int  SOCKAPI_       select(int fd, PFDSET fread, PFDSET fwrite, PFDSET fexception,
                     PCTIMEVAL timeout) { return 0; }
int  SOCKAPI_       sendto(SOCKET, RTP_PFCCHAR, int, int, PCSOCKADDR, int) { return 0; }

int  SOCKAPI_ setsockopt(SOCKET, int, int, RTP_PFCCHAR, int) { return 0; }
int  SOCKAPI_ shutdown(SOCKET, int) { return 0; }

unsigned long SOCKAPI_ inet_addr(RTP_PFCCHAR string) { return 0; }
RTP_PFCHAR        SOCKAPI_ inet_ntoa(struct in_addr in) { return 0; }

PFPROTOENT  SOCKAPI_ getprotobynumber(int proto) { return 0; }
PFPROTOENT  SOCKAPI_ getprotobyname(RTP_PFCHAR name) { return 0; }
PFSERVENT   SOCKAPI_ getservbyname(RTP_PFCHAR name, RTP_PFCHAR proto) { return 0; }
PFSERVENT   SOCKAPI_ getservbyport(int port, RTP_PFCHAR proto) { return 0; }

PFHOSTENT SOCKAPI_ gethostbyname(RTP_PFCHAR name) { return 0; }
PFHOSTENT SOCKAPI_ gethostbyaddr(RTP_PFCHAR addr, int len, int type) { return 0; }

int  SOCKAPI_ xn_clear_host_cache(void) { return 0; }
int    SOCKAPI_ xn_add_dns_server(RTP_PFCUINT8 addr) { return 0; }
int    SOCKAPI_ xn_set_server_list(RTP_UINT32 list[], int num_elements) { return 0; }
int    SOCKAPI_ xn_add_host_table_entry(RTP_PFCHAR name, RTP_UINT32 ipaddr) { return 0; }
int    SOCKAPI_ xn_delete_host_table_entry(RTP_PFCHAR name) { return 0; }
int    SOCKAPI_ xn_has_dns_name(void) { return 0; }

int        SOCKAPI_ xn_has_domain_name(void) { return 0; }
RTP_PFCHAR SOCKAPI_ xn_get_domain_name(void) { return 0; }
int    SOCKAPI_ xn_set_domain_name(RTP_PFCCHAR name) { return 0; }
int    SOCKAPI_ xn_name2ip_addr(RTP_PFUINT8 ip_addr, RTP_PFCCHAR name) { return 0; }


int         SOCKAPI_ xn_rtip_restart(void) { return 0; }
void        SOCKAPI_ xn_register_init_fnc(P_INIT_FNCS init_fnc, REGISTER_PROC fnc) {}
int         SOCKAPI_ xn_rtip_exit(void) { return 0; }
int         SOCKAPI_ xn_rarp(int iface_no) { return 0; }
int         SOCKAPI_ xn_bootp(int iface_no) { return 0; }

int         SOCKAPI_ _xn_ping(int sequence, int len, int ttl, RTP_PFUINT8 host,
                              int route_type, PROUTE_INFO route_info,
                              long wait_count,
                              long *elapsed_msec, RTP_BOOL send_802_2) { return 0; }


int         SOCKAPI_ set_ip(int iface_no, RTP_PFUINT8 local_ip_address, RTP_PFCUINT8 ip_mask, int added_by) { return 0; }
int         SOCKAPI_ xn_set_ip(int iface_no, RTP_PFUINT8 my_ip_address, RTP_PFCUINT8 ip_mask) { return 0; }
int         SOCKAPI_ xn_rt_add(RTP_PFCUINT8 net, RTP_PFCUINT8 mask, RTP_PFCUINT8 gw, 
                               RTP_UINT32 metric, int iface, int ttl) { return 0; }
int         SOCKAPI_ xn_rt_del(RTP_PFUINT8 net, RTP_PFCUINT8 mask) { return 0; }

void        SOCKAPI_ rt_del_iface(int iface_no, int added_by) { }

int         SOCKAPI_ xn_rt_cycle_gw(void) { return 0; }
int         SOCKAPI_ xn_abort(SOCKET socket, RTP_BOOL tcp_send_reset) { return 0; }

int         SOCKAPI_ xn_attach(int device_id, int minor_number, RTP_PFUINT8 his_ip_address,
                               int port_com, int baud_rate, char handshake_type,
                               RTP_PFUINT8 input_buffer, int input_buffer_len,
                               RTP_PFUINT8 output_buffer, int output_buffer_len) { return 0; }

int         SOCKAPI_ xn_interface_stats(int interface_no) { return 0; }

/* modem   */
int         SOCKAPI_ xn_scriptlogin(int iface_no, PAL_COMMAND script,
                                    int num_commands, RTP_PFCHAR term_str) { return 0; }
int         SOCKAPI_ xn_autologin(int iface_no, RTP_PFCHAR setupstr, RTP_PFCHAR phonenum,
                                  RTP_PFCHAR username, RTP_PFCHAR passwd, RTP_PFCHAR term_str) { return 0; }
int         SOCKAPI_ xn_autoanswer(int iface_no, RTP_PFCHAR setupstr, RTP_UINT16 timeout,
                                   RTP_PFCHAR term_str) { return 0; }
int         SOCKAPI_ xn_hangup(int iface_no, RTP_PFCHAR term_str) { return 0; }

int         SOCKAPI_ xn_interface_open_config(int device_id, int minor_number,
                                              IOADDRESS io_address,
                                              int irq_val,
                                              RTP_UINT16 mem_address) { return 0; }
RTP_BOOL     SOCKAPI_ xn_wait_tcp_output(int sec_wait) { return 0; }
int         SOCKAPI_ xn_interface_opt(int iface_no, int option_name,
                                      RTP_PFCCHAR option_value, int optionlen) { return 0; }
RTP_BOOL     SOCKAPI_ xn_tcp_is_write(SOCKET socket) { return 0; }
RTP_BOOL     SOCKAPI_ xn_tcp_is_read(SOCKET socket) { return 0; }
RTP_BOOL     SOCKAPI_ xn_tcp_is_connect(SOCKET socket) { return 0; }
int         SOCKAPI_ xn_getlasterror(void) { return 0; }
RTP_PFCCHAR     SOCKAPI_ xn_geterror_string(int errno_val) { return 0; }

int         SOCKAPI_ xn_ip_set_option(SOCKET socket, int option_name,
                                      RTP_PFCCHAR option_value, int optionlen) { return 0; }

int         SOCKAPI_ xn_arp_send(int interface_no) { return 0; }

int         SOCKAPI_ xn_arp_add(RTP_PFUINT8 ipn, RTP_PFUINT8 ethn, RTP_UINT32 time_to_live) { return 0; }
int         SOCKAPI_ xn_arp_del(RTP_PFUINT8 ipn) { return 0; }

int         SOCKAPI_ xn_pkt_data_max(SOCKET socket, RTP_PFUINT8 dest_ip_addr) { return 0; }

int         SOCKAPI_ xn_decode_base64(PBASE64_DECODE_CONTEXT pContext,RTP_PFUINT8 buffer, RTP_PFCHAR input, int input_length) { return 0; }
void        SOCKAPI_ xn_base64_decode_init(PBASE64_DECODE_CONTEXT pContext) {}

void        SOCKAPI_ xn_register_callbacks(PRTIP_CALLBACKS rcb) { }
PRTIP_CALLBACKS SOCKAPI_ xn_callbacks(void) { return 0; }

RTP_BOOL SOCKAPI_ wait_pkts_output(RTP_BOOL disable_output, long timeout) { return 0; }
int          SOCKAPI_ tc_interface_close(int iface_no) { return 0; }
int          SOCKAPI_ tc_interface_info(int iface_no, PIFACE_INFO pi_info) { return 0; }
RTP_BOOL SOCKAPI_ tc_interface_mcast(int iface_no, RTP_PFUINT8 mclist,
                               int nipaddrs, int add) { return 0; }
/* statistics routines   */

RTP_BOOL SOCKAPI_ xn_interface_statistics(int iface_no, PIFACE_STATS stats) { return 0; }

int         SOCKAPI_ xn_rtip_init(void) { return -1; }
int         SOCKAPI_ xn_device_table_add(int device_id,
                        int minor_number,
                        int iface_type,
                        RTP_PFCHAR device_name,
                        DEV_OPEN dev_open,
                        DEV_CLOSE dev_close,
                        DEV_XMIT dev_xmit,
                        DEV_XMIT_DONE dev_xmit_done,
                        DEV_PROCESS_INTERRUPTS dev_proc_interrupts,
                        DEV_STATS dev_stats,
                        DEV_SETMCAST dev_smcast) { return 0; }

// basic API functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// kernel functions

void	 	os_iface_exchange_bind(RTP_UINT16 sig_handle,  RTP_PFVOID pobject) { }
void        os_iface_exchange_send(RTP_UINT16 sig_handle,  RTP_PFVOID pobject, DCU msg) { }
DCU         os_iface_exchange_rcv(RTP_UINT16 sig_handle,   RTP_PFVOID pobject, long wait_count) { return 0; }
void        os_iface_exchange_clear(RTP_UINT16 sig_handle, RTP_PFVOID pobject) { }
void	 	os_port_exchange_bind(RTP_UINT16 sig_handle,  RTP_PFVOID pobject) { }
void        os_port_exchange_send(RTP_UINT16 sig_handle,  RTP_PFVOID pobject, DCU msg) { }
DCU         os_port_exchange_rcv(RTP_UINT16 sig_handle,   RTP_PFVOID pobject, long wait_count) { return 0; }
void        os_port_exchange_clear(RTP_UINT16 sig_handle, RTP_PFVOID pobject) { }

void        os_lock_task(void) { }
void        os_unlock_task(void) { }

void        os_set_sent_signal(PANYPORT port, RTP_BOOL success) { }
void        os_set_write_signal(PANYPORT port, RTP_BOOL success) { }
void        os_set_read_signal(PANYPORT port, RTP_BOOL success) { }
void        os_set_select_signal(PANYPORT port, int select_off) { }
DCU         os_rcvx_input_list(PIFACE pi) { return 0; }
void        os_sndx_input_list(PIFACE pi, DCU msg) { }

RTP_BOOL    os_memory_init(RTP_BOOL restart) { return 0; }
void        os_memory_free(void) { }
POS_LIST    os_list_add_front_off(POS_LIST head, POS_LIST entry, int offset) { return 0; }
POS_LIST    os_list_add_rear_off(POS_LIST head, POS_LIST entry, int offset) { return 0; }
POS_LIST    os_list_add_middle_off(POS_LIST head, POS_LIST entry,
                                   POS_LIST entry_prev, int offset) { return 0; }
POS_LIST    os_list_remove_off(POS_LIST head, POS_LIST entry, int offset) { return 0; }
POS_LIST    os_list_next_entry_off(POS_LIST head, POS_LIST entry, int offset) { return 0; }
POS_LIST    os_list_last_off(POS_LIST head, int offset) { return 0; }

void        os_track_allocation_stats_dump(void) { }

int         cnt_num_avail_pkts(int nbytes) { return 0; }
DCU         os_alloc_packet(int nbytes, int who) { return 0; }
DCU         os_alloc_packet_wait(int nbytes, PANYPORT port, int who) { return 0; }
DCU         os_alloc_packet_input(int nbytes, int who) { return 0; }
void        os_free_packet(DCU msg) { }
PUDPPORT    os_alloc_udpport(void) { return 0; }
void        os_free_udpport(PUDPPORT port) { }
PTCPPORT    os_alloc_tcpport(void) { return 0; }
void        os_free_tcpport(PTCPPORT port) { }
RTP_PFUINT8 os_alloc_tcpwind(void) { return 0; }
void        os_free_tcpwind(RTP_PFUINT8 wind) { }
void        os_clear_udpapp_exchg(PUDPPORT pport) { }
RTP_BOOL    os_udp_pkt_avail(PUDPPORT pport) { return 0; }
int         os_udp_first_pkt_size(PUDPPORT pport) { return 0; }
void        os_sndx_arpcache_list(int index, DCU msg) { }
DCU         os_rcvx_arpcache_list(int index) { return 0; }
RTP_BOOL    os_rmvx_arpcache_list(int index, DCU msg) { return 0; }

void         display_packet_lowwater(void) { }

/* File OSPORT.C   */
void        ks_invoke_input(PIFACE pi,DCU msg, int length) { }
void        ks_invoke_output(PIFACE pi, int xmit_complete_cnt) { }
void        ks_invoke_interrupt(PIFACE pi) { }
RTP_BOOL 	ks_resource_init(void) { return 0; }
void 		 os_exit_task(void) { }


/* File OSTASK.C   */
void        ks_kernel_timer_callback(void) { }

/* File OSENV.C   */
int         phys_to_virtual(RTP_PFUINT8 * virt, unsigned long phys) { return 0; }
RTP_UINT32  kvtop(RTP_PFUINT8 p) { return 0; }
void        ks_restore_interrupts(void) { }

// kernel functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// other functions
int  set_errno(int error) { return 0; }
void rtp_kern_init (void) { }
void rtp_kern_run (void) { }
void rtp_kern_abort (void) { }
void rtp_kern_exit  (int exitvalue) { }

RTP_IRQCNTXT rtp_irq_push_disable (void) { return 0; }
void         rtp_irq_pop          (RTP_IRQCNTXT previousLevel) { }
void         rtp_irq_disable      (void) { }
void         rtp_irq_enable       (void) { }
RTP_BOOL     rtp_irq_hook_interrupt(RTP_PFVOID piface, RTP_IRQ_FN_POINTER c_strategy, RTP_IRQ_FN_POINTER c_interrupt) { return 0; }

void tc_timer_main(void) { }
void tc_ip_process(int * iface_no) { }
void        tc_interrupt_process(PIFACE pi) { }
PIFACE      tc_get_local_pi(RTP_PFUINT8 ip_addr) { return 0; }

PANYPORT    api_sock_to_port(SOCKET socket_index) { return 0; }

// other functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// globals 

#ifdef __cplusplus
RTP_EXTERN "C" {
#endif

RTP_CUINT8 RTP_FAR ip_ffaddr[IP_ALEN] = { 0, 0, 0, 0};
RTP_CUINT8 RTP_FAR ip_nulladdr[IP_ALEN] = { 0, 0, 0, 0};
PIFACE        ifaces;

#ifdef __cplusplus
}
#endif



// globals
///////////////////////////////////////////////////////////////////////////////

void    	bpool   (RTP_PFVOID buffer, bufsize len) { }
RTP_PFVOID  bget    (bufsize size) { return 0; }
RTP_PFVOID  bgetz   (bufsize size) { return 0; }
RTP_PFVOID  bgetr   (RTP_PFVOID buffer, bufsize newsize) { return 0; }
void    	brel    (RTP_PFVOID buf) { }
void    	bectl   (int (*compact)(bufsize sizereq, int sequence),
                	RTP_PFVOID (*acquire)(bufsize size),
                	void (*release)(RTP_PFVOID buf), bufsize pool_incr) { }
void    bstats  	(bufsize *curalloc, bufsize *totfree, bufsize *maxfree,
                	long *nget, long *nrel) { }
void    bstatse 	(bufsize *pool_incr, long *npool, long *npget,
                	long *nprel, long *ndget, long *ndrel) { }
void    bufdump 	(RTP_PFVOID buf) { }
void    bpoold  	(RTP_PFVOID pool, int dumpalloc, int dumpfree) { }
int     bpoolv  	(RTP_PFVOID pool) { return 0; }


/************************************************************************
 * Non-ANSI C Standard library utility function declarations            *
 ************************************************************************/

long          rtp_memcat   (char *dst, long offset, const char *src, long size ) { return 0; }

int           rtp_stricmp  (const char *, const char * ) { return 0; }
char *        rtp_stristr  (char *, const char * ) { return 0; }
int           rtp_strncmp  (const char *, const char *, unsigned int ) { return 0; }
int           rtp_strnicmp (const char *, const char *, unsigned int ) { return 0; }

/************************************************************************
 * ANSI C Standard library utility function declarations (if needed)    *
 ************************************************************************/

void *        rtp_memchr   (const void * str, int chr, unsigned int n ) { return 0; }
int           rtp_memcmp   (const void * a, const void * b, unsigned int n ) { return 0; }
void *        rtp_memcpy   (void *a, const void *b, unsigned int n ) { return 0; }
void *        rtp_memmove  (void * a, const void * b, unsigned int n ) { return 0; }
void *        rtp_memset   (void * p, int b, unsigned int n ) { return 0; }
char *        rtp_strcat   (char * a, const char * b ) { return 0; }
char *        rtp_strncat  (char * a, const char * b, int n ) { return 0; }
char *        rtp_strchr   (const char * str, int find_chr ) { return 0; }
int           rtp_strcmp   (const char * s1, const char * s2 ) { return 0; }
char *        rtp_strcpy   (char * a, const char * b ) { return 0; }
unsigned int  rtp_strlen   (const char * string ) { return 0; }
char *        rtp_strncpy  (char * a, const char * b, unsigned int n ) { return 0; }
char *        rtp_strrchr  (const char * str, int find_chr ) { return 0; }
char *        rtp_strstr   (const char * str, const char * find_str ) { return 0; }
unsigned long rtp_strspn   (const char *str, const char *chrs ) { return 0; }
unsigned long rtp_strcspn  (const char *str, const char *chrs ) { return 0; }
char *        rtp_strpbrk  (char * str, const char * find_chrs ) { return 0; }
const char *  rtp_strtok   (const char *str, const char *tok ) { return 0; }
char *        rtp_itoa     (int, char *, int) { return 0; }
char *        rtp_ltoa     (long, char *, int) { return 0; }
char *        rtp_ultoa    (unsigned long, char *, int) { return 0; }
long          rtp_strtol   (const char * str, char ** delimiter, int base) { return 0; }
unsigned long rtp_strtoul  (const char * str, char ** delimiter, int base) { return 0; }


#ifdef __cplusplus
extern "C" {
#endif
void * rtp_malloc  (unsigned long size){ return NULL; }
void * rtp_calloc  (unsigned long num, unsigned long size){ return NULL; }
void * rtp_realloc (void * ptr, unsigned long size){ return NULL; }
void   rtp_free    (void * ptr){}
 
#ifdef __cplusplus
}
#endif

void        bget_init(void) { }

CFG_DNS_DATA        RTP_FAR cfg_dns_data;
CFG_IFACE_DATA      RTP_FAR cfg_iface_data;
CFG_IGMP_DATA       RTP_FAR cfg_igmp_data;
CFG_IP_DATA         RTP_FAR cfg_ip_data;
CFG_PACKET_DATA     RTP_FAR cfg_packet_data;
CFG_PROTOCOL_DATA   RTP_FAR cfg_protocol_data;



