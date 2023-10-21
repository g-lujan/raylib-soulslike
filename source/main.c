#include "raylib.h"
#include "controls/input.h"
#include "graphics/level.h"
#include "datastructures/hashmap.h"
#include "entities/player.h"
#include "graphics/animation.h"
#include "graphics/draw.h"
#include "graphics/tilemap.h"
#include "physics/body.h"
#include "physics/incline.h"
#include "entities/cell.h"

#include "stdlib.h"
#include "stdio.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

void game_loop(void);
void cap_framerate(void);

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "GAME");
  SetTargetFPS(FPS);
  game_loop();
  CloseWindow();
  return 0;
}

void game_loop(void){
  HashMap_Texture2D LEVEL_TEXTURES;
  hashinit_Texture2D(&LEVEL_TEXTURES);
  level_transition(LEVEL_TEXTURES, CASTLE_ENTRANCE);
  Map* map = tilemap_load("castle-entrance", "resources/castle-entrance/castle-entrance-map.json", "resources/castle-entrance/castle-entrance-tileset.json");
  Cell cell = create_cell(map);
  Player player = player_create((Vector2) { .x = 200.f, .y = 300.f }, 100);
  add_to_cell_head(&cell, &player.body);

  Input input;
  init_input(&input);
  while (!WindowShouldClose()) {
    cap_framerate();

    update_input(&input);
    update_player(&player, &input);
    update_cell(&cell);

    /* Draw step */
    BeginDrawing();
        ClearBackground(BLACK);
        Vector2 camera = (Vector2){ .x = (SCREEN_WIDTH/2) - player.body.boundary.x, .y = (SCREEN_HEIGHT/2) - player.body.boundary.y };
        draw_map(map, hashsearch_Texture2D(LEVEL_TEXTURES, map->source_texture), camera);
        draw_player(&player, hashsearch_Texture2D(LEVEL_TEXTURES, player.animations[player.state].texture_key), camera);
#ifdef DEBUG
        debug_draw_incline(cell.inclines_head, camera);
#endif
    EndDrawing();
  }
}


void cap_framerate(void)
{
  const float MIN_TIME_BETWEEN_FRAMES = 1 / 60.f;
  const float elapsed = GetFrameTime();
  if (MIN_TIME_BETWEEN_FRAMES > elapsed) {
    WaitTime(MIN_TIME_BETWEEN_FRAMES - elapsed);
  }
}