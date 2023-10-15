#include "level.h"
#include "raylib.h"
#include "stdio.h"

/************************************
 * Public functions implementations *
 ************************************/

void level_transition(HashMap_Texture2D level_textures, const enum GAME_LEVEL to){
    hashclear_Texture2D(level_textures);
    Texture2D t;
    switch(to){
        case CASTLE_ENTRANCE:
        printf(GetWorkingDirectory());
        hashinsert_Texture2D(level_textures, "player_idle", LoadTexture("resources/player/player_idle.png"));
        hashinsert_Texture2D(level_textures, "player_run", LoadTexture("resources/player/player_run.png"));
        hashinsert_Texture2D(level_textures, "player_jump", LoadTexture("resources/player/player_jump.png"));
        hashinsert_Texture2D(level_textures, "castle-entrance", LoadTexture("resources/castle-entrance/castle-entrance-tiles-packaged/castle-entrances-tiles-packaged.png"));
        break;
        case CASTLE:
        break;
    }
}