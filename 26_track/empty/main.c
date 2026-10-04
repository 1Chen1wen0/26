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
#include "beep.h"
#include "track.h"
#include "Key.h"
#include "jy61p.h"
#include "Key.h"
#include "Motor.h"
#include "oled.h"
#include "step_motor.h"
#include "Encoder.h"
#include "string.h"
#include "k230.h"
#include "ntb_time.h"


#define Basic_speed		18
#define Balance_speed	15
#define Position_Len	250


float LeftSpeed = 0,RightSpeed = 0,AveSpeed =0 ,DifSpeed =0;
float LeftPWM = 0,RightPWM = 0,AvePWM= 0,DifPWM = 0,YawPWM=0;
volatile int8_t Run_flag = 0,Stage_flag=0,Start_Flag = 0,Frist_Flag =1;
uint8_t Rounds =0;
int16_t Encoder1 = 0,Encoder2 =0;
int8_t Error;
int8_t Judge;
int8_t Position_Flag =0;
uint32_t Position;

//速度环
	PID_t SpeedPID = {					
	.Kp = 13,						
	.Ki = 5,						
	.Kd = 3,						
	
	.OutMax =800,					
	.OutMin =-800,					
		
	.ErrorIntMax = 600,				
	.ErrorIntMin = -600,				
};
////转向环	
	PID_t TurnPID = {					
	.Kp = 10,						
	.Ki = 0,						
	.Kd = 0.5,						
	
	.OutMax = 800,					
	.OutMin = -800,					
	
	.ErrorIntMax = 500,				
	.ErrorIntMin = -500,				
};
////位置环
	PID_t LocationPID = {
	.Kp = 0,						
	.Ki = 0,						
	.Kd = 0,						
	
	.OutMax = 100,					
	.OutMin = 100,					
	
	.ErrorIntMax = 0,				
	.ErrorIntMin = 0,

		
};		
		
	//云台X轴环	
	PID_t Steper_XPID = {					
	.Kp = 0,						
	.Ki = 0,						
	.Kd = 0,						
	
	.OutMax = 1000,					
	.OutMin = -1000,					
	
	.ErrorIntMax = 1000,				
	.ErrorIntMin = -1000,				
};	

	

int main(void)
{
    SYSCFG_DL_init();
    OLED_Init();
    OLED_ColorTurn(0);//0正常显示，1 反色显示
    OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
    OLED_Clear();
	PID_Init(&SpeedPID);
	PID_Init(&TurnPID);
	PID_Init(&Steper_XPID);
	encoder__init();
	Motor_Init();
	WIT_Init();//陀螺仪初始化函数，开启串口中断，清除中断标志位
//	stepper_init();
//	Stepper_start(3);

    //清除定时器中断标志
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
	DL_TimerA_startCounter(TIMER_0_INST);

	DL_TimerG_setTimerCount(NTB_INST, 0);
	DL_TimerG_stopCounter(NTB_INST);//时间戳停止计数
	
	NVIC_ClearPendingIRQ(BLE_INST_INT_IRQN);   
	NVIC_EnableIRQ(BLE_INST_INT_IRQN);
	
	NVIC_ClearPendingIRQ(k230_INST_INT_IRQN);   
	NVIC_EnableIRQ(k230_INST_INT_IRQN);
	//陀螺仪中断开启
	NVIC_ClearPendingIRQ(jy61p_INST_INT_IRQN); //清除中断标志位
	NVIC_EnableIRQ(jy61p_INST_INT_IRQN);//开启串口中断
	Serial_JY61P_Zero_Roll_Pitch();
	Serial_JY61P_Zero_Yaw();
	
	

    while (1) {
//jy61p test
		
//OLED_ShowSignedNum(0,0,wit_data.yaw,5,16);

//Stepper_SpeedControl(1,100);
//Stepper_SpeedControl(2,100);


//if(Key_Num)
//{
//	if(Key_Num ==1)
//	{
//		TurnPID.Target += 15;
//		Key_Num =0;			
//		
//	}
//	
	 	 if( Run_flag == 1 && Start_Flag == 1)
	 {	
		Last_NTB =  get_time_stamp_ms();
		 
		OLED_ShowNum(0,0,get_time_stamp_ms()/1000,3,16);//显示时间		
		OLED_Refresh();

	  }
printf("%d,%d,%d,%lld\n",Rounds,Run_flag,Start_Flag,get_time_stamp_ms());
		
//else if(Key_Num ==2)
//{
//	
//		TurnPID.Target -= 15;
//		Key_Num =0;
//	
//}
//}

if(Key_Num)
{
	if(Key_Num ==1)
	{
		Run_flag=1;
		Key_Num =0;
	}
	else if(Key_Num ==2)
	{
		Run_flag =2;
		Key_Num =0;
	
	}
	else if(Key_Num ==3)
	{
		Start_Flag = 1;
		Key_Num =0;	
	}
    else if(Key_Num ==4)//重置状态
	{
		DL_TimerG_setTimerCount(NTB_INST, 0);
		DL_TimerG_stopCounter(NTB_INST);
		Key_Num =0;
		Start_Flag = 0;
		Run_flag = 0;
		Start_Flag = 0;
		Frist_Flag = 1;
		SpeedPID.Target = 0;
	}
}
			
	

/*开始运行*/

//SpeedPID.Target = Basic_speed;
TurnPID.Actual = track_scan(2.3);
Total_TrackPins = (L1+L2+L3+Mid+R3+R2+R1);
if(Find_CrossRoad() == 1)
{
	Rounds++;
}

if(Start_Flag == 1)
{
	switch(Run_flag)
	{
		
		case(0):
			SpeedPID.Target = 0;
			TurnPID.Out = 0;
			Position =0;
			break;
		//第一题
		case(1):
			if(Frist_Flag == 1)
			{
				DL_TimerG_startCounter(NTB_INST);
				Rounds = 0;
				Frist_Flag = 0;
				SpeedPID.Target = Basic_speed;
			}
			
			if(Rounds)
			{
				SpeedPID.Target = 0;
				Start_Flag = 0;
				Run_flag = 0;
				Start_Flag = 0;
				Frist_Flag = 1;
				Rounds = 0;
				DL_TimerG_stopCounter(NTB_INST);
			}
			break;
			
		//第二题
		case(2):
			if(Frist_Flag == 1)
			{
				DL_TimerG_startCounter(NTB_INST);
				Rounds = 0;
				Frist_Flag = 0;
				SpeedPID.Target = Basic_speed;
				Position_Flag = 1;
			}
			if(Position>=1200)
			{
				SpeedPID.Target = 0;
				Start_Flag = 0;
				Run_flag = 0;
				Start_Flag = 0;
				Frist_Flag = 1;
				Rounds = 0;
				Position_Flag = 0;
				Position = 0;
				DL_TimerG_stopCounter(NTB_INST);
			}
		
			break;		
			
		default: break;			
	}




}

		}
	}
	
	
