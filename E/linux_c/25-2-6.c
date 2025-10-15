#include <asm-generic/errno-base.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
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

int is_empty_line(char *s) {
  int len = strlen(s);
  for (int i = 0; i < len; i++) {
    if (!isspace(*s)) {
      return 0;
    }
    s++;
  }
  return 1;
}

int main(int argc, char *argv[]) {
  FILE *in = fopen("25-2-6.ini", "r");
  if (in == NULL) {
    perror("25-2-6.ini");
    return 1;
  }

  FILE *out = fopen("25-2-6.xml", "w+");
  if (out == NULL) {
    perror("25-2-6.xml");
    return 1;
  }

  char buffer[200];
  char section[200];
  while (fgets(buffer, 200, in)) {
    // 空行
    if (is_empty_line(buffer)) {
      fputs("</", out);
      fputs(section, out);
      fputs(">\n", out);
      continue;
    }
    switch (buffer[0]) {
    // 注释行
    case ';':
      fputs("<!-- ", out);
      fputs(trim(buffer + 1), out);
      fputs(" -->\n", out);
      break;
    // Section行
    case '[':
      strcpy(section, buffer + 1);
      *(strchr(section, ']')) = '\0';
      fputs("<", out);
      fputs(section, out);
      fputs(">\n", out);
      break;
    default:
      char *delim = strchr(buffer, '=');
      *delim = '\0';
      char *key = trim(buffer);
      char *value = trim(delim + 1);
      fprintf(out, "\t<%s>%s</%s>\n", key, value, key);
    }
  }

  fputs("</", out);
  fputs(section, out);
  fputs(">\n", out);

  fclose(in);
  fclose(out);
  return 0;
}
