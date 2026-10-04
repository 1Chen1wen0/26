#ifndef	__TRACK_H__
#define __TRACK_H__

#include "ti_msp_dl_config.h"
#define L3	(!DL_GPIO_readPins(Track_Pin_L3_PORT, Track_Pin_L3_PIN))
#define L2	(!DL_GPIO_readPins(Track_Pin_L2_PORT, Track_Pin_L2_PIN))
#define L1	(!DL_GPIO_readPins(Track_Pin_L1_PORT, Track_Pin_L1_PIN))
#define L4	(!DL_GPIO_readPins(Track_Pin_L4_PORT, Track_Pin_L4_PIN))
#define R1	(!DL_GPIO_readPins(Track_Pin_R1_PORT, Track_Pin_R1_PIN))
#define R2	(!DL_GPIO_readPins(Track_Pin_R2_PORT, Track_Pin_R2_PIN))
#define R3	(!DL_GPIO_readPins(Track_Pin_R3_PORT, Track_Pin_R3_PIN))
#define R4	(!DL_GPIO_readPins(Track_Pin_R4_PORT, Track_Pin_R4_PIN))
float track_scan(float weight);
uint8_t find_track();
int8_t Judge_track();
int8_t Find_CrossRoad(void);
int8_t Find_RightAngle(void);
extern uint8_t Total_TrackPins;
#endif