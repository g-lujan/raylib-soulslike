#pragma once
#include "raylib.h"

void draw_map(const struct Map* const map, const Texture2D* map_texture, const Vector2 camera);
void draw_player(const struct Player *const player, const Texture2D* player_texture, const Vector2 camera);

#ifdef DEBUG
void debug_draw_incline(struct Cell_InclineNode* inclines, Vector2 camera);
#endif

