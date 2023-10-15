#pragma once

#include "raylib.h"
#include "../graphics/animation.h"
#include "../physics/body.h"

typedef enum {
    PLAYER_IDLE,
    PLAYER_RUN,
    PLAYER_ATTACKING_SWORD,
    PLAYER_ATTACKING_CROSSBOW,
    PLAYER_ATTACKING_SPECIAL,
    PLAYER_DEFENDING,
    PLAYER_DAMAGED,
    PLAYER_JUMP,
    PLAYER_FALLING,
    N_PLAYER_STATES
} PLAYER_STATE;

typedef struct Player
{
    Vector2 pos;
    unsigned char health;
    enum PLAYER_STATE state;
    Animation animations[N_PLAYER_STATES];
    Body body;
    bool flip;
} Player;

Player player_create(Vector2 start_pos, unsigned char max_health);
void update_player(Player* player, struct Input* input);