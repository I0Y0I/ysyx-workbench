#include <stdio.h>
struct rational {
  int x, y;
};

int gcm(int x, int y) {
  if (x < 0)
    return gcm(-x, y);
  if (x < y)
    return gcm(y, x);
  if (x % y == 0)
    return y;
  return gcm(y, x % y);
}

struct rational make_rational(int x, int y) {
  struct rational r;
  r.x = x;
  r.y = y;
  if (r.x == 0) {
    r.y = 1;
    return r;
  }
  if (r.y < 0) {
    r.x = -r.x;
    r.y = -r.y;
  }
  int g = gcm(r.x, r.y);
  r.x /= g;
  r.y /= g;
  return r;
}

void print_rational(struct rational r) {
  if (r.y == 1)
    printf("%d\n", r.x);
  else
    printf("%d/%d\n", r.x, r.y);
}

struct rational add_rational(struct rational a, struct rational b) {
  return make_rational(a.x * b.y + a.y * b.x, a.y * b.y);
}

struct rational sub_rational(struct rational a, struct rational b) {
  return make_rational(a.x * b.y - a.y * b.x, a.y * b.y);
}

struct rational mul_rational(struct rational a, struct rational b) {
  return make_rational(a.x * b.x, a.y * b.y);
}

struct rational div_rational(struct rational a, struct rational b) {
  return make_rational(a.x * b.y, a.y * b.x);
}

int main(void) {
  struct rational a = make_rational(1, 8);  /* a=1/8 */
  struct rational b = make_rational(-1, 8); /* b=-1/8 */
  print_rational(add_rational(a, b));
  print_rational(sub_rational(a, b));
  print_rational(mul_rational(a, b));
  print_rational(div_rational(a, b));

  return 0;
}
