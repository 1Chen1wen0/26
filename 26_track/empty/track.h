#ifndef	__TRACK_H__
#define __TRACK_H__

#include "ti_msp_dl_config.h"
#define L3	(!DL_GPIO_readPins(Track_Pin_L3_PORT, Track_Pin_L3_PIN))
#define L2	(!DL_GPIO_readPins(Track_Pin_L2_PORT, Track_Pin_L2_PIN))
#define L1	(!DL_GPIO_readPins(Track_Pin_L1_PORT, Track_Pin_L1_PIN))
#define Mid	(!DL_GPIO_readPins(Track_Pin_Mid_PORT, Track_Pin_Mid_PIN))
#define R1	(!DL_GPIO_readPins(Track_Pin_R1_PORT, Track_Pin_R1_PIN))
#define R2	(!DL_GPIO_readPins(Track_Pin_R2_PORT, Track_Pin_R2_PIN))
#define R3	(!DL_GPIO_readPins(Track_Pin_R3_PORT, Track_Pin_R3_PIN))

float track_scan(float weight);
int8_t Find_CrossRoad(void);
extern uint8_t Total_TrackPins;
#endif