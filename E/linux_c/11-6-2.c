#include <stdio.h>
double mysqrt(double y) {
  double x = y / 2.;
  double dx = x / 2.;
  int i;

  for (i = 0; i < 18; i++) {
    double z = y - x * x;
    if (z < -0.001)
      x = x - dx;
    else if (z > 0.001)
      x = x + dx;
    else
      return x;
    dx /= 2.;
  }
  return x;
}

int main(int argc, char *argv[]) {
  printf("%.3f\n", mysqrt(2));
  printf("%.3f\n", mysqrt(3));
  return 0;
}
