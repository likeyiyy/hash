/*************************************************************************
	> File Name: main.c
	> Author: likeyi
	> Mail: likeyi@sina.com 
	> Created Time: Thu 17 Jul 2014 02:01:01 PM CST
 ************************************************************************/

#include "includes.h"


int global_trace = ALLONE(5);

int        g_entitys_nums = 2;
entity_t ** g_entitys;

int main(int argc,char ** argv)
{
    /*
    * global_sysinfo init,need not return.
    * */
    srand((unsigned int)time(NULL));
    init_sysenv();
    
    g_entitys = malloc(g_entitys_nums * sizeof(entity_t *));
    for(int i = 0; i < g_entitys_nums; i++)
    {
        g_entitys[i] = init_signle_entity();
        hash_add_item(&global_sysinfo->entity_addr,
                      MAKE_HASH(g_entitys[i]->identity.addr),
                      g_entitys[i]);

    }

    hash_travel_viewer(global_sysinfo->entity_addr);
    for(int i = 0; i < g_entitys_nums; i++)
    {
        pthread_create(
            &g_entitys[i]->tid,
            NULL,
            g_entitys[i]->action.main_loop,
            g_entitys[i]
            );
    }

    pthread_exit(NULL);

}
