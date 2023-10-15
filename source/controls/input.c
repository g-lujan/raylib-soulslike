#include "raylib.h"
#include "input.h"
#include "stdbool.h"
#include <stdlib.h>

/********************************************
 * Forward declaration of private functions *
 ********************************************/

KeyboardKey convert_kb_key_to_raylib(KB_KEY key);


/************************************
 * Public functions implementations *
 ************************************/

void init_input(Input* const input)
{
    for (int i = 0; i < NUM_KB_KEYS; ++i) {
        input->key_states[i] = (KeyState){ .down = false, .pressed = false };
    }
}

void update_input(Input *const input){
    for (int i = 0; i < NUM_KB_KEYS; ++i) {
        input->key_states[i].down = IsKeyDown(convert_kb_key_to_raylib(i));
        input->key_states[i].pressed = IsKeyPressed(convert_kb_key_to_raylib(i));
    }
}


/************************************
 * Private functions implementations *
 ************************************/

KeyboardKey convert_kb_key_to_raylib(KB_KEY key) {
    switch (key) {
    case KB_KEY_A:
        return KEY_A;
    case KB_KEY_D:
        return KEY_D;
    case KB_KEY_S:
        return KEY_S;
    case KB_KEY_W:
        return KEY_W;
    case KB_KEY_LEFT:
        return KEY_LEFT;
    case KB_KEY_RIGHT:
        return KEY_RIGHT;
    case KB_KEY_DOWN:
        return KEY_DOWN;
    case KB_KEY_UP:
        return KEY_UP;
    case KB_KEY_SPACE:
        return KEY_SPACE;
    default:
        return KEY_NULL;
    }
}
