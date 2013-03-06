#include "base.h"
#include "core.h"
#include "image.h"
#include "control.h"

int control_max_speed=0;
int control_speed=0;
int control_direction=0;

float control_bend_factor=0.3;
float control_frank_add_factor=0.011;
float control_bend_add_factor=1;

volatile float control_speed_encode[31]={
    1,1,1,1,1,
    0.96,0.96,0.96,0.96,0.96,
    0.96,0.95,0.94,0.90,0.85,
    0.91,0.80,0.76,0.73,0.7,
    0.92,0.7,0.66,0.63,0.6,
    0.6,0.6,0.6,0.56,0.53,
    0.5
};

int control_Create(){
    control_Create_procedure();
    return Error(ERROR_SUCCED);
}

void control_Create_procedure(){
     control_Create_procedure_direction();
     control_Create_procedure_speed();
}

void control_Create_procedure_speed(){
    static float olds=0,si=0.6;
    int i=0,d=0;
    int speed=0;
    
    
    for(i=0;i<30;i++){
        d+=image_roade[30+i];
    }
    d=(d/30-29+control_direction)/2;
    if(d<0) d=-d;
    d=d>30 ? 30 : d;
    
    if(image_roade_over_is){
        speed=CONTROL_MAX_SPEED*0.5;
    }else{
        if(image_roade_bend_is){
            if(olds<20) olds+=control_bend_add_factor;
            speed=CONTROL_MAX_SPEED*control_speed_encode[d]*control_bend_factor+olds;
            si=0.7;
        }else{
            speed=CONTROL_MAX_SPEED*control_speed_encode[d]*si;
            olds=0;
            if(si<=1) si+=control_frank_add_factor;
        }
    }
    
    control_speed=core_Car_speed_control_pid(speed);
    core_Car_speed_write(control_speed);
}

void control_Create_procedure_direction(){
    static float oldk=0;
    int i=0;
    float k=0;
    if(image_roade_over_is){
        k=oldk;
    }else{
        for(i=0;i<45;i++){
            k+=image_roade[i];
        }
        k=k/45-29;
        oldk=k;
    }
    k=k>45 ? 45 : k<-45 ? -45 : k;
    control_direction=core_Car_direction_control_pid(k);
    core_Car_direction_write(control_direction);
}
