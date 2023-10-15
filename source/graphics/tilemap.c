#define CUTE_TILED_IMPLEMENTATION
#define CUTE_TILED_NO_EXTERNAL_TILESET_WARNING
#include "cute_tiled.h"
#include "raylib.h"
#include "tilemap.h"
#include "../physics/body.h"
#include "../physics/incline.h"
#include <stdlib.h>


/********************************************
 * Forward declaration of private functions *
 ********************************************/

Tile_Layer* generate_tilelayer(cute_tiled_layer_t* tiled_layer, cute_tiled_map_t* tiled_map, int num_tileset_cols);
Collider* generate_colliders(cute_tiled_layer_t* tiled_layer);
Clickable* generate_clickables(cute_tiled_layer_t* tiled_layer);
Inclined_Collider* generate_inclines(cute_tiled_layer_t* tiled_layer);
Tile create_tile(int raw_tile_id, int num_tileset_cols, Rectangle dst_rect);


/*********************************************************************************************
 * Constants used to retrieve bit info in tile id from Tiled                                 *
 * - Bits on the far end of the 32-bit gid are used for tile flags                           *
 * - Reference: https://doc.mapeditor.org/en/stable/reference/global-tile-ids/#tile-flipping *
 *********************************************************************************************/

const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;  // bit 32
const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;    // bit 31
const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;    // bit 30, the diag '\'
const unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000; // bit 29, unused in our case

/************************************
 * Public functions implementations *
 ************************************/

Map* tilemap_load(const char* map_texture, const char* map_path, const char* tiles_path)
{
    /********************************
     * Load the data from the jsons *
     ********************************/

    cute_tiled_map_t* tiled_map = cute_tiled_load_map_from_file(map_path, NULL);
    cute_tiled_layer_t* tiled_layer = tiled_map->layers;
    cute_tiled_tileset_t* tilesets = cute_tiled_load_external_tileset(tiles_path, NULL);
    int num_tileset_cols = tilesets->imagewidth / tilesets->tilewidth;

    /************************************************************
     * Create a map object that will be populated with the data *
     ************************************************************/

    Map* map = malloc(sizeof(Map));
    map->source_texture = map_texture;
    map->layers_head = NULL;
    map->colliders_head = NULL;
    map->layers_head = NULL;

    /************************************************************
     * iterate over every layer of the map from the json file   *
     ************************************************************/

    Tile_Layer* curr_layer = NULL;
    while (tiled_layer) {
        if (strcmp(tiled_layer->type.ptr, "tilelayer") == 0) {
            if (map->layers_head == NULL) {
                map->layers_head = generate_tilelayer(tiled_layer, tiled_map, num_tileset_cols);
                curr_layer = map->layers_head;
            }
            else if(curr_layer) {
                curr_layer->next = generate_tilelayer(tiled_layer, tiled_map, num_tileset_cols);
                curr_layer = curr_layer->next;
            }
        }
        else if (strcmp(tiled_layer->name.ptr, "Colliders") == 0) {
            map->colliders_head = generate_colliders(tiled_layer);
        }
        else if (strcmp(tiled_layer->name.ptr, "Selectors") == 0) {
            map->clickables_head = generate_clickables(tiled_layer);
        }
        else if (strcmp(tiled_layer->name.ptr, "Stairs") == 0) {
            map->inclines_head = generate_inclines(tiled_layer);
        }
        tiled_layer = tiled_layer->next;
    }
    cute_tiled_free_map(tiled_map);
    return map;
}

/************************************
 * Private functions implementations *
 ************************************/

Tile create_tile(int raw_tile_id, int num_tileset_cols, Rectangle dst_rect)
{
    bool horizontal_flip = (raw_tile_id & FLIPPED_HORIZONTALLY_FLAG);
    bool vertical_flip = (raw_tile_id & FLIPPED_VERTICALLY_FLAG);
    bool diag_flip = (raw_tile_id & FLIPPED_DIAGONALLY_FLAG);
    Flip flip = horizontal_flip && vertical_flip ? (Flip) { .horizontal = true, .vertical = true }
    : !diag_flip && horizontal_flip ? (Flip) { .horizontal = true, .vertical = false }
    : !diag_flip && vertical_flip ? (Flip) { .horizontal = false, .vertical = true }
    : (Flip) { .horizontal = false, .vertical = false };
    // Clear all four flags to get actual id
    int tile_id = (raw_tile_id & ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG | ROTATED_HEXAGONAL_120_FLAG));
    int src_x = (tile_id - 1) % num_tileset_cols;
    int src_y = (int)((tile_id - 1) / num_tileset_cols);

    return (Tile) { 
        .src_rect = { 32 * src_x, 32 * src_y, 32 * (flip.horizontal? -1 : 1), 32 * (flip.vertical ? -1 : 1) },
        .dst_rect = dst_rect
    };
}

