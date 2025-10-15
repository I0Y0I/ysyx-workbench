#include <stdio.h>
#include <stdlib.h>
int a[100] = {};

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

void quicksort(int start, int end) {
  int mid;
  if (end > start) {
    mid = partition(start, end);
    quicksort(start, mid - 1);
    quicksort(mid + 1, end);
  }
}

int main(int argc, char *argv[]) {
  int i;
  for (i = 0; i < 100; i++) {
    a[i] = rand();
  }
  quicksort(0, 99);
  for (i = 0; i < 100; i++) {
    printf("%08X ", a[i]);
    if (i % 4 == 3) {
      printf("\n");
    }
  }
  return 0;
}
