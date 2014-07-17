/*************************************************************************
	> File Name: sysenv.c
	> Author: likeyiyy
	> Mail: likeyiyy@sina.com 
	> Created Time: Thu 17 Jul 2014 09:02:26 PM CST
 ************************************************************************/

#include "includes.h"

sysinfo_t global_sysinfo_body;
sysinfo_t * global_sysinfo;

int init_sysenv(void)
{
    global_sysinfo->entity_addr = hash_create();

}
