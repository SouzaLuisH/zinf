#include "player.h"
#include "graphic.h"

// Set Player Status to initial state
int player_init_status(Player *player, bool keep_weapon)
{
    if(keep_weapon ==false){
        player->hasWeapon = false;
    }
    player->isWeaponActive = false;
    player->lives = 3;
    player->orientation = SOUTH;
    player->score = 0;
    player->position.x = 0;
    player->position.y = 0;

    return 1;
}

int move_player(Player *player,Vector2D* new_position)
{
    player->position = *new_position;
    return 1;
}

int player_orientation(Player *player, unsigned char pressed_key)
{

    switch (pressed_key)
    {
    case 's':
        player->orientation = SOUTH;
        break;
    case 'w':
        player->orientation = NORTH;
        break;

    case 'd':
        player->orientation = EAST;

    case 'a':
        player->orientation = WEST;
        break;
    }

    return 1;
}

void get_player_initial_position(Player *player, char map[][24], int width, int height)
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            if (map[i][j] == MAP_PLAYER_SPACE)
            {
                player->position.x = j * TILE_SIZE;
                player->position.y = i * TILE_SIZE;
            }
        }
    }
}

// ----- weapon functions ------//

bool player_has_weapon(Player *player)
{
    return player->hasWeapon;
}

bool is_player_weapon_active(Player *player)
{
    return player->isWeaponActive;
}

bool handle_player_weapon(Player *player, bool pressed_key)
{

    // BUG: when player press 'J' and after only move the aswd keys, the weapon still be activated
    // possible solution: implement a debouncing
    if (player_has_weapon(player) == false)
    {
        return false; // player do not have weapon
    }

    if (pressed_key)
    {
        player->isWeaponActive = true;
    }
    else if (!pressed_key) // no key is pressed
    {
        player->isWeaponActive = false;
    }

    return true;
}

// ----- lives functions ------//

int get_player_lives(Player *player)
{
    return player->lives;
}

int set_player_lives(Player *player, int n_lives)
{

    if (n_lives >= 0)
    {
        player->lives = n_lives;
        return 1;
    }
    return 0;
}

int increment_player_lives(Player *player, int n_lives)
{

    if (n_lives >= 0)
    {
        player->lives += n_lives;
        return 1;
    }
    return 0;
}

// ----- graphic --------//
void draw_player(Player *player)
{
    float x_coord = player->position.x;
    float y_coord = player->position.y;
    Orientation orientation = player->orientation;

    if (player_has_weapon(player) == true)
    {
        if (is_player_weapon_active(player))
        {
            // draw player with weapon active TODO use orientation to draw the player
            driver_draw_square(x_coord, y_coord, PLAYER_HITBOX_SIZE, 7); // violet
        }
        else
        {
            // draw player with weapon stand-by TODO use orientation to draw the player
            driver_draw_square(x_coord, y_coord, PLAYER_HITBOX_SIZE, 6); // purple
        }
    }
    else
    {
        // draw player without wearpon TODO use orientation to draw the player
        driver_draw_square(x_coord, y_coord, PLAYER_HITBOX_SIZE, 4); // green
        // driver_print_text("Without Weapon ", x_coord, y_coord, 0);
    }
}