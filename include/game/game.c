#include "game_def.h"
#include "player.h"
#include "enemies.h"
#include "graphic.h"
#include <stdio.h>
#include <stdlib.h>

Player Link;
Game_State Map_Data;

// ----------------- TEMPORARIO ----------------//
char archive[16][24] = {
    "     M        PPPP      ",
    "              M P       ",
    "               P        ",
    "       M    V PP        ",
    "          PPPP          ",
    "                 M      ",
    "              M         ",
    "                        ",
    "       J   P            ",
    "   M      P             ",
    "         E P            ",
    "PPPPPPPPPPPP            ",
    "                        ",
    "              M         ",
    "                       V",
    "                        ",
};

int get_color_for_map_char(char c)
{
    switch (c)
    {
    case ' ':
        return MAP_COLOR_EMPTY;
    case 'M':
        return MAP_COLOR_MONSTER;
    case 'E':
        return MAP_COLOR_WEAPON;
    case 'V':
        return MAP_COLOR_LIFE;
    case 'P':
        return MAP_COLOR_WALL;
    default:
        return MAP_COLOR_UNKNOWN;
    }
}

void draw_map(Game_State *state, int width, int height)
{
    for (int i = 0; i < state->n_lives; i++)
    {
        driver_draw_square(state->lives[i].x, state->lives[i].y, ITEM_SIZE, MAP_COLOR_LIFE);
    }

    for (int i = 0; i < state->n_monsters; i++)
    {
        driver_draw_square(state->monsters[i].position.x, state->monsters[i].position.y, ITEM_SIZE, MAP_COLOR_MONSTER);
    }

    for (int i = 0; i < state->n_walls; i++)
    {
        driver_draw_square(state->walls[i].x, state->walls[i].y, ITEM_SIZE, MAP_COLOR_WALL);
    }

    for (int i = 0; i < state->n_weapons; i++)
    {
        driver_draw_square(state->weapons[i].x, state->weapons[i].y, ITEM_SIZE, MAP_COLOR_WEAPON);
    }
}

//------------ INITIAL MAP FUNCITONS ----------------//
void fill_element_positions(Vector2D *position, char map[][MAP_WIDTH], int width, int height, char target_char)
{
    int index = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (map[i][j] == target_char)
            {
                position[index].x = j * ITEM_SIZE;
                position[index].y = i * ITEM_SIZE;
                index++;
            }
        }
    }
}

void get_initial_position_of_all_elements(Game_State *map, char map_char[][MAP_WIDTH], int width, int height)
{
    map->n_lives = 0;
    map->n_walls = 0;
    map->n_weapons = 0;
    map->n_monsters = 0;

    map->lives = NULL;
    map->monsters = NULL;
    map->walls = NULL;
    map->weapons = NULL;

    // get number of each elements
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            char c = map_char[i][j];
            if (c == MAP_LIFE_SPACE)
                map->n_lives++;
            else if (c == MAP_MONSTER_SPACE)
                map->n_monsters++;
            else if (c == MAP_WALL_SPACE)
                map->n_walls++;
            else if (c == MAP_WEAPON_SPACE)
                map->n_weapons++;
        }
    }
    // malloc the elements
    map->lives = (Vector2D *)malloc(sizeof(Vector2D) * map->n_lives);
    map->monsters = (Enemies *)malloc(sizeof(Enemies) * map->n_monsters);
    map->walls = (Vector2D *)malloc(sizeof(Vector2D) * map->n_walls);
    map->weapons = (Vector2D *)malloc(sizeof(Vector2D) * map->n_weapons);

    if (!map->lives || !map->monsters || !map->walls || !map->weapons)
    {
        fprintf(stderr, "Memory Alocation Error:[get_position_of_all_elements] .\n");
        exit(EXIT_FAILURE);
    }

    // declarar valores
    fill_element_positions(map->lives, map_char, width, height, MAP_LIFE_SPACE);
    fill_element_positions(map->walls, map_char, width, height, MAP_WALL_SPACE);
    fill_element_positions(map->weapons, map_char, width, height, MAP_WEAPON_SPACE);
    fill_monster_init_data(map->monsters, map_char, width, height, MAP_MONSTER_SPACE);
}

void free_all_elements(Game_State *map)
{
    free(map->lives);
    free(map->monsters);
    free(map->walls);
    free(map->weapons);
}
//----------------------------------------//

bool check_wall_colision(Vector2D position, Game_State *map)
{
    for (int i = 0; i < map->n_walls; i++)
    {
        if (position.x == map->walls[i].x && position.y == map->walls[i].y)
        {
            return true;
        }
    }

    if(position.x <0 || position.x > 24*ITEM_SIZE) return true;
    if(position.y <0 || position.y > 16*ITEM_SIZE) return true;

    return false;
}

void handle_player_movement(Player *player, Game_State *map, char key_pressed)
{
    float x_component = key_pressed == 'd' ? 1.00 : key_pressed == 'a' ? -1.00 : 0;
    float y_component = key_pressed == 'w' ? -1.00 : key_pressed == 's' ? 1.00 : 0;

    if (x_component == 0 & y_component == 0) return;

    Vector2D new_player_pos = {player->position.x + x_component * DEFAULT_PLAYER_VELOCITY,
                               player->position.y + y_component * DEFAULT_PLAYER_VELOCITY};

    if (check_wall_colision(new_player_pos, map) == false)
    {
        move_player(player, key_pressed);
    }
}

//------------------------------------//

int init_game_data()
{
    // TODO read map file, to set the positions of the enemies and player
    // TODO save map in matrix
    player_init_status(&Link);
    get_player_initial_position(&Link, archive, MAP_HEIGHT, MAP_WIDTH);
    get_initial_position_of_all_elements(&Map_Data, archive, MAP_WIDTH, MAP_HEIGHT);
    if (DEBUG_PRINTS)
    {
        printf("\nMonstros: %d, Vidas: %d, Armas: %d, Paredes: %d\n",
               Map_Data.n_monsters, Map_Data.n_lives, Map_Data.n_weapons, Map_Data.n_walls);
    }
    return 1;
}

void game_loop(char move)
{
    handle_player_movement(&Link, &Map_Data, move);
    handle_player_weapon(&Link, move);

    draw_map(&Map_Data, MAP_WIDTH, MAP_HEIGHT);
    draw_player(&Link);
}

int finish_game()
{
    free_all_elements(&Map_Data);

    return 1;
}