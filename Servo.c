#include "ti_msp_dl_config.h"


void Servo_Init()
{
	DL_Timer_startCounter(Servo_INST);
}

void Servo_SetAngle(uint8_t Num,uint16_t Angle)
{
	if(Angle>270)
	{Angle = 270;}
	else if(Angle<0)
	{Angle = 0;}
	 
	uint16_t Temp = Angle*200/270;
	
	if(Num == 1)
	{
		DL_TimerG_setCaptureCompareValue(Servo_INST,50+Temp,GPIO_Servo_C0_IDX);
	}
	else if(Num == 2)
	{
		DL_TimerG_setCaptureCompareValue(Servo_INST,50+Temp,GPIO_Servo_C1_IDX);
	}
	
}	




