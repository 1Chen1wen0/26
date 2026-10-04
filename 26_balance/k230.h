#ifndef __K230_H_
#define __K230_H_
#include "Pid.h"
#include <stdint.h>
#include "ti_msp_dl_config.h"

#define K230_DMA_RX_SIZE 32U
#define K230_PACKET_SIZE 30U

void K230_Init(void);
uint8_t Get_VisualError(PID_t *px);

extern volatile uint8_t k230_Serial_RxFlag;
extern char k230_Serial_RxPacket[K230_PACKET_SIZE];

#endif
