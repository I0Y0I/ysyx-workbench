#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 4
int a[N] = {1, 2, 3, 4};

void swap(int *a, int *b) {
  int c = *a;
  *a = *b;
  *b = c;
}

void printl(const int buf[]) {
  for (int i = 0; i < N; i++) {
    printf("%d ", buf[i]);
  }
  putchar('\n');
}

// Now at x, should swap(x, y)
void run(int x, const int buf[]) {
  if (x == N - 1) {
    printl(buf);
    return;
  }

  int y, new_buf[N];
  for (y = x; y < N; y++) {
    memcpy(new_buf, buf, N * sizeof(int));
    swap(&new_buf[x], &new_buf[y]);
    run(x + 1, new_buf);
  }
}

int main(int argc, char *argv[]) {
  run(0, a);
  return EXIT_SUCCESS;
}
