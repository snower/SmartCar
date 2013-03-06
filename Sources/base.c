#include <hidef.h>
#include "derivative.h"
#include "base.h"

void base_Init(void){
  base_Pll_init();
  base_Timer_init();
  base_Pulse_count_init();
  #ifdef MODULE_SCI
  base_Sci_init(SCI_SCI0,9600);
  #endif
  base_Pwm_init();
}

void base_Pll_init(void){
    CPMUCLKS=0x00;
    CPMUSYNR=0x80 | 0x0e;
    CPMUREFDIV=0xc0 | 0x03;
    CPMUPOSTDIV=0x00;
    CPMUOSC_OSCE=1;
    while(!(CPMUFLG_LOCK==1));
    CPMUCLKS_PLLSEL=1;
    _DISABLE_COP();
    EnableInterrupts;
}

/*port*/

/*timer*/
Timer_Function_Bak base_timer_fbak[9];

void base_Timer_init(){
    TSCR1=0x80;             
    TSCR2=0x06;
    TCTL1=0x00;
    TCTL2=0x00;
    TCTL3=0xa5;
    TCTL4=0xaa;
    TIE=0x00;
    TIOS=0x47;
    OC7M=0x00;
    OC7D=0x00;
}

void base_Timer_set(Timer_Channel tchannel,Timer_Status status,int tcount,Timer_Function_Bak bak){
    int *btc=(int*)0x0050;
    char t_tie=0x01;
    t_tie=t_tie<<tchannel;
    btc+=tchannel;
    *btc=tcount;
    
    switch(status){
      case TIMER_OPEN:
        base_timer_fbak[tchannel]=bak;
        if(tcount>0) TCNT=0x00;
        TIE |=t_tie;
        break;
      case TIMER_CLOSE:
        TIE &=~t_tie;
        base_timer_fbak[tchannel]=0;
        break;
    }
}

/*pulse*/
void base_Pulse_count_init(){
    PACTL=0x40;
    PAFLG=0x00;
}

void base_Pulse_count_set(char it){
    if(it==0) PACTL &=0xfd;
    else if(it==1) PACTL |=0x02;
}

/*sci*/
#ifdef MODULE_SCI

char *base_sci0_send_buffer;
int base_sci0_send_buffer_len=0;
char *base_sci0_recv_buffer;
int base_sci0_recv_buffer_len=0;
Sci_Flag base_sci0_flag;
SCI_BACK base_sci0_send_bak=0;
SCI_BACK base_sci0_recv_bak=0;

void base_Sci_init(SCI_CHANNEL sci,int baudrate){
  int *bsci=(int*)0x00c8;
  baudrate=60000000/16/baudrate;
  bsci+=sci;
  *bsci=baudrate;
  bsci+=1;
  *bsci=0x0000;
  
  base_sci0_flag.send_finish=1;
  base_sci0_flag.recv_finish=1;
}

int base_Sci0_write(char* data,int len,SCI_BACK bak){
   int i=0;
   if(!base_sci0_flag.send_finish || len<=0) return Error(ERROR_SCI_SEND_BUFFER_OVERFLOW);
   
   base_sci0_flag.send_finish=0;
   base_sci0_send_buffer=data;
   base_sci0_send_buffer_len=len;
   
   if(bak==0){
      SCI0CR2 |= 0x88;
      while(!base_sci0_flag.send_finish);
   } else{
      base_sci0_send_bak=bak;
      SCI0CR2= SCI0CR2 | 0x88;
   }
   return Error(ERROR_SUCCED);
}

int base_Sci0_read(char* buffer,int len,SCI_BACK bak){
   int i=0;
   if(!base_sci0_flag.recv_finish || len<=0) return Error(ERROR_SCI_RECV_BUFFER_OVERFLOW);
   
   base_sci0_flag.recv_finish=0;
   base_sci0_recv_buffer=buffer;
   base_sci0_recv_buffer_len=len;
   if(bak==0){
      SCI0CR2=SCI0CR2 | 0x24;
      while(!base_sci0_flag.recv_finish);
   } else{
      base_sci0_recv_bak=bak;
      SCI0CR2=SCI0CR2 | 0x24;
   }
   return Error(ERROR_SUCCED);
}

int base_Sci_write(SCI_CHANNEL sci,char* data,int len,SCI_BACK bak){
    switch(sci){
      case SCI_SCI0:
          return base_Sci0_write(data,len,bak);
    }
}

int base_Sci_read(SCI_CHANNEL sci,char* buffer,int len,SCI_BACK bak){
     switch(sci){
      case SCI_SCI0:
          return base_Sci0_read(buffer,len,bak);
    }
}


#endif

/*pwm*/
void base_Pwm_init(void){
  PWME=0x00;
  PWMCLK=0xff;
  PWMPRCLK=0x21;
  PWMSCLA=0x04;
  PWMSCLB=0x4b;
  PWMPOL=0x00;
  PWMCAE=0x00;
  PWMCTL=0xf0;
}

void base_Pwm_set(Pwm_Channel pchannel,int pper,int pdty){
    int *bpwm=(int*)0x00ac;
    uchar p_start=0x01;
    p_start = p_start<<(pchannel*2+1);
    bpwm+=pchannel;
    *bpwm=0x0000;
    bpwm+=4;
    *bpwm=pper;
    bpwm+=4;
    *bpwm=pdty;
    if(pper!=0) PWME |=p_start;
    else PWME &=~p_start;
}