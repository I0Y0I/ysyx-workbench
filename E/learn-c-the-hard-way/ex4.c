#include <stdio.h>
int main(int argc, char *argv[]) {
  int age = 25;
  int height = 185;

  // 我注意到valgrind没有检查到这个错误
  // 所以valgrind实际上是不检查未定义行为的
  printf("I am %d years old.\n");
  printf("I am %d inches tall.\n", height);

  return 0;
}
