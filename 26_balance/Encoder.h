#ifndef ENCODER_H__
#define ENCODER_H__
#include <stdint.h>
extern int32_t Get_Encoder_countA,Get_Encoder_countB,encoderA_cnt,encoderB_cnt;
void encoder__init(void);
int32_t Motor_GetSpeed(uint8_t num,int32_t* Get_Encoder_countA,int32_t* Get_Encoder_countB);

#endif