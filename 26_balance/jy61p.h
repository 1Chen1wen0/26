#ifndef __JY61P_H
#define __JY61P_H
#include "ti_msp_dl_config.h"
#include "stdint.h"
//extern uint8_t RollL, RollH, PitchL, PitchH, YawL, YawH, VL, VH, SUM;
//extern float Pitch,Roll,Yaw;
void Serial_JY61P_Zero_Roll_Pitch(void);
void Serial_JY61P_Zero_Yaw(void);

typedef struct {
    float pitch;
    float roll;
    float yaw;
    float temperature;
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
    int16_t version;
} WIT_Data_t;

extern WIT_Data_t wit_data;

void WIT_Init(void);

#endif
