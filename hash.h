/*************************************************************************
	> File Name: hash.h
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Tue 17 Jun 2014 04:36:40 PM CST
 ************************************************************************/

#ifndef L_HASH_H
#define L_HASH_H


typedef struct bucket { 
    struct list_head list;
    pthread_mutex_t  lock;
    uint32_t count;
} bucket_t; 

typedef int  (compare)(item_t * a,item_t * b);
typedef void (delete)(bucket_t * bucket);
typedef void (make_new)(struct blist ** new_blist,item_t * value);
typedef void (existed)(struct blist * blist,item_t * value);
typedef void (viewer)(bucket_t * bucket);

typedef struct _hash_table_ops
{
    compare  * compare_handler;
    delete   * delete_handler;
    make_new * new_handler;
    existed  * existed_handler;
    viewer   * viewer_handler;
}ht_ops_t;
               
typedef struct hash_table 
{
        int num_buckets;
        bucket_t * buckets;
        ht_ops_t * ops;
} hash_table;

/*
* 1. 创建
* */
hash_table * hash_create(int num,ht_ops_t * ops);
/*
* 2. 插入
* */
int  hash_add_item(hash_table ** ht,uint32_t key, void * value);
/*
* 3. 查找
* */
void * hash_lookup_item(hash_table * ht,
                        uint32_t key, 
                        void * value);
/*
* 4. 遍历并且删除不符合条件的.
* */
void  hash_travel_delete(hash_table * ht);

void  hash_travel_viewer(hash_table * ht);

uint32_t hash_count(hash_table *ht);



#endif
