/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000



/* Defines for Motor_Control */
#define Motor_Control_INST                                                 TIMA1
#define Motor_Control_INST_IRQHandler                           TIMA1_IRQHandler
#define Motor_Control_INST_INT_IRQN                             (TIMA1_INT_IRQn)
#define Motor_Control_INST_CLK_FREQ                                     10000000
/* GPIO defines for channel 0 */
#define GPIO_Motor_Control_C0_PORT                                         GPIOB
#define GPIO_Motor_Control_C0_PIN                                  DL_GPIO_PIN_0
#define GPIO_Motor_Control_C0_IOMUX                              (IOMUX_PINCM12)
#define GPIO_Motor_Control_C0_IOMUX_FUNC             IOMUX_PINCM12_PF_TIMA1_CCP0
#define GPIO_Motor_Control_C0_IDX                            DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_Motor_Control_C1_PORT                                         GPIOB
#define GPIO_Motor_Control_C1_PIN                                  DL_GPIO_PIN_1
#define GPIO_Motor_Control_C1_IOMUX                              (IOMUX_PINCM13)
#define GPIO_Motor_Control_C1_IOMUX_FUNC             IOMUX_PINCM13_PF_TIMA1_CCP1
#define GPIO_Motor_Control_C1_IDX                            DL_TIMER_CC_1_INDEX

/* Defines for Servo */
#define Servo_INST                                                         TIMA0
#define Servo_INST_IRQHandler                                   TIMA0_IRQHandler
#define Servo_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define Servo_INST_CLK_FREQ                                               100000
/* GPIO defines for channel 0 */
#define GPIO_Servo_C0_PORT                                                 GPIOB
#define GPIO_Servo_C0_PIN                                          DL_GPIO_PIN_8
#define GPIO_Servo_C0_IOMUX                                      (IOMUX_PINCM25)
#define GPIO_Servo_C0_IOMUX_FUNC                     IOMUX_PINCM25_PF_TIMA0_CCP0
#define GPIO_Servo_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_Servo_C1_PORT                                                 GPIOB
#define GPIO_Servo_C1_PIN                                          DL_GPIO_PIN_9
#define GPIO_Servo_C1_IOMUX                                      (IOMUX_PINCM26)
#define GPIO_Servo_C1_IOMUX_FUNC                     IOMUX_PINCM26_PF_TIMA0_CCP1
#define GPIO_Servo_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                           (199U)
/* Defines for NTB */
#define NTB_INST                                                        (TIMG12)
#define NTB_INST_IRQHandler                                    TIMG12_IRQHandler
#define NTB_INST_INT_IRQN                                      (TIMG12_INT_IRQn)
#define NTB_INST_LOAD_VALUE                                        (2999999999U)




/* Defines for OLED */
#define OLED_INST                                                           I2C0
#define OLED_INST_IRQHandler                                     I2C0_IRQHandler
#define OLED_INST_INT_IRQN                                         I2C0_INT_IRQn
#define OLED_BUS_SPEED_HZ                                                 100000
#define GPIO_OLED_SDA_PORT                                                 GPIOA
#define GPIO_OLED_SDA_PIN                                         DL_GPIO_PIN_28
#define GPIO_OLED_IOMUX_SDA                                       (IOMUX_PINCM3)
#define GPIO_OLED_IOMUX_SDA_FUNC                        IOMUX_PINCM3_PF_I2C0_SDA
#define GPIO_OLED_SCL_PORT                                                 GPIOA
#define GPIO_OLED_SCL_PIN                                         DL_GPIO_PIN_31
#define GPIO_OLED_IOMUX_SCL                                       (IOMUX_PINCM6)
#define GPIO_OLED_IOMUX_SCL_FUNC                        IOMUX_PINCM6_PF_I2C0_SCL

/* Defines for Jy61p */
#define Jy61p_INST                                                          I2C1
#define Jy61p_INST_IRQHandler                                    I2C1_IRQHandler
#define Jy61p_INST_INT_IRQN                                        I2C1_INT_IRQn
#define GPIO_Jy61p_SDA_PORT                                                GPIOA
#define GPIO_Jy61p_SDA_PIN                                        DL_GPIO_PIN_10
#define GPIO_Jy61p_IOMUX_SDA                                     (IOMUX_PINCM21)
#define GPIO_Jy61p_IOMUX_SDA_FUNC                      IOMUX_PINCM21_PF_I2C1_SDA
#define GPIO_Jy61p_SCL_PORT                                                GPIOA
#define GPIO_Jy61p_SCL_PIN                                        DL_GPIO_PIN_11
#define GPIO_Jy61p_IOMUX_SCL                                     (IOMUX_PINCM22)
#define GPIO_Jy61p_IOMUX_SCL_FUNC                      IOMUX_PINCM22_PF_I2C1_SCL


