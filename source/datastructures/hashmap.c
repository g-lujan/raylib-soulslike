#include "hashmap.h"
#include "raylib.h"
#include <stdlib.h>

/*
 * We will probably have less than 10 textures per level, so 200 is more than enough to minimize collisions
 * when making a hash by the string that identifies the texture.
 */
#define HASHMAP_TEX2D_SLOTS 200


/**********************
 * Private structures *
 **********************/

struct mapval_Texture2D {
    Texture2D texture;
    bool active;
    unsigned int key;
};

/*
 * We will not make deletions to this map, as when we need to transition to a new level that needs a
 * different set of textures, we can simply drop all textures currently in the map and load new ones.
 */
struct HashMap_Texture2D_internal {
    struct mapval_Texture2D underlying_array[HASHMAP_TEX2D_SLOTS];
};


/********************************************
 * Forward declaration of private functions *
 ********************************************/

unsigned int key_to_number(const char *const key);


/************************************
 * Public functions implementations *
 ************************************/

void hashinit_Texture2D(HashMap_Texture2D *const map){
    map->_internal = malloc(sizeof(struct HashMap_Texture2D_internal));
    for(int i = 0; i < HASHMAP_TEX2D_SLOTS; ++i){
        map->_internal->underlying_array[i].texture = (Texture2D){ .format = 0, .height = 0, .id = 0, .mipmaps = 0, .width = 0};
        map->_internal->underlying_array[i].active = false;
        map->_internal->underlying_array[i].key = 0;
    }
}

void hashinsert_Texture2D(const HashMap_Texture2D map, const char *const key, const Texture2D texture){
    const unsigned int key_first_hash = key_to_number(key) % HASHMAP_TEX2D_SLOTS;

    // linear probing will resolve possible collisions
    for(int i = 0; i < HASHMAP_TEX2D_SLOTS; ++i){
        int hashed_key = (key_first_hash + i) % HASHMAP_TEX2D_SLOTS;
        if(map._internal->underlying_array[hashed_key].active == false){
            map._internal->underlying_array[hashed_key].texture = texture;
            map._internal->underlying_array[hashed_key].active = true;
            map._internal->underlying_array[hashed_key].key = hashed_key;
            return;
        }
    }
}

struct Texture2D *hashsearch_Texture2D(const HashMap_Texture2D map, const char *const key){
    const unsigned int key_first_hash = key_to_number(key) % HASHMAP_TEX2D_SLOTS;
    for(int i = 0; i < HASHMAP_TEX2D_SLOTS; ++i){
        int hashed_key = (key_first_hash + i) % HASHMAP_TEX2D_SLOTS;
        if(map._internal->underlying_array[hashed_key].active == false){
            continue;
        }
        if(map._internal->underlying_array[hashed_key].key == hashed_key){
            return &map._internal->underlying_array[hashed_key].texture;
        }
    }
    return NULL;
}

void hashclear_Texture2D(const HashMap_Texture2D map){
    for(int i = 0; i < HASHMAP_TEX2D_SLOTS; ++i){
        UnloadTexture(map._internal->underlying_array[i].texture); 
        map._internal->underlying_array[i].active = false;
    }
}

/*************************************
 * Private functions implementations *
 *************************************/

/*
 * As a texture key will usually be a string with between 5 and 20 ASCII characters, we can simply
 * convert it to a numeric value by summing all characters before applying a hash function.
 * Note that permutations of the characters of the same word will generate the same number, but linear
 * probing will resolve collisions.
 */
unsigned int key_to_number(const char *const key){
    char *current_character = key;
    unsigned int number = 0;
    while(current_character && (*current_character) != '\0'){
        number += *current_character;
        ++current_character;
    }
    return number;
}