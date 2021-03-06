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

    entity_t search;
    search.identity.addr = address;
    entity_t * entity = hash_lookup_item(ht,MAKE_HASH(address),&search);
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

static void list_cast_handler(bucket_t * bucket,mail_t * mail)
{
    struct list_head * p, * list;
    struct blist * node;
    struct list_head * next;
    list = &bucket->list;
    list_for_each_safe(p,next,list)
    {
        node = list_entry(p,struct blist,listhead); 
        addr_t to = *(addr_t *)node->item;
        mail_t * clone = malloc(sizeof(mail_t));
        *clone = * mail;
        clone->dstaddr = to;
        _send_mail(to,clone);
    }

}
static int board_cast_mail(mail_t * mail)
{
    register int i = 0;
    register bucket_t * bucket;
    hash_table * ht = global_sysinfo->alloced;
    bucket = ht->buckets;
    while(i++ < ht->num_buckets)
    {
        pthread_mutex_lock(&bucket->lock);
        list_cast_handler(bucket,mail);
        pthread_mutex_unlock(&bucket->lock);
        bucket++;
    }
    
}
void * _main_loop(void * arg)
{
    entity_t * item = (entity_t *)arg;
    int result;
    mail_t * mail = malloc(sizeof(mail_t));

    addr_t self_addr = item->identity.addr;

    mail->data = malloc(20);
    strcpy(mail->data,"hello world ");
    mail->srcaddr = self_addr;
    mail->length = strlen(mail->data);

    mail_t * r_mail;
    //while(1)
    for(int j = 0; j < 1; j++)
    {
#if 0
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
                TRACE(LOG_INFO,"[%u] said:no such address [%u]\n",self_addr,*addr);
                push_buf(item->failed,addr);
            }
            else
            {
                printf("get the address\n");
                push_buf(item->waiting,addr);
            }
        }
#endif
        board_cast_mail(mail);

        sleep(1);

        while(1)
        {
            result = item->action.recv_mail(item->mailbox, &r_mail); 
            if(result != SUCCESSED)
            {
                break;
            }
            else
            {
                r_mail->data[r_mail->length-1] = '\0';
                TRACE(LOG_DEBUG,"[%u] GET [%u]->[%u] %s\n",self_addr,r_mail->srcaddr,r_mail->dstaddr,r_mail->data);
            }
        }
#if 0
        while(1)
            continue;
#endif

    }
    return NULL;
}

entity_ops global_entity_ops =
{
    .send_mail = _send_mail, 
    .recv_mail = _recv_mail,
    .main_loop = _main_loop,
};

static uint32_t get_id(void)
{
    static uint32_t id = 0;
    return ++id;
}
static addr_t get_addr(void)
{
    hash_table * ht = global_sysinfo->alloced;
    entity_t * entity = NULL;
    while(1)
    {
        addr_t address = rand()%GLOBAL_ENTITY_NUMS;
        entity = hash_lookup_item(ht,MAKE_HASH(address),&address);
        if(entity == NULL)
        {
            addr_t * new_addr = malloc(sizeof(addr_t));
            *new_addr = address;
            hash_add_item(&ht,MAKE_HASH(*new_addr),new_addr);
            return address;
        }
    }
}
entity_t * init_signle_entity(void)
{
    entity_t * e = malloc(sizeof(entity_t));
    e->identity.id      = get_id();
    e->identity.addr    = get_addr();
    e->identity.name = malloc(20);
    sprintf(e->identity.name,"name:%u",e->identity.addr);

    e->action    = global_entity_ops;

    e->mailbox   = init_queue(MAILBOX_SIZE,sizeof(mail_t));
    e->failed    = init_queue(FAILED_SIZE,sizeof(addr_t));
    e->successed = init_queue(SUCCESSED_SIZE,sizeof(addr_t));
    e->waiting   = init_queue(WAITING_SIZE,sizeof(addr_t));

    return e;
}
