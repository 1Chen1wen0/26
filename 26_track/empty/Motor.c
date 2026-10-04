#include <stdio.h>
#include "uart.h"
#include "Pid.h"
#include "beep.h"
#include "track.h"
#include "Key.h"

void Motor_Init(void)
{
		DL_Timer_startCounter(Motor_Control_INST);
		DL_GPIO_setPins(GPIOA,DL_GPIO_PIN_17);
		DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_19);
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_18);
		DL_GPIO_clearPins(GPIOA,DL_GPIO_PIN_16);	
		DL_Timer_setCaptureCompareValue(Motor_Control_INST,0,GPIO_Motor_Control_C1_IDX);
		DL_Timer_setCaptureCompareValue(Motor_Control_INST,0,GPIO_Motor_Control_C0_IDX);
}


void Motor_SetPWM(uint8_t n,int16_t Speed){
if(Speed>=1000)
{Speed = 1000;}
if(Speed<=-1000)
{Speed = -1000;}
//右轮控制
if(n==1){
if (Speed >= 0)
{
		DL_GPIO_setPins(GPIOA,DL_GPIO_PIN_17);
		DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_19);

	
		DL_TimerG_setCaptureCompareValue(Motor_Control_INST,Speed,GPIO_Motor_Control_C1_IDX);
}
else if(Speed<0)
{
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_19);
		DL_GPIO_clearPins(GPIOA,DL_GPIO_PIN_17);	
		DL_TimerG_setCaptureCompareValue(Motor_Control_INST,-Speed,GPIO_Motor_Control_C1_IDX);
}
}
//左轮控制
else if(n==2){
if (Speed >= 0)
{
		DL_GPIO_setPins(GPIOA,DL_GPIO_PIN_16);
		DL_GPIO_clearPins(GPIOB,DL_GPIO_PIN_18);
		DL_TimerG_setCaptureCompareValue(Motor_Control_INST,Speed,GPIO_Motor_Control_C0_IDX);
}
else if(Speed <0)
{
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_18);
		DL_GPIO_clearPins(GPIOA,DL_GPIO_PIN_16);	

		DL_TimerG_setCaptureCompareValue(Motor_Control_INST,-Speed,GPIO_Motor_Control_C0_IDX);
   }
  }
}

void motor_stop()
{
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_19);
		DL_GPIO_setPins(GPIOA,DL_GPIO_PIN_17);	
		DL_TimerG_setCaptureCompareValue(Motor_Control_INST,0,GPIO_Motor_Control_C1_IDX);

  		DL_GPIO_setPins(GPIOA,DL_GPIO_PIN_16);
		DL_GPIO_setPins(GPIOB,DL_GPIO_PIN_18);	
		DL_TimerG_setCaptureCompareValue(Motor_Control_INST,0,GPIO_Motor_Control_C0_IDX);
}

