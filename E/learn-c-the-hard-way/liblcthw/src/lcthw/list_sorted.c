#include "list_sorted.h"
#include "lcthw/dbg.h"
#include "lcthw/list.h"
#include <stdlib.h>
#include <string.h>
void List_insert_before(List *list, ListNode *next_node, void *value) {
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);
  node->value = value;

  check_list(list);

  // if list is empty, ignore prev_node, insert new node.
  if (List_empty(list)) {
    node->prev = NULL;
    node->next = NULL;
    list->first = node;
    list->last = node;
  }
  // else if next_node is NULL, insert new node at the tail of list.
  else if (next_node == NULL) {
    node->prev = list->last;
    node->next = NULL;
    list->last->next = node;
    list->last = node;
  }
  // else, common insert
  else {
    node->prev = next_node->prev;
    node->next = next_node;
    if (node->prev == NULL)
      list->first = node;
    else
      next_node->prev->next = node;
    next_node->prev = node;
  }

  list->count++;
  return;

error:
  if (node != NULL)
    free(node);
  return;
}

void List_insert_sorted(List *list, void *value, List_compare cmp) {
  check_list(list);
  LIST_FOREACH(list, first, next, cur) {
    if (cmp(cur->value, value) > 0)
      break;
  }

  List_insert_before(list, cur, value);
  return;

error:
  return;
}
