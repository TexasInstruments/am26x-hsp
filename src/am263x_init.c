
/********************************************************************
 * Copyright (C) 2025 Texas Instruments Incorporated.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include <stdint.h>
#include <stdio.h>

#include "am263x_main.h"

#include "MW_target_hardware_resources.h"
#include <drivers/uart/v0/lld/dma/edma/uart_dma_edma.h>
#include <drivers/uart/v0/lld/uart_lld.h>
#include <drivers/soc.h>
#include <drivers/edma.h>
#include <drivers/mcan.h>
#include <drivers/i2c.h>
#include <drivers/mcspi.h>
#include <drivers/rti/v0/rti.h>
#include <drivers/pinmux.h>
#include "kernel/dpl/DebugP.h"
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/TimerP.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/MutexArmP.h>
#include <kernel/dpl/HwiP.h>
#include <drivers/ipc_notify.h>
#include <drivers/ipc_notify/v1/ipc_notify_v1.h>
#include <kernel/nortos/dpl/r5/HwiP_armv7r_vim.h>
#include<math.h>


#define MCAN_COUNT                               (4)
#define APP_MCAN_EXT_ID_MASK                     (0x1FFFFFFFU)
#define APP_MCAN_STD_ID_MASK                     (0x7FFU)
#define APP_MCAN_STD_ID_SHIFT                    (18U)

#define TCA6416_REG_INPUT0              ((UInt8) 0x00U)
#define TCA6416_REG_INPUT1              ((UInt8) 0x01U)

/* Output register to change state of output BIT set to 1, output set HIGH */
#define TCA6416_REG_OUTPUT0             ((uint8_t) 0x02U)
#define TCA6416_REG_OUTPUT1             ((uint8_t) 0x03U)

/* Configuration register. BIT = '1' sets port to input, BIT = '0' sets
 * port to output */
#define TCA6416_REG_CONFIG0             ((uint8_t) 0x06U)
#define TCA6416_REG_CONFIG1             ((uint8_t) 0x07U)

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

static void SetupI2CTransfer(I2C_Handle handle,  uint32_t targetAddr,
                      uint8_t *writeData, uint32_t numWriteBytes,
                      uint8_t *readData,  uint32_t numReadBytes);


/* ----------- HwiP ----------- */
HwiP_Config gHwiConfig = {
    .intcBaseAddr = 0x50F00000u,
};

/* ----------- DebugP ----------- */
void putchar_(char character)
{
    /* Output to CCS console */
    putchar(character);
    /* Output to UART console */
    DebugP_uartLogWriterPutChar(character);
}

void Drivers_EPWMSyncOutputXBAR();
#define RODATA_CFG_SECTION __attribute__((section(".rodata.cfg")))
/* ----------- CacheP ----------- */
const CacheP_Config gCacheConfig RODATA_CFG_SECTION = {
    .enable = 1,
    .enableForceWrThru = 0,
};

/* ----------- MpuP_armv7 ----------- */
#define CONFIG_MPU_NUM_REGIONS  (4u)

const MpuP_Config gMpuConfig RODATA_CFG_SECTION = {
    .numRegions = CONFIG_MPU_NUM_REGIONS,
    .enableBackgroundRegion = 0,
    .enableMpu = 1,
};

const MpuP_RegionConfig gMpuRegionConfig[CONFIG_MPU_NUM_REGIONS] RODATA_CFG_SECTION =
{
    {
        .baseAddr = 0x0u,
        .size = MpuP_RegionSize_2G,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 0,
            .isBufferable = 0,
            .isSharable = 1,
            .isExecuteNever = 1,
            .tex = 0,
            .accessPerm = MpuP_AP_S_RW_U_R,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0x0u,
        .size = MpuP_RegionSize_32K,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 1,
            .isBufferable = 1,
            .isSharable = 0,
            .isExecuteNever = 0,
            .tex = 1,
            .accessPerm = MpuP_AP_S_RW_U_R,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0x80000u,
        .size = MpuP_RegionSize_32K,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 1,
            .isBufferable = 1,
            .isSharable = 0,
            .isExecuteNever = 0,
            .tex = 1,
            .accessPerm = MpuP_AP_S_RW_U_R,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0x70000000u,
        .size = MpuP_RegionSize_2M,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 1,
            .isBufferable = 1,
            .isSharable = 0,
            .isExecuteNever = 0,
            .tex = 1,
            .accessPerm = MpuP_AP_S_RW_U_R,
            .subregionDisableMask = 0x0u
        },
    },
    {
        .baseAddr = 0x50000000u,
        .size = MpuP_RegionSize_1M,
        .attrs = {
            .isEnable = 1,
            .isCacheable = 0,
            .isBufferable = 0,
            .isSharable = 0,
            .isExecuteNever = 1,
            .tex = 1,
            .accessPerm = MpuP_AP_ALL_RW,
            .subregionDisableMask = 0x0u
        },
    },
};

/* ----------- TimerP ----------- */
#define CONFIG_TIMER0_CLOCK_SRC_MUX_ADDR (0x5320811Cu)
#define CONFIG_TIMER0_CLOCK_SRC_WUCPUCLK (0x0u)

#define CONFIG_TIMER0                  (0u)
#define CONFIG_TIMER0_BASE_ADDR        (0x52182000u)
#define CONFIG_TIMER0_INT_NUM          (98u)
#define CONFIG_TIMER0_INPUT_CLK_HZ     (25000000u)
#define CONFIG_TIMER0_INPUT_PRE_SCALER (1u)
/* This represents the required time period 'floored' to nearest usec */
#define CONFIG_TIMER0_USEC_PER_TICK    (1u)
/* This represents the required time period 'floored' to nearest nsec */
#define CONFIG_TIMER0_NSEC_PER_TICK    (1000u)
/* This represents the actual time period 'floored' to nearest nsec */
#define CONFIG_TIMER0_NSEC_PER_TICK_ACTUAL  (1000u)
/* NOTE: nsec per tick is used as input to the timer API */
/* NOTE: The actual nsec per tick that will be achieved depends on timer input clock hz precision */

#define TIMER_NUM_INSTANCES  (1u)

extern HwiP_Object gTimerHwiObj[TIMER_NUM_INSTANCES];
extern uint32_t gTimerBaseAddr[TIMER_NUM_INSTANCES];


HwiP_Object gTimerHwiObj[TIMER_NUM_INSTANCES];
uint32_t gTimerBaseAddr[TIMER_NUM_INSTANCES];


/* ----------- ClockP ----------- */
#define RTI0_CLOCK_SRC_MUX_ADDR (0x53208114u)
#define RTI0_CLOCK_SRC_WUCPUCLK (0x0u)
#define RTI0_BASE_ADDR     (0x52180000u)


ClockP_Config gClockConfig = {
    .timerBaseAddr = RTI0_BASE_ADDR, 
    .timerHwiIntNum = 84,
    .timerInputClkHz = 25000000,
    .timerInputPreScaler = 1,
    .usecPerTick = 1000,
    .intrPriority = 15,
};


#define BOOT_SECTION __attribute__((section(".text.boot")))

/* This function is called by _c_int00 */
void BOOT_SECTION __mpu_init() 
{
    MpuP_init();
    
    CacheP_init();
}

typedef struct Pinmux_Cfg{

    int16_t offset;

    uint32_t settings;

    uint8_t enable;
} PinmuxCfg;



/*
 * UART
 */


/* UART objects - initialized by the driver */
#define CONFIG_UART_NUM_LLD_INSTANCES 6
UARTLLD_Object gUartObject[CONFIG_UART_NUM_LLD_INSTANCES];
UARTLLD_Handle gUartHandleLld[CONFIG_UART_NUM_LLD_INSTANCES];
UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_LLD_INSTANCES];
UART_Config gUartConfig[CONFIG_UART_NUM_INSTANCES];
uint32_t gUartConfigNum = 0;
Pinmux_PerCfg_t gPinMuxMainDomainCfg_init[140];


void Drivers_uartClose(void)
{
  
}


