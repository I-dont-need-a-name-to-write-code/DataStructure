#include "./vector.h"

// Public Functions

Vector vector_create(size_t e_size, size_t capacity) {
    if(!capacity) capacity = 1;
    ASSERT(e_size, "[ERROR] Element size cannot be NULL\n");
    void *buffer = malloc(capacity * e_size);
    ASSERT(buffer, "[ERROR] Could not allocate %zu bytes : %s\n",
           capacity * e_size, strerror(errno));
    Vector array = {
        ._e_size          = e_size,
        ._size            = 0,
        ._capacity        = capacity,
        ._buffer          = buffer
    };
    return array;
}

Vector vector_copy(const Vector *array) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    ASSERT(array->_buffer, "[ERROR] Could not copy as _buffer addr is null\n");
    Vector array_clone = vector_create(array->_e_size, array->_capacity);
    array_clone._size = array->_size;
    memcpy(array_clone._buffer, array->_buffer, array->_e_size * array->_size);
    return array_clone;
}

Vector vector_move(Vector *array) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    Vector stolen_array = *array;
    *array = (Vector){ 0 };
    return stolen_array;
}

void vector_free(Vector *array) {
    if(!array) return;
    if(array->_buffer) {
        free(array->_buffer);
    }
    *array = (Vector){ 0 };
}

size_t vector_size(const Vector *array) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    return array->_size;
}

size_t vector_capacity(const Vector *array) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    return array->_capacity;
}

bool vector_is_empty(const Vector *array) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    return (array->_size == 0);   
}

void vector_push_back(Vector *array, const void *p_data) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    ASSERT(array->_buffer, "[ERROR] Could not push_back as _buffer addr is null\n");
    if(array->_size >= array->_capacity) {
        vector_realloc_(array, array->_capacity << 1);
    }
    memcpy((char*)array->_buffer + array->_e_size * array->_size++,
           p_data, array->_e_size);
}

void vector_pop_back(Vector *array) {
    vector_remove(array, array->_size - 1, true);
}

Vector vector_slice(const Vector *array, size_t s_idx, size_t e_idx) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    ASSERT(array->_buffer,  "[ERROR] Could not slice as _buffer addr is null\n");
    ASSERT((s_idx <= e_idx), "[ERROR] Condition (s_idx(%zu) > e_idx(%zu)) cannot hold for vector_slice\n", s_idx, e_idx);
    ASSERT((s_idx <= array->_size) && (e_idx <= array->_size), 
           "[ERROR] Condition (s_idx(%zu) > array->_size) OR (e_idx(%zu) > array->_size) cannot hold for vector_slice\n", s_idx, e_idx);
    size_t element_count = (e_idx - s_idx);
    Vector sub_array = vector_create(array->_e_size, element_count);
    sub_array._size = element_count;
    memcpy((char*)sub_array._buffer,
           (char*)array->_buffer + array->_e_size * s_idx, element_count * array->_e_size);
    return sub_array;
}

void vector_remove(Vector *array, size_t index, bool do_realloc) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    ASSERT(array->_buffer, "[ERROR] Could not remove element as _buffer addr is null\n");
    ASSERT((index < array->_size), "[ERROR] Cannot remove non existent element from array\n");
    memcpy((char*)array->_buffer + array->_e_size * index,
           (char*)array->_buffer + array->_e_size * (index + 1), 
           (array->_size - index - 1) * array->_e_size);
    array->_size -= 1;
    size_t half_capacity = array->_capacity >> 1;
    if(do_realloc && (array->_size < half_capacity)) {
        vector_realloc_(array, half_capacity);
    }
}

void vector_write(const Vector *array, FILE *stream) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    ASSERT(array->_buffer, "[ERROR] Could not write as _buffer addr is null\n");
    size_t b = fwrite(array->_buffer, array->_e_size, array->_size, stream);
    ASSERT((b == array->_size), "[ERROR] Could not write the correct number of elements to stream\n");
}

void vector_for_each(Vector* array, void (*lambda)(Vector *array, size_t index)) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    ASSERT(lambda, "[ERROR] Lambda is NULL\n");
    for(size_t i = 0; i < array->_size; ++i) {
        lambda(array, i);
    }
}

