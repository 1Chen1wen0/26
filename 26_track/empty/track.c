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
#include "ntb_time.h"

typedef enum
{
    CROSSROAD_STATE_IDLE = 0,
    CROSSROAD_STATE_WAITING_FOR_EXIT
} CrossroadState_t;

uint8_t Total_TrackPins;
//扫描灰度传感器，左侧为正方向，根据位置分配比重
float track_scan(float weight)
{
    //原：L3 && !L2 && !L1 && !Mid && !R1 && !R2 && !R3
    if(!L3 && L2 && L1 && Mid && R1 && R2 && R3)
    {
        return -13*weight;
    }
    //原：L3 && L2 && !L1 && !Mid && !R1 && !R2 && !R3
    else if(!L3 && !L2 && L1 && Mid && R1 && R2 && R3)
    {
        return -11*weight;
    }
    //原：L3 && L2 && L1 && !Mid && !R1 && !R2 && !R3
    else if(!L3 && !L2 && !L1 && Mid && R1 && R2 && R3)
    {
        return -10*weight;
    }
    //原：!L3 && L2 && !L1 && !Mid && !R1 && !R2 && !R3
    else if(L3 && !L2 && L1 && Mid && R1 && R2 && R3)
    {
        return -8*weight;
    }
    //原：!L3 && L2 && L1 && !Mid && !R1 && !R2 && !R3
    else if(L3 && !L2 && !L1 && Mid && R1 && R2 && R3)
    {
        return -6*weight;
    }
    //原重复错误分支已修正，原：!L3 && !L2 && L1 && !Mid && !R1 && !R2 && !R3
    else if(L3 && L2 && !L1 && Mid && R1 && R2 && R3)
    {
        return -5*weight;
    }
    //原：!L3 && !L2 && L1 && Mid && !R1 && !R2 && !R3
    else if(L3 && L2 && !L1 && !Mid && R1 && R2 && R3)
    {
        return -4*weight;
    }
    //原：!L3 && !L2 && !L1 && Mid && !R1 && !R2 && !R3
    else if(L3 && L2 && L1 && !Mid && R1 && R2 && R3)
    {
        return 0;
    }
    //原：!L3 && !L2 && L1 && Mid && R1 && !R2 && !R3
    else if(L3 && L2 && !L1 && !Mid && !R1 && R2 && R3)
    {
        return 0;
    }
    //原：!L3 && !L2 && !L1 && Mid && R1 && !R2 && !R3
    else if(L3 && L2 && L1 && !Mid && !R1 && R2 && R3)
    {
        return 4*weight;
    }
    //原：!L3 && !L2 && !L1 && !Mid && R1 && !R2 && !R3
    else if(L3 && L2 && L1 && Mid && !R1 && R2 && R3)
    {
        return 5*weight;
    }
    //原：!L3 && !L2 && !L1 && !Mid && R1 && R2 && !R3
    else if(L3 && L2 && L1 && Mid && !R1 && !R2 && R3)
    {
        return 6*weight;
    }
    //原：!L3 && !L2 && !L1 && !Mid && !R1 && R2 && !R3
    else if(L3 && L2 && L1 && Mid && R1 && !R2 && R3)
    {
        return 8*weight;
    }
    //原：!L3 && !L2 && !L1 && !Mid && R1 && R2 && R3
    else if(L3 && L2 && L1 && Mid && !R1 && !R2 && !R3)
    {
        return 10*weight;
    }
    //原：!L3 && !L2 && !L1 && !Mid && !R1 && R2 && R3
    else if(L3 && L2 && L1 && Mid && R1 && !R2 && !R3)
    {
        return 11*weight;
    }
    //原：!L3 && !L2 && !L1 && !Mid && !R1 && !R2 && R3
    else if(L3 && L2 && L1 && Mid && R1 && R2 && !R3)
    {
        return 13*weight;
    }

	else if(L3 && L2 && L1 && Mid && R1 && R2 && R3)
    {
        return 0;
    }
    else
    {
        return 0;
    }
    //原全部传感器无黑线，取反后全部传感器都检测到黑线


}


//TIMG12定时器可用时间戳
int8_t Find_CrossRoad(void)
{
	
    // 静态变量：函数调用期间值持续保留
    static CrossroadState_t crossroad_state = CROSSROAD_STATE_IDLE;
    static uint32_t time_ntb = 0;     // 进入十字时刻时间戳

    uint8_t cur_total = Total_TrackPins;

    switch (crossroad_state)
    {
        case CROSSROAD_STATE_IDLE:
            // 检测到铺满黑线，判定驶入十字路口
            if (cur_total >= 4)
            {
                crossroad_state = CROSSROAD_STATE_WAITING_FOR_EXIT;
                time_ntb = get_time_stamp_ms();
            }
            break;

        case CROSSROAD_STATE_WAITING_FOR_EXIT:
            // 条件1：黑线变少 + 满足消抖时间 → 成功走完十字路口
            if (cur_total <= 2 && cur_total > 0)
            {
                if ((get_time_stamp_ms() - time_ntb) > 100)
                {
                    crossroad_state = CROSSROAD_STATE_IDLE;
                    time_ntb = 0;
                    return 1;
                }
            }
            // 条件2：又重新铺满黑线 → 宽大弯道，不是十字，重置计时
            else if (cur_total >= 4)
            {
                time_ntb = get_time_stamp_ms();
            }
            // 条件3：长时间无法驶出，超时强制复位，防止状态卡死
            else if ((get_time_stamp_ms() - time_ntb) > 500)
            {
                crossroad_state = CROSSROAD_STATE_IDLE;
                time_ntb = 0U;
            }
            break;
    }
return 0;
 

}



