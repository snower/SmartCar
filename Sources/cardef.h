#ifndef _CARDEF_H
#define _CARDEF_H

#include <stdlib.h>

#define DEBUG
//#define ERROR
#define MODULE_SCI
//#define MODULE_COMMAND

#define sleep(ms) core_sleep(ms)

#ifdef ERROR
#define Error(code) base_Port_write(A,code)
#else
#define Error(code) code
#endif

#define ERROR_SUCCED 0
#define ERROR_UNKNOWN_ERROR 1

#define ERROR_SCI_SEND_BUFFER_OVERFLOW 20
#define ERROR_SCI_RECV_BUFFER_OVERFLOW 21
#define ERROR_SCI_SEND_TIMEOUT 22
#define ERROR_SCI_RECV_TIMEOUT 23

#endif