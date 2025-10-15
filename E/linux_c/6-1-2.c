#include <stdio.h>
int main(int argc, char *argv[]) {
  int i, a = 0;
  for (i = 1; i < 100; i++) {
    if (i / 10 == 9) {
      a++;
    }
    if (i % 10 == 9) {
      a++;
    }
  }
  printf("%d", a);
}
