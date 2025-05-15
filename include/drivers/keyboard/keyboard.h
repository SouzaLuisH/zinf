#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "raylib.h"

#define N_KEYS_MAPPED 8

unsigned char driver_keyboard(void);
void read_keyboard(unsigned char *key);

#endif // KEYBOARD_H


