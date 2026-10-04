/*
 * FreeRTOS on MSPM0G3507 (Cortex-M0+, 32MHz)
 * Three LED tasks with different blink frequencies
 */

#include "FreeRTOS.h"
#include "task.h"
#include "ti_msp_dl_config.h"
#include "Delay.h"
#include "oled.h"
#include <stdio.h>
#include "uart.h"
#include "Pid.h"
#include "beep.h"
#include "track.h"
#include "Key.h"
#include "jy61p.h"
#include "Key.h"
#include "Motor.h"
#include "oled.h"
#include "Encoder.h"
#include "string.h"
#include "k230.h"
#include "ntb_time.h"


#define Basic_speed		15
#define Balance_speed1	10
#define Balance_speed2	10
#define Position_Len	250

typedef enum
{
	RUN_STATE_IDLE = 0,
	RUN_STATE_FIRST_QUESTION,
	RUN_STATE_SECOND_QUESTION,
	RUN_STATE_THIRD_QUESTION
} RunState_t;


float LeftSpeed = 0,RightSpeed = 0,AveSpeed =0 ,DifSpeed =0;
float Final_Speed;
float LeftPWM = 0,RightPWM = 0,AvePWM= 0,DifPWM = 0,YawPWM=0;
volatile RunState_t Run_State = RUN_STATE_IDLE;
volatile int8_t Stage_flag=0,Start_Flag = 0,Frist_Flag =1;
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

	



	
TaskHandle_t xMajorTaskHandle = NULL;
TaskHandle_t xMotorTaskHandle = NULL;
TaskHandle_t xOLEDTaskHandle = NULL;


void vMajorTask(void *pvParameters);
void vMotorTask(void *pvParameters);
void vOLEDTask(void *pvParameters);


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
//    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
//	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
//	DL_TimerA_startCounter(TIMER_0_INST);

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
//    SYSCFG_DL_initPower();
//    SYSCFG_DL_GPIO_init();
//    SYSCFG_DL_SYSCTL_init();

    /* 三个 LED 初始熄灭（各端口分别设置） */


    xTaskCreate(vMajorTask, "Major", configMINIMAL_STACK_SIZE, NULL, 2, &xMajorTaskHandle);
    xTaskCreate(vMotorTask, "Motor", configMINIMAL_STACK_SIZE, NULL, 3, &xMotorTaskHandle);
    xTaskCreate(vOLEDTask, "OLED", configMINIMAL_STACK_SIZE, NULL, 1, &xOLEDTaskHandle);
	
    vTaskStartScheduler();

    for (;;) { }
}


