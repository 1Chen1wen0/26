#ifndef STEP_MOTOR_H
#define STEP_MOTOR_H

// 接线
// 第二路
// PA12 PWM
// PA13 DIR
// PA14 DCY
// PA15 SLP
// PA16 RST

#include "ti_msp_dl_config.h"
#include <stdint.h>
void stepper_init(void);
void Stepper_SpeedControl(uint8_t num,int16_t Speed);
void Stepper_start(uint8_t num);
void Stepper_AngleControl(uint8_t angle,uint8_t num);
void Stepper_stop(uint8_t num);
#endif // STEP_MOTOR_H
