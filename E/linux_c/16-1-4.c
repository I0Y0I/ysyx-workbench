#include <stdio.h>
unsigned int rotate_right(unsigned int x, int y) {
  while (y--) {
    x = (x >> 1) | (x << 31);
  }
  return x;
}

int main(int argc, char *argv[]) {
  printf("%x >> %d = %x", 0xdeadbeef, 16, rotate_right(0xdeadbeef, 16));
  return 0;
}
