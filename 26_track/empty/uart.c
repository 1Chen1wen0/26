#include "ti_msp_dl_config.h"
#include "Delay.h"
#include "oled.h"
#include <stdio.h>
#include <stdarg.h>
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
#include "stdlib.h"

void hhSerialSendByte(UART_Regs *uart, const uint8_t chr)
{
    DL_UART_transmitDataBlocking(uart, chr);
}

void hhSerialSendString(UART_Regs *uart, const char *str)
{
    while (*str) {
        hhSerialSendByte(uart, (uint8_t) *str);
        str++;
    }
}

void hhSerialSendArray(UART_Regs *uart, uint8_t *Array,uint16_t Length){
for(uint16_t i=0;i<Length;i++){
hhSerialSendByte(uart,Array[i]);
}
}

uint32_t Serial_Pow(UART_Regs *uart,uint32_t X, uint32_t Y)
{
uint32_t Result = 1;
while (Y --)
{
Result *= X;
}
return Result;
}

void hhSerial_SendNumber(UART_Regs *uart,uint32_t Number, uint8_t Length)
{
uint8_t i;
for (i = 0; i < Length; i ++)
{
hhSerialSendByte(uart,Number / Serial_Pow(uart,10, Length - i - 1) % 10 + '0');
}
}

uint8_t Serial_RxFlag;
uint8_t Serial_GetRxFlag(void)
{
if (Serial_RxFlag == 1)
{
Serial_RxFlag = 0;
return 1;
}
return 0;
}
//串口重定向
int fputc(int c, FILE* stream)
{
	DL_UART_Main_transmitDataBlocking(BLE_INST, c);
    return c;
}
 
int fputs(const char* restrict s, FILE* restrict stream)
{
    uint16_t i, len;
    len = strlen(s);
    for(i=0; i<len; i++)
    {
        DL_UART_Main_transmitDataBlocking(BLE_INST, s[i]);
    }
    return len;
}
 
int puts(const char *_ptr)
{
        int count = fputs(_ptr,stdout);
        count +=fputs("\n",stdout);
        return count;
}

//自定义Printf
 void Serial_Printf(UART_Regs *uart,char *format, ...)
{
char String[100];
va_list arg;
va_start(arg, format);
vsprintf(String, format, arg);
va_end(arg);
hhSerialSendString(uart,String);
}


uint8_t Serial_RxFlag;
char Serial_RxPacket[100];
void BLE_INST_IRQHandler()
{
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;
    switch (DL_UART_getPendingInterrupt(BLE_INST))
    {
	
    case DL_UART_IIDX_RX:
        {   
	uint8_t ByteRecv = DL_UART_Main_receiveData(BLE_INST);
	
			
        if (RxState == 0)
        {
            if ((ByteRecv == '[') && Serial_RxFlag == 0)//把数据读取走后才接收下一个数据包防止粘包问题

            {
                RxState = 1;
                pRxPacket = 0;
//				Serial_RxFlag = 1;
            }

        }
        else if (RxState == 1)
        {
            if (ByteRecv == ']')
			
            {
                RxState = 0;
                Serial_RxPacket[pRxPacket] = '\0';//字符串必须带'\0'作为结束标志
                Serial_RxFlag = 1;
            }
            else
            {
				Serial_RxPacket[pRxPacket] = ByteRecv;
				pRxPacket ++;
				DL_UART_Main_transmitData(BLE_INST,ByteRecv);             
//                hhSerialSendByte(BLE_INST,ByteRecv );           
            }
        }
            break;
        }
    
    default:
        break;
		}
}

void CallbackAnalysis(PID_t *p)//远程调参，通过逗号分割字符串，第一个字符串为“slider”，接下来三个参数为kp,ki,kd.
{ 
//if (Serial_RxFlag == 1)
//	{		
		char *Tag = strtok(Serial_RxPacket, ",");

		//接收滑杆值调参
		if (strcmp(Tag, "slider") == 0)
		{
            char *Para = strtok(NULL, ",");
            if(strcmp(Para, "1") == 0)
			{p->Kp = atoi(strtok(NULL, ","));}
            if(strcmp(Para, "2") == 0)
			{p->Ki = atof(strtok(NULL, ","));}
            if(strcmp(Para, "3") == 0)
			{p->Kd = atof(strtok(NULL, ","));}
		}	
		if(strcmp(Tag, "stop") == 0)
		{
						
		}
	Serial_RxFlag = 0;
//	}

    
}