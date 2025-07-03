#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"
#include "enemies.h"
#include "game_def.h"
#include "graphic.h"
#include "keyboard.h"
#include "player.h"

typedef struct {
    int stage_conter;
    bool is_first_stage;
} game_status_t;

typedef struct tipo_score {
    char nome[20];
    int score;
} TIPO_SCORE;

// TEMPORARIO
void draw_map(Game_State *state, int width, int height) {
    for (int i = 0; i < state->n_lives; i++) {
        if (state->lives[i].isEnable) {
            driver_print_element(state->lives[i].position.x, state->lives[i].position.y + STATUS_BOARD_OFFSET, 'V');
        }
    }

    for (int i = 0; i < state->n_monsters; i++) {
        if (state->monsters[i].isEnable) {
            driver_print_monster(state->monsters[i].position.x, state->monsters[i].position.y + STATUS_BOARD_OFFSET, state->monsters[i].orientation);
        }
    }

    for (int i = 0; i < state->n_walls; i++) {
        driver_print_element(state->walls[i].x, state->walls[i].y + STATUS_BOARD_OFFSET, 'P');
    }

    for (int i = 0; i < state->n_weapons; i++) {
        if (state->weapons[i].isEnable) {
            driver_print_element(state->weapons[i].position.x, state->weapons[i].position.y + STATUS_BOARD_OFFSET, 'E');
        }
    }
}

void draw_dashboard(Player player, game_status_t status) {
    driver_print_statusboard(player.lives, player.score, status.stage_conter, MAP_WIDTH * TILE_SIZE, STATUS_BOARD_OFFSET);
}

//------- ARCHIVE FUNCTIONS ---------//

int read_map_archive(char *map, char *arq_nome) {
    int i = 0, j = 0;
    char r_char = 0;
    FILE *arq_map = fopen(arq_nome, "r");
    if (arq_map == NULL) {
        if (DEBUG_PRINTS) {
            printf("ERRO! O mapa está com algum problema.");
        }
        return 1;
    }

    while ((r_char = fgetc(arq_map)) != EOF) {
        if (r_char != '\r' && r_char != '\n' && r_char != '\0') {
            if (i < MAP_WIDTH * MAP_HEIGHT) {
                *map = r_char;
                map++;
                i++;
            }
        }
    }
    fclose(arq_map);
    return 0;
}

int try_open_map(int stage_no) {
    char arq_path[50] = {0};
    sprintf(arq_path, MAP_PATH_PREFIX "%d.txt", stage_no);

    FILE *arq_map = fopen(arq_path, "r");
    if (arq_map == NULL) {
        return 0;
    }
    fclose(arq_map);
    return 1;
}

//----- INITIAL MAP FUNCITONS -----//

void fill_wall_positions(Vector2D *position, char map[][MAP_WIDTH], int width, int height, char target_char) {
    int index = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (map[i][j] == target_char) {
                position[index].x = j * TILE_SIZE;
                position[index].y = i * TILE_SIZE;
                index++;
            }
        }
    }
}

void fill_elements_init_data(Element *elements, char map[][MAP_WIDTH], int width, int height, char target_char) {
    int index = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (map[i][j] == target_char) {
                elements[index].isEnable = true;
                elements[index].position.x = j * TILE_SIZE;
                elements[index].position.y = i * TILE_SIZE;
                index++;
            }
        }
    }
}

void get_initial_position_of_all_elements(Game_State *map, char map_char[][MAP_WIDTH], int width, int height) {
    map->n_lives = 0;
    map->n_walls = 0;
    map->n_weapons = 0;
    map->n_monsters = 0;

    map->lives = NULL;
    map->monsters = NULL;
    map->walls = NULL;
    map->weapons = NULL;

    // get number of each elements
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
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

    if (!map->lives || !map->monsters || !map->walls || !map->weapons) {
        fprintf(stderr, "Memory Alocation Error:[get_position_of_all_elements] .\n");
        exit(EXIT_FAILURE);
    }

    // declarar valores
    fill_wall_positions(map->walls, map_char, width, height, MAP_WALL_SPACE);
    fill_elements_init_data(map->lives, map_char, width, height, MAP_LIFE_SPACE);
    fill_elements_init_data(map->weapons, map_char, width, height, MAP_WEAPON_SPACE);
    fill_monster_init_data(map->monsters, map_char, width, height, MAP_MONSTER_SPACE);
}

