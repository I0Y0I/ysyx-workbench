#ifndef _ex19_h
#define _ex19_h

#include "object.h"

struct Monster {
  Object proto;
  int hit_points;
};

typedef struct Monster Monster;

int Monster_attack(void *self, int damage);
int Monster_init(void *self);

Monster *Monster_New(char *description);

struct Room {
  Object proto;

  Monster *bad_guy;

  struct Room *north;
  struct Room *south;
  struct Room *east;
  struct Room *west;
};

typedef struct Room Room;

void *Room_move(void *self, Direction direction);
int Room_attack(void *self, int damage);
int Room_init(void *self);

Room *Room_New(char *description);

struct Map {
  Object proto;
  Room *start;
  Room *location;
};

typedef struct Map Map;

void *Map_move(void *self, Direction direction);
int Map_attack(void *self, int damage);
int Map_init(void *self);

Map *Map_New(char *description);

int process_input(Map *game);
#endif
