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
#include "Encoder.h"
#include <stdint.h>
uint32_t gpio_interrup;
int32_t Get_Encoder_countA,Get_Encoder_countB,encoderA_cnt,encoderB_cnt;
void encoder__init(void)
{
	//编码器引脚外部中断
	NVIC_ClearPendingIRQ(GPIOB_INT_IRQn);
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
    NVIC_ClearPendingIRQ(GPIOA_INT_IRQn);
	NVIC_EnableIRQ(GPIOA_INT_IRQn);
}

int32_t Motor_GetSpeed(uint8_t num,int32_t* Get_Encoder_countA,int32_t* Get_Encoder_countB)
{
	int32_t Temp = 0; 
	if(num ==1)
	{
		Temp = *Get_Encoder_countA;
		*Get_Encoder_countA = 0;
		return Temp;
	}
	else if(num ==2)
	{
		Temp = *Get_Encoder_countB;
		*Get_Encoder_countB = 0;
		return Temp;
	}
	else
	{return 0;}
}

/*******************************************************
函数功能：外部中断模拟编码器信号
入口函数：无
返回  值：无
***********************************************************/
void GROUP1_IRQHandler(void)
{
	
	//获取中断信号
	gpio_interrup = DL_GPIO_getEnabledInterruptStatus(GPIOB,Encoder_E2A_PIN) | DL_GPIO_getEnabledInterruptStatus(GPIOA,Encoder_E1A_PIN);
	//encoder1（左）
	if((gpio_interrup & Encoder_E1A_PIN)==Encoder_E1A_PIN)
	{
		if(!DL_GPIO_readPins(GPIOA,Encoder_E1B_PIN))
		{
			Get_Encoder_countA++;
			
		}
		else
		{
			Get_Encoder_countA--;
		}
	}
	//encoder2（右）
	else if((gpio_interrup & Encoder_E2A_PIN)==Encoder_E2A_PIN)
	{
		if(!DL_GPIO_readPins(GPIOB,Encoder_E2B_PIN))
		{
			Get_Encoder_countB--;
		}
		else
		{
			Get_Encoder_countB++;
		}
	}
	
	DL_GPIO_clearInterruptStatus(GPIOB,Encoder_E2A_PIN);
    DL_GPIO_clearInterruptStatus(GPIOA,Encoder_E1A_PIN);
}