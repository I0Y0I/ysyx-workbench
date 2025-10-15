#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 20

int a[N];

void gen_random(int upper_bound) {
  srand(time(NULL));
  int i;
  for (i = 0; i < N; i++)
    a[i] = rand() % upper_bound;
}

int howmany(int value) {
  int count = 0, i;
  for (i = 0; i < N; i++)
    if (a[i] == value)
      ++count;
  return count;
}

int main(void) {
  int i, nomore = 0, histogram[10] = {0};

  gen_random(10);
  printf("value\thow many\n");
  for (i = 0; i < N; i++)
    histogram[a[i]]++;

  for (i = 0; i < 10; i++) {
    putchar('0' + i);
    putchar(' ');
  }
  putchar('\n');

  while (!nomore) {
    nomore = 1;
    for (i = 0; i < 10; i++) {
      (histogram[i]--) > 0 ? putchar('*') : putchar(' ');
      if (histogram[i] > 0)
        nomore = 0;
      putchar(' ');
    }
    putchar('\n');
  }

  return 0;
}
