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
#include "ntb_time.h"
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
	//24h进入弯道过早时情况
	else if(L3 && L2 && L1 && !Mid && !R1 && !R2 && !R3)
	{
		return -6*weight;
	}
	else if(!L3 && !L2 && !L1 && !Mid && R1 && R2 && R3)
	{
		return 6*weight;
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


//找到黑色
uint8_t find_track()
{
	static uint32_t time_ntb = 0; 
    if(!L3 || !L2 || !L1 || !Mid || !R1 || !R2 || !R3)
    {
		time_ntb = get_time_stamp_ms();
			if((!L3 || !L2 || !L1 || !Mid || !R1 || !R2 || !R3) && (get_time_stamp_ms() - time_ntb>50))
			{
				return 1;
			}
			else
			{
				return 0;
			}
		
    }

	return 0;
}


//8路寻迹判断黑色消失时为进入下一阶段还是脱轨
int8_t Judge_track()//判断出界还是寻迹结束，左出界返回1，右出界返回-1，寻迹结束返回2,
{
    static uint8_t Pre_L3 = 1,Pre_Mid = 1,Pre_R3 = 1,Pre_R2 = 1,Pre_R1 = 1,Pre_L2 =1,Pre_L1 =11;

			Pre_R3  = R3;
			Pre_R2  = R2;
			Pre_R1  = R1;
			Pre_Mid  = Mid;
			Pre_L1  = L1;
			Pre_L2  = L2;
			Pre_L3  = L3;
		

	if(!find_track())
	{
		//delay_ms(50);

			if(Pre_L3 == 0 && L3 == 1)
			{
				return -1;
			}
			else if(Pre_R3 == 0 && R3 == 1)
			{
				return 1;
			}
//			//灯光不合适时L1，R1识别到，L3，R3会同时识别到
//			if((Pre_L3 == 0 && L3 == 1) && Pre_L1 == 0)
//			{
//				return 2;
//			}
//			else if((Pre_R3 == 0 && R3 == 1) && Pre_R1 == 0)
//			{
//				return 2;
//			}	
			else if((Pre_L2 == 0 && L2 == 1) || (Pre_L1 == 0 && L1 == 1) || (Pre_Mid == 0 && Mid == 1) || (Pre_R1 == 0 && R1 == 1) || (Pre_R2 == 0 && R2 == 1))
			{
				return 2;
			}

			else
			{
				return 0;
			}
		
	}
	return 0;
}

//TIMG12定时器可用时间戳
int8_t Find_CrossRoad(void)
{
    // 静态变量：函数调用期间值持续保留
    static uint8_t cross_state = 0;    // 0空闲直道 1十字等待驶出
    static uint32_t time_ntb = 0;     // 进入十字时刻时间戳

    uint8_t cur_total = Total_TrackPins;

    switch (cross_state)
    {
        case 0:
            // 检测到铺满黑线，判定驶入十字路口
            if (cur_total >= 6)
            {
                cross_state = 1;
                time_ntb = get_time_stamp_ms();
            }
            break;

        case 1:
            // 条件1：黑线变少 + 满足消抖时间 → 成功走完十字路口
            if (cur_total < 4)
            {
                if ((get_time_stamp_ms() - time_ntb) > 80)
                {
                    cross_state = 0;
                    time_ntb = 0U;
                    return 1;
                }
            }
            // 条件2：又重新铺满黑线 → 宽大弯道，不是十字，重置计时
            else if (cur_total >= 6)
            {
                time_ntb = get_time_stamp_ms();
            }
            // 条件3：长时间无法驶出，超时强制复位，防止状态卡死
            else if ((get_time_stamp_ms() - time_ntb) > 500)
            {
                cross_state = 0;
                time_ntb = 0U;
            }
            break;
    }

    return 0;
}


int8_t Find_RightAngle(void)
{
    // 状态定义
    // 0:空闲直道  1:左直角弯道等待驶出  2:右直角弯道等待驶出
    static uint8_t cross_state = 0;
    // 计时时间戳 静态保存
    static uint32_t time_ntb = 0;

    uint8_t cur_total = Total_TrackPins;

    switch(cross_state)
    {
        case 0:
            // 条件：总触发4~5个 + 最左边L3无黑线 → 左直角弯驶入
            if(cur_total >= 4 && cur_total < 6 && L3 == 0)
            {
                cross_state = 1;
                time_ntb = get_time_stamp_ms(); // 记录进入时刻
            }
            // 条件：总触发4~5个 + 最右边R3无黑线 → 右直角弯驶入
            else if(cur_total >= 4 && cur_total < 6 && R3 == 0)
            {
                cross_state = 2;
                time_ntb = get_time_stamp_ms();
            }
            break;

        case 1: // 左直角 等待驶出+消抖延时100ms
            if(cur_total < 3)
            {
                // 满足消抖时间才判定有效
                if((get_time_stamp_ms() - time_ntb) > 80)
                {
                    cross_state = 0;
                    time_ntb = 0;
                    return -1;  // 返回-1：左直角识别完成
                }
            }
            // 又重新回到大量黑线，属于宽大弯道干扰，重置计时
            else if(cur_total >= 4)
            {
                time_ntb = get_time_stamp_ms();
            }
            // 长时间走不完弯道，超时强制复位，防止状态卡死
            else if((get_time_stamp_ms() - time_ntb) > 500)
            {
                cross_state = 0;
                time_ntb = 0;
            }
            break;

        case 2: // 右直角 等待驶出+消抖延时100ms
            if(cur_total < 3)
            {
                if((get_time_stamp_ms() - time_ntb) > 80)
                {
                    cross_state = 0;
                    time_ntb = 0;
                    return 1;   // 返回1：右直角识别完成
                }
            }
            else if(cur_total >= 4)
            {
                time_ntb = get_time_stamp_ms();
            }
            else if((get_time_stamp_ms() - time_ntb) > 500)
            {
                cross_state = 0;
                time_ntb = 0;
            }
            break;
    }

    return 0; // 直道，无弯道
}

