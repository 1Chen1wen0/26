#include "ti_msp_dl_config.h"
#include "Delay.h"
#include "jy61p.h"
// 定义接收变量
//uint8_t RollL, RollH, PitchL, PitchH, YawL, YawH, VL, VH, SUM;
//float Pitch,Roll,Yaw;
//// 串口接收状态标识
//#define WAIT_HEADER1 0
//#define WAIT_HEADER2 1
//#define RECEIVE_DATA 2

//uint8_t RxState = WAIT_HEADER1;
//uint8_t receivedData[9];
//uint8_t dataIndex = 0;



//发送置偏航角置零命令
void Serial_JY61P_Zero_Roll_Pitch(void){
    DL_UART_Main_transmitDataBlocking(jy61p_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X69);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X88);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XB5);
	delay_ms(100);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X01);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X08);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X00);
	delay_ms(100);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X00);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X00);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X00);
}
void Serial_JY61P_Zero_Yaw(void){
    DL_UART_Main_transmitDataBlocking(jy61p_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X69);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X88);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XB5);
	delay_ms(100);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X01);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X04);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X00);
	delay_ms(100);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XFF);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0XAA);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X00);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X00);
	DL_UART_Main_transmitDataBlocking(jy61p_INST,0X00);
}




uint8_t wit_dmaBuffer[33];

WIT_Data_t wit_data;

void jy61p_INST_IRQHandler(void)
{
    uint8_t checkSum, packCnt = 0;
    extern uint8_t wit_dmaBuffer[33];

    DL_DMA_disableChannel(DMA, DMA_CH0_CHAN_ID);
    uint8_t rxSize = 32 - DL_DMA_getTransferSize(DMA, DMA_CH0_CHAN_ID);

    if(DL_UART_isRXFIFOEmpty(jy61p_INST) == false)
        wit_dmaBuffer[rxSize++] = DL_UART_receiveData(jy61p_INST);

    while(rxSize >= 11)
    {
        checkSum=0;
        for(int i=packCnt*11; i<(packCnt+1)*11-1; i++)
            checkSum += wit_dmaBuffer[i];

        if((wit_dmaBuffer[packCnt*11] == 0x55) && (checkSum == wit_dmaBuffer[packCnt*11+10]))
        {
            if(wit_dmaBuffer[packCnt*11+1] == 0x51)
            {
                wit_data.ax = (int16_t)((wit_dmaBuffer[packCnt*11+3]<<8)|wit_dmaBuffer[packCnt*11+2]) / 2.048; //mg
                wit_data.ay = (int16_t)((wit_dmaBuffer[packCnt*11+5]<<8)|wit_dmaBuffer[packCnt*11+4]) / 2.048; //mg
                wit_data.az = (int16_t)((wit_dmaBuffer[packCnt*11+7]<<8)|wit_dmaBuffer[packCnt*11+6]) / 2.048; //mg
                wit_data.temperature =  (int16_t)((wit_dmaBuffer[packCnt*11+9]<<8)|wit_dmaBuffer[packCnt*11+8]) / 100.0; //°C
            }
            else if(wit_dmaBuffer[packCnt*11+1] == 0x52)
            {
                wit_data.gx = (int16_t)((wit_dmaBuffer[packCnt*11+3]<<8)|wit_dmaBuffer[packCnt*11+2]) / 16.384; //°/S
                wit_data.gy = (int16_t)((wit_dmaBuffer[packCnt*11+5]<<8)|wit_dmaBuffer[packCnt*11+4]) / 16.384; //°/S
                wit_data.gz = (int16_t)((wit_dmaBuffer[packCnt*11+7]<<8)|wit_dmaBuffer[packCnt*11+6]) / 16.384; //°/S
            }
            else if(wit_dmaBuffer[packCnt*11+1] == 0x53)
            {
                wit_data.roll  = (int16_t)((wit_dmaBuffer[packCnt*11+3]<<8)|wit_dmaBuffer[packCnt*11+2]) / 32768.0 * 180.0; //°
                wit_data.pitch = (int16_t)((wit_dmaBuffer[packCnt*11+5]<<8)|wit_dmaBuffer[packCnt*11+4]) / 32768.0 * 180.0; //°
                wit_data.yaw   = (int16_t)((wit_dmaBuffer[packCnt*11+7]<<8)|wit_dmaBuffer[packCnt*11+6]) / 32768.0 * 180.0; //°
                wit_data.version = (int16_t)((wit_dmaBuffer[packCnt*11+9]<<8)|wit_dmaBuffer[packCnt*11+8]);
            }
        }

        rxSize -= 11;
        packCnt++;
    }
	    uint8_t dummy[4];
    DL_UART_drainRXFIFO(jy61p_INST, dummy, 4);

    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &wit_dmaBuffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, 32);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);
}

void WIT_Init(void)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)(&jy61p_INST->RXDATA));
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t)&wit_dmaBuffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, 32);
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID);

    NVIC_EnableIRQ(jy61p_INST_INT_IRQN);
}
