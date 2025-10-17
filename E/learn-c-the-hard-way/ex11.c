#include <stdio.h>

int main(int argc, char *argv[]) {
  // go through each string in argv

  int i = 0;
  while (i < argc) {
    printf("arg %d: %s\n", i, argv[i]);
    i++;
  }

  // 倒序执行
  i = argc;
  while (i) {
    i--;
    printf("arg %d: %s\n", i, argv[i]);
  }

  // let's make our own array of strings
  char *states[] = {"California", "Oregon", "Washington", "Texas"};

  int num_states = 4;
  i = 0; // watch for this
  while (i < num_states) {
    printf("state %d: %s\n", i, states[i]);
    i++;
  }

  // 复制，其实复制的是指针
  i = 0;
  while (i < argc && i < num_states) {
    states[i] = argv[i];
    i++;
  }

  i = 0; // watch for this
  while (i < num_states) {
    printf("state %d: %s\n", i, states[i]);
    i++;
  }

  return 0;
}
