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
#include "Servo.h"


#define Basic_speed		25
#define Track_speed		20
#define Turn_Angle		38.66//28
#define Angle_fix1   	-12 //2题出弯修正
#define Angle_fix2   	11 //3出弯修正
#define Angle_fix3   	35 //3，4入弯修正
#define Position_Len	250


float LeftSpeed = 0,RightSpeed = 0,AveSpeed =0 ,DifSpeed =0;
float LeftPWM = 0,RightPWM = 0,AvePWM= 0,DifPWM = 0,YawPWM=0;
volatile uint8_t Run_flag = 0,Stage_flag=0;
uint8_t Rounds =0;
int16_t Encoder1 = 0,Encoder2 =0;
int8_t Error;
int8_t Judge;
int8_t YawFlag  = 1,Position_Flag =0;
uint8_t Total_TrackPins;
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
//偏航角环(这个版本里为角度积分PID)		
	PID_t YawPID = {
	.Kp = 10,						
	.Ki = 0.2,						
	.Kd = 0.8,						
	
	.OutMax = 700,					
	.OutMin = -700,					
	
	.ErrorIntMax = 300,				
	.ErrorIntMin = -300,		
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
	//云台Y轴环	
	PID_t Steper_YPID = {					
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
	PID_Init(&YawPID);
	PID_Init(&Steper_XPID);
	PID_Init(&Steper_YPID);
	encoder__init();
	Motor_Init();
	Servo_Init();
	WIT_Init();//陀螺仪初始化函数，开启串口中断，清除中断标志位
	Warning_Init();
//	stepper_init();
//	Stepper_start(3);

    //清除定时器中断标志
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
	DL_TimerA_startCounter(TIMER_0_INST);
//	NVIC_ClearPendingIRQ(Angle_TIMER_INST_INT_IRQN);
//	NVIC_EnableIRQ(Angle_TIMER_INST_INT_IRQN);
//	DL_TimerG_startCounter(Angle_TIMER_INST);
	
	
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
//		Motor_SetPWM(1,-100);		//LeftPWM输出给左轮电朿
//		Motor_SetPWM(2,-100);		//RightPWM输出给右轮电朿
//		

	
//		for(int i = 0;i<270;i++)
//		{
//		Servo_SetAngle(1,i);
//		Servo_SetAngle(2,i);
//		delay_ms(10);
//		}
//DL_TimerG_setCaptureCompareValue(Servo_INST,125,GPIO_Servo_C0_IDX);
		Servo_SetAngle(1,50);
//		Servo_SetAngle(2,200);
	
//if(find_track() == 1 && Run_flag == 0)
//{
//	warning();
//	Run_flag = 1;
//}

//if( Run_flag == 1)
//{
//	if(Judge_track() == 2)
//	{		
//		warning();
//		SpeedPID.Target = 0;
//		motor_stop();
//		Run_flag = 2;
//	}
//}

if(Key_Num)
{
	if(Key_Num ==1)
	{
		//SpeedPID.Target += 15;
		Last_NTB  = get_time_stamp_ms();				
		
	}
	if(Key_Num ==1 && (get_time_stamp_ms() - Last_NTB >300))
	{
		Key_Num =0;
		
	}
//else if(Key_Num ==2)
//	{
//		SpeedPID.Target -= 15;
//		Run_flag = 0;
//		Key_Num =0;
//	}
}
OLED_ShowNum(0,16,Key_Num,5,16);
OLED_ShowChar(0,0,Serial_RxPacket[1],16);
OLED_Refresh();
//if(Key_Num)
//{
//	if(Key_Num ==1)
//	{
//		Key_Num = 0;
//		YawPID.Target +=45;
//	}
//	else if(Key_Num ==2)
//	{
//		
//		Key_Num = 0;
//		YawPID.Target -=45;
//	}
//}
//YawPID.Target = 0;			
//			

/*开始运行*/
/*
//SpeedPID.Target = Basic_speed;
TurnPID.Actual = track_scan(2.3);
Total_TrackPins = (L1+L2+L3+Mid+R3+R2+R1);
//warning();

if(Key_Num)
{
	Run_flag =Key_Num;
	Key_Num =0;
}

	switch(Run_flag)
	{
		case(0):
			SpeedPID.Target = 0;
			YawPID.Out = 0;
			TurnPID.Out = 0;
			Position =0;
			break;
		//第一题
		case(1):
			
			SpeedPID.Target = Basic_speed;
			if(YawFlag)
			{
				YawFlag = 0;
				YawPID.Actual = 0;
				YawPID.Target = 0;
				delay_ms(200);//消抖
			}
			
			if(find_track())
			{
				//一题结束时操作，停止电机，目标值设为零
				SpeedPID.Target = 0;
				Motor_SetPWM(1,0);		//LeftPWM输出给左轮电朿
				Motor_SetPWM(2,0);		//RightPWM输出给右轮电朿
				Run_flag = 0;
				Stage_flag = 0;
			    YawFlag = 1;
				warning();				
				break;
			}
			break;

//			
//		//第二题
		case(2):
			switch(Stage_flag)
			{
				case(0):
					SpeedPID.Target = Basic_speed;
					if(YawFlag)
					{
						YawFlag = 0;
						YawPID.Actual = 0;
						YawPID.Target = 0;
						delay_ms(200);//消抖
					}
					
					if(find_track())
					{
						//一题结束时操作，停止电机，目标值设为零
						Stage_flag = 1;
						YawFlag = 1;
						warning();				
						break;
					}
					break;
						
				case(1)://寻迹模版
					SpeedPID.Target = Track_speed;
					YawPID.Out  = 0;
					Judge = Judge_track();
					if(Judge)//识别不到黑色
					{
						if(Judge == 1)//右出界
						{
							TurnPID.Actual = 13;
							break;
						}
						else if(Judge == -1)//左出界
						{
							TurnPID.Actual = -13;
							break;
						}
						else if(Judge == 2)//寻迹结束
						{
							Stage_flag = 2;
							warning();
							break;
						}
						
					}
					break;

					case(2):
					SpeedPID.Target = Basic_speed;
					if(YawFlag)
					{
						YawFlag = 0;
						YawPID.Actual = 0;
						YawPID.Target = Angle_fix1;
					}

					if(find_track())
					{
						Stage_flag = 3;
						YawFlag = 1;
						warning();				
						break;
					}
					break;
					case(3):
					SpeedPID.Target = Track_speed;
					Judge = Judge_track();
					if(Judge)//识别不到黑色
					{
						if(Judge == 1)//右出界
						{
							TurnPID.Actual = 13;
							break;
						}
						else if(Judge == -1)//左出界
						{
							TurnPID.Actual = -13;
							break;
						}
						else if(Judge == 2)//寻迹结束
						{
							SpeedPID.Target = 0;
							motor_stop();
							Stage_flag = 0;
							Run_flag = 0;
							warning();
							break;
						}
					break;						
						
					}
				default:break;
			}
			break;
			
			
				
//		//第三题	
		case(3):
			switch(Stage_flag)
			{
				case(0):
					SpeedPID.Target = Basic_speed;
					if(YawFlag)
					{
						YawFlag = 0;
						YawPID.Actual = 0;
						YawPID.Target = -Turn_Angle;
						delay_ms(200);
						Position_Flag = 1;
						
					}
					if(Position>=Position_Len)
					{
						YawPID.Target += Angle_fix3;
						Position_Flag = 0;
						Position = 0;
					}
					
					
					if(find_track())
					{
						//等待转正
						SpeedPID.Target = 0;
//						YawPID.Target += Angle_fix3;
						delay_ms(50);
						Position_Flag = 0;
						Position = 0;
						Stage_flag = 1;
						YawFlag = 1;
						warning();				
						break;
					}
					break;
						
				case(1)://寻迹模版
					SpeedPID.Target = Track_speed;
					YawPID.Out  = 0;
					Judge = Judge_track();
					if(Judge)//识别不到黑色
					{
						if(Judge == 1)//右出界
						{
							TurnPID.Actual = 11;
							break;
						}
						else if(Judge == -1)//左出界
						{
							TurnPID.Actual = -11;
							break;
						}
						else if(Judge == 2)//寻迹结束
						{
							Stage_flag = 2;
							warning();
							break;
						}
						
					}
					break;

					case(2):
					SpeedPID.Target = Basic_speed;
					if(YawFlag)
					{
						YawFlag = 0;
						YawPID.Actual = 0;
						YawPID.Target = Turn_Angle+Angle_fix2;
						delay_ms(200);
						Position_Flag = 1;
						
						
					}
					
					if(Position>=Position_Len+50)
					{
						YawPID.Target -= Angle_fix3;
						Position_Flag = 0;
						Position = 0;
						
					}
					
					if(find_track())
					{
						SpeedPID.Target = 0;
//						YawPID.Target -= Angle_fix3;
						delay_ms(50);
						Position_Flag = 0;
						Position = 0;
						Stage_flag = 3;
						YawFlag = 1;
						warning();				
						break;
					}
					break;
					case(3):
					SpeedPID.Target = Track_speed;
					Judge = Judge_track();
					if(Judge)//识别不到黑色
					{
						if(Judge == 1)//右出界
						{
							TurnPID.Actual = 11;
							break;
						}
						else if(Judge == -1)//左出界
						{
							TurnPID.Actual = -11;
							break;
						}
						else if(Judge == 2)//寻迹结束
						{
							SpeedPID.Target = 0;
							motor_stop();
							Stage_flag = 0;
							Run_flag = 0;
							warning();
							break;
						}
					break;						
						
					}
				default:break;
			}
			break;

			
			
// 		//第四题	
			case(4):
			switch(Stage_flag)
			{
				case(0):
					SpeedPID.Target = Basic_speed;
					if(YawFlag)
					{

						YawFlag = 0;
						YawPID.Actual = 0;
						YawPID.Target = -Turn_Angle;
						if(Rounds>0)
						{
							YawPID.Target = -Turn_Angle-Angle_fix2;
						}
						else
						{
							delay_ms(200);	
						}
						Position_Flag = 1;

					}
					
					if(Rounds == 0)
					{
						if(Position>=Position_Len)
						{
							YawPID.Target += Angle_fix3;
							Position_Flag = 0;
							Position = 0;
						}
					}
					else if(Rounds > 0)
					{
						if(Position>=Position_Len+40)
						{
							YawPID.Target += Angle_fix3;
							Position_Flag = 0;
							Position = 0;
						}
					}

					
					if(find_track())
					{
						//等待转正
//						SpeedPID.Target = 0;
//						YawPID.Target += Angle_fix3;
//						delay_ms(50);
						Position_Flag = 0;
						Position = 0;
						Stage_flag = 1;
						YawFlag = 1;
						warning();	
					}
					break;
						
				case(1)://寻迹模版
					SpeedPID.Target = Track_speed;
					YawPID.Out  = 0;
					Judge = Judge_track();
					if(Judge)//识别不到黑色
					{
						if(Judge == 1)//右出界
						{
							TurnPID.Actual = 11;
							break;
						}
						else if(Judge == -1)//左出界
						{
							TurnPID.Actual = -11;
							break;
						}
						else if(Judge == 2)//寻迹结束
						{
							Stage_flag = 2;
							warning();
							break;
						}
						
					}
					break;

					case(2):
					SpeedPID.Target = Basic_speed;
					if(YawFlag)
					{
						YawFlag = 0;
						YawPID.Actual = 0;	
						YawPID.Target = Turn_Angle+Angle_fix2;
						delay_ms(200);
						Position_Flag = 1;
					}
					
					if(Position>=Position_Len+80)
					{
						YawPID.Target -= Angle_fix3;
						Position_Flag = 0;
						Position = 0;	
					}
					if(find_track())
					{
						//等待转正
//						SpeedPID.Target = 0;
//						YawPID.Target += Angle_fix3;
//						delay_ms(50);
						Stage_flag = 3;
						YawFlag = 1;
						Position_Flag = 0;
						Position = 0;
					
						warning();	
					}
					
					break;
					
					case(3):
					SpeedPID.Target = Track_speed;
					Judge = Judge_track();
					if(Judge)//识别不到黑色
					{
						if(Judge == 1)//右出界
						{
							TurnPID.Actual = 11;
							break;
						}
						else if(Judge == -1)//左出界
						{
							TurnPID.Actual = -11;
							break;
						}
						else if(Judge == 2)//寻迹结束
						{
							Rounds++;
							if(Rounds >=4)
							{
								SpeedPID.Target = 0;
								motor_stop();
								Stage_flag = 0;
								Run_flag = 0;
								warning();
							}
							else
							{
								Stage_flag = 0;
								warning();								
							}
							break;
						}
					break;						
						
					}				
			default:break;
				}					
		default: break;			
	}


//云台控制
////根据k230串口发送的标志位判断是否启动云台
	
//			if(Key_Num)
//			{
//				if(k230_flag == 0)
//				{
//				Stepper_stop(1);
//				Stepper_stop(2);
//				}

//				else if(k230_flag ==1)
//				{
//				Stepper_start(3);
//				Get_VisualError(&Steper_XPID,&Steper_YPID);
//				}

//				else if(k230_flag == 2)
//				{
//				Stepper_start(3);
//				Stepper_SpeedControl(1,300);
//				}
//			}
*/

		}
	}