void free_all_elements(Game_State *map) {
    free(map->lives);
    free(map->monsters);
    free(map->walls);
    free(map->weapons);
}
//--------------------------------------------

//----- HANDLE ELEMENTS FUNCTIONS -----//

void handle_extra_lifes(Player *player, Game_State *map) {
    int life_position = 0;

    if (check_life_colision(player->position, &life_position, map)) {
        if (map->lives[life_position].isEnable) {
            map->lives[life_position].isEnable = false;
            player->lives += 1;
            if (DEBUG_PRINTS) printf("\nPLAYER LIVES: %d\n", player->lives);
        }
    }
}

void handle_weapon_elements(Player *player, Game_State *map) {
    int weapon_position = 0;

    if (check_weapon_colision(player->position, &weapon_position, map)) {
        if (map->weapons[weapon_position].isEnable) {
            map->weapons[weapon_position].isEnable = false;
            player->hasWeapon = true;
            if (DEBUG_PRINTS) printf("\nPLAYER HAS WEAPON: %d\n", player->hasWeapon);
        }
    }
}

void handle_player_movement(Player *player, Game_State *map, uint8_t key_pressed) {
    float x_component = 0.0f;
    float y_component = 0.0f;
    Orientation new_Orientation = player->orientation;

    if ((key_pressed & KEY_BIT_W) || (key_pressed & KEY_BIT_UP)) {
        y_component = -1.00f;
        new_Orientation = NORTH;
    } else if ((key_pressed & KEY_BIT_S) || (key_pressed & KEY_BIT_DOWN)) {
        y_component = 1.00f;
        new_Orientation = SOUTH;
    }

    if ((key_pressed & KEY_BIT_D) || (key_pressed & KEY_BIT_RIGHT)) {
        x_component = 1.00f;
        new_Orientation = EAST;
    } else if ((key_pressed & KEY_BIT_A) || (key_pressed & KEY_BIT_LEFT)) {
        x_component = -1.00f;
        new_Orientation = WEST;
    }

    // normalize the vector if player moves in diagonal
    if (x_component != 0 && y_component != 0) {
        /*
            The x and y components are unitary lenght,
            So to normilize we just need divide the components by square root of 2.
        */
        x_component /= ROOT_SQUARE_OF_2;  // normalize x-axis
        y_component /= ROOT_SQUARE_OF_2;  // normalize y-axis

        handle_player_weapon(player, false);  // if player moves in diagonal, can not use the weapon
    } else {                                  // if player moves in one direction, He can uses his weapon
        handle_player_weapon(player, check_user_active_weapon(key_pressed));
    }

    if (x_component == 0 && y_component == 0) return;

    Vector2D new_player_pos = {player->position.x + x_component * DEFAULT_PLAYER_VELOCITY * get_frame_time(), player->position.y + y_component * DEFAULT_PLAYER_VELOCITY * get_frame_time()};

    if (player->orientation != new_Orientation) {
        player->orientation = new_Orientation;
    }

    if (check_wall_colision(new_player_pos, map) == false) {
        move_player(player, &new_player_pos);
    }
}

void handle_player_damage(Player *player, int value_of_damage) {
    if (player->last_damage_time == 0) {
        player->lives -= value_of_damage;
        player->last_damage_time = TIME_PLAYER_INTOCHABLE_AFTER_DAMAGE;
    }
}

void handle_player_monster_interation(Player *player, Game_State *map) {
    for (int i = 0; i < map->n_monsters; i++) {
        Enemies *monster = &map->monsters[i];

        if (monster->isEnable && check_monster_player_colision(player->position, monster->position)) {
            handle_player_damage(player, 1);
        }

        if (monster->isEnable && check_monster_weapon_colision(player, monster->position)) {
            monster->isEnable = false;
            player->score += monster->reward;
        }
    }
}

