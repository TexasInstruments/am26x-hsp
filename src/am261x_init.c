#include <stdint.h>
#include <stdio.h>

#include "am261x_main.h"

#include "MW_target_hardware_resources.h"
#include <drivers/uart/v0/lld/dma/edma/uart_dma_edma.h>
#include <drivers/uart/v0/lld/uart_lld.h>
#include <drivers/soc.h>
#include <drivers/edma.h>
#include <drivers/pinmux.h>
#include <drivers/mcan.h>
#include "kernel/dpl/DebugP.h"
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <board/ioexp/ioexp_tca6424.h>
#include <board/ioexp/ioexp_tca6416.h>
#include <board/eeprom.h>
#include <board/eeprom/eeprom_cat24m.h>
#include <drivers/i2c.h>
#include <drivers/mcspi.h>
#include <drivers/edma.h>
#include <drivers/mcspi/v0/lld/dma/mcspi_dma.h>
#include <drivers/mcspi/v0/lld/dma/edma/mcspi_dma_edma.h>
#include <drivers/edma.h>
#include <drivers/uart.h>
#include <board/ioexp/ioexp_tca6408.h>
#include<math.h>



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

/* ----------- ClockP ----------- */
#define RTI0_CLOCK_SRC_MUX_ADDR (0x53208140u)
#define RTI0_CLOCK_SRC_WUCPUCLK (0x0u)
#define RTI0_BASE_ADDR     (0x52180000u)

uint32_t gR5ClockFrequency = SOC_RCM_R5_FREQ_500MHZ;

ClockP_Config gClockConfig = {
    .timerBaseAddr = RTI0_BASE_ADDR, 
    .timerHwiIntNum = 124,
    .timerInputClkHz = 25000000,
    .timerInputPreScaler = 1,
    .usecPerTick = 1000,
    .intrPriority = 15,
};


#define BOOT_SECTION __attribute__((section(".text.boot"), do_not_share))

/* This function is called by _c_int00 */
void BOOT_SECTION __mpu_init() 
{
    MpuP_init();
    
    CacheP_init();
}



/*
 * UART
 */


// /* UART objects - initialized by the driver */
#define CONFIG_UART_NUM_LLD_INSTANCES 6
UARTLLD_Object gUartObject[CONFIG_UART_NUM_LLD_INSTANCES];
UARTLLD_Handle gUartHandleLld[CONFIG_UART_NUM_LLD_INSTANCES];
// UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_LLD_INSTANCES];
UART_Config gUartConfig[CONFIG_UART_NUM_INSTANCES];
uint32_t gUartConfigNum = 0;


void Drivers_uartClose(void)
{
  
}



static Pinmux_PerCfg_t gPinMuxMainDomainCfg[] = {

    {PIN_GPIO0, ( PIN_MODE(MW_PIN_OSPI0_CSN0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO1, ( PIN_MODE(MW_PIN_OSPI0_CSN1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO2, ( PIN_MODE(MW_PIN_OSPI0_CLK_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO3, ( PIN_MODE(MW_PIN_OSPI0_D0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO4, ( PIN_MODE(MW_PIN_OSPI0_D1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO5, ( PIN_MODE(MW_PIN_OSPI0_D2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO6, ( PIN_MODE(MW_PIN_OSPI0_D3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO7, ( PIN_MODE(MW_PIN_MCAN0_RX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO8, ( PIN_MODE(MW_PIN_MCAN0_TX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO9, ( PIN_MODE(MW_PIN_MCAN1_RX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO10, ( PIN_MODE(MW_PIN_MCAN1_TX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO11, ( PIN_MODE(MW_PIN_SPI0_CS0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO12, ( PIN_MODE(MW_PIN_SPI0_CLK_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO13, ( PIN_MODE(MW_PIN_SPI0_D0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO14, ( PIN_MODE(MW_PIN_SPI0_D1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO15, ( PIN_MODE(MW_PIN_SPI1_CS0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO16, ( PIN_MODE(MW_PIN_SPI1_CLK_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO17, ( PIN_MODE(MW_PIN_SPI1_D0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO18, ( PIN_MODE(MW_PIN_SPI1_D1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO19, ( PIN_MODE(MW_PIN_LIN1_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO20, ( PIN_MODE(MW_PIN_LIN1_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO21, ( PIN_MODE(MW_PIN_LIN2_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO22, ( PIN_MODE(MW_PIN_LIN2_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO23, ( PIN_MODE(MW_PIN_I2C1_SCL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO24, ( PIN_MODE(MW_PIN_I2C1_SDA_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO25, ( PIN_MODE(MW_PIN_UART0_RTSN_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO26, ( PIN_MODE(MW_PIN_UART0_CTSN_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO27, ( PIN_MODE(MW_PIN_UART0_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO28, ( PIN_MODE(MW_PIN_UART0_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO29, ( PIN_MODE(MW_PIN_RGMII1_RXC_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO30, ( PIN_MODE(MW_PIN_RGMII1_RX_CTL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO31, ( PIN_MODE(MW_PIN_RGMII1_RD0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO32, ( PIN_MODE(MW_PIN_RGMII1_RD1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO33, ( PIN_MODE(MW_PIN_RGMII1_RD2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO34, ( PIN_MODE(MW_PIN_RGMII1_RD3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO35, ( PIN_MODE(MW_PIN_RGMII1_TXC_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO36, ( PIN_MODE(MW_PIN_RGMII1_TX_CTL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO37, ( PIN_MODE(MW_PIN_RGMII1_TD0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO38, ( PIN_MODE(MW_PIN_RGMII1_TD1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO39, ( PIN_MODE(MW_PIN_RGMII1_TD2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO40, ( PIN_MODE(MW_PIN_RGMII1_TD3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO41, ( PIN_MODE(MW_PIN_MDIO_MDIO_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO42, ( PIN_MODE(MW_PIN_MDIO_MDC_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO43, ( PIN_MODE(MW_PIN_EPWM0_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO44, ( PIN_MODE(MW_PIN_EPWM0_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO45, ( PIN_MODE(MW_PIN_EPWM1_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO46, ( PIN_MODE(MW_PIN_EPWM1_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO47, ( PIN_MODE(MW_PIN_EPWM2_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO48, ( PIN_MODE(MW_PIN_EPWM2_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO49, ( PIN_MODE(MW_PIN_EPWM3_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO50, ( PIN_MODE(MW_PIN_EPWM3_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO51, ( PIN_MODE(MW_PIN_EPWM4_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO52, ( PIN_MODE(MW_PIN_EPWM4_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO53, ( PIN_MODE(MW_PIN_EPWM5_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO54, ( PIN_MODE(MW_PIN_EPWM5_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO55, ( PIN_MODE(MW_PIN_EPWM6_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO56, ( PIN_MODE(MW_PIN_EPWM6_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO57, ( PIN_MODE(MW_PIN_EPWM7_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO58, ( PIN_MODE(MW_PIN_EPWM7_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO59, ( PIN_MODE(MW_PIN_EPWM8_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO60, ( PIN_MODE(MW_PIN_EPWM8_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO61, ( PIN_MODE(MW_PIN_EPWM9_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO62, ( PIN_MODE(MW_PIN_EPWM9_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO63, ( PIN_MODE(MW_PIN_LIN0_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO64, ( PIN_MODE(MW_PIN_LIN0_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO65, ( PIN_MODE(MW_PIN_OSPI0_ECC_FAIL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO66, ( PIN_MODE(MW_PIN_OSPI0_RESET_OUT0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO67, ( PIN_MODE(MW_PIN_PR1_PRU0_GPIO0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO68, ( PIN_MODE(MW_PIN_PR1_PRU0_GPIO1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO69, ( PIN_MODE(MW_PIN_PR1_PRU0_GPIO2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO70, ( PIN_MODE(MW_PIN_PR1_PRU0_GPIO9_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO71, ( PIN_MODE(MW_PIN_PR1_PRU1_GPIO0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO72, ( PIN_MODE(MW_PIN_PR1_PRU1_GPIO1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO73, ( PIN_MODE(MW_PIN_PR1_PRU1_GPIO2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO74, ( PIN_MODE(MW_PIN_PR1_PRU1_GPIO9_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO75, ( PIN_MODE(MW_PIN_UART1_RXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO76, ( PIN_MODE(MW_PIN_UART1_TXD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO77, ( PIN_MODE(MW_PIN_MMC_CLK_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO78, ( PIN_MODE(MW_PIN_MMC_CMD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO79, ( PIN_MODE(MW_PIN_MMC_DAT0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO80, ( PIN_MODE(MW_PIN_MMC_DAT1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO81, ( PIN_MODE(MW_PIN_MMC_DAT2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO82, ( PIN_MODE(MW_PIN_MMC_DAT3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO83, ( PIN_MODE(MW_PIN_MMC_SDWP_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO84, ( PIN_MODE(MW_PIN_MMC_SDCD_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO85, ( PIN_MODE(MW_PIN_PR0_MDIO_MDIO_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO86, ( PIN_MODE(MW_PIN_PR0_MDIO_MDC_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO87, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO5_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO88, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO9_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO89, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO10_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO90, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO8_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO91, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO6_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO92, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO4_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO93, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO94, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO95, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO96, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO97, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO16_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO98, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO15_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO99, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO11_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO100, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO12_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO101, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO13_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO102, ( PIN_MODE(MW_PIN_PR0_PRU0_GPIO14_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO103, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO5_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO104, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO9_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO105, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO10_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO106, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO8_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO107, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO6_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO108, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO4_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO109, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO110, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO111, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO112, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO113, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO16_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO114, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO15_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO115, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO11_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO116, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO12_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO117, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO13_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO118, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO14_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO119, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO19_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO120, ( PIN_MODE(MW_PIN_PR0_PRU1_GPIO18_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO121, ( PIN_MODE(MW_PIN_EXT_REFCLK0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO122, ( PIN_MODE(MW_PIN_SDFM0_CLK0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO123, ( PIN_MODE(MW_PIN_SDFM0_D0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO124, ( PIN_MODE(MW_PIN_SDFM0_CLK1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO125, ( PIN_MODE(MW_PIN_SDFM0_D1_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO126, ( PIN_MODE(MW_PIN_SDFM0_CLK2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO127, ( PIN_MODE(MW_PIN_SDFM0_D2_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO128, ( PIN_MODE(MW_PIN_SDFM0_CLK3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO129, ( PIN_MODE(MW_PIN_SDFM0_D3_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO130, ( PIN_MODE(MW_PIN_EQEP0_A_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO131, ( PIN_MODE(MW_PIN_EQEP0_B_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO132, ( PIN_MODE(MW_PIN_EQEP0_STROBE_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO133, ( PIN_MODE(MW_PIN_EQEP0_INDEX_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO134, ( PIN_MODE(MW_PIN_I2C0_SDA_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO135, ( PIN_MODE(MW_PIN_I2C0_SCL_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO136, ( PIN_MODE(MW_PIN_UART1_RTSN_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO137, ( PIN_MODE(MW_PIN_UART2_RTSN_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    {PIN_GPIO138, ( PIN_MODE(MW_PIN_CLKOUT0_IOMUX) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW ) },
    
    {PINMUX_END, PINMUX_END}
};



#define SOC_MODULES_END     (0xFFFFFFFFu)
#define SOCModulesMaxConfig 14U

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

    
        gSocModules[indx] = SOC_RcmPeripheralId_LIN0_UART0; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN0_UART0;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
           
    

    if(MW_UART1_ENABLE_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_LIN1_UART1; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN1_UART1;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    }

    if(MW_UART2_ENABLE_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_LIN2_UART2; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN2_UART2;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    }

    if(MW_UART3_ENABLE_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_LIN3_UART3; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_LIN3_UART3;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 48000000;
        indx++;
    }


    if(MW_ENABLE_MCAN0_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCAN0; 
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCAN0;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT0;
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

    if(MW_ENABLE_MCSPI0_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCSPI0;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCSPI0;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_SYS_CLK;
        gSocModulesClockFrequency[indx].clkRate = 50000000; 
        indx++;
    }

    if(MW_ENABLE_MCSPI1_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCSPI1;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCSPI1;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_SYS_CLK;
        gSocModulesClockFrequency[indx].clkRate = 50000000; 
        indx++;
    }

    if(MW_ENABLE_MCSPI2_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCSPI2;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCSPI2;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_SYS_CLK;
        gSocModulesClockFrequency[indx].clkRate = 50000000; 
        indx++;
    }

    if(MW_ENABLE_MCSPI3_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_MCSPI3;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_MCSPI3;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_SYS_CLK;
        gSocModulesClockFrequency[indx].clkRate = 50000000; 
        indx++;
    }

    if(MW_SDFM_ENABLE_SDFM0_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_CONTROLSS_PLL;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_CONTROLSS_PLL;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2;
        gSocModulesClockFrequency[indx].clkRate = 500000000; 
        indx++;
    }

    if(MW_SDFM_ENABLE_SDFM1_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_CONTROLSS_PLL;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_CONTROLSS_PLL;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_CORE_HSDIV0_CLKOUT2;
        gSocModulesClockFrequency[indx].clkRate = 500000000; 
        indx++;
    }
    if(MW_ENABLE_I2C_SEL == 1)
    {
        gSocModules[indx] = SOC_RcmPeripheralId_I2C;
        gSocModulesClockFrequency[indx].moduleId = SOC_RcmPeripheralId_I2C;
        gSocModulesClockFrequency[indx].clkId = SOC_RcmPeripheralClockSource_DPLL_PER_HSDIV0_CLKOUT0;
        gSocModulesClockFrequency[indx].clkRate = 48000000; 
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
uint32_t intrNumUART0, intrNumUART1, intrNumUART2, intrNumUART3, intrNumUART4, intrNumUART5, intrNum;
uint32_t intrNum, intcBaseAddr;
// extern UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_LLD_INSTANCES];
UARTLLD_Handle gUartHandle0, gUartHandle1, gUartHandle2, gUartHandle3, gUartHandle4, gUartHandle5;
// UARTLLD_Handle gUartHandleLld[CONFIG_UART_NUM_LLD_INSTANCES];

UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_LLD_INSTANCES];


#define CONFIG_UART_CONSOLE (0U)


void Drivers_uartOpen(void)
{
        
    int32_t status = UART_STATUS_SUCCESS;
    uint32_t baudRate, intrNum, baseAddr, BaudRateSel;
    uint32_t i;
    
    for(i = 0; i<CONFIG_UART_NUM_LLD_INSTANCES; i++)
    {
        status = UART_STATUS_SUCCESS;
        intrNumUART0 = 73U;
        gUartInitObject[i].inputClkFreq       = 48000000U;
        gUartInitObject[i].hwFlowControl      = FALSE;
        gUartInitObject[i].hwFlowControlThr   = UART_RXTRIGLVL_16;
        gUartInitObject[i].clockP_get         = ClockP_getTicks;
        gUartInitObject[i].clockP_usecToTick  = ClockP_usecToTicks;
        

        if(i == 0)
        {
            intrNumUART0 = 73;
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
            intrNumUART1 = 74;
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
            intrNumUART2 = 75;
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
            intrNumUART3 = 76;
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
            intrNumUART4 = 77;
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
            intrNumUART5 = 78;
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

            gUartInitObject[i].baudRate   = baudRate;
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

            int32_t status = UART_STATUS_SUCCESS;

            gUartInitObject[i].baudRate   = baudRate;
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

            if(i == 0 && (MW_UART0_TRANSFER_MODE_SEL == 1))
            {
               
                if(MW_UART0_CTSINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[0]->baseAddr, UART_INTR_CTS);  
                if(MW_UART0_RTSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[0]->baseAddr, UART_INTR_RTS); 
                if(MW_UART0_XOFFINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[0]->baseAddr, UART_INTR_XOFF); 
                if(MW_UART0_SLEEPMODEINT_SEL == 1)
                   UART_intrEnable(gUartHandleLld[0]->baseAddr, UART_INTR_SLEEPMODE); 
                if(MW_UART0_INTLINESTATUS_SEL == 1)
                    UART_intrEnable(gUartHandleLld[0]->baseAddr, UART_INTR_LINE_STAT);
                if(MW_UART0_MODEMSTATUSINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[0]->baseAddr, UART_INTR_MODEM_STAT);
                if(MW_UART0_RHRINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[0]->baseAddr, UART_INTR_RHR_CTI);
                if(MW_UART0_THREINT_SEL == 1)
                    UART_intrEnable(gUartHandleLld[0]->baseAddr, UART_INTR_THR);
                   
            }
            else if(i == 1 && (MW_UART1_TRANSFER_MODE_SEL == 1))
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
            else if(i == 2 && (MW_UART2_TRANSFER_MODE_SEL == 1))
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
            else if(i == 3 && (MW_UART3_TRANSFER_MODE_SEL == 1))
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
            else if(i == 4 && (MW_UART4_TRANSFER_MODE_SEL == 1))
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
            else if(i == 5 && (MW_UART5_TRANSFER_MODE_SEL == 1))
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
    Pinmux_config(gPinMuxMainDomainCfg, PINMUX_DOMAIN_ID_MAIN);
    
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
    DebugP_logZoneEnable(DebugP_LOG_ZONE_ERROR);
    DebugP_logZoneEnable(DebugP_LOG_ZONE_WARN);
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
    
    uint32_t group00_mask[32];
    uint32_t group01_mask[32];
    uint32_t group02_mask[32];
    uint32_t group03_mask[32];
    uint32_t group04_mask[32];
    uint32_t group05_mask[32];
    uint32_t group06_mask[32];
    uint32_t group07_mask[32];


    group00_mask[0] = (MW_INTXBAR0_EPWM0_INT_SEL<<0) | (MW_INTXBAR0_EPWM1_INT_SEL<<1) | (MW_INTXBAR0_EPWM2_INT_SEL<<2) | (MW_INTXBAR0_EPWM3_INT_SEL<<3) | (MW_INTXBAR0_EPWM4_INT_SEL<<4) | (MW_INTXBAR0_EPWM5_INT_SEL<<5) | (MW_INTXBAR0_EPWM6_INT_SEL<<6) | (MW_INTXBAR0_EPWM7_INT_SEL<<7) | (MW_INTXBAR0_EPWM8_INT_SEL<<8) | (MW_INTXBAR0_EPWM9_INT_SEL<<9);
    group01_mask[0] = (MW_INTXBAR0_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR0_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR0_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR0_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR0_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR0_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR0_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR0_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR0_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR0_EPWM9_TZINT_SEL<<9);
    group02_mask[0] = (MW_INTXBAR0_ADC0_INT1_SEL<<0) | (MW_INTXBAR0_ADC0_INT2_SEL<<1) | (MW_INTXBAR0_ADC0_INT3_SEL<<2) | (MW_INTXBAR0_ADC0_INT4_SEL<<3) | (MW_INTXBAR0_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR0_ADC1_INT1_SEL<<5) | (MW_INTXBAR0_ADC1_INT2_SEL<<6) | (MW_INTXBAR0_ADC1_INT3_SEL<<7) | (MW_INTXBAR0_ADC1_INT4_SEL<<8) | (MW_INTXBAR0_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR0_ADC2_INT1_SEL<<10) | (MW_INTXBAR0_ADC2_INT2_SEL<<11) | (MW_INTXBAR0_ADC2_INT3_SEL<<12) | (MW_INTXBAR0_ADC2_INT4_SEL<<13) | (MW_INTXBAR0_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR0_EVTAGGR0_SEL<<15);
    group03_mask[0] = (MW_INTXBAR0_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR0_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR0_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR0_FSITX0_INT2N_SEL<<3);
    group04_mask[0] = (MW_INTXBAR0_SD0_ERR_SEL<<0) | (MW_INTXBAR0_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR0_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR0_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR0_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR0_SD1_ERR_SEL<<5) | (MW_INTXBAR0_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR0_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR0_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR0_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[0] = (MW_INTXBAR0_ECAP0_INT_SEL<<0) | (MW_INTXBAR0_ECAP1_INT_SEL<<1) | (MW_INTXBAR0_ECAP2_INT_SEL<<2) | (MW_INTXBAR0_ECAP3_INT_SEL<<3) | (MW_INTXBAR0_ECAP4_INT_SEL<<4) | (MW_INTXBAR0_ECAP5_INT_SEL<<5) | (MW_INTXBAR0_ECAP6_INT_SEL<<6) | (MW_INTXBAR0_ECAP7_INT_SEL<<7);
    group06_mask[0] = (MW_INTXBAR0_EQEP0_INT_SEL<<0) | (MW_INTXBAR0_EQEP1_INT_SEL<<1);
    group07_mask[0] = (MW_INTXBAR0_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR0_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR0_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR0_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR0_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR0_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR0_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR0_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR0_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR0_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR0_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR0_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR0_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR0_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR0_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR0_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR0_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR0_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[1] = (MW_INTXBAR1_EPWM0_INT_SEL<<0) | (MW_INTXBAR1_EPWM1_INT_SEL<<1) | (MW_INTXBAR1_EPWM2_INT_SEL<<2) | (MW_INTXBAR1_EPWM3_INT_SEL<<3) | (MW_INTXBAR1_EPWM4_INT_SEL<<4) | (MW_INTXBAR1_EPWM5_INT_SEL<<5) | (MW_INTXBAR1_EPWM6_INT_SEL<<6) | (MW_INTXBAR1_EPWM7_INT_SEL<<7) | (MW_INTXBAR1_EPWM8_INT_SEL<<8) | (MW_INTXBAR1_EPWM9_INT_SEL<<9);
    group01_mask[1] = (MW_INTXBAR1_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR1_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR1_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR1_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR1_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR1_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR1_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR1_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR1_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR1_EPWM9_TZINT_SEL<<9);
    group02_mask[1] = (MW_INTXBAR1_ADC0_INT1_SEL<<0) | (MW_INTXBAR1_ADC0_INT2_SEL<<1) | (MW_INTXBAR1_ADC0_INT3_SEL<<2) | (MW_INTXBAR1_ADC0_INT4_SEL<<3) | (MW_INTXBAR1_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR1_ADC1_INT1_SEL<<5) | (MW_INTXBAR1_ADC1_INT2_SEL<<6) | (MW_INTXBAR1_ADC1_INT3_SEL<<7) | (MW_INTXBAR1_ADC1_INT4_SEL<<8) | (MW_INTXBAR1_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR1_ADC2_INT1_SEL<<10) | (MW_INTXBAR1_ADC2_INT2_SEL<<11) | (MW_INTXBAR1_ADC2_INT3_SEL<<12) | (MW_INTXBAR1_ADC2_INT4_SEL<<13) | (MW_INTXBAR1_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR1_EVTAGGR0_SEL<<15);
    group03_mask[1] = (MW_INTXBAR1_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR1_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR1_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR1_FSITX0_INT2N_SEL<<3);
    group04_mask[1] = (MW_INTXBAR1_SD0_ERR_SEL<<0) | (MW_INTXBAR1_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR1_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR1_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR1_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR1_SD1_ERR_SEL<<5) | (MW_INTXBAR1_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR1_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR1_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR1_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[1] = (MW_INTXBAR1_ECAP0_INT_SEL<<0) | (MW_INTXBAR1_ECAP1_INT_SEL<<1) | (MW_INTXBAR1_ECAP2_INT_SEL<<2) | (MW_INTXBAR1_ECAP3_INT_SEL<<3) | (MW_INTXBAR1_ECAP4_INT_SEL<<4) | (MW_INTXBAR1_ECAP5_INT_SEL<<5) | (MW_INTXBAR1_ECAP6_INT_SEL<<6) | (MW_INTXBAR1_ECAP7_INT_SEL<<7);
    group06_mask[1] = (MW_INTXBAR1_EQEP0_INT_SEL<<0) | (MW_INTXBAR1_EQEP1_INT_SEL<<1);
    group07_mask[1] = (MW_INTXBAR1_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR1_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR1_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR1_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR1_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR1_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR1_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR1_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR1_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR1_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR1_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR1_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR1_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR1_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR1_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR1_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR1_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR1_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[2] = (MW_INTXBAR2_EPWM0_INT_SEL<<0) | (MW_INTXBAR2_EPWM1_INT_SEL<<1) | (MW_INTXBAR2_EPWM2_INT_SEL<<2) | (MW_INTXBAR2_EPWM3_INT_SEL<<3) | (MW_INTXBAR2_EPWM4_INT_SEL<<4) | (MW_INTXBAR2_EPWM5_INT_SEL<<5) | (MW_INTXBAR2_EPWM6_INT_SEL<<6) | (MW_INTXBAR2_EPWM7_INT_SEL<<7) | (MW_INTXBAR2_EPWM8_INT_SEL<<8) | (MW_INTXBAR2_EPWM9_INT_SEL<<9);
    group01_mask[2] = (MW_INTXBAR2_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR2_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR2_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR2_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR2_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR2_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR2_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR2_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR2_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR2_EPWM9_TZINT_SEL<<9);
    group02_mask[2] = (MW_INTXBAR2_ADC0_INT1_SEL<<0) | (MW_INTXBAR2_ADC0_INT2_SEL<<1) | (MW_INTXBAR2_ADC0_INT3_SEL<<2) | (MW_INTXBAR2_ADC0_INT4_SEL<<3) | (MW_INTXBAR2_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR2_ADC1_INT1_SEL<<5) | (MW_INTXBAR2_ADC1_INT2_SEL<<6) | (MW_INTXBAR2_ADC1_INT3_SEL<<7) | (MW_INTXBAR2_ADC1_INT4_SEL<<8) | (MW_INTXBAR2_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR2_ADC2_INT1_SEL<<10) | (MW_INTXBAR2_ADC2_INT2_SEL<<11) | (MW_INTXBAR2_ADC2_INT3_SEL<<12) | (MW_INTXBAR2_ADC2_INT4_SEL<<13) | (MW_INTXBAR2_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR2_EVTAGGR0_SEL<<15);
    group03_mask[2] = (MW_INTXBAR2_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR2_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR2_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR2_FSITX0_INT2N_SEL<<3);
    group04_mask[2] = (MW_INTXBAR2_SD0_ERR_SEL<<0) | (MW_INTXBAR2_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR2_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR2_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR2_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR2_SD1_ERR_SEL<<5) | (MW_INTXBAR2_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR2_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR2_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR2_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[2] = (MW_INTXBAR2_ECAP0_INT_SEL<<0) | (MW_INTXBAR2_ECAP1_INT_SEL<<1) | (MW_INTXBAR2_ECAP2_INT_SEL<<2) | (MW_INTXBAR2_ECAP3_INT_SEL<<3) | (MW_INTXBAR2_ECAP4_INT_SEL<<4) | (MW_INTXBAR2_ECAP5_INT_SEL<<5) | (MW_INTXBAR2_ECAP6_INT_SEL<<6) | (MW_INTXBAR2_ECAP7_INT_SEL<<7);
    group06_mask[2] = (MW_INTXBAR2_EQEP0_INT_SEL<<0) | (MW_INTXBAR2_EQEP1_INT_SEL<<1);
    group07_mask[2] = (MW_INTXBAR2_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR2_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR2_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR2_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR2_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR2_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR2_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR2_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR2_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR2_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR2_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR2_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR2_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR2_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR2_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR2_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR2_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR2_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[3] = (MW_INTXBAR3_EPWM0_INT_SEL<<0) | (MW_INTXBAR3_EPWM1_INT_SEL<<1) | (MW_INTXBAR3_EPWM2_INT_SEL<<2) | (MW_INTXBAR3_EPWM3_INT_SEL<<3) | (MW_INTXBAR3_EPWM4_INT_SEL<<4) | (MW_INTXBAR3_EPWM5_INT_SEL<<5) | (MW_INTXBAR3_EPWM6_INT_SEL<<6) | (MW_INTXBAR3_EPWM7_INT_SEL<<7) | (MW_INTXBAR3_EPWM8_INT_SEL<<8) | (MW_INTXBAR3_EPWM9_INT_SEL<<9);
    group01_mask[3] = (MW_INTXBAR3_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR3_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR3_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR3_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR3_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR3_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR3_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR3_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR3_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR3_EPWM9_TZINT_SEL<<9);
    group02_mask[3] = (MW_INTXBAR3_ADC0_INT1_SEL<<0) | (MW_INTXBAR3_ADC0_INT2_SEL<<1) | (MW_INTXBAR3_ADC0_INT3_SEL<<2) | (MW_INTXBAR3_ADC0_INT4_SEL<<3) | (MW_INTXBAR3_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR3_ADC1_INT1_SEL<<5) | (MW_INTXBAR3_ADC1_INT2_SEL<<6) | (MW_INTXBAR3_ADC1_INT3_SEL<<7) | (MW_INTXBAR3_ADC1_INT4_SEL<<8) | (MW_INTXBAR3_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR3_ADC2_INT1_SEL<<10) | (MW_INTXBAR3_ADC2_INT2_SEL<<11) | (MW_INTXBAR3_ADC2_INT3_SEL<<12) | (MW_INTXBAR3_ADC2_INT4_SEL<<13) | (MW_INTXBAR3_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR3_EVTAGGR0_SEL<<15);
    group03_mask[3] = (MW_INTXBAR3_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR3_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR3_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR3_FSITX0_INT2N_SEL<<3);
    group04_mask[3] = (MW_INTXBAR3_SD0_ERR_SEL<<0) | (MW_INTXBAR3_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR3_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR3_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR3_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR3_SD1_ERR_SEL<<5) | (MW_INTXBAR3_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR3_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR3_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR3_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[3] = (MW_INTXBAR3_ECAP0_INT_SEL<<0) | (MW_INTXBAR3_ECAP1_INT_SEL<<1) | (MW_INTXBAR3_ECAP2_INT_SEL<<2) | (MW_INTXBAR3_ECAP3_INT_SEL<<3) | (MW_INTXBAR3_ECAP4_INT_SEL<<4) | (MW_INTXBAR3_ECAP5_INT_SEL<<5) | (MW_INTXBAR3_ECAP6_INT_SEL<<6) | (MW_INTXBAR3_ECAP7_INT_SEL<<7);
    group06_mask[3] = (MW_INTXBAR3_EQEP0_INT_SEL<<0) | (MW_INTXBAR3_EQEP1_INT_SEL<<1);
    group07_mask[3] = (MW_INTXBAR3_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR3_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR3_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR3_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR3_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR3_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR3_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR3_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR3_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR3_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR3_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR3_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR3_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR3_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR3_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR3_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR3_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR3_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[4] = (MW_INTXBAR4_EPWM0_INT_SEL<<0) | (MW_INTXBAR4_EPWM1_INT_SEL<<1) | (MW_INTXBAR4_EPWM2_INT_SEL<<2) | (MW_INTXBAR4_EPWM3_INT_SEL<<3) | (MW_INTXBAR4_EPWM4_INT_SEL<<4) | (MW_INTXBAR4_EPWM5_INT_SEL<<5) | (MW_INTXBAR4_EPWM6_INT_SEL<<6) | (MW_INTXBAR4_EPWM7_INT_SEL<<7) | (MW_INTXBAR4_EPWM8_INT_SEL<<8) | (MW_INTXBAR4_EPWM9_INT_SEL<<9);
    group01_mask[4] = (MW_INTXBAR4_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR4_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR4_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR4_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR4_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR4_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR4_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR4_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR4_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR4_EPWM9_TZINT_SEL<<9);
    group02_mask[4] = (MW_INTXBAR4_ADC0_INT1_SEL<<0) | (MW_INTXBAR4_ADC0_INT2_SEL<<1) | (MW_INTXBAR4_ADC0_INT3_SEL<<2) | (MW_INTXBAR4_ADC0_INT4_SEL<<3) | (MW_INTXBAR4_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR4_ADC1_INT1_SEL<<5) | (MW_INTXBAR4_ADC1_INT2_SEL<<6) | (MW_INTXBAR4_ADC1_INT3_SEL<<7) | (MW_INTXBAR4_ADC1_INT4_SEL<<8) | (MW_INTXBAR4_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR4_ADC2_INT1_SEL<<10) | (MW_INTXBAR4_ADC2_INT2_SEL<<11) | (MW_INTXBAR4_ADC2_INT3_SEL<<12) | (MW_INTXBAR4_ADC2_INT4_SEL<<13) | (MW_INTXBAR4_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR4_EVTAGGR0_SEL<<15);
    group03_mask[4] = (MW_INTXBAR4_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR4_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR4_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR4_FSITX0_INT2N_SEL<<3);
    group04_mask[4] = (MW_INTXBAR4_SD0_ERR_SEL<<0) | (MW_INTXBAR4_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR4_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR4_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR4_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR4_SD1_ERR_SEL<<5) | (MW_INTXBAR4_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR4_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR4_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR4_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[4] = (MW_INTXBAR4_ECAP0_INT_SEL<<0) | (MW_INTXBAR4_ECAP1_INT_SEL<<1) | (MW_INTXBAR4_ECAP2_INT_SEL<<2) | (MW_INTXBAR4_ECAP3_INT_SEL<<3) | (MW_INTXBAR4_ECAP4_INT_SEL<<4) | (MW_INTXBAR4_ECAP5_INT_SEL<<5) | (MW_INTXBAR4_ECAP6_INT_SEL<<6) | (MW_INTXBAR4_ECAP7_INT_SEL<<7);
    group06_mask[4] = (MW_INTXBAR4_EQEP0_INT_SEL<<0) | (MW_INTXBAR4_EQEP1_INT_SEL<<1);
    group07_mask[4] = (MW_INTXBAR4_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR4_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR4_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR4_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR4_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR4_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR4_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR4_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR4_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR4_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR4_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR4_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR4_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR4_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR4_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR4_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR4_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR4_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[5] = (MW_INTXBAR5_EPWM0_INT_SEL<<0) | (MW_INTXBAR5_EPWM1_INT_SEL<<1) | (MW_INTXBAR5_EPWM2_INT_SEL<<2) | (MW_INTXBAR5_EPWM3_INT_SEL<<3) | (MW_INTXBAR5_EPWM4_INT_SEL<<4) | (MW_INTXBAR5_EPWM5_INT_SEL<<5) | (MW_INTXBAR5_EPWM6_INT_SEL<<6) | (MW_INTXBAR5_EPWM7_INT_SEL<<7) | (MW_INTXBAR5_EPWM8_INT_SEL<<8) | (MW_INTXBAR5_EPWM9_INT_SEL<<9);
    group01_mask[5] = (MW_INTXBAR5_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR5_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR5_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR5_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR5_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR5_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR5_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR5_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR5_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR5_EPWM9_TZINT_SEL<<9);
    group02_mask[5] = (MW_INTXBAR5_ADC0_INT1_SEL<<0) | (MW_INTXBAR5_ADC0_INT2_SEL<<1) | (MW_INTXBAR5_ADC0_INT3_SEL<<2) | (MW_INTXBAR5_ADC0_INT4_SEL<<3) | (MW_INTXBAR5_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR5_ADC1_INT1_SEL<<5) | (MW_INTXBAR5_ADC1_INT2_SEL<<6) | (MW_INTXBAR5_ADC1_INT3_SEL<<7) | (MW_INTXBAR5_ADC1_INT4_SEL<<8) | (MW_INTXBAR5_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR5_ADC2_INT1_SEL<<10) | (MW_INTXBAR5_ADC2_INT2_SEL<<11) | (MW_INTXBAR5_ADC2_INT3_SEL<<12) | (MW_INTXBAR5_ADC2_INT4_SEL<<13) | (MW_INTXBAR5_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR5_EVTAGGR0_SEL<<15);
    group03_mask[5] = (MW_INTXBAR5_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR5_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR5_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR5_FSITX0_INT2N_SEL<<3);
    group04_mask[5] = (MW_INTXBAR5_SD0_ERR_SEL<<0) | (MW_INTXBAR5_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR5_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR5_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR5_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR5_SD1_ERR_SEL<<5) | (MW_INTXBAR5_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR5_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR5_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR5_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[5] = (MW_INTXBAR5_ECAP0_INT_SEL<<0) | (MW_INTXBAR5_ECAP1_INT_SEL<<1) | (MW_INTXBAR5_ECAP2_INT_SEL<<2) | (MW_INTXBAR5_ECAP3_INT_SEL<<3) | (MW_INTXBAR5_ECAP4_INT_SEL<<4) | (MW_INTXBAR5_ECAP5_INT_SEL<<5) | (MW_INTXBAR5_ECAP6_INT_SEL<<6) | (MW_INTXBAR5_ECAP7_INT_SEL<<7);
    group06_mask[5] = (MW_INTXBAR5_EQEP0_INT_SEL<<0) | (MW_INTXBAR5_EQEP1_INT_SEL<<1);
    group07_mask[5] = (MW_INTXBAR5_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR5_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR5_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR5_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR5_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR5_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR5_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR5_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR5_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR5_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR5_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR5_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR5_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR5_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR5_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR5_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR5_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR5_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[6] = (MW_INTXBAR6_EPWM0_INT_SEL<<0) | (MW_INTXBAR6_EPWM1_INT_SEL<<1) | (MW_INTXBAR6_EPWM2_INT_SEL<<2) | (MW_INTXBAR6_EPWM3_INT_SEL<<3) | (MW_INTXBAR6_EPWM4_INT_SEL<<4) | (MW_INTXBAR6_EPWM5_INT_SEL<<5) | (MW_INTXBAR6_EPWM6_INT_SEL<<6) | (MW_INTXBAR6_EPWM7_INT_SEL<<7) | (MW_INTXBAR6_EPWM8_INT_SEL<<8) | (MW_INTXBAR6_EPWM9_INT_SEL<<9);
    group01_mask[6] = (MW_INTXBAR6_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR6_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR6_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR6_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR6_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR6_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR6_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR6_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR6_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR6_EPWM9_TZINT_SEL<<9);
    group02_mask[6] = (MW_INTXBAR6_ADC0_INT1_SEL<<0) | (MW_INTXBAR6_ADC0_INT2_SEL<<1) | (MW_INTXBAR6_ADC0_INT3_SEL<<2) | (MW_INTXBAR6_ADC0_INT4_SEL<<3) | (MW_INTXBAR6_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR6_ADC1_INT1_SEL<<5) | (MW_INTXBAR6_ADC1_INT2_SEL<<6) | (MW_INTXBAR6_ADC1_INT3_SEL<<7) | (MW_INTXBAR6_ADC1_INT4_SEL<<8) | (MW_INTXBAR6_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR6_ADC2_INT1_SEL<<10) | (MW_INTXBAR6_ADC2_INT2_SEL<<11) | (MW_INTXBAR6_ADC2_INT3_SEL<<12) | (MW_INTXBAR6_ADC2_INT4_SEL<<13) | (MW_INTXBAR6_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR6_EVTAGGR0_SEL<<15);
    group03_mask[6] = (MW_INTXBAR6_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR6_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR6_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR6_FSITX0_INT2N_SEL<<3);
    group04_mask[6] = (MW_INTXBAR6_SD0_ERR_SEL<<0) | (MW_INTXBAR6_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR6_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR6_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR6_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR6_SD1_ERR_SEL<<5) | (MW_INTXBAR6_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR6_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR6_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR6_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[6] = (MW_INTXBAR6_ECAP0_INT_SEL<<0) | (MW_INTXBAR6_ECAP1_INT_SEL<<1) | (MW_INTXBAR6_ECAP2_INT_SEL<<2) | (MW_INTXBAR6_ECAP3_INT_SEL<<3) | (MW_INTXBAR6_ECAP4_INT_SEL<<4) | (MW_INTXBAR6_ECAP5_INT_SEL<<5) | (MW_INTXBAR6_ECAP6_INT_SEL<<6) | (MW_INTXBAR6_ECAP7_INT_SEL<<7);
    group06_mask[6] = (MW_INTXBAR6_EQEP0_INT_SEL<<0) | (MW_INTXBAR6_EQEP1_INT_SEL<<1);
    group07_mask[6] = (MW_INTXBAR6_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR6_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR6_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR6_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR6_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR6_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR6_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR6_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR6_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR6_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR6_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR6_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR6_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR6_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR6_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR6_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR6_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR6_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[7] = (MW_INTXBAR7_EPWM0_INT_SEL<<0) | (MW_INTXBAR7_EPWM1_INT_SEL<<1) | (MW_INTXBAR7_EPWM2_INT_SEL<<2) | (MW_INTXBAR7_EPWM3_INT_SEL<<3) | (MW_INTXBAR7_EPWM4_INT_SEL<<4) | (MW_INTXBAR7_EPWM5_INT_SEL<<5) | (MW_INTXBAR7_EPWM6_INT_SEL<<6) | (MW_INTXBAR7_EPWM7_INT_SEL<<7) | (MW_INTXBAR7_EPWM8_INT_SEL<<8) | (MW_INTXBAR7_EPWM9_INT_SEL<<9);
    group01_mask[7] = (MW_INTXBAR7_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR7_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR7_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR7_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR7_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR7_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR7_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR7_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR7_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR7_EPWM9_TZINT_SEL<<9);
    group02_mask[7] = (MW_INTXBAR7_ADC0_INT1_SEL<<0) | (MW_INTXBAR7_ADC0_INT2_SEL<<1) | (MW_INTXBAR7_ADC0_INT3_SEL<<2) | (MW_INTXBAR7_ADC0_INT4_SEL<<3) | (MW_INTXBAR7_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR7_ADC1_INT1_SEL<<5) | (MW_INTXBAR7_ADC1_INT2_SEL<<6) | (MW_INTXBAR7_ADC1_INT3_SEL<<7) | (MW_INTXBAR7_ADC1_INT4_SEL<<8) | (MW_INTXBAR7_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR7_ADC2_INT1_SEL<<10) | (MW_INTXBAR7_ADC2_INT2_SEL<<11) | (MW_INTXBAR7_ADC2_INT3_SEL<<12) | (MW_INTXBAR7_ADC2_INT4_SEL<<13) | (MW_INTXBAR7_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR7_EVTAGGR0_SEL<<15);
    group03_mask[7] = (MW_INTXBAR7_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR7_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR7_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR7_FSITX0_INT2N_SEL<<3);
    group04_mask[7] = (MW_INTXBAR7_SD0_ERR_SEL<<0) | (MW_INTXBAR7_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR7_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR7_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR7_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR7_SD1_ERR_SEL<<5) | (MW_INTXBAR7_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR7_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR7_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR7_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[7] = (MW_INTXBAR7_ECAP0_INT_SEL<<0) | (MW_INTXBAR7_ECAP1_INT_SEL<<1) | (MW_INTXBAR7_ECAP2_INT_SEL<<2) | (MW_INTXBAR7_ECAP3_INT_SEL<<3) | (MW_INTXBAR7_ECAP4_INT_SEL<<4) | (MW_INTXBAR7_ECAP5_INT_SEL<<5) | (MW_INTXBAR7_ECAP6_INT_SEL<<6) | (MW_INTXBAR7_ECAP7_INT_SEL<<7);
    group06_mask[7] = (MW_INTXBAR7_EQEP0_INT_SEL<<0) | (MW_INTXBAR7_EQEP1_INT_SEL<<1);
    group07_mask[7] = (MW_INTXBAR7_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR7_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR7_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR7_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR7_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR7_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR7_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR7_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR7_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR7_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR7_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR7_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR7_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR7_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR7_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR7_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR7_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR7_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[8] = (MW_INTXBAR8_EPWM0_INT_SEL<<0) | (MW_INTXBAR8_EPWM1_INT_SEL<<1) | (MW_INTXBAR8_EPWM2_INT_SEL<<2) | (MW_INTXBAR8_EPWM3_INT_SEL<<3) | (MW_INTXBAR8_EPWM4_INT_SEL<<4) | (MW_INTXBAR8_EPWM5_INT_SEL<<5) | (MW_INTXBAR8_EPWM6_INT_SEL<<6) | (MW_INTXBAR8_EPWM7_INT_SEL<<7) | (MW_INTXBAR8_EPWM8_INT_SEL<<8) | (MW_INTXBAR8_EPWM9_INT_SEL<<9);
    group01_mask[8] = (MW_INTXBAR8_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR8_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR8_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR8_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR8_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR8_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR8_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR8_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR8_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR8_EPWM9_TZINT_SEL<<9);
    group02_mask[8] = (MW_INTXBAR8_ADC0_INT1_SEL<<0) | (MW_INTXBAR8_ADC0_INT2_SEL<<1) | (MW_INTXBAR8_ADC0_INT3_SEL<<2) | (MW_INTXBAR8_ADC0_INT4_SEL<<3) | (MW_INTXBAR8_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR8_ADC1_INT1_SEL<<5) | (MW_INTXBAR8_ADC1_INT2_SEL<<6) | (MW_INTXBAR8_ADC1_INT3_SEL<<7) | (MW_INTXBAR8_ADC1_INT4_SEL<<8) | (MW_INTXBAR8_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR8_ADC2_INT1_SEL<<10) | (MW_INTXBAR8_ADC2_INT2_SEL<<11) | (MW_INTXBAR8_ADC2_INT3_SEL<<12) | (MW_INTXBAR8_ADC2_INT4_SEL<<13) | (MW_INTXBAR8_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR8_EVTAGGR0_SEL<<15);
    group03_mask[8] = (MW_INTXBAR8_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR8_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR8_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR8_FSITX0_INT2N_SEL<<3);
    group04_mask[8] = (MW_INTXBAR8_SD0_ERR_SEL<<0) | (MW_INTXBAR8_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR8_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR8_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR8_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR8_SD1_ERR_SEL<<5) | (MW_INTXBAR8_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR8_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR8_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR8_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[8] = (MW_INTXBAR8_ECAP0_INT_SEL<<0) | (MW_INTXBAR8_ECAP1_INT_SEL<<1) | (MW_INTXBAR8_ECAP2_INT_SEL<<2) | (MW_INTXBAR8_ECAP3_INT_SEL<<3) | (MW_INTXBAR8_ECAP4_INT_SEL<<4) | (MW_INTXBAR8_ECAP5_INT_SEL<<5) | (MW_INTXBAR8_ECAP6_INT_SEL<<6) | (MW_INTXBAR8_ECAP7_INT_SEL<<7);
    group06_mask[8] = (MW_INTXBAR8_EQEP0_INT_SEL<<0) | (MW_INTXBAR8_EQEP1_INT_SEL<<1);
    group07_mask[8] = (MW_INTXBAR8_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR8_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR8_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR8_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR8_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR8_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR8_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR8_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR8_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR8_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR8_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR8_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR8_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR8_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR8_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR8_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR8_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR8_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[9] = (MW_INTXBAR9_EPWM0_INT_SEL<<0) | (MW_INTXBAR9_EPWM1_INT_SEL<<1) | (MW_INTXBAR9_EPWM2_INT_SEL<<2) | (MW_INTXBAR9_EPWM3_INT_SEL<<3) | (MW_INTXBAR9_EPWM4_INT_SEL<<4) | (MW_INTXBAR9_EPWM5_INT_SEL<<5) | (MW_INTXBAR9_EPWM6_INT_SEL<<6) | (MW_INTXBAR9_EPWM7_INT_SEL<<7) | (MW_INTXBAR9_EPWM8_INT_SEL<<8) | (MW_INTXBAR9_EPWM9_INT_SEL<<9);
    group01_mask[9] = (MW_INTXBAR9_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR9_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR9_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR9_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR9_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR9_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR9_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR9_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR9_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR9_EPWM9_TZINT_SEL<<9);
    group02_mask[9] = (MW_INTXBAR9_ADC0_INT1_SEL<<0) | (MW_INTXBAR9_ADC0_INT2_SEL<<1) | (MW_INTXBAR9_ADC0_INT3_SEL<<2) | (MW_INTXBAR9_ADC0_INT4_SEL<<3) | (MW_INTXBAR9_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR9_ADC1_INT1_SEL<<5) | (MW_INTXBAR9_ADC1_INT2_SEL<<6) | (MW_INTXBAR9_ADC1_INT3_SEL<<7) | (MW_INTXBAR9_ADC1_INT4_SEL<<8) | (MW_INTXBAR9_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR9_ADC2_INT1_SEL<<10) | (MW_INTXBAR9_ADC2_INT2_SEL<<11) | (MW_INTXBAR9_ADC2_INT3_SEL<<12) | (MW_INTXBAR9_ADC2_INT4_SEL<<13) | (MW_INTXBAR9_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR9_EVTAGGR0_SEL<<15);
    group03_mask[9] = (MW_INTXBAR9_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR9_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR9_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR9_FSITX0_INT2N_SEL<<3);
    group04_mask[9] = (MW_INTXBAR9_SD0_ERR_SEL<<0) | (MW_INTXBAR9_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR9_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR9_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR9_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR9_SD1_ERR_SEL<<5) | (MW_INTXBAR9_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR9_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR9_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR9_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[9] = (MW_INTXBAR9_ECAP0_INT_SEL<<0) | (MW_INTXBAR9_ECAP1_INT_SEL<<1) | (MW_INTXBAR9_ECAP2_INT_SEL<<2) | (MW_INTXBAR9_ECAP3_INT_SEL<<3) | (MW_INTXBAR9_ECAP4_INT_SEL<<4) | (MW_INTXBAR9_ECAP5_INT_SEL<<5) | (MW_INTXBAR9_ECAP6_INT_SEL<<6) | (MW_INTXBAR9_ECAP7_INT_SEL<<7);
    group06_mask[9] = (MW_INTXBAR9_EQEP0_INT_SEL<<0) | (MW_INTXBAR9_EQEP1_INT_SEL<<1);
    group07_mask[9] = (MW_INTXBAR9_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR9_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR9_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR9_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR9_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR9_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR9_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR9_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR9_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR9_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR9_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR9_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR9_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR9_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR9_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR9_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR9_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR9_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[10] = (MW_INTXBAR10_EPWM0_INT_SEL<<0) | (MW_INTXBAR10_EPWM1_INT_SEL<<1) | (MW_INTXBAR10_EPWM2_INT_SEL<<2) | (MW_INTXBAR10_EPWM3_INT_SEL<<3) | (MW_INTXBAR10_EPWM4_INT_SEL<<4) | (MW_INTXBAR10_EPWM5_INT_SEL<<5) | (MW_INTXBAR10_EPWM6_INT_SEL<<6) | (MW_INTXBAR10_EPWM7_INT_SEL<<7) | (MW_INTXBAR10_EPWM8_INT_SEL<<8) | (MW_INTXBAR10_EPWM9_INT_SEL<<9);
    group01_mask[10] = (MW_INTXBAR10_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR10_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR10_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR10_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR10_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR10_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR10_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR10_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR10_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR10_EPWM9_TZINT_SEL<<9);
    group02_mask[10] = (MW_INTXBAR10_ADC0_INT1_SEL<<0) | (MW_INTXBAR10_ADC0_INT2_SEL<<1) | (MW_INTXBAR10_ADC0_INT3_SEL<<2) | (MW_INTXBAR10_ADC0_INT4_SEL<<3) | (MW_INTXBAR10_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR10_ADC1_INT1_SEL<<5) | (MW_INTXBAR10_ADC1_INT2_SEL<<6) | (MW_INTXBAR10_ADC1_INT3_SEL<<7) | (MW_INTXBAR10_ADC1_INT4_SEL<<8) | (MW_INTXBAR10_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR10_ADC2_INT1_SEL<<10) | (MW_INTXBAR10_ADC2_INT2_SEL<<11) | (MW_INTXBAR10_ADC2_INT3_SEL<<12) | (MW_INTXBAR10_ADC2_INT4_SEL<<13) | (MW_INTXBAR10_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR10_EVTAGGR0_SEL<<15);
    group03_mask[10] = (MW_INTXBAR10_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR10_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR10_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR10_FSITX0_INT2N_SEL<<3);
    group04_mask[10] = (MW_INTXBAR10_SD0_ERR_SEL<<0) | (MW_INTXBAR10_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR10_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR10_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR10_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR10_SD1_ERR_SEL<<5) | (MW_INTXBAR10_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR10_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR10_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR10_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[10] = (MW_INTXBAR10_ECAP0_INT_SEL<<0) | (MW_INTXBAR10_ECAP1_INT_SEL<<1) | (MW_INTXBAR10_ECAP2_INT_SEL<<2) | (MW_INTXBAR10_ECAP3_INT_SEL<<3) | (MW_INTXBAR10_ECAP4_INT_SEL<<4) | (MW_INTXBAR10_ECAP5_INT_SEL<<5) | (MW_INTXBAR10_ECAP6_INT_SEL<<6) | (MW_INTXBAR10_ECAP7_INT_SEL<<7);
    group06_mask[10] = (MW_INTXBAR10_EQEP0_INT_SEL<<0) | (MW_INTXBAR10_EQEP1_INT_SEL<<1);
    group07_mask[10] = (MW_INTXBAR10_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR10_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR10_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR10_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR10_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR10_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR10_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR10_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR10_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR10_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR10_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR10_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR10_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR10_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR10_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR10_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR10_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR10_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[11] = (MW_INTXBAR11_EPWM0_INT_SEL<<0) | (MW_INTXBAR11_EPWM1_INT_SEL<<1) | (MW_INTXBAR11_EPWM2_INT_SEL<<2) | (MW_INTXBAR11_EPWM3_INT_SEL<<3) | (MW_INTXBAR11_EPWM4_INT_SEL<<4) | (MW_INTXBAR11_EPWM5_INT_SEL<<5) | (MW_INTXBAR11_EPWM6_INT_SEL<<6) | (MW_INTXBAR11_EPWM7_INT_SEL<<7) | (MW_INTXBAR11_EPWM8_INT_SEL<<8) | (MW_INTXBAR11_EPWM9_INT_SEL<<9);
    group01_mask[11] = (MW_INTXBAR11_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR11_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR11_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR11_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR11_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR11_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR11_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR11_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR11_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR11_EPWM9_TZINT_SEL<<9);
    group02_mask[11] = (MW_INTXBAR11_ADC0_INT1_SEL<<0) | (MW_INTXBAR11_ADC0_INT2_SEL<<1) | (MW_INTXBAR11_ADC0_INT3_SEL<<2) | (MW_INTXBAR11_ADC0_INT4_SEL<<3) | (MW_INTXBAR11_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR11_ADC1_INT1_SEL<<5) | (MW_INTXBAR11_ADC1_INT2_SEL<<6) | (MW_INTXBAR11_ADC1_INT3_SEL<<7) | (MW_INTXBAR11_ADC1_INT4_SEL<<8) | (MW_INTXBAR11_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR11_ADC2_INT1_SEL<<10) | (MW_INTXBAR11_ADC2_INT2_SEL<<11) | (MW_INTXBAR11_ADC2_INT3_SEL<<12) | (MW_INTXBAR11_ADC2_INT4_SEL<<13) | (MW_INTXBAR11_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR11_EVTAGGR0_SEL<<15);
    group03_mask[11] = (MW_INTXBAR11_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR11_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR11_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR11_FSITX0_INT2N_SEL<<3);
    group04_mask[11] = (MW_INTXBAR11_SD0_ERR_SEL<<0) | (MW_INTXBAR11_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR11_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR11_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR11_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR11_SD1_ERR_SEL<<5) | (MW_INTXBAR11_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR11_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR11_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR11_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[11] = (MW_INTXBAR11_ECAP0_INT_SEL<<0) | (MW_INTXBAR11_ECAP1_INT_SEL<<1) | (MW_INTXBAR11_ECAP2_INT_SEL<<2) | (MW_INTXBAR11_ECAP3_INT_SEL<<3) | (MW_INTXBAR11_ECAP4_INT_SEL<<4) | (MW_INTXBAR11_ECAP5_INT_SEL<<5) | (MW_INTXBAR11_ECAP6_INT_SEL<<6) | (MW_INTXBAR11_ECAP7_INT_SEL<<7);
    group06_mask[11] = (MW_INTXBAR11_EQEP0_INT_SEL<<0) | (MW_INTXBAR11_EQEP1_INT_SEL<<1);
    group07_mask[11] = (MW_INTXBAR11_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR11_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR11_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR11_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR11_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR11_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR11_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR11_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR11_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR11_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR11_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR11_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR11_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR11_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR11_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR11_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR11_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR11_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[12] = (MW_INTXBAR12_EPWM0_INT_SEL<<0) | (MW_INTXBAR12_EPWM1_INT_SEL<<1) | (MW_INTXBAR12_EPWM2_INT_SEL<<2) | (MW_INTXBAR12_EPWM3_INT_SEL<<3) | (MW_INTXBAR12_EPWM4_INT_SEL<<4) | (MW_INTXBAR12_EPWM5_INT_SEL<<5) | (MW_INTXBAR12_EPWM6_INT_SEL<<6) | (MW_INTXBAR12_EPWM7_INT_SEL<<7) | (MW_INTXBAR12_EPWM8_INT_SEL<<8) | (MW_INTXBAR12_EPWM9_INT_SEL<<9);
    group01_mask[12] = (MW_INTXBAR12_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR12_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR12_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR12_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR12_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR12_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR12_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR12_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR12_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR12_EPWM9_TZINT_SEL<<9);
    group02_mask[12] = (MW_INTXBAR12_ADC0_INT1_SEL<<0) | (MW_INTXBAR12_ADC0_INT2_SEL<<1) | (MW_INTXBAR12_ADC0_INT3_SEL<<2) | (MW_INTXBAR12_ADC0_INT4_SEL<<3) | (MW_INTXBAR12_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR12_ADC1_INT1_SEL<<5) | (MW_INTXBAR12_ADC1_INT2_SEL<<6) | (MW_INTXBAR12_ADC1_INT3_SEL<<7) | (MW_INTXBAR12_ADC1_INT4_SEL<<8) | (MW_INTXBAR12_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR12_ADC2_INT1_SEL<<10) | (MW_INTXBAR12_ADC2_INT2_SEL<<11) | (MW_INTXBAR12_ADC2_INT3_SEL<<12) | (MW_INTXBAR12_ADC2_INT4_SEL<<13) | (MW_INTXBAR12_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR12_EVTAGGR0_SEL<<15);
    group03_mask[12] = (MW_INTXBAR12_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR12_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR12_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR12_FSITX0_INT2N_SEL<<3);
    group04_mask[12] = (MW_INTXBAR12_SD0_ERR_SEL<<0) | (MW_INTXBAR12_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR12_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR12_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR12_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR12_SD1_ERR_SEL<<5) | (MW_INTXBAR12_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR12_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR12_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR12_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[12] = (MW_INTXBAR12_ECAP0_INT_SEL<<0) | (MW_INTXBAR12_ECAP1_INT_SEL<<1) | (MW_INTXBAR12_ECAP2_INT_SEL<<2) | (MW_INTXBAR12_ECAP3_INT_SEL<<3) | (MW_INTXBAR12_ECAP4_INT_SEL<<4) | (MW_INTXBAR12_ECAP5_INT_SEL<<5) | (MW_INTXBAR12_ECAP6_INT_SEL<<6) | (MW_INTXBAR12_ECAP7_INT_SEL<<7);
    group06_mask[12] = (MW_INTXBAR12_EQEP0_INT_SEL<<0) | (MW_INTXBAR12_EQEP1_INT_SEL<<1);
    group07_mask[12] = (MW_INTXBAR12_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR12_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR12_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR12_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR12_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR12_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR12_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR12_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR12_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR12_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR12_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR12_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR12_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR12_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR12_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR12_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR12_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR12_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[13] = (MW_INTXBAR13_EPWM0_INT_SEL<<0) | (MW_INTXBAR13_EPWM1_INT_SEL<<1) | (MW_INTXBAR13_EPWM2_INT_SEL<<2) | (MW_INTXBAR13_EPWM3_INT_SEL<<3) | (MW_INTXBAR13_EPWM4_INT_SEL<<4) | (MW_INTXBAR13_EPWM5_INT_SEL<<5) | (MW_INTXBAR13_EPWM6_INT_SEL<<6) | (MW_INTXBAR13_EPWM7_INT_SEL<<7) | (MW_INTXBAR13_EPWM8_INT_SEL<<8) | (MW_INTXBAR13_EPWM9_INT_SEL<<9);
    group01_mask[13] = (MW_INTXBAR13_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR13_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR13_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR13_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR13_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR13_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR13_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR13_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR13_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR13_EPWM9_TZINT_SEL<<9);
    group02_mask[13] = (MW_INTXBAR13_ADC0_INT1_SEL<<0) | (MW_INTXBAR13_ADC0_INT2_SEL<<1) | (MW_INTXBAR13_ADC0_INT3_SEL<<2) | (MW_INTXBAR13_ADC0_INT4_SEL<<3) | (MW_INTXBAR13_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR13_ADC1_INT1_SEL<<5) | (MW_INTXBAR13_ADC1_INT2_SEL<<6) | (MW_INTXBAR13_ADC1_INT3_SEL<<7) | (MW_INTXBAR13_ADC1_INT4_SEL<<8) | (MW_INTXBAR13_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR13_ADC2_INT1_SEL<<10) | (MW_INTXBAR13_ADC2_INT2_SEL<<11) | (MW_INTXBAR13_ADC2_INT3_SEL<<12) | (MW_INTXBAR13_ADC2_INT4_SEL<<13) | (MW_INTXBAR13_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR13_EVTAGGR0_SEL<<15);
    group03_mask[13] = (MW_INTXBAR13_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR13_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR13_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR13_FSITX0_INT2N_SEL<<3);
    group04_mask[13] = (MW_INTXBAR13_SD0_ERR_SEL<<0) | (MW_INTXBAR13_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR13_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR13_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR13_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR13_SD1_ERR_SEL<<5) | (MW_INTXBAR13_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR13_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR13_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR13_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[13] = (MW_INTXBAR13_ECAP0_INT_SEL<<0) | (MW_INTXBAR13_ECAP1_INT_SEL<<1) | (MW_INTXBAR13_ECAP2_INT_SEL<<2) | (MW_INTXBAR13_ECAP3_INT_SEL<<3) | (MW_INTXBAR13_ECAP4_INT_SEL<<4) | (MW_INTXBAR13_ECAP5_INT_SEL<<5) | (MW_INTXBAR13_ECAP6_INT_SEL<<6) | (MW_INTXBAR13_ECAP7_INT_SEL<<7);
    group06_mask[13] = (MW_INTXBAR13_EQEP0_INT_SEL<<0) | (MW_INTXBAR13_EQEP1_INT_SEL<<1);
    group07_mask[13] = (MW_INTXBAR13_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR13_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR13_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR13_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR13_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR13_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR13_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR13_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR13_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR13_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR13_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR13_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR13_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR13_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR13_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR13_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR13_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR13_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[14] = (MW_INTXBAR14_EPWM0_INT_SEL<<0) | (MW_INTXBAR14_EPWM1_INT_SEL<<1) | (MW_INTXBAR14_EPWM2_INT_SEL<<2) | (MW_INTXBAR14_EPWM3_INT_SEL<<3) | (MW_INTXBAR14_EPWM4_INT_SEL<<4) | (MW_INTXBAR14_EPWM5_INT_SEL<<5) | (MW_INTXBAR14_EPWM6_INT_SEL<<6) | (MW_INTXBAR14_EPWM7_INT_SEL<<7) | (MW_INTXBAR14_EPWM8_INT_SEL<<8) | (MW_INTXBAR14_EPWM9_INT_SEL<<9);
    group01_mask[14] = (MW_INTXBAR14_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR14_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR14_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR14_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR14_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR14_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR14_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR14_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR14_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR14_EPWM9_TZINT_SEL<<9);
    group02_mask[14] = (MW_INTXBAR14_ADC0_INT1_SEL<<0) | (MW_INTXBAR14_ADC0_INT2_SEL<<1) | (MW_INTXBAR14_ADC0_INT3_SEL<<2) | (MW_INTXBAR14_ADC0_INT4_SEL<<3) | (MW_INTXBAR14_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR14_ADC1_INT1_SEL<<5) | (MW_INTXBAR14_ADC1_INT2_SEL<<6) | (MW_INTXBAR14_ADC1_INT3_SEL<<7) | (MW_INTXBAR14_ADC1_INT4_SEL<<8) | (MW_INTXBAR14_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR14_ADC2_INT1_SEL<<10) | (MW_INTXBAR14_ADC2_INT2_SEL<<11) | (MW_INTXBAR14_ADC2_INT3_SEL<<12) | (MW_INTXBAR14_ADC2_INT4_SEL<<13) | (MW_INTXBAR14_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR14_EVTAGGR0_SEL<<15);
    group03_mask[14] = (MW_INTXBAR14_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR14_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR14_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR14_FSITX0_INT2N_SEL<<3);
    group04_mask[14] = (MW_INTXBAR14_SD0_ERR_SEL<<0) | (MW_INTXBAR14_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR14_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR14_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR14_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR14_SD1_ERR_SEL<<5) | (MW_INTXBAR14_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR14_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR14_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR14_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[14] = (MW_INTXBAR14_ECAP0_INT_SEL<<0) | (MW_INTXBAR14_ECAP1_INT_SEL<<1) | (MW_INTXBAR14_ECAP2_INT_SEL<<2) | (MW_INTXBAR14_ECAP3_INT_SEL<<3) | (MW_INTXBAR14_ECAP4_INT_SEL<<4) | (MW_INTXBAR14_ECAP5_INT_SEL<<5) | (MW_INTXBAR14_ECAP6_INT_SEL<<6) | (MW_INTXBAR14_ECAP7_INT_SEL<<7);
    group06_mask[14] = (MW_INTXBAR14_EQEP0_INT_SEL<<0) | (MW_INTXBAR14_EQEP1_INT_SEL<<1);
    group07_mask[14] = (MW_INTXBAR14_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR14_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR14_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR14_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR14_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR14_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR14_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR14_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR14_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR14_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR14_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR14_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR14_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR14_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR14_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR14_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR14_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR14_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[15] = (MW_INTXBAR15_EPWM0_INT_SEL<<0) | (MW_INTXBAR15_EPWM1_INT_SEL<<1) | (MW_INTXBAR15_EPWM2_INT_SEL<<2) | (MW_INTXBAR15_EPWM3_INT_SEL<<3) | (MW_INTXBAR15_EPWM4_INT_SEL<<4) | (MW_INTXBAR15_EPWM5_INT_SEL<<5) | (MW_INTXBAR15_EPWM6_INT_SEL<<6) | (MW_INTXBAR15_EPWM7_INT_SEL<<7) | (MW_INTXBAR15_EPWM8_INT_SEL<<8) | (MW_INTXBAR15_EPWM9_INT_SEL<<9);
    group01_mask[15] = (MW_INTXBAR15_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR15_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR15_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR15_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR15_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR15_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR15_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR15_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR15_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR15_EPWM9_TZINT_SEL<<9);
    group02_mask[15] = (MW_INTXBAR15_ADC0_INT1_SEL<<0) | (MW_INTXBAR15_ADC0_INT2_SEL<<1) | (MW_INTXBAR15_ADC0_INT3_SEL<<2) | (MW_INTXBAR15_ADC0_INT4_SEL<<3) | (MW_INTXBAR15_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR15_ADC1_INT1_SEL<<5) | (MW_INTXBAR15_ADC1_INT2_SEL<<6) | (MW_INTXBAR15_ADC1_INT3_SEL<<7) | (MW_INTXBAR15_ADC1_INT4_SEL<<8) | (MW_INTXBAR15_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR15_ADC2_INT1_SEL<<10) | (MW_INTXBAR15_ADC2_INT2_SEL<<11) | (MW_INTXBAR15_ADC2_INT3_SEL<<12) | (MW_INTXBAR15_ADC2_INT4_SEL<<13) | (MW_INTXBAR15_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR15_EVTAGGR0_SEL<<15);
    group03_mask[15] = (MW_INTXBAR15_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR15_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR15_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR15_FSITX0_INT2N_SEL<<3);
    group04_mask[15] = (MW_INTXBAR15_SD0_ERR_SEL<<0) | (MW_INTXBAR15_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR15_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR15_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR15_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR15_SD1_ERR_SEL<<5) | (MW_INTXBAR15_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR15_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR15_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR15_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[15] = (MW_INTXBAR15_ECAP0_INT_SEL<<0) | (MW_INTXBAR15_ECAP1_INT_SEL<<1) | (MW_INTXBAR15_ECAP2_INT_SEL<<2) | (MW_INTXBAR15_ECAP3_INT_SEL<<3) | (MW_INTXBAR15_ECAP4_INT_SEL<<4) | (MW_INTXBAR15_ECAP5_INT_SEL<<5) | (MW_INTXBAR15_ECAP6_INT_SEL<<6) | (MW_INTXBAR15_ECAP7_INT_SEL<<7);
    group06_mask[15] = (MW_INTXBAR15_EQEP0_INT_SEL<<0) | (MW_INTXBAR15_EQEP1_INT_SEL<<1);
    group07_mask[15] = (MW_INTXBAR15_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR15_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR15_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR15_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR15_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR15_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR15_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR15_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR15_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR15_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR15_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR15_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR15_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR15_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR15_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR15_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR15_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR15_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[16] = (MW_INTXBAR16_EPWM0_INT_SEL<<0) | (MW_INTXBAR16_EPWM1_INT_SEL<<1) | (MW_INTXBAR16_EPWM2_INT_SEL<<2) | (MW_INTXBAR16_EPWM3_INT_SEL<<3) | (MW_INTXBAR16_EPWM4_INT_SEL<<4) | (MW_INTXBAR16_EPWM5_INT_SEL<<5) | (MW_INTXBAR16_EPWM6_INT_SEL<<6) | (MW_INTXBAR16_EPWM7_INT_SEL<<7) | (MW_INTXBAR16_EPWM8_INT_SEL<<8) | (MW_INTXBAR16_EPWM9_INT_SEL<<9);
    group01_mask[16] = (MW_INTXBAR16_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR16_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR16_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR16_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR16_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR16_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR16_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR16_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR16_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR16_EPWM9_TZINT_SEL<<9);
    group02_mask[16] = (MW_INTXBAR16_ADC0_INT1_SEL<<0) | (MW_INTXBAR16_ADC0_INT2_SEL<<1) | (MW_INTXBAR16_ADC0_INT3_SEL<<2) | (MW_INTXBAR16_ADC0_INT4_SEL<<3) | (MW_INTXBAR16_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR16_ADC1_INT1_SEL<<5) | (MW_INTXBAR16_ADC1_INT2_SEL<<6) | (MW_INTXBAR16_ADC1_INT3_SEL<<7) | (MW_INTXBAR16_ADC1_INT4_SEL<<8) | (MW_INTXBAR16_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR16_ADC2_INT1_SEL<<10) | (MW_INTXBAR16_ADC2_INT2_SEL<<11) | (MW_INTXBAR16_ADC2_INT3_SEL<<12) | (MW_INTXBAR16_ADC2_INT4_SEL<<13) | (MW_INTXBAR16_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR16_EVTAGGR0_SEL<<15);
    group03_mask[16] = (MW_INTXBAR16_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR16_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR16_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR16_FSITX0_INT2N_SEL<<3);
    group04_mask[16] = (MW_INTXBAR16_SD0_ERR_SEL<<0) | (MW_INTXBAR16_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR16_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR16_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR16_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR16_SD1_ERR_SEL<<5) | (MW_INTXBAR16_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR16_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR16_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR16_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[16] = (MW_INTXBAR16_ECAP0_INT_SEL<<0) | (MW_INTXBAR16_ECAP1_INT_SEL<<1) | (MW_INTXBAR16_ECAP2_INT_SEL<<2) | (MW_INTXBAR16_ECAP3_INT_SEL<<3) | (MW_INTXBAR16_ECAP4_INT_SEL<<4) | (MW_INTXBAR16_ECAP5_INT_SEL<<5) | (MW_INTXBAR16_ECAP6_INT_SEL<<6) | (MW_INTXBAR16_ECAP7_INT_SEL<<7);
    group06_mask[16] = (MW_INTXBAR16_EQEP0_INT_SEL<<0) | (MW_INTXBAR16_EQEP1_INT_SEL<<1);
    group07_mask[16] = (MW_INTXBAR16_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR16_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR16_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR16_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR16_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR16_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR16_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR16_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR16_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR16_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR16_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR16_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR16_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR16_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR16_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR16_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR16_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR16_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[17] = (MW_INTXBAR17_EPWM0_INT_SEL<<0) | (MW_INTXBAR17_EPWM1_INT_SEL<<1) | (MW_INTXBAR17_EPWM2_INT_SEL<<2) | (MW_INTXBAR17_EPWM3_INT_SEL<<3) | (MW_INTXBAR17_EPWM4_INT_SEL<<4) | (MW_INTXBAR17_EPWM5_INT_SEL<<5) | (MW_INTXBAR17_EPWM6_INT_SEL<<6) | (MW_INTXBAR17_EPWM7_INT_SEL<<7) | (MW_INTXBAR17_EPWM8_INT_SEL<<8) | (MW_INTXBAR17_EPWM9_INT_SEL<<9);
    group01_mask[17] = (MW_INTXBAR17_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR17_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR17_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR17_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR17_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR17_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR17_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR17_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR17_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR17_EPWM9_TZINT_SEL<<9);
    group02_mask[17] = (MW_INTXBAR17_ADC0_INT1_SEL<<0) | (MW_INTXBAR17_ADC0_INT2_SEL<<1) | (MW_INTXBAR17_ADC0_INT3_SEL<<2) | (MW_INTXBAR17_ADC0_INT4_SEL<<3) | (MW_INTXBAR17_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR17_ADC1_INT1_SEL<<5) | (MW_INTXBAR17_ADC1_INT2_SEL<<6) | (MW_INTXBAR17_ADC1_INT3_SEL<<7) | (MW_INTXBAR17_ADC1_INT4_SEL<<8) | (MW_INTXBAR17_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR17_ADC2_INT1_SEL<<10) | (MW_INTXBAR17_ADC2_INT2_SEL<<11) | (MW_INTXBAR17_ADC2_INT3_SEL<<12) | (MW_INTXBAR17_ADC2_INT4_SEL<<13) | (MW_INTXBAR17_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR17_EVTAGGR0_SEL<<15);
    group03_mask[17] = (MW_INTXBAR17_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR17_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR17_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR17_FSITX0_INT2N_SEL<<3);
    group04_mask[17] = (MW_INTXBAR17_SD0_ERR_SEL<<0) | (MW_INTXBAR17_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR17_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR17_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR17_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR17_SD1_ERR_SEL<<5) | (MW_INTXBAR17_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR17_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR17_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR17_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[17] = (MW_INTXBAR17_ECAP0_INT_SEL<<0) | (MW_INTXBAR17_ECAP1_INT_SEL<<1) | (MW_INTXBAR17_ECAP2_INT_SEL<<2) | (MW_INTXBAR17_ECAP3_INT_SEL<<3) | (MW_INTXBAR17_ECAP4_INT_SEL<<4) | (MW_INTXBAR17_ECAP5_INT_SEL<<5) | (MW_INTXBAR17_ECAP6_INT_SEL<<6) | (MW_INTXBAR17_ECAP7_INT_SEL<<7);
    group06_mask[17] = (MW_INTXBAR17_EQEP0_INT_SEL<<0) | (MW_INTXBAR17_EQEP1_INT_SEL<<1);
    group07_mask[17] = (MW_INTXBAR17_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR17_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR17_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR17_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR17_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR17_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR17_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR17_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR17_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR17_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR17_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR17_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR17_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR17_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR17_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR17_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR17_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR17_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[18] = (MW_INTXBAR18_EPWM0_INT_SEL<<0) | (MW_INTXBAR18_EPWM1_INT_SEL<<1) | (MW_INTXBAR18_EPWM2_INT_SEL<<2) | (MW_INTXBAR18_EPWM3_INT_SEL<<3) | (MW_INTXBAR18_EPWM4_INT_SEL<<4) | (MW_INTXBAR18_EPWM5_INT_SEL<<5) | (MW_INTXBAR18_EPWM6_INT_SEL<<6) | (MW_INTXBAR18_EPWM7_INT_SEL<<7) | (MW_INTXBAR18_EPWM8_INT_SEL<<8) | (MW_INTXBAR18_EPWM9_INT_SEL<<9);
    group01_mask[18] = (MW_INTXBAR18_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR18_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR18_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR18_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR18_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR18_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR18_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR18_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR18_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR18_EPWM9_TZINT_SEL<<9);
    group02_mask[18] = (MW_INTXBAR18_ADC0_INT1_SEL<<0) | (MW_INTXBAR18_ADC0_INT2_SEL<<1) | (MW_INTXBAR18_ADC0_INT3_SEL<<2) | (MW_INTXBAR18_ADC0_INT4_SEL<<3) | (MW_INTXBAR18_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR18_ADC1_INT1_SEL<<5) | (MW_INTXBAR18_ADC1_INT2_SEL<<6) | (MW_INTXBAR18_ADC1_INT3_SEL<<7) | (MW_INTXBAR18_ADC1_INT4_SEL<<8) | (MW_INTXBAR18_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR18_ADC2_INT1_SEL<<10) | (MW_INTXBAR18_ADC2_INT2_SEL<<11) | (MW_INTXBAR18_ADC2_INT3_SEL<<12) | (MW_INTXBAR18_ADC2_INT4_SEL<<13) | (MW_INTXBAR18_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR18_EVTAGGR0_SEL<<15);
    group03_mask[18] = (MW_INTXBAR18_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR18_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR18_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR18_FSITX0_INT2N_SEL<<3);
    group04_mask[18] = (MW_INTXBAR18_SD0_ERR_SEL<<0) | (MW_INTXBAR18_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR18_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR18_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR18_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR18_SD1_ERR_SEL<<5) | (MW_INTXBAR18_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR18_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR18_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR18_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[18] = (MW_INTXBAR18_ECAP0_INT_SEL<<0) | (MW_INTXBAR18_ECAP1_INT_SEL<<1) | (MW_INTXBAR18_ECAP2_INT_SEL<<2) | (MW_INTXBAR18_ECAP3_INT_SEL<<3) | (MW_INTXBAR18_ECAP4_INT_SEL<<4) | (MW_INTXBAR18_ECAP5_INT_SEL<<5) | (MW_INTXBAR18_ECAP6_INT_SEL<<6) | (MW_INTXBAR18_ECAP7_INT_SEL<<7);
    group06_mask[18] = (MW_INTXBAR18_EQEP0_INT_SEL<<0) | (MW_INTXBAR18_EQEP1_INT_SEL<<1);
    group07_mask[18] = (MW_INTXBAR18_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR18_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR18_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR18_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR18_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR18_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR18_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR18_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR18_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR18_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR18_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR18_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR18_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR18_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR18_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR18_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR18_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR18_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[19] = (MW_INTXBAR19_EPWM0_INT_SEL<<0) | (MW_INTXBAR19_EPWM1_INT_SEL<<1) | (MW_INTXBAR19_EPWM2_INT_SEL<<2) | (MW_INTXBAR19_EPWM3_INT_SEL<<3) | (MW_INTXBAR19_EPWM4_INT_SEL<<4) | (MW_INTXBAR19_EPWM5_INT_SEL<<5) | (MW_INTXBAR19_EPWM6_INT_SEL<<6) | (MW_INTXBAR19_EPWM7_INT_SEL<<7) | (MW_INTXBAR19_EPWM8_INT_SEL<<8) | (MW_INTXBAR19_EPWM9_INT_SEL<<9);
    group01_mask[19] = (MW_INTXBAR19_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR19_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR19_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR19_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR19_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR19_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR19_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR19_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR19_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR19_EPWM9_TZINT_SEL<<9);
    group02_mask[19] = (MW_INTXBAR19_ADC0_INT1_SEL<<0) | (MW_INTXBAR19_ADC0_INT2_SEL<<1) | (MW_INTXBAR19_ADC0_INT3_SEL<<2) | (MW_INTXBAR19_ADC0_INT4_SEL<<3) | (MW_INTXBAR19_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR19_ADC1_INT1_SEL<<5) | (MW_INTXBAR19_ADC1_INT2_SEL<<6) | (MW_INTXBAR19_ADC1_INT3_SEL<<7) | (MW_INTXBAR19_ADC1_INT4_SEL<<8) | (MW_INTXBAR19_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR19_ADC2_INT1_SEL<<10) | (MW_INTXBAR19_ADC2_INT2_SEL<<11) | (MW_INTXBAR19_ADC2_INT3_SEL<<12) | (MW_INTXBAR19_ADC2_INT4_SEL<<13) | (MW_INTXBAR19_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR19_EVTAGGR0_SEL<<15);
    group03_mask[19] = (MW_INTXBAR19_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR19_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR19_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR19_FSITX0_INT2N_SEL<<3);
    group04_mask[19] = (MW_INTXBAR19_SD0_ERR_SEL<<0) | (MW_INTXBAR19_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR19_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR19_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR19_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR19_SD1_ERR_SEL<<5) | (MW_INTXBAR19_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR19_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR19_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR19_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[19] = (MW_INTXBAR19_ECAP0_INT_SEL<<0) | (MW_INTXBAR19_ECAP1_INT_SEL<<1) | (MW_INTXBAR19_ECAP2_INT_SEL<<2) | (MW_INTXBAR19_ECAP3_INT_SEL<<3) | (MW_INTXBAR19_ECAP4_INT_SEL<<4) | (MW_INTXBAR19_ECAP5_INT_SEL<<5) | (MW_INTXBAR19_ECAP6_INT_SEL<<6) | (MW_INTXBAR19_ECAP7_INT_SEL<<7);
    group06_mask[19] = (MW_INTXBAR19_EQEP0_INT_SEL<<0) | (MW_INTXBAR19_EQEP1_INT_SEL<<1);
    group07_mask[19] = (MW_INTXBAR19_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR19_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR19_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR19_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR19_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR19_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR19_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR19_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR19_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR19_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR19_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR19_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR19_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR19_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR19_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR19_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR19_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR19_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[20] = (MW_INTXBAR20_EPWM0_INT_SEL<<0) | (MW_INTXBAR20_EPWM1_INT_SEL<<1) | (MW_INTXBAR20_EPWM2_INT_SEL<<2) | (MW_INTXBAR20_EPWM3_INT_SEL<<3) | (MW_INTXBAR20_EPWM4_INT_SEL<<4) | (MW_INTXBAR20_EPWM5_INT_SEL<<5) | (MW_INTXBAR20_EPWM6_INT_SEL<<6) | (MW_INTXBAR20_EPWM7_INT_SEL<<7) | (MW_INTXBAR20_EPWM8_INT_SEL<<8) | (MW_INTXBAR20_EPWM9_INT_SEL<<9);
    group01_mask[20] = (MW_INTXBAR20_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR20_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR20_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR20_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR20_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR20_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR20_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR20_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR20_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR20_EPWM9_TZINT_SEL<<9);
    group02_mask[20] = (MW_INTXBAR20_ADC0_INT1_SEL<<0) | (MW_INTXBAR20_ADC0_INT2_SEL<<1) | (MW_INTXBAR20_ADC0_INT3_SEL<<2) | (MW_INTXBAR20_ADC0_INT4_SEL<<3) | (MW_INTXBAR20_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR20_ADC1_INT1_SEL<<5) | (MW_INTXBAR20_ADC1_INT2_SEL<<6) | (MW_INTXBAR20_ADC1_INT3_SEL<<7) | (MW_INTXBAR20_ADC1_INT4_SEL<<8) | (MW_INTXBAR20_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR20_ADC2_INT1_SEL<<10) | (MW_INTXBAR20_ADC2_INT2_SEL<<11) | (MW_INTXBAR20_ADC2_INT3_SEL<<12) | (MW_INTXBAR20_ADC2_INT4_SEL<<13) | (MW_INTXBAR20_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR20_EVTAGGR0_SEL<<15);
    group03_mask[20] = (MW_INTXBAR20_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR20_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR20_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR20_FSITX0_INT2N_SEL<<3);
    group04_mask[20] = (MW_INTXBAR20_SD0_ERR_SEL<<0) | (MW_INTXBAR20_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR20_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR20_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR20_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR20_SD1_ERR_SEL<<5) | (MW_INTXBAR20_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR20_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR20_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR20_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[20] = (MW_INTXBAR20_ECAP0_INT_SEL<<0) | (MW_INTXBAR20_ECAP1_INT_SEL<<1) | (MW_INTXBAR20_ECAP2_INT_SEL<<2) | (MW_INTXBAR20_ECAP3_INT_SEL<<3) | (MW_INTXBAR20_ECAP4_INT_SEL<<4) | (MW_INTXBAR20_ECAP5_INT_SEL<<5) | (MW_INTXBAR20_ECAP6_INT_SEL<<6) | (MW_INTXBAR20_ECAP7_INT_SEL<<7);
    group06_mask[20] = (MW_INTXBAR20_EQEP0_INT_SEL<<0) | (MW_INTXBAR20_EQEP1_INT_SEL<<1);
    group07_mask[20] = (MW_INTXBAR20_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR20_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR20_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR20_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR20_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR20_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR20_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR20_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR20_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR20_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR20_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR20_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR20_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR20_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR20_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR20_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR20_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR20_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[21] = (MW_INTXBAR21_EPWM0_INT_SEL<<0) | (MW_INTXBAR21_EPWM1_INT_SEL<<1) | (MW_INTXBAR21_EPWM2_INT_SEL<<2) | (MW_INTXBAR21_EPWM3_INT_SEL<<3) | (MW_INTXBAR21_EPWM4_INT_SEL<<4) | (MW_INTXBAR21_EPWM5_INT_SEL<<5) | (MW_INTXBAR21_EPWM6_INT_SEL<<6) | (MW_INTXBAR21_EPWM7_INT_SEL<<7) | (MW_INTXBAR21_EPWM8_INT_SEL<<8) | (MW_INTXBAR21_EPWM9_INT_SEL<<9);
    group01_mask[21] = (MW_INTXBAR21_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR21_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR21_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR21_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR21_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR21_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR21_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR21_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR21_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR21_EPWM9_TZINT_SEL<<9);
    group02_mask[21] = (MW_INTXBAR21_ADC0_INT1_SEL<<0) | (MW_INTXBAR21_ADC0_INT2_SEL<<1) | (MW_INTXBAR21_ADC0_INT3_SEL<<2) | (MW_INTXBAR21_ADC0_INT4_SEL<<3) | (MW_INTXBAR21_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR21_ADC1_INT1_SEL<<5) | (MW_INTXBAR21_ADC1_INT2_SEL<<6) | (MW_INTXBAR21_ADC1_INT3_SEL<<7) | (MW_INTXBAR21_ADC1_INT4_SEL<<8) | (MW_INTXBAR21_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR21_ADC2_INT1_SEL<<10) | (MW_INTXBAR21_ADC2_INT2_SEL<<11) | (MW_INTXBAR21_ADC2_INT3_SEL<<12) | (MW_INTXBAR21_ADC2_INT4_SEL<<13) | (MW_INTXBAR21_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR21_EVTAGGR0_SEL<<15);
    group03_mask[21] = (MW_INTXBAR21_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR21_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR21_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR21_FSITX0_INT2N_SEL<<3);
    group04_mask[21] = (MW_INTXBAR21_SD0_ERR_SEL<<0) | (MW_INTXBAR21_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR21_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR21_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR21_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR21_SD1_ERR_SEL<<5) | (MW_INTXBAR21_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR21_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR21_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR21_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[21] = (MW_INTXBAR21_ECAP0_INT_SEL<<0) | (MW_INTXBAR21_ECAP1_INT_SEL<<1) | (MW_INTXBAR21_ECAP2_INT_SEL<<2) | (MW_INTXBAR21_ECAP3_INT_SEL<<3) | (MW_INTXBAR21_ECAP4_INT_SEL<<4) | (MW_INTXBAR21_ECAP5_INT_SEL<<5) | (MW_INTXBAR21_ECAP6_INT_SEL<<6) | (MW_INTXBAR21_ECAP7_INT_SEL<<7);
    group06_mask[21] = (MW_INTXBAR21_EQEP0_INT_SEL<<0) | (MW_INTXBAR21_EQEP1_INT_SEL<<1);
    group07_mask[21] = (MW_INTXBAR21_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR21_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR21_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR21_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR21_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR21_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR21_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR21_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR21_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR21_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR21_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR21_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR21_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR21_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR21_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR21_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR21_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR21_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[22] = (MW_INTXBAR22_EPWM0_INT_SEL<<0) | (MW_INTXBAR22_EPWM1_INT_SEL<<1) | (MW_INTXBAR22_EPWM2_INT_SEL<<2) | (MW_INTXBAR22_EPWM3_INT_SEL<<3) | (MW_INTXBAR22_EPWM4_INT_SEL<<4) | (MW_INTXBAR22_EPWM5_INT_SEL<<5) | (MW_INTXBAR22_EPWM6_INT_SEL<<6) | (MW_INTXBAR22_EPWM7_INT_SEL<<7) | (MW_INTXBAR22_EPWM8_INT_SEL<<8) | (MW_INTXBAR22_EPWM9_INT_SEL<<9);
    group01_mask[22] = (MW_INTXBAR22_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR22_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR22_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR22_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR22_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR22_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR22_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR22_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR22_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR22_EPWM9_TZINT_SEL<<9);
    group02_mask[22] = (MW_INTXBAR22_ADC0_INT1_SEL<<0) | (MW_INTXBAR22_ADC0_INT2_SEL<<1) | (MW_INTXBAR22_ADC0_INT3_SEL<<2) | (MW_INTXBAR22_ADC0_INT4_SEL<<3) | (MW_INTXBAR22_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR22_ADC1_INT1_SEL<<5) | (MW_INTXBAR22_ADC1_INT2_SEL<<6) | (MW_INTXBAR22_ADC1_INT3_SEL<<7) | (MW_INTXBAR22_ADC1_INT4_SEL<<8) | (MW_INTXBAR22_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR22_ADC2_INT1_SEL<<10) | (MW_INTXBAR22_ADC2_INT2_SEL<<11) | (MW_INTXBAR22_ADC2_INT3_SEL<<12) | (MW_INTXBAR22_ADC2_INT4_SEL<<13) | (MW_INTXBAR22_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR22_EVTAGGR0_SEL<<15);
    group03_mask[22] = (MW_INTXBAR22_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR22_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR22_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR22_FSITX0_INT2N_SEL<<3);
    group04_mask[22] = (MW_INTXBAR22_SD0_ERR_SEL<<0) | (MW_INTXBAR22_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR22_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR22_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR22_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR22_SD1_ERR_SEL<<5) | (MW_INTXBAR22_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR22_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR22_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR22_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[22] = (MW_INTXBAR22_ECAP0_INT_SEL<<0) | (MW_INTXBAR22_ECAP1_INT_SEL<<1) | (MW_INTXBAR22_ECAP2_INT_SEL<<2) | (MW_INTXBAR22_ECAP3_INT_SEL<<3) | (MW_INTXBAR22_ECAP4_INT_SEL<<4) | (MW_INTXBAR22_ECAP5_INT_SEL<<5) | (MW_INTXBAR22_ECAP6_INT_SEL<<6) | (MW_INTXBAR22_ECAP7_INT_SEL<<7);
    group06_mask[22] = (MW_INTXBAR22_EQEP0_INT_SEL<<0) | (MW_INTXBAR22_EQEP1_INT_SEL<<1);
    group07_mask[22] = (MW_INTXBAR22_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR22_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR22_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR22_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR22_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR22_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR22_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR22_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR22_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR22_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR22_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR22_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR22_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR22_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR22_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR22_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR22_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR22_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[23] = (MW_INTXBAR23_EPWM0_INT_SEL<<0) | (MW_INTXBAR23_EPWM1_INT_SEL<<1) | (MW_INTXBAR23_EPWM2_INT_SEL<<2) | (MW_INTXBAR23_EPWM3_INT_SEL<<3) | (MW_INTXBAR23_EPWM4_INT_SEL<<4) | (MW_INTXBAR23_EPWM5_INT_SEL<<5) | (MW_INTXBAR23_EPWM6_INT_SEL<<6) | (MW_INTXBAR23_EPWM7_INT_SEL<<7) | (MW_INTXBAR23_EPWM8_INT_SEL<<8) | (MW_INTXBAR23_EPWM9_INT_SEL<<9);
    group01_mask[23] = (MW_INTXBAR23_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR23_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR23_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR23_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR23_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR23_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR23_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR23_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR23_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR23_EPWM9_TZINT_SEL<<9);
    group02_mask[23] = (MW_INTXBAR23_ADC0_INT1_SEL<<0) | (MW_INTXBAR23_ADC0_INT2_SEL<<1) | (MW_INTXBAR23_ADC0_INT3_SEL<<2) | (MW_INTXBAR23_ADC0_INT4_SEL<<3) | (MW_INTXBAR23_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR23_ADC1_INT1_SEL<<5) | (MW_INTXBAR23_ADC1_INT2_SEL<<6) | (MW_INTXBAR23_ADC1_INT3_SEL<<7) | (MW_INTXBAR23_ADC1_INT4_SEL<<8) | (MW_INTXBAR23_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR23_ADC2_INT1_SEL<<10) | (MW_INTXBAR23_ADC2_INT2_SEL<<11) | (MW_INTXBAR23_ADC2_INT3_SEL<<12) | (MW_INTXBAR23_ADC2_INT4_SEL<<13) | (MW_INTXBAR23_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR23_EVTAGGR0_SEL<<15);
    group03_mask[23] = (MW_INTXBAR23_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR23_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR23_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR23_FSITX0_INT2N_SEL<<3);
    group04_mask[23] = (MW_INTXBAR23_SD0_ERR_SEL<<0) | (MW_INTXBAR23_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR23_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR23_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR23_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR23_SD1_ERR_SEL<<5) | (MW_INTXBAR23_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR23_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR23_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR23_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[23] = (MW_INTXBAR23_ECAP0_INT_SEL<<0) | (MW_INTXBAR23_ECAP1_INT_SEL<<1) | (MW_INTXBAR23_ECAP2_INT_SEL<<2) | (MW_INTXBAR23_ECAP3_INT_SEL<<3) | (MW_INTXBAR23_ECAP4_INT_SEL<<4) | (MW_INTXBAR23_ECAP5_INT_SEL<<5) | (MW_INTXBAR23_ECAP6_INT_SEL<<6) | (MW_INTXBAR23_ECAP7_INT_SEL<<7);
    group06_mask[23] = (MW_INTXBAR23_EQEP0_INT_SEL<<0) | (MW_INTXBAR23_EQEP1_INT_SEL<<1);
    group07_mask[23] = (MW_INTXBAR23_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR23_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR23_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR23_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR23_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR23_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR23_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR23_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR23_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR23_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR23_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR23_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR23_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR23_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR23_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR23_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR23_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR23_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[24] = (MW_INTXBAR24_EPWM0_INT_SEL<<0) | (MW_INTXBAR24_EPWM1_INT_SEL<<1) | (MW_INTXBAR24_EPWM2_INT_SEL<<2) | (MW_INTXBAR24_EPWM3_INT_SEL<<3) | (MW_INTXBAR24_EPWM4_INT_SEL<<4) | (MW_INTXBAR24_EPWM5_INT_SEL<<5) | (MW_INTXBAR24_EPWM6_INT_SEL<<6) | (MW_INTXBAR24_EPWM7_INT_SEL<<7) | (MW_INTXBAR24_EPWM8_INT_SEL<<8) | (MW_INTXBAR24_EPWM9_INT_SEL<<9);
    group01_mask[24] = (MW_INTXBAR24_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR24_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR24_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR24_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR24_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR24_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR24_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR24_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR24_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR24_EPWM9_TZINT_SEL<<9);
    group02_mask[24] = (MW_INTXBAR24_ADC0_INT1_SEL<<0) | (MW_INTXBAR24_ADC0_INT2_SEL<<1) | (MW_INTXBAR24_ADC0_INT3_SEL<<2) | (MW_INTXBAR24_ADC0_INT4_SEL<<3) | (MW_INTXBAR24_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR24_ADC1_INT1_SEL<<5) | (MW_INTXBAR24_ADC1_INT2_SEL<<6) | (MW_INTXBAR24_ADC1_INT3_SEL<<7) | (MW_INTXBAR24_ADC1_INT4_SEL<<8) | (MW_INTXBAR24_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR24_ADC2_INT1_SEL<<10) | (MW_INTXBAR24_ADC2_INT2_SEL<<11) | (MW_INTXBAR24_ADC2_INT3_SEL<<12) | (MW_INTXBAR24_ADC2_INT4_SEL<<13) | (MW_INTXBAR24_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR24_EVTAGGR0_SEL<<15);
    group03_mask[24] = (MW_INTXBAR24_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR24_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR24_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR24_FSITX0_INT2N_SEL<<3);
    group04_mask[24] = (MW_INTXBAR24_SD0_ERR_SEL<<0) | (MW_INTXBAR24_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR24_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR24_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR24_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR24_SD1_ERR_SEL<<5) | (MW_INTXBAR24_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR24_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR24_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR24_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[24] = (MW_INTXBAR24_ECAP0_INT_SEL<<0) | (MW_INTXBAR24_ECAP1_INT_SEL<<1) | (MW_INTXBAR24_ECAP2_INT_SEL<<2) | (MW_INTXBAR24_ECAP3_INT_SEL<<3) | (MW_INTXBAR24_ECAP4_INT_SEL<<4) | (MW_INTXBAR24_ECAP5_INT_SEL<<5) | (MW_INTXBAR24_ECAP6_INT_SEL<<6) | (MW_INTXBAR24_ECAP7_INT_SEL<<7);
    group06_mask[24] = (MW_INTXBAR24_EQEP0_INT_SEL<<0) | (MW_INTXBAR24_EQEP1_INT_SEL<<1);
    group07_mask[24] = (MW_INTXBAR24_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR24_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR24_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR24_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR24_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR24_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR24_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR24_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR24_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR24_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR24_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR24_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR24_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR24_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR24_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR24_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR24_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR24_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[25] = (MW_INTXBAR25_EPWM0_INT_SEL<<0) | (MW_INTXBAR25_EPWM1_INT_SEL<<1) | (MW_INTXBAR25_EPWM2_INT_SEL<<2) | (MW_INTXBAR25_EPWM3_INT_SEL<<3) | (MW_INTXBAR25_EPWM4_INT_SEL<<4) | (MW_INTXBAR25_EPWM5_INT_SEL<<5) | (MW_INTXBAR25_EPWM6_INT_SEL<<6) | (MW_INTXBAR25_EPWM7_INT_SEL<<7) | (MW_INTXBAR25_EPWM8_INT_SEL<<8) | (MW_INTXBAR25_EPWM9_INT_SEL<<9);
    group01_mask[25] = (MW_INTXBAR25_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR25_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR25_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR25_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR25_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR25_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR25_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR25_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR25_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR25_EPWM9_TZINT_SEL<<9);
    group02_mask[25] = (MW_INTXBAR25_ADC0_INT1_SEL<<0) | (MW_INTXBAR25_ADC0_INT2_SEL<<1) | (MW_INTXBAR25_ADC0_INT3_SEL<<2) | (MW_INTXBAR25_ADC0_INT4_SEL<<3) | (MW_INTXBAR25_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR25_ADC1_INT1_SEL<<5) | (MW_INTXBAR25_ADC1_INT2_SEL<<6) | (MW_INTXBAR25_ADC1_INT3_SEL<<7) | (MW_INTXBAR25_ADC1_INT4_SEL<<8) | (MW_INTXBAR25_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR25_ADC2_INT1_SEL<<10) | (MW_INTXBAR25_ADC2_INT2_SEL<<11) | (MW_INTXBAR25_ADC2_INT3_SEL<<12) | (MW_INTXBAR25_ADC2_INT4_SEL<<13) | (MW_INTXBAR25_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR25_EVTAGGR0_SEL<<15);
    group03_mask[25] = (MW_INTXBAR25_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR25_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR25_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR25_FSITX0_INT2N_SEL<<3);
    group04_mask[25] = (MW_INTXBAR25_SD0_ERR_SEL<<0) | (MW_INTXBAR25_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR25_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR25_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR25_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR25_SD1_ERR_SEL<<5) | (MW_INTXBAR25_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR25_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR25_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR25_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[25] = (MW_INTXBAR25_ECAP0_INT_SEL<<0) | (MW_INTXBAR25_ECAP1_INT_SEL<<1) | (MW_INTXBAR25_ECAP2_INT_SEL<<2) | (MW_INTXBAR25_ECAP3_INT_SEL<<3) | (MW_INTXBAR25_ECAP4_INT_SEL<<4) | (MW_INTXBAR25_ECAP5_INT_SEL<<5) | (MW_INTXBAR25_ECAP6_INT_SEL<<6) | (MW_INTXBAR25_ECAP7_INT_SEL<<7);
    group06_mask[25] = (MW_INTXBAR25_EQEP0_INT_SEL<<0) | (MW_INTXBAR25_EQEP1_INT_SEL<<1);
    group07_mask[25] = (MW_INTXBAR25_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR25_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR25_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR25_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR25_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR25_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR25_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR25_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR25_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR25_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR25_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR25_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR25_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR25_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR25_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR25_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR25_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR25_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[26] = (MW_INTXBAR26_EPWM0_INT_SEL<<0) | (MW_INTXBAR26_EPWM1_INT_SEL<<1) | (MW_INTXBAR26_EPWM2_INT_SEL<<2) | (MW_INTXBAR26_EPWM3_INT_SEL<<3) | (MW_INTXBAR26_EPWM4_INT_SEL<<4) | (MW_INTXBAR26_EPWM5_INT_SEL<<5) | (MW_INTXBAR26_EPWM6_INT_SEL<<6) | (MW_INTXBAR26_EPWM7_INT_SEL<<7) | (MW_INTXBAR26_EPWM8_INT_SEL<<8) | (MW_INTXBAR26_EPWM9_INT_SEL<<9);
    group01_mask[26] = (MW_INTXBAR26_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR26_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR26_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR26_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR26_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR26_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR26_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR26_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR26_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR26_EPWM9_TZINT_SEL<<9);
    group02_mask[26] = (MW_INTXBAR26_ADC0_INT1_SEL<<0) | (MW_INTXBAR26_ADC0_INT2_SEL<<1) | (MW_INTXBAR26_ADC0_INT3_SEL<<2) | (MW_INTXBAR26_ADC0_INT4_SEL<<3) | (MW_INTXBAR26_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR26_ADC1_INT1_SEL<<5) | (MW_INTXBAR26_ADC1_INT2_SEL<<6) | (MW_INTXBAR26_ADC1_INT3_SEL<<7) | (MW_INTXBAR26_ADC1_INT4_SEL<<8) | (MW_INTXBAR26_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR26_ADC2_INT1_SEL<<10) | (MW_INTXBAR26_ADC2_INT2_SEL<<11) | (MW_INTXBAR26_ADC2_INT3_SEL<<12) | (MW_INTXBAR26_ADC2_INT4_SEL<<13) | (MW_INTXBAR26_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR26_EVTAGGR0_SEL<<15);
    group03_mask[26] = (MW_INTXBAR26_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR26_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR26_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR26_FSITX0_INT2N_SEL<<3);
    group04_mask[26] = (MW_INTXBAR26_SD0_ERR_SEL<<0) | (MW_INTXBAR26_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR26_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR26_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR26_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR26_SD1_ERR_SEL<<5) | (MW_INTXBAR26_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR26_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR26_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR26_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[26] = (MW_INTXBAR26_ECAP0_INT_SEL<<0) | (MW_INTXBAR26_ECAP1_INT_SEL<<1) | (MW_INTXBAR26_ECAP2_INT_SEL<<2) | (MW_INTXBAR26_ECAP3_INT_SEL<<3) | (MW_INTXBAR26_ECAP4_INT_SEL<<4) | (MW_INTXBAR26_ECAP5_INT_SEL<<5) | (MW_INTXBAR26_ECAP6_INT_SEL<<6) | (MW_INTXBAR26_ECAP7_INT_SEL<<7);
    group06_mask[26] = (MW_INTXBAR26_EQEP0_INT_SEL<<0) | (MW_INTXBAR26_EQEP1_INT_SEL<<1);
    group07_mask[26] = (MW_INTXBAR26_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR26_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR26_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR26_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR26_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR26_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR26_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR26_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR26_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR26_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR26_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR26_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR26_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR26_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR26_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR26_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR26_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR26_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[27] = (MW_INTXBAR27_EPWM0_INT_SEL<<0) | (MW_INTXBAR27_EPWM1_INT_SEL<<1) | (MW_INTXBAR27_EPWM2_INT_SEL<<2) | (MW_INTXBAR27_EPWM3_INT_SEL<<3) | (MW_INTXBAR27_EPWM4_INT_SEL<<4) | (MW_INTXBAR27_EPWM5_INT_SEL<<5) | (MW_INTXBAR27_EPWM6_INT_SEL<<6) | (MW_INTXBAR27_EPWM7_INT_SEL<<7) | (MW_INTXBAR27_EPWM8_INT_SEL<<8) | (MW_INTXBAR27_EPWM9_INT_SEL<<9);
    group01_mask[27] = (MW_INTXBAR27_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR27_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR27_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR27_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR27_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR27_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR27_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR27_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR27_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR27_EPWM9_TZINT_SEL<<9);
    group02_mask[27] = (MW_INTXBAR27_ADC0_INT1_SEL<<0) | (MW_INTXBAR27_ADC0_INT2_SEL<<1) | (MW_INTXBAR27_ADC0_INT3_SEL<<2) | (MW_INTXBAR27_ADC0_INT4_SEL<<3) | (MW_INTXBAR27_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR27_ADC1_INT1_SEL<<5) | (MW_INTXBAR27_ADC1_INT2_SEL<<6) | (MW_INTXBAR27_ADC1_INT3_SEL<<7) | (MW_INTXBAR27_ADC1_INT4_SEL<<8) | (MW_INTXBAR27_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR27_ADC2_INT1_SEL<<10) | (MW_INTXBAR27_ADC2_INT2_SEL<<11) | (MW_INTXBAR27_ADC2_INT3_SEL<<12) | (MW_INTXBAR27_ADC2_INT4_SEL<<13) | (MW_INTXBAR27_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR27_EVTAGGR0_SEL<<15);
    group03_mask[27] = (MW_INTXBAR27_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR27_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR27_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR27_FSITX0_INT2N_SEL<<3);
    group04_mask[27] = (MW_INTXBAR27_SD0_ERR_SEL<<0) | (MW_INTXBAR27_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR27_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR27_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR27_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR27_SD1_ERR_SEL<<5) | (MW_INTXBAR27_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR27_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR27_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR27_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[27] = (MW_INTXBAR27_ECAP0_INT_SEL<<0) | (MW_INTXBAR27_ECAP1_INT_SEL<<1) | (MW_INTXBAR27_ECAP2_INT_SEL<<2) | (MW_INTXBAR27_ECAP3_INT_SEL<<3) | (MW_INTXBAR27_ECAP4_INT_SEL<<4) | (MW_INTXBAR27_ECAP5_INT_SEL<<5) | (MW_INTXBAR27_ECAP6_INT_SEL<<6) | (MW_INTXBAR27_ECAP7_INT_SEL<<7);
    group06_mask[27] = (MW_INTXBAR27_EQEP0_INT_SEL<<0) | (MW_INTXBAR27_EQEP1_INT_SEL<<1);
    group07_mask[27] = (MW_INTXBAR27_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR27_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR27_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR27_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR27_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR27_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR27_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR27_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR27_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR27_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR27_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR27_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR27_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR27_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR27_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR27_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR27_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR27_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[28] = (MW_INTXBAR28_EPWM0_INT_SEL<<0) | (MW_INTXBAR28_EPWM1_INT_SEL<<1) | (MW_INTXBAR28_EPWM2_INT_SEL<<2) | (MW_INTXBAR28_EPWM3_INT_SEL<<3) | (MW_INTXBAR28_EPWM4_INT_SEL<<4) | (MW_INTXBAR28_EPWM5_INT_SEL<<5) | (MW_INTXBAR28_EPWM6_INT_SEL<<6) | (MW_INTXBAR28_EPWM7_INT_SEL<<7) | (MW_INTXBAR28_EPWM8_INT_SEL<<8) | (MW_INTXBAR28_EPWM9_INT_SEL<<9);
    group01_mask[28] = (MW_INTXBAR28_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR28_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR28_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR28_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR28_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR28_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR28_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR28_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR28_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR28_EPWM9_TZINT_SEL<<9);
    group02_mask[28] = (MW_INTXBAR28_ADC0_INT1_SEL<<0) | (MW_INTXBAR28_ADC0_INT2_SEL<<1) | (MW_INTXBAR28_ADC0_INT3_SEL<<2) | (MW_INTXBAR28_ADC0_INT4_SEL<<3) | (MW_INTXBAR28_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR28_ADC1_INT1_SEL<<5) | (MW_INTXBAR28_ADC1_INT2_SEL<<6) | (MW_INTXBAR28_ADC1_INT3_SEL<<7) | (MW_INTXBAR28_ADC1_INT4_SEL<<8) | (MW_INTXBAR28_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR28_ADC2_INT1_SEL<<10) | (MW_INTXBAR28_ADC2_INT2_SEL<<11) | (MW_INTXBAR28_ADC2_INT3_SEL<<12) | (MW_INTXBAR28_ADC2_INT4_SEL<<13) | (MW_INTXBAR28_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR28_EVTAGGR0_SEL<<15);
    group03_mask[28] = (MW_INTXBAR28_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR28_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR28_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR28_FSITX0_INT2N_SEL<<3);
    group04_mask[28] = (MW_INTXBAR28_SD0_ERR_SEL<<0) | (MW_INTXBAR28_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR28_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR28_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR28_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR28_SD1_ERR_SEL<<5) | (MW_INTXBAR28_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR28_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR28_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR28_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[28] = (MW_INTXBAR28_ECAP0_INT_SEL<<0) | (MW_INTXBAR28_ECAP1_INT_SEL<<1) | (MW_INTXBAR28_ECAP2_INT_SEL<<2) | (MW_INTXBAR28_ECAP3_INT_SEL<<3) | (MW_INTXBAR28_ECAP4_INT_SEL<<4) | (MW_INTXBAR28_ECAP5_INT_SEL<<5) | (MW_INTXBAR28_ECAP6_INT_SEL<<6) | (MW_INTXBAR28_ECAP7_INT_SEL<<7);
    group06_mask[28] = (MW_INTXBAR28_EQEP0_INT_SEL<<0) | (MW_INTXBAR28_EQEP1_INT_SEL<<1);
    group07_mask[28] = (MW_INTXBAR28_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR28_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR28_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR28_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR28_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR28_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR28_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR28_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR28_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR28_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR28_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR28_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR28_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR28_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR28_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR28_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR28_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR28_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[29] = (MW_INTXBAR29_EPWM0_INT_SEL<<0) | (MW_INTXBAR29_EPWM1_INT_SEL<<1) | (MW_INTXBAR29_EPWM2_INT_SEL<<2) | (MW_INTXBAR29_EPWM3_INT_SEL<<3) | (MW_INTXBAR29_EPWM4_INT_SEL<<4) | (MW_INTXBAR29_EPWM5_INT_SEL<<5) | (MW_INTXBAR29_EPWM6_INT_SEL<<6) | (MW_INTXBAR29_EPWM7_INT_SEL<<7) | (MW_INTXBAR29_EPWM8_INT_SEL<<8) | (MW_INTXBAR29_EPWM9_INT_SEL<<9);
    group01_mask[29] = (MW_INTXBAR29_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR29_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR29_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR29_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR29_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR29_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR29_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR29_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR29_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR29_EPWM9_TZINT_SEL<<9);
    group02_mask[29] = (MW_INTXBAR29_ADC0_INT1_SEL<<0) | (MW_INTXBAR29_ADC0_INT2_SEL<<1) | (MW_INTXBAR29_ADC0_INT3_SEL<<2) | (MW_INTXBAR29_ADC0_INT4_SEL<<3) | (MW_INTXBAR29_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR29_ADC1_INT1_SEL<<5) | (MW_INTXBAR29_ADC1_INT2_SEL<<6) | (MW_INTXBAR29_ADC1_INT3_SEL<<7) | (MW_INTXBAR29_ADC1_INT4_SEL<<8) | (MW_INTXBAR29_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR29_ADC2_INT1_SEL<<10) | (MW_INTXBAR29_ADC2_INT2_SEL<<11) | (MW_INTXBAR29_ADC2_INT3_SEL<<12) | (MW_INTXBAR29_ADC2_INT4_SEL<<13) | (MW_INTXBAR29_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR29_EVTAGGR0_SEL<<15);
    group03_mask[29] = (MW_INTXBAR29_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR29_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR29_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR29_FSITX0_INT2N_SEL<<3);
    group04_mask[29] = (MW_INTXBAR29_SD0_ERR_SEL<<0) | (MW_INTXBAR29_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR29_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR29_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR29_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR29_SD1_ERR_SEL<<5) | (MW_INTXBAR29_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR29_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR29_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR29_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[29] = (MW_INTXBAR29_ECAP0_INT_SEL<<0) | (MW_INTXBAR29_ECAP1_INT_SEL<<1) | (MW_INTXBAR29_ECAP2_INT_SEL<<2) | (MW_INTXBAR29_ECAP3_INT_SEL<<3) | (MW_INTXBAR29_ECAP4_INT_SEL<<4) | (MW_INTXBAR29_ECAP5_INT_SEL<<5) | (MW_INTXBAR29_ECAP6_INT_SEL<<6) | (MW_INTXBAR29_ECAP7_INT_SEL<<7);
    group06_mask[29] = (MW_INTXBAR29_EQEP0_INT_SEL<<0) | (MW_INTXBAR29_EQEP1_INT_SEL<<1);
    group07_mask[29] = (MW_INTXBAR29_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR29_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR29_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR29_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR29_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR29_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR29_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR29_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR29_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR29_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR29_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR29_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR29_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR29_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR29_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR29_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR29_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR29_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[30] = (MW_INTXBAR30_EPWM0_INT_SEL<<0) | (MW_INTXBAR30_EPWM1_INT_SEL<<1) | (MW_INTXBAR30_EPWM2_INT_SEL<<2) | (MW_INTXBAR30_EPWM3_INT_SEL<<3) | (MW_INTXBAR30_EPWM4_INT_SEL<<4) | (MW_INTXBAR30_EPWM5_INT_SEL<<5) | (MW_INTXBAR30_EPWM6_INT_SEL<<6) | (MW_INTXBAR30_EPWM7_INT_SEL<<7) | (MW_INTXBAR30_EPWM8_INT_SEL<<8) | (MW_INTXBAR30_EPWM9_INT_SEL<<9);
    group01_mask[30] = (MW_INTXBAR30_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR30_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR30_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR30_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR30_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR30_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR30_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR30_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR30_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR30_EPWM9_TZINT_SEL<<9);
    group02_mask[30] = (MW_INTXBAR30_ADC0_INT1_SEL<<0) | (MW_INTXBAR30_ADC0_INT2_SEL<<1) | (MW_INTXBAR30_ADC0_INT3_SEL<<2) | (MW_INTXBAR30_ADC0_INT4_SEL<<3) | (MW_INTXBAR30_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR30_ADC1_INT1_SEL<<5) | (MW_INTXBAR30_ADC1_INT2_SEL<<6) | (MW_INTXBAR30_ADC1_INT3_SEL<<7) | (MW_INTXBAR30_ADC1_INT4_SEL<<8) | (MW_INTXBAR30_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR30_ADC2_INT1_SEL<<10) | (MW_INTXBAR30_ADC2_INT2_SEL<<11) | (MW_INTXBAR30_ADC2_INT3_SEL<<12) | (MW_INTXBAR30_ADC2_INT4_SEL<<13) | (MW_INTXBAR30_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR30_EVTAGGR0_SEL<<15);
    group03_mask[30] = (MW_INTXBAR30_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR30_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR30_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR30_FSITX0_INT2N_SEL<<3);
    group04_mask[30] = (MW_INTXBAR30_SD0_ERR_SEL<<0) | (MW_INTXBAR30_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR30_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR30_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR30_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR30_SD1_ERR_SEL<<5) | (MW_INTXBAR30_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR30_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR30_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR30_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[30] = (MW_INTXBAR30_ECAP0_INT_SEL<<0) | (MW_INTXBAR30_ECAP1_INT_SEL<<1) | (MW_INTXBAR30_ECAP2_INT_SEL<<2) | (MW_INTXBAR30_ECAP3_INT_SEL<<3) | (MW_INTXBAR30_ECAP4_INT_SEL<<4) | (MW_INTXBAR30_ECAP5_INT_SEL<<5) | (MW_INTXBAR30_ECAP6_INT_SEL<<6) | (MW_INTXBAR30_ECAP7_INT_SEL<<7);
    group06_mask[30] = (MW_INTXBAR30_EQEP0_INT_SEL<<0) | (MW_INTXBAR30_EQEP1_INT_SEL<<1);
    group07_mask[30] = (MW_INTXBAR30_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR30_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR30_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR30_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR30_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR30_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR30_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR30_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR30_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR30_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR30_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR30_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR30_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR30_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR30_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR30_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR30_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR30_CMPSSA8_CTRIPH_INT_SEL);
    group00_mask[31] = (MW_INTXBAR31_EPWM0_INT_SEL<<0) | (MW_INTXBAR31_EPWM1_INT_SEL<<1) | (MW_INTXBAR31_EPWM2_INT_SEL<<2) | (MW_INTXBAR31_EPWM3_INT_SEL<<3) | (MW_INTXBAR31_EPWM4_INT_SEL<<4) | (MW_INTXBAR31_EPWM5_INT_SEL<<5) | (MW_INTXBAR31_EPWM6_INT_SEL<<6) | (MW_INTXBAR31_EPWM7_INT_SEL<<7) | (MW_INTXBAR31_EPWM8_INT_SEL<<8) | (MW_INTXBAR31_EPWM9_INT_SEL<<9);
    group01_mask[31] = (MW_INTXBAR31_EPWM0_TZINT_SEL<<0) | (MW_INTXBAR31_EPWM1_TZINT_SEL<<1) | (MW_INTXBAR31_EPWM2_TZINT_SEL<<2) | (MW_INTXBAR31_EPWM3_TZINT_SEL<<3) | (MW_INTXBAR31_EPWM4_TZINT_SEL<<4) | (MW_INTXBAR31_EPWM5_TZINT_SEL<<5) | (MW_INTXBAR31_EPWM6_TZINT_SEL<<6) | (MW_INTXBAR31_EPWM7_TZINT_SEL<<7) | (MW_INTXBAR31_EPWM8_TZINT_SEL<<8) | (MW_INTXBAR31_EPWM9_TZINT_SEL<<9);
    group02_mask[31] = (MW_INTXBAR31_ADC0_INT1_SEL<<0) | (MW_INTXBAR31_ADC0_INT2_SEL<<1) | (MW_INTXBAR31_ADC0_INT3_SEL<<2) | (MW_INTXBAR31_ADC0_INT4_SEL<<3) | (MW_INTXBAR31_ADC0_EVTINT_SEL<<4) | (MW_INTXBAR31_ADC1_INT1_SEL<<5) | (MW_INTXBAR31_ADC1_INT2_SEL<<6) | (MW_INTXBAR31_ADC1_INT3_SEL<<7) | (MW_INTXBAR31_ADC1_INT4_SEL<<8) | (MW_INTXBAR31_ADC1_EVTINT_SEL<<9) | (MW_INTXBAR31_ADC2_INT1_SEL<<10) | (MW_INTXBAR31_ADC2_INT2_SEL<<11) | (MW_INTXBAR31_ADC2_INT3_SEL<<12) | (MW_INTXBAR31_ADC2_INT4_SEL<<13) | (MW_INTXBAR31_ADC2_EVTINT_SEL<<14) | (MW_INTXBAR31_EVTAGGR0_SEL<<15);
    group03_mask[31] = (MW_INTXBAR31_FSIRX0_INT1N_SEL<<0) | (MW_INTXBAR31_FSIRX0_INT2N_SEL<<1) | (MW_INTXBAR31_FSITX0_INT1N_SEL<<2) | (MW_INTXBAR31_FSITX0_INT2N_SEL<<3);
    group04_mask[31] = (MW_INTXBAR31_SD0_ERR_SEL<<0) | (MW_INTXBAR31_SD0_FILT0_DRINT_SEL<<1) | (MW_INTXBAR31_SD0_FILT1_DRINT_SEL<<2) | (MW_INTXBAR31_SD0_FILT2_DRINT_SEL<<3) | (MW_INTXBAR31_SD0_FILT3_DRINT_SEL<<4) | (MW_INTXBAR31_SD1_ERR_SEL<<5) | (MW_INTXBAR31_SD1_FILT0_DRINT_SEL<<6) | (MW_INTXBAR31_SD1_FILT1_DRINT_SEL<<7) | (MW_INTXBAR31_SD1_FILT2_DRINT_SEL<<8) | (MW_INTXBAR31_SD1_FILT3_DRINT_SEL<<9);
    group05_mask[31] = (MW_INTXBAR31_ECAP0_INT_SEL<<0) | (MW_INTXBAR31_ECAP1_INT_SEL<<1) | (MW_INTXBAR31_ECAP2_INT_SEL<<2) | (MW_INTXBAR31_ECAP3_INT_SEL<<3) | (MW_INTXBAR31_ECAP4_INT_SEL<<4) | (MW_INTXBAR31_ECAP5_INT_SEL<<5) | (MW_INTXBAR31_ECAP6_INT_SEL<<6) | (MW_INTXBAR31_ECAP7_INT_SEL<<7);
    group06_mask[31] = (MW_INTXBAR31_EQEP0_INT_SEL<<0) | (MW_INTXBAR31_EQEP1_INT_SEL<<1);
    group07_mask[31] = (MW_INTXBAR31_CMPSSA0_CTRIPL_INT_SEL<<0) | (MW_INTXBAR31_CMPSSA0_CTRIPH_INT_SEL<<1) | (MW_INTXBAR31_CMPSSA1_CTRIPL_INT_SEL<<2) | (MW_INTXBAR31_CMPSSA1_CTRIPH_INT_SEL<<3) | (MW_INTXBAR31_CMPSSA2_CTRIPL_INT_SEL<<4) | (MW_INTXBAR31_CMPSSA2_CTRIPH_INT_SEL<<5) | (MW_INTXBAR31_CMPSSA3_CTRIPL_INT_SEL<<6) | (MW_INTXBAR31_CMPSSA3_CTRIPH_INT_SEL<<7) | (MW_INTXBAR31_CMPSSA4_CTRIPL_INT_SEL<<8) | (MW_INTXBAR31_CMPSSA4_CTRIPH_INT_SEL<<9) | (MW_INTXBAR31_CMPSSA5_CTRIPL_INT_SEL<<10) | (MW_INTXBAR31_CMPSSA5_CTRIPH_INT_SEL<<19) | (MW_INTXBAR31_CMPSSA6_CTRIPL_INT_SEL<<20) | (MW_INTXBAR31_CMPSSA6_CTRIPH_INT_SEL<<21) | (MW_INTXBAR31_CMPSSA7_CTRIPL_INT_SEL<<22) | (MW_INTXBAR31_CMPSSA7_CTRIPH_INT_SEL<<23) | (MW_INTXBAR31_CMPSSA8_CTRIPL_INT_SEL<<24) | (MW_INTXBAR31_CMPSSA8_CTRIPH_INT_SEL);                                                
                                                                        
    uint8_t out;

    for( out = 0 ; out < 32 ; out++ )
    {
        SOC_xbarSelectInterruptXBarInputSource_ext(CSL_CONTROLSS_INTXBAR_U_BASE, out, 
                                               group00_mask[out], group01_mask[out], 
                                               group02_mask[out], group03_mask[out], 
                                               group04_mask[out], group05_mask[out], 
                                               group06_mask[out], group07_mask[out], 0, 0);
        
    }
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
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNCICSS10_OUTPUT_SEL;
            break;
        case 15:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNCICSS11_OUTPUT_SEL;
            break;
        case 16:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNCICSS12_OUTPUT_SEL;
            break;
        case 17:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNCICSS13_OUTPUT_SEL;
            break;
        case 18:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNCICSS14_OUTPUT_SEL;
            break;
        case 19:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNCICSS15_OUTPUT_SEL;
            break;
        case 20:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNCICSS16_OUTPUT_SEL;
            break;
        case 21:
            output_val = MW_GPIO_INT_XBAR_SOC_TIMESYNCICSS17_OUTPUT_SEL;
            break;
        case 22:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE00_SEL;
            break;
        case 23:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE01_SEL;
            break;
        case 24:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE02_SEL;
            break;
        case 25:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE03_SEL;
            break;
        case 26:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE10_SEL;
            break;
        case 27:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE11_SEL;
            break;
        case 28:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE12_SEL;
            break;
        case 29:
            output_val = MW_GPIO_INT_XBAR_VIM_MODULE13_SEL;
            break;
        default:
            output_val = 0;
            break;
        }

        SOC_xbarSelectGpioIntrXbarInputSource(CSL_GPIO_INTR_XBAR_U_BASE, out, output_val);

       
    }
}
void Drivers_inputXbarOpen(void)
{
    uint32_t InputXBAR_mask[32];

    InputXBAR_mask[0] = MW_INPUT_XBAR0_SEL;
    InputXBAR_mask[1] = MW_INPUT_XBAR1_SEL;
    InputXBAR_mask[2] = MW_INPUT_XBAR2_SEL;
    InputXBAR_mask[3] = MW_INPUT_XBAR3_SEL;
    InputXBAR_mask[4] = MW_INPUT_XBAR4_SEL;
    InputXBAR_mask[5] = MW_INPUT_XBAR5_SEL;
    InputXBAR_mask[6] = MW_INPUT_XBAR6_SEL;
    InputXBAR_mask[7] = MW_INPUT_XBAR7_SEL;
    InputXBAR_mask[8] = MW_INPUT_XBAR8_SEL;
    InputXBAR_mask[9] = MW_INPUT_XBAR9_SEL;
    InputXBAR_mask[10] = MW_INPUT_XBAR10_SEL;
    InputXBAR_mask[11] = MW_INPUT_XBAR11_SEL;
    InputXBAR_mask[12] = MW_INPUT_XBAR12_SEL;
    InputXBAR_mask[13] = MW_INPUT_XBAR13_SEL;
    InputXBAR_mask[14] = MW_INPUT_XBAR14_SEL;
    InputXBAR_mask[15] = MW_INPUT_XBAR15_SEL;
    InputXBAR_mask[16] = MW_INPUT_XBAR16_SEL;
    InputXBAR_mask[17] = MW_INPUT_XBAR17_SEL;
    InputXBAR_mask[18] = MW_INPUT_XBAR18_SEL;
    InputXBAR_mask[19] = MW_INPUT_XBAR19_SEL;
    InputXBAR_mask[20] = MW_INPUT_XBAR20_SEL;
    InputXBAR_mask[21] = MW_INPUT_XBAR21_SEL;
    InputXBAR_mask[22] = MW_INPUT_XBAR22_SEL;
    InputXBAR_mask[23] = MW_INPUT_XBAR23_SEL;
    InputXBAR_mask[24] = MW_INPUT_XBAR24_SEL;
    InputXBAR_mask[25] = MW_INPUT_XBAR25_SEL;
    InputXBAR_mask[26] = MW_INPUT_XBAR26_SEL;
    InputXBAR_mask[27] = MW_INPUT_XBAR27_SEL;
    InputXBAR_mask[28] = MW_INPUT_XBAR28_SEL;
    InputXBAR_mask[29] = MW_INPUT_XBAR29_SEL;
    InputXBAR_mask[30] = MW_INPUT_XBAR30_SEL;
    InputXBAR_mask[31] = MW_INPUT_XBAR31_SEL;
  
    for(int i = 0; i < 32; i++)
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

    group0_epwmxbar[0] = (MW_EPWMXBAR0_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR0_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR0_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR0_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR0_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR0_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR0_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR0_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR0_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR0_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR0_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR0_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR0_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR0_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR0_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR0_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR0_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR0_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[0] = (MW_EPWMXBAR0_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR0_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR0_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR0_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR0_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR0_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR0_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR0_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR0_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR0_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR0_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR0_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR0_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR0_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR0_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR0_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR0_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR0_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR0_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR0_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR0_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR0_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR0_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR0_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[0] = (MW_EPWMXBAR0_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR0_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR0_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR0_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR0_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR0_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR0_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR0_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR0_GPIO0_SEL == 1 | MW_EPWMXBAR0_GPIO1_SEL == 1 | MW_EPWMXBAR0_GPIO2_SEL == 1 | MW_EPWMXBAR0_GPIO3_SEL == 1 | MW_EPWMXBAR0_GPIO4_SEL == 1 | MW_EPWMXBAR0_GPIO5_SEL == 1 | MW_EPWMXBAR0_GPIO6_SEL == 1 | MW_EPWMXBAR0_GPIO7_SEL == 1| MW_EPWMXBAR0_GPIO8_SEL == 1 | MW_EPWMXBAR0_GPIO9_SEL == 1 | MW_EPWMXBAR0_GPIO10_SEL == 1| MW_EPWMXBAR0_GPIO11_SEL == 1| MW_EPWMXBAR0_GPIO12_SEL == 1 | MW_EPWMXBAR0_GPIO13_SEL == 1 | MW_EPWMXBAR0_GPIO14_SEL == 1 | MW_EPWMXBAR0_GPIO15_SEL == 1 | MW_EPWMXBAR0_GPIO16_SEL == 1 | MW_EPWMXBAR0_GPIO17_SEL == 1 | MW_EPWMXBAR0_GPIO18_SEL == 1 | MW_EPWMXBAR0_GPIO19_SEL == 1 | MW_EPWMXBAR0_GPIO20_SEL == 1 | MW_EPWMXBAR0_GPIO21_SEL == 1 | MW_EPWMXBAR0_GPIO22_SEL == 1 | MW_EPWMXBAR0_GPIO23_SEL == 1 | MW_EPWMXBAR0_GPIO24_SEL == 1 | MW_EPWMXBAR0_GPIO25_SEL == 1 | MW_EPWMXBAR0_GPIO26_SEL == 1 | MW_EPWMXBAR0_GPIO27_SEL == 1 | MW_EPWMXBAR0_GPIO28_SEL == 1 | MW_EPWMXBAR0_GPIO29_SEL == 1 | MW_EPWMXBAR0_GPIO30_SEL == 1 | MW_EPWMXBAR0_GPIO31_SEL == 1 | MW_EPWMXBAR0_GPIO32_SEL == 1 | MW_EPWMXBAR0_GPIO33_SEL == 1 | MW_EPWMXBAR0_GPIO34_SEL == 1 | MW_EPWMXBAR0_GPIO35_SEL == 1 | MW_EPWMXBAR0_GPIO36_SEL == 1 | MW_EPWMXBAR0_GPIO37_SEL == 1 | MW_EPWMXBAR0_GPIO38_SEL == 1 | MW_EPWMXBAR0_GPIO39_SEL == 1 | MW_EPWMXBAR0_GPIO40_SEL == 1 | MW_EPWMXBAR0_GPIO41_SEL == 1 | MW_EPWMXBAR0_GPIO42_SEL == 1 | MW_EPWMXBAR0_GPIO43_SEL == 1 | MW_EPWMXBAR0_GPIO44_SEL == 1 | | MW_EPWMXBAR0_GPIO45_SEL == 1 | MW_EPWMXBAR0_GPIO46_SEL == 1 | MW_EPWMXBAR0_GPIO47_SEL == 1| MW_EPWMXBAR0_GPIO48_SEL == 1 | MW_EPWMXBAR0_GPIO49_SEL == 1 | MW_EPWMXBAR0_GPIO50_SEL == 1 | MW_EPWMXBAR0_GPIO51_SEL == 1 | MW_EPWMXBAR0_GPIO52_SEL == 1 | MW_EPWMXBAR0_GPIO53_SEL == 1 | MW_EPWMXBAR0_GPIO54_SEL == 1 | MW_EPWMXBAR0_GPIO55_SEL == 1 | MW_EPWMXBAR0_GPIO56_SEL == 1 | MW_EPWMXBAR0_GPIO57_SEL == 1| MW_EPWMXBAR0_GPIO58_SEL == 1 | MW_EPWMXBAR0_GPIO59_SEL == 1 | MW_EPWMXBAR0_GPIO60_SEL == 1 | MW_EPWMXBAR0_GPIO61_SEL == 1 | MW_EPWMXBAR0_GPIO62_SEL == 1 | MW_EPWMXBAR0_GPIO63_SEL == 1 | MW_EPWMXBAR0_GPIO64_SEL == 1 | | MW_EPWMXBAR0_GPIO59_SEL == 1 | MW_EPWMXBAR0_GPIO60_SEL == 1 | MW_EPWMXBAR0_GPIO61_SEL == 1 | MW_EPWMXBAR0_GPIO62_SEL == 1 | MW_EPWMXBAR0_GPIO63_SEL == 1 | MW_EPWMXBAR0_GPIO64_SEL == 1 | MW_EPWMXBAR0_GPIO65_SEL == 1 | MW_EPWMXBAR0_GPIO66_SEL == 1 | MW_EPWMXBAR0_GPIO67_SEL == 1 | MW_EPWMXBAR0_GPIO68_SEL == 1 | MW_EPWMXBAR0_GPIO69_SEL == 1 | MW_EPWMXBAR0_GPIO70_SEL == 1 | MW_EPWMXBAR0_GPIO71_SEL == 1 | MW_EPWMXBAR0_GPIO72_SEL == 1 | MW_EPWMXBAR0_GPIO73_SEL == 1 | MW_EPWMXBAR0_GPIO74_SEL == 1 | MW_EPWMXBAR0_GPIO75_SEL == 1 | MW_EPWMXBAR0_GPIO76_SEL == 1 | MW_EPWMXBAR0_GPIO77_SEL == 1 | MW_EPWMXBAR0_GPIO78_SEL == 1 | | MW_EPWMXBAR0_GPIO80_SEL == 1 | MW_EPWMXBAR0_GPIO81_SEL == 1 | MW_EPWMXBAR0_GPIO82_SEL == 1 | MW_EPWMXBAR0_GPIO83_SEL == 1 | MW_EPWMXBAR0_GPIO84_SEL == 1 | MW_EPWMXBAR0_GPIO85_SEL == 1 | MW_EPWMXBAR0_GPIO86_SEL == 1 | MW_EPWMXBAR0_GPIO87_SEL == 1 | MW_EPWMXBAR0_GPIO88_SEL == 1 | MW_EPWMXBAR0_GPIO89_SEL == 1 | MW_EPWMXBAR0_GPIO90_SEL == 1 | MW_EPWMXBAR0_GPIO91_SEL == 1 | MW_EPWMXBAR0_GPIO92_SEL == 1 | MW_EPWMXBAR0_GPIO93_SEL == 1 | MW_EPWMXBAR0_GPIO94_SEL == 1 | MW_EPWMXBAR0_GPIO95_SEL == 1 | MW_EPWMXBAR0_GPIO96_SEL == 1 | MW_EPWMXBAR0_GPIO97_SEL == 1 | MW_EPWMXBAR0_GPIO98_SEL == 1 | MW_EPWMXBAR0_GPIO99_SEL == 1 | MW_EPWMXBAR0_GPIO100_SEL == 1 | MW_EPWMXBAR0_GPIO101_SEL == 1 | MW_EPWMXBAR0_GPIO102_SEL == 1 | MW_EPWMXBAR0_GPIO103_SEL == 1 | MW_EPWMXBAR0_GPIO104_SEL == 1 | MW_EPWMXBAR0_GPIO105_SEL == 1 | MW_EPWMXBAR0_GPIO106_SEL == 1 | MW_EPWMXBAR0_GPIO107_SEL == 1 | | MW_EPWMXBAR0_GPIO108_SEL == 1 | MW_EPWMXBAR0_GPIO109_SEL == 1| MW_EPWMXBAR0_GPIO110_SEL == 1 | MW_EPWMXBAR0_GPIO111_SEL == 1 | MW_EPWMXBAR0_GPIO112_SEL == 1 | MW_EPWMXBAR0_GPIO113_SEL == 1 | MW_EPWMXBAR0_GPIO114_SEL == 1 | MW_EPWMXBAR0_GPIO115_SEL == 1 | MW_EPWMXBAR0_GPIO116_SEL == 1 | MW_EPWMXBAR0_GPIO117_SEL == 1 | MW_EPWMXBAR0_GPIO118_SEL == 1 | MW_EPWMXBAR0_GPIO119_SEL == 1 | MW_EPWMXBAR0_GPIO120_SEL == 1 | MW_EPWMXBAR0_GPIO121_SEL == 1 | MW_EPWMXBAR0_GPIO122_SEL == 1 | MW_EPWMXBAR0_GPIO123_SEL == 1 | MW_EPWMXBAR0_GPIO124_SEL == 1 | MW_EPWMXBAR0_GPIO125_SEL == 1 | MW_EPWMXBAR0_GPIO126_SEL == 1 | MW_EPWMXBAR0_GPIO127_SEL == 1 | MW_EPWMXBAR0_GPIO128_SEL == 1 | MW_EPWMXBAR0_GPIO129_SEL == 1 | MW_EPWMXBAR0_GPIO130_SEL == 1 | MW_EPWMXBAR0_GPIO131_SEL == 1 | MW_EPWMXBAR0_GPIO132_SEL == 1 | MW_EPWMXBAR0_GPIO133_SEL == 1 | MW_EPWMXBAR0_GPIO134_SEL == 1 | MW_EPWMXBAR0_GPIO135_SEL == 1 | MW_EPWMXBAR0_GPIO136_SEL == 1 | MW_EPWMXBAR0_GPIO137_SEL == 1 | MW_EPWMXBAR0_GPIO139_SEL == 1 | MW_EPWMXBAR0_GPIO140_SEL == 1 | MW_EPWMXBAR0_GPIO141_SEL == 1 | MW_EPWMXBAR0_GPIO142_SEL == 1 | MW_EPWMXBAR0_GPIO143_SEL == 1 | MW_EPWMXBAR0_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR0_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR0_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[0] = EPWM_XBAR_0;
    // else
        group3_epwmxbar[0] = 0;
    group4_epwmxbar[0] = (MW_EPWMXBAR0_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR0_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR0_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR0_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR0_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR0_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR0_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR0_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR0_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR0_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[0] = (MW_EPWMXBAR0_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR0_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR0_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR0_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR0_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR0_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR0_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR0_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR0_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR0_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[0] = (MW_EPWMXBAR0_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR0_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR0_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR0_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR0_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR0_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR0_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR0_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR0_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR0_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[0] = (MW_EPWMXBAR0_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR0_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR0_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR0_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR0_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR0_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR0_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR0_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR0_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR0_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR0_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR0_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR0_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR0_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[0] = (MW_EPWMXBAR0_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR0_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR0_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR0_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[1] = (MW_EPWMXBAR1_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR1_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR1_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR1_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR1_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR1_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR1_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR1_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR1_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR1_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR1_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR1_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR1_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR1_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR1_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR1_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR1_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR1_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[1] = (MW_EPWMXBAR1_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR1_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR1_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR1_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR1_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR1_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR1_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR1_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR1_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR1_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR1_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR1_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR1_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR1_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR1_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR1_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR1_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR1_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR1_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR1_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR1_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR1_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR1_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR1_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[1] = (MW_EPWMXBAR1_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR1_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR1_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR1_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR1_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR1_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR1_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR1_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR1_GPIO0_SEL == 1 | MW_EPWMXBAR1_GPIO1_SEL == 1 | MW_EPWMXBAR1_GPIO2_SEL == 1 | MW_EPWMXBAR1_GPIO3_SEL == 1 | MW_EPWMXBAR1_GPIO4_SEL == 1 | MW_EPWMXBAR1_GPIO5_SEL == 1 | MW_EPWMXBAR1_GPIO6_SEL == 1 | MW_EPWMXBAR1_GPIO7_SEL == 1| MW_EPWMXBAR1_GPIO8_SEL == 1 | MW_EPWMXBAR1_GPIO9_SEL == 1 | MW_EPWMXBAR1_GPIO10_SEL == 1| MW_EPWMXBAR1_GPIO11_SEL == 1| MW_EPWMXBAR1_GPIO12_SEL == 1 | MW_EPWMXBAR1_GPIO13_SEL == 1 | MW_EPWMXBAR1_GPIO14_SEL == 1 | MW_EPWMXBAR1_GPIO15_SEL == 1 | MW_EPWMXBAR1_GPIO16_SEL == 1 | MW_EPWMXBAR1_GPIO17_SEL == 1 | MW_EPWMXBAR1_GPIO18_SEL == 1 | MW_EPWMXBAR1_GPIO19_SEL == 1 | MW_EPWMXBAR1_GPIO20_SEL == 1 | MW_EPWMXBAR1_GPIO21_SEL == 1 | MW_EPWMXBAR1_GPIO22_SEL == 1 | MW_EPWMXBAR1_GPIO23_SEL == 1 | MW_EPWMXBAR1_GPIO24_SEL == 1 | MW_EPWMXBAR1_GPIO25_SEL == 1 | MW_EPWMXBAR1_GPIO26_SEL == 1 | MW_EPWMXBAR1_GPIO27_SEL == 1 | MW_EPWMXBAR1_GPIO28_SEL == 1 | MW_EPWMXBAR1_GPIO29_SEL == 1 | MW_EPWMXBAR1_GPIO30_SEL == 1 | MW_EPWMXBAR1_GPIO31_SEL == 1 | MW_EPWMXBAR1_GPIO32_SEL == 1 | MW_EPWMXBAR1_GPIO33_SEL == 1 | MW_EPWMXBAR1_GPIO34_SEL == 1 | MW_EPWMXBAR1_GPIO35_SEL == 1 | MW_EPWMXBAR1_GPIO36_SEL == 1 | MW_EPWMXBAR1_GPIO37_SEL == 1 | MW_EPWMXBAR1_GPIO38_SEL == 1 | MW_EPWMXBAR1_GPIO39_SEL == 1 | MW_EPWMXBAR1_GPIO40_SEL == 1 | MW_EPWMXBAR1_GPIO41_SEL == 1 | MW_EPWMXBAR1_GPIO42_SEL == 1 | MW_EPWMXBAR1_GPIO43_SEL == 1 | MW_EPWMXBAR1_GPIO44_SEL == 1 | | MW_EPWMXBAR1_GPIO45_SEL == 1 | MW_EPWMXBAR1_GPIO46_SEL == 1 | MW_EPWMXBAR1_GPIO47_SEL == 1| MW_EPWMXBAR1_GPIO48_SEL == 1 | MW_EPWMXBAR1_GPIO49_SEL == 1 | MW_EPWMXBAR1_GPIO50_SEL == 1 | MW_EPWMXBAR1_GPIO51_SEL == 1 | MW_EPWMXBAR1_GPIO52_SEL == 1 | MW_EPWMXBAR1_GPIO53_SEL == 1 | MW_EPWMXBAR1_GPIO54_SEL == 1 | MW_EPWMXBAR1_GPIO55_SEL == 1 | MW_EPWMXBAR1_GPIO56_SEL == 1 | MW_EPWMXBAR1_GPIO57_SEL == 1| MW_EPWMXBAR1_GPIO58_SEL == 1 | MW_EPWMXBAR1_GPIO59_SEL == 1 | MW_EPWMXBAR1_GPIO60_SEL == 1 | MW_EPWMXBAR1_GPIO61_SEL == 1 | MW_EPWMXBAR1_GPIO62_SEL == 1 | MW_EPWMXBAR1_GPIO63_SEL == 1 | MW_EPWMXBAR1_GPIO64_SEL == 1 | | MW_EPWMXBAR1_GPIO59_SEL == 1 | MW_EPWMXBAR1_GPIO60_SEL == 1 | MW_EPWMXBAR1_GPIO61_SEL == 1 | MW_EPWMXBAR1_GPIO62_SEL == 1 | MW_EPWMXBAR1_GPIO63_SEL == 1 | MW_EPWMXBAR1_GPIO64_SEL == 1 | MW_EPWMXBAR1_GPIO65_SEL == 1 | MW_EPWMXBAR1_GPIO66_SEL == 1 | MW_EPWMXBAR1_GPIO67_SEL == 1 | MW_EPWMXBAR1_GPIO68_SEL == 1 | MW_EPWMXBAR1_GPIO69_SEL == 1 | MW_EPWMXBAR1_GPIO70_SEL == 1 | MW_EPWMXBAR1_GPIO71_SEL == 1 | MW_EPWMXBAR1_GPIO72_SEL == 1 | MW_EPWMXBAR1_GPIO73_SEL == 1 | MW_EPWMXBAR1_GPIO74_SEL == 1 | MW_EPWMXBAR1_GPIO75_SEL == 1 | MW_EPWMXBAR1_GPIO76_SEL == 1 | MW_EPWMXBAR1_GPIO77_SEL == 1 | MW_EPWMXBAR1_GPIO78_SEL == 1 | | MW_EPWMXBAR1_GPIO80_SEL == 1 | MW_EPWMXBAR1_GPIO81_SEL == 1 | MW_EPWMXBAR1_GPIO82_SEL == 1 | MW_EPWMXBAR1_GPIO83_SEL == 1 | MW_EPWMXBAR1_GPIO84_SEL == 1 | MW_EPWMXBAR1_GPIO85_SEL == 1 | MW_EPWMXBAR1_GPIO86_SEL == 1 | MW_EPWMXBAR1_GPIO87_SEL == 1 | MW_EPWMXBAR1_GPIO88_SEL == 1 | MW_EPWMXBAR1_GPIO89_SEL == 1 | MW_EPWMXBAR1_GPIO90_SEL == 1 | MW_EPWMXBAR1_GPIO91_SEL == 1 | MW_EPWMXBAR1_GPIO92_SEL == 1 | MW_EPWMXBAR1_GPIO93_SEL == 1 | MW_EPWMXBAR1_GPIO94_SEL == 1 | MW_EPWMXBAR1_GPIO95_SEL == 1 | MW_EPWMXBAR1_GPIO96_SEL == 1 | MW_EPWMXBAR1_GPIO97_SEL == 1 | MW_EPWMXBAR1_GPIO98_SEL == 1 | MW_EPWMXBAR1_GPIO99_SEL == 1 | MW_EPWMXBAR1_GPIO100_SEL == 1 | MW_EPWMXBAR1_GPIO101_SEL == 1 | MW_EPWMXBAR1_GPIO102_SEL == 1 | MW_EPWMXBAR1_GPIO103_SEL == 1 | MW_EPWMXBAR1_GPIO104_SEL == 1 | MW_EPWMXBAR1_GPIO105_SEL == 1 | MW_EPWMXBAR1_GPIO106_SEL == 1 | MW_EPWMXBAR1_GPIO107_SEL == 1 | | MW_EPWMXBAR1_GPIO108_SEL == 1 | MW_EPWMXBAR1_GPIO109_SEL == 1| MW_EPWMXBAR1_GPIO110_SEL == 1 | MW_EPWMXBAR1_GPIO111_SEL == 1 | MW_EPWMXBAR1_GPIO112_SEL == 1 | MW_EPWMXBAR1_GPIO113_SEL == 1 | MW_EPWMXBAR1_GPIO114_SEL == 1 | MW_EPWMXBAR1_GPIO115_SEL == 1 | MW_EPWMXBAR1_GPIO116_SEL == 1 | MW_EPWMXBAR1_GPIO117_SEL == 1 | MW_EPWMXBAR1_GPIO118_SEL == 1 | MW_EPWMXBAR1_GPIO119_SEL == 1 | MW_EPWMXBAR1_GPIO120_SEL == 1 | MW_EPWMXBAR1_GPIO121_SEL == 1 | MW_EPWMXBAR1_GPIO122_SEL == 1 | MW_EPWMXBAR1_GPIO123_SEL == 1 | MW_EPWMXBAR1_GPIO124_SEL == 1 | MW_EPWMXBAR1_GPIO125_SEL == 1 | MW_EPWMXBAR1_GPIO126_SEL == 1 | MW_EPWMXBAR1_GPIO127_SEL == 1 | MW_EPWMXBAR1_GPIO128_SEL == 1 | MW_EPWMXBAR1_GPIO129_SEL == 1 | MW_EPWMXBAR1_GPIO130_SEL == 1 | MW_EPWMXBAR1_GPIO131_SEL == 1 | MW_EPWMXBAR1_GPIO132_SEL == 1 | MW_EPWMXBAR1_GPIO133_SEL == 1 | MW_EPWMXBAR1_GPIO134_SEL == 1 | MW_EPWMXBAR1_GPIO135_SEL == 1 | MW_EPWMXBAR1_GPIO136_SEL == 1 | MW_EPWMXBAR1_GPIO137_SEL == 1 | MW_EPWMXBAR1_GPIO139_SEL == 1 | MW_EPWMXBAR1_GPIO140_SEL == 1 | MW_EPWMXBAR1_GPIO141_SEL == 1 | MW_EPWMXBAR1_GPIO142_SEL == 1 | MW_EPWMXBAR1_GPIO143_SEL == 1 | MW_EPWMXBAR1_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR1_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR1_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[1] = EPWM_XBAR_1;
    // else
        group3_epwmxbar[1] = 0;
    group4_epwmxbar[1] = (MW_EPWMXBAR1_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR1_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR1_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR1_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR1_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR1_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR1_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR1_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR1_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR1_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[1] = (MW_EPWMXBAR1_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR1_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR1_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR1_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR1_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR1_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR1_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR1_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR1_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR1_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[1] = (MW_EPWMXBAR1_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR1_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR1_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR1_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR1_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR1_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR1_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR1_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR1_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR1_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[1] = (MW_EPWMXBAR1_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR1_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR1_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR1_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR1_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR1_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR1_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR1_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR1_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR1_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR1_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR1_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR1_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR1_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[1] = (MW_EPWMXBAR1_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR1_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR1_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR1_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[2] = (MW_EPWMXBAR2_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR2_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR2_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR2_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR2_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR2_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR2_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR2_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR2_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR2_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR2_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR2_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR2_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR2_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR2_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR2_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR2_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR2_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[2] = (MW_EPWMXBAR2_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR2_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR2_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR2_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR2_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR2_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR2_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR2_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR2_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR2_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR2_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR2_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR2_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR2_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR2_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR2_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR2_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR2_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR2_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR2_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR2_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR2_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR2_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR2_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[2] = (MW_EPWMXBAR2_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR2_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR2_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR2_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR2_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR2_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR2_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR2_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR2_GPIO0_SEL == 1 | MW_EPWMXBAR2_GPIO1_SEL == 1 | MW_EPWMXBAR2_GPIO2_SEL == 1 | MW_EPWMXBAR2_GPIO3_SEL == 1 | MW_EPWMXBAR2_GPIO4_SEL == 1 | MW_EPWMXBAR2_GPIO5_SEL == 1 | MW_EPWMXBAR2_GPIO6_SEL == 1 | MW_EPWMXBAR2_GPIO7_SEL == 1| MW_EPWMXBAR2_GPIO8_SEL == 1 | MW_EPWMXBAR2_GPIO9_SEL == 1 | MW_EPWMXBAR2_GPIO10_SEL == 1| MW_EPWMXBAR2_GPIO11_SEL == 1| MW_EPWMXBAR2_GPIO12_SEL == 1 | MW_EPWMXBAR2_GPIO13_SEL == 1 | MW_EPWMXBAR2_GPIO14_SEL == 1 | MW_EPWMXBAR2_GPIO15_SEL == 1 | MW_EPWMXBAR2_GPIO16_SEL == 1 | MW_EPWMXBAR2_GPIO17_SEL == 1 | MW_EPWMXBAR2_GPIO18_SEL == 1 | MW_EPWMXBAR2_GPIO19_SEL == 1 | MW_EPWMXBAR2_GPIO20_SEL == 1 | MW_EPWMXBAR2_GPIO21_SEL == 1 | MW_EPWMXBAR2_GPIO22_SEL == 1 | MW_EPWMXBAR2_GPIO23_SEL == 1 | MW_EPWMXBAR2_GPIO24_SEL == 1 | MW_EPWMXBAR2_GPIO25_SEL == 1 | MW_EPWMXBAR2_GPIO26_SEL == 1 | MW_EPWMXBAR2_GPIO27_SEL == 1 | MW_EPWMXBAR2_GPIO28_SEL == 1 | MW_EPWMXBAR2_GPIO29_SEL == 1 | MW_EPWMXBAR2_GPIO30_SEL == 1 | MW_EPWMXBAR2_GPIO31_SEL == 1 | MW_EPWMXBAR2_GPIO32_SEL == 1 | MW_EPWMXBAR2_GPIO33_SEL == 1 | MW_EPWMXBAR2_GPIO34_SEL == 1 | MW_EPWMXBAR2_GPIO35_SEL == 1 | MW_EPWMXBAR2_GPIO36_SEL == 1 | MW_EPWMXBAR2_GPIO37_SEL == 1 | MW_EPWMXBAR2_GPIO38_SEL == 1 | MW_EPWMXBAR2_GPIO39_SEL == 1 | MW_EPWMXBAR2_GPIO40_SEL == 1 | MW_EPWMXBAR2_GPIO41_SEL == 1 | MW_EPWMXBAR2_GPIO42_SEL == 1 | MW_EPWMXBAR2_GPIO43_SEL == 1 | MW_EPWMXBAR2_GPIO44_SEL == 1 | | MW_EPWMXBAR2_GPIO45_SEL == 1 | MW_EPWMXBAR2_GPIO46_SEL == 1 | MW_EPWMXBAR2_GPIO47_SEL == 1| MW_EPWMXBAR2_GPIO48_SEL == 1 | MW_EPWMXBAR2_GPIO49_SEL == 1 | MW_EPWMXBAR2_GPIO50_SEL == 1 | MW_EPWMXBAR2_GPIO51_SEL == 1 | MW_EPWMXBAR2_GPIO52_SEL == 1 | MW_EPWMXBAR2_GPIO53_SEL == 1 | MW_EPWMXBAR2_GPIO54_SEL == 1 | MW_EPWMXBAR2_GPIO55_SEL == 1 | MW_EPWMXBAR2_GPIO56_SEL == 1 | MW_EPWMXBAR2_GPIO57_SEL == 1| MW_EPWMXBAR2_GPIO58_SEL == 1 | MW_EPWMXBAR2_GPIO59_SEL == 1 | MW_EPWMXBAR2_GPIO60_SEL == 1 | MW_EPWMXBAR2_GPIO61_SEL == 1 | MW_EPWMXBAR2_GPIO62_SEL == 1 | MW_EPWMXBAR2_GPIO63_SEL == 1 | MW_EPWMXBAR2_GPIO64_SEL == 1 | | MW_EPWMXBAR2_GPIO59_SEL == 1 | MW_EPWMXBAR2_GPIO60_SEL == 1 | MW_EPWMXBAR2_GPIO61_SEL == 1 | MW_EPWMXBAR2_GPIO62_SEL == 1 | MW_EPWMXBAR2_GPIO63_SEL == 1 | MW_EPWMXBAR2_GPIO64_SEL == 1 | MW_EPWMXBAR2_GPIO65_SEL == 1 | MW_EPWMXBAR2_GPIO66_SEL == 1 | MW_EPWMXBAR2_GPIO67_SEL == 1 | MW_EPWMXBAR2_GPIO68_SEL == 1 | MW_EPWMXBAR2_GPIO69_SEL == 1 | MW_EPWMXBAR2_GPIO70_SEL == 1 | MW_EPWMXBAR2_GPIO71_SEL == 1 | MW_EPWMXBAR2_GPIO72_SEL == 1 | MW_EPWMXBAR2_GPIO73_SEL == 1 | MW_EPWMXBAR2_GPIO74_SEL == 1 | MW_EPWMXBAR2_GPIO75_SEL == 1 | MW_EPWMXBAR2_GPIO76_SEL == 1 | MW_EPWMXBAR2_GPIO77_SEL == 1 | MW_EPWMXBAR2_GPIO78_SEL == 1 | | MW_EPWMXBAR2_GPIO80_SEL == 1 | MW_EPWMXBAR2_GPIO81_SEL == 1 | MW_EPWMXBAR2_GPIO82_SEL == 1 | MW_EPWMXBAR2_GPIO83_SEL == 1 | MW_EPWMXBAR2_GPIO84_SEL == 1 | MW_EPWMXBAR2_GPIO85_SEL == 1 | MW_EPWMXBAR2_GPIO86_SEL == 1 | MW_EPWMXBAR2_GPIO87_SEL == 1 | MW_EPWMXBAR2_GPIO88_SEL == 1 | MW_EPWMXBAR2_GPIO89_SEL == 1 | MW_EPWMXBAR2_GPIO90_SEL == 1 | MW_EPWMXBAR2_GPIO91_SEL == 1 | MW_EPWMXBAR2_GPIO92_SEL == 1 | MW_EPWMXBAR2_GPIO93_SEL == 1 | MW_EPWMXBAR2_GPIO94_SEL == 1 | MW_EPWMXBAR2_GPIO95_SEL == 1 | MW_EPWMXBAR2_GPIO96_SEL == 1 | MW_EPWMXBAR2_GPIO97_SEL == 1 | MW_EPWMXBAR2_GPIO98_SEL == 1 | MW_EPWMXBAR2_GPIO99_SEL == 1 | MW_EPWMXBAR2_GPIO100_SEL == 1 | MW_EPWMXBAR2_GPIO101_SEL == 1 | MW_EPWMXBAR2_GPIO102_SEL == 1 | MW_EPWMXBAR2_GPIO103_SEL == 1 | MW_EPWMXBAR2_GPIO104_SEL == 1 | MW_EPWMXBAR2_GPIO105_SEL == 1 | MW_EPWMXBAR2_GPIO106_SEL == 1 | MW_EPWMXBAR2_GPIO107_SEL == 1 | | MW_EPWMXBAR2_GPIO108_SEL == 1 | MW_EPWMXBAR2_GPIO109_SEL == 1| MW_EPWMXBAR2_GPIO110_SEL == 1 | MW_EPWMXBAR2_GPIO111_SEL == 1 | MW_EPWMXBAR2_GPIO112_SEL == 1 | MW_EPWMXBAR2_GPIO113_SEL == 1 | MW_EPWMXBAR2_GPIO114_SEL == 1 | MW_EPWMXBAR2_GPIO115_SEL == 1 | MW_EPWMXBAR2_GPIO116_SEL == 1 | MW_EPWMXBAR2_GPIO117_SEL == 1 | MW_EPWMXBAR2_GPIO118_SEL == 1 | MW_EPWMXBAR2_GPIO119_SEL == 1 | MW_EPWMXBAR2_GPIO120_SEL == 1 | MW_EPWMXBAR2_GPIO121_SEL == 1 | MW_EPWMXBAR2_GPIO122_SEL == 1 | MW_EPWMXBAR2_GPIO123_SEL == 1 | MW_EPWMXBAR2_GPIO124_SEL == 1 | MW_EPWMXBAR2_GPIO125_SEL == 1 | MW_EPWMXBAR2_GPIO126_SEL == 1 | MW_EPWMXBAR2_GPIO127_SEL == 1 | MW_EPWMXBAR2_GPIO128_SEL == 1 | MW_EPWMXBAR2_GPIO129_SEL == 1 | MW_EPWMXBAR2_GPIO130_SEL == 1 | MW_EPWMXBAR2_GPIO131_SEL == 1 | MW_EPWMXBAR2_GPIO132_SEL == 1 | MW_EPWMXBAR2_GPIO133_SEL == 1 | MW_EPWMXBAR2_GPIO134_SEL == 1 | MW_EPWMXBAR2_GPIO135_SEL == 1 | MW_EPWMXBAR2_GPIO136_SEL == 1 | MW_EPWMXBAR2_GPIO137_SEL == 1 | MW_EPWMXBAR2_GPIO139_SEL == 1 | MW_EPWMXBAR2_GPIO140_SEL == 1 | MW_EPWMXBAR2_GPIO141_SEL == 1 | MW_EPWMXBAR2_GPIO142_SEL == 1 | MW_EPWMXBAR2_GPIO143_SEL == 1 | MW_EPWMXBAR2_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR2_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR2_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[2] = EPWM_XBAR_2;
    // else
        group3_epwmxbar[2] = 0;
    group4_epwmxbar[2] = (MW_EPWMXBAR2_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR2_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR2_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR2_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR2_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR2_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR2_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR2_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR2_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR2_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[2] = (MW_EPWMXBAR2_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR2_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR2_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR2_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR2_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR2_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR2_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR2_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR2_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR2_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[2] = (MW_EPWMXBAR2_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR2_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR2_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR2_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR2_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR2_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR2_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR2_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR2_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR2_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[2] = (MW_EPWMXBAR2_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR2_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR2_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR2_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR2_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR2_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR2_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR2_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR2_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR2_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR2_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR2_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR2_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR2_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[2] = (MW_EPWMXBAR2_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR2_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR2_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR2_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[3] = (MW_EPWMXBAR3_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR3_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR3_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR3_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR3_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR3_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR3_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR3_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR3_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR3_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR3_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR3_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR3_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR3_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR3_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR3_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR3_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR3_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[3] = (MW_EPWMXBAR3_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR3_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR3_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR3_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR3_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR3_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR3_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR3_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR3_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR3_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR3_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR3_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR3_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR3_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR3_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR3_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR3_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR3_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR3_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR3_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR3_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR3_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR3_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR3_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[3] = (MW_EPWMXBAR3_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR3_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR3_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR3_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR3_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR3_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR3_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR3_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR3_GPIO0_SEL == 1 | MW_EPWMXBAR3_GPIO1_SEL == 1 | MW_EPWMXBAR3_GPIO2_SEL == 1 | MW_EPWMXBAR3_GPIO3_SEL == 1 | MW_EPWMXBAR3_GPIO4_SEL == 1 | MW_EPWMXBAR3_GPIO5_SEL == 1 | MW_EPWMXBAR3_GPIO6_SEL == 1 | MW_EPWMXBAR3_GPIO7_SEL == 1| MW_EPWMXBAR3_GPIO8_SEL == 1 | MW_EPWMXBAR3_GPIO9_SEL == 1 | MW_EPWMXBAR3_GPIO10_SEL == 1| MW_EPWMXBAR3_GPIO11_SEL == 1| MW_EPWMXBAR3_GPIO12_SEL == 1 | MW_EPWMXBAR3_GPIO13_SEL == 1 | MW_EPWMXBAR3_GPIO14_SEL == 1 | MW_EPWMXBAR3_GPIO15_SEL == 1 | MW_EPWMXBAR3_GPIO16_SEL == 1 | MW_EPWMXBAR3_GPIO17_SEL == 1 | MW_EPWMXBAR3_GPIO18_SEL == 1 | MW_EPWMXBAR3_GPIO19_SEL == 1 | MW_EPWMXBAR3_GPIO20_SEL == 1 | MW_EPWMXBAR3_GPIO21_SEL == 1 | MW_EPWMXBAR3_GPIO22_SEL == 1 | MW_EPWMXBAR3_GPIO23_SEL == 1 | MW_EPWMXBAR3_GPIO24_SEL == 1 | MW_EPWMXBAR3_GPIO25_SEL == 1 | MW_EPWMXBAR3_GPIO26_SEL == 1 | MW_EPWMXBAR3_GPIO27_SEL == 1 | MW_EPWMXBAR3_GPIO28_SEL == 1 | MW_EPWMXBAR3_GPIO29_SEL == 1 | MW_EPWMXBAR3_GPIO30_SEL == 1 | MW_EPWMXBAR3_GPIO31_SEL == 1 | MW_EPWMXBAR3_GPIO32_SEL == 1 | MW_EPWMXBAR3_GPIO33_SEL == 1 | MW_EPWMXBAR3_GPIO34_SEL == 1 | MW_EPWMXBAR3_GPIO35_SEL == 1 | MW_EPWMXBAR3_GPIO36_SEL == 1 | MW_EPWMXBAR3_GPIO37_SEL == 1 | MW_EPWMXBAR3_GPIO38_SEL == 1 | MW_EPWMXBAR3_GPIO39_SEL == 1 | MW_EPWMXBAR3_GPIO40_SEL == 1 | MW_EPWMXBAR3_GPIO41_SEL == 1 | MW_EPWMXBAR3_GPIO42_SEL == 1 | MW_EPWMXBAR3_GPIO43_SEL == 1 | MW_EPWMXBAR3_GPIO44_SEL == 1 | | MW_EPWMXBAR3_GPIO45_SEL == 1 | MW_EPWMXBAR3_GPIO46_SEL == 1 | MW_EPWMXBAR3_GPIO47_SEL == 1| MW_EPWMXBAR3_GPIO48_SEL == 1 | MW_EPWMXBAR3_GPIO49_SEL == 1 | MW_EPWMXBAR3_GPIO50_SEL == 1 | MW_EPWMXBAR3_GPIO51_SEL == 1 | MW_EPWMXBAR3_GPIO52_SEL == 1 | MW_EPWMXBAR3_GPIO53_SEL == 1 | MW_EPWMXBAR3_GPIO54_SEL == 1 | MW_EPWMXBAR3_GPIO55_SEL == 1 | MW_EPWMXBAR3_GPIO56_SEL == 1 | MW_EPWMXBAR3_GPIO57_SEL == 1| MW_EPWMXBAR3_GPIO58_SEL == 1 | MW_EPWMXBAR3_GPIO59_SEL == 1 | MW_EPWMXBAR3_GPIO60_SEL == 1 | MW_EPWMXBAR3_GPIO61_SEL == 1 | MW_EPWMXBAR3_GPIO62_SEL == 1 | MW_EPWMXBAR3_GPIO63_SEL == 1 | MW_EPWMXBAR3_GPIO64_SEL == 1 | | MW_EPWMXBAR3_GPIO59_SEL == 1 | MW_EPWMXBAR3_GPIO60_SEL == 1 | MW_EPWMXBAR3_GPIO61_SEL == 1 | MW_EPWMXBAR3_GPIO62_SEL == 1 | MW_EPWMXBAR3_GPIO63_SEL == 1 | MW_EPWMXBAR3_GPIO64_SEL == 1 | MW_EPWMXBAR3_GPIO65_SEL == 1 | MW_EPWMXBAR3_GPIO66_SEL == 1 | MW_EPWMXBAR3_GPIO67_SEL == 1 | MW_EPWMXBAR3_GPIO68_SEL == 1 | MW_EPWMXBAR3_GPIO69_SEL == 1 | MW_EPWMXBAR3_GPIO70_SEL == 1 | MW_EPWMXBAR3_GPIO71_SEL == 1 | MW_EPWMXBAR3_GPIO72_SEL == 1 | MW_EPWMXBAR3_GPIO73_SEL == 1 | MW_EPWMXBAR3_GPIO74_SEL == 1 | MW_EPWMXBAR3_GPIO75_SEL == 1 | MW_EPWMXBAR3_GPIO76_SEL == 1 | MW_EPWMXBAR3_GPIO77_SEL == 1 | MW_EPWMXBAR3_GPIO78_SEL == 1 | | MW_EPWMXBAR3_GPIO80_SEL == 1 | MW_EPWMXBAR3_GPIO81_SEL == 1 | MW_EPWMXBAR3_GPIO82_SEL == 1 | MW_EPWMXBAR3_GPIO83_SEL == 1 | MW_EPWMXBAR3_GPIO84_SEL == 1 | MW_EPWMXBAR3_GPIO85_SEL == 1 | MW_EPWMXBAR3_GPIO86_SEL == 1 | MW_EPWMXBAR3_GPIO87_SEL == 1 | MW_EPWMXBAR3_GPIO88_SEL == 1 | MW_EPWMXBAR3_GPIO89_SEL == 1 | MW_EPWMXBAR3_GPIO90_SEL == 1 | MW_EPWMXBAR3_GPIO91_SEL == 1 | MW_EPWMXBAR3_GPIO92_SEL == 1 | MW_EPWMXBAR3_GPIO93_SEL == 1 | MW_EPWMXBAR3_GPIO94_SEL == 1 | MW_EPWMXBAR3_GPIO95_SEL == 1 | MW_EPWMXBAR3_GPIO96_SEL == 1 | MW_EPWMXBAR3_GPIO97_SEL == 1 | MW_EPWMXBAR3_GPIO98_SEL == 1 | MW_EPWMXBAR3_GPIO99_SEL == 1 | MW_EPWMXBAR3_GPIO100_SEL == 1 | MW_EPWMXBAR3_GPIO101_SEL == 1 | MW_EPWMXBAR3_GPIO102_SEL == 1 | MW_EPWMXBAR3_GPIO103_SEL == 1 | MW_EPWMXBAR3_GPIO104_SEL == 1 | MW_EPWMXBAR3_GPIO105_SEL == 1 | MW_EPWMXBAR3_GPIO106_SEL == 1 | MW_EPWMXBAR3_GPIO107_SEL == 1 | | MW_EPWMXBAR3_GPIO108_SEL == 1 | MW_EPWMXBAR3_GPIO109_SEL == 1| MW_EPWMXBAR3_GPIO110_SEL == 1 | MW_EPWMXBAR3_GPIO111_SEL == 1 | MW_EPWMXBAR3_GPIO112_SEL == 1 | MW_EPWMXBAR3_GPIO113_SEL == 1 | MW_EPWMXBAR3_GPIO114_SEL == 1 | MW_EPWMXBAR3_GPIO115_SEL == 1 | MW_EPWMXBAR3_GPIO116_SEL == 1 | MW_EPWMXBAR3_GPIO117_SEL == 1 | MW_EPWMXBAR3_GPIO118_SEL == 1 | MW_EPWMXBAR3_GPIO119_SEL == 1 | MW_EPWMXBAR3_GPIO120_SEL == 1 | MW_EPWMXBAR3_GPIO121_SEL == 1 | MW_EPWMXBAR3_GPIO122_SEL == 1 | MW_EPWMXBAR3_GPIO123_SEL == 1 | MW_EPWMXBAR3_GPIO124_SEL == 1 | MW_EPWMXBAR3_GPIO125_SEL == 1 | MW_EPWMXBAR3_GPIO126_SEL == 1 | MW_EPWMXBAR3_GPIO127_SEL == 1 | MW_EPWMXBAR3_GPIO128_SEL == 1 | MW_EPWMXBAR3_GPIO129_SEL == 1 | MW_EPWMXBAR3_GPIO130_SEL == 1 | MW_EPWMXBAR3_GPIO131_SEL == 1 | MW_EPWMXBAR3_GPIO132_SEL == 1 | MW_EPWMXBAR3_GPIO133_SEL == 1 | MW_EPWMXBAR3_GPIO134_SEL == 1 | MW_EPWMXBAR3_GPIO135_SEL == 1 | MW_EPWMXBAR3_GPIO136_SEL == 1 | MW_EPWMXBAR3_GPIO137_SEL == 1 | MW_EPWMXBAR3_GPIO139_SEL == 1 | MW_EPWMXBAR3_GPIO140_SEL == 1 | MW_EPWMXBAR3_GPIO141_SEL == 1 | MW_EPWMXBAR3_GPIO142_SEL == 1 | MW_EPWMXBAR3_GPIO143_SEL == 1 | MW_EPWMXBAR3_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR3_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR3_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[3] = EPWM_XBAR_3;
    // else
        group3_epwmxbar[3] = 0;
    group4_epwmxbar[3] = (MW_EPWMXBAR3_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR3_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR3_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR3_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR3_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR3_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR3_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR3_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR3_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR3_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[3] = (MW_EPWMXBAR3_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR3_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR3_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR3_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR3_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR3_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR3_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR3_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR3_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR3_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[3] = (MW_EPWMXBAR3_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR3_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR3_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR3_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR3_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR3_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR3_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR3_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR3_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR3_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[3] = (MW_EPWMXBAR3_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR3_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR3_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR3_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR3_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR3_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR3_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR3_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR3_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR3_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR3_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR3_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR3_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR3_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[3] = (MW_EPWMXBAR3_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR3_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR3_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR3_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[4] = (MW_EPWMXBAR4_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR4_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR4_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR4_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR4_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR4_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR4_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR4_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR4_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR4_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR4_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR4_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR4_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR4_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR4_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR4_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR4_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR4_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[4] = (MW_EPWMXBAR4_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR4_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR4_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR4_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR4_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR4_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR4_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR4_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR4_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR4_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR4_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR4_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR4_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR4_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR4_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR4_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR4_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR4_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR4_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR4_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR4_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR4_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR4_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR4_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[4] = (MW_EPWMXBAR4_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR4_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR4_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR4_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR4_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR4_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR4_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR4_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR4_GPIO0_SEL == 1 | MW_EPWMXBAR4_GPIO1_SEL == 1 | MW_EPWMXBAR4_GPIO2_SEL == 1 | MW_EPWMXBAR4_GPIO3_SEL == 1 | MW_EPWMXBAR4_GPIO4_SEL == 1 | MW_EPWMXBAR4_GPIO5_SEL == 1 | MW_EPWMXBAR4_GPIO6_SEL == 1 | MW_EPWMXBAR4_GPIO7_SEL == 1| MW_EPWMXBAR4_GPIO8_SEL == 1 | MW_EPWMXBAR4_GPIO9_SEL == 1 | MW_EPWMXBAR4_GPIO10_SEL == 1| MW_EPWMXBAR4_GPIO11_SEL == 1| MW_EPWMXBAR4_GPIO12_SEL == 1 | MW_EPWMXBAR4_GPIO13_SEL == 1 | MW_EPWMXBAR4_GPIO14_SEL == 1 | MW_EPWMXBAR4_GPIO15_SEL == 1 | MW_EPWMXBAR4_GPIO16_SEL == 1 | MW_EPWMXBAR4_GPIO17_SEL == 1 | MW_EPWMXBAR4_GPIO18_SEL == 1 | MW_EPWMXBAR4_GPIO19_SEL == 1 | MW_EPWMXBAR4_GPIO20_SEL == 1 | MW_EPWMXBAR4_GPIO21_SEL == 1 | MW_EPWMXBAR4_GPIO22_SEL == 1 | MW_EPWMXBAR4_GPIO23_SEL == 1 | MW_EPWMXBAR4_GPIO24_SEL == 1 | MW_EPWMXBAR4_GPIO25_SEL == 1 | MW_EPWMXBAR4_GPIO26_SEL == 1 | MW_EPWMXBAR4_GPIO27_SEL == 1 | MW_EPWMXBAR4_GPIO28_SEL == 1 | MW_EPWMXBAR4_GPIO29_SEL == 1 | MW_EPWMXBAR4_GPIO30_SEL == 1 | MW_EPWMXBAR4_GPIO31_SEL == 1 | MW_EPWMXBAR4_GPIO32_SEL == 1 | MW_EPWMXBAR4_GPIO33_SEL == 1 | MW_EPWMXBAR4_GPIO34_SEL == 1 | MW_EPWMXBAR4_GPIO35_SEL == 1 | MW_EPWMXBAR4_GPIO36_SEL == 1 | MW_EPWMXBAR4_GPIO37_SEL == 1 | MW_EPWMXBAR4_GPIO38_SEL == 1 | MW_EPWMXBAR4_GPIO39_SEL == 1 | MW_EPWMXBAR4_GPIO40_SEL == 1 | MW_EPWMXBAR4_GPIO41_SEL == 1 | MW_EPWMXBAR4_GPIO42_SEL == 1 | MW_EPWMXBAR4_GPIO43_SEL == 1 | MW_EPWMXBAR4_GPIO44_SEL == 1 | | MW_EPWMXBAR4_GPIO45_SEL == 1 | MW_EPWMXBAR4_GPIO46_SEL == 1 | MW_EPWMXBAR4_GPIO47_SEL == 1| MW_EPWMXBAR4_GPIO48_SEL == 1 | MW_EPWMXBAR4_GPIO49_SEL == 1 | MW_EPWMXBAR4_GPIO50_SEL == 1 | MW_EPWMXBAR4_GPIO51_SEL == 1 | MW_EPWMXBAR4_GPIO52_SEL == 1 | MW_EPWMXBAR4_GPIO53_SEL == 1 | MW_EPWMXBAR4_GPIO54_SEL == 1 | MW_EPWMXBAR4_GPIO55_SEL == 1 | MW_EPWMXBAR4_GPIO56_SEL == 1 | MW_EPWMXBAR4_GPIO57_SEL == 1| MW_EPWMXBAR4_GPIO58_SEL == 1 | MW_EPWMXBAR4_GPIO59_SEL == 1 | MW_EPWMXBAR4_GPIO60_SEL == 1 | MW_EPWMXBAR4_GPIO61_SEL == 1 | MW_EPWMXBAR4_GPIO62_SEL == 1 | MW_EPWMXBAR4_GPIO63_SEL == 1 | MW_EPWMXBAR4_GPIO64_SEL == 1 | | MW_EPWMXBAR4_GPIO59_SEL == 1 | MW_EPWMXBAR4_GPIO60_SEL == 1 | MW_EPWMXBAR4_GPIO61_SEL == 1 | MW_EPWMXBAR4_GPIO62_SEL == 1 | MW_EPWMXBAR4_GPIO63_SEL == 1 | MW_EPWMXBAR4_GPIO64_SEL == 1 | MW_EPWMXBAR4_GPIO65_SEL == 1 | MW_EPWMXBAR4_GPIO66_SEL == 1 | MW_EPWMXBAR4_GPIO67_SEL == 1 | MW_EPWMXBAR4_GPIO68_SEL == 1 | MW_EPWMXBAR4_GPIO69_SEL == 1 | MW_EPWMXBAR4_GPIO70_SEL == 1 | MW_EPWMXBAR4_GPIO71_SEL == 1 | MW_EPWMXBAR4_GPIO72_SEL == 1 | MW_EPWMXBAR4_GPIO73_SEL == 1 | MW_EPWMXBAR4_GPIO74_SEL == 1 | MW_EPWMXBAR4_GPIO75_SEL == 1 | MW_EPWMXBAR4_GPIO76_SEL == 1 | MW_EPWMXBAR4_GPIO77_SEL == 1 | MW_EPWMXBAR4_GPIO78_SEL == 1 | | MW_EPWMXBAR4_GPIO80_SEL == 1 | MW_EPWMXBAR4_GPIO81_SEL == 1 | MW_EPWMXBAR4_GPIO82_SEL == 1 | MW_EPWMXBAR4_GPIO83_SEL == 1 | MW_EPWMXBAR4_GPIO84_SEL == 1 | MW_EPWMXBAR4_GPIO85_SEL == 1 | MW_EPWMXBAR4_GPIO86_SEL == 1 | MW_EPWMXBAR4_GPIO87_SEL == 1 | MW_EPWMXBAR4_GPIO88_SEL == 1 | MW_EPWMXBAR4_GPIO89_SEL == 1 | MW_EPWMXBAR4_GPIO90_SEL == 1 | MW_EPWMXBAR4_GPIO91_SEL == 1 | MW_EPWMXBAR4_GPIO92_SEL == 1 | MW_EPWMXBAR4_GPIO93_SEL == 1 | MW_EPWMXBAR4_GPIO94_SEL == 1 | MW_EPWMXBAR4_GPIO95_SEL == 1 | MW_EPWMXBAR4_GPIO96_SEL == 1 | MW_EPWMXBAR4_GPIO97_SEL == 1 | MW_EPWMXBAR4_GPIO98_SEL == 1 | MW_EPWMXBAR4_GPIO99_SEL == 1 | MW_EPWMXBAR4_GPIO100_SEL == 1 | MW_EPWMXBAR4_GPIO101_SEL == 1 | MW_EPWMXBAR4_GPIO102_SEL == 1 | MW_EPWMXBAR4_GPIO103_SEL == 1 | MW_EPWMXBAR4_GPIO104_SEL == 1 | MW_EPWMXBAR4_GPIO105_SEL == 1 | MW_EPWMXBAR4_GPIO106_SEL == 1 | MW_EPWMXBAR4_GPIO107_SEL == 1 | | MW_EPWMXBAR4_GPIO108_SEL == 1 | MW_EPWMXBAR4_GPIO109_SEL == 1| MW_EPWMXBAR4_GPIO110_SEL == 1 | MW_EPWMXBAR4_GPIO111_SEL == 1 | MW_EPWMXBAR4_GPIO112_SEL == 1 | MW_EPWMXBAR4_GPIO113_SEL == 1 | MW_EPWMXBAR4_GPIO114_SEL == 1 | MW_EPWMXBAR4_GPIO115_SEL == 1 | MW_EPWMXBAR4_GPIO116_SEL == 1 | MW_EPWMXBAR4_GPIO117_SEL == 1 | MW_EPWMXBAR4_GPIO118_SEL == 1 | MW_EPWMXBAR4_GPIO119_SEL == 1 | MW_EPWMXBAR4_GPIO120_SEL == 1 | MW_EPWMXBAR4_GPIO121_SEL == 1 | MW_EPWMXBAR4_GPIO122_SEL == 1 | MW_EPWMXBAR4_GPIO123_SEL == 1 | MW_EPWMXBAR4_GPIO124_SEL == 1 | MW_EPWMXBAR4_GPIO125_SEL == 1 | MW_EPWMXBAR4_GPIO126_SEL == 1 | MW_EPWMXBAR4_GPIO127_SEL == 1 | MW_EPWMXBAR4_GPIO128_SEL == 1 | MW_EPWMXBAR4_GPIO129_SEL == 1 | MW_EPWMXBAR4_GPIO130_SEL == 1 | MW_EPWMXBAR4_GPIO131_SEL == 1 | MW_EPWMXBAR4_GPIO132_SEL == 1 | MW_EPWMXBAR4_GPIO133_SEL == 1 | MW_EPWMXBAR4_GPIO134_SEL == 1 | MW_EPWMXBAR4_GPIO135_SEL == 1 | MW_EPWMXBAR4_GPIO136_SEL == 1 | MW_EPWMXBAR4_GPIO137_SEL == 1 | MW_EPWMXBAR4_GPIO139_SEL == 1 | MW_EPWMXBAR4_GPIO140_SEL == 1 | MW_EPWMXBAR4_GPIO141_SEL == 1 | MW_EPWMXBAR4_GPIO142_SEL == 1 | MW_EPWMXBAR4_GPIO143_SEL == 1 | MW_EPWMXBAR4_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR4_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR4_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[4] = EPWM_XBAR_4;
    // else
        group3_epwmxbar[4] = 0;
    group4_epwmxbar[4] = (MW_EPWMXBAR4_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR4_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR4_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR4_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR4_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR4_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR4_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR4_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR4_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR4_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[4] = (MW_EPWMXBAR4_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR4_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR4_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR4_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR4_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR4_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR4_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR4_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR4_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR4_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[4] = (MW_EPWMXBAR4_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR4_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR4_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR4_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR4_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR4_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR4_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR4_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR4_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR4_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[4] = (MW_EPWMXBAR4_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR4_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR4_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR4_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR4_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR4_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR4_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR4_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR4_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR4_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR4_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR4_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR4_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR4_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[4] = (MW_EPWMXBAR4_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR4_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR4_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR4_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[5] = (MW_EPWMXBAR5_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR5_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR5_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR5_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR5_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR5_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR5_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR5_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR5_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR5_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR5_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR5_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR5_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR5_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR5_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR5_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR5_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR5_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[5] = (MW_EPWMXBAR5_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR5_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR5_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR5_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR5_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR5_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR5_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR5_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR5_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR5_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR5_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR5_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR5_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR5_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR5_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR5_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR5_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR5_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR5_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR5_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR5_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR5_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR5_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR5_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[5] = (MW_EPWMXBAR5_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR5_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR5_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR5_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR5_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR5_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR5_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR5_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR5_GPIO0_SEL == 1 | MW_EPWMXBAR5_GPIO1_SEL == 1 | MW_EPWMXBAR5_GPIO2_SEL == 1 | MW_EPWMXBAR5_GPIO3_SEL == 1 | MW_EPWMXBAR5_GPIO4_SEL == 1 | MW_EPWMXBAR5_GPIO5_SEL == 1 | MW_EPWMXBAR5_GPIO6_SEL == 1 | MW_EPWMXBAR5_GPIO7_SEL == 1| MW_EPWMXBAR5_GPIO8_SEL == 1 | MW_EPWMXBAR5_GPIO9_SEL == 1 | MW_EPWMXBAR5_GPIO10_SEL == 1| MW_EPWMXBAR5_GPIO11_SEL == 1| MW_EPWMXBAR5_GPIO12_SEL == 1 | MW_EPWMXBAR5_GPIO13_SEL == 1 | MW_EPWMXBAR5_GPIO14_SEL == 1 | MW_EPWMXBAR5_GPIO15_SEL == 1 | MW_EPWMXBAR5_GPIO16_SEL == 1 | MW_EPWMXBAR5_GPIO17_SEL == 1 | MW_EPWMXBAR5_GPIO18_SEL == 1 | MW_EPWMXBAR5_GPIO19_SEL == 1 | MW_EPWMXBAR5_GPIO20_SEL == 1 | MW_EPWMXBAR5_GPIO21_SEL == 1 | MW_EPWMXBAR5_GPIO22_SEL == 1 | MW_EPWMXBAR5_GPIO23_SEL == 1 | MW_EPWMXBAR5_GPIO24_SEL == 1 | MW_EPWMXBAR5_GPIO25_SEL == 1 | MW_EPWMXBAR5_GPIO26_SEL == 1 | MW_EPWMXBAR5_GPIO27_SEL == 1 | MW_EPWMXBAR5_GPIO28_SEL == 1 | MW_EPWMXBAR5_GPIO29_SEL == 1 | MW_EPWMXBAR5_GPIO30_SEL == 1 | MW_EPWMXBAR5_GPIO31_SEL == 1 | MW_EPWMXBAR5_GPIO32_SEL == 1 | MW_EPWMXBAR5_GPIO33_SEL == 1 | MW_EPWMXBAR5_GPIO34_SEL == 1 | MW_EPWMXBAR5_GPIO35_SEL == 1 | MW_EPWMXBAR5_GPIO36_SEL == 1 | MW_EPWMXBAR5_GPIO37_SEL == 1 | MW_EPWMXBAR5_GPIO38_SEL == 1 | MW_EPWMXBAR5_GPIO39_SEL == 1 | MW_EPWMXBAR5_GPIO40_SEL == 1 | MW_EPWMXBAR5_GPIO41_SEL == 1 | MW_EPWMXBAR5_GPIO42_SEL == 1 | MW_EPWMXBAR5_GPIO43_SEL == 1 | MW_EPWMXBAR5_GPIO44_SEL == 1 | | MW_EPWMXBAR5_GPIO45_SEL == 1 | MW_EPWMXBAR5_GPIO46_SEL == 1 | MW_EPWMXBAR5_GPIO47_SEL == 1| MW_EPWMXBAR5_GPIO48_SEL == 1 | MW_EPWMXBAR5_GPIO49_SEL == 1 | MW_EPWMXBAR5_GPIO50_SEL == 1 | MW_EPWMXBAR5_GPIO51_SEL == 1 | MW_EPWMXBAR5_GPIO52_SEL == 1 | MW_EPWMXBAR5_GPIO53_SEL == 1 | MW_EPWMXBAR5_GPIO54_SEL == 1 | MW_EPWMXBAR5_GPIO55_SEL == 1 | MW_EPWMXBAR5_GPIO56_SEL == 1 | MW_EPWMXBAR5_GPIO57_SEL == 1| MW_EPWMXBAR5_GPIO58_SEL == 1 | MW_EPWMXBAR5_GPIO59_SEL == 1 | MW_EPWMXBAR5_GPIO60_SEL == 1 | MW_EPWMXBAR5_GPIO61_SEL == 1 | MW_EPWMXBAR5_GPIO62_SEL == 1 | MW_EPWMXBAR5_GPIO63_SEL == 1 | MW_EPWMXBAR5_GPIO64_SEL == 1 | | MW_EPWMXBAR5_GPIO59_SEL == 1 | MW_EPWMXBAR5_GPIO60_SEL == 1 | MW_EPWMXBAR5_GPIO61_SEL == 1 | MW_EPWMXBAR5_GPIO62_SEL == 1 | MW_EPWMXBAR5_GPIO63_SEL == 1 | MW_EPWMXBAR5_GPIO64_SEL == 1 | MW_EPWMXBAR5_GPIO65_SEL == 1 | MW_EPWMXBAR5_GPIO66_SEL == 1 | MW_EPWMXBAR5_GPIO67_SEL == 1 | MW_EPWMXBAR5_GPIO68_SEL == 1 | MW_EPWMXBAR5_GPIO69_SEL == 1 | MW_EPWMXBAR5_GPIO70_SEL == 1 | MW_EPWMXBAR5_GPIO71_SEL == 1 | MW_EPWMXBAR5_GPIO72_SEL == 1 | MW_EPWMXBAR5_GPIO73_SEL == 1 | MW_EPWMXBAR5_GPIO74_SEL == 1 | MW_EPWMXBAR5_GPIO75_SEL == 1 | MW_EPWMXBAR5_GPIO76_SEL == 1 | MW_EPWMXBAR5_GPIO77_SEL == 1 | MW_EPWMXBAR5_GPIO78_SEL == 1 | | MW_EPWMXBAR5_GPIO80_SEL == 1 | MW_EPWMXBAR5_GPIO81_SEL == 1 | MW_EPWMXBAR5_GPIO82_SEL == 1 | MW_EPWMXBAR5_GPIO83_SEL == 1 | MW_EPWMXBAR5_GPIO84_SEL == 1 | MW_EPWMXBAR5_GPIO85_SEL == 1 | MW_EPWMXBAR5_GPIO86_SEL == 1 | MW_EPWMXBAR5_GPIO87_SEL == 1 | MW_EPWMXBAR5_GPIO88_SEL == 1 | MW_EPWMXBAR5_GPIO89_SEL == 1 | MW_EPWMXBAR5_GPIO90_SEL == 1 | MW_EPWMXBAR5_GPIO91_SEL == 1 | MW_EPWMXBAR5_GPIO92_SEL == 1 | MW_EPWMXBAR5_GPIO93_SEL == 1 | MW_EPWMXBAR5_GPIO94_SEL == 1 | MW_EPWMXBAR5_GPIO95_SEL == 1 | MW_EPWMXBAR5_GPIO96_SEL == 1 | MW_EPWMXBAR5_GPIO97_SEL == 1 | MW_EPWMXBAR5_GPIO98_SEL == 1 | MW_EPWMXBAR5_GPIO99_SEL == 1 | MW_EPWMXBAR5_GPIO100_SEL == 1 | MW_EPWMXBAR5_GPIO101_SEL == 1 | MW_EPWMXBAR5_GPIO102_SEL == 1 | MW_EPWMXBAR5_GPIO103_SEL == 1 | MW_EPWMXBAR5_GPIO104_SEL == 1 | MW_EPWMXBAR5_GPIO105_SEL == 1 | MW_EPWMXBAR5_GPIO106_SEL == 1 | MW_EPWMXBAR5_GPIO107_SEL == 1 | | MW_EPWMXBAR5_GPIO108_SEL == 1 | MW_EPWMXBAR5_GPIO109_SEL == 1| MW_EPWMXBAR5_GPIO110_SEL == 1 | MW_EPWMXBAR5_GPIO111_SEL == 1 | MW_EPWMXBAR5_GPIO112_SEL == 1 | MW_EPWMXBAR5_GPIO113_SEL == 1 | MW_EPWMXBAR5_GPIO114_SEL == 1 | MW_EPWMXBAR5_GPIO115_SEL == 1 | MW_EPWMXBAR5_GPIO116_SEL == 1 | MW_EPWMXBAR5_GPIO117_SEL == 1 | MW_EPWMXBAR5_GPIO118_SEL == 1 | MW_EPWMXBAR5_GPIO119_SEL == 1 | MW_EPWMXBAR5_GPIO120_SEL == 1 | MW_EPWMXBAR5_GPIO121_SEL == 1 | MW_EPWMXBAR5_GPIO122_SEL == 1 | MW_EPWMXBAR5_GPIO123_SEL == 1 | MW_EPWMXBAR5_GPIO124_SEL == 1 | MW_EPWMXBAR5_GPIO125_SEL == 1 | MW_EPWMXBAR5_GPIO126_SEL == 1 | MW_EPWMXBAR5_GPIO127_SEL == 1 | MW_EPWMXBAR5_GPIO128_SEL == 1 | MW_EPWMXBAR5_GPIO129_SEL == 1 | MW_EPWMXBAR5_GPIO130_SEL == 1 | MW_EPWMXBAR5_GPIO131_SEL == 1 | MW_EPWMXBAR5_GPIO132_SEL == 1 | MW_EPWMXBAR5_GPIO133_SEL == 1 | MW_EPWMXBAR5_GPIO134_SEL == 1 | MW_EPWMXBAR5_GPIO135_SEL == 1 | MW_EPWMXBAR5_GPIO136_SEL == 1 | MW_EPWMXBAR5_GPIO137_SEL == 1 | MW_EPWMXBAR5_GPIO139_SEL == 1 | MW_EPWMXBAR5_GPIO140_SEL == 1 | MW_EPWMXBAR5_GPIO141_SEL == 1 | MW_EPWMXBAR5_GPIO142_SEL == 1 | MW_EPWMXBAR5_GPIO143_SEL == 1 | MW_EPWMXBAR5_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR5_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR5_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[5] = EPWM_XBAR_5;
    // else
        group3_epwmxbar[5] = 0;
    group4_epwmxbar[5] = (MW_EPWMXBAR5_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR5_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR5_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR5_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR5_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR5_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR5_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR5_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR5_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR5_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[5] = (MW_EPWMXBAR5_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR5_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR5_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR5_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR5_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR5_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR5_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR5_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR5_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR5_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[5] = (MW_EPWMXBAR5_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR5_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR5_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR5_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR5_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR5_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR5_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR5_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR5_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR5_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[5] = (MW_EPWMXBAR5_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR5_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR5_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR5_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR5_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR5_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR5_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR5_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR5_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR5_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR5_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR5_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR5_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR5_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[5] = (MW_EPWMXBAR5_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR5_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR5_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR5_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[6] = (MW_EPWMXBAR6_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR6_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR6_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR6_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR6_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR6_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR6_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR6_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR6_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR6_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR6_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR6_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR6_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR6_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR6_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR6_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR6_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR6_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[6] = (MW_EPWMXBAR6_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR6_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR6_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR6_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR6_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR6_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR6_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR6_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR6_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR6_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR6_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR6_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR6_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR6_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR6_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR6_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR6_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR6_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR6_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR6_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR6_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR6_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR6_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR6_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[6] = (MW_EPWMXBAR6_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR6_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR6_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR6_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR6_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR6_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR6_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR6_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR6_GPIO0_SEL == 1 | MW_EPWMXBAR6_GPIO1_SEL == 1 | MW_EPWMXBAR6_GPIO2_SEL == 1 | MW_EPWMXBAR6_GPIO3_SEL == 1 | MW_EPWMXBAR6_GPIO4_SEL == 1 | MW_EPWMXBAR6_GPIO5_SEL == 1 | MW_EPWMXBAR6_GPIO6_SEL == 1 | MW_EPWMXBAR6_GPIO7_SEL == 1| MW_EPWMXBAR6_GPIO8_SEL == 1 | MW_EPWMXBAR6_GPIO9_SEL == 1 | MW_EPWMXBAR6_GPIO10_SEL == 1| MW_EPWMXBAR6_GPIO11_SEL == 1| MW_EPWMXBAR6_GPIO12_SEL == 1 | MW_EPWMXBAR6_GPIO13_SEL == 1 | MW_EPWMXBAR6_GPIO14_SEL == 1 | MW_EPWMXBAR6_GPIO15_SEL == 1 | MW_EPWMXBAR6_GPIO16_SEL == 1 | MW_EPWMXBAR6_GPIO17_SEL == 1 | MW_EPWMXBAR6_GPIO18_SEL == 1 | MW_EPWMXBAR6_GPIO19_SEL == 1 | MW_EPWMXBAR6_GPIO20_SEL == 1 | MW_EPWMXBAR6_GPIO21_SEL == 1 | MW_EPWMXBAR6_GPIO22_SEL == 1 | MW_EPWMXBAR6_GPIO23_SEL == 1 | MW_EPWMXBAR6_GPIO24_SEL == 1 | MW_EPWMXBAR6_GPIO25_SEL == 1 | MW_EPWMXBAR6_GPIO26_SEL == 1 | MW_EPWMXBAR6_GPIO27_SEL == 1 | MW_EPWMXBAR6_GPIO28_SEL == 1 | MW_EPWMXBAR6_GPIO29_SEL == 1 | MW_EPWMXBAR6_GPIO30_SEL == 1 | MW_EPWMXBAR6_GPIO31_SEL == 1 | MW_EPWMXBAR6_GPIO32_SEL == 1 | MW_EPWMXBAR6_GPIO33_SEL == 1 | MW_EPWMXBAR6_GPIO34_SEL == 1 | MW_EPWMXBAR6_GPIO35_SEL == 1 | MW_EPWMXBAR6_GPIO36_SEL == 1 | MW_EPWMXBAR6_GPIO37_SEL == 1 | MW_EPWMXBAR6_GPIO38_SEL == 1 | MW_EPWMXBAR6_GPIO39_SEL == 1 | MW_EPWMXBAR6_GPIO40_SEL == 1 | MW_EPWMXBAR6_GPIO41_SEL == 1 | MW_EPWMXBAR6_GPIO42_SEL == 1 | MW_EPWMXBAR6_GPIO43_SEL == 1 | MW_EPWMXBAR6_GPIO44_SEL == 1 | | MW_EPWMXBAR6_GPIO45_SEL == 1 | MW_EPWMXBAR6_GPIO46_SEL == 1 | MW_EPWMXBAR6_GPIO47_SEL == 1| MW_EPWMXBAR6_GPIO48_SEL == 1 | MW_EPWMXBAR6_GPIO49_SEL == 1 | MW_EPWMXBAR6_GPIO50_SEL == 1 | MW_EPWMXBAR6_GPIO51_SEL == 1 | MW_EPWMXBAR6_GPIO52_SEL == 1 | MW_EPWMXBAR6_GPIO53_SEL == 1 | MW_EPWMXBAR6_GPIO54_SEL == 1 | MW_EPWMXBAR6_GPIO55_SEL == 1 | MW_EPWMXBAR6_GPIO56_SEL == 1 | MW_EPWMXBAR6_GPIO57_SEL == 1| MW_EPWMXBAR6_GPIO58_SEL == 1 | MW_EPWMXBAR6_GPIO59_SEL == 1 | MW_EPWMXBAR6_GPIO60_SEL == 1 | MW_EPWMXBAR6_GPIO61_SEL == 1 | MW_EPWMXBAR6_GPIO62_SEL == 1 | MW_EPWMXBAR6_GPIO63_SEL == 1 | MW_EPWMXBAR6_GPIO64_SEL == 1 | | MW_EPWMXBAR6_GPIO59_SEL == 1 | MW_EPWMXBAR6_GPIO60_SEL == 1 | MW_EPWMXBAR6_GPIO61_SEL == 1 | MW_EPWMXBAR6_GPIO62_SEL == 1 | MW_EPWMXBAR6_GPIO63_SEL == 1 | MW_EPWMXBAR6_GPIO64_SEL == 1 | MW_EPWMXBAR6_GPIO65_SEL == 1 | MW_EPWMXBAR6_GPIO66_SEL == 1 | MW_EPWMXBAR6_GPIO67_SEL == 1 | MW_EPWMXBAR6_GPIO68_SEL == 1 | MW_EPWMXBAR6_GPIO69_SEL == 1 | MW_EPWMXBAR6_GPIO70_SEL == 1 | MW_EPWMXBAR6_GPIO71_SEL == 1 | MW_EPWMXBAR6_GPIO72_SEL == 1 | MW_EPWMXBAR6_GPIO73_SEL == 1 | MW_EPWMXBAR6_GPIO74_SEL == 1 | MW_EPWMXBAR6_GPIO75_SEL == 1 | MW_EPWMXBAR6_GPIO76_SEL == 1 | MW_EPWMXBAR6_GPIO77_SEL == 1 | MW_EPWMXBAR6_GPIO78_SEL == 1 | | MW_EPWMXBAR6_GPIO80_SEL == 1 | MW_EPWMXBAR6_GPIO81_SEL == 1 | MW_EPWMXBAR6_GPIO82_SEL == 1 | MW_EPWMXBAR6_GPIO83_SEL == 1 | MW_EPWMXBAR6_GPIO84_SEL == 1 | MW_EPWMXBAR6_GPIO85_SEL == 1 | MW_EPWMXBAR6_GPIO86_SEL == 1 | MW_EPWMXBAR6_GPIO87_SEL == 1 | MW_EPWMXBAR6_GPIO88_SEL == 1 | MW_EPWMXBAR6_GPIO89_SEL == 1 | MW_EPWMXBAR6_GPIO90_SEL == 1 | MW_EPWMXBAR6_GPIO91_SEL == 1 | MW_EPWMXBAR6_GPIO92_SEL == 1 | MW_EPWMXBAR6_GPIO93_SEL == 1 | MW_EPWMXBAR6_GPIO94_SEL == 1 | MW_EPWMXBAR6_GPIO95_SEL == 1 | MW_EPWMXBAR6_GPIO96_SEL == 1 | MW_EPWMXBAR6_GPIO97_SEL == 1 | MW_EPWMXBAR6_GPIO98_SEL == 1 | MW_EPWMXBAR6_GPIO99_SEL == 1 | MW_EPWMXBAR6_GPIO100_SEL == 1 | MW_EPWMXBAR6_GPIO101_SEL == 1 | MW_EPWMXBAR6_GPIO102_SEL == 1 | MW_EPWMXBAR6_GPIO103_SEL == 1 | MW_EPWMXBAR6_GPIO104_SEL == 1 | MW_EPWMXBAR6_GPIO105_SEL == 1 | MW_EPWMXBAR6_GPIO106_SEL == 1 | MW_EPWMXBAR6_GPIO107_SEL == 1 | | MW_EPWMXBAR6_GPIO108_SEL == 1 | MW_EPWMXBAR6_GPIO109_SEL == 1| MW_EPWMXBAR6_GPIO110_SEL == 1 | MW_EPWMXBAR6_GPIO111_SEL == 1 | MW_EPWMXBAR6_GPIO112_SEL == 1 | MW_EPWMXBAR6_GPIO113_SEL == 1 | MW_EPWMXBAR6_GPIO114_SEL == 1 | MW_EPWMXBAR6_GPIO115_SEL == 1 | MW_EPWMXBAR6_GPIO116_SEL == 1 | MW_EPWMXBAR6_GPIO117_SEL == 1 | MW_EPWMXBAR6_GPIO118_SEL == 1 | MW_EPWMXBAR6_GPIO119_SEL == 1 | MW_EPWMXBAR6_GPIO120_SEL == 1 | MW_EPWMXBAR6_GPIO121_SEL == 1 | MW_EPWMXBAR6_GPIO122_SEL == 1 | MW_EPWMXBAR6_GPIO123_SEL == 1 | MW_EPWMXBAR6_GPIO124_SEL == 1 | MW_EPWMXBAR6_GPIO125_SEL == 1 | MW_EPWMXBAR6_GPIO126_SEL == 1 | MW_EPWMXBAR6_GPIO127_SEL == 1 | MW_EPWMXBAR6_GPIO128_SEL == 1 | MW_EPWMXBAR6_GPIO129_SEL == 1 | MW_EPWMXBAR6_GPIO130_SEL == 1 | MW_EPWMXBAR6_GPIO131_SEL == 1 | MW_EPWMXBAR6_GPIO132_SEL == 1 | MW_EPWMXBAR6_GPIO133_SEL == 1 | MW_EPWMXBAR6_GPIO134_SEL == 1 | MW_EPWMXBAR6_GPIO135_SEL == 1 | MW_EPWMXBAR6_GPIO136_SEL == 1 | MW_EPWMXBAR6_GPIO137_SEL == 1 | MW_EPWMXBAR6_GPIO139_SEL == 1 | MW_EPWMXBAR6_GPIO140_SEL == 1 | MW_EPWMXBAR6_GPIO141_SEL == 1 | MW_EPWMXBAR6_GPIO142_SEL == 1 | MW_EPWMXBAR6_GPIO143_SEL == 1 | MW_EPWMXBAR6_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR6_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR6_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[6] = EPWM_XBAR_6;
    // else
        group3_epwmxbar[6] = 0;
    group4_epwmxbar[6] = (MW_EPWMXBAR6_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR6_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR6_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR6_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR6_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR6_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR6_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR6_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR6_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR6_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[6] = (MW_EPWMXBAR6_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR6_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR6_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR6_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR6_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR6_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR6_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR6_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR6_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR6_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[6] = (MW_EPWMXBAR6_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR6_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR6_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR6_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR6_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR6_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR6_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR6_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR6_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR6_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[6] = (MW_EPWMXBAR6_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR6_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR6_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR6_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR6_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR6_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR6_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR6_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR6_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR6_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR6_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR6_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR6_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR6_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[6] = (MW_EPWMXBAR6_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR6_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR6_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR6_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[7] = (MW_EPWMXBAR7_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR7_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR7_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR7_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR7_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR7_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR7_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR7_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR7_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR7_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR7_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR7_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR7_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR7_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR7_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR7_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR7_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR7_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[7] = (MW_EPWMXBAR7_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR7_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR7_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR7_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR7_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR7_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR7_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR7_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR7_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR7_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR7_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR7_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR7_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR7_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR7_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR7_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR7_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR7_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR7_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR7_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR7_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR7_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR7_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR7_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[7] = (MW_EPWMXBAR7_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR7_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR7_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR7_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR7_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR7_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR7_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR7_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR7_GPIO0_SEL == 1 | MW_EPWMXBAR7_GPIO1_SEL == 1 | MW_EPWMXBAR7_GPIO2_SEL == 1 | MW_EPWMXBAR7_GPIO3_SEL == 1 | MW_EPWMXBAR7_GPIO4_SEL == 1 | MW_EPWMXBAR7_GPIO5_SEL == 1 | MW_EPWMXBAR7_GPIO6_SEL == 1 | MW_EPWMXBAR7_GPIO7_SEL == 1| MW_EPWMXBAR7_GPIO8_SEL == 1 | MW_EPWMXBAR7_GPIO9_SEL == 1 | MW_EPWMXBAR7_GPIO10_SEL == 1| MW_EPWMXBAR7_GPIO11_SEL == 1| MW_EPWMXBAR7_GPIO12_SEL == 1 | MW_EPWMXBAR7_GPIO13_SEL == 1 | MW_EPWMXBAR7_GPIO14_SEL == 1 | MW_EPWMXBAR7_GPIO15_SEL == 1 | MW_EPWMXBAR7_GPIO16_SEL == 1 | MW_EPWMXBAR7_GPIO17_SEL == 1 | MW_EPWMXBAR7_GPIO18_SEL == 1 | MW_EPWMXBAR7_GPIO19_SEL == 1 | MW_EPWMXBAR7_GPIO20_SEL == 1 | MW_EPWMXBAR7_GPIO21_SEL == 1 | MW_EPWMXBAR7_GPIO22_SEL == 1 | MW_EPWMXBAR7_GPIO23_SEL == 1 | MW_EPWMXBAR7_GPIO24_SEL == 1 | MW_EPWMXBAR7_GPIO25_SEL == 1 | MW_EPWMXBAR7_GPIO26_SEL == 1 | MW_EPWMXBAR7_GPIO27_SEL == 1 | MW_EPWMXBAR7_GPIO28_SEL == 1 | MW_EPWMXBAR7_GPIO29_SEL == 1 | MW_EPWMXBAR7_GPIO30_SEL == 1 | MW_EPWMXBAR7_GPIO31_SEL == 1 | MW_EPWMXBAR7_GPIO32_SEL == 1 | MW_EPWMXBAR7_GPIO33_SEL == 1 | MW_EPWMXBAR7_GPIO34_SEL == 1 | MW_EPWMXBAR7_GPIO35_SEL == 1 | MW_EPWMXBAR7_GPIO36_SEL == 1 | MW_EPWMXBAR7_GPIO37_SEL == 1 | MW_EPWMXBAR7_GPIO38_SEL == 1 | MW_EPWMXBAR7_GPIO39_SEL == 1 | MW_EPWMXBAR7_GPIO40_SEL == 1 | MW_EPWMXBAR7_GPIO41_SEL == 1 | MW_EPWMXBAR7_GPIO42_SEL == 1 | MW_EPWMXBAR7_GPIO43_SEL == 1 | MW_EPWMXBAR7_GPIO44_SEL == 1 | | MW_EPWMXBAR7_GPIO45_SEL == 1 | MW_EPWMXBAR7_GPIO46_SEL == 1 | MW_EPWMXBAR7_GPIO47_SEL == 1| MW_EPWMXBAR7_GPIO48_SEL == 1 | MW_EPWMXBAR7_GPIO49_SEL == 1 | MW_EPWMXBAR7_GPIO50_SEL == 1 | MW_EPWMXBAR7_GPIO51_SEL == 1 | MW_EPWMXBAR7_GPIO52_SEL == 1 | MW_EPWMXBAR7_GPIO53_SEL == 1 | MW_EPWMXBAR7_GPIO54_SEL == 1 | MW_EPWMXBAR7_GPIO55_SEL == 1 | MW_EPWMXBAR7_GPIO56_SEL == 1 | MW_EPWMXBAR7_GPIO57_SEL == 1| MW_EPWMXBAR7_GPIO58_SEL == 1 | MW_EPWMXBAR7_GPIO59_SEL == 1 | MW_EPWMXBAR7_GPIO60_SEL == 1 | MW_EPWMXBAR7_GPIO61_SEL == 1 | MW_EPWMXBAR7_GPIO62_SEL == 1 | MW_EPWMXBAR7_GPIO63_SEL == 1 | MW_EPWMXBAR7_GPIO64_SEL == 1 | | MW_EPWMXBAR7_GPIO59_SEL == 1 | MW_EPWMXBAR7_GPIO60_SEL == 1 | MW_EPWMXBAR7_GPIO61_SEL == 1 | MW_EPWMXBAR7_GPIO62_SEL == 1 | MW_EPWMXBAR7_GPIO63_SEL == 1 | MW_EPWMXBAR7_GPIO64_SEL == 1 | MW_EPWMXBAR7_GPIO65_SEL == 1 | MW_EPWMXBAR7_GPIO66_SEL == 1 | MW_EPWMXBAR7_GPIO67_SEL == 1 | MW_EPWMXBAR7_GPIO68_SEL == 1 | MW_EPWMXBAR7_GPIO69_SEL == 1 | MW_EPWMXBAR7_GPIO70_SEL == 1 | MW_EPWMXBAR7_GPIO71_SEL == 1 | MW_EPWMXBAR7_GPIO72_SEL == 1 | MW_EPWMXBAR7_GPIO73_SEL == 1 | MW_EPWMXBAR7_GPIO74_SEL == 1 | MW_EPWMXBAR7_GPIO75_SEL == 1 | MW_EPWMXBAR7_GPIO76_SEL == 1 | MW_EPWMXBAR7_GPIO77_SEL == 1 | MW_EPWMXBAR7_GPIO78_SEL == 1 | | MW_EPWMXBAR7_GPIO80_SEL == 1 | MW_EPWMXBAR7_GPIO81_SEL == 1 | MW_EPWMXBAR7_GPIO82_SEL == 1 | MW_EPWMXBAR7_GPIO83_SEL == 1 | MW_EPWMXBAR7_GPIO84_SEL == 1 | MW_EPWMXBAR7_GPIO85_SEL == 1 | MW_EPWMXBAR7_GPIO86_SEL == 1 | MW_EPWMXBAR7_GPIO87_SEL == 1 | MW_EPWMXBAR7_GPIO88_SEL == 1 | MW_EPWMXBAR7_GPIO89_SEL == 1 | MW_EPWMXBAR7_GPIO90_SEL == 1 | MW_EPWMXBAR7_GPIO91_SEL == 1 | MW_EPWMXBAR7_GPIO92_SEL == 1 | MW_EPWMXBAR7_GPIO93_SEL == 1 | MW_EPWMXBAR7_GPIO94_SEL == 1 | MW_EPWMXBAR7_GPIO95_SEL == 1 | MW_EPWMXBAR7_GPIO96_SEL == 1 | MW_EPWMXBAR7_GPIO97_SEL == 1 | MW_EPWMXBAR7_GPIO98_SEL == 1 | MW_EPWMXBAR7_GPIO99_SEL == 1 | MW_EPWMXBAR7_GPIO100_SEL == 1 | MW_EPWMXBAR7_GPIO101_SEL == 1 | MW_EPWMXBAR7_GPIO102_SEL == 1 | MW_EPWMXBAR7_GPIO103_SEL == 1 | MW_EPWMXBAR7_GPIO104_SEL == 1 | MW_EPWMXBAR7_GPIO105_SEL == 1 | MW_EPWMXBAR7_GPIO106_SEL == 1 | MW_EPWMXBAR7_GPIO107_SEL == 1 | | MW_EPWMXBAR7_GPIO108_SEL == 1 | MW_EPWMXBAR7_GPIO109_SEL == 1| MW_EPWMXBAR7_GPIO110_SEL == 1 | MW_EPWMXBAR7_GPIO111_SEL == 1 | MW_EPWMXBAR7_GPIO112_SEL == 1 | MW_EPWMXBAR7_GPIO113_SEL == 1 | MW_EPWMXBAR7_GPIO114_SEL == 1 | MW_EPWMXBAR7_GPIO115_SEL == 1 | MW_EPWMXBAR7_GPIO116_SEL == 1 | MW_EPWMXBAR7_GPIO117_SEL == 1 | MW_EPWMXBAR7_GPIO118_SEL == 1 | MW_EPWMXBAR7_GPIO119_SEL == 1 | MW_EPWMXBAR7_GPIO120_SEL == 1 | MW_EPWMXBAR7_GPIO121_SEL == 1 | MW_EPWMXBAR7_GPIO122_SEL == 1 | MW_EPWMXBAR7_GPIO123_SEL == 1 | MW_EPWMXBAR7_GPIO124_SEL == 1 | MW_EPWMXBAR7_GPIO125_SEL == 1 | MW_EPWMXBAR7_GPIO126_SEL == 1 | MW_EPWMXBAR7_GPIO127_SEL == 1 | MW_EPWMXBAR7_GPIO128_SEL == 1 | MW_EPWMXBAR7_GPIO129_SEL == 1 | MW_EPWMXBAR7_GPIO130_SEL == 1 | MW_EPWMXBAR7_GPIO131_SEL == 1 | MW_EPWMXBAR7_GPIO132_SEL == 1 | MW_EPWMXBAR7_GPIO133_SEL == 1 | MW_EPWMXBAR7_GPIO134_SEL == 1 | MW_EPWMXBAR7_GPIO135_SEL == 1 | MW_EPWMXBAR7_GPIO136_SEL == 1 | MW_EPWMXBAR7_GPIO137_SEL == 1 | MW_EPWMXBAR7_GPIO139_SEL == 1 | MW_EPWMXBAR7_GPIO140_SEL == 1 | MW_EPWMXBAR7_GPIO141_SEL == 1 | MW_EPWMXBAR7_GPIO142_SEL == 1 | MW_EPWMXBAR7_GPIO143_SEL == 1 | MW_EPWMXBAR7_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR7_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR7_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[7] = EPWM_XBAR_7;
    // else
        group3_epwmxbar[7] = 0;
    group4_epwmxbar[7] = (MW_EPWMXBAR7_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR7_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR7_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR7_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR7_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR7_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR7_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR7_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR7_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR7_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[7] = (MW_EPWMXBAR7_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR7_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR7_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR7_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR7_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR7_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR7_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR7_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR7_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR7_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[7] = (MW_EPWMXBAR7_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR7_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR7_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR7_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR7_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR7_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR7_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR7_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR7_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR7_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[7] = (MW_EPWMXBAR7_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR7_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR7_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR7_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR7_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR7_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR7_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR7_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR7_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR7_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR7_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR7_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR7_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR7_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[7] = (MW_EPWMXBAR7_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR7_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR7_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR7_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[8] = (MW_EPWMXBAR8_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR8_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR8_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR8_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR8_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR8_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR8_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR8_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR8_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR8_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR8_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR8_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR8_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR8_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR8_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR8_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR8_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR8_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[8] = (MW_EPWMXBAR8_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR8_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR8_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR8_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR8_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR8_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR8_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR8_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR8_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR8_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR8_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR8_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR8_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR8_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR8_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR8_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR8_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR8_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR8_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR8_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR8_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR8_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR8_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR8_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[8] = (MW_EPWMXBAR8_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR8_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR8_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR8_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR8_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR8_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR8_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR8_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR8_GPIO0_SEL == 1 | MW_EPWMXBAR8_GPIO1_SEL == 1 | MW_EPWMXBAR8_GPIO2_SEL == 1 | MW_EPWMXBAR8_GPIO3_SEL == 1 | MW_EPWMXBAR8_GPIO4_SEL == 1 | MW_EPWMXBAR8_GPIO5_SEL == 1 | MW_EPWMXBAR8_GPIO6_SEL == 1 | MW_EPWMXBAR8_GPIO7_SEL == 1| MW_EPWMXBAR8_GPIO8_SEL == 1 | MW_EPWMXBAR8_GPIO9_SEL == 1 | MW_EPWMXBAR8_GPIO10_SEL == 1| MW_EPWMXBAR8_GPIO11_SEL == 1| MW_EPWMXBAR8_GPIO12_SEL == 1 | MW_EPWMXBAR8_GPIO13_SEL == 1 | MW_EPWMXBAR8_GPIO14_SEL == 1 | MW_EPWMXBAR8_GPIO15_SEL == 1 | MW_EPWMXBAR8_GPIO16_SEL == 1 | MW_EPWMXBAR8_GPIO17_SEL == 1 | MW_EPWMXBAR8_GPIO18_SEL == 1 | MW_EPWMXBAR8_GPIO19_SEL == 1 | MW_EPWMXBAR8_GPIO20_SEL == 1 | MW_EPWMXBAR8_GPIO21_SEL == 1 | MW_EPWMXBAR8_GPIO22_SEL == 1 | MW_EPWMXBAR8_GPIO23_SEL == 1 | MW_EPWMXBAR8_GPIO24_SEL == 1 | MW_EPWMXBAR8_GPIO25_SEL == 1 | MW_EPWMXBAR8_GPIO26_SEL == 1 | MW_EPWMXBAR8_GPIO27_SEL == 1 | MW_EPWMXBAR8_GPIO28_SEL == 1 | MW_EPWMXBAR8_GPIO29_SEL == 1 | MW_EPWMXBAR8_GPIO30_SEL == 1 | MW_EPWMXBAR8_GPIO31_SEL == 1 | MW_EPWMXBAR8_GPIO32_SEL == 1 | MW_EPWMXBAR8_GPIO33_SEL == 1 | MW_EPWMXBAR8_GPIO34_SEL == 1 | MW_EPWMXBAR8_GPIO35_SEL == 1 | MW_EPWMXBAR8_GPIO36_SEL == 1 | MW_EPWMXBAR8_GPIO37_SEL == 1 | MW_EPWMXBAR8_GPIO38_SEL == 1 | MW_EPWMXBAR8_GPIO39_SEL == 1 | MW_EPWMXBAR8_GPIO40_SEL == 1 | MW_EPWMXBAR8_GPIO41_SEL == 1 | MW_EPWMXBAR8_GPIO42_SEL == 1 | MW_EPWMXBAR8_GPIO43_SEL == 1 | MW_EPWMXBAR8_GPIO44_SEL == 1 | | MW_EPWMXBAR8_GPIO45_SEL == 1 | MW_EPWMXBAR8_GPIO46_SEL == 1 | MW_EPWMXBAR8_GPIO47_SEL == 1| MW_EPWMXBAR8_GPIO48_SEL == 1 | MW_EPWMXBAR8_GPIO49_SEL == 1 | MW_EPWMXBAR8_GPIO50_SEL == 1 | MW_EPWMXBAR8_GPIO51_SEL == 1 | MW_EPWMXBAR8_GPIO52_SEL == 1 | MW_EPWMXBAR8_GPIO53_SEL == 1 | MW_EPWMXBAR8_GPIO54_SEL == 1 | MW_EPWMXBAR8_GPIO55_SEL == 1 | MW_EPWMXBAR8_GPIO56_SEL == 1 | MW_EPWMXBAR8_GPIO57_SEL == 1| MW_EPWMXBAR8_GPIO58_SEL == 1 | MW_EPWMXBAR8_GPIO59_SEL == 1 | MW_EPWMXBAR8_GPIO60_SEL == 1 | MW_EPWMXBAR8_GPIO61_SEL == 1 | MW_EPWMXBAR8_GPIO62_SEL == 1 | MW_EPWMXBAR8_GPIO63_SEL == 1 | MW_EPWMXBAR8_GPIO64_SEL == 1 | | MW_EPWMXBAR8_GPIO59_SEL == 1 | MW_EPWMXBAR8_GPIO60_SEL == 1 | MW_EPWMXBAR8_GPIO61_SEL == 1 | MW_EPWMXBAR8_GPIO62_SEL == 1 | MW_EPWMXBAR8_GPIO63_SEL == 1 | MW_EPWMXBAR8_GPIO64_SEL == 1 | MW_EPWMXBAR8_GPIO65_SEL == 1 | MW_EPWMXBAR8_GPIO66_SEL == 1 | MW_EPWMXBAR8_GPIO67_SEL == 1 | MW_EPWMXBAR8_GPIO68_SEL == 1 | MW_EPWMXBAR8_GPIO69_SEL == 1 | MW_EPWMXBAR8_GPIO70_SEL == 1 | MW_EPWMXBAR8_GPIO71_SEL == 1 | MW_EPWMXBAR8_GPIO72_SEL == 1 | MW_EPWMXBAR8_GPIO73_SEL == 1 | MW_EPWMXBAR8_GPIO74_SEL == 1 | MW_EPWMXBAR8_GPIO75_SEL == 1 | MW_EPWMXBAR8_GPIO76_SEL == 1 | MW_EPWMXBAR8_GPIO77_SEL == 1 | MW_EPWMXBAR8_GPIO78_SEL == 1 | | MW_EPWMXBAR8_GPIO80_SEL == 1 | MW_EPWMXBAR8_GPIO81_SEL == 1 | MW_EPWMXBAR8_GPIO82_SEL == 1 | MW_EPWMXBAR8_GPIO83_SEL == 1 | MW_EPWMXBAR8_GPIO84_SEL == 1 | MW_EPWMXBAR8_GPIO85_SEL == 1 | MW_EPWMXBAR8_GPIO86_SEL == 1 | MW_EPWMXBAR8_GPIO87_SEL == 1 | MW_EPWMXBAR8_GPIO88_SEL == 1 | MW_EPWMXBAR8_GPIO89_SEL == 1 | MW_EPWMXBAR8_GPIO90_SEL == 1 | MW_EPWMXBAR8_GPIO91_SEL == 1 | MW_EPWMXBAR8_GPIO92_SEL == 1 | MW_EPWMXBAR8_GPIO93_SEL == 1 | MW_EPWMXBAR8_GPIO94_SEL == 1 | MW_EPWMXBAR8_GPIO95_SEL == 1 | MW_EPWMXBAR8_GPIO96_SEL == 1 | MW_EPWMXBAR8_GPIO97_SEL == 1 | MW_EPWMXBAR8_GPIO98_SEL == 1 | MW_EPWMXBAR8_GPIO99_SEL == 1 | MW_EPWMXBAR8_GPIO100_SEL == 1 | MW_EPWMXBAR8_GPIO101_SEL == 1 | MW_EPWMXBAR8_GPIO102_SEL == 1 | MW_EPWMXBAR8_GPIO103_SEL == 1 | MW_EPWMXBAR8_GPIO104_SEL == 1 | MW_EPWMXBAR8_GPIO105_SEL == 1 | MW_EPWMXBAR8_GPIO106_SEL == 1 | MW_EPWMXBAR8_GPIO107_SEL == 1 | | MW_EPWMXBAR8_GPIO108_SEL == 1 | MW_EPWMXBAR8_GPIO109_SEL == 1| MW_EPWMXBAR8_GPIO110_SEL == 1 | MW_EPWMXBAR8_GPIO111_SEL == 1 | MW_EPWMXBAR8_GPIO112_SEL == 1 | MW_EPWMXBAR8_GPIO113_SEL == 1 | MW_EPWMXBAR8_GPIO114_SEL == 1 | MW_EPWMXBAR8_GPIO115_SEL == 1 | MW_EPWMXBAR8_GPIO116_SEL == 1 | MW_EPWMXBAR8_GPIO117_SEL == 1 | MW_EPWMXBAR8_GPIO118_SEL == 1 | MW_EPWMXBAR8_GPIO119_SEL == 1 | MW_EPWMXBAR8_GPIO120_SEL == 1 | MW_EPWMXBAR8_GPIO121_SEL == 1 | MW_EPWMXBAR8_GPIO122_SEL == 1 | MW_EPWMXBAR8_GPIO123_SEL == 1 | MW_EPWMXBAR8_GPIO124_SEL == 1 | MW_EPWMXBAR8_GPIO125_SEL == 1 | MW_EPWMXBAR8_GPIO126_SEL == 1 | MW_EPWMXBAR8_GPIO127_SEL == 1 | MW_EPWMXBAR8_GPIO128_SEL == 1 | MW_EPWMXBAR8_GPIO129_SEL == 1 | MW_EPWMXBAR8_GPIO130_SEL == 1 | MW_EPWMXBAR8_GPIO131_SEL == 1 | MW_EPWMXBAR8_GPIO132_SEL == 1 | MW_EPWMXBAR8_GPIO133_SEL == 1 | MW_EPWMXBAR8_GPIO134_SEL == 1 | MW_EPWMXBAR8_GPIO135_SEL == 1 | MW_EPWMXBAR8_GPIO136_SEL == 1 | MW_EPWMXBAR8_GPIO137_SEL == 1 | MW_EPWMXBAR8_GPIO139_SEL == 1 | MW_EPWMXBAR8_GPIO140_SEL == 1 | MW_EPWMXBAR8_GPIO141_SEL == 1 | MW_EPWMXBAR8_GPIO142_SEL == 1 | MW_EPWMXBAR8_GPIO143_SEL == 1 | MW_EPWMXBAR8_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR8_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR8_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[8] = EPWM_XBAR_8;
    // else
        group3_epwmxbar[8] = 0;
    group4_epwmxbar[8] = (MW_EPWMXBAR8_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR8_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR8_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR8_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR8_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR8_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR8_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR8_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR8_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR8_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[8] = (MW_EPWMXBAR8_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR8_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR8_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR8_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR8_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR8_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR8_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR8_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR8_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR8_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[8] = (MW_EPWMXBAR8_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR8_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR8_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR8_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR8_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR8_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR8_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR8_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR8_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR8_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[8] = (MW_EPWMXBAR8_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR8_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR8_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR8_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR8_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR8_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR8_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR8_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR8_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR8_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR8_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR8_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR8_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR8_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[8] = (MW_EPWMXBAR8_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR8_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR8_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR8_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[9] = (MW_EPWMXBAR9_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR9_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR9_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR9_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR9_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR9_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR9_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR9_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR9_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR9_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR9_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR9_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR9_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR9_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR9_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR9_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR9_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR9_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[9] = (MW_EPWMXBAR9_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR9_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR9_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR9_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR9_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR9_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR9_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR9_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR9_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR9_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR9_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR9_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR9_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR9_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR9_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR9_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR9_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR9_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR9_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR9_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR9_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR9_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR9_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR9_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[9] = (MW_EPWMXBAR9_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR9_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR9_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR9_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR9_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR9_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR9_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR9_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR9_GPIO0_SEL == 1 | MW_EPWMXBAR9_GPIO1_SEL == 1 | MW_EPWMXBAR9_GPIO2_SEL == 1 | MW_EPWMXBAR9_GPIO3_SEL == 1 | MW_EPWMXBAR9_GPIO4_SEL == 1 | MW_EPWMXBAR9_GPIO5_SEL == 1 | MW_EPWMXBAR9_GPIO6_SEL == 1 | MW_EPWMXBAR9_GPIO7_SEL == 1| MW_EPWMXBAR9_GPIO8_SEL == 1 | MW_EPWMXBAR9_GPIO9_SEL == 1 | MW_EPWMXBAR9_GPIO10_SEL == 1| MW_EPWMXBAR9_GPIO11_SEL == 1| MW_EPWMXBAR9_GPIO12_SEL == 1 | MW_EPWMXBAR9_GPIO13_SEL == 1 | MW_EPWMXBAR9_GPIO14_SEL == 1 | MW_EPWMXBAR9_GPIO15_SEL == 1 | MW_EPWMXBAR9_GPIO16_SEL == 1 | MW_EPWMXBAR9_GPIO17_SEL == 1 | MW_EPWMXBAR9_GPIO18_SEL == 1 | MW_EPWMXBAR9_GPIO19_SEL == 1 | MW_EPWMXBAR9_GPIO20_SEL == 1 | MW_EPWMXBAR9_GPIO21_SEL == 1 | MW_EPWMXBAR9_GPIO22_SEL == 1 | MW_EPWMXBAR9_GPIO23_SEL == 1 | MW_EPWMXBAR9_GPIO24_SEL == 1 | MW_EPWMXBAR9_GPIO25_SEL == 1 | MW_EPWMXBAR9_GPIO26_SEL == 1 | MW_EPWMXBAR9_GPIO27_SEL == 1 | MW_EPWMXBAR9_GPIO28_SEL == 1 | MW_EPWMXBAR9_GPIO29_SEL == 1 | MW_EPWMXBAR9_GPIO30_SEL == 1 | MW_EPWMXBAR9_GPIO31_SEL == 1 | MW_EPWMXBAR9_GPIO32_SEL == 1 | MW_EPWMXBAR9_GPIO33_SEL == 1 | MW_EPWMXBAR9_GPIO34_SEL == 1 | MW_EPWMXBAR9_GPIO35_SEL == 1 | MW_EPWMXBAR9_GPIO36_SEL == 1 | MW_EPWMXBAR9_GPIO37_SEL == 1 | MW_EPWMXBAR9_GPIO38_SEL == 1 | MW_EPWMXBAR9_GPIO39_SEL == 1 | MW_EPWMXBAR9_GPIO40_SEL == 1 | MW_EPWMXBAR9_GPIO41_SEL == 1 | MW_EPWMXBAR9_GPIO42_SEL == 1 | MW_EPWMXBAR9_GPIO43_SEL == 1 | MW_EPWMXBAR9_GPIO44_SEL == 1 | | MW_EPWMXBAR9_GPIO45_SEL == 1 | MW_EPWMXBAR9_GPIO46_SEL == 1 | MW_EPWMXBAR9_GPIO47_SEL == 1| MW_EPWMXBAR9_GPIO48_SEL == 1 | MW_EPWMXBAR9_GPIO49_SEL == 1 | MW_EPWMXBAR9_GPIO50_SEL == 1 | MW_EPWMXBAR9_GPIO51_SEL == 1 | MW_EPWMXBAR9_GPIO52_SEL == 1 | MW_EPWMXBAR9_GPIO53_SEL == 1 | MW_EPWMXBAR9_GPIO54_SEL == 1 | MW_EPWMXBAR9_GPIO55_SEL == 1 | MW_EPWMXBAR9_GPIO56_SEL == 1 | MW_EPWMXBAR9_GPIO57_SEL == 1| MW_EPWMXBAR9_GPIO58_SEL == 1 | MW_EPWMXBAR9_GPIO59_SEL == 1 | MW_EPWMXBAR9_GPIO60_SEL == 1 | MW_EPWMXBAR9_GPIO61_SEL == 1 | MW_EPWMXBAR9_GPIO62_SEL == 1 | MW_EPWMXBAR9_GPIO63_SEL == 1 | MW_EPWMXBAR9_GPIO64_SEL == 1 | | MW_EPWMXBAR9_GPIO59_SEL == 1 | MW_EPWMXBAR9_GPIO60_SEL == 1 | MW_EPWMXBAR9_GPIO61_SEL == 1 | MW_EPWMXBAR9_GPIO62_SEL == 1 | MW_EPWMXBAR9_GPIO63_SEL == 1 | MW_EPWMXBAR9_GPIO64_SEL == 1 | MW_EPWMXBAR9_GPIO65_SEL == 1 | MW_EPWMXBAR9_GPIO66_SEL == 1 | MW_EPWMXBAR9_GPIO67_SEL == 1 | MW_EPWMXBAR9_GPIO68_SEL == 1 | MW_EPWMXBAR9_GPIO69_SEL == 1 | MW_EPWMXBAR9_GPIO70_SEL == 1 | MW_EPWMXBAR9_GPIO71_SEL == 1 | MW_EPWMXBAR9_GPIO72_SEL == 1 | MW_EPWMXBAR9_GPIO73_SEL == 1 | MW_EPWMXBAR9_GPIO74_SEL == 1 | MW_EPWMXBAR9_GPIO75_SEL == 1 | MW_EPWMXBAR9_GPIO76_SEL == 1 | MW_EPWMXBAR9_GPIO77_SEL == 1 | MW_EPWMXBAR9_GPIO78_SEL == 1 | | MW_EPWMXBAR9_GPIO80_SEL == 1 | MW_EPWMXBAR9_GPIO81_SEL == 1 | MW_EPWMXBAR9_GPIO82_SEL == 1 | MW_EPWMXBAR9_GPIO83_SEL == 1 | MW_EPWMXBAR9_GPIO84_SEL == 1 | MW_EPWMXBAR9_GPIO85_SEL == 1 | MW_EPWMXBAR9_GPIO86_SEL == 1 | MW_EPWMXBAR9_GPIO87_SEL == 1 | MW_EPWMXBAR9_GPIO88_SEL == 1 | MW_EPWMXBAR9_GPIO89_SEL == 1 | MW_EPWMXBAR9_GPIO90_SEL == 1 | MW_EPWMXBAR9_GPIO91_SEL == 1 | MW_EPWMXBAR9_GPIO92_SEL == 1 | MW_EPWMXBAR9_GPIO93_SEL == 1 | MW_EPWMXBAR9_GPIO94_SEL == 1 | MW_EPWMXBAR9_GPIO95_SEL == 1 | MW_EPWMXBAR9_GPIO96_SEL == 1 | MW_EPWMXBAR9_GPIO97_SEL == 1 | MW_EPWMXBAR9_GPIO98_SEL == 1 | MW_EPWMXBAR9_GPIO99_SEL == 1 | MW_EPWMXBAR9_GPIO100_SEL == 1 | MW_EPWMXBAR9_GPIO101_SEL == 1 | MW_EPWMXBAR9_GPIO102_SEL == 1 | MW_EPWMXBAR9_GPIO103_SEL == 1 | MW_EPWMXBAR9_GPIO104_SEL == 1 | MW_EPWMXBAR9_GPIO105_SEL == 1 | MW_EPWMXBAR9_GPIO106_SEL == 1 | MW_EPWMXBAR9_GPIO107_SEL == 1 | | MW_EPWMXBAR9_GPIO108_SEL == 1 | MW_EPWMXBAR9_GPIO109_SEL == 1| MW_EPWMXBAR9_GPIO110_SEL == 1 | MW_EPWMXBAR9_GPIO111_SEL == 1 | MW_EPWMXBAR9_GPIO112_SEL == 1 | MW_EPWMXBAR9_GPIO113_SEL == 1 | MW_EPWMXBAR9_GPIO114_SEL == 1 | MW_EPWMXBAR9_GPIO115_SEL == 1 | MW_EPWMXBAR9_GPIO116_SEL == 1 | MW_EPWMXBAR9_GPIO117_SEL == 1 | MW_EPWMXBAR9_GPIO118_SEL == 1 | MW_EPWMXBAR9_GPIO119_SEL == 1 | MW_EPWMXBAR9_GPIO120_SEL == 1 | MW_EPWMXBAR9_GPIO121_SEL == 1 | MW_EPWMXBAR9_GPIO122_SEL == 1 | MW_EPWMXBAR9_GPIO123_SEL == 1 | MW_EPWMXBAR9_GPIO124_SEL == 1 | MW_EPWMXBAR9_GPIO125_SEL == 1 | MW_EPWMXBAR9_GPIO126_SEL == 1 | MW_EPWMXBAR9_GPIO127_SEL == 1 | MW_EPWMXBAR9_GPIO128_SEL == 1 | MW_EPWMXBAR9_GPIO129_SEL == 1 | MW_EPWMXBAR9_GPIO130_SEL == 1 | MW_EPWMXBAR9_GPIO131_SEL == 1 | MW_EPWMXBAR9_GPIO132_SEL == 1 | MW_EPWMXBAR9_GPIO133_SEL == 1 | MW_EPWMXBAR9_GPIO134_SEL == 1 | MW_EPWMXBAR9_GPIO135_SEL == 1 | MW_EPWMXBAR9_GPIO136_SEL == 1 | MW_EPWMXBAR9_GPIO137_SEL == 1 | MW_EPWMXBAR9_GPIO139_SEL == 1 | MW_EPWMXBAR9_GPIO140_SEL == 1 | MW_EPWMXBAR9_GPIO141_SEL == 1 | MW_EPWMXBAR9_GPIO142_SEL == 1 | MW_EPWMXBAR9_GPIO143_SEL == 1 | MW_EPWMXBAR9_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR9_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR9_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[9] = EPWM_XBAR_9;
    // else
        group3_epwmxbar[9] = 0;
    group4_epwmxbar[9] = (MW_EPWMXBAR9_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR9_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR9_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR9_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR9_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR9_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR9_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR9_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR9_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR9_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[9] = (MW_EPWMXBAR9_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR9_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR9_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR9_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR9_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR9_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR9_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR9_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR9_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR9_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[9] = (MW_EPWMXBAR9_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR9_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR9_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR9_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR9_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR9_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR9_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR9_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR9_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR9_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[9] = (MW_EPWMXBAR9_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR9_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR9_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR9_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR9_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR9_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR9_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR9_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR9_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR9_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR9_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR9_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR9_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR9_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[9] = (MW_EPWMXBAR9_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR9_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR9_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR9_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[10] = (MW_EPWMXBAR10_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR10_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR10_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR10_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR10_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR10_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR10_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR10_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR10_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR10_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR10_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR10_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR10_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR10_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR10_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR10_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR10_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR10_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[10] = (MW_EPWMXBAR10_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR10_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR10_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR10_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR10_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR10_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR10_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR10_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR10_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR10_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR10_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR10_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR10_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR10_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR10_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR10_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR10_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR10_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR10_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR10_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR10_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR10_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR10_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR10_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[10] = (MW_EPWMXBAR10_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR10_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR10_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR10_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR10_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR10_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR10_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR10_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR10_GPIO0_SEL == 1 | MW_EPWMXBAR10_GPIO1_SEL == 1 | MW_EPWMXBAR10_GPIO2_SEL == 1 | MW_EPWMXBAR10_GPIO3_SEL == 1 | MW_EPWMXBAR10_GPIO4_SEL == 1 | MW_EPWMXBAR10_GPIO5_SEL == 1 | MW_EPWMXBAR10_GPIO6_SEL == 1 | MW_EPWMXBAR10_GPIO7_SEL == 1| MW_EPWMXBAR10_GPIO8_SEL == 1 | MW_EPWMXBAR10_GPIO9_SEL == 1 | MW_EPWMXBAR10_GPIO10_SEL == 1| MW_EPWMXBAR10_GPIO11_SEL == 1| MW_EPWMXBAR10_GPIO12_SEL == 1 | MW_EPWMXBAR10_GPIO13_SEL == 1 | MW_EPWMXBAR10_GPIO14_SEL == 1 | MW_EPWMXBAR10_GPIO15_SEL == 1 | MW_EPWMXBAR10_GPIO16_SEL == 1 | MW_EPWMXBAR10_GPIO17_SEL == 1 | MW_EPWMXBAR10_GPIO18_SEL == 1 | MW_EPWMXBAR10_GPIO19_SEL == 1 | MW_EPWMXBAR10_GPIO20_SEL == 1 | MW_EPWMXBAR10_GPIO21_SEL == 1 | MW_EPWMXBAR10_GPIO22_SEL == 1 | MW_EPWMXBAR10_GPIO23_SEL == 1 | MW_EPWMXBAR10_GPIO24_SEL == 1 | MW_EPWMXBAR10_GPIO25_SEL == 1 | MW_EPWMXBAR10_GPIO26_SEL == 1 | MW_EPWMXBAR10_GPIO27_SEL == 1 | MW_EPWMXBAR10_GPIO28_SEL == 1 | MW_EPWMXBAR10_GPIO29_SEL == 1 | MW_EPWMXBAR10_GPIO30_SEL == 1 | MW_EPWMXBAR10_GPIO31_SEL == 1 | MW_EPWMXBAR10_GPIO32_SEL == 1 | MW_EPWMXBAR10_GPIO33_SEL == 1 | MW_EPWMXBAR10_GPIO34_SEL == 1 | MW_EPWMXBAR10_GPIO35_SEL == 1 | MW_EPWMXBAR10_GPIO36_SEL == 1 | MW_EPWMXBAR10_GPIO37_SEL == 1 | MW_EPWMXBAR10_GPIO38_SEL == 1 | MW_EPWMXBAR10_GPIO39_SEL == 1 | MW_EPWMXBAR10_GPIO40_SEL == 1 | MW_EPWMXBAR10_GPIO41_SEL == 1 | MW_EPWMXBAR10_GPIO42_SEL == 1 | MW_EPWMXBAR10_GPIO43_SEL == 1 | MW_EPWMXBAR10_GPIO44_SEL == 1 | | MW_EPWMXBAR10_GPIO45_SEL == 1 | MW_EPWMXBAR10_GPIO46_SEL == 1 | MW_EPWMXBAR10_GPIO47_SEL == 1| MW_EPWMXBAR10_GPIO48_SEL == 1 | MW_EPWMXBAR10_GPIO49_SEL == 1 | MW_EPWMXBAR10_GPIO50_SEL == 1 | MW_EPWMXBAR10_GPIO51_SEL == 1 | MW_EPWMXBAR10_GPIO52_SEL == 1 | MW_EPWMXBAR10_GPIO53_SEL == 1 | MW_EPWMXBAR10_GPIO54_SEL == 1 | MW_EPWMXBAR10_GPIO55_SEL == 1 | MW_EPWMXBAR10_GPIO56_SEL == 1 | MW_EPWMXBAR10_GPIO57_SEL == 1| MW_EPWMXBAR10_GPIO58_SEL == 1 | MW_EPWMXBAR10_GPIO59_SEL == 1 | MW_EPWMXBAR10_GPIO60_SEL == 1 | MW_EPWMXBAR10_GPIO61_SEL == 1 | MW_EPWMXBAR10_GPIO62_SEL == 1 | MW_EPWMXBAR10_GPIO63_SEL == 1 | MW_EPWMXBAR10_GPIO64_SEL == 1 | | MW_EPWMXBAR10_GPIO59_SEL == 1 | MW_EPWMXBAR10_GPIO60_SEL == 1 | MW_EPWMXBAR10_GPIO61_SEL == 1 | MW_EPWMXBAR10_GPIO62_SEL == 1 | MW_EPWMXBAR10_GPIO63_SEL == 1 | MW_EPWMXBAR10_GPIO64_SEL == 1 | MW_EPWMXBAR10_GPIO65_SEL == 1 | MW_EPWMXBAR10_GPIO66_SEL == 1 | MW_EPWMXBAR10_GPIO67_SEL == 1 | MW_EPWMXBAR10_GPIO68_SEL == 1 | MW_EPWMXBAR10_GPIO69_SEL == 1 | MW_EPWMXBAR10_GPIO70_SEL == 1 | MW_EPWMXBAR10_GPIO71_SEL == 1 | MW_EPWMXBAR10_GPIO72_SEL == 1 | MW_EPWMXBAR10_GPIO73_SEL == 1 | MW_EPWMXBAR10_GPIO74_SEL == 1 | MW_EPWMXBAR10_GPIO75_SEL == 1 | MW_EPWMXBAR10_GPIO76_SEL == 1 | MW_EPWMXBAR10_GPIO77_SEL == 1 | MW_EPWMXBAR10_GPIO78_SEL == 1 | | MW_EPWMXBAR10_GPIO80_SEL == 1 | MW_EPWMXBAR10_GPIO81_SEL == 1 | MW_EPWMXBAR10_GPIO82_SEL == 1 | MW_EPWMXBAR10_GPIO83_SEL == 1 | MW_EPWMXBAR10_GPIO84_SEL == 1 | MW_EPWMXBAR10_GPIO85_SEL == 1 | MW_EPWMXBAR10_GPIO86_SEL == 1 | MW_EPWMXBAR10_GPIO87_SEL == 1 | MW_EPWMXBAR10_GPIO88_SEL == 1 | MW_EPWMXBAR10_GPIO89_SEL == 1 | MW_EPWMXBAR10_GPIO90_SEL == 1 | MW_EPWMXBAR10_GPIO91_SEL == 1 | MW_EPWMXBAR10_GPIO92_SEL == 1 | MW_EPWMXBAR10_GPIO93_SEL == 1 | MW_EPWMXBAR10_GPIO94_SEL == 1 | MW_EPWMXBAR10_GPIO95_SEL == 1 | MW_EPWMXBAR10_GPIO96_SEL == 1 | MW_EPWMXBAR10_GPIO97_SEL == 1 | MW_EPWMXBAR10_GPIO98_SEL == 1 | MW_EPWMXBAR10_GPIO99_SEL == 1 | MW_EPWMXBAR10_GPIO100_SEL == 1 | MW_EPWMXBAR10_GPIO101_SEL == 1 | MW_EPWMXBAR10_GPIO102_SEL == 1 | MW_EPWMXBAR10_GPIO103_SEL == 1 | MW_EPWMXBAR10_GPIO104_SEL == 1 | MW_EPWMXBAR10_GPIO105_SEL == 1 | MW_EPWMXBAR10_GPIO106_SEL == 1 | MW_EPWMXBAR10_GPIO107_SEL == 1 | | MW_EPWMXBAR10_GPIO108_SEL == 1 | MW_EPWMXBAR10_GPIO109_SEL == 1| MW_EPWMXBAR10_GPIO110_SEL == 1 | MW_EPWMXBAR10_GPIO111_SEL == 1 | MW_EPWMXBAR10_GPIO112_SEL == 1 | MW_EPWMXBAR10_GPIO113_SEL == 1 | MW_EPWMXBAR10_GPIO114_SEL == 1 | MW_EPWMXBAR10_GPIO115_SEL == 1 | MW_EPWMXBAR10_GPIO116_SEL == 1 | MW_EPWMXBAR10_GPIO117_SEL == 1 | MW_EPWMXBAR10_GPIO118_SEL == 1 | MW_EPWMXBAR10_GPIO119_SEL == 1 | MW_EPWMXBAR10_GPIO120_SEL == 1 | MW_EPWMXBAR10_GPIO121_SEL == 1 | MW_EPWMXBAR10_GPIO122_SEL == 1 | MW_EPWMXBAR10_GPIO123_SEL == 1 | MW_EPWMXBAR10_GPIO124_SEL == 1 | MW_EPWMXBAR10_GPIO125_SEL == 1 | MW_EPWMXBAR10_GPIO126_SEL == 1 | MW_EPWMXBAR10_GPIO127_SEL == 1 | MW_EPWMXBAR10_GPIO128_SEL == 1 | MW_EPWMXBAR10_GPIO129_SEL == 1 | MW_EPWMXBAR10_GPIO130_SEL == 1 | MW_EPWMXBAR10_GPIO131_SEL == 1 | MW_EPWMXBAR10_GPIO132_SEL == 1 | MW_EPWMXBAR10_GPIO133_SEL == 1 | MW_EPWMXBAR10_GPIO134_SEL == 1 | MW_EPWMXBAR10_GPIO135_SEL == 1 | MW_EPWMXBAR10_GPIO136_SEL == 1 | MW_EPWMXBAR10_GPIO137_SEL == 1 | MW_EPWMXBAR10_GPIO139_SEL == 1 | MW_EPWMXBAR10_GPIO140_SEL == 1 | MW_EPWMXBAR10_GPIO141_SEL == 1 | MW_EPWMXBAR10_GPIO142_SEL == 1 | MW_EPWMXBAR10_GPIO143_SEL == 1 | MW_EPWMXBAR10_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR10_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR10_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[10] = EPWM_XBAR_10;
    // else
    group3_epwmxbar[10] = 0;
    group4_epwmxbar[10] = (MW_EPWMXBAR10_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR10_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR10_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR10_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR10_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR10_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR10_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR10_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR10_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR10_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[10] = (MW_EPWMXBAR10_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR10_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR10_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR10_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR10_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR10_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR10_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR10_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR10_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR10_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[10] = (MW_EPWMXBAR10_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR10_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR10_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR10_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR10_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR10_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR10_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR10_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR10_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR10_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[10] = (MW_EPWMXBAR10_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR10_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR10_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR10_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR10_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR10_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR10_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR10_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR10_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR10_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR10_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR10_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR10_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR10_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[10] = (MW_EPWMXBAR10_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR10_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR10_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR10_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[11] = (MW_EPWMXBAR11_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR11_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR11_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR11_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR11_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR11_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR11_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR11_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR11_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR11_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR11_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR11_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR11_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR11_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR11_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR11_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR11_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR11_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[11] = (MW_EPWMXBAR11_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR11_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR11_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR11_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR11_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR11_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR11_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR11_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR11_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR11_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR11_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR11_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR11_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR11_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR11_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR11_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR11_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR11_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR11_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR11_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR11_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR11_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR11_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR11_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[11] = (MW_EPWMXBAR11_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR11_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR11_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR11_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR11_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR11_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR11_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR11_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR11_GPIO0_SEL == 1 | MW_EPWMXBAR11_GPIO1_SEL == 1 | MW_EPWMXBAR11_GPIO2_SEL == 1 | MW_EPWMXBAR11_GPIO3_SEL == 1 | MW_EPWMXBAR11_GPIO4_SEL == 1 | MW_EPWMXBAR11_GPIO5_SEL == 1 | MW_EPWMXBAR11_GPIO6_SEL == 1 | MW_EPWMXBAR11_GPIO7_SEL == 1| MW_EPWMXBAR11_GPIO8_SEL == 1 | MW_EPWMXBAR11_GPIO9_SEL == 1 | MW_EPWMXBAR11_GPIO10_SEL == 1| MW_EPWMXBAR11_GPIO11_SEL == 1| MW_EPWMXBAR11_GPIO12_SEL == 1 | MW_EPWMXBAR11_GPIO13_SEL == 1 | MW_EPWMXBAR11_GPIO14_SEL == 1 | MW_EPWMXBAR11_GPIO15_SEL == 1 | MW_EPWMXBAR11_GPIO16_SEL == 1 | MW_EPWMXBAR11_GPIO17_SEL == 1 | MW_EPWMXBAR11_GPIO18_SEL == 1 | MW_EPWMXBAR11_GPIO19_SEL == 1 | MW_EPWMXBAR11_GPIO20_SEL == 1 | MW_EPWMXBAR11_GPIO21_SEL == 1 | MW_EPWMXBAR11_GPIO22_SEL == 1 | MW_EPWMXBAR11_GPIO23_SEL == 1 | MW_EPWMXBAR11_GPIO24_SEL == 1 | MW_EPWMXBAR11_GPIO25_SEL == 1 | MW_EPWMXBAR11_GPIO26_SEL == 1 | MW_EPWMXBAR11_GPIO27_SEL == 1 | MW_EPWMXBAR11_GPIO28_SEL == 1 | MW_EPWMXBAR11_GPIO29_SEL == 1 | MW_EPWMXBAR11_GPIO30_SEL == 1 | MW_EPWMXBAR11_GPIO31_SEL == 1 | MW_EPWMXBAR11_GPIO32_SEL == 1 | MW_EPWMXBAR11_GPIO33_SEL == 1 | MW_EPWMXBAR11_GPIO34_SEL == 1 | MW_EPWMXBAR11_GPIO35_SEL == 1 | MW_EPWMXBAR11_GPIO36_SEL == 1 | MW_EPWMXBAR11_GPIO37_SEL == 1 | MW_EPWMXBAR11_GPIO38_SEL == 1 | MW_EPWMXBAR11_GPIO39_SEL == 1 | MW_EPWMXBAR11_GPIO40_SEL == 1 | MW_EPWMXBAR11_GPIO41_SEL == 1 | MW_EPWMXBAR11_GPIO42_SEL == 1 | MW_EPWMXBAR11_GPIO43_SEL == 1 | MW_EPWMXBAR11_GPIO44_SEL == 1 | | MW_EPWMXBAR11_GPIO45_SEL == 1 | MW_EPWMXBAR11_GPIO46_SEL == 1 | MW_EPWMXBAR11_GPIO47_SEL == 1| MW_EPWMXBAR11_GPIO48_SEL == 1 | MW_EPWMXBAR11_GPIO49_SEL == 1 | MW_EPWMXBAR11_GPIO50_SEL == 1 | MW_EPWMXBAR11_GPIO51_SEL == 1 | MW_EPWMXBAR11_GPIO52_SEL == 1 | MW_EPWMXBAR11_GPIO53_SEL == 1 | MW_EPWMXBAR11_GPIO54_SEL == 1 | MW_EPWMXBAR11_GPIO55_SEL == 1 | MW_EPWMXBAR11_GPIO56_SEL == 1 | MW_EPWMXBAR11_GPIO57_SEL == 1| MW_EPWMXBAR11_GPIO58_SEL == 1 | MW_EPWMXBAR11_GPIO59_SEL == 1 | MW_EPWMXBAR11_GPIO60_SEL == 1 | MW_EPWMXBAR11_GPIO61_SEL == 1 | MW_EPWMXBAR11_GPIO62_SEL == 1 | MW_EPWMXBAR11_GPIO63_SEL == 1 | MW_EPWMXBAR11_GPIO64_SEL == 1 | | MW_EPWMXBAR11_GPIO59_SEL == 1 | MW_EPWMXBAR11_GPIO60_SEL == 1 | MW_EPWMXBAR11_GPIO61_SEL == 1 | MW_EPWMXBAR11_GPIO62_SEL == 1 | MW_EPWMXBAR11_GPIO63_SEL == 1 | MW_EPWMXBAR11_GPIO64_SEL == 1 | MW_EPWMXBAR11_GPIO65_SEL == 1 | MW_EPWMXBAR11_GPIO66_SEL == 1 | MW_EPWMXBAR11_GPIO67_SEL == 1 | MW_EPWMXBAR11_GPIO68_SEL == 1 | MW_EPWMXBAR11_GPIO69_SEL == 1 | MW_EPWMXBAR11_GPIO70_SEL == 1 | MW_EPWMXBAR11_GPIO71_SEL == 1 | MW_EPWMXBAR11_GPIO72_SEL == 1 | MW_EPWMXBAR11_GPIO73_SEL == 1 | MW_EPWMXBAR11_GPIO74_SEL == 1 | MW_EPWMXBAR11_GPIO75_SEL == 1 | MW_EPWMXBAR11_GPIO76_SEL == 1 | MW_EPWMXBAR11_GPIO77_SEL == 1 | MW_EPWMXBAR11_GPIO78_SEL == 1 | | MW_EPWMXBAR11_GPIO80_SEL == 1 | MW_EPWMXBAR11_GPIO81_SEL == 1 | MW_EPWMXBAR11_GPIO82_SEL == 1 | MW_EPWMXBAR11_GPIO83_SEL == 1 | MW_EPWMXBAR11_GPIO84_SEL == 1 | MW_EPWMXBAR11_GPIO85_SEL == 1 | MW_EPWMXBAR11_GPIO86_SEL == 1 | MW_EPWMXBAR11_GPIO87_SEL == 1 | MW_EPWMXBAR11_GPIO88_SEL == 1 | MW_EPWMXBAR11_GPIO89_SEL == 1 | MW_EPWMXBAR11_GPIO90_SEL == 1 | MW_EPWMXBAR11_GPIO91_SEL == 1 | MW_EPWMXBAR11_GPIO92_SEL == 1 | MW_EPWMXBAR11_GPIO93_SEL == 1 | MW_EPWMXBAR11_GPIO94_SEL == 1 | MW_EPWMXBAR11_GPIO95_SEL == 1 | MW_EPWMXBAR11_GPIO96_SEL == 1 | MW_EPWMXBAR11_GPIO97_SEL == 1 | MW_EPWMXBAR11_GPIO98_SEL == 1 | MW_EPWMXBAR11_GPIO99_SEL == 1 | MW_EPWMXBAR11_GPIO100_SEL == 1 | MW_EPWMXBAR11_GPIO101_SEL == 1 | MW_EPWMXBAR11_GPIO102_SEL == 1 | MW_EPWMXBAR11_GPIO103_SEL == 1 | MW_EPWMXBAR11_GPIO104_SEL == 1 | MW_EPWMXBAR11_GPIO105_SEL == 1 | MW_EPWMXBAR11_GPIO106_SEL == 1 | MW_EPWMXBAR11_GPIO107_SEL == 1 | | MW_EPWMXBAR11_GPIO108_SEL == 1 | MW_EPWMXBAR11_GPIO109_SEL == 1| MW_EPWMXBAR11_GPIO110_SEL == 1 | MW_EPWMXBAR11_GPIO111_SEL == 1 | MW_EPWMXBAR11_GPIO112_SEL == 1 | MW_EPWMXBAR11_GPIO113_SEL == 1 | MW_EPWMXBAR11_GPIO114_SEL == 1 | MW_EPWMXBAR11_GPIO115_SEL == 1 | MW_EPWMXBAR11_GPIO116_SEL == 1 | MW_EPWMXBAR11_GPIO117_SEL == 1 | MW_EPWMXBAR11_GPIO118_SEL == 1 | MW_EPWMXBAR11_GPIO119_SEL == 1 | MW_EPWMXBAR11_GPIO120_SEL == 1 | MW_EPWMXBAR11_GPIO121_SEL == 1 | MW_EPWMXBAR11_GPIO122_SEL == 1 | MW_EPWMXBAR11_GPIO123_SEL == 1 | MW_EPWMXBAR11_GPIO124_SEL == 1 | MW_EPWMXBAR11_GPIO125_SEL == 1 | MW_EPWMXBAR11_GPIO126_SEL == 1 | MW_EPWMXBAR11_GPIO127_SEL == 1 | MW_EPWMXBAR11_GPIO128_SEL == 1 | MW_EPWMXBAR11_GPIO129_SEL == 1 | MW_EPWMXBAR11_GPIO130_SEL == 1 | MW_EPWMXBAR11_GPIO131_SEL == 1 | MW_EPWMXBAR11_GPIO132_SEL == 1 | MW_EPWMXBAR11_GPIO133_SEL == 1 | MW_EPWMXBAR11_GPIO134_SEL == 1 | MW_EPWMXBAR11_GPIO135_SEL == 1 | MW_EPWMXBAR11_GPIO136_SEL == 1 | MW_EPWMXBAR11_GPIO137_SEL == 1 | MW_EPWMXBAR11_GPIO139_SEL == 1 | MW_EPWMXBAR11_GPIO140_SEL == 1 | MW_EPWMXBAR11_GPIO141_SEL == 1 | MW_EPWMXBAR11_GPIO142_SEL == 1 | MW_EPWMXBAR11_GPIO143_SEL == 1 | MW_EPWMXBAR11_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR11_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR11_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[11] = EPWM_XBAR_11;
    // else
        group3_epwmxbar[11] = 0;
    group4_epwmxbar[11] = (MW_EPWMXBAR11_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR11_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR11_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR11_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR11_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR11_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR11_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR11_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR11_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR11_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[11] = (MW_EPWMXBAR11_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR11_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR11_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR11_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR11_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR11_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR11_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR11_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR11_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR11_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[11] = (MW_EPWMXBAR11_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR11_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR11_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR11_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR11_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR11_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR11_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR11_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR11_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR11_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[11] = (MW_EPWMXBAR11_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR11_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR11_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR11_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR11_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR11_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR11_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR11_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR11_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR11_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR11_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR11_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR11_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR11_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[11] = (MW_EPWMXBAR11_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR11_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR11_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR11_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[12] = (MW_EPWMXBAR12_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR12_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR12_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR12_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR12_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR12_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR12_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR12_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR12_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR12_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR12_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR12_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR12_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR12_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR12_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR12_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR12_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR12_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[12] = (MW_EPWMXBAR12_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR12_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR12_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR12_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR12_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR12_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR12_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR12_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR12_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR12_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR12_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR12_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR12_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR12_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR12_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR12_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR12_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR12_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR12_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR12_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR12_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR12_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR12_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR12_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[12] = (MW_EPWMXBAR12_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR12_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR12_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR12_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR12_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR12_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR12_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR12_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR12_GPIO0_SEL == 1 | MW_EPWMXBAR12_GPIO1_SEL == 1 | MW_EPWMXBAR12_GPIO2_SEL == 1 | MW_EPWMXBAR12_GPIO3_SEL == 1 | MW_EPWMXBAR12_GPIO4_SEL == 1 | MW_EPWMXBAR12_GPIO5_SEL == 1 | MW_EPWMXBAR12_GPIO6_SEL == 1 | MW_EPWMXBAR12_GPIO7_SEL == 1| MW_EPWMXBAR12_GPIO8_SEL == 1 | MW_EPWMXBAR12_GPIO9_SEL == 1 | MW_EPWMXBAR12_GPIO10_SEL == 1| MW_EPWMXBAR12_GPIO11_SEL == 1| MW_EPWMXBAR12_GPIO12_SEL == 1 | MW_EPWMXBAR12_GPIO13_SEL == 1 | MW_EPWMXBAR12_GPIO14_SEL == 1 | MW_EPWMXBAR12_GPIO15_SEL == 1 | MW_EPWMXBAR12_GPIO16_SEL == 1 | MW_EPWMXBAR12_GPIO17_SEL == 1 | MW_EPWMXBAR12_GPIO18_SEL == 1 | MW_EPWMXBAR12_GPIO19_SEL == 1 | MW_EPWMXBAR12_GPIO20_SEL == 1 | MW_EPWMXBAR12_GPIO21_SEL == 1 | MW_EPWMXBAR12_GPIO22_SEL == 1 | MW_EPWMXBAR12_GPIO23_SEL == 1 | MW_EPWMXBAR12_GPIO24_SEL == 1 | MW_EPWMXBAR12_GPIO25_SEL == 1 | MW_EPWMXBAR12_GPIO26_SEL == 1 | MW_EPWMXBAR12_GPIO27_SEL == 1 | MW_EPWMXBAR12_GPIO28_SEL == 1 | MW_EPWMXBAR12_GPIO29_SEL == 1 | MW_EPWMXBAR12_GPIO30_SEL == 1 | MW_EPWMXBAR12_GPIO31_SEL == 1 | MW_EPWMXBAR12_GPIO32_SEL == 1 | MW_EPWMXBAR12_GPIO33_SEL == 1 | MW_EPWMXBAR12_GPIO34_SEL == 1 | MW_EPWMXBAR12_GPIO35_SEL == 1 | MW_EPWMXBAR12_GPIO36_SEL == 1 | MW_EPWMXBAR12_GPIO37_SEL == 1 | MW_EPWMXBAR12_GPIO38_SEL == 1 | MW_EPWMXBAR12_GPIO39_SEL == 1 | MW_EPWMXBAR12_GPIO40_SEL == 1 | MW_EPWMXBAR12_GPIO41_SEL == 1 | MW_EPWMXBAR12_GPIO42_SEL == 1 | MW_EPWMXBAR12_GPIO43_SEL == 1 | MW_EPWMXBAR12_GPIO44_SEL == 1 | | MW_EPWMXBAR12_GPIO45_SEL == 1 | MW_EPWMXBAR12_GPIO46_SEL == 1 | MW_EPWMXBAR12_GPIO47_SEL == 1| MW_EPWMXBAR12_GPIO48_SEL == 1 | MW_EPWMXBAR12_GPIO49_SEL == 1 | MW_EPWMXBAR12_GPIO50_SEL == 1 | MW_EPWMXBAR12_GPIO51_SEL == 1 | MW_EPWMXBAR12_GPIO52_SEL == 1 | MW_EPWMXBAR12_GPIO53_SEL == 1 | MW_EPWMXBAR12_GPIO54_SEL == 1 | MW_EPWMXBAR12_GPIO55_SEL == 1 | MW_EPWMXBAR12_GPIO56_SEL == 1 | MW_EPWMXBAR12_GPIO57_SEL == 1| MW_EPWMXBAR12_GPIO58_SEL == 1 | MW_EPWMXBAR12_GPIO59_SEL == 1 | MW_EPWMXBAR12_GPIO60_SEL == 1 | MW_EPWMXBAR12_GPIO61_SEL == 1 | MW_EPWMXBAR12_GPIO62_SEL == 1 | MW_EPWMXBAR12_GPIO63_SEL == 1 | MW_EPWMXBAR12_GPIO64_SEL == 1 | | MW_EPWMXBAR12_GPIO59_SEL == 1 | MW_EPWMXBAR12_GPIO60_SEL == 1 | MW_EPWMXBAR12_GPIO61_SEL == 1 | MW_EPWMXBAR12_GPIO62_SEL == 1 | MW_EPWMXBAR12_GPIO63_SEL == 1 | MW_EPWMXBAR12_GPIO64_SEL == 1 | MW_EPWMXBAR12_GPIO65_SEL == 1 | MW_EPWMXBAR12_GPIO66_SEL == 1 | MW_EPWMXBAR12_GPIO67_SEL == 1 | MW_EPWMXBAR12_GPIO68_SEL == 1 | MW_EPWMXBAR12_GPIO69_SEL == 1 | MW_EPWMXBAR12_GPIO70_SEL == 1 | MW_EPWMXBAR12_GPIO71_SEL == 1 | MW_EPWMXBAR12_GPIO72_SEL == 1 | MW_EPWMXBAR12_GPIO73_SEL == 1 | MW_EPWMXBAR12_GPIO74_SEL == 1 | MW_EPWMXBAR12_GPIO75_SEL == 1 | MW_EPWMXBAR12_GPIO76_SEL == 1 | MW_EPWMXBAR12_GPIO77_SEL == 1 | MW_EPWMXBAR12_GPIO78_SEL == 1 | | MW_EPWMXBAR12_GPIO80_SEL == 1 | MW_EPWMXBAR12_GPIO81_SEL == 1 | MW_EPWMXBAR12_GPIO82_SEL == 1 | MW_EPWMXBAR12_GPIO83_SEL == 1 | MW_EPWMXBAR12_GPIO84_SEL == 1 | MW_EPWMXBAR12_GPIO85_SEL == 1 | MW_EPWMXBAR12_GPIO86_SEL == 1 | MW_EPWMXBAR12_GPIO87_SEL == 1 | MW_EPWMXBAR12_GPIO88_SEL == 1 | MW_EPWMXBAR12_GPIO89_SEL == 1 | MW_EPWMXBAR12_GPIO90_SEL == 1 | MW_EPWMXBAR12_GPIO91_SEL == 1 | MW_EPWMXBAR12_GPIO92_SEL == 1 | MW_EPWMXBAR12_GPIO93_SEL == 1 | MW_EPWMXBAR12_GPIO94_SEL == 1 | MW_EPWMXBAR12_GPIO95_SEL == 1 | MW_EPWMXBAR12_GPIO96_SEL == 1 | MW_EPWMXBAR12_GPIO97_SEL == 1 | MW_EPWMXBAR12_GPIO98_SEL == 1 | MW_EPWMXBAR12_GPIO99_SEL == 1 | MW_EPWMXBAR12_GPIO100_SEL == 1 | MW_EPWMXBAR12_GPIO101_SEL == 1 | MW_EPWMXBAR12_GPIO102_SEL == 1 | MW_EPWMXBAR12_GPIO103_SEL == 1 | MW_EPWMXBAR12_GPIO104_SEL == 1 | MW_EPWMXBAR12_GPIO105_SEL == 1 | MW_EPWMXBAR12_GPIO106_SEL == 1 | MW_EPWMXBAR12_GPIO107_SEL == 1 | | MW_EPWMXBAR12_GPIO108_SEL == 1 | MW_EPWMXBAR12_GPIO109_SEL == 1| MW_EPWMXBAR12_GPIO110_SEL == 1 | MW_EPWMXBAR12_GPIO111_SEL == 1 | MW_EPWMXBAR12_GPIO112_SEL == 1 | MW_EPWMXBAR12_GPIO113_SEL == 1 | MW_EPWMXBAR12_GPIO114_SEL == 1 | MW_EPWMXBAR12_GPIO115_SEL == 1 | MW_EPWMXBAR12_GPIO116_SEL == 1 | MW_EPWMXBAR12_GPIO117_SEL == 1 | MW_EPWMXBAR12_GPIO118_SEL == 1 | MW_EPWMXBAR12_GPIO119_SEL == 1 | MW_EPWMXBAR12_GPIO120_SEL == 1 | MW_EPWMXBAR12_GPIO121_SEL == 1 | MW_EPWMXBAR12_GPIO122_SEL == 1 | MW_EPWMXBAR12_GPIO123_SEL == 1 | MW_EPWMXBAR12_GPIO124_SEL == 1 | MW_EPWMXBAR12_GPIO125_SEL == 1 | MW_EPWMXBAR12_GPIO126_SEL == 1 | MW_EPWMXBAR12_GPIO127_SEL == 1 | MW_EPWMXBAR12_GPIO128_SEL == 1 | MW_EPWMXBAR12_GPIO129_SEL == 1 | MW_EPWMXBAR12_GPIO130_SEL == 1 | MW_EPWMXBAR12_GPIO131_SEL == 1 | MW_EPWMXBAR12_GPIO132_SEL == 1 | MW_EPWMXBAR12_GPIO133_SEL == 1 | MW_EPWMXBAR12_GPIO134_SEL == 1 | MW_EPWMXBAR12_GPIO135_SEL == 1 | MW_EPWMXBAR12_GPIO136_SEL == 1 | MW_EPWMXBAR12_GPIO137_SEL == 1 | MW_EPWMXBAR12_GPIO139_SEL == 1 | MW_EPWMXBAR12_GPIO140_SEL == 1 | MW_EPWMXBAR12_GPIO141_SEL == 1 | MW_EPWMXBAR12_GPIO142_SEL == 1 | MW_EPWMXBAR12_GPIO143_SEL == 1 | MW_EPWMXBAR12_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR12_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR12_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[12] = EPWM_XBAR_12;
    // else
        group3_epwmxbar[12] = 0;
    group4_epwmxbar[12] = (MW_EPWMXBAR12_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR12_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR12_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR12_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR12_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR12_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR12_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR12_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR12_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR12_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[12] = (MW_EPWMXBAR12_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR12_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR12_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR12_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR12_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR12_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR12_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR12_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR12_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR12_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[12] = (MW_EPWMXBAR12_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR12_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR12_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR12_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR12_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR12_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR12_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR12_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR12_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR12_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[12] = (MW_EPWMXBAR12_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR12_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR12_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR12_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR12_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR12_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR12_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR12_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR12_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR12_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR12_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR12_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR12_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR12_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[12] = (MW_EPWMXBAR12_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR12_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR12_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR12_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[13] = (MW_EPWMXBAR13_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR13_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR13_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR13_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR13_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR13_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR13_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR13_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR13_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR13_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR13_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR13_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR13_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR13_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR13_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR13_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR13_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR13_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[13] = (MW_EPWMXBAR13_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR13_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR13_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR13_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR13_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR13_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR13_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR13_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR13_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR13_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR13_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR13_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR13_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR13_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR13_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR13_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR13_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR13_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR13_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR13_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR13_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR13_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR13_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR13_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[13] = (MW_EPWMXBAR13_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR13_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR13_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR13_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR13_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR13_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR13_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR13_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR13_GPIO0_SEL == 1 | MW_EPWMXBAR13_GPIO1_SEL == 1 | MW_EPWMXBAR13_GPIO2_SEL == 1 | MW_EPWMXBAR13_GPIO3_SEL == 1 | MW_EPWMXBAR13_GPIO4_SEL == 1 | MW_EPWMXBAR13_GPIO5_SEL == 1 | MW_EPWMXBAR13_GPIO6_SEL == 1 | MW_EPWMXBAR13_GPIO7_SEL == 1| MW_EPWMXBAR13_GPIO8_SEL == 1 | MW_EPWMXBAR13_GPIO9_SEL == 1 | MW_EPWMXBAR13_GPIO10_SEL == 1| MW_EPWMXBAR13_GPIO11_SEL == 1| MW_EPWMXBAR13_GPIO12_SEL == 1 | MW_EPWMXBAR13_GPIO13_SEL == 1 | MW_EPWMXBAR13_GPIO14_SEL == 1 | MW_EPWMXBAR13_GPIO15_SEL == 1 | MW_EPWMXBAR13_GPIO16_SEL == 1 | MW_EPWMXBAR13_GPIO17_SEL == 1 | MW_EPWMXBAR13_GPIO18_SEL == 1 | MW_EPWMXBAR13_GPIO19_SEL == 1 | MW_EPWMXBAR13_GPIO20_SEL == 1 | MW_EPWMXBAR13_GPIO21_SEL == 1 | MW_EPWMXBAR13_GPIO22_SEL == 1 | MW_EPWMXBAR13_GPIO23_SEL == 1 | MW_EPWMXBAR13_GPIO24_SEL == 1 | MW_EPWMXBAR13_GPIO25_SEL == 1 | MW_EPWMXBAR13_GPIO26_SEL == 1 | MW_EPWMXBAR13_GPIO27_SEL == 1 | MW_EPWMXBAR13_GPIO28_SEL == 1 | MW_EPWMXBAR13_GPIO29_SEL == 1 | MW_EPWMXBAR13_GPIO30_SEL == 1 | MW_EPWMXBAR13_GPIO31_SEL == 1 | MW_EPWMXBAR13_GPIO32_SEL == 1 | MW_EPWMXBAR13_GPIO33_SEL == 1 | MW_EPWMXBAR13_GPIO34_SEL == 1 | MW_EPWMXBAR13_GPIO35_SEL == 1 | MW_EPWMXBAR13_GPIO36_SEL == 1 | MW_EPWMXBAR13_GPIO37_SEL == 1 | MW_EPWMXBAR13_GPIO38_SEL == 1 | MW_EPWMXBAR13_GPIO39_SEL == 1 | MW_EPWMXBAR13_GPIO40_SEL == 1 | MW_EPWMXBAR13_GPIO41_SEL == 1 | MW_EPWMXBAR13_GPIO42_SEL == 1 | MW_EPWMXBAR13_GPIO43_SEL == 1 | MW_EPWMXBAR13_GPIO44_SEL == 1 | | MW_EPWMXBAR13_GPIO45_SEL == 1 | MW_EPWMXBAR13_GPIO46_SEL == 1 | MW_EPWMXBAR13_GPIO47_SEL == 1| MW_EPWMXBAR13_GPIO48_SEL == 1 | MW_EPWMXBAR13_GPIO49_SEL == 1 | MW_EPWMXBAR13_GPIO50_SEL == 1 | MW_EPWMXBAR13_GPIO51_SEL == 1 | MW_EPWMXBAR13_GPIO52_SEL == 1 | MW_EPWMXBAR13_GPIO53_SEL == 1 | MW_EPWMXBAR13_GPIO54_SEL == 1 | MW_EPWMXBAR13_GPIO55_SEL == 1 | MW_EPWMXBAR13_GPIO56_SEL == 1 | MW_EPWMXBAR13_GPIO57_SEL == 1| MW_EPWMXBAR13_GPIO58_SEL == 1 | MW_EPWMXBAR13_GPIO59_SEL == 1 | MW_EPWMXBAR13_GPIO60_SEL == 1 | MW_EPWMXBAR13_GPIO61_SEL == 1 | MW_EPWMXBAR13_GPIO62_SEL == 1 | MW_EPWMXBAR13_GPIO63_SEL == 1 | MW_EPWMXBAR13_GPIO64_SEL == 1 | | MW_EPWMXBAR13_GPIO59_SEL == 1 | MW_EPWMXBAR13_GPIO60_SEL == 1 | MW_EPWMXBAR13_GPIO61_SEL == 1 | MW_EPWMXBAR13_GPIO62_SEL == 1 | MW_EPWMXBAR13_GPIO63_SEL == 1 | MW_EPWMXBAR13_GPIO64_SEL == 1 | MW_EPWMXBAR13_GPIO65_SEL == 1 | MW_EPWMXBAR13_GPIO66_SEL == 1 | MW_EPWMXBAR13_GPIO67_SEL == 1 | MW_EPWMXBAR13_GPIO68_SEL == 1 | MW_EPWMXBAR13_GPIO69_SEL == 1 | MW_EPWMXBAR13_GPIO70_SEL == 1 | MW_EPWMXBAR13_GPIO71_SEL == 1 | MW_EPWMXBAR13_GPIO72_SEL == 1 | MW_EPWMXBAR13_GPIO73_SEL == 1 | MW_EPWMXBAR13_GPIO74_SEL == 1 | MW_EPWMXBAR13_GPIO75_SEL == 1 | MW_EPWMXBAR13_GPIO76_SEL == 1 | MW_EPWMXBAR13_GPIO77_SEL == 1 | MW_EPWMXBAR13_GPIO78_SEL == 1 | | MW_EPWMXBAR13_GPIO80_SEL == 1 | MW_EPWMXBAR13_GPIO81_SEL == 1 | MW_EPWMXBAR13_GPIO82_SEL == 1 | MW_EPWMXBAR13_GPIO83_SEL == 1 | MW_EPWMXBAR13_GPIO84_SEL == 1 | MW_EPWMXBAR13_GPIO85_SEL == 1 | MW_EPWMXBAR13_GPIO86_SEL == 1 | MW_EPWMXBAR13_GPIO87_SEL == 1 | MW_EPWMXBAR13_GPIO88_SEL == 1 | MW_EPWMXBAR13_GPIO89_SEL == 1 | MW_EPWMXBAR13_GPIO90_SEL == 1 | MW_EPWMXBAR13_GPIO91_SEL == 1 | MW_EPWMXBAR13_GPIO92_SEL == 1 | MW_EPWMXBAR13_GPIO93_SEL == 1 | MW_EPWMXBAR13_GPIO94_SEL == 1 | MW_EPWMXBAR13_GPIO95_SEL == 1 | MW_EPWMXBAR13_GPIO96_SEL == 1 | MW_EPWMXBAR13_GPIO97_SEL == 1 | MW_EPWMXBAR13_GPIO98_SEL == 1 | MW_EPWMXBAR13_GPIO99_SEL == 1 | MW_EPWMXBAR13_GPIO100_SEL == 1 | MW_EPWMXBAR13_GPIO101_SEL == 1 | MW_EPWMXBAR13_GPIO102_SEL == 1 | MW_EPWMXBAR13_GPIO103_SEL == 1 | MW_EPWMXBAR13_GPIO104_SEL == 1 | MW_EPWMXBAR13_GPIO105_SEL == 1 | MW_EPWMXBAR13_GPIO106_SEL == 1 | MW_EPWMXBAR13_GPIO107_SEL == 1 | | MW_EPWMXBAR13_GPIO108_SEL == 1 | MW_EPWMXBAR13_GPIO109_SEL == 1| MW_EPWMXBAR13_GPIO110_SEL == 1 | MW_EPWMXBAR13_GPIO111_SEL == 1 | MW_EPWMXBAR13_GPIO112_SEL == 1 | MW_EPWMXBAR13_GPIO113_SEL == 1 | MW_EPWMXBAR13_GPIO114_SEL == 1 | MW_EPWMXBAR13_GPIO115_SEL == 1 | MW_EPWMXBAR13_GPIO116_SEL == 1 | MW_EPWMXBAR13_GPIO117_SEL == 1 | MW_EPWMXBAR13_GPIO118_SEL == 1 | MW_EPWMXBAR13_GPIO119_SEL == 1 | MW_EPWMXBAR13_GPIO120_SEL == 1 | MW_EPWMXBAR13_GPIO121_SEL == 1 | MW_EPWMXBAR13_GPIO122_SEL == 1 | MW_EPWMXBAR13_GPIO123_SEL == 1 | MW_EPWMXBAR13_GPIO124_SEL == 1 | MW_EPWMXBAR13_GPIO125_SEL == 1 | MW_EPWMXBAR13_GPIO126_SEL == 1 | MW_EPWMXBAR13_GPIO127_SEL == 1 | MW_EPWMXBAR13_GPIO128_SEL == 1 | MW_EPWMXBAR13_GPIO129_SEL == 1 | MW_EPWMXBAR13_GPIO130_SEL == 1 | MW_EPWMXBAR13_GPIO131_SEL == 1 | MW_EPWMXBAR13_GPIO132_SEL == 1 | MW_EPWMXBAR13_GPIO133_SEL == 1 | MW_EPWMXBAR13_GPIO134_SEL == 1 | MW_EPWMXBAR13_GPIO135_SEL == 1 | MW_EPWMXBAR13_GPIO136_SEL == 1 | MW_EPWMXBAR13_GPIO137_SEL == 1 | MW_EPWMXBAR13_GPIO139_SEL == 1 | MW_EPWMXBAR13_GPIO140_SEL == 1 | MW_EPWMXBAR13_GPIO141_SEL == 1 | MW_EPWMXBAR13_GPIO142_SEL == 1 | MW_EPWMXBAR13_GPIO143_SEL == 1 | MW_EPWMXBAR13_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR13_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR13_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[13] = EPWM_XBAR_13;
    // else
        group3_epwmxbar[13] = 0;
    group4_epwmxbar[13] = (MW_EPWMXBAR13_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR13_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR13_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR13_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR13_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR13_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR13_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR13_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR13_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR13_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[13] = (MW_EPWMXBAR13_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR13_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR13_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR13_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR13_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR13_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR13_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR13_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR13_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR13_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[13] = (MW_EPWMXBAR13_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR13_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR13_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR13_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR13_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR13_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR13_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR13_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR13_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR13_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[13] = (MW_EPWMXBAR13_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR13_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR13_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR13_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR13_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR13_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR13_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR13_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR13_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR13_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR13_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR13_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR13_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR13_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[13] = (MW_EPWMXBAR13_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR13_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR13_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR13_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[14] = (MW_EPWMXBAR14_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR14_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR14_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR14_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR14_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR14_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR14_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR14_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR14_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR14_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR14_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR14_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR14_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR14_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR14_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR14_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR14_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR14_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[14] = (MW_EPWMXBAR14_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR14_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR14_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR14_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR14_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR14_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR14_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR14_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR14_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR14_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR14_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR14_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR14_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR14_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR14_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR14_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR14_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR14_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR14_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR14_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR14_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR14_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR14_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR14_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[14] = (MW_EPWMXBAR14_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR14_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR14_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR14_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR14_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR14_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR14_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR14_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR14_GPIO0_SEL == 1 | MW_EPWMXBAR14_GPIO1_SEL == 1 | MW_EPWMXBAR14_GPIO2_SEL == 1 | MW_EPWMXBAR14_GPIO3_SEL == 1 | MW_EPWMXBAR14_GPIO4_SEL == 1 | MW_EPWMXBAR14_GPIO5_SEL == 1 | MW_EPWMXBAR14_GPIO6_SEL == 1 | MW_EPWMXBAR14_GPIO7_SEL == 1| MW_EPWMXBAR14_GPIO8_SEL == 1 | MW_EPWMXBAR14_GPIO9_SEL == 1 | MW_EPWMXBAR14_GPIO10_SEL == 1| MW_EPWMXBAR14_GPIO11_SEL == 1| MW_EPWMXBAR14_GPIO12_SEL == 1 | MW_EPWMXBAR14_GPIO13_SEL == 1 | MW_EPWMXBAR14_GPIO14_SEL == 1 | MW_EPWMXBAR14_GPIO15_SEL == 1 | MW_EPWMXBAR14_GPIO16_SEL == 1 | MW_EPWMXBAR14_GPIO17_SEL == 1 | MW_EPWMXBAR14_GPIO18_SEL == 1 | MW_EPWMXBAR14_GPIO19_SEL == 1 | MW_EPWMXBAR14_GPIO20_SEL == 1 | MW_EPWMXBAR14_GPIO21_SEL == 1 | MW_EPWMXBAR14_GPIO22_SEL == 1 | MW_EPWMXBAR14_GPIO23_SEL == 1 | MW_EPWMXBAR14_GPIO24_SEL == 1 | MW_EPWMXBAR14_GPIO25_SEL == 1 | MW_EPWMXBAR14_GPIO26_SEL == 1 | MW_EPWMXBAR14_GPIO27_SEL == 1 | MW_EPWMXBAR14_GPIO28_SEL == 1 | MW_EPWMXBAR14_GPIO29_SEL == 1 | MW_EPWMXBAR14_GPIO30_SEL == 1 | MW_EPWMXBAR14_GPIO31_SEL == 1 | MW_EPWMXBAR14_GPIO32_SEL == 1 | MW_EPWMXBAR14_GPIO33_SEL == 1 | MW_EPWMXBAR14_GPIO34_SEL == 1 | MW_EPWMXBAR14_GPIO35_SEL == 1 | MW_EPWMXBAR14_GPIO36_SEL == 1 | MW_EPWMXBAR14_GPIO37_SEL == 1 | MW_EPWMXBAR14_GPIO38_SEL == 1 | MW_EPWMXBAR14_GPIO39_SEL == 1 | MW_EPWMXBAR14_GPIO40_SEL == 1 | MW_EPWMXBAR14_GPIO41_SEL == 1 | MW_EPWMXBAR14_GPIO42_SEL == 1 | MW_EPWMXBAR14_GPIO43_SEL == 1 | MW_EPWMXBAR14_GPIO44_SEL == 1 | | MW_EPWMXBAR14_GPIO45_SEL == 1 | MW_EPWMXBAR14_GPIO46_SEL == 1 | MW_EPWMXBAR14_GPIO47_SEL == 1| MW_EPWMXBAR14_GPIO48_SEL == 1 | MW_EPWMXBAR14_GPIO49_SEL == 1 | MW_EPWMXBAR14_GPIO50_SEL == 1 | MW_EPWMXBAR14_GPIO51_SEL == 1 | MW_EPWMXBAR14_GPIO52_SEL == 1 | MW_EPWMXBAR14_GPIO53_SEL == 1 | MW_EPWMXBAR14_GPIO54_SEL == 1 | MW_EPWMXBAR14_GPIO55_SEL == 1 | MW_EPWMXBAR14_GPIO56_SEL == 1 | MW_EPWMXBAR14_GPIO57_SEL == 1| MW_EPWMXBAR14_GPIO58_SEL == 1 | MW_EPWMXBAR14_GPIO59_SEL == 1 | MW_EPWMXBAR14_GPIO60_SEL == 1 | MW_EPWMXBAR14_GPIO61_SEL == 1 | MW_EPWMXBAR14_GPIO62_SEL == 1 | MW_EPWMXBAR14_GPIO63_SEL == 1 | MW_EPWMXBAR14_GPIO64_SEL == 1 | | MW_EPWMXBAR14_GPIO59_SEL == 1 | MW_EPWMXBAR14_GPIO60_SEL == 1 | MW_EPWMXBAR14_GPIO61_SEL == 1 | MW_EPWMXBAR14_GPIO62_SEL == 1 | MW_EPWMXBAR14_GPIO63_SEL == 1 | MW_EPWMXBAR14_GPIO64_SEL == 1 | MW_EPWMXBAR14_GPIO65_SEL == 1 | MW_EPWMXBAR14_GPIO66_SEL == 1 | MW_EPWMXBAR14_GPIO67_SEL == 1 | MW_EPWMXBAR14_GPIO68_SEL == 1 | MW_EPWMXBAR14_GPIO69_SEL == 1 | MW_EPWMXBAR14_GPIO70_SEL == 1 | MW_EPWMXBAR14_GPIO71_SEL == 1 | MW_EPWMXBAR14_GPIO72_SEL == 1 | MW_EPWMXBAR14_GPIO73_SEL == 1 | MW_EPWMXBAR14_GPIO74_SEL == 1 | MW_EPWMXBAR14_GPIO75_SEL == 1 | MW_EPWMXBAR14_GPIO76_SEL == 1 | MW_EPWMXBAR14_GPIO77_SEL == 1 | MW_EPWMXBAR14_GPIO78_SEL == 1 | | MW_EPWMXBAR14_GPIO80_SEL == 1 | MW_EPWMXBAR14_GPIO81_SEL == 1 | MW_EPWMXBAR14_GPIO82_SEL == 1 | MW_EPWMXBAR14_GPIO83_SEL == 1 | MW_EPWMXBAR14_GPIO84_SEL == 1 | MW_EPWMXBAR14_GPIO85_SEL == 1 | MW_EPWMXBAR14_GPIO86_SEL == 1 | MW_EPWMXBAR14_GPIO87_SEL == 1 | MW_EPWMXBAR14_GPIO88_SEL == 1 | MW_EPWMXBAR14_GPIO89_SEL == 1 | MW_EPWMXBAR14_GPIO90_SEL == 1 | MW_EPWMXBAR14_GPIO91_SEL == 1 | MW_EPWMXBAR14_GPIO92_SEL == 1 | MW_EPWMXBAR14_GPIO93_SEL == 1 | MW_EPWMXBAR14_GPIO94_SEL == 1 | MW_EPWMXBAR14_GPIO95_SEL == 1 | MW_EPWMXBAR14_GPIO96_SEL == 1 | MW_EPWMXBAR14_GPIO97_SEL == 1 | MW_EPWMXBAR14_GPIO98_SEL == 1 | MW_EPWMXBAR14_GPIO99_SEL == 1 | MW_EPWMXBAR14_GPIO100_SEL == 1 | MW_EPWMXBAR14_GPIO101_SEL == 1 | MW_EPWMXBAR14_GPIO102_SEL == 1 | MW_EPWMXBAR14_GPIO103_SEL == 1 | MW_EPWMXBAR14_GPIO104_SEL == 1 | MW_EPWMXBAR14_GPIO105_SEL == 1 | MW_EPWMXBAR14_GPIO106_SEL == 1 | MW_EPWMXBAR14_GPIO107_SEL == 1 | | MW_EPWMXBAR14_GPIO108_SEL == 1 | MW_EPWMXBAR14_GPIO109_SEL == 1| MW_EPWMXBAR14_GPIO110_SEL == 1 | MW_EPWMXBAR14_GPIO111_SEL == 1 | MW_EPWMXBAR14_GPIO112_SEL == 1 | MW_EPWMXBAR14_GPIO113_SEL == 1 | MW_EPWMXBAR14_GPIO114_SEL == 1 | MW_EPWMXBAR14_GPIO115_SEL == 1 | MW_EPWMXBAR14_GPIO116_SEL == 1 | MW_EPWMXBAR14_GPIO117_SEL == 1 | MW_EPWMXBAR14_GPIO118_SEL == 1 | MW_EPWMXBAR14_GPIO119_SEL == 1 | MW_EPWMXBAR14_GPIO120_SEL == 1 | MW_EPWMXBAR14_GPIO121_SEL == 1 | MW_EPWMXBAR14_GPIO122_SEL == 1 | MW_EPWMXBAR14_GPIO123_SEL == 1 | MW_EPWMXBAR14_GPIO124_SEL == 1 | MW_EPWMXBAR14_GPIO125_SEL == 1 | MW_EPWMXBAR14_GPIO126_SEL == 1 | MW_EPWMXBAR14_GPIO127_SEL == 1 | MW_EPWMXBAR14_GPIO128_SEL == 1 | MW_EPWMXBAR14_GPIO129_SEL == 1 | MW_EPWMXBAR14_GPIO130_SEL == 1 | MW_EPWMXBAR14_GPIO131_SEL == 1 | MW_EPWMXBAR14_GPIO132_SEL == 1 | MW_EPWMXBAR14_GPIO133_SEL == 1 | MW_EPWMXBAR14_GPIO134_SEL == 1 | MW_EPWMXBAR14_GPIO135_SEL == 1 | MW_EPWMXBAR14_GPIO136_SEL == 1 | MW_EPWMXBAR14_GPIO137_SEL == 1 | MW_EPWMXBAR14_GPIO139_SEL == 1 | MW_EPWMXBAR14_GPIO140_SEL == 1 | MW_EPWMXBAR14_GPIO141_SEL == 1 | MW_EPWMXBAR14_GPIO142_SEL == 1 | MW_EPWMXBAR14_GPIO143_SEL == 1 | MW_EPWMXBAR14_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR14_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR14_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[14] = EPWM_XBAR_14;
    // else
        group3_epwmxbar[14] = 0;
    group4_epwmxbar[14] = (MW_EPWMXBAR14_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR14_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR14_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR14_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR14_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR14_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR14_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR14_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR14_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR14_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[14] = (MW_EPWMXBAR14_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR14_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR14_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR14_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR14_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR14_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR14_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR14_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR14_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR14_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[14] = (MW_EPWMXBAR14_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR14_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR14_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR14_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR14_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR14_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR14_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR14_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR14_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR14_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[14] = (MW_EPWMXBAR14_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR14_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR14_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR14_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR14_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR14_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR14_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR14_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR14_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR14_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR14_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR14_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR14_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR14_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[14] = (MW_EPWMXBAR14_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR14_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR14_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR14_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[15] = (MW_EPWMXBAR15_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR15_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR15_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR15_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR15_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR15_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR15_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR15_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR15_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR15_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR15_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR15_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR15_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR15_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR15_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR15_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR15_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR15_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[15] = (MW_EPWMXBAR15_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR15_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR15_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR15_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR15_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR15_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR15_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR15_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR15_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR15_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR15_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR15_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR15_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR15_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR15_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR15_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR15_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR15_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR15_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR15_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR15_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR15_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR15_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR15_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[15] = (MW_EPWMXBAR15_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR15_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR15_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR15_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR15_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR15_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR15_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR15_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR15_GPIO0_SEL == 1 | MW_EPWMXBAR15_GPIO1_SEL == 1 | MW_EPWMXBAR15_GPIO2_SEL == 1 | MW_EPWMXBAR15_GPIO3_SEL == 1 | MW_EPWMXBAR15_GPIO4_SEL == 1 | MW_EPWMXBAR15_GPIO5_SEL == 1 | MW_EPWMXBAR15_GPIO6_SEL == 1 | MW_EPWMXBAR15_GPIO7_SEL == 1| MW_EPWMXBAR15_GPIO8_SEL == 1 | MW_EPWMXBAR15_GPIO9_SEL == 1 | MW_EPWMXBAR15_GPIO10_SEL == 1| MW_EPWMXBAR15_GPIO11_SEL == 1| MW_EPWMXBAR15_GPIO12_SEL == 1 | MW_EPWMXBAR15_GPIO13_SEL == 1 | MW_EPWMXBAR15_GPIO14_SEL == 1 | MW_EPWMXBAR15_GPIO15_SEL == 1 | MW_EPWMXBAR15_GPIO16_SEL == 1 | MW_EPWMXBAR15_GPIO17_SEL == 1 | MW_EPWMXBAR15_GPIO18_SEL == 1 | MW_EPWMXBAR15_GPIO19_SEL == 1 | MW_EPWMXBAR15_GPIO20_SEL == 1 | MW_EPWMXBAR15_GPIO21_SEL == 1 | MW_EPWMXBAR15_GPIO22_SEL == 1 | MW_EPWMXBAR15_GPIO23_SEL == 1 | MW_EPWMXBAR15_GPIO24_SEL == 1 | MW_EPWMXBAR15_GPIO25_SEL == 1 | MW_EPWMXBAR15_GPIO26_SEL == 1 | MW_EPWMXBAR15_GPIO27_SEL == 1 | MW_EPWMXBAR15_GPIO28_SEL == 1 | MW_EPWMXBAR15_GPIO29_SEL == 1 | MW_EPWMXBAR15_GPIO30_SEL == 1 | MW_EPWMXBAR15_GPIO31_SEL == 1 | MW_EPWMXBAR15_GPIO32_SEL == 1 | MW_EPWMXBAR15_GPIO33_SEL == 1 | MW_EPWMXBAR15_GPIO34_SEL == 1 | MW_EPWMXBAR15_GPIO35_SEL == 1 | MW_EPWMXBAR15_GPIO36_SEL == 1 | MW_EPWMXBAR15_GPIO37_SEL == 1 | MW_EPWMXBAR15_GPIO38_SEL == 1 | MW_EPWMXBAR15_GPIO39_SEL == 1 | MW_EPWMXBAR15_GPIO40_SEL == 1 | MW_EPWMXBAR15_GPIO41_SEL == 1 | MW_EPWMXBAR15_GPIO42_SEL == 1 | MW_EPWMXBAR15_GPIO43_SEL == 1 | MW_EPWMXBAR15_GPIO44_SEL == 1 | | MW_EPWMXBAR15_GPIO45_SEL == 1 | MW_EPWMXBAR15_GPIO46_SEL == 1 | MW_EPWMXBAR15_GPIO47_SEL == 1| MW_EPWMXBAR15_GPIO48_SEL == 1 | MW_EPWMXBAR15_GPIO49_SEL == 1 | MW_EPWMXBAR15_GPIO50_SEL == 1 | MW_EPWMXBAR15_GPIO51_SEL == 1 | MW_EPWMXBAR15_GPIO52_SEL == 1 | MW_EPWMXBAR15_GPIO53_SEL == 1 | MW_EPWMXBAR15_GPIO54_SEL == 1 | MW_EPWMXBAR15_GPIO55_SEL == 1 | MW_EPWMXBAR15_GPIO56_SEL == 1 | MW_EPWMXBAR15_GPIO57_SEL == 1| MW_EPWMXBAR15_GPIO58_SEL == 1 | MW_EPWMXBAR15_GPIO59_SEL == 1 | MW_EPWMXBAR15_GPIO60_SEL == 1 | MW_EPWMXBAR15_GPIO61_SEL == 1 | MW_EPWMXBAR15_GPIO62_SEL == 1 | MW_EPWMXBAR15_GPIO63_SEL == 1 | MW_EPWMXBAR15_GPIO64_SEL == 1 | | MW_EPWMXBAR15_GPIO59_SEL == 1 | MW_EPWMXBAR15_GPIO60_SEL == 1 | MW_EPWMXBAR15_GPIO61_SEL == 1 | MW_EPWMXBAR15_GPIO62_SEL == 1 | MW_EPWMXBAR15_GPIO63_SEL == 1 | MW_EPWMXBAR15_GPIO64_SEL == 1 | MW_EPWMXBAR15_GPIO65_SEL == 1 | MW_EPWMXBAR15_GPIO66_SEL == 1 | MW_EPWMXBAR15_GPIO67_SEL == 1 | MW_EPWMXBAR15_GPIO68_SEL == 1 | MW_EPWMXBAR15_GPIO69_SEL == 1 | MW_EPWMXBAR15_GPIO70_SEL == 1 | MW_EPWMXBAR15_GPIO71_SEL == 1 | MW_EPWMXBAR15_GPIO72_SEL == 1 | MW_EPWMXBAR15_GPIO73_SEL == 1 | MW_EPWMXBAR15_GPIO74_SEL == 1 | MW_EPWMXBAR15_GPIO75_SEL == 1 | MW_EPWMXBAR15_GPIO76_SEL == 1 | MW_EPWMXBAR15_GPIO77_SEL == 1 | MW_EPWMXBAR15_GPIO78_SEL == 1 | | MW_EPWMXBAR15_GPIO80_SEL == 1 | MW_EPWMXBAR15_GPIO81_SEL == 1 | MW_EPWMXBAR15_GPIO82_SEL == 1 | MW_EPWMXBAR15_GPIO83_SEL == 1 | MW_EPWMXBAR15_GPIO84_SEL == 1 | MW_EPWMXBAR15_GPIO85_SEL == 1 | MW_EPWMXBAR15_GPIO86_SEL == 1 | MW_EPWMXBAR15_GPIO87_SEL == 1 | MW_EPWMXBAR15_GPIO88_SEL == 1 | MW_EPWMXBAR15_GPIO89_SEL == 1 | MW_EPWMXBAR15_GPIO90_SEL == 1 | MW_EPWMXBAR15_GPIO91_SEL == 1 | MW_EPWMXBAR15_GPIO92_SEL == 1 | MW_EPWMXBAR15_GPIO93_SEL == 1 | MW_EPWMXBAR15_GPIO94_SEL == 1 | MW_EPWMXBAR15_GPIO95_SEL == 1 | MW_EPWMXBAR15_GPIO96_SEL == 1 | MW_EPWMXBAR15_GPIO97_SEL == 1 | MW_EPWMXBAR15_GPIO98_SEL == 1 | MW_EPWMXBAR15_GPIO99_SEL == 1 | MW_EPWMXBAR15_GPIO100_SEL == 1 | MW_EPWMXBAR15_GPIO101_SEL == 1 | MW_EPWMXBAR15_GPIO102_SEL == 1 | MW_EPWMXBAR15_GPIO103_SEL == 1 | MW_EPWMXBAR15_GPIO104_SEL == 1 | MW_EPWMXBAR15_GPIO105_SEL == 1 | MW_EPWMXBAR15_GPIO106_SEL == 1 | MW_EPWMXBAR15_GPIO107_SEL == 1 | | MW_EPWMXBAR15_GPIO108_SEL == 1 | MW_EPWMXBAR15_GPIO109_SEL == 1| MW_EPWMXBAR15_GPIO110_SEL == 1 | MW_EPWMXBAR15_GPIO111_SEL == 1 | MW_EPWMXBAR15_GPIO112_SEL == 1 | MW_EPWMXBAR15_GPIO113_SEL == 1 | MW_EPWMXBAR15_GPIO114_SEL == 1 | MW_EPWMXBAR15_GPIO115_SEL == 1 | MW_EPWMXBAR15_GPIO116_SEL == 1 | MW_EPWMXBAR15_GPIO117_SEL == 1 | MW_EPWMXBAR15_GPIO118_SEL == 1 | MW_EPWMXBAR15_GPIO119_SEL == 1 | MW_EPWMXBAR15_GPIO120_SEL == 1 | MW_EPWMXBAR15_GPIO121_SEL == 1 | MW_EPWMXBAR15_GPIO122_SEL == 1 | MW_EPWMXBAR15_GPIO123_SEL == 1 | MW_EPWMXBAR15_GPIO124_SEL == 1 | MW_EPWMXBAR15_GPIO125_SEL == 1 | MW_EPWMXBAR15_GPIO126_SEL == 1 | MW_EPWMXBAR15_GPIO127_SEL == 1 | MW_EPWMXBAR15_GPIO128_SEL == 1 | MW_EPWMXBAR15_GPIO129_SEL == 1 | MW_EPWMXBAR15_GPIO130_SEL == 1 | MW_EPWMXBAR15_GPIO131_SEL == 1 | MW_EPWMXBAR15_GPIO132_SEL == 1 | MW_EPWMXBAR15_GPIO133_SEL == 1 | MW_EPWMXBAR15_GPIO134_SEL == 1 | MW_EPWMXBAR15_GPIO135_SEL == 1 | MW_EPWMXBAR15_GPIO136_SEL == 1 | MW_EPWMXBAR15_GPIO137_SEL == 1 | MW_EPWMXBAR15_GPIO139_SEL == 1 | MW_EPWMXBAR15_GPIO140_SEL == 1 | MW_EPWMXBAR15_GPIO141_SEL == 1 | MW_EPWMXBAR15_GPIO142_SEL == 1 | MW_EPWMXBAR15_GPIO143_SEL == 1 | MW_EPWMXBAR15_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR15_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR15_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[15] = EPWM_XBAR_15;
    // else
        group3_epwmxbar[15] = 0;
    group4_epwmxbar[15] = (MW_EPWMXBAR15_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR15_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR15_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR15_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR15_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR15_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR15_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR15_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR15_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR15_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[15] = (MW_EPWMXBAR15_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR15_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR15_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR15_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR15_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR15_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR15_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR15_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR15_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR15_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[15] = (MW_EPWMXBAR15_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR15_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR15_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR15_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR15_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR15_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR15_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR15_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR15_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR15_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[15] = (MW_EPWMXBAR15_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR15_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR15_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR15_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR15_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR15_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR15_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR15_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR15_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR15_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR15_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR15_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR15_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR15_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[15] = (MW_EPWMXBAR15_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR15_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR15_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR15_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[16] = (MW_EPWMXBAR16_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR16_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR16_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR16_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR16_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR16_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR16_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR16_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR16_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR16_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR16_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR16_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR16_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR16_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR16_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR16_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR16_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR16_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[16] = (MW_EPWMXBAR16_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR16_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR16_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR16_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR16_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR16_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR16_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR16_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR16_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR16_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR16_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR16_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR16_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR16_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR16_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR16_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR16_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR16_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR16_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR16_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR16_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR16_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR16_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR16_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[16] = (MW_EPWMXBAR16_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR16_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR16_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR16_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR16_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR16_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR16_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR16_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR16_GPIO0_SEL == 1 | MW_EPWMXBAR16_GPIO1_SEL == 1 | MW_EPWMXBAR16_GPIO2_SEL == 1 | MW_EPWMXBAR16_GPIO3_SEL == 1 | MW_EPWMXBAR16_GPIO4_SEL == 1 | MW_EPWMXBAR16_GPIO5_SEL == 1 | MW_EPWMXBAR16_GPIO6_SEL == 1 | MW_EPWMXBAR16_GPIO7_SEL == 1| MW_EPWMXBAR16_GPIO8_SEL == 1 | MW_EPWMXBAR16_GPIO9_SEL == 1 | MW_EPWMXBAR16_GPIO10_SEL == 1| MW_EPWMXBAR16_GPIO11_SEL == 1| MW_EPWMXBAR16_GPIO12_SEL == 1 | MW_EPWMXBAR16_GPIO13_SEL == 1 | MW_EPWMXBAR16_GPIO14_SEL == 1 | MW_EPWMXBAR16_GPIO15_SEL == 1 | MW_EPWMXBAR16_GPIO16_SEL == 1 | MW_EPWMXBAR16_GPIO17_SEL == 1 | MW_EPWMXBAR16_GPIO18_SEL == 1 | MW_EPWMXBAR16_GPIO19_SEL == 1 | MW_EPWMXBAR16_GPIO20_SEL == 1 | MW_EPWMXBAR16_GPIO21_SEL == 1 | MW_EPWMXBAR16_GPIO22_SEL == 1 | MW_EPWMXBAR16_GPIO23_SEL == 1 | MW_EPWMXBAR16_GPIO24_SEL == 1 | MW_EPWMXBAR16_GPIO25_SEL == 1 | MW_EPWMXBAR16_GPIO26_SEL == 1 | MW_EPWMXBAR16_GPIO27_SEL == 1 | MW_EPWMXBAR16_GPIO28_SEL == 1 | MW_EPWMXBAR16_GPIO29_SEL == 1 | MW_EPWMXBAR16_GPIO30_SEL == 1 | MW_EPWMXBAR16_GPIO31_SEL == 1 | MW_EPWMXBAR16_GPIO32_SEL == 1 | MW_EPWMXBAR16_GPIO33_SEL == 1 | MW_EPWMXBAR16_GPIO34_SEL == 1 | MW_EPWMXBAR16_GPIO35_SEL == 1 | MW_EPWMXBAR16_GPIO36_SEL == 1 | MW_EPWMXBAR16_GPIO37_SEL == 1 | MW_EPWMXBAR16_GPIO38_SEL == 1 | MW_EPWMXBAR16_GPIO39_SEL == 1 | MW_EPWMXBAR16_GPIO40_SEL == 1 | MW_EPWMXBAR16_GPIO41_SEL == 1 | MW_EPWMXBAR16_GPIO42_SEL == 1 | MW_EPWMXBAR16_GPIO43_SEL == 1 | MW_EPWMXBAR16_GPIO44_SEL == 1 | | MW_EPWMXBAR16_GPIO45_SEL == 1 | MW_EPWMXBAR16_GPIO46_SEL == 1 | MW_EPWMXBAR16_GPIO47_SEL == 1| MW_EPWMXBAR16_GPIO48_SEL == 1 | MW_EPWMXBAR16_GPIO49_SEL == 1 | MW_EPWMXBAR16_GPIO50_SEL == 1 | MW_EPWMXBAR16_GPIO51_SEL == 1 | MW_EPWMXBAR16_GPIO52_SEL == 1 | MW_EPWMXBAR16_GPIO53_SEL == 1 | MW_EPWMXBAR16_GPIO54_SEL == 1 | MW_EPWMXBAR16_GPIO55_SEL == 1 | MW_EPWMXBAR16_GPIO56_SEL == 1 | MW_EPWMXBAR16_GPIO57_SEL == 1| MW_EPWMXBAR16_GPIO58_SEL == 1 | MW_EPWMXBAR16_GPIO59_SEL == 1 | MW_EPWMXBAR16_GPIO60_SEL == 1 | MW_EPWMXBAR16_GPIO61_SEL == 1 | MW_EPWMXBAR16_GPIO62_SEL == 1 | MW_EPWMXBAR16_GPIO63_SEL == 1 | MW_EPWMXBAR16_GPIO64_SEL == 1 | | MW_EPWMXBAR16_GPIO59_SEL == 1 | MW_EPWMXBAR16_GPIO60_SEL == 1 | MW_EPWMXBAR16_GPIO61_SEL == 1 | MW_EPWMXBAR16_GPIO62_SEL == 1 | MW_EPWMXBAR16_GPIO63_SEL == 1 | MW_EPWMXBAR16_GPIO64_SEL == 1 | MW_EPWMXBAR16_GPIO65_SEL == 1 | MW_EPWMXBAR16_GPIO66_SEL == 1 | MW_EPWMXBAR16_GPIO67_SEL == 1 | MW_EPWMXBAR16_GPIO68_SEL == 1 | MW_EPWMXBAR16_GPIO69_SEL == 1 | MW_EPWMXBAR16_GPIO70_SEL == 1 | MW_EPWMXBAR16_GPIO71_SEL == 1 | MW_EPWMXBAR16_GPIO72_SEL == 1 | MW_EPWMXBAR16_GPIO73_SEL == 1 | MW_EPWMXBAR16_GPIO74_SEL == 1 | MW_EPWMXBAR16_GPIO75_SEL == 1 | MW_EPWMXBAR16_GPIO76_SEL == 1 | MW_EPWMXBAR16_GPIO77_SEL == 1 | MW_EPWMXBAR16_GPIO78_SEL == 1 | | MW_EPWMXBAR16_GPIO80_SEL == 1 | MW_EPWMXBAR16_GPIO81_SEL == 1 | MW_EPWMXBAR16_GPIO82_SEL == 1 | MW_EPWMXBAR16_GPIO83_SEL == 1 | MW_EPWMXBAR16_GPIO84_SEL == 1 | MW_EPWMXBAR16_GPIO85_SEL == 1 | MW_EPWMXBAR16_GPIO86_SEL == 1 | MW_EPWMXBAR16_GPIO87_SEL == 1 | MW_EPWMXBAR16_GPIO88_SEL == 1 | MW_EPWMXBAR16_GPIO89_SEL == 1 | MW_EPWMXBAR16_GPIO90_SEL == 1 | MW_EPWMXBAR16_GPIO91_SEL == 1 | MW_EPWMXBAR16_GPIO92_SEL == 1 | MW_EPWMXBAR16_GPIO93_SEL == 1 | MW_EPWMXBAR16_GPIO94_SEL == 1 | MW_EPWMXBAR16_GPIO95_SEL == 1 | MW_EPWMXBAR16_GPIO96_SEL == 1 | MW_EPWMXBAR16_GPIO97_SEL == 1 | MW_EPWMXBAR16_GPIO98_SEL == 1 | MW_EPWMXBAR16_GPIO99_SEL == 1 | MW_EPWMXBAR16_GPIO100_SEL == 1 | MW_EPWMXBAR16_GPIO101_SEL == 1 | MW_EPWMXBAR16_GPIO102_SEL == 1 | MW_EPWMXBAR16_GPIO103_SEL == 1 | MW_EPWMXBAR16_GPIO104_SEL == 1 | MW_EPWMXBAR16_GPIO105_SEL == 1 | MW_EPWMXBAR16_GPIO106_SEL == 1 | MW_EPWMXBAR16_GPIO107_SEL == 1 | | MW_EPWMXBAR16_GPIO108_SEL == 1 | MW_EPWMXBAR16_GPIO109_SEL == 1| MW_EPWMXBAR16_GPIO110_SEL == 1 | MW_EPWMXBAR16_GPIO111_SEL == 1 | MW_EPWMXBAR16_GPIO112_SEL == 1 | MW_EPWMXBAR16_GPIO113_SEL == 1 | MW_EPWMXBAR16_GPIO114_SEL == 1 | MW_EPWMXBAR16_GPIO115_SEL == 1 | MW_EPWMXBAR16_GPIO116_SEL == 1 | MW_EPWMXBAR16_GPIO117_SEL == 1 | MW_EPWMXBAR16_GPIO118_SEL == 1 | MW_EPWMXBAR16_GPIO119_SEL == 1 | MW_EPWMXBAR16_GPIO120_SEL == 1 | MW_EPWMXBAR16_GPIO121_SEL == 1 | MW_EPWMXBAR16_GPIO122_SEL == 1 | MW_EPWMXBAR16_GPIO123_SEL == 1 | MW_EPWMXBAR16_GPIO124_SEL == 1 | MW_EPWMXBAR16_GPIO125_SEL == 1 | MW_EPWMXBAR16_GPIO126_SEL == 1 | MW_EPWMXBAR16_GPIO127_SEL == 1 | MW_EPWMXBAR16_GPIO128_SEL == 1 | MW_EPWMXBAR16_GPIO129_SEL == 1 | MW_EPWMXBAR16_GPIO130_SEL == 1 | MW_EPWMXBAR16_GPIO131_SEL == 1 | MW_EPWMXBAR16_GPIO132_SEL == 1 | MW_EPWMXBAR16_GPIO133_SEL == 1 | MW_EPWMXBAR16_GPIO134_SEL == 1 | MW_EPWMXBAR16_GPIO135_SEL == 1 | MW_EPWMXBAR16_GPIO136_SEL == 1 | MW_EPWMXBAR16_GPIO137_SEL == 1 | MW_EPWMXBAR16_GPIO139_SEL == 1 | MW_EPWMXBAR16_GPIO140_SEL == 1 | MW_EPWMXBAR16_GPIO141_SEL == 1 | MW_EPWMXBAR16_GPIO142_SEL == 1 | MW_EPWMXBAR16_GPIO143_SEL == 1 | MW_EPWMXBAR16_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR16_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR16_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[16] = EPWM_XBAR_16;
    // else
        group3_epwmxbar[16] = 0;
    group4_epwmxbar[16] = (MW_EPWMXBAR16_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR16_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR16_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR16_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR16_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR16_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR16_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR16_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR16_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR16_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[16] = (MW_EPWMXBAR16_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR16_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR16_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR16_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR16_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR16_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR16_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR16_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR16_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR16_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[16] = (MW_EPWMXBAR16_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR16_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR16_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR16_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR16_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR16_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR16_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR16_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR16_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR16_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[16] = (MW_EPWMXBAR16_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR16_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR16_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR16_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR16_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR16_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR16_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR16_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR16_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR16_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR16_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR16_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR16_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR16_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[16] = (MW_EPWMXBAR16_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR16_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR16_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR16_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[17] = (MW_EPWMXBAR17_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR17_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR17_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR17_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR17_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR17_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR17_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR17_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR17_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR17_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR17_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR17_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR17_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR17_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR17_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR17_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR17_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR17_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[17] = (MW_EPWMXBAR17_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR17_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR17_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR17_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR17_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR17_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR17_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR17_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR17_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR17_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR17_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR17_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR17_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR17_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR17_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR17_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR17_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR17_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR17_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR17_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR17_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR17_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR17_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR17_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[17] = (MW_EPWMXBAR17_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR17_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR17_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR17_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR17_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR17_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR17_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR17_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR17_GPIO0_SEL == 1 | MW_EPWMXBAR17_GPIO1_SEL == 1 | MW_EPWMXBAR17_GPIO2_SEL == 1 | MW_EPWMXBAR17_GPIO3_SEL == 1 | MW_EPWMXBAR17_GPIO4_SEL == 1 | MW_EPWMXBAR17_GPIO5_SEL == 1 | MW_EPWMXBAR17_GPIO6_SEL == 1 | MW_EPWMXBAR17_GPIO7_SEL == 1| MW_EPWMXBAR17_GPIO8_SEL == 1 | MW_EPWMXBAR17_GPIO9_SEL == 1 | MW_EPWMXBAR17_GPIO10_SEL == 1| MW_EPWMXBAR17_GPIO11_SEL == 1| MW_EPWMXBAR17_GPIO12_SEL == 1 | MW_EPWMXBAR17_GPIO13_SEL == 1 | MW_EPWMXBAR17_GPIO14_SEL == 1 | MW_EPWMXBAR17_GPIO15_SEL == 1 | MW_EPWMXBAR17_GPIO16_SEL == 1 | MW_EPWMXBAR17_GPIO17_SEL == 1 | MW_EPWMXBAR17_GPIO18_SEL == 1 | MW_EPWMXBAR17_GPIO19_SEL == 1 | MW_EPWMXBAR17_GPIO20_SEL == 1 | MW_EPWMXBAR17_GPIO21_SEL == 1 | MW_EPWMXBAR17_GPIO22_SEL == 1 | MW_EPWMXBAR17_GPIO23_SEL == 1 | MW_EPWMXBAR17_GPIO24_SEL == 1 | MW_EPWMXBAR17_GPIO25_SEL == 1 | MW_EPWMXBAR17_GPIO26_SEL == 1 | MW_EPWMXBAR17_GPIO27_SEL == 1 | MW_EPWMXBAR17_GPIO28_SEL == 1 | MW_EPWMXBAR17_GPIO29_SEL == 1 | MW_EPWMXBAR17_GPIO30_SEL == 1 | MW_EPWMXBAR17_GPIO31_SEL == 1 | MW_EPWMXBAR17_GPIO32_SEL == 1 | MW_EPWMXBAR17_GPIO33_SEL == 1 | MW_EPWMXBAR17_GPIO34_SEL == 1 | MW_EPWMXBAR17_GPIO35_SEL == 1 | MW_EPWMXBAR17_GPIO36_SEL == 1 | MW_EPWMXBAR17_GPIO37_SEL == 1 | MW_EPWMXBAR17_GPIO38_SEL == 1 | MW_EPWMXBAR17_GPIO39_SEL == 1 | MW_EPWMXBAR17_GPIO40_SEL == 1 | MW_EPWMXBAR17_GPIO41_SEL == 1 | MW_EPWMXBAR17_GPIO42_SEL == 1 | MW_EPWMXBAR17_GPIO43_SEL == 1 | MW_EPWMXBAR17_GPIO44_SEL == 1 | | MW_EPWMXBAR17_GPIO45_SEL == 1 | MW_EPWMXBAR17_GPIO46_SEL == 1 | MW_EPWMXBAR17_GPIO47_SEL == 1| MW_EPWMXBAR17_GPIO48_SEL == 1 | MW_EPWMXBAR17_GPIO49_SEL == 1 | MW_EPWMXBAR17_GPIO50_SEL == 1 | MW_EPWMXBAR17_GPIO51_SEL == 1 | MW_EPWMXBAR17_GPIO52_SEL == 1 | MW_EPWMXBAR17_GPIO53_SEL == 1 | MW_EPWMXBAR17_GPIO54_SEL == 1 | MW_EPWMXBAR17_GPIO55_SEL == 1 | MW_EPWMXBAR17_GPIO56_SEL == 1 | MW_EPWMXBAR17_GPIO57_SEL == 1| MW_EPWMXBAR17_GPIO58_SEL == 1 | MW_EPWMXBAR17_GPIO59_SEL == 1 | MW_EPWMXBAR17_GPIO60_SEL == 1 | MW_EPWMXBAR17_GPIO61_SEL == 1 | MW_EPWMXBAR17_GPIO62_SEL == 1 | MW_EPWMXBAR17_GPIO63_SEL == 1 | MW_EPWMXBAR17_GPIO64_SEL == 1 | | MW_EPWMXBAR17_GPIO59_SEL == 1 | MW_EPWMXBAR17_GPIO60_SEL == 1 | MW_EPWMXBAR17_GPIO61_SEL == 1 | MW_EPWMXBAR17_GPIO62_SEL == 1 | MW_EPWMXBAR17_GPIO63_SEL == 1 | MW_EPWMXBAR17_GPIO64_SEL == 1 | MW_EPWMXBAR17_GPIO65_SEL == 1 | MW_EPWMXBAR17_GPIO66_SEL == 1 | MW_EPWMXBAR17_GPIO67_SEL == 1 | MW_EPWMXBAR17_GPIO68_SEL == 1 | MW_EPWMXBAR17_GPIO69_SEL == 1 | MW_EPWMXBAR17_GPIO70_SEL == 1 | MW_EPWMXBAR17_GPIO71_SEL == 1 | MW_EPWMXBAR17_GPIO72_SEL == 1 | MW_EPWMXBAR17_GPIO73_SEL == 1 | MW_EPWMXBAR17_GPIO74_SEL == 1 | MW_EPWMXBAR17_GPIO75_SEL == 1 | MW_EPWMXBAR17_GPIO76_SEL == 1 | MW_EPWMXBAR17_GPIO77_SEL == 1 | MW_EPWMXBAR17_GPIO78_SEL == 1 | | MW_EPWMXBAR17_GPIO80_SEL == 1 | MW_EPWMXBAR17_GPIO81_SEL == 1 | MW_EPWMXBAR17_GPIO82_SEL == 1 | MW_EPWMXBAR17_GPIO83_SEL == 1 | MW_EPWMXBAR17_GPIO84_SEL == 1 | MW_EPWMXBAR17_GPIO85_SEL == 1 | MW_EPWMXBAR17_GPIO86_SEL == 1 | MW_EPWMXBAR17_GPIO87_SEL == 1 | MW_EPWMXBAR17_GPIO88_SEL == 1 | MW_EPWMXBAR17_GPIO89_SEL == 1 | MW_EPWMXBAR17_GPIO90_SEL == 1 | MW_EPWMXBAR17_GPIO91_SEL == 1 | MW_EPWMXBAR17_GPIO92_SEL == 1 | MW_EPWMXBAR17_GPIO93_SEL == 1 | MW_EPWMXBAR17_GPIO94_SEL == 1 | MW_EPWMXBAR17_GPIO95_SEL == 1 | MW_EPWMXBAR17_GPIO96_SEL == 1 | MW_EPWMXBAR17_GPIO97_SEL == 1 | MW_EPWMXBAR17_GPIO98_SEL == 1 | MW_EPWMXBAR17_GPIO99_SEL == 1 | MW_EPWMXBAR17_GPIO100_SEL == 1 | MW_EPWMXBAR17_GPIO101_SEL == 1 | MW_EPWMXBAR17_GPIO102_SEL == 1 | MW_EPWMXBAR17_GPIO103_SEL == 1 | MW_EPWMXBAR17_GPIO104_SEL == 1 | MW_EPWMXBAR17_GPIO105_SEL == 1 | MW_EPWMXBAR17_GPIO106_SEL == 1 | MW_EPWMXBAR17_GPIO107_SEL == 1 | | MW_EPWMXBAR17_GPIO108_SEL == 1 | MW_EPWMXBAR17_GPIO109_SEL == 1| MW_EPWMXBAR17_GPIO110_SEL == 1 | MW_EPWMXBAR17_GPIO111_SEL == 1 | MW_EPWMXBAR17_GPIO112_SEL == 1 | MW_EPWMXBAR17_GPIO113_SEL == 1 | MW_EPWMXBAR17_GPIO114_SEL == 1 | MW_EPWMXBAR17_GPIO115_SEL == 1 | MW_EPWMXBAR17_GPIO116_SEL == 1 | MW_EPWMXBAR17_GPIO117_SEL == 1 | MW_EPWMXBAR17_GPIO118_SEL == 1 | MW_EPWMXBAR17_GPIO119_SEL == 1 | MW_EPWMXBAR17_GPIO120_SEL == 1 | MW_EPWMXBAR17_GPIO121_SEL == 1 | MW_EPWMXBAR17_GPIO122_SEL == 1 | MW_EPWMXBAR17_GPIO123_SEL == 1 | MW_EPWMXBAR17_GPIO124_SEL == 1 | MW_EPWMXBAR17_GPIO125_SEL == 1 | MW_EPWMXBAR17_GPIO126_SEL == 1 | MW_EPWMXBAR17_GPIO127_SEL == 1 | MW_EPWMXBAR17_GPIO128_SEL == 1 | MW_EPWMXBAR17_GPIO129_SEL == 1 | MW_EPWMXBAR17_GPIO130_SEL == 1 | MW_EPWMXBAR17_GPIO131_SEL == 1 | MW_EPWMXBAR17_GPIO132_SEL == 1 | MW_EPWMXBAR17_GPIO133_SEL == 1 | MW_EPWMXBAR17_GPIO134_SEL == 1 | MW_EPWMXBAR17_GPIO135_SEL == 1 | MW_EPWMXBAR17_GPIO136_SEL == 1 | MW_EPWMXBAR17_GPIO137_SEL == 1 | MW_EPWMXBAR17_GPIO139_SEL == 1 | MW_EPWMXBAR17_GPIO140_SEL == 1 | MW_EPWMXBAR17_GPIO141_SEL == 1 | MW_EPWMXBAR17_GPIO142_SEL == 1 | MW_EPWMXBAR17_GPIO143_SEL == 1 | MW_EPWMXBAR17_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR17_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR17_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[17] = EPWM_XBAR_17;
    // else
        group3_epwmxbar[17] = 0;
    group4_epwmxbar[17] = (MW_EPWMXBAR17_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR17_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR17_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR17_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR17_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR17_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR17_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR17_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR17_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR17_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[17] = (MW_EPWMXBAR17_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR17_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR17_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR17_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR17_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR17_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR17_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR17_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR17_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR17_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[17] = (MW_EPWMXBAR17_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR17_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR17_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR17_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR17_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR17_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR17_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR17_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR17_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR17_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[17] = (MW_EPWMXBAR17_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR17_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR17_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR17_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR17_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR17_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR17_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR17_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR17_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR17_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR17_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR17_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR17_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR17_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[17] = (MW_EPWMXBAR17_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR17_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR17_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR17_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[18] = (MW_EPWMXBAR18_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR18_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR18_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR18_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR18_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR18_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR18_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR18_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR18_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR18_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR18_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR18_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR18_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR18_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR18_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR18_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR18_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR18_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[18] = (MW_EPWMXBAR18_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR18_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR18_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR18_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR18_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR18_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR18_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR18_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR18_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR18_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR18_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR18_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR18_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR18_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR18_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR18_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR18_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR18_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR18_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR18_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR18_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR18_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR18_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR18_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[18] = (MW_EPWMXBAR18_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR18_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR18_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR18_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR18_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR18_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR18_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR18_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR18_GPIO0_SEL == 1 | MW_EPWMXBAR18_GPIO1_SEL == 1 | MW_EPWMXBAR18_GPIO2_SEL == 1 | MW_EPWMXBAR18_GPIO3_SEL == 1 | MW_EPWMXBAR18_GPIO4_SEL == 1 | MW_EPWMXBAR18_GPIO5_SEL == 1 | MW_EPWMXBAR18_GPIO6_SEL == 1 | MW_EPWMXBAR18_GPIO7_SEL == 1| MW_EPWMXBAR18_GPIO8_SEL == 1 | MW_EPWMXBAR18_GPIO9_SEL == 1 | MW_EPWMXBAR18_GPIO10_SEL == 1| MW_EPWMXBAR18_GPIO11_SEL == 1| MW_EPWMXBAR18_GPIO12_SEL == 1 | MW_EPWMXBAR18_GPIO13_SEL == 1 | MW_EPWMXBAR18_GPIO14_SEL == 1 | MW_EPWMXBAR18_GPIO15_SEL == 1 | MW_EPWMXBAR18_GPIO16_SEL == 1 | MW_EPWMXBAR18_GPIO17_SEL == 1 | MW_EPWMXBAR18_GPIO18_SEL == 1 | MW_EPWMXBAR18_GPIO19_SEL == 1 | MW_EPWMXBAR18_GPIO20_SEL == 1 | MW_EPWMXBAR18_GPIO21_SEL == 1 | MW_EPWMXBAR18_GPIO22_SEL == 1 | MW_EPWMXBAR18_GPIO23_SEL == 1 | MW_EPWMXBAR18_GPIO24_SEL == 1 | MW_EPWMXBAR18_GPIO25_SEL == 1 | MW_EPWMXBAR18_GPIO26_SEL == 1 | MW_EPWMXBAR18_GPIO27_SEL == 1 | MW_EPWMXBAR18_GPIO28_SEL == 1 | MW_EPWMXBAR18_GPIO29_SEL == 1 | MW_EPWMXBAR18_GPIO30_SEL == 1 | MW_EPWMXBAR18_GPIO31_SEL == 1 | MW_EPWMXBAR18_GPIO32_SEL == 1 | MW_EPWMXBAR18_GPIO33_SEL == 1 | MW_EPWMXBAR18_GPIO34_SEL == 1 | MW_EPWMXBAR18_GPIO35_SEL == 1 | MW_EPWMXBAR18_GPIO36_SEL == 1 | MW_EPWMXBAR18_GPIO37_SEL == 1 | MW_EPWMXBAR18_GPIO38_SEL == 1 | MW_EPWMXBAR18_GPIO39_SEL == 1 | MW_EPWMXBAR18_GPIO40_SEL == 1 | MW_EPWMXBAR18_GPIO41_SEL == 1 | MW_EPWMXBAR18_GPIO42_SEL == 1 | MW_EPWMXBAR18_GPIO43_SEL == 1 | MW_EPWMXBAR18_GPIO44_SEL == 1 | | MW_EPWMXBAR18_GPIO45_SEL == 1 | MW_EPWMXBAR18_GPIO46_SEL == 1 | MW_EPWMXBAR18_GPIO47_SEL == 1| MW_EPWMXBAR18_GPIO48_SEL == 1 | MW_EPWMXBAR18_GPIO49_SEL == 1 | MW_EPWMXBAR18_GPIO50_SEL == 1 | MW_EPWMXBAR18_GPIO51_SEL == 1 | MW_EPWMXBAR18_GPIO52_SEL == 1 | MW_EPWMXBAR18_GPIO53_SEL == 1 | MW_EPWMXBAR18_GPIO54_SEL == 1 | MW_EPWMXBAR18_GPIO55_SEL == 1 | MW_EPWMXBAR18_GPIO56_SEL == 1 | MW_EPWMXBAR18_GPIO57_SEL == 1| MW_EPWMXBAR18_GPIO58_SEL == 1 | MW_EPWMXBAR18_GPIO59_SEL == 1 | MW_EPWMXBAR18_GPIO60_SEL == 1 | MW_EPWMXBAR18_GPIO61_SEL == 1 | MW_EPWMXBAR18_GPIO62_SEL == 1 | MW_EPWMXBAR18_GPIO63_SEL == 1 | MW_EPWMXBAR18_GPIO64_SEL == 1 | | MW_EPWMXBAR18_GPIO59_SEL == 1 | MW_EPWMXBAR18_GPIO60_SEL == 1 | MW_EPWMXBAR18_GPIO61_SEL == 1 | MW_EPWMXBAR18_GPIO62_SEL == 1 | MW_EPWMXBAR18_GPIO63_SEL == 1 | MW_EPWMXBAR18_GPIO64_SEL == 1 | MW_EPWMXBAR18_GPIO65_SEL == 1 | MW_EPWMXBAR18_GPIO66_SEL == 1 | MW_EPWMXBAR18_GPIO67_SEL == 1 | MW_EPWMXBAR18_GPIO68_SEL == 1 | MW_EPWMXBAR18_GPIO69_SEL == 1 | MW_EPWMXBAR18_GPIO70_SEL == 1 | MW_EPWMXBAR18_GPIO71_SEL == 1 | MW_EPWMXBAR18_GPIO72_SEL == 1 | MW_EPWMXBAR18_GPIO73_SEL == 1 | MW_EPWMXBAR18_GPIO74_SEL == 1 | MW_EPWMXBAR18_GPIO75_SEL == 1 | MW_EPWMXBAR18_GPIO76_SEL == 1 | MW_EPWMXBAR18_GPIO77_SEL == 1 | MW_EPWMXBAR18_GPIO78_SEL == 1 | | MW_EPWMXBAR18_GPIO80_SEL == 1 | MW_EPWMXBAR18_GPIO81_SEL == 1 | MW_EPWMXBAR18_GPIO82_SEL == 1 | MW_EPWMXBAR18_GPIO83_SEL == 1 | MW_EPWMXBAR18_GPIO84_SEL == 1 | MW_EPWMXBAR18_GPIO85_SEL == 1 | MW_EPWMXBAR18_GPIO86_SEL == 1 | MW_EPWMXBAR18_GPIO87_SEL == 1 | MW_EPWMXBAR18_GPIO88_SEL == 1 | MW_EPWMXBAR18_GPIO89_SEL == 1 | MW_EPWMXBAR18_GPIO90_SEL == 1 | MW_EPWMXBAR18_GPIO91_SEL == 1 | MW_EPWMXBAR18_GPIO92_SEL == 1 | MW_EPWMXBAR18_GPIO93_SEL == 1 | MW_EPWMXBAR18_GPIO94_SEL == 1 | MW_EPWMXBAR18_GPIO95_SEL == 1 | MW_EPWMXBAR18_GPIO96_SEL == 1 | MW_EPWMXBAR18_GPIO97_SEL == 1 | MW_EPWMXBAR18_GPIO98_SEL == 1 | MW_EPWMXBAR18_GPIO99_SEL == 1 | MW_EPWMXBAR18_GPIO100_SEL == 1 | MW_EPWMXBAR18_GPIO101_SEL == 1 | MW_EPWMXBAR18_GPIO102_SEL == 1 | MW_EPWMXBAR18_GPIO103_SEL == 1 | MW_EPWMXBAR18_GPIO104_SEL == 1 | MW_EPWMXBAR18_GPIO105_SEL == 1 | MW_EPWMXBAR18_GPIO106_SEL == 1 | MW_EPWMXBAR18_GPIO107_SEL == 1 | | MW_EPWMXBAR18_GPIO108_SEL == 1 | MW_EPWMXBAR18_GPIO109_SEL == 1| MW_EPWMXBAR18_GPIO110_SEL == 1 | MW_EPWMXBAR18_GPIO111_SEL == 1 | MW_EPWMXBAR18_GPIO112_SEL == 1 | MW_EPWMXBAR18_GPIO113_SEL == 1 | MW_EPWMXBAR18_GPIO114_SEL == 1 | MW_EPWMXBAR18_GPIO115_SEL == 1 | MW_EPWMXBAR18_GPIO116_SEL == 1 | MW_EPWMXBAR18_GPIO117_SEL == 1 | MW_EPWMXBAR18_GPIO118_SEL == 1 | MW_EPWMXBAR18_GPIO119_SEL == 1 | MW_EPWMXBAR18_GPIO120_SEL == 1 | MW_EPWMXBAR18_GPIO121_SEL == 1 | MW_EPWMXBAR18_GPIO122_SEL == 1 | MW_EPWMXBAR18_GPIO123_SEL == 1 | MW_EPWMXBAR18_GPIO124_SEL == 1 | MW_EPWMXBAR18_GPIO125_SEL == 1 | MW_EPWMXBAR18_GPIO126_SEL == 1 | MW_EPWMXBAR18_GPIO127_SEL == 1 | MW_EPWMXBAR18_GPIO128_SEL == 1 | MW_EPWMXBAR18_GPIO129_SEL == 1 | MW_EPWMXBAR18_GPIO130_SEL == 1 | MW_EPWMXBAR18_GPIO131_SEL == 1 | MW_EPWMXBAR18_GPIO132_SEL == 1 | MW_EPWMXBAR18_GPIO133_SEL == 1 | MW_EPWMXBAR18_GPIO134_SEL == 1 | MW_EPWMXBAR18_GPIO135_SEL == 1 | MW_EPWMXBAR18_GPIO136_SEL == 1 | MW_EPWMXBAR18_GPIO137_SEL == 1 | MW_EPWMXBAR18_GPIO139_SEL == 1 | MW_EPWMXBAR18_GPIO140_SEL == 1 | MW_EPWMXBAR18_GPIO141_SEL == 1 | MW_EPWMXBAR18_GPIO142_SEL == 1 | MW_EPWMXBAR18_GPIO143_SEL == 1 | MW_EPWMXBAR18_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR18_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR18_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[18] = EPWM_XBAR_18;
    // else
        group3_epwmxbar[18] = 0;
    group4_epwmxbar[18] = (MW_EPWMXBAR18_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR18_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR18_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR18_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR18_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR18_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR18_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR18_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR18_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR18_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[18] = (MW_EPWMXBAR18_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR18_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR18_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR18_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR18_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR18_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR18_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR18_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR18_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR18_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[18] = (MW_EPWMXBAR18_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR18_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR18_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR18_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR18_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR18_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR18_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR18_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR18_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR18_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[18] = (MW_EPWMXBAR18_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR18_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR18_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR18_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR18_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR18_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR18_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR18_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR18_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR18_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR18_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR18_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR18_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR18_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[18] = (MW_EPWMXBAR18_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR18_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR18_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR18_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[19] = (MW_EPWMXBAR19_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR19_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR19_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR19_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR19_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR19_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR19_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR19_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR19_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR19_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR19_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR19_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR19_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR19_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR19_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR19_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR19_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR19_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[19] = (MW_EPWMXBAR19_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR19_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR19_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR19_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR19_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR19_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR19_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR19_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR19_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR19_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR19_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR19_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR19_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR19_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR19_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR19_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR19_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR19_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR19_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR19_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR19_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR19_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR19_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR19_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[19] = (MW_EPWMXBAR19_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR19_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR19_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR19_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR19_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR19_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR19_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR19_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR19_GPIO0_SEL == 1 | MW_EPWMXBAR19_GPIO1_SEL == 1 | MW_EPWMXBAR19_GPIO2_SEL == 1 | MW_EPWMXBAR19_GPIO3_SEL == 1 | MW_EPWMXBAR19_GPIO4_SEL == 1 | MW_EPWMXBAR19_GPIO5_SEL == 1 | MW_EPWMXBAR19_GPIO6_SEL == 1 | MW_EPWMXBAR19_GPIO7_SEL == 1| MW_EPWMXBAR19_GPIO8_SEL == 1 | MW_EPWMXBAR19_GPIO9_SEL == 1 | MW_EPWMXBAR19_GPIO10_SEL == 1| MW_EPWMXBAR19_GPIO11_SEL == 1| MW_EPWMXBAR19_GPIO12_SEL == 1 | MW_EPWMXBAR19_GPIO13_SEL == 1 | MW_EPWMXBAR19_GPIO14_SEL == 1 | MW_EPWMXBAR19_GPIO15_SEL == 1 | MW_EPWMXBAR19_GPIO16_SEL == 1 | MW_EPWMXBAR19_GPIO17_SEL == 1 | MW_EPWMXBAR19_GPIO18_SEL == 1 | MW_EPWMXBAR19_GPIO19_SEL == 1 | MW_EPWMXBAR19_GPIO20_SEL == 1 | MW_EPWMXBAR19_GPIO21_SEL == 1 | MW_EPWMXBAR19_GPIO22_SEL == 1 | MW_EPWMXBAR19_GPIO23_SEL == 1 | MW_EPWMXBAR19_GPIO24_SEL == 1 | MW_EPWMXBAR19_GPIO25_SEL == 1 | MW_EPWMXBAR19_GPIO26_SEL == 1 | MW_EPWMXBAR19_GPIO27_SEL == 1 | MW_EPWMXBAR19_GPIO28_SEL == 1 | MW_EPWMXBAR19_GPIO29_SEL == 1 | MW_EPWMXBAR19_GPIO30_SEL == 1 | MW_EPWMXBAR19_GPIO31_SEL == 1 | MW_EPWMXBAR19_GPIO32_SEL == 1 | MW_EPWMXBAR19_GPIO33_SEL == 1 | MW_EPWMXBAR19_GPIO34_SEL == 1 | MW_EPWMXBAR19_GPIO35_SEL == 1 | MW_EPWMXBAR19_GPIO36_SEL == 1 | MW_EPWMXBAR19_GPIO37_SEL == 1 | MW_EPWMXBAR19_GPIO38_SEL == 1 | MW_EPWMXBAR19_GPIO39_SEL == 1 | MW_EPWMXBAR19_GPIO40_SEL == 1 | MW_EPWMXBAR19_GPIO41_SEL == 1 | MW_EPWMXBAR19_GPIO42_SEL == 1 | MW_EPWMXBAR19_GPIO43_SEL == 1 | MW_EPWMXBAR19_GPIO44_SEL == 1 | | MW_EPWMXBAR19_GPIO45_SEL == 1 | MW_EPWMXBAR19_GPIO46_SEL == 1 | MW_EPWMXBAR19_GPIO47_SEL == 1| MW_EPWMXBAR19_GPIO48_SEL == 1 | MW_EPWMXBAR19_GPIO49_SEL == 1 | MW_EPWMXBAR19_GPIO50_SEL == 1 | MW_EPWMXBAR19_GPIO51_SEL == 1 | MW_EPWMXBAR19_GPIO52_SEL == 1 | MW_EPWMXBAR19_GPIO53_SEL == 1 | MW_EPWMXBAR19_GPIO54_SEL == 1 | MW_EPWMXBAR19_GPIO55_SEL == 1 | MW_EPWMXBAR19_GPIO56_SEL == 1 | MW_EPWMXBAR19_GPIO57_SEL == 1| MW_EPWMXBAR19_GPIO58_SEL == 1 | MW_EPWMXBAR19_GPIO59_SEL == 1 | MW_EPWMXBAR19_GPIO60_SEL == 1 | MW_EPWMXBAR19_GPIO61_SEL == 1 | MW_EPWMXBAR19_GPIO62_SEL == 1 | MW_EPWMXBAR19_GPIO63_SEL == 1 | MW_EPWMXBAR19_GPIO64_SEL == 1 | | MW_EPWMXBAR19_GPIO59_SEL == 1 | MW_EPWMXBAR19_GPIO60_SEL == 1 | MW_EPWMXBAR19_GPIO61_SEL == 1 | MW_EPWMXBAR19_GPIO62_SEL == 1 | MW_EPWMXBAR19_GPIO63_SEL == 1 | MW_EPWMXBAR19_GPIO64_SEL == 1 | MW_EPWMXBAR19_GPIO65_SEL == 1 | MW_EPWMXBAR19_GPIO66_SEL == 1 | MW_EPWMXBAR19_GPIO67_SEL == 1 | MW_EPWMXBAR19_GPIO68_SEL == 1 | MW_EPWMXBAR19_GPIO69_SEL == 1 | MW_EPWMXBAR19_GPIO70_SEL == 1 | MW_EPWMXBAR19_GPIO71_SEL == 1 | MW_EPWMXBAR19_GPIO72_SEL == 1 | MW_EPWMXBAR19_GPIO73_SEL == 1 | MW_EPWMXBAR19_GPIO74_SEL == 1 | MW_EPWMXBAR19_GPIO75_SEL == 1 | MW_EPWMXBAR19_GPIO76_SEL == 1 | MW_EPWMXBAR19_GPIO77_SEL == 1 | MW_EPWMXBAR19_GPIO78_SEL == 1 | | MW_EPWMXBAR19_GPIO80_SEL == 1 | MW_EPWMXBAR19_GPIO81_SEL == 1 | MW_EPWMXBAR19_GPIO82_SEL == 1 | MW_EPWMXBAR19_GPIO83_SEL == 1 | MW_EPWMXBAR19_GPIO84_SEL == 1 | MW_EPWMXBAR19_GPIO85_SEL == 1 | MW_EPWMXBAR19_GPIO86_SEL == 1 | MW_EPWMXBAR19_GPIO87_SEL == 1 | MW_EPWMXBAR19_GPIO88_SEL == 1 | MW_EPWMXBAR19_GPIO89_SEL == 1 | MW_EPWMXBAR19_GPIO90_SEL == 1 | MW_EPWMXBAR19_GPIO91_SEL == 1 | MW_EPWMXBAR19_GPIO92_SEL == 1 | MW_EPWMXBAR19_GPIO93_SEL == 1 | MW_EPWMXBAR19_GPIO94_SEL == 1 | MW_EPWMXBAR19_GPIO95_SEL == 1 | MW_EPWMXBAR19_GPIO96_SEL == 1 | MW_EPWMXBAR19_GPIO97_SEL == 1 | MW_EPWMXBAR19_GPIO98_SEL == 1 | MW_EPWMXBAR19_GPIO99_SEL == 1 | MW_EPWMXBAR19_GPIO100_SEL == 1 | MW_EPWMXBAR19_GPIO101_SEL == 1 | MW_EPWMXBAR19_GPIO102_SEL == 1 | MW_EPWMXBAR19_GPIO103_SEL == 1 | MW_EPWMXBAR19_GPIO104_SEL == 1 | MW_EPWMXBAR19_GPIO105_SEL == 1 | MW_EPWMXBAR19_GPIO106_SEL == 1 | MW_EPWMXBAR19_GPIO107_SEL == 1 | | MW_EPWMXBAR19_GPIO108_SEL == 1 | MW_EPWMXBAR19_GPIO109_SEL == 1| MW_EPWMXBAR19_GPIO110_SEL == 1 | MW_EPWMXBAR19_GPIO111_SEL == 1 | MW_EPWMXBAR19_GPIO112_SEL == 1 | MW_EPWMXBAR19_GPIO113_SEL == 1 | MW_EPWMXBAR19_GPIO114_SEL == 1 | MW_EPWMXBAR19_GPIO115_SEL == 1 | MW_EPWMXBAR19_GPIO116_SEL == 1 | MW_EPWMXBAR19_GPIO117_SEL == 1 | MW_EPWMXBAR19_GPIO118_SEL == 1 | MW_EPWMXBAR19_GPIO119_SEL == 1 | MW_EPWMXBAR19_GPIO120_SEL == 1 | MW_EPWMXBAR19_GPIO121_SEL == 1 | MW_EPWMXBAR19_GPIO122_SEL == 1 | MW_EPWMXBAR19_GPIO123_SEL == 1 | MW_EPWMXBAR19_GPIO124_SEL == 1 | MW_EPWMXBAR19_GPIO125_SEL == 1 | MW_EPWMXBAR19_GPIO126_SEL == 1 | MW_EPWMXBAR19_GPIO127_SEL == 1 | MW_EPWMXBAR19_GPIO128_SEL == 1 | MW_EPWMXBAR19_GPIO129_SEL == 1 | MW_EPWMXBAR19_GPIO130_SEL == 1 | MW_EPWMXBAR19_GPIO131_SEL == 1 | MW_EPWMXBAR19_GPIO132_SEL == 1 | MW_EPWMXBAR19_GPIO133_SEL == 1 | MW_EPWMXBAR19_GPIO134_SEL == 1 | MW_EPWMXBAR19_GPIO135_SEL == 1 | MW_EPWMXBAR19_GPIO136_SEL == 1 | MW_EPWMXBAR19_GPIO137_SEL == 1 | MW_EPWMXBAR19_GPIO139_SEL == 1 | MW_EPWMXBAR19_GPIO140_SEL == 1 | MW_EPWMXBAR19_GPIO141_SEL == 1 | MW_EPWMXBAR19_GPIO142_SEL == 1 | MW_EPWMXBAR19_GPIO143_SEL == 1 | MW_EPWMXBAR19_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR19_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR19_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[19] = EPWM_XBAR_19;
    // else
        group3_epwmxbar[19] = 0;
    group4_epwmxbar[19] = (MW_EPWMXBAR19_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR19_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR19_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR19_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR19_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR19_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR19_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR19_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR19_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR19_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[19] = (MW_EPWMXBAR19_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR19_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR19_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR19_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR19_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR19_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR19_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR19_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR19_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR19_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[19] = (MW_EPWMXBAR19_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR19_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR19_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR19_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR19_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR19_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR19_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR19_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR19_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR19_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[19] = (MW_EPWMXBAR19_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR19_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR19_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR19_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR19_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR19_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR19_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR19_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR19_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR19_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR19_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR19_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR19_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR19_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[19] = (MW_EPWMXBAR19_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR19_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR19_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR19_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[20] = (MW_EPWMXBAR20_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR20_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR20_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR20_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR20_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR20_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR20_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR20_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR20_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR20_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR20_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR20_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR20_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR20_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR20_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR20_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR20_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR20_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[20] = (MW_EPWMXBAR20_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR20_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR20_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR20_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR20_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR20_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR20_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR20_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR20_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR20_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR20_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR20_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR20_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR20_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR20_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR20_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR20_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR20_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR20_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR20_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR20_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR20_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR20_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR20_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[20] = (MW_EPWMXBAR20_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR20_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR20_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR20_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR20_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR20_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR20_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR20_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR20_GPIO0_SEL == 1 | MW_EPWMXBAR20_GPIO1_SEL == 1 | MW_EPWMXBAR20_GPIO2_SEL == 1 | MW_EPWMXBAR20_GPIO3_SEL == 1 | MW_EPWMXBAR20_GPIO4_SEL == 1 | MW_EPWMXBAR20_GPIO5_SEL == 1 | MW_EPWMXBAR20_GPIO6_SEL == 1 | MW_EPWMXBAR20_GPIO7_SEL == 1| MW_EPWMXBAR20_GPIO8_SEL == 1 | MW_EPWMXBAR20_GPIO9_SEL == 1 | MW_EPWMXBAR20_GPIO10_SEL == 1| MW_EPWMXBAR20_GPIO11_SEL == 1| MW_EPWMXBAR20_GPIO12_SEL == 1 | MW_EPWMXBAR20_GPIO13_SEL == 1 | MW_EPWMXBAR20_GPIO14_SEL == 1 | MW_EPWMXBAR20_GPIO15_SEL == 1 | MW_EPWMXBAR20_GPIO16_SEL == 1 | MW_EPWMXBAR20_GPIO17_SEL == 1 | MW_EPWMXBAR20_GPIO18_SEL == 1 | MW_EPWMXBAR20_GPIO19_SEL == 1 | MW_EPWMXBAR20_GPIO20_SEL == 1 | MW_EPWMXBAR20_GPIO21_SEL == 1 | MW_EPWMXBAR20_GPIO22_SEL == 1 | MW_EPWMXBAR20_GPIO23_SEL == 1 | MW_EPWMXBAR20_GPIO24_SEL == 1 | MW_EPWMXBAR20_GPIO25_SEL == 1 | MW_EPWMXBAR20_GPIO26_SEL == 1 | MW_EPWMXBAR20_GPIO27_SEL == 1 | MW_EPWMXBAR20_GPIO28_SEL == 1 | MW_EPWMXBAR20_GPIO29_SEL == 1 | MW_EPWMXBAR20_GPIO30_SEL == 1 | MW_EPWMXBAR20_GPIO31_SEL == 1 | MW_EPWMXBAR20_GPIO32_SEL == 1 | MW_EPWMXBAR20_GPIO33_SEL == 1 | MW_EPWMXBAR20_GPIO34_SEL == 1 | MW_EPWMXBAR20_GPIO35_SEL == 1 | MW_EPWMXBAR20_GPIO36_SEL == 1 | MW_EPWMXBAR20_GPIO37_SEL == 1 | MW_EPWMXBAR20_GPIO38_SEL == 1 | MW_EPWMXBAR20_GPIO39_SEL == 1 | MW_EPWMXBAR20_GPIO40_SEL == 1 | MW_EPWMXBAR20_GPIO41_SEL == 1 | MW_EPWMXBAR20_GPIO42_SEL == 1 | MW_EPWMXBAR20_GPIO43_SEL == 1 | MW_EPWMXBAR20_GPIO44_SEL == 1 | | MW_EPWMXBAR20_GPIO45_SEL == 1 | MW_EPWMXBAR20_GPIO46_SEL == 1 | MW_EPWMXBAR20_GPIO47_SEL == 1| MW_EPWMXBAR20_GPIO48_SEL == 1 | MW_EPWMXBAR20_GPIO49_SEL == 1 | MW_EPWMXBAR20_GPIO50_SEL == 1 | MW_EPWMXBAR20_GPIO51_SEL == 1 | MW_EPWMXBAR20_GPIO52_SEL == 1 | MW_EPWMXBAR20_GPIO53_SEL == 1 | MW_EPWMXBAR20_GPIO54_SEL == 1 | MW_EPWMXBAR20_GPIO55_SEL == 1 | MW_EPWMXBAR20_GPIO56_SEL == 1 | MW_EPWMXBAR20_GPIO57_SEL == 1| MW_EPWMXBAR20_GPIO58_SEL == 1 | MW_EPWMXBAR20_GPIO59_SEL == 1 | MW_EPWMXBAR20_GPIO60_SEL == 1 | MW_EPWMXBAR20_GPIO61_SEL == 1 | MW_EPWMXBAR20_GPIO62_SEL == 1 | MW_EPWMXBAR20_GPIO63_SEL == 1 | MW_EPWMXBAR20_GPIO64_SEL == 1 | | MW_EPWMXBAR20_GPIO59_SEL == 1 | MW_EPWMXBAR20_GPIO60_SEL == 1 | MW_EPWMXBAR20_GPIO61_SEL == 1 | MW_EPWMXBAR20_GPIO62_SEL == 1 | MW_EPWMXBAR20_GPIO63_SEL == 1 | MW_EPWMXBAR20_GPIO64_SEL == 1 | MW_EPWMXBAR20_GPIO65_SEL == 1 | MW_EPWMXBAR20_GPIO66_SEL == 1 | MW_EPWMXBAR20_GPIO67_SEL == 1 | MW_EPWMXBAR20_GPIO68_SEL == 1 | MW_EPWMXBAR20_GPIO69_SEL == 1 | MW_EPWMXBAR20_GPIO70_SEL == 1 | MW_EPWMXBAR20_GPIO71_SEL == 1 | MW_EPWMXBAR20_GPIO72_SEL == 1 | MW_EPWMXBAR20_GPIO73_SEL == 1 | MW_EPWMXBAR20_GPIO74_SEL == 1 | MW_EPWMXBAR20_GPIO75_SEL == 1 | MW_EPWMXBAR20_GPIO76_SEL == 1 | MW_EPWMXBAR20_GPIO77_SEL == 1 | MW_EPWMXBAR20_GPIO78_SEL == 1 | | MW_EPWMXBAR20_GPIO80_SEL == 1 | MW_EPWMXBAR20_GPIO81_SEL == 1 | MW_EPWMXBAR20_GPIO82_SEL == 1 | MW_EPWMXBAR20_GPIO83_SEL == 1 | MW_EPWMXBAR20_GPIO84_SEL == 1 | MW_EPWMXBAR20_GPIO85_SEL == 1 | MW_EPWMXBAR20_GPIO86_SEL == 1 | MW_EPWMXBAR20_GPIO87_SEL == 1 | MW_EPWMXBAR20_GPIO88_SEL == 1 | MW_EPWMXBAR20_GPIO89_SEL == 1 | MW_EPWMXBAR20_GPIO90_SEL == 1 | MW_EPWMXBAR20_GPIO91_SEL == 1 | MW_EPWMXBAR20_GPIO92_SEL == 1 | MW_EPWMXBAR20_GPIO93_SEL == 1 | MW_EPWMXBAR20_GPIO94_SEL == 1 | MW_EPWMXBAR20_GPIO95_SEL == 1 | MW_EPWMXBAR20_GPIO96_SEL == 1 | MW_EPWMXBAR20_GPIO97_SEL == 1 | MW_EPWMXBAR20_GPIO98_SEL == 1 | MW_EPWMXBAR20_GPIO99_SEL == 1 | MW_EPWMXBAR20_GPIO100_SEL == 1 | MW_EPWMXBAR20_GPIO101_SEL == 1 | MW_EPWMXBAR20_GPIO102_SEL == 1 | MW_EPWMXBAR20_GPIO103_SEL == 1 | MW_EPWMXBAR20_GPIO104_SEL == 1 | MW_EPWMXBAR20_GPIO105_SEL == 1 | MW_EPWMXBAR20_GPIO106_SEL == 1 | MW_EPWMXBAR20_GPIO107_SEL == 1 | | MW_EPWMXBAR20_GPIO108_SEL == 1 | MW_EPWMXBAR20_GPIO109_SEL == 1| MW_EPWMXBAR20_GPIO110_SEL == 1 | MW_EPWMXBAR20_GPIO111_SEL == 1 | MW_EPWMXBAR20_GPIO112_SEL == 1 | MW_EPWMXBAR20_GPIO113_SEL == 1 | MW_EPWMXBAR20_GPIO114_SEL == 1 | MW_EPWMXBAR20_GPIO115_SEL == 1 | MW_EPWMXBAR20_GPIO116_SEL == 1 | MW_EPWMXBAR20_GPIO117_SEL == 1 | MW_EPWMXBAR20_GPIO118_SEL == 1 | MW_EPWMXBAR20_GPIO119_SEL == 1 | MW_EPWMXBAR20_GPIO120_SEL == 1 | MW_EPWMXBAR20_GPIO121_SEL == 1 | MW_EPWMXBAR20_GPIO122_SEL == 1 | MW_EPWMXBAR20_GPIO123_SEL == 1 | MW_EPWMXBAR20_GPIO124_SEL == 1 | MW_EPWMXBAR20_GPIO125_SEL == 1 | MW_EPWMXBAR20_GPIO126_SEL == 1 | MW_EPWMXBAR20_GPIO127_SEL == 1 | MW_EPWMXBAR20_GPIO128_SEL == 1 | MW_EPWMXBAR20_GPIO129_SEL == 1 | MW_EPWMXBAR20_GPIO130_SEL == 1 | MW_EPWMXBAR20_GPIO131_SEL == 1 | MW_EPWMXBAR20_GPIO132_SEL == 1 | MW_EPWMXBAR20_GPIO133_SEL == 1 | MW_EPWMXBAR20_GPIO134_SEL == 1 | MW_EPWMXBAR20_GPIO135_SEL == 1 | MW_EPWMXBAR20_GPIO136_SEL == 1 | MW_EPWMXBAR20_GPIO137_SEL == 1 | MW_EPWMXBAR20_GPIO139_SEL == 1 | MW_EPWMXBAR20_GPIO140_SEL == 1 | MW_EPWMXBAR20_GPIO141_SEL == 1 | MW_EPWMXBAR20_GPIO142_SEL == 1 | MW_EPWMXBAR20_GPIO143_SEL == 1 | MW_EPWMXBAR20_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR20_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR20_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[20] = EPWM_XBAR_20;
    // else
        group3_epwmxbar[20] = 0;
    group4_epwmxbar[20] = (MW_EPWMXBAR20_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR20_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR20_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR20_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR20_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR20_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR20_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR20_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR20_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR20_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[20] = (MW_EPWMXBAR20_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR20_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR20_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR20_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR20_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR20_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR20_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR20_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR20_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR20_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[20] = (MW_EPWMXBAR20_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR20_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR20_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR20_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR20_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR20_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR20_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR20_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR20_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR20_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[20] = (MW_EPWMXBAR20_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR20_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR20_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR20_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR20_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR20_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR20_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR20_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR20_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR20_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR20_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR20_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR20_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR20_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[20] = (MW_EPWMXBAR20_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR20_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR20_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR20_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[21] = (MW_EPWMXBAR21_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR21_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR21_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR21_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR21_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR21_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR21_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR21_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR21_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR21_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR21_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR21_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR21_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR21_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR21_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR21_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR21_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR21_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[21] = (MW_EPWMXBAR21_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR21_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR21_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR21_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR21_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR21_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR21_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR21_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR21_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR21_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR21_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR21_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR21_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR21_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR21_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR21_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR21_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR21_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR21_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR21_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR21_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR21_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR21_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR21_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[21] = (MW_EPWMXBAR21_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR21_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR21_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR21_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR21_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR21_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR21_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR21_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR21_GPIO0_SEL == 1 | MW_EPWMXBAR21_GPIO1_SEL == 1 | MW_EPWMXBAR21_GPIO2_SEL == 1 | MW_EPWMXBAR21_GPIO3_SEL == 1 | MW_EPWMXBAR21_GPIO4_SEL == 1 | MW_EPWMXBAR21_GPIO5_SEL == 1 | MW_EPWMXBAR21_GPIO6_SEL == 1 | MW_EPWMXBAR21_GPIO7_SEL == 1| MW_EPWMXBAR21_GPIO8_SEL == 1 | MW_EPWMXBAR21_GPIO9_SEL == 1 | MW_EPWMXBAR21_GPIO10_SEL == 1| MW_EPWMXBAR21_GPIO11_SEL == 1| MW_EPWMXBAR21_GPIO12_SEL == 1 | MW_EPWMXBAR21_GPIO13_SEL == 1 | MW_EPWMXBAR21_GPIO14_SEL == 1 | MW_EPWMXBAR21_GPIO15_SEL == 1 | MW_EPWMXBAR21_GPIO16_SEL == 1 | MW_EPWMXBAR21_GPIO17_SEL == 1 | MW_EPWMXBAR21_GPIO18_SEL == 1 | MW_EPWMXBAR21_GPIO19_SEL == 1 | MW_EPWMXBAR21_GPIO20_SEL == 1 | MW_EPWMXBAR21_GPIO21_SEL == 1 | MW_EPWMXBAR21_GPIO22_SEL == 1 | MW_EPWMXBAR21_GPIO23_SEL == 1 | MW_EPWMXBAR21_GPIO24_SEL == 1 | MW_EPWMXBAR21_GPIO25_SEL == 1 | MW_EPWMXBAR21_GPIO26_SEL == 1 | MW_EPWMXBAR21_GPIO27_SEL == 1 | MW_EPWMXBAR21_GPIO28_SEL == 1 | MW_EPWMXBAR21_GPIO29_SEL == 1 | MW_EPWMXBAR21_GPIO30_SEL == 1 | MW_EPWMXBAR21_GPIO31_SEL == 1 | MW_EPWMXBAR21_GPIO32_SEL == 1 | MW_EPWMXBAR21_GPIO33_SEL == 1 | MW_EPWMXBAR21_GPIO34_SEL == 1 | MW_EPWMXBAR21_GPIO35_SEL == 1 | MW_EPWMXBAR21_GPIO36_SEL == 1 | MW_EPWMXBAR21_GPIO37_SEL == 1 | MW_EPWMXBAR21_GPIO38_SEL == 1 | MW_EPWMXBAR21_GPIO39_SEL == 1 | MW_EPWMXBAR21_GPIO40_SEL == 1 | MW_EPWMXBAR21_GPIO41_SEL == 1 | MW_EPWMXBAR21_GPIO42_SEL == 1 | MW_EPWMXBAR21_GPIO43_SEL == 1 | MW_EPWMXBAR21_GPIO44_SEL == 1 | | MW_EPWMXBAR21_GPIO45_SEL == 1 | MW_EPWMXBAR21_GPIO46_SEL == 1 | MW_EPWMXBAR21_GPIO47_SEL == 1| MW_EPWMXBAR21_GPIO48_SEL == 1 | MW_EPWMXBAR21_GPIO49_SEL == 1 | MW_EPWMXBAR21_GPIO50_SEL == 1 | MW_EPWMXBAR21_GPIO51_SEL == 1 | MW_EPWMXBAR21_GPIO52_SEL == 1 | MW_EPWMXBAR21_GPIO53_SEL == 1 | MW_EPWMXBAR21_GPIO54_SEL == 1 | MW_EPWMXBAR21_GPIO55_SEL == 1 | MW_EPWMXBAR21_GPIO56_SEL == 1 | MW_EPWMXBAR21_GPIO57_SEL == 1| MW_EPWMXBAR21_GPIO58_SEL == 1 | MW_EPWMXBAR21_GPIO59_SEL == 1 | MW_EPWMXBAR21_GPIO60_SEL == 1 | MW_EPWMXBAR21_GPIO61_SEL == 1 | MW_EPWMXBAR21_GPIO62_SEL == 1 | MW_EPWMXBAR21_GPIO63_SEL == 1 | MW_EPWMXBAR21_GPIO64_SEL == 1 | | MW_EPWMXBAR21_GPIO59_SEL == 1 | MW_EPWMXBAR21_GPIO60_SEL == 1 | MW_EPWMXBAR21_GPIO61_SEL == 1 | MW_EPWMXBAR21_GPIO62_SEL == 1 | MW_EPWMXBAR21_GPIO63_SEL == 1 | MW_EPWMXBAR21_GPIO64_SEL == 1 | MW_EPWMXBAR21_GPIO65_SEL == 1 | MW_EPWMXBAR21_GPIO66_SEL == 1 | MW_EPWMXBAR21_GPIO67_SEL == 1 | MW_EPWMXBAR21_GPIO68_SEL == 1 | MW_EPWMXBAR21_GPIO69_SEL == 1 | MW_EPWMXBAR21_GPIO70_SEL == 1 | MW_EPWMXBAR21_GPIO71_SEL == 1 | MW_EPWMXBAR21_GPIO72_SEL == 1 | MW_EPWMXBAR21_GPIO73_SEL == 1 | MW_EPWMXBAR21_GPIO74_SEL == 1 | MW_EPWMXBAR21_GPIO75_SEL == 1 | MW_EPWMXBAR21_GPIO76_SEL == 1 | MW_EPWMXBAR21_GPIO77_SEL == 1 | MW_EPWMXBAR21_GPIO78_SEL == 1 | | MW_EPWMXBAR21_GPIO80_SEL == 1 | MW_EPWMXBAR21_GPIO81_SEL == 1 | MW_EPWMXBAR21_GPIO82_SEL == 1 | MW_EPWMXBAR21_GPIO83_SEL == 1 | MW_EPWMXBAR21_GPIO84_SEL == 1 | MW_EPWMXBAR21_GPIO85_SEL == 1 | MW_EPWMXBAR21_GPIO86_SEL == 1 | MW_EPWMXBAR21_GPIO87_SEL == 1 | MW_EPWMXBAR21_GPIO88_SEL == 1 | MW_EPWMXBAR21_GPIO89_SEL == 1 | MW_EPWMXBAR21_GPIO90_SEL == 1 | MW_EPWMXBAR21_GPIO91_SEL == 1 | MW_EPWMXBAR21_GPIO92_SEL == 1 | MW_EPWMXBAR21_GPIO93_SEL == 1 | MW_EPWMXBAR21_GPIO94_SEL == 1 | MW_EPWMXBAR21_GPIO95_SEL == 1 | MW_EPWMXBAR21_GPIO96_SEL == 1 | MW_EPWMXBAR21_GPIO97_SEL == 1 | MW_EPWMXBAR21_GPIO98_SEL == 1 | MW_EPWMXBAR21_GPIO99_SEL == 1 | MW_EPWMXBAR21_GPIO100_SEL == 1 | MW_EPWMXBAR21_GPIO101_SEL == 1 | MW_EPWMXBAR21_GPIO102_SEL == 1 | MW_EPWMXBAR21_GPIO103_SEL == 1 | MW_EPWMXBAR21_GPIO104_SEL == 1 | MW_EPWMXBAR21_GPIO105_SEL == 1 | MW_EPWMXBAR21_GPIO106_SEL == 1 | MW_EPWMXBAR21_GPIO107_SEL == 1 | | MW_EPWMXBAR21_GPIO108_SEL == 1 | MW_EPWMXBAR21_GPIO109_SEL == 1| MW_EPWMXBAR21_GPIO110_SEL == 1 | MW_EPWMXBAR21_GPIO111_SEL == 1 | MW_EPWMXBAR21_GPIO112_SEL == 1 | MW_EPWMXBAR21_GPIO113_SEL == 1 | MW_EPWMXBAR21_GPIO114_SEL == 1 | MW_EPWMXBAR21_GPIO115_SEL == 1 | MW_EPWMXBAR21_GPIO116_SEL == 1 | MW_EPWMXBAR21_GPIO117_SEL == 1 | MW_EPWMXBAR21_GPIO118_SEL == 1 | MW_EPWMXBAR21_GPIO119_SEL == 1 | MW_EPWMXBAR21_GPIO120_SEL == 1 | MW_EPWMXBAR21_GPIO121_SEL == 1 | MW_EPWMXBAR21_GPIO122_SEL == 1 | MW_EPWMXBAR21_GPIO123_SEL == 1 | MW_EPWMXBAR21_GPIO124_SEL == 1 | MW_EPWMXBAR21_GPIO125_SEL == 1 | MW_EPWMXBAR21_GPIO126_SEL == 1 | MW_EPWMXBAR21_GPIO127_SEL == 1 | MW_EPWMXBAR21_GPIO128_SEL == 1 | MW_EPWMXBAR21_GPIO129_SEL == 1 | MW_EPWMXBAR21_GPIO130_SEL == 1 | MW_EPWMXBAR21_GPIO131_SEL == 1 | MW_EPWMXBAR21_GPIO132_SEL == 1 | MW_EPWMXBAR21_GPIO133_SEL == 1 | MW_EPWMXBAR21_GPIO134_SEL == 1 | MW_EPWMXBAR21_GPIO135_SEL == 1 | MW_EPWMXBAR21_GPIO136_SEL == 1 | MW_EPWMXBAR21_GPIO137_SEL == 1 | MW_EPWMXBAR21_GPIO139_SEL == 1 | MW_EPWMXBAR21_GPIO140_SEL == 1 | MW_EPWMXBAR21_GPIO141_SEL == 1 | MW_EPWMXBAR21_GPIO142_SEL == 1 | MW_EPWMXBAR21_GPIO143_SEL == 1 | MW_EPWMXBAR21_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR21_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR21_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[21] = EPWM_XBAR_21;
    // else
        group3_epwmxbar[21] = 0;
    group4_epwmxbar[21] = (MW_EPWMXBAR21_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR21_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR21_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR21_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR21_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR21_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR21_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR21_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR21_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR21_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[21] = (MW_EPWMXBAR21_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR21_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR21_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR21_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR21_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR21_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR21_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR21_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR21_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR21_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[21] = (MW_EPWMXBAR21_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR21_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR21_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR21_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR21_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR21_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR21_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR21_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR21_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR21_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[21] = (MW_EPWMXBAR21_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR21_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR21_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR21_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR21_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR21_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR21_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR21_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR21_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR21_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR21_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR21_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR21_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR21_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[21] = (MW_EPWMXBAR21_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR21_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR21_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR21_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[22] = (MW_EPWMXBAR22_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR22_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR22_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR22_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR22_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR22_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR22_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR22_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR22_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR22_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR22_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR22_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR22_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR22_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR22_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR22_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR22_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR22_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[22] = (MW_EPWMXBAR22_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR22_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR22_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR22_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR22_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR22_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR22_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR22_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR22_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR22_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR22_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR22_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR22_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR22_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR22_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR22_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR22_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR22_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR22_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR22_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR22_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR22_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR22_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR22_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[22] = (MW_EPWMXBAR22_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR22_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR22_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR22_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR22_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR22_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR22_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR22_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR22_GPIO0_SEL == 1 | MW_EPWMXBAR22_GPIO1_SEL == 1 | MW_EPWMXBAR22_GPIO2_SEL == 1 | MW_EPWMXBAR22_GPIO3_SEL == 1 | MW_EPWMXBAR22_GPIO4_SEL == 1 | MW_EPWMXBAR22_GPIO5_SEL == 1 | MW_EPWMXBAR22_GPIO6_SEL == 1 | MW_EPWMXBAR22_GPIO7_SEL == 1| MW_EPWMXBAR22_GPIO8_SEL == 1 | MW_EPWMXBAR22_GPIO9_SEL == 1 | MW_EPWMXBAR22_GPIO10_SEL == 1| MW_EPWMXBAR22_GPIO11_SEL == 1| MW_EPWMXBAR22_GPIO12_SEL == 1 | MW_EPWMXBAR22_GPIO13_SEL == 1 | MW_EPWMXBAR22_GPIO14_SEL == 1 | MW_EPWMXBAR22_GPIO15_SEL == 1 | MW_EPWMXBAR22_GPIO16_SEL == 1 | MW_EPWMXBAR22_GPIO17_SEL == 1 | MW_EPWMXBAR22_GPIO18_SEL == 1 | MW_EPWMXBAR22_GPIO19_SEL == 1 | MW_EPWMXBAR22_GPIO20_SEL == 1 | MW_EPWMXBAR22_GPIO21_SEL == 1 | MW_EPWMXBAR22_GPIO22_SEL == 1 | MW_EPWMXBAR22_GPIO23_SEL == 1 | MW_EPWMXBAR22_GPIO24_SEL == 1 | MW_EPWMXBAR22_GPIO25_SEL == 1 | MW_EPWMXBAR22_GPIO26_SEL == 1 | MW_EPWMXBAR22_GPIO27_SEL == 1 | MW_EPWMXBAR22_GPIO28_SEL == 1 | MW_EPWMXBAR22_GPIO29_SEL == 1 | MW_EPWMXBAR22_GPIO30_SEL == 1 | MW_EPWMXBAR22_GPIO31_SEL == 1 | MW_EPWMXBAR22_GPIO32_SEL == 1 | MW_EPWMXBAR22_GPIO33_SEL == 1 | MW_EPWMXBAR22_GPIO34_SEL == 1 | MW_EPWMXBAR22_GPIO35_SEL == 1 | MW_EPWMXBAR22_GPIO36_SEL == 1 | MW_EPWMXBAR22_GPIO37_SEL == 1 | MW_EPWMXBAR22_GPIO38_SEL == 1 | MW_EPWMXBAR22_GPIO39_SEL == 1 | MW_EPWMXBAR22_GPIO40_SEL == 1 | MW_EPWMXBAR22_GPIO41_SEL == 1 | MW_EPWMXBAR22_GPIO42_SEL == 1 | MW_EPWMXBAR22_GPIO43_SEL == 1 | MW_EPWMXBAR22_GPIO44_SEL == 1 | | MW_EPWMXBAR22_GPIO45_SEL == 1 | MW_EPWMXBAR22_GPIO46_SEL == 1 | MW_EPWMXBAR22_GPIO47_SEL == 1| MW_EPWMXBAR22_GPIO48_SEL == 1 | MW_EPWMXBAR22_GPIO49_SEL == 1 | MW_EPWMXBAR22_GPIO50_SEL == 1 | MW_EPWMXBAR22_GPIO51_SEL == 1 | MW_EPWMXBAR22_GPIO52_SEL == 1 | MW_EPWMXBAR22_GPIO53_SEL == 1 | MW_EPWMXBAR22_GPIO54_SEL == 1 | MW_EPWMXBAR22_GPIO55_SEL == 1 | MW_EPWMXBAR22_GPIO56_SEL == 1 | MW_EPWMXBAR22_GPIO57_SEL == 1| MW_EPWMXBAR22_GPIO58_SEL == 1 | MW_EPWMXBAR22_GPIO59_SEL == 1 | MW_EPWMXBAR22_GPIO60_SEL == 1 | MW_EPWMXBAR22_GPIO61_SEL == 1 | MW_EPWMXBAR22_GPIO62_SEL == 1 | MW_EPWMXBAR22_GPIO63_SEL == 1 | MW_EPWMXBAR22_GPIO64_SEL == 1 | | MW_EPWMXBAR22_GPIO59_SEL == 1 | MW_EPWMXBAR22_GPIO60_SEL == 1 | MW_EPWMXBAR22_GPIO61_SEL == 1 | MW_EPWMXBAR22_GPIO62_SEL == 1 | MW_EPWMXBAR22_GPIO63_SEL == 1 | MW_EPWMXBAR22_GPIO64_SEL == 1 | MW_EPWMXBAR22_GPIO65_SEL == 1 | MW_EPWMXBAR22_GPIO66_SEL == 1 | MW_EPWMXBAR22_GPIO67_SEL == 1 | MW_EPWMXBAR22_GPIO68_SEL == 1 | MW_EPWMXBAR22_GPIO69_SEL == 1 | MW_EPWMXBAR22_GPIO70_SEL == 1 | MW_EPWMXBAR22_GPIO71_SEL == 1 | MW_EPWMXBAR22_GPIO72_SEL == 1 | MW_EPWMXBAR22_GPIO73_SEL == 1 | MW_EPWMXBAR22_GPIO74_SEL == 1 | MW_EPWMXBAR22_GPIO75_SEL == 1 | MW_EPWMXBAR22_GPIO76_SEL == 1 | MW_EPWMXBAR22_GPIO77_SEL == 1 | MW_EPWMXBAR22_GPIO78_SEL == 1 | | MW_EPWMXBAR22_GPIO80_SEL == 1 | MW_EPWMXBAR22_GPIO81_SEL == 1 | MW_EPWMXBAR22_GPIO82_SEL == 1 | MW_EPWMXBAR22_GPIO83_SEL == 1 | MW_EPWMXBAR22_GPIO84_SEL == 1 | MW_EPWMXBAR22_GPIO85_SEL == 1 | MW_EPWMXBAR22_GPIO86_SEL == 1 | MW_EPWMXBAR22_GPIO87_SEL == 1 | MW_EPWMXBAR22_GPIO88_SEL == 1 | MW_EPWMXBAR22_GPIO89_SEL == 1 | MW_EPWMXBAR22_GPIO90_SEL == 1 | MW_EPWMXBAR22_GPIO91_SEL == 1 | MW_EPWMXBAR22_GPIO92_SEL == 1 | MW_EPWMXBAR22_GPIO93_SEL == 1 | MW_EPWMXBAR22_GPIO94_SEL == 1 | MW_EPWMXBAR22_GPIO95_SEL == 1 | MW_EPWMXBAR22_GPIO96_SEL == 1 | MW_EPWMXBAR22_GPIO97_SEL == 1 | MW_EPWMXBAR22_GPIO98_SEL == 1 | MW_EPWMXBAR22_GPIO99_SEL == 1 | MW_EPWMXBAR22_GPIO100_SEL == 1 | MW_EPWMXBAR22_GPIO101_SEL == 1 | MW_EPWMXBAR22_GPIO102_SEL == 1 | MW_EPWMXBAR22_GPIO103_SEL == 1 | MW_EPWMXBAR22_GPIO104_SEL == 1 | MW_EPWMXBAR22_GPIO105_SEL == 1 | MW_EPWMXBAR22_GPIO106_SEL == 1 | MW_EPWMXBAR22_GPIO107_SEL == 1 | | MW_EPWMXBAR22_GPIO108_SEL == 1 | MW_EPWMXBAR22_GPIO109_SEL == 1| MW_EPWMXBAR22_GPIO110_SEL == 1 | MW_EPWMXBAR22_GPIO111_SEL == 1 | MW_EPWMXBAR22_GPIO112_SEL == 1 | MW_EPWMXBAR22_GPIO113_SEL == 1 | MW_EPWMXBAR22_GPIO114_SEL == 1 | MW_EPWMXBAR22_GPIO115_SEL == 1 | MW_EPWMXBAR22_GPIO116_SEL == 1 | MW_EPWMXBAR22_GPIO117_SEL == 1 | MW_EPWMXBAR22_GPIO118_SEL == 1 | MW_EPWMXBAR22_GPIO119_SEL == 1 | MW_EPWMXBAR22_GPIO120_SEL == 1 | MW_EPWMXBAR22_GPIO121_SEL == 1 | MW_EPWMXBAR22_GPIO122_SEL == 1 | MW_EPWMXBAR22_GPIO123_SEL == 1 | MW_EPWMXBAR22_GPIO124_SEL == 1 | MW_EPWMXBAR22_GPIO125_SEL == 1 | MW_EPWMXBAR22_GPIO126_SEL == 1 | MW_EPWMXBAR22_GPIO127_SEL == 1 | MW_EPWMXBAR22_GPIO128_SEL == 1 | MW_EPWMXBAR22_GPIO129_SEL == 1 | MW_EPWMXBAR22_GPIO130_SEL == 1 | MW_EPWMXBAR22_GPIO131_SEL == 1 | MW_EPWMXBAR22_GPIO132_SEL == 1 | MW_EPWMXBAR22_GPIO133_SEL == 1 | MW_EPWMXBAR22_GPIO134_SEL == 1 | MW_EPWMXBAR22_GPIO135_SEL == 1 | MW_EPWMXBAR22_GPIO136_SEL == 1 | MW_EPWMXBAR22_GPIO137_SEL == 1 | MW_EPWMXBAR22_GPIO139_SEL == 1 | MW_EPWMXBAR22_GPIO140_SEL == 1 | MW_EPWMXBAR22_GPIO141_SEL == 1 | MW_EPWMXBAR22_GPIO142_SEL == 1 | MW_EPWMXBAR22_GPIO143_SEL == 1 | MW_EPWMXBAR22_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR22_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR22_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[22] = EPWM_XBAR_22;
    // else
        group3_epwmxbar[22] = 0;
    group4_epwmxbar[22] = (MW_EPWMXBAR22_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR22_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR22_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR22_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR22_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR22_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR22_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR22_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR22_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR22_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[22] = (MW_EPWMXBAR22_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR22_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR22_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR22_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR22_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR22_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR22_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR22_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR22_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR22_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[22] = (MW_EPWMXBAR22_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR22_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR22_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR22_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR22_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR22_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR22_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR22_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR22_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR22_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[22] = (MW_EPWMXBAR22_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR22_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR22_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR22_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR22_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR22_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR22_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR22_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR22_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR22_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR22_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR22_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR22_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR22_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[22] = (MW_EPWMXBAR22_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR22_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR22_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR22_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[23] = (MW_EPWMXBAR23_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR23_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR23_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR23_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR23_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR23_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR23_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR23_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR23_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR23_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR23_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR23_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR23_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR23_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR23_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR23_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR23_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR23_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[23] = (MW_EPWMXBAR23_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR23_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR23_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR23_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR23_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR23_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR23_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR23_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR23_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR23_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR23_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR23_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR23_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR23_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR23_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR23_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR23_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR23_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR23_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR23_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR23_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR23_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR23_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR23_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[23] = (MW_EPWMXBAR23_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR23_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR23_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR23_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR23_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR23_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR23_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR23_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR23_GPIO0_SEL == 1 | MW_EPWMXBAR23_GPIO1_SEL == 1 | MW_EPWMXBAR23_GPIO2_SEL == 1 | MW_EPWMXBAR23_GPIO3_SEL == 1 | MW_EPWMXBAR23_GPIO4_SEL == 1 | MW_EPWMXBAR23_GPIO5_SEL == 1 | MW_EPWMXBAR23_GPIO6_SEL == 1 | MW_EPWMXBAR23_GPIO7_SEL == 1| MW_EPWMXBAR23_GPIO8_SEL == 1 | MW_EPWMXBAR23_GPIO9_SEL == 1 | MW_EPWMXBAR23_GPIO10_SEL == 1| MW_EPWMXBAR23_GPIO11_SEL == 1| MW_EPWMXBAR23_GPIO12_SEL == 1 | MW_EPWMXBAR23_GPIO13_SEL == 1 | MW_EPWMXBAR23_GPIO14_SEL == 1 | MW_EPWMXBAR23_GPIO15_SEL == 1 | MW_EPWMXBAR23_GPIO16_SEL == 1 | MW_EPWMXBAR23_GPIO17_SEL == 1 | MW_EPWMXBAR23_GPIO18_SEL == 1 | MW_EPWMXBAR23_GPIO19_SEL == 1 | MW_EPWMXBAR23_GPIO20_SEL == 1 | MW_EPWMXBAR23_GPIO21_SEL == 1 | MW_EPWMXBAR23_GPIO22_SEL == 1 | MW_EPWMXBAR23_GPIO23_SEL == 1 | MW_EPWMXBAR23_GPIO24_SEL == 1 | MW_EPWMXBAR23_GPIO25_SEL == 1 | MW_EPWMXBAR23_GPIO26_SEL == 1 | MW_EPWMXBAR23_GPIO27_SEL == 1 | MW_EPWMXBAR23_GPIO28_SEL == 1 | MW_EPWMXBAR23_GPIO29_SEL == 1 | MW_EPWMXBAR23_GPIO30_SEL == 1 | MW_EPWMXBAR23_GPIO31_SEL == 1 | MW_EPWMXBAR23_GPIO32_SEL == 1 | MW_EPWMXBAR23_GPIO33_SEL == 1 | MW_EPWMXBAR23_GPIO34_SEL == 1 | MW_EPWMXBAR23_GPIO35_SEL == 1 | MW_EPWMXBAR23_GPIO36_SEL == 1 | MW_EPWMXBAR23_GPIO37_SEL == 1 | MW_EPWMXBAR23_GPIO38_SEL == 1 | MW_EPWMXBAR23_GPIO39_SEL == 1 | MW_EPWMXBAR23_GPIO40_SEL == 1 | MW_EPWMXBAR23_GPIO41_SEL == 1 | MW_EPWMXBAR23_GPIO42_SEL == 1 | MW_EPWMXBAR23_GPIO43_SEL == 1 | MW_EPWMXBAR23_GPIO44_SEL == 1 | | MW_EPWMXBAR23_GPIO45_SEL == 1 | MW_EPWMXBAR23_GPIO46_SEL == 1 | MW_EPWMXBAR23_GPIO47_SEL == 1| MW_EPWMXBAR23_GPIO48_SEL == 1 | MW_EPWMXBAR23_GPIO49_SEL == 1 | MW_EPWMXBAR23_GPIO50_SEL == 1 | MW_EPWMXBAR23_GPIO51_SEL == 1 | MW_EPWMXBAR23_GPIO52_SEL == 1 | MW_EPWMXBAR23_GPIO53_SEL == 1 | MW_EPWMXBAR23_GPIO54_SEL == 1 | MW_EPWMXBAR23_GPIO55_SEL == 1 | MW_EPWMXBAR23_GPIO56_SEL == 1 | MW_EPWMXBAR23_GPIO57_SEL == 1| MW_EPWMXBAR23_GPIO58_SEL == 1 | MW_EPWMXBAR23_GPIO59_SEL == 1 | MW_EPWMXBAR23_GPIO60_SEL == 1 | MW_EPWMXBAR23_GPIO61_SEL == 1 | MW_EPWMXBAR23_GPIO62_SEL == 1 | MW_EPWMXBAR23_GPIO63_SEL == 1 | MW_EPWMXBAR23_GPIO64_SEL == 1 | | MW_EPWMXBAR23_GPIO59_SEL == 1 | MW_EPWMXBAR23_GPIO60_SEL == 1 | MW_EPWMXBAR23_GPIO61_SEL == 1 | MW_EPWMXBAR23_GPIO62_SEL == 1 | MW_EPWMXBAR23_GPIO63_SEL == 1 | MW_EPWMXBAR23_GPIO64_SEL == 1 | MW_EPWMXBAR23_GPIO65_SEL == 1 | MW_EPWMXBAR23_GPIO66_SEL == 1 | MW_EPWMXBAR23_GPIO67_SEL == 1 | MW_EPWMXBAR23_GPIO68_SEL == 1 | MW_EPWMXBAR23_GPIO69_SEL == 1 | MW_EPWMXBAR23_GPIO70_SEL == 1 | MW_EPWMXBAR23_GPIO71_SEL == 1 | MW_EPWMXBAR23_GPIO72_SEL == 1 | MW_EPWMXBAR23_GPIO73_SEL == 1 | MW_EPWMXBAR23_GPIO74_SEL == 1 | MW_EPWMXBAR23_GPIO75_SEL == 1 | MW_EPWMXBAR23_GPIO76_SEL == 1 | MW_EPWMXBAR23_GPIO77_SEL == 1 | MW_EPWMXBAR23_GPIO78_SEL == 1 | | MW_EPWMXBAR23_GPIO80_SEL == 1 | MW_EPWMXBAR23_GPIO81_SEL == 1 | MW_EPWMXBAR23_GPIO82_SEL == 1 | MW_EPWMXBAR23_GPIO83_SEL == 1 | MW_EPWMXBAR23_GPIO84_SEL == 1 | MW_EPWMXBAR23_GPIO85_SEL == 1 | MW_EPWMXBAR23_GPIO86_SEL == 1 | MW_EPWMXBAR23_GPIO87_SEL == 1 | MW_EPWMXBAR23_GPIO88_SEL == 1 | MW_EPWMXBAR23_GPIO89_SEL == 1 | MW_EPWMXBAR23_GPIO90_SEL == 1 | MW_EPWMXBAR23_GPIO91_SEL == 1 | MW_EPWMXBAR23_GPIO92_SEL == 1 | MW_EPWMXBAR23_GPIO93_SEL == 1 | MW_EPWMXBAR23_GPIO94_SEL == 1 | MW_EPWMXBAR23_GPIO95_SEL == 1 | MW_EPWMXBAR23_GPIO96_SEL == 1 | MW_EPWMXBAR23_GPIO97_SEL == 1 | MW_EPWMXBAR23_GPIO98_SEL == 1 | MW_EPWMXBAR23_GPIO99_SEL == 1 | MW_EPWMXBAR23_GPIO100_SEL == 1 | MW_EPWMXBAR23_GPIO101_SEL == 1 | MW_EPWMXBAR23_GPIO102_SEL == 1 | MW_EPWMXBAR23_GPIO103_SEL == 1 | MW_EPWMXBAR23_GPIO104_SEL == 1 | MW_EPWMXBAR23_GPIO105_SEL == 1 | MW_EPWMXBAR23_GPIO106_SEL == 1 | MW_EPWMXBAR23_GPIO107_SEL == 1 | | MW_EPWMXBAR23_GPIO108_SEL == 1 | MW_EPWMXBAR23_GPIO109_SEL == 1| MW_EPWMXBAR23_GPIO110_SEL == 1 | MW_EPWMXBAR23_GPIO111_SEL == 1 | MW_EPWMXBAR23_GPIO112_SEL == 1 | MW_EPWMXBAR23_GPIO113_SEL == 1 | MW_EPWMXBAR23_GPIO114_SEL == 1 | MW_EPWMXBAR23_GPIO115_SEL == 1 | MW_EPWMXBAR23_GPIO116_SEL == 1 | MW_EPWMXBAR23_GPIO117_SEL == 1 | MW_EPWMXBAR23_GPIO118_SEL == 1 | MW_EPWMXBAR23_GPIO119_SEL == 1 | MW_EPWMXBAR23_GPIO120_SEL == 1 | MW_EPWMXBAR23_GPIO121_SEL == 1 | MW_EPWMXBAR23_GPIO122_SEL == 1 | MW_EPWMXBAR23_GPIO123_SEL == 1 | MW_EPWMXBAR23_GPIO124_SEL == 1 | MW_EPWMXBAR23_GPIO125_SEL == 1 | MW_EPWMXBAR23_GPIO126_SEL == 1 | MW_EPWMXBAR23_GPIO127_SEL == 1 | MW_EPWMXBAR23_GPIO128_SEL == 1 | MW_EPWMXBAR23_GPIO129_SEL == 1 | MW_EPWMXBAR23_GPIO130_SEL == 1 | MW_EPWMXBAR23_GPIO131_SEL == 1 | MW_EPWMXBAR23_GPIO132_SEL == 1 | MW_EPWMXBAR23_GPIO133_SEL == 1 | MW_EPWMXBAR23_GPIO134_SEL == 1 | MW_EPWMXBAR23_GPIO135_SEL == 1 | MW_EPWMXBAR23_GPIO136_SEL == 1 | MW_EPWMXBAR23_GPIO137_SEL == 1 | MW_EPWMXBAR23_GPIO139_SEL == 1 | MW_EPWMXBAR23_GPIO140_SEL == 1 | MW_EPWMXBAR23_GPIO141_SEL == 1 | MW_EPWMXBAR23_GPIO142_SEL == 1 | MW_EPWMXBAR23_GPIO143_SEL == 1 | MW_EPWMXBAR23_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR23_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR23_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[23] = EPWM_XBAR_23;
    // else
        group3_epwmxbar[23] = 0;
    group4_epwmxbar[23] = (MW_EPWMXBAR23_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR23_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR23_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR23_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR23_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR23_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR23_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR23_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR23_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR23_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[23] = (MW_EPWMXBAR23_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR23_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR23_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR23_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR23_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR23_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR23_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR23_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR23_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR23_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[23] = (MW_EPWMXBAR23_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR23_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR23_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR23_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR23_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR23_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR23_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR23_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR23_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR23_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[23] = (MW_EPWMXBAR23_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR23_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR23_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR23_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR23_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR23_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR23_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR23_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR23_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR23_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR23_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR23_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR23_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR23_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[23] = (MW_EPWMXBAR23_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR23_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR23_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR23_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[24] = (MW_EPWMXBAR24_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR24_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR24_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR24_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR24_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR24_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR24_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR24_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR24_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR24_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR24_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR24_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR24_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR24_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR24_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR24_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR24_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR24_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[24] = (MW_EPWMXBAR24_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR24_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR24_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR24_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR24_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR24_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR24_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR24_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR24_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR24_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR24_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR24_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR24_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR24_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR24_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR24_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR24_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR24_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR24_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR24_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR24_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR24_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR24_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR24_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[24] = (MW_EPWMXBAR24_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR24_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR24_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR24_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR24_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR24_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR24_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR24_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR24_GPIO0_SEL == 1 | MW_EPWMXBAR24_GPIO1_SEL == 1 | MW_EPWMXBAR24_GPIO2_SEL == 1 | MW_EPWMXBAR24_GPIO3_SEL == 1 | MW_EPWMXBAR24_GPIO4_SEL == 1 | MW_EPWMXBAR24_GPIO5_SEL == 1 | MW_EPWMXBAR24_GPIO6_SEL == 1 | MW_EPWMXBAR24_GPIO7_SEL == 1| MW_EPWMXBAR24_GPIO8_SEL == 1 | MW_EPWMXBAR24_GPIO9_SEL == 1 | MW_EPWMXBAR24_GPIO10_SEL == 1| MW_EPWMXBAR24_GPIO11_SEL == 1| MW_EPWMXBAR24_GPIO12_SEL == 1 | MW_EPWMXBAR24_GPIO13_SEL == 1 | MW_EPWMXBAR24_GPIO14_SEL == 1 | MW_EPWMXBAR24_GPIO15_SEL == 1 | MW_EPWMXBAR24_GPIO16_SEL == 1 | MW_EPWMXBAR24_GPIO17_SEL == 1 | MW_EPWMXBAR24_GPIO18_SEL == 1 | MW_EPWMXBAR24_GPIO19_SEL == 1 | MW_EPWMXBAR24_GPIO20_SEL == 1 | MW_EPWMXBAR24_GPIO21_SEL == 1 | MW_EPWMXBAR24_GPIO22_SEL == 1 | MW_EPWMXBAR24_GPIO23_SEL == 1 | MW_EPWMXBAR24_GPIO24_SEL == 1 | MW_EPWMXBAR24_GPIO25_SEL == 1 | MW_EPWMXBAR24_GPIO26_SEL == 1 | MW_EPWMXBAR24_GPIO27_SEL == 1 | MW_EPWMXBAR24_GPIO28_SEL == 1 | MW_EPWMXBAR24_GPIO29_SEL == 1 | MW_EPWMXBAR24_GPIO30_SEL == 1 | MW_EPWMXBAR24_GPIO31_SEL == 1 | MW_EPWMXBAR24_GPIO32_SEL == 1 | MW_EPWMXBAR24_GPIO33_SEL == 1 | MW_EPWMXBAR24_GPIO34_SEL == 1 | MW_EPWMXBAR24_GPIO35_SEL == 1 | MW_EPWMXBAR24_GPIO36_SEL == 1 | MW_EPWMXBAR24_GPIO37_SEL == 1 | MW_EPWMXBAR24_GPIO38_SEL == 1 | MW_EPWMXBAR24_GPIO39_SEL == 1 | MW_EPWMXBAR24_GPIO40_SEL == 1 | MW_EPWMXBAR24_GPIO41_SEL == 1 | MW_EPWMXBAR24_GPIO42_SEL == 1 | MW_EPWMXBAR24_GPIO43_SEL == 1 | MW_EPWMXBAR24_GPIO44_SEL == 1 | | MW_EPWMXBAR24_GPIO45_SEL == 1 | MW_EPWMXBAR24_GPIO46_SEL == 1 | MW_EPWMXBAR24_GPIO47_SEL == 1| MW_EPWMXBAR24_GPIO48_SEL == 1 | MW_EPWMXBAR24_GPIO49_SEL == 1 | MW_EPWMXBAR24_GPIO50_SEL == 1 | MW_EPWMXBAR24_GPIO51_SEL == 1 | MW_EPWMXBAR24_GPIO52_SEL == 1 | MW_EPWMXBAR24_GPIO53_SEL == 1 | MW_EPWMXBAR24_GPIO54_SEL == 1 | MW_EPWMXBAR24_GPIO55_SEL == 1 | MW_EPWMXBAR24_GPIO56_SEL == 1 | MW_EPWMXBAR24_GPIO57_SEL == 1| MW_EPWMXBAR24_GPIO58_SEL == 1 | MW_EPWMXBAR24_GPIO59_SEL == 1 | MW_EPWMXBAR24_GPIO60_SEL == 1 | MW_EPWMXBAR24_GPIO61_SEL == 1 | MW_EPWMXBAR24_GPIO62_SEL == 1 | MW_EPWMXBAR24_GPIO63_SEL == 1 | MW_EPWMXBAR24_GPIO64_SEL == 1 | | MW_EPWMXBAR24_GPIO59_SEL == 1 | MW_EPWMXBAR24_GPIO60_SEL == 1 | MW_EPWMXBAR24_GPIO61_SEL == 1 | MW_EPWMXBAR24_GPIO62_SEL == 1 | MW_EPWMXBAR24_GPIO63_SEL == 1 | MW_EPWMXBAR24_GPIO64_SEL == 1 | MW_EPWMXBAR24_GPIO65_SEL == 1 | MW_EPWMXBAR24_GPIO66_SEL == 1 | MW_EPWMXBAR24_GPIO67_SEL == 1 | MW_EPWMXBAR24_GPIO68_SEL == 1 | MW_EPWMXBAR24_GPIO69_SEL == 1 | MW_EPWMXBAR24_GPIO70_SEL == 1 | MW_EPWMXBAR24_GPIO71_SEL == 1 | MW_EPWMXBAR24_GPIO72_SEL == 1 | MW_EPWMXBAR24_GPIO73_SEL == 1 | MW_EPWMXBAR24_GPIO74_SEL == 1 | MW_EPWMXBAR24_GPIO75_SEL == 1 | MW_EPWMXBAR24_GPIO76_SEL == 1 | MW_EPWMXBAR24_GPIO77_SEL == 1 | MW_EPWMXBAR24_GPIO78_SEL == 1 | | MW_EPWMXBAR24_GPIO80_SEL == 1 | MW_EPWMXBAR24_GPIO81_SEL == 1 | MW_EPWMXBAR24_GPIO82_SEL == 1 | MW_EPWMXBAR24_GPIO83_SEL == 1 | MW_EPWMXBAR24_GPIO84_SEL == 1 | MW_EPWMXBAR24_GPIO85_SEL == 1 | MW_EPWMXBAR24_GPIO86_SEL == 1 | MW_EPWMXBAR24_GPIO87_SEL == 1 | MW_EPWMXBAR24_GPIO88_SEL == 1 | MW_EPWMXBAR24_GPIO89_SEL == 1 | MW_EPWMXBAR24_GPIO90_SEL == 1 | MW_EPWMXBAR24_GPIO91_SEL == 1 | MW_EPWMXBAR24_GPIO92_SEL == 1 | MW_EPWMXBAR24_GPIO93_SEL == 1 | MW_EPWMXBAR24_GPIO94_SEL == 1 | MW_EPWMXBAR24_GPIO95_SEL == 1 | MW_EPWMXBAR24_GPIO96_SEL == 1 | MW_EPWMXBAR24_GPIO97_SEL == 1 | MW_EPWMXBAR24_GPIO98_SEL == 1 | MW_EPWMXBAR24_GPIO99_SEL == 1 | MW_EPWMXBAR24_GPIO100_SEL == 1 | MW_EPWMXBAR24_GPIO101_SEL == 1 | MW_EPWMXBAR24_GPIO102_SEL == 1 | MW_EPWMXBAR24_GPIO103_SEL == 1 | MW_EPWMXBAR24_GPIO104_SEL == 1 | MW_EPWMXBAR24_GPIO105_SEL == 1 | MW_EPWMXBAR24_GPIO106_SEL == 1 | MW_EPWMXBAR24_GPIO107_SEL == 1 | | MW_EPWMXBAR24_GPIO108_SEL == 1 | MW_EPWMXBAR24_GPIO109_SEL == 1| MW_EPWMXBAR24_GPIO110_SEL == 1 | MW_EPWMXBAR24_GPIO111_SEL == 1 | MW_EPWMXBAR24_GPIO112_SEL == 1 | MW_EPWMXBAR24_GPIO113_SEL == 1 | MW_EPWMXBAR24_GPIO114_SEL == 1 | MW_EPWMXBAR24_GPIO115_SEL == 1 | MW_EPWMXBAR24_GPIO116_SEL == 1 | MW_EPWMXBAR24_GPIO117_SEL == 1 | MW_EPWMXBAR24_GPIO118_SEL == 1 | MW_EPWMXBAR24_GPIO119_SEL == 1 | MW_EPWMXBAR24_GPIO120_SEL == 1 | MW_EPWMXBAR24_GPIO121_SEL == 1 | MW_EPWMXBAR24_GPIO122_SEL == 1 | MW_EPWMXBAR24_GPIO123_SEL == 1 | MW_EPWMXBAR24_GPIO124_SEL == 1 | MW_EPWMXBAR24_GPIO125_SEL == 1 | MW_EPWMXBAR24_GPIO126_SEL == 1 | MW_EPWMXBAR24_GPIO127_SEL == 1 | MW_EPWMXBAR24_GPIO128_SEL == 1 | MW_EPWMXBAR24_GPIO129_SEL == 1 | MW_EPWMXBAR24_GPIO130_SEL == 1 | MW_EPWMXBAR24_GPIO131_SEL == 1 | MW_EPWMXBAR24_GPIO132_SEL == 1 | MW_EPWMXBAR24_GPIO133_SEL == 1 | MW_EPWMXBAR24_GPIO134_SEL == 1 | MW_EPWMXBAR24_GPIO135_SEL == 1 | MW_EPWMXBAR24_GPIO136_SEL == 1 | MW_EPWMXBAR24_GPIO137_SEL == 1 | MW_EPWMXBAR24_GPIO139_SEL == 1 | MW_EPWMXBAR24_GPIO140_SEL == 1 | MW_EPWMXBAR24_GPIO141_SEL == 1 | MW_EPWMXBAR24_GPIO142_SEL == 1 | MW_EPWMXBAR24_GPIO143_SEL == 1 | MW_EPWMXBAR24_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR24_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR24_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[24] = EPWM_XBAR_24;
    // else
        group3_epwmxbar[24] = 0;
    group4_epwmxbar[24] = (MW_EPWMXBAR24_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR24_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR24_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR24_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR24_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR24_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR24_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR24_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR24_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR24_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[24] = (MW_EPWMXBAR24_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR24_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR24_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR24_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR24_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR24_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR24_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR24_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR24_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR24_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[24] = (MW_EPWMXBAR24_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR24_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR24_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR24_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR24_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR24_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR24_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR24_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR24_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR24_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[24] = (MW_EPWMXBAR24_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR24_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR24_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR24_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR24_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR24_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR24_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR24_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR24_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR24_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR24_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR24_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR24_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR24_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[24] = (MW_EPWMXBAR24_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR24_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR24_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR24_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[25] = (MW_EPWMXBAR25_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR25_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR25_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR25_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR25_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR25_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR25_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR25_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR25_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR25_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR25_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR25_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR25_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR25_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR25_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR25_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR25_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR25_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[25] = (MW_EPWMXBAR25_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR25_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR25_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR25_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR25_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR25_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR25_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR25_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR25_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR25_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR25_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR25_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR25_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR25_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR25_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR25_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR25_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR25_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR25_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR25_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR25_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR25_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR25_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR25_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[25] = (MW_EPWMXBAR25_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR25_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR25_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR25_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR25_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR25_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR25_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR25_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR25_GPIO0_SEL == 1 | MW_EPWMXBAR25_GPIO1_SEL == 1 | MW_EPWMXBAR25_GPIO2_SEL == 1 | MW_EPWMXBAR25_GPIO3_SEL == 1 | MW_EPWMXBAR25_GPIO4_SEL == 1 | MW_EPWMXBAR25_GPIO5_SEL == 1 | MW_EPWMXBAR25_GPIO6_SEL == 1 | MW_EPWMXBAR25_GPIO7_SEL == 1| MW_EPWMXBAR25_GPIO8_SEL == 1 | MW_EPWMXBAR25_GPIO9_SEL == 1 | MW_EPWMXBAR25_GPIO10_SEL == 1| MW_EPWMXBAR25_GPIO11_SEL == 1| MW_EPWMXBAR25_GPIO12_SEL == 1 | MW_EPWMXBAR25_GPIO13_SEL == 1 | MW_EPWMXBAR25_GPIO14_SEL == 1 | MW_EPWMXBAR25_GPIO15_SEL == 1 | MW_EPWMXBAR25_GPIO16_SEL == 1 | MW_EPWMXBAR25_GPIO17_SEL == 1 | MW_EPWMXBAR25_GPIO18_SEL == 1 | MW_EPWMXBAR25_GPIO19_SEL == 1 | MW_EPWMXBAR25_GPIO20_SEL == 1 | MW_EPWMXBAR25_GPIO21_SEL == 1 | MW_EPWMXBAR25_GPIO22_SEL == 1 | MW_EPWMXBAR25_GPIO23_SEL == 1 | MW_EPWMXBAR25_GPIO24_SEL == 1 | MW_EPWMXBAR25_GPIO25_SEL == 1 | MW_EPWMXBAR25_GPIO26_SEL == 1 | MW_EPWMXBAR25_GPIO27_SEL == 1 | MW_EPWMXBAR25_GPIO28_SEL == 1 | MW_EPWMXBAR25_GPIO29_SEL == 1 | MW_EPWMXBAR25_GPIO30_SEL == 1 | MW_EPWMXBAR25_GPIO31_SEL == 1 | MW_EPWMXBAR25_GPIO32_SEL == 1 | MW_EPWMXBAR25_GPIO33_SEL == 1 | MW_EPWMXBAR25_GPIO34_SEL == 1 | MW_EPWMXBAR25_GPIO35_SEL == 1 | MW_EPWMXBAR25_GPIO36_SEL == 1 | MW_EPWMXBAR25_GPIO37_SEL == 1 | MW_EPWMXBAR25_GPIO38_SEL == 1 | MW_EPWMXBAR25_GPIO39_SEL == 1 | MW_EPWMXBAR25_GPIO40_SEL == 1 | MW_EPWMXBAR25_GPIO41_SEL == 1 | MW_EPWMXBAR25_GPIO42_SEL == 1 | MW_EPWMXBAR25_GPIO43_SEL == 1 | MW_EPWMXBAR25_GPIO44_SEL == 1 | | MW_EPWMXBAR25_GPIO45_SEL == 1 | MW_EPWMXBAR25_GPIO46_SEL == 1 | MW_EPWMXBAR25_GPIO47_SEL == 1| MW_EPWMXBAR25_GPIO48_SEL == 1 | MW_EPWMXBAR25_GPIO49_SEL == 1 | MW_EPWMXBAR25_GPIO50_SEL == 1 | MW_EPWMXBAR25_GPIO51_SEL == 1 | MW_EPWMXBAR25_GPIO52_SEL == 1 | MW_EPWMXBAR25_GPIO53_SEL == 1 | MW_EPWMXBAR25_GPIO54_SEL == 1 | MW_EPWMXBAR25_GPIO55_SEL == 1 | MW_EPWMXBAR25_GPIO56_SEL == 1 | MW_EPWMXBAR25_GPIO57_SEL == 1| MW_EPWMXBAR25_GPIO58_SEL == 1 | MW_EPWMXBAR25_GPIO59_SEL == 1 | MW_EPWMXBAR25_GPIO60_SEL == 1 | MW_EPWMXBAR25_GPIO61_SEL == 1 | MW_EPWMXBAR25_GPIO62_SEL == 1 | MW_EPWMXBAR25_GPIO63_SEL == 1 | MW_EPWMXBAR25_GPIO64_SEL == 1 | | MW_EPWMXBAR25_GPIO59_SEL == 1 | MW_EPWMXBAR25_GPIO60_SEL == 1 | MW_EPWMXBAR25_GPIO61_SEL == 1 | MW_EPWMXBAR25_GPIO62_SEL == 1 | MW_EPWMXBAR25_GPIO63_SEL == 1 | MW_EPWMXBAR25_GPIO64_SEL == 1 | MW_EPWMXBAR25_GPIO65_SEL == 1 | MW_EPWMXBAR25_GPIO66_SEL == 1 | MW_EPWMXBAR25_GPIO67_SEL == 1 | MW_EPWMXBAR25_GPIO68_SEL == 1 | MW_EPWMXBAR25_GPIO69_SEL == 1 | MW_EPWMXBAR25_GPIO70_SEL == 1 | MW_EPWMXBAR25_GPIO71_SEL == 1 | MW_EPWMXBAR25_GPIO72_SEL == 1 | MW_EPWMXBAR25_GPIO73_SEL == 1 | MW_EPWMXBAR25_GPIO74_SEL == 1 | MW_EPWMXBAR25_GPIO75_SEL == 1 | MW_EPWMXBAR25_GPIO76_SEL == 1 | MW_EPWMXBAR25_GPIO77_SEL == 1 | MW_EPWMXBAR25_GPIO78_SEL == 1 | | MW_EPWMXBAR25_GPIO80_SEL == 1 | MW_EPWMXBAR25_GPIO81_SEL == 1 | MW_EPWMXBAR25_GPIO82_SEL == 1 | MW_EPWMXBAR25_GPIO83_SEL == 1 | MW_EPWMXBAR25_GPIO84_SEL == 1 | MW_EPWMXBAR25_GPIO85_SEL == 1 | MW_EPWMXBAR25_GPIO86_SEL == 1 | MW_EPWMXBAR25_GPIO87_SEL == 1 | MW_EPWMXBAR25_GPIO88_SEL == 1 | MW_EPWMXBAR25_GPIO89_SEL == 1 | MW_EPWMXBAR25_GPIO90_SEL == 1 | MW_EPWMXBAR25_GPIO91_SEL == 1 | MW_EPWMXBAR25_GPIO92_SEL == 1 | MW_EPWMXBAR25_GPIO93_SEL == 1 | MW_EPWMXBAR25_GPIO94_SEL == 1 | MW_EPWMXBAR25_GPIO95_SEL == 1 | MW_EPWMXBAR25_GPIO96_SEL == 1 | MW_EPWMXBAR25_GPIO97_SEL == 1 | MW_EPWMXBAR25_GPIO98_SEL == 1 | MW_EPWMXBAR25_GPIO99_SEL == 1 | MW_EPWMXBAR25_GPIO100_SEL == 1 | MW_EPWMXBAR25_GPIO101_SEL == 1 | MW_EPWMXBAR25_GPIO102_SEL == 1 | MW_EPWMXBAR25_GPIO103_SEL == 1 | MW_EPWMXBAR25_GPIO104_SEL == 1 | MW_EPWMXBAR25_GPIO105_SEL == 1 | MW_EPWMXBAR25_GPIO106_SEL == 1 | MW_EPWMXBAR25_GPIO107_SEL == 1 | | MW_EPWMXBAR25_GPIO108_SEL == 1 | MW_EPWMXBAR25_GPIO109_SEL == 1| MW_EPWMXBAR25_GPIO110_SEL == 1 | MW_EPWMXBAR25_GPIO111_SEL == 1 | MW_EPWMXBAR25_GPIO112_SEL == 1 | MW_EPWMXBAR25_GPIO113_SEL == 1 | MW_EPWMXBAR25_GPIO114_SEL == 1 | MW_EPWMXBAR25_GPIO115_SEL == 1 | MW_EPWMXBAR25_GPIO116_SEL == 1 | MW_EPWMXBAR25_GPIO117_SEL == 1 | MW_EPWMXBAR25_GPIO118_SEL == 1 | MW_EPWMXBAR25_GPIO119_SEL == 1 | MW_EPWMXBAR25_GPIO120_SEL == 1 | MW_EPWMXBAR25_GPIO121_SEL == 1 | MW_EPWMXBAR25_GPIO122_SEL == 1 | MW_EPWMXBAR25_GPIO123_SEL == 1 | MW_EPWMXBAR25_GPIO124_SEL == 1 | MW_EPWMXBAR25_GPIO125_SEL == 1 | MW_EPWMXBAR25_GPIO126_SEL == 1 | MW_EPWMXBAR25_GPIO127_SEL == 1 | MW_EPWMXBAR25_GPIO128_SEL == 1 | MW_EPWMXBAR25_GPIO129_SEL == 1 | MW_EPWMXBAR25_GPIO130_SEL == 1 | MW_EPWMXBAR25_GPIO131_SEL == 1 | MW_EPWMXBAR25_GPIO132_SEL == 1 | MW_EPWMXBAR25_GPIO133_SEL == 1 | MW_EPWMXBAR25_GPIO134_SEL == 1 | MW_EPWMXBAR25_GPIO135_SEL == 1 | MW_EPWMXBAR25_GPIO136_SEL == 1 | MW_EPWMXBAR25_GPIO137_SEL == 1 | MW_EPWMXBAR25_GPIO139_SEL == 1 | MW_EPWMXBAR25_GPIO140_SEL == 1 | MW_EPWMXBAR25_GPIO141_SEL == 1 | MW_EPWMXBAR25_GPIO142_SEL == 1 | MW_EPWMXBAR25_GPIO143_SEL == 1 | MW_EPWMXBAR25_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR25_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR25_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[25] = EPWM_XBAR_25;
    // else
        group3_epwmxbar[25] = 0;
    group4_epwmxbar[25] = (MW_EPWMXBAR25_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR25_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR25_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR25_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR25_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR25_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR25_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR25_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR25_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR25_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[25] = (MW_EPWMXBAR25_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR25_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR25_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR25_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR25_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR25_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR25_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR25_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR25_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR25_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[25] = (MW_EPWMXBAR25_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR25_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR25_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR25_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR25_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR25_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR25_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR25_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR25_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR25_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[25] = (MW_EPWMXBAR25_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR25_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR25_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR25_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR25_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR25_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR25_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR25_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR25_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR25_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR25_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR25_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR25_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR25_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[25] = (MW_EPWMXBAR25_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR25_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR25_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR25_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[26] = (MW_EPWMXBAR26_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR26_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR26_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR26_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR26_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR26_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR26_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR26_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR26_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR26_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR26_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR26_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR26_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR26_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR26_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR26_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR26_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR26_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[26] = (MW_EPWMXBAR26_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR26_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR26_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR26_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR26_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR26_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR26_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR26_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR26_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR26_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR26_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR26_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR26_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR26_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR26_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR26_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR26_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR26_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR26_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR26_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR26_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR26_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR26_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR26_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[26] = (MW_EPWMXBAR26_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR26_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR26_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR26_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR26_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR26_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR26_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR26_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR26_GPIO0_SEL == 1 | MW_EPWMXBAR26_GPIO1_SEL == 1 | MW_EPWMXBAR26_GPIO2_SEL == 1 | MW_EPWMXBAR26_GPIO3_SEL == 1 | MW_EPWMXBAR26_GPIO4_SEL == 1 | MW_EPWMXBAR26_GPIO5_SEL == 1 | MW_EPWMXBAR26_GPIO6_SEL == 1 | MW_EPWMXBAR26_GPIO7_SEL == 1| MW_EPWMXBAR26_GPIO8_SEL == 1 | MW_EPWMXBAR26_GPIO9_SEL == 1 | MW_EPWMXBAR26_GPIO10_SEL == 1| MW_EPWMXBAR26_GPIO11_SEL == 1| MW_EPWMXBAR26_GPIO12_SEL == 1 | MW_EPWMXBAR26_GPIO13_SEL == 1 | MW_EPWMXBAR26_GPIO14_SEL == 1 | MW_EPWMXBAR26_GPIO15_SEL == 1 | MW_EPWMXBAR26_GPIO16_SEL == 1 | MW_EPWMXBAR26_GPIO17_SEL == 1 | MW_EPWMXBAR26_GPIO18_SEL == 1 | MW_EPWMXBAR26_GPIO19_SEL == 1 | MW_EPWMXBAR26_GPIO20_SEL == 1 | MW_EPWMXBAR26_GPIO21_SEL == 1 | MW_EPWMXBAR26_GPIO22_SEL == 1 | MW_EPWMXBAR26_GPIO23_SEL == 1 | MW_EPWMXBAR26_GPIO24_SEL == 1 | MW_EPWMXBAR26_GPIO25_SEL == 1 | MW_EPWMXBAR26_GPIO26_SEL == 1 | MW_EPWMXBAR26_GPIO27_SEL == 1 | MW_EPWMXBAR26_GPIO28_SEL == 1 | MW_EPWMXBAR26_GPIO29_SEL == 1 | MW_EPWMXBAR26_GPIO30_SEL == 1 | MW_EPWMXBAR26_GPIO31_SEL == 1 | MW_EPWMXBAR26_GPIO32_SEL == 1 | MW_EPWMXBAR26_GPIO33_SEL == 1 | MW_EPWMXBAR26_GPIO34_SEL == 1 | MW_EPWMXBAR26_GPIO35_SEL == 1 | MW_EPWMXBAR26_GPIO36_SEL == 1 | MW_EPWMXBAR26_GPIO37_SEL == 1 | MW_EPWMXBAR26_GPIO38_SEL == 1 | MW_EPWMXBAR26_GPIO39_SEL == 1 | MW_EPWMXBAR26_GPIO40_SEL == 1 | MW_EPWMXBAR26_GPIO41_SEL == 1 | MW_EPWMXBAR26_GPIO42_SEL == 1 | MW_EPWMXBAR26_GPIO43_SEL == 1 | MW_EPWMXBAR26_GPIO44_SEL == 1 | | MW_EPWMXBAR26_GPIO45_SEL == 1 | MW_EPWMXBAR26_GPIO46_SEL == 1 | MW_EPWMXBAR26_GPIO47_SEL == 1| MW_EPWMXBAR26_GPIO48_SEL == 1 | MW_EPWMXBAR26_GPIO49_SEL == 1 | MW_EPWMXBAR26_GPIO50_SEL == 1 | MW_EPWMXBAR26_GPIO51_SEL == 1 | MW_EPWMXBAR26_GPIO52_SEL == 1 | MW_EPWMXBAR26_GPIO53_SEL == 1 | MW_EPWMXBAR26_GPIO54_SEL == 1 | MW_EPWMXBAR26_GPIO55_SEL == 1 | MW_EPWMXBAR26_GPIO56_SEL == 1 | MW_EPWMXBAR26_GPIO57_SEL == 1| MW_EPWMXBAR26_GPIO58_SEL == 1 | MW_EPWMXBAR26_GPIO59_SEL == 1 | MW_EPWMXBAR26_GPIO60_SEL == 1 | MW_EPWMXBAR26_GPIO61_SEL == 1 | MW_EPWMXBAR26_GPIO62_SEL == 1 | MW_EPWMXBAR26_GPIO63_SEL == 1 | MW_EPWMXBAR26_GPIO64_SEL == 1 | | MW_EPWMXBAR26_GPIO59_SEL == 1 | MW_EPWMXBAR26_GPIO60_SEL == 1 | MW_EPWMXBAR26_GPIO61_SEL == 1 | MW_EPWMXBAR26_GPIO62_SEL == 1 | MW_EPWMXBAR26_GPIO63_SEL == 1 | MW_EPWMXBAR26_GPIO64_SEL == 1 | MW_EPWMXBAR26_GPIO65_SEL == 1 | MW_EPWMXBAR26_GPIO66_SEL == 1 | MW_EPWMXBAR26_GPIO67_SEL == 1 | MW_EPWMXBAR26_GPIO68_SEL == 1 | MW_EPWMXBAR26_GPIO69_SEL == 1 | MW_EPWMXBAR26_GPIO70_SEL == 1 | MW_EPWMXBAR26_GPIO71_SEL == 1 | MW_EPWMXBAR26_GPIO72_SEL == 1 | MW_EPWMXBAR26_GPIO73_SEL == 1 | MW_EPWMXBAR26_GPIO74_SEL == 1 | MW_EPWMXBAR26_GPIO75_SEL == 1 | MW_EPWMXBAR26_GPIO76_SEL == 1 | MW_EPWMXBAR26_GPIO77_SEL == 1 | MW_EPWMXBAR26_GPIO78_SEL == 1 | | MW_EPWMXBAR26_GPIO80_SEL == 1 | MW_EPWMXBAR26_GPIO81_SEL == 1 | MW_EPWMXBAR26_GPIO82_SEL == 1 | MW_EPWMXBAR26_GPIO83_SEL == 1 | MW_EPWMXBAR26_GPIO84_SEL == 1 | MW_EPWMXBAR26_GPIO85_SEL == 1 | MW_EPWMXBAR26_GPIO86_SEL == 1 | MW_EPWMXBAR26_GPIO87_SEL == 1 | MW_EPWMXBAR26_GPIO88_SEL == 1 | MW_EPWMXBAR26_GPIO89_SEL == 1 | MW_EPWMXBAR26_GPIO90_SEL == 1 | MW_EPWMXBAR26_GPIO91_SEL == 1 | MW_EPWMXBAR26_GPIO92_SEL == 1 | MW_EPWMXBAR26_GPIO93_SEL == 1 | MW_EPWMXBAR26_GPIO94_SEL == 1 | MW_EPWMXBAR26_GPIO95_SEL == 1 | MW_EPWMXBAR26_GPIO96_SEL == 1 | MW_EPWMXBAR26_GPIO97_SEL == 1 | MW_EPWMXBAR26_GPIO98_SEL == 1 | MW_EPWMXBAR26_GPIO99_SEL == 1 | MW_EPWMXBAR26_GPIO100_SEL == 1 | MW_EPWMXBAR26_GPIO101_SEL == 1 | MW_EPWMXBAR26_GPIO102_SEL == 1 | MW_EPWMXBAR26_GPIO103_SEL == 1 | MW_EPWMXBAR26_GPIO104_SEL == 1 | MW_EPWMXBAR26_GPIO105_SEL == 1 | MW_EPWMXBAR26_GPIO106_SEL == 1 | MW_EPWMXBAR26_GPIO107_SEL == 1 | | MW_EPWMXBAR26_GPIO108_SEL == 1 | MW_EPWMXBAR26_GPIO109_SEL == 1| MW_EPWMXBAR26_GPIO110_SEL == 1 | MW_EPWMXBAR26_GPIO111_SEL == 1 | MW_EPWMXBAR26_GPIO112_SEL == 1 | MW_EPWMXBAR26_GPIO113_SEL == 1 | MW_EPWMXBAR26_GPIO114_SEL == 1 | MW_EPWMXBAR26_GPIO115_SEL == 1 | MW_EPWMXBAR26_GPIO116_SEL == 1 | MW_EPWMXBAR26_GPIO117_SEL == 1 | MW_EPWMXBAR26_GPIO118_SEL == 1 | MW_EPWMXBAR26_GPIO119_SEL == 1 | MW_EPWMXBAR26_GPIO120_SEL == 1 | MW_EPWMXBAR26_GPIO121_SEL == 1 | MW_EPWMXBAR26_GPIO122_SEL == 1 | MW_EPWMXBAR26_GPIO123_SEL == 1 | MW_EPWMXBAR26_GPIO124_SEL == 1 | MW_EPWMXBAR26_GPIO125_SEL == 1 | MW_EPWMXBAR26_GPIO126_SEL == 1 | MW_EPWMXBAR26_GPIO127_SEL == 1 | MW_EPWMXBAR26_GPIO128_SEL == 1 | MW_EPWMXBAR26_GPIO129_SEL == 1 | MW_EPWMXBAR26_GPIO130_SEL == 1 | MW_EPWMXBAR26_GPIO131_SEL == 1 | MW_EPWMXBAR26_GPIO132_SEL == 1 | MW_EPWMXBAR26_GPIO133_SEL == 1 | MW_EPWMXBAR26_GPIO134_SEL == 1 | MW_EPWMXBAR26_GPIO135_SEL == 1 | MW_EPWMXBAR26_GPIO136_SEL == 1 | MW_EPWMXBAR26_GPIO137_SEL == 1 | MW_EPWMXBAR26_GPIO139_SEL == 1 | MW_EPWMXBAR26_GPIO140_SEL == 1 | MW_EPWMXBAR26_GPIO141_SEL == 1 | MW_EPWMXBAR26_GPIO142_SEL == 1 | MW_EPWMXBAR26_GPIO143_SEL == 1 | MW_EPWMXBAR26_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR26_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR26_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[26] = EPWM_XBAR_26;
    // else
        group3_epwmxbar[26] = 0;
    group4_epwmxbar[26] = (MW_EPWMXBAR26_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR26_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR26_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR26_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR26_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR26_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR26_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR26_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR26_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR26_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[26] = (MW_EPWMXBAR26_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR26_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR26_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR26_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR26_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR26_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR26_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR26_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR26_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR26_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[26] = (MW_EPWMXBAR26_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR26_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR26_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR26_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR26_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR26_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR26_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR26_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR26_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR26_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[26] = (MW_EPWMXBAR26_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR26_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR26_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR26_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR26_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR26_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR26_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR26_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR26_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR26_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR26_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR26_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR26_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR26_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[26] = (MW_EPWMXBAR26_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR26_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR26_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR26_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[27] = (MW_EPWMXBAR27_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR27_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR27_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR27_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR27_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR27_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR27_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR27_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR27_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR27_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR27_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR27_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR27_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR27_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR27_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR27_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR27_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR27_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[27] = (MW_EPWMXBAR27_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR27_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR27_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR27_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR27_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR27_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR27_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR27_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR27_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR27_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR27_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR27_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR27_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR27_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR27_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR27_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR27_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR27_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR27_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR27_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR27_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR27_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR27_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR27_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[27] = (MW_EPWMXBAR27_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR27_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR27_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR27_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR27_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR27_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR27_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR27_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR27_GPIO0_SEL == 1 | MW_EPWMXBAR27_GPIO1_SEL == 1 | MW_EPWMXBAR27_GPIO2_SEL == 1 | MW_EPWMXBAR27_GPIO3_SEL == 1 | MW_EPWMXBAR27_GPIO4_SEL == 1 | MW_EPWMXBAR27_GPIO5_SEL == 1 | MW_EPWMXBAR27_GPIO6_SEL == 1 | MW_EPWMXBAR27_GPIO7_SEL == 1| MW_EPWMXBAR27_GPIO8_SEL == 1 | MW_EPWMXBAR27_GPIO9_SEL == 1 | MW_EPWMXBAR27_GPIO10_SEL == 1| MW_EPWMXBAR27_GPIO11_SEL == 1| MW_EPWMXBAR27_GPIO12_SEL == 1 | MW_EPWMXBAR27_GPIO13_SEL == 1 | MW_EPWMXBAR27_GPIO14_SEL == 1 | MW_EPWMXBAR27_GPIO15_SEL == 1 | MW_EPWMXBAR27_GPIO16_SEL == 1 | MW_EPWMXBAR27_GPIO17_SEL == 1 | MW_EPWMXBAR27_GPIO18_SEL == 1 | MW_EPWMXBAR27_GPIO19_SEL == 1 | MW_EPWMXBAR27_GPIO20_SEL == 1 | MW_EPWMXBAR27_GPIO21_SEL == 1 | MW_EPWMXBAR27_GPIO22_SEL == 1 | MW_EPWMXBAR27_GPIO23_SEL == 1 | MW_EPWMXBAR27_GPIO24_SEL == 1 | MW_EPWMXBAR27_GPIO25_SEL == 1 | MW_EPWMXBAR27_GPIO26_SEL == 1 | MW_EPWMXBAR27_GPIO27_SEL == 1 | MW_EPWMXBAR27_GPIO28_SEL == 1 | MW_EPWMXBAR27_GPIO29_SEL == 1 | MW_EPWMXBAR27_GPIO30_SEL == 1 | MW_EPWMXBAR27_GPIO31_SEL == 1 | MW_EPWMXBAR27_GPIO32_SEL == 1 | MW_EPWMXBAR27_GPIO33_SEL == 1 | MW_EPWMXBAR27_GPIO34_SEL == 1 | MW_EPWMXBAR27_GPIO35_SEL == 1 | MW_EPWMXBAR27_GPIO36_SEL == 1 | MW_EPWMXBAR27_GPIO37_SEL == 1 | MW_EPWMXBAR27_GPIO38_SEL == 1 | MW_EPWMXBAR27_GPIO39_SEL == 1 | MW_EPWMXBAR27_GPIO40_SEL == 1 | MW_EPWMXBAR27_GPIO41_SEL == 1 | MW_EPWMXBAR27_GPIO42_SEL == 1 | MW_EPWMXBAR27_GPIO43_SEL == 1 | MW_EPWMXBAR27_GPIO44_SEL == 1 | | MW_EPWMXBAR27_GPIO45_SEL == 1 | MW_EPWMXBAR27_GPIO46_SEL == 1 | MW_EPWMXBAR27_GPIO47_SEL == 1| MW_EPWMXBAR27_GPIO48_SEL == 1 | MW_EPWMXBAR27_GPIO49_SEL == 1 | MW_EPWMXBAR27_GPIO50_SEL == 1 | MW_EPWMXBAR27_GPIO51_SEL == 1 | MW_EPWMXBAR27_GPIO52_SEL == 1 | MW_EPWMXBAR27_GPIO53_SEL == 1 | MW_EPWMXBAR27_GPIO54_SEL == 1 | MW_EPWMXBAR27_GPIO55_SEL == 1 | MW_EPWMXBAR27_GPIO56_SEL == 1 | MW_EPWMXBAR27_GPIO57_SEL == 1| MW_EPWMXBAR27_GPIO58_SEL == 1 | MW_EPWMXBAR27_GPIO59_SEL == 1 | MW_EPWMXBAR27_GPIO60_SEL == 1 | MW_EPWMXBAR27_GPIO61_SEL == 1 | MW_EPWMXBAR27_GPIO62_SEL == 1 | MW_EPWMXBAR27_GPIO63_SEL == 1 | MW_EPWMXBAR27_GPIO64_SEL == 1 | | MW_EPWMXBAR27_GPIO59_SEL == 1 | MW_EPWMXBAR27_GPIO60_SEL == 1 | MW_EPWMXBAR27_GPIO61_SEL == 1 | MW_EPWMXBAR27_GPIO62_SEL == 1 | MW_EPWMXBAR27_GPIO63_SEL == 1 | MW_EPWMXBAR27_GPIO64_SEL == 1 | MW_EPWMXBAR27_GPIO65_SEL == 1 | MW_EPWMXBAR27_GPIO66_SEL == 1 | MW_EPWMXBAR27_GPIO67_SEL == 1 | MW_EPWMXBAR27_GPIO68_SEL == 1 | MW_EPWMXBAR27_GPIO69_SEL == 1 | MW_EPWMXBAR27_GPIO70_SEL == 1 | MW_EPWMXBAR27_GPIO71_SEL == 1 | MW_EPWMXBAR27_GPIO72_SEL == 1 | MW_EPWMXBAR27_GPIO73_SEL == 1 | MW_EPWMXBAR27_GPIO74_SEL == 1 | MW_EPWMXBAR27_GPIO75_SEL == 1 | MW_EPWMXBAR27_GPIO76_SEL == 1 | MW_EPWMXBAR27_GPIO77_SEL == 1 | MW_EPWMXBAR27_GPIO78_SEL == 1 | | MW_EPWMXBAR27_GPIO80_SEL == 1 | MW_EPWMXBAR27_GPIO81_SEL == 1 | MW_EPWMXBAR27_GPIO82_SEL == 1 | MW_EPWMXBAR27_GPIO83_SEL == 1 | MW_EPWMXBAR27_GPIO84_SEL == 1 | MW_EPWMXBAR27_GPIO85_SEL == 1 | MW_EPWMXBAR27_GPIO86_SEL == 1 | MW_EPWMXBAR27_GPIO87_SEL == 1 | MW_EPWMXBAR27_GPIO88_SEL == 1 | MW_EPWMXBAR27_GPIO89_SEL == 1 | MW_EPWMXBAR27_GPIO90_SEL == 1 | MW_EPWMXBAR27_GPIO91_SEL == 1 | MW_EPWMXBAR27_GPIO92_SEL == 1 | MW_EPWMXBAR27_GPIO93_SEL == 1 | MW_EPWMXBAR27_GPIO94_SEL == 1 | MW_EPWMXBAR27_GPIO95_SEL == 1 | MW_EPWMXBAR27_GPIO96_SEL == 1 | MW_EPWMXBAR27_GPIO97_SEL == 1 | MW_EPWMXBAR27_GPIO98_SEL == 1 | MW_EPWMXBAR27_GPIO99_SEL == 1 | MW_EPWMXBAR27_GPIO100_SEL == 1 | MW_EPWMXBAR27_GPIO101_SEL == 1 | MW_EPWMXBAR27_GPIO102_SEL == 1 | MW_EPWMXBAR27_GPIO103_SEL == 1 | MW_EPWMXBAR27_GPIO104_SEL == 1 | MW_EPWMXBAR27_GPIO105_SEL == 1 | MW_EPWMXBAR27_GPIO106_SEL == 1 | MW_EPWMXBAR27_GPIO107_SEL == 1 | | MW_EPWMXBAR27_GPIO108_SEL == 1 | MW_EPWMXBAR27_GPIO109_SEL == 1| MW_EPWMXBAR27_GPIO110_SEL == 1 | MW_EPWMXBAR27_GPIO111_SEL == 1 | MW_EPWMXBAR27_GPIO112_SEL == 1 | MW_EPWMXBAR27_GPIO113_SEL == 1 | MW_EPWMXBAR27_GPIO114_SEL == 1 | MW_EPWMXBAR27_GPIO115_SEL == 1 | MW_EPWMXBAR27_GPIO116_SEL == 1 | MW_EPWMXBAR27_GPIO117_SEL == 1 | MW_EPWMXBAR27_GPIO118_SEL == 1 | MW_EPWMXBAR27_GPIO119_SEL == 1 | MW_EPWMXBAR27_GPIO120_SEL == 1 | MW_EPWMXBAR27_GPIO121_SEL == 1 | MW_EPWMXBAR27_GPIO122_SEL == 1 | MW_EPWMXBAR27_GPIO123_SEL == 1 | MW_EPWMXBAR27_GPIO124_SEL == 1 | MW_EPWMXBAR27_GPIO125_SEL == 1 | MW_EPWMXBAR27_GPIO126_SEL == 1 | MW_EPWMXBAR27_GPIO127_SEL == 1 | MW_EPWMXBAR27_GPIO128_SEL == 1 | MW_EPWMXBAR27_GPIO129_SEL == 1 | MW_EPWMXBAR27_GPIO130_SEL == 1 | MW_EPWMXBAR27_GPIO131_SEL == 1 | MW_EPWMXBAR27_GPIO132_SEL == 1 | MW_EPWMXBAR27_GPIO133_SEL == 1 | MW_EPWMXBAR27_GPIO134_SEL == 1 | MW_EPWMXBAR27_GPIO135_SEL == 1 | MW_EPWMXBAR27_GPIO136_SEL == 1 | MW_EPWMXBAR27_GPIO137_SEL == 1 | MW_EPWMXBAR27_GPIO139_SEL == 1 | MW_EPWMXBAR27_GPIO140_SEL == 1 | MW_EPWMXBAR27_GPIO141_SEL == 1 | MW_EPWMXBAR27_GPIO142_SEL == 1 | MW_EPWMXBAR27_GPIO143_SEL == 1 | MW_EPWMXBAR27_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR27_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR27_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[27] = EPWM_XBAR_27;
    // else
        group3_epwmxbar[27] = 0;
    group4_epwmxbar[27] = (MW_EPWMXBAR27_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR27_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR27_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR27_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR27_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR27_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR27_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR27_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR27_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR27_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[27] = (MW_EPWMXBAR27_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR27_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR27_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR27_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR27_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR27_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR27_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR27_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR27_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR27_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[27] = (MW_EPWMXBAR27_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR27_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR27_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR27_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR27_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR27_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR27_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR27_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR27_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR27_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[27] = (MW_EPWMXBAR27_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR27_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR27_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR27_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR27_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR27_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR27_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR27_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR27_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR27_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR27_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR27_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR27_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR27_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[27] = (MW_EPWMXBAR27_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR27_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR27_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR27_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[28] = (MW_EPWMXBAR28_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR28_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR28_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR28_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR28_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR28_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR28_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR28_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR28_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR28_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR28_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR28_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR28_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR28_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR28_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR28_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR28_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR28_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[28] = (MW_EPWMXBAR28_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR28_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR28_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR28_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR28_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR28_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR28_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR28_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR28_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR28_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR28_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR28_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR28_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR28_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR28_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR28_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR28_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR28_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR28_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR28_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR28_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR28_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR28_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR28_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[28] = (MW_EPWMXBAR28_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR28_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR28_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR28_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR28_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR28_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR28_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR28_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR28_GPIO0_SEL == 1 | MW_EPWMXBAR28_GPIO1_SEL == 1 | MW_EPWMXBAR28_GPIO2_SEL == 1 | MW_EPWMXBAR28_GPIO3_SEL == 1 | MW_EPWMXBAR28_GPIO4_SEL == 1 | MW_EPWMXBAR28_GPIO5_SEL == 1 | MW_EPWMXBAR28_GPIO6_SEL == 1 | MW_EPWMXBAR28_GPIO7_SEL == 1| MW_EPWMXBAR28_GPIO8_SEL == 1 | MW_EPWMXBAR28_GPIO9_SEL == 1 | MW_EPWMXBAR28_GPIO10_SEL == 1| MW_EPWMXBAR28_GPIO11_SEL == 1| MW_EPWMXBAR28_GPIO12_SEL == 1 | MW_EPWMXBAR28_GPIO13_SEL == 1 | MW_EPWMXBAR28_GPIO14_SEL == 1 | MW_EPWMXBAR28_GPIO15_SEL == 1 | MW_EPWMXBAR28_GPIO16_SEL == 1 | MW_EPWMXBAR28_GPIO17_SEL == 1 | MW_EPWMXBAR28_GPIO18_SEL == 1 | MW_EPWMXBAR28_GPIO19_SEL == 1 | MW_EPWMXBAR28_GPIO20_SEL == 1 | MW_EPWMXBAR28_GPIO21_SEL == 1 | MW_EPWMXBAR28_GPIO22_SEL == 1 | MW_EPWMXBAR28_GPIO23_SEL == 1 | MW_EPWMXBAR28_GPIO24_SEL == 1 | MW_EPWMXBAR28_GPIO25_SEL == 1 | MW_EPWMXBAR28_GPIO26_SEL == 1 | MW_EPWMXBAR28_GPIO27_SEL == 1 | MW_EPWMXBAR28_GPIO28_SEL == 1 | MW_EPWMXBAR28_GPIO29_SEL == 1 | MW_EPWMXBAR28_GPIO30_SEL == 1 | MW_EPWMXBAR28_GPIO31_SEL == 1 | MW_EPWMXBAR28_GPIO32_SEL == 1 | MW_EPWMXBAR28_GPIO33_SEL == 1 | MW_EPWMXBAR28_GPIO34_SEL == 1 | MW_EPWMXBAR28_GPIO35_SEL == 1 | MW_EPWMXBAR28_GPIO36_SEL == 1 | MW_EPWMXBAR28_GPIO37_SEL == 1 | MW_EPWMXBAR28_GPIO38_SEL == 1 | MW_EPWMXBAR28_GPIO39_SEL == 1 | MW_EPWMXBAR28_GPIO40_SEL == 1 | MW_EPWMXBAR28_GPIO41_SEL == 1 | MW_EPWMXBAR28_GPIO42_SEL == 1 | MW_EPWMXBAR28_GPIO43_SEL == 1 | MW_EPWMXBAR28_GPIO44_SEL == 1 | | MW_EPWMXBAR28_GPIO45_SEL == 1 | MW_EPWMXBAR28_GPIO46_SEL == 1 | MW_EPWMXBAR28_GPIO47_SEL == 1| MW_EPWMXBAR28_GPIO48_SEL == 1 | MW_EPWMXBAR28_GPIO49_SEL == 1 | MW_EPWMXBAR28_GPIO50_SEL == 1 | MW_EPWMXBAR28_GPIO51_SEL == 1 | MW_EPWMXBAR28_GPIO52_SEL == 1 | MW_EPWMXBAR28_GPIO53_SEL == 1 | MW_EPWMXBAR28_GPIO54_SEL == 1 | MW_EPWMXBAR28_GPIO55_SEL == 1 | MW_EPWMXBAR28_GPIO56_SEL == 1 | MW_EPWMXBAR28_GPIO57_SEL == 1| MW_EPWMXBAR28_GPIO58_SEL == 1 | MW_EPWMXBAR28_GPIO59_SEL == 1 | MW_EPWMXBAR28_GPIO60_SEL == 1 | MW_EPWMXBAR28_GPIO61_SEL == 1 | MW_EPWMXBAR28_GPIO62_SEL == 1 | MW_EPWMXBAR28_GPIO63_SEL == 1 | MW_EPWMXBAR28_GPIO64_SEL == 1 | | MW_EPWMXBAR28_GPIO59_SEL == 1 | MW_EPWMXBAR28_GPIO60_SEL == 1 | MW_EPWMXBAR28_GPIO61_SEL == 1 | MW_EPWMXBAR28_GPIO62_SEL == 1 | MW_EPWMXBAR28_GPIO63_SEL == 1 | MW_EPWMXBAR28_GPIO64_SEL == 1 | MW_EPWMXBAR28_GPIO65_SEL == 1 | MW_EPWMXBAR28_GPIO66_SEL == 1 | MW_EPWMXBAR28_GPIO67_SEL == 1 | MW_EPWMXBAR28_GPIO68_SEL == 1 | MW_EPWMXBAR28_GPIO69_SEL == 1 | MW_EPWMXBAR28_GPIO70_SEL == 1 | MW_EPWMXBAR28_GPIO71_SEL == 1 | MW_EPWMXBAR28_GPIO72_SEL == 1 | MW_EPWMXBAR28_GPIO73_SEL == 1 | MW_EPWMXBAR28_GPIO74_SEL == 1 | MW_EPWMXBAR28_GPIO75_SEL == 1 | MW_EPWMXBAR28_GPIO76_SEL == 1 | MW_EPWMXBAR28_GPIO77_SEL == 1 | MW_EPWMXBAR28_GPIO78_SEL == 1 | | MW_EPWMXBAR28_GPIO80_SEL == 1 | MW_EPWMXBAR28_GPIO81_SEL == 1 | MW_EPWMXBAR28_GPIO82_SEL == 1 | MW_EPWMXBAR28_GPIO83_SEL == 1 | MW_EPWMXBAR28_GPIO84_SEL == 1 | MW_EPWMXBAR28_GPIO85_SEL == 1 | MW_EPWMXBAR28_GPIO86_SEL == 1 | MW_EPWMXBAR28_GPIO87_SEL == 1 | MW_EPWMXBAR28_GPIO88_SEL == 1 | MW_EPWMXBAR28_GPIO89_SEL == 1 | MW_EPWMXBAR28_GPIO90_SEL == 1 | MW_EPWMXBAR28_GPIO91_SEL == 1 | MW_EPWMXBAR28_GPIO92_SEL == 1 | MW_EPWMXBAR28_GPIO93_SEL == 1 | MW_EPWMXBAR28_GPIO94_SEL == 1 | MW_EPWMXBAR28_GPIO95_SEL == 1 | MW_EPWMXBAR28_GPIO96_SEL == 1 | MW_EPWMXBAR28_GPIO97_SEL == 1 | MW_EPWMXBAR28_GPIO98_SEL == 1 | MW_EPWMXBAR28_GPIO99_SEL == 1 | MW_EPWMXBAR28_GPIO100_SEL == 1 | MW_EPWMXBAR28_GPIO101_SEL == 1 | MW_EPWMXBAR28_GPIO102_SEL == 1 | MW_EPWMXBAR28_GPIO103_SEL == 1 | MW_EPWMXBAR28_GPIO104_SEL == 1 | MW_EPWMXBAR28_GPIO105_SEL == 1 | MW_EPWMXBAR28_GPIO106_SEL == 1 | MW_EPWMXBAR28_GPIO107_SEL == 1 | | MW_EPWMXBAR28_GPIO108_SEL == 1 | MW_EPWMXBAR28_GPIO109_SEL == 1| MW_EPWMXBAR28_GPIO110_SEL == 1 | MW_EPWMXBAR28_GPIO111_SEL == 1 | MW_EPWMXBAR28_GPIO112_SEL == 1 | MW_EPWMXBAR28_GPIO113_SEL == 1 | MW_EPWMXBAR28_GPIO114_SEL == 1 | MW_EPWMXBAR28_GPIO115_SEL == 1 | MW_EPWMXBAR28_GPIO116_SEL == 1 | MW_EPWMXBAR28_GPIO117_SEL == 1 | MW_EPWMXBAR28_GPIO118_SEL == 1 | MW_EPWMXBAR28_GPIO119_SEL == 1 | MW_EPWMXBAR28_GPIO120_SEL == 1 | MW_EPWMXBAR28_GPIO121_SEL == 1 | MW_EPWMXBAR28_GPIO122_SEL == 1 | MW_EPWMXBAR28_GPIO123_SEL == 1 | MW_EPWMXBAR28_GPIO124_SEL == 1 | MW_EPWMXBAR28_GPIO125_SEL == 1 | MW_EPWMXBAR28_GPIO126_SEL == 1 | MW_EPWMXBAR28_GPIO127_SEL == 1 | MW_EPWMXBAR28_GPIO128_SEL == 1 | MW_EPWMXBAR28_GPIO129_SEL == 1 | MW_EPWMXBAR28_GPIO130_SEL == 1 | MW_EPWMXBAR28_GPIO131_SEL == 1 | MW_EPWMXBAR28_GPIO132_SEL == 1 | MW_EPWMXBAR28_GPIO133_SEL == 1 | MW_EPWMXBAR28_GPIO134_SEL == 1 | MW_EPWMXBAR28_GPIO135_SEL == 1 | MW_EPWMXBAR28_GPIO136_SEL == 1 | MW_EPWMXBAR28_GPIO137_SEL == 1 | MW_EPWMXBAR28_GPIO139_SEL == 1 | MW_EPWMXBAR28_GPIO140_SEL == 1 | MW_EPWMXBAR28_GPIO141_SEL == 1 | MW_EPWMXBAR28_GPIO142_SEL == 1 | MW_EPWMXBAR28_GPIO143_SEL == 1 | MW_EPWMXBAR28_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR28_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR28_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[28] = EPWM_XBAR_28;
    // else
        group3_epwmxbar[28] = 0;
    group4_epwmxbar[28] = (MW_EPWMXBAR28_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR28_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR28_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR28_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR28_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR28_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR28_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR28_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR28_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR28_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[28] = (MW_EPWMXBAR28_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR28_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR28_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR28_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR28_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR28_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR28_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR28_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR28_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR28_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[28] = (MW_EPWMXBAR28_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR28_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR28_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR28_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR28_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR28_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR28_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR28_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR28_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR28_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[28] = (MW_EPWMXBAR28_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR28_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR28_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR28_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR28_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR28_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR28_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR28_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR28_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR28_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR28_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR28_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR28_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR28_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[28] = (MW_EPWMXBAR28_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR28_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR28_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR28_SOCBXBAR_SOCB_SEL << 17);
    group0_epwmxbar[29] = (MW_EPWMXBAR29_CMPSSA0_CTRIPL_SEL << 0) | (MW_EPWMXBAR29_CMPSSA0_CTRIPH_SEL << 1) | (MW_EPWMXBAR29_CMPSSA1_CTRIPL_SEL << 2) | (MW_EPWMXBAR29_CMPSSA1_CTRIPH_SEL << 3) | (MW_EPWMXBAR29_CMPSSA2_CTRIPL_SEL << 4) | (MW_EPWMXBAR29_CMPSSA2_CTRIPH_SEL << 5) | (MW_EPWMXBAR29_CMPSSA3_CTRIPL_SEL << 6) | (MW_EPWMXBAR29_CMPSSA3_CTRIPH_SEL << 7) | (MW_EPWMXBAR29_CMPSSA4_CTRIPL_SEL << 8) | (MW_EPWMXBAR29_CMPSSA4_CTRIPH_SEL << 9) | (MW_EPWMXBAR29_CMPSSA5_CTRIPL_SEL << 10) | (MW_EPWMXBAR29_CMPSSA5_CTRIPH_SEL << 11) | (MW_EPWMXBAR29_CMPSSA6_CTRIPL_SEL << 12) | (MW_EPWMXBAR29_CMPSSA6_CTRIPH_SEL << 13) | (MW_EPWMXBAR29_CMPSSA7_CTRIPL_SEL << 14) | (MW_EPWMXBAR29_CMPSSA7_CTRIPH_SEL << 15) | (MW_EPWMXBAR29_CMPSSA8_CTRIPL_SEL << 16) | (MW_EPWMXBAR29_CMPSSA8_CTRIPH_SEL << 17);
    group1_epwmxbar[29] = (MW_EPWMXBAR29_SD0_FILT0_CEVT1_SEL << 0) | (MW_EPWMXBAR29_SD0_FILT0_CEVT2_SEL << 1) | (MW_EPWMXBAR29_SD0_FILT0_CMPHZ_SEL << 2) | (MW_EPWMXBAR29_SD0_FILT1_CEVT1_SEL << 3) | (MW_EPWMXBAR29_SD0_FILT1_CEVT2_SEL << 4) | (MW_EPWMXBAR29_SD0_FILT1_CMPHZ_SEL << 5) | (MW_EPWMXBAR29_SD0_FILT2_CEVT1_SEL << 6) | (MW_EPWMXBAR29_SD0_FILT2_CEVT2_SEL << 7) | (MW_EPWMXBAR29_SD0_FILT2_CMPHZ_SEL << 8) | (MW_EPWMXBAR29_SD0_FILT3_CEVT1_SEL << 9) | (MW_EPWMXBAR29_SD0_FILT3_CEVT2_SEL << 10) | (MW_EPWMXBAR29_SD0_FILT3_CMPHZ_SEL << 11) | (MW_EPWMXBAR29_SD1_FILT0_CEVT1_SEL << 12) | (MW_EPWMXBAR29_SD1_FILT0_CEVT2_SEL << 13) | (MW_EPWMXBAR29_SD1_FILT0_CMPHZ_SEL << 14) | (MW_EPWMXBAR29_SD1_FILT1_CEVT1_SEL << 15) | (MW_EPWMXBAR29_SD1_FILT1_CEVT2_SEL << 16) | (MW_EPWMXBAR29_SD1_FILT1_CMPHZ_SEL << 17) | (MW_EPWMXBAR29_SD1_FILT2_CEVT1_SEL << 18) | (MW_EPWMXBAR29_SD1_FILT2_CEVT2_SEL << 19) | (MW_EPWMXBAR29_SD1_FILT2_CMPHZ_SEL << 20) | (MW_EPWMXBAR29_SD1_FILT3_CEVT1_SEL << 21) | (MW_EPWMXBAR29_SD1_FILT3_CEVT2_SEL << 22) | (MW_EPWMXBAR29_SD1_FILT3_CMPHZ_SEL << 23);
    group2_epwmxbar[29] = (MW_EPWMXBAR29_ADC0_EVT1_SEL << 0) | (MW_EPWMXBAR29_ADC0_EVT2_SEL << 1) | (MW_EPWMXBAR29_ADC0_EVT3_SEL << 2) | (MW_EPWMXBAR29_ADC0_EVT4_SEL << 3) | (MW_EPWMXBAR29_ADC1_EVT1_SEL << 4) | (MW_EPWMXBAR29_ADC1_EVT2_SEL << 5) | (MW_EPWMXBAR29_ADC1_EVT3_SEL << 6) | (MW_EPWMXBAR29_ADC1_EVT4_SEL << 7);
    // if(MW_EPWMXBAR29_GPIO0_SEL == 1 | MW_EPWMXBAR29_GPIO1_SEL == 1 | MW_EPWMXBAR29_GPIO2_SEL == 1 | MW_EPWMXBAR29_GPIO3_SEL == 1 | MW_EPWMXBAR29_GPIO4_SEL == 1 | MW_EPWMXBAR29_GPIO5_SEL == 1 | MW_EPWMXBAR29_GPIO6_SEL == 1 | MW_EPWMXBAR29_GPIO7_SEL == 1| MW_EPWMXBAR29_GPIO8_SEL == 1 | MW_EPWMXBAR29_GPIO9_SEL == 1 | MW_EPWMXBAR29_GPIO10_SEL == 1| MW_EPWMXBAR29_GPIO11_SEL == 1| MW_EPWMXBAR29_GPIO12_SEL == 1 | MW_EPWMXBAR29_GPIO13_SEL == 1 | MW_EPWMXBAR29_GPIO14_SEL == 1 | MW_EPWMXBAR29_GPIO15_SEL == 1 | MW_EPWMXBAR29_GPIO16_SEL == 1 | MW_EPWMXBAR29_GPIO17_SEL == 1 | MW_EPWMXBAR29_GPIO18_SEL == 1 | MW_EPWMXBAR29_GPIO19_SEL == 1 | MW_EPWMXBAR29_GPIO20_SEL == 1 | MW_EPWMXBAR29_GPIO21_SEL == 1 | MW_EPWMXBAR29_GPIO22_SEL == 1 | MW_EPWMXBAR29_GPIO23_SEL == 1 | MW_EPWMXBAR29_GPIO24_SEL == 1 | MW_EPWMXBAR29_GPIO25_SEL == 1 | MW_EPWMXBAR29_GPIO26_SEL == 1 | MW_EPWMXBAR29_GPIO27_SEL == 1 | MW_EPWMXBAR29_GPIO28_SEL == 1 | MW_EPWMXBAR29_GPIO29_SEL == 1 | MW_EPWMXBAR29_GPIO30_SEL == 1 | MW_EPWMXBAR29_GPIO31_SEL == 1 | MW_EPWMXBAR29_GPIO32_SEL == 1 | MW_EPWMXBAR29_GPIO33_SEL == 1 | MW_EPWMXBAR29_GPIO34_SEL == 1 | MW_EPWMXBAR29_GPIO35_SEL == 1 | MW_EPWMXBAR29_GPIO36_SEL == 1 | MW_EPWMXBAR29_GPIO37_SEL == 1 | MW_EPWMXBAR29_GPIO38_SEL == 1 | MW_EPWMXBAR29_GPIO39_SEL == 1 | MW_EPWMXBAR29_GPIO40_SEL == 1 | MW_EPWMXBAR29_GPIO41_SEL == 1 | MW_EPWMXBAR29_GPIO42_SEL == 1 | MW_EPWMXBAR29_GPIO43_SEL == 1 | MW_EPWMXBAR29_GPIO44_SEL == 1 | | MW_EPWMXBAR29_GPIO45_SEL == 1 | MW_EPWMXBAR29_GPIO46_SEL == 1 | MW_EPWMXBAR29_GPIO47_SEL == 1| MW_EPWMXBAR29_GPIO48_SEL == 1 | MW_EPWMXBAR29_GPIO49_SEL == 1 | MW_EPWMXBAR29_GPIO50_SEL == 1 | MW_EPWMXBAR29_GPIO51_SEL == 1 | MW_EPWMXBAR29_GPIO52_SEL == 1 | MW_EPWMXBAR29_GPIO53_SEL == 1 | MW_EPWMXBAR29_GPIO54_SEL == 1 | MW_EPWMXBAR29_GPIO55_SEL == 1 | MW_EPWMXBAR29_GPIO56_SEL == 1 | MW_EPWMXBAR29_GPIO57_SEL == 1| MW_EPWMXBAR29_GPIO58_SEL == 1 | MW_EPWMXBAR29_GPIO59_SEL == 1 | MW_EPWMXBAR29_GPIO60_SEL == 1 | MW_EPWMXBAR29_GPIO61_SEL == 1 | MW_EPWMXBAR29_GPIO62_SEL == 1 | MW_EPWMXBAR29_GPIO63_SEL == 1 | MW_EPWMXBAR29_GPIO64_SEL == 1 | | MW_EPWMXBAR29_GPIO59_SEL == 1 | MW_EPWMXBAR29_GPIO60_SEL == 1 | MW_EPWMXBAR29_GPIO61_SEL == 1 | MW_EPWMXBAR29_GPIO62_SEL == 1 | MW_EPWMXBAR29_GPIO63_SEL == 1 | MW_EPWMXBAR29_GPIO64_SEL == 1 | MW_EPWMXBAR29_GPIO65_SEL == 1 | MW_EPWMXBAR29_GPIO66_SEL == 1 | MW_EPWMXBAR29_GPIO67_SEL == 1 | MW_EPWMXBAR29_GPIO68_SEL == 1 | MW_EPWMXBAR29_GPIO69_SEL == 1 | MW_EPWMXBAR29_GPIO70_SEL == 1 | MW_EPWMXBAR29_GPIO71_SEL == 1 | MW_EPWMXBAR29_GPIO72_SEL == 1 | MW_EPWMXBAR29_GPIO73_SEL == 1 | MW_EPWMXBAR29_GPIO74_SEL == 1 | MW_EPWMXBAR29_GPIO75_SEL == 1 | MW_EPWMXBAR29_GPIO76_SEL == 1 | MW_EPWMXBAR29_GPIO77_SEL == 1 | MW_EPWMXBAR29_GPIO78_SEL == 1 | | MW_EPWMXBAR29_GPIO80_SEL == 1 | MW_EPWMXBAR29_GPIO81_SEL == 1 | MW_EPWMXBAR29_GPIO82_SEL == 1 | MW_EPWMXBAR29_GPIO83_SEL == 1 | MW_EPWMXBAR29_GPIO84_SEL == 1 | MW_EPWMXBAR29_GPIO85_SEL == 1 | MW_EPWMXBAR29_GPIO86_SEL == 1 | MW_EPWMXBAR29_GPIO87_SEL == 1 | MW_EPWMXBAR29_GPIO88_SEL == 1 | MW_EPWMXBAR29_GPIO89_SEL == 1 | MW_EPWMXBAR29_GPIO90_SEL == 1 | MW_EPWMXBAR29_GPIO91_SEL == 1 | MW_EPWMXBAR29_GPIO92_SEL == 1 | MW_EPWMXBAR29_GPIO93_SEL == 1 | MW_EPWMXBAR29_GPIO94_SEL == 1 | MW_EPWMXBAR29_GPIO95_SEL == 1 | MW_EPWMXBAR29_GPIO96_SEL == 1 | MW_EPWMXBAR29_GPIO97_SEL == 1 | MW_EPWMXBAR29_GPIO98_SEL == 1 | MW_EPWMXBAR29_GPIO99_SEL == 1 | MW_EPWMXBAR29_GPIO100_SEL == 1 | MW_EPWMXBAR29_GPIO101_SEL == 1 | MW_EPWMXBAR29_GPIO102_SEL == 1 | MW_EPWMXBAR29_GPIO103_SEL == 1 | MW_EPWMXBAR29_GPIO104_SEL == 1 | MW_EPWMXBAR29_GPIO105_SEL == 1 | MW_EPWMXBAR29_GPIO106_SEL == 1 | MW_EPWMXBAR29_GPIO107_SEL == 1 | | MW_EPWMXBAR29_GPIO108_SEL == 1 | MW_EPWMXBAR29_GPIO109_SEL == 1| MW_EPWMXBAR29_GPIO110_SEL == 1 | MW_EPWMXBAR29_GPIO111_SEL == 1 | MW_EPWMXBAR29_GPIO112_SEL == 1 | MW_EPWMXBAR29_GPIO113_SEL == 1 | MW_EPWMXBAR29_GPIO114_SEL == 1 | MW_EPWMXBAR29_GPIO115_SEL == 1 | MW_EPWMXBAR29_GPIO116_SEL == 1 | MW_EPWMXBAR29_GPIO117_SEL == 1 | MW_EPWMXBAR29_GPIO118_SEL == 1 | MW_EPWMXBAR29_GPIO119_SEL == 1 | MW_EPWMXBAR29_GPIO120_SEL == 1 | MW_EPWMXBAR29_GPIO121_SEL == 1 | MW_EPWMXBAR29_GPIO122_SEL == 1 | MW_EPWMXBAR29_GPIO123_SEL == 1 | MW_EPWMXBAR29_GPIO124_SEL == 1 | MW_EPWMXBAR29_GPIO125_SEL == 1 | MW_EPWMXBAR29_GPIO126_SEL == 1 | MW_EPWMXBAR29_GPIO127_SEL == 1 | MW_EPWMXBAR29_GPIO128_SEL == 1 | MW_EPWMXBAR29_GPIO129_SEL == 1 | MW_EPWMXBAR29_GPIO130_SEL == 1 | MW_EPWMXBAR29_GPIO131_SEL == 1 | MW_EPWMXBAR29_GPIO132_SEL == 1 | MW_EPWMXBAR29_GPIO133_SEL == 1 | MW_EPWMXBAR29_GPIO134_SEL == 1 | MW_EPWMXBAR29_GPIO135_SEL == 1 | MW_EPWMXBAR29_GPIO136_SEL == 1 | MW_EPWMXBAR29_GPIO137_SEL == 1 | MW_EPWMXBAR29_GPIO139_SEL == 1 | MW_EPWMXBAR29_GPIO140_SEL == 1 | MW_EPWMXBAR29_GPIO141_SEL == 1 | MW_EPWMXBAR29_GPIO142_SEL == 1 | MW_EPWMXBAR29_GPIO143_SEL == 1 | MW_EPWMXBAR29_ICSSGPO0_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO1_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO2_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO3_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO4_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO5_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO6_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO7_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO8_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO9_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO10_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO11_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO12_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO13_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO14_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO15_PORT0_SEL == 1 | MW_EPWMXBAR29_ICSSGPO0_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO1_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO2_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO3_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO4_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO5_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO6_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO7_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO8_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO9_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO10_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO11_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO12_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO13_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO14_PORT1_SEL == 1 | MW_EPWMXBAR29_ICSSGPO15_PORT1_SEL == 1)
    //     group3_epwmxbar[29] = EPWM_XBAR_29;
    // else
        group3_epwmxbar[29] = 0;
    group4_epwmxbar[29] = (MW_EPWMXBAR29_EPWM0_TRIPOUT_SEL << 0) | (MW_EPWMXBAR29_EPWM1_TRIPOUT_SEL << 1) | (MW_EPWMXBAR29_EPWM2_TRIPOUT_SEL << 2) | (MW_EPWMXBAR29_EPWM3_TRIPOUT_SEL << 3) | (MW_EPWMXBAR29_EPWM4_TRIPOUT_SEL << 4) | (MW_EPWMXBAR29_EPWM5_TRIPOUT_SEL << 5) | (MW_EPWMXBAR29_EPWM6_TRIPOUT_SEL << 6) | (MW_EPWMXBAR29_EPWM7_TRIPOUT_SEL << 7) | (MW_EPWMXBAR29_EPWM8_TRIPOUT_SEL << 8) | (MW_EPWMXBAR29_EPWM9_TRIPOUT_SEL << 9);
    group5_epwmxbar[29] = (MW_EPWMXBAR29_DEL0_TRIP_SEL << 0) | (MW_EPWMXBAR29_DEL1_TRIP_SEL << 1) | (MW_EPWMXBAR29_DEL2_TRIP_SEL << 2) | (MW_EPWMXBAR29_DEL3_TRIP_SEL << 3) | (MW_EPWMXBAR29_DEL4_TRIP_SEL << 4) | (MW_EPWMXBAR29_DEL5_TRIP_SEL << 5) | (MW_EPWMXBAR29_DEL6_TRIP_SEL << 6) | (MW_EPWMXBAR29_DEL7_TRIP_SEL << 7) | (MW_EPWMXBAR29_DEL8_TRIP_SEL << 8) | (MW_EPWMXBAR29_DEL9_TRIP_SEL << 9);
    group6_epwmxbar[29] = (MW_EPWMXBAR29_DEL0_ACTIVE_SEL << 0) | (MW_EPWMXBAR29_DEL1_ACTIVE_SEL << 1) | (MW_EPWMXBAR29_DEL2_ACTIVE_SEL << 2) | (MW_EPWMXBAR29_DEL3_ACTIVE_SEL << 3) | (MW_EPWMXBAR29_DEL4_ACTIVE_SEL << 4) | (MW_EPWMXBAR29_DEL5_ACTIVE_SEL << 5) | (MW_EPWMXBAR29_DEL6_ACTIVE_SEL << 6) | (MW_EPWMXBAR29_DEL7_ACTIVE_SEL << 7) | (MW_EPWMXBAR29_DEL8_ACTIVE_SEL << 8) | (MW_EPWMXBAR29_DEL9_ACTIVE_SEL << 9);
    group7_epwmxbar[29] = (MW_EPWMXBAR29_EQEP0_ERR_SEL << 0) | (MW_EPWMXBAR29_EQEP1_ERR_SEL << 1) | (MW_EPWMXBAR29_FSIRX0_RX_TRIG0_SEL << 8) | (MW_EPWMXBAR29_FSIRX0_RX_TRIG1_SEL << 9) | (MW_EPWMXBAR29_FSIRX0_RX_TRIG2_SEL << 10) | (MW_EPWMXBAR29_FSIRX0_RX_TRIG3_SEL << 11) | (MW_EPWMXBAR29_ECAP0_TRIPOUT_SEL << 16) | (MW_EPWMXBAR29_ECAP1_TRIPOUT_SEL << 17) | (MW_EPWMXBAR29_ECAP2_TRIPOUT_SEL << 18) | (MW_EPWMXBAR29_ECAP3_TRIPOUT_SEL << 19) | (MW_EPWMXBAR29_ECAP4_TRIPOUT_SEL << 20) | (MW_EPWMXBAR29_ECAP5_TRIPOUT_SEL << 21) | (MW_EPWMXBAR29_ECAP6_TRIPOUT_SEL << 22) | (MW_EPWMXBAR29_ECAP7_TRIPOUT_SEL << 23);
    group8_epwmxbar[29] = (MW_EPWMXBAR29_SYNCOUTXBAR0_SEL << 0) | (MW_EPWMXBAR29_SYNCOUTXBAR1_SEL << 1) | (MW_EPWMXBAR29_SOCAXBAR_SOCA_SEL << 16) | (MW_EPWMXBAR29_SOCBXBAR_SOCB_SEL << 17);
    latch = (MW_EPWMXBAR0_INV_OUT_LATCH_SEL << 0) | (MW_EPWMXBAR1_INV_OUT_LATCH_SEL << 1) | (MW_EPWMXBAR2_INV_OUT_LATCH_SEL << 2) | (MW_EPWMXBAR3_INV_OUT_LATCH_SEL << 3) | (MW_EPWMXBAR4_INV_OUT_LATCH_SEL << 4) | (MW_EPWMXBAR5_INV_OUT_LATCH_SEL << 5) | (MW_EPWMXBAR6_INV_OUT_LATCH_SEL << 6) | (MW_EPWMXBAR7_INV_OUT_LATCH_SEL << 7) | (MW_EPWMXBAR8_INV_OUT_LATCH_SEL << 8) | (MW_EPWMXBAR9_INV_OUT_LATCH_SEL << 9) | (MW_EPWMXBAR10_INV_OUT_LATCH_SEL << 10) | (MW_EPWMXBAR11_INV_OUT_LATCH_SEL << 11) | (MW_EPWMXBAR12_INV_OUT_LATCH_SEL << 12) | (MW_EPWMXBAR13_INV_OUT_LATCH_SEL << 13) | (MW_EPWMXBAR14_INV_OUT_LATCH_SEL << 14) | (MW_EPWMXBAR15_INV_OUT_LATCH_SEL << 15) | (MW_EPWMXBAR16_INV_OUT_LATCH_SEL << 16) | (MW_EPWMXBAR17_INV_OUT_LATCH_SEL << 17) | (MW_EPWMXBAR18_INV_OUT_LATCH_SEL << 18) | (MW_EPWMXBAR19_INV_OUT_LATCH_SEL << 19) | (MW_EPWMXBAR20_INV_OUT_LATCH_SEL << 20) | (MW_EPWMXBAR21_INV_OUT_LATCH_SEL << 21) | (MW_EPWMXBAR22_INV_OUT_LATCH_SEL << 22) | (MW_EPWMXBAR23_INV_OUT_LATCH_SEL << 23) | (MW_EPWMXBAR24_INV_OUT_LATCH_SEL << 24) | (MW_EPWMXBAR25_INV_OUT_LATCH_SEL << 25) | (MW_EPWMXBAR26_INV_OUT_LATCH_SEL << 26) | (MW_EPWMXBAR27_INV_OUT_LATCH_SEL << 27) | (MW_EPWMXBAR28_INV_OUT_LATCH_SEL << 28) | (MW_EPWMXBAR29_INV_OUT_LATCH_SEL << 29);

    for(uint8_t i = 0; i< 30; i++)
    {
        SOC_xbarSelectPWMXBarInputSource_ext(CSL_CONTROLSS_PWMXBAR_U_BASE, i, group0_epwmxbar[i], group1_epwmxbar[i], group2_epwmxbar[i], group3_epwmxbar[i], group4_epwmxbar[i], group5_epwmxbar[i], group6_epwmxbar[i], group7_epwmxbar[i], group8_epwmxbar[i], 0);
    }

    SOC_xbarInvertPWMXBarOutputSignalBeforeLatch(CSL_CONTROLSS_PWMXBAR_U_BASE, latch);

}

void Drivers_outputXbarOpen(void)
{
    uint32_t group00_outputxbar[16];
    uint32_t group01_outputxbar[16];
    uint32_t group02_outputxbar[16];
    uint32_t group03_outputxbar[16];
    uint32_t group04_outputxbar[16];
    uint32_t group05_outputxbar[16];
    uint32_t group06_outputxbar[16];
    uint32_t group07_outputxbar[16];
    uint32_t group08_outputxbar[16];
    uint32_t group09_outputxbar[16];
    

    uint32_t invertOutputBeforeLatch[16];
    uint32_t invertOutput[16];
    uint32_t latchSignalSource[16];
    uint32_t stretchedPulseOutput[16];
    uint32_t stretchedPulseLength[16];

    group00_outputxbar[0] = (MW_OUTPUTXBAR0_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR0_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR0_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR0_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR0_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR0_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR0_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR0_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR0_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR0_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[0] = (MW_OUTPUTXBAR0_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR0_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR0_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR0_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR0_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR0_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR0_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR0_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR0_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR0_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[0] = (MW_OUTPUTXBAR0_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR0_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR0_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR0_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR0_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR0_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR0_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR0_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR0_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR0_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[0] = (MW_OUTPUTXBAR0_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR0_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR0_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR0_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR0_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR0_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR0_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR0_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR0_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR0_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[0] = (MW_OUTPUTXBAR0_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR0_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR0_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR0_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR0_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR0_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR0_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR0_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR0_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR0_DEL9_TRIP_SEL<<9);
    group05_outputxbar[0] = (MW_OUTPUTXBAR0_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR0_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR0_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR0_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR0_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR0_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR0_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR0_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR0_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR0_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR0_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR0_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR0_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR0_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR0_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR0_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR0_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR0_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR0_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR0_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR0_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR0_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR0_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR0_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[0] = (MW_OUTPUTXBAR0_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR0_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR0_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR0_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR0_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR0_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR0_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR0_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR0_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR0_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR0_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR0_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR0_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR0_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR0_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR0_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR0_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR0_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[0] = (MW_OUTPUTXBAR0_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR0_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR0_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR0_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR0_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR0_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR0_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR0_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR0_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR0_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR0_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR0_ADC2_EVT4_SEL<<11);
    group08_outputxbar[0] = (MW_OUTPUTXBAR0_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR0_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR0_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR0_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR0_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR0_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR0_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR0_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR0_ECAP0_SEL<<10) | (MW_OUTPUTXBAR0_ECAP1_SEL<<11) | (MW_OUTPUTXBAR0_ECAP2_SEL<<12) | (MW_OUTPUTXBAR0_ECAP3_SEL<<13) | (MW_OUTPUTXBAR0_ECAP4_SEL<<14) | (MW_OUTPUTXBAR0_ECAP5_SEL<<15) | (MW_OUTPUTXBAR0_ECAP6_SEL<<16) | (MW_OUTPUTXBAR0_ECAP7_SEL<<17);
    group09_outputxbar[0] =  (MW_OUTPUTXBAR0_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR0_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR0_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR0_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR2_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR2_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[1] = (MW_OUTPUTXBAR1_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR1_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR1_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR1_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR1_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR1_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR1_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR1_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR1_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR1_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[1] = (MW_OUTPUTXBAR1_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR1_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR1_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR1_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR1_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR1_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR1_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR1_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR1_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR1_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[1] = (MW_OUTPUTXBAR1_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR1_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR1_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR1_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR1_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR1_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR1_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR1_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR1_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR1_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[1] = (MW_OUTPUTXBAR1_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR1_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR1_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR1_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR1_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR1_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR1_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR1_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR1_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR1_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[1] = (MW_OUTPUTXBAR1_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR1_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR1_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR1_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR1_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR1_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR1_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR1_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR1_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR1_DEL9_TRIP_SEL<<9);
    group05_outputxbar[1] = (MW_OUTPUTXBAR1_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR1_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR1_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR1_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR1_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR1_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR1_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR1_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR1_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR1_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR1_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR1_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR1_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR1_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR1_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR1_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR1_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR1_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR1_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR1_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR1_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR1_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR1_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR1_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[1] = (MW_OUTPUTXBAR1_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR1_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR1_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR1_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR1_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR1_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR1_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR1_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR1_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR1_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR1_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR1_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR1_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR1_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR1_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR1_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR1_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR1_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[1] = (MW_OUTPUTXBAR1_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR1_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR1_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR1_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR1_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR1_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR1_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR1_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR1_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR1_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR1_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR1_ADC2_EVT4_SEL<<11);
    group08_outputxbar[1] = (MW_OUTPUTXBAR1_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR1_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR1_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR1_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR1_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR1_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR1_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR1_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR1_ECAP0_SEL<<10) | (MW_OUTPUTXBAR1_ECAP1_SEL<<11) | (MW_OUTPUTXBAR1_ECAP2_SEL<<12) | (MW_OUTPUTXBAR1_ECAP3_SEL<<13) | (MW_OUTPUTXBAR1_ECAP4_SEL<<14) | (MW_OUTPUTXBAR1_ECAP5_SEL<<15) | (MW_OUTPUTXBAR1_ECAP6_SEL<<16) | (MW_OUTPUTXBAR1_ECAP7_SEL<<17);
    group09_outputxbar[1] =  (MW_OUTPUTXBAR1_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR1_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR1_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR1_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR2_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR2_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[2] = (MW_OUTPUTXBAR2_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR2_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR2_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR2_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR2_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR2_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR2_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR2_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR2_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR2_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[2] = (MW_OUTPUTXBAR2_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR2_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR2_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR2_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR2_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR2_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR2_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR2_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR2_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR2_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[2] = (MW_OUTPUTXBAR2_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR2_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR2_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR2_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR2_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR2_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR2_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR2_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR2_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR2_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[2] = (MW_OUTPUTXBAR2_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR2_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR2_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR2_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR2_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR2_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR2_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR2_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR2_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR2_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[2] = (MW_OUTPUTXBAR2_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR2_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR2_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR2_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR2_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR2_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR2_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR2_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR2_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR2_DEL9_TRIP_SEL<<9);
    group05_outputxbar[2] = (MW_OUTPUTXBAR2_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR2_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR2_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR2_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR2_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR2_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR2_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR2_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR2_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR2_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR2_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR2_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR2_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR2_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR2_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR2_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR2_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR2_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR2_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR2_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR2_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR2_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR2_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR2_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[2] = (MW_OUTPUTXBAR2_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR2_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR2_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR2_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR2_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR2_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR2_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR2_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR2_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR2_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR2_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR2_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR2_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR2_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR2_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR2_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR2_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR2_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[2] = (MW_OUTPUTXBAR2_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR2_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR2_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR2_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR2_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR2_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR2_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR2_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR2_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR2_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR2_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR2_ADC2_EVT4_SEL<<11);
    group08_outputxbar[2] = (MW_OUTPUTXBAR2_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR2_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR2_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR2_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR2_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR2_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR2_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR2_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR2_ECAP0_SEL<<10) | (MW_OUTPUTXBAR2_ECAP1_SEL<<11) | (MW_OUTPUTXBAR2_ECAP2_SEL<<12) | (MW_OUTPUTXBAR2_ECAP3_SEL<<13) | (MW_OUTPUTXBAR2_ECAP4_SEL<<14) | (MW_OUTPUTXBAR2_ECAP5_SEL<<15) | (MW_OUTPUTXBAR2_ECAP6_SEL<<16) | (MW_OUTPUTXBAR2_ECAP7_SEL<<17);
    group09_outputxbar[2] =  (MW_OUTPUTXBAR2_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR2_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR2_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR2_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR2_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR2_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR2_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[3] = (MW_OUTPUTXBAR3_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR3_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR3_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR3_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR3_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR3_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR3_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR3_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR3_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR3_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[3] = (MW_OUTPUTXBAR3_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR3_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR3_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR3_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR3_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR3_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR3_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR3_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR3_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR3_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[3] = (MW_OUTPUTXBAR3_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR3_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR3_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR3_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR3_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR3_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR3_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR3_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR3_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR3_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[3] = (MW_OUTPUTXBAR3_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR3_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR3_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR3_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR3_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR3_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR3_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR3_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR3_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR3_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[3] = (MW_OUTPUTXBAR3_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR3_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR3_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR3_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR3_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR3_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR3_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR3_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR3_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR3_DEL9_TRIP_SEL<<9);
    group05_outputxbar[3] = (MW_OUTPUTXBAR3_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR3_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR3_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR3_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR3_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR3_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR3_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR3_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR3_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR3_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR3_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR3_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR3_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR3_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR3_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR3_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR3_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR3_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR3_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR3_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR3_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR3_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR3_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR3_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[3] = (MW_OUTPUTXBAR3_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR3_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR3_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR3_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR3_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR3_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR3_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR3_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR3_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR3_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR3_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR3_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR3_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR3_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR3_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR3_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR3_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR3_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[3] = (MW_OUTPUTXBAR3_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR3_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR3_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR3_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR3_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR3_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR3_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR3_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR3_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR3_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR3_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR3_ADC2_EVT4_SEL<<11);
    group08_outputxbar[3] = (MW_OUTPUTXBAR3_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR3_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR3_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR3_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR3_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR3_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR3_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR3_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR3_ECAP0_SEL<<10) | (MW_OUTPUTXBAR3_ECAP1_SEL<<11) | (MW_OUTPUTXBAR3_ECAP2_SEL<<12) | (MW_OUTPUTXBAR3_ECAP3_SEL<<13) | (MW_OUTPUTXBAR3_ECAP4_SEL<<14) | (MW_OUTPUTXBAR3_ECAP5_SEL<<15) | (MW_OUTPUTXBAR3_ECAP6_SEL<<16) | (MW_OUTPUTXBAR3_ECAP7_SEL<<17);
    group09_outputxbar[3] =  (MW_OUTPUTXBAR3_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR3_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR3_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR3_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR3_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR3_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR3_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR3_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR3_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR3_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[4] = (MW_OUTPUTXBAR4_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR4_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR4_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR4_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR4_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR4_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR4_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR4_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR4_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR4_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[4] = (MW_OUTPUTXBAR4_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR4_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR4_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR4_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR4_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR4_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR4_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR4_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR4_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR4_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[4] = (MW_OUTPUTXBAR4_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR4_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR4_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR4_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR4_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR4_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR4_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR4_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR4_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR4_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[4] = (MW_OUTPUTXBAR4_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR4_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR4_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR4_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR4_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR4_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR4_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR4_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR4_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR4_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[4] = (MW_OUTPUTXBAR4_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR4_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR4_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR4_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR4_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR4_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR4_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR4_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR4_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR4_DEL9_TRIP_SEL<<9);
    group05_outputxbar[4] = (MW_OUTPUTXBAR4_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR4_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR4_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR4_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR4_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR4_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR4_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR4_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR4_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR4_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR4_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR4_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR4_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR4_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR4_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR4_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR4_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR4_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR4_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR4_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR4_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR4_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR4_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR4_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[4] = (MW_OUTPUTXBAR4_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR4_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR4_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR4_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR4_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR4_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR4_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR4_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR4_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR4_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR4_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR4_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR4_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR4_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR4_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR4_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR4_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR4_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[4] = (MW_OUTPUTXBAR4_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR4_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR4_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR4_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR4_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR4_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR4_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR4_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR4_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR4_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR4_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR4_ADC2_EVT4_SEL<<11);
    group08_outputxbar[4] = (MW_OUTPUTXBAR4_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR4_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR4_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR4_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR4_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR4_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR4_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR4_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR4_ECAP0_SEL<<10) | (MW_OUTPUTXBAR4_ECAP1_SEL<<11) | (MW_OUTPUTXBAR4_ECAP2_SEL<<12) | (MW_OUTPUTXBAR4_ECAP3_SEL<<13) | (MW_OUTPUTXBAR4_ECAP4_SEL<<14) | (MW_OUTPUTXBAR4_ECAP5_SEL<<15) | (MW_OUTPUTXBAR4_ECAP6_SEL<<16) | (MW_OUTPUTXBAR4_ECAP7_SEL<<17);
    group09_outputxbar[4] =  (MW_OUTPUTXBAR4_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR4_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR4_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR4_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR4_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR4_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR4_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR4_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR4_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR4_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[5] = (MW_OUTPUTXBAR5_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR5_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR5_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR5_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR5_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR5_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR5_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR5_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR5_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR5_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[5] = (MW_OUTPUTXBAR5_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR5_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR5_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR5_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR5_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR5_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR5_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR5_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR5_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR5_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[5] = (MW_OUTPUTXBAR5_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR5_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR5_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR5_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR5_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR5_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR5_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR5_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR5_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR5_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[5] = (MW_OUTPUTXBAR5_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR5_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR5_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR5_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR5_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR5_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR5_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR5_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR5_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR5_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[5] = (MW_OUTPUTXBAR5_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR5_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR5_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR5_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR5_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR5_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR5_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR5_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR5_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR5_DEL9_TRIP_SEL<<9);
    group05_outputxbar[5] = (MW_OUTPUTXBAR5_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR5_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR5_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR5_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR5_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR5_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR5_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR5_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR5_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR5_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR5_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR5_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR5_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR5_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR5_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR5_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR5_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR5_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR5_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR5_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR5_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR5_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR5_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR5_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[5] = (MW_OUTPUTXBAR5_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR5_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR5_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR5_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR5_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR5_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR5_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR5_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR5_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR5_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR5_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR5_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR5_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR5_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR5_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR5_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR5_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR5_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[5] = (MW_OUTPUTXBAR5_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR5_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR5_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR5_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR5_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR5_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR5_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR5_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR5_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR5_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR5_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR5_ADC2_EVT4_SEL<<11);
    group08_outputxbar[5] = (MW_OUTPUTXBAR5_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR5_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR5_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR5_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR5_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR5_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR5_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR5_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR5_ECAP0_SEL<<10) | (MW_OUTPUTXBAR5_ECAP1_SEL<<11) | (MW_OUTPUTXBAR5_ECAP2_SEL<<12) | (MW_OUTPUTXBAR5_ECAP3_SEL<<13) | (MW_OUTPUTXBAR5_ECAP4_SEL<<14) | (MW_OUTPUTXBAR5_ECAP5_SEL<<15) | (MW_OUTPUTXBAR5_ECAP6_SEL<<16) | (MW_OUTPUTXBAR5_ECAP7_SEL<<17);
    group09_outputxbar[5] =  (MW_OUTPUTXBAR5_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR5_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR5_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR5_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR5_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR5_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR5_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR5_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR5_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR5_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[6] = (MW_OUTPUTXBAR6_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR6_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR6_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR6_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR6_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR6_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR6_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR6_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR6_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR6_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[6] = (MW_OUTPUTXBAR6_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR6_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR6_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR6_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR6_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR6_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR6_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR6_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR6_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR6_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[6] = (MW_OUTPUTXBAR6_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR6_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR6_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR6_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR6_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR6_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR6_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR6_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR6_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR6_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[6] = (MW_OUTPUTXBAR6_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR6_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR6_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR6_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR6_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR6_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR6_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR6_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR6_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR6_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[6] = (MW_OUTPUTXBAR6_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR6_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR6_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR6_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR6_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR6_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR6_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR6_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR6_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR6_DEL9_TRIP_SEL<<9);
    group05_outputxbar[6] = (MW_OUTPUTXBAR6_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR6_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR6_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR6_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR6_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR6_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR6_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR6_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR6_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR6_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR6_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR6_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR6_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR6_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR6_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR6_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR6_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR6_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR6_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR6_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR6_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR6_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR6_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR6_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[6] = (MW_OUTPUTXBAR6_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR6_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR6_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR6_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR6_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR6_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR6_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR6_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR6_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR6_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR6_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR6_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR6_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR6_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR6_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR6_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR6_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR6_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[6] = (MW_OUTPUTXBAR6_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR6_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR6_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR6_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR6_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR6_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR6_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR6_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR6_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR6_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR6_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR6_ADC2_EVT4_SEL<<11);
    group08_outputxbar[6] = (MW_OUTPUTXBAR6_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR6_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR6_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR6_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR6_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR6_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR6_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR6_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR6_ECAP0_SEL<<10) | (MW_OUTPUTXBAR6_ECAP1_SEL<<11) | (MW_OUTPUTXBAR6_ECAP2_SEL<<12) | (MW_OUTPUTXBAR6_ECAP3_SEL<<13) | (MW_OUTPUTXBAR6_ECAP4_SEL<<14) | (MW_OUTPUTXBAR6_ECAP5_SEL<<15) | (MW_OUTPUTXBAR6_ECAP6_SEL<<16) | (MW_OUTPUTXBAR6_ECAP7_SEL<<17);
    group09_outputxbar[6] =  (MW_OUTPUTXBAR6_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR6_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR6_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR6_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR6_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR6_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR6_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR6_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR6_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR6_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[7] = (MW_OUTPUTXBAR7_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR7_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR7_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR7_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR7_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR7_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR7_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR7_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR7_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR7_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[7] = (MW_OUTPUTXBAR7_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR7_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR7_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR7_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR7_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR7_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR7_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR7_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR7_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR7_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[7] = (MW_OUTPUTXBAR7_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR7_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR7_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR7_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR7_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR7_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR7_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR7_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR7_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR7_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[7] = (MW_OUTPUTXBAR7_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR7_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR7_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR7_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR7_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR7_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR7_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR7_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR7_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR7_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[7] = (MW_OUTPUTXBAR7_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR7_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR7_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR7_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR7_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR7_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR7_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR7_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR7_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR7_DEL9_TRIP_SEL<<9);
    group05_outputxbar[7] = (MW_OUTPUTXBAR7_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR7_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR7_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR7_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR7_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR7_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR7_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR7_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR7_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR7_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR7_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR7_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR7_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR7_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR7_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR7_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR7_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR7_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR7_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR7_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR7_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR7_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR7_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR7_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[7] = (MW_OUTPUTXBAR7_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR7_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR7_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR7_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR7_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR7_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR7_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR7_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR7_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR7_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR7_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR7_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR7_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR7_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR7_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR7_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR7_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR7_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[7] = (MW_OUTPUTXBAR7_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR7_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR7_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR7_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR7_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR7_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR7_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR7_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR7_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR7_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR7_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR7_ADC2_EVT4_SEL<<11);
    group08_outputxbar[7] = (MW_OUTPUTXBAR7_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR7_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR7_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR7_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR7_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR7_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR7_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR7_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR7_ECAP0_SEL<<10) | (MW_OUTPUTXBAR7_ECAP1_SEL<<11) | (MW_OUTPUTXBAR7_ECAP2_SEL<<12) | (MW_OUTPUTXBAR7_ECAP3_SEL<<13) | (MW_OUTPUTXBAR7_ECAP4_SEL<<14) | (MW_OUTPUTXBAR7_ECAP5_SEL<<15) | (MW_OUTPUTXBAR7_ECAP6_SEL<<16) | (MW_OUTPUTXBAR7_ECAP7_SEL<<17);
    group09_outputxbar[7] =  (MW_OUTPUTXBAR7_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR7_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR7_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR7_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR7_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR7_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR7_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR7_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR7_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR7_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[8] = (MW_OUTPUTXBAR8_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR8_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR8_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR8_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR8_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR8_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR8_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR8_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR8_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR8_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[8] = (MW_OUTPUTXBAR8_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR8_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR8_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR8_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR8_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR8_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR8_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR8_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR8_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR8_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[8] = (MW_OUTPUTXBAR8_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR8_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR8_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR8_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR8_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR8_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR8_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR8_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR8_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR8_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[8] = (MW_OUTPUTXBAR8_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR8_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR8_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR8_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR8_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR8_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR8_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR8_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR8_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR8_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[8] = (MW_OUTPUTXBAR8_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR8_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR8_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR8_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR8_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR8_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR8_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR8_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR8_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR8_DEL9_TRIP_SEL<<9);
    group05_outputxbar[8] = (MW_OUTPUTXBAR8_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR8_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR8_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR8_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR8_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR8_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR8_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR8_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR8_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR8_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR8_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR8_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR8_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR8_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR8_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR8_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR8_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR8_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR8_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR8_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR8_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR8_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR8_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR8_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[8] = (MW_OUTPUTXBAR8_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR8_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR8_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR8_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR8_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR8_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR8_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR8_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR8_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR8_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR8_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR8_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR8_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR8_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR8_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR8_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR8_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR8_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[8] = (MW_OUTPUTXBAR8_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR8_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR8_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR8_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR8_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR8_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR8_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR8_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR8_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR8_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR8_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR8_ADC2_EVT4_SEL<<11);
    group08_outputxbar[8] = (MW_OUTPUTXBAR8_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR8_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR8_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR8_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR8_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR8_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR8_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR8_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR8_ECAP0_SEL<<10) | (MW_OUTPUTXBAR8_ECAP1_SEL<<11) | (MW_OUTPUTXBAR8_ECAP2_SEL<<12) | (MW_OUTPUTXBAR8_ECAP3_SEL<<13) | (MW_OUTPUTXBAR8_ECAP4_SEL<<14) | (MW_OUTPUTXBAR8_ECAP5_SEL<<15) | (MW_OUTPUTXBAR8_ECAP6_SEL<<16) | (MW_OUTPUTXBAR8_ECAP7_SEL<<17);
    group09_outputxbar[8] = (MW_OUTPUTXBAR8_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR8_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR8_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR8_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR8_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR8_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR8_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR8_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR8_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR8_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[9] = (MW_OUTPUTXBAR9_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR9_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR9_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR9_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR9_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR9_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR9_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR9_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR9_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR9_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[9] = (MW_OUTPUTXBAR9_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR9_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR9_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR9_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR9_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR9_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR9_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR9_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR9_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR9_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[9] = (MW_OUTPUTXBAR9_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR9_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR9_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR9_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR9_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR9_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR9_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR9_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR9_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR9_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[9] = (MW_OUTPUTXBAR9_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR9_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR9_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR9_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR9_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR9_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR9_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR9_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR9_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR9_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[9] = (MW_OUTPUTXBAR9_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR9_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR9_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR9_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR9_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR9_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR9_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR9_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR9_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR9_DEL9_TRIP_SEL<<9);
    group05_outputxbar[9] = (MW_OUTPUTXBAR9_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR9_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR9_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR9_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR9_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR9_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR9_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR9_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR9_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR9_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR9_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR9_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR9_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR9_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR9_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR9_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR9_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR9_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR9_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR9_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR9_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR9_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR9_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR9_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[9] = (MW_OUTPUTXBAR9_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR9_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR9_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR9_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR9_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR9_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR9_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR9_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR9_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR9_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR9_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR9_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR9_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR9_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR9_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR9_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR9_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR9_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[9] = (MW_OUTPUTXBAR9_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR9_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR9_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR9_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR9_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR9_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR9_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR9_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR9_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR9_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR9_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR9_ADC2_EVT4_SEL<<11);
    group08_outputxbar[9] = (MW_OUTPUTXBAR9_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR9_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR9_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR9_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR9_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR9_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR9_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR9_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR9_ECAP0_SEL<<10) | (MW_OUTPUTXBAR9_ECAP1_SEL<<11) | (MW_OUTPUTXBAR9_ECAP2_SEL<<12) | (MW_OUTPUTXBAR9_ECAP3_SEL<<13) | (MW_OUTPUTXBAR9_ECAP4_SEL<<14) | (MW_OUTPUTXBAR9_ECAP5_SEL<<15) | (MW_OUTPUTXBAR9_ECAP6_SEL<<16) | (MW_OUTPUTXBAR9_ECAP7_SEL<<17);
    group09_outputxbar[9] =  (MW_OUTPUTXBAR9_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR9_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR9_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR9_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR9_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR9_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR9_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR9_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR9_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR9_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[10] = (MW_OUTPUTXBAR10_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR10_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR10_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR10_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR10_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR10_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR10_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR10_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR10_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR10_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[10] = (MW_OUTPUTXBAR10_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR10_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR10_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR10_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR10_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR10_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR10_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR10_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR10_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR10_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[10] = (MW_OUTPUTXBAR10_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR10_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR10_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR10_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR10_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR10_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR10_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR10_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR10_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR10_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[10] = (MW_OUTPUTXBAR10_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR10_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR10_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR10_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR10_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR10_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR10_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR10_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR10_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR10_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[10] = (MW_OUTPUTXBAR10_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR10_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR10_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR10_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR10_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR10_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR10_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR10_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR10_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR10_DEL9_TRIP_SEL<<9);
    group05_outputxbar[10] = (MW_OUTPUTXBAR10_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR10_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR10_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR10_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR10_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR10_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR10_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR10_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR10_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR10_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR10_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR10_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR10_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR10_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR10_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR10_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR10_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR10_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR10_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR10_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR10_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR10_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR10_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR10_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[10] = (MW_OUTPUTXBAR10_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR10_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR10_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR10_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR10_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR10_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR10_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR10_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR10_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR10_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR10_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR10_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR10_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR10_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR10_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR10_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR10_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR10_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[10] = (MW_OUTPUTXBAR10_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR10_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR10_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR10_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR10_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR10_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR10_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR10_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR10_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR10_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR10_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR10_ADC2_EVT4_SEL<<11);
    group08_outputxbar[10] = (MW_OUTPUTXBAR10_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR10_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR10_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR10_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR10_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR10_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR10_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR10_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR10_ECAP0_SEL<<10) | (MW_OUTPUTXBAR10_ECAP1_SEL<<11) | (MW_OUTPUTXBAR10_ECAP2_SEL<<12) | (MW_OUTPUTXBAR10_ECAP3_SEL<<13) | (MW_OUTPUTXBAR10_ECAP4_SEL<<14) | (MW_OUTPUTXBAR10_ECAP5_SEL<<15) | (MW_OUTPUTXBAR10_ECAP6_SEL<<16) | (MW_OUTPUTXBAR10_ECAP7_SEL<<17);
    group09_outputxbar[10] =  (MW_OUTPUTXBAR10_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR10_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR10_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR10_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR12_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR12_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[11] = (MW_OUTPUTXBAR11_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR11_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR11_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR11_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR11_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR11_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR11_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR11_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR11_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR11_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[11] = (MW_OUTPUTXBAR11_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR11_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR11_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR11_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR11_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR11_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR11_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR11_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR11_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR11_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[11] = (MW_OUTPUTXBAR11_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR11_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR11_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR11_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR11_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR11_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR11_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR11_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR11_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR11_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[11] = (MW_OUTPUTXBAR11_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR11_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR11_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR11_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR11_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR11_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR11_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR11_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR11_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR11_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[11] = (MW_OUTPUTXBAR11_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR11_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR11_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR11_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR11_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR11_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR11_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR11_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR11_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR11_DEL9_TRIP_SEL<<9);
    group05_outputxbar[11] = (MW_OUTPUTXBAR11_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR11_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR11_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR11_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR11_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR11_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR11_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR11_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR11_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR11_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR11_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR11_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR11_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR11_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR11_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR11_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR11_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR11_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR11_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR11_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR11_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR11_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR11_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR11_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[11] = (MW_OUTPUTXBAR11_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR11_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR11_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR11_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR11_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR11_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR11_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR11_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR11_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR11_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR11_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR11_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR11_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR11_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR11_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR11_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR11_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR11_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[11] = (MW_OUTPUTXBAR11_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR11_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR11_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR11_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR11_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR11_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR11_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR11_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR11_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR11_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR11_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR11_ADC2_EVT4_SEL<<11);
    group08_outputxbar[11] = (MW_OUTPUTXBAR11_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR11_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR11_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR11_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR11_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR11_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR11_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR11_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR11_ECAP0_SEL<<10) | (MW_OUTPUTXBAR11_ECAP1_SEL<<11) | (MW_OUTPUTXBAR11_ECAP2_SEL<<12) | (MW_OUTPUTXBAR11_ECAP3_SEL<<13) | (MW_OUTPUTXBAR11_ECAP4_SEL<<14) | (MW_OUTPUTXBAR11_ECAP5_SEL<<15) | (MW_OUTPUTXBAR11_ECAP6_SEL<<16) | (MW_OUTPUTXBAR11_ECAP7_SEL<<17);
    group09_outputxbar[11] =  (MW_OUTPUTXBAR11_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR11_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR11_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR11_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR12_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR12_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[12] = (MW_OUTPUTXBAR12_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR12_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR12_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR12_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR12_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR12_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR12_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR12_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR12_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR12_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[12] = (MW_OUTPUTXBAR12_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR12_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR12_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR12_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR12_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR12_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR12_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR12_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR12_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR12_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[12] = (MW_OUTPUTXBAR12_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR12_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR12_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR12_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR12_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR12_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR12_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR12_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR12_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR12_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[12] = (MW_OUTPUTXBAR12_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR12_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR12_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR12_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR12_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR12_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR12_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR12_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR12_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR12_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[12] = (MW_OUTPUTXBAR12_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR12_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR12_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR12_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR12_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR12_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR12_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR12_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR12_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR12_DEL9_TRIP_SEL<<9);
    group05_outputxbar[12] = (MW_OUTPUTXBAR12_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR12_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR12_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR12_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR12_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR12_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR12_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR12_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR12_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR12_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR12_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR12_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR12_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR12_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR12_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR12_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR12_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR12_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR12_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR12_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR12_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR12_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR12_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR12_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[12] = (MW_OUTPUTXBAR12_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR12_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR12_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR12_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR12_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR12_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR12_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR12_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR12_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR12_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR12_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR12_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR12_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR12_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR12_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR12_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR12_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR12_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[12] = (MW_OUTPUTXBAR12_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR12_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR12_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR12_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR12_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR12_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR12_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR12_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR12_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR12_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR12_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR12_ADC2_EVT4_SEL<<11);
    group08_outputxbar[12] = (MW_OUTPUTXBAR12_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR12_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR12_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR12_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR12_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR12_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR12_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR12_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR12_ECAP0_SEL<<10) | (MW_OUTPUTXBAR12_ECAP1_SEL<<11) | (MW_OUTPUTXBAR12_ECAP2_SEL<<12) | (MW_OUTPUTXBAR12_ECAP3_SEL<<13) | (MW_OUTPUTXBAR12_ECAP4_SEL<<14) | (MW_OUTPUTXBAR12_ECAP5_SEL<<15) | (MW_OUTPUTXBAR12_ECAP6_SEL<<16) | (MW_OUTPUTXBAR12_ECAP7_SEL<<17);
    group09_outputxbar[12] =  (MW_OUTPUTXBAR12_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR12_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR12_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR12_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR12_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR12_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR12_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[13] = (MW_OUTPUTXBAR13_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR13_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR13_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR13_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR13_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR13_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR13_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR13_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR13_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR13_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[13] = (MW_OUTPUTXBAR13_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR13_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR13_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR13_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR13_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR13_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR13_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR13_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR13_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR13_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[13] = (MW_OUTPUTXBAR13_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR13_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR13_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR13_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR13_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR13_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR13_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR13_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR13_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR13_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[13] = (MW_OUTPUTXBAR13_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR13_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR13_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR13_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR13_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR13_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR13_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR13_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR13_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR13_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[13] = (MW_OUTPUTXBAR13_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR13_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR13_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR13_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR13_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR13_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR13_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR13_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR13_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR13_DEL9_TRIP_SEL<<9);
    group05_outputxbar[13] = (MW_OUTPUTXBAR13_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR13_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR13_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR13_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR13_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR13_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR13_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR13_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR13_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR13_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR13_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR13_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR13_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR13_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR13_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR13_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR13_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR13_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR13_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR13_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR13_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR13_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR13_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR13_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[13] = (MW_OUTPUTXBAR13_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR13_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR13_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR13_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR13_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR13_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR13_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR13_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR13_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR13_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR13_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR13_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR13_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR13_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR13_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR13_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR13_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR13_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[13] = (MW_OUTPUTXBAR13_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR13_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR13_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR13_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR13_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR13_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR13_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR13_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR13_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR13_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR13_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR13_ADC2_EVT4_SEL<<11);
    group08_outputxbar[13] = (MW_OUTPUTXBAR13_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR13_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR13_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR13_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR13_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR13_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR13_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR13_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR13_ECAP0_SEL<<10) | (MW_OUTPUTXBAR13_ECAP1_SEL<<11) | (MW_OUTPUTXBAR13_ECAP2_SEL<<12) | (MW_OUTPUTXBAR13_ECAP3_SEL<<13) | (MW_OUTPUTXBAR13_ECAP4_SEL<<14) | (MW_OUTPUTXBAR13_ECAP5_SEL<<15) | (MW_OUTPUTXBAR13_ECAP6_SEL<<16) | (MW_OUTPUTXBAR13_ECAP7_SEL<<17);
    group09_outputxbar[13] =  (MW_OUTPUTXBAR13_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR13_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR13_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR13_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR13_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR13_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR13_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR13_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR13_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR13_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[14] = (MW_OUTPUTXBAR14_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR14_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR14_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR14_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR14_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR14_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR14_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR14_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR14_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR14_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[14] = (MW_OUTPUTXBAR14_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR14_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR14_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR14_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR14_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR14_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR14_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR14_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR14_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR14_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[14] = (MW_OUTPUTXBAR14_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR14_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR14_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR14_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR14_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR14_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR14_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR14_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR14_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR14_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[14] = (MW_OUTPUTXBAR14_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR14_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR14_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR14_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR14_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR14_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR14_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR14_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR14_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR14_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[14] = (MW_OUTPUTXBAR14_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR14_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR14_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR14_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR14_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR14_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR14_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR14_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR14_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR14_DEL9_TRIP_SEL<<9);
    group05_outputxbar[14] = (MW_OUTPUTXBAR14_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR14_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR14_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR14_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR14_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR14_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR14_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR14_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR14_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR14_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR14_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR14_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR14_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR14_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR14_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR14_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR14_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR14_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR14_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR14_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR14_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR14_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR14_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR14_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[14] = (MW_OUTPUTXBAR14_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR14_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR14_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR14_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR14_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR14_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR14_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR14_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR14_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR14_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR14_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR14_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR14_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR14_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR14_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR14_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR14_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR14_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[14] = (MW_OUTPUTXBAR14_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR14_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR14_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR14_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR14_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR14_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR14_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR14_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR14_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR14_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR14_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR14_ADC2_EVT4_SEL<<11);
    group08_outputxbar[14] = (MW_OUTPUTXBAR14_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR14_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR14_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR14_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR14_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR14_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR14_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR14_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR14_ECAP0_SEL<<10) | (MW_OUTPUTXBAR14_ECAP1_SEL<<11) | (MW_OUTPUTXBAR14_ECAP2_SEL<<12) | (MW_OUTPUTXBAR14_ECAP3_SEL<<13) | (MW_OUTPUTXBAR14_ECAP4_SEL<<14) | (MW_OUTPUTXBAR14_ECAP5_SEL<<15) | (MW_OUTPUTXBAR14_ECAP6_SEL<<16) | (MW_OUTPUTXBAR14_ECAP7_SEL<<17);
    group09_outputxbar[14] =  (MW_OUTPUTXBAR14_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR14_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR14_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR14_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR14_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR14_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR14_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR14_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR14_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR14_INTXBAR_OUT15_SEL<<25);
    group00_outputxbar[15] = (MW_OUTPUTXBAR15_EPWM0_TRIPOUT_SEL<<0) | (MW_OUTPUTXBAR15_EPWM1_TRIPOUT_SEL<<1) | (MW_OUTPUTXBAR15_EPWM2_TRIPOUT_SEL<<2) | (MW_OUTPUTXBAR15_EPWM3_TRIPOUT_SEL<<3) | (MW_OUTPUTXBAR15_EPWM4_TRIPOUT_SEL<<4) | (MW_OUTPUTXBAR15_EPWM5_TRIPOUT_SEL<<5) | (MW_OUTPUTXBAR15_EPWM6_TRIPOUT_SEL<<6) | (MW_OUTPUTXBAR15_EPWM7_TRIPOUT_SEL<<7) | (MW_OUTPUTXBAR15_EPWM8_TRIPOUT_SEL<<8) | (MW_OUTPUTXBAR15_EPWM9_TRIPOUT_SEL<<9);
    group01_outputxbar[15] = (MW_OUTPUTXBAR15_EPWM0_SOCA_SEL<<0) | (MW_OUTPUTXBAR15_EPWM1_SOCA_SEL<<1) | (MW_OUTPUTXBAR15_EPWM2_SOCA_SEL<<2) | (MW_OUTPUTXBAR15_EPWM3_SOCA_SEL<<3) | (MW_OUTPUTXBAR15_EPWM4_SOCA_SEL<<4) | (MW_OUTPUTXBAR15_EPWM5_SOCA_SEL<<5) | (MW_OUTPUTXBAR15_EPWM6_SOCA_SEL<<6) | (MW_OUTPUTXBAR15_EPWM7_SOCA_SEL<<7) | (MW_OUTPUTXBAR15_EPWM8_SOCA_SEL<<8) | (MW_OUTPUTXBAR15_EPWM9_SOCA_SEL<<9);
    group02_outputxbar[15] = (MW_OUTPUTXBAR15_EPWM0_SOCB_SEL<<0) | (MW_OUTPUTXBAR15_EPWM1_SOCB_SEL<<1) | (MW_OUTPUTXBAR15_EPWM2_SOCB_SEL<<2) | (MW_OUTPUTXBAR15_EPWM3_SOCB_SEL<<3) | (MW_OUTPUTXBAR15_EPWM4_SOCB_SEL<<4) | (MW_OUTPUTXBAR15_EPWM5_SOCB_SEL<<5) | (MW_OUTPUTXBAR15_EPWM6_SOCB_SEL<<6) | (MW_OUTPUTXBAR15_EPWM7_SOCB_SEL<<7) | (MW_OUTPUTXBAR15_EPWM8_SOCB_SEL<<8) | (MW_OUTPUTXBAR15_EPWM9_SOCB_SEL<<9);
    group03_outputxbar[15] = (MW_OUTPUTXBAR15_DEL0_ACTIVE_SEL<<0) | (MW_OUTPUTXBAR15_DEL1_ACTIVE_SEL<<1) | (MW_OUTPUTXBAR15_DEL2_ACTIVE_SEL<<2) | (MW_OUTPUTXBAR15_DEL3_ACTIVE_SEL<<3) | (MW_OUTPUTXBAR15_DEL4_ACTIVE_SEL<<4) | (MW_OUTPUTXBAR15_DEL5_ACTIVE_SEL<<5) | (MW_OUTPUTXBAR15_DEL6_ACTIVE_SEL<<6) | (MW_OUTPUTXBAR15_DEL7_ACTIVE_SEL<<7) | (MW_OUTPUTXBAR15_DEL8_ACTIVE_SEL<<8) | (MW_OUTPUTXBAR15_DEL9_ACTIVE_SEL<<9);
    group04_outputxbar[15] = (MW_OUTPUTXBAR15_DEL0_TRIP_SEL<<0) | (MW_OUTPUTXBAR15_DEL1_TRIP_SEL<<1) | (MW_OUTPUTXBAR15_DEL2_TRIP_SEL<<2) | (MW_OUTPUTXBAR15_DEL3_TRIP_SEL<<3) | (MW_OUTPUTXBAR15_DEL4_TRIP_SEL<<4) | (MW_OUTPUTXBAR15_DEL5_TRIP_SEL<<5) | (MW_OUTPUTXBAR15_DEL6_TRIP_SEL<<6) | (MW_OUTPUTXBAR15_DEL7_TRIP_SEL<<7) | (MW_OUTPUTXBAR15_DEL8_TRIP_SEL<<8) | (MW_OUTPUTXBAR15_DEL9_TRIP_SEL<<9);
    group05_outputxbar[15] = (MW_OUTPUTXBAR15_SD0_FILT0_CEVT1_SEL<<0) | (MW_OUTPUTXBAR15_SD0_FILT0_CEVT2_SEL<<1) | (MW_OUTPUTXBAR15_SD0_FILT0_CMPHZ_SEL<<2) | (MW_OUTPUTXBAR15_SD0_FILT1_CEVT1_SEL<<3) | (MW_OUTPUTXBAR15_SD0_FILT1_CEVT2_SEL<<4) | (MW_OUTPUTXBAR15_SD0_FILT1_CMPHZ_SEL<<5) | (MW_OUTPUTXBAR15_SD0_FILT2_CEVT1_SEL<<6) | (MW_OUTPUTXBAR15_SD0_FILT2_CEVT2_SEL<<7) | (MW_OUTPUTXBAR15_SD0_FILT2_CMPHZ_SEL<<8) | (MW_OUTPUTXBAR15_SD0_FILT3_CEVT1_SEL<<9) | (MW_OUTPUTXBAR15_SD0_FILT3_CEVT2_SEL<<10) | (MW_OUTPUTXBAR15_SD0_FILT3_CMPHZ_SEL<<11) | (MW_OUTPUTXBAR15_SD1_FILT0_CEVT1_SEL<<12) | (MW_OUTPUTXBAR15_SD1_FILT0_CEVT2_SEL<<13) | (MW_OUTPUTXBAR15_SD1_FILT0_CMPHZ_SEL<<14) | (MW_OUTPUTXBAR15_SD1_FILT1_CEVT1_SEL<<15) | (MW_OUTPUTXBAR15_SD1_FILT1_CEVT2_SEL<<16) | (MW_OUTPUTXBAR15_SD1_FILT1_CMPHZ_SEL<<17) | (MW_OUTPUTXBAR15_SD1_FILT2_CEVT1_SEL<<18) | (MW_OUTPUTXBAR15_SD1_FILT2_CEVT2_SEL<<19) | (MW_OUTPUTXBAR15_SD1_FILT2_CMPHZ_SEL<<20) | (MW_OUTPUTXBAR15_SD1_FILT3_CEVT1_SEL<<21) | (MW_OUTPUTXBAR15_SD1_FILT3_CEVT2_SEL<<22) | (MW_OUTPUTXBAR15_SD1_FILT3_CMPHZ_SEL<<23);
    group06_outputxbar[15] = (MW_OUTPUTXBAR15_CMPSSA0_CTRIPL_SEL<<0) | (MW_OUTPUTXBAR15_CMPSSA0_CTRIPH_SEL<<1) | (MW_OUTPUTXBAR15_CMPSSA1_CTRIPL_SEL<<2) | (MW_OUTPUTXBAR15_CMPSSA1_CTRIPH_SEL<<3) | (MW_OUTPUTXBAR15_CMPSSA2_CTRIPL_SEL<<4) | (MW_OUTPUTXBAR15_CMPSSA2_CTRIPH_SEL<<5) | (MW_OUTPUTXBAR15_CMPSSA3_CTRIPL_SEL<<6) | (MW_OUTPUTXBAR15_CMPSSA3_CTRIPH_SEL<<7) | (MW_OUTPUTXBAR15_CMPSSA4_CTRIPL_SEL<<8) | (MW_OUTPUTXBAR15_CMPSSA4_CTRIPH_SEL<<9) | (MW_OUTPUTXBAR15_CMPSSA5_CTRIPL_SEL<<10) | (MW_OUTPUTXBAR15_CMPSSA5_CTRIPH_SEL<<11) | (MW_OUTPUTXBAR15_CMPSSA6_CTRIPL_SEL<<12) | (MW_OUTPUTXBAR15_CMPSSA6_CTRIPH_SEL<<13) | (MW_OUTPUTXBAR15_CMPSSA7_CTRIPL_SEL<<14) | (MW_OUTPUTXBAR15_CMPSSA7_CTRIPH_SEL<<15) | (MW_OUTPUTXBAR15_CMPSSA8_CTRIPL_SEL<<16) | (MW_OUTPUTXBAR15_CMPSSA8_CTRIPH_SEL<<17);
    group07_outputxbar[15] = (MW_OUTPUTXBAR15_ADC0_EVT1_SEL<<0) | (MW_OUTPUTXBAR15_ADC0_EVT2_SEL<<1) | (MW_OUTPUTXBAR15_ADC0_EVT3_SEL<<2) | (MW_OUTPUTXBAR15_ADC0_EVT4_SEL<<3) | (MW_OUTPUTXBAR15_ADC1_EVT1_SEL<<4) | (MW_OUTPUTXBAR15_ADC1_EVT2_SEL<<5) | (MW_OUTPUTXBAR15_ADC1_EVT3_SEL<<6) | (MW_OUTPUTXBAR15_ADC1_EVT4_SEL<<7) | (MW_OUTPUTXBAR15_ADC2_EVT1_SEL<<8) | (MW_OUTPUTXBAR15_ADC2_EVT2_SEL<<9) | (MW_OUTPUTXBAR15_ADC2_EVT3_SEL<<10) | (MW_OUTPUTXBAR15_ADC2_EVT4_SEL<<11);
    group08_outputxbar[15] = (MW_OUTPUTXBAR15_EPWM_SYNCOUT_XBAR0_SEL<<0) | ( MW_OUTPUTXBAR15_EPWM_SYNCOUT_XBAR1_SEL<<1) | ( MW_OUTPUTXBAR15_EPWM_SYNCOUT_XBAR2_SEL<<2) | ( MW_OUTPUTXBAR15_EPWM_SYNCOUT_XBAR3_SEL<<3) | (MW_OUTPUTXBAR15_EQEP0_I_OUT_SEL<<4) | (MW_OUTPUTXBAR15_EQEP0_S_OUT_SEL<<5) | (MW_OUTPUTXBAR15_EQEP1_I_OUT_SEL<<6) | (MW_OUTPUTXBAR15_EQEP1_S_OUT_SEL<<7) | (MW_OUTPUTXBAR15_ECAP0_SEL<<10) | (MW_OUTPUTXBAR15_ECAP1_SEL<<11) | (MW_OUTPUTXBAR15_ECAP2_SEL<<12) | (MW_OUTPUTXBAR15_ECAP3_SEL<<13) | (MW_OUTPUTXBAR15_ECAP4_SEL<<14) | (MW_OUTPUTXBAR15_ECAP5_SEL<<15) | (MW_OUTPUTXBAR15_ECAP6_SEL<<16) | (MW_OUTPUTXBAR15_ECAP7_SEL<<17);
    group09_outputxbar[15] =  (MW_OUTPUTXBAR15_FSIRX0_RX_TRIG0_SEL<<0) | (MW_OUTPUTXBAR15_FSIRX0_RX_TRIG1_SEL<<1) | (MW_OUTPUTXBAR15_FSIRX0_RX_TRIG2_SEL<<2) | (MW_OUTPUTXBAR15_FSIRX0_RX_TRIG3_SEL<<3) | (MW_OUTPUTXBAR15_INPUTXBAR_OUT7_SEL<<16) | (MW_OUTPUTXBAR15_INPUTXBAR_OUT15_SEL<<17) | (MW_OUTPUTXBAR15_INPUTXBAR_OUT21_SEL<<18) | (MW_OUTPUTXBAR15_INPUTXBAR_OUT31_SEL<<19) | (MW_OUTPUTXBAR15_INTXBAR_OUT7_SEL<<24) | (MW_OUTPUTXBAR15_INTXBAR_OUT15_SEL<<25);
    
           

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

        SOC_xbarSelectOutputXBarInputSource(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, i, group00_outputxbar[i], group01_outputxbar[i], group02_outputxbar[i], group03_outputxbar[i], group04_outputxbar[i], group05_outputxbar[i], group06_outputxbar[i], group07_outputxbar[i], group08_outputxbar[i], group09_outputxbar[i], 0);    
    }

   
    SOC_xbarInvertOutputXBarOutputSignalBeforeLatch(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
    SOC_xbarInvertOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
    SOC_xbarSelectLatchOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
    SOC_xbarSelectStretchedPulseOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);
    SOC_xbarSelectStretchedPulseLengthOutputXBarOutputSignal(CSL_CONTROLSS_OUTPUTXBAR_U_BASE, 0);

    Drivers_EPWMSyncOutputXBAR();


}
void Drivers_EPWMSyncOutputXBAR()
{
    uint32_t group0EPWMSyncOutputXbar[16];
    for(int i=0;i<=15;i++)
    {
        if((i==0 && (MW_OUTPUTXBAR0_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR0_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[0] = (MW_OUTPUTXBAR0_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR0_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR0_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR0_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR0_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR0_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR0_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR0_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR0_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR0_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR0_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[0]);
            else if(MW_OUTPUTXBAR0_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[0]); 
            else if(MW_OUTPUTXBAR0_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[0]);  
            else if(MW_OUTPUTXBAR0_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[0]);  
        }
        else if((i==1 && (MW_OUTPUTXBAR1_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR1_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[1] = (MW_OUTPUTXBAR1_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR1_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR1_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR1_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR1_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR1_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR1_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR1_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR1_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR1_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR1_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[1]);
            else if(MW_OUTPUTXBAR1_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[1]); 
            else if(MW_OUTPUTXBAR1_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[1]);  
            else if(MW_OUTPUTXBAR1_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[1]);  
        }
        else if((i==2 && (MW_OUTPUTXBAR2_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR2_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[2] = (MW_OUTPUTXBAR2_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR2_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR2_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR2_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR2_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR2_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR2_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR2_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR2_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR2_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR2_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[2]);
            else if(MW_OUTPUTXBAR2_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[2]); 
            else if(MW_OUTPUTXBAR2_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[2]);  
            else if(MW_OUTPUTXBAR2_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[2]);  
        }
        else if((i==3 && (MW_OUTPUTXBAR3_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR3_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[3] = (MW_OUTPUTXBAR3_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR3_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR3_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR3_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR3_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR3_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR3_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR3_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR3_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR3_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR3_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[3]);
            else if(MW_OUTPUTXBAR3_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[3]); 
            else if(MW_OUTPUTXBAR3_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[3]);  
            else if(MW_OUTPUTXBAR3_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[3]);  
        }
        else if((i==4 && (MW_OUTPUTXBAR4_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR4_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[4] = (MW_OUTPUTXBAR4_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR4_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR4_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR4_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR4_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR4_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR4_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR4_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR4_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR4_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR4_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[4]);
            else if(MW_OUTPUTXBAR4_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[4]); 
            else if(MW_OUTPUTXBAR4_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[4]);  
            else if(MW_OUTPUTXBAR4_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[4]);  
        }
        else if((i==5 && (MW_OUTPUTXBAR5_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR5_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[5] = (MW_OUTPUTXBAR5_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR5_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR5_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR5_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR5_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR5_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR5_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR5_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR5_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR5_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR5_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[5]);
            else if(MW_OUTPUTXBAR5_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[5]); 
            else if(MW_OUTPUTXBAR5_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[5]);  
            else if(MW_OUTPUTXBAR5_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[5]);  
        }
        else if((i==6 && (MW_OUTPUTXBAR6_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR6_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[6] = (MW_OUTPUTXBAR6_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR6_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR6_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR6_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR6_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR6_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR6_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR6_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR6_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR6_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR6_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[6]);
            else if(MW_OUTPUTXBAR6_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[6]); 
            else if(MW_OUTPUTXBAR6_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[6]);  
            else if(MW_OUTPUTXBAR6_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[6]);  
        }
        else if((i==7 && (MW_OUTPUTXBAR7_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR7_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[7] = (MW_OUTPUTXBAR7_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR7_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR7_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR7_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR7_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR7_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR7_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR7_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR7_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR7_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR7_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[7]);
            else if(MW_OUTPUTXBAR7_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[7]); 
            else if(MW_OUTPUTXBAR7_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[7]);  
            else if(MW_OUTPUTXBAR7_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[7]);  
        }
        else if((i==8 && (MW_OUTPUTXBAR8_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR8_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[8] = (MW_OUTPUTXBAR8_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR8_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR8_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR8_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR8_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR8_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR8_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR8_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR8_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR8_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR8_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[8]);
            else if(MW_OUTPUTXBAR8_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[8]); 
            else if(MW_OUTPUTXBAR8_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[8]);  
            else if(MW_OUTPUTXBAR8_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[8]);  
        }
        else if((i==9 && (MW_OUTPUTXBAR9_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR9_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[9] = (MW_OUTPUTXBAR9_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR9_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR9_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR9_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR9_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR9_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR9_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR9_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR9_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR9_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR9_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[9]);
            else if(MW_OUTPUTXBAR9_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[9]); 
            else if(MW_OUTPUTXBAR9_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[9]);  
            else if(MW_OUTPUTXBAR9_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[9]);  
        }
        else if((i==10 && (MW_OUTPUTXBAR10_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR10_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[10] = (MW_OUTPUTXBAR10_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR10_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR10_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR10_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR10_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR10_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR10_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR10_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR10_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR10_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR10_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[10]);
            else if(MW_OUTPUTXBAR10_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[10]); 
            else if(MW_OUTPUTXBAR10_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[10]);  
            else if(MW_OUTPUTXBAR10_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[10]);  
        }
        else if((i==11 && (MW_OUTPUTXBAR11_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR11_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[11] = (MW_OUTPUTXBAR11_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR11_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR11_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR11_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR11_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR11_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR11_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR11_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR11_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR11_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR11_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[11]);
            else if(MW_OUTPUTXBAR11_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[11]); 
            else if(MW_OUTPUTXBAR11_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[11]);  
            else if(MW_OUTPUTXBAR11_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[11]);  
        }
        else if((i==12 && (MW_OUTPUTXBAR12_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR12_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[12] = (MW_OUTPUTXBAR12_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR12_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR12_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR12_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR12_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR12_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR12_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR12_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR12_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR12_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR12_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[12]);
            else if(MW_OUTPUTXBAR12_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[12]); 
            else if(MW_OUTPUTXBAR12_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[12]);  
            else if(MW_OUTPUTXBAR12_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[12]);  
        }
        else if((i==13 && (MW_OUTPUTXBAR13_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR13_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[13] = (MW_OUTPUTXBAR13_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR13_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR13_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR13_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR13_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR13_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR13_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR13_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR13_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR13_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR13_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[13]);
            else if(MW_OUTPUTXBAR13_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[13]); 
            else if(MW_OUTPUTXBAR13_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[13]);  
            else if(MW_OUTPUTXBAR13_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[13]);  
        }
        else if((i==14 && (MW_OUTPUTXBAR14_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR14_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[14] = (MW_OUTPUTXBAR14_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR14_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR14_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR14_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR14_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR14_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR14_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR14_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR14_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR14_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR14_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[14]);
            else if(MW_OUTPUTXBAR14_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[14]); 
            else if(MW_OUTPUTXBAR14_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[14]);  
            else if(MW_OUTPUTXBAR14_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[14]);  
        }
        else if((i==15 && (MW_OUTPUTXBAR15_EPWM0_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM1_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM2_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM3_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM4_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM5_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM6_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM7_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM8_SYNCOUT_SEL | MW_OUTPUTXBAR15_EPWM9_SYNCOUT_SEL)))
        {
            group0EPWMSyncOutputXbar[15] = (MW_OUTPUTXBAR15_EPWM0_SYNCOUT_SEL << 0) | (MW_OUTPUTXBAR15_EPWM1_SYNCOUT_SEL << 1) | (MW_OUTPUTXBAR15_EPWM2_SYNCOUT_SEL << 2) | (MW_OUTPUTXBAR15_EPWM3_SYNCOUT_SEL << 3) | (MW_OUTPUTXBAR15_EPWM4_SYNCOUT_SEL << 4) | (MW_OUTPUTXBAR15_EPWM5_SYNCOUT_SEL << 5) | (MW_OUTPUTXBAR15_EPWM6_SYNCOUT_SEL << 6) | (MW_OUTPUTXBAR15_EPWM7_SYNCOUT_SEL << 7) | (MW_OUTPUTXBAR15_EPWM8_SYNCOUT_SEL << 8) | (MW_OUTPUTXBAR15_EPWM9_SYNCOUT_SEL << 9);
           
            if(MW_OUTPUTXBAR15_EPWM0SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 0, group0EPWMSyncOutputXbar[15]);
            else if(MW_OUTPUTXBAR15_EPWM1SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 1, group0EPWMSyncOutputXbar[15]); 
            else if(MW_OUTPUTXBAR15_EPWM2SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 2, group0EPWMSyncOutputXbar[15]);  
            else if(MW_OUTPUTXBAR15_EPWM3SYNCOUT_INSTANCE_SEL == 1)
                SOC_xbarSelectPWMSyncOutXBarInput(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, 3, group0EPWMSyncOutputXbar[15]);  
        }
       
    }
}

void Drivers_epwmSyncoutXbarOpen()
{
    uint32_t group0EpwmSyncoutXbar[4];
    uint32_t group1EpwmSyncoutXbar[4];

    group0EpwmSyncoutXbar[0] = (MW_EPWMSYNCOUTXBAR0_EPWM0_SYNCOUT_SEL << 0) | (MW_EPWMSYNCOUTXBAR0_EPWM1_SYNCOUT_SEL << 1) | (MW_EPWMSYNCOUTXBAR0_EPWM2_SYNCOUT_SEL << 2) | (MW_EPWMSYNCOUTXBAR0_EPWM3_SYNCOUT_SEL << 3) | (MW_EPWMSYNCOUTXBAR0_EPWM4_SYNCOUT_SEL << 4) | (MW_EPWMSYNCOUTXBAR0_EPWM5_SYNCOUT_SEL << 5) | (MW_EPWMSYNCOUTXBAR0_EPWM6_SYNCOUT_SEL << 6) | (MW_EPWMSYNCOUTXBAR0_EPWM7_SYNCOUT_SEL << 7) | (MW_EPWMSYNCOUTXBAR0_EPWM8_SYNCOUT_SEL << 8) | (MW_EPWMSYNCOUTXBAR0_EPWM9_SYNCOUT_SEL << 9);
    group1EpwmSyncoutXbar[0] = (MW_EPWMSYNCOUTXBAR0_ECAP0_SYNCOUT_SEL << 0) | (MW_EPWMSYNCOUTXBAR0_ECAP1_SYNCOUT_SEL << 1) | (MW_EPWMSYNCOUTXBAR0_ECAP2_SYNCOUT_SEL < 2) | (MW_EPWMSYNCOUTXBAR0_ECAP3_SYNCOUT_SEL << 3) | (MW_EPWMSYNCOUTXBAR0_ECAP4_SYNCOUT_SEL << 4) | (MW_EPWMSYNCOUTXBAR0_ECAP5_SYNCOUT_SEL << 5) | (MW_EPWMSYNCOUTXBAR0_ECAP6_SYNCOUT_SEL << 6) | (MW_EPWMSYNCOUTXBAR0_ECAP7_SYNCOUT_SEL << 7);
    group0EpwmSyncoutXbar[1] = (MW_EPWMSYNCOUTXBAR1_EPWM0_SYNCOUT_SEL << 0) | (MW_EPWMSYNCOUTXBAR1_EPWM1_SYNCOUT_SEL << 1) | (MW_EPWMSYNCOUTXBAR1_EPWM2_SYNCOUT_SEL << 2) | (MW_EPWMSYNCOUTXBAR1_EPWM3_SYNCOUT_SEL << 3) | (MW_EPWMSYNCOUTXBAR1_EPWM4_SYNCOUT_SEL << 4) | (MW_EPWMSYNCOUTXBAR1_EPWM5_SYNCOUT_SEL << 5) | (MW_EPWMSYNCOUTXBAR1_EPWM6_SYNCOUT_SEL << 6) | (MW_EPWMSYNCOUTXBAR1_EPWM7_SYNCOUT_SEL << 7) | (MW_EPWMSYNCOUTXBAR1_EPWM8_SYNCOUT_SEL << 8) | (MW_EPWMSYNCOUTXBAR1_EPWM9_SYNCOUT_SEL << 9);
    group1EpwmSyncoutXbar[1] = (MW_EPWMSYNCOUTXBAR1_ECAP0_SYNCOUT_SEL << 0) | (MW_EPWMSYNCOUTXBAR1_ECAP1_SYNCOUT_SEL << 1) | (MW_EPWMSYNCOUTXBAR1_ECAP2_SYNCOUT_SEL < 2) | (MW_EPWMSYNCOUTXBAR1_ECAP3_SYNCOUT_SEL << 3) | (MW_EPWMSYNCOUTXBAR1_ECAP4_SYNCOUT_SEL << 4) | (MW_EPWMSYNCOUTXBAR1_ECAP5_SYNCOUT_SEL << 5) | (MW_EPWMSYNCOUTXBAR1_ECAP6_SYNCOUT_SEL << 6) | (MW_EPWMSYNCOUTXBAR1_ECAP7_SYNCOUT_SEL << 7);
    group0EpwmSyncoutXbar[2] = (MW_EPWMSYNCOUTXBAR2_EPWM0_SYNCOUT_SEL << 0) | (MW_EPWMSYNCOUTXBAR2_EPWM1_SYNCOUT_SEL << 1) | (MW_EPWMSYNCOUTXBAR2_EPWM2_SYNCOUT_SEL << 2) | (MW_EPWMSYNCOUTXBAR2_EPWM3_SYNCOUT_SEL << 3) | (MW_EPWMSYNCOUTXBAR2_EPWM4_SYNCOUT_SEL << 4) | (MW_EPWMSYNCOUTXBAR2_EPWM5_SYNCOUT_SEL << 5) | (MW_EPWMSYNCOUTXBAR2_EPWM6_SYNCOUT_SEL << 6) | (MW_EPWMSYNCOUTXBAR2_EPWM7_SYNCOUT_SEL << 7) | (MW_EPWMSYNCOUTXBAR2_EPWM8_SYNCOUT_SEL << 8) | (MW_EPWMSYNCOUTXBAR2_EPWM9_SYNCOUT_SEL << 9);
    group1EpwmSyncoutXbar[2] = (MW_EPWMSYNCOUTXBAR2_ECAP0_SYNCOUT_SEL << 0) | (MW_EPWMSYNCOUTXBAR2_ECAP1_SYNCOUT_SEL << 1) | (MW_EPWMSYNCOUTXBAR2_ECAP2_SYNCOUT_SEL < 2) | (MW_EPWMSYNCOUTXBAR2_ECAP3_SYNCOUT_SEL << 3) | (MW_EPWMSYNCOUTXBAR2_ECAP4_SYNCOUT_SEL << 4) | (MW_EPWMSYNCOUTXBAR2_ECAP5_SYNCOUT_SEL << 5) | (MW_EPWMSYNCOUTXBAR2_ECAP6_SYNCOUT_SEL << 6) | (MW_EPWMSYNCOUTXBAR2_ECAP7_SYNCOUT_SEL << 7);
    group0EpwmSyncoutXbar[3] = (MW_EPWMSYNCOUTXBAR3_EPWM0_SYNCOUT_SEL << 0) | (MW_EPWMSYNCOUTXBAR3_EPWM1_SYNCOUT_SEL << 1) | (MW_EPWMSYNCOUTXBAR3_EPWM2_SYNCOUT_SEL << 2) | (MW_EPWMSYNCOUTXBAR3_EPWM3_SYNCOUT_SEL << 3) | (MW_EPWMSYNCOUTXBAR3_EPWM4_SYNCOUT_SEL << 4) | (MW_EPWMSYNCOUTXBAR3_EPWM5_SYNCOUT_SEL << 5) | (MW_EPWMSYNCOUTXBAR3_EPWM6_SYNCOUT_SEL << 6) | (MW_EPWMSYNCOUTXBAR3_EPWM7_SYNCOUT_SEL << 7) | (MW_EPWMSYNCOUTXBAR3_EPWM8_SYNCOUT_SEL << 8) | (MW_EPWMSYNCOUTXBAR3_EPWM9_SYNCOUT_SEL << 9);
    group1EpwmSyncoutXbar[3] = (MW_EPWMSYNCOUTXBAR3_ECAP0_SYNCOUT_SEL << 0) | (MW_EPWMSYNCOUTXBAR3_ECAP1_SYNCOUT_SEL << 1) | (MW_EPWMSYNCOUTXBAR3_ECAP2_SYNCOUT_SEL < 2) | (MW_EPWMSYNCOUTXBAR3_ECAP3_SYNCOUT_SEL << 3) | (MW_EPWMSYNCOUTXBAR3_ECAP4_SYNCOUT_SEL << 4) | (MW_EPWMSYNCOUTXBAR3_ECAP5_SYNCOUT_SEL << 5) | (MW_EPWMSYNCOUTXBAR3_ECAP6_SYNCOUT_SEL << 6) | (MW_EPWMSYNCOUTXBAR3_ECAP7_SYNCOUT_SEL << 7);
    
    for(uint8_t i = 0; i < 4; i++)
    {
        SOC_xbarSelectPWMSyncOutXBarInput_ext(CSL_CONTROLSS_PWMSYNCOUTXBAR_U_BASE, i, group0EpwmSyncoutXbar[i], group1EpwmSyncoutXbar[i]);
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
#define MCAN_COUNT                               (2)
#define APP_MCAN_EXT_ID_MASK                     (0x1FFFFFFFU)
#define APP_MCAN_STD_ID_MASK                     (0x7FFU)
#define APP_MCAN_STD_ID_SHIFT                    (18U)


int32_t Board_eepromOpen(void);
void Board_eepromClose(void);

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


void mcanEnableTransceiverLP(void)
{
    uint32_t    gpioBaseAddr, pinNum;

    gpioBaseAddr = (uint32_t)AddrTranslateP_getLocalAddr(CSL_GPIO0_U_BASE);
    pinNum       = 132;

    GPIO_setDirMode(gpioBaseAddr, pinNum, GPIO_DIRECTION_OUTPUT);

    GPIO_pinWriteLow(gpioBaseAddr, pinNum);
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
    

}

void MCAN_initialization(void)
{
    if(MW_ENABLE_MCAN0_SEL | MW_ENABLE_MCAN1_SEL | MW_ENABLE_MCAN2_SEL | MW_ENABLE_MCAN3_SEL)
    {
        
        uint32_t                   i, j;
        int32_t                 status = SystemP_SUCCESS;
        

        for(i = 0; i < MCAN_COUNT; i++)
        {
           
            if(((i == 0) && (MW_ENABLE_MCAN0_SEL == 1)))
            {
                if((MW_ENABLE_INTLOOPBACK_MCAN0_SEL == 0) && (MW_ENABLE_GPIO_TCV_MCAN0_SEL == 1))
                    mcanEnableTransceiverLP();
                else if((MW_ENABLE_INTLOOPBACK_MCAN0_SEL == 0) && (MW_ENABLE_I2C_TCV_MCAN0_SEL == 1))
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
                if((MW_ENABLE_INTLOOPBACK_MCAN1_SEL == 0) && (MW_ENABLE_GPIO_TCV_MCAN1_SEL == 1))
                    mcanEnableTransceiverLP();
                else if((MW_ENABLE_INTLOOPBACK_MCAN1_SEL == 0) && (MW_ENABLE_I2C_TCV_MCAN1_SEL == 1))
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
        

    }
}
}

#define MCSPI_NUM_INSTANCES (4U)
#define MCSPI_CHANNEL_NUM (2U)
MCSPI_ChConfig gConfigMcspi0ChCfg[2U];
MCSPI_ChConfig gConfigMcspi1ChCfg[2U];
MCSPI_ChConfig gConfigMcspi2ChCfg[2U];
MCSPI_ChConfig gConfigMcspi3ChCfg[2U];
MCSPI_ChConfig gConfigMcspi4ChCfg[2U];
MCSPILLD_InitObject gMcspiInitObject[MCSPI_NUM_INSTANCES];
MCSPI_ChConfig *gConfigMcspiChCfg;

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

        DebugP_assert(status == MCSPI_STATUS_SUCCESS);   
    }

    return;
}  

#define CONFIG_IOEXP0 (0U)
#define CONFIG_IOEXP1 (1U)

// if(MW_ENABLE_IO_EXPANDER1_SEL == 1)
//     #define CONFIG_IOEXP_NUM_INSTANCES (2U)
// else
#define CONFIG_IOEXP_NUM_INSTANCES (1U)
#define CONFIG_I2C0 (0U)
#define CONFIG_I2C_HLD_NUM_INSTANCES (1U)

#define IO_EXP_USB2_0_MUX_SEL0_LINE      (0)
/* Port Number : 0, Pin Number : 1. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_VPP_LDO_EN_LINE      (1)
/* Port Number : 0, Pin Number : 2. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_LED_DRIVER_EN_LINE      (2)
/* Port Number : 0, Pin Number : 3. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_MCAN_MUX_SEL_LINE      (3)
/* Port Number : 0, Pin Number : 4. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_BP_BO_MUX_EN_LINE      (4)
/* Port Number : 0, Pin Number : 5. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_BP_MUX_SW_S1_LINE      (5)
/* Port Number : 0, Pin Number : 6. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_BP_BO_MUX_EN_N_LINE    (6)
/* Port Number : 0, Pin Number : 7. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_BP_MUX_SW_SO_LINE      (7)


#define IO_EXP_CPSW_ICSS_BRD_CONN_DET1_LINE      (0)
/* Port Number : 0, Pin Number : 1. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_ETH0_CPSW2_RST_LINE      (1)
/* Port Number : 0, Pin Number : 2. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_ETH1_CPSW1_RST_LINE      (2)
/* Port Number : 0, Pin Number : 3. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_MDIO_MDC_MUX_SEL1_LINE      (3)
/* Port Number : 0, Pin Number : 4. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_MDIO_MDC_MUX_SEL2_LINE      (4)
/* Port Number : 0, Pin Number : 5. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_CPSW_ICSS_BRD_CONN_DET2_LINE      (5)
/* Port Number : 0, Pin Number : 6. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_FSI_EQEP_MUX_SEL_LINE      (6)
/* Port Number : 0, Pin Number : 7. Line Number : (Port Number * 8) + Pin Number */
#define IO_EXP_OSPI1_MUX_SEL_LINE      (7)



void Board_ioexpInit();
void Board_init(void);
void Board_deinit(void);


int32_t Board_driversOpen(void)
{
    
    int32_t status = SystemP_SUCCESS;
    if(MW_ENABLE_IO_EXPANDER1_SEL | MW_ENABLE_IO_EXPANDER0_SEL)
    {
        if(status==SystemP_SUCCESS)
        {
            status = Board_ioexpOpen();
            if(status == SystemP_FAILURE)
            {
                DebugP_log("IO Expander Configuration Failed!!\r\n");
            }
        }
    }


    return status;
}

void Board_driversClose(void)
{

}

/*
 * IOEXP
 */
int32_t Board_ioexpOpen()
{
    int32_t  status = SystemP_SUCCESS;
    static TCA6408_Config  gTCA6408_Config;
    TCA6408_Params      TCA6408Params;
    TCA6408_Params_init(&TCA6408Params);
    if(MW_ENABLE_IO_EXPANDER0_SEL)
    {
        if(MW_IO_EXPANDER0_I2C_ADDR_SEL == 0)
        {
            TCA6408Params.i2cAddress  = 0x20U;
            status = TCA6408_open(&gTCA6408_Config, &TCA6408Params);

            if(MW_IO_EXPANDER0_USB2P0_MUX_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_USB2_0_MUX_SEL0_LINE,
                    MW_IO_EXPANDER0_USB2P0_MUX_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_USB2_0_MUX_SEL0_LINE,
                    TCA6408_MODE_OUTPUT);

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }

            }

            if(MW_IO_EXPANDER0_VPPLDO_EN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_VPP_LDO_EN_LINE,
                    MW_IO_EXPANDER0_VPPLDO_EN_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_VPP_LDO_EN_LINE,
                    TCA6408_MODE_OUTPUT);

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER0_LEDDRIVER_EN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_LED_DRIVER_EN_LINE,
                    MW_IO_EXPANDER0_LEDDRIVER_EN_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_LED_DRIVER_EN_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER0_MCANMUX_SEL_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_MCAN_MUX_SEL_LINE,
                    MW_IO_EXPANDER0_MCANMUX_SEL_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_MCAN_MUX_SEL_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }
            if(MW_IO_EXPANDER0_BPBOMUX_EN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_BP_BO_MUX_EN_LINE,
                    MW_IO_EXPANDER0_BPBOMUX_EN_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_BP_BO_MUX_EN_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }
            if(MW_IO_EXPANDER0_BPMUX_SWS1_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_BP_MUX_SW_S1_LINE,
                    MW_IO_EXPANDER0_BPMUX_SWS1_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_BP_MUX_SW_S1_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER0_BPBOMUX_ENN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_BP_BO_MUX_EN_N_LINE,
                    MW_IO_EXPANDER0_BPBOMUX_ENN_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_BP_BO_MUX_EN_N_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER0_BPBOMUX_ENN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_BP_MUX_SW_SO_LINE,
                    MW_IO_EXPANDER0_BPMUX_SWSO_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_BP_MUX_SW_SO_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

        }
        else if(MW_IO_EXPANDER0_I2C_ADDR_SEL == 1)
        {
            TCA6408Params.i2cAddress  = 0x21U;
            status = TCA6408_open(&gTCA6408_Config, &TCA6408Params);

            if(MW_IO_EXPANDER0_CPSW_ICSS_BRDCONN_DET1_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_CPSW_ICSS_BRD_CONN_DET1_LINE  ,
                    MW_IO_EXPANDER0_CPSW_ICSS_BRDCONN_DET1_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_CPSW_ICSS_BRD_CONN_DET1_LINE  ,
                    TCA6408_MODE_OUTPUT);

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }

            }

            if(MW_IO_EXPANDER0_ETH0_CPSW2_RST_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_ETH0_CPSW2_RST_LINE,
                    MW_IO_EXPANDER0_ETH0_CPSW2_RST_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_ETH0_CPSW2_RST_LINE ,
                    TCA6408_MODE_OUTPUT);

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER0_ETH1_CPSW1_RST_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_ETH1_CPSW1_RST_LINE,
                    MW_IO_EXPANDER0_ETH1_CPSW1_RST_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_ETH1_CPSW1_RST_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER0_MDIO_MDC_MUXSEL1_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_MDIO_MDC_MUX_SEL1_LINE,
                    MW_IO_EXPANDER0_MDIO_MDC_MUXSEL1_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_MDIO_MDC_MUX_SEL1_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }
            if(MW_IO_EXPANDER0_MDIO_MDC_MUXSEL2_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_MDIO_MDC_MUX_SEL2_LINE,
                    MW_IO_EXPANDER0_MDIO_MDC_MUXSEL2_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_MDIO_MDC_MUX_SEL2_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }
            if(MW_IO_EXPANDER0_CPSW_ICSS_BRDCONN_DET2_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_CPSW_ICSS_BRD_CONN_DET2_LINE,
                    MW_IO_EXPANDER0_CPSW_ICSS_BRDCONN_DET2_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_CPSW_ICSS_BRD_CONN_DET2_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER0_FSI_EQEP_MUXSEL_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_FSI_EQEP_MUX_SEL_LINE,
                    MW_IO_EXPANDER0_FSI_EQEP_MUXSEL_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_FSI_EQEP_MUX_SEL_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER0_OSPI1_MUXSEL_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_OSPI1_MUX_SEL_LINE,
                    MW_IO_EXPANDER0_OSPI1_MUXSEL_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_OSPI1_MUX_SEL_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

        }

    }

    if(MW_ENABLE_IO_EXPANDER1_SEL == 1)
    {
        if(MW_IO_EXPANDER1_I2C_ADDR_SEL == 0)
        {
            TCA6408Params.i2cAddress  = 0x20U;
            status = TCA6408_open(&gTCA6408_Config, &TCA6408Params);

            if(MW_IO_EXPANDER1_USB2P0_MUX_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_USB2_0_MUX_SEL0_LINE,
                    MW_IO_EXPANDER1_USB2P0_MUX_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_USB2_0_MUX_SEL0_LINE,
                    TCA6408_MODE_OUTPUT);

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }

            }

            if(MW_IO_EXPANDER1_VPPLDO_EN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_VPP_LDO_EN_LINE,
                    MW_IO_EXPANDER1_VPPLDO_EN_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_VPP_LDO_EN_LINE,
                    TCA6408_MODE_OUTPUT);

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER1_LEDDRIVER_EN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_LED_DRIVER_EN_LINE,
                    MW_IO_EXPANDER1_LEDDRIVER_EN_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_LED_DRIVER_EN_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER1_MCANMUX_SEL_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_MCAN_MUX_SEL_LINE,
                    MW_IO_EXPANDER1_MCANMUX_SEL_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_MCAN_MUX_SEL_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }
            if(MW_IO_EXPANDER1_BPBOMUX_EN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_BP_BO_MUX_EN_LINE,
                    MW_IO_EXPANDER1_BPBOMUX_EN_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_BP_BO_MUX_EN_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }
            if(MW_IO_EXPANDER1_BPMUX_SWS1_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_BP_MUX_SW_S1_LINE,
                    MW_IO_EXPANDER1_BPMUX_SWS1_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_BP_MUX_SW_S1_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER1_BPBOMUX_ENN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_BP_BO_MUX_EN_N_LINE,
                    MW_IO_EXPANDER1_BPBOMUX_ENN_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_BP_BO_MUX_EN_N_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER1_BPBOMUX_ENN_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_BP_MUX_SW_SO_LINE,
                    MW_IO_EXPANDER1_BPMUX_SWSO_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_BP_MUX_SW_SO_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

        }
        else if(MW_IO_EXPANDER1_I2C_ADDR_SEL == 1)
        {
            TCA6408Params.i2cAddress  = 0x21U;
            status = TCA6408_open(&gTCA6408_Config, &TCA6408Params);

            if(MW_IO_EXPANDER1_CPSW_ICSS_BRDCONN_DET1_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_CPSW_ICSS_BRD_CONN_DET1_LINE  ,
                    MW_IO_EXPANDER1_CPSW_ICSS_BRDCONN_DET1_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_CPSW_ICSS_BRD_CONN_DET1_LINE  ,
                    TCA6408_MODE_OUTPUT);

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }

            }

            if(MW_IO_EXPANDER1_ETH0_CPSW2_RST_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_ETH0_CPSW2_RST_LINE,
                    MW_IO_EXPANDER1_ETH0_CPSW2_RST_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_ETH0_CPSW2_RST_LINE ,
                    TCA6408_MODE_OUTPUT);

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER1_ETH1_CPSW1_RST_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_ETH1_CPSW1_RST_LINE,
                    MW_IO_EXPANDER1_ETH1_CPSW1_RST_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_ETH1_CPSW1_RST_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER1_MDIO_MDC_MUXSEL1_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_MDIO_MDC_MUX_SEL1_LINE,
                    MW_IO_EXPANDER1_MDIO_MDC_MUXSEL1_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_MDIO_MDC_MUX_SEL1_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }
            if(MW_IO_EXPANDER1_MDIO_MDC_MUXSEL2_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_MDIO_MDC_MUX_SEL2_LINE,
                    MW_IO_EXPANDER1_MDIO_MDC_MUXSEL2_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_MDIO_MDC_MUX_SEL2_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }
            if(MW_IO_EXPANDER1_CPSW_ICSS_BRDCONN_DET2_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_CPSW_ICSS_BRD_CONN_DET2_LINE,
                    MW_IO_EXPANDER1_CPSW_ICSS_BRDCONN_DET2_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_CPSW_ICSS_BRD_CONN_DET2_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER1_FSI_EQEP_MUXSEL_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_FSI_EQEP_MUX_SEL_LINE,
                    MW_IO_EXPANDER1_FSI_EQEP_MUXSEL_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_FSI_EQEP_MUX_SEL_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

            if(MW_IO_EXPANDER1_OSPI1_MUXSEL_MODE_SEL == 1)
            {
                status = TCA6408_setOutput(
                    &gTCA6408_Config,
                    IO_EXP_OSPI1_MUX_SEL_LINE,
                    MW_IO_EXPANDER1_OSPI1_MUXSEL_STATE_SEL);

                status += TCA6408_config(
                    &gTCA6408_Config,
                    IO_EXP_OSPI1_MUX_SEL_LINE,
                    TCA6408_MODE_OUTPUT); 

                if(status != SystemP_SUCCESS)
                {
                    DebugP_log("Failure to Set IO Expander lines\r\n");
                    TCA6408_close(&gTCA6408_Config);
                }
            }

        }

    }

    return status;

}

static I2C_HwAttrs gI2cHwAttrs[CONFIG_I2C_HLD_NUM_INSTANCES] =
{
    {
        .baseAddr       = CSL_I2C0_U_BASE,
        .intNum         = 79,
        .eventId        = 0,
        .funcClk        = 48000000U,
        .enableIntr     = MW_ENABLE_INT_SEL,
        .ownTargetAddr   = MW_TARGET_ADDRESS_SEL,
    },
};

static I2C_Object gI2cObjects[CONFIG_I2C_HLD_NUM_INSTANCES];

I2C_Config gI2cConfig[CONFIG_I2C_HLD_NUM_INSTANCES] =
{
    {
        .object = &gI2cObjects[CONFIG_I2C0],
        .hwAttrs = &gI2cHwAttrs[CONFIG_I2C0]
    },
};

uint32_t gI2cConfigNum = CONFIG_I2C_HLD_NUM_INSTANCES;

I2C_Handle gI2cHandle[CONFIG_I2C_HLD_NUM_INSTANCES];
I2C_Params gI2cParams[CONFIG_I2C_HLD_NUM_INSTANCES] =
{
    {
        .transferMode        = I2C_MODE_BLOCKING,
        .transferCallbackFxn = NULL,
        .bitRate             = I2C_400KHZ,
    },
};

void Drivers_i2cOpen(void);
void Drivers_i2cClose(void);

void Drivers_i2cOpen(void)
{
    int32_t  status = SystemP_SUCCESS;
    uint32_t instCnt;


    if(MW_ENABLE_I2C_SEL == 1)
    {
        for(instCnt = 0U; instCnt < CONFIG_I2C_HLD_NUM_INSTANCES; instCnt++)
        {
            gI2cHandle[instCnt] = NULL;   
        }


        for(instCnt = 0U; instCnt < CONFIG_I2C_HLD_NUM_INSTANCES; instCnt++)
        {
            gI2cHandle[instCnt] = I2C_open(instCnt, &gI2cParams[instCnt]);
            if(NULL == gI2cHandle[instCnt])
            {
                DebugP_logError("I2C open failed for HLD instance %d !!!\r\n", instCnt);
                status = SystemP_FAILURE;
                break;
            }
        }

        if(SystemP_SUCCESS != status)
        {
            Drivers_i2cClose();  
        }
    }
    return;
}

void Drivers_i2cClose(void)
{
    uint32_t instCnt;

    for(instCnt = 0U; instCnt < CONFIG_I2C_HLD_NUM_INSTANCES; instCnt++)
    {
        if(gI2cHandle[instCnt] != NULL)
        {
            I2C_close(gI2cHandle[instCnt]);
            gI2cHandle[instCnt] = NULL;
        }
    }
    return;
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
    /* Now we can do pinmux */
    Pinmux_init();
    I2C_init();

    {
        /* ADC */

        /* ADC Reference and Monitor Configurations */
        {
            if(MW_ENABLE_ADC0_SEL == 1)
            {
                SOC_enableAdcInternalReference(0, TRUE);
                ClockP_usleep(800);
                /* Monitor control is enabled for buffer 1 */
                SOC_enableAdcReferenceMonitor(0, TRUE);
                /* Wait for  ClockP_sleep */
                ClockP_usleep(1);
                /* Assert the monitor status! */
                DebugP_assert(SOC_getAdcReferenceStatus(0) == true);
            }
            /* Internal Reference Buffer 0, corresponds to ADC instance 1 is enabled */
            if(MW_ENABLE_ADC1_SEL == 1)
            {
                SOC_enableAdcInternalReference(1, TRUE);
                ClockP_usleep(800);
                /* Monitor control is enabled for buffer 0 */
                SOC_enableAdcReferenceMonitor(1, TRUE);
                /* Wait for  ClockP_sleep */
                ClockP_usleep(1);
                /* Assert the monitor status! */
                DebugP_assert(SOC_getAdcReferenceStatus(1) == true);
            }
            /* Internal Reference Buffer 1, corresponds to ADC instance 2 is enabled */
            if(MW_ENABLE_ADC2_SEL == 1)
            {
                SOC_enableAdcInternalReference(2, TRUE);
                ClockP_usleep(800);
                /* Monitor control is enabled for buffer 1 */
                SOC_enableAdcReferenceMonitor(2, TRUE);
                /* Wait for  ClockP_sleep */
                ClockP_usleep(1);
                /* Assert the monitor status! */
                DebugP_assert(SOC_getAdcReferenceStatus(2) == true);
            }
        }
        /* ADC - DAC Loop Back Configurations */
        {
            SOC_enableAdcDacLoopback(FALSE);
        }
        /* Global Force Configurations */
        {
            if(MW_ENABLE_ADC0_SEL == 1)
                SOC_enableAdcGlobalForce(0, FALSE);
            if(MW_ENABLE_ADC1_SEL == 1)
                SOC_enableAdcGlobalForce(1, FALSE);
            if(MW_ENABLE_ADC2_SEL == 1)    
                SOC_enableAdcGlobalForce(2, FALSE);
        }
    }


   

}

void Drivers_open(void)
{
    Drivers_inputXbarOpen();
    Drivers_intXbarOpen();
    Drivers_outputXbarOpen();
    Drivers_epwmSyncoutXbarOpen();
    Drivers_gpioIntXbarOpen();
    Drivers_EPWMXbarOpen();
    Drivers_uartOpen();
    Drivers_i2cOpen();
    if(MW_ENABLE_MCAN_SEL == 1)
    {
        Driver_mcanOpen();
        MCAN_initialization();
    }
    if((MW_ENABLE_MCSPI0_SEL == 1) | (MW_ENABLE_MCSPI1_SEL == 1) | (MW_ENABLE_MCSPI2_SEL == 1) | (MW_ENABLE_MCSPI3_SEL == 1))
    {
        MCSPI_channel_init();
        MCSPI_object_init();
        Drivers_mcspiOpen();
    }

    
}


void am261x_HardwareInit_Fcn(void)
{

    System_init();
    Drivers_open();
    Board_driversOpen();

	return;
}

