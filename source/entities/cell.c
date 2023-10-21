#include "cell.h"
#include "../physics/body.h"
#include "../physics/incline.h"
#include "../graphics/tilemap.h"
#include "stdlib.h"

/************************************
 * Public functions implementations *
 ************************************/

void update_cell(Cell* cell)
{
	Cell_BodyNode* curr_body = cell->bodies_head;
	while (curr_body && curr_body->body->kinematic) {
		resolve_collisions(curr_body->body);
		body_apply_kinematics(curr_body->body);
		curr_body = curr_body->next;
	}

	/* update collision state */
	curr_body = cell->bodies_head;
	while (curr_body) {
		Cell_BodyNode* next = curr_body->next;
		while (next) {
			body_update_collisions(curr_body->body, next->body);

			/* update slopes */
			curr_body->body->collision[INCLINE] = false;
			curr_body->body->slope = 0.f;
			Cell_InclineNode *curr_incline = cell->inclines_head;
			while (curr_incline && curr_body->body->kinematic) {
				incline_update_collisions(curr_body->body, curr_incline->incline);
				curr_incline = curr_incline->next;
			}
			
			next = next->next;
		}
		curr_body = curr_body->next;
	}
}

Cell create_cell(Map *map) {
    Cell cell = (Cell){
      .area = (Rectangle){.x = 0, .y = 0, .width = 600, .height = 800 },
      .bodies_head = malloc(sizeof(Cell_BodyNode)),
      .inclines_head = malloc(sizeof(Cell_InclineNode))
    };
    cell.bodies_head->body = NULL;
    cell.bodies_head->next = NULL;
    cell.bodies_head->prev = NULL;
    cell.inclines_head->incline = NULL;
    cell.inclines_head->next = NULL;
    cell.inclines_head->prev = NULL;

    Collider* current_collider = map->colliders_head;
    Cell_BodyNode* current_cell_node = NULL;
    while (current_collider) {
        current_collider->body.cell = &cell;
        if (current_collider == map->colliders_head) {
            cell.bodies_head->next = NULL;
            cell.bodies_head->prev = NULL;
            cell.bodies_head->body = &current_collider->body;
            current_cell_node = cell.bodies_head;
        }
        else {
            current_cell_node->next = malloc(sizeof(Cell_BodyNode));
            current_cell_node->next->prev = current_cell_node;
            current_cell_node = current_cell_node->next;
            current_cell_node->next = NULL;
            current_cell_node->body = &current_collider->body;
        }
        current_collider = current_collider->next;
    }

    Inclined_Collider* current_incline_collider = map->inclines_head;
    Cell_InclineNode* current_incline_node = NULL;
    while (current_incline_collider) {
        current_incline_collider->incline.cell = &cell;
        if (current_incline_collider == map->inclines_head) {
            cell.inclines_head->incline = &current_incline_collider->incline;
            current_incline_node = cell.inclines_head;
        }
        else {
            Cell_InclineNode* newNode = malloc(sizeof(Cell_InclineNode));
            current_incline_node->next = newNode;
            newNode->prev = current_incline_node;

            current_incline_node = newNode;
            current_incline_node->next = NULL;
            current_incline_node->incline = &current_incline_collider->incline;
        }
        current_incline_collider = current_incline_collider->next;
    }
    return cell;
}

void add_to_cell_head(Cell *cell, Body *to_add) {
    Cell_BodyNode* new_head_node = malloc(sizeof(Cell_BodyNode));
    new_head_node->body = to_add;
    to_add->cell = new_head_node;
    cell->bodies_head->prev = new_head_node;
    new_head_node->next = cell->bodies_head;
    new_head_node->prev = NULL;
    cell->bodies_head = new_head_node;
}