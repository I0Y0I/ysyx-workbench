#include <stdio.h>
#include <ctype.h>

// forward declarations
void print_letters(char arg[]);
void print_nums(char arg[]);
void print_others(char arg[]);

void print_arguments(int argc, char *argv[]) {
  int i = 0;

  for (i = 0; i < argc; i++) {
    print_letters(argv[i]);
    print_nums(argv[i]);
    print_others(argv[i]);
  }
}

void print_letters(char arg[]) {
  int i = 0;

  for (i = 0; arg[i] != '\0'; i++) {
    char ch = arg[i];

    if (isalpha(ch) || isblank(ch)) {
      printf("'%c' == %d ", ch, ch);
    }
  }

  printf("\n");
}

void print_nums(char arg[]) {
  int i = 0;

  for (i = 0; arg[i] != '\0'; i++) {
    char ch = arg[i];
    if (isdigit(ch) || isblank(ch)) {
      putchar(ch);
    }
  }
  printf("\n");
}

void print_others(char arg[]) {
  int i = 0;

  for (i = 0; arg[i] != '\0'; i++) {
    char ch = arg[i];
    if (!isdigit(ch) && !isalpha(ch)) {
      putchar(ch);
    }
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  print_arguments(argc, argv);
  return 0;
}
