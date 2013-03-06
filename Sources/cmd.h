#ifndef _CMD_H
#define _CMD_H

#include <hidef.h>
#include "derivative.h"
#include  "cardef.h"

#ifdef MODULE_COMMAND

extern int cmd_Init();
extern int cmd_Back_function(char*,int,int);
extern int cmd_Input(char*,int);
extern int cmd_Output(char*,int);
extern int cmd_Command(char*,int);
extern int cmd_Command_send(char cmd,char *args,int len);
extern int cmd_Data(char*,int);
extern int cmd_Data_send(char *data,int len);

#define CMD_INPUT 1
#define CMD_OUTPUT 10
#define CMD_COMMAND 20
#define CMD_DATA 50

#define CMD_COMMAND_SPEED_SET 1
#define CMD_COMMAND_DIRECTION_SET 2

#endif

#endif