/* Defines for k230 */
#define k230_INST                                                          UART2
#define k230_INST_FREQUENCY                                             40000000
#define k230_INST_IRQHandler                                    UART2_IRQHandler
#define k230_INST_INT_IRQN                                        UART2_INT_IRQn
#define GPIO_k230_RX_PORT                                                  GPIOB
#define GPIO_k230_TX_PORT                                                  GPIOB
#define GPIO_k230_RX_PIN                                          DL_GPIO_PIN_16
#define GPIO_k230_TX_PIN                                          DL_GPIO_PIN_15
#define GPIO_k230_IOMUX_RX                                       (IOMUX_PINCM33)
#define GPIO_k230_IOMUX_TX                                       (IOMUX_PINCM32)
#define GPIO_k230_IOMUX_RX_FUNC                        IOMUX_PINCM33_PF_UART2_RX
#define GPIO_k230_IOMUX_TX_FUNC                        IOMUX_PINCM32_PF_UART2_TX
#define k230_BAUD_RATE                                                  (115200)
#define k230_IBRD_40_MHZ_115200_BAUD                                        (21)
#define k230_FBRD_40_MHZ_115200_BAUD                                        (45)
/* Defines for BLE */
#define BLE_INST                                                           UART0
#define BLE_INST_FREQUENCY                                              40000000
#define BLE_INST_IRQHandler                                     UART0_IRQHandler
#define BLE_INST_INT_IRQN                                         UART0_INT_IRQn
#define GPIO_BLE_RX_PORT                                                   GPIOA
#define GPIO_BLE_TX_PORT                                                   GPIOA
#define GPIO_BLE_RX_PIN                                            DL_GPIO_PIN_1
#define GPIO_BLE_TX_PIN                                            DL_GPIO_PIN_0
#define GPIO_BLE_IOMUX_RX                                         (IOMUX_PINCM2)
#define GPIO_BLE_IOMUX_TX                                         (IOMUX_PINCM1)
#define GPIO_BLE_IOMUX_RX_FUNC                          IOMUX_PINCM2_PF_UART0_RX
#define GPIO_BLE_IOMUX_TX_FUNC                          IOMUX_PINCM1_PF_UART0_TX
#define BLE_BAUD_RATE                                                     (9600)
#define BLE_IBRD_40_MHZ_9600_BAUD                                          (260)
#define BLE_FBRD_40_MHZ_9600_BAUD                                           (27)
/* Defines for jy61p */
#define jy61p_INST                                                         UART1
#define jy61p_INST_FREQUENCY                                            40000000
#define jy61p_INST_IRQHandler                                   UART1_IRQHandler
#define jy61p_INST_INT_IRQN                                       UART1_INT_IRQn
#define GPIO_jy61p_RX_PORT                                                 GPIOA
#define GPIO_jy61p_TX_PORT                                                 GPIOA
#define GPIO_jy61p_RX_PIN                                          DL_GPIO_PIN_9
#define GPIO_jy61p_TX_PIN                                          DL_GPIO_PIN_8
#define GPIO_jy61p_IOMUX_RX                                      (IOMUX_PINCM20)
#define GPIO_jy61p_IOMUX_TX                                      (IOMUX_PINCM19)
#define GPIO_jy61p_IOMUX_RX_FUNC                       IOMUX_PINCM20_PF_UART1_RX
#define GPIO_jy61p_IOMUX_TX_FUNC                       IOMUX_PINCM19_PF_UART1_TX
#define jy61p_BAUD_RATE                                                 (115200)
#define jy61p_IBRD_40_MHZ_115200_BAUD                                       (21)
#define jy61p_FBRD_40_MHZ_115200_BAUD                                       (45)





/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (0)
#define jy61p_INST_DMA_TRIGGER                               (DMA_UART1_RX_TRIG)


/* Port definition for Pin Group beep */
#define beep_PORT                                                        (GPIOB)

