////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "tinyhal.h"
#include "mem.h"
#include "sys.h"
#include "tcpip.h"

//--//

#ifdef __cplusplus
extern "C" {
#endif

//--//

//--//
//--// System Initailization, call before anything else
//--//

sys_timeouts timeout_list;

//--//

void sys_init(void)
{
    
    timeout_list.next = NULL;
}

//--// 
//--// Timeout functions
//--//
struct sys_timeouts *sys_arch_timeouts(void)
{
    return &timeout_list;
}

//--//
//--// Semaphore functions. 
//--//

void AcquireSemaphore(volatile UINT32* semaphore)
{
    *semaphore = *semaphore - 1;
}

void ReleaseSemaphore(volatile UINT32* semaphore)
{
    *semaphore = *semaphore + 1;
}

bool IsSemaphoreGreen(volatile UINT32* semaphore)
{
    return *semaphore != 0;
}

sys_sem_t sys_sem_new(u8_t count)
{
    //return dummy_semaphore;
    volatile UINT32* semaphore = (volatile UINT32*)mem_malloc(sizeof(UINT32));
    *semaphore = count;

    return (sys_sem_t)semaphore;
}


void sys_sem_signal(sys_sem_t sem)
{
    volatile UINT32* semaphore = (volatile UINT32*)sem;
    ReleaseSemaphore(semaphore);

    if(IsSemaphoreGreen(semaphore))
    {
        Events_Set(SYSTEM_EVENT_FLAG_NETWORK);
    }
}

u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{
    volatile UINT32* semaphore = (volatile UINT32*)sem;

    if(timeout != 0) 
    {
        INT64 now = ::HAL_Time_CurrentTime();
        INT64 elapsed = now + (timeout * 10000);
        
        while(elapsed > ::HAL_Time_CurrentTime()) 
        {
            if(IsSemaphoreGreen(semaphore)) 
            {
                AcquireSemaphore(semaphore); 
                break;
            }

            if(INTERRUPTS_ENABLED_STATE())
            {
                if(Events_WaitForEvents(SYSTEM_EVENT_FLAG_NETWORK, timeout))
                {
                    Events_Clear(SYSTEM_EVENT_FLAG_NETWORK);
                }
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        while(1) 
        {
            // wait and call the continuation for tcpip_thread
            if(IsSemaphoreGreen(semaphore)) 
            {
                AcquireSemaphore(semaphore);
                break;
            }
            
            if(INTERRUPTS_ENABLED_STATE())
            {
                Events_WaitForEvents(SYSTEM_EVENT_FLAG_NETWORK, EVENTS_TIMEOUT_INFINITE);
                Events_Clear(SYSTEM_EVENT_FLAG_NETWORK);
            }
            else
            {
                break;
            }
        }
    }
    
    return *semaphore;
}

void sys_sem_free(sys_sem_t sem)
{
    volatile UINT32* semaphore = (volatile UINT32*)sem;

    *semaphore = 0;
    
    mem_free(sem);
}

//--//
//--// Time jiffies (since power up) 
//--//
u32_t sys_jiffies(void)
{
    // IMPLEMENTED AS sys_now
    UINT64 currentTime = HAL_Time_CurrentTime() / 10000;

    return (u32_t)(currentTime);
}

void sys_signal_sock_event()
{
    Events_Set(SYSTEM_EVENT_FLAG_SOCKET);
}


//--//
//--// Mailbox functions. 
//--//

sys_mbox_t sys_mbox_new(int size)
{    
    Hal_Queue_UnknownSize<OpaqueQueueNode>* queue = (Hal_Queue_UnknownSize<OpaqueQueueNode>*)mem_malloc(sizeof(Hal_Queue_UnknownSize<OpaqueQueueNode>));
    OpaqueQueueNode* memory = (OpaqueQueueNode*)mem_malloc(sizeof(OpaqueQueueNode) * size);
    memset(memory, 0, sizeof(OpaqueQueueNode) * size);
    queue->Initialize(memory, size);

    return queue;
}

void sys_mbox_post(sys_mbox_t mbox, void *msg)
{
    Hal_Queue_UnknownSize<OpaqueQueueNode>* queue = (Hal_Queue_UnknownSize<OpaqueQueueNode>*)mbox;

    OpaqueQueueNode* node = NULL;
    
    if((node = queue->Push()) != NULL) 
    {
        //tcpip_msg* m = (tcpip_msg*)mem_malloc(sizeof(tcpip_msg));
        //api_msg* a = (api_msg*)mem_malloc(sizeof(api_msg));
        //sys_sem_t* s = (sys_sem_t*)mem_malloc(sizeof(sys_sem_t));
        //if(!a || !m || !s) return;
        
        //memset(m,0,sizeof(tcpip_msg));
        //memset(a,0,sizeof(api_msg));
        //memset(s,0,sizeof(sys_sem_t));

        //memcpy(s,((tcpip_msg*)msg)->sem,sizeof(sys_sem_t));
        //memcpy(a,((tcpip_msg*)msg)->msg.apimsg,sizeof(api_msg));
        //memcpy(m,msg,sizeof(tcpip_msg));
        

        //m->msg.apimsg = a;
        
        //node->payload = m;
        
        node->payload = msg;
    }

    SOCKETS_RestartTcpIpProcessor(0);
}

err_t sys_mbox_trypost(sys_mbox_t mbox, void *msg)
{

    Hal_Queue_UnknownSize<OpaqueQueueNode>* queue = (Hal_Queue_UnknownSize<OpaqueQueueNode>*)mbox;

    OpaqueQueueNode* node = queue->Push();

    if(node != NULL) {
        
        //tcpip_msg* m = (tcpip_msg*)mem_malloc(sizeof(tcpip_msg));
        //memset(m,0,sizeof(tcpip_msg));
        //memcpy(m,msg,sizeof(tcpip_msg));
        //node->payload = m;

        node->payload = msg;

        return ERR_OK;    
    }

    SOCKETS_RestartTcpIpProcessor(0);

    return ERR_MEM;
}

u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
    Hal_Queue_UnknownSize<OpaqueQueueNode>* queue = (Hal_Queue_UnknownSize<OpaqueQueueNode>*)mbox;

    // TODO
    // TODO
    // TODO
    // implement timeout
    OpaqueQueueNode* node = queue->Pop();

    if(node) 
    {
        *msg = node->payload;
        return 0;
    }
    else 
    {
        // schedule the tcp thread to run when the next timeout occurs
        // Any incoming data should retrigger the thread prior to timeout
        SOCKETS_RestartTcpIpProcessor(timeout * 1000);

        *msg = NULL; 
        return SYS_MBOX_EMPTY;    
    }

    // time needed
    // do not return SYS_ARCH_TIMEOUT or the system will wait forever
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t mbox, void **msg)
{
    Hal_Queue_UnknownSize<OpaqueQueueNode>* queue = (Hal_Queue_UnknownSize<OpaqueQueueNode>*)mbox;

    OpaqueQueueNode* node = queue->Pop();

    if(node)
    {
        *msg = node->payload;
    }
    else
    {
        *msg = NULL;
        return SYS_MBOX_EMPTY;
    }

    return 0;    
}

void sys_mbox_free(sys_mbox_t mbox)
{
    Hal_Queue_UnknownSize<OpaqueQueueNode>* queue = (Hal_Queue_UnknownSize<OpaqueQueueNode>*)mbox;
    
    mem_free(queue->Storage());
    mem_free(queue);
}

//--//
//--// Thread functions. 
//--// 

sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{
    if(strcmp(name,TCPIP_THREAD_NAME) == 0)
    {
        SOCKETS_CreateTcpIpProcessor(thread, arg);
    }

    return NULL;
}

void sys_thread_free(char *name)
{
    if(strcmp(name,TCPIP_THREAD_NAME) == 0)
    {
        // the tcip thread will be aborted later
        SOCKETS_RestartTcpIpProcessor(0xFFFFFFFF);
    }
}

//--//
//--// Time functions. 
//--// 

// Returns the current time in milliseconds.
u32_t sys_now(void) 
{
    UINT64 currentTime = HAL_Time_CurrentTime() / 10000;

    return (u32_t)(currentTime);
}

//--//
//--// Critical Region Protection. 
//--// 

#if SYS_LIGHTWEIGHT_PROT
sys_prot_t sys_arch_protect(void)
{
    if(INTERRUPTS_ENABLED_STATE())
    {
        DISABLE_INTERRUPTS();
        return 1;
    }

    return 0;
}

void sys_arch_unprotect(sys_prot_t pval)
{
    if(pval == 1)
    {
        ENABLE_INTERRUPTS();
    }
}
#endif

//--//

#ifdef __cplusplus
}
#endif
