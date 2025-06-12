#ifndef PLAYER_C
#define PLAYER_C

#include "game_def.h"

int player_init_status(Player *player, bool keep_weapon);


// movimentation
int player_movimentation(Player *player,unsigned char pressed_key);
int move_player(Player *player,Vector2D* new_position);
void get_player_initial_position(Player *player, char map[][24], int lenght, int height);

// weapon
bool player_has_weapon(Player *player);
bool is_player_weapon_active(Player *player);
bool handle_player_weapon(Player *player, bool pressed_key);

// lives
int get_player_lives(Player* player);
int set_player_lives(Player* player,int n_lives);
int increment_player_lives(Player* player,int n_lives);

//graphic
void draw_player(Player* player);

#endif //PLAYER_C