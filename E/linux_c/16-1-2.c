#include <stdio.h>
int countbit(unsigned int x) {
  int n = 0;
  while (x) {
    if (x & 1)
      n++;
    x >>= 1;
  }
  return n;
}

int main(int argc, char *argv[]) {
  printf("%d\n", countbit(0b00101010100000100001));
  return 0;
}
