#ifndef GAME_DEF
#define GAME_DEF
#include "stdbool.h"

//----- DEBUG DEFINES ------//
#define DEBUG_PRINTS true
#define ENABLE_MONSTER_MOVE true

//----- ARCHIVE DEFINES ----//
#define MAP_PATH_PREFIX "../../resources/maps/mapa"
#define MAX_LENGHT_NICKNAME 32

//----- RANKING DEFINES ----//
#define MAX_RANKING_ITENS 5

//----- GAME DEFINES ----//
#define DEFAULT_PLAYER_VELOCITY 220   // pixels per second
#define DEFALUT_ENEMIES_VELOCITY 200  // pixels per second
#define TILE_SIZE 50.0f
#define PLAYER_HITBOX_SIZE 44.0f
#define WEAPON_N_OF_TILES 3
#define TIME_PLAYER_INTOCHABLE_AFTER_DAMAGE 1.5f
#define PLAYER_BLINK_PERIOD 0.2f
#define STATUS_BOARD_OFFSET 60

//----- SCREEN DEFINES -----//
#define WINDOW_WIDHT TILE_SIZE *MAP_WIDTH
#define WINDOW_HEIGHT (TILE_SIZE * MAP_HEIGHT + STATUS_BOARD_OFFSET + 20)

//----- MATH DEFINES ----//
#define ROOT_SQUARE_OF_2 1.4142135f

//----- MAP DEFINES ----//
#define MAP_WIDTH 24
#define MAP_HEIGHT 16
#define MAP_BLANC_SPACE ' '
#define MAP_PLAYER_SPACE 'J'
#define MAP_MONSTER_SPACE 'M'
#define MAP_WEAPON_SPACE 'E'
#define MAP_LIFE_SPACE 'V'
#define MAP_WALL_SPACE 'P'

// TODO move colors to graphic.h
#define MAP_COLOR_EMPTY 2    // BLACK
#define MAP_COLOR_MONSTER 5  // YELLOW
#define MAP_COLOR_WEAPON 1   // GRAY
#define MAP_COLOR_LIFE 3     // RED
#define MAP_COLOR_WALL 0     // WHITE
#define MAP_COLOR_UNKNOWN -1

//---- GAME STRUTURES ----///
typedef struct {
    float x;
    float y;
} Vector2D;

typedef enum { SOUTH = 0, NORTH = 1, WEST = 2, EAST = 3 } Orientation;

typedef struct {
    Vector2D position;
    Orientation orientation;
    int score;
    int lives;
    bool hasWeapon;
    bool isWeaponActive;
    float last_damage_time;
    bool isVisible;
    bool isMoving;
    // warpon type
} Player;

typedef struct {
    Vector2D position;
    Orientation orientation;
    int lives;
    int reward;
    bool isEnable;
    float last_change_time;
} Enemies;

typedef struct {
    Vector2D position;
    bool isEnable;
} Element;

typedef struct {  // TODO : create a level flag to use later, when we implement the continue game function
    int n_walls;
    Vector2D *walls;
    int n_monsters;
    Enemies *monsters;
    int n_lives;
    Element *lives;
    int n_weapons;
    Element *weapons;
    int current_stage;
} Game_State;

#endif