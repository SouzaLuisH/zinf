#include "game_def.h"
#include "player.h"
#include "enemies.h"
#include <stdio.h>
#include "graphic.h"

Player Link;
Enemies Monster[MAX_ENEMIES_PER_LEVEL]; // create 10 enemies(should i create MAX_ENEMIES_PER_LEVEL????)

char archive[16][24] = {
    "     M        PPPP      ",
    "              M P       ",
    "               P        ",
    "       M    V PP        ",
    "          PPPP          ",
    "                 M      ",
    "              M         ",
    "                        ",
    "       J   P            ",
    "   M      P             ",
    "         E P            ",
    "PPPPPPPPPPPP            ",
    "                        ",
    "              M         ",
    "                       V",
    "                        ",
};

void draw_map(char map[][24], int lenght, int height)
{
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 24; j++)
        {
            switch (map[i][j])
            {
            case ' ':
                driver_draw_square(j * 50, (i * 50), 50, 2); // black
                break;
            case 'J':
                // driver_draw_square( j * 50, ( i * 50), 50, 3); // red
                break;
            case 'M':
                driver_draw_square(j * 50, (i * 50), 50, 0); // white
                break;
            case 'E':
                driver_draw_square(j * 50, (i * 50), 50, 1); // gray
                break;
            case 'V':
                driver_draw_square(j * 50, (i * 50), 50, 4); // green
                break;
            case 'P':
                driver_draw_square(j * 50, (i * 50), 50, 5); // yellow
                break;

            default:
                break;
            }
        }
    }
}



int testegame(char move)
{
    static bool game_was_init = false;

    if (!game_was_init)
    {
        // TODO read map file, to set the positions of the enemies and player
        // TODO save map in matrix
        player_init_status(&Link);
        get_player_initial_position(&Link, archive, 16, 24);
        game_was_init = true;

        for (int i = 0; i < MAX_ENEMIES_PER_LEVEL; i++) //NAO GOSTEI, FAZER DIFERENTE
        {
            monster_init_status(&Monster[i]);
        }
    }

    player_movimentation(&Link, move);
    handle_player_weapon(&Link, move);
    draw_map(archive,16,24);

    draw_player(&Link);
    for (int i = 0; i < MAX_ENEMIES_PER_LEVEL; i++) //NAO GOSTEI, FAZER DIFERENTE
    {
        // draw_monster(&Monster[i]);
    }

    return 1;
}