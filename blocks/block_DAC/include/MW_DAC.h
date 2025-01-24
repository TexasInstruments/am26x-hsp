#ifndef UART_MW_H_
#define UART_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include <drivers/dac.h>
#include <drivers/soc.h>
#include <math.h>
#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif
#include "MW_target_hardware_resources.h"


typedef struct{
    
    uint32_t base;
    DAC_ReferenceVoltage DACRef;
    DAC_LoadMode LoadMode;
    uint16_t PWMSyncSignal;
    bool Enable_output;
    uint16_t ShdwValue;
    bool DACLockControl;
    bool DACLockOutput;
    bool DACLockShadow;
} DACSetupStruct;

void DAC_setup(DACSetupStruct *ptr);
void DAC_step(uint32_t base, uint16_t DACVal);
void DAC_release(uint32_t base);



#endif