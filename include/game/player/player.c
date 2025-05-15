#include "player.h"
#include "graphic.h"

// Set Player Status to initial state
int player_init_status(Player *player)
{
    player->hasWeapon = false;
    player->isWeaponActive = false;
    player->lives = 3;
    player->orientation = south;
    player->score = 0;
    player->position.x = 0;
    player->position.y = 0;

    return 1;
}

int player_movimentation(Player *player, unsigned char pressed_key)
{
    float move_x = pressed_key == 'd' ? 1.00 : pressed_key == 'a' ? -1.00
                                                                  : 0;
    float move_y = pressed_key == 'w' ? -1.00 : pressed_key == 's' ? 1.00
                                                                   : 0;

    player->position.x += move_x * DEFAULT_PLAYER_VELOCITY;
    player->position.y += move_y * DEFAULT_PLAYER_VELOCITY;

    return 1;
}

int player_orientation(Player *player, unsigned char pressed_key)
{

    switch (pressed_key)
    {
    case 's':
        player->orientation = south;
        break;
    case 'w':
        player->orientation = north;
        break;

    case 'd':
        player->orientation = east;

    case 'a':
        player->orientation = west;
        break;
    }

    return 1;
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
bool handle_player_weapon(Player *player, unsigned char pressed_key)
{
    if (player_has_weapon(player) == false)
    {
        return false; // player do not have weapon
    }

    if (pressed_key == 'j')
    {
        player->isWeaponActive = true;
    }
    else if (pressed_key == '#') // no key is pressed
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

    if (player_has_weapon(player) == true)
    {
        if (is_player_weapon_active(player))
        {
            // draw player with weapon active
            DrawText("Weapon active", x_coord, y_coord, 20, WHITE);
        }
        else
        {
            // draw player with weapon stand-by
            DrawText("Weapon stand-by", x_coord, y_coord, 20, WHITE);
        }
    }
    else
    {
        // draw player without wearpon
        DrawText("Without Weapon ", x_coord, y_coord, 20, WHITE);
    }
}