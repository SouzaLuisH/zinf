#ifndef KEYBOARD_H
#define KEYBOARD_H

#define N_KEYS_MAPPED 8

unsigned char driver_keyboard(bool game_mode);
void read_keyboard(unsigned char *key, bool game_mode);


#endif // KEYBOARD_H


