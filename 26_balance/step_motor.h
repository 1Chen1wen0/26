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
void Stepper_start(void);
void Stepper_SpeedControl(int16_t Speed);
void Stepper_Stop(void);

#endif // STEP_MOTOR_H
