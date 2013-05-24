#ifndef _lcthw_stack_h
#define _lcthw_stack_h

#include <lcthw/list.h>

typedef List Stack;

#define Stack_create() List_create()
#define Stack_destroy(L) List_destroy(L)
#define Stack_push(L, V) List_unshift((L), (V))
#define Stack_peek(L) List_first(L)
#define Stack_pop(L) List_shift((L))
#define Stack_count(L) List_count(L)

#define STACK_FOREACH(L, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->first; _node != NULL; V = _node = _node->next)

#endif
