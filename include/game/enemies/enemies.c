#include "enemies.h"
#include "graphic.h"
#include "stdlib.h"

int monster_init_status(Enemies *Monster)
{
    Monster->lives = 1;
    Monster->orientation = SOUTH;                           // TODO random orientation
    Monster->position.x =0; // TODO get position from text map
    Monster->position.y = 0; // TODO get position from text map
    Monster->reward = 99*((float)rand()/RAND_MAX); +1;// TODO random reward
    Monster->isEnable = true;
    return 1;
}


void fill_monster_init_data(Enemies *monsters, char map[][MAP_WIDTH], int width, int height, char target_char)
{
    int index = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (map[i][j] == target_char)
            {
                monster_init_status(&monsters[index]);
                monsters[index].position.x = j * ITEM_SIZE;
                monsters[index].position.y = i * ITEM_SIZE;
                index++;
            }
        }
    }
}




void draw_monster(Enemies *Monster)
{
    float x_coord = Monster->position.x;
    float y_coord = Monster->position.y;
    driver_draw_square(x_coord,y_coord,50,3);
}