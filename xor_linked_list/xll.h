#ifndef XLL_H
#define XLL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(cond, ...) \
    if(!(cond)) { \
        fprintf(stderr, __VA_ARGS__); \
        exit(1); \
    }

#define XORLL_CREATE(type) xorll_create(sizeof(type))

typedef struct xor_node xor_node;
struct xor_node {
    void *_data;
    xor_node *_both;
};

typedef struct Xorll Xorll;
struct Xorll {
    xor_node *_head;
    xor_node *_tail;
    size_t _e_size;
    size_t _size;
};

// Public Functions
Xorll xorll_create(size_t e_size);
void xorll_free(Xorll *list);
void xorll_push_back(Xorll *list, void *data);

// Private Functions
xor_node *xor_(const xor_node *prev, const xor_node *next);
xor_node *xorll_first_(Xorll *list);
xor_node *xorll_last_(Xorll *list);

// Template Functions
#define MAKEFUNC_XORLL_PRINTF(type, fmt, ...) \
    static void xorll_printf_##type(Xorll *list) { \
        xor_node *prev = NULL; \
        xor_node *node = list->_head; \
        while(node) { \
            type x = *(type*)node->_data; \
            printf(fmt, __VA_ARGS__); \
            xor_node *temp = node; \
            node = xor_(node->_both, prev); \
            if(node) printf(", "); \
            prev = temp; \
        } \
    }

#define XORLL_PRINTF(p_list, type) \
    printf(#p_list" -> [ "); \
    xorll_printf_##type(p_list); \
    printf(" ]\n")

#endif // XLL_H
