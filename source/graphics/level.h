#pragma once

#include "../datastructures/hashmap.h"


typedef enum {
    CASTLE_ENTRANCE,
    CASTLE
} GAME_LEVEL;

void level_transition(HashMap_Texture2D level_textures, const enum GAME_LEVEL to);