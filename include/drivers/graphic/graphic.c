#include "graphic.h"
#include "raylib.h"
#include <stddef.h>
#include <stdio.h>

// player
static Texture2D Player_North = {0};
static Texture2D Player_South = {0};
static Texture2D Player_West = {0};
static Texture2D Player_East = {0};

// monster
static Texture2D Monster_North = {0};
static Texture2D Monster_South = {0};
static Texture2D Monster_West = {0};
static Texture2D Monster_East = {0};

// element
static Texture2D Sword = {0};
static Texture2D Wall = {0};
static Texture2D Life = {0};

float get_frame_time()
{
    return GetFrameTime();
}

void driver_load_all_textures()
{
    // player
    Player_North = LoadTexture(PLAYER_SPRITES_PATH "jogador-norte.png");
    Player_South = LoadTexture(PLAYER_SPRITES_PATH "jogador-sul.png");
    Player_West = LoadTexture(PLAYER_SPRITES_PATH "jogador-oeste.png");
    Player_East = LoadTexture(PLAYER_SPRITES_PATH "jogador-leste.png");

    // monster
    Monster_North = LoadTexture(MONSTER_SPRITES_PATH "monstro-norte.png");
    Monster_South = LoadTexture(MONSTER_SPRITES_PATH "monstro-sul.png");
    Monster_West = LoadTexture(MONSTER_SPRITES_PATH "monstro-oeste.png");
    Monster_East = LoadTexture(MONSTER_SPRITES_PATH "monstro-leste.png");

    // elements
    Sword = LoadTexture(ELEMENTS_SPRITES_PATH "espada.png");
    Wall = LoadTexture(ELEMENTS_SPRITES_PATH "parede.png");
    Life = LoadTexture(ELEMENTS_SPRITES_PATH "vida.png");
}

void driver_unload_all_textures()
{
    // player
    UnloadTexture(Player_North);
    UnloadTexture(Player_South);
    UnloadTexture(Player_West);
    UnloadTexture(Player_East);

    // monster
    UnloadTexture(Monster_North);
    UnloadTexture(Monster_South);
    UnloadTexture(Monster_West);
    UnloadTexture(Monster_East);

    // elements
    UnloadTexture(Sword);
    UnloadTexture(Wall);
    UnloadTexture(Life);
}

void driver_print_player(float x, float y, int orientation)
{
    enum
    {
        SOUTH = 0,
        NORTH = 1,
        WEST = 2,
        EAST = 3
    } Orientation_enum;

    Orientation_enum = orientation;
    Vector2 position = {x, y};
    Texture2D *Player = NULL;
    float scale = 0.13f;
    switch (Orientation_enum)
    {
    case NORTH:
        Player = &Player_North;
        break;
    case SOUTH:
        Player = &Player_South;
        break;
    case WEST:
        Player = &Player_West;
        break;
    case EAST:
        Player = &Player_East;
        break;
    }

    if (Player != NULL)
    {
        DrawTextureEx(*Player, position, 0.0f, scale, WHITE);
    }
}

void driver_print_monster(float x, float y, int orientation)
{
    enum
    {
        SOUTH = 0,
        NORTH = 1,
        WEST = 2,
        EAST = 3
    } Orientation_enum;

    Orientation_enum = orientation;
    Vector2 position = {x, y};
    Texture2D *Monster = NULL;

    float scale = 0.16f;

    switch (Orientation_enum)
    {
    case NORTH:
        Monster = &Monster_North;
        break;
    case SOUTH:
        Monster = &Monster_South;
        break;
    case WEST:
        Monster = &Monster_West;
        break;
    case EAST:
        Monster = &Monster_East;
        break;
    }

    if (Monster != NULL)
    {
        DrawTextureEx(*Monster, position, 0.0f, scale, WHITE);
    }
}

void driver_print_element(float x, float y, char element)
{
    Vector2 position = {x, y};
    float scale = 0.16f;
    Texture2D *Element = NULL;
    switch (element)
    {
    case 'P':
        Element = &Wall;
        scale = 0.24;
        break;
    case 'V':
        Element = &Life;
        break;
    case 'E':
        Element = &Sword;
        scale = 0.05;
        break;
    }
    if (Element != NULL)
    {
        DrawTextureEx(*Element, position, 0.0f, scale, WHITE);
    }
}

void driver_print_statusboard(int player_lives, int player_score, int game_stage, int width, int height)
{

    char buffer[60] = {0};
    sprintf(buffer, "Lives: %d  Score: %d   Stage: %d", player_lives, player_score, game_stage);
    DrawRectangle(0, 0, width, height, GRAY);
    driver_print_text(buffer, 10, 10, 0);
}

void driver_print_end_game_victory(int width, int height)
{

    // char buffer[60] = {0};
    // sprintf(buffer, "YOU WIN !");
    // // DrawRectangle(0, 0, width, height, BLACK);
    driver_print_text("YOU WIN !", 10, 10, 0);
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
        WHITE,  // 0
        GRAY,   // 1
        BLACK,  // 2
        RED,    // 3
        GREEN,  // 4
        YELLOW, // 5
        PURPLE, // 6
        VIOLET, // 7
        GOLD,   // 8
    };

    DrawRectangle(x, y, side_lenght, side_lenght, availableColors[color]);
}
