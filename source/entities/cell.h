#pragma once

#include "raylib.h"

/* 
   A Cell is a spacial partition that contains physical entities.
   We will only process physics in the Cell that the player is in.
 */
typedef struct Cell {
	Rectangle area;
	struct Cell_BodyNode* bodies_head;
	struct Cell_InclineNode* inclines_head;
} Cell;

Cell create_cell(struct Map* map);
void update_cell(Cell* cell);
void add_to_cell_head(Cell* cell, struct Body* to_add);