/*************************************************************************
	> File Name: hash.c
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Tue 17 Jun 2014 04:36:47 PM CST
 ************************************************************************/

#include "includes.h"
extern uint32_t global_trace;

  
static int next_prime(int x)
{
     long i, j;
     int f;
     
     x = (x==0)?1:x;
     i = x;
     while (i++)
     {
          f=1;
          for (j=2; j<i; j++)
          {
               if (i%j == 0)
               {
                    f=0;
                    break;
                    
               }
          }
          if (f)
          {
             return (int)i;
          }
     }
    return 0;
}

struct blist * find_list(struct list_head * head, 
                         item_t * item,
                         compare * compare_handler)
{
        struct list_head * p;
        struct blist * node;
        list_for_each(p,head)
        {   
                node = list_entry(p,struct blist,listhead);
                if(compare_handler(node->item, item) == 0)
                {   
                    return node;
                }   
        }   
        return NULL;

}

/*
* 1. 创建
* */
hash_table * hash_create(int num,ht_ops_t * ops)
{
    exit_if_ptr_is_null(ops,"ht ops is null");

    hash_table * result;
    bucket_t * b;
    int bytes;
    int i;

    result = malloc(sizeof(hash_table));
    exit_if_ptr_is_null(result,"Initizial hash table Error"); 
    num = next_prime(num);

    bytes = num * sizeof(bucket_t);

    result->buckets = b = malloc(bytes);
    exit_if_ptr_is_null(result->buckets,"hash table buckets alloc error");

    result->ops = malloc(sizeof(ht_ops_t));
    exit_if_ptr_is_null(result->ops,"hash ops alloc error");
    memcpy(result->ops,ops,sizeof(ht_ops_t));

    result->num_buckets = num;

    i = num;
    while(--i >= 0)
    {
        INIT_LIST_HEAD(&b->list); 
        pthread_mutex_init(&b->lock, NULL);
        b->count = 0;
        b++;
    }
    return result;
}
/*
* 2. 查找
* 查找的本质是，给出一组特性
* hash_lookup把特性编程key，然后再把比较特性。
* */
void * hash_lookup_item(hash_table * ht, 
                        uint32_t key, 
                        void * value)
{

    struct list_head * ll;
    bucket_t * bucket = &ht -> buckets[key % ht->num_buckets];
    ll = &bucket->list;
    compare * compare_handler = ht->ops->compare_handler;
    if(compare_handler == NULL)
    {
        TRACE(LOG_FAULT,"compare_handler can not be NULL\n");
        exit(0);
    }
    return (void *) find_list(ll,value,compare_handler);    
}
/*
* 3. 插入
* 严格说来,blist 完全没有任何问题。
* */
int  hash_add_item(hash_table ** htp, uint32_t key, void * value )
{
    struct list_head * ll;
    struct blist * blist;
    struct blist * new_blist;
    hash_table * ht = *htp;
    item_t * item = (item_t *)value;
    //manager_t * manager = list_entry(htp,manager_t,ht);
    /*
     * 
     * */
    bucket_t * bucket = &ht -> buckets[key % ht->num_buckets];

    pthread_mutex_lock(&bucket->lock);

    ll = &bucket->list;

    compare * compare_handler = ht->ops->compare_handler;

    if(compare_handler == NULL)
    {
        TRACE(LOG_FAULT,"compare_handler can not be NULL\n");
        exit(0);
    }

    blist = find_list(ll,item,compare_handler);    
    /*
     * 假如不存在于链表中。
     * */
    if(!blist)
    {
        /*
         * 初始化，并且添加到新建的列表里。
         * */
        if(ht->ops->new_handler)
        {
            ht->ops->new_handler(&new_blist,value);
            INIT_LIST_HEAD(&new_blist->listhead);
            list_add_tail(&new_blist->listhead,ll);
            ++bucket->count;
        }
        else
        {
            TRACE(LOG_WARN,"make_new should not be NULL");
        }

    }
    /* 
    * Found it, and memcpy it.
    * 主要是这一段无法公共化，其他的hash函数可能找到后，并不会copy
    * */
    else
    {
        TRACE(LOG_INFO,"existed_handler:%p\n",ht->ops->existed_handler);
        if(ht->ops->existed_handler)
        {
            ht->ops->existed_handler(blist,value);
        }

    }
    pthread_mutex_unlock(&bucket->lock);
    return 0;
}
/*
* 4. 遍历
* */

void  hash_travel_delete(hash_table * ht)
{
    register int i = 0;
    register bucket_t * bucket;
    delete * delete_item = ht->ops->delete_handler;
    bucket = ht->buckets;
    while(i++ < ht->num_buckets)
    {

        pthread_mutex_lock(&bucket->lock);
        if(delete_item == NULL)
        {
            TRACE(LOG_WARN,"delete_item is null\n");
        }
        else
        {
            delete_item(bucket);
        }
        pthread_mutex_unlock(&bucket->lock);
        bucket++;
    }
}


void  hash_travel_viewer(hash_table * ht)
{
    register int i = 0;
    register bucket_t * bucket;
    viewer * viewer_handler = ht->ops->viewer_handler;
    bucket = ht->buckets;
    while(i++ < ht->num_buckets)
    {

        pthread_mutex_lock(&bucket->lock);
        if(viewer_handler == NULL)
        {
            TRACE(LOG_WARN,"viewer handler is null\n");
        }
        else
        {
            viewer_handler(bucket);
        }
        pthread_mutex_unlock(&bucket->lock);
        bucket++;
    }
}
/*
 * unsigned int hash_count(hash_table *ht)
 *
 * Return total number of elements contained in hash table.
 */

uint32_t hash_count(hash_table * ht)

{
    register int i = 0;
    register int cnt = 0;
    register bucket_t *bucket;

    bucket = ht->buckets;
    while (i++ < ht->num_buckets)
    {
	    cnt += bucket->count;
	    bucket++;
    }

    return cnt;
}
