#include <stdio.h>
#include <stdlib.h>

#define N(i) 

int next[100];
int prev[100];
int dead[100];

int main(int argc, char *argv[])
{
  if (argc != 3) {
    return -1;
  }
  int n = atoi(argv[1]);
  int m = atoi(argv[2])-1;
  int x, i, j;
  // 初始化环形链表
  for (i = 0; i < n; i++) {
    next[i] = i+1;
    prev[i] = i-1;
    dead[i] = 0;
  }
  next[n-1] = 0;
  prev[0] = n-1;

  x = 0;
  for (i = 0; i < n-1; i++) {
    for (j = 0; j < m; j++) {
      x = next[x];
    }

    // 杀了x
    prev[next[x]] = prev[x];
    next[prev[x]] = next[x];
    dead[x] = 1;

    // 从下一个人开始数
    x = next[x];
  }
  printf("%d活下来了\n", x);
}
