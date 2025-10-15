#include <stdio.h>
#include <stdlib.h>
#define N 100000

int a[N];

void gen_random(int upper_bound, int down_bound) {
  int i;
  for (i = 0; i < N; i++)
    a[i] = rand() % (upper_bound - down_bound + 1) + down_bound;
}

int howmany(int value) {
  int count = 0, i;
  for (i = 0; i < N; i++)
    if (a[i] == value)
      ++count;
  return count;
}

int main(void) {
  int i;

  gen_random(20, 10);
  printf("value\thow many\n");
  for (i = 10; i < 21; i++)
    printf("%d\t%d\n", i, howmany(i));

  return 0;
}
