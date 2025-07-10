#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "archive.h"
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

//----- GAME DRAW FUNCITONS -----//
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
//--------------------------------------------

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
        printf("Memory Alocation Error:[get_position_of_all_elements] .\n");
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
            increment_player_lives(player, 1);
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

        // handle_player_weapon(player, false);  // if player moves in diagonal, can not use the weapon
    } else { /* if player moves in one direction, He can uses his weapon*/
        // handle_player_weapon(player, check_user_active_weapon(key_pressed));
    }

    if (x_component == 0 && y_component == 0) {
        player->isMoving = false;
        return;
    }

    player->isMoving = true;

    if (player->orientation != new_Orientation) {
        player->orientation = new_Orientation;
    }

    Vector2D new_player_pos_y = {player->position.x, player->position.y + y_component * DEFAULT_PLAYER_VELOCITY * get_frame_time()};
    if (check_wall_colision(new_player_pos_y, map) == false) {
        move_player(player, &new_player_pos_y);
    }

    Vector2D new_player_pos_x = {player->position.x + x_component * DEFAULT_PLAYER_VELOCITY * get_frame_time(), player->position.y};
    if (check_wall_colision(new_player_pos_x, map) == false) {
        move_player(player, &new_player_pos_x);
    }
}

void handle_player_damage(Player *player, int value_of_damage) {
    if (player->last_damage_time == 0) {
        set_player_lives(player, player->lives - value_of_damage);
        player->last_damage_time = TIME_PLAYER_INTOCHABLE_AFTER_DAMAGE;
    }
}

void handle_player_monster_interation(Player *player, Game_State *map) {
    for (int i = 0; i < map->n_monsters; i++) {
        Enemies *monster = &map->monsters[i];

        if (monster->isEnable && check_monster_weapon_colision(player, monster->position)) {
            monster->isEnable = false;
            player->score += monster->reward;
        }

        if (monster->isEnable && check_monster_player_colision(player->position, monster->position)) {
            handle_player_damage(player, 1);
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
//--------------------------------------------//

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
//--------------------------------------------//

// --- HANDLE RANKING FUNCTIONS --- //
int handle_ranking(Player *Link, Game_State *Map_Data) {
    static char player_name[20] = "";
    static bool waiting_for_name = true;  // novo controle de estado

    if (check_is_available_ranking(Link->score)) {
        driver_print_get_name(player_name, WINDOW_HEIGHT);

        if (waiting_for_name) {
            if (get_player_name(player_name, 20)) {
                waiting_for_name = false;  // avança estado
            }
            return 0;  // espera o usuário digitar
        } else {
            save_score(player_name, Link->score);
            free_all_elements(Map_Data);
            waiting_for_name = true;
            player_name[0] = '\0';  // reseta para o próximo uso
            return 1;               // encerra o jogo
        }

    } else {
        free_all_elements(Map_Data);
        return 1;
    }
}

//--------------------------------------------//

// ---- HANDLE STAGES ---- //
int handle_stage_transition(game_status_t *game_status, Player *Link, Game_State *Map_Data) {
    static int stage_transition_state = 0;  // 0 = esperando vitória, 1 = mostrar vitória, 2 = ranking final

    if (stage_transition_state == 0) {
        if (check_win_condition(Map_Data)) {
            if (try_open_map(game_status->stage_conter + 1) == false) {
                stage_transition_state = 1;  // Fim do jogo, mostrar mensagem de vitória
            } else {
                // Próxima fase existe
                game_status->stage_conter++;
                free_all_elements(Map_Data);
                init_game_data(game_status->stage_conter, true, Link, Map_Data);
                return -1;  // Continua o jogo com nova fase
            }
        }
        return -1;  // Ainda jogando
    }

    if (stage_transition_state == 1) {
        driver_print_end_game_victory(WINDOW_HEIGHT);

        uint8_t key = 0;
        read_keyboard(&key, false);
        if (key & KEY_BIT_ENTER) {
            stage_transition_state = 2;  // Vai para ranking
        }
        return 0;  // Espera o jogador apertar Enter
    }

    if (stage_transition_state == 2) {
        int result = handle_ranking(Link, Map_Data);
        if (result == 1) {
            stage_transition_state = 0;  // Reset para a próxima vez que vencer
        }
        return result;
    }

    return -1;  // Nunca deve chegar aqui
}

int handle_defeat(Player *Link, Game_State *Map_Data) {
    static int defeat_stage = 0;  // 0 = mostrar mensagem, 1 = obter nome

    if (check_defeat_condition(*Link) == false) {
        defeat_stage = 0;  // Reset quando o jogador não está derrotado
        return -1;         // Continua o jogo
    }

    if (defeat_stage == 0) {
        driver_print_end_game_defeat(WINDOW_HEIGHT);

        uint8_t key = 0;
        read_keyboard(&key, false);
        if (key & KEY_BIT_ENTER) {
            defeat_stage = 1;  // Avança para a próxima etapa
        }

        return 0;  // Continua esperando
    }

    if (defeat_stage == 1) {
        int result = handle_ranking(Link, Map_Data);
        if (result == 1) {
            defeat_stage = 0;  // Reset após salvar ranking
        }
        return result;
    }

    return -1;  // nunca deve chegar aqui
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
        if (init_game_data(game_status.stage_conter, false, &Link, &Map_Data)) {
            return 1;
        }
        driver_camera_init(WINDOW_WIDHT, WINDOW_HEIGHT, TILE_SIZE);
    }

    // set camera target
    driver_camera_set_target(Link.position.x, Link.position.y);

    // --- Lógica do jogo
    int transition_result = handle_stage_transition(&game_status, &Link, &Map_Data);
    if (transition_result != -1) return transition_result;

    int defeat_result = handle_defeat(&Link, &Map_Data);
    if (defeat_result != -1) return defeat_result;

    // Entrada do jogador e movimentação
    uint8_t keys_read = 0;
    read_keyboard(&keys_read, true);
    handle_player_movement(&Link, &Map_Data, keys_read);
    handle_player_weapon(&Link, check_user_active_weapon(keys_read));
    handle_extra_lifes(&Link, &Map_Data);
    handle_weapon_elements(&Link, &Map_Data);
    handle_player_monster_interation(&Link, &Map_Data);

    if (ENABLE_MONSTER_MOVE) {
        handle_monster_movement(&Map_Data);
    }

    handle_counter_times(&Link);

    // --- Desenho do mundo (com câmera)
    driver_camera_begin();
    driver_draw_background(STATUS_BOARD_OFFSET);
    draw_map(&Map_Data, MAP_WIDTH, MAP_HEIGHT);
    draw_player(&Link);
    driver_camera_end();

    // textos e informações da interface
    draw_dashboard(Link, game_status);

    return 0;
}
