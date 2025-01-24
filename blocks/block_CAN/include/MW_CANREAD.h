#ifndef CANREAD_MW_H_
#define CANREAD_MW_H_

#include <stdint.h>
#include <drivers/soc.h>
#include <drivers/mcan.h>
#include "MW_target_hardware_resources.h"
#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif

typedef struct{

    uint32_t base;
    uint32_t instance;
    bool Enable_blockingrx;
    uint32_t rxSource;
    uint32_t buffernum;

}CANREAD_step_struct;

extern void
CANRead_Step(CANREAD_step_struct *ptr, const void *data_len, const void *rx_data);

extern void
CANREAD_release(uint32_t base);

#endif