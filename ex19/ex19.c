#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "game_mech.h"

int Map_init(void *self) {
    assert(self != NULL);
    Map *map = self;

    Room *hall = NEW(Room, "The great Hall");
    Room *throne = NEW(Room, "The throne room");
    Room *arena = NEW(Room, "The arena, with the minotaur");
    Room *kitchen = NEW(Room, "Kitchen, you have the knife now");
    Room *bathroom = NEW(Room, "The bathroom with dirtymonster");

    arena->bad_guy = NEW(Monster, "The evil minotaur");
    bathroom->bad_guy = NEW(Monster, "Ugly smelling dirtymonster");

    hall->north = throne;

    throne->west = arena;
    throne->east = kitchen;
    throne->south = hall;
    throne->north = bathroom;

    arena->east = throne;
    kitchen->west = throne;
    bathroom->south = throne;

    map->start = hall;
    map->location = hall;

    return 1;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    Map *game = NEW(Map, "The Hall of the Minotaur.");
    
    printf("You enter the ");
    game->location->_(describe)(game->location);

    while (process_input(game)) {}

    return 0;
}

