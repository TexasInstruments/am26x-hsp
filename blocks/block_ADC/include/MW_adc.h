#ifndef ADC_MW_H_
#define ADC_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/adc.h>
#include <kernel/dpl/DebugP.h>
#include <drivers/soc.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/TaskP.h>
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
                     uint32_t module; 
                     ADC_ClkPrescale clkPrescale; 
                     ADC_Resolution resolution; 
                     ADC_SignalMode signalMode; 
                     ADC_PriorityMode priMode; 
                     ADC_SOCNumber socNumber; 
                     ADC_Trigger soc_trigger_source; 
                     ADC_Channel channels; 
                     ADC_Channel channels1; 
                     uint32_t sampleWindow; 
                     ADC_IntSOCTrigger interrupt_trigger; 
                     ADC_IntNumber interrupt_selectionnumber; 
                     ADC_SOCNumber interrupt_soc; 
                     bool enable_adc_continuousmode1; 
                     bool enable_adc_continuousmode2; 
                     bool enable_adc_continuousmode3;
                     bool enable_adc_continuousmode4; 
                     bool enable_adc;
                     ADC_PulseMode pulse_mode; 
                     ADC_PPBNumber ppbBlock; 
                     ADC_SOCNumber ppbsocNumber; 
                     int16_t calibration_offset; 
                     int16_t reference_offset; 
                     int32_t high_trip_limit; 
                     int32_t low_trip_limit; 
                     bool Enable_PPB_Two_Complement; 
                     bool Enable_PPB_Event_CBC_Clear; 
                     bool Enable_PPB_Event_ADC_Trip_High;
                     bool Enable_PPB_Event_ADC_Trip_Low;
                     bool Enable_PPB_Event_ADC_Zero_Crossing;
                     bool Enable_PPB_Interrupt_Event_ADC_Trip_High;
                     bool Enable_PPB_Interrupt_Event_ADC_Trip_Low;
                     bool Enable_PPB_Interrupt_Event_ADC_Zero_Crossing;
                     ADC_Trigger burstmode_trigger; 
                     uint16_t burstSize; 
                     bool Enable_BurstMode;
                     uint32_t result_base;
                     uint16_t delay;
                     bool Enable_Interrupt;
                     uint16_t ADC_Module;
                     
} ADCSetupStruct;

typedef struct{
                    uint32_t base;
                    ADC_IntNumber interrupt_selectionnumber;
} ADCReleaseStruct;

void ADC_init(ADCSetupStruct *ptr);



uint32_t ADC_step(uint32_t base, ADC_IntNumber interrupt_selectionnumber, ADC_Trigger soc_trigger_source, uint32_t result_base, ADC_SOCNumber socNumber, ADC_PPBNumber ppbBlock, uint16_t delay, uint16_t data_type, bool interrupt_enable);

void  ADC_release(ADCReleaseStruct *ptr);

#endif
	


