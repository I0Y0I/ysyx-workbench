#include <stdio.h>
char a[4][3][2] = {{{'a', 'b'}, {'c', 'd'}, {'e', 'f'}},
                   {{'g', 'h'}, {'i', 'j'}, {'k', 'l'}},
                   {{'m', 'n'}, {'o', 'p'}, {'q', 'r'}},
                   {{'s', 't'}, {'u', 'v'}, {'w', 'x'}}};

char (*pa)[2] = &a[1][0];
char (*ppa)[3][2] = &a[1];

int main(int argc, char *argv[]) {
  putchar(pa[5][1]);
  putchar(ppa[1][2][1]);
  return 0;
}
