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
#include "stdlib.h"


uint8_t k230_Serial_RxFlag;
uint8_t k230_ByteRecv;
char k230_Serial_RxPacket[10];


void k230_INST_IRQHandler()
{

    switch (DL_UART_getPendingInterrupt(k230_INST))
    {
    case DL_UART_IIDX_RX:
        {   
	k230_ByteRecv = DL_UART_Main_receiveData(k230_INST);
	
            static uint8_t RxState = 0;
            static uint8_t pRxPacket = 0;			
        if (RxState == 0)
        {
            if ((k230_ByteRecv == '{') && k230_Serial_RxFlag == 0)//把数据读取走后才接收下一个数据包防止粘包问题

            {
                RxState = 1;
                pRxPacket = 0;
				k230_Serial_RxFlag = 1;
            }

        }
        else if (RxState == 1)
        {
            if (k230_ByteRecv == '}')
			
            {
                RxState = 0;
                Serial_RxPacket[pRxPacket] = '\0';//字符串必须带'\0'作为结束标志
                k230_Serial_RxFlag = 0;
            }
            else
            {
				DL_UART_Main_transmitData(k230_INST,k230_ByteRecv);
                Serial_RxPacket[pRxPacket] = k230_ByteRecv;
                //hhSerialSendByte(k230_INST,k230_ByteRecv );
                pRxPacket ++;
            }
        }
            break;
        }
    
    default:
        break;
		}
}
//void k230_INST_IRQHandler()
//{
//    switch (DL_UART_getPendingInterrupt(k230_INST))
//    {
//    case DL_UART_IIDX_RX:
//        {   
//            static uint8_t RxState = 0;
//            static uint8_t pRxPacket = 0;
//        if (RxState == 0)
//        {
//            if ((k230_ByteRecv == '{') && k230_Serial_RxFlag == 0)//把数据读取走后才接收下一个数据包防止粘包问题

//            {
//                RxState = 1;
//                pRxPacket = 0;
//				Serial_RxFlag = 1;
//            }

//        }
//        else if (RxState == 1)
//        {
//            if (k230_ByteRecv == '}')

//            {
//                RxState = 0;
//                k230_Serial_RxPacket[pRxPacket] = '\0';//字符串必须带'\0'作为结束标志
//                Serial_RxFlag = 0;
//            }
//            else
//            {
//                k230_Serial_RxPacket[pRxPacket] = ByteRecv;
//                pRxPacket ++;
//            }
//        }
//            break;
//        }
//    
//    default:
//        break;
//    }
//}
uint8_t k230_flag = 0;
//格式:{flag,x,y}
void Get_VisualError(PID_t *px,PID_t *py)//从k230获取与靶心的误差
{ 
	
	if(px ==NULL || py ==NULL)
	{return ;}
		char *Tag = strtok(k230_Serial_RxPacket, ",");

		//接收xy轴坐标误差
		if (strcmp(Tag, "1") == 0)
		{
			k230_flag = 1;
			px->Error0 = atof(strtok(NULL, ","));
			py->Error0 = atof(strtok(NULL, ","));
		}
		
		else if(strcmp(Tag,"2") == 0)
		{k230_flag = 2;}
}
    
