#include "lcthw/list_algos.h"
#include "minunit.h"
#include "lcthw/list.h"
#include "lcthw/list_sorted.h"

char *test1 = "This is test1";
char *test2 = "This is test2";
char *test3 = "This is test3";
char *test4 = "This is test4";

char *test_insert_before() {
  List *list = List_create();

  // Insert to NULL
  List_insert_before(list, NULL, test1);
  // Insert to first
  List_insert_before(list, List_get(list, 0), test2);
  // Insert to last
  List_insert_before(list, NULL, test3);
  // Insert to middle
  List_insert_before(list, List_get(list, 1), test4);

  // The order should be test2->test4->test1->test3
  mu_assert(List_count(list) == 4, "Wrong count after push 4 nodes.");
  char *val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test4, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop.");

  List_destroy(list);
  return NULL;
}

char *test_insert_sorted() {
  List *list = List_create();

  List_insert_sorted(list, test4, (List_compare)strcmp);
  List_insert_sorted(list, test3, (List_compare)strcmp);
  List_insert_sorted(list, test2, (List_compare)strcmp);
  List_insert_sorted(list, test1, (List_compare)strcmp);

  // The order should be test1->test2->test3->test4
  mu_assert(List_count(list) == 4, "Wrong count after push 4 nodes.");
  char *val = List_pop(list);
  mu_assert(val == test4, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop.");
  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");

  List_destroy(list);
  return NULL;
}

char *all_tests() {
  mu_suite_start();
  mu_run_test(test_insert_before);
  mu_run_test(test_insert_sorted);
  return NULL;
}

RUN_TESTS(all_tests);
