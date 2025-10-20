#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "object.h"

int Map_init(void *self) {
  assert(self != NULL);
  Map *map = self;

  // make some rooms for a small map
  Room *hall = Room_New("The great Hall");
  assert(hall != NULL);
  Room *throne = Room_New("The throne room");
  assert(throne != NULL);
  Room *arena = Room_New("The arena, with the minotaur");
  assert(arena != NULL);
  Room *kitchen = Room_New("Kitchen, you have the knife now");
  assert(kitchen != NULL);

  // put the bad guy in the arena
  arena->bad_guy = Monster_New("The evil minotaur");
  assert(arena->bad_guy != NULL);

  // setup the map rooms
  hall->north = throne;

  throne->west = arena;
  throne->east = kitchen;
  throne->south = hall;

  arena->east = throne;
  kitchen->west = throne;

  // start the map and the character off in the hall
  map->start = hall;
  map->location = hall;

  return 1;
}

int main(int argc, char *argv[]) {
  // simple way to setup the randomness
  srand(time(NULL));

  // make our map to work with
  Map *game = Map_New("The Hall of the Minotaur.");

  assert(game != NULL);

  printf("You enter the ");
  assert(game->location != NULL);
  game->location->_(describe)(game->location);

  while (process_input(game)) {
  }

  return 0;
}
