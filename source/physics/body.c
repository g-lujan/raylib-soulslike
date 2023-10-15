#include "body.h"
#include "stdio.h"
#include <math.h>

/* Positive y points downwards */
const float GRAVITY = 200.f;

/************************************
 * Public functions implementations *
 ************************************/

void body_update_collisions(Body *body, Body *other)
{
	if (!CheckCollisionRecs(body->boundary, other->boundary)) {
		return;
	}

	Rectangle collision_rect = GetCollisionRec(body->boundary, other->boundary);
	if (body->boundary.x - body->prev_position.x > 1.f || other->prev_position.x - other->boundary.x > 1.f) {
		if (collision_rect.height > 10.f && body->boundary.x < other->boundary.x) {
			body->collision[RIGHT] = true;
			other->collision[LEFT] = true;
		}
	}
	if (body->prev_position.x - body->boundary.x > 1.f || other->boundary.x - other->prev_position.x > 1.f) {
		if (collision_rect.height > 10.f && other->boundary.x < body->boundary.x) {
			body->collision[LEFT] = true;
			other->collision[RIGHT] = true;
		}
	}

	if (body->boundary.y - body->prev_position.y > 1.f || other->prev_position.y - other->boundary.y > 1.f) {
		// body should be above other (ie. y_body < y_other)
		if (collision_rect.width > 2.f && body->boundary.y < other->boundary.y) {
			body->collision[BOTTOM] = true;
			other->collision[TOP] = true;
		}
	}
	if (body->prev_position.y - body->boundary.y > 1.f || other->boundary.y - other->prev_position.y > 1.f) {
		// other should be above body (ie. y_other < y_body)
		if (collision_rect.width > 2.f && other->boundary.y < body->boundary.y) {
			body->collision[TOP] = true;
			other->collision[BOTTOM] = true;
		}
	}

};

void resolve_collisions(Body* body)
{
	if (body->collision[RIGHT]) {
		body->boundary.x = body->prev_position.x;
		body->collision[RIGHT] = false;
	}
	if (body->collision[LEFT]) {
		body->boundary.x = body->prev_position.x;
		body->collision[LEFT] = false;
	}
	if (body->collision[TOP]) {
		if (body->speed.y >= 0.f) {
			body->boundary.y = body->prev_position.y;
			body->speed.y = 0.f;
			body->collision[TOP] = false;
		}
	}
	if (body->collision[BOTTOM]){
		body->boundary.y = body->prev_position.y;
		body->collision[BOTTOM] = false;
		body->acceleration.y = 0.f;
	}
	else {
		body->acceleration.y = GRAVITY;
	}
}

void body_apply_kinematics(Body* body) {
	const float dt = GetFrameTime();
	body->prev_position = (Vector2){ .x = body->boundary.x, .y = body->boundary.y };
	
	if (body->collision[INCLINE] && body->speed.y > -1.f) {
		body->boundary.y += dt * body->speed.x * sin(body->slope);
		body->boundary.x += dt * body->speed.x * cos(body->slope);
		body->speed.y = 0;
		return;
	}
	body->boundary.x += dt * body->speed.x;
	body->boundary.y += dt * body->speed.y;
	const float TERMINAL_VELOCITY = 250.f;
	body->speed.y += body->speed.y < TERMINAL_VELOCITY ? 2.5f * body->acceleration.y * dt : 0.f;
}