//定时中断,10毫秒触发一次
//电机和云台调控
 void TIMER_0_INST_IRQHandler(void)
 {          
	static uint8_t count1 = 0,count2 = 0,count3 = 0;
     if( DL_TimerG_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO)
     {	
		count1++;
		count2++;
		count3++;
		Key_Tick();

	/*车轮控制*/
	if(count1>=3)
	{
	 count1 = 0;
	 
	 Encoder1= Motor_GetSpeed(1,&Get_Encoder_countA,&Get_Encoder_countB);
	 Encoder2= Motor_GetSpeed(2,&Get_Encoder_countA,&Get_Encoder_countB);
	//里程计
	if(Position_Flag)
	{
		Position += (Encoder1+Encoder1)/2;
	}

	//读取编码器换算速度:周长*编码器值/（编码器线数*四倍频*减速比*单位时间），单位cm/s	一倍频，减速比28，线数13
	LeftSpeed =(3.14*6.5*Encoder1)/28/13/0.03;
	RightSpeed =(3.14*6.5*Encoder2)/28/13/0.03;
		
	
	AveSpeed = (LeftSpeed + RightSpeed)/2.0;//平均速度
	DifSpeed =  RightSpeed- LeftSpeed;//差分速度

		
//速度环更新	
	SpeedPID.Actual = AveSpeed;
	PID_Update(&SpeedPID);
    AvePWM = SpeedPID.Out;
		
	//寻迹转向环更新
	TurnPID.Target = 0;
	PID_Update(&TurnPID);
	DifPWM = TurnPID.Out;
	
	//串口调试
	//printf("%f,%f,%f,%f\n",LeftSpeed,RightSpeed,RightPWM,SpeedPID.ErrorInt);
	
	//printf("%lld,%lld\n",get_time_stamp_ms(),Last_NTB);
	
	//输出
	//DifPWM<0右转,DifPWM>0左转
	LeftPWM = AvePWM + DifPWM / 2;		//由平均 PWM和差分PWM计算得到左轮PWM
	RightPWM = AvePWM - DifPWM / 2;		//由平均PWM和差分PWM计算得到右轮PWM



	if (LeftPWM > 1000) {LeftPWM = 1000;} else if (LeftPWM < -1000) {LeftPWM = -1000;}
	if (RightPWM > 1000) {RightPWM = 1000;} else if (RightPWM < -1000) {RightPWM = -1000;}
		
	Motor_SetPWM(1, LeftPWM);		
	Motor_SetPWM(2, RightPWM);


	}

	//RightPWM输出给右轮电朿
	
	 /*云台控制*/
	 if(count2>=2)
	 {	
	//Steper_XPID.Actual = ;
	//PID_Update(&Steper_XPID);
		count2 = 0;

	  }
	 	 if(count3>=10 && Run_flag == 1 && Start_Flag == 1)
	 {	


	  }
 }
}
 
//		Get_VisualError(&Steper_XPID,&Steper_YPID);
//		Error_PID_Updata(&Steper_XPID);
//		Error_PID_Updata(&Steper_YPID);

//		Stepper_SpeedControl(1,Steper_XPID.Out);
//		Stepper_SpeedControl(2,Steper_YPID.Out);
