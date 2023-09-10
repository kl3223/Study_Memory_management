//
// Created by 16520 on 2023-09-08.
//
#include "yamm.h"
#define NODE_NUM 1024
MEMORY_MANAGE* memory_manage;


MEMORY_MANAGE* init_memory_manage(int mem_size, int node_num, int* error_num)
{
    memory_manage = (MEMORY_MANAGE*) malloc(sizeof (MEMORY_MANAGE));
    if(NULL == memory_manage)
    {
        *error_num = -1;
        return NULL;
    }
    memory_manage->mem_addr = (void*) malloc(mem_size);
    if(NULL == memory_manage->mem_addr)
    {
        *error_num = -1;
        return NULL;
    }
    memory_manage->mem_size = mem_size;
    memory_manage->free_size = mem_size;
    memory_manage->node_num =node_num;
    memory_manage->alloc_num=0;
    memory_manage->free_num =1;
    memory_manage->idle_num=node_num-1;

    memory_manage->alloc_node = manage_node_init(NULL,0);
    memory_manage->free_node = manage_node_init(NULL,0);
    memory_manage->idle_node = manage_node_init(NULL,0);
    *error_num = 0;
    return memory_manage;
}

void manage_node_merge(MANAGE_NODE* merge_node)
{
    if(!memory_manage || !memory_manage->free_node ||!memory_manage->idle_num)
    {
        printf("(%s)error",__func__ );
        return ;
    }
    MANAGE_NODE* pick = NULL;
    MANAGE_NODE* next =NULL;
    LISTNODE* cur = NULL;
    long node_addr = 0;
    int node_size = 0;

    pick = !memory_manage->alloc_num ? (MANAGE_NODE*)memory_manage->free_node->node.next : merge_node;
    while(pick)
    {
        next = (MANAGE_NODE*)pick->node.next;
        node_addr = (long)pick->addr;
        cur = memory_manage->free_node->node.next;
        while(cur)
        {
            if((pick->addr + pick->size) == ((MANAGE_NODE*)cur)->addr)
            {
                pick = manage_node_pop(pick);
                ((MANAGE_NODE*)cur)->size += pick->size;
                ((MANAGE_NODE*)cur)->addr = pick->addr;
                pick->size = 0;
                pick->addr = NULL;
                mange_node_push(memory_manage->idle_node,pick);
                memory_manage->free_num--;
                memory_manage->idle_num++;
                break;
            }
            else
            {
                cur = cur->next;
            }
        }
        pick = next;
    }
    return ;
}


int yamm_init(int size)
{
    if(size < 0)
    {
        printf("(%s)error\n",__func__ );
        return -1;
    }
    int  i = 0;
    int error_num = 0;
    MANAGE_NODE* init_node = NULL;
    memory_manage = init_memory_manage(size, NODE_NUM, &error_num);
    if(error_num == -1)
    {
        printf("(%s)error\n",__func__ );
        yamm_destroy();
        return -1;
    }

    init_node = manage_node_init(memory_manage->mem_addr, memory_manage->mem_size);
    if(init_node == NULL)
    {
        printf("(%s)error\n",__func__ );
        yamm_destroy();
        return -1;
    }
    add_listnode(&memory_manage->free_node->node, &init_node->node);
    LISTNODE* cur = &memory_manage->idle_node->node;
    for(i = 0; i < NODE_NUM ;i++)
    {
        init_node = manage_node_init(NULL, 0);
        add_listnode(cur,&init_node->node);
        cur = cur->next;
    }
    return 0;
}

void* yamm_alloc(int size)
{
    if(size<0)
    {
        printf("(%s)error: input size < 0\n",__func__);
        return NULL;
    }
    if(!memory_manage || !memory_manage->free_node || !memory_manage->idle_num)
    {
        printf("(%s)error: init the yamm first\n",__func__);
        return NULL;
    }
    if(!memory_manage->idle_num)
    {
        printf("(%s)error: there is no idle node left\n",__func__);
        return NULL;
    }
    LISTNODE* cur = &memory_manage->free_node->node;
    MANAGE_NODE* origin = NULL;
    MANAGE_NODE* pick = NULL;

    while(cur)
    {
        if(((MANAGE_NODE*)cur)->size == size)
        {
            origin = (MANAGE_NODE*)cur;
            pick = manage_node_pop((MANAGE_NODE*)origin);
            mange_node_push(memory_manage->alloc_node,pick);
            memory_manage->idle_num--;
            memory_manage->alloc_num++;
            memory_manage->free_size -= size;
            return pick->addr;
        }
        if(((MANAGE_NODE*)cur)->size>=size)
        {
            origin = (MANAGE_NODE*)cur;
            pick = manage_node_pop((MANAGE_NODE*)memory_manage->idle_node->node.next);
            pick->addr = origin->addr;
            pick->size = size;
            ((MANAGE_NODE*)cur)->addr = (void *)((long)origin->addr+(long)size);
            ((MANAGE_NODE*)cur)->size -= size;
            mange_node_push(memory_manage->alloc_node, pick);
            memory_manage->idle_num--;
            memory_manage->alloc_num++;
            memory_manage->free_size -= size;
            return pick->addr;
        }
        cur = cur->next;
    }
    return NULL;
}

