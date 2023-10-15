#pragma once

#include "raylib.h"

typedef struct Flip {
	bool vertical;
	bool horizontal;
} Flip;

typedef struct Tile {
	Rectangle src_rect;
	Rectangle dst_rect;
} Tile;

typedef struct Tile_Layer {
	Tile* tiles;
	int n_tiles;
	struct Tile_Layer* next;
} Tile_Layer;

typedef struct Clickable {
	Tile tile;
	struct Clickable* next;
} Clickable;

typedef struct Map {
	char *source_texture;
	Tile_Layer* layers_head;
	struct Collider* colliders_head;
	Clickable* clickables_head;
	struct Inclined_Collider* inclines_head;
} Map;

Map* tilemap_load(const char* map_texture, const char* map_path, const char* tiles_path);
