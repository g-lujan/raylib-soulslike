#include "animation.h"

/************************************
 * Public functions implementations *
 ************************************/

/* Called every frame */
void animation_step(Animation* animation)
{
	animation->frames_counter++;
	if (animation->frames_counter < animation->n_frames_to_step) {
		return;
	}

	// rewind to a certain point when the end is reached
	if (animation->curr_col == animation->cols - 1 && animation->curr_line == animation->lines - 1) {
		animation->curr_col = animation->loop_to_col;
		animation->curr_line = animation->loop_to_line;
		animation->frames_counter = 0;
		return;
	}

	// if last col, rewind to first col and go to next line
	if (animation->curr_col == animation->cols - 1) {
		animation->curr_col = 0;
		animation->curr_line = animation->curr_line == animation->lines - 1? 0 : animation->curr_line + 1;
	}
	// else, just go to next col
	else {
		animation->curr_col++;
	}
	animation->frames_counter = 0;
}
