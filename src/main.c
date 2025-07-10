#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "archive.h"
#include "game.h"
#include "graphic.h"
#include "keyboard.h"
#include "raylib.h"
#include "string.h"

//------------------------ Global type Var--------------------------------//

typedef enum game_mode_options { in_game = 0, ranking_menu, main_menu, quit } GameMode;

//------------------------ Menu Functions--------------------------------//
void pause_menu_screen(uint8_t menu_key, enum game_mode_options *game_mode, bool *is_paused, bool *is_new_game) {
    static int menu_index = 1;

    if (menu_key & KEY_BIT_S && menu_index < 3) {
        menu_index++;

    } else if (menu_key & KEY_BIT_W && menu_index > 1) {
        menu_index--;
    } else if (menu_key & KEY_BIT_ENTER) {
        switch (menu_index) {
            case 1:
                *is_paused = false;
                break;
            case 2:
                *is_paused = false;
                *is_new_game = true;
                *game_mode = main_menu;
                break;
            case 3:
                *game_mode = quit;
                break;
        }
        menu_index = 1;
    }
    int offset_x = 200, offset_y = WINDOW_HEIGHT / 2;
    driver_print_text("Game Paused", offset_x, offset_y, 4);
    driver_print_text("Resume", offset_x, offset_y * 1.2, !(menu_index == 1));
    driver_print_text("Back to Menu", offset_x, offset_y * 1.3, !(menu_index == 2));
    driver_print_text("Quit", offset_x, offset_y * 1.4, !(menu_index == 3));
}

void main_menu_f(uint8_t menu_key, enum game_mode_options *game_mode) {
    ClearBackground(BLACK);
    static int menu_index = 1;
    if (menu_key & KEY_BIT_S && menu_index < 3) {
        menu_index++;

    } else if (menu_key & KEY_BIT_W && menu_index > 1) {
        menu_index--;
    } else if (menu_key & KEY_BIT_ENTER) {
        switch (menu_index) {
            case 1:
                *game_mode = in_game;
                break;
            case 2:
                *game_mode = ranking_menu;
                break;
            case 3:
                *game_mode = quit;
                break;
        }
        menu_index = 1;
    }
    int offset_x = 200, offset_y = WINDOW_HEIGHT / 2;
    driver_print_text("ZINF", offset_x, offset_y, 4);
    driver_print_text("Início", offset_x, offset_y * 1.2, !(menu_index == 1));
    driver_print_text("Scoreboard", offset_x, offset_y * 1.3, !(menu_index == 2));
    driver_print_text("Sair", offset_x, offset_y * 1.4, !(menu_index == 3));
}

void in_game_f(uint8_t menu_key, enum game_mode_options *game_mode) {
    ClearBackground(BLACK);

    static bool is_paused = false, is_new_game = true;
    int is_finish_of_game = 0;

    if (is_paused) {
        pause_menu_screen(menu_key, game_mode, &is_paused, &is_new_game);
    } else {
        is_finish_of_game = game_loop(is_new_game);
        is_new_game = false;

        if (is_finish_of_game) {
            is_new_game = true;
            *game_mode = main_menu;
        }

        if (menu_key & KEY_BIT_TAB) {
            is_paused = true;
        }
    }
}

void ranking_f(uint8_t menu_key, enum game_mode_options *game_screen) {
    TIPO_SCORE scores[MAX_RANKING_ITENS] = {0};
    int offset_x = 100, offset_y = WINDOW_HEIGHT / 2;
    char aux[80] = {0};
    int n_items = get_ranking_info(scores, MAX_RANKING_ITENS);

    if (menu_key & KEY_BIT_ENTER) *game_screen = main_menu;

    ClearBackground(BLACK);
    driver_print_text("SCOREBOARD", offset_x, offset_y - 100, 4);

    if (n_items == -1) {
        sprintf(aux, "There are not enough players for the ranking");
        driver_print_text(aux, offset_x, offset_y, 0);

    } else {
        for (int i = 0; i < n_items; i++) {
            sprintf(aux, "(%d) %s - %d", i + 1, scores[i].nome, scores[i].score);
            float idx_offset = (1.0f + (float)(i % 10) / 10);
            driver_print_text(aux, offset_x, offset_y * idx_offset, 0);
        }
    }
}

//-------------------------- App Loop Function -------------------------------//

void app_loop() {
    GameMode game_screen = main_menu;
    uint8_t menu_key_press = 0;
    SetTargetFPS(60);
    InitWindow(WINDOW_WIDHT, WINDOW_HEIGHT, "ZINF");
    driver_load_all_textures();
    SetExitKey(0);
    srand(time(NULL));

    while (!WindowShouldClose()) {
        BeginDrawing();
        read_keyboard(&menu_key_press, false);
        switch (game_screen) {
            case main_menu:
                main_menu_f(menu_key_press, &game_screen);
                break;
            case in_game:
                in_game_f(menu_key_press, &game_screen);
                break;
            case ranking_menu:
                ranking_f(menu_key_press, &game_screen);
                break;
            case quit:
                driver_unload_all_textures();
                CloseWindow();
                return;
            default:
                break;
        }
        if (!WindowShouldClose()) EndDrawing();
    }
    driver_unload_all_textures();
    CloseWindow();
}

int main() {
    app_loop();
    return 0;
}

/*
        Mapa:
         - Leitura de arquivos txt OK
         - criar matriz preenchida a partir deles OK
         - criar estrutura para os elementos iniciais do jogo OK
         - Plotar o mapa OK
         - Imagens OK
         - (extra) Mexer na camera
         - (extra) animacaoes OK
        Inimigos
         - Movimentação aleátoria OK
         - Pontuação OK
         - Sistema de vidas OK
         - Imagens OK
         - (extra) inimigos mais inteligentes
        - (extra) animacaoes

        Jogador:
         - Posicao Inicial OK
         - Movimentacao OK
         - Orientacao OK
         - Ativacao Espada OK
         - Pontuacao OK
         - Vidas OK
         - Itens OK
         - Imagens OK
         - (extra) animacaoes

        Logica:
                - Colisao com o mapa OK
                - Ataque do inimigo OK
                - Ataque do jogador OK
                        * ataque com espada OK
                - Pontuação OK
                - Vidas perder/ganhar OK
                - Passar de fase OK
        Driver:
                - Leitura do Teclado  OK
                - Desenho do mapa  OK
                - Desenhar personagens OK*
                - (extra) Animacoes
                - (extra) camera
        Menu:
                - Menu principal OK
                - Menu Pause OK
                - Ranking
                        * Leitura/edição de arquivo.bin OK
                - (extra) salvar jogo

*/