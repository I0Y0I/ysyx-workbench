#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int quit_sig = 0;

void quit_handler(int sig) { quit_sig = 1; }

int get_n() {
  FILE *f = fopen("test.txt", "r");
  if (f == NULL) {
    return 0;
  }
  char last_line[30];
  char line_buffer[30];

  while (fgets(line_buffer, 30, f)) {
    strncpy(last_line, line_buffer, 30);
  }
  fclose(f);

  int n = 0;
  sscanf(last_line, "%d", &n);
  return n;
}

void write_line(FILE *f, int n) {
  // Write n
  fprintf(f, "%d ", n);

  time_t timestamp;
  struct tm *timeinfo;
  time(&timestamp);
  timeinfo = localtime(&timestamp);

  char buffer[21];
  strftime(buffer, 21, "%Y-%m-%d %H:%M:%S\n", timeinfo);
  fputs(buffer, f);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, quit_handler);

  int n = get_n();

  FILE *f;
  f = fopen("test.txt", "a+");

  while (!quit_sig) {
    n++;
    write_line(f, n);
    sleep(1);
  }

  fclose(f);

  return 0;
}
