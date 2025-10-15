#include <stdio.h>

int main(int argc, char *argv[]) {
  int x = 0, y = 0, n = 4;

  while (1) {
    scanf("%d", &x);
    y = x / n + (x % n == 0 ? 0 : 1);
    printf("%d\n", y);
  }

  return 0;
}
