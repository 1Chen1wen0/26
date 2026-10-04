#include "Delay.h"

void Delay_init(void)
{
		// 滴答定时器初始化
	SYSCFG_DL_init();


}

void Delay_us(unsigned long __us)
{
	 uint32_t ticks;
    uint32_t told, tnow, tcnt = 38;

    // 计算需要的时钟数 = 延迟微秒数 * 每微秒的时钟数
    ticks = __us * (32000000 / 1000000);

    // 获取当前的SysTick值
    told = SysTick->VAL;

    while (1)
    {
        // 重复刷新获取当前的SysTick值
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;

            told = tnow;

            // 如果达到了需要的时钟数，就退出循环
            if (tcnt >= ticks)
                break;
        }
    }

}
void Delay_ms(unsigned long ms) 
{
	Delay_us( ms * 1000 );
}

	void Delay_1us(unsigned long __us)
{
		Delay_us(__us);
	}
	void Delay_1ms(unsigned long ms)
	{
			Delay_ms(ms); 
	}