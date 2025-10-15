#include <stdio.h>
#include <string.h>
char *last_str = NULL;

char *my_strtok_r(char *str, const char *delim, char **saveptr) {
  // if str is NULL, use saved ptr
  if (str == NULL)
    str = *saveptr;
  // if both str and saved ptr is NULL, return NULL
  if (str == NULL)
    return NULL;

  // skip head char in delim
  str += strspn(str, delim);
  // after skip, if str is empty, return NULL and set saved ptr NULL
  if (*str == '\0') {
    *saveptr = NULL;
    return NULL;
  }

  char *c = strpbrk(str, delim);
  if (c != NULL) {
    *c = '\0';
    *saveptr = c + 1;
    return str;
  }
  *saveptr = NULL;
  return str;
}

char *my_strtok(char *str, const char *delim) {
  return my_strtok_r(str, delim, &last_str);
}

int main(int argc, char *argv[]) {
  // char str[] = ":root:x::0:root:/root:/bin/bash:";
  // char *token;

  // token = my_strtok(str, ":");
  // printf("%s\n", token);
  // while ((token=my_strtok(NULL, ":")) != NULL) {
  // printf("%s\n", token);
  // }

  char str[] = "a/bbb///cc;xxx:yyy:";
  char *str1 = str;
  char delim1[] = ":;";
  char delim2[] = "/";

  char *str2, *token, *subtoken;
  char *saveptr1, *saveptr2;
  int j;

  for (j = 1;; j++, str1 = NULL) {
    token = my_strtok_r(str1, delim1, &saveptr1);
    if (token == NULL)
      break;
    printf("%d: %s\n", j, token);

    for (str2 = token;; str2 = NULL) {
      subtoken = my_strtok_r(str2, delim2, &saveptr2);
      if (subtoken == NULL)
        break;
      printf(" --> %s\n", subtoken);
    }
  }

  return 0;
}
