/////////////////////////////////////////////////
//      Generated file Do Not Modify
//			FileName: default_user_config.h
/////////////////////////////////////////////////


#define _LWIPOPTS_H_

#ifndef  _default_user_config_H_
#define _default_user_config_H_

 #define  NO_SYS   0

 #ifndef MEM_ALIGNMENT
 #define  MEM_ALIGNMENT   4
 #endif

 #ifndef MEM_SIZE
 #define  MEM_SIZE   65536
 #endif

 #ifndef MEMP_NUM_PBUF
 #define  MEMP_NUM_PBUF   32
 #endif

 #ifndef MEMP_NUM_UDP_PCB
 #define  MEMP_NUM_UDP_PCB   8
 #endif

 #ifndef MEMP_NUM_TCP_PCB
 #define  MEMP_NUM_TCP_PCB   16
 #endif

 #ifndef MEMP_NUM_TCP_PCB_LISTEN
 #define  MEMP_NUM_TCP_PCB_LISTEN   8
 #endif

 #ifndef MEMP_NUM_TCP_SEG
 #define  MEMP_NUM_TCP_SEG   32
 #endif

 #ifndef MEMP_NUM_SYS_TIMEOUT
 #define  MEMP_NUM_SYS_TIMEOUT   12
 #endif

 #ifndef MEMP_NUM_NETBUF
 #define  MEMP_NUM_NETBUF   20
 #endif

 #ifndef MEMP_NUM_NETCONN
 #define  MEMP_NUM_NETCONN   24
 #endif

 #ifndef MEMP_NUM_API_MSG
 #define  MEMP_NUM_API_MSG   40
 #endif

 #ifndef MEMP_NUM_TCPIP_MSG
 #define  MEMP_NUM_TCPIP_MSG   32
 #endif

 #define  MEM_RECLAIM   1

 #define  MEMP_RECLAIM   1

 #ifndef PBUF_POOL_SIZE
 #define  PBUF_POOL_SIZE   128
 #endif

 #ifndef PBUF_POOL_BUFSIZE
 #define  PBUF_POOL_BUFSIZE   512
 #endif

 #ifndef PBUF_LINK_HLEN
 #define  PBUF_LINK_HLEN   16
 #endif

 #define  LWIP_DHCP   1

 #define  DHCP_DOES_ARP_CHECK   1

 #define  LWIP_STATS   1

 #define  IP_STATS   1

 #define  ICMP_STATS   1

 #define  UDP_STATS   1

 #define  TCP_STATS   1

 #define  LWIP_TCP   1

 #ifndef TCP_TTL
 #define  TCP_TTL   255
 #endif

 #define  TCP_QUEUE_OOSEQ   1

 #ifndef TCP_MSS
 #define  TCP_MSS   1460
 #endif

 #ifndef TCP_SND_BUFF
 #define  TCP_SND_BUFF   5840
 #endif

 #ifndef TCP_SND_QUEUELEN
 #define  TCP_SND_QUEUELEN   16
 #endif

 #ifndef TCP_WND
 #define  TCP_WND   8192
 #endif

 #ifndef TCP_MAXRTX
 #define  TCP_MAXRTX   12
 #endif

 #ifndef TCP_SYNMAXRTX
 #define  TCP_SYNMAXRTX   4
 #endif

 #ifndef TCP_SNDLOWAT
 #define  TCP_SNDLOWAT   2920
 #endif

 #ifndef ARP_TABLE_SIZE
 #define  ARP_TABLE_SIZE   10
 #endif

 #define  ARP_QUEUEING   1

 #define  ETHARP_ALWAYS_INSERT   1

 #define  IP_FORWARD   1

 #define  IP_OPTIONS   1

 #define  IP_REASSEMBLY   1

 #define  IP_FRAG   1

 #define  ICMP_TTL   255

 #define  LWIP_UDP   1

 #define  UDP_TTL   255

 #define  SYS_LEIGHTWEIGHT_PROTO   1

 #define  LWIP_COMPAT_SOCKETS   1

 #define  LWIP_PROVIDE_ERRNO   1

 #ifndef TCPIP_THREAD_PRIO
 #define  TCPIP_THREAD_PRIO   5
 #endif

 #ifndef DEFAULT_THREAD_PRIO
 #define  DEFAULT_THREAD_PRIO   10
 #endif

 #ifndef LOW_THREAD_PRIO
 #define  LOW_THREAD_PRIO   29
 #endif

 #endif
