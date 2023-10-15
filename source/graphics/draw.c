#include "draw.h"
#include "raylib.h"
#include "../entities/player.h"
#include "animation.h"
#include "tilemap.h"
#include "level.h"
#include "../physics/incline.h"

/************************************
 * Public functions implementations *
 ************************************/

void draw_map(const Map *const map, const Texture2D* map_texture, const Vector2 camera)
{
    Tile_Layer* curr_layer = map->layers_head;
    while (curr_layer) {
        Tile* tiles = curr_layer->tiles;
        for (int i = 0; i < curr_layer->n_tiles; ++i) {
            Rectangle dst_rect_cam = (Rectangle){
                .x = tiles[i].dst_rect.x + camera.x,
                .y = tiles[i].dst_rect.y + camera.y,
                .width = tiles[i].dst_rect.width,
                .height = tiles[i].dst_rect.height
            };
            DrawTexturePro(*map_texture, tiles[i].src_rect, dst_rect_cam, (Vector2) { 0, 0 }, 0.f, WHITE);
        }
        curr_layer = curr_layer->next;
    }
}

void draw_player(const Player* const player, const Texture2D *texture, const Vector2 camera)
{
    Rectangle player_dest = (Rectangle){ .x = player->body.boundary.x - 106, .y = player->body.boundary.y - 30, .width = 128 * 2, .height = 64 * 2 };
    Rectangle player_dest_cam = (Rectangle){ .x = player_dest.x + camera.x, .y = player_dest.y + camera.y, .width = player_dest.width, .height = player_dest.height };
    Animation* animation = &player->animations[player->state];
    animation_step(animation);
    Rectangle src = (Rectangle){
        .x = animation->curr_col * animation->step_x,
        .y = animation->curr_line * animation->step_y,
        .width = animation->src_first_frame.width * (player->flip ? -1 : 1),
        .height = animation->src_first_frame.height };
    DrawTexturePro(*texture, src, player_dest_cam, (Vector2) { 0, 0 }, 0.0f, WHITE);

#ifdef DEBUG
    DrawRectangle(player->body.boundary.x + camera.x + player->body.boundary.width, player->body.boundary.y + camera.y + player->body.boundary.height, 10, 10, RED);
    DrawRectangleLines(player->body.boundary.x + camera.x, player->body.boundary.y + camera.y, 
        player->body.boundary.width, player->body.boundary.height, RED);
#endif
}

#ifdef DEBUG
void debug_draw_incline(Cell_InclineNode *inclines, Vector2 camera) {
    Cell_InclineNode* curr_incline = inclines;
    while (curr_incline) {
        DrawLine(curr_incline->incline->start.x + camera.x, curr_incline->incline->start.y + camera.y, curr_incline->incline->end.x + camera.x, curr_incline->incline->end.y + camera.y, BLUE);
        curr_incline = curr_incline->next;
    }
}
#endif