Clickable* generate_clickables(cute_tiled_layer_t* tiled_layer)
{
    cute_tiled_object_t* curr_object = tiled_layer->objects;
    Clickable* clickables_head = NULL;
    Clickable* clickables_curr = NULL;
    while (curr_object) {
        Clickable* clickable = malloc(sizeof(Clickable));
        clickable->tile = (Tile){ .dst_rect = (Rectangle){.x = curr_object->x, .y = curr_object->y, .width = curr_object->width, .height = curr_object->height},
                                 .src_rect = (Rectangle){.x = curr_object->x, .y = curr_object->y, .width = curr_object->width, .height = curr_object->height}
        };
        if (clickables_head == NULL) {
            clickables_head = clickable;
            clickables_curr = clickables_head;
        }
        else {
            clickables_curr->next = clickable;
            clickables_curr = clickables_curr->next;
        }
        curr_object = curr_object->next;
    }
    return clickables_head;
}

Collider* generate_colliders(cute_tiled_layer_t* tiled_layer)
{
    cute_tiled_object_t* curr_object = tiled_layer->objects;
    Collider* colliders_head = NULL;
    Collider* colliders_curr = NULL;
    while (curr_object) {
        Collider* collider = malloc(sizeof(Collider));
        Body body = (Body){
            .speed = (Vector2){.x = 0, .y = 0},
            .acceleration = (Vector2){.x = 0, .y = 0} ,
            .cell = NULL,
            .boundary = (Rectangle){.x = curr_object->x, .y = curr_object->y, .width = curr_object->width, .height = curr_object->height },
            .kinematic = false,
        };
        collider->body = body;
        collider->next = NULL;
        if (colliders_head == NULL) {
            colliders_head = collider;
            colliders_curr = colliders_head;
        }
        else {
            colliders_curr->next = collider;
            colliders_curr = collider;
        }
        curr_object = curr_object->next;
    }
    return colliders_head;
}

Inclined_Collider* generate_inclines(cute_tiled_layer_t* tiled_layer)
{
    cute_tiled_object_t* curr_object = tiled_layer->objects;
    Inclined_Collider* inclines_head = NULL;
    Inclined_Collider* inclines_curr = NULL;
    while (curr_object) {
        Inclined_Collider* inclined_collider = malloc(sizeof(Inclined_Collider));
        inclined_collider->incline = (Incline){
            .start = (Vector2){.x = curr_object->x, .y = curr_object->y },
            .end = (Vector2){.x = curr_object->x + curr_object->vertices[2], .y = curr_object->y + curr_object->vertices[3] },
            .slope = (curr_object->vertices[3] - curr_object->vertices[1]) / (curr_object->vertices[2] - curr_object->vertices[0])
        };
        inclined_collider->next = NULL;
        if (inclines_head == NULL) {
            inclines_head = inclined_collider;
            inclines_curr = inclines_head;
        }
        else {
            inclines_curr->next = inclined_collider;
            inclines_curr = inclines_curr->next;
        }
        curr_object = curr_object->next;
    }
    return inclines_head;
}

Tile_Layer* generate_tilelayer(cute_tiled_layer_t* tiled_layer, cute_tiled_map_t* tiled_map, int num_tileset_cols)
{
    // layer object that we will use to hold the tiles
    Tile_Layer* layer = malloc(sizeof(Tile_Layer));
    layer->tiles = malloc(tiled_layer->data_count * sizeof(Tile));
    layer->n_tiles = tiled_layer->data_count;
    layer->next = NULL;
    // build the current tile to put in the layer
    int dst_tile_x = 0, dst_tile_y = 0;
    for (int i = 0; i < tiled_layer->data_count; ++i, dst_tile_x++) {
        if (dst_tile_x == tiled_map->width) {
            dst_tile_y++;
            dst_tile_x = 0;
        }
        layer->tiles[i] = create_tile(tiled_layer->data[i], num_tileset_cols, (Rectangle) { dst_tile_x * 32, dst_tile_y * 32, 32, 32 });
    }
    return layer;
}

void map_clear(Map* map) {
    // Layer is malloc'd once
    Tile_Layer* curr_layer = map->layers_head;
    while (curr_layer) {
        // Tiles malloc'd many times
        free(curr_layer->tiles);
        curr_layer = curr_layer->next;
    }
    free(map->layers_head);

    // Colliders are malloc'd many times 
    Collider* temp = NULL;
    while (map->colliders_head) {
        temp = map->colliders_head;
        map->colliders_head = map->colliders_head->next;
        free(temp);
    }
}
