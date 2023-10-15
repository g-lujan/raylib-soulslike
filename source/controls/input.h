#pragma once

#include "raylib.h"
#include "stdbool.h"

typedef enum {
    KB_KEY_LEFT,
    KB_KEY_RIGHT,
    KB_KEY_UP,
    KB_KEY_DOWN,
    KB_KEY_A,
    KB_KEY_S,
    KB_KEY_D,
    KB_KEY_W,
    KB_KEY_SPACE,
    NUM_KB_KEYS
} KB_KEY;

typedef struct KeyState {
    bool pressed;
    bool down;
} KeyState;


typedef struct Input {
    KeyState key_states[NUM_KB_KEYS];
} Input;

void init_input(Input* const input);
void update_input(Input *const input);