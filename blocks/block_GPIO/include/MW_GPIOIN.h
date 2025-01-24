#ifndef GPIO_MW_H_
#define GPIO_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/gpio.h>
#include <kernel/dpl/DebugP.h>
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

} GPIOInputSetupStruct;

typedef struct{
    uint32_t GPIOBaseAddress;
    uint32_t pinInstance;
    
} GPIOInputStepStruct;

typedef struct{

    uint32_t GPIOBaseAddress;
    uint32_t pinInstance;
    bool EnableInterrupt;


} GPIOInputReleaseStruct;

void GPIOInput_setup(GPIOInputSetupStruct *ptr);
uint32_t GPIOInput_step(GPIOInputStepStruct *ptr);
void GPIOInput_release(GPIOInputReleaseStruct *ptr);
#endif 
