/*************************************************************************
	> File Name: main.c
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Thu 17 Jul 2014 02:01:01 PM CST
 ************************************************************************/

#include "includes.h"


int global_trace = ALLONE(5);

static int compare_item(item_t * a,item_t * b)
{
    if(a->id == b->id)
    {
        return 0;
    }
    return -1;
}
static void make_new_item(struct blist ** new_blist,item_t * value)
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
        printf("%d ",node->item->id);
    }
    printf("\n");

}
ht_ops_t ht_ops = 
{
    .compare_handler = compare_item,
    .new_handler = make_new_item,
    .viewer_handler = viewer_item,
    .delete_handler = NULL,
    .existed_handler = NULL,
};
int main(int argc,char ** argv)
{
    hash_table * ht = hash_create(212,&ht_ops);
    item_t * item;
    item = malloc(sizeof(item_t) * 500);

    for(int i = 0; i < 1000; i+= 2)
    {
        item[i].id = i;
        item[i].data = malloc(15);
        sprintf(item[i].data,"I am %d item",i);
        hash_add_item(&ht,i,(void *)&item[i]);
    }


    printf("hash count : %u \n",hash_count(ht));

    hash_travel_viewer(ht);

    {
        if(hash_lookup_item(ht,512,&item[2]) != NULL)
        {
            printf("hit it %d\n",item[2].id);
        }
        else
        {
            printf("not hit it \n");
        }
    }
}
