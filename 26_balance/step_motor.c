#include "step_motor.h" 
#include "ti_msp_dl_config.h"
#include "Delay.h"
#include "oled.h"
#include <stdio.h>
#include "uart.h"
#include "step_motor.h"
#include "Pid.h"
#include "beep.h"
#include "track.h"
#include "Key.h"
#include "jy61p.h"
#include "Key.h"
#include "Motor.h"
#include "oled.h"
#include "step_motor.h"

void stepper_init(void)
{
   
    DL_GPIO_setPins(Step_ENA_PORT, Step_ENA_PIN);
    DL_GPIO_setPins(Step_DIR_PORT, Step_DIR_PIN);
}

void Stepper_start(void)
{
	DL_GPIO_setPins(Step_ENA_PORT, Step_ENA_PIN);
    NVIC_EnableIRQ(Stepper_INST_INT_IRQN);    
    DL_Timer_startCounter(Stepper_INST);  
}
//角速度控制,度/s
//float period_sec = 1.0f / frequency;
// 1 / Step_A_INST_CLK_FREQ
// period_sec = (1/Step_A_INST_CLK_FREQ)
void Stepper_SpeedControl(int16_t Speed)//选择电机：1为下2为上。选择方向：下电机1为左0为右，上电机1为下0为上。速度控制
{
    uint32_t frequency = (uint32_t)(abs(Speed)/0.05625);
    frequency = frequency > 0 ? frequency : 1;
    uint32_t period  = Stepper_INST_CLK_FREQ/frequency;
    period = period <65536 ? period :65535;
    period = period > 800 ? period : 800;
    

        if(Speed>0)
        {
            DL_GPIO_setPins(Step_DIR_PORT, Step_DIR_PIN);
            DL_Timer_setLoadValue(Stepper_INST,period);
            DL_Timer_setCaptureCompareValue(Stepper_INST,period/2,GPIO_Stepper_C0_IDX);
        }
        else if(Speed<=0)
        {
            DL_GPIO_clearPins(Step_DIR_PORT, Step_DIR_PIN);
            DL_Timer_setLoadValue(Stepper_INST,period);
            DL_Timer_setCaptureCompareValue(Stepper_INST,period/2,GPIO_Stepper_C0_IDX);
        }
           
}

void Stepper_Stop(void)
{
	DL_GPIO_clearPins(Step_ENA_PORT, Step_ENA_PIN);
}


