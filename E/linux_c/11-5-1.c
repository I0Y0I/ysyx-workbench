#include <stdio.h>
#include <stdlib.h>

int a[10];

int find_min() {
  int min = a[0], i;
  for (i = 1; i < 10; i++) {
    if (a[i] < min) {
      min = a[i];
    }
  }
  return min;
}

int main(int argc, char *argv[]) {
  int i;
  for (i = 0; i < 10; i++) {
    a[i] = rand();
    printf("%d ", a[i]);
  }
  printf("\n");
  printf("%d", find_min());
  return 0;
}
