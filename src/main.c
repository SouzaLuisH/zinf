
#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <stdio.h>

#define WINDOW_WIDHT 1600
#define WINDOW_HEIGHT 800

//----------------------------------------------------//
#define MAP_SIZE 40

#define CELL_WIDTH (WINDOW_WIDHT / MAP_SIZE)
#define CELL_HEIGHT (WINDOW_HEIGHT / MAP_SIZE)

#define POSITION_RATIO_X WINDOW_WIDHT - (x * CELL_WIDTH)
#define POSITION_RATIO_Y WINDOW_HEIGHT - (y * CELL_HEIGHT)
//----------------------------------------------------//

//------------------------ Global Var--------------------------------//
enum game_mode_options
{
	in_game = 0,
	ranking_menu,
	main_menu,
	quit
};

int global_press_key = 0;
enum game_mode_options global_game_screen = main_menu;

//------------------------ Keyboard Functions--------------------------------//
int driverKeyboard()
{
	static float key_repeat_timer = 0.0f;
	static bool is_key_pressed[6] = {false}; // Para 6 teclas monitoradas

	float delay_to_start_repeat = 0.4f;
	float repeat_rate = 0.1f;

	int command = ' ';

	struct
	{
		int key;
		char command;
	} keys[] = {
		{KEY_W, 'w'},
		{KEY_A, 'a'},
		{KEY_S, 's'},
		{KEY_D, 'd'},
		{KEY_ENTER, '\n'},
		{KEY_P, 'p'}};

	for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++)
	{
		if (IsKeyPressed(keys[i].key))
		{
			command = keys[i].command;
			key_repeat_timer = delay_to_start_repeat;
			is_key_pressed[i] = true;
			break;
		}
		else if (IsKeyDown(keys[i].key) && is_key_pressed[i])
		{
			key_repeat_timer -= GetFrameTime();
			if (key_repeat_timer <= 0.0f)
			{
				command = keys[i].command;
				key_repeat_timer = repeat_rate;
				break;
			}
		}
		else
		{
			is_key_pressed[i] = false;
		}
	}

	return command;
}
void readKeyboard(int *key)
{
	*key = driverKeyboard();
}

//------------------------ Print Functions--------------------------------//

void driver_print_text(char *text, int x, int y, Color cor)
{
	DrawText(text, x, y, 40, cor);
}

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

	driver_print_text("ZINF", offset_x, offset_y, GREEN);
	driver_print_text("Início", offset_x, offset_y * 1.2, menu_index == 1 ? WHITE : GRAY);
	driver_print_text("Ranking", offset_x, offset_y * 1.3, menu_index == 2 ? WHITE : GRAY);
	driver_print_text("Sair", offset_x, offset_y * 1.4, menu_index == 3 ? WHITE : GRAY);
}

//-------------------------- Game Function -------------------------------//

float x = 0;
float y = 0;

void in_game_f()
{
	static bool is_paused = false;

	if (is_paused)
	{
		DrawRectangle(50, WINDOW_HEIGHT / 2, 900, 300, RED);
		driver_print_text("Pressione:", 80, WINDOW_HEIGHT / 2 + 80, WHITE);
		driver_print_text("-> A tecla Enter para voltar ao menu", 80, WINDOW_HEIGHT / 2 +120, WHITE);
		driver_print_text("-> A tecla W para retomar ", 80, WINDOW_HEIGHT / 2 + 160, WHITE);

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

		switch (global_press_key)
		{
		case 'w':
			y -= 20;
			break;
		case 's':
			y += 20;
			break;
		case 'a':
			x -= 20;
			break;
		case 'd':
		if(x+1<WINDOW_WIDHT-20)
			x += 20;
			break;
		case 'p':
			is_paused = true;
			break;
		}
		driver_print_text("-> [P]: pause ", WINDOW_WIDHT -300,WINDOW_HEIGHT - 50, WHITE);
		DrawRectangle(x, y, 20, 20, WHITE);
	}
}

int main()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(WINDOW_WIDHT, WINDOW_HEIGHT, "ZINF");
	SetExitKey(0);

	// SearchAndSetResourceDir("resources");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		readKeyboard(&global_press_key);

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

	CloseWindow();
	return 0;
}
