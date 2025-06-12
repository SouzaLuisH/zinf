#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "player.h"
#include "keyboard.h"
#include "enemies.h"
#include "graphic.h"

#include "game_def.h"

Player Link;
Game_State Map_Data;

// ----------------- TEMPORARIO ----------------//

char proto_mapa(char map[MAP_HEIGHT][MAP_WIDTH], char *arq_nome)
{
    int i = 0, j = 0;
    FILE *arq_map = fopen(arq_nome, "r");
    if (arq_map == NULL)
    {
        if (DEBUG_PRINTS)
        {
            printf("ERRO! O mapa está com algum problema.");
        }
        return 1;
    }

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        while (j < MAP_WIDTH)
        {
            char cont = fgetc(arq_map);

            if (cont != '\r' && cont != '\n' && cont != '\0')
            {
                map[i][j] = cont;
                j++;
            }
        }
        j = 0;
    }
}

void draw_map(Game_State *state, int width, int height)
{
    for (int i = 0; i < state->n_lives; i++)
    {
        if (state->lives[i].isEnable)
        {

            driver_draw_square(state->lives[i].position.x, state->lives[i].position.y, TILE_SIZE, MAP_COLOR_LIFE);
        }
    }

    for (int i = 0; i < state->n_monsters; i++)
    {
        if (state->monsters[i].isEnable)
        {
            driver_draw_square(state->monsters[i].position.x, state->monsters[i].position.y, TILE_SIZE, MAP_COLOR_MONSTER);
        }
    }

    for (int i = 0; i < state->n_walls; i++)
    {
        driver_draw_square(state->walls[i].x, state->walls[i].y, TILE_SIZE, MAP_COLOR_WALL);
    }

    for (int i = 0; i < state->n_weapons; i++)
    {
        if (state->weapons[i].isEnable)
        {

            driver_draw_square(state->weapons[i].position.x, state->weapons[i].position.y, TILE_SIZE, MAP_COLOR_WEAPON);
        }
    }
}

//------------ INITIAL MAP FUNCITONS ----------------//
void fill_wall_positions(Vector2D *position, char map[][MAP_WIDTH], int width, int height, char target_char)
{
    int index = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (map[i][j] == target_char)
            {
                position[index].x = j * TILE_SIZE;
                position[index].y = i * TILE_SIZE;
                index++;
            }
        }
    }
}

void fill_elements_init_data(Element *elements, char map[][MAP_WIDTH], int width, int height, char target_char)
{
    int index = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (map[i][j] == target_char)
            {
                elements[index].isEnable = true;
                elements[index].position.x = j * TILE_SIZE;
                elements[index].position.y = i * TILE_SIZE;
                index++;
            }
        }
    }
}

//--------------------------------------------//

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
    map->lives = (Element *)malloc(sizeof(Element) * map->n_lives);
    map->weapons = (Element *)malloc(sizeof(Element) * map->n_weapons);
    map->monsters = (Enemies *)malloc(sizeof(Enemies) * map->n_monsters);
    map->walls = (Vector2D *)malloc(sizeof(Vector2D) * map->n_walls);

    if (!map->lives || !map->monsters || !map->walls || !map->weapons)
    {
        fprintf(stderr, "Memory Alocation Error:[get_position_of_all_elements] .\n");
        exit(EXIT_FAILURE);
    }

    // declarar valores
    fill_wall_positions(map->walls, map_char, width, height, MAP_WALL_SPACE);
    fill_elements_init_data(map->lives, map_char, width, height, MAP_LIFE_SPACE);
    fill_elements_init_data(map->weapons, map_char, width, height, MAP_WEAPON_SPACE);

    fill_monster_init_data(map->monsters, map_char, width, height, MAP_MONSTER_SPACE);
}

void free_all_elements(Game_State *map)
{
    free(map->lives);
    free(map->monsters);
    free(map->walls);
    free(map->weapons);
}

//-------- Colision check function------------------------//

bool check_wall_colision(Vector2D position, Game_State *map)
{
    const float HITBOX_OFFSET = (TILE_SIZE - PLAYER_HITBOX_SIZE) / 2.0f;

    // Define a área de colisão (hitbox) do jogador, centralizada
    float player_hitbox_x = position.x;
    float player_hitbox_y = position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    for (int i = 0; i < map->n_walls; i++)
    {
        float wall_x = map->walls[i].x;
        float wall_y = map->walls[i].y;
        float wall_width = TILE_SIZE;
        float wall_height = TILE_SIZE;

        // Verifica colisão AABB entre o jogador e a parede
        if (player_hitbox_x < wall_x + wall_width &&
            player_hitbox_x + player_hitbox_width > wall_x &&
            player_hitbox_y < wall_y + wall_height &&
            player_hitbox_y + player_hitbox_height > wall_y)
        {
            return true;
        }
    }

    // Verifica colisão com as bordas do mapa
    float map_width_pixels = MAP_WIDTH * TILE_SIZE;
    float map_height_pixels = MAP_HEIGHT * TILE_SIZE;

    if (player_hitbox_x < 0 || player_hitbox_x + player_hitbox_width > map_width_pixels)
        return true;
    if (player_hitbox_y < 0 || player_hitbox_y + player_hitbox_height > map_height_pixels)
        return true;

    return false;
}

bool check_life_colision(Vector2D position, int *life_position, Game_State *map)
{
    for (int i = 0; i < map->n_lives; i++)
    {
        if (position.x == map->lives[i].position.x && position.y == map->lives[i].position.y)
        {
            *life_position = i;
            return true;
        }
    }

    return false;
}

