#ifndef __K230_H_
#define __K230_H_
#include "Pid.h"
#include <stdint.h>
void Get_VisualError(PID_t* px,PID_t* py);
extern uint8_t k230_flag;

extern uint8_t k230_Serial_RxFlag;
extern uint8_t k230_ByteRecv;
extern char k230_Serial_RxPacket[100];

#endif