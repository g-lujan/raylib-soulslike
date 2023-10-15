#include "incline.h"
#include "body.h"

/************************************
 * Public functions implementations *
 ************************************/

void incline_update_collisions(Body* body, Incline* incline)
{
	Vector2 body_botton_right = (Vector2){ .x = body->boundary.x + body->boundary.width, .y = body->boundary.y + body->boundary.height };

	if (CheckCollisionPointLine(body_botton_right, incline->start, incline->end, 5) && incline->slope < 0.f) {
		body->collision[INCLINE] = true;
		body->slope = incline->slope;
	}
	
	Vector2 body_botton_left = (Vector2){ .x = body->boundary.x, .y = body->boundary.y + body->boundary.height };
	if (CheckCollisionPointLine(body_botton_left, incline->start, incline->end, 5) && incline->slope > 0.f) {
		body->collision[INCLINE] = true;
		body->slope = incline->slope;
	}

}
