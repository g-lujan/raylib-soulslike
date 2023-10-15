#pragma once

#include "raylib.h"

typedef struct Animation {
    /* Key of the texture in the LEVEL_TEXTURES map */
	char* texture_key;

    Rectangle src_first_frame;
    Rectangle dest;
    int step_x;
    int step_y;
    int lines;
    int cols;
    int curr_line;
    int curr_col;
    int loop_to_col;
    int loop_to_line;

    /* step after n frames elapsed */
    int n_frames_to_step;

    /*
     * frames_counter: Counts the amount of game frames elapsed since last animation frame.
     * If Game FPS is 60 and our anim FPS is 8, than after every 60/8 Game Frames
     * we need to go to the next frame by moving a step.
     */
    int frames_counter;
} Animation;

void animation_step(Animation *animation);