#include "cell.h"
#include "../physics/body.h"
#include "../physics/incline.h"

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