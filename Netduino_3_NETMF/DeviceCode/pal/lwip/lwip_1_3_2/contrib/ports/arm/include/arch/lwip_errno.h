#ifndef __ERRNO_H_
#define __ERRNO_H_
#if defined(__RENESAS__)
volatile int errno;
#else
int errno;
#endif
int GetSocketError(void);
#endif /* __ERRNO_H_ */
