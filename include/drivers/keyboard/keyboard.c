#include <stdbool.h>
#include "keyboard.h"
#include "raylib.h"

void read_keyboard(unsigned char *key, bool game_mode)
{
	*key = driver_keyboard(game_mode);
}

unsigned char driver_keyboard(bool game_mode)
{
	static float key_repeat_timer = 0.0f;
	static bool is_key_pressed[N_KEYS_MAPPED] = {false}; // Para 6 teclas monitoradas

	float delay_to_start_repeat = 0.4f;
	float repeat_rate = 0.1f;

	unsigned char command = '#';
	int i = 0;
	struct
	{
		int key;
		unsigned char command;
	} keys[N_KEYS_MAPPED] = {
		{KEY_W, 'w'},
		{KEY_A, 'a'},
		{KEY_S, 's'},
		{KEY_D, 'd'},
		{KEY_ENTER, '\n'},
		{KEY_P, 'p'},
		{KEY_J, 'j'},
		{KEY_UP, 'w'},
	};
	
	if (game_mode)
	{

		while (i < sizeof(keys) / sizeof(keys[0]))
		{
			if (IsKeyDown(keys[i].key))
			{
				command = keys[i].command;
				i = sizeof(keys) / sizeof(keys[0]); // para sair do loop
			}
			else{
				command = '#';
			}

			i++;
		}
		return command;
	}


	while (i < sizeof(keys) / sizeof(keys[0]))
	{
		if (IsKeyPressed(keys[i].key))
		{
			command = keys[i].command;
			key_repeat_timer = delay_to_start_repeat;
			is_key_pressed[i] = true;
			i = sizeof(keys) / sizeof(keys[0]); // para sair do loop
		}
		else if (IsKeyDown(keys[i].key) && is_key_pressed[i])
		{
			key_repeat_timer -= GetFrameTime();
			if (key_repeat_timer <= 0.0f)
			{
				command = keys[i].command;
				key_repeat_timer = repeat_rate;
				i = sizeof(keys) / sizeof(keys[0]); // para sair do loop
			}
			else
			{
				command = '\0';
			}
		}
		else
		{
			is_key_pressed[i] = false;
		}
		i++;
	}

	return command;
};
