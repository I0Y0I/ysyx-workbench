#include <stdio.h>
typedef void (*callback_t)(void *);
void repeat(callback_t f, void *param) {
  f(param);
  f(param);
}

void hello(void *str) { printf("Hello, %s\n", (char *)str); }

int main(int argc, char *argv[]) {
  repeat(hello, "ioyoi");
  return 0;
}
