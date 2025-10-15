#include <stdio.h>
void diamond_n(int x, char c, int n) {
  int i;
  if (n == x) {
    for (i = 0; i < n; i++)
      printf("%c", c);
    printf("\n");
    return;
  }
  for (i = 0; i < (x - n) >> 1; i++)
    printf(" ");
  for (i = 0; i < n; i++)
    printf("%c", c);
  for (i = 0; i < (x - n) >> 1; i++)
    printf(" ");
  printf("\n");
  diamond_n(x, c, n + 2);
  for (i = 0; i < (x - n) >> 1; i++)
    printf(" ");
  for (i = 0; i < n; i++)
    printf("%c", c);
  for (i = 0; i < (x - n) >> 1; i++)
    printf(" ");
  printf("\n");
}

void diamond(int x, char c) {
  if (x % 2 == 0) {
    printf("Even Error!\n");
    return;
  }
  diamond_n(x, c, 1);
}

int main(int argc, char *argv[]) {
  diamond(3, '*');
  diamond(5, '+');
  return 0;
}
