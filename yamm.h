//
// Created by 16520 on 2023-09-07.
//

#ifndef YAMM_YAMM_H
#define YAMM_YAMM_H
#include "node.h"
#include "buf.h"
#include <stdio.h>

typedef struct memory_manage
{
    void* mem_addr;
    int mem_size;
    int free_size;

    unsigned int node_num;
    unsigned int alloc_num;
    unsigned int free_num;
    unsigned int idle_num;

    MANAGE_NODE* alloc_node;    //已分配节点
    MANAGE_NODE* free_node;     //未分配节点
    MANAGE_NODE* idle_node;     //闲置节点
}MEMORY_MANAGE;

extern MEMORY_MANAGE* memory_manage;
MEMORY_MANAGE* init_memory_manage(int , int , int* );
void manage_node_merge(MANAGE_NODE*);
int yamm_init(int );
void* yamm_alloc(int );
void yamm_free(void* );
void yamm_destroy(void );
void yamm_dump(void );

#endif //YAMM_YAMM_H
