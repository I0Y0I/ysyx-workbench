#include <stdio.h>

void read_by_value(int x) { printf("read_by_value:%d\n", x); }

void read_by_ptr(int *x) { printf("read_by_value:%d\n", *x); }

void set_by_value(int x) { x++; }

void set_by_ptr(int *x) { (*x)++; }

int main(int argc, char *argv[]) {
  static int x = 0;
  int *p = &x;
  set_by_value(x);
  read_by_value(x);
  set_by_ptr(p);
  read_by_ptr(p);
  return 0;
}
