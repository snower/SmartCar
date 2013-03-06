#ifndef _BASE_H
#define _BASE_H

#include <hidef.h>
#include "derivative.h"
#include  "cardef.h"

extern void base_Init(void);

extern void base_Pll_init(void);

/*port*/
#define base_Port_set(port,value)  DDR##port=value
#define base_Port_read(port) PORT##port
#define base_Port_write(port,value) PORT##port=value

/*timer*/
typedef char Timer_Channel;
#define TIMER_CHANNEL_MAIN 8
#define TIMER_CHANNEL0 0
#define TIMER_CHANNEL1 1
#define TIMER_CHANNEL2 2
#define TIMER_CHANNEL3 3
#define TIMER_CHANNEL4 4
#define TIMER_CHANNEL5 5
#define TIMER_CHANNEL6 6
#define TIMER_CHANNEL7 7

typedef char Timer_Status;
#define TIMER_OPEN 0
#define TIMER_CLOSE 1

typedef void (* Timer_Function_Bak)(Timer_Channel,int);
extern Timer_Function_Bak base_timer_fbak[9];
extern void base_Timer_init();
extern void base_Timer_set(Timer_Channel tchannel,Timer_Status status,int tcount,Timer_Function_Bak bak);

/*pulse*/
extern void base_Pulse_count_init();
extern void base_Pulse_count_set();
#define base_Pulse_count_write(value) (PACNT=value)
#define base_Pulse_count_read() (PACNT)

/*sci*/
#ifdef MODULE_SCI

typedef struct{
    byte send_finish    :1;
    byte recv_finish    :1;
} Sci_Flag;

typedef char SCI_CHANNEL;
typedef void(*SCI_BACK)(char*,int,int);
#define SCI_SCI0 0
#define SCI_SCI1 8

extern char *base_sci0_send_buffer;
extern int base_sci0_send_buffer_len;
extern char *base_sci0_recv_buffer;
extern int base_sci0_recv_buffer_len;
extern Sci_Flag base_sci0_flag;
extern SCI_BACK base_sci0_send_bak;
extern SCI_BACK base_sci0_recv_bak;

extern void base_Sci_init(SCI_CHANNEL sci,int baudrate);
extern int base_Sci_write(SCI_CHANNEL sci,char* data,int len,SCI_BACK bak);
extern int base_Sci_read(SCI_CHANNEL sci,char* buffer,int len,SCI_BACK bak);

#endif

/*pwm*/
typedef char Pwm_Channel;
#define PWM_CHANNEL0 0
#define PWM_CHANNEL1 1
#define PWM_CHANNEL2 2
#define PWM_CHANNEL3 3

extern void base_Pwm_init(void);
extern void base_Pwm_set(Pwm_Channel pchannel,int pper,int pdty);

#endif