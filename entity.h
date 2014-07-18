/*************************************************************************
	> File Name: entity.h
	> Author: likeyiyy
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 17 Jul 2014 09:10:33 PM CST
 ************************************************************************/

#ifndef L_ENTITY_H
#define L_ENTITY_H
#include "queue.h"

typedef uint32_t addr_t;
typedef struct
{
    uint32_t length;
    char  * data;
}mail_t;
typedef struct 
{
    uint32_t id;
    addr_t   addr;
    uint8_t * name;
}eid_t;
typedef struct 
{
    int (*send_mail)(addr_t address,mail_t * mailp);
    int (*recv_mail)(queue_t * mailbox,mail_t ** mailpp);
    void *(*main_loop)(void * arg);
}entity_ops;
typedef struct
{
    pthread_t   tid;
    eid_t       identity;
    entity_ops  action;
    queue_t *   mailbox;
    queue_t *   failed;
    queue_t *   successed;
    queue_t *   waiting;
}entity_t;
typedef entity_t item_t;
#endif
