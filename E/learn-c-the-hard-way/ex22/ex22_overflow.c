#include <stdio.h>

void f(int x) {
  x++;
  printf("%d\n", x);
  f(x);
}

int main(int argc, char *argv[]) {
  int x = 0;
  f(x);
  return 0;
}
