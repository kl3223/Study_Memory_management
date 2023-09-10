//
// Created by 16520 on 2023-09-07.
//

#ifndef YAMM_NODE_H
#define YAMM_NODE_H

#include <stdio.h>

typedef struct list_node
{
    struct list_node* prev;
    struct list_node* next;
}LISTNODE;

static inline void init_listnode(LISTNODE* node)
{
    if(NULL == node)
    {
        printf("(%s)error\n",__func__ );
        return;
    }
    node->next = NULL;
    node->prev = NULL;
};

static inline void add_listnode(LISTNODE* prev_node, LISTNODE* new_node)
{
    if(!prev_node || !new_node)
    {
        printf("(%s)error: input is NULL\n",__func__ );
        return;
    }

    if (prev_node->next == NULL)
    {
        prev_node->next = new_node;
        new_node->prev = prev_node;
    }
    else
    {
        //(prev_node->next)->prev = new_node;
        new_node->next = prev_node->next;
        prev_node->next = new_node;
        new_node->prev = prev_node;
        new_node->next->prev = new_node;
    }
}

static inline void delete_listnode(LISTNODE* del_node)
{
    if(NULL== del_node)
    {
        printf("(%s)error: input is NULL\n",__func__ );
        return;
    }
    if (NULL == del_node->prev)
    {
        del_node->next->prev = NULL;
        del_node->next = NULL;
    }
    else if (NULL == del_node->next)
    {
        del_node->prev->next = NULL;
        del_node->prev = NULL;
    }
    else
    {
        del_node->prev->next = del_node->next;
        del_node->next->prev = del_node->prev;
        del_node->prev = NULL;
        del_node->next = NULL;
    }
    return;
}
#endif //YAMM_NODE_H
