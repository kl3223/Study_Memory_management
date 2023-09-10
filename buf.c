//
// Created by 16520 on 2023-09-07.
//
#include "buf.h"

MANAGE_NODE* manage_node_init(void* addr, int size)
{
    if(0 > size)
    {
        printf("(error)\n");

        return NULL;
    }
    MANAGE_NODE* manage_node = (MANAGE_NODE*)malloc(sizeof (MANAGE_NODE));
    if(NULL == manage_node)
    {
        printf("(%s)error\n",__func__ );
        return NULL;
    }
    manage_node->addr = addr;
    manage_node->size = size;
    init_listnode(&manage_node->node);
    return manage_node;
}

MANAGE_NODE* manage_node_pop(MANAGE_NODE* manage_node)
{
    if(NULL == manage_node)
    {
        printf("(%s)error:input is NULL\n",__func__ );
        return NULL;
    }
    LISTNODE* del_node = &manage_node->node;
    delete_listnode(del_node);
    return (MANAGE_NODE*)del_node;
}

void mange_node_push(MANAGE_NODE* prev_manage_node, MANAGE_NODE* new_manage_node)
{
    if(NULL==prev_manage_node || NULL== new_manage_node)
    {
        printf("(%s)error:input is NULL\n",__func__ );
        return;
    }
    LISTNODE* prev = &prev_manage_node->node;
    add_listnode(prev,&new_manage_node->node);
}




