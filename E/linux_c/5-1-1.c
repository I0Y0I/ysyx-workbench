#include <stdbool.h>
#include <stdio.h>

bool is_leap_year(int year) {
  if (year % 4 == 0 && year % 100 != 0)
    return 1;
  return 0;
}

int main(int argc, char *argv[]) {
  printf("%b %b %b", is_leap_year(1999), is_leap_year(2000),
         is_leap_year(2004));
  return 0;
}
