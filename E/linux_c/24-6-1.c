#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void myprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  const char *p = format;

  while (*p) {
    if (*p != '%') {
      putc(*p, stdout);
    } else {
      p++;
      switch (*p) {
      case '%': {
        putc('%', stdout);
        break;
      }
      case 'd': {
        int i = va_arg(args, int);
        printf("%d", i);
        break;
      }
      case 'f': {
        double d = va_arg(args, double);
        printf("%f", d);
        break;
      }
      default: {
        putc('%', stdout);
        putc(*p, stdout);
        break;
      }
      }
    }
    p++;
  }
  va_end(args);
}

int main(int argc, char *argv[]) {
  myprintf("%%-%d-%f\n", 1234, 1.234);
  return EXIT_SUCCESS;
}