static PinmuxCfg gPinMuxMainDomainCfg[] = {

    
    {PIN_QSPI_CSN0, ( PIN_MODE(MW_PIN_QSPI0_CSN0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX0_ENABLE_SEL},
    {PIN_QSPI_CSN1, ( PIN_MODE(MW_PIN_QSPI0_CSN1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX1_ENABLE_SEL },
    {PIN_QSPI_CLK, ( PIN_MODE(MW_PIN_QSPI0_CLK_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX2_ENABLE_SEL },
    {PIN_QSPI_D0, ( PIN_MODE(MW_PIN_QSPI0_D0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX3_ENABLE_SEL  },
    {PIN_QSPI_D1, ( PIN_MODE(MW_PIN_QSPI0_D1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX4_ENABLE_SEL  },
    {PIN_QSPI_D2, ( PIN_MODE(MW_PIN_QSPI0_D2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX5_ENABLE_SEL  },
    {PIN_QSPI_D3, ( PIN_MODE(MW_PIN_QSPI0_D3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX6_ENABLE_SEL  },
    {PIN_MCAN0_RX, ( PIN_MODE(MW_PIN_MCAN0_RX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX7_ENABLE_SEL  },
    {PIN_MCAN0_TX, ( PIN_MODE(MW_PIN_MCAN0_TX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX8_ENABLE_SEL  },
    {PIN_MCAN1_RX, ( PIN_MODE(MW_PIN_MCAN1_RX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX9_ENABLE_SEL  },
    {PIN_MCAN1_TX, ( PIN_MODE(MW_PIN_MCAN1_TX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX10_ENABLE_SEL  },
    {PIN_SPI0_CS0, ( PIN_MODE(MW_PIN_SPI0_CS0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX11_ENABLE_SEL  },
    {PIN_SPI0_CLK, ( PIN_MODE(MW_PIN_SPI0_CLK_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX12_ENABLE_SEL  },
    {PIN_SPI0_D0, ( PIN_MODE(MW_PIN_SPI0_D0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX13_ENABLE_SEL  },
    {PIN_SPI0_D1, ( PIN_MODE(MW_PIN_SPI0_D1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX14_ENABLE_SEL  },
    {PIN_SPI1_CS0, ( PIN_MODE(MW_PIN_SPI1_CS0_IOMUX) | PIN_PULL_UP | PIN_SLEW_RATE_LOW ), MW_PINMUX15_ENABLE_SEL  },
    {PIN_SPI1_CLK, ( PIN_MODE(MW_PIN_SPI1_CLK_IOMUX) | PIN_PULL_UP | PIN_SLEW_RATE_LOW ), MW_PINMUX16_ENABLE_SEL  },
    {PIN_SPI1_D0, ( PIN_MODE(MW_PIN_SPI1_D0_IOMUX) | PIN_PULL_UP | PIN_SLEW_RATE_LOW ), MW_PINMUX17_ENABLE_SEL  },
    {PIN_SPI1_D1, ( PIN_MODE(MW_PIN_SPI1_D1_IOMUX) | PIN_PULL_UP | PIN_SLEW_RATE_LOW ), MW_PINMUX18_ENABLE_SEL  },
    {PIN_LIN1_RXD, ( PIN_MODE(MW_PIN_LIN1_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX19_ENABLE_SEL  },
    {PIN_LIN1_TXD, ( PIN_MODE(MW_PIN_LIN1_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX20_ENABLE_SEL  },
    {PIN_LIN2_RXD, ( PIN_MODE(MW_PIN_LIN2_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX21_ENABLE_SEL  },
    {PIN_LIN2_TXD, ( PIN_MODE(MW_PIN_LIN2_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX22_ENABLE_SEL  },
    {PIN_I2C1_SCL, ( PIN_MODE(MW_PIN_I2C1_SCL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX23_ENABLE_SEL  },
    {PIN_I2C1_SDA, ( PIN_MODE(MW_PIN_I2C1_SDA_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX24_ENABLE_SEL  },
    {PIN_UART0_RTSN, ( PIN_MODE(MW_PIN_UART0_RTSN_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX25_ENABLE_SEL  },
    {PIN_UART0_CTSN, ( PIN_MODE(MW_PIN_UART0_CTSN_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX26_ENABLE_SEL  },
    {PIN_UART0_RXD, ( PIN_MODE(MW_PIN_UART0_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX27_ENABLE_SEL  },
    {PIN_UART0_TXD, ( PIN_MODE(MW_PIN_UART0_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX28_ENABLE_SEL  },
    {PIN_RGMII1_RXC, ( PIN_MODE(MW_PIN_RGMII1_RXC_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX29_ENABLE_SEL  },
    {PIN_RGMII1_RX_CTL, ( PIN_MODE(MW_PIN_RGMII1_RX_CTL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX30_ENABLE_SEL  },
    {PIN_RGMII1_RD0, ( PIN_MODE(MW_PIN_RGMII1_RD0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX31_ENABLE_SEL  },
    {PIN_RGMII1_RD1, ( PIN_MODE(MW_PIN_RGMII1_RD1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX32_ENABLE_SEL  },
    {PIN_RGMII1_RD2, ( PIN_MODE(MW_PIN_RGMII1_RD2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX33_ENABLE_SEL  },
    {PIN_RGMII1_RD3, ( PIN_MODE(MW_PIN_RGMII1_RD3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX34_ENABLE_SEL  },
    {PIN_RGMII1_TXC, ( PIN_MODE(MW_PIN_RGMII1_TXC_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX35_ENABLE_SEL  },
    {PIN_RGMII1_TX_CTL, ( PIN_MODE(MW_PIN_RGMII1_TX_CTL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX36_ENABLE_SEL  },
    {PIN_RGMII1_TD0, ( PIN_MODE(MW_PIN_RGMII1_TD0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX37_ENABLE_SEL  },
    {PIN_RGMII1_TD1, ( PIN_MODE(MW_PIN_RGMII1_TD1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX38_ENABLE_SEL  },
    {PIN_RGMII1_TD2, ( PIN_MODE(MW_PIN_RGMII1_TD2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX39_ENABLE_SEL  },
    {PIN_RGMII1_TD3, ( PIN_MODE(MW_PIN_RGMII1_TD3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX40_ENABLE_SEL  },
    {PIN_MDIO_MDIO, ( PIN_MODE(MW_PIN_MDIO0_MDIO_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX41_ENABLE_SEL  },
    {PIN_MDIO_MDC, ( PIN_MODE(MW_PIN_MDIO0_MDC_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX42_ENABLE_SEL  },
    {PIN_EPWM0_A, ( PIN_MODE(MW_PIN_EPWM0_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX43_ENABLE_SEL  },
    {PIN_EPWM0_B, ( PIN_MODE(MW_PIN_EPWM0_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX44_ENABLE_SEL  },
    {PIN_EPWM1_A, ( PIN_MODE(MW_PIN_EPWM1_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX45_ENABLE_SEL  },
    {PIN_EPWM1_B, ( PIN_MODE(MW_PIN_EPWM1_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX46_ENABLE_SEL  },
    {PIN_EPWM2_A, ( PIN_MODE(MW_PIN_EPWM2_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX47_ENABLE_SEL  },
    {PIN_EPWM2_B, ( PIN_MODE(MW_PIN_EPWM2_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX48_ENABLE_SEL  },
    {PIN_EPWM3_A, ( PIN_MODE(MW_PIN_EPWM3_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX49_ENABLE_SEL  },
    {PIN_EPWM3_B, ( PIN_MODE(MW_PIN_EPWM3_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX50_ENABLE_SEL  },
    {PIN_EPWM4_A, ( PIN_MODE(MW_PIN_EPWM4_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX51_ENABLE_SEL  },
    {PIN_EPWM4_B, ( PIN_MODE(MW_PIN_EPWM4_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX52_ENABLE_SEL  },
    {PIN_EPWM5_A, ( PIN_MODE(MW_PIN_EPWM5_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX53_ENABLE_SEL  },
    {PIN_EPWM5_B, ( PIN_MODE(MW_PIN_EPWM5_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX54_ENABLE_SEL  },
    {PIN_EPWM6_A, ( PIN_MODE(MW_PIN_EPWM6_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX55_ENABLE_SEL  },
    {PIN_EPWM6_B, ( PIN_MODE(MW_PIN_EPWM6_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX56_ENABLE_SEL  },
    {PIN_EPWM7_A, ( PIN_MODE(MW_PIN_EPWM7_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX57_ENABLE_SEL  },
    {PIN_EPWM7_B, ( PIN_MODE(MW_PIN_EPWM7_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX58_ENABLE_SEL  },
    {PIN_EPWM8_A, ( PIN_MODE(MW_PIN_EPWM8_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX59_ENABLE_SEL  },
    {PIN_EPWM8_B, ( PIN_MODE(MW_PIN_EPWM8_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX60_ENABLE_SEL  },
    {PIN_EPWM9_A, ( PIN_MODE(MW_PIN_EPWM9_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX61_ENABLE_SEL  },
    {PIN_EPWM9_B, ( PIN_MODE(MW_PIN_EPWM9_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX62_ENABLE_SEL  },
    {PIN_EPWM10_A, ( PIN_MODE(MW_PIN_EPWM10_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX63_ENABLE_SEL  },
    {PIN_EPWM10_B, ( PIN_MODE(MW_PIN_EPWM10_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX64_ENABLE_SEL  },
    {PIN_EPWM11_A, ( PIN_MODE(MW_PIN_EPWM11_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX65_ENABLE_SEL  },
    {PIN_EPWM11_B, ( PIN_MODE(MW_PIN_EPWM11_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX66_ENABLE_SEL  },
    {PIN_EPWM12_A, ( PIN_MODE(MW_PIN_EPWM12_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX67_ENABLE_SEL  },
    {PIN_EPWM12_B, ( PIN_MODE(MW_PIN_EPWM12_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX68_ENABLE_SEL  },
    {PIN_EPWM13_A, ( PIN_MODE(MW_PIN_EPWM13_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX69_ENABLE_SEL  },
    {PIN_EPWM13_B, ( PIN_MODE(MW_PIN_EPWM13_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX70_ENABLE_SEL  },
    {PIN_EPWM14_A, ( PIN_MODE(MW_PIN_EPWM14_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX71_ENABLE_SEL  },
    {PIN_EPWM14_B, ( PIN_MODE(MW_PIN_EPWM14_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX72_ENABLE_SEL  },
    {PIN_EPWM15_A, ( PIN_MODE(MW_PIN_EPWM15_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX73_ENABLE_SEL  },
    {PIN_EPWM15_B, ( PIN_MODE(MW_PIN_EPWM15_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX74_ENABLE_SEL  },
    {PIN_UART1_RXD, ( PIN_MODE(MW_PIN_UART1_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX75_ENABLE_SEL  },
    {PIN_UART1_TXD, ( PIN_MODE(MW_PIN_UART1_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX76_ENABLE_SEL  },
    {PIN_MMC_CLK, ( PIN_MODE(MW_PIN_MMC0_CLK_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX77_ENABLE_SEL  },
    {PIN_MMC_CMD, ( PIN_MODE(MW_PIN_MMC0_CMD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX78_ENABLE_SEL  },
    {PIN_MMC_DAT0, ( PIN_MODE(MW_PIN_MMC0_D0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX79_ENABLE_SEL  },
    {PIN_MMC_DAT1, ( PIN_MODE(MW_PIN_MMC0_D1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX80_ENABLE_SEL  },
    {PIN_MMC_DAT2, ( PIN_MODE(MW_PIN_MMC0_D2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX81_ENABLE_SEL  },
    {PIN_MMC_DAT3, ( PIN_MODE(MW_PIN_MMC0_D3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX82_ENABLE_SEL  },
    {PIN_MMC_SDWP, ( PIN_MODE(MW_PIN_MMC0_WP_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX83_ENABLE_SEL  },
    {PIN_MMC_SDCD, ( PIN_MODE(MW_PIN_MMC0_CD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX84_ENABLE_SEL  },
    {PIN_PR0_MDIO_MDIO, ( PIN_MODE(MW_PIN_PR0_MDIO0_MDIO_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX85_ENABLE_SEL  },
    {PIN_PR0_MDIO_MDC, ( PIN_MODE(MW_PIN_PR0_MDIO0_MDC_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX86_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO5, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO5_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX87_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO9, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO9_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX88_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO10, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO10_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX89_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO8, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO8_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX90_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO6, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO6_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX91_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO4, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO4_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX92_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO0, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX93_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO1, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX94_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO2, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX95_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO3, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX96_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO16, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO16_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX97_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO15, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO15_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX98_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO11, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO11_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX99_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO12, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO12_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX100_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO13, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO13_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX101_ENABLE_SEL  },
    {PIN_PR0_PRU0_GPIO14, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO14_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX102_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO5, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO5_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX103_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO9, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO9_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX104_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO10, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO10_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX105_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO8, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO8_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX106_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO6, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO6_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX107_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO4, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO4_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX108_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO0, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX109_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO1, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX110_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO2, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX111_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO3, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX112_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO16, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO16_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX113_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO15, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO15_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX114_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO11, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO11_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX115_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO12, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO12_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX116_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO13, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO13_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX117_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO14, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO14_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX118_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO19, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO19_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX119_ENABLE_SEL  },
    {PIN_PR0_PRU1_GPIO18, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO18_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX120_ENABLE_SEL  },
    {PIN_EXT_REFCLK0, ( PIN_MODE(MW_PIN_EXT_REFCLK0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX121_ENABLE_SEL  },
    {PIN_SDFM0_CLK0, ( PIN_MODE(MW_PIN_SDFM0_CLK0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX122_ENABLE_SEL  },
    {PIN_SDFM0_D0, ( PIN_MODE(MW_PIN_SDFM0_D0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX123_ENABLE_SEL  },
    {PIN_SDFM0_CLK1, ( PIN_MODE(MW_PIN_SDFM0_CLK1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX124_ENABLE_SEL  },
    {PIN_SDFM0_D1, ( PIN_MODE(MW_PIN_SDFM0_D1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX125_ENABLE_SEL  },
    {PIN_SDFM0_CLK2, ( PIN_MODE(MW_PIN_SDFM0_CLK2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX126_ENABLE_SEL  },
    {PIN_SDFM0_D2, ( PIN_MODE(MW_PIN_SDFM0_D2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX127_ENABLE_SEL  },
    {PIN_SDFM0_CLK3, ( PIN_MODE(MW_PIN_SDFM0_CLK3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX128_ENABLE_SEL  },
    {PIN_SDFM0_D3, ( PIN_MODE(MW_PIN_SDFM0_D3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX129_ENABLE_SEL  },
    {PIN_EQEP0_A, ( PIN_MODE(MW_PIN_EQEP0_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX130_ENABLE_SEL  },
    {PIN_EQEP0_B, ( PIN_MODE(MW_PIN_EQEP0_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX131_ENABLE_SEL  },
    {PIN_EQEP0_STROBE, ( PIN_MODE(MW_PIN_EQEP0_STROBE_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX132_ENABLE_SEL  },
    {PIN_EQEP0_INDEX, ( PIN_MODE(MW_PIN_EQEP0_INDEX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX133_ENABLE_SEL  },
    {PIN_I2C0_SDA, ( PIN_MODE(MW_PIN_I2C0_SDA_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX134_ENABLE_SEL  },
    {PIN_I2C0_SCL, ( PIN_MODE(MW_PIN_I2C0_SCL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX135_ENABLE_SEL  },
    {PIN_MCAN2_TX, ( PIN_MODE(MW_PIN_MCAN2_TX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX136_ENABLE_SEL  },
    {PIN_MCAN2_RX, ( PIN_MODE(MW_PIN_MCAN2_RX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX137_ENABLE_SEL  },
    {PIN_CLKOUT0, ( PIN_MODE(MW_PIN_CLKOUT0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ), MW_PINMUX138_ENABLE_SEL  },
    
    {PINMUX_END, PINMUX_END, 1}
};



#define SOC_MODULES_END     (0xFFFFFFFFu)
#define SOCModulesMaxConfig 13U

typedef struct {

    uint32_t moduleId;
    uint32_t clkId;
    uint32_t clkRate;

} SOC_ModuleClockFrequency;


uint32_t gSocModules[SOCModulesMaxConfig];
SOC_ModuleClockFrequency gSocModulesClockFrequency[SOCModulesMaxConfig];

void SOCModulesInit(void)
{
    uint8_t indx = 0;

    #ifdef MW_EXTMODE_RUNNING
    String board = STR(MW_TARGETHARDWARE);
    String XCPInterface = STR(MW_EXTMODE_CONFIGURATION);
    String ExtRunning = STR(MW_EXTMODE_RUNNING);
    #endif

    #ifdef MW_EXTMODE_RUNNING
    if((strcmp(XCPInterface, "XCP on Serial AM263")))
        gSocModules[indx] = SOC_RcmPeripheralId_LIN0_UART0; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN0_UART0;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    #endif

   
        gSocModules[indx] = SOC_RcmPeripheralId_LIN0_UART0; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN0_UART0;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    
    

    if(MW_UART1_ENABLE_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_LIN1_UART1; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN1_UART1;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    }

    if(MW_UART2_ENABLE_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_LIN2_UART2; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN2_UART2;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    }

    if(MW_UART3_ENABLE_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_LIN3_UART3; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN3_UART3;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    }

    if(MW_UART4_ENABLE_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_LIN4_UART4; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN4_UART4;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    }

    if(MW_UART5_ENABLE_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_LIN5_UART5; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN5_UART5;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT1;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    }

    #ifdef MW_EXTMODE_RUNNING
    if((strcmp(XCPInterface, "CAN") == 0) && (strcmp(board, "AM263X ControlCard") == 0))
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCAN1; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCAN1;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 80000000;
        indx++;   
    }
    else if((strcmp(XCPInterface, "CAN") == 0) && (strcmp(board, "AM263X LP") == 0))
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCAN0; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCAN0;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 80000000;
        indx++;
    }
    #endif
    if(MW_ENABLE_MCAN0_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCAN0; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCAN0;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 80000000;
        indx++;
    }

    if(MW_ENABLE_MCAN1_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCAN1; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCAN1;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 80000000;
        indx++;;
    }
   

    if(MW_ENABLE_MCAN2_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCAN2; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCAN2;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 80000000;
        indx++;
    }

    if(MW_ENABLE_MCAN3_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCAN3; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCAN3;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 80000000;
        indx++;
    }

    if(MW_SDFM_ENABLE_SDFM0_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_CONTROLSS_PLL;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_CONTROLSS_PLL;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2;
        gSocModulesClockFrequency[indx].clkRate = 400000000; 
        indx++;
    }

    if(MW_SDFM_ENABLE_SDFM1_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_CONTROLSS_PLL;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_CONTROLSS_PLL;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2;
        gSocModulesClockFrequency[indx].clkRate = 400000000; 
        indx++;
    }

    gSocModules[indx] = SOC_MODULES_END;
    gSocModulesClockFrequency[indx].moduleId = SOC_MODULES_END; 
    gSocModulesClockFrequency[indx].clkId = SOC_MODULES_END;
    gSocModulesClockFrequency[indx].clkRate = SOC_MODULES_END;
}


void Module_clockEnable(void)
{
    int32_t status;
    uint32_t i = 0;

    while(gSocModules[i]!=SOC_MODULES_END)
    {
        status = SOC_moduleClockEnable(gSocModules[i], 1);
        DebugP_assertNoLog(status == SystemP_SUCCESS);
        i++;
    }
}

void Module_clockDisable(void)
{
    int32_t status;
    uint32_t i = 0;

    while(gSocModules[i]!=SOC_MODULES_END)
    {
        status = SOC_moduleClockEnable(gSocModules[i], 0);
        DebugP_assertNoLog(status == SystemP_SUCCESS);
        i++;
    }
}

void Module_clockSetFrequency(void)
{
    int32_t status;
    uint32_t i = 0;

    while(gSocModulesClockFrequency[i].moduleId!=SOC_MODULES_END)
    {
        status = SOC_moduleSetClockFrequency(
                    gSocModulesClockFrequency[i].moduleId,
                    gSocModulesClockFrequency[i].clkId,
                    gSocModulesClockFrequency[i].clkRate
                    );
        DebugP_assertNoLog(status == SystemP_SUCCESS);
        i++;
    }
}


// uint32_t intrNumUART0, intrNumUART1, intrNumUART2, intrNumUART3, intrNumUART4, intrNumUART5, intrNum;
// uint32_t intrNum, intcBaseAddr;
// uint32_t gUartVimStsAddrUART0;
// uint32_t gUartVimStsClrMaskUART0;
// extern UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_LLD_INSTANCES];
// UARTLLD_Handle gUartHandle0, gUartHandle1, gUartHandle2, gUartHandle3, gUartHandle4, gUartHandle5;
// #define CONFIG_UART0 (0U)
// #define CONFIG_UART_NUM_INSTANCES (1U)
// UARTLLD_Object gUartObject[CONFIG_UART_NUM_INSTANCES];
// UARTLLD_Handle gUartHandle[CONFIG_UART_NUM_INSTANCES];
// UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_INSTANCES];
// {
//     {
//     .inputClkFreq       = 48000000U,
//     .baudRate           = 115200,
//     .dataLength         = UART_LEN_8,
//     .stopBits           = UART_STOPBITS_1,
//     .parityType         = UART_PARITY_NONE,
//     .readReturnMode     = UART_READ_RETURN_MODE_FULL,
//     .hwFlowControl      = FALSE,
//     .hwFlowControlThr   = UART_RXTRIGLVL_16,
//     .transferMode       = UART_CONFIG_MODE_POLLED,
//     .intrNum            = 38U,
//     .intrPriority       = 4U,
//     .operMode           = UART_OPER_MODE_16X,
//     .rxTrigLvl          = UART_RXTRIGLVL_8,
//     .txTrigLvl          = UART_TXTRIGLVL_32,
//     .clockP_get         = ClockP_getTicks,
//     .clockP_usecToTick  = ClockP_usecToTicks,
//     .writeCompleteCallbackFxn = NULL,
//     .readCompleteCallbackFxn  = NULL,
//     },

// };
void Drivers_uartOpen(void)
{

    int32_t status = UART_STATUS_SUCCESS;
    uint32_t baudRate, intrNum, baseAddr, BaudRateSel;
    uint32_t i;
    uint32_t intrNumUART0, intrNumUART1, intrNumUART2, intrNumUART3, intrNumUART4, intrNumUART5;
    
    for(i = 0; i<CONFIG_UART_NUM_LLD_INSTANCES; i++)
    {
        status = UART_STATUS_SUCCESS;
    //     intrNumUART0 = 38;
        gUartInitObject[i].inputClkFreq       = 48000000;
        gUartInitObject[i].hwFlowControl      = FALSE;
        gUartInitObject[i].hwFlowControlThr   = UART_RXTRIGLVL_16;
        gUartInitObject[i].clockP_get         = ClockP_getTicks;
        gUartInitObject[i].clockP_usecToTick  = ClockP_usecToTicks;
        

        if(i == 0)
        {
            intrNumUART0 = 38;
            BaudRateSel = MW_UART0_BAUDRATE_SEL;
            baseAddr = CSL_UART0_U_BASE;
            gUartInitObject[i].stopBits           = MW_UART0_STOP_BIT_SEL;
            gUartInitObject[i].operMode           = MW_UART0_OPERATIONAL_MODE_SEL;
            gUartInitObject[i].dataLength         = MW_UART0_DATALENGTH_SEL;
            gUartInitObject[i].readReturnMode     = MW_UART0_READ_RETURNMODE_SEL;
            if(MW_UART0_TRANSFER_MODE_SEL == 0)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_POLLED;
            else if(MW_UART0_TRANSFER_MODE_SEL == 1)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_USER_INTR;
            gUartInitObject[i].intrPriority       = 4U;
            gUartInitObject[i].intrNum            = intrNumUART0;

            if(MW_UART0_PARITY_TYPE_SEL == 0)
                gUartInitObject[i].parityType = UART_PARITY_NONE;
            else if(MW_UART0_PARITY_TYPE_SEL == 1)
                gUartInitObject[i].parityType = UART_PARITY_ODD;
            else if(MW_UART0_PARITY_TYPE_SEL == 2)
                gUartInitObject[i].parityType = UART_PARITY_EVEN;
            else if(MW_UART0_PARITY_TYPE_SEL == 3)
                gUartInitObject[i].parityType = UART_PARITY_FORCED0;
            else if(MW_UART0_PARITY_TYPE_SEL == 4)
                gUartInitObject[i].parityType = UART_PARITY_FORCED1;

            if(MW_UART0_RX_TRIGGER_SEL == 0)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_1;
            else if(MW_UART0_RX_TRIGGER_SEL == 1)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_8;
            else if(MW_UART0_RX_TRIGGER_SEL == 2)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_16;
            else if(MW_UART0_RX_TRIGGER_SEL == 3)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_56;
            else if(MW_UART0_RX_TRIGGER_SEL == 4)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_60;

            if(MW_UART0_TX_TRIGGER_SEL == 0)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_1;
            else if(MW_UART0_TX_TRIGGER_SEL == 1)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_8;
            else if(MW_UART0_TX_TRIGGER_SEL == 2)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_16;
            else if(MW_UART0_TX_TRIGGER_SEL == 3)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_32;
            else if(MW_UART0_TX_TRIGGER_SEL == 4)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_56;
           
            gUartInitObject[i].writeCompleteCallbackFxn = NULL;
            gUartInitObject[i].readCompleteCallbackFxn  = NULL;
           

    
        }
        else if(i == 1)
        {
            BaudRateSel = MW_UART1_BAUDRATE_SEL;
            intrNumUART1 = 39;
            baseAddr = CSL_UART1_U_BASE;
            gUartInitObject[i].stopBits           = MW_UART1_STOP_BIT_SEL;
            gUartInitObject[i].operMode           = MW_UART1_OPERATIONAL_MODE_SEL;
            gUartInitObject[i].dataLength         = MW_UART1_DATALENGTH_SEL;
            gUartInitObject[i].readReturnMode     = MW_UART1_READ_RETURNMODE_SEL;
            if(MW_UART1_TRANSFER_MODE_SEL == 0)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_POLLED;
            else if(MW_UART1_TRANSFER_MODE_SEL == 1)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_USER_INTR;
            gUartInitObject[i].intrPriority       = 4U;
            gUartInitObject[i].intrNum            = intrNumUART1;

            if(MW_UART1_PARITY_TYPE_SEL == 0)
                gUartInitObject[i].parityType = UART_PARITY_NONE;
            else if(MW_UART1_PARITY_TYPE_SEL == 1)
                gUartInitObject[i].parityType = UART_PARITY_ODD;
            else if(MW_UART1_PARITY_TYPE_SEL == 2)
                gUartInitObject[i].parityType = UART_PARITY_EVEN;
            else if(MW_UART1_PARITY_TYPE_SEL == 3)
                gUartInitObject[i].parityType = UART_PARITY_FORCED0;
            else if(MW_UART1_PARITY_TYPE_SEL == 4)
                gUartInitObject[i].parityType = UART_PARITY_FORCED1;

            if(MW_UART1_RX_TRIGGER_SEL == 0)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_1;
            else if(MW_UART1_RX_TRIGGER_SEL == 1)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_8;
            else if(MW_UART1_RX_TRIGGER_SEL == 2)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_16;
            else if(MW_UART1_RX_TRIGGER_SEL == 3)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_56;
            else if(MW_UART1_RX_TRIGGER_SEL == 4)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_60;

            if(MW_UART1_TX_TRIGGER_SEL == 0)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_1;
            else if(MW_UART1_TX_TRIGGER_SEL == 1)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_8;
            else if(MW_UART1_TX_TRIGGER_SEL == 2)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_16;
            else if(MW_UART1_TX_TRIGGER_SEL == 3)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_32;
            else if(MW_UART1_TX_TRIGGER_SEL == 4)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_56;
            
            gUartInitObject[i].writeCompleteCallbackFxn = NULL;
            gUartInitObject[i].readCompleteCallbackFxn  = NULL;
            
        }
        else if(i == 2)
        {
            BaudRateSel = MW_UART2_BAUDRATE_SEL;
            intrNumUART2 = 40;
            baseAddr = CSL_UART2_U_BASE;
            gUartInitObject[i].stopBits           = MW_UART2_STOP_BIT_SEL;
            gUartInitObject[i].operMode           = MW_UART2_OPERATIONAL_MODE_SEL;
            gUartInitObject[i].dataLength         = MW_UART2_DATALENGTH_SEL;
            gUartInitObject[i].readReturnMode     = MW_UART2_READ_RETURNMODE_SEL;
            if(MW_UART2_TRANSFER_MODE_SEL == 0)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_POLLED;
            else if(MW_UART2_TRANSFER_MODE_SEL == 1)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_USER_INTR;
            gUartInitObject[i].intrPriority       = 4U;
            gUartInitObject[i].intrNum            = intrNumUART2;

            if(MW_UART2_PARITY_TYPE_SEL == 0)
                gUartInitObject[i].parityType = UART_PARITY_NONE;
            else if(MW_UART2_PARITY_TYPE_SEL == 1)
                gUartInitObject[i].parityType = UART_PARITY_ODD;
            else if(MW_UART2_PARITY_TYPE_SEL == 2)
                gUartInitObject[i].parityType = UART_PARITY_EVEN;
            else if(MW_UART2_PARITY_TYPE_SEL == 3)
                gUartInitObject[i].parityType = UART_PARITY_FORCED0;
            else if(MW_UART2_PARITY_TYPE_SEL == 4)
                gUartInitObject[i].parityType = UART_PARITY_FORCED1;

            if(MW_UART2_RX_TRIGGER_SEL == 0)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_1;
            else if(MW_UART2_RX_TRIGGER_SEL == 1)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_8;
            else if(MW_UART2_RX_TRIGGER_SEL == 2)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_16;
            else if(MW_UART2_RX_TRIGGER_SEL == 3)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_56;
            else if(MW_UART2_RX_TRIGGER_SEL == 4)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_60;

            if(MW_UART2_TX_TRIGGER_SEL == 0)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_1;
            else if(MW_UART2_TX_TRIGGER_SEL == 1)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_8;
            else if(MW_UART2_TX_TRIGGER_SEL == 2)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_16;
            else if(MW_UART2_TX_TRIGGER_SEL == 3)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_32;
            else if(MW_UART2_TX_TRIGGER_SEL == 4)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_56;
         
                gUartInitObject[i].writeCompleteCallbackFxn = NULL;
                gUartInitObject[i].readCompleteCallbackFxn  = NULL;
           
        }
        else if(i == 3)
        {
            BaudRateSel = MW_UART3_BAUDRATE_SEL;
            intrNumUART3 = 41;
            baseAddr = CSL_UART3_U_BASE;
            gUartInitObject[i].stopBits           = MW_UART3_STOP_BIT_SEL;
            gUartInitObject[i].operMode           = MW_UART3_OPERATIONAL_MODE_SEL;
            gUartInitObject[i].dataLength         = MW_UART3_DATALENGTH_SEL;
            gUartInitObject[i].readReturnMode     = MW_UART3_READ_RETURNMODE_SEL;
            if(MW_UART3_TRANSFER_MODE_SEL == 0)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_POLLED;
            else if(MW_UART3_TRANSFER_MODE_SEL == 1)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_USER_INTR;
            gUartInitObject[i].intrPriority       = 4U;
            gUartInitObject[i].intrNum            = intrNumUART3;

            if(MW_UART3_PARITY_TYPE_SEL == 0)
                gUartInitObject[i].parityType = UART_PARITY_NONE;
            else if(MW_UART3_PARITY_TYPE_SEL == 1)
                gUartInitObject[i].parityType = UART_PARITY_ODD;
            else if(MW_UART3_PARITY_TYPE_SEL == 2)
                gUartInitObject[i].parityType = UART_PARITY_EVEN;
            else if(MW_UART3_PARITY_TYPE_SEL == 3)
                gUartInitObject[i].parityType = UART_PARITY_FORCED0;
            else if(MW_UART3_PARITY_TYPE_SEL == 4)
                gUartInitObject[i].parityType = UART_PARITY_FORCED1;

            if(MW_UART3_RX_TRIGGER_SEL == 0)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_1;
            else if(MW_UART3_RX_TRIGGER_SEL == 1)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_8;
            else if(MW_UART3_RX_TRIGGER_SEL == 2)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_16;
            else if(MW_UART3_RX_TRIGGER_SEL == 3)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_56;
            else if(MW_UART3_RX_TRIGGER_SEL == 4)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_60;

            if(MW_UART3_TX_TRIGGER_SEL == 0)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_1;
            else if(MW_UART3_TX_TRIGGER_SEL == 1)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_8;
            else if(MW_UART3_TX_TRIGGER_SEL == 2)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_16;
            else if(MW_UART3_TX_TRIGGER_SEL == 3)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_32;
            else if(MW_UART3_TX_TRIGGER_SEL == 4)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_56;
           
            gUartInitObject[i].writeCompleteCallbackFxn = NULL;
            gUartInitObject[i].readCompleteCallbackFxn  = NULL;
           
        }
        else if(i == 4)
        {
            BaudRateSel = MW_UART4_BAUDRATE_SEL;
            intrNumUART4 = 42;
            baseAddr = CSL_UART4_U_BASE;
            gUartInitObject[i].stopBits           = MW_UART4_STOP_BIT_SEL;
            gUartInitObject[i].operMode           = MW_UART4_OPERATIONAL_MODE_SEL;
            gUartInitObject[i].dataLength         = MW_UART4_DATALENGTH_SEL;
            gUartInitObject[i].readReturnMode     = MW_UART4_READ_RETURNMODE_SEL;
            if(MW_UART4_TRANSFER_MODE_SEL == 0)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_POLLED;
            else if(MW_UART4_TRANSFER_MODE_SEL == 1)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_USER_INTR;
            gUartInitObject[i].intrPriority       = 4U;
            gUartInitObject[i].intrNum            = intrNumUART4;

            if(MW_UART4_PARITY_TYPE_SEL == 0)
                gUartInitObject[i].parityType = UART_PARITY_NONE;
            else if(MW_UART4_PARITY_TYPE_SEL == 1)
                gUartInitObject[i].parityType = UART_PARITY_ODD;
            else if(MW_UART4_PARITY_TYPE_SEL == 2)
                gUartInitObject[i].parityType = UART_PARITY_EVEN;
            else if(MW_UART4_PARITY_TYPE_SEL == 3)
                gUartInitObject[i].parityType = UART_PARITY_FORCED0;
            else if(MW_UART4_PARITY_TYPE_SEL == 4)
                gUartInitObject[i].parityType = UART_PARITY_FORCED1;

            if(MW_UART4_RX_TRIGGER_SEL == 0)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_1;
            else if(MW_UART4_RX_TRIGGER_SEL == 1)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_8;
            else if(MW_UART4_RX_TRIGGER_SEL == 2)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_16;
            else if(MW_UART4_RX_TRIGGER_SEL == 3)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_56;
            else if(MW_UART4_RX_TRIGGER_SEL == 4)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_60;

            if(MW_UART4_TX_TRIGGER_SEL == 0)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_1;
            else if(MW_UART4_TX_TRIGGER_SEL == 1)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_8;
            else if(MW_UART4_TX_TRIGGER_SEL == 2)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_16;
            else if(MW_UART4_TX_TRIGGER_SEL == 3)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_32;
            else if(MW_UART4_TX_TRIGGER_SEL == 4)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_56;
          
            gUartInitObject[i].writeCompleteCallbackFxn = NULL;
            gUartInitObject[i].readCompleteCallbackFxn  = NULL;
           
        }
        else if(i == 5)
        {
            BaudRateSel = MW_UART5_BAUDRATE_SEL;
            intrNumUART5 = 43;
            baseAddr = CSL_UART5_U_BASE;
            gUartInitObject[i].stopBits           = MW_UART5_STOP_BIT_SEL;
            gUartInitObject[i].operMode           = MW_UART5_OPERATIONAL_MODE_SEL;
            gUartInitObject[i].dataLength         = MW_UART5_DATALENGTH_SEL;
            gUartInitObject[i].readReturnMode     = MW_UART5_READ_RETURNMODE_SEL;
            if(MW_UART5_TRANSFER_MODE_SEL == 0)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_POLLED;
            else if(MW_UART5_TRANSFER_MODE_SEL == 1)
                gUartInitObject[i].transferMode       = UART_CONFIG_MODE_USER_INTR;
            gUartInitObject[i].intrPriority       = 4U;
            gUartInitObject[i].intrNum            = intrNumUART5;

            if(MW_UART5_PARITY_TYPE_SEL == 0)
                gUartInitObject[i].parityType = UART_PARITY_NONE;
            else if(MW_UART5_PARITY_TYPE_SEL == 1)
                gUartInitObject[i].parityType = UART_PARITY_ODD;
            else if(MW_UART5_PARITY_TYPE_SEL == 2)
                gUartInitObject[i].parityType = UART_PARITY_EVEN;
            else if(MW_UART5_PARITY_TYPE_SEL == 3)
                gUartInitObject[i].parityType = UART_PARITY_FORCED0;
            else if(MW_UART5_PARITY_TYPE_SEL == 4)
                gUartInitObject[i].parityType = UART_PARITY_FORCED1;

            if(MW_UART5_RX_TRIGGER_SEL == 0)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_1;
            else if(MW_UART5_RX_TRIGGER_SEL == 1)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_8;
            else if(MW_UART5_RX_TRIGGER_SEL == 2)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_16;
            else if(MW_UART5_RX_TRIGGER_SEL == 3)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_56;
            else if(MW_UART5_RX_TRIGGER_SEL == 4)
                gUartInitObject[i].rxTrigLvl      = UART_RXTRIGLVL_60;

            if(MW_UART5_TX_TRIGGER_SEL == 0)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_1;
            else if(MW_UART5_TX_TRIGGER_SEL == 1)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_8;
            else if(MW_UART5_TX_TRIGGER_SEL == 2)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_16;
            else if(MW_UART5_TX_TRIGGER_SEL == 3)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_32;
            else if(MW_UART5_TX_TRIGGER_SEL == 4)
                gUartInitObject[i].txTrigLvl      = UART_TXTRIGLVL_56;
           
            gUartInitObject[i].writeCompleteCallbackFxn = NULL;
            gUartInitObject[i].readCompleteCallbackFxn  = NULL;
            
          
        }
        
            if(BaudRateSel == 0)
                baudRate = 110;
            else if(BaudRateSel == 1)
                baudRate = 300;
            else if(BaudRateSel == 2)
                baudRate = 600;
            else if(BaudRateSel == 3)
                baudRate = 1200;
            else if(BaudRateSel == 4)
                baudRate = 2400;
            else if(BaudRateSel == 5)
                baudRate = 4800;
            else if(BaudRateSel == 6)
                baudRate = 9600;
            else if(BaudRateSel == 7)
                baudRate = 14400;
            else if(BaudRateSel == 8)
                baudRate = 19200;
            else if(BaudRateSel == 9)
                baudRate = 38400;
            else if(BaudRateSel == 10)
                baudRate = 57600;
            else if(BaudRateSel == 11)
                baudRate = 115200;
            else if(BaudRateSel == 12)
                baudRate = 230400;
            else if(BaudRateSel == 13)
                baudRate = 460800;
            else if(BaudRateSel == 14)
                baudRate = 921600;
            int32_t status = UART_STATUS_SUCCESS;
            gUartInitObject[i].baudRate   =  baudRate;
            gUartHandleLld[i]             = &gUartObject[i];
            gUartHandleLld[i]->state      = UART_STATE_RESET;
            gUartHandleLld[i]->baseAddr   = baseAddr;
            gUartHandleLld[i]->hUartInit = &gUartInitObject[i];


            if((i==0) && MW_UART0_ENABLE_SEL == 1)
            {
                  status += UART_lld_init(gUartHandleLld[i]);
            }
            else if((i==1) && MW_UART1_ENABLE_SEL == 1)
            {
                status += UART_lld_init(gUartHandleLld[i]);
            }
            else if((i==2) && MW_UART2_ENABLE_SEL == 1)
            {
                status += UART_lld_init(gUartHandleLld[i]);
            }
            else if((i==3) && MW_UART3_ENABLE_SEL == 1)
            {
                status += UART_lld_init(gUartHandleLld[i]);
            }
            else if((i==4) && MW_UART4_ENABLE_SEL == 1)
            {
                status += UART_lld_init(gUartHandleLld[i]);
            }
            else if((i==5) && MW_UART5_ENABLE_SEL == 1)
            {
                status += UART_lld_init(gUartHandleLld[i]);
            }

            DebugP_assert(status == UART_STATUS_SUCCESS);

            if(i == 0 && (MW_UART0_TRANSFER_MODE_SEL == 1) && (MW_UART0_ENABLE_SEL == 1))
            {
               
                if(MW_UART0_CTSINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_CTS);  
                if(MW_UART0_RTSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RTS); 
                if(MW_UART0_XOFFINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_XOFF); 
                if(MW_UART0_SLEEPMODEINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_SLEEPMODE); 
                if(MW_UART0_INTLINESTATUS_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_LINE_STAT);
                if(MW_UART0_MODEMSTATUSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_MODEM_STAT);
                if(MW_UART0_RHRINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RHR_CTI);
                if(MW_UART0_THREINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_THR);
                   
            }
            else if(i == 1 && (MW_UART1_TRANSFER_MODE_SEL == 1) && (MW_UART1_ENABLE_SEL == 1))
            {
                if(MW_UART1_CTSINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_CTS);  
                if(MW_UART1_RTSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RTS); 
                if(MW_UART1_XOFFINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_XOFF); 
                if(MW_UART1_SLEEPMODEINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_SLEEPMODE); 
                if(MW_UART1_INTLINESTATUS_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_LINE_STAT);
                if(MW_UART1_MODEMSTATUSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_MODEM_STAT);
                if(MW_UART1_RHRINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RHR_CTI);
                if(MW_UART1_THREINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_THR);
                
            }
            else if(i == 2 && (MW_UART2_TRANSFER_MODE_SEL == 1) && (MW_UART2_ENABLE_SEL == 1))
            {
                if(MW_UART2_CTSINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_CTS);  
                if(MW_UART2_RTSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RTS); 
                if(MW_UART2_XOFFINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_XOFF); 
                if(MW_UART2_SLEEPMODEINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_SLEEPMODE); 
                if(MW_UART2_INTLINESTATUS_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_LINE_STAT);
                if(MW_UART2_MODEMSTATUSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_MODEM_STAT);
                if(MW_UART2_RHRINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RHR_CTI);
                if(MW_UART2_THREINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_THR);
            }
            else if(i == 3 && (MW_UART3_TRANSFER_MODE_SEL == 1) && (MW_UART3_ENABLE_SEL == 1))
            {
                if(MW_UART3_CTSINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_CTS);  
                if(MW_UART3_RTSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RTS); 
                if(MW_UART3_XOFFINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_XOFF); 
                if(MW_UART3_SLEEPMODEINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_SLEEPMODE); 
                if(MW_UART3_INTLINESTATUS_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_LINE_STAT);
                if(MW_UART3_MODEMSTATUSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_MODEM_STAT);
                if(MW_UART3_RHRINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RHR_CTI);
                if(MW_UART3_THREINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_THR);
            }
            else if(i == 4 && (MW_UART4_TRANSFER_MODE_SEL == 1) && (MW_UART4_ENABLE_SEL == 1))
            {
                if(MW_UART4_CTSINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_CTS);  
                if(MW_UART4_RTSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RTS); 
                if(MW_UART4_XOFFINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_XOFF); 
                if(MW_UART4_SLEEPMODEINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_SLEEPMODE); 
                if(MW_UART4_INTLINESTATUS_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_LINE_STAT);
                if(MW_UART4_MODEMSTATUSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_MODEM_STAT);
                if(MW_UART4_RHRINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RHR_CTI);
                if(MW_UART4_THREINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_THR);
            }
            else if(i == 5 && (MW_UART5_TRANSFER_MODE_SEL == 1) && (MW_UART5_ENABLE_SEL == 1))
            {
                
                if(MW_UART5_CTSINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_CTS);  
                if(MW_UART5_RTSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RTS); 
                if(MW_UART5_XOFFINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_XOFF); 
                if(MW_UART5_SLEEPMODEINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_SLEEPMODE); 
                if(MW_UART5_INTLINESTATUS_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_LINE_STAT);
                if(MW_UART5_MODEMSTATUSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_MODEM_STAT);
                if(MW_UART5_RHRINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_RHR_CTI);
                if(MW_UART5_THREINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[i]->baseAddr, UART_INTR_THR);         
            }


    }
}




void Pinmux_init(void)
{
    Pinmux_PerCfg_t gPinMuxMainDomainCfg_init[140];
    uint8_t j = 0;

    for(uint8_t i=0; i<140; i++)
    {
        if(gPinMuxMainDomainCfg[i].enable == TRUE)
        {
            gPinMuxMainDomainCfg_init[j].offset = gPinMuxMainDomainCfg[i].offset;
            gPinMuxMainDomainCfg_init[j].settings = gPinMuxMainDomainCfg[i].settings;
            gPinMuxMainDomainCfg[i].offset++;
            j++;
        }
    }
    Pinmux_config(gPinMuxMainDomainCfg_init, PINMUX_DOMAIN_ID_MAIN);


    // Pinmux_config(gPinMuxMainDomainCfg, PINMUX_DOMAIN_ID_MAIN);
    
}

void PowerClock_init(void)
{
    SOCModulesInit();   
    Module_clockEnable();
    Module_clockSetFrequency();
}


void Dpl_init(void)
{
    /* initialize Hwi but keep interrupts disabled */
    HwiP_init();

    /* init debug log zones early */
    /* Debug log init */
    /* UART console to use for reading input */
    DebugP_uartSetDrvIndex(CONFIG_UART0);


    /* set timer clock source */
    SOC_controlModuleUnlockMMR(SOC_DOMAIN_ID_MAIN, MSS_RCM_PARTITION0);
    *(volatile uint32_t*)(RTI0_CLOCK_SRC_MUX_ADDR) = RTI0_CLOCK_SRC_WUCPUCLK;
    SOC_controlModuleLockMMR(SOC_DOMAIN_ID_MAIN, MSS_RCM_PARTITION0);
    /* initialize Clock */
    ClockP_init();

    /* Enable interrupt handling */
    HwiP_enable();
}


void Drivers_intXbarOpen(void)
{
    /* Interrupt XBAR*/
    
    uint32_t group0_mask[32];
    uint32_t group1_mask[32];
    uint32_t group2_mask[32];
    uint32_t group3_mask[32];
    uint32_t group4_mask[32];
    uint32_t group5_mask[32];
    uint32_t group6_mask[32];

    group0_mask[0] = (uint32_t)MW_INTXBAR0_EPWMINT_SEL;
    group1_mask[0] = (uint32_t)MW_INTXBAR0_EPWMTZINT_SEL;
    group2_mask[0] = (uint32_t)MW_INTXBAR0_ADCINT_SEL;
    group3_mask[0] = (uint32_t)MW_INTXBAR0_FSIRXINT_SEL;
    group4_mask[0] = (uint32_t)MW_INTXBAR0_SDFMINT_SEL;
    group5_mask[0] = (uint32_t)MW_INTXBAR0_ECAPINT_SEL;
    group6_mask[0] = (uint32_t)MW_INTXBAR0_EQEPINT_SEL;
    group0_mask[1] = (uint32_t)MW_INTXBAR1_EPWMINT_SEL;
    group1_mask[1] = (uint32_t)MW_INTXBAR1_EPWMTZINT_SEL;
    group2_mask[1] = (uint32_t)MW_INTXBAR1_ADCINT_SEL;
    group3_mask[1] = (uint32_t)MW_INTXBAR1_FSIRXINT_SEL;
    group4_mask[1] = (uint32_t)MW_INTXBAR1_SDFMINT_SEL;
    group5_mask[1] = (uint32_t)MW_INTXBAR1_ECAPINT_SEL;
    group6_mask[1] = (uint32_t)MW_INTXBAR1_EQEPINT_SEL;
    group0_mask[2] = (uint32_t)MW_INTXBAR2_EPWMINT_SEL;
    group1_mask[2] = (uint32_t)MW_INTXBAR2_EPWMTZINT_SEL;
    group2_mask[2] = (uint32_t)MW_INTXBAR2_ADCINT_SEL;
    group3_mask[2] = (uint32_t)MW_INTXBAR2_FSIRXINT_SEL;
    group4_mask[2] = (uint32_t)MW_INTXBAR2_SDFMINT_SEL;
    group5_mask[2] = (uint32_t)MW_INTXBAR2_ECAPINT_SEL;
    group6_mask[2] = (uint32_t)MW_INTXBAR2_EQEPINT_SEL;
    group0_mask[3] = (uint32_t)MW_INTXBAR3_EPWMINT_SEL;
    group1_mask[3] = (uint32_t)MW_INTXBAR3_EPWMTZINT_SEL;
    group2_mask[3] = (uint32_t)MW_INTXBAR3_ADCINT_SEL;
    group3_mask[3] = (uint32_t)MW_INTXBAR3_FSIRXINT_SEL;
    group4_mask[3] = (uint32_t)MW_INTXBAR3_SDFMINT_SEL;
    group5_mask[3] = (uint32_t)MW_INTXBAR3_ECAPINT_SEL;
    group6_mask[3] = (uint32_t)MW_INTXBAR3_EQEPINT_SEL;
    group0_mask[4] = (uint32_t)MW_INTXBAR4_EPWMINT_SEL;
    group1_mask[4] = (uint32_t)MW_INTXBAR4_EPWMTZINT_SEL;
    group2_mask[4] = (uint32_t)MW_INTXBAR4_ADCINT_SEL;
    group3_mask[4] = (uint32_t)MW_INTXBAR4_FSIRXINT_SEL;
    group4_mask[4] = (uint32_t)MW_INTXBAR4_SDFMINT_SEL;
    group5_mask[4] = (uint32_t)MW_INTXBAR4_ECAPINT_SEL;
    group6_mask[4] = (uint32_t)MW_INTXBAR4_EQEPINT_SEL;
    group0_mask[5] = (uint32_t)MW_INTXBAR5_EPWMINT_SEL;
    group1_mask[5] = (uint32_t)MW_INTXBAR5_EPWMTZINT_SEL;
    group2_mask[5] = (uint32_t)MW_INTXBAR5_ADCINT_SEL;
    group3_mask[5] = (uint32_t)MW_INTXBAR5_FSIRXINT_SEL;
    group4_mask[5] = (uint32_t)MW_INTXBAR5_SDFMINT_SEL;
    group5_mask[5] = (uint32_t)MW_INTXBAR5_ECAPINT_SEL;
    group6_mask[5] = (uint32_t)MW_INTXBAR5_EQEPINT_SEL;
    group0_mask[6] = (uint32_t)MW_INTXBAR6_EPWMINT_SEL;
    group1_mask[6] = (uint32_t)MW_INTXBAR6_EPWMTZINT_SEL;
    group2_mask[6] = (uint32_t)MW_INTXBAR6_ADCINT_SEL;
    group3_mask[6] = (uint32_t)MW_INTXBAR6_FSIRXINT_SEL;
    group4_mask[6] = (uint32_t)MW_INTXBAR6_SDFMINT_SEL;
    group5_mask[6] = (uint32_t)MW_INTXBAR6_ECAPINT_SEL;
    group6_mask[6] = (uint32_t)MW_INTXBAR6_EQEPINT_SEL;
    group0_mask[7] = (uint32_t)MW_INTXBAR7_EPWMINT_SEL;
    group1_mask[7] = (uint32_t)MW_INTXBAR7_EPWMTZINT_SEL;
    group2_mask[7] = (uint32_t)MW_INTXBAR7_ADCINT_SEL;
    group3_mask[7] = (uint32_t)MW_INTXBAR7_FSIRXINT_SEL;
    group4_mask[7] = (uint32_t)MW_INTXBAR7_SDFMINT_SEL;
    group5_mask[7] = (uint32_t)MW_INTXBAR7_ECAPINT_SEL;
    group6_mask[7] = (uint32_t)MW_INTXBAR7_EQEPINT_SEL;
    group0_mask[8] = (uint32_t)MW_INTXBAR8_EPWMINT_SEL;
    group1_mask[8] = (uint32_t)MW_INTXBAR8_EPWMTZINT_SEL;
    group2_mask[8] = (uint32_t)MW_INTXBAR8_ADCINT_SEL;
    group3_mask[8] = (uint32_t)MW_INTXBAR8_FSIRXINT_SEL;
    group4_mask[8] = (uint32_t)MW_INTXBAR8_SDFMINT_SEL;
    group5_mask[8] = (uint32_t)MW_INTXBAR8_ECAPINT_SEL;
    group6_mask[8] = (uint32_t)MW_INTXBAR8_EQEPINT_SEL;
    group0_mask[9] = (uint32_t)MW_INTXBAR9_EPWMINT_SEL;
    group1_mask[9] = (uint32_t)MW_INTXBAR9_EPWMTZINT_SEL;
    group2_mask[9] = (uint32_t)MW_INTXBAR9_ADCINT_SEL;
    group3_mask[9] = (uint32_t)MW_INTXBAR9_FSIRXINT_SEL;
    group4_mask[9] = (uint32_t)MW_INTXBAR9_SDFMINT_SEL;
    group5_mask[9] = (uint32_t)MW_INTXBAR9_ECAPINT_SEL;
    group6_mask[9] = (uint32_t)MW_INTXBAR9_EQEPINT_SEL;
    group0_mask[10] = (uint32_t)MW_INTXBAR10_EPWMINT_SEL;
    group1_mask[10] = (uint32_t)MW_INTXBAR10_EPWMTZINT_SEL;
    group2_mask[10] = (uint32_t)MW_INTXBAR10_ADCINT_SEL;
    group3_mask[10] = (uint32_t)MW_INTXBAR10_FSIRXINT_SEL;
    group4_mask[10] = (uint32_t)MW_INTXBAR10_SDFMINT_SEL;
    group5_mask[10] = (uint32_t)MW_INTXBAR10_ECAPINT_SEL;
    group6_mask[10] = (uint32_t)MW_INTXBAR10_EQEPINT_SEL;
    group0_mask[11] = (uint32_t)MW_INTXBAR11_EPWMINT_SEL;
    group1_mask[11] = (uint32_t)MW_INTXBAR11_EPWMTZINT_SEL;
    group2_mask[11] = (uint32_t)MW_INTXBAR11_ADCINT_SEL;
    group3_mask[11] = (uint32_t)MW_INTXBAR11_FSIRXINT_SEL;
    group4_mask[11] = (uint32_t)MW_INTXBAR11_SDFMINT_SEL;
    group5_mask[11] = (uint32_t)MW_INTXBAR11_ECAPINT_SEL;
    group6_mask[11] = (uint32_t)MW_INTXBAR11_EQEPINT_SEL;
    group0_mask[12] = (uint32_t)MW_INTXBAR12_EPWMINT_SEL;
    group1_mask[12] = (uint32_t)MW_INTXBAR12_EPWMTZINT_SEL;
    group2_mask[12] = (uint32_t)MW_INTXBAR12_ADCINT_SEL;
    group3_mask[12] = (uint32_t)MW_INTXBAR12_FSIRXINT_SEL;
    group4_mask[12] = (uint32_t)MW_INTXBAR12_SDFMINT_SEL;
    group5_mask[12] = (uint32_t)MW_INTXBAR12_ECAPINT_SEL;
    group6_mask[12] = (uint32_t)MW_INTXBAR12_EQEPINT_SEL;
    group0_mask[13] = (uint32_t)MW_INTXBAR13_EPWMINT_SEL;
    group1_mask[13] = (uint32_t)MW_INTXBAR13_EPWMTZINT_SEL;
    group2_mask[13] = (uint32_t)MW_INTXBAR13_ADCINT_SEL;
    group3_mask[13] = (uint32_t)MW_INTXBAR13_FSIRXINT_SEL;
    group4_mask[13] = (uint32_t)MW_INTXBAR13_SDFMINT_SEL;
    group5_mask[13] = (uint32_t)MW_INTXBAR13_ECAPINT_SEL;
    group6_mask[13] = (uint32_t)MW_INTXBAR13_EQEPINT_SEL;
    group0_mask[14] = (uint32_t)MW_INTXBAR14_EPWMINT_SEL;
    group1_mask[14] = (uint32_t)MW_INTXBAR14_EPWMTZINT_SEL;
    group2_mask[14] = (uint32_t)MW_INTXBAR14_ADCINT_SEL;
    group3_mask[14] = (uint32_t)MW_INTXBAR14_FSIRXINT_SEL;
    group4_mask[14] = (uint32_t)MW_INTXBAR14_SDFMINT_SEL;
    group5_mask[14] = (uint32_t)MW_INTXBAR14_ECAPINT_SEL;
    group6_mask[14] = (uint32_t)MW_INTXBAR14_EQEPINT_SEL;
    group0_mask[15] = (uint32_t)MW_INTXBAR15_EPWMINT_SEL;
    group1_mask[15] = (uint32_t)MW_INTXBAR15_EPWMTZINT_SEL;
    group2_mask[15] = (uint32_t)MW_INTXBAR15_ADCINT_SEL;
    group3_mask[15] = (uint32_t)MW_INTXBAR15_FSIRXINT_SEL;
    group4_mask[15] = (uint32_t)MW_INTXBAR15_SDFMINT_SEL;
    group5_mask[15] = (uint32_t)MW_INTXBAR15_ECAPINT_SEL;
    group6_mask[15] = (uint32_t)MW_INTXBAR15_EQEPINT_SEL;
    group0_mask[16] = (uint32_t)MW_INTXBAR16_EPWMINT_SEL;
    group1_mask[16] = (uint32_t)MW_INTXBAR16_EPWMTZINT_SEL;
    group2_mask[16] = (uint32_t)MW_INTXBAR16_ADCINT_SEL;
    group3_mask[16] = (uint32_t)MW_INTXBAR16_FSIRXINT_SEL;
    group4_mask[16] = (uint32_t)MW_INTXBAR16_SDFMINT_SEL;
    group5_mask[16] = (uint32_t)MW_INTXBAR16_ECAPINT_SEL;
    group6_mask[16] = (uint32_t)MW_INTXBAR16_EQEPINT_SEL;
    group0_mask[17] = (uint32_t)MW_INTXBAR17_EPWMINT_SEL;
    group1_mask[17] = (uint32_t)MW_INTXBAR17_EPWMTZINT_SEL;
    group2_mask[17] = (uint32_t)MW_INTXBAR17_ADCINT_SEL;
    group3_mask[17] = (uint32_t)MW_INTXBAR17_FSIRXINT_SEL;
    group4_mask[17] = (uint32_t)MW_INTXBAR17_SDFMINT_SEL;
    group5_mask[17] = (uint32_t)MW_INTXBAR17_ECAPINT_SEL;
    group6_mask[17] = (uint32_t)MW_INTXBAR17_EQEPINT_SEL;
    group0_mask[18] = (uint32_t)MW_INTXBAR18_EPWMINT_SEL;
    group1_mask[18] = (uint32_t)MW_INTXBAR18_EPWMTZINT_SEL;
    group2_mask[18] = (uint32_t)MW_INTXBAR18_ADCINT_SEL;
    group3_mask[18] = (uint32_t)MW_INTXBAR18_FSIRXINT_SEL;
    group4_mask[18] = (uint32_t)MW_INTXBAR18_SDFMINT_SEL;
    group5_mask[18] = (uint32_t)MW_INTXBAR18_ECAPINT_SEL;
    group6_mask[18] = (uint32_t)MW_INTXBAR18_EQEPINT_SEL;
    group0_mask[19] = (uint32_t)MW_INTXBAR19_EPWMINT_SEL;
    group1_mask[19] = (uint32_t)MW_INTXBAR19_EPWMTZINT_SEL;
    group2_mask[19] = (uint32_t)MW_INTXBAR19_ADCINT_SEL;
    group3_mask[19] = (uint32_t)MW_INTXBAR19_FSIRXINT_SEL;
    group4_mask[19] = (uint32_t)MW_INTXBAR19_SDFMINT_SEL;
    group5_mask[19] = (uint32_t)MW_INTXBAR19_ECAPINT_SEL;
    group6_mask[19] = (uint32_t)MW_INTXBAR19_EQEPINT_SEL;
    group0_mask[20] = (uint32_t)MW_INTXBAR20_EPWMINT_SEL;
    group1_mask[20] = (uint32_t)MW_INTXBAR20_EPWMTZINT_SEL;
    group2_mask[20] = (uint32_t)MW_INTXBAR20_ADCINT_SEL;
    group3_mask[20] = (uint32_t)MW_INTXBAR20_FSIRXINT_SEL;
    group4_mask[20] = (uint32_t)MW_INTXBAR20_SDFMINT_SEL;
    group5_mask[20] = (uint32_t)MW_INTXBAR20_ECAPINT_SEL;
    group6_mask[20] = (uint32_t)MW_INTXBAR20_EQEPINT_SEL;
    group0_mask[21] = (uint32_t)MW_INTXBAR21_EPWMINT_SEL;
    group1_mask[21] = (uint32_t)MW_INTXBAR21_EPWMTZINT_SEL;
    group2_mask[21] = (uint32_t)MW_INTXBAR21_ADCINT_SEL;
    group3_mask[21] = (uint32_t)MW_INTXBAR21_FSIRXINT_SEL;
    group4_mask[21] = (uint32_t)MW_INTXBAR21_SDFMINT_SEL;
    group5_mask[21] = (uint32_t)MW_INTXBAR21_ECAPINT_SEL;
    group6_mask[21] = (uint32_t)MW_INTXBAR21_EQEPINT_SEL;
    group0_mask[22] = (uint32_t)MW_INTXBAR22_EPWMINT_SEL;
    group1_mask[22] = (uint32_t)MW_INTXBAR22_EPWMTZINT_SEL;
    group2_mask[22] = (uint32_t)MW_INTXBAR22_ADCINT_SEL;
    group3_mask[22] = (uint32_t)MW_INTXBAR22_FSIRXINT_SEL;
    group4_mask[22] = (uint32_t)MW_INTXBAR22_SDFMINT_SEL;
    group5_mask[22] = (uint32_t)MW_INTXBAR22_ECAPINT_SEL;
    group6_mask[22] = (uint32_t)MW_INTXBAR22_EQEPINT_SEL;
    group0_mask[23] = (uint32_t)MW_INTXBAR23_EPWMINT_SEL;
    group1_mask[23] = (uint32_t)MW_INTXBAR23_EPWMTZINT_SEL;
    group2_mask[23] = (uint32_t)MW_INTXBAR23_ADCINT_SEL;
    group3_mask[23] = (uint32_t)MW_INTXBAR23_FSIRXINT_SEL;
    group4_mask[23] = (uint32_t)MW_INTXBAR23_SDFMINT_SEL;
    group5_mask[23] = (uint32_t)MW_INTXBAR23_ECAPINT_SEL;
    group6_mask[23] = (uint32_t)MW_INTXBAR23_EQEPINT_SEL;
    group0_mask[24] = (uint32_t)MW_INTXBAR24_EPWMINT_SEL;
    group1_mask[24] = (uint32_t)MW_INTXBAR24_EPWMTZINT_SEL;
    group2_mask[24] = (uint32_t)MW_INTXBAR24_ADCINT_SEL;
    group3_mask[24] = (uint32_t)MW_INTXBAR24_FSIRXINT_SEL;
    group4_mask[24] = (uint32_t)MW_INTXBAR24_SDFMINT_SEL;
    group5_mask[24] = (uint32_t)MW_INTXBAR24_ECAPINT_SEL;
    group6_mask[24] = (uint32_t)MW_INTXBAR24_EQEPINT_SEL;
    group0_mask[25] = (uint32_t)MW_INTXBAR25_EPWMINT_SEL;
    group1_mask[25] = (uint32_t)MW_INTXBAR25_EPWMTZINT_SEL;
    group2_mask[25] = (uint32_t)MW_INTXBAR25_ADCINT_SEL;
    group3_mask[25] = (uint32_t)MW_INTXBAR25_FSIRXINT_SEL;
    group4_mask[25] = (uint32_t)MW_INTXBAR25_SDFMINT_SEL;
    group5_mask[25] = (uint32_t)MW_INTXBAR25_ECAPINT_SEL;
    group6_mask[25] = (uint32_t)MW_INTXBAR25_EQEPINT_SEL;
    group0_mask[26] = (uint32_t)MW_INTXBAR26_EPWMINT_SEL;
    group1_mask[26] = (uint32_t)MW_INTXBAR26_EPWMTZINT_SEL;
    group2_mask[26] = (uint32_t)MW_INTXBAR26_ADCINT_SEL;
    group3_mask[26] = (uint32_t)MW_INTXBAR26_FSIRXINT_SEL;
    group4_mask[26] = (uint32_t)MW_INTXBAR26_SDFMINT_SEL;
    group5_mask[26] = (uint32_t)MW_INTXBAR26_ECAPINT_SEL;
    group6_mask[26] = (uint32_t)MW_INTXBAR26_EQEPINT_SEL;
    group0_mask[27] = (uint32_t)MW_INTXBAR27_EPWMINT_SEL;
    group1_mask[27] = (uint32_t)MW_INTXBAR27_EPWMTZINT_SEL;
    group2_mask[27] = (uint32_t)MW_INTXBAR27_ADCINT_SEL;
    group3_mask[27] = (uint32_t)MW_INTXBAR27_FSIRXINT_SEL;
    group4_mask[27] = (uint32_t)MW_INTXBAR27_SDFMINT_SEL;
    group5_mask[27] = (uint32_t)MW_INTXBAR27_ECAPINT_SEL;
    group6_mask[27] = (uint32_t)MW_INTXBAR27_EQEPINT_SEL;
    group0_mask[28] = (uint32_t)MW_INTXBAR28_EPWMINT_SEL;
    group1_mask[28] = (uint32_t)MW_INTXBAR28_EPWMTZINT_SEL;
    group2_mask[28] = (uint32_t)MW_INTXBAR28_ADCINT_SEL;
    group3_mask[28] = (uint32_t)MW_INTXBAR28_FSIRXINT_SEL;
    group4_mask[28] = (uint32_t)MW_INTXBAR28_SDFMINT_SEL;
    group5_mask[28] = (uint32_t)MW_INTXBAR28_ECAPINT_SEL;
    group6_mask[28] = (uint32_t)MW_INTXBAR28_EQEPINT_SEL;
    group0_mask[29] = (uint32_t)MW_INTXBAR29_EPWMINT_SEL;
    group1_mask[29] = (uint32_t)MW_INTXBAR29_EPWMTZINT_SEL;
    group2_mask[29] = (uint32_t)MW_INTXBAR29_ADCINT_SEL;
    group3_mask[29] = (uint32_t)MW_INTXBAR29_FSIRXINT_SEL;
    group4_mask[29] = (uint32_t)MW_INTXBAR29_SDFMINT_SEL;
    group5_mask[29] = (uint32_t)MW_INTXBAR29_ECAPINT_SEL;
    group6_mask[29] = (uint32_t)MW_INTXBAR29_EQEPINT_SEL;
    group0_mask[30] = (uint32_t)MW_INTXBAR30_EPWMINT_SEL;
    group1_mask[30] = (uint32_t)MW_INTXBAR30_EPWMTZINT_SEL;
    group2_mask[30] = (uint32_t)MW_INTXBAR30_ADCINT_SEL;
    group3_mask[30] = (uint32_t)MW_INTXBAR30_FSIRXINT_SEL;
    group4_mask[30] = (uint32_t)MW_INTXBAR30_SDFMINT_SEL;
    group5_mask[30] = (uint32_t)MW_INTXBAR30_ECAPINT_SEL;
    group6_mask[30] = (uint32_t)MW_INTXBAR30_EQEPINT_SEL;
    group0_mask[31] = (uint32_t)MW_INTXBAR31_EPWMINT_SEL;
    group1_mask[31] = (uint32_t)MW_INTXBAR31_EPWMTZINT_SEL;
    group2_mask[31] = (uint32_t)MW_INTXBAR31_ADCINT_SEL;
    group3_mask[31] = (uint32_t)MW_INTXBAR31_FSIRXINT_SEL;
    group4_mask[31] = (uint32_t)MW_INTXBAR31_SDFMINT_SEL;
    group5_mask[31] = (uint32_t)MW_INTXBAR31_ECAPINT_SEL;
    group6_mask[31] = (uint32_t)MW_INTXBAR31_EQEPINT_SEL;   
   
             
    uint8_t out;

    for( out = 0 ; out < 32 ; out++ )
    {

        if(((group0_mask[out]!=0) | (group1_mask[out]!=0) | (group2_mask[out]!=0) | (group3_mask[out]!=0) | (group4_mask[out]!=0) | (group5_mask[out]!=0) | (group6_mask[out]!=0)))
        {
            SOC_xbarSelectInterruptXBarInputSource(CSL_CONTROLSS_INTXBAR_U_BASE, out, 
                                                group0_mask[out], group1_mask[out], 
                                                group2_mask[out], group3_mask[out], 
                                                group4_mask[out], group5_mask[out], 
                                                group6_mask[out]);
        }
        
    }

    // SOC_xbarSelectInterruptXBarInputSource(CSL_CONTROLSS_INTXBAR_U_BASE, 22, 0, 0, 0, 0, 0, ( INT_XBAR_ECAP0_INT ), 0);

}

void Drivers_gpioIntXbarOpen()
{
    uint8_t out;
    uint8_t output_val =0;
    for(out = 0; out < 30; out ++)
    {
        switch(out)
        {
        case 0:
            output_val = MW_GPIO_INT_XBAR0_OUTPUT_SEL;
            break;
        case 1:
            output_val = MW_GPIO_INT_XBAR1_OUTPUT_SEL;
            break;
        case 2:
            output_val = MW_GPIO_INT_XBAR2_OUTPUT_SEL;
            break;
        case 3:
            output_val = MW_GPIO_INT_XBAR3_OUTPUT_SEL;
            break;
        case 4:
            output_val = MW_GPIO_INT_XBAR_DMA0_OUTPUT_SEL;
            break;
        case 5:
            output_val = MW_GPIO_INT_XBAR_DMA1_OUTPUT_SEL;
            break;
        case 6:
            output_val = MW_GPIO_INT_XBAR_DMA2_OUTPUT_SEL;
            break;
        case 7:
            output_val = MW_GPIO_INT_XBAR_DMA3_OUTPUT_SEL;
            break;
        case 8:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNC10_OUTPUT_SEL;
            break;
        case 9:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNC11_OUTPUT_SEL;
            break;
        case 10:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNC12_OUTPUT_SEL;
            break;
        case 11:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNC13_OUTPUT_SEL;
            break;
        case 12:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNC14_OUTPUT_SEL;
            break;
        case 13:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNC15_OUTPUT_SEL;
            break;
        case 14:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE00_SEL;
            break;
        case 15:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE01_SEL;
            break;
        case 16:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE02_SEL;
            break;
        case 17:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE03_SEL;
            break;
        case 18:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE10_SEL;
            break;
        case 19:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE11_SEL;
            break;
        case 20:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE12_SEL;
            break;
        case 21:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE13_SEL;
            break;
        case 22:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE20_SEL;
            break;
        case 23:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE21_SEL;
            break;
        case 24:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE22_SEL;
            break;
        case 25:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE23_SEL;
            break;
        case 26:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE30_SEL;
            break;
        case 27:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE31_SEL;
            break;
        case 28:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE32_SEL;
            break;
        case 29:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE33_SEL;
            break;
        default:
            output_val = 0;
            break;
        }

        if(output_val!=0)
            SOC_xbarSelectGpioIntrXbarInputSource(CSL_GPIO_INTR_XBAR_U_BASE, out, output_val);
       
    }
}
void Drivers_inputXbarOpen(void)
{
    uint32_t InputXBAR_mask[32];

    InputXBAR_mask[0] = MW_INPUTXBAR0_SEL;
    InputXBAR_mask[1] = MW_INPUTXBAR1_SEL;
    InputXBAR_mask[2] = MW_INPUTXBAR2_SEL;
    InputXBAR_mask[3] = MW_INPUTXBAR3_SEL;
    InputXBAR_mask[4] = MW_INPUTXBAR4_SEL;
    InputXBAR_mask[5] = MW_INPUTXBAR5_SEL;
    InputXBAR_mask[6] = MW_INPUTXBAR6_SEL;
    InputXBAR_mask[7] = MW_INPUTXBAR7_SEL;
    InputXBAR_mask[8] = MW_INPUTXBAR8_SEL;
    InputXBAR_mask[9] = MW_INPUTXBAR9_SEL;
    InputXBAR_mask[10] = MW_INPUTXBAR10_SEL;
    InputXBAR_mask[11] = MW_INPUTXBAR11_SEL;
    InputXBAR_mask[12] = MW_INPUTXBAR12_SEL;
    InputXBAR_mask[13] = MW_INPUTXBAR13_SEL;
    InputXBAR_mask[14] = MW_INPUTXBAR14_SEL;
    InputXBAR_mask[15] = MW_INPUTXBAR15_SEL;
    InputXBAR_mask[16] = MW_INPUTXBAR16_SEL;
    InputXBAR_mask[17] = MW_INPUTXBAR17_SEL;
    InputXBAR_mask[18] = MW_INPUTXBAR18_SEL;
    InputXBAR_mask[19] = MW_INPUTXBAR19_SEL;
    InputXBAR_mask[20] = MW_INPUTXBAR20_SEL;
    InputXBAR_mask[21] = MW_INPUTXBAR21_SEL;
    InputXBAR_mask[22] = MW_INPUTXBAR22_SEL;
    InputXBAR_mask[23] = MW_INPUTXBAR23_SEL;
    InputXBAR_mask[24] = MW_INPUTXBAR24_SEL;
    InputXBAR_mask[25] = MW_INPUTXBAR25_SEL;
    InputXBAR_mask[26] = MW_INPUTXBAR26_SEL;
    InputXBAR_mask[27] = MW_INPUTXBAR27_SEL;
    InputXBAR_mask[28] = MW_INPUTXBAR28_SEL;
    InputXBAR_mask[29] = MW_INPUTXBAR29_SEL;
    InputXBAR_mask[30] = MW_INPUTXBAR30_SEL;
    InputXBAR_mask[31] = MW_INPUTXBAR31_SEL;
  
    for(int i = 0; i < 32; i++)
    {
        if(InputXBAR_mask[i]!=0)
        {
            if(InputXBAR_mask[i] < 144)
            {
                SOC_xbarSelectInputXBarInputSource(CSL_CONTROLSS_INPUTXBAR_U_BASE, i, 0, InputXBAR_mask[i], 0);
            }
            else
            {
                SOC_xbarSelectInputXBarInputSource(CSL_CONTROLSS_INPUTXBAR_U_BASE, i, 1, 0, InputXBAR_mask[i]-144); 
            }
        }
    }

}

void Drivers_EPWMXbarOpen()
{
    uint32_t group0_epwmxbar[30];
    uint32_t group1_epwmxbar[30];
    uint32_t group2_epwmxbar[30];
    uint32_t group3_epwmxbar[30];
    uint32_t group4_epwmxbar[30];
    uint32_t group5_epwmxbar[30];
    uint32_t group6_epwmxbar[30];
    uint32_t group7_epwmxbar[30];
    uint32_t group8_epwmxbar[30];
    uint32_t latch;

    bool XBARSet = false;

        group0_epwmxbar[0] = (uint32_t)MW_EPWMXBAR0_GROUP0_SEL;
    group1_epwmxbar[0] = (uint32_t)MW_EPWMXBAR0_GROUP1_SEL;
    group2_epwmxbar[0] = (uint32_t)MW_EPWMXBAR0_GROUP2_SEL;
    group3_epwmxbar[0] = (uint32_t)MW_EPWMXBAR0_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR0_GROUP4_SEL == 0)
    //     group4_epwmxbar[0] = EPWM_XBAR_0;
    // else
        group4_epwmxbar[0] = 0;
    group5_epwmxbar[0] = (uint32_t)MW_EPWMXBAR0_GROUP5_SEL;
    group6_epwmxbar[0] = (uint32_t)MW_EPWMXBAR0_GROUP6_SEL;
    group7_epwmxbar[0] = (uint32_t)MW_EPWMXBAR0_GROUP7_SEL;
    group8_epwmxbar[0] = (uint32_t)MW_EPWMXBAR0_GROUP8_SEL;

    group0_epwmxbar[1] = (uint32_t)MW_EPWMXBAR1_GROUP0_SEL;
    group1_epwmxbar[1] = (uint32_t)MW_EPWMXBAR1_GROUP1_SEL;
    group2_epwmxbar[1] = (uint32_t)MW_EPWMXBAR1_GROUP2_SEL;
    group3_epwmxbar[1] = (uint32_t)MW_EPWMXBAR1_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR1_GROUP4_SEL == 0)
    //     group4_epwmxbar[1] = EPWM_XBAR_1;
    // else
        group4_epwmxbar[1] = 0;
    group5_epwmxbar[1] = (uint32_t)MW_EPWMXBAR1_GROUP5_SEL;
    group6_epwmxbar[1] = (uint32_t)MW_EPWMXBAR1_GROUP6_SEL;
    group7_epwmxbar[1] = (uint32_t)MW_EPWMXBAR1_GROUP7_SEL;
    group8_epwmxbar[1] = (uint32_t)MW_EPWMXBAR1_GROUP8_SEL;

    group0_epwmxbar[2] = (uint32_t)MW_EPWMXBAR2_GROUP0_SEL;
    group1_epwmxbar[2] = (uint32_t)MW_EPWMXBAR2_GROUP1_SEL;
    group2_epwmxbar[2] = (uint32_t)MW_EPWMXBAR2_GROUP2_SEL;
    group3_epwmxbar[2] = (uint32_t)MW_EPWMXBAR2_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR2_GROUP4_SEL == 0)
    //     group4_epwmxbar[2] = EPWM_XBAR_2;
    // else
        group4_epwmxbar[2] = 0;
    group5_epwmxbar[2] = (uint32_t)MW_EPWMXBAR2_GROUP5_SEL;
    group6_epwmxbar[2] = (uint32_t)MW_EPWMXBAR2_GROUP6_SEL;
    group7_epwmxbar[2] = (uint32_t)MW_EPWMXBAR2_GROUP7_SEL;
    group8_epwmxbar[2] = (uint32_t)MW_EPWMXBAR2_GROUP8_SEL;

    group0_epwmxbar[3] = (uint32_t)MW_EPWMXBAR3_GROUP0_SEL;
    group1_epwmxbar[3] = (uint32_t)MW_EPWMXBAR3_GROUP1_SEL;
    group2_epwmxbar[3] = (uint32_t)MW_EPWMXBAR3_GROUP2_SEL;
    group3_epwmxbar[3] = (uint32_t)MW_EPWMXBAR3_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR3_GROUP4_SEL == 0)
    //     group4_epwmxbar[3] = EPWM_XBAR_3;
    // else
        group4_epwmxbar[3] = 0;
    group5_epwmxbar[3] = (uint32_t)MW_EPWMXBAR3_GROUP5_SEL;
    group6_epwmxbar[3] = (uint32_t)MW_EPWMXBAR3_GROUP6_SEL;
    group7_epwmxbar[3] = (uint32_t)MW_EPWMXBAR3_GROUP7_SEL;
    group8_epwmxbar[3] = (uint32_t)MW_EPWMXBAR3_GROUP8_SEL;

    group0_epwmxbar[4] = (uint32_t)MW_EPWMXBAR4_GROUP0_SEL;
    group1_epwmxbar[4] = (uint32_t)MW_EPWMXBAR4_GROUP1_SEL;
    group2_epwmxbar[4] = (uint32_t)MW_EPWMXBAR4_GROUP2_SEL;
    group3_epwmxbar[4] = (uint32_t)MW_EPWMXBAR4_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR4_GROUP4_SEL == 0)
    //     group4_epwmxbar[4] = EPWM_XBAR_4;
    // else
        group4_epwmxbar[4] = 0;
    group5_epwmxbar[4] = (uint32_t)MW_EPWMXBAR4_GROUP5_SEL;
    group6_epwmxbar[4] = (uint32_t)MW_EPWMXBAR4_GROUP6_SEL;
    group7_epwmxbar[4] = (uint32_t)MW_EPWMXBAR4_GROUP7_SEL;
    group8_epwmxbar[4] = (uint32_t)MW_EPWMXBAR4_GROUP8_SEL;

    group0_epwmxbar[5] = (uint32_t)MW_EPWMXBAR5_GROUP0_SEL;
    group1_epwmxbar[5] = (uint32_t)MW_EPWMXBAR5_GROUP1_SEL;
    group2_epwmxbar[5] = (uint32_t)MW_EPWMXBAR5_GROUP2_SEL;
    group3_epwmxbar[5] = (uint32_t)MW_EPWMXBAR5_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR5_GROUP4_SEL == 0)
    //     group4_epwmxbar[5] = EPWM_XBAR_5;
    // else
        group4_epwmxbar[5] = 0;
    group5_epwmxbar[5] = (uint32_t)MW_EPWMXBAR5_GROUP5_SEL;
    group6_epwmxbar[5] = (uint32_t)MW_EPWMXBAR5_GROUP6_SEL;
    group7_epwmxbar[5] = (uint32_t)MW_EPWMXBAR5_GROUP7_SEL;
    group8_epwmxbar[5] = (uint32_t)MW_EPWMXBAR5_GROUP8_SEL;

    group0_epwmxbar[6] = (uint32_t)MW_EPWMXBAR6_GROUP0_SEL;
    group1_epwmxbar[6] = (uint32_t)MW_EPWMXBAR6_GROUP1_SEL;
    group2_epwmxbar[6] = (uint32_t)MW_EPWMXBAR6_GROUP2_SEL;
    group3_epwmxbar[6] = (uint32_t)MW_EPWMXBAR6_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR6_GROUP4_SEL == 0)
    //     group4_epwmxbar[6] = EPWM_XBAR_6;
    // else
        group4_epwmxbar[6] = 0;
    group5_epwmxbar[6] = (uint32_t)MW_EPWMXBAR6_GROUP5_SEL;
    group6_epwmxbar[6] = (uint32_t)MW_EPWMXBAR6_GROUP6_SEL;
    group7_epwmxbar[6] = (uint32_t)MW_EPWMXBAR6_GROUP7_SEL;
    group8_epwmxbar[6] = (uint32_t)MW_EPWMXBAR6_GROUP8_SEL;

    group0_epwmxbar[7] = (uint32_t)MW_EPWMXBAR7_GROUP0_SEL;
    group1_epwmxbar[7] = (uint32_t)MW_EPWMXBAR7_GROUP1_SEL;
    group2_epwmxbar[7] = (uint32_t)MW_EPWMXBAR7_GROUP2_SEL;
    group3_epwmxbar[7] = (uint32_t)MW_EPWMXBAR7_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR7_GROUP4_SEL == 0)
    //     group4_epwmxbar[7] = EPWM_XBAR_7;
    // else
        group4_epwmxbar[7] = 0;
    group5_epwmxbar[7] = (uint32_t)MW_EPWMXBAR7_GROUP5_SEL;
    group6_epwmxbar[7] = (uint32_t)MW_EPWMXBAR7_GROUP6_SEL;
    group7_epwmxbar[7] = (uint32_t)MW_EPWMXBAR7_GROUP7_SEL;
    group8_epwmxbar[7] = (uint32_t)MW_EPWMXBAR7_GROUP8_SEL;

    group0_epwmxbar[8] = (uint32_t)MW_EPWMXBAR8_GROUP0_SEL;
    group1_epwmxbar[8] = (uint32_t)MW_EPWMXBAR8_GROUP1_SEL;
    group2_epwmxbar[8] = (uint32_t)MW_EPWMXBAR8_GROUP2_SEL;
    group3_epwmxbar[8] = (uint32_t)MW_EPWMXBAR8_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR8_GROUP4_SEL == 0)
    //     group4_epwmxbar[8] = EPWM_XBAR_8;
    // else
        group4_epwmxbar[8] = 0;
    group5_epwmxbar[8] = (uint32_t)MW_EPWMXBAR8_GROUP5_SEL;
    group6_epwmxbar[8] = (uint32_t)MW_EPWMXBAR8_GROUP6_SEL;
    group7_epwmxbar[8] = (uint32_t)MW_EPWMXBAR8_GROUP7_SEL;
    group8_epwmxbar[8] = (uint32_t)MW_EPWMXBAR8_GROUP8_SEL;

    group0_epwmxbar[9] = (uint32_t)MW_EPWMXBAR9_GROUP0_SEL;
    group1_epwmxbar[9] = (uint32_t)MW_EPWMXBAR9_GROUP1_SEL;
    group2_epwmxbar[9] = (uint32_t)MW_EPWMXBAR9_GROUP2_SEL;
    group3_epwmxbar[9] = (uint32_t)MW_EPWMXBAR9_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR9_GROUP4_SEL == 0)
    //     group4_epwmxbar[9] = EPWM_XBAR_9;
    // else
        group4_epwmxbar[9] = 0;
    group5_epwmxbar[9] = (uint32_t)MW_EPWMXBAR9_GROUP5_SEL;
    group6_epwmxbar[9] = (uint32_t)MW_EPWMXBAR9_GROUP6_SEL;
    group7_epwmxbar[9] = (uint32_t)MW_EPWMXBAR9_GROUP7_SEL;
    group8_epwmxbar[9] = (uint32_t)MW_EPWMXBAR9_GROUP8_SEL;

   group0_epwmxbar[10] = (uint32_t)MW_EPWMXBAR10_GROUP0_SEL;
    group1_epwmxbar[10] = (uint32_t)MW_EPWMXBAR10_GROUP1_SEL;
    group2_epwmxbar[10] = (uint32_t)MW_EPWMXBAR10_GROUP2_SEL;
    group3_epwmxbar[10] = (uint32_t)MW_EPWMXBAR10_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR0_GROUP4_SEL == 0)
    //     group4_epwmxbar[10] = EPWM_XBAR_10;
    // else
        group4_epwmxbar[10] = 0;
    group5_epwmxbar[10] = (uint32_t)MW_EPWMXBAR10_GROUP5_SEL;
    group6_epwmxbar[10] = (uint32_t)MW_EPWMXBAR10_GROUP6_SEL;
    group7_epwmxbar[10] = (uint32_t)MW_EPWMXBAR10_GROUP7_SEL;
    group8_epwmxbar[10] = (uint32_t)MW_EPWMXBAR10_GROUP8_SEL;

    group0_epwmxbar[11] = (uint32_t)MW_EPWMXBAR11_GROUP0_SEL;
    group1_epwmxbar[11] = (uint32_t)MW_EPWMXBAR11_GROUP1_SEL;
    group2_epwmxbar[11] = (uint32_t)MW_EPWMXBAR11_GROUP2_SEL;
    group3_epwmxbar[11] = (uint32_t)MW_EPWMXBAR11_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR1_GROUP4_SEL == 0)
    //     group4_epwmxbar[11] = EPWM_XBAR_11;
    // else
        group4_epwmxbar[11] = 0;
    group5_epwmxbar[11] = (uint32_t)MW_EPWMXBAR11_GROUP5_SEL;
    group6_epwmxbar[11] = (uint32_t)MW_EPWMXBAR11_GROUP6_SEL;
    group7_epwmxbar[11] = (uint32_t)MW_EPWMXBAR11_GROUP7_SEL;
    group8_epwmxbar[11] = (uint32_t)MW_EPWMXBAR11_GROUP8_SEL;

    group0_epwmxbar[12] = (uint32_t)MW_EPWMXBAR12_GROUP0_SEL;
    group1_epwmxbar[12] = (uint32_t)MW_EPWMXBAR12_GROUP1_SEL;
    group2_epwmxbar[12] = (uint32_t)MW_EPWMXBAR12_GROUP2_SEL;
    group3_epwmxbar[12] = (uint32_t)MW_EPWMXBAR12_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR2_GROUP4_SEL == 0)
    //     group4_epwmxbar[12] = EPWM_XBAR_12;
    // else
        group4_epwmxbar[12] = 0;
    group5_epwmxbar[12] = (uint32_t)MW_EPWMXBAR12_GROUP5_SEL;
    group6_epwmxbar[12] = (uint32_t)MW_EPWMXBAR12_GROUP6_SEL;
    group7_epwmxbar[12] = (uint32_t)MW_EPWMXBAR12_GROUP7_SEL;
    group8_epwmxbar[12] = (uint32_t)MW_EPWMXBAR12_GROUP8_SEL;

    group0_epwmxbar[13] = (uint32_t)MW_EPWMXBAR13_GROUP0_SEL;
    group1_epwmxbar[13] = (uint32_t)MW_EPWMXBAR13_GROUP1_SEL;
    group2_epwmxbar[13] = (uint32_t)MW_EPWMXBAR13_GROUP2_SEL;
    group3_epwmxbar[13] = (uint32_t)MW_EPWMXBAR13_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR3_GROUP4_SEL == 0)
    //     group4_epwmxbar[13] = EPWM_XBAR_13;
    // else
        group4_epwmxbar[13] = 0;
    group5_epwmxbar[13] = (uint32_t)MW_EPWMXBAR13_GROUP5_SEL;
    group6_epwmxbar[13] = (uint32_t)MW_EPWMXBAR13_GROUP6_SEL;
    group7_epwmxbar[13] = (uint32_t)MW_EPWMXBAR13_GROUP7_SEL;
    group8_epwmxbar[13] = (uint32_t)MW_EPWMXBAR13_GROUP8_SEL;

    group0_epwmxbar[14] = (uint32_t)MW_EPWMXBAR14_GROUP0_SEL;
    group1_epwmxbar[14] = (uint32_t)MW_EPWMXBAR14_GROUP1_SEL;
    group2_epwmxbar[14] = (uint32_t)MW_EPWMXBAR14_GROUP2_SEL;
    group3_epwmxbar[14] = (uint32_t)MW_EPWMXBAR14_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR4_GROUP4_SEL == 0)
    //     group4_epwmxbar[14] = EPWM_XBAR_14;
    // else
        group4_epwmxbar[14] = 0;
    group5_epwmxbar[14] = (uint32_t)MW_EPWMXBAR14_GROUP5_SEL;
    group6_epwmxbar[14] = (uint32_t)MW_EPWMXBAR14_GROUP6_SEL;
    group7_epwmxbar[14] = (uint32_t)MW_EPWMXBAR14_GROUP7_SEL;
    group8_epwmxbar[14] = (uint32_t)MW_EPWMXBAR14_GROUP8_SEL;

    group0_epwmxbar[15] = (uint32_t)MW_EPWMXBAR15_GROUP0_SEL;
    group1_epwmxbar[15] = (uint32_t)MW_EPWMXBAR15_GROUP1_SEL;
    group2_epwmxbar[15] = (uint32_t)MW_EPWMXBAR15_GROUP2_SEL;
    group3_epwmxbar[15] = (uint32_t)MW_EPWMXBAR15_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR5_GROUP4_SEL == 0)
    //     group4_epwmxbar[15] = EPWM_XBAR_15;
    // else
        group4_epwmxbar[15] = 0;
    group5_epwmxbar[15] = (uint32_t)MW_EPWMXBAR15_GROUP5_SEL;
    group6_epwmxbar[15] = (uint32_t)MW_EPWMXBAR15_GROUP6_SEL;
    group7_epwmxbar[15] = (uint32_t)MW_EPWMXBAR15_GROUP7_SEL;
    group8_epwmxbar[15] = (uint32_t)MW_EPWMXBAR15_GROUP8_SEL;

    group0_epwmxbar[16] = (uint32_t)MW_EPWMXBAR16_GROUP0_SEL;
    group1_epwmxbar[16] = (uint32_t)MW_EPWMXBAR16_GROUP1_SEL;
    group2_epwmxbar[16] = (uint32_t)MW_EPWMXBAR16_GROUP2_SEL;
    group3_epwmxbar[16] = (uint32_t)MW_EPWMXBAR16_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR6_GROUP4_SEL == 0)
    //     group4_epwmxbar[16] = EPWM_XBAR_16;
    // else
        group4_epwmxbar[16] = 0;
    group5_epwmxbar[16] = (uint32_t)MW_EPWMXBAR16_GROUP5_SEL;
    group6_epwmxbar[16] = (uint32_t)MW_EPWMXBAR16_GROUP6_SEL;
    group7_epwmxbar[16] = (uint32_t)MW_EPWMXBAR16_GROUP7_SEL;
    group8_epwmxbar[16] = (uint32_t)MW_EPWMXBAR16_GROUP8_SEL;

    group0_epwmxbar[17] = (uint32_t)MW_EPWMXBAR17_GROUP0_SEL;
    group1_epwmxbar[17] = (uint32_t)MW_EPWMXBAR17_GROUP1_SEL;
    group2_epwmxbar[17] = (uint32_t)MW_EPWMXBAR17_GROUP2_SEL;
    group3_epwmxbar[17] = (uint32_t)MW_EPWMXBAR17_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR7_GROUP4_SEL == 0)
    //     group4_epwmxbar[17] = EPWM_XBAR_17;
    // else
        group4_epwmxbar[17] = 0;
    group5_epwmxbar[17] = (uint32_t)MW_EPWMXBAR17_GROUP5_SEL;
    group6_epwmxbar[17] = (uint32_t)MW_EPWMXBAR17_GROUP6_SEL;
    group7_epwmxbar[17] = (uint32_t)MW_EPWMXBAR17_GROUP7_SEL;
    group8_epwmxbar[17] = (uint32_t)MW_EPWMXBAR17_GROUP8_SEL;

    group0_epwmxbar[18] = (uint32_t)MW_EPWMXBAR18_GROUP0_SEL;
    group1_epwmxbar[18] = (uint32_t)MW_EPWMXBAR18_GROUP1_SEL;
    group2_epwmxbar[18] = (uint32_t)MW_EPWMXBAR18_GROUP2_SEL;
    group3_epwmxbar[18] = (uint32_t)MW_EPWMXBAR18_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR8_GROUP4_SEL == 0)
    //     group4_epwmxbar[18] = EPWM_XBAR_18;
    // else
        group4_epwmxbar[18] = 0;
    group5_epwmxbar[18] = (uint32_t)MW_EPWMXBAR18_GROUP5_SEL;
    group6_epwmxbar[18] = (uint32_t)MW_EPWMXBAR18_GROUP6_SEL;
    group7_epwmxbar[18] = (uint32_t)MW_EPWMXBAR18_GROUP7_SEL;
    group8_epwmxbar[18] = (uint32_t)MW_EPWMXBAR18_GROUP8_SEL;

    group0_epwmxbar[19] = (uint32_t)MW_EPWMXBAR19_GROUP0_SEL;
    group1_epwmxbar[19] = (uint32_t)MW_EPWMXBAR19_GROUP1_SEL;
    group2_epwmxbar[19] = (uint32_t)MW_EPWMXBAR19_GROUP2_SEL;
    group3_epwmxbar[19] = (uint32_t)MW_EPWMXBAR19_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR9_GROUP4_SEL == 0)
    //     group4_epwmxbar[19] = EPWM_XBAR_19;
    // else
        group4_epwmxbar[19] = 0;
    group5_epwmxbar[19] = (uint32_t)MW_EPWMXBAR19_GROUP5_SEL;
    group6_epwmxbar[19] = (uint32_t)MW_EPWMXBAR19_GROUP6_SEL;
    group7_epwmxbar[19] = (uint32_t)MW_EPWMXBAR19_GROUP7_SEL;
    group8_epwmxbar[19] = (uint32_t)MW_EPWMXBAR19_GROUP8_SEL;

   group0_epwmxbar[20] = (uint32_t)MW_EPWMXBAR20_GROUP0_SEL;
    group1_epwmxbar[20] = (uint32_t)MW_EPWMXBAR20_GROUP1_SEL;
    group2_epwmxbar[20] = (uint32_t)MW_EPWMXBAR20_GROUP2_SEL;
    group3_epwmxbar[20] = (uint32_t)MW_EPWMXBAR20_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR0_GROUP4_SEL == 0)
    //     group4_epwmxbar[20] = EPWM_XBAR_20;
    // else
        group4_epwmxbar[20] = 0;
    group5_epwmxbar[20] = (uint32_t)MW_EPWMXBAR20_GROUP5_SEL;
    group6_epwmxbar[20] = (uint32_t)MW_EPWMXBAR20_GROUP6_SEL;
    group7_epwmxbar[20] = (uint32_t)MW_EPWMXBAR20_GROUP7_SEL;
    group8_epwmxbar[20] = (uint32_t)MW_EPWMXBAR20_GROUP8_SEL;

    group0_epwmxbar[21] = (uint32_t)MW_EPWMXBAR21_GROUP0_SEL;
    group1_epwmxbar[21] = (uint32_t)MW_EPWMXBAR21_GROUP1_SEL;
    group2_epwmxbar[21] = (uint32_t)MW_EPWMXBAR21_GROUP2_SEL;
    group3_epwmxbar[21] = (uint32_t)MW_EPWMXBAR21_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR2_GROUP4_SEL == 0)
    //     group4_epwmxbar[21] = EPWM_XBAR_21;
    // else
        group4_epwmxbar[21] = 0;
    group5_epwmxbar[21] = (uint32_t)MW_EPWMXBAR21_GROUP5_SEL;
    group6_epwmxbar[21] = (uint32_t)MW_EPWMXBAR21_GROUP6_SEL;
    group7_epwmxbar[21] = (uint32_t)MW_EPWMXBAR21_GROUP7_SEL;
    group8_epwmxbar[21] = (uint32_t)MW_EPWMXBAR21_GROUP8_SEL;

    group0_epwmxbar[22] = (uint32_t)MW_EPWMXBAR22_GROUP0_SEL;
    group1_epwmxbar[22] = (uint32_t)MW_EPWMXBAR22_GROUP1_SEL;
    group2_epwmxbar[22] = (uint32_t)MW_EPWMXBAR22_GROUP2_SEL;
    group3_epwmxbar[22] = (uint32_t)MW_EPWMXBAR22_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR2_GROUP4_SEL == 0)
    //     group4_epwmxbar[22] = EPWM_XBAR_22;
    // else
        group4_epwmxbar[22] = 0;
    group5_epwmxbar[22] = (uint32_t)MW_EPWMXBAR22_GROUP5_SEL;
    group6_epwmxbar[22] = (uint32_t)MW_EPWMXBAR22_GROUP6_SEL;
    group7_epwmxbar[22] = (uint32_t)MW_EPWMXBAR22_GROUP7_SEL;
    group8_epwmxbar[22] = (uint32_t)MW_EPWMXBAR22_GROUP8_SEL;

    group0_epwmxbar[23] = (uint32_t)MW_EPWMXBAR23_GROUP0_SEL;
    group1_epwmxbar[23] = (uint32_t)MW_EPWMXBAR23_GROUP1_SEL;
    group2_epwmxbar[23] = (uint32_t)MW_EPWMXBAR23_GROUP2_SEL;
    group3_epwmxbar[23] = (uint32_t)MW_EPWMXBAR23_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR3_GROUP4_SEL == 0)
    //     group4_epwmxbar[23] = EPWM_XBAR_23;
    // else
        group4_epwmxbar[23] = 0;
    group5_epwmxbar[23] = (uint32_t)MW_EPWMXBAR23_GROUP5_SEL;
    group6_epwmxbar[23] = (uint32_t)MW_EPWMXBAR23_GROUP6_SEL;
    group7_epwmxbar[23] = (uint32_t)MW_EPWMXBAR23_GROUP7_SEL;
    group8_epwmxbar[23] = (uint32_t)MW_EPWMXBAR23_GROUP8_SEL;

    group0_epwmxbar[24] = (uint32_t)MW_EPWMXBAR24_GROUP0_SEL;
    group1_epwmxbar[24] = (uint32_t)MW_EPWMXBAR24_GROUP1_SEL;
    group2_epwmxbar[24] = (uint32_t)MW_EPWMXBAR24_GROUP2_SEL;
    group3_epwmxbar[24] = (uint32_t)MW_EPWMXBAR24_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR4_GROUP4_SEL == 0)
    //     group4_epwmxbar[24] = EPWM_XBAR_24;
    // else
        group4_epwmxbar[24] = 0;
    group5_epwmxbar[24] = (uint32_t)MW_EPWMXBAR24_GROUP5_SEL;
    group6_epwmxbar[24] = (uint32_t)MW_EPWMXBAR24_GROUP6_SEL;
    group7_epwmxbar[24] = (uint32_t)MW_EPWMXBAR24_GROUP7_SEL;
    group8_epwmxbar[24] = (uint32_t)MW_EPWMXBAR24_GROUP8_SEL;

    group0_epwmxbar[25] = (uint32_t)MW_EPWMXBAR25_GROUP0_SEL;
    group1_epwmxbar[25] = (uint32_t)MW_EPWMXBAR25_GROUP1_SEL;
    group2_epwmxbar[25] = (uint32_t)MW_EPWMXBAR25_GROUP2_SEL;
    group3_epwmxbar[25] = (uint32_t)MW_EPWMXBAR25_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR5_GROUP4_SEL == 0)
    //     group4_epwmxbar[25] = EPWM_XBAR_25;
    // else
        group4_epwmxbar[25] = 0;
    group5_epwmxbar[25] = (uint32_t)MW_EPWMXBAR25_GROUP5_SEL;
    group6_epwmxbar[25] = (uint32_t)MW_EPWMXBAR25_GROUP6_SEL;
    group7_epwmxbar[25] = (uint32_t)MW_EPWMXBAR25_GROUP7_SEL;
    group8_epwmxbar[25] = (uint32_t)MW_EPWMXBAR25_GROUP8_SEL;

    group0_epwmxbar[26] = (uint32_t)MW_EPWMXBAR26_GROUP0_SEL;
    group1_epwmxbar[26] = (uint32_t)MW_EPWMXBAR26_GROUP1_SEL;
    group2_epwmxbar[26] = (uint32_t)MW_EPWMXBAR26_GROUP2_SEL;
    group3_epwmxbar[26] = (uint32_t)MW_EPWMXBAR26_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR6_GROUP4_SEL == 0)
    //     group4_epwmxbar[26] = EPWM_XBAR_26;
    // else
        group4_epwmxbar[26] = 0;
    group5_epwmxbar[26] = (uint32_t)MW_EPWMXBAR26_GROUP5_SEL;
    group6_epwmxbar[26] = (uint32_t)MW_EPWMXBAR26_GROUP6_SEL;
    group7_epwmxbar[26] = (uint32_t)MW_EPWMXBAR26_GROUP7_SEL;
    group8_epwmxbar[26] = (uint32_t)MW_EPWMXBAR26_GROUP8_SEL;

    group0_epwmxbar[27] = (uint32_t)MW_EPWMXBAR27_GROUP0_SEL;
    group1_epwmxbar[27] = (uint32_t)MW_EPWMXBAR27_GROUP1_SEL;
    group2_epwmxbar[27] = (uint32_t)MW_EPWMXBAR27_GROUP2_SEL;
    group3_epwmxbar[27] = (uint32_t)MW_EPWMXBAR27_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR7_GROUP4_SEL == 0)
    //     group4_epwmxbar[27] = EPWM_XBAR_27;
    // else
        group4_epwmxbar[27] = 0;
    group5_epwmxbar[27] = (uint32_t)MW_EPWMXBAR27_GROUP5_SEL;
    group6_epwmxbar[27] = (uint32_t)MW_EPWMXBAR27_GROUP6_SEL;
    group7_epwmxbar[27] = (uint32_t)MW_EPWMXBAR27_GROUP7_SEL;
    group8_epwmxbar[27] = (uint32_t)MW_EPWMXBAR27_GROUP8_SEL;

    group0_epwmxbar[28] = (uint32_t)MW_EPWMXBAR28_GROUP0_SEL;
    group1_epwmxbar[28] = (uint32_t)MW_EPWMXBAR28_GROUP1_SEL;
    group2_epwmxbar[28] = (uint32_t)MW_EPWMXBAR28_GROUP2_SEL;
    group3_epwmxbar[28] = (uint32_t)MW_EPWMXBAR28_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR8_GROUP4_SEL == 0)
    //     group4_epwmxbar[28] = EPWM_XBAR_28;
    // else
        group4_epwmxbar[28] = 0;
    group5_epwmxbar[28] = (uint32_t)MW_EPWMXBAR28_GROUP5_SEL;
    group6_epwmxbar[28] = (uint32_t)MW_EPWMXBAR28_GROUP6_SEL;
    group7_epwmxbar[28] = (uint32_t)MW_EPWMXBAR28_GROUP7_SEL;
    group8_epwmxbar[28] = (uint32_t)MW_EPWMXBAR28_GROUP8_SEL;

    group0_epwmxbar[29] = (uint32_t)MW_EPWMXBAR29_GROUP0_SEL;
    group1_epwmxbar[29] = (uint32_t)MW_EPWMXBAR29_GROUP1_SEL;
    group2_epwmxbar[29] = (uint32_t)MW_EPWMXBAR29_GROUP2_SEL;
    group3_epwmxbar[29] = (uint32_t)MW_EPWMXBAR29_GROUP3_SEL;
    // if((uint32_t)MW_EPWMXBAR9_GROUP4_SEL == 0)
    //     group4_epwmxbar[29] = EPWM_XBAR_29;
    // else
        group4_epwmxbar[29] = 0;
    group5_epwmxbar[29] = (uint32_t)MW_EPWMXBAR29_GROUP5_SEL;
    group6_epwmxbar[29] = (uint32_t)MW_EPWMXBAR29_GROUP6_SEL;
    group7_epwmxbar[29] = (uint32_t)MW_EPWMXBAR29_GROUP7_SEL;
    group8_epwmxbar[29] = (uint32_t)MW_EPWMXBAR29_GROUP8_SEL;

    latch = (uint8_t)MW_EPWM_XBAR_INV_LATCH_SEL;

   
    for(uint8_t i = 0; i< 30; i++)
    {
        if(((group0_epwmxbar[i]!=0) | (group1_epwmxbar[i]!=0) | (group2_epwmxbar[i]!=0) | (group3_epwmxbar[i]!=0) | (group4_epwmxbar[i]!=0) | (group5_epwmxbar[i]!=0) | (group6_epwmxbar[i]!=0) | (group7_epwmxbar[i]!=0) | (group8_epwmxbar[i]!=0)))
        {
            SOC_xbarSelectPWMXBarInputSource(CSL_CONTROLSS_PWMXBAR_U_BASE, i, group0_epwmxbar[i], group1_epwmxbar[i], group2_epwmxbar[i], group3_epwmxbar[i], group4_epwmxbar[i], group5_epwmxbar[i], group6_epwmxbar[i], group7_epwmxbar[i], group8_epwmxbar[i]);
        }   
    }

    SOC_xbarInvertPWMXBarOutputSignalBeforeLatch(CSL_CONTROLSS_PWMXBAR_U_BASE, latch);

    // SOC_xbarSelectPWMXBarInputSource(CSL_CONTROLSS_PWMXBAR_U_BASE, 3, ( EPWM_XBAR_CMPSSA1_CTRIPH ), 0, 0, 0, 0, 0, 0, 0, 0);
    // SOC_xbarInvertPWMXBarOutputSignalBeforeLatch(CSL_CONTROLSS_PWMXBAR_U_BASE, 0);

}

void Drivers_outputXbarOpen(void)
{
    uint32_t group0_outputxbar[16];
    uint32_t group1_outputxbar[16];
    uint32_t group2_outputxbar[16];
    uint32_t group3_outputxbar[16];
    uint32_t group4_outputxbar[16];
    uint32_t group5_outputxbar[16];
    uint32_t group6_outputxbar[16];
    uint32_t group7_outputxbar[16];
    uint32_t group8_outputxbar[16];
    uint32_t group9_outputxbar[16];
    uint32_t group10_outputxbar[16];

    uint32_t invertOutputBeforeLatch[16];
    uint32_t invertOutput[16];
    uint32_t latchSignalSource[16];
    uint32_t stretchedPulseOutput[16];
    uint32_t stretchedPulseLength[16];

    group0_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP0_SEL;
    group1_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP1_SEL;
    group2_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP2_SEL;
    group3_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP3_SEL;
    group4_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP4_SEL;
    group5_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP5_SEL;
    group6_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP6_SEL;
    group7_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP7_SEL;
    group8_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP8_SEL;
    group9_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP9_SEL;
    group10_outputxbar[0] = (uint32_t)MW_OUTPUTXBAR0_GROUP10_SEL;
    group0_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP0_SEL;
    group1_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP1_SEL;
    group2_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP2_SEL;
    group3_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP3_SEL;
    group4_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP4_SEL;
    group5_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP5_SEL;
    group6_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP6_SEL;
    group7_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP7_SEL;
    group8_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP8_SEL;
    group9_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP9_SEL;
    group10_outputxbar[1] = (uint32_t)MW_OUTPUTXBAR1_GROUP10_SEL;
    group0_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP0_SEL;
    group1_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP1_SEL;
    group2_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP2_SEL;
    group3_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP3_SEL;
    group4_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP4_SEL;
    group5_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP5_SEL;
    group6_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP6_SEL;
    group7_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP7_SEL;
    group8_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP8_SEL;
    group9_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP9_SEL;
    group10_outputxbar[2] = (uint32_t)MW_OUTPUTXBAR2_GROUP10_SEL;
    group0_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP0_SEL;
    group1_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP1_SEL;
    group2_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP2_SEL;
    group3_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP3_SEL;
    group4_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP4_SEL;
    group5_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP5_SEL;
    group6_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP6_SEL;
    group7_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP7_SEL;
    group8_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP8_SEL;
    group9_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP9_SEL;
    group10_outputxbar[3] = (uint32_t)MW_OUTPUTXBAR3_GROUP10_SEL;
    group0_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP0_SEL;
    group1_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP1_SEL;
    group2_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP2_SEL;
    group3_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP3_SEL;
    group4_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP4_SEL;
    group5_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP5_SEL;
    group6_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP6_SEL;
    group7_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP7_SEL;
    group8_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP8_SEL;
    group9_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP9_SEL;
    group10_outputxbar[4] = (uint32_t)MW_OUTPUTXBAR4_GROUP10_SEL;
    group0_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP0_SEL;
    group1_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP1_SEL;
    group2_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP2_SEL;
    group3_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP3_SEL;
    group4_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP4_SEL;
    group5_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP5_SEL;
    group6_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP6_SEL;
    group7_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP7_SEL;
    group8_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP8_SEL;
    group9_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP9_SEL;
    group10_outputxbar[5] = (uint32_t)MW_OUTPUTXBAR5_GROUP10_SEL;
    group0_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP0_SEL;
    group1_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP1_SEL;
    group2_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP2_SEL;
    group3_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP3_SEL;
    group4_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP4_SEL;
    group5_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP5_SEL;
    group6_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP6_SEL;
    group7_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP7_SEL;
    group8_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP8_SEL;
    group9_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP9_SEL;
    group10_outputxbar[6] = (uint32_t)MW_OUTPUTXBAR6_GROUP10_SEL;
    group0_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP0_SEL;
    group1_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP1_SEL;
    group2_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP2_SEL;
    group3_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP3_SEL;
    group4_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP4_SEL;
    group5_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP5_SEL;
    group6_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP6_SEL;
    group7_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP7_SEL;
    group8_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP8_SEL;
    group9_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP9_SEL;
    group10_outputxbar[7] = (uint32_t)MW_OUTPUTXBAR7_GROUP10_SEL;
    group0_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP0_SEL;
    group1_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP1_SEL;
    group2_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP2_SEL;
    group3_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP3_SEL;
    group4_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP4_SEL;
    group5_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP5_SEL;
    group6_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP6_SEL;
    group7_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP7_SEL;
    group8_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP8_SEL;
    group9_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP9_SEL;
    group10_outputxbar[8] = (uint32_t)MW_OUTPUTXBAR8_GROUP10_SEL;
    group0_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP0_SEL;
    group1_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP1_SEL;
    group2_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP2_SEL;
    group3_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP3_SEL;
    group4_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP4_SEL;
    group5_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP5_SEL;
    group6_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP6_SEL;
    group7_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP7_SEL;
    group8_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP8_SEL;
    group9_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP9_SEL;
    group10_outputxbar[9] = (uint32_t)MW_OUTPUTXBAR9_GROUP10_SEL;
    group0_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP0_SEL;
    group1_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP1_SEL;
    group2_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP2_SEL;
    group3_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP3_SEL;
    group4_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP4_SEL;
    group5_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP5_SEL;
    group6_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP6_SEL;
    group7_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP7_SEL;
    group8_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP8_SEL;
    group9_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP9_SEL;
    group10_outputxbar[10] = (uint32_t)MW_OUTPUTXBAR10_GROUP10_SEL;
    group0_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP0_SEL;
    group1_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP1_SEL;
    group2_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP2_SEL;
    group3_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP3_SEL;
    group4_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP4_SEL;
    group5_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP5_SEL;
    group6_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP6_SEL;
    group7_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP7_SEL;
    group8_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP8_SEL;
    group9_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP9_SEL;
    group10_outputxbar[11] = (uint32_t)MW_OUTPUTXBAR11_GROUP10_SEL;
    group0_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP0_SEL;
    group1_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP1_SEL;
    group2_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP2_SEL;
    group3_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP3_SEL;
    group4_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP4_SEL;
    group5_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP5_SEL;
    group6_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP6_SEL;
    group7_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP7_SEL;
    group8_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP8_SEL;
    group9_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP9_SEL;
    group10_outputxbar[12] = (uint32_t)MW_OUTPUTXBAR12_GROUP10_SEL;
    group0_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP0_SEL;
    group1_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP1_SEL;
    group2_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP2_SEL;
    group3_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP3_SEL;
    group4_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP4_SEL;
    group5_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP5_SEL;
    group6_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP6_SEL;
    group7_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP7_SEL;
    group8_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP8_SEL;
    group9_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP9_SEL;
    group10_outputxbar[13] = (uint32_t)MW_OUTPUTXBAR13_GROUP10_SEL;
    group0_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP0_SEL;
    group1_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP1_SEL;
    group2_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP2_SEL;
    group3_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP3_SEL;
    group4_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP4_SEL;
    group5_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP5_SEL;
    group6_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP6_SEL;
    group7_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP7_SEL;
    group8_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP8_SEL;
    group9_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP9_SEL;
    group10_outputxbar[14] = (uint32_t)MW_OUTPUTXBAR14_GROUP10_SEL;
    group0_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP0_SEL;
    group1_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP1_SEL;
    group2_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP2_SEL;
    group3_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP3_SEL;
    group4_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP4_SEL;
    group5_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP5_SEL;
    group6_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP6_SEL;
    group7_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP7_SEL;
    group8_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP8_SEL;
    group9_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP9_SEL;
    group10_outputxbar[15] = (uint32_t)MW_OUTPUTXBAR15_GROUP10_SEL;


    
    for(int i=0; i<=15; i++)
    {
        switch(i)
        {
            case 0:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR0_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR0_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR0_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR0_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR0_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 1:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR1_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR1_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR1_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR1_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR1_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 2:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR2_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR2_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR2_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR2_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR2_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 3:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR3_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR3_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR3_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR3_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR3_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 4:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR4_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR4_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR4_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR4_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR4_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 5:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR5_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR5_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR5_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR5_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR5_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 6:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR6_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR6_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR6_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR6_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR6_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 7:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR7_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR7_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR7_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR7_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR7_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 8:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR8_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR8_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR8_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR8_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR8_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 9:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR9_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR9_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR9_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR9_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR9_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 10:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR10_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR10_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR10_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR10_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR10_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 11:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR11_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR11_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR11_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR11_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR11_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 12:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR12_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR12_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR12_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR12_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR12_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 13:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR13_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR13_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR13_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR13_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR13_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 14:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR14_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR14_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR14_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR14_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR14_STRETCHED_PULSE_LENGTH_SEL;
                break;
            case 15:
                invertOutputBeforeLatch[i] = MW_OUTPUTXBAR15_INVERT_OUTPUT_BEFORE_LATCH_SEL;
                invertOutput[i] = MW_OUTPUTXBAR15_INVERT_XBAR_OUTPUT_SEL;
                latchSignalSource[i] = MW_OUTPUTXBAR15_LATCH_SIGNAL_SOURCE_SEL;
                stretchedPulseOutput[i] = MW_OUTPUTXBAR15_STRETCHED_PULSE_SOURCE_SEL;
                stretchedPulseLength[i] = MW_OUTPUTXBAR15_STRETCHED_PULSE_LENGTH_SEL;
                break;
            default:
                break;
        }     
    }

    uint32_t keyFactor = 0;
    for(int i=0;i<=15;i++)
    {
        keyFactor = pow(2, i);
        if(((group0_outputxbar[i]!=0) | (group1_outputxbar[i]!=0) | (group2_outputxbar[i]!=0) | (group3_outputxbar[i]!=0) | (group4_outputxbar[i]!=0) | (group5_outputxbar[i]!=0) | (group6_outputxbar[i]!=0) | (group7_outputxbar[i]!=0) | (group8_outputxbar[i]!=0) | (group9_outputxbar[i]!=0) | (group10_outputxbar[i]!=0)))
        {
            SOC_xbarSelectOutputXBarInputSource(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, i, group0_outputxbar[i], group1_outputxbar[i], group2_outputxbar[i], group3_outputxbar[i], group4_outputxbar[i], group5_outputxbar[i], group6_outputxbar[i], group7_outputxbar[i], group8_outputxbar[i], group9_outputxbar[i], group10_outputxbar[i]);
            if(invertOutputBeforeLatch[i] == 0)
                SOC_xbarInvertOutputXBarOutputSignalBeforeLatch(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
            else
                SOC_xbarInvertOutputXBarOutputSignalBeforeLatch(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, keyFactor);
            if(invertOutput[i] == 0)
                SOC_xbarInvertOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, keyFactor);
            else
                SOC_xbarInvertOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
            if(latchSignalSource[i] == 0)
                SOC_xbarSelectLatchOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
            else
                SOC_xbarSelectLatchOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, keyFactor);
            if(stretchedPulseOutput[i] == 0)
                SOC_xbarSelectStretchedPulseOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
            else
                SOC_xbarSelectStretchedPulseOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, keyFactor);
            if(stretchedPulseLength[i] == 0)
                SOC_xbarSelectStretchedPulseLengthOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
            else
                SOC_xbarSelectStretchedPulseLengthOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, keyFactor);
        }

    }


    Drivers_EPWMSyncOutputXBAR();
}

void Drivers_EPWMSyncOutputXBAR()
{
    uint32_t groupEPWMSyncOutputXbar[16];
    for(int i=0;i<=15;i++)
    {

        if((i==0) && (MW_OUTPUTXBAR0_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR0_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR0_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR0_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR0_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR0_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR0_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR0_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR0_GROUP9_SEL); 
        }
        else if((i==1) && (MW_OUTPUTXBAR1_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR1_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR1_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR1_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR1_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR1_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR1_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR1_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR1_GROUP9_SEL); 
        }
        else if((i==2) && (MW_OUTPUTXBAR2_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR2_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR2_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR2_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR2_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR2_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR2_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR2_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR2_GROUP9_SEL); 
        }
        else if((i==3) && (MW_OUTPUTXBAR3_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR3_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR3_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR3_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR3_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR3_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR3_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR3_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR3_GROUP9_SEL); 
        }
        else if((i==4) && (MW_OUTPUTXBAR4_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR4_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR4_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR4_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR4_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR4_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR4_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR4_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR4_GROUP9_SEL); 
        }
        else if((i==5) && (MW_OUTPUTXBAR5_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR5_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR5_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR5_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR5_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR5_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR5_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR5_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR5_GROUP9_SEL); 
        }
        else if((i==6) && (MW_OUTPUTXBAR6_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR6_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR6_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR6_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR6_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR6_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR6_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR6_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR6_GROUP9_SEL); 
        }
        else if((i==7) && (MW_OUTPUTXBAR7_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR7_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR7_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR7_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR7_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR7_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR7_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR7_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR7_GROUP9_SEL);  
        }
        else if((i==8) && (MW_OUTPUTXBAR8_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR8_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR8_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR8_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR8_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR8_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR8_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR8_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR8_GROUP9_SEL);  
        }
        else if((i==9) && (MW_OUTPUTXBAR9_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR9_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR9_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR9_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR9_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR9_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR9_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR9_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR9_GROUP9_SEL);  
        }
        else if((i==10) && (MW_OUTPUTXBAR10_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR10_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR10_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR10_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR10_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR10_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR10_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR10_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR10_GROUP9_SEL);  
        }
        else if((i==11) && (MW_OUTPUTXBAR11_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR11_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR11_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR11_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR11_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR11_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR11_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR11_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR11_GROUP9_SEL); 
        }
        else if((i==12) && (MW_OUTPUTXBAR12_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR12_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR12_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR12_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR12_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR12_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR12_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR12_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR12_GROUP9_SEL); 
        }
        else if((i==13) && (MW_OUTPUTXBAR13_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR13_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR13_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR13_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR13_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR13_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR13_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR13_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR13_GROUP9_SEL); 
        }
        else if((i==14) && (MW_OUTPUTXBAR14_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR14_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR14_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR14_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR14_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR14_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR14_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR14_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR14_GROUP9_SEL); 
        }
        else if((i==15) && (MW_OUTPUTXBAR15_GROUP9_SEL != 0))
        {
            if(MW_OUTPUTXBAR15_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, MW_OUTPUTXBAR15_GROUP9_SEL);    
            else if(MW_OUTPUTXBAR15_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, MW_OUTPUTXBAR15_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR15_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, MW_OUTPUTXBAR15_GROUP9_SEL);  
            else if(MW_OUTPUTXBAR15_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, MW_OUTPUTXBAR15_GROUP9_SEL); 
        }
   
    }
}

void Drivers_epwmSyncoutXbarOpen()
{
    uint32_t groupEpwmSyncoutXbar[4];

    groupEpwmSyncoutXbar[0] = MW_EPWM_SYNCOUT_XBAR0_SEL;
    groupEpwmSyncoutXbar[1] = MW_EPWM_SYNCOUT_XBAR1_SEL;
    groupEpwmSyncoutXbar[2] = MW_EPWM_SYNCOUT_XBAR2_SEL;
    groupEpwmSyncoutXbar[3] = MW_EPWM_SYNCOUT_XBAR3_SEL;

    for(uint8_t i = 0; i < 4; i++)
    {
        if(groupEpwmSyncoutXbar[i]!=0)
        {
            SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, i,  groupEpwmSyncoutXbar[i]);
        }
    }
}

MCAN_BitTimingParams gMcanBitTimingDefaultParams;
MCAN_InitParams            initParams = {0U};
MCAN_ConfigParams          configParams = {0U};
MCAN_MsgRAMConfigParams    msgRAMConfigParams = {0U};
MCAN_BitTimingParams       bitTimes = {0U};
MCAN_ExtMsgIDFilterElement extFiltElem[MW_ELEMCNT_EXTFILT_MCAN0_SEL] = {0U};
MCAN_StdMsgIDFilterElement stdFiltElem[MW_ELEMCNT_STDFILT_MCAN0_SEL] = {0U};
uint32_t filter_cnt;


void Driver_mcanOpen(void)
{
    
    gMcanBitTimingDefaultParams.nomRatePrescalar = MW_PRESCALER_NOM_BITRATE_SEL;
    gMcanBitTimingDefaultParams.nomTimeSeg1 = MW_TIMESEG1_NOM_BITRATE_SEL;
    gMcanBitTimingDefaultParams.nomTimeSeg2 = MW_TIMESEG2_NOM_BITRATE_SEL;
    gMcanBitTimingDefaultParams.nomSynchJumpWidth = MW_SYNCH_JUMPWIDTH_NOM_BITRATE_SEL;
    gMcanBitTimingDefaultParams.dataRatePrescalar = MW_PRESCALER_DATA_BITRATE_SEL;
    gMcanBitTimingDefaultParams.dataTimeSeg1 = MW_TIMESEG1_DATA_BITRATE_SEL;
    gMcanBitTimingDefaultParams.dataTimeSeg2 = MW_TIMESEG2_DATA_BITRATE_SEL;
    gMcanBitTimingDefaultParams.dataSynchJumpWidth = MW_SYNCH_JUMPWIDTH_DATA_BITRATE_SEL;

}
#define CONFIG_I2C0 (0U)
#define CONFIG_I2C_NUM_INSTANCES (1U)
static I2C_HwAttrs gI2cHwAttrs[CONFIG_I2C_NUM_INSTANCES] =
{
    {
        .baseAddr       = CSL_I2C2_U_BASE,
        .intNum         = 46,
        .eventId        = 0,
        .funcClk        = 96000000U,
        .enableIntr     = 1,
        .ownTargetAddr   = 0x1C,
    },
};

static I2C_Object gI2cObjects[CONFIG_I2C_NUM_INSTANCES];
/* I2C driver configuration */
I2C_Config gI2cConfig[CONFIG_I2C_NUM_INSTANCES] =
{
    {
        .object = &gI2cObjects[CONFIG_I2C0],
        .hwAttrs = &gI2cHwAttrs[CONFIG_I2C0]
    },
};

uint32_t gI2cConfigNum = CONFIG_I2C_NUM_INSTANCES;

I2C_Handle gI2cHandle[CONFIG_I2C_NUM_INSTANCES];

/* I2C Driver Parameters */
I2C_Params gI2cParams[CONFIG_I2C_NUM_INSTANCES] =
{
    {
        .transferMode        = I2C_MODE_BLOCKING,
        .transferCallbackFxn = NULL,
        .bitRate             = I2C_400KHZ,
    },
};

void Drivers_i2cOpen(void)
{
    uint32_t instCnt;
    int32_t  status = SystemP_SUCCESS;

    for(instCnt = 0U; instCnt < CONFIG_I2C_NUM_INSTANCES; instCnt++)
    {
        gI2cHandle[instCnt] = NULL;   /* Init to NULL so that we can exit gracefully */
    }

    /* Open all instances */
    for(instCnt = 0U; instCnt < CONFIG_I2C_NUM_INSTANCES; instCnt++)
    {
        gI2cHandle[instCnt] = I2C_open(instCnt, &gI2cParams[instCnt]);
        if(NULL == gI2cHandle[instCnt])
        {
            DebugP_logError("I2C open failed for instance %d !!!\r\n", instCnt);
            status = SystemP_FAILURE;
            break;
        }
    }

    return;
}

void Drivers_i2cClose(void)
{
    uint32_t instCnt;

    /* Close all instances that are open */
    for(instCnt = 0U; instCnt < CONFIG_I2C_NUM_INSTANCES; instCnt++)
    {
        if(gI2cHandle[instCnt] != NULL)
        {
            I2C_close(gI2cHandle[instCnt]);
            gI2cHandle[instCnt] = NULL;
        }
    }

    return;
}

void mcanEnableTransceiverLP(void)
{
    GPIO_setDirMode(CSL_GPIO0_U_BASE, 51, GPIO_DIRECTION_INPUT);
    uint32_t    gpioBaseAddr, pinNum;

    gpioBaseAddr = (uint32_t)AddrTranslateP_getLocalAddr(CSL_GPIO0_U_BASE);
    pinNum       = 51;

    GPIO_setDirMode(gpioBaseAddr, pinNum, GPIO_DIRECTION_OUTPUT);

    GPIO_pinWriteLow(gpioBaseAddr, pinNum);
}

static void SetupI2CTransfer(I2C_Handle handle,  uint32_t targetAddr,
                      uint8_t *writeData, uint32_t numWriteBytes,
                      uint8_t *readData,  uint32_t numReadBytes)
{
    int32_t status;
    I2C_Transaction i2cTransaction;

    /* Enable Transceiver */
    I2C_Transaction_init(&i2cTransaction);
    i2cTransaction.targetAddress = targetAddr;
    i2cTransaction.writeBuf = (uint8_t *)&writeData[0];
    i2cTransaction.writeCount = numWriteBytes;
    i2cTransaction.readBuf = (uint8_t *)&readData[0];
    i2cTransaction.readCount = numReadBytes;
    status = I2C_transfer(handle, &i2cTransaction);
    DebugP_assert(SystemP_SUCCESS == status);
}

void mcanEnableTransceiverCC(void)
{
    I2C_Handle      i2cHandle;
    uint8_t         dataToSlave[4];

    i2cHandle = gI2cHandle[CONFIG_I2C0];
    dataToSlave[0] = TCA6416_REG_CONFIG0;
    dataToSlave[1] = 0x0U;
    SetupI2CTransfer(i2cHandle, 0x20, &dataToSlave[0], 1, &dataToSlave[1], 1);
    /* set the P00 to 0 make them output ports. */
    dataToSlave[1] &= ~(0x1U);
    SetupI2CTransfer(i2cHandle, 0x20, &dataToSlave[0], 2, NULL, 0);

    /* Get the port values. */
    dataToSlave[0] = TCA6416_REG_INPUT0;
    dataToSlave[1] = 0x0U;
    SetupI2CTransfer(i2cHandle, 0x20, &dataToSlave[0], 1, &dataToSlave[1], 1);

    /* Set P10 and P11 to 0.
     */
    dataToSlave[0] = TCA6416_REG_OUTPUT0;
    dataToSlave[1] &= ~(0x1);
    SetupI2CTransfer(i2cHandle, 0x20, &dataToSlave[0], 2, NULL, 0);
}


void App_mcanEnableIntr(uint32_t instance)
{
    uint32_t intrMask = 0x0;

    if(instance == 0)
    {

        if(MW_RX_FIFO0_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_NEW_MSG;
        if(MW_RX_FIFO0_WTRMARK_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_WATERMARK;
        if(MW_RX_FIFO0_FULL_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_FULL;
        if(MW_RX_FIFO0_MSGLOST_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_MSG_LOST;
        if(MW_RX_FIFO1_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_NEW_MSG;
        if(MW_RX_FIFO1_WTRMARK_INTR_MCAN0_SEL == 1)     
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_WATERMARK; 
        if(MW_RX_FIFO1_FULL_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_FULL;
        if(MW_RX_FIFO1_MSGLOST_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_MSG_LOST; 
        if(MW_HIGH_PRIOTYMSG_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_HIGH_PRIO_MSG;
        if(MW_TX_COMPLETED_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TRANS_COMPLETE;
        if(MW_TX_CANCELFIN_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TRANS_CANCEL_FINISH;
        if(MW_TX_FIFOEMPTY_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_FIFO_EMPTY;
        if(MW_TX_FIFOEVT_NEWENTRY_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_NEW_ENTRY;
        if(MW_TX_FIFOEVT_WTRMARK_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_WATERMARK;
        if(MW_TX_FIFOEVT_FULL_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_FULL;
        if(MW_TX_FIFOEVT_ELTLOST_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_ELEM_LOST;
        if(MW_TIMESTMP_WRPAROUND_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TIMESTAMP_WRAPAROUND;
        if(MW_MSGRAM_ACSFAIL_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_MSG_RAM_ACCESS_FAILURE;
        if(MW_TIMEOUT_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TIMEOUT;  
        if(MW_MSGSTR_DEDRXBUF_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG;
        if(MW_BITERR_CORR_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BIT_ERR_CORRECTED;
        if(MW_BITERR_UNCORR_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BIT_ERR_UNCORRECTED;
        if(MW_ERRLOGGING_OVF_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_ERR_LOG_OVRFLW;
        if(MW_ERRPASSIVE_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_ERR_PASSIVE; 
        if(MW_WARNINGSTS_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_WARNING_STATUS;
        if(MW_BUSOFFSTS_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BUS_OFF_STATUS;
        if(MW_WATCHDOG_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_WATCHDOG;
        if(MW_PROTOCOLERR_ARBPHASE_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_PROTOCOL_ERR_ARB;
        if(MCAN_INTR_SRC_PROTOCOL_ERR_DATA == 1)
            intrMask = intrMask | MCAN_INTR_SRC_PROTOCOL_ERR_DATA;
        if(MW_ACSRESADDR_INTR_MCAN0_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RES_ADDR_ACCESS;

        MCAN_enableIntr(CSL_MCAN0_MSG_RAM_U_BASE, intrMask, (uint32_t)TRUE);
        MCAN_enableIntr(CSL_MCAN0_MSG_RAM_U_BASE, MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
        MCAN_selectIntrLine(CSL_MCAN0_MSG_RAM_U_BASE, intrMask, MCAN_INTR_LINE_NUM_0);
        MCAN_enableIntrLine(CSL_MCAN0_MSG_RAM_U_BASE, MCAN_INTR_LINE_NUM_0, (uint32_t)TRUE);       

    }
    else if(instance == 1)
    {
        if(MW_RX_FIFO0_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_NEW_MSG;
        if(MW_RX_FIFO0_WTRMARK_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_WATERMARK;
        if(MW_RX_FIFO0_FULL_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_FULL;
        if(MW_RX_FIFO0_MSGLOST_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_MSG_LOST;
        if(MW_RX_FIFO1_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_NEW_MSG;
        if(MW_RX_FIFO1_WTRMARK_INTR_MCAN1_SEL == 1)     
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_WATERMARK; 
        if(MW_RX_FIFO1_FULL_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_FULL;
        if(MW_RX_FIFO1_MSGLOST_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_MSG_LOST; 
        if(MW_HIGH_PRIOTYMSG_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_HIGH_PRIO_MSG;
        if(MW_TX_COMPLETED_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TRANS_COMPLETE;
        if(MW_TX_CANCELFIN_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TRANS_CANCEL_FINISH;
        if(MW_TX_FIFOEMPTY_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_FIFO_EMPTY;
        if(MW_TX_FIFOEVT_NEWENTRY_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_NEW_ENTRY;
        if(MW_TX_FIFOEVT_WTRMARK_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_WATERMARK;
        if(MW_TX_FIFOEVT_FULL_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_FULL;
        if(MW_TX_FIFOEVT_ELTLOST_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_ELEM_LOST;
        if(MW_TIMESTMP_WRPAROUND_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TIMESTAMP_WRAPAROUND;
        if(MW_MSGRAM_ACSFAIL_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_MSG_RAM_ACCESS_FAILURE;
        if(MW_TIMEOUT_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TIMEOUT;  
        if(MW_MSGSTR_DEDRXBUF_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG;
        if(MW_BITERR_CORR_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BIT_ERR_CORRECTED;
        if(MW_BITERR_UNCORR_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BIT_ERR_UNCORRECTED;
        if(MW_ERRLOGGING_OVF_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_ERR_LOG_OVRFLW;
        if(MW_ERRPASSIVE_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_ERR_PASSIVE; 
        if(MW_WARNINGSTS_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_WARNING_STATUS;
        if(MW_BUSOFFSTS_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BUS_OFF_STATUS;
        if(MW_WATCHDOG_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_WATCHDOG;
        if(MW_PROTOCOLERR_ARBPHASE_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_PROTOCOL_ERR_ARB;
        if(MCAN_INTR_SRC_PROTOCOL_ERR_DATA == 1)
            intrMask = intrMask | MCAN_INTR_SRC_PROTOCOL_ERR_DATA;
        if(MW_ACSRESADDR_INTR_MCAN1_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RES_ADDR_ACCESS;

        MCAN_enableIntr(CSL_MCAN1_MSG_RAM_U_BASE, intrMask, (uint32_t)TRUE);
        MCAN_enableIntr(CSL_MCAN1_MSG_RAM_U_BASE, MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
        MCAN_selectIntrLine(CSL_MCAN1_MSG_RAM_U_BASE, intrMask, MCAN_INTR_LINE_NUM_0);
        MCAN_enableIntrLine(CSL_MCAN1_MSG_RAM_U_BASE, MCAN_INTR_LINE_NUM_0, (uint32_t)TRUE); 
    }
    else if(instance == 2)
    {
        if(MW_RX_FIFO0_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_NEW_MSG;
        if(MW_RX_FIFO0_WTRMARK_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_WATERMARK;
        if(MW_RX_FIFO0_FULL_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_FULL;
        if(MW_RX_FIFO0_MSGLOST_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_MSG_LOST;
        if(MW_RX_FIFO1_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_NEW_MSG;
        if(MW_RX_FIFO1_WTRMARK_INTR_MCAN2_SEL == 1)     
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_WATERMARK; 
        if(MW_RX_FIFO1_FULL_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_FULL;
        if(MW_RX_FIFO1_MSGLOST_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_MSG_LOST; 
        if(MW_HIGH_PRIOTYMSG_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_HIGH_PRIO_MSG;
        if(MW_TX_COMPLETED_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TRANS_COMPLETE;
        if(MW_TX_CANCELFIN_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TRANS_CANCEL_FINISH;
        if(MW_TX_FIFOEMPTY_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_FIFO_EMPTY;
        if(MW_TX_FIFOEVT_NEWENTRY_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_NEW_ENTRY;
        if(MW_TX_FIFOEVT_WTRMARK_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_WATERMARK;
        if(MW_TX_FIFOEVT_FULL_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_FULL;
        if(MW_TX_FIFOEVT_ELTLOST_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_ELEM_LOST;
        if(MW_TIMESTMP_WRPAROUND_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TIMESTAMP_WRAPAROUND;
        if(MW_MSGRAM_ACSFAIL_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_MSG_RAM_ACCESS_FAILURE;
        if(MW_TIMEOUT_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TIMEOUT;  
        if(MW_MSGSTR_DEDRXBUF_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG;
        if(MW_BITERR_CORR_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BIT_ERR_CORRECTED;
        if(MW_BITERR_UNCORR_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BIT_ERR_UNCORRECTED;
        if(MW_ERRLOGGING_OVF_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_ERR_LOG_OVRFLW;
        if(MW_ERRPASSIVE_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_ERR_PASSIVE; 
        if(MW_WARNINGSTS_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_WARNING_STATUS;
        if(MW_BUSOFFSTS_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BUS_OFF_STATUS;
        if(MW_WATCHDOG_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_WATCHDOG;
        if(MW_PROTOCOLERR_ARBPHASE_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_PROTOCOL_ERR_ARB;
        if(MCAN_INTR_SRC_PROTOCOL_ERR_DATA == 1)
            intrMask = intrMask | MCAN_INTR_SRC_PROTOCOL_ERR_DATA;
        if(MW_ACSRESADDR_INTR_MCAN2_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RES_ADDR_ACCESS;

        MCAN_enableIntr(CSL_MCAN2_MSG_RAM_U_BASE, intrMask, (uint32_t)TRUE);
        MCAN_enableIntr(CSL_MCAN2_MSG_RAM_U_BASE, MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
        MCAN_selectIntrLine(CSL_MCAN2_MSG_RAM_U_BASE, intrMask, MCAN_INTR_LINE_NUM_0);
        MCAN_enableIntrLine(CSL_MCAN2_MSG_RAM_U_BASE, MCAN_INTR_LINE_NUM_0, (uint32_t)TRUE); 
    }
    else if(instance == 3)
    {
        if(MW_RX_FIFO0_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_NEW_MSG;
        if(MW_RX_FIFO0_WTRMARK_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_WATERMARK;
        if(MW_RX_FIFO0_FULL_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_FULL;
        if(MW_RX_FIFO0_MSGLOST_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO0_MSG_LOST;
        if(MW_RX_FIFO1_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_NEW_MSG;
        if(MW_RX_FIFO1_WTRMARK_INTR_MCAN3_SEL == 1)     
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_WATERMARK; 
        if(MW_RX_FIFO1_FULL_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_FULL;
        if(MW_RX_FIFO1_MSGLOST_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RX_FIFO1_MSG_LOST; 
        if(MW_HIGH_PRIOTYMSG_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_HIGH_PRIO_MSG;
        if(MW_TX_COMPLETED_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TRANS_COMPLETE;
        if(MW_TX_CANCELFIN_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TRANS_CANCEL_FINISH;
        if(MW_TX_FIFOEMPTY_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_FIFO_EMPTY;
        if(MW_TX_FIFOEVT_NEWENTRY_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_NEW_ENTRY;
        if(MW_TX_FIFOEVT_WTRMARK_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_WATERMARK;
        if(MW_TX_FIFOEVT_FULL_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_FULL;
        if(MW_TX_FIFOEVT_ELTLOST_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TX_EVT_FIFO_ELEM_LOST;
        if(MW_TIMESTMP_WRPAROUND_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TIMESTAMP_WRAPAROUND;
        if(MW_MSGRAM_ACSFAIL_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_MSG_RAM_ACCESS_FAILURE;
        if(MW_TIMEOUT_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_TIMEOUT;  
        if(MW_MSGSTR_DEDRXBUF_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG;
        if(MW_BITERR_CORR_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BIT_ERR_CORRECTED;
        if(MW_BITERR_UNCORR_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BIT_ERR_UNCORRECTED;
        if(MW_ERRLOGGING_OVF_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_ERR_LOG_OVRFLW;
        if(MW_ERRPASSIVE_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_ERR_PASSIVE; 
        if(MW_WARNINGSTS_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_WARNING_STATUS;
        if(MW_BUSOFFSTS_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_BUS_OFF_STATUS;
        if(MW_WATCHDOG_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_WATCHDOG;
        if(MW_PROTOCOLERR_ARBPHASE_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_PROTOCOL_ERR_ARB;
        if(MCAN_INTR_SRC_PROTOCOL_ERR_DATA == 1)
            intrMask = intrMask | MCAN_INTR_SRC_PROTOCOL_ERR_DATA;
        if(MW_ACSRESADDR_INTR_MCAN3_SEL == 1)
            intrMask = intrMask | MCAN_INTR_SRC_RES_ADDR_ACCESS;

        MCAN_enableIntr(CSL_MCAN3_MSG_RAM_U_BASE, intrMask, (uint32_t)TRUE);
        MCAN_enableIntr(CSL_MCAN3_MSG_RAM_U_BASE, MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
        MCAN_selectIntrLine(CSL_MCAN3_MSG_RAM_U_BASE, intrMask, MCAN_INTR_LINE_NUM_0);
        MCAN_enableIntrLine(CSL_MCAN3_MSG_RAM_U_BASE, MCAN_INTR_LINE_NUM_0, (uint32_t)TRUE);  
    }

}



void MCAN_initialization(void)
{


    bool tcvENABLE = FALSE;

    String board = STR(MW_TARGETHARDWARE);
    String XCPInterface = STR(MW_EXTMODE_CONFIGURATION);

    #ifdef MW_EXTMODE_RUNNING
    if((strcmp(XCPInterface, "CAN") == 0) && (strcmp(board, "AM263X LP") == 0))
    {
        mcanEnableTransceiverLP();
        tcvENABLE = TRUE;
    }
    else if((strcmp(XCPInterface, "CAN") == 0) && (strcmp(board, "AM263X ControlCard") == 0))
    {
        mcanEnableTransceiverCC();
        tcvENABLE = TRUE;
    }
    #endif

    if(MW_ENABLE_MCAN0_SEL | MW_ENABLE_MCAN1_SEL | MW_ENABLE_MCAN2_SEL | MW_ENABLE_MCAN3_SEL)
    {
        
        uint32_t                   i, j;
        int32_t                 status = SystemP_SUCCESS;
        
        

        for(i = 0; i < MCAN_COUNT; i++)
        {
           
            if(((i == 0) && (MW_ENABLE_MCAN0_SEL == 1)))
            {
                if((MW_ENABLE_INTLOOPBACK_MCAN0_SEL == 0) && (MW_ENABLE_GPIO_TCV_MCAN0_SEL == 1) && !tcvENABLE)
                    mcanEnableTransceiverLP();
                else if((MW_ENABLE_INTLOOPBACK_MCAN0_SEL == 0) && (MW_ENABLE_I2C_TCV_MCAN0_SEL == 1) && !tcvENABLE)
                    mcanEnableTransceiverCC();

                MCAN_initOperModeParams(&initParams);
                if(MW_PROTOCOL_MCAN0_SEL == 0)
                     initParams.fdMode = TRUE;
                else
                     initParams.fdMode = FALSE;    
                
                if(MW_ENABLE_BRS_MCAN0_SEL == 1)
                    initParams.brsEnable = TRUE;
                else
                    initParams.brsEnable = FALSE;

                MCAN_initGlobalFilterConfigParams(&configParams);
                MCAN_initSetBitTimeParams(&bitTimes);
                MCAN_initMsgRamConfigParams(&msgRAMConfigParams);

                msgRAMConfigParams.lss = MW_ELEMCNT_STDFILT_MCAN0_SEL;
                msgRAMConfigParams.lse = MW_ELEMCNT_EXTFILT_MCAN0_SEL;
                msgRAMConfigParams.txBufCnt = MW_ELEMCNT_TXBUF_MCAN0_SEL;
                msgRAMConfigParams.txFIFOCnt = MW_ELEMCNT_TXFIFO_MCAN0_SEL;
                msgRAMConfigParams.txBufMode = MCAN_TX_MEM_TYPE_BUF;
                msgRAMConfigParams.txEventFIFOCnt = MW_ELEMCNT_TXEVENTFIFO_MCAN0_SEL;
                msgRAMConfigParams.rxFIFO0Cnt = MW_ELEMCNT_RXFIFO0_MCAN0_SEL;
                msgRAMConfigParams.rxFIFO1Cnt = MW_ELEMCNT_RXFIFO1_MCAN0_SEL;
               

                if(MW_ENABLE_BLOCKINGFIFO0_MCAN0_SEL == 1)
                    msgRAMConfigParams.rxFIFO0OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;

                if(MW_ENABLE_BLOCKINGFIFO1_MCAN0_SEL == 1)
                    msgRAMConfigParams.rxFIFO1OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;  

                status = MCAN_calcMsgRamParamsStartAddr(&msgRAMConfigParams);
                DebugP_assert(status == CSL_PASS);

                if(MW_ENABLE_STDFILT_MCAN0_SEL == 1)
                    filter_cnt = (uint32_t)MW_ELEMCNT_STDFILT_MCAN0_SEL;
                else if(MW_ELEMCNT_EXTFILT_MCAN0_SEL == 1)
                    filter_cnt = (uint32_t)MW_ELEMCNT_EXTFILT_MCAN0_SEL;

                
                    if(MW_ENABLE_STDFILT_MCAN0_SEL == 1)
                    {
                        for(j = 0; j < MW_ELEMCNT_STDFILT_MCAN0_SEL; j++)
                        {
                            stdFiltElem[j].sfid1 = (uint32_t)(MW_STDFILT_ID1_MCAN0_SEL) + j;
                            if(MW_STDFILT_CONFIG_MCAN0_SEL != 7)
                                stdFiltElem[j].sfid2 = (uint32_t)(MW_STDFILT_ID2_MCAN0_SEL);
                            else
                                stdFiltElem[j].sfid2 = (uint32_t)(MW_STDFILT_ID2_MCAN0_SEL) + j;
                            if(MW_STDFILT_CONFIG_MCAN0_SEL == 7)
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_BUFFER;
                            else if(MW_STDFILT_CONFIG_MCAN0_SEL == 1 | MW_STDFILT_CONFIG_MCAN0_SEL == 5)
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_FIFO0;
                            else if(MW_STDFILT_CONFIG_MCAN0_SEL == 2 | MW_STDFILT_CONFIG_MCAN0_SEL == 6)  
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_FIFO1;
                            stdFiltElem[j].sft = MW_STDFILT_TYPE_MCAN0_SEL;
                        }
                    }

                    if(MW_ENABLE_EXTFILT_MCAN0_SEL == 1)
                    {
                        for(j = 0; j < MW_ELEMCNT_EXTFILT_MCAN0_SEL; j++)
                        {
                            extFiltElem[j].efid1 = (uint32_t)(MW_EXTFILT_ID1_MCAN0_SEL) + j;
                            if(MW_EXTFILT_CONFIG_MCAN0_SEL != 7)
                                extFiltElem[j].efid2 = (uint32_t)(MW_EXTFILT_ID2_MCAN0_SEL);
                            else
                                extFiltElem[j].efid2 = (uint32_t)(MW_EXTFILT_ID2_MCAN0_SEL) + j;
                            if(MW_EXTFILT_CONFIG_MCAN0_SEL  == 7)
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_BUFFER;
                            else if(MW_EXTFILT_CONFIG_MCAN0_SEL == 1 | MW_EXTFILT_CONFIG_MCAN0_SEL == 5)
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_FIFO0;
                            else if(MW_EXTFILT_CONFIG_MCAN0_SEL == 2 | MW_EXTFILT_CONFIG_MCAN0_SEL == 6)    
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_FIFO1;
                            extFiltElem[j].eft = MW_EXTFILT_TYPE_MCAN0_SEL;
                        }

                    }
                

                while (FALSE == MCAN_isMemInitDone(CSL_MCAN0_MSG_RAM_U_BASE))
                {}
                MCAN_setOpMode(CSL_MCAN0_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_SW_INIT);
                while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(CSL_MCAN0_MSG_RAM_U_BASE))
                {}

                MCAN_init(CSL_MCAN0_MSG_RAM_U_BASE, &initParams);
                MCAN_config(CSL_MCAN0_MSG_RAM_U_BASE, &configParams);
                MCAN_setBitTime(CSL_MCAN0_MSG_RAM_U_BASE, &bitTimes);
                MCAN_msgRAMConfig(CSL_MCAN0_MSG_RAM_U_BASE, &msgRAMConfigParams);
                MCAN_setExtIDAndMask(CSL_MCAN0_MSG_RAM_U_BASE, APP_MCAN_EXT_ID_MASK);

                
                    
                if(MW_ENABLE_STDFILT_MCAN0_SEL == 1)
                {
                    for(j = 0; j < MW_ELEMCNT_STDFILT_MCAN0_SEL; j++)
                    {
                        MCAN_addStdMsgIDFilter(CSL_MCAN0_MSG_RAM_U_BASE, j, &stdFiltElem[j]);
                    }
                }
                        
                if(MW_ENABLE_EXTFILT_MCAN0_SEL == 1)
                {
                    for(j = 0; j < MW_ELEMCNT_EXTFILT_MCAN0_SEL; j++)
                    {
                        MCAN_addExtMsgIDFilter(CSL_MCAN0_MSG_RAM_U_BASE, j, &extFiltElem[j]);
                    }

                }

                if (MW_ENABLE_INTLOOPBACK_MCAN0_SEL == 1)
                {
                    MCAN_lpbkModeEnable(CSL_MCAN0_MSG_RAM_U_BASE, MCAN_LPBK_MODE_INTERNAL, TRUE);
                }

                MCAN_setOpMode(CSL_MCAN0_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_NORMAL);
                while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(CSL_MCAN0_MSG_RAM_U_BASE))
                {}

            App_mcanEnableIntr(i);

        }
        else if(((i == 1) && (MW_ENABLE_MCAN1_SEL == 1)))
       {
                if((MW_ENABLE_INTLOOPBACK_MCAN1_SEL == 0) && (MW_ENABLE_GPIO_TCV_MCAN1_SEL == 1) && !tcvENABLE)
                    mcanEnableTransceiverLP();
                else if((MW_ENABLE_INTLOOPBACK_MCAN1_SEL == 0) && (MW_ENABLE_I2C_TCV_MCAN1_SEL == 1) && !tcvENABLE)
                    mcanEnableTransceiverCC();

                MCAN_initOperModeParams(&initParams);
                if(MW_PROTOCOL_MCAN1_SEL == 0)
                     initParams.fdMode = TRUE;
                else
                     initParams.fdMode = FALSE;    
                
                if(MW_ENABLE_BRS_MCAN1_SEL == 1)
                    initParams.brsEnable = TRUE;
                else
                    initParams.brsEnable = FALSE;

                MCAN_initGlobalFilterConfigParams(&configParams);
                MCAN_initSetBitTimeParams(&bitTimes);
                MCAN_initMsgRamConfigParams(&msgRAMConfigParams);

                msgRAMConfigParams.lss = MW_ELEMCNT_STDFILT_MCAN1_SEL;
                msgRAMConfigParams.lse = MW_ELEMCNT_EXTFILT_MCAN1_SEL;
                msgRAMConfigParams.txBufCnt = MW_ELEMCNT_TXBUF_MCAN1_SEL;
                msgRAMConfigParams.txFIFOCnt = MW_ELEMCNT_TXFIFO_MCAN1_SEL;
                msgRAMConfigParams.txBufMode = MCAN_TX_MEM_TYPE_BUF;
                msgRAMConfigParams.txEventFIFOCnt = MW_ELEMCNT_TXEVENTFIFO_MCAN1_SEL;
                msgRAMConfigParams.rxFIFO0Cnt = MW_ELEMCNT_RXFIFO0_MCAN1_SEL;
                msgRAMConfigParams.rxFIFO1Cnt = MW_ELEMCNT_RXFIFO1_MCAN1_SEL;
               

                if(MW_ENABLE_BLOCKINGFIFO0_MCAN1_SEL == 1)
                    msgRAMConfigParams.rxFIFO0OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;

                if(MW_ENABLE_BLOCKINGFIFO1_MCAN1_SEL == 1)
                    msgRAMConfigParams.rxFIFO1OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;  

                status = MCAN_calcMsgRamParamsStartAddr(&msgRAMConfigParams);
                DebugP_assert(status == CSL_PASS);

                if(MW_ENABLE_STDFILT_MCAN1_SEL == 1)
                    filter_cnt = (uint32_t)MW_ELEMCNT_STDFILT_MCAN1_SEL;
                else if(MW_ELEMCNT_EXTFILT_MCAN1_SEL == 1)
                    filter_cnt = (uint32_t)MW_ELEMCNT_EXTFILT_MCAN1_SEL;

                
                    if(MW_ENABLE_STDFILT_MCAN1_SEL == 1)
                    {
                        for(j = 0; j < MW_ELEMCNT_STDFILT_MCAN1_SEL; j++)
                        {
                            stdFiltElem[j].sfid1 = (uint32_t)(MW_STDFILT_ID1_MCAN1_SEL) + j;
                            if(MW_STDFILT_CONFIG_MCAN1_SEL != 7)
                                stdFiltElem[j].sfid2 = (uint32_t)(MW_STDFILT_ID2_MCAN1_SEL);
                            else
                                stdFiltElem[j].sfid2 = (uint32_t)(MW_STDFILT_ID2_MCAN1_SEL) + j;
                            if(MW_STDFILT_CONFIG_MCAN1_SEL == 7)
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_BUFFER;
                            else if(MW_STDFILT_CONFIG_MCAN1_SEL == 1 | MW_STDFILT_CONFIG_MCAN1_SEL == 5)
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_FIFO0;
                            else if(MW_STDFILT_CONFIG_MCAN1_SEL == 2 | MW_STDFILT_CONFIG_MCAN1_SEL == 6)  
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_FIFO1;
                            stdFiltElem[j].sft = MW_STDFILT_TYPE_MCAN1_SEL;
                        }
                    }

                    if(MW_ENABLE_EXTFILT_MCAN1_SEL == 1)
                    {
                        for(j = 0; j < MW_ELEMCNT_EXTFILT_MCAN1_SEL; j++)
                        {
                            extFiltElem[j].efid1 = (uint32_t)(MW_EXTFILT_ID1_MCAN1_SEL) + j;
                            if(MW_EXTFILT_CONFIG_MCAN1_SEL != 7)
                                extFiltElem[j].efid2 = (uint32_t)(MW_EXTFILT_ID2_MCAN1_SEL);
                            else
                                extFiltElem[j].efid2 = (uint32_t)(MW_EXTFILT_ID2_MCAN1_SEL) + j;
                            if(MW_EXTFILT_CONFIG_MCAN1_SEL  == 7)
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_BUFFER;
                            else if(MW_EXTFILT_CONFIG_MCAN1_SEL == 1 | MW_EXTFILT_CONFIG_MCAN1_SEL == 5)
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_FIFO0;
                            else if(MW_EXTFILT_CONFIG_MCAN1_SEL == 2 | MW_EXTFILT_CONFIG_MCAN1_SEL == 6)    
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_FIFO1;
                            extFiltElem[j].eft = MW_EXTFILT_TYPE_MCAN1_SEL;
                        }

                    }
                

                while (FALSE == MCAN_isMemInitDone(CSL_MCAN1_MSG_RAM_U_BASE))
                {}
                MCAN_setOpMode(CSL_MCAN1_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_SW_INIT);
                while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(CSL_MCAN1_MSG_RAM_U_BASE))
                {}

                MCAN_init(CSL_MCAN1_MSG_RAM_U_BASE, &initParams);
                MCAN_config(CSL_MCAN1_MSG_RAM_U_BASE, &configParams);
                MCAN_setBitTime(CSL_MCAN1_MSG_RAM_U_BASE, &bitTimes);
                MCAN_msgRAMConfig(CSL_MCAN1_MSG_RAM_U_BASE, &msgRAMConfigParams);
                MCAN_setExtIDAndMask(CSL_MCAN1_MSG_RAM_U_BASE, APP_MCAN_EXT_ID_MASK);

                
                    
                if(MW_ENABLE_STDFILT_MCAN1_SEL == 1)
                {
                    for(j = 0; j < MW_ELEMCNT_STDFILT_MCAN1_SEL; j++)
                    {
                        MCAN_addStdMsgIDFilter(CSL_MCAN1_MSG_RAM_U_BASE, j, &stdFiltElem[j]);
                    }
                }
                        
                if(MW_ENABLE_EXTFILT_MCAN1_SEL == 1)
                {
                    for(j = 0; j < MW_ELEMCNT_EXTFILT_MCAN1_SEL; j++)
                    {
                        MCAN_addExtMsgIDFilter(CSL_MCAN1_MSG_RAM_U_BASE, j, &extFiltElem[j]);
                    }

                }

                if (MW_ENABLE_INTLOOPBACK_MCAN1_SEL == 1)
                {
                    MCAN_lpbkModeEnable(CSL_MCAN1_MSG_RAM_U_BASE, MCAN_LPBK_MODE_INTERNAL, TRUE);
                }

                MCAN_setOpMode(CSL_MCAN1_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_NORMAL);
                while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(CSL_MCAN1_MSG_RAM_U_BASE))
                {}

            App_mcanEnableIntr(i);


            

        }
        else if(((i == 2) && (MW_ENABLE_MCAN2_SEL == 1)))
        
        {
                if((MW_ENABLE_INTLOOPBACK_MCAN2_SEL == 0) && (MW_ENABLE_GPIO_TCV_MCAN2_SEL == 1) && !tcvENABLE)
                    mcanEnableTransceiverLP();
                else if((MW_ENABLE_INTLOOPBACK_MCAN2_SEL == 0) && (MW_ENABLE_I2C_TCV_MCAN2_SEL == 1) && !tcvENABLE)
                    mcanEnableTransceiverCC();

                MCAN_initOperModeParams(&initParams);
                if(MW_PROTOCOL_MCAN2_SEL == 0)
                     initParams.fdMode = TRUE;
                else
                     initParams.fdMode = FALSE;    
                
                if(MW_ENABLE_BRS_MCAN2_SEL == 1)
                    initParams.brsEnable = TRUE;
                else
                    initParams.brsEnable = FALSE;

                MCAN_initGlobalFilterConfigParams(&configParams);
                MCAN_initSetBitTimeParams(&bitTimes);
                MCAN_initMsgRamConfigParams(&msgRAMConfigParams);

                msgRAMConfigParams.lss = MW_ELEMCNT_STDFILT_MCAN2_SEL;
                msgRAMConfigParams.lse = MW_ELEMCNT_EXTFILT_MCAN2_SEL;
                msgRAMConfigParams.txBufCnt = MW_ELEMCNT_TXBUF_MCAN2_SEL;
                msgRAMConfigParams.txFIFOCnt = MW_ELEMCNT_TXFIFO_MCAN2_SEL;
                msgRAMConfigParams.txBufMode = MCAN_TX_MEM_TYPE_BUF;
                msgRAMConfigParams.txEventFIFOCnt = MW_ELEMCNT_TXEVENTFIFO_MCAN2_SEL;
                msgRAMConfigParams.rxFIFO0Cnt = MW_ELEMCNT_RXFIFO0_MCAN2_SEL;
                msgRAMConfigParams.rxFIFO1Cnt = MW_ELEMCNT_RXFIFO1_MCAN2_SEL;
               

                if(MW_ENABLE_BLOCKINGFIFO0_MCAN2_SEL == 1)
                    msgRAMConfigParams.rxFIFO0OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;

                if(MW_ENABLE_BLOCKINGFIFO1_MCAN2_SEL == 1)
                    msgRAMConfigParams.rxFIFO1OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;  

                status = MCAN_calcMsgRamParamsStartAddr(&msgRAMConfigParams);
                DebugP_assert(status == CSL_PASS);

                if(MW_ENABLE_STDFILT_MCAN2_SEL == 1)
                    filter_cnt = (uint32_t)MW_ELEMCNT_STDFILT_MCAN2_SEL;
                else if(MW_ELEMCNT_EXTFILT_MCAN2_SEL == 1)
                    filter_cnt = (uint32_t)MW_ELEMCNT_EXTFILT_MCAN2_SEL;

                
                    if(MW_ENABLE_STDFILT_MCAN2_SEL == 1)
                    {
                        for(j = 0; j < MW_ELEMCNT_STDFILT_MCAN2_SEL; j++)
                        {
                            stdFiltElem[j].sfid1 = (uint32_t)(MW_STDFILT_ID1_MCAN2_SEL) + j;
                            if(MW_STDFILT_CONFIG_MCAN2_SEL != 7)
                                stdFiltElem[j].sfid2 = (uint32_t)(MW_STDFILT_ID2_MCAN2_SEL);
                            else
                                stdFiltElem[j].sfid2 = (uint32_t)(MW_STDFILT_ID2_MCAN2_SEL) + j;
                            if(MW_STDFILT_CONFIG_MCAN2_SEL == 7)
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_BUFFER;
                            else if(MW_STDFILT_CONFIG_MCAN2_SEL == 1 | MW_STDFILT_CONFIG_MCAN2_SEL == 5)
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_FIFO0;
                            else if(MW_STDFILT_CONFIG_MCAN2_SEL == 2 | MW_STDFILT_CONFIG_MCAN2_SEL == 6)  
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_FIFO1;
                            stdFiltElem[j].sft = MW_STDFILT_TYPE_MCAN2_SEL;
                        }
                    }

                    if(MW_ENABLE_EXTFILT_MCAN2_SEL == 1)
                    {
                        for(j = 0; j < MW_ELEMCNT_EXTFILT_MCAN2_SEL; j++)
                        {
                            extFiltElem[j].efid1 = (uint32_t)(MW_EXTFILT_ID1_MCAN2_SEL) + j;
                            if(MW_EXTFILT_CONFIG_MCAN2_SEL != 7)
                                extFiltElem[j].efid2 = (uint32_t)(MW_EXTFILT_ID2_MCAN2_SEL);
                            else
                                extFiltElem[j].efid2 = (uint32_t)(MW_EXTFILT_ID2_MCAN2_SEL) + j;
                            if(MW_EXTFILT_CONFIG_MCAN2_SEL  == 7)
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_BUFFER;
                            else if(MW_EXTFILT_CONFIG_MCAN2_SEL == 1 | MW_EXTFILT_CONFIG_MCAN2_SEL == 5)
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_FIFO0;
                            else if(MW_EXTFILT_CONFIG_MCAN2_SEL == 2 | MW_EXTFILT_CONFIG_MCAN2_SEL == 6)    
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_FIFO1;
                            extFiltElem[j].eft = MW_EXTFILT_TYPE_MCAN2_SEL;
                        }

                    }

                while (FALSE == MCAN_isMemInitDone(CSL_MCAN2_MSG_RAM_U_BASE))
                {}
                MCAN_setOpMode(CSL_MCAN2_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_SW_INIT);
                while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(CSL_MCAN2_MSG_RAM_U_BASE))
                {}

                MCAN_init(CSL_MCAN2_MSG_RAM_U_BASE, &initParams);
                MCAN_config(CSL_MCAN2_MSG_RAM_U_BASE, &configParams);
                MCAN_setBitTime(CSL_MCAN2_MSG_RAM_U_BASE, &bitTimes);
                MCAN_msgRAMConfig(CSL_MCAN2_MSG_RAM_U_BASE, &msgRAMConfigParams);
                MCAN_setExtIDAndMask(CSL_MCAN2_MSG_RAM_U_BASE, APP_MCAN_EXT_ID_MASK);

                
                    
                if(MW_ENABLE_STDFILT_MCAN2_SEL == 1)
                {
                    for(j = 0; j < MW_ELEMCNT_STDFILT_MCAN2_SEL; j++)
                    {
                        MCAN_addStdMsgIDFilter(CSL_MCAN2_MSG_RAM_U_BASE, j, &stdFiltElem[j]);
                    }
                }
                        
                if(MW_ENABLE_EXTFILT_MCAN2_SEL == 1)
                { 
                    for(j = 0; j < MW_ELEMCNT_EXTFILT_MCAN2_SEL; j++)
                    {
                        MCAN_addExtMsgIDFilter(CSL_MCAN2_MSG_RAM_U_BASE, j, &extFiltElem[j]);
                    }

                }

                if (MW_ENABLE_INTLOOPBACK_MCAN2_SEL == 1)
                {
                    MCAN_lpbkModeEnable(CSL_MCAN2_MSG_RAM_U_BASE, MCAN_LPBK_MODE_INTERNAL, TRUE);
                }

                MCAN_setOpMode(CSL_MCAN2_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_NORMAL);
                while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(CSL_MCAN2_MSG_RAM_U_BASE))
                {}

            App_mcanEnableIntr(i);

        }
        else if(((i == 3) && (MW_ENABLE_MCAN3_SEL == 1)))
       {
                if((MW_ENABLE_INTLOOPBACK_MCAN3_SEL == 0) && (MW_ENABLE_GPIO_TCV_MCAN3_SEL == 1) && !tcvENABLE)
                    mcanEnableTransceiverLP();
                else if((MW_ENABLE_INTLOOPBACK_MCAN3_SEL == 0) && (MW_ENABLE_I2C_TCV_MCAN3_SEL == 1) && !tcvENABLE)
                    mcanEnableTransceiverCC();

                MCAN_initOperModeParams(&initParams);
                if(MW_PROTOCOL_MCAN3_SEL == 0)
                     initParams.fdMode = TRUE;
                else
                     initParams.fdMode = FALSE;    
                
                if(MW_ENABLE_BRS_MCAN3_SEL == 1)
                    initParams.brsEnable = TRUE;
                else
                    initParams.brsEnable = FALSE;

                MCAN_initGlobalFilterConfigParams(&configParams);
                MCAN_initSetBitTimeParams(&bitTimes);
                MCAN_initMsgRamConfigParams(&msgRAMConfigParams);

                msgRAMConfigParams.lss = MW_ELEMCNT_STDFILT_MCAN3_SEL;
                msgRAMConfigParams.lse = MW_ELEMCNT_EXTFILT_MCAN3_SEL;
                msgRAMConfigParams.txBufCnt = MW_ELEMCNT_TXBUF_MCAN3_SEL;
                msgRAMConfigParams.txFIFOCnt = MW_ELEMCNT_TXFIFO_MCAN3_SEL;
                msgRAMConfigParams.txBufMode = MCAN_TX_MEM_TYPE_BUF;
                msgRAMConfigParams.txEventFIFOCnt = MW_ELEMCNT_TXEVENTFIFO_MCAN3_SEL;
                msgRAMConfigParams.rxFIFO0Cnt = MW_ELEMCNT_RXFIFO0_MCAN3_SEL;
                msgRAMConfigParams.rxFIFO1Cnt = MW_ELEMCNT_RXFIFO1_MCAN3_SEL;
               

                if(MW_ENABLE_BLOCKINGFIFO0_MCAN3_SEL == 1)
                    msgRAMConfigParams.rxFIFO0OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;

                if(MW_ENABLE_BLOCKINGFIFO1_MCAN3_SEL == 1)
                    msgRAMConfigParams.rxFIFO1OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;  

                status = MCAN_calcMsgRamParamsStartAddr(&msgRAMConfigParams);
                DebugP_assert(status == CSL_PASS);

                if(MW_ENABLE_STDFILT_MCAN3_SEL == 1)
                    filter_cnt = (uint32_t)MW_ELEMCNT_STDFILT_MCAN3_SEL;
                else if(MW_ELEMCNT_EXTFILT_MCAN3_SEL == 1)
                    filter_cnt = (uint32_t)MW_ELEMCNT_EXTFILT_MCAN3_SEL;

                
                    if(MW_ENABLE_STDFILT_MCAN3_SEL == 1)
                    {
                        for(j = 0; j < MW_ELEMCNT_STDFILT_MCAN3_SEL; j++)
                        {
                            stdFiltElem[j].sfid1 = (uint32_t)(MW_STDFILT_ID1_MCAN3_SEL) + j;
                            if(MW_STDFILT_CONFIG_MCAN3_SEL != 7)
                                stdFiltElem[j].sfid2 = (uint32_t)(MW_STDFILT_ID2_MCAN3_SEL);
                            else
                                stdFiltElem[j].sfid2 = (uint32_t)(MW_STDFILT_ID2_MCAN3_SEL) + j;
                            if(MW_STDFILT_CONFIG_MCAN3_SEL == 7)
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_BUFFER;
                            else if(MW_STDFILT_CONFIG_MCAN3_SEL == 1 | MW_STDFILT_CONFIG_MCAN3_SEL == 5)
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_FIFO0;
                            else if(MW_STDFILT_CONFIG_MCAN3_SEL == 2 | MW_STDFILT_CONFIG_MCAN3_SEL == 6)  
                                stdFiltElem[j].sfec = MCAN_STD_FILT_ELEM_FIFO1;
                            stdFiltElem[j].sft = MW_STDFILT_TYPE_MCAN3_SEL;
                        }
                    }

                    if(MW_ENABLE_EXTFILT_MCAN3_SEL == 1)
                    {
                        for(j = 0; j < MW_ELEMCNT_EXTFILT_MCAN3_SEL; j++)
                        {
                            extFiltElem[j].efid1 = (uint32_t)(MW_EXTFILT_ID1_MCAN3_SEL) + j;
                            if(MW_EXTFILT_CONFIG_MCAN3_SEL != 7)
                                extFiltElem[j].efid2 = (uint32_t)(MW_EXTFILT_ID2_MCAN3_SEL);
                            else
                                extFiltElem[j].efid2 = (uint32_t)(MW_EXTFILT_ID2_MCAN3_SEL) + j;
                            if(MW_EXTFILT_CONFIG_MCAN3_SEL  == 7)
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_BUFFER;
                            else if(MW_EXTFILT_CONFIG_MCAN3_SEL == 1 | MW_EXTFILT_CONFIG_MCAN3_SEL == 5)
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_FIFO0;
                            else if(MW_EXTFILT_CONFIG_MCAN3_SEL == 2 | MW_EXTFILT_CONFIG_MCAN3_SEL == 6)    
                                extFiltElem[j].efec = MCAN_EXT_FILT_ELEM_FIFO1;
                            extFiltElem[j].eft = MW_EXTFILT_TYPE_MCAN3_SEL;
                        }

                    }
                

                while (FALSE == MCAN_isMemInitDone(CSL_MCAN3_MSG_RAM_U_BASE))
                {}
                MCAN_setOpMode(CSL_MCAN3_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_SW_INIT);
                while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(CSL_MCAN3_MSG_RAM_U_BASE))
                {}

                MCAN_init(CSL_MCAN3_MSG_RAM_U_BASE, &initParams);
                MCAN_config(CSL_MCAN3_MSG_RAM_U_BASE, &configParams);
                MCAN_setBitTime(CSL_MCAN3_MSG_RAM_U_BASE, &bitTimes);
                MCAN_msgRAMConfig(CSL_MCAN3_MSG_RAM_U_BASE, &msgRAMConfigParams);
                MCAN_setExtIDAndMask(CSL_MCAN3_MSG_RAM_U_BASE, APP_MCAN_EXT_ID_MASK);

                
                    
                if(MW_ENABLE_STDFILT_MCAN3_SEL == 1)
                {
                    for(j = 0; j < MW_ELEMCNT_STDFILT_MCAN3_SEL; j++)
                    {
                        MCAN_addStdMsgIDFilter(CSL_MCAN3_MSG_RAM_U_BASE, j, &stdFiltElem[j]);
                    }
                }
                        
                if(MW_ENABLE_EXTFILT_MCAN3_SEL == 1)
                {
                    for(j = 0; j < MW_ELEMCNT_EXTFILT_MCAN3_SEL; j++)
                    {
                        MCAN_addExtMsgIDFilter(CSL_MCAN3_MSG_RAM_U_BASE, j, &extFiltElem[j]);
                    }

                }

                if (MW_ENABLE_INTLOOPBACK_MCAN3_SEL == 1)
                {
                    MCAN_lpbkModeEnable(CSL_MCAN3_MSG_RAM_U_BASE, MCAN_LPBK_MODE_INTERNAL, TRUE);
                }

                MCAN_setOpMode(CSL_MCAN3_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_NORMAL);
                while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(CSL_MCAN3_MSG_RAM_U_BASE))
                {}

            App_mcanEnableIntr(i);

        }

    }
}
}

#define MCSPI_NUM_INSTANCES (5U)
#define MCSPI_CHANNEL_NUM (2U)
MCSPI_ChConfig gConfigMcspi0ChCfg[2U];
MCSPI_ChConfig gConfigMcspi1ChCfg[2U];
MCSPI_ChConfig gConfigMcspi2ChCfg[2U];
MCSPI_ChConfig gConfigMcspi3ChCfg[2U];
MCSPI_ChConfig gConfigMcspi4ChCfg[2U];
MCSPILLD_InitObject gMcspiInitObject[MCSPI_NUM_INSTANCES];
MCSPI_ChConfig *gConfigMcspiChCfg;

// MCSPI_ChConfig gConfigMcspi1ChCfg[1U] =
// {
//     {
//         .chNum              = MCSPI_CHANNEL_0,
//         .frameFormat        = MCSPI_FF_POL0_PHA0,
//         .bitRate            = 1000000,
//         .csPolarity         = MCSPI_CS_POL_LOW,
//         .trMode             = MCSPI_TR_MODE_TX_RX,
//         .inputSelect        = MCSPI_IS_D0,
//         .dpe0               = MCSPI_DPE_ENABLE,
//         .dpe1               = MCSPI_DPE_DISABLE,
//         .slvCsSelect        = MCSPI_SLV_CS_SELECT_0,
//         .startBitEnable     = FALSE,
//         .startBitPolarity   = MCSPI_SB_POL_LOW,
//         .turboEnable        = FALSE,
//         .csIdleTime         = MCSPI_TCS0_0_CLK,
//         .defaultTxData      = 0x0U,
//         .txFifoTrigLvl      = 16U,
//         .rxFifoTrigLvl      = 16U,
//     },
// };

// MCSPILLD_InitObject gMcspiInitObject[1U] =
// {
//     {
//         .inputClkFreq                = 50000000,
//         .intrNum                     = CSLR_R5FSS0_CORE0_INTR_MCSPI1_INTR,
//         .operMode                    = MCSPI_OPER_MODE_POLLED,
//         .intrPriority                = 4U,
//         .chMode                      = MCSPI_CH_MODE_SINGLE,
//         .pinMode                     = MCSPI_PINMODE_4PIN,
//         .initDelay                   = MCSPI_INITDLY_0,
//         .multiWordAccess             = FALSE,
//         .msMode                      = MCSPI_MS_MODE_CONTROLLER,
//         .chEnabled                   = {TRUE, FALSE, FALSE, FALSE},
//         .mcspiDmaHandle              = NULL,
//         .chObj[0].chCfg              = &gConfigMcspi1ChCfg[0],
//         .chObj[0].dmaChCfg           = NULL,
//         .chObj[0].dmaChConfigNum     = 0,
//         .clockP_get                  = ClockP_getTicks,
//         .transferCallbackFxn         = NULL,
//         .errorCallbackFxn            = NULL,
//     },
// };
void MCSPI_channel_init(void)
{
    for(uint8_t i=0; i<MCSPI_NUM_INSTANCES; i++)
    {
        for(uint8_t j = 0; j<MCSPI_CHANNEL_NUM; j++)
        {
            if((i == 0) && (j == 0) && (MW_MCSPI0_ENABLE_CH0_SEL == TRUE))
            {
                gConfigMcspi0ChCfg[j].chNum =  MCSPI_CHANNEL_0;
                gConfigMcspi0ChCfg[j].frameFormat = MW_MCSPI0_CH0_FRAME_FORMAT_SEL;
                gConfigMcspi0ChCfg[j].bitRate = MW_MCSPI0_CH0_CLK_FREQ_SEL;
                if(MW_MCSPI0_CH0_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi0ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI0_CH0_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi0ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI0_TR_MODE_SEL == 0)
                    gConfigMcspi0ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI0_TR_MODE_SEL == 1)
                    gConfigMcspi0ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI0_TR_MODE_SEL == 2)
                    gConfigMcspi0ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI0_INPUT_SELECT_SEL == 0)
                    gConfigMcspi0ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI0_INPUT_SELECT_SEL == 1)
                    gConfigMcspi0ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI0_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi0ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI0_ENABLE_D0TX_SEL == 1)
                    gConfigMcspi0ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI0_ENABLE_D1TX_SEL == 0)
                gConfigMcspi0ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI0_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi0ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI0_TR_MODE_SEL == 0)
                {
                    gConfigMcspi0ChCfg[j].txFifoTrigLvl = MW_MCSPI0_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi0ChCfg[j].rxFifoTrigLvl = MW_MCSPI0_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI0_TR_MODE_SEL == 1)
                {
                    gConfigMcspi0ChCfg[j].txFifoTrigLvl = MW_MCSPI0_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI0_TR_MODE_SEL == 1)
                {
                gConfigMcspi0ChCfg[j].rxFifoTrigLvl = MW_MCSPI0_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi0ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi0ChCfg[j].startBitEnable = MW_MCSPI0_CH0_ENABLE_START_BIT_SEL;
                if(MW_MCSPI0_CH0_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi0ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI0_CH0_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi0ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi0ChCfg[j].turboEnable = MW_MCSPI0_CH0_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi0ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi0ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI0_CH0_DEFAULT_TX_DATA_SEL);

                

            }
            else if((i == 0) && (j == 1) && (MW_MCSPI0_ENABLE_CH1_SEL == TRUE))
            {

                gConfigMcspi0ChCfg[j].chNum =  MCSPI_CHANNEL_1;
                gConfigMcspi0ChCfg[j].frameFormat = MW_MCSPI0_CH1_FRAME_FORMAT_SEL;
                gConfigMcspi0ChCfg[j].bitRate = MW_MCSPI0_CH1_CLK_FREQ_SEL;
                if(MW_MCSPI0_CH1_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi0ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI0_CH1_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi0ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI0_TR_MODE_SEL == 0)
                    gConfigMcspi0ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI0_TR_MODE_SEL == 1)
                    gConfigMcspi0ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI0_TR_MODE_SEL == 2)
                    gConfigMcspi0ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI0_INPUT_SELECT_SEL == 0)
                gConfigMcspi0ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI0_INPUT_SELECT_SEL == 1)
                    gConfigMcspi0ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI0_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi0ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI0_ENABLE_D0TX_SEL == 1)
                gConfigMcspi0ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI0_ENABLE_D1TX_SEL == 0)
                gConfigMcspi0ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI0_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi0ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI0_TR_MODE_SEL == 0)
                {
                    gConfigMcspi0ChCfg[j].txFifoTrigLvl = MW_MCSPI0_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi0ChCfg[j].rxFifoTrigLvl = MW_MCSPI0_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI0_TR_MODE_SEL == 1)
                {
                    gConfigMcspi0ChCfg[j].txFifoTrigLvl = MW_MCSPI0_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI0_TR_MODE_SEL == 1)
                {
                gConfigMcspi0ChCfg[j].rxFifoTrigLvl = MW_MCSPI0_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi0ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi0ChCfg[j].startBitEnable = MW_MCSPI0_CH1_ENABLE_START_BIT_SEL;
                if(MW_MCSPI0_CH1_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi0ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI0_CH1_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi0ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi0ChCfg[j].turboEnable = MW_MCSPI0_CH1_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi0ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi0ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI0_CH1_DEFAULT_TX_DATA_SEL);

            }
            else if((i == 1) && (j == 0) && (MW_MCSPI1_ENABLE_CH0_SEL == TRUE))
            {
                gConfigMcspi1ChCfg[j].chNum =  MCSPI_CHANNEL_0;
                gConfigMcspi1ChCfg[j].frameFormat = MW_MCSPI1_CH0_FRAME_FORMAT_SEL;
                gConfigMcspi1ChCfg[j].bitRate = MW_MCSPI1_CH0_CLK_FREQ_SEL;
                if(MW_MCSPI1_CH0_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi1ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI1_CH0_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi1ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI1_TR_MODE_SEL == 0)
                    gConfigMcspi1ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI1_TR_MODE_SEL == 1)
                    gConfigMcspi1ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI1_TR_MODE_SEL == 2)
                    gConfigMcspi1ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI1_INPUT_SELECT_SEL == 0)
                gConfigMcspi1ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI1_INPUT_SELECT_SEL == 1)
                    gConfigMcspi1ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI1_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi1ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI1_ENABLE_D0TX_SEL == 1)
                gConfigMcspi1ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI1_ENABLE_D1TX_SEL == 0)
                gConfigMcspi1ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI1_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi1ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI1_TR_MODE_SEL == 0)
                {
                    gConfigMcspi1ChCfg[j].txFifoTrigLvl = MW_MCSPI1_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi1ChCfg[j].rxFifoTrigLvl = MW_MCSPI1_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI1_TR_MODE_SEL == 1)
                {
                    gConfigMcspi1ChCfg[j].txFifoTrigLvl = MW_MCSPI1_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI1_TR_MODE_SEL == 1)
                {
                gConfigMcspi1ChCfg[j].rxFifoTrigLvl = MW_MCSPI1_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi1ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi1ChCfg[j].startBitEnable = MW_MCSPI1_CH0_ENABLE_START_BIT_SEL;
                if(MW_MCSPI1_CH0_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi1ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI1_CH0_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi1ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi1ChCfg[j].turboEnable = MW_MCSPI1_CH0_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi1ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi1ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI1_CH0_DEFAULT_TX_DATA_SEL);

            }
            else if((i == 1) && (j == 1) && (MW_MCSPI1_ENABLE_CH1_SEL == TRUE))
            {

                gConfigMcspi1ChCfg[j].chNum =  MCSPI_CHANNEL_1;
                gConfigMcspi1ChCfg[j].frameFormat = MW_MCSPI1_CH1_FRAME_FORMAT_SEL;
                gConfigMcspi1ChCfg[j].bitRate = MW_MCSPI1_CH1_CLK_FREQ_SEL;
                if(MW_MCSPI1_CH1_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi1ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI1_CH1_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi1ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI1_TR_MODE_SEL == 0)
                    gConfigMcspi1ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI1_TR_MODE_SEL == 1)
                    gConfigMcspi1ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI1_TR_MODE_SEL == 2)
                    gConfigMcspi1ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI1_INPUT_SELECT_SEL == 0)
                gConfigMcspi1ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI1_INPUT_SELECT_SEL == 1)
                    gConfigMcspi1ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI1_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi1ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI1_ENABLE_D0TX_SEL == 1)
                gConfigMcspi1ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI1_ENABLE_D1TX_SEL == 0)
                gConfigMcspi1ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI1_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi1ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI1_TR_MODE_SEL == 0)
                {
                    gConfigMcspi1ChCfg[j].txFifoTrigLvl = MW_MCSPI1_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi1ChCfg[j].rxFifoTrigLvl = MW_MCSPI1_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI1_TR_MODE_SEL == 1)
                {
                    gConfigMcspi1ChCfg[j].txFifoTrigLvl = MW_MCSPI1_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI1_TR_MODE_SEL == 1)
                {
                gConfigMcspi1ChCfg[j].rxFifoTrigLvl = MW_MCSPI1_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi1ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi1ChCfg[j].startBitEnable = MW_MCSPI1_CH1_ENABLE_START_BIT_SEL;
                if(MW_MCSPI1_CH1_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi1ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI1_CH1_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi1ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi1ChCfg[j].turboEnable = MW_MCSPI1_CH1_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi1ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi1ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI1_CH1_DEFAULT_TX_DATA_SEL);

            }
            else if((i == 2) && (j == 0) && (MW_MCSPI2_ENABLE_CH0_SEL == TRUE))
            {
                gConfigMcspi2ChCfg[j].chNum =  MCSPI_CHANNEL_0;
                gConfigMcspi2ChCfg[j].frameFormat = MW_MCSPI2_CH0_FRAME_FORMAT_SEL;
                gConfigMcspi2ChCfg[j].bitRate = MW_MCSPI2_CH0_CLK_FREQ_SEL;
                if(MW_MCSPI2_CH0_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi2ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI2_CH0_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi2ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI2_TR_MODE_SEL == 0)
                    gConfigMcspi2ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI2_TR_MODE_SEL == 1)
                    gConfigMcspi2ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI2_TR_MODE_SEL == 2)
                    gConfigMcspi2ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI2_INPUT_SELECT_SEL == 0)
                gConfigMcspi2ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI2_INPUT_SELECT_SEL == 1)
                    gConfigMcspi2ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI2_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi2ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI2_ENABLE_D0TX_SEL == 1)
                gConfigMcspi2ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI2_ENABLE_D1TX_SEL == 0)
                gConfigMcspi2ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI2_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi2ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI2_TR_MODE_SEL == 0)
                {
                    gConfigMcspi2ChCfg[j].txFifoTrigLvl = MW_MCSPI2_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi2ChCfg[j].rxFifoTrigLvl = MW_MCSPI2_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI2_TR_MODE_SEL == 1)
                {
                    gConfigMcspi2ChCfg[j].txFifoTrigLvl = MW_MCSPI2_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI2_TR_MODE_SEL == 1)
                {
                gConfigMcspi2ChCfg[j].rxFifoTrigLvl = MW_MCSPI2_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi2ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi2ChCfg[j].startBitEnable = MW_MCSPI2_CH0_ENABLE_START_BIT_SEL;
                if(MW_MCSPI2_CH0_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi2ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI2_CH0_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi2ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi2ChCfg[j].turboEnable = MW_MCSPI2_CH0_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi2ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi2ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI2_CH0_DEFAULT_TX_DATA_SEL);

            }
            else if((i == 2) && (j == 1) && (MW_MCSPI2_ENABLE_CH1_SEL == TRUE))
            {

                gConfigMcspi2ChCfg[j].chNum =  MCSPI_CHANNEL_1;
                gConfigMcspi2ChCfg[j].frameFormat = MW_MCSPI2_CH1_FRAME_FORMAT_SEL;
                gConfigMcspi2ChCfg[j].bitRate = MW_MCSPI2_CH1_CLK_FREQ_SEL;
                if(MW_MCSPI2_CH1_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi2ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI2_CH1_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi2ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI2_TR_MODE_SEL == 0)
                    gConfigMcspi2ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI2_TR_MODE_SEL == 1)
                    gConfigMcspi2ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI2_TR_MODE_SEL == 2)
                    gConfigMcspi2ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI2_INPUT_SELECT_SEL == 0)
                gConfigMcspi2ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI2_INPUT_SELECT_SEL == 1)
                    gConfigMcspi2ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI2_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi2ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI2_ENABLE_D0TX_SEL == 1)
                gConfigMcspi2ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI2_ENABLE_D1TX_SEL == 0)
                gConfigMcspi2ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI2_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi2ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI2_TR_MODE_SEL == 0)
                {
                    gConfigMcspi2ChCfg[j].txFifoTrigLvl = MW_MCSPI2_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi2ChCfg[j].rxFifoTrigLvl = MW_MCSPI2_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI2_TR_MODE_SEL == 1)
                {
                    gConfigMcspi2ChCfg[j].txFifoTrigLvl = MW_MCSPI2_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI2_TR_MODE_SEL == 1)
                {
                gConfigMcspi2ChCfg[j].rxFifoTrigLvl = MW_MCSPI2_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi2ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi2ChCfg[j].startBitEnable = MW_MCSPI2_CH1_ENABLE_START_BIT_SEL;
                if(MW_MCSPI2_CH1_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi2ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI2_CH1_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi2ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi2ChCfg[j].turboEnable = MW_MCSPI2_CH1_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi2ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi2ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI2_CH1_DEFAULT_TX_DATA_SEL);

            }
            else if((i == 3) && (j == 0) && (MW_MCSPI3_ENABLE_CH0_SEL == TRUE))
            {
                gConfigMcspi3ChCfg[j].chNum =  MCSPI_CHANNEL_0;
                gConfigMcspi3ChCfg[j].frameFormat = MW_MCSPI3_CH0_FRAME_FORMAT_SEL;
                gConfigMcspi3ChCfg[j].bitRate = MW_MCSPI3_CH0_CLK_FREQ_SEL;
                if(MW_MCSPI3_CH0_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi3ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI3_CH0_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi3ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI3_TR_MODE_SEL == 0)
                    gConfigMcspi3ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI3_TR_MODE_SEL == 1)
                    gConfigMcspi3ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI3_TR_MODE_SEL == 2)
                    gConfigMcspi3ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI3_INPUT_SELECT_SEL == 0)
                gConfigMcspi3ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI3_INPUT_SELECT_SEL == 1)
                    gConfigMcspi3ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI3_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi3ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI3_ENABLE_D0TX_SEL == 1)
                gConfigMcspi3ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI3_ENABLE_D1TX_SEL == 0)
                gConfigMcspi3ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI3_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi3ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI3_TR_MODE_SEL == 0)
                {
                    gConfigMcspi3ChCfg[j].txFifoTrigLvl = MW_MCSPI3_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi3ChCfg[j].rxFifoTrigLvl = MW_MCSPI3_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI3_TR_MODE_SEL == 1)
                {
                    gConfigMcspi3ChCfg[j].txFifoTrigLvl = MW_MCSPI3_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI3_TR_MODE_SEL == 1)
                {
                gConfigMcspi3ChCfg[j].rxFifoTrigLvl = MW_MCSPI3_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi3ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi3ChCfg[j].startBitEnable = MW_MCSPI3_CH0_ENABLE_START_BIT_SEL;
                if(MW_MCSPI3_CH0_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi3ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI3_CH0_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi3ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi3ChCfg[j].turboEnable = MW_MCSPI3_CH0_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi3ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi3ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI3_CH0_DEFAULT_TX_DATA_SEL);

            }
            else if((i == 3) && (j == 1) && (MW_MCSPI3_ENABLE_CH1_SEL == TRUE))
            {

                gConfigMcspi3ChCfg[j].chNum =  MCSPI_CHANNEL_1;
                gConfigMcspi3ChCfg[j].frameFormat = MW_MCSPI3_CH1_FRAME_FORMAT_SEL;
                gConfigMcspi3ChCfg[j].bitRate = MW_MCSPI3_CH1_CLK_FREQ_SEL;
                if(MW_MCSPI3_CH1_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi3ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI3_CH1_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi3ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI3_TR_MODE_SEL == 0)
                    gConfigMcspi3ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI3_TR_MODE_SEL == 1)
                    gConfigMcspi3ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI3_TR_MODE_SEL == 2)
                    gConfigMcspi3ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI3_INPUT_SELECT_SEL == 0)
                gConfigMcspi3ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI3_INPUT_SELECT_SEL == 1)
                    gConfigMcspi3ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI3_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi3ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI3_ENABLE_D0TX_SEL == 1)
                gConfigMcspi3ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI3_ENABLE_D1TX_SEL == 0)
                gConfigMcspi3ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI3_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi3ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI3_TR_MODE_SEL == 0)
                {
                    gConfigMcspi3ChCfg[j].txFifoTrigLvl = MW_MCSPI3_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi3ChCfg[j].rxFifoTrigLvl = MW_MCSPI3_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI3_TR_MODE_SEL == 1)
                {
                    gConfigMcspi3ChCfg[j].txFifoTrigLvl = MW_MCSPI3_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI3_TR_MODE_SEL == 1)
                {
                gConfigMcspi3ChCfg[j].rxFifoTrigLvl = MW_MCSPI3_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi3ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi3ChCfg[j].startBitEnable = MW_MCSPI3_CH1_ENABLE_START_BIT_SEL;
                if(MW_MCSPI3_CH1_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi3ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI3_CH1_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi3ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi3ChCfg[j].turboEnable = MW_MCSPI3_CH1_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi3ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi3ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI3_CH1_DEFAULT_TX_DATA_SEL);

            }
            else if((i == 4) && (j == 0) && (MW_MCSPI4_ENABLE_CH0_SEL == TRUE))
            {
                gConfigMcspi4ChCfg[j].chNum =  MCSPI_CHANNEL_0;
                gConfigMcspi4ChCfg[j].frameFormat = MW_MCSPI4_CH0_FRAME_FORMAT_SEL;
                gConfigMcspi4ChCfg[j].bitRate = MW_MCSPI4_CH0_CLK_FREQ_SEL;
                if(MW_MCSPI4_CH0_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi4ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI4_CH0_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi4ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI4_TR_MODE_SEL == 0)
                    gConfigMcspi4ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI4_TR_MODE_SEL == 1)
                    gConfigMcspi4ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI4_TR_MODE_SEL == 2)
                    gConfigMcspi4ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI4_INPUT_SELECT_SEL == 0)
                gConfigMcspi4ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI4_INPUT_SELECT_SEL == 1)
                    gConfigMcspi4ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI4_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi4ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI4_ENABLE_D0TX_SEL == 1)
                gConfigMcspi4ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI4_ENABLE_D1TX_SEL == 0)
                gConfigMcspi4ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI4_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi4ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI4_TR_MODE_SEL == 0)
                {
                    gConfigMcspi4ChCfg[j].txFifoTrigLvl = MW_MCSPI4_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi4ChCfg[j].rxFifoTrigLvl = MW_MCSPI4_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI4_TR_MODE_SEL == 1)
                {
                    gConfigMcspi4ChCfg[j].txFifoTrigLvl = MW_MCSPI4_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI4_TR_MODE_SEL == 1)
                {
                gConfigMcspi4ChCfg[j].rxFifoTrigLvl = MW_MCSPI4_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi4ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi4ChCfg[j].startBitEnable = MW_MCSPI4_CH0_ENABLE_START_BIT_SEL;
                if(MW_MCSPI4_CH0_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi4ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI4_CH0_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi4ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi4ChCfg[j].turboEnable = MW_MCSPI4_CH0_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi4ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi4ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI4_CH0_DEFAULT_TX_DATA_SEL);

            }
            else if((i == 4) && (j == 1) && (MW_MCSPI4_ENABLE_CH1_SEL == TRUE))
            {

                gConfigMcspi4ChCfg[j].chNum =  MCSPI_CHANNEL_1;
                gConfigMcspi4ChCfg[j].frameFormat = MW_MCSPI4_CH1_FRAME_FORMAT_SEL;
                gConfigMcspi4ChCfg[j].bitRate = MW_MCSPI4_CH1_CLK_FREQ_SEL;
                if(MW_MCSPI4_CH1_CHIP_SELECT_POLARITY_SEL == 0)
                gConfigMcspi4ChCfg[j].csPolarity = MCSPI_CS_POL_LOW;
                else if(MW_MCSPI4_CH1_CHIP_SELECT_POLARITY_SEL == 1)
                    gConfigMcspi4ChCfg[j].csPolarity = MCSPI_CS_POL_HIGH;
                if(MW_MCSPI4_TR_MODE_SEL == 0)
                    gConfigMcspi4ChCfg[j].trMode = MCSPI_TR_MODE_TX_RX;
                else if(MW_MCSPI4_TR_MODE_SEL == 1)
                    gConfigMcspi4ChCfg[j].trMode = MCSPI_TR_MODE_TX_ONLY;
                else if(MW_MCSPI4_TR_MODE_SEL == 2)
                    gConfigMcspi4ChCfg[j].trMode = MCSPI_TR_MODE_RX_ONLY;
                if(MW_MCSPI4_INPUT_SELECT_SEL == 0)
                gConfigMcspi4ChCfg[j].inputSelect = MCSPI_IS_D0;
                else if(MW_MCSPI4_INPUT_SELECT_SEL == 1)
                    gConfigMcspi4ChCfg[j].inputSelect = MCSPI_IS_D1;
                if(MW_MCSPI4_ENABLE_D0TX_SEL == 0)
                    gConfigMcspi4ChCfg[j].dpe0 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI4_ENABLE_D0TX_SEL == 1)
                gConfigMcspi4ChCfg[j].dpe0 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI4_ENABLE_D1TX_SEL == 0)
                gConfigMcspi4ChCfg[j].dpe1 =  MCSPI_DPE_DISABLE;
                else if(MW_MCSPI4_ENABLE_D1TX_SEL == 1)
                    gConfigMcspi4ChCfg[j].dpe1 =  MCSPI_DPE_ENABLE;   
                if(MW_MCSPI4_TR_MODE_SEL == 0)
                {
                    gConfigMcspi4ChCfg[j].txFifoTrigLvl = MW_MCSPI4_FIFO_TRIGLVLTX_SEL;
                    gConfigMcspi4ChCfg[j].rxFifoTrigLvl = MW_MCSPI4_FIFO_TRIGLVLRX_SEL;   
                }
                else if(MW_MCSPI4_TR_MODE_SEL == 1)
                {
                    gConfigMcspi4ChCfg[j].txFifoTrigLvl = MW_MCSPI4_FIFO_TRIGLVLTX_SEL;    
                }
                else if(MW_MCSPI4_TR_MODE_SEL == 1)
                {
                gConfigMcspi4ChCfg[j].rxFifoTrigLvl = MW_MCSPI4_FIFO_TRIGLVLRX_SEL; 
                }
                gConfigMcspi4ChCfg[j].slvCsSelect = MCSPI_SLV_CS_SELECT_0;
                gConfigMcspi4ChCfg[j].startBitEnable = MW_MCSPI4_CH1_ENABLE_START_BIT_SEL;
                if(MW_MCSPI4_CH1_START_BIT_POLARITY_SEL == 0)
                    gConfigMcspi4ChCfg[j].startBitPolarity = MCSPI_SB_POL_LOW;
                else if(MW_MCSPI4_CH1_START_BIT_POLARITY_SEL == 1)  
                    gConfigMcspi4ChCfg[j].startBitPolarity = MCSPI_SB_POL_HIGH; 
                gConfigMcspi4ChCfg[j].turboEnable = MW_MCSPI4_CH1_ENABLE_TURBO_MODE_SEL;
                gConfigMcspi4ChCfg[j].csIdleTime = MCSPI_TCS0_0_CLK;
                gConfigMcspi4ChCfg[j].defaultTxData = (uint32_t)(MW_MCSPI4_CH1_DEFAULT_TX_DATA_SEL);

            }



        }
    }
}

void MCSPI_object_init(void)
{
    bool channel0_enable = false;
    bool channel1_enable = false;

    for(uint8_t i = 0; i< MCSPI_NUM_INSTANCES; i++)
    {
        channel0_enable = false;
        channel1_enable = false;

        gMcspiInitObject[i].inputClkFreq = 50000000;
        gMcspiInitObject[i].operMode  = MCSPI_OPER_MODE_POLLED;
        gMcspiInitObject[i].intrPriority = 4U;
        gMcspiInitObject[i].mcspiDmaHandle = NULL;
        gMcspiInitObject[i].clockP_get = ClockP_getTicks;
        gMcspiInitObject[i].transferCallbackFxn = NULL;
        gMcspiInitObject[i].errorCallbackFxn = NULL;
        if(i==0)
        {
            gMcspiInitObject[i].intrNum = CSLR_R5FSS0_CORE0_INTR_MCSPI0_INTR;
            if((MW_MCSPI0_MODE_OPERATION_SEL == 0) | (MW_MCSPI0_MODE_OPERATION_SEL == 2))
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_SINGLE;
            else if(MW_MCSPI0_MODE_OPERATION_SEL == 1)  
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_MULTI;
            if(MW_MCSPI0_PIN_MODE_SEL == 0)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_3PIN;
            else if(MW_MCSPI0_PIN_MODE_SEL == 1)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_4PIN; 
            if(MW_MCSPI0_INIT_DELAY_SEL == 0)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_0;
            else if(MW_MCSPI0_INIT_DELAY_SEL == 1)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_4;
            else if(MW_MCSPI0_INIT_DELAY_SEL == 2)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_8;
            else if(MW_MCSPI0_INIT_DELAY_SEL == 3)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_16;
            else if(MW_MCSPI0_INIT_DELAY_SEL == 4)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_32;
            gMcspiInitObject[i].multiWordAccess = MW_MCSPI0_ENABLE_MULTIWORD_ACCESS_SEL;
            if((MW_MCSPI0_MODE_OPERATION_SEL == 0) | (MW_MCSPI0_MODE_OPERATION_SEL == 1))
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_CONTROLLER;
            else if(MW_MCSPI0_MODE_OPERATION_SEL == 2)
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_PERIPHERAL;
            if(MW_MCSPI0_ENABLE_CH0_SEL == TRUE)
                channel0_enable = true;
            if(MW_MCSPI0_ENABLE_CH1_SEL == 1)
            {
                channel1_enable = true;
                gMcspiInitObject[i].chObj[1].chCfg = &gConfigMcspi0ChCfg[0];
                gMcspiInitObject[i].chObj[1].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[1].dmaChConfigNum = 0;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi0ChCfg[1];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;
            }
            else
            {
                channel1_enable = false;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi0ChCfg[0];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;

            }   

            gMcspiInitObject[i].chEnabled[0] = channel0_enable; 
            gMcspiInitObject[i].chEnabled[1] = channel1_enable;
            gMcspiInitObject[i].chEnabled[2] = FALSE;
            gMcspiInitObject[i].chEnabled[3] = FALSE; 

        }     
        else if(i==1)
        {
            gMcspiInitObject[i].intrNum = CSLR_R5FSS0_CORE0_INTR_MCSPI1_INTR;
            if((MW_MCSPI1_MODE_OPERATION_SEL == 0) | (MW_MCSPI1_MODE_OPERATION_SEL == 2))
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_SINGLE;
            else if(MW_MCSPI1_MODE_OPERATION_SEL == 1)  
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_MULTI;
            if(MW_MCSPI1_PIN_MODE_SEL == 0)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_3PIN;
            else if(MW_MCSPI1_PIN_MODE_SEL == 1)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_4PIN; 
            if(MW_MCSPI1_INIT_DELAY_SEL == 0)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_0;
            else if(MW_MCSPI1_INIT_DELAY_SEL == 1)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_4;
            else if(MW_MCSPI1_INIT_DELAY_SEL == 2)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_8;
            else if(MW_MCSPI1_INIT_DELAY_SEL == 3)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_16;
            else if(MW_MCSPI1_INIT_DELAY_SEL == 4)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_32;
            gMcspiInitObject[i].multiWordAccess = MW_MCSPI1_ENABLE_MULTIWORD_ACCESS_SEL;
            if((MW_MCSPI1_MODE_OPERATION_SEL == 0) | (MW_MCSPI1_MODE_OPERATION_SEL == 1))
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_CONTROLLER;
            else if(MW_MCSPI1_MODE_OPERATION_SEL == 2)
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_PERIPHERAL;
            if(MW_MCSPI1_ENABLE_CH0_SEL == TRUE)
                channel0_enable = true;

            if(MW_MCSPI1_ENABLE_CH1_SEL == 1)
            {
                channel1_enable = true;
                gMcspiInitObject[i].chObj[1].chCfg = &gConfigMcspi1ChCfg[0];
                gMcspiInitObject[i].chObj[1].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[1].dmaChConfigNum = 0;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi1ChCfg[1];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;
            }
            else
            {
                channel1_enable = false;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi1ChCfg[0];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;

            }  

            gMcspiInitObject[i].chEnabled[0] = channel0_enable; 
            gMcspiInitObject[i].chEnabled[1] = channel1_enable;
            gMcspiInitObject[i].chEnabled[2] = false;
            gMcspiInitObject[i].chEnabled[3] = false;
        }
        else if(i==2)
        {
            gMcspiInitObject[i].intrNum = CSLR_R5FSS0_CORE0_INTR_MCSPI2_INTR;
            if((MW_MCSPI2_MODE_OPERATION_SEL == 0) | (MW_MCSPI2_MODE_OPERATION_SEL == 2))
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_SINGLE;
            else if(MW_MCSPI2_MODE_OPERATION_SEL == 1)  
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_MULTI;
            if(MW_MCSPI2_PIN_MODE_SEL == 0)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_3PIN;
            else if(MW_MCSPI2_PIN_MODE_SEL == 1)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_4PIN; 
            if(MW_MCSPI2_INIT_DELAY_SEL == 0)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_0;
            else if(MW_MCSPI2_INIT_DELAY_SEL == 1)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_4;
            else if(MW_MCSPI2_INIT_DELAY_SEL == 2)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_8;
            else if(MW_MCSPI2_INIT_DELAY_SEL == 3)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_16;
            else if(MW_MCSPI2_INIT_DELAY_SEL == 4)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_32;
            gMcspiInitObject[i].multiWordAccess = MW_MCSPI2_ENABLE_MULTIWORD_ACCESS_SEL;
            if((MW_MCSPI2_MODE_OPERATION_SEL == 0) | (MW_MCSPI2_MODE_OPERATION_SEL == 1))
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_CONTROLLER;
            else if(MW_MCSPI2_MODE_OPERATION_SEL == 2)
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_PERIPHERAL;
            if(MW_MCSPI2_ENABLE_CH0_SEL == TRUE)
                channel0_enable = true;
            if(MW_MCSPI2_ENABLE_CH1_SEL == 1)
            {
                channel1_enable = true;
                gMcspiInitObject[i].chObj[1].chCfg = &gConfigMcspi2ChCfg[0];
                gMcspiInitObject[i].chObj[1].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[1].dmaChConfigNum = 0;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi2ChCfg[1];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;
            }
            else
            {
                channel1_enable = false;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi2ChCfg[0];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;

            }  
            gMcspiInitObject[i].chEnabled[0] = channel0_enable; 
            gMcspiInitObject[i].chEnabled[1] = channel1_enable;
            gMcspiInitObject[i].chEnabled[2] = false;
            gMcspiInitObject[i].chEnabled[3] = false;
        }
        else if(i==3)
        {
            gMcspiInitObject[i].intrNum = CSLR_R5FSS0_CORE0_INTR_MCSPI3_INTR;
            if((MW_MCSPI3_MODE_OPERATION_SEL == 0) | (MW_MCSPI3_MODE_OPERATION_SEL == 2))
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_SINGLE;
            else if(MW_MCSPI3_MODE_OPERATION_SEL == 1)  
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_MULTI;
            if(MW_MCSPI3_PIN_MODE_SEL == 0)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_3PIN;
            else if(MW_MCSPI3_PIN_MODE_SEL == 1)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_4PIN; 
            if(MW_MCSPI3_INIT_DELAY_SEL == 0)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_0;
            else if(MW_MCSPI3_INIT_DELAY_SEL == 1)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_4;
            else if(MW_MCSPI3_INIT_DELAY_SEL == 2)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_8;
            else if(MW_MCSPI3_INIT_DELAY_SEL == 3)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_16;
            else if(MW_MCSPI3_INIT_DELAY_SEL == 4)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_32;
            gMcspiInitObject[i].multiWordAccess = MW_MCSPI3_ENABLE_MULTIWORD_ACCESS_SEL;
            if((MW_MCSPI3_MODE_OPERATION_SEL == 0) | (MW_MCSPI3_MODE_OPERATION_SEL == 1))
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_CONTROLLER;
            else if(MW_MCSPI3_MODE_OPERATION_SEL == 2)
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_PERIPHERAL;
            if(MW_MCSPI3_ENABLE_CH0_SEL == TRUE)
                channel0_enable = true;
            if(MW_MCSPI3_ENABLE_CH1_SEL == 1)
            {
                channel1_enable = true;
                gMcspiInitObject[i].chObj[1].chCfg = &gConfigMcspi3ChCfg[0];
                gMcspiInitObject[i].chObj[1].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[1].dmaChConfigNum = 0;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi3ChCfg[1];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;
            }
            else
            {
                channel1_enable = false;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi3ChCfg[0];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;

            }
            gMcspiInitObject[i].chEnabled[0] = channel0_enable; 
            gMcspiInitObject[i].chEnabled[1] = channel1_enable;
            gMcspiInitObject[i].chEnabled[2] = false;
            gMcspiInitObject[i].chEnabled[3] = false;
        }
        else if(i==4)
        {

            gMcspiInitObject[i].intrNum = CSLR_R5FSS0_CORE0_INTR_MCSPI4_INTR;
            if((MW_MCSPI4_MODE_OPERATION_SEL == 0) | (MW_MCSPI4_MODE_OPERATION_SEL == 2))
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_SINGLE;
            else if(MW_MCSPI4_MODE_OPERATION_SEL == 1)  
                gMcspiInitObject[i].chMode = MCSPI_CH_MODE_MULTI;
            if(MW_MCSPI4_PIN_MODE_SEL == 0)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_3PIN;
            else if(MW_MCSPI4_PIN_MODE_SEL == 1)
                gMcspiInitObject[i].pinMode = MCSPI_PINMODE_4PIN; 
            if(MW_MCSPI4_INIT_DELAY_SEL == 0)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_0;
            else if(MW_MCSPI4_INIT_DELAY_SEL == 1)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_4;
            else if(MW_MCSPI4_INIT_DELAY_SEL == 2)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_8;
            else if(MW_MCSPI4_INIT_DELAY_SEL == 3)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_16;
            else if(MW_MCSPI4_INIT_DELAY_SEL == 4)
                gMcspiInitObject[i].initDelay = MCSPI_INITDLY_32;
            gMcspiInitObject[i].multiWordAccess = MW_MCSPI4_ENABLE_MULTIWORD_ACCESS_SEL;
            if((MW_MCSPI4_MODE_OPERATION_SEL == 0) | (MW_MCSPI4_MODE_OPERATION_SEL == 1))
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_CONTROLLER;
            else if(MW_MCSPI4_MODE_OPERATION_SEL == 2)
                gMcspiInitObject[i].msMode = MCSPI_MS_MODE_PERIPHERAL;
            if(MW_MCSPI4_ENABLE_CH0_SEL == TRUE)
                channel0_enable = true;
            if(MW_MCSPI4_ENABLE_CH1_SEL == 1)
            {
                channel1_enable = true;
                gMcspiInitObject[i].chObj[1].chCfg = &gConfigMcspi4ChCfg[0];
                gMcspiInitObject[i].chObj[1].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[1].dmaChConfigNum = 0;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi4ChCfg[1];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;
            }
            else
            {
                channel1_enable = false;
                gMcspiInitObject[i].chObj[0].chCfg = &gConfigMcspi4ChCfg[0];
                gMcspiInitObject[i].chObj[0].dmaChCfg = NULL;
                gMcspiInitObject[i].chObj[0].dmaChConfigNum = 0;

            } 

            gMcspiInitObject[i].chEnabled[0] = channel0_enable; 
            gMcspiInitObject[i].chEnabled[1] = channel1_enable;
            gMcspiInitObject[i].chEnabled[2] = false;
            gMcspiInitObject[i].chEnabled[3] = false;

        }
    }
}

MCSPILLD_Object gMcspiObject[MCSPI_NUM_INSTANCES];
MCSPILLD_Handle gMcspiHandle[MCSPI_NUM_INSTANCES];

void Drivers_mcspiOpen()
{
    uint8_t i = 0;
    int32_t status = MCSPI_STATUS_SUCCESS;
    for(i=0; i<MCSPI_NUM_INSTANCES; i++)
    {
        gMcspiHandle[i] = &gMcspiObject[i];
        gMcspiHandle[i]->state = MCSPI_STATE_RESET;
        if(i==0)
            gMcspiHandle[i]->baseAddr = CSL_MCSPI0_U_BASE;
        else if(i==1)
            gMcspiHandle[i]->baseAddr = CSL_MCSPI1_U_BASE;
        else if(i==2)
            gMcspiHandle[i]->baseAddr = CSL_MCSPI2_U_BASE;
        else if(i==3)
            gMcspiHandle[i]->baseAddr = CSL_MCSPI3_U_BASE;
        gMcspiHandle[i]->hMcspiInit = &gMcspiInitObject[i];

        if((i==0) && (MW_ENABLE_MCSPI0_SEL == 1))
            status = MCSPI_lld_init(gMcspiHandle[0]);
        else if((i==1) && (MW_ENABLE_MCSPI1_SEL == 1))
            status = MCSPI_lld_init(gMcspiHandle[1]);
        else if((i==2) && (MW_ENABLE_MCSPI2_SEL == 1))
            status = MCSPI_lld_init(gMcspiHandle[2]);
        else if((i==3) && (MW_ENABLE_MCSPI3_SEL == 1))
            status = MCSPI_lld_init(gMcspiHandle[3]);
        else if((i==4) && (MW_ENABLE_MCSPI4_SEL == 1))
            status = MCSPI_lld_init(gMcspiHandle[4]);

        DebugP_assert(status == MCSPI_STATUS_SUCCESS);   
    }

    return;
}

#define MSS_MBOX_MEM                (CSL_MBOX_SRAM_U_BASE)
#define MSS_MBOX_MEM_SIZE           (16U*1024U)

/*
* SW queue between each pair of CPUs
*
* place SW queues at the bottom of the dedicated mailbox memories.
* Driver assume this memory is init to zero in bootloader as it's ECC protected and
* needs to be intialized only once and to ensure that only one core has done the
* mailbox ram initialization before ipc_init. If SBL is not used then Gel does the initialization.
* We need 4*3 SW Q's for the 4x R5F to send messages to each other, i.e 384 B.
*
* Rest of the mailbox memory cna be used for ipc_rpmessage or custom message passing.
*/
#define R5FSS0_0_TO_R5FSS0_1_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*12U))
#define R5FSS0_0_TO_R5FSS1_0_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*11U))
#define R5FSS0_0_TO_R5FSS1_1_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*10U))
#define R5FSS0_1_TO_R5FSS0_0_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*9U))
#define R5FSS0_1_TO_R5FSS1_0_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*8U))
#define R5FSS0_1_TO_R5FSS1_1_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*7U))
#define R5FSS1_0_TO_R5FSS0_0_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*6U))
#define R5FSS1_0_TO_R5FSS0_1_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*5U))
#define R5FSS1_0_TO_R5FSS1_1_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*4U))
#define R5FSS1_1_TO_R5FSS0_0_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*3U))
#define R5FSS1_1_TO_R5FSS0_1_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*2U))
#define R5FSS1_1_TO_R5FSS1_0_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*1U))

#define MSS_MBOX_MEM                (CSL_MBOX_SRAM_U_BASE)
#define MSS_MBOX_MEM_SIZE           (16U*1024U)

#include <drivers/ipc_rpmsg.h>


uint16_t IPC_Shared_Mem_SizeMemorySize = 0U;
uint16_t VringMemorySize = 0U;
uint16_t VringNumBuffers = 0U;
uint16_t VringBufferSize = 0U;
uint16_t TotalVringCount = 0U;


/* This function is called within IpcNotify_init, this function returns core specific IPC config */
void IpcNotify_getConfig(IpcNotify_InterruptConfig **interruptConfig, uint32_t *interruptConfigNum)
{
//     /* extern globals that are specific to this core */
    extern IpcNotify_InterruptConfig gIpcNotifyInterruptConfig_r5fss0_0[];
    extern uint32_t gIpcNotifyInterruptConfigNum_r5fss0_0;
    extern IpcNotify_InterruptConfig gIpcNotifyInterruptConfig_r5fss0_1[];
    extern uint32_t gIpcNotifyInterruptConfigNum_r5fss0_1;
    extern IpcNotify_InterruptConfig gIpcNotifyInterruptConfig_r5fss1_0[];
    extern uint32_t gIpcNotifyInterruptConfigNum_r5fss1_0;
    extern IpcNotify_InterruptConfig gIpcNotifyInterruptConfig_r5fss1_1[];
    extern uint32_t gIpcNotifyInterruptConfigNum_r5fss1_1;

    if(MW_CPUINFO_CORESELECTION == 0)
    {
        *interruptConfig = &gIpcNotifyInterruptConfig_r5fss0_0[0];
        *interruptConfigNum = gIpcNotifyInterruptConfigNum_r5fss0_0;
    }
    else if(MW_CPUINFO_CORESELECTION == 1)
    {
        *interruptConfig = &gIpcNotifyInterruptConfig_r5fss0_1[0];
        *interruptConfigNum = gIpcNotifyInterruptConfigNum_r5fss0_1;
    }
    else if(MW_CPUINFO_CORESELECTION == 2)
    {
        *interruptConfig = &gIpcNotifyInterruptConfig_r5fss1_0[0];
        *interruptConfigNum = gIpcNotifyInterruptConfigNum_r5fss1_0;
    }
    else if(MW_CPUINFO_CORESELECTION == 3)
    {
        *interruptConfig = &gIpcNotifyInterruptConfig_r5fss1_1[0];
        *interruptConfigNum = gIpcNotifyInterruptConfigNum_r5fss1_1;
    }
}

/* This function is called within IpcNotify_init, this function allocates SW queue */
void IpcNotify_allocSwQueue(IpcNotify_MailboxConfig *mailboxConfig)
{
    IpcNotify_MailboxConfig (*mailboxConfigPtr)[CSL_CORE_ID_MAX] = (void *)mailboxConfig;

    if(MW_CPUINFO_CORESELECTION == 0)
    {
        if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_0][CSL_CORE_ID_R5FSS0_1].swQ = R5FSS0_0_TO_R5FSS0_1_SW_QUEUE;   
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_1][CSL_CORE_ID_R5FSS0_0].swQ = R5FSS0_1_TO_R5FSS0_0_SW_QUEUE;
        }

        if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_0][CSL_CORE_ID_R5FSS1_0].swQ = R5FSS0_0_TO_R5FSS1_0_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_0][CSL_CORE_ID_R5FSS0_0].swQ = R5FSS1_0_TO_R5FSS0_0_SW_QUEUE;
        }

        if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_0][CSL_CORE_ID_R5FSS1_1].swQ = R5FSS0_0_TO_R5FSS1_1_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_1][CSL_CORE_ID_R5FSS0_0].swQ = R5FSS1_1_TO_R5FSS0_0_SW_QUEUE;
        }
    }
    else if(MW_CPUINFO_CORESELECTION == 1)
    {
        if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_1][CSL_CORE_ID_R5FSS0_0].swQ = R5FSS0_1_TO_R5FSS0_0_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_0][CSL_CORE_ID_R5FSS0_1].swQ = R5FSS0_0_TO_R5FSS0_1_SW_QUEUE;
        }

        if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_1][CSL_CORE_ID_R5FSS1_0].swQ = R5FSS0_1_TO_R5FSS1_0_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_0][CSL_CORE_ID_R5FSS0_1].swQ = R5FSS1_0_TO_R5FSS0_1_SW_QUEUE;
        }

        if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_1][CSL_CORE_ID_R5FSS1_1].swQ = R5FSS0_1_TO_R5FSS1_1_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_1][CSL_CORE_ID_R5FSS0_1].swQ = R5FSS1_1_TO_R5FSS0_1_SW_QUEUE; 
        }
    }
    else if(MW_CPUINFO_CORESELECTION == 2)
    {
        if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_0][CSL_CORE_ID_R5FSS0_0].swQ = R5FSS1_0_TO_R5FSS0_0_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_0][CSL_CORE_ID_R5FSS1_0].swQ = R5FSS0_0_TO_R5FSS1_0_SW_QUEUE;
        }

        if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_0][CSL_CORE_ID_R5FSS0_1].swQ = R5FSS1_0_TO_R5FSS0_1_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_1][CSL_CORE_ID_R5FSS1_0].swQ = R5FSS0_1_TO_R5FSS1_0_SW_QUEUE;
        }

        if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_0][CSL_CORE_ID_R5FSS1_1].swQ = R5FSS1_0_TO_R5FSS1_1_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_1][CSL_CORE_ID_R5FSS1_0].swQ = R5FSS1_1_TO_R5FSS1_0_SW_QUEUE;
        }   
    }
    else if(MW_CPUINFO_CORESELECTION == 3)
    {
        if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_1][CSL_CORE_ID_R5FSS0_0].swQ = R5FSS1_1_TO_R5FSS0_0_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_0][CSL_CORE_ID_R5FSS1_1].swQ = R5FSS0_0_TO_R5FSS1_1_SW_QUEUE;
        }

        if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_1][CSL_CORE_ID_R5FSS0_1].swQ = R5FSS1_1_TO_R5FSS0_1_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS0_1][CSL_CORE_ID_R5FSS1_1].swQ = R5FSS0_1_TO_R5FSS1_1_SW_QUEUE;
        }

        if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
        {
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_1][CSL_CORE_ID_R5FSS1_0].swQ = R5FSS1_1_TO_R5FSS1_0_SW_QUEUE;
            mailboxConfigPtr[CSL_CORE_ID_R5FSS1_0][CSL_CORE_ID_R5FSS1_1].swQ = R5FSS1_0_TO_R5FSS1_1_SW_QUEUE;
        }
    }
}

uint16_t Core0_Tx_Core1_Rx_Core0 = 0U;
uint16_t Core0_Tx_Core2_Rx_Core0 = 0U;
uint16_t Core0_Tx_Core3_Rx_Core0 = 0U;
uint16_t Core1_Tx_Core0_Rx_Core1 = 0U;
uint16_t Core1_Tx_Core2_Rx_Core1 = 0U;
uint16_t Core1_Tx_Core3_Rx_Core1 = 0U;
uint16_t Core2_Tx_Core0_Rx_Core2 = 0U;
uint16_t Core2_Tx_Core1_Rx_Core2 = 0U;
uint16_t Core2_Tx_Core3_Rx_Core2 = 0U;
uint16_t Core3_Tx_Core0_Rx_Core3 = 0U;
uint16_t Core3_Tx_Core1_Rx_Core3 = 0U;
uint16_t Core3_Tx_Core2_Rx_Core3 = 0U;

#define IPC_RPMESSAGE_VRING_SIZE          RPMESSAGE_VRING_SIZE(MW_RP_NUM_BUFFERS_SEL, MW_RP_BUFFER_SIZE_SEL)
#define IPC_SHARED_MEM_SIZE               (IPC_RPMESSAGE_VRING_SIZE * (uint16_t)((MW_NUM_REMOTE_CORES_SEL+1)*(MW_NUM_REMOTE_CORES_SEL)))

uint8_t gIpcSharedMem[IPC_SHARED_MEM_SIZE] __attribute__((aligned(128), section(".bss.ipc_vring_mem")));

uint16_t IPCCores = 0U;

void RPMemory_init()
{

    uint16_t offSetTx = 0U;
    uint16_t offSetRx = 0U;

    uint16_t VringSize = RPMESSAGE_VRING_SIZE(MW_RP_NUM_BUFFERS_SEL, MW_RP_BUFFER_SIZE_SEL);

    
    if(((MW_CPUINFO_CORESELECTION == 0) && (MW_R5FSS0CORE0_ENABLE_SELFCORE_SEL == 1)))
    {
        if((MW_R5FSS0CORE0_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 1))
        {
            Core0_Tx_Core1_Rx_Core0 = offSetTx;
            offSetTx += VringSize;
        }

        if((MW_R5FSS0CORE0_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS1CORE0_ENABLE_SEL == 1))
        {
            Core0_Tx_Core2_Rx_Core0 = offSetTx;
            offSetTx += VringSize;
        }

        if((MW_R5FSS0CORE0_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS1CORE1_ENABLE_SEL == 1))
        {
            Core0_Tx_Core3_Rx_Core0 = offSetTx;
            // offSetTx += VringSize;
        }

    }
    else if((MW_CPUINFO_CORESELECTION == 1) && (MW_R5FSS0CORE1_ENABLE_SELFCORE_SEL == 1))
    {
        if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
            offSetTx += (IPCCores * VringSize);

        if((MW_R5FSS0CORE1_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS0CORE0_ENABLE_SEL == 1))
        {
            Core1_Tx_Core0_Rx_Core1 = offSetTx;
            offSetTx += VringSize;
        }

        if((MW_R5FSS0CORE1_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS1CORE0_ENABLE_SEL == 1))
        {
            Core1_Tx_Core2_Rx_Core1 = offSetTx;
            offSetTx += VringSize;
        }

        if((MW_R5FSS0CORE1_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS1CORE1_ENABLE_SEL == 1))
        {
            Core1_Tx_Core3_Rx_Core1 = offSetTx;
        }

    }
    else if((MW_CPUINFO_CORESELECTION == 2) && (MW_R5FSS1CORE0_ENABLE_SELFCORE_SEL == 1))
    {
        if((MW_R5FSS0CORE0_ENABLE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 1))
            offSetTx += (2 * (IPCCores * VringSize));
        else if((MW_R5FSS0CORE0_ENABLE_SEL == 1) || (MW_R5FSS0CORE1_ENABLE_SEL == 1))
            offSetTx += (IPCCores * VringSize);

        if((MW_R5FSS1CORE0_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS0CORE0_ENABLE_SEL == 1))
        {
            Core2_Tx_Core0_Rx_Core2 = offSetTx;
            offSetTx += VringSize;
        }

        if((MW_R5FSS1CORE0_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 1))
        {
            Core2_Tx_Core1_Rx_Core2 = offSetTx;
            offSetTx += VringSize;
        }

        if((MW_R5FSS1CORE0_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS1CORE1_ENABLE_SEL == 1))
        {
            Core2_Tx_Core3_Rx_Core2 = offSetTx;
            offSetTx += VringSize;
        }

    }
    else if((MW_CPUINFO_CORESELECTION == 3) && (MW_R5FSS1CORE1_ENABLE_SELFCORE_SEL == 1))
    {
        if((MW_R5FSS0CORE0_ENABLE_SEL == 1))
            offSetTx += (IPCCores * VringSize);

        if((MW_R5FSS0CORE1_ENABLE_SEL == 1))
            offSetTx += (IPCCores * VringSize);

        if((MW_R5FSS1CORE0_ENABLE_SEL == 1))
            offSetTx += (IPCCores * VringSize);
        

        if((MW_R5FSS1CORE1_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS0CORE0_ENABLE_SEL == 1))
        {
            Core3_Tx_Core0_Rx_Core3 = offSetTx;
            offSetTx += VringSize;
        }

        if((MW_R5FSS1CORE1_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 1))
        {
            Core3_Tx_Core1_Rx_Core3 = offSetTx;
            offSetTx += VringSize;
        }

        if((MW_R5FSS1CORE1_ENABLE_SELFCORE_SEL == 1) && (MW_R5FSS1CORE0_ENABLE_SEL == 1))
        {
            Core3_Tx_Core2_Rx_Core3 = offSetTx;
            offSetTx += VringSize;
        }

    }


    if(((MW_CPUINFO_CORESELECTION == 0) && (MW_R5FSS0CORE0_ENABLE_SELFCORE_SEL == 1)))
    {
        offSetRx += (IPCCores * VringSize);

        if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
        {
            Core1_Tx_Core0_Rx_Core1 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }

        if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
        {
            Core2_Tx_Core0_Rx_Core2 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }

        if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
        {
            Core3_Tx_Core0_Rx_Core3 = offSetRx;
           
        }

    }
    else if(((MW_CPUINFO_CORESELECTION == 1) && (MW_R5FSS0CORE1_ENABLE_SELFCORE_SEL == 1)))
    {

        if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
        {
            Core0_Tx_Core1_Rx_Core0 = offSetRx; 
            offSetRx += (IPCCores * VringSize);
            offSetRx += VringSize;
            offSetRx += (IPCCores * VringSize);
        }
        else
        {
            offSetRx += (IPCCores * VringSize);
        }

        if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
        {
            Core2_Tx_Core1_Rx_Core2 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }

        if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
        {
            Core3_Tx_Core1_Rx_Core3 = offSetRx;
        }
    }
    else if(((MW_CPUINFO_CORESELECTION == 2) && (MW_R5FSS1CORE0_ENABLE_SELFCORE_SEL == 1)))
    {

        if((MW_R5FSS0CORE0_ENABLE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 1))
        {
            offSetRx += VringSize;
            Core0_Tx_Core2_Rx_Core0 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }
        else if((MW_R5FSS0CORE0_ENABLE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 0))
        {
            Core0_Tx_Core2_Rx_Core0 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }

        if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
        {
            Core1_Tx_Core2_Rx_Core1 = offSetRx;
            offSetRx += (IPCCores * VringSize);
            offSetRx += VringSize;
            offSetRx += (IPCCores * VringSize);
        }
        else
        {
            offSetRx += VringSize;
            offSetRx += (IPCCores * VringSize);
        }

        if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
        {
            Core3_Tx_Core2_Rx_Core3 = offSetRx;
        }
    }
    else if(((MW_CPUINFO_CORESELECTION == 3) && (MW_R5FSS1CORE1_ENABLE_SELFCORE_SEL == 1)))
    {

        if((MW_R5FSS0CORE0_ENABLE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 1) && (MW_R5FSS1CORE0_ENABLE_SEL == 1))
        {
            offSetRx += (2 * VringSize);
            Core0_Tx_Core3_Rx_Core0 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }
        else if(((MW_R5FSS0CORE0_ENABLE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 1) && (MW_R5FSS1CORE0_ENABLE_SEL == 0)) || ((MW_R5FSS0CORE0_ENABLE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 0) && (MW_R5FSS1CORE0_ENABLE_SEL == 1)))
        {
            offSetRx += VringSize;
            Core0_Tx_Core3_Rx_Core0 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }
        else if((MW_R5FSS0CORE0_ENABLE_SEL == 1) && (MW_R5FSS0CORE1_ENABLE_SEL == 0) && (MW_R5FSS1CORE0_ENABLE_SEL == 0))
        {
            Core0_Tx_Core3_Rx_Core0 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }
        else if((MW_R5FSS0CORE0_ENABLE_SEL == 0) && (MW_R5FSS0CORE1_ENABLE_SEL == 1))
        {
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
                offSetRx += VringSize;   
        }

        if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
        {
            Core1_Tx_Core2_Rx_Core1 = offSetRx;
            offSetRx += (IPCCores * VringSize);
        }

        if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
        {
            Core3_Tx_Core2_Rx_Core3 = offSetRx;
        }
    }
}



void System_init(void)
{
    /* DPL init sets up address transalation unit, on some CPUs this is needed
     * to access SCICLIENT services, hence this needs to happen first
     */
    Dpl_init();
    /* initialize PMU */
    CycleCounterP_init(SOC_getSelfCpuClk());
    PowerClock_init();
    Pinmux_init();
    String str2 = STR(MW_TARGETHARDWARE);
    uint16_t offSetTx = 0U;
    uint8_t numRemoteCores = 0;
    if((MW_ENABLE_MCAN_SEL == 1) && (strcmp(str2, "AM263X ControlCard") == 0))
    {
        I2C_init();
    }

    {
        IpcNotify_Params notifyParams_core0, notifyParams_core1, notifyParams_core2, notifyParams_core3;
        int32_t status, status_rpmsg;


        if((MW_CPUINFO_CORESELECTION == 0) && (MW_R5FSS0CORE0_ENABLE_SELFCORE_SEL == 1))
        {

            /* initialize parameters to default */
            IpcNotify_Params_init(&notifyParams_core0);

            /* specify the priority of IPC Notify interrupt */
            notifyParams_core0.intrPriority = 15U;

            /* specify the core on which this API is called */

            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
                numRemoteCores++;
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)   
                numRemoteCores++;
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                numRemoteCores++;
            notifyParams_core0.selfCoreId = CSL_CORE_ID_R5FSS0_0;

            IPCCores = numRemoteCores;

            /* list the cores that will do IPC Notify with this core
            * Make sure to NOT list 'self' core in the list below
            */
            notifyParams_core0.numCores = numRemoteCores;
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
                notifyParams_core0.coreIdList[0] = CSL_CORE_ID_R5FSS0_1;
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1) 
                notifyParams_core0.coreIdList[1] = CSL_CORE_ID_R5FSS1_0;
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                notifyParams_core0.coreIdList[2] = CSL_CORE_ID_R5FSS1_1;

            notifyParams_core0.isMailboxIpcEnabled = 0;

            notifyParams_core0.isCrcEnabled = 0;

            notifyParams_core0.isIPCIntrRouter = 0;

            /* initialize the IPC Notify module */
            status = IpcNotify_init(&notifyParams_core0);
            DebugP_assert(status==SystemP_SUCCESS);

            RPMessage_Params rpmsgParamsCore0;
            // int32_t status_rpmsg;

            RPMessage_Params_init(&rpmsgParamsCore0);

            RPMemory_init();

            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore0.vringTxBaseAddr[CSL_CORE_ID_R5FSS0_1] = (uintptr_t)(&gIpcSharedMem[Core0_Tx_Core1_Rx_Core0]);
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore0.vringTxBaseAddr[CSL_CORE_ID_R5FSS1_0] = (uintptr_t)(&gIpcSharedMem[Core0_Tx_Core2_Rx_Core0]);
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore0.vringTxBaseAddr[CSL_CORE_ID_R5FSS1_1] = (uintptr_t)(&gIpcSharedMem[Core0_Tx_Core3_Rx_Core0]);

            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore0.vringRxBaseAddr[CSL_CORE_ID_R5FSS0_1] = (uintptr_t)(&gIpcSharedMem[Core1_Tx_Core0_Rx_Core1]);
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore0.vringRxBaseAddr[CSL_CORE_ID_R5FSS1_0] = (uintptr_t)(&gIpcSharedMem[Core2_Tx_Core0_Rx_Core2]);
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore0.vringRxBaseAddr[CSL_CORE_ID_R5FSS1_1] = (uintptr_t)(&gIpcSharedMem[Core3_Tx_Core0_Rx_Core3]);


            rpmsgParamsCore0.vringSize = IPC_RPMESSAGE_VRING_SIZE;
            rpmsgParamsCore0.vringNumBuf = MW_RP_NUM_BUFFERS_SEL;
            rpmsgParamsCore0.vringMsgSize = MW_RP_BUFFER_SIZE_SEL;
            rpmsgParamsCore0.isCrcEnabled = 0;

            /* initialize the IPC RP Message module */
            status_rpmsg = RPMessage_init(&rpmsgParamsCore0);
            DebugP_assert(status_rpmsg==SystemP_SUCCESS);
        }
        else if((MW_CPUINFO_CORESELECTION == 1) && (MW_R5FSS0CORE1_ENABLE_SELFCORE_SEL == 1))
        {
            IpcNotify_Params_init(&notifyParams_core1);

            /* specify the priority of IPC Notify interrupt */
            notifyParams_core1.intrPriority = 15U;

            /* specify the core on which this API is called */

            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                numRemoteCores++;
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)   
                numRemoteCores++;
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                numRemoteCores++;
            notifyParams_core1.selfCoreId = CSL_CORE_ID_R5FSS0_1;

            IPCCores = numRemoteCores;

            /* list the cores that will do IPC Notify with this core
            * Make sure to NOT list 'self' core in the list below
            */
            notifyParams_core1.numCores = numRemoteCores;
            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                notifyParams_core1.coreIdList[0] = CSL_CORE_ID_R5FSS0_0;
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1) 
                notifyParams_core1.coreIdList[1] = CSL_CORE_ID_R5FSS1_0;
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                notifyParams_core1.coreIdList[2] = CSL_CORE_ID_R5FSS1_1;

            notifyParams_core1.isMailboxIpcEnabled = 0;

            notifyParams_core1.isCrcEnabled = 0;

            notifyParams_core1.isIPCIntrRouter = 0;

            /* initialize the IPC Notify module */
            status = IpcNotify_init(&notifyParams_core1);
            DebugP_assert(status==SystemP_SUCCESS);    

            RPMessage_Params rpmsgParamsCore1;
            // int32_t status;

            /* initialize parameters to default */
            RPMessage_Params_init(&rpmsgParamsCore1);

            RPMemory_init();

            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore1.vringTxBaseAddr[CSL_CORE_ID_R5FSS0_0] = (uintptr_t)(&gIpcSharedMem[Core1_Tx_Core0_Rx_Core1]);
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore1.vringTxBaseAddr[CSL_CORE_ID_R5FSS1_0] = (uintptr_t)(&gIpcSharedMem[Core1_Tx_Core2_Rx_Core1]);
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore1.vringTxBaseAddr[CSL_CORE_ID_R5FSS1_1] = (uintptr_t)(&gIpcSharedMem[Core1_Tx_Core3_Rx_Core1]);
            
            /* RX VRINGs */
            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore1.vringRxBaseAddr[CSL_CORE_ID_R5FSS0_0] = (uintptr_t)(&gIpcSharedMem[Core0_Tx_Core1_Rx_Core0]);
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore1.vringRxBaseAddr[CSL_CORE_ID_R5FSS1_0] = (uintptr_t)(&gIpcSharedMem[Core2_Tx_Core1_Rx_Core2]);
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore1.vringRxBaseAddr[CSL_CORE_ID_R5FSS1_1] = (uintptr_t)(&gIpcSharedMem[Core3_Tx_Core1_Rx_Core3]);

            rpmsgParamsCore1.vringSize = IPC_RPMESSAGE_VRING_SIZE;
            rpmsgParamsCore1.vringNumBuf = MW_RP_NUM_BUFFERS_SEL;
            rpmsgParamsCore1.vringMsgSize = MW_RP_BUFFER_SIZE_SEL;
            rpmsgParamsCore1.isCrcEnabled = 0;

            /* initialize the IPC RP Message module */
            status_rpmsg = RPMessage_init(&rpmsgParamsCore1);
            DebugP_assert(status_rpmsg==SystemP_SUCCESS);
        }
        else if((MW_CPUINFO_CORESELECTION == 2) && (MW_R5FSS1CORE0_ENABLE_SELFCORE_SEL == 1))
        {
            IpcNotify_Params_init(&notifyParams_core2);

            /* specify the priority of IPC Notify interrupt */
            notifyParams_core2.intrPriority = 15U;

            /* specify the core on which this API is called */

            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                numRemoteCores++;
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)   
                numRemoteCores++;
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                numRemoteCores++;
            notifyParams_core2.selfCoreId = CSL_CORE_ID_R5FSS1_0;
            IPCCores = numRemoteCores;

            /* list the cores that will do IPC Notify with this core
            * Make sure to NOT list 'self' core in the list below
            */
            notifyParams_core2.numCores = numRemoteCores;
            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                notifyParams_core2.coreIdList[0] = CSL_CORE_ID_R5FSS0_0;
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1) 
                notifyParams_core2.coreIdList[1] = CSL_CORE_ID_R5FSS0_1;
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                notifyParams_core2.coreIdList[2] = CSL_CORE_ID_R5FSS1_1;

            notifyParams_core2.isMailboxIpcEnabled = 0;

            notifyParams_core2.isCrcEnabled = 0;

            notifyParams_core2.isIPCIntrRouter = 0;

            /* initialize the IPC Notify module */
            status = IpcNotify_init(&notifyParams_core2);
            DebugP_assert(status==SystemP_SUCCESS);   

            RPMessage_Params rpmsgParamsCore2;
            // int32_t status_rpmsg;

            RPMessage_Params_init(&rpmsgParamsCore2);

            RPMemory_init();

            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore2.vringTxBaseAddr[CSL_CORE_ID_R5FSS0_0] = (uintptr_t)(&gIpcSharedMem[Core2_Tx_Core0_Rx_Core2]);
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore2.vringTxBaseAddr[CSL_CORE_ID_R5FSS0_1] = (uintptr_t)(&gIpcSharedMem[Core2_Tx_Core1_Rx_Core2]);
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore2.vringTxBaseAddr[CSL_CORE_ID_R5FSS1_1] = (uintptr_t)(&gIpcSharedMem[Core2_Tx_Core3_Rx_Core2]);

            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore2.vringRxBaseAddr[CSL_CORE_ID_R5FSS0_0] = (uintptr_t)(&gIpcSharedMem[Core0_Tx_Core2_Rx_Core0]);
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore2.vringRxBaseAddr[CSL_CORE_ID_R5FSS0_1] = (uintptr_t)(&gIpcSharedMem[Core1_Tx_Core2_Rx_Core1]);
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore2.vringRxBaseAddr[CSL_CORE_ID_R5FSS1_1] = (uintptr_t)(&gIpcSharedMem[Core3_Tx_Core2_Rx_Core3]);


            rpmsgParamsCore2.vringSize = IPC_RPMESSAGE_VRING_SIZE;
            rpmsgParamsCore2.vringNumBuf = MW_RP_NUM_BUFFERS_SEL;
            rpmsgParamsCore2.vringMsgSize = MW_RP_BUFFER_SIZE_SEL;
            rpmsgParamsCore2.isCrcEnabled = 0;

            /* initialize the IPC RP Message module */
            status_rpmsg = RPMessage_init(&rpmsgParamsCore2);
            DebugP_assert(status_rpmsg==SystemP_SUCCESS);
        }
        else if((MW_CPUINFO_CORESELECTION == 3) && (MW_R5FSS1CORE1_ENABLE_SELFCORE_SEL == 1))
        {
         
            IpcNotify_Params_init(&notifyParams_core3);

            /* specify the priority of IPC Notify interrupt */
            notifyParams_core3.intrPriority = 15U;

            /* specify the core on which this API is called */

            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                numRemoteCores++;
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)   
                numRemoteCores++;
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
                numRemoteCores++;
            notifyParams_core3.selfCoreId = CSL_CORE_ID_R5FSS1_1;

            IPCCores = numRemoteCores;

            /* list the cores that will do IPC Notify with this core
            * Make sure to NOT list 'self' core in the list below
            */
            notifyParams_core3.numCores = numRemoteCores;
            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                notifyParams_core3.coreIdList[0] = CSL_CORE_ID_R5FSS0_0;
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1) 
                notifyParams_core3.coreIdList[1] = CSL_CORE_ID_R5FSS0_1;
            if(MW_R5FSS1CORE1_ENABLE_SEL == 1)
                notifyParams_core3.coreIdList[2] = CSL_CORE_ID_R5FSS1_0;

            notifyParams_core3.isMailboxIpcEnabled = 0;

            notifyParams_core3.isCrcEnabled = 0;

            notifyParams_core3.isIPCIntrRouter = 0;

            /* initialize the IPC Notify module */
            status = IpcNotify_init(&notifyParams_core3);
            DebugP_assert(status==SystemP_SUCCESS);  

            RPMessage_Params rpmsgParamsCore3;
            // int32_t status_rpmsg;

            RPMessage_Params_init(&rpmsgParamsCore3);

            RPMemory_init();

            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore3.vringTxBaseAddr[CSL_CORE_ID_R5FSS0_0] = (uintptr_t)(&gIpcSharedMem[Core3_Tx_Core0_Rx_Core3]);
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore3.vringTxBaseAddr[CSL_CORE_ID_R5FSS0_1] = (uintptr_t)(&gIpcSharedMem[Core3_Tx_Core1_Rx_Core3]);
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore3.vringTxBaseAddr[CSL_CORE_ID_R5FSS1_0] = (uintptr_t)(&gIpcSharedMem[Core3_Tx_Core2_Rx_Core3]);

            if(MW_R5FSS0CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore3.vringRxBaseAddr[CSL_CORE_ID_R5FSS0_0] = (uintptr_t)(&gIpcSharedMem[Core0_Tx_Core3_Rx_Core0]);
            if(MW_R5FSS0CORE1_ENABLE_SEL == 1)
                rpmsgParamsCore3.vringRxBaseAddr[CSL_CORE_ID_R5FSS0_1] = (uintptr_t)(&gIpcSharedMem[Core1_Tx_Core3_Rx_Core1]);
            if(MW_R5FSS1CORE0_ENABLE_SEL == 1)
                rpmsgParamsCore3.vringRxBaseAddr[CSL_CORE_ID_R5FSS1_0] = (uintptr_t)(&gIpcSharedMem[Core2_Tx_Core3_Rx_Core2]);


            rpmsgParamsCore3.vringSize = IPC_RPMESSAGE_VRING_SIZE;
            rpmsgParamsCore3.vringNumBuf = MW_RP_NUM_BUFFERS_SEL;
            rpmsgParamsCore3.vringMsgSize = MW_RP_BUFFER_SIZE_SEL;
            rpmsgParamsCore3.isCrcEnabled = 0;

            /* initialize the IPC RP Message module */
            status_rpmsg = RPMessage_init(&rpmsgParamsCore3);
            DebugP_assert(status_rpmsg==SystemP_SUCCESS); 
        }

    }
    
   

}

void Drivers_open(void)
{

    String str2 = STR(MW_TARGETHARDWARE);
    if((MW_ENABLE_MCAN_SEL == 1) && (strcmp(str2, "AM263X ControlCard") == 0))
    {
        Drivers_i2cOpen();
    }
    if((MW_ENABLE_MCAN_SEL == 1))
    {
        Driver_mcanOpen();
        MCAN_initialization();
    }
    Drivers_inputXbarOpen();
    Drivers_intXbarOpen();
    Drivers_outputXbarOpen();
    Drivers_epwmSyncoutXbarOpen();
    Drivers_gpioIntXbarOpen();
    Drivers_EPWMXbarOpen();
    Drivers_uartOpen();
    if((MW_ENABLE_MCSPI0_SEL == 1) | (MW_ENABLE_MCSPI1_SEL == 1) | (MW_ENABLE_MCSPI2_SEL == 1) | (MW_ENABLE_MCSPI3_SEL == 1) | (MW_ENABLE_MCSPI4_SEL == 1))
    {
        MCSPI_channel_init();
        MCSPI_object_init();
        Drivers_mcspiOpen();
    }

    
}


void am263x_HardwareInit_Fcn(void)
{

    System_init();
    //Board_init(); //UNUSED
    Drivers_open();
    //Board_driversOpen(); //UNUSED

	return;
}

