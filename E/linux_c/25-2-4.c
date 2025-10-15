// 拷贝二进制文件时会删除所有的\0
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
  char buf[100];
  while(fgets(buf, 100, fileA)) {
    fputs(buf, fileB);
  }
  fclose(fileA);
  fclose(fileB);
  return EXIT_SUCCESS;
}
