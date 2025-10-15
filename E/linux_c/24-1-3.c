#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  char *s = NULL;
  int i = 0;
  while (1) {
    i++;
    printf("%dMB\n", i);
    s = malloc(1024 * 1024);
    *s = '\0';
  }
  return EXIT_SUCCESS;
}
