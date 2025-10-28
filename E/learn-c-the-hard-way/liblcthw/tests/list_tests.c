#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>
#include <stdlib.h>

char *test1 = "This is test1";
char *test2 = "This is test2";
char *test3 = "This is test3";
char *test4 = "This is test4";

List *create_and_push() {
  List *list = List_create();
  List_push(list, test1);
  List_push(list, test2);
  List_push(list, test3);
  List_push(list, test4);
  return list;
}

char *test_create_destroy() {
  List *list = create_and_push();
  mu_assert(list != NULL, "failed to create list.");

  List_destroy(list);
  return NULL;
}

char *test_push_shift_node() {
  List *list1 = create_and_push();
  List *list2 = List_create();

  List_push_node(list2, List_shift_node(list1));
  List_push_node(list2, List_shift_node(list1));
  List_push_node(list2, List_shift_node(list1));
  List_push_node(list2, List_shift_node(list1));

  mu_assert(List_count(list1) == 0, "Wrong count after shift 4 nodes.");
  mu_assert(List_count(list2) == 4, "Wrong count after push 4 nodes.");

  char *val = List_pop(list2);
  mu_assert(val == test4, "Wrong value on pop.");
  val = List_pop(list2);
  mu_assert(val == test3, "Wrong value on pop.");
  val = List_pop(list2);
  mu_assert(val == test2, "Wrong value on pop.");
  val = List_pop(list2);
  mu_assert(val == test1, "Wrong value on pop.");

  List_destroy(list1);
  List_destroy(list2);
  return NULL;
}

char *test_push_pop() {
  List *list = List_create();

  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value after push.");
  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value after push.");
  List_push(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value after push.");
  List_push(list, test4);
  mu_assert(List_last(list) == test4, "Wrong last value after push.");

  mu_assert(List_count(list) == 4, "Wrong count after push 4 values.");

  char *val = List_pop(list);
  mu_assert(val == test4, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");

  mu_assert(List_count(list) == 0, "Wrong count after pop 4 values.");

  List_destroy(list);
  return NULL;
}

char *test_shift_unshift() {
  List *list = List_create();

  List_unshift(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value after unshift.");
  List_unshift(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value after unshift.");
  List_unshift(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value after unshift.");
  List_unshift(list, test4);
  mu_assert(List_last(list) == test4, "Wrong last value after unshift.");

  mu_assert(List_count(list) == 4, "Wrong count after unshift 4 values.");

  char *val = List_shift(list);
  mu_assert(val == test1, "Wrong value on shift.");
  val = List_shift(list);
  mu_assert(val == test2, "Wrong value on shift.");
  val = List_shift(list);
  mu_assert(val == test3, "Wrong value on shift.");
  val = List_shift(list);
  mu_assert(val == test4, "Wrong value on shift.");

  mu_assert(List_count(list) == 0, "Wrong count after shift 4 values.");

  List_destroy(list);
  return NULL;
}

char *test_remove() {
  List *list = create_and_push();

  // 删除位于中间的元素
  mu_assert(List_remove(list, list->first->next) == test2,
            "Wrong return value on remove.");
  mu_assert(List_count(list) == 3, "Wrong count after remove.");
  // 删除第一个元素
  mu_assert(List_remove(list, list->first) == test1,
            "Wrong return value on remove.");
  mu_assert(List_count(list) == 2, "Wrong count after remove.");
  // 删除最后一个元素
  mu_assert(List_remove(list, list->last) == test4,
            "Wrong return value on remove.");
  mu_assert(List_count(list) == 1, "Wrong count after remove.");
  // 删除唯一的元素
  mu_assert(List_remove(list, list->last) == test3,
            "Wrong return value on remove.");
  mu_assert(List_count(list) == 0, "Wrong count after remove.");

  List_destroy(list);
  return NULL;
}

char *test_copy() {
  List *list1 = create_and_push();

  List *list2 = List_copy(list1);
  mu_assert(list2 != NULL, "Failed to copy list1 to list2.");

  // list1 should stay the same
  mu_assert(List_count(list1) == 4, "Wrong count after copy.");
  char *val = List_pop(list1);
  mu_assert(val == test4, "Wrong value after copy.");
  val = List_pop(list1);
  mu_assert(val == test3, "Wrong value after copy.");
  val = List_pop(list1);
  mu_assert(val == test2, "Wrong value after copy.");
  val = List_pop(list1);
  mu_assert(val == test1, "Wrong value after copy.");

  mu_assert(List_count(list2) == 4, "Wrong count on copy.");
  val = List_pop(list2);
  mu_assert(val == test4, "Wrong value on copy.");
  val = List_pop(list2);
  mu_assert(val == test3, "Wrong value on copy.");
  val = List_pop(list2);
  mu_assert(val == test2, "Wrong value on copy.");
  val = List_pop(list2);
  mu_assert(val == test1, "Wrong value on copy.");

  List_destroy(list1);
  List_destroy(list2);
  return NULL;
}

char *test_concat() {
  List *list1 = List_create();
  List *list2 = List_create();
  List_push(list1, test1);
  List_push(list1, test2);
  List_push(list2, test3);
  List_push(list2, test4);

  List_concat(list1, list2);
  mu_assert(List_count(list1) == 4, "Wrong count of list1 after concat.");
  mu_assert(List_count(list2) == 0, "Wrong count of list2 after concat.");

  char *val = List_pop(list1);
  mu_assert(val == test4, "Wrong value after pop.");
  val = List_pop(list1);
  mu_assert(val == test3, "Wrong value after pop.");
  val = List_pop(list1);
  mu_assert(val == test2, "Wrong value after pop.");
  val = List_pop(list1);
  mu_assert(val == test1, "Wrong value after pop.");

  List_destroy(list1);
  List_destroy(list2);
  return NULL;
}

char *test_split() {
  List *list1 = create_and_push();
  List *list2 = List_split(list1, list1->first->next);

  mu_assert(List_count(list1) == 1, "Wrong count of list1 after split.");
  mu_assert(List_count(list2) == 3, "Wrong count of list2 after split.");

  char *val = List_pop(list2);
  mu_assert(val == test4, "Wrong list2 value after split.");
  val = List_pop(list2);
  mu_assert(val == test3, "Wrong list2 value after split.");
  val = List_pop(list2);
  mu_assert(val == test2, "Wrong list2 value after split.");
  val = List_pop(list1);
  mu_assert(val == test1, "Wrong list1 value after split.");

  List_destroy(list1);
  List_destroy(list2);
  return NULL;
}

char *test_next() {
  List *list = create_and_push();

  mu_assert(List_next(list->first, 2)->value == test3,
            "test1's next 2nd Node should be test3.");
  mu_assert(List_next(list->first, 4) == NULL,
            "test1's next 4th Node should be NULL.");

  List_destroy(list);
  return NULL;
}

char *test_get() {
  List *list = create_and_push();

  mu_assert(List_get(list, 0)->value == test1, "list[0] should be test1.");
  mu_assert(List_get(list, 2)->value == test3, "list[2] should be test3.");
  mu_assert(List_get(list, 4) == NULL, "list[4] should be NULL.");

  List_destroy(list);

  return NULL;
}

char *all_tests() {
  mu_suite_start();
  mu_run_test(test_create_destroy);
  mu_run_test(test_push_pop);
  mu_run_test(test_shift_unshift);
  mu_run_test(test_push_shift_node);
  mu_run_test(test_remove);
  mu_run_test(test_copy);
  mu_run_test(test_concat);
  mu_run_test(test_split);
  mu_run_test(test_next);
  mu_run_test(test_get);
  return NULL;
}

RUN_TESTS(all_tests);
