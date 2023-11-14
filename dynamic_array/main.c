#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "./vector.h"

int main(void) {
    srand(time(NULL));
    Vector array = VECTOR_CREATE(int64_t, (size_t)NULL);
    for(int i = 0; i < 10; ++i) {
        vector_push_back(&array, (int64_t[]) { i });
    }
    vector_free(&array);
    return 0;
}
