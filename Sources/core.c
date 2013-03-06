#include <hidef.h>
#include "derivative.h"
#include "base.h"
#include "core.h"
#include "cmd.h"
#include "car.h"

/*sys*/
ulong core_time_current=0;
void core_sleep_time(Timer_Channel channel,int time){
     core_time_current+=10;
}

void core_sleep(int ms){
    /*core_time_current=0;
    base_Timer_set(TIMER_CHANNEL6,TIMER_OPEN,9375,core_sleep_time);
    while(core_time_current<ms);
    base_Timer_set(TIMER_CHANNEL6,TIMER_CLOSE,0,0);*/
    int i=0,j=0;
    for(i=0;i<ms;i++){
        for(j=0;j<20000;j++){
            asm nop;
        }
    }
}


void core_Console_write(char *str){
    int len=0;
    while(str[len++]!='\0' && len<15);
    cmd_Output(str,len);
}

void core_Console_read(char *str){
    char ct[15];
    int i=0;
    cmd_Input(ct,15);
    for(i=0;i<15;i++){
      if(ct!=0) *(str+i)=ct[i];
      else break;
    }
}

int core_Itoa(int i,char *a){
    int index=0;
    if(i<0){
      a[index]='-';
      index++;
      i=abs(i);
    }
    if(i>=10000){
        a[index]=i/10000%10+'0';
        index++;
    }
    if(i>=1000){
        a[index]=i/1000%10+'0';
        index++;
    }
    if(i>=100){
        a[index]=i/100%10+'0';
        index++;
    }
    if(i>=10){
        a[index]=i/10%10+'0';
        index++;
    }
    if(i>=0){
        a[index]=i%10+'0';
        index++;
    }
    a[index]='\0';
    index++;
    return index;
}

void core_Outi(char *head,int len,int i){
    #ifdef MODULE_SCI
    char istr[7];
    int il=0;
    il=core_Itoa(i,istr);
    base_Sci_write(SCI_SCI0,head,len,0);
    base_Sci_write(SCI_SCI0,istr,il,0);
    #endif
}

/*car speed*/
int core_car_speed_setting=0;
int core_car_speed_current=0;
int core_Car_speed_start(){
    base_Pwm_set(PWM_CHANNEL0,1500,1500);
    base_Pwm_set(PWM_CHANNEL2,1500,1500);
}

int core_Car_speed_set(int speed){
    speed=speed>100 ? 100 : speed<-100 ? -100 : speed;
    if(speed>=0){
        base_Pwm_set(PWM_CHANNEL0,500,500-5*speed);
        base_Pwm_set(PWM_CHANNEL2,500,500);
     } else{
        base_Pwm_set(PWM_CHANNEL0,500,500);
        base_Pwm_set(PWM_CHANNEL2,500,500-5*abs(speed));
    }
}

int core_Car_speed_close(){
    base_Pwm_set(PWM_CHANNEL0,0,0);
    base_Pwm_set(PWM_CHANNEL2,0,0);
}

//car speed pid
float core_car_speed_kp=1.65;
float core_car_speed_ki=0.025;
float core_car_speed_kd=0.65;

int core_car_speed_e=0;
int core_car_speed_e1=0;
int core_car_speed_e2=0;
int core_car_speed_u1=0;

int core_Car_speed_control_pid(int speed){
    core_car_speed_e2=core_car_speed_e1;
    core_car_speed_e1=core_car_speed_e;
    core_car_speed_e=speed-core_car_speed_current;
    core_car_speed_u1=core_Pid_type_add(core_car_speed_e,core_car_speed_e1,core_car_speed_e2,core_car_speed_kp,core_car_speed_ki,core_car_speed_kd,core_car_speed_u1);
    return core_car_speed_u1;
}

/*car direction*/
int core_car_direction_setting=0;
int core_car_direction_current=0;

int core_Car_direction_set(int size){
  size= size>30 ? 30 : size<-30 ? -30 : size;
  if(size>=0){
      base_Pwm_set(PWM_CHANNEL3,500,500-152-100*size/90);
  } else{
      base_Pwm_set(PWM_CHANNEL3,500,500-152+100*abs(size)/90);
  }
}

//car direction pid
volatile float core_car_direction_kp[5]={1.2,1.5,1.4,0.9,0.9};
volatile float core_car_direction_ki[5]={0.04,0.04,0.05,0.01,0.01};
volatile float core_car_direction_kd[5]={0.55,0.45,0.55,0.1,0.1};

int core_car_direction_e=0;
int core_car_direction_e1=0;
int core_car_direction_e2=0;
int core_car_direction_u1=0;

int core_Car_direction_control_pid(int direction){
    int i=direction>=0 ? direction/10 : (0-direction)/10;
    core_car_direction_e2=core_car_direction_e1;
    core_car_direction_e1=core_car_direction_e;
    core_car_direction_e=direction;
    core_car_direction_u1=core_Pid_type_add(core_car_direction_e,core_car_direction_e1,core_car_direction_e2,core_car_direction_kp[i],core_car_direction_ki[i],core_car_direction_kd[i],core_car_direction_u1);
    return core_car_direction_u1;
}

/* car detection speed*/
int core_detection_speed_current=0;
int core_Detection_speed(){
    core_detection_speed_current=base_Pulse_count_read();
    core_car_speed_current=((float)core_detection_speed_current)/25;
    base_Pulse_count_write(0);
}

/*brake*/
void core_Car_brake(){
    sleep(1000);
    core_Car_speed_write(0);
    sleep(2000);
    car_start=0;
}