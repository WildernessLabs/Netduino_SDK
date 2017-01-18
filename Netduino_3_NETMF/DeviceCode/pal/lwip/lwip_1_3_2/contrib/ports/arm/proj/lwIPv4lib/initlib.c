//
// initlib.c
//
// An example initialisation function for the lwIP library.
//
// It calls initialisation functions for selected submodules, including the
// system-specific layer and the network interface layer.
//
// After calling this function you should use netif_add() to initialise
// your actual network interface(s) and netif_set_default() to set the
// default interface.
//
#include "lwip\sys.h"
#include "lwip\mem.h"
#include "lwip\memp.h"
#include "lwip\pbuf.h"
#include "lwip\udp.h"
#include "lwip\tcp.h"
#include "lwip\tcpip.h"
#include "lwip\netif.h"
#include "lwip\stats.h"
#include "lwip\raw.h"

#include "netif\etharp.h"

#include "initlib.h"
#include "lwipopts.h"

static void
tcpip_init_done(void *arg)
{
  sys_sem_t *sem;
  sem = arg;
  sys_sem_signal(*sem);
}

void lwip_init(u8_t* lwip_memory){
  sys_sem_t sem;

	extern u8_t** ADI_TOOLS_ram_ptr;
	extern u32_t  ADI_TOOLS_ram_len;
	extern u8_t** ADI_TOOLS_memp_ptr;
	extern u32_t  ADI_TOOLS_memp_len;
        extern u8_t** ADI_TOOLS_pbuf_pool_ptr;
        extern u32_t  ADI_TOOLS_pbuf_pool_len;
	
	// caller must ensure that the area of memory supplied as 'lwip_memory'
	// is at least as long as the sum of the 'len' values
	*ADI_TOOLS_ram_ptr       = lwip_memory;
	*ADI_TOOLS_memp_ptr      = lwip_memory
                                     + ADI_TOOLS_ram_len;
        *ADI_TOOLS_pbuf_pool_ptr = lwip_memory 
                                     + ADI_TOOLS_ram_len
                                     + ADI_TOOLS_memp_len;
	
  stats_init();
  sys_init();
  mem_init();
  memp_init();
  pbuf_init();
  netif_init();
  
  sem = sys_sem_new(0);
  tcpip_init(tcpip_init_done, &sem);
  sys_sem_wait(sem);
  sys_sem_free(sem);
  
#if LWIP_SOCKET
  lwip_socket_init();
#endif /* LWIP_SOCKET */

  ip_init();
#if LWIP_ARP
  etharp_init();
#endif /* LWIP_ARP */

#if LWIP_RAW
  raw_init();
#endif /* LWIP_RAW */

#if LWIP_UDP
  udp_init();
#endif /* LWIP_UDP */

#if LWIP_TCP
  tcp_init();
#endif /* LWIP_TCP */

#if LWIP_AUTOIP
  autoip_init();
#endif /* LWIP_AUTOIP */

#if LWIP_IGMP
  igmp_init();
#endif /* LWIP_IGMP */

#if LWIP_DNS
  dns_init();
#endif /* LWIP_DNS */
}

