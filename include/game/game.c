#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "player.h"
#include "keyboard.h"
#include "enemies.h"
#include "graphic.h"

#include "game_def.h"

// TEMPORARIO
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

//------- ARCHIVE FUNCTIONS ---------//

int read_map_archive(char map[MAP_HEIGHT][MAP_WIDTH], char *arq_nome)
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
    return 0;
}

//----------------------------------//

//----- INITIAL MAP FUNCITONS -----//

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
    // calloc the elements
    map->lives = (Element *)calloc(map->n_lives, sizeof(Element));
    map->weapons = (Element *)calloc(map->n_weapons, sizeof(Element));
    map->monsters = (Enemies *)calloc(map->n_monsters, sizeof(Enemies));
    map->walls = (Vector2D *)calloc(map->n_walls, sizeof(Vector2D));

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

//----- HANDLE CHECK FUNCTIONS -----//

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

    float player_hitbox_x = position.x;
    float player_hitbox_y = position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    for (int i = 0; i < map->n_lives; i++)
    {
        float life_x = map->lives[i].position.x;
        float life_y = map->lives[i].position.y;
        float life_width = TILE_SIZE;
        float life_height = TILE_SIZE;

        // Verifica colisão AABB entre o jogador e a parede
        if (player_hitbox_x < life_x + life_width &&
            player_hitbox_x + player_hitbox_width > life_x &&
            player_hitbox_y < life_y + life_height &&
            player_hitbox_y + player_hitbox_height > life_y)
        {
            *life_position = i;
            return true;
        }
    }

    return false;
}

bool check_weapon_colision(Vector2D position, int *weapon_position, Game_State *map)
{

    // Define a área de colisão(hitbox) do jogador, centralizada
    float player_hitbox_x = position.x;
    float player_hitbox_y = position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    for (int i = 0; i < map->n_weapons; i++)
    {
        float weapon_x = map->weapons[i].position.x;
        float weapon_y = map->weapons[i].position.y;
        float weapon_width = TILE_SIZE;
        float weapon_height = TILE_SIZE;

        // Verifica colisão AABB entre o jogador e a parede
        if (player_hitbox_x < weapon_x + weapon_width &&
            player_hitbox_x + player_hitbox_width > weapon_x &&
            player_hitbox_y < weapon_y + weapon_height &&
            player_hitbox_y + player_hitbox_height > weapon_y)
        {
            *weapon_position = i;
            return true;
        }
    }

    return false;
}

bool check_monster_player_colision(Vector2D position, int *monster_position, Game_State *map)
{

    float player_hitbox_x = position.x;
    float player_hitbox_y = position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    for (int i = 0; i < map->n_monsters; i++)
    {
        float monster_x = map->monsters[i].position.x;
        float monster_y = map->monsters[i].position.y;
        float monster_width = TILE_SIZE;
        float monster_height = TILE_SIZE;

        // AABB colision
        if (player_hitbox_x < monster_x + monster_width &&
            player_hitbox_x + player_hitbox_width > monster_x &&
            player_hitbox_y < monster_y + monster_height &&
            player_hitbox_y + player_hitbox_height > monster_y)
        {
            *monster_position = i;
            return true;
        }
    }

    return false;
}

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

//----- HANDLE ELEMENTS FUNCTIONS -----//

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

    if (x_component == 0 && y_component == 0)
        return;

    // TO DO, normalize vector if player is moving in diagonal
    Vector2D new_player_pos = {player->position.x + x_component * DEFAULT_PLAYER_VELOCITY * get_frame_time(),
                               player->position.y + y_component * DEFAULT_PLAYER_VELOCITY * get_frame_time()};

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
                // TODO levar dano mas ficar 1 tempo invuneravel para nao levar danos infinitos
                // talvez flag na struct para piscar na tela;
                // player->lives -= 1;
                Vector2D new_position = {player->position.x - 10, player->position.y + 10};
                move_player(player, &new_position);
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

//--------------------------------------------//

int init_game_data(int stage_no, bool keep_player_status, Player *player, Game_State *Map_Data)
{
    char arq_path[50] = {0};
    int is_map_not_read = 0;
    char map_matrix[MAP_HEIGHT][MAP_WIDTH];

    sprintf(arq_path, MAP_PATH_PREFIX "%d.txt", stage_no);

    if (DEBUG_PRINTS)
    {
        printf("============\n Map Path: %s \n ============", arq_path);
    }

    is_map_not_read = read_map_archive(map_matrix, arq_path);
    if (is_map_not_read)
    {
        return 1;
    }

    player_init_status(player, keep_player_status);
    get_player_initial_position(player, map_matrix, MAP_HEIGHT, MAP_WIDTH);
    get_initial_position_of_all_elements(Map_Data, map_matrix, MAP_WIDTH, MAP_HEIGHT);

    if (DEBUG_PRINTS)
    {
        printf("\nMonstros: %d, Vidas: %d, Armas: %d, Paredes: %d\n",
               Map_Data->n_monsters, Map_Data->n_lives, Map_Data->n_weapons, Map_Data->n_walls);
    }
    return 0;
}

int game_loop(bool is_a_new_game)
{
    //--- game structs
    static Player Link;
    static Game_State Map_Data;

    // -- game aux vars
    static int stage_conter = 1;
    static bool is_first_stage = true;
    int finish_of_game = 0;
    uint8_t keys_read = 0;

    // -- init game verifications
    if (is_a_new_game)
    {
        stage_conter = 1;
        is_first_stage = true;
    }

    if (is_first_stage)
    {
        is_first_stage = false;
        finish_of_game = init_game_data(stage_conter, false, &Link, &Map_Data);
        if (finish_of_game)
        {
            return 1;
        }
    }

    // -- fisish game verifications
    if (check_win_condition(&Map_Data))
    {
        stage_conter++;
        free_all_elements(&Map_Data);
        finish_of_game = init_game_data(stage_conter, true, &Link, &Map_Data);
        if (finish_of_game)
        { // TODO mensagem of end game loop
            return 1;
        }
    }

    //--- calling game funcions
    read_keyboard(&keys_read, true);
    handle_player_movement(&Link, &Map_Data, keys_read);
    handle_extra_lifes(&Link, &Map_Data);
    handle_weapon_elements(&Link, &Map_Data);
    handle_player_weapon(&Link, check_user_active_weapon(keys_read));
    handle_player_monster_interation(&Link, &Map_Data);
    draw_map(&Map_Data, MAP_WIDTH, MAP_HEIGHT);
    draw_player(&Link);
    return 0;
}
