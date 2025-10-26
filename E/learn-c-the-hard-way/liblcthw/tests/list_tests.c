#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static List *list = NULL;
static List *list2 = NULL;
char *test1 = "This is test1";
char *test2 = "This is test2";
char *test3 = "This is test3";
char *test4 = "This is test4";

char *test_create() {
  list = List_create();
  mu_assert(list != NULL, "failed to create list.");
  return NULL;
}

char *test_destroy() {
  List_clear_destroy(list);
  List_clear_destroy(list2);
  return NULL;
}

char *test_push_pop() {
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
  return NULL;
}

char *test_shift_unshift() {
  List_unshift(list, test1);
  mu_assert(List_first(list) == test1, "Wrong first value after unshift.");
  List_unshift(list, test2);
  mu_assert(List_first(list) == test2, "Wrong first value after unshift.");
  List_unshift(list, test3);
  mu_assert(List_first(list) == test3, "Wrong first value after unshift.");
  List_unshift(list, test4);
  mu_assert(List_first(list) == test4, "Wrong first value after unshift.");

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
  return NULL;
}

char *test_remove() {
  List_push(list, test1);
  List_push(list, test2);
  List_push(list, test3);
  List_push(list, test4);

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

  return NULL;
}

char *test_copy() {
  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value after push.");
  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value after push.");
  List_push(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value after push.");
  List_push(list, test4);
  mu_assert(List_last(list) == test4, "Wrong last value after push.");

  mu_assert(List_count(list) == 4, "Wrong count after push 4 values.");

  list2 = List_copy(list);
  mu_assert(list2 != NULL, "Failed to copy list to list2.");

  char *val = List_pop(list2);
  mu_assert(val == test4, "Wrong value on pop.");
  val = List_pop(list2);
  mu_assert(val == test3, "Wrong value on pop.");
  val = List_pop(list2);
  mu_assert(val == test2, "Wrong value on pop.");
  val = List_pop(list2);
  mu_assert(val == test1, "Wrong value on pop.");

  mu_assert(List_count(list2) == 0, "Wrong count after pop 4 values.");

  List_pop(list);
  List_pop(list);
  List_pop(list);
  List_pop(list);
  mu_assert(List_count(list) == 0, "Wrong count after pop 4 values.");
  return NULL;
}

char *test_concat() {
  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value after push.");
  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value after push.");
  List_push(list2, test3);
  mu_assert(List_last(list2) == test3, "Wrong last value after push.");
  List_push(list2, test4);
  mu_assert(List_last(list2) == test4, "Wrong last value after push.");

  List_concat(list, list2);
  mu_assert(List_count(list) == 4, "Wrong count after concat.");

  char *val = List_pop(list);
  mu_assert(val == test4, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");

  mu_assert(List_count(list) == 0, "Wrong count after pop 4 values.");
  return NULL;
}

char *test_split() {
  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value after push.");
  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value after push.");
  List_push(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value after push.");
  List_push(list, test4);
  mu_assert(List_last(list) == test4, "Wrong last value after push.");

  list2 = List_split(list, list->first->next);
  mu_assert(List_count(list) == 1, "Wrong count of list after split.");
  mu_assert(List_count(list2) == 3, "Wrong count of list2 after split.");


  char *val = List_pop(list2);
  mu_assert(val == test4, "Wrong value on pop.");
  val = List_pop(list2);
  mu_assert(val == test3, "Wrong value on pop.");
  val = List_pop(list2);
  mu_assert(val == test2, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");
  return NULL;
}

char *all_tests() {
  mu_suite_start();
  mu_run_test(test_create);
  mu_run_test(test_push_pop);
  mu_run_test(test_shift_unshift);
  mu_run_test(test_remove);
  mu_run_test(test_copy);
  mu_run_test(test_concat);
  mu_run_test(test_split);
  mu_run_test(test_destroy);
  return NULL;
}

RUN_TESTS(all_tests);
