#include <hidef.h>
#include "derivative.h"
#include "base.h"

#pragma CODE_SEG __NEAR_SEG NON_BANKED

/*timer*/
interrupt VectorNumber_Vtimovf void interrupt_Timer(void){
    TFLG2=0x80;
    if(base_timer_fbak[8]!=0){
        (base_timer_fbak[8])(TIMER_CHANNEL0,TCNT);
    }
}

interrupt VectorNumber_Vtimch0 void interrupt_Timer0(void){
    if(base_timer_fbak[0]!=0){
        (base_timer_fbak[0])(TIMER_CHANNEL0,TC0);
    }
    TFLG1_C0F=1;
    TCNT=0x00;
}

interrupt VectorNumber_Vtimch1 void interrupt_Timer1(void){
    TFLG1_C1F=1;
    if(base_timer_fbak[1]!=0){
        (base_timer_fbak[1])(TIMER_CHANNEL1,TC1);
    }
    TCNT=0x00;
}

interrupt VectorNumber_Vtimch2 void interrupt_Timer2(void){
    if(base_timer_fbak[2]!=0){
        (base_timer_fbak[2])(TIMER_CHANNEL2,TC2);
    }
    TFLG1_C2F=1;
    TCNT=0x00;
}

interrupt VectorNumber_Vtimch3 void interrupt_Timer3(void){
    if(base_timer_fbak[3]!=0){
        (base_timer_fbak[3])(TIMER_CHANNEL3,TC3);
    }
    TFLG1_C3F=1;
    TCNT=0x00;
}

interrupt VectorNumber_Vtimch4 void interrupt_Timer4(void){
    if(base_timer_fbak[4]!=0){
        (base_timer_fbak[4])(TIMER_CHANNEL4,TC4);
    }
    TFLG1_C4F=1;
    TCNT=0x00;
}

interrupt VectorNumber_Vtimch5 void interrupt_Timer5(void){
    if(base_timer_fbak[5]!=0){
        (base_timer_fbak[5])(TIMER_CHANNEL5,TC5);
    }
    TFLG1_C5F=1;
    TCNT=0x00;
}

interrupt VectorNumber_Vtimch6 void interrupt_Timer6(void){
    if(base_timer_fbak[6]!=0){
        (base_timer_fbak[6])(TIMER_CHANNEL6,TC6);
    }
    TFLG1_C6F=1;
    TCNT=0x00;
}

interrupt VectorNumber_Vtimch7 void interrupt_Timer7(void){
    if(base_timer_fbak[7]!=0){
        (base_timer_fbak[7])(TIMER_CHANNEL7,TC7);
    }
    TFLG1_C7F=1;
    TCNT=0x00;
}

/*sci*/
#ifdef MODULE_SCI

interrupt VectorNumber_Vsci0 void interrupt_SCI0(void){
   static int recv_index=0,send_index=0;
   
   if(SCI0SR1_RDRF){
      base_sci0_recv_buffer[recv_index]=SCI0DRL;
      if(recv_index!=0 || base_sci0_recv_buffer[recv_index]==0) recv_index++;
      if(recv_index==base_sci0_recv_buffer_len || recv_index==base_sci0_recv_buffer[2]){
         while(!SCI0SR1_IDLE);
         recv_index=0;
         base_sci0_flag.recv_finish=1;
         SCI0CR2 &= 0xdb;
         if(base_sci0_recv_bak!=0){
             base_sci0_recv_bak(base_sci0_recv_buffer,recv_index,0);
         }
      }
   }
   
   if(SCI0SR1_TDRE){
         SCI0DRH=0x0;
         SCI0DRL=base_sci0_send_buffer[send_index];
         send_index++;
         if(send_index==base_sci0_send_buffer_len){
             while(!SCI0SR1_TC);
             send_index=0;
             base_sci0_flag.send_finish=1;
             SCI0CR2 &= 0x77;
             if(base_sci0_send_bak!=0){
                base_sci0_send_bak(base_sci0_send_buffer,15,0);
             }
         }
   }
}

#endif