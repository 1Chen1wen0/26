
#include "ti_msp_dl_config.h"
#include "Delay.h"
#include "oled.h"
#include <stdio.h>
#include "uart.h"
#include "step_motor.h"



uint8_t Key_Num;

uint8_t Key_GetNum(void)
{
	uint8_t Temp;
	if (Key_Num)
	{
		Temp = Key_Num;
		Key_Num = 0;
		return Temp;
	}
	return 0;
}

uint8_t Key_GetState(void)
{
	if (DL_GPIO_readPins(GPIOB,Key_Key_1_PIN) == 0)
	{
		return 1;
	}
	else if (DL_GPIO_readPins(GPIOA, Key_Key_2_PIN) == 0)
	{
		return 2;
	}
	else if (DL_GPIO_readPins(GPIOA, Key_Key_3_PIN) == 0)
	{
		return 3;
	}
	else if (DL_GPIO_readPins(GPIOA, Key_Key_4_PIN) == 0)
	{
		return 4;
	}
	return 0;
}

void Key_Tick(void)
{
	static uint8_t Count;
	static uint8_t CurrState, PrevState;
	
	Count ++;
	if (Count >= 20)
	{
		Count = 0;
		
		PrevState = CurrState;
		CurrState = Key_GetState();
		
		if (CurrState == 0 && PrevState != 0)
		{
			Key_Num = PrevState;
		}
	}
}
