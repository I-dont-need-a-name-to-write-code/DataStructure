#include <stdio.h>
#include "./xll.h"

typedef struct Entity {
    int id;
    float weight;
} Entity;

//#ifndef MADEFUNC_XORLL_PRINTF_INT
//    #define MADEFUNC_XORLL_PRINTF_INT
//    MAKEFUNC_XORLL_PRINTF(int, "%d", x)
//#endif

#ifndef MADEFUNC_XORLL_PRINTF_ENTITY
    #define MADEFUNC_XORLL_PRINTF_ENTITY
    MAKEFUNC_XORLL_PRINTF(Entity, "{ id: %d, w: %f }", x.id, x.weight)
#endif


int main(void) {
    Xorll list = XORLL_CREATE(Entity);
    for(int i = 0; i < 3; ++i) {
        xorll_push_back(&list, (Entity[]){ 
            { .id = i + 1, .weight = 100.0f * (rand() / (float)RAND_MAX)  }
        });
    }
    
    XORLL_PRINTF(&list, Entity);
    printf("size : %zu\n", list._size);
    xorll_free(&list);

    return 0;
}
