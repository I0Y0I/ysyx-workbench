#include <stdio.h>
unsigned int multiply(unsigned int x, unsigned int y) {
  unsigned int ans = 0;
  while (y) {
    if (y & 1)
      ans += x;
    x <<= 1;
    y >>= 1;
  }
  return ans;
}

int main(int argc, char *argv[]) {
  printf("%d x %d = %d\n", 10, 12, multiply(10, 12));
  return 0;
}
