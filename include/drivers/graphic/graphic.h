#ifndef GRAPHIC_H
#define GRAPHIC_H

#define PLAYER_SPRITES_PATH "../../include/drivers/graphic/textures/player/"
#define MONSTER_SPRITES_PATH "../../include/drivers/graphic/textures/monster/"



float get_frame_time();
void driver_print_text(char *text, int x, int y, int color);
void driver_draw_square(float x, float y, float side_lenght, int color);
void driver_print_player(float x, float y, int orientation);
void driver_monster_player(float x, float y, int orientation);
void driver_unload_all_textures();
void driver_load_all_textures();


#endif // GRAPHIC_H


