/*************************************************************************
	> File Name: sysenv.c
	> Author: likeyiyy
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 17 Jul 2014 09:02:26 PM CST
 ************************************************************************/

#include "includes.h"

sysinfo_t global_sysinfo_body;
sysinfo_t * global_sysinfo = &global_sysinfo_body;

static int compare_addr(void * item,void * item2)
{
    entity_t * e = (entity_t *)item;
    entity_t * f = (entity_t *)item2;
    if(e->identity.addr == f->identity.addr)
    {
        return 0;
    }
    return EUEQUEAL_ADDRESS;
}
static void make_new_addr(struct blist ** new_blist,void * value)
{
    *new_blist = malloc(sizeof(struct blist));
    (*new_blist)->item = value;
}
static void viewer_item(bucket_t * bucket)
{
    struct list_head * p, * list;
    struct blist * node;
    struct list_head * next;
    list = &bucket->list;
    list_for_each_safe(p,next,list)
    {
        node = list_entry(p,struct blist,listhead); 
        entity_t * e = (entity_t *)node->item;
        printf("%d ",e->identity.addr);
        if(p->next == list)
            printf("\n");
    }

}
ht_ops_t ht_ops = 
{
    .compare_handler = compare_addr,
    .new_handler = make_new_addr,
    .viewer_handler = viewer_item,
    .delete_handler = NULL,
    .existed_handler = NULL,
};

static int compare_addr2(void * item,void * address)
{
    if(*(addr_t *)item == *(addr_t *)address)
    {
        return 0;
    }
    return EUEQUEAL_ADDRESS;
}
ht_ops_t ht_ops2 = 
{
    .compare_handler = compare_addr2,
    .new_handler = make_new_addr,
    .viewer_handler = NULL,
    .delete_handler = NULL,
    .existed_handler = NULL,
};
int init_sysenv(void)
{
    global_sysinfo->entity_addr = hash_create(GLOBAL_ENTITY_NUMS/20,&ht_ops);
    global_sysinfo->alloced = hash_create(GLOBAL_ENTITY_NUMS/20,&ht_ops2);
    global_sysinfo->sys_ops     = NULL;
    return 0;
}
