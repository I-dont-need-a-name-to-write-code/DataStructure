#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#define ASSERT(cond, ...) \
    if(!(cond)) { \
        fprintf(stderr, __VA_ARGS__); \
        exit(1); \
    }                           

#define VECTOR_CREATE(type, cap) vector_create(sizeof(type), (cap))
#define VECTOR_ELEMENT(p_array, type, index) (*((type*)(p_array)->_buffer + (index)))
#define VECTOR_ELEMENT_TOP(p_array, type) VECTOR_ELEMENT(p_array, type, ((p_array)->_size - 1))
#define VECTOR_IS_EMPTY(p_array) ((p_array)->_size == (size_t)0)

typedef struct Vector Vector;
struct Vector {
    size_t _e_size;
    size_t _size;
    size_t _capacity; 
    void  *_buffer; 
};

// Public Functions
Vector vector_create(size_t e_size, size_t capacity);
Vector vector_copy(const Vector *array);
Vector vector_move(Vector *array); 
void vector_free(Vector *array);
size_t vector_size(const Vector *array);
size_t vector_capacity(const Vector *array);
bool vector_is_empty(const Vector *array);
void vector_push_back(Vector *array, const void *p_data);
void vector_pop_back(Vector *array);
Vector vector_slice(const Vector *array, size_t s_idx, size_t e_idx); 
void vector_remove(Vector *array, size_t index, bool do_realloc);
void vector_write(const Vector *array, FILE *stream);
void vector_for_each(Vector* array, void (*lambda)(Vector *array, size_t index));
Vector vector_filter(const Vector* array, bool (*lambda)(const Vector *array, size_t index));
void vector_sort(Vector *array, bool (*cmpfunc)(const void *ptr1, const void *ptr2));

// Private Functions
void swap_bytes_(void *a, void *b, size_t bytes);
size_t random_index_(size_t min, size_t max); 
void vector_realloc_(Vector *array, size_t new_capacity);
void vector_insertion_sort_(Vector *array, size_t low, size_t high, bool (*cmpfunc)(const void *ptr1, const void *ptr2));
size_t vector_partition_(Vector *array, size_t low, size_t high, bool (*cmpfunc)(const void *ptr1, const void *ptr2));
void vector_hybrid_quick_sort_(Vector *array, size_t low, size_t high, bool (*cmpfunc)(const void *ptr1, const void *ptr2));

// Template Functions
#define MAKEFUNC_VECTOR_PRINTF(type, fmt, ...) \
    static inline void vector_printf_##type(Vector *array) { \
        for(size_t i = 0; i < vector_size(array); ++i) { \
            type x = VECTOR_ELEMENT(array, type, i); \
            if(i > 0) printf(", "); \
            printf(fmt, __VA_ARGS__); \
        } \
    }
#define VECTOR_PRINTF(p_array, type) \
    printf(#p_array" := [ (%zu, %zu) | { ", (p_array)->_size, (p_array)->_capacity); \
    vector_printf_##type(p_array); \
    printf(" } ]\n")

#endif // VECTOR_H
