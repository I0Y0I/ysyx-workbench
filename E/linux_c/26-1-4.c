#include <stdio.h>
#include <stdlib.h>
typedef struct Node {
  struct Node * next;
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int secend;
} Node;

Node * head;
char buffer[100];

Node * make_node() {
  int i, year, month, day, hour, minute, secend;
  sscanf(buffer, "%d %d-%d-%d %d:%d:%d", &i, &year, &month, &day, &hour, &minute, &secend);
  Node * n = malloc(sizeof(Node));
  n->year = year;
  n->month = month;
  n->day = day;
  n->hour = hour;
  n->minute = minute;
  n->secend = secend;
  return n;
}

int less_than(Node * a, Node * b) {
  if (a->hour != b->hour) return a->hour < b->hour;
  if (a->minute != b->minute) return a->minute < b->minute;
  if (a->secend != b->secend) return a->secend < b->secend;
  if (a->year != b->year) return a->year < b->year;
  if (a->month != b->month) return a->month < b->month;
  if (a->day != b->day) return a->day < b->day;
  return 0;
}

void insert(Node * n) {
  Node * prev = NULL;
  Node * curr = head;
  while (curr && less_than(curr, n)) {
    prev = curr;
    curr = curr->next;
  }

  if (prev) {
    prev->next = n;
    n->next = curr;
  } else {
    head = n;
    n->next = curr;
  }
}

int main(int argc, char *argv[])
{
  FILE * inFile;
  inFile = fopen("in.txt", "r");

  while (fgets(buffer, 100, inFile) != NULL) {
    insert(make_node());
  }
  fclose(inFile);

  FILE * outFile;
  outFile = fopen("out.txt", "w");

  Node * n = head;
  int i = 1;
  while (n) {
    fprintf(outFile, "%d %d-%d-%d %d:%d:%d\n", i, n->year, n->month, n->day, n->hour, n->minute, n->secend);
    n = n->next;
    i++;
  }
  fclose(outFile);

  return EXIT_SUCCESS;
}
