#include <stdio.h>
#include <stdlib.h>
int a[10] = {};

void swap(int x, int y) {
  int temp = a[x];
  a[x] = a[y];
  a[y] = temp;
}

int partition(int start, int end) {
  int mid = start++;
  for (; start <= end; start++) {
    if (a[start] < a[mid]) {
      swap(mid, start);
      mid++;
      swap(mid, start);
    }
  }
  return mid;
}

int order_statistic(int start, int end, int k) {
  int i;
  i = partition(start, end);
  if (k - 1 == i)
    return a[i];
  else if (k - 1 > i)
    return order_statistic(i + 1, end, k - i);
  else
    return order_statistic(start, i - 1, k);
}

int main(int argc, char *argv[]) {
  int i;
  for (i = 0; i < 10; i++) {
    a[i] = rand() % 10;
  }
  for (i = 0; i < 10; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
  printf("%d", order_statistic(0, 9, 3));
  return 0;
}
