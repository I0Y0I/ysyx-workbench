#include <stdio.h>
int main(int argc, char *argv[]) {
  int x, y, z, r = 0;
  for (x = 2; x < 5; x++) {
    for (y = 2; y < 5; y++) {
      for (z = 2; z < 5; z++) {
        if (x < 3 && y > 3)
          r = (r + 1) << 1;
        // else if (x >= 3 && y >= 3)
        // r = (r + 1) << 1;
        else if (z > 3 && x >= 3)
          r = (r + 1) << 1;
        else if (z <= 3 && y >= 3)
          r = (r + 1) << 1;
        else
          r = r << 1;
      }
    }
  }

  printf("%X", r);
  return 0;
}

// 没想到简单方法，画表填色求解。
