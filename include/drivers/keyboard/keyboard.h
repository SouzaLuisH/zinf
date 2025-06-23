#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#define N_KEYS_MAPPED 12

#define KEY_BIT_W     0x01  // 0000 0001 (key 1)
#define KEY_BIT_UP    0x01  // 0000 0001 (key 2)

#define KEY_BIT_A     0x02  // 0000 0010 (key 3)
#define KEY_BIT_LEFT  0x02  // 0000 0010 (key 4)

#define KEY_BIT_S     0x04  // 0000 0100 (key 5)
#define KEY_BIT_DOWN  0x04  // 0000 0100 (key 6)

#define KEY_BIT_D     0x08  // 0000 1000 (key 7)
#define KEY_BIT_RIGHT 0x08  // 0000 1000 (key 8)

#define KEY_BIT_ENTER 0x10  // 0001 0000 (key 9)
#define KEY_BIT_P     0x20  // 0010 0000 (key 10)
#define KEY_BIT_J     0x40  // 0100 0000 (key 11)
#define KEY_BIT_TAB   0x80  // 1000 0000 (key 12)






uint8_t driver_keyboard(bool game_mode);
void read_keyboard(uint8_t *key, bool game_mode);


#endif // KEYBOARD_H


