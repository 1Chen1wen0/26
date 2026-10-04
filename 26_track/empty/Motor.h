#include "stdint.h"
#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_SetPWM(uint8_t n,int16_t Speed);
void motor_stop();
void Motor_Init();
#endif