/* Defines for LED_: GPIOB.4 with pinCMx 17 on package pin 52 */
#define beep_LED__PIN                                            (DL_GPIO_PIN_4)
#define beep_LED__IOMUX                                          (IOMUX_PINCM17)
/* Port definition for Pin Group Warning */
#define Warning_PORT                                                     (GPIOB)

/* Defines for LED: GPIOB.5 with pinCMx 18 on package pin 53 */
#define Warning_LED_PIN                                          (DL_GPIO_PIN_5)
#define Warning_LED_IOMUX                                        (IOMUX_PINCM18)
/* Defines for Key_1: GPIOA.22 with pinCMx 47 on package pin 18 */
#define Key_Key_1_PORT                                                   (GPIOA)
#define Key_Key_1_PIN                                           (DL_GPIO_PIN_22)
#define Key_Key_1_IOMUX                                          (IOMUX_PINCM47)
/* Defines for Key_2: GPIOB.24 with pinCMx 52 on package pin 23 */
#define Key_Key_2_PORT                                                   (GPIOB)
#define Key_Key_2_PIN                                           (DL_GPIO_PIN_24)
#define Key_Key_2_IOMUX                                          (IOMUX_PINCM52)
/* Defines for Key_3: GPIOA.15 with pinCMx 37 on package pin 8 */
#define Key_Key_3_PORT                                                   (GPIOA)
#define Key_Key_3_PIN                                           (DL_GPIO_PIN_15)
#define Key_Key_3_IOMUX                                          (IOMUX_PINCM37)
/* Defines for Key_4: GPIOA.17 with pinCMx 39 on package pin 10 */
#define Key_Key_4_PORT                                                   (GPIOA)
#define Key_Key_4_PIN                                           (DL_GPIO_PIN_17)
#define Key_Key_4_IOMUX                                          (IOMUX_PINCM39)
/* Defines for L3: GPIOB.19 with pinCMx 45 on package pin 16 */
#define Track_Pin_L3_PORT                                                (GPIOB)
#define Track_Pin_L3_PIN                                        (DL_GPIO_PIN_19)
#define Track_Pin_L3_IOMUX                                       (IOMUX_PINCM45)
/* Defines for L2: GPIOB.17 with pinCMx 43 on package pin 14 */
#define Track_Pin_L2_PORT                                                (GPIOB)
#define Track_Pin_L2_PIN                                        (DL_GPIO_PIN_17)
#define Track_Pin_L2_IOMUX                                       (IOMUX_PINCM43)
/* Defines for L1: GPIOA.16 with pinCMx 38 on package pin 9 */
#define Track_Pin_L1_PORT                                                (GPIOA)
#define Track_Pin_L1_PIN                                        (DL_GPIO_PIN_16)
#define Track_Pin_L1_IOMUX                                       (IOMUX_PINCM38)
/* Defines for Mid: GPIOA.14 with pinCMx 36 on package pin 7 */
#define Track_Pin_Mid_PORT                                               (GPIOA)
#define Track_Pin_Mid_PIN                                       (DL_GPIO_PIN_14)
#define Track_Pin_Mid_IOMUX                                      (IOMUX_PINCM36)
/* Defines for R1: GPIOB.20 with pinCMx 48 on package pin 19 */
#define Track_Pin_R1_PORT                                                (GPIOB)
#define Track_Pin_R1_PIN                                        (DL_GPIO_PIN_20)
#define Track_Pin_R1_IOMUX                                       (IOMUX_PINCM48)
/* Defines for R2: GPIOB.25 with pinCMx 56 on package pin 27 */
#define Track_Pin_R2_PORT                                                (GPIOB)
#define Track_Pin_R2_PIN                                        (DL_GPIO_PIN_25)
#define Track_Pin_R2_IOMUX                                       (IOMUX_PINCM56)
/* Defines for R3: GPIOA.25 with pinCMx 55 on package pin 26 */
#define Track_Pin_R3_PORT                                                (GPIOA)
#define Track_Pin_R3_PIN                                        (DL_GPIO_PIN_25)
#define Track_Pin_R3_IOMUX                                       (IOMUX_PINCM55)
/* Defines for PIN_2: GPIOA.27 with pinCMx 60 on package pin 31 */
#define Track_Pin_PIN_2_PORT                                             (GPIOA)
#define Track_Pin_PIN_2_PIN                                     (DL_GPIO_PIN_27)
#define Track_Pin_PIN_2_IOMUX                                    (IOMUX_PINCM60)
/* Defines for Ain_2: GPIOA.30 with pinCMx 5 on package pin 37 */
#define Motor_Ain_2_PORT                                                 (GPIOA)
#define Motor_Ain_2_PIN                                         (DL_GPIO_PIN_30)
#define Motor_Ain_2_IOMUX                                         (IOMUX_PINCM5)
/* Defines for Ain_1: GPIOB.21 with pinCMx 49 on package pin 20 */
#define Motor_Ain_1_PORT                                                 (GPIOB)
#define Motor_Ain_1_PIN                                         (DL_GPIO_PIN_21)
#define Motor_Ain_1_IOMUX                                        (IOMUX_PINCM49)
/* Defines for Bin_1: GPIOB.14 with pinCMx 31 on package pin 2 */
#define Motor_Bin_1_PORT                                                 (GPIOB)
#define Motor_Bin_1_PIN                                         (DL_GPIO_PIN_14)
#define Motor_Bin_1_IOMUX                                        (IOMUX_PINCM31)
/* Defines for Bin_2: GPIOA.7 with pinCMx 14 on package pin 49 */
#define Motor_Bin_2_PORT                                                 (GPIOA)
#define Motor_Bin_2_PIN                                          (DL_GPIO_PIN_7)
#define Motor_Bin_2_IOMUX                                        (IOMUX_PINCM14)
/* Defines for E2A: GPIOB.10 with pinCMx 27 on package pin 62 */
#define Encoder_E2A_PORT                                                 (GPIOB)
// pins affected by this interrupt request:["E2A"]
#define Encoder_GPIOB_INT_IRQN                                  (GPIOB_INT_IRQn)
#define Encoder_GPIOB_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define Encoder_E2A_IIDX                                    (DL_GPIO_IIDX_DIO10)
#define Encoder_E2A_PIN                                         (DL_GPIO_PIN_10)
#define Encoder_E2A_IOMUX                                        (IOMUX_PINCM27)
/* Defines for E2B: GPIOB.11 with pinCMx 28 on package pin 63 */
#define Encoder_E2B_PORT                                                 (GPIOB)
#define Encoder_E2B_PIN                                         (DL_GPIO_PIN_11)
#define Encoder_E2B_IOMUX                                        (IOMUX_PINCM28)
/* Defines for E1A: GPIOA.26 with pinCMx 59 on package pin 30 */
#define Encoder_E1A_PORT                                                 (GPIOA)
// pins affected by this interrupt request:["E1A"]
#define Encoder_GPIOA_INT_IRQN                                  (GPIOA_INT_IRQn)
#define Encoder_GPIOA_INT_IIDX                  (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define Encoder_E1A_IIDX                                    (DL_GPIO_IIDX_DIO26)
#define Encoder_E1A_PIN                                         (DL_GPIO_PIN_26)
#define Encoder_E1A_IOMUX                                        (IOMUX_PINCM59)
/* Defines for E1B: GPIOA.24 with pinCMx 54 on package pin 25 */
#define Encoder_E1B_PORT                                                 (GPIOA)
#define Encoder_E1B_PIN                                         (DL_GPIO_PIN_24)
#define Encoder_E1B_IOMUX                                        (IOMUX_PINCM54)
/* Port definition for Pin Group Magnant */
#define Magnant_PORT                                                     (GPIOB)

/* Defines for PIN_0: GPIOB.12 with pinCMx 29 on package pin 64 */
#define Magnant_PIN_0_PIN                                       (DL_GPIO_PIN_12)
#define Magnant_PIN_0_IOMUX                                      (IOMUX_PINCM29)
/* Defines for PIN_1: GPIOB.13 with pinCMx 30 on package pin 1 */
#define Magnant_PIN_1_PIN                                       (DL_GPIO_PIN_13)
#define Magnant_PIN_1_IOMUX                                      (IOMUX_PINCM30)



/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_Motor_Control_init(void);
void SYSCFG_DL_Servo_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_NTB_init(void);
void SYSCFG_DL_OLED_init(void);
void SYSCFG_DL_Jy61p_init(void);
void SYSCFG_DL_k230_init(void);
void SYSCFG_DL_BLE_init(void);
void SYSCFG_DL_jy61p_init(void);
void SYSCFG_DL_DMA_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
