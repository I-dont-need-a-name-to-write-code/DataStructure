#include "./xll.h"

// Public Functions

Xorll xorll_create(size_t e_size) {
    ASSERT(e_size, "[ERROR] Xorll e_size cannot be Zero\n");
    Xorll list = {
        ._head   = NULL,
        ._tail   = NULL,
        ._e_size = e_size,
        ._size   = 0
    };
    return list;
}

void xorll_free(Xorll *list) {
    if(!list) return;
    xor_node *node = list->_head;
    xor_node *prev = NULL;
    while(node) {
        if(node->_data) free(node->_data);
        if(prev) free(prev);
        xor_node *curr = node;
        node = xor_(node->_both, prev);
        prev = curr;
    }
    if(prev) free(prev);
    *list = (Xorll){ 0 };
}

void xorll_push_back(Xorll *list, void *data) {
    ASSERT(list, "[ERROR] List cannot be NULL\n");
    xor_node *new_node = (xor_node*)malloc(sizeof(xor_node));
    ASSERT(new_node, "[ERROR] Failed to allocate %zu bytes\n", sizeof(xor_node));
    new_node->_data = malloc(list->_e_size);
    ASSERT(new_node->_data, "[ERROR] Failed to allocate %zu bytes\n", list->_e_size);
    memcpy(new_node->_data, data, list->_e_size);
    list->_size += 1;
    if(!list->_head) {
        new_node->_both = NULL;
        list->_head = new_node;
        list->_tail = new_node;
        return;
    }
    new_node->_both = list->_tail;
    list->_tail->_both = xor_(list->_tail->_both, new_node);
    list->_tail = new_node;
}


// Private Functions

xor_node *xor_(const xor_node *prev, const xor_node *next) {
    return (xor_node*)((size_t)prev ^ (size_t)next);
}

xor_node *xorll_first_(Xorll *list) {
    return list->_head;
}

xor_node *xorll_last_(Xorll *list) {
    return list->_tail;
}
