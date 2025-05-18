#ifndef ENEMIES_H
#define ENEMIES_H
#include "game_def.h"

int monster_init_status(Enemies* Monster);
void fill_monster_init_data(Enemies *monsters, char map[][MAP_WIDTH], int width, int height, char target_char);
void draw_monster(Enemies *Monster);

#endif // ENEMIES_H