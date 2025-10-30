#ifndef lcthw_List_sorted_h
#define lcthw_List_sorted_h

#include <lcthw/list.h>
#include <lcthw/list_algos.h>

void List_insert_before(List *list, ListNode *next_node, void *value);
void List_insert_sorted(List *list, void *value, List_compare cmp);

#endif // !lcthw_List_sorted_h
