#include <math.h>
#include <stdio.h>

struct complex_struct {
  double x, y;
};

double real_part(struct complex_struct z) { return z.x; }

double img_part(struct complex_struct z) { return z.y; }

double magnitude(struct complex_struct z) {
  return sqrt(z.x * z.x + z.y * z.y);
}

double angle(struct complex_struct z) { return atan2(z.y, z.x); }

struct complex_struct make_from_real_img(double x, double y) {
  struct complex_struct z;
  z.x = x;
  z.y = y;
  return z;
}

struct complex_struct make_from_mag_ang(double r, double A) {
  return make_from_real_img(r * cos(A), r * sin(A));
}

struct complex_struct add_complex(struct complex_struct z1,
                                  struct complex_struct z2) {
  return make_from_real_img(real_part(z1) + real_part(z2),
                            img_part(z1) + img_part(z2));
}

struct complex_struct sub_complex(struct complex_struct z1,
                                  struct complex_struct z2) {
  return make_from_real_img(real_part(z1) - real_part(z2),
                            img_part(z1) - img_part(z2));
}

struct complex_struct mul_complex(struct complex_struct z1,
                                  struct complex_struct z2) {
  return make_from_mag_ang(magnitude(z1) * magnitude(z2),
                           angle(z1) + angle(z2));
}

struct complex_struct div_complexr(struct complex_struct z1,
                                   struct complex_struct z2) {
  return make_from_mag_ang(magnitude(z1) * magnitude(z2),
                           angle(z1) + angle(z2));
}

void print_complex(struct complex_struct z) {
  if (img_part(z) == 0.) {
    printf("%f\n", real_part(z));
    return;
  }
  if (real_part(z) == 0.) {
    printf("%fi\n", img_part(z));
    return;
  }
  printf("%f%+fi\n", real_part(z), img_part(z));
}

int main(int argc, char *argv[]) {
  struct complex_struct a, b, c, d;
  a = make_from_real_img(1., 0.);
  b = make_from_real_img(0., -2.);
  c = make_from_real_img(-1.0, 2.0);
  d = make_from_real_img(1.0, -2.0);
  print_complex(a);
  print_complex(b);
  print_complex(c);
  print_complex(d);
  return 0;
}
