#pragma once

#include "raylib.h"

typedef enum {	TOP, BOTTOM, RIGHT, LEFT, INCLINE, NUM_SIDES } SIDE;

typedef struct Body {
	Rectangle boundary;
	Vector2 prev_position; // to resolve collisions
	Vector2 speed;
	Vector2 acceleration;
	bool collision[NUM_SIDES];
	bool kinematic;
	struct Cell* cell;
	float slope;
} Body;

typedef struct Cell_BodyNode Cell_BodyNode;

typedef struct Cell_BodyNode {
	Body* body;
	Cell_BodyNode* next;
	Cell_BodyNode* prev;
} Cell_BodyNode;

typedef struct Collider {
	Body body;
	struct Collider* next;
} Collider;

void body_update_collisions(Body* body, Body* other);
void resolve_collisions(Body* body);
void body_apply_kinematics(Body* body);