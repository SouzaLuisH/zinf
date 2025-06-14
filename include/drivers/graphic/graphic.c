#include "graphic.h"
#include "raylib.h"

float get_frame_time()
{
    return GetFrameTime();
}

void driver_print_text(char *text, int x, int y, int color)
{

    Color availableColors[] = {
        WHITE,
        GRAY,
        BLACK,
        RED,
        GREEN,
    };

    DrawText(text, x, y, 40, availableColors[color]);
}

void driver_draw_square(float x, float y, float side_lenght, int color)
{

    if (color < 0)
    {
        return;
    }

    Color availableColors[] = {
        WHITE, // 0
        GRAY,  // 1
        BLACK, // 2
        RED,   // 3
        GREEN, // 4
        YELLOW, // 5
        PURPLE, //6
        VIOLET, //7
        GOLD,//8
    };

    DrawRectangle(x, y, side_lenght, side_lenght, availableColors[color]);
}