#include "enemies.h"
#include "graphic.h"
#include "stdlib.h"

int monster_init_status(Enemies *monster)
{
    monster->lives = 1;
    monster->orientation = SOUTH;                          
    monster->position.x =0;
    monster->position.y = 0; 
    monster->reward = 99*((float)rand()/RAND_MAX); +1;
    monster->isEnable = true;
    monster->last_change_time =0;
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
                monsters[index].position.x = j * TILE_SIZE;
                monsters[index].position.y = i * TILE_SIZE;
                index++;
            }
        }
    }
}

