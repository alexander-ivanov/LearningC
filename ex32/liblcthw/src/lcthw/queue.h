#ifndef _lcthw_queue_h
#define _lcthw_queue_h

#include <lcthw/list.h>

typedef List Queue;

#define Queue_create() List_create()
#define Queue_destroy(L) List_destroy(L)
#define Queue_send(L, V) List_push((L), (V))
#define Queue_peek(L) List_first(L)
#define Queue_recv(L) List_shift((L))
#define Queue_count(L) List_count(L)

#define QUEUE_FOREACH(L, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->first; _node != NULL; V = _node = _node->next)

#endif
