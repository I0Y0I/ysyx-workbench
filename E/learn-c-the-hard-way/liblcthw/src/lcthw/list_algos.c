#include "lcthw/list_algos.h"
#include "lcthw/dbg.h"
#include "lcthw/list.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
  ListNode *a = prev_list->first;
  ListNode *b = next_list->first;
  while (a != NULL || b != NULL) {
    ListNode *n = NULL;
    if (a == NULL) {
      n = b;
      b = b->next;
    } else if (b == NULL) {
      n = a;
      a = a->next;
    } else if (cmp(a->value, b->value) > 0) {
      n = b;
      b = b->next;
    } else {
      n = a;
      a = a->next;
    }
    List_add_node(list, n);
  }
  free(prev_list);
  free(next_list);
}

List *List_merge_sort(List *list, List_compare cmp) {
  if (List_count(list) < 2)
    return list;

  List *prev_list = list;
  List *next_list = List_split(list, List_get(list, List_count(list) >> 1));

  List *sort_prev_list = List_merge_sort(prev_list, cmp);
  List *sort_next_list = List_merge_sort(next_list, cmp);

  list = List_create();
  merge(list, sort_prev_list, sort_next_list, cmp);
  return list;
}

List *List_merge_sort_iter(List *list, List_compare cmp) {
  List *prev_list, *next_list, *remain_list;
  int len = List_count(list);
  int seg;
  for (seg = 1; seg < len; seg <<= 1) {
    remain_list = list;
    list = List_create();
    while (remain_list) {
      prev_list = remain_list;
      next_list = List_split(remain_list, List_get(remain_list, seg));
      remain_list = List_split(prev_list, List_get(prev_list, seg));
    }
  }
  return NULL;
}
