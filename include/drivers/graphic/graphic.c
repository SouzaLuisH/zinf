#include "graphic.h"
#include "raylib.h"


float get_frame_time(){
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