void yamm_free(void* ptr)
{
    if(NULL == ptr)
    {
        printf("(%s)error: input is NULL\n",__func__);
        return ;
    }
    if(!memory_manage || !memory_manage->free_node || !memory_manage->alloc_num)
    {
        printf("(%s)error: init the memory first\n",__func__);
        return ;
    }
    MANAGE_NODE* pick = NULL;
    LISTNODE* cur = &memory_manage->alloc_node->node;
    while(cur)
    {
        if(ptr == ((MANAGE_NODE*)cur)->addr)
        {
            pick = manage_node_pop((MANAGE_NODE*)cur);
            mange_node_push(memory_manage->free_node,pick);
            memory_manage->alloc_num--;
            memory_manage->free_num++;
            memory_manage->free_size+=pick->size;
            manage_node_merge(pick);
            break;
        }
        cur=cur->next;
    }
}

void yamm_destroy(void )
{
    int i = 0;
    if(!memory_manage || !memory_manage->alloc_node || !memory_manage->free_node || !memory_manage->idle_node)
    {
        printf("(%s)error\n",__func__);
        return ;
    }
    if(memory_manage->mem_addr)
    {
        free(memory_manage->mem_addr);
        memory_manage->mem_addr=NULL;
    }
    LISTNODE* cur = NULL;
    MANAGE_NODE* node_array[3] = {memory_manage->alloc_node,memory_manage->free_node,memory_manage->idle_node};
    for(i = 0;i<3;i++)
    {
        cur = &node_array[i]->node;
        while(cur)
        {
            if(cur->prev)
            {
                if(cur->prev)
                {
                    free(cur->prev);
                    cur->prev = NULL;
                }
                if(NULL == cur->next)
                {
                    free(cur);
                    cur = NULL;
                    break;
                }
                cur = cur->next;
            }
        }
        free(memory_manage);
        memory_manage = NULL;
    }
}

void yamm_dump(void )
{
    if(!memory_manage || !memory_manage->alloc_node || !memory_manage->free_node || !memory_manage->idle_node)
    {
        printf("(%s)error：yamm has not start work yet\n",__func__);
        return ;
    }
    printf("----------------------------------------------\n");
    printf("TOTAL MEMORY >>> \naddr: 0x%1x | size: %d | node num: %d\n\n",
           (unsigned long)memory_manage->mem_addr,
           memory_manage->mem_addr,
           memory_manage->mem_size,memory_manage->node_num);
    char* node_name[3] = {"ALLOC","FREE","IDLE"};
    int node_size[3] = {memory_manage->mem_size - memory_manage->free_size,
                        memory_manage->free_size,0};
    int node_number[3] = {memory_manage->alloc_num,
                          memory_manage->free_num,
                          memory_manage->idle_num};
    LISTNODE* candidate_node[3] = {memory_manage->alloc_node->node.next,
                              memory_manage->free_node->node.next,
                              memory_manage->idle_node->node.next};
    LISTNODE* cur = NULL;
    int i = 0;
    int j = 0;
    for(i = 0; i < 3; i++)
    {
        cur = candidate_node[i];
        printf(">>>%s | Node Num: %d | Size :%d\n",node_name[i], node_number[i], node_size[i]);
        while (cur)
        {
            printf("    |------Addr: 0x%lx | Size: %-8d",(unsigned long)(((MANAGE_NODE*)cur)->addr),
                   ((MANAGE_NODE*)cur)->size);
            if(2 == i)
            {
                printf("\n                  ...\n");
                break;
            }
            printf("[");
            double size_num = (double)((MANAGE_NODE*)cur)->size/memory_manage->mem_size*20;
            for(j = 0; j < size_num; j++)
            {
                printf("#");
            }
            for (j = 0; j < 20 -size_num; j++)
            {
                printf("-");
            }
            printf(" ]%.2f%%\n",size_num/20*100);
            cur = cur->next;
        }
        printf("\n");

    }

    printf("-------------------------------------------------------------------\n");
}
