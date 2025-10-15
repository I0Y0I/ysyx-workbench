#include <math.h>
#include <stdio.h>
double myround(double x) {
  double abs = x < 0 ? -x : x;
  double c = ceil(abs);
  double f = floor(abs);

  if (c - abs > abs - f)
    return x < 0 ? -f : f;
  return x < 0 ? -c : c;
}

int main(int argc, char *argv[]) {
  printf("%lf, %lf, %lf, %lf", myround(-3.51), myround(4.49), myround(-3.5),
         myround(3.5));
  return 0;
}
