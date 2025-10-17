#include <stdio.h>

int main(int argc, char *argv[]) {
  // let's make our own array of strings
  char *states[] = {"California", "Oregon", "Washington", "Texas"};
  // 超出数组范围，恰好打印了一个(null)也就是0指针。
  int num_states = 5;

  // 居然是可以赋值的，argv不是const的！
  // argv[0] = states[0];

  int i = 0;
  // go through each string in argv
  // why am I skipping argv[0]?
  // argv[0] 是shell的第一个参数，也就是可执行文件名，但不是exe的第一个参数
  for (i = 0; i < argc; i++) {
    printf("arg %d: %s\n", i, argv[i]);
  }

  states[0] = argv[0];
  for (i = 0; i < num_states; i++) {
    printf("state %d: %s\n", i, states[i]);
  }

  return 0;
}

// python传入参数似乎不会被崩溃，但fish只允许我传入最多2097152个参数
// 通过`getconf ARG_MAX`
