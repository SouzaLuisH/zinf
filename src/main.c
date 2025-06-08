
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include "keyboard.h"
#include "graphic.h"
#include <game_def.h>

#define WINDOW_WIDHT 1200
#define WINDOW_HEIGHT 800

//------------------------ Global Var--------------------------------//
enum game_mode_options
{
	in_game = 0,
	ranking_menu,
	main_menu,
	quit
};

unsigned char global_press_key = 0;
enum game_mode_options global_game_screen = main_menu;

//------------------------ Menu Functions--------------------------------//

int main_menu_f()
{
	static int menu_index = 1;

	if (global_press_key == 's' && (menu_index) < 3)
	{
		menu_index = menu_index + 1;
	}
	else if (global_press_key == 'w' && (menu_index > 1))
	{
		menu_index = menu_index - 1;
	}
	else if (global_press_key == '\n')
	{
		switch (menu_index)
		{
		case 1:
			global_game_screen = in_game;
			break;
		case 2:
			global_game_screen = ranking_menu;
			break;
		case 3:
			global_game_screen = quit;
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

void in_game_f()
{
	static bool is_paused = false;

	if (is_paused)
	{
		DrawRectangle(50, WINDOW_HEIGHT / 2, 900, 300, RED);
		driver_print_text("Pressione:", 80, WINDOW_HEIGHT / 2 + 80, 0);
		driver_print_text("-> A tecla Enter para voltar ao menu", 80, WINDOW_HEIGHT / 2 + 120, 0);
		driver_print_text("-> A tecla W para retomar ", 80, WINDOW_HEIGHT / 2 + 160, 0);

		if (global_press_key == '\n')
		{
			is_paused = false;
			global_game_screen = main_menu;
		}
		else if (global_press_key == 'w')
		{
			is_paused = false;
		}
	}
	else
	{
		if (global_press_key == 'p')
		{
			is_paused = true;
		}

		game_loop(global_press_key);
		driver_print_text("-> [P]: pause ", WINDOW_WIDHT - 300, WINDOW_HEIGHT - 50, 0);
	}
}

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(WINDOW_WIDHT, WINDOW_HEIGHT, "ZINF");
	SetExitKey(0);
	srand(32);
	// SearchAndSetResourceDir("resources");
	init_game_data('3');
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		read_keyboard(&global_press_key);

		switch (global_game_screen)
		{
		case main_menu:
			main_menu_f();
			break;

		case in_game:
			in_game_f();
			break;
			// case ranking

		case quit:
			CloseWindow();
			break;

		default:
			break;
		}

		EndDrawing();
	}

	finish_game();
	CloseWindow();
	return 0;
}

/*
	Mapa:
	 - Leitura de arquivos txt
	 - criar matriz preenchida a partir deles OK
	 - criar estrutura para os elementos iniciais do jogo OK
	 - Plotar o mapa OK
	 - Imagens
	 - (extra) Mexer na camera
	 - (extra) animacaoes
	Inimigos
	 - Movimentação aleátoria
	 - Pontuação OK
	 - Sistema de vidas OK
	 - Imagens
	 - (extra) inimigos mais inteligentes
	- (extra) animacaoes

	Jogador:
	 - Posicao Inicial OK
	 - Movimentacao OK
	 - Orientacao
	 - Ativacao Espada OK
	 - Pontuacao OK
	 - Vidas OK
	 - Itens OK
	 - Imagens
	 - (extra) animacaoes

	Logica:
		- Colisao com o mapa OK
		- Ataque do inimigo OK
		- Ataque do jogador OK
			* ataque com espada
		- Pontuação OK
		- Vidas perder/ganhar OK
		- Passar de fase
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