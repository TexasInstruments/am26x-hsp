#ifndef GPIOOUT_MW_H_
#define GPIOOUT_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/gpio.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>

#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif


typedef struct{
    uint32_t GPIOBaseAddress;
    uint32_t pinInstance;
    uint32_t trigType;
    bool EnableInterrupt;
    

} GPIOOutputSetupStruct;

typedef struct{
    uint32_t GPIOBaseAddress;
    uint32_t pinInstance;
    uint32_t input_signal;
    
} GPIOOutputStepStruct;

typedef struct{

    uint32_t GPIOBaseAddress;
    uint32_t pinInstance;

} GPIOOutputReleaseStruct;

void GPIOOutput_setup(GPIOOutputSetupStruct *ptr);
void GPIOOutput_step(GPIOOutputStepStruct *ptr);
void GPIOOutput_release(GPIOOutputReleaseStruct *ptr);
#endif 