#include "base.h"
#include "core.h"
#include "cmd.h"
#include "image.h"
#include "car.h"

#ifdef MODULE_COMMAND

char cmd_buffer[20];
char* cmd_input_addr;
int cmd_input_len;
char cmd_input_recv;
     
int cmd_Init(){
    base_Sci_read(SCI_SCI0,cmd_buffer,20,cmd_Back_function);
    return Error(ERROR_SUCCED);
}

int cmd_Back_function(char *data,int len,int error){
    switch(data[0]){
      case CMD_INPUT:
          memcpy(cmd_input_addr,data+3,len-3);
          break;
      case CMD_COMMAND:
          cmd_Command(data+3,len-3);
          break;
      case CMD_DATA:
          cmd_Data(data+3,len-3);
          break;
    }
    base_Sci_read(SCI_SCI0,cmd_buffer,20,cmd_Back_function);
    return Error(ERROR_SUCCED);
}

int cmd_Input(char *data,int len){
    cmd_input_addr=data;
    cmd_input_len=len;
    cmd_input_recv=0;
    while(!cmd_input_recv);
    cmd_input_addr=0;
    cmd_input_len=0;
    cmd_input_recv=0;
    return Error(ERROR_SUCCED);
}

int cmd_Output(char *data,int len){
    char type[2];
    char end=0;
    type[0]=CMD_OUTPUT;
    type[1]=len;
    base_Sci_write(SCI_SCI0,type,2,0);
    base_Sci_write(SCI_SCI0,data,len,0);
    base_Sci_write(SCI_SCI0,&end,1,0);
    return Error(ERROR_SUCCED);  
}

int cmd_Command(char *data,int len){
    Image* image;
    switch(data[0]){
      case CMD_COMMAND_SPEED_SET:
        core_Car_speed_write(data[1]);
        break;
      case CMD_COMMAND_DIRECTION_SET:
        core_Car_direction_set(data[1]);
        break;
    }
    return Error(ERROR_SUCCED);
}

int cmd_Command_send(char cmd,char *args,int len){
    char type[3];
    char end=0;
    type[0]=CMD_COMMAND;
    type[1]=len+1;
    type[2]=cmd;
    base_Sci_write(SCI_SCI0,type,3,0);
    base_Sci_write(SCI_SCI0,args,len,0);
    base_Sci_write(SCI_SCI0,&end,1,0);
    return Error(ERROR_SUCCED);
}

int cmd_Data(char *data,int len){
    return Error(ERROR_SUCCED);
}

int cmd_Data_send(char *data,int len){
    char type[2];
    char end=0;
    type[0]=CMD_DATA;
    type[1]=len;
    base_Sci_write(SCI_SCI0,type,2,0);
    base_Sci_write(SCI_SCI0,data,len,0);
    base_Sci_write(SCI_SCI0,&end,1,0);
    return Error(ERROR_SUCCED);
}

#endif