#include <stdio.h>
int main(int argc, char *argv[]) {
  int i = 0xcffffff3;
  // 这是uint的右移，无论如何都补0
  printf("%x\n", 0xcffffff3 >> 2);
  // 这是int的右移，首位为1，补的是1
  printf("%x\n", i >> 2);
  return 0;
}
