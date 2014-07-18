/*************************************************************************
	> File Name: sysenv.h
	> Author: likeyiyy
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 17 Jul 2014 09:02:17 PM CST
 ************************************************************************/

#ifndef L_SYSENV_H
#define L_SYSENV_H
typedef struct
{

}sys_ops_t;

typedef struct
{
    hash_table * entity_addr;
    hash_table * alloced;
    sys_ops_t  * sys_ops;
}sysinfo_t;

extern sysinfo_t * global_sysinfo;

/*
* 1.init
* */
int init_sysenv(void);

#endif
