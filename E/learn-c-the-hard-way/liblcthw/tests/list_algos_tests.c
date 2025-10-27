#include "lcthw/dbg.h"
#include "lcthw/list.h"
#include "minunit.h"
#include <lcthw/list_algos.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};

#define N 10
int num_values[N];
#define NUM_VALUES 5

List *create_words() {
  int i = 0;
  List *words = List_create();

  for (i = 0; i < NUM_VALUES; i++) {
    List_push(words, values[i]);
  }

  return words;
}

int is_sorted(List *words) {
  LIST_FOREACH(words, first, next, cur) {
    if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
      debug("%s %s", (char *)cur->value, (char *)cur->next->value);
      return 0;
    }
  }

  return 1;
}

char *test_bubble_sort() {
  List *words = create_words();

  // should work on a list that needs sorting
  int rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort failed.");
  mu_assert(is_sorted(words), "Words are not sorted after bubble sort.");

  // should work on an already sorted list
  rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort of already sorted failed.");
  mu_assert(is_sorted(words), "Words should be sort if already bubble sorted.");

  List_destroy(words);

  // should work on an empty list
  words = List_create();
  rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort failed on empty list.");
  mu_assert(is_sorted(words), "Words should be sorted if empty.");

  List_destroy(words);

  return NULL;
}

char *test_merge_sort_recu() {
  List *words = create_words();

  // should work on a list that needs sorting
  List *res = List_merge_sort_recu(words, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

  List *res2 = List_merge_sort_recu(res, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
  List_destroy(res2);
  List_destroy(res);

  List_destroy(words);
  return NULL;
}

char *test_merge_sort_iter() {
  List *words = create_words();

  // should work on a list that needs sorting
  List *res = List_merge_sort_iter(words, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Words are not sorted after merge sort.");

  List *res2 = List_merge_sort_iter(res, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Should still be sorted after merge sort.");
  List_destroy(res2);
  List_destroy(res);

  List_destroy(words);
  return NULL;
}

int int_cmp(int *a, int *b) { return *a - *b; }

void init_random_num() {
  int i;
  for (i = 0; i < N; i++) {
    num_values[i] = rand();
  }
}

List *create_nums(int n) {
  List *list = List_create();

  while (n--) {
    List_push(list, &num_values[n]);
  }

  return list;
}

char *tb_sort() {
  init_random_num();
  int i;
  List *list;
  time_t start_time, end_time;
  for (i = 10; i <= N; i *= 2) {
    list = create_nums(i);
    start_time = clock();
    List_bubble_sort(list, (List_compare)int_cmp);
    end_time = clock();
    log_info("bubble sort %d items used %f s\n", i,
             (double)(end_time - start_time) / CLOCKS_PER_SEC);
    List_destroy(list);

    list = create_nums(i);
    start_time = clock();
    List_merge_sort_recu(list, (List_compare)int_cmp);
    end_time = clock();
    log_info("merge sort %d items used %f s\n", i,
             (double)(end_time - start_time) / CLOCKS_PER_SEC);
    List_destroy(list);

    list = create_nums(i);
    start_time = clock();
    List_merge_sort_iter(list, (List_compare)int_cmp);
    end_time = clock();
    log_info("merge iter sort %d items used %f s\n", i,
             (double)(end_time - start_time) / CLOCKS_PER_SEC);
    List_destroy(list);
  }

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_bubble_sort);
  mu_run_test(test_merge_sort_recu);
  mu_run_test(test_merge_sort_iter);
  mu_run_test(tb_sort);

  return NULL;
}

RUN_TESTS(all_tests);
