#ifndef GAME_DEF
#define GAME_DEF
#include "stdbool.h"

//----- DEBUG ------//
#define DEBUG_PRINTS true

//----- GAME DEFINES ----//
#define DEFAULT_PLAYER_VELOCITY 20 //pixels per frame 

//---- GAME STRUTURES ----///
typedef struct {
    float x;
    float y;
} Vector2D;

typedef struct {
    Vector2D position;
    enum {south= 0, north=1, west = 2, east = 3} orientation;
    int score;
    int lives;
    bool hasWeapon;
    bool isWeaponActive;
    //warpon type
}Player;

// --------------- FUNCTION HEADERS --------//
int testegame(char move);


#endif