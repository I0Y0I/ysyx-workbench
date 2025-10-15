#include <stdio.h>
void print_seperate(int x) {
  if (x < 0)
    x = -x;
  printf("%d, %d\n", x % 10, (x / 10) % 10);
}

int main(int argc, char *argv[]) {
  print_seperate(123);
  print_seperate(-123);
  return 0;
}
