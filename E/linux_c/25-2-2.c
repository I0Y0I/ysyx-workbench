#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  if (argc != 3) {
    printf("Usage: ./25-2-2 fileA fileB\n");
    exit(1);
  }
  FILE *fileA, *fileB;
  fileA = fopen(argv[1], "r");
  if (!fileA) {
    perror(argv[1]);
    exit(1);
  }
  fileB = fopen(argv[2], "w+");
  if (!fileB) {
    perror(argv[2]);
    exit(1);
  }
  int ch;
  while ((ch = getc(fileA)) != EOF) {
    putc(ch, fileB);
  }
  fclose(fileA);
  fclose(fileB);
  return EXIT_SUCCESS;
}
