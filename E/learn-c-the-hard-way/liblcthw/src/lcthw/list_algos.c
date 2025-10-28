#include "lcthw/list_algos.h"
#include "lcthw/list.h"
#include <stdlib.h>
void List_swap(ListNode *a, ListNode *b) {
  void *buf = a->value;
  a->value = b->value;
  b->value = buf;
}

int List_bubble_sort(List *list, List_compare cmp) {
  if (list == NULL || List_count(list) < 2) {
    return 0;
  }

  int swapped;
  do {
    swapped = 0;
    ListNode *prev = list->first;
    ListNode *next = prev->next;
    while (next != NULL) {
      if (cmp(prev->value, next->value) > 0) {
        swapped = 1;
        List_swap(prev, next);
      }
      prev = next;
      next = next->next;
    }
  } while (swapped);

  return 0;
}

// merge prev_list and next_list into list
void merge(List *list, List *prev_list, List *next_list, List_compare cmp) {
  while (List_not_empty(prev_list) || List_not_empty(next_list)) {
    ListNode *n = NULL;
    if (List_empty(prev_list)) {
      n = List_shift_node(next_list);
    } else if (List_empty(next_list)) {
      n = List_shift_node(prev_list);
    } else if (cmp(prev_list->first->value, next_list->first->value) > 0) {
      n = List_shift_node(next_list);
    } else {
      n = List_shift_node(prev_list);
    }
    List_push_node(list, n);
  }
  List_destroy(prev_list);
  List_destroy(next_list);
}

List *List_merge_sort(List *list, List_compare cmp) {
  if (List_count(list) < 2)
    return list;

  // create two new list
  List *prev_list = List_split(list, list->first);
  List *next_list =
      List_split(prev_list, List_get(prev_list, List_count(prev_list) >> 1));

  // 1. sorted_list = list, list will be destroied in merge.
  // 2. sorted_list != list, should destroy list manually.
  List *sort_prev_list = List_merge_sort(prev_list, cmp);
  List *sort_next_list = List_merge_sort(next_list, cmp);

  if (sort_prev_list != prev_list)
    List_destroy(prev_list);
  if (sort_next_list != next_list)
    List_destroy(next_list);

  merge(list, sort_prev_list, sort_next_list, cmp);
  return list;
}

List *List_merge_sort_recu(List *list, List_compare cmp) {
  list = List_copy(list);
  List *sorted_list = List_merge_sort(list, cmp);
  return sorted_list;
}

List *List_merge_sort_iter(List *list, List_compare cmp) {
  // new list will be returned. Don't need to be destroid.
  list = List_copy(list);
  if (List_count(list) < 2)
    return list;
  List *prev_list, *next_list, *remain_list;
  int len = List_count(list);
  int seg;
  for (seg = 1; seg < len; seg <<= 1) {
    remain_list = list;
    list = List_create();
    while (List_count(remain_list)) {
      // create 2 lists, prev_list and next_list will be destroid in merge.
      // remain_list need to be destroid manually.
      prev_list = remain_list;
      next_list = List_split(remain_list, List_get(remain_list, seg));
      remain_list = List_split(prev_list, List_get(prev_list, seg));
      merge(list, prev_list, next_list, cmp);
    }
    List_destroy(remain_list);
  }
  return list;
}
