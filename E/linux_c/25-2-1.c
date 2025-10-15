#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  FILE *fp;
  fp = fopen("/etc/shadow", "r");
  if (fp == NULL) {
    perror("Open /etc/shadow");
    exit(1);
  }
  fp = fopen("/root", "r+");
  if (fp == NULL) {
    perror("Open /root");
    exit(1);
  }
  return EXIT_SUCCESS;
}
