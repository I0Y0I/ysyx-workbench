#include <stdio.h>
double mypow(double x, int n) {
  if (n == 0)
    return 1;
  if (n % 2 == 0) {
    double x2 = mypow(x, n / 2);
    return x2 * x2;
  } else
    return x * mypow(x, n - 1);
}

int main(int argc, char *argv[]) {
  printf("%f\n", mypow(2., 4));
  printf("%f\n", mypow(2., 5));
  return 0;
}
