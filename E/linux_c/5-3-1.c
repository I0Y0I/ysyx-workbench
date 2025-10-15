#include <stdio.h>

int gcd(int a, int b) {
  if (a % b == 0)
    return b;
  return gcd(b, a % b);
}

int main(int argc, char *argv[]) {
  int a, b;
  while (1) {
    scanf("%d %d", &a, &b);
    if (a > b)
      printf("%d\n", gcd(a, b));
    else
      printf("%d\n", gcd(b, a));
  }
  return 0;
}
