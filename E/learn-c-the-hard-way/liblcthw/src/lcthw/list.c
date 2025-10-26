#include <assert.h>
#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <stdlib.h>
#include <string.h>

List *List_create() { return calloc(1, sizeof(List)); }

void check_list(List *list) {
  check(list != NULL, "list must not be NULL.");
  check(list->count >= 0, "list's count is less than 0.");
  check(list->first != NULL || list->count == 0,
        "list's first is NULL while count is not 0.");
  return;
error:
  exit(1);
}

void List_destroy(List *list) {
  check_list(list);
  LIST_FOREACH(list, first, next, cur) {
    if (cur->prev) {
      free(cur->prev);
    }
  }

  free(list->last);
  free(list);
}

void List_clear(List *list) {
  check_list(list);
  LIST_FOREACH(list, first, next, cur) { free(cur->value); }
}

void List_clear_destroy(List *list) {
  check_list(list);
  // List_clear(list);
  // List_destroy(list);
  LIST_FOREACH(list, first, next, cur) {
    free(cur->value);
    if (cur->prev)
      free(cur->prev);
  }
  free(list->last);
  free(list);
}

void List_push(List *list, void *value) {
  check_list(list);
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->last == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->last->next = node;
    node->prev = list->last;
    list->last = node;
  }

  list->count++;
error:
  return;
}

void *List_pop(List *list) {
  check_list(list);
  ListNode *node = list->last;
  return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value) {
  check_list(list);
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->first == NULL) {
    list->first = node;
    list->last = node;
  } else {
    list->first->prev = node;
    node->next = list->first;
    list->first = node;
  }

  list->count++;
error:
  return;
}

void *List_shift(List *list) {
  check_list(list);
  return List_pop(list);
}

void *List_remove(List *list, ListNode *node) {
  check_list(list);
  void *result = NULL;

  check(list->first && list->last, "List is empty.");
  check(node, "node can not be NULL");

  if (node == list->first && node == list->last) {
    list->first = NULL;
    list->last = NULL;
  } else if (node == list->first) {
    list->first = node->next;
    check(list->first != NULL,
          "Invalid list, somehow got a first that is NULL.");
    list->first->prev = NULL;
  } else if (node == list->last) {
    list->last = node->prev;
    check(list->last != NULL, "Invalid list, somehow got a next that is NULL.");
    list->last->next = NULL;
  } else {
    node->next->prev = node->prev;
    node->prev->next = node->next;
  }

  list->count--;
  result = node->value;
  free(node);

error:
  return result;
}

List *List_copy(List *from_list) {
  List *to_list = List_create();
  check(to_list != NULL, "Failed create new list.");

  if (from_list == NULL)
    return to_list;

  LIST_FOREACH(from_list, first, next, cur) { List_push(to_list, cur->value); }

  check(List_count(from_list) == List_count(to_list), "Failed copy to list");

  return to_list;

error:
  List_clear_destroy(to_list);
  return NULL;
}

void List_concat(List *to_list, List *from_list) {
  check_list(to_list);
  if (from_list == NULL)
    return;

  ListNode *a = to_list->last;
  ListNode *b = from_list->first;

  // 若from_list为空，直接返回to_list
  if (b == NULL) {
    free(from_list);
    return;
  }

  // to_list为空，需要同时设置first和last
  if (a == NULL) {
    b->prev = a;
    to_list->first = b;
    to_list->last = from_list->last;
    to_list->count = from_list->count;
    free(from_list);
    return;
  }

  // to_list、from_list均非空，正常拼接
  b->prev = a;
  a->next = b;
  to_list->last = from_list->last;
  to_list->count += from_list->count;
  free(from_list);
  return;
}

List *List_split(List *from_list, ListNode *node) {
  check_list(from_list);
  List *to_list = List_create();
  check(to_list != NULL, "Failed create new list.");

  int i = 0;
  LIST_FOREACH(from_list, first, next, cur) {
    if (cur == node) {
      to_list->first = cur;
      to_list->last = from_list->last;
      to_list->count = List_count(from_list) - i;

      if (cur->prev == NULL) {
        from_list->first = NULL;
      }
      from_list->last = cur->prev;
      cur->prev = NULL;
      from_list->count = i;
      return to_list;
    }
    i++;
  }

  check(1, "from_list has no node.");
error:
  List_clear_destroy(to_list);
  return NULL;
}
