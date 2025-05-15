#include "game_def.h"
#include "player.h"

Player Link;

int testegame(char move)
{
    static bool game_was_init = false;

    if (!game_was_init)
    {
        player_init_status(&Link);
        game_was_init = true;
    }

    player_movimentation(&Link, move);
    handle_player_weapon(&Link,move);
    draw_player(&Link);
    return 1;
}