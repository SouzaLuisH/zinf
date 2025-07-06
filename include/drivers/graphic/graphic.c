#include "graphic.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "raylib.h"

#define MAX_INPUT_CHARS 32
#define PLAYER_ANIM_FRAMES 4

// player
static Texture2D Player_North[PLAYER_ANIM_FRAMES] = {0};
static Texture2D Player_South[PLAYER_ANIM_FRAMES] = {0};
static Texture2D Player_West[PLAYER_ANIM_FRAMES] = {0};
static Texture2D Player_East[PLAYER_ANIM_FRAMES] = {0};

// monster
static Texture2D Monster_North = {0};
static Texture2D Monster_South = {0};
static Texture2D Monster_West = {0};
static Texture2D Monster_East = {0};

// element
static Texture2D Sword = {0};
static Texture2D Wall = {0};
static Texture2D Life = {0};
static Texture2D Magic = {0};

// camera
static Camera2D camera = {0};
static float camera_zoom = 1.2f;


// ------------ Help Functions --------------------------//

float get_frame_time() { return GetFrameTime(); }

static void get_frame_path(char *path, int tam, const char *name_fmt,int frame) {
  char filename[50];
  snprintf(filename, sizeof(filename), name_fmt, frame);
  snprintf(path, tam, "%s/%s", PLAYER_SPRITES_PATH, filename);
}
//-----------------------------------------------------------//

// ------------ Camera Functions --------------------------//
void driver_camera_init(int screen_widht,int screen_height, int offset ) {
  camera.zoom = camera_zoom;
  camera.offset = (Vector2){screen_widht / 2.0f, screen_height / 2.0f};
}

void driver_camera_set_target(float x, float y){
  Vector2 player_center =(Vector2){x + 50 / 2.0f, y + 50 / 2.0f};
  camera.target.x += (player_center.x - camera.target.x) * 0.1f;
  camera.target.y += (player_center.y - camera.target.y) * 0.1f;

}

void driver_camera_begin(){
    BeginMode2D(camera);

}

void driver_camera_end(){
    EndMode2D();  
}
//-----------------------------------------------------------//


// ------------ Load Textures Function --------------------------//
void driver_load_all_textures() {
  char path[256];

  // player
  for (int i = 0; i < PLAYER_ANIM_FRAMES; i++) {
    get_frame_path(path, sizeof(path), "jogador-sul-%d.png", i + 1);
    Player_South[i] = LoadTexture(path);

    get_frame_path(path, sizeof(path), "jogador-norte-%d.png", i + 1);
    Player_North[i] = LoadTexture(path);

    get_frame_path(path, sizeof(path), "jogador-oeste-%d.png", i + 1);
    Player_West[i] = LoadTexture(path);

    get_frame_path(path, sizeof(path), "jogador-leste-%d.png", i + 1);
    Player_East[i] = LoadTexture(path);
  }

  // monster
  Monster_North = LoadTexture(MONSTER_SPRITES_PATH "monstro-norte.png");
  Monster_South = LoadTexture(MONSTER_SPRITES_PATH "monstro-sul.png");
  Monster_West = LoadTexture(MONSTER_SPRITES_PATH "monstro-oeste.png");
  Monster_East = LoadTexture(MONSTER_SPRITES_PATH "monstro-leste.png");

  // elements
  Sword = LoadTexture(ELEMENTS_SPRITES_PATH "espada.png");
  Wall = LoadTexture(ELEMENTS_SPRITES_PATH "parede.png");
  Life = LoadTexture(ELEMENTS_SPRITES_PATH "vida.png");
  Magic = LoadTexture(ELEMENTS_SPRITES_PATH "magic.png");
}

void driver_unload_all_textures() {
  // player
  for (int i = 0; i < PLAYER_ANIM_FRAMES; i++) {
    UnloadTexture(Player_South[i]);
    UnloadTexture(Player_North[i]);
    UnloadTexture(Player_West[i]);
    UnloadTexture(Player_East[i]);
  }

  // monster
  UnloadTexture(Monster_North);
  UnloadTexture(Monster_South);
  UnloadTexture(Monster_West);
  UnloadTexture(Monster_East);

  // elements
  UnloadTexture(Sword);
  UnloadTexture(Wall);
  UnloadTexture(Life);
  UnloadTexture(Magic);
}
//-----------------------------------------------------------//


