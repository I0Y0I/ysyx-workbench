#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
char *shrink_space(char *dest, const char *src, size_t n) {
  size_t i = 0, j = 0;
  int lastempty = 0;
  while (i < n && src[j] != '\0') {
    if (isblank(src[j])) {
      if (lastempty) {
        j++;
      } else {
        dest[i++] = ' ';
        j++;
        lastempty = 1;
      }
    } else {
      dest[i++] = src[j++];
      lastempty = 0;
    }
  }
  while (i < n) {
    dest[i++] = '\0';
  }
  return dest;
}

char s[] = "This Content hoho       is ok\
        ok?\
\
        file system\
uttered words   ok ok      ?\
end.";
char d[100];

int main(int argc, char *argv[]) {
  puts(shrink_space(d, s, 100));
  return 0;
}