void handle_monster_movement(Game_State *map_data) {
    for (int i = 0; i < map_data->n_monsters; i++) {
        Enemies *monster = &map_data->monsters[i];
        monster->last_change_time -= get_frame_time();
        Vector2D new_position = {0, 0};

        if (monster->last_change_time <= 0) {
            monster->orientation = (Orientation)(int)(rand() % 4);
            monster->last_change_time = 0.5f + ((float)rand() / RAND_MAX) * 1.0f;  // random time to change orientaton
        }

        switch (monster->orientation) {
            case NORTH:
                new_position.y = -DEFALUT_ENEMIES_VELOCITY * get_frame_time();
                break;
            case SOUTH:
                new_position.y = DEFALUT_ENEMIES_VELOCITY * get_frame_time();
                break;
            case WEST:
                new_position.x = -DEFALUT_ENEMIES_VELOCITY * get_frame_time();
                break;
            case EAST:
                new_position.x = DEFALUT_ENEMIES_VELOCITY * get_frame_time();
                break;
        }

        new_position.x += monster->position.x;
        new_position.y += monster->position.y;

        if (check_wall_colision(new_position, map_data) == false) {
            monster->position = new_position;
        } else {
            monster->last_change_time = 0;
        }
    }
}

//----- HANDLE TIMER FUNCTIONS -----//
void handle_counter_times(Player *player) {
    static float blink_player_timer = 0.2f;

    //--- Player blink
    if (player->last_damage_time > 0) {
        player->last_damage_time -= get_frame_time();

        if (player->last_damage_time < 0.0f) {
            player->last_damage_time = 0.0f;
        }

        blink_player_timer -= get_frame_time();

        if (blink_player_timer <= 0.0f) {
            player->isVisible = !(player->isVisible);
            blink_player_timer = PLAYER_BLINK_PERIOD;
        }
    } else {
        player->isVisible = true;
    }
    //-------------
}
//--------------------------------------------//

// --- HANDLE INITIAL SETTINGS --- //
int init_game_data(int stage_no, bool keep_player_status, Player *player, Game_State *Map_Data) {
    char arq_path[50] = {0};
    char map_matrix[MAP_HEIGHT][MAP_WIDTH];

    sprintf(arq_path, MAP_PATH_PREFIX "%d.txt", stage_no);

    if (DEBUG_PRINTS) {
        printf("============\n Map Path: %s \n ============", arq_path);
    }

    // --- check if map was opened
    if (read_map_archive((char *)map_matrix, arq_path)) {
        return 1;
    }

    player_init_status(player, keep_player_status);
    get_player_initial_position(player, map_matrix, MAP_HEIGHT, MAP_WIDTH);
    get_initial_position_of_all_elements(Map_Data, map_matrix, MAP_WIDTH, MAP_HEIGHT);

    if (DEBUG_PRINTS) {
        printf("\nMonstros: %d, Vidas: %d, Armas: %d, Paredes: %d\n", Map_Data->n_monsters, Map_Data->n_lives, Map_Data->n_weapons, Map_Data->n_walls);
    }
    return 0;
}

// ---- HANDLE STAGES ---- //
// Helper to handle win condition, stage transition, and end game
int handle_stage_transition(game_status_t *game_status, Player *Link, Game_State *Map_Data) {
    if (check_win_condition(Map_Data)) {
        if (try_open_map(game_status->stage_conter + 1) == false) {
            driver_print_end_game_victory(TILE_SIZE * MAP_WIDTH, TILE_SIZE * MAP_HEIGHT + STATUS_BOARD_OFFSET + 20);

            uint8_t keys_read = 0;
            read_keyboard(&keys_read, false);
            if (keys_read & KEY_BIT_ENTER) {
                free_all_elements(Map_Data);
                // TODO: ranking
                return 1;  // End game
            }
            return 0;  // Wait for user
        } else {
            game_status->stage_conter++;
            free_all_elements(Map_Data);
            init_game_data(game_status->stage_conter, true, Link, Map_Data);
        }
    }
    return -1;  // Continue game
}