void vMajorTask(void *pvParameters)
{
    for (;;) {
if(Key_Num)
{
	if(Key_Num ==1)
	{
		if(Run_State<RUN_STATE_THIRD_QUESTION)
		{
			Run_State = (RunState_t)(Run_State + 1);
		}
		Key_Num =0;
	}
	else if(Key_Num ==2)
	{
		if(Run_State>RUN_STATE_IDLE)
		{
			Run_State = (RunState_t)(Run_State - 1);
		}
		Key_Num =0;
	
	}
	else if(Key_Num ==3)
	{
		Start_Flag = 1;		
		Key_Num =0;	
		delay_ms(50);
	}
    else if(Key_Num ==4)//重置状态
	{
		DL_TimerG_setTimerCount(NTB_INST, 0);
		DL_TimerG_stopCounter(NTB_INST);
		Key_Num =0;
		Start_Flag = 0;
		Run_State = RUN_STATE_IDLE;
		Start_Flag = 0;
		Frist_Flag = 1;
		Rounds = 0;
		SpeedPID.Target = 0;
	}
}
			
	

/*开始运行*/

//SpeedPID.Target = Basic_speed;
TurnPID.Actual = track_scan(2.3);
Total_TrackPins = (!L1+!L2+!L3+!Mid+!R3+!R2+!R1);
	if(Find_CrossRoad() == 1)
	{
		Rounds++;
	}
//printf("%d,%d,%d,%lld\n",Rounds,Run_State,Start_Flag,get_time_stamp_ms());
if(Start_Flag == 1)
{
	switch(Run_State)
	{
		
		case RUN_STATE_IDLE:
			SpeedPID.Target = 0;
			TurnPID.Out = 0;
			Position =0;
			break;
		//第一题
		case RUN_STATE_FIRST_QUESTION:
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
				Run_State = RUN_STATE_IDLE;
				Start_Flag = 0;
				Frist_Flag = 1;
				Rounds = 0;
				DL_TimerG_stopCounter(NTB_INST);
			}
			break;
			
		//第二题
		case RUN_STATE_SECOND_QUESTION:
			if(Frist_Flag == 1)
			{
				DL_TimerG_startCounter(NTB_INST);
				Rounds = 0;
				Frist_Flag = 0;
				Final_Speed = Balance_speed1;
				Last_NTB = get_time_stamp_ms();				
				Position_Flag = 1;
			}
				
			if(Position>=2600)
			{
				Final_Speed = 0;
				Start_Flag = 0;
				Run_State = RUN_STATE_IDLE;
				Start_Flag = 0;
				Frist_Flag = 1;
				Rounds = 0;
				Position_Flag = 0;
				Position = 0;
				DL_TimerG_stopCounter(NTB_INST);
			}
			if(SpeedPID.Target < Final_Speed && get_time_stamp_ms()-Last_NTB >=100)
			{
				SpeedPID.Target++;
				Last_NTB = get_time_stamp_ms();
			}
//			else if(SpeedPID.Target > Final_Speed && get_time_stamp_ms()-Last_NTB >=100)
//			{
//				SpeedPID.Target--;
//				Last_NTB = get_time_stamp_ms();
//			}		
			break;
		//第三题
		case RUN_STATE_THIRD_QUESTION:
			if(Frist_Flag == 1)
			{
				DL_TimerG_startCounter(NTB_INST);
				Rounds = 0;
				Frist_Flag = 0;
				SpeedPID.Target = Balance_speed2;
			}
			
			if(Rounds)
			{
				SpeedPID.Target = 0;
				Start_Flag = 0;
				Run_State = RUN_STATE_IDLE;
				Start_Flag = 0;
				Frist_Flag = 1;
				Rounds = 0;
				DL_TimerG_stopCounter(NTB_INST);
			}
			break;			
			
		default: break;			
		}
}
        vTaskDelay(pdMS_TO_TICKS(2));
   
	}
}

void vMotorTask(void *pvParameters)
{
    static uint8_t Count = 0;
    TickType_t last_wake_time;

    last_wake_time = xTaskGetTickCount();

    while (1)
    {
	Count++;
	Key_Tick();
	if(Count>=3)
	{
		Count = 0;
		
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
	
        vTaskDelayUntil(
            &last_wake_time,
            pdMS_TO_TICKS(10)
        );
    }
		
    
}

/* LED3 (PA7): 700ms */
void vOLEDTask(void *pvParameters)
{
    for (;;) {
        
		 
		OLED_ShowNum(0,0,get_time_stamp_ms()/1000,3,16);//显示时间
		OLED_ShowChar(24,0,'s',16);
		OLED_ShowNum(0,16,Run_State,2,16);//显示时间
		OLED_ShowNum(0,32,Total_TrackPins,2,16);//显示时间
		OLED_Refresh();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void vTrackTask(void *pvParameters)
{
    for (;;) {

		
         vTaskDelay(pdMS_TO_TICKS(2));
    }
}       
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    for (;;) { }
}

void vApplicationMallocFailedHook(void)
{
    for (;;) { }
}
// void TIMER_0_INST_IRQHandler(void)
// {          
//	
//     if( DL_TimerG_getPendingInterrupt(TIMER_0_INST) == DL_TIMER_IIDX_ZERO)
//     {	 }
//}
// 
