/*************************************************************************
	> File Name: includes.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Wed 30 Apr 2014 03:02:48 PM CST
 ************************************************************************/
#ifndef INCLUDES_H
#define INCLUDES_H
#include <stdio.h>       
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <getopt.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if_packet.h>
#include <linux/if.h> 
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/if_ether.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>


#ifdef TILERA_PLATFORM
#include <sys/mman.h>
#include <sys/dataplane.h>
#include <tmc/alloc.h>
#include <arch/atomic.h>
#include <arch/sim.h>

#include <gxio/mpipe.h>
#include <tmc/cpus.h>
#include <tmc/mem.h>
#include <tmc/spin.h>
#include <tmc/sync.h>
#include <tmc/task.h>
#endif


#define PROCBLOCK_SIZE 32
#define PAGE_SHIFT     12

#define bool _Bool
#define true 1
#define false 0
//#define LIKEYI_DEBUG
#ifdef LIKEYI_DEBUG
#define DEBUG(format,...) printf("FILE: "__FILE__", LINE: %d: "format"\n", __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)  
#endif
#define COLLECTOR_MODE 0
#define GENERATOR_MODE 1

#ifdef TILERA_PLATFORM
/* Align "p" mod "align", assuming "p" is a "void*". */
#define ALIGN(p, align) do { (p) += -(long)(p) & ((align) - 1);  } while(0)
/* Help check for errors. */
#define VERIFY(VAL, WHAT)                                    \
        do {                                                 \
        long long __val = (VAL);                             \
            if (__val < 0)                                   \
            tmc_task_die("Failure in '%s': %lld: %s.",       \
                (WHAT), __val, gxio_strerror(__val));        \
        } while (0)
#endif

static inline void exit_if_ptr_is_null(void * ptr,const char * message) 
{
    if(ptr == NULL)
    {   
        printf("%s\n",message);
        exit(-1);
    }   
}

#define ALLONE(n) (0xFFFFFFFF >> (32 - n))

#define LOG_FAULT   (1 << 0)
#define LOG_ERROR   (1 << 1)
#define LOG_WARN    (1 << 2)
#define LOG_INFO    (1 << 3)
#define LOG_DEBUG   (1 << 4)

#define TRACE(trace,fmt, ...)                                            	\
        do {                                                                \
            if ((global_trace & (trace)) == LOG_FAULT)            			\
                printf("[FAULT] <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__); 			\
            else if ((global_trace & (trace)) == LOG_ERROR)            	    \
                printf("[ERROR] <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__); 			\
            else if ((global_trace & (trace)) == LOG_WARN)            	    \
                printf("[WARN ] <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__); 			\
            else if ((global_trace & (trace)) == LOG_INFO)            	    \
                printf("[INFO ] <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__); 			\
            else if ((global_trace & (trace)) == LOG_DEBUG)            	    \
                printf("[DEBUG] <%s> "fmt"", __FUNCTION__, ##__VA_ARGS__); 			\
        } while(0)

typedef struct 
{
    uint32_t id;
    unsigned char * data;
}item_t;

#define MAKE_HASH(n) (n)

#include "error.h"
#include "queue.h"
#include "list.h"
#include "hash.h"

#include "entity.h"
#include "sysenv.h"

#endif
