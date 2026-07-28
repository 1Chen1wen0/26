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
/*
void stepper_init(void)
{
   // DL_GPIO_setPins(GPIOB, Step_Motor_DIR_1_PIN);
    DL_GPIO_setPins(GPIOA, Step_Motor_DCY_1_PIN);
    DL_GPIO_setPins(GPIOB, Step_Motor_SLP_1_PIN);
    DL_GPIO_setPins(GPIOA, Step_Motor_RST_1_PIN);
   // DL_GPIO_setPins(GPIOB, Step_Motor_DIR_2_PIN);
    DL_GPIO_setPins(GPIOB, Step_Motor_RST_2_PIN);
    DL_GPIO_setPins(GPIOA, Step_Motor_DCY_2_PIN);
    DL_GPIO_setPins(GPIOA, Step_Motor_SLP_2_PIN);
    NVIC_EnableIRQ(Step_A_INST_INT_IRQN);//走固定角度解除
    NVIC_EnableIRQ(Step_B_INST_INT_IRQN);
}

void Stepper_start(uint8_t num)
{
    NVIC_EnableIRQ(Step_B_INST_INT_IRQN);
    NVIC_EnableIRQ(Step_A_INST_INT_IRQN);
    if(num==1)
    {
        DL_Timer_startCounter(Step_A_INST);
        
    }
    else if(num ==2)
    {
        DL_Timer_startCounter(Step_B_INST);
    }
    else if(num ==3)
    {
        DL_Timer_startCounter(Step_A_INST);
        DL_Timer_startCounter(Step_B_INST);
    }
    
}
//角速度控制,度/s
//float period_sec = 1.0f / frequency;
// 1 / Step_A_INST_CLK_FREQ
// period_sec = (1/Step_A_INST_CLK_FREQ)
void Stepper_SpeedControl(uint8_t num,int16_t Speed)//选择电机：1为下2为上。选择方向：下电机1为左0为右，上电机1为下0为上。速度控制
{
    uint32_t frequency = (uint32_t)(abs(Speed)/0.05625);
    frequency = frequency > 0 ? frequency : 1;
    uint32_t period  = Step_A_INST_CLK_FREQ/frequency;
    period = period <65536 ? period :65535;
    period = period > 800 ? period : 800;
    
    if(num ==1)
    {
        if(Speed>0)
        {
            DL_GPIO_setPins(GPIOB, Step_Motor_DIR_1_PIN);
            DL_Timer_setLoadValue(Step_A_INST,period);
            DL_Timer_setCaptureCompareValue(Step_A_INST,period/2,GPIO_Step_A_C0_IDX);
        }
        else if(Speed<=0)
        {
            DL_GPIO_clearPins(GPIOB, Step_Motor_DIR_1_PIN);
            DL_Timer_setLoadValue(Step_A_INST,period);
            DL_Timer_setCaptureCompareValue(Step_A_INST,period/2,GPIO_Step_A_C0_IDX);
        }
        
    }
    else if(num ==2)
    {
        if(Speed>0)
        {
            DL_GPIO_setPins(GPIOB, Step_Motor_DIR_2_PIN);
            DL_Timer_setLoadValue(Step_B_INST,period);
            DL_Timer_setCaptureCompareValue(Step_B_INST,period/2,GPIO_Step_B_C0_IDX);
        }
        else if(Speed<=0)
        {
            DL_GPIO_clearPins(GPIOB, Step_Motor_DIR_2_PIN);
            DL_Timer_setLoadValue(Step_B_INST,period);
            DL_Timer_setCaptureCompareValue(Step_B_INST,period/2,GPIO_Step_B_C0_IDX);
        }

    }
    
}
//以固定速度走固定角度
 uint32_t stepper1_rest = 0;
 uint32_t stepper2_rest = 0;
void Stepper_AngleControl(uint8_t angle,uint8_t num)
{
    if(num ==1)
    {
        stepper1_rest = (uint32_t)(angle/0.05625);
        Stepper_start(1);
    }
    else if(num==2)
    {
        stepper2_rest = (uint32_t)(angle/0.05625);
        Stepper_start(2);
    }
    
}
void Stepper_stop(uint8_t num)
{
    if(num==1)
    {DL_Timer_stopCounter(Step_A_INST);}
    else if(num==2)
    {DL_Timer_stopCounter(Step_B_INST);}
}

void Step_A_INST_IRQHandler()
{
    switch (DL_Timer_getPendingInterrupt(Step_A_INST))
    {
        case DL_TIMER_IIDX_ZERO:
        {
            if(stepper1_rest>0)
            {
                stepper1_rest -- ;
                break;
            }
            else if(stepper1_rest==0)
            {
                Stepper_stop(1);
                break;
            }
        }
    default:
        break;
    }
}
void Step_B_INST_IRQHandler()
{
    switch (DL_Timer_getPendingInterrupt(Step_B_INST))
    {
        case DL_TIMER_IIDX_LOAD:
        {
            if(stepper2_rest>0)
            {
                stepper2_rest -- ;
                break;
            }
            else if(stepper2_rest==0)
            {
                Stepper_stop(2);
                break;
            }
        }
    default:
        break;
    }
}
*/
