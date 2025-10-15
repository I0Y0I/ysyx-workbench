#include <stdio.h>
int main(int argc, char *argv[])
{
  int ch;
  while ((ch = getchar()) != EOF) {
    printf("Now!\n");
  }
  return 0;
}