// Helper for player name input, returns 1 if name is confirmed, 0 otherwise
static int get_player_name(char *player_name, int max_len) {
    static int name_pos = 0;
    int key = 0;
    get_keyboard_letter(&key);
    if (name_pos < max_len - 1) {
        player_name[name_pos++] = (char)key;
        player_name[name_pos] = '\0';
    } else if (key == 8 && name_pos > 0) {  // Backspace
        name_pos--;
        player_name[name_pos] = '\0';
    } else if ((key & KEY_BIT_ENTER) && name_pos > 0) {
        name_pos = 0;
        return 1;  // Name confirmed
    }
    // Show the name being typed
    driver_print_get_name(player_name);
    return 0;
}

// Helper to save score to file
static void save_score(const char *name, int score) {
    TIPO_SCORE entry;
    strncpy(entry.nome, name, sizeof(entry.nome) - 1);
    entry.nome[sizeof(entry.nome) - 1] = '\0';
    entry.score = score;
    FILE *f = fopen("ranking.bin", "ab");
    if (f) {
        fwrite(&entry, sizeof(TIPO_SCORE), 1, f);
        fclose(f);
    }
}

// Handles ranking input and saving after defeat
static int handle_ranking(Player *Link, Game_State *Map_Data) {
    static int ranking_state = 0;
    static char player_name[20] = "";
    uint8_t key = 0;

    if (ranking_state == 0) {
        read_keyboard(&key, false);
        if (key & KEY_BIT_ENTER) {
            ranking_state = 1;
            player_name[0] = '\0';
        }
        return 0;  // Wait for user
    } else if (ranking_state == 1) {
        if (get_player_name(player_name, sizeof(player_name))) {
            save_score(player_name, Link->score);
            free_all_elements(Map_Data);
            ranking_state = 0;
            return 1;  // End game
        }
        return 0;  // Wait for user
    }
    return -1;
}

// Only checks defeat and calls ranking handler if needed
int handle_defeat(Player *Link, Game_State *Map_Data) {
    if (check_defeat_condition(*Link)) {
        driver_print_end_game_defeat(TILE_SIZE * MAP_WIDTH, TILE_SIZE * MAP_HEIGHT + STATUS_BOARD_OFFSET + 20);
        return handle_ranking(Link, Map_Data);
    }
    return -1;  // Continue game
}
//--------------------------------------------//

int game_loop(bool is_a_new_game) {
    //--- game structs
    static Player Link;
    static Game_State Map_Data;
    static game_status_t game_status = {1, true};

    // Handle new game initialization
    if (is_a_new_game) {
        game_status.stage_conter = 1;
        game_status.is_first_stage = true;
    }

    // Initialize stage if needed
    if (game_status.is_first_stage) {
        game_status.is_first_stage = false;
        // error in map archive opening
        if (init_game_data(game_status.stage_conter, false, &Link, &Map_Data)) {
            return 1;
        }
    }

    // Handle win/next-stage/end-game logic
    int transition_result = handle_stage_transition(&game_status, &Link, &Map_Data);
    if (transition_result != -1) {
        return transition_result;
    }

    // Handle defeat logic
    int defeat_result = handle_defeat(&Link, &Map_Data);
    if (defeat_result != -1) {
        return defeat_result;
    }

    //--- Main game logic
    uint8_t keys_read = 0;
    read_keyboard(&keys_read, true);
    handle_player_movement(&Link, &Map_Data, keys_read);
    handle_extra_lifes(&Link, &Map_Data);
    handle_weapon_elements(&Link, &Map_Data);
    handle_player_monster_interation(&Link, &Map_Data);

    if (ENABLE_MONSTER_MOVE) {
        handle_monster_movement(&Map_Data);
    }

    //--- Timers and drawing
    handle_counter_times(&Link);
    draw_map(&Map_Data, MAP_WIDTH, MAP_HEIGHT);
    draw_player(&Link);
    draw_dashboard(Link, game_status);
    return 0;
}
