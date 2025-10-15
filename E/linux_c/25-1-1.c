#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
  char * buf;
  buf = strdup("hello_world\n");
  memcpy(buf+1, buf, 13);
  printf("%s", buf);
  free(buf);
  buf = strdup("hello_world\n");
  memmove(buf+1, buf, 13);
  printf("%s", buf);
  free(buf);
  return 0;
}
