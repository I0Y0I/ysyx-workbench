#include <stdio.h>

int fib(int n) {
  int a = 1, b = 1;
  int i;
  for (i = 1; i < n; i++) {
    b = a + b;
    a = b - a;
  }
  return b;
}

int main() {
  printf("%d", fib(50));
  return 0;
}
