#include <stdio.h>
#include <stdlib.h>
typedef struct Node {
  int x;
  struct Node * next;
} Node;

Node * head;

Node * make_node(int x) {
  Node * n = malloc(sizeof(Node));
  n->x = x;
  n->next = NULL;
  return n;
}

void free_node(Node * n) {
  free(n);
}

Node * search(Node * head, int x) {
  while (head) {
    if (head->x == x) {
      return head;
    }
    head = head->next;
  }
  return NULL;
}

void insert(Node * n) {
  n->next = head;
  head = n;
}

void insertX(int x) {
  Node * n = make_node(x);
  Node * cur = head;
  Node * pre = NULL;
  while (cur && cur->x < x) {
    pre = cur;
    cur = cur->next;
  }

  if (pre) {
    n->next = pre->next;
    pre->next = n;
  } else {
    insert(n);
  }
}

void delete(Node * n) {
  if (head == n) {
    head = n->next;
    free_node(n);
    return;
  }
  Node * pre = head;
  while(pre) {
    if (pre->next == n) {
      pre->next = n->next;
      free_node(n);
    }
  }
}

void traverse(void (*visit) (Node *)) {
  Node * n = head;
  while (n) {
    visit(n);
    n = n->next;
  }
}

void destroy(void) {
  Node * n = head;
  Node * m;
  while (n) {
    m = n;
    n = n->next;
    free_node(m);
  }
  head = NULL;
}

void print(void) {
  Node * n = head;
  while (n) {
    printf("%d ", n->x);
    n = n->next;
  }
  printf("\n");
}

void reverse() {
  Node * n = head;
  Node * pre = NULL;
  Node * next;
  while (n) {
    next = n->next;
    n->next = pre;
    pre = n;
    n = next;
  }
  head = pre;
}

int main(int argc, char *argv[])
{
  head = NULL;
  insertX(1);
  insertX(3);
  insertX(2);
  print();
  reverse();
  print();
  return 0;
}
