#pragma once
#include "raylib.h"

typedef struct HashMap_Texture2D {
    struct HashMap_Texture2D_internal *_internal;
} HashMap_Texture2D;

void hashinit_Texture2D(HashMap_Texture2D *const map);
void hashclear_Texture2D(const HashMap_Texture2D map);

struct Texture2D *hashsearch_Texture2D(const HashMap_Texture2D map, const char *const key);
void hashinsert_Texture2D(const HashMap_Texture2D map, const char *const key, const Texture2D texture);