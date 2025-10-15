#include <stdio.h>
#include <stdlib.h>
typedef struct Node {
  int x;
  struct Node * next;
} Node;

Node * head = NULL;
Node * tail = NULL;

Node * make_node(int x) {
  Node * n = malloc(sizeof(Node));
  n->x = x;
  n->next = NULL;
  return n;
}

void free_node(Node * n) {
  free(n);
}

void enqueue(int x) {
  Node * n = make_node(x);
  if (tail) {
    tail->next = n;
  }
  tail = n;
  if (!head) {
    head = n;
  }
}

int dequeue() {
  if (!head) {
    return -1;
  }
  Node * n = head;
  head = head->next;
  if (tail == n) {
    tail = NULL;
  }
  int x = n->x;
  free_node(n);
  return x;
}

int main(int argc, char *argv[])
{
  enqueue(1);
  enqueue(2);
  printf("%d\n", dequeue());
  enqueue(3);
  printf("%d\n", dequeue());
  printf("%d\n", dequeue());
  printf("%d\n", dequeue());
  return EXIT_SUCCESS;
}
