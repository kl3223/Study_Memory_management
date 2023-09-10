#include <stdio.h>
#include "node.h"
#include "yamm.h"
#include "node.h"
#include "buf.h"


int main()
{
    yamm_init(1000);
    char* a = yamm_alloc(30);
    char* b = yamm_alloc(40);
    yamm_dump();
    char* c = yamm_alloc(30);
    yamm_dump();
    yamm_free((void* )b);
    yamm_dump();
    yamm_alloc(100);
    yamm_dump();
    yamm_free((void *)c);
    yamm_dump();
    return 0;
}


