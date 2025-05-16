#include "enemies.h"
#include "graphic.h"
#include "stdlib.h"

int monster_init_status(Enemies *Monster)
{
   

    Monster->lives = 1;
    Monster->orientation = SOUTH;                           // TODO random orientation
    Monster->position.x = 1150 * ((float)rand() / RAND_MAX); // TODO get position from text map
    Monster->position.y = 750 * ((float)rand() / RAND_MAX); // TODO get position from text map
    Monster->reward = 100;                                  // TODO random reward

    return 1;
}


void draw_monster(Enemies *Monster)
{
    float x_coord = Monster->position.x;
    float y_coord = Monster->position.y;
    driver_draw_square(x_coord,y_coord,50,3);
}