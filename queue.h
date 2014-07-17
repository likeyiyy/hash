/*************************************************************************
	> File Name: queue.h
	> Author: likeyi
	> Mail: likeyiyy@sina.com 
	> Created Time: Wed 30 Apr 2014 03:30:41 PM CST
 ************************************************************************/
#ifndef QUEUE_MANAGER_H
#define QUEUE_MANAGER_H
typedef struct _queue
{
    int push_pos;
    int pop_pos;
    void ** node;
    int item_size;
    unsigned int total;
    unsigned int length;
    pthread_mutex_t mutex;
    pthread_cond_t empty;
    pthread_cond_t full;
}queue_t;
/*
* This is just a queue not a pool
* */
queue_t * init_queue(int length,int item_size);

void destroy_queue(queue_t * queue);

bool push_buf(queue_t * queue,void * data);

bool pop_buf(queue_t * queue,void ** data);

bool is_empty_queue(queue_t * queue);

bool is_full_queue(queue_t * queue);
#endif
