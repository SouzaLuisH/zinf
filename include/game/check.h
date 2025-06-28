#ifndef CHECK_C
#define CHECK_C

#include <stdint.h>
#include "game_def.h"

bool check_wall_colision(Vector2D position, Game_State *map);
bool check_life_colision(Vector2D position, int *life_position, Game_State *map);
bool check_weapon_colision(Vector2D position, int *weapon_position, Game_State *map);
bool check_monster_player_colision(Vector2D player_position, Vector2D monster_position);
bool check_monster_weapon_colision(Player *player, Vector2D monster_position);
bool check_win_condition(Game_State *map);
bool check_user_active_weapon(uint8_t input);

#endif