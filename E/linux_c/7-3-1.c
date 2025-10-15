#include <complex.h>
#include <math.h>
#include <stdio.h>
enum coordinate_type { RECTANGULAR, POLAR };
typedef struct complex_struct {
  enum coordinate_type t;
  double a, b;
} complex_struct;

struct complex_struct make_from_real_img(double x, double y) {
  struct complex_struct z;
  z.t = RECTANGULAR;
  z.a = x;
  z.b = y;
  return z;
}

struct complex_struct make_from_mag_ang(double r, double A) {
  struct complex_struct z;
  z.t = POLAR;
  z.a = r;
  z.b = A;
  return z;
}

double real_part(const complex_struct z) {
  return z.t == RECTANGULAR ? z.a : z.a * cos(z.b);
}

double img_part(const complex_struct z) {
  return z.t == RECTANGULAR ? z.b : z.a * sin(z.b);
}

double magnitude(const complex_struct z) {
  return z.t == RECTANGULAR ? sqrt(z.a * z.a + z.b * z.b) : z.a;
}

double angle(const complex_struct z) {
  return z.t == RECTANGULAR ? atan(z.b / z.a) : z.b;
}

int main(int argc, char *argv[]) {
  complex_struct z1 = make_from_real_img(3., 4.);
  complex_struct z2 = make_from_mag_ang(2., 3.1415926 / 3);
  printf("%f %f %f %f\n", real_part(z1), img_part(z1), magnitude(z1),
         angle(z1));
  printf("%f %f %f %f\n", real_part(z2), img_part(z2), magnitude(z2),
         angle(z2));
  return 0;
}
