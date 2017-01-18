#ifndef __POLLOS_H_
#define __POLLOS_H_

#include "rtp.h"

typedef struct pollos_signal
{
	#define CLEAR_THE_SIGNAL -1
	RTP_UINT32 	signal_timeout;
	int 		signal_count;
} POLLOS_SIGNAL;
typedef POLLOS_SIGNAL * P_POLLOS_SIGNAL;

#ifdef __cplusplus
extern "C" {
#endif

void 		poll_os_cycle(void);
void 		pollos_signal(P_POLLOS_SIGNAL psig);
RTP_BOOL 	pollos_test(P_POLLOS_SIGNAL psig, RTP_UINT32 timeout);
void 	 	pollos_clear_q(P_POLLOS_SIGNAL psig);
void 		pollos_sleep(RTP_UINT32 timeout);

#ifdef __cplusplus
}
#endif
#endif /* __POLLOS_H_ */
