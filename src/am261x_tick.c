
/* This is an example for setting the interrupt handler for bareboard scheduling */

#include <stdint.h>
/* Add your includes */
#include "MW_target_hardware_resources.h"

extern void rt_OneStep();

unsigned long schdl_counter;
unsigned long schdl_counter_max;

volatile uint32_t taskToSchedule;

void __disable_irq()
{
    /* HwiP_disableIRQ*/
    return;
}

void __enable_irq()
{
    /* HwiP_enableIRQ*/
    return;
}

    
/* Modify the following with your interrupt handler */
/* Simulink generated rt_OneStep is reentrant, so the interrupt handler 
   can save conext and invoke rt_OneStep */

static void Tick_Isr(void)
{
    /* The rt_OneStep() function expects interrupts to be disabled. 
       Hence disable interrupt before rt_OneStep(). */
    __disable_irq();
    rt_OneStep();
    /* The rt_OneStep() function disables interrupt. 
       Hence enable interrupt immediately after rt_OneStep(). */
    __enable_irq();

}




/* ----------- TimerP ----------- */
#include <drivers/rti/v0/rti.h>
#include <drivers/soc.h>
#include <kernel/dpl/AddrTranslateP.h>

// #define CONFIG_RTI0                        (0u)
// #define CONFIG_RTI0_BASE_ADDR              (0x52181000u)
// #define CONFIG_RTI0_INPUT_CLK_HZ           (25000000u)
// #define CONFIG_RTI0_CNTR0_OUTPUT_CLK_HZ    (1000000u)
// #define CONFIG_RTI0_COMP0_SRC              (RTI_TMR_CNT_BLK_INDEX_0)
// #define CONFIG_RTI0_COMP0_INPUT_CLK_HZ     (1000000u)
// #define CONFIG_RTI0_INT_NUM_EVENT0         (91u)
// #define CONFIG_RTI0_INT_NUM_EVENT1         (92u)
// #define CONFIG_RTI0_INT_NUM_EVENT2         (93u)
// #define CONFIG_RTI0_INT_NUM_EVENT3         (94u)
// #define CONFIG_RTI0_LOCK_UNLOCK_DOMAIN     (SOC_DOMAIN_ID_MAIN)
// #define CONFIG_RTI0_LOCK_UNLOCK_PARTITION  (MSS_RCM_PARTITION0)
// #define CONFIG_RTI0_CLOCK_SRC_MUX_ADDR     (0x53208118u)
// #define CONFIG_RTI0_CLOCK_SRC_VALUE        (0x0u)
// #define CONFIG_RTI0_CLOCK_SRC_WUCPUCLK (0x0u)
// #define RTI_NUM_INSTANCES                     (1u)


#define CONFIG_RTI0                        (0u)
#define CONFIG_RTI0_BASE_ADDR              (0x52181000u)
#define CONFIG_RTI0_CNTR0_OUTPUT_CLK_HZ    (1000000u)
#define CONFIG_RTI0_INPUT_CLK_HZ           (25000000u)
#define CONFIG_RTI0_COMP0_SRC              (RTI_TMR_CNT_BLK_INDEX_0)
#define CONFIG_RTI0_INT_NUM_EVENT0         (130u)
#define CONFIG_RTI0_INT_NUM_EVENT1         (131u)
#define CONFIG_RTI0_INT_NUM_EVENT2         (132u)
#define CONFIG_RTI0_INT_NUM_EVENT3         (133u)
#define CONFIG_RTI0_LOCK_UNLOCK_DOMAIN     (SOC_DOMAIN_ID_MAIN)
#define CONFIG_RTI0_LOCK_UNLOCK_PARTITION  (MSS_RCM_PARTITION0)
#define CONFIG_RTI0_CLOCK_SRC_MUX_ADDR     (0x53208118u)
#define CONFIG_RTI0_CLOCK_SRC_VALUE        (0x0u)
#define CONFIG_RTI0_CLOCK_SRC_WUCPUCLK (0x0u)
#define RTI_NUM_INSTANCES                     (1u)



HwiP_Object gRtiEvent0HwiObj[RTI_NUM_INSTANCES];
HwiP_Object gRtiEvent1HwiObj[RTI_NUM_INSTANCES];
HwiP_Object gRtiEvent2HwiObj[RTI_NUM_INSTANCES];
HwiP_Object gRtiEvent3HwiObj[RTI_NUM_INSTANCES];

