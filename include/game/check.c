#include "check.h"

#include <stdio.h>

#include "archive.h"
#include "keyboard.h"

bool check_wall_colision(Vector2D position, Game_State *map) {
    const float HITBOX_OFFSET = (TILE_SIZE - PLAYER_HITBOX_SIZE) / 2.0f;

    // Define a área de colisão (hitbox) do jogador, centralizada
    float player_hitbox_x = position.x;
    float player_hitbox_y = position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    for (int i = 0; i < map->n_walls; i++) {
        float wall_x = map->walls[i].x;
        float wall_y = map->walls[i].y;
        float wall_width = TILE_SIZE;
        float wall_height = TILE_SIZE;

        // Verifica colisão AABB entre o jogador e a parede
        if (player_hitbox_x < wall_x + wall_width && player_hitbox_x + player_hitbox_width > wall_x && player_hitbox_y < wall_y + wall_height && player_hitbox_y + player_hitbox_height > wall_y) {
            return true;
        }
    }

    // Verifica colisão com as bordas do mapa
    float map_width_pixels = MAP_WIDTH * TILE_SIZE;
    float map_height_pixels = MAP_HEIGHT * TILE_SIZE;

    if (player_hitbox_x < 0 || player_hitbox_x + player_hitbox_width > map_width_pixels) return true;
    if (player_hitbox_y < 0 || player_hitbox_y + player_hitbox_height > map_height_pixels) return true;

    return false;
}

bool check_life_colision(Vector2D position, int *life_position, Game_State *map) {
    float player_hitbox_x = position.x;
    float player_hitbox_y = position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    for (int i = 0; i < map->n_lives; i++) {
        float life_x = map->lives[i].position.x;
        float life_y = map->lives[i].position.y;
        float life_width = TILE_SIZE;
        float life_height = TILE_SIZE;

        // Verifica colisão AABB entre o jogador e a parede
        if (player_hitbox_x < life_x + life_width && player_hitbox_x + player_hitbox_width > life_x && player_hitbox_y < life_y + life_height && player_hitbox_y + player_hitbox_height > life_y) {
            *life_position = i;
            return true;
        }
    }

    return false;
}

bool check_weapon_colision(Vector2D position, int *weapon_position, Game_State *map) {
    // Define a área de colisão(hitbox) do jogador, centralizada
    float player_hitbox_x = position.x;
    float player_hitbox_y = position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    for (int i = 0; i < map->n_weapons; i++) {
        float weapon_x = map->weapons[i].position.x;
        float weapon_y = map->weapons[i].position.y;
        float weapon_width = TILE_SIZE;
        float weapon_height = TILE_SIZE;

        // Verifica colisão AABB entre o jogador e a parede
        if (player_hitbox_x < weapon_x + weapon_width && player_hitbox_x + player_hitbox_width > weapon_x && player_hitbox_y < weapon_y + weapon_height && player_hitbox_y + player_hitbox_height > weapon_y) {
            *weapon_position = i;
            return true;
        }
    }

    return false;
}

bool check_monster_player_colision(Vector2D player_position, Vector2D monster_position) {
    float player_hitbox_x = player_position.x;
    float player_hitbox_y = player_position.y;
    float player_hitbox_width = PLAYER_HITBOX_SIZE;
    float player_hitbox_height = PLAYER_HITBOX_SIZE;

    float monster_size = TILE_SIZE;

    // AABB colision
    if (player_hitbox_x < monster_position.x + monster_size && player_hitbox_x + player_hitbox_width > monster_position.x && player_hitbox_y < monster_position.y + monster_size && player_hitbox_y + player_hitbox_height > monster_position.y) {
        return true;
    }

    return false;
}

bool check_monster_weapon_colision(Player *player, Vector2D monster_position) {
    float weapon_hitbox_x = player->position.x;
    float weapon_hitbox_y = player->position.y;
    float weapon_hitbox_width = TILE_SIZE;
    float weapon_hitbox_height = TILE_SIZE;

    float monster_size = TILE_SIZE;

    // AABB colision - set the size and position of weapon area according the player orientation
    switch (player->orientation) {
        case NORTH:
            weapon_hitbox_y -= PLAYER_HITBOX_SIZE * WEAPON_N_OF_TILES;
            weapon_hitbox_height = PLAYER_HITBOX_SIZE * WEAPON_N_OF_TILES;
            break;
        case SOUTH:
            weapon_hitbox_y += PLAYER_HITBOX_SIZE;
            weapon_hitbox_height = PLAYER_HITBOX_SIZE * WEAPON_N_OF_TILES;
            break;
        case WEST:
            weapon_hitbox_x -= PLAYER_HITBOX_SIZE * WEAPON_N_OF_TILES;
            weapon_hitbox_width = PLAYER_HITBOX_SIZE * WEAPON_N_OF_TILES;
            break;
        case EAST:
            weapon_hitbox_x += PLAYER_HITBOX_SIZE;
            weapon_hitbox_width = PLAYER_HITBOX_SIZE * WEAPON_N_OF_TILES;
            break;
    }
    if (player->hasWeapon && player->isWeaponActive) {
        if (weapon_hitbox_x < monster_position.x + monster_size && weapon_hitbox_x + weapon_hitbox_width > monster_position.x && weapon_hitbox_y < monster_position.y + monster_size && weapon_hitbox_y + weapon_hitbox_height > monster_position.y) {
            return true;
        }
    }

    return false;
}

bool check_win_condition(Game_State *map) {
    for (int i = 0; i < map->n_monsters; i++) {
        if (map->monsters[i].isEnable == true) {
            return false;
        }
    }
    return true;
}

bool check_defeat_condition(Player player) {
    if (player.lives == 0) {
        return true;
    }
    return false;
}

bool check_user_active_weapon(uint8_t input) {
    if (input & KEY_BIT_J) {
        return true;
    }
    return false;
}

bool check_is_available_ranking(int i_score) {
    int n_elements = 0;
    int lowest_score = get_lowest_ranking_score(&n_elements);

    if (i_score > lowest_score) {
        return true;
    }

    if (n_elements < MAX_RANKING_ITENS) {
        return true;
    }

    return false;
}