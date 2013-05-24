#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

struct ListNode;

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    void *value;
} ListNode;

typedef struct List {
    int count;
    ListNode *first;
    ListNode *last;
} List;

List *List_create();
/* Free memory for all list nodes*/
void List_destroy(List *list);
/* Free memory for all values in the list*/
void List_clear(List *list);
/* Free memory for all list nodes and it's values*/
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

/* Add new node to the end of the list with "value" param as a value */
void List_push(List *list, void *value);
/* Remove last element of the list and return it's value */
void *List_pop(List *list);

/* Add new node before the head of the list with "value" param as a value */
void List_unshift(List *list, void *value);
/* Remove first element of the list and return it's value */
void *List_shift(List *list);

/* Remove element specified by node param from the list and return it's value */
void *List_remove(List *list, ListNode *node);

/* TODO */
// copy, join, split

/* Macro to generate iteration code through the list*/
#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif

