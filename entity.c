/*************************************************************************
	> File Name: entity.c
	> Author: likeyiyy
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 17 Jul 2014 09:53:41 PM CST
 ************************************************************************/

#include "includes.h"

static inline int _push_to_mailbox(entity_t * entity,mail_t * mail)
{
    if(!is_full_queue(entity->mailbox))
    {
        return push_buf(entity->mailbox,mail); 
    }
    return EQUEUE_FULL;
}
static int _send_mail(addr_t address,mail_t * mail)
{
    hash_table * ht = global_sysinfo->entity_addr;
    entity_t * entity = hash_lookup_item(ht,MAKE_HASH(address),&address);
    if(entity == NULL)
    {
        return ENO_SUCH_ADDRESS;
    }
    int result = _push_to_mailbox(entity,mail);
    return result;
}

static int _recv_mail(queue_t * mailbox,mail_t ** mailp)
{
    if(!is_empty_queue(mailbox))
    {
        pop_buf(mailbox,(void **)mailp);
        return 0;
    }
    return EQUEUE_EMPTY;
}
#define GLOBAL_ENTITY_NUMS  0xffff
#define PER_SEND_NUMS       60

void * _main_loop(void * arg)
{
    entity_t * item = (entity_t *)arg;
    int result;
    mail_t mail;

    mail.data = malloc(20);
    strcpy(mail.data,"hello world");
    mail.length = strlen(mail.data);

    mail_t * r_mail;
    while(1)
    {
        for(int i = 0; i < PER_SEND_NUMS;i++)
        {
            addr_t * addr = malloc(sizeof(addr_t));
            *addr = rand()%GLOBAL_ENTITY_NUMS;
            result = item->action.send_mail(*addr,
                    &mail);
            /*
             * 假如发送失败，代表没有这个地址的实体，结束。
             * 实际中，可能先发送，然后进入一个待回复队列，
             * 假如指定质检后还没有回复，那么同样假如失败
             * 队列。
             * */
            if(result != SUCCESSED)
            {
                push_buf(item->failed,addr);
            }
            else
            {
                push_buf(item->waiting,addr);
            }
        }

        while(1)
        {
            result = item->action.recv_mail(item->mailbox, &r_mail); 
            if(result != SUCCESSED)
            {
                break;
            }
            else
            {
                printf("no thing to do now.\n");
            }
        }

    }
}

entity_ops global_entity_ops =
{
    .send_mail = _send_mail, 
    .recv_mail = _recv_mail,
    .main_loop = _main_loop
};