/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
//cmd.exe /C "$P../../tools/keil/syscfg.bat '$P' empty.syscfg"
#include "ti_msp_dl_config.h"
#include "Delay.h"
#include "oled.h"
#include <stdio.h>
#include "uart.h"
#include "step_motor.h"
#include "Pid.h"
#include "Key.h"
#include "jy61p.h"
#include "Key.h"
#include "step_motor.h"
#include "Encoder.h"
#include "string.h"
#include "k230.h"
#include "ntb_time.h"
#include "usart.h"
#include "X_V2.h"
#include "fifo.h"

#define Basic_speed		18
#define Position_Len	250


float Turn_Angle = 0,Turn_Speed = 0,Actual_Angle = 0;
float LeftPWM = 0,RightPWM = 0,AvePWM= 0,DifPWM = 0,YawPWM=0;
volatile int8_t Run_flag = 0,Stage_flag=0,Start_Flag = 0,Frist_Flag =1;
uint8_t Rounds =0;
int16_t Encoder1 = 0,Encoder2 =0;
int8_t Error;
int8_t Judge;
int8_t Position_Flag =0;
uint32_t Position;
float Stepper_fix = 1;

//加速度环
	PID_t AXPID = {
	.Kp = 0.3,						
	.Ki = 0,						
	.Kd = 0,						
	
	.OutMax = 100,					
	.OutMin = -100,					
	
	.ErrorIntMax = 0,				
	.ErrorIntMin = 0,

		
};		
		
	//云台X轴环	
	PID_t Steper_XPID = {					
	.Kp = 0.05,						
	.Ki = 0,						
	.Kd = 0.25,						
	
	.OutMax = 1000,					
	.OutMin = -1000,					
	
	.ErrorIntMax = 1000,				
	.ErrorIntMin = -1000,				

};	

	

int main(void)
{
    SYSCFG_DL_init();
	PID_Init(&AXPID);
	PID_Init(&Steper_XPID);
	encoder__init();
	WIT_Init();//陀螺仪初始化函数，开启串口中断，清除中断标志位
	K230_Init();//K230 UART2 DMA接收，RX Timeout判定一帧结束
//	stepper_init();
//	Stepper_start(3);

    //清除定时器中断标志
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
	DL_TimerA_startCounter(TIMER_0_INST);

	DL_TimerG_setTimerCount(NTB_INST, 0);
//	DL_TimerG_stopCounter(NTB_INST);//时间戳停止计数
//	
	NVIC_ClearPendingIRQ(BLE_INST_INT_IRQN);   
	NVIC_EnableIRQ(BLE_INST_INT_IRQN);
	
	//陀螺仪中断开启
	NVIC_ClearPendingIRQ(jy61p_INST_INT_IRQN); //清除中断标志位
	NVIC_EnableIRQ(jy61p_INST_INT_IRQN);//开启串口中断

	
	// 清除串口中断标志
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	// 使能串口中断
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
	X_V2_Reset_Motor(1);
	
	X_V2_En_Control(1,false,false);
	delay_ms(100);
	X_V2_Reset_CurPos_To_Zero(1);
	/*开始执行主程序*/
    while (1) 
	{



if(Key_Num)
{
	if(Key_Num ==1)
	{
		hhSerialSendString(k230_INST,"{0}");//k230,+-5cm运动
		Run_flag =1;
		X_V2_En_Control(1,true,false);		
		Key_Num =0;
		

	}
	else if(Key_Num ==2)
	{		
//		hhSerialSendString(k230_INST,"{1}");//k230定点
		Run_flag =2;
		X_V2_En_Control(1,true,false);
		delay_ms(50);
		X_V2_Bypass_Pos_LV_Control(1, 0, 20, 65.5, 1, 0);
		Key_Num =0;	
		
	
	}
	else if(Key_Num ==3)
	{
		Start_Flag = 1;
		hhSerialSendString(k230_INST,"{1}");//k230定点
		delay_ms(100);
		hhSerialSendString(k230_INST,"{2}");//k230启动
		X_V2_Reset_CurPos_To_Zero(1);
		Key_Num =0;	
		delay_ms(100);
		
	}
    else if(Key_Num ==4)//重置状态
	{
		hhSerialSendString(k230_INST,"{3}");//k230重置
		Key_Num =0;
		Start_Flag = 0;
		Run_flag =0;
		X_V2_En_Control(1,false,false);
		delay_ms(100);
		X_V2_Reset_CurPos_To_Zero(1);
		delay_ms(100);
	}
}
	

//开始运行//需手动复位校准时间，按两次按键1
if(Start_Flag)
{
	if(Run_flag == 2)
	{
		if(Get_VisualError(&Steper_XPID) != 0U)
		{
			
			PID_Update(&Steper_XPID);
			
			Actual_Angle = Steper_XPID.Out+AXPID.Out;
			
			if(Actual_Angle < -17)
			{
				Actual_Angle = -17;
			}
			else if(Actual_Angle > 17)
			{
				Actual_Angle = 17;
			}

			if(Actual_Angle > 0)
			{
				X_V2_Bypass_Pos_LV_Control(1, 0, 20, Actual_Angle, 1, 0);
			}
			else
			{
				X_V2_Bypass_Pos_LV_Control(1, 1, 20,Actual_Angle , 1, 0);
			}
			
		}
	}
//	else if(Run_flag == 1)
//	{
//	delay_ms(50);
//	X_V2_Bypass_Pos_LV_Control(1, 1, 2000.0f, 7, 1, 0);	
//	delay_ms(550);
//	X_V2_Bypass_Pos_LV_Control(1, 0, 2000.0f, 5, 1, 0);		
//	delay_ms(1000);
//	X_V2_Bypass_Pos_LV_Control(1, 1, 2000.0f, 3, 1, 0);	
//	Run_flag = 0;	
//	}
}
//printf("%f\n",AXPID.Out);
	}
}


	
//定时中断,10毫秒触发一次
//电机和云台调控
 void TIMER_0_INST_IRQHandler(void)
 {          
	static uint8_t count1 = 0;
     if( DL_TimerG_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO)
     {	
	
		
	count1++;	 
	if(count1>=3)
	{
		if(Start_Flag)
		{		
			AXPID.Error0 =  wit_data.ax;
			AXPID_Update(&AXPID);
			
		}
		count1 = 0;
	}
Key_Tick();

     }
}
 