bool check_weapon_colision(Vector2D position, int *weapon_position, Game_State *map)
{
    for (int i = 0; i < map->n_weapons; i++)
    {
        if (position.x == map->weapons[i].position.x && position.y == map->weapons[i].position.y)
        {
            *weapon_position = i;
            return true;
        }
    }

    return false;
}

bool check_monster_player_colision(Vector2D position, int *monster_position, Game_State *map)
{
    for (int i = 0; i < map->n_monsters; i++)
    {
        if (position.x == map->monsters[i].position.x && position.y == map->monsters[i].position.y)
        {
            *monster_position = i;
            return true;
        }
    }

    return false;
}

//----------handle elements functions -----//

void handle_extra_lifes(Player *player, Game_State *map)
{
    int life_position = 0;

    if (check_life_colision(player->position, &life_position, map))
    {
        if (map->lives[life_position].isEnable)
        {
            map->lives[life_position].isEnable = false;
            player->lives += 1;
            if (DEBUG_PRINTS)
                printf("\nPLAYER LIVES: %d\n", player->lives);
        }
    }
}

void handle_weapon_elements(Player *player, Game_State *map)
{
    int weapon_position = 0;

    if (check_weapon_colision(player->position, &weapon_position, map))
    {
        if (map->weapons[weapon_position].isEnable)
        {
            map->weapons[weapon_position].isEnable = false;
            player->hasWeapon = true;
            if (DEBUG_PRINTS)
                printf("\nPLAYER HAS WEAPON: %d\n", player->hasWeapon);
        }
    }
}

void handle_player_movement(Player *player, Game_State *map, uint8_t key_pressed)
{

    float x_component = 0.00;
    float y_component = 0.00;

    if ((key_pressed & KEY_BIT_W) || (key_pressed & KEY_BIT_UP))
    {
        y_component = -1.00f;
    }
    else if ((key_pressed & KEY_BIT_S) || (key_pressed & KEY_BIT_DOWN))
    {
        y_component = 1.00f;
    }

    if ((key_pressed & KEY_BIT_D) || (key_pressed & KEY_BIT_RIGHT))
    {
        x_component = 1.00f;
    }
    else if ((key_pressed & KEY_BIT_A) || (key_pressed & KEY_BIT_LEFT))
    {
        x_component = -1.00f;
    }

    if (x_component == 0 & y_component == 0)
        return;

    // TODO, normalize vector if player is moving in diagonal
    Vector2D new_player_pos = {player->position.x + x_component * DEFAULT_PLAYER_VELOCITY * 0.016,
                               player->position.y + y_component * DEFAULT_PLAYER_VELOCITY * 0.016};

    if (check_wall_colision(new_player_pos, map) == false)
    {
        move_player(player, &new_player_pos);
    }
}

void handle_player_monster_interation(Player *player, Game_State *map)
{
    int monster_position = 0;

    if (check_monster_player_colision(player->position, &monster_position, map))
    {
        if (map->monsters[monster_position].isEnable)
        {
            if (player->hasWeapon && player->isWeaponActive)
            {

                map->monsters[monster_position].isEnable = false;
                player->score += map->monsters[monster_position].reward;
            }
            else
            {
                player->lives -= 1;
                handle_player_movement(player, map, 's'); // TODO implement move by orientation
                handle_player_movement(player, map, 'a'); // TODO implement move by orientation
            }

            if (DEBUG_PRINTS)
            {

                printf("\nPLAYER HAS WEAPON: %d", player->hasWeapon);
                printf("\nPLAYER LIVES: %d", player->lives);
                printf("\nPLAYER SCORE: %d\n\n", player->score);
            }
        }
    }
}

//-------handle check function -----//

bool check_win_condition(Game_State *map)
{
    for (int i = 0; i < map->n_monsters; i++)
    {
        if (map->monsters[i].isEnable == true)
        {
            return false;
        }
    }
    return true;
}

bool check_user_active_weapon(uint8_t input)
{
    if (input & KEY_BIT_J)
    {
        return true;
    }
    return false;
}
//--------------------------------------------

int init_game_data(int stage_no, bool keep_weapon)
{
    char arq_path[50] = {0};
    sprintf(arq_path, "../../include/game/maps/mapa_%d.txt", stage_no);
    char archive[MAP_HEIGHT][MAP_WIDTH];

    if (DEBUG_PRINTS)
    {
        printf("============\n Map Path: %s \n ============", arq_path);
    }
    proto_mapa(archive, arq_path); // TODO get the value to check the end of the game
    player_init_status(&Link, keep_weapon);
    get_player_initial_position(&Link, archive, MAP_HEIGHT, MAP_WIDTH);
    get_initial_position_of_all_elements(&Map_Data, archive, MAP_WIDTH, MAP_HEIGHT);

    if (DEBUG_PRINTS)
    {
        printf("\nMonstros: %d, Vidas: %d, Armas: %d, Paredes: %d\n",
               Map_Data.n_monsters, Map_Data.n_lives, Map_Data.n_weapons, Map_Data.n_walls);
    }
    return 0;
}

void game_loop()
{
    uint8_t keys_read = 0;
    read_keyboard(&keys_read, true);

    handle_player_movement(&Link, &Map_Data, keys_read);
    handle_extra_lifes(&Link, &Map_Data);
    handle_weapon_elements(&Link, &Map_Data);
    //---

    if (check_win_condition(&Map_Data))
    {
        free_all_elements(&Map_Data);
        init_game_data(2, true); // TODO, incrementar 1 mapa
    }

    handle_player_weapon(&Link, check_user_active_weapon(keys_read));
    handle_player_monster_interation(&Link, &Map_Data);
    draw_map(&Map_Data, MAP_WIDTH, MAP_HEIGHT);
    draw_player(&Link);
}

int finish_game()
{
    free_all_elements(&Map_Data);

    return 0;
}