//定时中断,10毫秒触发一次
//电机和云台调控
 void TIMER_0_INST_IRQHandler(void)
 {          
	static uint8_t count1 = 0,count2 = 0;
     if( DL_TimerG_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO)
     {	
		count1++;
		count2++;
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
	
	if(!YawFlag)
	{
		YawPID.Actual += wit_data.gz*0.02;	
		Angle_PID_Update(&YawPID);
		TurnPID.Out = 0;
	}
	//寻迹中
	else
	{
		YawPID.Actual = 0;
		YawPID.Target = 0;
		YawPID.Out = 0;
	}

//	DifPWM = TurnPID.Out + YawPID.Out ;
	//printf("%f,%f\n",SpeedPID.Target,SpeedPID.Actual);
	printf("%lld,%lld\n",get_time_stamp_ms(),Last_NTB);
	//printf("%d,%d,%d\n",wit_data.gz,wit_data.gx,wit_data.gy);
	//输出
	//DifPWM<0右转,DifPWM>0左转
	LeftPWM = AvePWM + DifPWM / 2;		//由平均 PWM和差分PWM计算得到左轮PWM
	RightPWM = AvePWM - DifPWM / 2;		//由平均PWM和差分PWM计算得到右轮PWM

	if (LeftPWM > 1000) {LeftPWM = 1000;} else if (LeftPWM < -1000) {LeftPWM = -1000;}
	if (RightPWM > 1000) {RightPWM = 1000;} else if (RightPWM < -1000) {RightPWM = -1000;}
		
//	Motor_SetPWM(1, LeftPWM);		
//	Motor_SetPWM(2, RightPWM);	
	
	}

	//RightPWM输出给右轮电朿
	
	 /*云台控制*/
	 if(count2>=2)
	 {
//		count2 = 0;
//		Get_VisualError(&Steper_XPID,&Steper_YPID);
//		Error_PID_Updata(&Steper_XPID);
//		Error_PID_Updata(&Steper_YPID);

//		Stepper_SpeedControl(1,Steper_XPID.Out);
//		Stepper_SpeedControl(2,Steper_YPID.Out);
	  }
 }
}
 