void RTI0_event0Isr(void *args)
{
    Tick_Isr();
    
    RTI_intStatusClear(CONFIG_RTI0_BASE_ADDR, RTI_TMR_INT_INT0_FLAG);
    HwiP_clearInt(CONFIG_RTI0_INT_NUM_EVENT0);
}

void Drivers_rtiOpen(uint64_t timeInNsec)
{
    HwiP_Params rtiHwiParams;
    int32_t status;
    // uint32_t baseAddr;
    uint32_t cntrPrescaler;
    uint32_t compPrescaler;

    // baseAddr = CONFIG_RTI0_BASE_ADDR;

    // /* Configure RTI input clock source */
    // SOC_controlModuleUnlockMMR(SOC_DOMAIN_ID_MAIN, MSS_RCM_PARTITION0);
    // *(volatile uint32_t*)AddrTranslateP_getLocalAddr(CONFIG_RTI0_CLOCK_SRC_MUX_ADDR) = CONFIG_RTI0_CLOCK_SRC_WUCPUCLK;
    // SOC_controlModuleLockMMR(SOC_DOMAIN_ID_MAIN, MSS_RCM_PARTITION0);

    // /* Enable/Disable Continue on Suspend */
    // RTIG_setStallMode(baseAddr, RTI_GC_STALL_MODE_OFF);


    uint32_t baseAddr;

    baseAddr = CONFIG_RTI0_BASE_ADDR;

    /* Configure RTI input clock source */
    SOC_controlModuleUnlockMMR(SOC_DOMAIN_ID_MAIN, MSS_RCM_PARTITION0);
    *(volatile uint32_t*)AddrTranslateP_getLocalAddr(CONFIG_RTI0_CLOCK_SRC_MUX_ADDR) = CONFIG_RTI0_CLOCK_SRC_WUCPUCLK;
    SOC_controlModuleLockMMR(SOC_DOMAIN_ID_MAIN, MSS_RCM_PARTITION0);

    /* Enable/Disable Continue on Suspend */
    RTIG_setStallMode(baseAddr, RTI_GC_STALL_MODE_OFF);

    /* Configure Counter block 0 */
    cntrPrescaler = (CONFIG_RTI0_INPUT_CLK_HZ/CONFIG_RTI0_CNTR0_OUTPUT_CLK_HZ) - 1;
    RTI_counterConfigure(baseAddr, RTI_TMR_CNT_BLK_INDEX_0, RTI_TMR_CLK_SRC_COUNTER, RTI_TMR_NTU_0, cntrPrescaler);
    RTI_captureConfig(baseAddr, RTI_TMR_CNT_BLK_INDEX_0, RTI_TMR_CAPTURE_EVT_0);


    /* Configure Compare event 0 */
    compPrescaler = (timeInNsec*CONFIG_RTI0_INPUT_CLK_HZ )/1000000000;

    RTI_compareEventConfig(baseAddr, RTI_TMR_CMP_BLK_INDEX_0, CONFIG_RTI0_COMP0_SRC, compPrescaler, compPrescaler);
    RTI_intStatusClear(baseAddr, RTI_TMR_INT_INT0_FLAG);
    HwiP_Params_init(&rtiHwiParams);
    rtiHwiParams.intNum = CONFIG_RTI0_INT_NUM_EVENT0;
    rtiHwiParams.callback = RTI0_event0Isr;
    rtiHwiParams.isPulse = 0;
    rtiHwiParams.priority = 4;
    status = HwiP_construct(&gRtiEvent0HwiObj[CONFIG_RTI0], &rtiHwiParams);
    DebugP_assertNoLog(status==SystemP_SUCCESS);

    RTI_intEnable(baseAddr, RTI_TMR_INT_INT0_FLAG);

    RTI_intDisable(baseAddr, RTI_TMR_INT_DMA0_FLAG);


}






void TickConfig(float modelBaseRate)
{
    //modelBaseRate is in units of seconds
    //Convert to nanoseconds

	uint64_t tick_period;
	tick_period = (uint64_t)(1000000000.0 * modelBaseRate);
	
	taskToSchedule = (uint32_t) Tick_Isr;
    
    Drivers_rtiOpen(tick_period);
    RTI_counterEnable(CONFIG_RTI0_BASE_ADDR, RTI_TMR_CNT_BLK_INDEX_0);

}


