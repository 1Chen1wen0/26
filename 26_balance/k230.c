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
#include "ntb_time.h"
#include "k230.h"

volatile uint8_t k230_Serial_RxFlag;
char k230_Serial_RxPacket[K230_PACKET_SIZE];

static uint8_t k230_DmaRxBuffer[K230_DMA_RX_SIZE];
static char k230_FrameBuffer[K230_PACKET_SIZE];
static uint8_t k230_FrameIndex;
static uint8_t k230_InFrame;

static void K230_DMA_Start(void)
{
    DL_DMA_setSrcAddr(
        DMA,
        DMA_CH1_CHAN_ID,
        (uint32_t)&k230_INST->RXDATA
    );
    DL_DMA_setDestAddr(
        DMA,
        DMA_CH1_CHAN_ID,
        (uint32_t)&k230_DmaRxBuffer[0]
    );
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, K230_DMA_RX_SIZE);
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID);
}

static void K230_ParseBytes(const uint8_t *data, uint16_t length)
{
    uint16_t i;

    for(i = 0U; i < length; i++)
    {
        uint8_t byte = data[i];

        if(byte == (uint8_t)'{')
        {
            k230_InFrame = 1U;
            k230_FrameIndex = 0U;
            continue;
        }

        if(k230_InFrame == 0U)
        {
            continue;
        }

        if(byte == (uint8_t)'}')
        {
            k230_InFrame = 0U;
            if(k230_Serial_RxFlag == 0U)
            {
                k230_FrameBuffer[k230_FrameIndex] = '\0';
                memcpy(k230_Serial_RxPacket,
                       k230_FrameBuffer,
                       (size_t)k230_FrameIndex + 1U);
                k230_Serial_RxFlag = 1U;
            }
            continue;
        }

        if(k230_FrameIndex < (K230_PACKET_SIZE - 1U))
        {
            k230_FrameBuffer[k230_FrameIndex++] = (char)byte;
        }
        else
        {
            k230_InFrame = 0U;
            k230_FrameIndex = 0U;
        }
    }
}

void K230_Init(void)
{
    DL_DMA_disableChannel(DMA, DMA_CH1_CHAN_ID);
    while(DL_UART_isRXFIFOEmpty(k230_INST) == false)
    {
        (void)DL_UART_Main_receiveData(k230_INST);
    }

    k230_Serial_RxFlag = 0U;
    k230_FrameIndex = 0U;
    k230_InFrame = 0U;
    K230_DMA_Start();

    NVIC_ClearPendingIRQ(k230_INST_INT_IRQN);
    NVIC_EnableIRQ(k230_INST_INT_IRQN);
}

void k230_INST_IRQHandler(void)
{
    uint16_t received;

    if(DL_UART_Main_getPendingInterrupt(k230_INST) !=
       DL_UART_MAIN_IIDX_RX_TIMEOUT_ERROR)
    {
        return;
    }

    DL_DMA_disableChannel(DMA, DMA_CH1_CHAN_ID);
    received = (uint16_t)(K230_DMA_RX_SIZE -
               DL_DMA_getTransferSize(DMA, DMA_CH1_CHAN_ID));

    while((DL_UART_isRXFIFOEmpty(k230_INST) == false) &&
          (received < K230_DMA_RX_SIZE))
    {
        k230_DmaRxBuffer[received++] =
            (uint8_t)DL_UART_Main_receiveData(k230_INST);
    }

    while(DL_UART_isRXFIFOEmpty(k230_INST) == false)
    {
        (void)DL_UART_Main_receiveData(k230_INST);
    }

    K230_ParseBytes(k230_DmaRxBuffer, received);
    K230_DMA_Start();
}

//格式:{Error}
uint8_t Get_VisualError(PID_t *px)//从k230获取与靶心的误差
{ 
	static uint8_t  Count= 0;
	//若结构体不为空
	if(px ==NULL)
	{return 0U;}
	
	//只有收到完整的{Error}数据帧后才更新误差
	if(k230_Serial_RxFlag == 1)
	{
		px->Error0 = (float)atof(k230_Serial_RxPacket);
		k230_Serial_RxFlag = 0;
		Count = 0;
		return 1U;
	}
	//若长时间未接收到误差值，则误差值返回0
	else if(k230_Serial_RxFlag == 0)	
	{
		switch(Count)
		{
			case (0):
			Last_NTB = get_time_stamp_ms();
			Count = 1;
			break;
			
			case(1):
			if((get_time_stamp_ms() - Last_NTB)>2000)
			{
				px->Error0 = 0;
				Count = 0;
			}
			break;
			
			default: break;
			
		}
		
	}

	return 0U;
}
    
