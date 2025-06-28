#ifndef GRAPHIC_H
#define GRAPHIC_H

#define PLAYER_SPRITES_PATH "../../resources/textures/player/"
#define MONSTER_SPRITES_PATH "../../resources/textures/monster/"
#define ELEMENTS_SPRITES_PATH "../../resources/textures/elements/"




float get_frame_time();
void driver_print_statusboard(int player_lives,int player_score, int game_stage, int width, int height);
void driver_print_text(char *text, int x, int y, int color);
void driver_draw_square(float x, float y, float side_lenght, int color);
void driver_print_player(float x, float y, int orientation);
void driver_print_monster(float x, float y, int orientation);
void driver_print_element(float x, float y, char element);
void driver_unload_all_textures();
void driver_load_all_textures();


#endif // GRAPHIC_H


