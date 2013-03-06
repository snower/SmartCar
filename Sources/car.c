#include "base.h"
#include "core.h"
#include "image.h"
#include "control.h"
#include "cmd.h"
#include "car.h"

char car_start=0;

int car_Init(){
    base_Init();
    #ifdef MODULE_COMMAND
    cmd_Init();
    #endif
    return Error(ERROR_SUCCED); 
}

int car_Start(){
    int i=0;
    char h=0x01;
    
    base_Port_set(B,0x00);
    core_Car_speed_start();
    core_Car_direction_write(0);
    
    switch(base_Port_read(B) & 0x0f){
      case 0x01:
          #ifdef MODULE_SCI
          while(1){
              image_Get_roade();
              for(i=0;i<60;i++){
                  if(image_roade[60-i-1]>0 && image_roade[60-i-1]<60) image_image[i*60+image_roade[60-i-1]]=0x00;
              }
              base_Sci_write(SCI_SCI0,&h,1,0);
              for(i=0;i<360;i++){
                  base_Sci_write(SCI_SCI0,image_image+i*10,10,0);
                  sleep(10);
              }
          }
          #endif
      break;
      case 0x02:
          i=20;
          while(1){
              core_Car_speed_write(30);
              core_Car_direction_write(i);
              if(i==20) i=-20;
              else i=20;
              sleep(200);
          }
      break;
      
      
      case 0x03:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.3,1.2,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.02,0.02,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.2,0.2,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.4;
          control_frank_add_factor=0.05;
          control_bend_add_factor=1.5;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x04:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.4,1.5,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.04,0.05,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.45,0.55,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x05:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.5,1.6,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.04,0.05,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.45,0.55,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x06:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.4,1.6,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.04,0.05,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.45,0.55,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x07:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.5,1.5,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.04,0.05,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.45,0.55,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      
      case 0x08:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.6,1.7,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.03,0.04,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.45,0.3,0.3,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x09:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.8,1.9,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.03,0.04,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.4,0.4,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x0a:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.5,1.5,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.04,0.04,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.35,0.45,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x0b:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.4,1.5,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.04,0.05,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.45,0.55,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x0c:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.3,1.2,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.03,0.04,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.55,0.6,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x0d:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.3,1.2,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.05,0.06,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.3,0.2,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.35;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x0e:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.3,1.2,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.02,0.02,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.2,0.2,0.2,0.1);
          control_max_speed=50;
          control_bend_factor=0.3;
          control_frank_add_factor=0.02;
          control_bend_add_factor=1;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
      case 0x0f:
          core_Direction_pid_set(core_car_direction_kp,1.2,1.3,1.2,1.2,0.9);
          core_Direction_pid_set(core_car_direction_ki,0.04,0.02,0.02,0.02,0.01);
          core_Direction_pid_set(core_car_direction_kd,0.55,0.2,0.2,0.2,0.1);
          control_max_speed=60;
          control_bend_factor=0.3;
          control_frank_add_factor=0.03;
          control_bend_add_factor=0.5;
          sleep(500);
          while(car_start){
              core_Detection_speed();
              image_Get_roade();
              control_Create();
              if(!image_roade_bend_is && image_roade_bend_is) core_Car_brake();
          }
      break;
    }
    return Error(ERROR_SUCCED);
}