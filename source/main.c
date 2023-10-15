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
  Player player = player_create((Vector2) { .x = 200.f, .y = 300.f }, 100);
  Map* map = tilemap_load("castle-entrance", "resources/castle-entrance/castle-entrance-map.json", "resources/castle-entrance/castle-entrance-tileset.json");

  Cell cell = (Cell){
      .area = (Rectangle){.x = 0, .y = 0, .width = 600, .height = 800 }, 
      .bodies_head = malloc(sizeof(Cell_BodyNode)),
      .inclines_head = malloc(sizeof(Cell_InclineNode))
  };
  cell.bodies_head->body = NULL;
  cell.bodies_head->next = NULL;
  cell.bodies_head->prev = NULL;
  cell.inclines_head->incline = NULL;
  cell.inclines_head->next = NULL;
  cell.inclines_head->prev = NULL;

  player.body.cell = &cell;
  cell.bodies_head->body = &player.body;

  Collider* current_collider = map->colliders_head;
  Cell_BodyNode* current_cell_node = NULL;
  while (current_collider) {
      current_collider->body.cell = &cell;
      if (current_collider == map->colliders_head) {
          cell.bodies_head->next = malloc(sizeof(Cell_BodyNode));
          cell.bodies_head->next->next = NULL;
          cell.bodies_head->next->prev = NULL;
          cell.bodies_head->next->body = &current_collider->body;
          current_cell_node = cell.bodies_head->next;
      }
      else {
          current_cell_node->next = malloc(sizeof(Cell_BodyNode));
          current_cell_node->next->next = NULL;
          current_cell_node->next->prev = NULL;
          current_cell_node->next->body = &current_collider->body;
          current_cell_node = current_cell_node->next;
      }
      current_collider = current_collider->next;
  }

  Inclined_Collider* current_incline_collider = map->inclines_head;
  Cell_InclineNode* current_incline_node = NULL;
  while (current_incline_collider) {
      current_incline_collider->incline.cell = &cell;
      if (current_incline_collider == map->inclines_head) {
          cell.inclines_head->incline = &current_incline_collider->incline;
          current_incline_node = cell.inclines_head;
      }
      else {
          Cell_InclineNode *newNode = malloc(sizeof(Cell_InclineNode));
          current_incline_node->next = newNode;
          newNode->prev = current_incline_node;

          current_incline_node = newNode;
          current_incline_node->next = NULL;
          current_incline_node->incline = &current_incline_collider->incline;
      }
      current_incline_collider = current_incline_collider->next;
  }

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