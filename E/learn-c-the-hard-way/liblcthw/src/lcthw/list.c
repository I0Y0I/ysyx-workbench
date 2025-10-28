#include <assert.h>
#include <lcthw/list.h>
#include <lcthw/dbg.h>
#include <stdlib.h>

List *List_create() { return calloc(1, sizeof(List)); }

void List_destroy(List *list) {
  check_list(list);
  ListNode *cur, *next_cur;

  for (cur = list->first; cur != NULL; cur = next_cur) {
    next_cur = cur->next;
    free(cur);
  }

  free(list);
error:
  return;
}

void List_clear(List *list) {
  check_list(list);
  LIST_FOREACH(list, first, next, cur) { free(cur->value); }

error:
  return;
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
error:
  return;
}

void List_push_node(List *list, ListNode *node) {
  check_list(list);
  if (list->count++) {
    node->prev = list->last;
    node->next = NULL;
    list->last->next = node;
    list->last = node;
  } else {
    node->prev = NULL;
    node->next = NULL;
    list->first = node;
    list->last = node;
  }
error:
  return;
}

ListNode *List_shift_node(List *list) {
  check_list(list);
  ListNode *node = list->first;
  if (node == NULL) {
    return NULL;
  }
  // already last node, after shift, list should be empty
  if (!(--(list->count))) {
    list->last = NULL;
  }
  list->first = node->next;
  node->prev = NULL;
  node->next = NULL;
  return node;

error:
  return NULL;
}

void List_push(List *list, void *value) {
  check_list(list);
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);
  node->value = value;
  List_push_node(list, node);

error:
  return;
}

void *List_pop(List *list) {
  check_list(list);
  ListNode *node = list->last;
  return node != NULL ? List_remove(list, node) : NULL;

error:
  return NULL;
}

void List_unshift(List *list, void *value) { return List_push(list, value); }

void *List_shift(List *list) {
  check_list(list);
  ListNode *node = list->first;
  return node != NULL ? List_remove(list, node) : NULL;

error:
  return NULL;
}

void *List_remove(List *list, ListNode *node) {
  check_list(list);

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
  void *result = node->value;
  free(node);
  return result;

error:
  return NULL;
}

// copy from_list to to_list, then return to_list
// will create a new list!
List *List_copy(List *from_list) {
  List *to_list = List_create();
  check(to_list != NULL, "Failed create new list.");

  check_list(from_list);

  LIST_FOREACH(from_list, first, next, cur) { List_push(to_list, cur->value); }

  check(List_count(from_list) == List_count(to_list), "Failed copy to list");

  return to_list;

error:
  List_destroy(to_list);
  return NULL;
}

// Concat from_list to to_list
// After concat, from_list will be an empty list.
void List_concat(List *to_list, List *from_list) {
  check_list(to_list);
  check_list(from_list);

  ListNode *a = to_list->last;
  ListNode *b = from_list->first;

  // 若from_list为空，直接返回
  if (b == NULL) {
    return;
  }

  // to_list为空，需要同时设置first和last
  if (a == NULL) {
    to_list->first = b;
    // to_list不为空，连接a, b
  } else {
    b->prev = a;
    a->next = b;
  }

  to_list->last = from_list->last;
  to_list->count += from_list->count;

  // to_list、from_list均非空，正常拼接
  from_list->first = NULL;
  from_list->last = NULL;
  from_list->count = 0;

error:
  return;
}

// Split from_list to from_list and to_list
// to_list->first should be node
// will create a new list!
// if node not in from_list, from_list will stay the same,
// while to_list will be an empty list.
List *List_split(List *from_list, ListNode *node) {
  List *to_list = List_create();
  check(to_list != NULL, "Failed create new list.");

  check_list(from_list);

  int i = 0;
  LIST_FOREACH(from_list, first, next, cur) {
    if (cur == node) {
      to_list->first = cur;
      to_list->last = from_list->last;
      to_list->count = List_count(from_list) - i;

      if (cur->prev == NULL) {
        from_list->first = NULL;
      } else {
        cur->prev->next = NULL;
      }
      from_list->last = cur->prev;
      cur->prev = NULL;
      from_list->count = i;
      break;
    }
    i++;
  }

  return to_list;

error:
  List_destroy(to_list);
  return NULL;
}

ListNode *List_next(ListNode *n, int i) {
  while (i--) {
    if (n == NULL)
      return NULL;
    n = n->next;
  }
  return n;
}

// get list[n]
ListNode *List_get(List *list, int i) {
  check_list(list);
  return List_next(list->first, i);
error:
  return NULL;
}
