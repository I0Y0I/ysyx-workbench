#include <stdio.h>
void increment(int *x) { (*x)++; }

int main(int argc, char *argv[]) {
  int i = 1, j = 2;
  increment(&i);
  increment(&j);
  printf("%d %d", i, j);
  return 0;
}
