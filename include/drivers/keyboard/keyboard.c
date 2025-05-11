#include "keyboard.h"

void read_keyboard(int *key)
{
	*key = driver_keyboard();
}

int driver_keyboard()
{
	static float key_repeat_timer = 0.0f;
	static bool is_key_pressed[6] = {false}; // Para 6 teclas monitoradas

	float delay_to_start_repeat = 0.4f;
	float repeat_rate = 0.1f;

	int command = ' ';
	int i = 0;
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

	while (i < sizeof(keys) / sizeof(keys[0]))
	{
		if (IsKeyPressed(keys[i].key))
		{
			command = keys[i].command;
			key_repeat_timer = delay_to_start_repeat;
			is_key_pressed[i] = true;
			i = sizeof(keys) / sizeof(keys[0]);// para sair do loop
		}
		else if (IsKeyDown(keys[i].key) && is_key_pressed[i])
		{
			key_repeat_timer -= GetFrameTime();
			if (key_repeat_timer <= 0.0f)
			{
				command = keys[i].command;
				key_repeat_timer = repeat_rate;
				i = sizeof(keys) / sizeof(keys[0]);// para sair do loop
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
