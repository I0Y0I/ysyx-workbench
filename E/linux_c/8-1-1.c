#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]) {
  char a[10] = "qwer\0qwer\0";
  char b[10];
  memcpy(a, b, 10 * sizeof(char));
  char *i = &a[0];
  char *j = &b[0];
  for (int x = 0; x < 0; x++) {
    putchar(*i++);
    putchar(*j++);
    putchar('\n');
  }
  return 0;
}