// ------------ Draw Game Elements --------------------------//
void driver_print_player(float x, float y, int orientation, int is_moving) {
  enum { SOUTH = 0, NORTH = 1, WEST = 2, EAST = 3 } Orientation_enum;

  Orientation_enum = orientation;
  Vector2 position = {x, y};
  Texture2D *frames = NULL;

  float scale = 0.13f;
  switch (Orientation_enum) {
    case NORTH:
      frames = Player_North;
      break;
    case SOUTH:
      frames = Player_South;
      break;
    case WEST:
      frames = Player_West;
      break;
    case EAST:
      frames = Player_East;
      break;
  }

  if (frames != NULL) {
    float time = GetTime();
    int frame = ((int)(time * 8)) % PLAYER_ANIM_FRAMES;  // index of frame 8 fps

    if (is_moving) {
      DrawTextureEx(frames[frame], position, 0.0f, scale, WHITE);
    } else {
      DrawTextureEx(frames[0], position, 0.0f, scale, WHITE);
    }
  }
}

void driver_print_monster(float x, float y, int orientation) {
  enum { SOUTH = 0, NORTH = 1, WEST = 2, EAST = 3 } Orientation_enum;

  Orientation_enum = orientation;
  Vector2 position = {x, y};
  Texture2D *Monster = NULL;

  float scale = 0.16f;

  switch (Orientation_enum) {
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

  if (Monster != NULL) {
    DrawTextureEx(*Monster, position, 0.0f, scale, WHITE);
  }
}

void driver_print_element(float x, float y, char element) {
  Vector2 position = {x, y};
  float scale = 0.16f;
  Texture2D *Element = NULL;
  switch (element) {
    case 'P':
      Element = &Wall;
      scale = 0.24;
      break;
    case 'V':
      Element = &Life;
      break;
    case 'E':
      Element = &Sword;
      scale = 0.11;
      break;
    case 'M':
      Element = &Magic;
      scale = 0.11;
      break;
  }
  if (Element != NULL) {
    DrawTextureEx(*Element, position, 0.0f, scale, WHITE);
  }
}

void driver_draw_background(int offset) {
  static Texture2D background = {0};
  if (background.id == 0) {
    background = LoadTexture(BACKGROUND_PATH);
  }
  DrawTexture(background, 0, offset, WHITE);
}
//-----------------------------------------------------------//


// ------------ Draw Menu Screens --------------------------//
void driver_print_statusboard(int player_lives, int player_score,int game_stage, int width, int height) {
  char buffer[60] = {0};
  sprintf(buffer, "Lives: %d  Score: %d   Stage: %d", player_lives,player_score, game_stage);
  DrawRectangle(0, 0, width, height, GRAY);
  driver_print_text(buffer, 10, 10, 0);
}

void driver_print_end_game_victory(int height) {
   ClearBackground(BLACK);
  int offset_x = 200, offset_y = height / 2;
  driver_print_text("YOU WON !", offset_x, offset_y, 4);;
  driver_print_text("Press Enter to continue", offset_x, offset_y * 1.4, 0);
}

void driver_print_end_game_defeat(int height) {
  ClearBackground(BLACK);
  int offset_x = 200, offset_y = height / 2;
  driver_print_text("YOU DIED !", offset_x, offset_y, 4);;
  driver_print_text("Press Enter to continue", offset_x, offset_y * 1.4, 0);
}

void driver_print_get_name(char *name,int height) {
  ClearBackground(BLACK);
  float offset_x = 200, offset_y = height / 2;
  DrawText("Enter your nickname to insert into the scoreboard", offset_x, offset_y*1.2, 30, GREEN);
  
  Rectangle textBox = {offset_x, offset_y*1.4, 350, 60};
  DrawRectangleRec(textBox, GRAY);
  DrawText(name, (int)textBox.x + 5, (int)textBox.y + 15, 30, GREEN);

  driver_print_text("Press Enter to continue", offset_x, offset_y * 1.6, 0);
}

void driver_print_text(char *text, int x, int y, int color) {
  Color availableColors[] = {
      WHITE, GRAY, BLACK, RED, GREEN,
  };

  DrawText(text, x, y, 40, availableColors[color]);
}

void driver_draw_square(float x, float y, float side_lenght, int color) {
  if (color < 0) {
    return;
  }

  Color availableColors[] = {
      WHITE,   // 0
      GRAY,    // 1
      BLACK,   // 2
      RED,     // 3
      GREEN,   // 4
      YELLOW,  // 5
      PURPLE,  // 6
      VIOLET,  // 7
      GOLD,    // 8
  };

  DrawRectangle(x, y, side_lenght, side_lenght, availableColors[color]);
}
//-----------------------------------------------------------//