Vector vector_filter(const Vector* array, bool (*lambda)(const Vector *array, size_t index)) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    Vector filtered_array = vector_create(array->_e_size, (size_t)NULL);
    for(size_t i = 0; i < array->_size; ++i) {
        if(lambda(array, i)) vector_push_back(&filtered_array, (char*)array->_buffer + i * array->_e_size);
    }
    return filtered_array;
}

void vector_sort(Vector *array, bool (*cmpfunc)(const void *ptr1, const void *ptr2)) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    ASSERT(cmpfunc, "[ERROR] CmpFunc is NULL\n");
    if(!array->_size) return;
    vector_hybrid_quick_sort_(array, 0, (array->_size - 1), cmpfunc);
}

// Private Functions

void swap_bytes_(void *a, void *b, size_t bytes) {
    char temp;
    char *addr1;
    char *addr2;
    for(size_t i = 0; i < bytes; ++i) {
        addr1 = ((char*)a + i);
        addr2 = ((char*)b + i);
        temp   = *addr1;
        *addr1 = *addr2;
        *addr2 =   temp;
    }
}

size_t random_index_(size_t min, size_t max) {
    float x = (float)rand() / (float)RAND_MAX;
    size_t idx = (size_t)(min + x * (float)(max - min));
    return idx;
}

void vector_realloc_(Vector *array, size_t new_capacity) {
    ASSERT(array, "[ERROR] Vector is NULL\n");
    void *new_buffer = malloc(new_capacity * array->_e_size); 
    ASSERT(new_buffer, "[ERROR] Could not allocate %zu bytes : %s\n",
           new_capacity * array->_e_size, strerror(errno));
    for(size_t i = 0; i < array->_size * array->_e_size; ++i) {
        *((char*)new_buffer + i) = *((char*)array->_buffer + i);
    }
    free(array->_buffer);
    array->_buffer   = new_buffer;
    array->_capacity = new_capacity;
}

void vector_insertion_sort_(Vector *array, size_t low, size_t high, bool (*cmpfunc)(const void *ptr1, const void *ptr2)) {
    char *addr_low  = (char*)array->_buffer + array->_e_size * low;
    char *addr_high = (char*)array->_buffer + array->_e_size * high;
    size_t e_size = array->_e_size;
    char temp[e_size];
    for(char *addr_i = addr_low + e_size; addr_i <= addr_high; addr_i = addr_i + e_size) {
        char *addr_j = addr_i - e_size;
        memcpy(temp, addr_i, e_size);
        while(addr_j >= addr_low && !cmpfunc(addr_j, temp)) {
            memcpy(addr_j + e_size, addr_j, e_size);
            addr_j -= e_size;
        }
        memcpy(addr_j + e_size, temp, e_size);
    }
}

size_t vector_partition_(Vector *array, size_t low, size_t high, bool (*cmpfunc)(const void *ptr1, const void *ptr2)) {
    size_t idx = random_index_(low, high);
    char *pivot_addr = (char*)array->_buffer + array->_e_size * high;
    swap_bytes_((char*)array->_buffer + array->_e_size * idx, pivot_addr, array->_e_size);
    size_t i = low - 1;
    for(size_t j = low; j < high; ++j) {
        if(cmpfunc((const char*)array->_buffer + array->_e_size * j, pivot_addr)) {
            i++;
            if(i != j) { 
                swap_bytes_((char*)array->_buffer + array->_e_size * j, 
                            (char*)array->_buffer + array->_e_size * i, array->_e_size);
            }
        }
    } 
    swap_bytes_((char*)array->_buffer + array->_e_size * (i + 1), pivot_addr, array->_e_size);
    return (i + 1);
}

void vector_hybrid_quick_sort_(Vector *array, size_t low, size_t high, bool (*cmpfunc)(const void *ptr1, const void *ptr2)) {
    if(low >= high) return;
    if(high - low + 1 < 10) {
        vector_insertion_sort_(array, low, high, cmpfunc);
        return;
    }
    size_t p = vector_partition_(array, low, high, cmpfunc);
    vector_hybrid_quick_sort_(array, low, p - 1, cmpfunc);
    vector_hybrid_quick_sort_(array, p + 1, high, cmpfunc);
}

