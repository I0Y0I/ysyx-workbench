#include <stdio.h>
#include <string.h>
char *my_strcpy(char *restrict dst, const char *restrict src) {
  memcpy(dst, src, (strlen(src) + 1) * sizeof(char));
  return dst;
}

char s[] = "Hello!\n";
char a[10], b[10];

int main(int argc, char *argv[]) {
  puts(strcpy(a, s));
  puts(strcpy(b, s));
  return 0;
}
