#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("ERROR: You need at least one argument.\n");
    // this is how you abort a program
    return 1;
  }

  int i = 0, j = 0;
  char letter;
  for (j = 1; j < argc; j++) {
    for (i = 0; (letter = argv[j][i]) != '\0'; i++) {
      // char letter = argv[1][i];

      switch (letter) {
      case 'a':
      case 'A':
        printf("%d: 'A'\n", i);
        break;

      case 'e':
      case 'E':
        printf("%d: 'E'\n", i);
        // 删除这个，当触发e时也会打印“I”
        break;

      case 'i':
      case 'I':
        printf("%d: 'I'\n", i);
        break;

      case 'o':
      case 'O':
        printf("%d: 'O'\n", i);
        break;

      case 'u':
      case 'U':
        printf("%d: 'U'\n", i);
        break;

      case 'y':
      case 'Y':
        if (i > 2) {
          // it's only sometimes Y
          printf("%d: 'Y'\n", i);
          // 放这里比较合适
          break;
        }
        // 我觉得这个不该加啊，不然当Y处于第一个时就不会被打印
        // break;

      default:
        printf("%d: %c is not a vowel\n", i, letter);
      }
    }
    puts("");
  }
  return 0;
}
