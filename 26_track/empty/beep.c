#include "ti_msp_dl_config.h"
#include "Delay.h"
#include "oled.h"
#include <stdio.h>
#include "uart.h"
#include "step_motor.h"
/*
void Warning_Init(void)
{
	DL_GPIO_setPins(GPIOA,DL_GPIO_PIN_11);
    DL_GPIO_setPins(GPIOB, Warning_LED_PIN);
}

void warning(void)
{
    DL_GPIO_clearPins(GPIOA,DL_GPIO_PIN_11);
    DL_GPIO_clearPins(GPIOB, Warning_LED_PIN);
    delay_ms(20);
    DL_GPIO_setPins(GPIOA,DL_GPIO_PIN_11);
    DL_GPIO_setPins(GPIOB, Warning_LED_PIN);
}
*/