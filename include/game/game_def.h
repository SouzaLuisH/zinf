#ifndef GAME_DEF
#define GAME_DEF
#include "stdbool.h"

//----- DEBUG ------//
#define DEBUG_PRINTS true

//----- GAME DEFINES ----//
#define DEFAULT_PLAYER_VELOCITY 50 //pixels per frame 
#define MAX_ENEMIES_PER_LEVEL 5

//---- GAME STRUTURES ----///
typedef struct {
    float x;
    float y;
} Vector2D;

typedef enum {
    SOUTH = 0,
    NORTH = 1,
    WEST  = 2,
    EAST  = 3
} Orientation;

typedef struct {
    Vector2D position;
    Orientation orientation;
    int score;
    int lives;
    bool hasWeapon;
    bool isWeaponActive;
    //warpon type
}Player;

typedef struct {
    Vector2D position;
    Orientation orientation;
    int lives;
    int reward;
}Enemies;




// --------------- FUNCTION HEADERS --------//
int testegame(char move);


#endif