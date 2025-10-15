#include <stdio.h>
#include <stdlib.h>

int a[10];

int find_min_2() {
  int min1 = a[0], min2 = a[1], i;
  for (i = 2; i < 10; i++) {
    if (a[i] < min1) {
      min2 = min1;
      min1 = a[i];
      continue;
    }
    if (a[i] < min2) {
      min2 = a[i];
    }
  }
  return min2;
}

int main(int argc, char *argv[]) {
  int i;
  for (i = 0; i < 10; i++) {
    a[i] = rand() % 10;
    printf("%d ", a[i]);
  }
  printf("\n");
  printf("%d", find_min_2());
  return 0;
}
