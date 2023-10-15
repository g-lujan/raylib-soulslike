#include "player.h"
#include "../controls/input.h"
#include "stdio.h"


/* Internal array of actions on player when a key is down */
void (*player_action_key_down[NUM_KB_KEYS]) (Player* player);

/********************************************
 * Forward declaration of private functions *
 ********************************************/

/* Initialize the array of actions on player when a key is down  */
void player_init_action_keys();

/************************************
 * Public functions implementations *
 ************************************/

Player player_create(Vector2 start_pos, unsigned char max_health)
{
	Player player = (Player){.pos = start_pos, .health = max_health, .state = PLAYER_IDLE};
	player.flip = false;

	/* Setup body */
	player.body.boundary = (Rectangle){ player.pos.x, player.pos.y, 49, 98 };
	player.body.speed = (Vector2){ .x = 0.f, .y = 0.f };
	player.body.acceleration = (Vector2){ .x = 0.f, .y = 0.f };
	player.body.collision[TOP] = false;
	player.body.collision[BOTTOM] = false;
	player.body.collision[LEFT] = false;
	player.body.collision[RIGHT] = false;
	player.body.kinematic = true;

	/* Setup animations */
	player.animations[PLAYER_IDLE] = (Animation){
		.texture_key = "player_idle",
		.n_frames_to_step = 15, // step after every 30 frames (half a seconds at 60FPS)
		.src_first_frame = (Rectangle) {.x = 0, .y = 0, .height = 64, .width = 128},
		.dest = (Rectangle){ player.pos.x, player.pos.y, 128 * 2, 64 * 2 },
		.step_x = 128,
		.step_y = 64,
		.lines = 4,
		.cols = 2,
		.curr_col = 0,
		.curr_line = 0,
		.loop_to_col = 0,
		.loop_to_line = 0,
		.frames_counter = 0,
	};
	player.animations[PLAYER_RUN] = (Animation){
		.texture_key = "player_run",
		.n_frames_to_step = 5, // step after every 15 frames (quarter of a seconds at 60FPS)
		.src_first_frame = (Rectangle) {.x = 0, .y = 0, .height = 64, .width = 128},
		.dest = (Rectangle){ player.pos.x, player.pos.y, 128 * 2, 64 * 2 },
		.step_x = 128,
		.step_y = 64,
		.lines = 4,
		.cols = 2,
		.loop_to_col = 0,
		.loop_to_line = 0,
		.curr_col = 0,
		.curr_line = 0,
		.frames_counter = 0,
	};
	player.animations[PLAYER_JUMP] = (Animation){
		.texture_key = "player_jump",
		.n_frames_to_step = 20, // step after every 15 frames (quarter of a seconds at 60FPS)
		.src_first_frame = (Rectangle) {.x = 0, .y = 0, .height = 64, .width = 128},
		.dest = (Rectangle){ player.pos.x, player.pos.y, 128 * 2, 64 * 2 },
		.step_x = 128,
		.step_y = 64,
		.lines = 4,
		.cols = 2,
		.loop_to_col = 0,
		.loop_to_line = 0,
		.curr_col = 0,
		.curr_line = 0,
		.frames_counter = 0,
	};
	player.animations[PLAYER_FALLING] = (Animation){
		.texture_key = "player_jump",
		.n_frames_to_step = 30, // step after every 15 frames (quarter of a seconds at 60FPS)
		.src_first_frame = (Rectangle) {.x = 0, .y = 0, .height = 64, .width = 128},
		.dest = (Rectangle){ player.pos.x, player.pos.y, 128 * 2, 64 * 2 },
		.step_x = 128,
		.step_y = 64,
		.lines = 2,
		.cols = 2,
		.loop_to_line = 0,
		.loop_to_col = 1,
		.curr_line = 0,
		.curr_col = 1,
		.frames_counter = 0,
	};

	player_init_action_keys();

	return player;
}

void update_player(Player *player, Input *input) {
	if (player->body.collision[BOTTOM] || player->body.collision[INCLINE]) {
		player->state = PLAYER_IDLE;
	}
	else if(player->body.speed.y > 0.f) {
		player->state = PLAYER_FALLING;
	}
	
	player->body.speed.x = 0.f;
	for (int i = 0; i < NUM_KB_KEYS; ++i) {
		if (input->key_states[i].down) {
			player_action_key_down[i](player);
		}
	}
}

/*******************************************
 * Available Actions when a key is pressed *
 *******************************************/

void player_run_right(Player* player) {
	player->body.speed.x = 100.f;
	if (player->body.collision[BOTTOM] || player->body.collision[INCLINE]) {
		player->state = PLAYER_RUN;
	}
	player->flip = false;
}

void player_run_left(Player* player) {
	player->body.speed.x = -100.f;
	if (player->body.collision[BOTTOM] || player->body.collision[INCLINE]) {
		player->state = PLAYER_RUN;
	}
	player->flip = true;
}

void player_jump(Player* player) {
	if ((player->body.collision[BOTTOM] || player->body.collision[INCLINE]) && player->state != PLAYER_JUMP) {
		player->body.speed.y = -350.f;
		player->state = PLAYER_JUMP;
	}
}

void player_do_nothing(Player* player) {
}

/************************************
 * Bind actions to keys             *
 ************************************/

void player_init_action_keys()
{
	player_action_key_down[KB_KEY_RIGHT] = player_run_right;
	player_action_key_down[KB_KEY_D] = player_run_right;
	player_action_key_down[KB_KEY_LEFT] = player_run_left;
	player_action_key_down[KB_KEY_A] = player_run_left;
	player_action_key_down[KB_KEY_SPACE] = player_jump;
	
	player_action_key_down[KB_KEY_DOWN] = player_do_nothing;
	player_action_key_down[KB_KEY_S] = player_do_nothing;
	player_action_key_down[KB_KEY_UP] = player_do_nothing;
	player_action_key_down[KB_KEY_W] = player_do_nothing;
}
