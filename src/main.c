
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "graphic.h"
#include "keyboard.h"
#include "raylib.h"
#include "resource_dir.h"  // utility header for SearchAndSetResourceDir

#define WINDOW_WIDHT TILE_SIZE *MAP_WIDTH
#define WINDOW_HEIGHT (TILE_SIZE * MAP_HEIGHT + STATUS_BOARD_OFFSET + 20)

//------------------------ Global type Var--------------------------------//

typedef enum game_mode_options { in_game = 0, ranking_menu, main_menu, quit } GameMode;

//------------------------ Menu Functions--------------------------------//

int main_menu_f(uint8_t menu_key, enum game_mode_options *game_mode) {
    static int menu_index = 1;

    if (menu_key & KEY_BIT_S && (menu_index) < 3) {
        menu_index = menu_index + 1;
    } else if (menu_key & KEY_BIT_W && (menu_index > 1)) {
        menu_index = menu_index - 1;
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

    int offset_x = 200;
    int offset_y = WINDOW_HEIGHT / 2;

    driver_print_text("ZINF", offset_x, offset_y, 4);
    driver_print_text("Início", offset_x, offset_y * 1.2, !(menu_index == 1));
    driver_print_text("Ranking", offset_x, offset_y * 1.3, !(menu_index == 2));
    driver_print_text("Sair", offset_x, offset_y * 1.4, !(menu_index == 3));
}

//-------------------------- Game Function -------------------------------//

void in_game_f(uint8_t menu_key, enum game_mode_options *game_mode) {
    static bool is_paused = false;
    static bool is_new_game = true;

    int is_finish_of_game = 0;

    if (is_paused) {
        DrawRectangle(50, WINDOW_HEIGHT / 2, 900, 300, RED);
        driver_print_text("Pressione:", 80, WINDOW_HEIGHT / 2 + 80, 0);
        driver_print_text("-> A tecla Enter para voltar ao menu", 80, WINDOW_HEIGHT / 2 + 120, 0);
        driver_print_text("-> A tecla W para retomar ", 80, WINDOW_HEIGHT / 2 + 160, 0);

        if (menu_key & KEY_BIT_ENTER) {
            is_paused = false;
            is_new_game = true;
            *game_mode = main_menu;
        } else if (menu_key & KEY_BIT_W || menu_key & KEY_BIT_TAB) {
            is_paused = false;
        }
    } else {
        if (menu_key & KEY_BIT_TAB) {
            is_paused = true;
        }

        //----------- RUN DA GAME ----------//
        is_finish_of_game = game_loop(is_new_game);
        is_new_game = false;

        if (is_finish_of_game) {
            is_new_game = true;
            *game_mode = main_menu;
        }
        driver_print_text("-> [Tab]: pause ", WINDOW_WIDHT - 300, WINDOW_HEIGHT - 50, 0);
    }
}

int main() {
    GameMode game_screen = main_menu;
    uint8_t menu_key_press = 0;

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(WINDOW_WIDHT, WINDOW_HEIGHT, "ZINF");
    driver_load_all_textures();
    SetExitKey(0);

    srand(32);
    // SearchAndSetResourceDir("resources");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        read_keyboard(&menu_key_press, false);

        switch (game_screen) {
            case main_menu:
                main_menu_f(menu_key_press, &game_screen);
                break;

            case in_game:
                in_game_f(menu_key_press, &game_screen);
                break;
                // case ranking

            case quit:
                driver_unload_all_textures();
                CloseWindow();
                break;

            default:
                break;
        }

        if (!WindowShouldClose()) {
            EndDrawing();
        }
    }

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
         - (extra) animacaoes
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
                        * Leitura/edição de arquivo.bin
                - (extra) salvar jogo

*/