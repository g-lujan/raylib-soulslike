#pragma once

#include "raylib.h"

typedef struct Incline {
	Vector2 start;
	Vector2 end;
	float slope;
	struct Cell* cell;
} Incline;

typedef struct Cell_InclineNode Cell_InclineNode;

typedef struct Cell_InclineNode {
	Incline *incline;
	Cell_InclineNode* next;
	Cell_InclineNode* prev;
} Cell_InclineNode;

typedef struct Inclined_Collider {
	Incline incline;
	struct Inclined_Collider* next;
} Inclined_Collider;

void incline_update_collisions(struct Body* body, Incline* other);
