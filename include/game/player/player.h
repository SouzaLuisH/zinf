#ifndef PLAYER_C
#define PLAYER_C

#include "game_def.h"

int player_init_status(Player* player);

// movimentation
int player_movimentation(Player *player,unsigned char pressed_key);
int handle_player_movement(Player *player, unsigned char pressed_key);
void get_player_initial_position(Player *player, char map[][24], int lenght, int height);

// weapon
bool player_has_weapon(Player *player);
bool is_player_weapon_active(Player *player);
bool handle_player_weapon(Player *player, unsigned char pressed_key);

// lives
int get_player_lives(Player* player);
int set_player_lives(Player* player,int n_lives);
int increment_player_lives(Player* player,int n_lives);

//graphic
void draw_player(Player* player);

#endif //PLAYER_C