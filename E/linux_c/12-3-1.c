#include <stdio.h>
#define MAX_ROW 5
#define MAX_COL 5

typedef struct point {
  int x, y;
} point;

point p;
int dic;

typedef enum Status {
  PATH = 0,
  WALL,
  TRIED,
  FROM_LEFT,
  FROM_UP,
  FROM_RIGHT,
  FROM_BOTTOM,
  START
} Status;

char maze_symbol[] = " #x>V<Ao";

Status maze[MAX_ROW][MAX_COL] = {
    {7, 1, 0, 0, 0}, {0, 1, 0, 1, 0}, {0, 0, 0, 1, 0},
    {0, 1, 1, 1, 0}, {0, 0, 0, 1, 0},
};

void print_maze() {
  int i, j;
  printf("+-----+\n");
  for (i = 0; i < MAX_ROW; i++) {
    putchar('|');
    for (j = 0; j < MAX_COL; j++) {
      if (i == MAX_ROW - 1 && j == MAX_COL - 1)
        putchar('O');
      else
        putchar(maze_symbol[maze[i][j]]);
    }
    printf("|\n");
  }
  printf("+-----+\n");
}

Status get_stat(point *p) { return maze[p->x][p->y]; }

void set_stat(point *p, Status s) { maze[p->x][p->y] = s; }

int is_valid(point *p) {
  if (p->x < 0 || p->x >= MAX_ROW)
    return 0;
  if (p->y < 0 || p->y >= MAX_COL)
    return 0;
  return get_stat(p) == PATH;
}

int visit(point *p, int direction) {
  switch (direction) {
  case 0:
    p->y++;
    if (is_valid(p)) {
      set_stat(p, FROM_LEFT);
      return 1;
    } else {
      p->y--;
    }
    break;
  case 1:
    p->x++;
    if (is_valid(p)) {
      set_stat(p, FROM_UP);
      return 1;
    } else {
      p->x--;
    }
    break;
  case 2:
    p->y--;
    if (is_valid(p)) {
      set_stat(p, FROM_RIGHT);
      return 1;
    } else {
      p->y++;
    }
    break;
  case 3:
    p->x--;
    if (is_valid(p)) {
      set_stat(p, FROM_BOTTOM);
      return 1;
    } else {
      p->x++;
    }
    break;
  }
  return 0;
}

void back(point *p) {
  switch (get_stat(p)) {
  case FROM_LEFT:
    set_stat(p, TRIED);
    p->y--;
    break;
  case FROM_UP:
    set_stat(p, TRIED);
    p->x--;
    break;
  case FROM_RIGHT:
    set_stat(p, TRIED);
    p->y++;
    break;
  case FROM_BOTTOM:
    set_stat(p, TRIED);
    p->x++;
    break;
  default:;
  }
}

int is_start(point *p) { return p->x == 0 && p->y == 0; }

int is_end(point *p) { return p->x == MAX_ROW - 1 && p->y == MAX_COL - 1; }

int main(int argc, char *argv[]) {
  while (!is_end(&p)) {
    print_maze();
    // 首先尝试寻找下一个点
    int can_visit = 0;
    for (int i = 3; i >= 0; i--) {
      if (visit(&p, i)) {
        can_visit = 1;
        break;
      }
    }
    // 尝试完毕，出栈
    if (!can_visit && is_start(&p))
      return -1;

    if (!can_visit)
      back(&p);
  }
  return 0;
}
