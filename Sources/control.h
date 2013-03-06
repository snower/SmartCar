#ifndef _CONTROL_H
#define _CONTROL_H

#include <hidef.h>
#include "derivative.h"
#include  "cardef.h"

#define CONTROL_MAX_SPEED control_max_speed

extern int control_max_speed;
extern int control_speed;
extern int control_direction;

extern float control_bend_factor;
extern float control_frank_add_factor;
extern float control_bend_add_factor;

extern int control_Create();

void control_Create_procedure();
void control_Create_procedure_speed();
void control_Create_procedure_direction();

#endif