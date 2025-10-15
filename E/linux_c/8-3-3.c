#include <stdio.h>
#include <stdlib.h>
#define N 4
#define M 2
int a[N] = {1, 2, 3, 4};
int buf[M];

void printl(const int buf[]) {
  for (int i = 0; i < M; i++) {
    printf("%d ", buf[i]);
  }
  putchar('\n');
}

// already select x num, now decide if buf[x] = a[y]
void run(int x, int y) {
  if (x == M) {
    printl(buf);
    return;
  }

  // decide can a[y] not in buf
  if (M - x < N - y)
    run(x, y + 1);
  buf[x] = a[y];
  run(x + 1, y + 1);
}

int main(int argc, char *argv[]) {
  run(0, 0);
  return EXIT_SUCCESS;
}
