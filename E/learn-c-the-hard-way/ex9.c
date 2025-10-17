#include <stdio.h>

int main(int argc, char *argv[]) {
  int numbers[4] = {0};
  char name[4] = {'a'};

  // first, print them out raw
  printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2],
         numbers[3]);

  printf("name each: %c %c %c %c\n", name[0], name[1], name[2], name[3]);

  printf("name: %s\n", name);

  // setup the numbers
  numbers[0] = 1;
  numbers[1] = 2;
  numbers[2] = 3;
  numbers[3] = 4;

  // setup the name
  name[0] = 'Z';
  name[1] = 'e';
  name[2] = 'd';
  name[3] = '\0';

  // then print them out initialized
  printf("numbers: %d %d %d %d\n", numbers[0], numbers[1], numbers[2],
         numbers[3]);

  printf("name each: %c %c %c %c\n", name[0], name[1], name[2], name[3]);

  // print the name like a string
  printf("name: %s\n", name);

  // another way to use name
  char *another = "Zed";

  printf("another: %s\n", another);

  printf("another each: %c %c %c %c\n", another[0], another[1], another[2],
         another[3]);

  // 尝试用字符给int赋值
  // 没有任何警告
  numbers[0] = 'a';
  numbers[1] = 'b';
  numbers[2] = 'c';
  numbers[3] = '\0';
  printf("numbers: %c %c %c %c\n", numbers[0], numbers[1], numbers[2],
         numbers[3]);

  // 尝试用int给char赋值
  // 只有转换溢出时会有警告
  // Valgrind不会报错
  name[0] = 1;
  name[1] = 2;
  name[2] = 3;
  name[3] = 100000;
  printf("name each: %d %d %d %d\n", name[0], name[1], name[2], name[3]);

  name[3] = '\0';
  // 实际存储方式：
  // 3 2 1 0 (字节)
  // 0 3 2 1
  printf("black magic: %d\n", *(int *)name);
  printf("should be: %d\n", (3 << 16) + (2 << 8) + 1);

  // 当然可以工作，他们是等价的！
  name[0] = 'Z';
  name[1] = 'e';
  name[2] = 'd';
  name[3] = '\0';
  printf("%s\n", (char *)name);

  return 0;
}
