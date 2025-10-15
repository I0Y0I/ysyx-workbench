#include <stdio.h>
#include <stdlib.h>
char s[] = "10000abcdefg";

int main(int argc, char *argv[]) {
  long int a, b;
  char *c;
  a = atoi(s);
  b = strtol(s, &c, 10);
  perror("");
  printf("%ld %ld %s\n", a, b, c);
  return EXIT_SUCCESS;
}
