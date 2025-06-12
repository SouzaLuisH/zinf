#include <stdbool.h>
#include "keyboard.h"
#include "raylib.h"

void read_keyboard(uint8_t *pressed_keys, bool game_mode)
{
	*pressed_keys = driver_keyboard(game_mode);
}

uint8_t driver_keyboard(bool game_mode)
{
	static float key_repeat_timer = 0.0f;
	static bool is_key_pressed[N_KEYS_MAPPED] = {false}; // Para 6 teclas monitoradas

	float delay_to_start_repeat = 0.4f;
	float repeat_rate = 0.1f;
	int i = 0;
	uint8_t command_mask = 0x00;
	struct
	{
		int key;
		uint8_t bit;
	} keys[N_KEYS_MAPPED] = {
		{KEY_W, KEY_BIT_W},
		{KEY_UP, KEY_BIT_UP},
		{KEY_A, KEY_BIT_A},
		{KEY_LEFT, KEY_BIT_LEFT},
		{KEY_S, KEY_BIT_S},
		{KEY_DOWN, KEY_BIT_DOWN},
		{KEY_D, KEY_BIT_D},
		{KEY_RIGHT, KEY_BIT_RIGHT},
		{KEY_ENTER, KEY_BIT_ENTER},
		{KEY_P, KEY_BIT_P},
		{KEY_J, KEY_BIT_J},

	};

	if (game_mode)
	{
		for (i = 0; i < N_KEYS_MAPPED; i++)
		{
			if (IsKeyDown(keys[i].key))
			{
				command_mask |= keys[i].bit;
			}
		}
		return command_mask;
	}

	while (i < sizeof(keys) / sizeof(keys[0]))
	{
		if (IsKeyPressed(keys[i].key))
		{
			command_mask = keys[i].bit;
			key_repeat_timer = delay_to_start_repeat;
			is_key_pressed[i] = true;
			i = sizeof(keys) / sizeof(keys[0]); // para sair do loop
		}
		else if (IsKeyDown(keys[i].key) && is_key_pressed[i])
		{
			key_repeat_timer -= GetFrameTime();
			if (key_repeat_timer <= 0.0f)
			{
				command_mask = keys[i].bit;
				key_repeat_timer = repeat_rate;
				i = sizeof(keys) / sizeof(keys[0]); // para sair do loop
			}
		}
		else
		{
			is_key_pressed[i] = false;
		}
		i++;
	}

	return command_mask;
};
