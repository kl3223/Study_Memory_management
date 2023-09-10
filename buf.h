//
// Created by 16520 on 2023-09-07.
//

#ifndef YAMM_BUF_H
#define YAMM_BUF_H
#include "node.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct manage_node
{
    LISTNODE node;
    int size;
    void* addr;
}MANAGE_NODE;

MANAGE_NODE* manage_node_init(void*, int);
MANAGE_NODE* manage_node_pop(MANAGE_NODE*);
void mange_node_push(MANAGE_NODE*, MANAGE_NODE*);




#endif //YAMM_BUF_H
