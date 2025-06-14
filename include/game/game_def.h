#ifndef GAME_DEF
#define GAME_DEF
#include "stdbool.h"

//----- DEBUG ------//
#define DEBUG_PRINTS true

//----- GAME DEFINES ----//
#define DEFAULT_PLAYER_VELOCITY 50 // pixels per frame
#define MAX_ENEMIES_PER_LEVEL 5

//----- MAP DEFINES ----//
#define MAP_BLANC_SPACE ' '
#define MAP_PLAYER_SPACE 'J'
#define MAP_MONSTER_SPACE 'M'
#define MAP_WEAPON_SPACE 'E'
#define MAP_LIFE_SPACE 'V'
#define MAP_WALL_SPACE 'P'

#define MAP_WIDTH 24
#define MAP_HEIGHT 16
#define ITEM_SIZE 50

//TODO move colors to graphic.h
#define MAP_COLOR_EMPTY 2   // BLACK
#define MAP_COLOR_MONSTER 5 // YELLOW
#define MAP_COLOR_WEAPON 1  // GRAY
#define MAP_COLOR_LIFE 3    // RED
#define MAP_COLOR_WALL 0    // WHITE
#define MAP_COLOR_UNKNOWN -1



//---- GAME STRUTURES ----///
typedef struct
{
    float x;
    float y;
} Vector2D;

typedef enum
{
    SOUTH = 0,
    NORTH = 1,
    WEST = 2,
    EAST = 3
} Orientation;

typedef struct
{
    Vector2D position;
    Orientation orientation;
    int score;
    int lives;
    bool hasWeapon;
    bool isWeaponActive;
    // warpon type
} Player;

typedef struct
{
    Vector2D position;
    Orientation orientation;
    int lives;
    int reward;
    bool isEnable;
} Enemies;

typedef struct 
{
    Vector2D position;
    bool isEnable;
} Element;



typedef struct
{   //TODO : create a level flag to use later, when we implement the continue game function
    int n_walls;
    Vector2D *walls;
    int n_monsters;
    Enemies *monsters;
    int n_lives;
    Element *lives; // TODO create a struct to flag if is_enable
    int n_weapons;
    Element *weapons;// TODO create a struct to flag if is_enable
} Game_State;

// --------------- FUNCTION HEADERS --------//
void game_loop(char move);
int init_game_data();
int finish_game();

#endif