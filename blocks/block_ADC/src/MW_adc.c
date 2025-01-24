

#include "MW_adc.h"



void ADC_init(ADCSetupStruct *ptr)
{
    SOC_enableAdcReference(ptr->module);
  
    ADC_setPrescaler(ptr->base, ptr->clkPrescale);
    ADC_setMode(ptr->base, ptr->resolution, ptr->signalMode);
    ADC_setSOCPriority(ptr->base, ptr->priMode);

    if(ptr->signalMode == 0)
        ADC_setupSOC(ptr->base, ptr->socNumber, ptr->soc_trigger_source, ptr->channels, ptr->sampleWindow);
    else
        ADC_setupSOC(ptr->base, ptr->socNumber, ptr->soc_trigger_source, ptr->channels1, ptr->sampleWindow);
    ADC_setInterruptSOCTrigger(ptr->base, ptr->socNumber, ptr->interrupt_trigger);
   
    if(ptr->enable_adc)
    {
        ADC_enableConverter(ptr->base);
        ClockP_usleep(500);
    }
    else
        ADC_disableConverter(ptr->base);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
    ADC_setInterruptPulseMode(ptr->base, ptr->pulse_mode);

    //Interrupt Mode
    if(ptr->Enable_Interrupt)
    {
        ADC_enableInterrupt(ptr->base, ptr->interrupt_selectionnumber);
        ADC_setInterruptSource(ptr->base, ptr->interrupt_selectionnumber, ptr->interrupt_soc);
        if(ptr->enable_adc_continuousmode1 || ptr->enable_adc_continuousmode2 || ptr->enable_adc_continuousmode3 || ptr->enable_adc_continuousmode4)
            ADC_enableContinuousMode(ptr->base, ptr->interrupt_selectionnumber);
        else
            ADC_disableContinuousMode(ptr->base, ptr->interrupt_selectionnumber);
    }
    
    
    ADC_setupPPB(ptr->base, ptr->ppbBlock, ptr->ppbsocNumber);
    if((ptr->Enable_PPB_Event_ADC_Trip_High==0) && (ptr->Enable_PPB_Event_ADC_Trip_Low==0) && (ptr->Enable_PPB_Event_ADC_Zero_Crossing==0))
        ADC_disablePPBEvent(ptr->base, ptr->ppbBlock, (ADC_EVT_TRIPHI | ADC_EVT_TRIPLO | ADC_EVT_ZERO));
    else
    {
        uint16_t PPBEvent = 0x0000;

        if(ptr->Enable_PPB_Event_ADC_Trip_High)
            PPBEvent = PPBEvent | (uint16_t)(ADC_EVT_TRIPHI);
        if(ptr->Enable_PPB_Event_ADC_Trip_Low)
            PPBEvent = PPBEvent | (uint16_t)(ADC_EVT_TRIPLO);
        if(ptr->Enable_PPB_Event_ADC_Zero_Crossing)
            PPBEvent = PPBEvent | (uint16_t)(ADC_EVT_ZERO); 

        ADC_enablePPBEvent(ptr->base, ptr->ppbBlock, PPBEvent);
    }

    if((ptr->Enable_PPB_Interrupt_Event_ADC_Trip_High==0) && (ptr->Enable_PPB_Interrupt_Event_ADC_Trip_Low==0) && (ptr->Enable_PPB_Interrupt_Event_ADC_Zero_Crossing==0))
        ADC_disablePPBEventInterrupt(ptr->base, ptr->ppbBlock, (ADC_EVT_TRIPHI | ADC_EVT_TRIPLO | ADC_EVT_ZERO));
    else
    {
        uint16_t PPBInterruptEvent = 0x0000;

        if(ptr->Enable_PPB_Interrupt_Event_ADC_Trip_High)
            PPBInterruptEvent = PPBInterruptEvent | (uint16_t)(ADC_EVT_TRIPHI);
        if(ptr->Enable_PPB_Interrupt_Event_ADC_Trip_Low)
            PPBInterruptEvent = PPBInterruptEvent | (uint16_t)(ADC_EVT_TRIPLO);
        if(ptr->Enable_PPB_Interrupt_Event_ADC_Zero_Crossing)
            PPBInterruptEvent = PPBInterruptEvent | (uint16_t)(ADC_EVT_ZERO); 

        ADC_enablePPBEventInterrupt(ptr->base, ptr->ppbBlock, PPBInterruptEvent);
    }


    ADC_setPPBCalibrationOffset(ptr->base, ptr->ppbBlock, ptr->calibration_offset);
    ADC_setPPBReferenceOffset(ptr->base, ptr->ppbBlock, ptr->reference_offset);

    if(ptr->Enable_PPB_Two_Complement)
        ADC_enablePPBTwosComplement(ptr->base, ptr->ppbBlock);
    else
        ADC_disablePPBTwosComplement(ptr->base, ptr->ppbBlock);

    ADC_setPPBTripLimits(ptr->base, ptr->ppbBlock, ptr->high_trip_limit, ptr->low_trip_limit);

    if(ptr->Enable_PPB_Event_CBC_Clear)
        ADC_enablePPBEventCBCClear(ptr->base, ptr->ppbBlock);
    else
        ADC_disablePPBEventCBCClear(ptr->base, ptr->ppbBlock);

    ADC_setBurstModeConfig(ptr->base, ptr->burstmode_trigger, ptr->burstSize);
    if(ptr->Enable_BurstMode)
        ADC_enableBurstMode(ptr->base);
    else
        ADC_disableBurstMode(ptr->base);


    

  
}

   
uint32_t ADC_step(uint32_t base, ADC_IntNumber interrupt_selectionnumber, ADC_Trigger soc_trigger_source, uint32_t result_base, ADC_SOCNumber socNumber, ADC_PPBNumber ppbBlock, uint16_t delay, uint16_t data_type, bool interrupt_enable)
{
    uint32_t y;
    
    if(soc_trigger_source == 0)
    {
        if(interrupt_enable == false)
        {   
            ADC_forceSOC(base, socNumber);

            while(((HW_RD_REG16(base + CSL_ADC_ADCCTL1) & CSL_ADC_ADCCTL1_ADCBSYCHN_MASK) >> CSL_ADC_ADCCTL1_ADCBSYCHN_SHIFT) != (uint16_t)socNumber)
            {
                /* Wait for the SOC conversion to complete */
            }
            
            y = ADC_readResult(result_base, socNumber); 
        }
        else
        {
            ADC_forceSOC(base, socNumber);
            while(ADC_getInterruptStatus(base, interrupt_selectionnumber) == false)
            {
                /* Wait for the SOC conversion to complete */
            }
            
            y = ADC_readResult(result_base, socNumber); 
        }
    }
    else
    {
        y = ADC_readResult(result_base, socNumber);   
    }
    
    return y;
}
              
void ADC_release(ADCReleaseStruct *ptr)
{
   
    ADC_disableConverter(ptr->base);
    ADC_disableInterrupt(ptr->base, ptr->interrupt_selectionnumber);
    ADC_clearInterruptStatus(ptr->base, ptr->interrupt_selectionnumber);
    ADC_clearInterruptOverflowStatus(ptr->base, ptr->interrupt_selectionnumber);
}




