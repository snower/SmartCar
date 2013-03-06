#ifndef _CORE_H
#define _CORE_H

#include <hidef.h>
#include "derivative.h"
#include  "cardef.h"

/*sys*/
extern void core_Console_write(char *str);
extern void core_Console_read(char *str);
extern int core_Itoa(int i,char *a);
extern void core_Outi(char *head,int len,int i);

/*car speed*/
extern int core_car_speed_setting;
extern int core_car_speed_current;

extern int core_Car_speed_start();
extern int core_Car_speed_set(int speed);
extern int core_Car_speed_close();

#define core_Car_speed_write(speed) core_car_speed_setting=speed,core_Car_speed_set(speed)
#define core_Car_speed_read() core_car_speed_current

//car speed pid
int core_Car_speed_control_pid(int speed);

/*car direction*/
extern int core_car_direction_setting;
extern int core_car_direction_current;

extern int core_Car_direction_set(int size);
#define core_Car_direction_write(direction) core_car_direction_setting=direction,core_Car_direction_set(direction),core_car_direction_current=direction
#define core_Car_direction_read() core_car_direction_current

//car direction pid
extern float core_car_direction_kp[5];
extern float core_car_direction_ki[5];
extern float core_car_direction_kd[5];
#define core_Direction_pid_set(pid,pid1,pid2,pid3,pid4,pid5) pid[0]=pid1,pid[1]=pid2,pid[2]=pid3,pid[3]=pid4,pid[4]=pid5 
int core_Car_direction_control_pid(int direction);


/*detection*/

extern int core_detection_speed_current;
extern int core_Detection_speed();

/*brake*/
extern void core_Car_brake();

/*pid*/
#define core_Pid_type_add(e,e1,e2,kp,ki,kd,u1) u1+kp*(e-e1)+ki*e+kd*(e-2*e1+e2)

#endif