#include <ctype.h>
#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char cwd_path[PATH_MAX];
char lib_path[] = "/usr/include/";
char buf_path[PATH_MAX];

char *ltrim(char *s) {
  while (isspace(*s)) {
    s++;
  }
  return s;
}

char *rtrim(char *s) {
  char *back = s + strlen(s);
  while (isspace(*--back) && back != s)
    ;
  *(back + 1) = '\0';
  return s;
}

char *trim(char *s) { return rtrim(ltrim(s)); }

int load_includes(const char *s);

int find_std_includes(const char *s) {
  strcpy(buf_path, lib_path);
  strncat(buf_path, s, PATH_MAX - strlen(buf_path));
  return load_includes(buf_path);
}

int find_usr_includes(const char *s) {
  strcpy(buf_path, cwd_path);
  strncat(buf_path, s, PATH_MAX - strlen(buf_path));
  return load_includes(buf_path);
}

int load_includes(const char *s) {
  FILE *f = fopen(s, "r");
  if (f == NULL) {
    perror(buf_path);
    return 1;
  }
  puts(s);

  char buf[200];
  while (fgets(buf, 200, f)) {
    if (strncmp(buf, "#include", 8) == 0) {
      char *name = trim(buf + 8);
      if (name[0] == '<') {
        *(strchr(name, '>')) = '\0';
        find_std_includes(name + 1);
      } else if (name[0] == '"') {
        *(strchr(name + 1, '"')) = '\0';
        find_usr_includes(name + 1);
      }
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    errno = EINVAL;
    return -1;
  }

  realpath(argv[1], cwd_path);
  *(strrchr(cwd_path, '/') + 1) = '\0';
  printf("cwd:%s\n", cwd_path);

  load_includes(argv[1]);
  return 0;
}
