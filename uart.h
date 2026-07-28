#ifndef UART_H
#define UART_H

#include "ti_msp_dl_config.h"
#include <stdio.h>
#include "Pid.h"
extern char Serial_RxPacket[];
extern uint8_t Serial_RxFlag;
extern uint8_t ByteRecv;
void hhSerialSendByte(UART_Regs *uart, const uint8_t chr);
void hhSerialSendString(UART_Regs *uart, const char *str);
void hhSerialSendArray(UART_Regs *uart, uint8_t *Array,uint16_t Length);
uint32_t Serial_Pow(UART_Regs *uart,uint32_t X, uint32_t Y);
void hhSerial_SendNumber(UART_Regs *uart,uint32_t Number, uint8_t Length);
uint8_t Serial_GetRxFlag(void);
 void Serial_Printf(UART_Regs *uart,char *format, ...);
void CallbackAnalysis(PID_t *p);
#endif /* UART_H */
