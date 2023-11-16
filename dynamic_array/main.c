#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "./vector.h"

#ifndef MADEFUNC_VECTOR_PRINTF_INT
    #define MADEFUNC_VECTOR_PRINTF_INT
    MAKEFUNC_VECTOR_PRINTF(int, "%d", x)
#endif

int main(void) {
    srand(time(NULL));
    Vector array = VECTOR_CREATE(int, (size_t)NULL);
    for(int i = 0; i < 10; ++i) {
        vector_push_back(&array, (int[]) { i + 1 });
    }
    VECTOR_PRINTF(&array, int);
    vector_free(&array);
    return 0;
}
