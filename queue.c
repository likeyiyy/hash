/*************************************************************************
	> File Name: queue.c
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Wed 30 Apr 2014 03:30:33 PM CST
 ************************************************************************/
#include "includes.h"
/*
* Date:     2014年4月30日15:53:00
* Author:   likeyi
* Function: 初始化一个所有空闲的池子。
* Input:
* Output:
* */
queue_t * init_queue(int numbers,int item_size)
{
    assert(numbers > 0);
    queue_t * queue = malloc(sizeof(queue_t));
    exit_if_ptr_is_null(queue,"alloc queue error");
    queue->node = malloc(numbers * sizeof(void *));
    exit_if_ptr_is_null(queue->node,"alloc queue node error");
    bzero(queue->node,numbers);
    queue->item_size = item_size;
    queue->total = numbers;
    queue->length = 0;
    queue->pop_pos = 0;
    /*
    * 初始化时为满的。
    * */
    queue->push_pos = 0;
    pthread_mutex_init(&queue->mutex,NULL);
    pthread_cond_init(&queue->empty,NULL);
    return queue;
}
/*
* 销毁一个缓冲区池子
* 时间：2014年5月8日17:49:21
* 作者: likeyi
* */
void destroy_queue(queue_t * queue)
{
    pthread_mutex_destroy(&queue->mutex); 
    pthread_cond_destroy(&queue->empty);
    free(queue->node);
    queue->node = NULL;
    free(queue);
    queue = NULL;
}
bool push_buf(queue_t * queue,void * data)
{
    pthread_mutex_lock(&queue->mutex);
    while(((queue->push_pos + 1) % queue->total == queue->pop_pos))
    {
        DEBUG("Error:queue is full\n");
        //pthread_mutex_unlock(&queue->mutex);
        pthread_cond_wait(&queue->full,&queue->mutex);
    }
    queue->node[queue->push_pos] = data;
    ++queue->push_pos;
    ++queue->length;
    if(queue->push_pos >= queue->total)
    {
        queue->push_pos = 0;
    }
    pthread_cond_signal(&queue->empty);
    pthread_mutex_unlock(&queue->mutex);
    return true;
}
/*
*  本函数不用is_empty_queue，因为防止锁中锁
* */
bool pop_buf(queue_t * queue,void ** data)
{
    pthread_mutex_lock(&queue->mutex);
    while(queue->push_pos == queue->pop_pos)
    {
        DEBUG("Error:queue is empty\n");
        pthread_cond_wait(&queue->empty,&queue->mutex);
    }
    *data = queue->node[queue->pop_pos];
    ++queue->pop_pos;
    --queue->length;
    if(queue->pop_pos >= queue->total)
    {
        queue->pop_pos = 0;
    }
    pthread_cond_signal(&queue->full);
    pthread_mutex_unlock(&queue->mutex);
    return true;
}
/*
*  
* */
bool is_empty_queue(queue_t * queue)
{
    bool result;
    pthread_mutex_lock(&queue->mutex);
    result = (queue->push_pos == queue->pop_pos);
    pthread_mutex_unlock(&queue->mutex);
    return result;
}
bool is_full_queue(queue_t * queue)
{
    bool result;
    pthread_mutex_lock(&queue->mutex);
    result =((queue->push_pos + 1) % queue->total == queue->pop_pos);
    pthread_mutex_unlock(&queue->mutex);
    return result;
}
