
#include "MW_ECAP.h"


void ECAP_setup(ECAPSetupStruct *ptr)
{
   ECAP_disableInterrupt(ptr->base,
		(ECAP_ISR_SOURCE_CAPTURE_EVENT_1  |
		 ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
		 ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
		 ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
		 ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
		 ECAP_ISR_SOURCE_COUNTER_PERIOD   |
		 ECAP_ISR_SOURCE_COUNTER_COMPARE  |
         ECAP_ISR_SOURCE_MUNIT_1_ERROR_EVT1 |
         ECAP_ISR_SOURCE_MUNIT_1_ERROR_EVT2 |
         ECAP_ISR_SOURCE_MUNIT_2_ERROR_EVT1 |
         ECAP_ISR_SOURCE_MUNIT_2_ERROR_EVT2));

    ECAP_clearInterrupt(ptr->base,
	  	(ECAP_ISR_SOURCE_CAPTURE_EVENT_1 |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_2  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_3  |
		ECAP_ISR_SOURCE_CAPTURE_EVENT_4  |
		ECAP_ISR_SOURCE_COUNTER_OVERFLOW |
		ECAP_ISR_SOURCE_COUNTER_PERIOD   |
		ECAP_ISR_SOURCE_COUNTER_COMPARE  |
        ECAP_ISR_SOURCE_MUNIT_1_ERROR_EVT1 |
        ECAP_ISR_SOURCE_MUNIT_1_ERROR_EVT2 |
        ECAP_ISR_SOURCE_MUNIT_2_ERROR_EVT1 |
        ECAP_ISR_SOURCE_MUNIT_2_ERROR_EVT2));

    if(ptr->ECAP_Mode == 0)
    {
        ECAP_disableTimeStampCapture(ptr->base);
        ECAP_stopCounter(ptr->base);
        ECAP_enableCaptureMode(ptr->base);
        ECAP_setCaptureMode(ptr->base, ptr->capture_mode, ptr->ecap_events);
        ECAP_setEventPrescaler(ptr->base, ptr->preScalerValue);
        ECAP_setEventPolarity(ptr->base, ECAP_EVENT_1, ptr->Event1_Polarity);
        ECAP_setEventPolarity(ptr->base, ECAP_EVENT_2, ptr->Event2_Polarity);
        ECAP_setEventPolarity(ptr->base, ECAP_EVENT_3, ptr->Event3_Polarity);
        ECAP_setEventPolarity(ptr->base, ECAP_EVENT_4, ptr->Event4_Polarity);
        if(ptr->Enable_CounterReset_Event1)
            ECAP_enableCounterResetOnEvent(ptr->base, ECAP_EVENT_1);
        else
            ECAP_disableCounterResetOnEvent(ptr->base, ECAP_EVENT_1);
        if(ptr->Enable_CounterReset_Event2)
            ECAP_enableCounterResetOnEvent(ptr->base, ECAP_EVENT_2);
        else
            ECAP_disableCounterResetOnEvent(ptr->base, ECAP_EVENT_2);
        if(ptr->Enable_CounterReset_Event3)
            ECAP_enableCounterResetOnEvent(ptr->base, ECAP_EVENT_3);
        else
            ECAP_disableCounterResetOnEvent(ptr->base, ECAP_EVENT_3);
        if(ptr->Enable_CounterReset_Event4)
            ECAP_enableCounterResetOnEvent(ptr->base, ECAP_EVENT_4);
        else
            ECAP_disableCounterResetOnEvent(ptr->base, ECAP_EVENT_4);
        ECAP_selectECAPInput(ptr->base, ptr->inputCaptureSignals);
        ECAP_selectQualPeriod(ptr->base, ptr->pulse_width_filter_select);
        ECAP_setPhaseShiftCount(ptr->base, ptr->phaseValue);
        if(ptr->enable_LoadCounter)
            ECAP_enableLoadCounter(ptr->base);
        else    
            ECAP_disableLoadCounter(ptr->base);
        if(ptr->LoadCounter && ptr->enable_LoadCounter)
            ECAP_loadCounter(ptr->base);
        
        ECAP_setSyncOutMode(ptr->base, ptr->sync_outmode);

        if(ptr->reset_counters)
            ECAP_resetCounters(ptr->base);
        ECAP_setEmulationMode(ptr->base, ptr->emulation_mode);
        ECAP_setSyncInPulseSource(ptr->base, ptr->syncInPulseSrc);
        ECAP_startCounter(ptr->base);
        ECAP_enableTimeStampCapture(ptr->base);
        if(ptr->rearm_ecap)
            ECAP_reArm(ptr->base);

        if(ptr->UseInterrupt && ptr->EnableInterrupt)
            {
                uint16_t ECAPIntFlags = 0x0U;
                if(ptr->Enable_Event1_ISR)
                    ECAPIntFlags = ECAPIntFlags | ECAP_ISR_SOURCE_CAPTURE_EVENT_1;
                if(ptr->Enable_Event2_ISR)
                    ECAPIntFlags = ECAPIntFlags | ECAP_ISR_SOURCE_CAPTURE_EVENT_2;
                if(ptr->Enable_Event3_ISR)
                    ECAPIntFlags = ECAPIntFlags | ECAP_ISR_SOURCE_CAPTURE_EVENT_3;
                if(ptr->Enable_Event4_ISR)
                    ECAPIntFlags = ECAPIntFlags | ECAP_ISR_SOURCE_CAPTURE_EVENT_4;
                if(ptr->Counter_Overflow_ISR)
                    ECAPIntFlags = ECAPIntFlags | ECAP_ISR_SOURCE_COUNTER_OVERFLOW;

                ECAP_enableInterrupt(ptr->base, ECAPIntFlags);

            }

        if(ptr->trip_signal!=0)
            ECAP_selectTripSignal(ptr->base, ptr->trip_signal);
        ECAP_selectGlobalLoadStrobe(ptr->base, ptr->global_strobe);

        if(ptr->Enable_signal_MUNIT1)
        {
            ECAP_enableSignalMonitoringUnit(ptr->base, ptr->Monitoring_type_MUNIT1);
            ECAP_selectMonitoringType(ptr->base, ECAP_MONITORING_UNIT_1, ptr->Monitoring_type_MUNIT1);
            if(ptr->Monitoring_minval_MUNIT1 != 0)
                ECAP_configureMinValue(ptr->base, ECAP_MONITORING_UNIT_1, ptr->Monitoring_minval_MUNIT1);
            if(ptr->Monitoring_maxval_MUNIT1 != 0)
                ECAP_configureMaxValue(ptr->base, ECAP_MONITORING_UNIT_1, ptr->Monitoring_maxval_MUNIT1);
            if(ptr->Enable_syncin_MUNIT1)
            {
                ECAP_enableShadowMinMaxRegisters(ptr->base, ECAP_MONITORING_UNIT_1);
                ECAP_selectShadowLoadMode(ptr->base, ECAP_MONITORING_UNIT_1, ptr->SHDW_Load_Mode_MUNIT1);
                if(ptr->Force_copy_from_shadow_MUNIT1)
                    ECAP_enableSoftwareSync(ptr->base, ECAP_MONITORING_UNIT_1);
                if(ptr->SHDW_min_val_MUNIT1 != 0)
                    ECAP_configureShadowMinValue(ptr->base, ECAP_MONITORING_UNIT_1, ptr->SHDW_min_val_MUNIT1);
                if(ptr->SHDW_max_val_MUNIT1)
                    ECAP_configureShadowMaxValue(ptr->base, ECAP_MONITORING_UNIT_1, ptr->SHDW_max_val_MUNIT1);
                if(ptr->Enable_Debug_MUNIT1)
                    ECAP_enableDebugRange(ptr->base, ECAP_MONITORING_UNIT_1);
            }
        }

        if(ptr->Enable_signal_MUNIT2)
        {
            ECAP_enableSignalMonitoringUnit(ptr->base, ptr->Monitoring_type_MUNIT2);
            ECAP_selectMonitoringType(ptr->base, ECAP_MONITORING_UNIT_2, ptr->Monitoring_type_MUNIT2);
            if(ptr->Monitoring_minval_MUNIT2 != 0)
                ECAP_configureMinValue(ptr->base, ECAP_MONITORING_UNIT_2, ptr->Monitoring_minval_MUNIT2);
            if(ptr->Monitoring_maxval_MUNIT2 != 0)
                ECAP_configureMaxValue(ptr->base, ECAP_MONITORING_UNIT_2, ptr->Monitoring_maxval_MUNIT2);
            if(ptr->Enable_syncin_MUNIT2)
            {
                ECAP_enableShadowMinMaxRegisters(ptr->base, ECAP_MONITORING_UNIT_2);
                ECAP_selectShadowLoadMode(ptr->base, ECAP_MONITORING_UNIT_2, ptr->SHDW_Load_Mode_MUNIT2);
                if(ptr->Force_copy_from_shadow_MUNIT2)
                    ECAP_enableSoftwareSync(ptr->base, ECAP_MONITORING_UNIT_2);
                if(ptr->SHDW_min_val_MUNIT2 != 0)
                    ECAP_configureShadowMinValue(ptr->base, ECAP_MONITORING_UNIT_2, ptr->SHDW_min_val_MUNIT2);
                if(ptr->SHDW_max_val_MUNIT2)
                    ECAP_configureShadowMaxValue(ptr->base, ECAP_MONITORING_UNIT_2, ptr->SHDW_max_val_MUNIT2);
                if(ptr->Enable_Debug_MUNIT2)
                    ECAP_enableDebugRange(ptr->base, ECAP_MONITORING_UNIT_2);
            }
        }

    }
    else if(ptr->ECAP_Mode == 1)
    {
       
        ECAP_stopCounter(ptr->base);
        ECAP_enableAPWMMode(ptr->base);
        ECAP_setAPWMPeriod(ptr->base, ptr->apwmPeriod);
        ECAP_setAPWMCompare(ptr->base, ptr->apwmCompare);
        ECAP_setAPWMPolarity(ptr->base, ptr->apwm_polarity);
        ECAP_setPhaseShiftCount(ptr->base, ptr->phaseValue);
        if(ptr->enable_LoadCounter)
            ECAP_enableLoadCounter(ptr->base);
        else    
            ECAP_disableLoadCounter(ptr->base);
        if(ptr->LoadCounter && ptr->enable_LoadCounter)
            ECAP_loadCounter(ptr->base);
        ECAP_setSyncOutMode(ptr->base, ptr->sync_outmode);
        ECAP_setEmulationMode(ptr->base, ptr->emulation_mode);
        ECAP_setSyncInPulseSource(ptr->base, ptr->syncInPulseSrc);
        ECAP_startCounter(ptr->base);

        if(ptr->UseInterrupt && ptr->EnableInterrupt)
            {
                uint16_t ECAPIntFlagsAPWM = 0x0U;
                if(ptr->Counter_Period_ISR)
                    ECAPIntFlagsAPWM = ECAPIntFlagsAPWM | ECAP_ISR_SOURCE_COUNTER_PERIOD;
                if(ptr->Counter_Compare_ISR)
                    ECAPIntFlagsAPWM = ECAPIntFlagsAPWM | ECAP_ISR_SOURCE_COUNTER_COMPARE;

                ECAP_enableInterrupt(ptr->base, ECAPIntFlagsAPWM);
            }
    }


}

void ECAP_step(ECAPStepStruct *ptr, const void* dataOutput, const void* Capture_Flags, const void* Overflow_Flags)
{
    volatile uint16_t status;

    uint32_t *data_Output = (uint32_t *)dataOutput;
    uint16_t *CaptureFlags = (uint16_t *)Capture_Flags;
    uint16_t *OverflowFlags = (uint16_t *)Overflow_Flags;

    // *OverflowFlags = (uint16_t)(ECAP_getInterruptSource(ptr->base) & ECAP_ISR_SOURCE_COUNTER_OVERFLOW);

    if(ptr->ECAP_Mode == 0)
    {
        status = ECAP_getInterruptSource(ptr->base);
        if(ptr->ecap_events == 0)
        {
            status = (uint16_t)(status & ECAP_ISR_SOURCE_CAPTURE_EVENT_1);
            *CaptureFlags = (uint16_t)(status >> 1U);

            while(status & (ECAP_ISR_SOURCE_CAPTURE_EVENT_1 == 0U))
            {

            }
            
            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_1);

            if(!ptr->UseInterrupt | !ptr->EnableInterrupt | !ptr->Enable_Event1_ISR)
                ECAP_clearInterrupt(ptr->base, ECAP_ISR_SOURCE_CAPTURE_EVENT_1);   


        }
        else if(ptr->ecap_events == 1)
        {
            status = (uint16_t)(status & ECAP_ISR_SOURCE_CAPTURE_EVENT_2);
            *CaptureFlags = (uint16_t)(status >> 2U);

            while(status & (ECAP_ISR_SOURCE_CAPTURE_EVENT_2 == 0U))
            {

            }

            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_1);
            data_Output+=4;
            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_2);
            data_Output+=4;

            if(!ptr->UseInterrupt | !ptr->EnableInterrupt | !ptr->Enable_Event2_ISR)
                ECAP_clearInterrupt(ptr->base, ECAP_ISR_SOURCE_CAPTURE_EVENT_2);
          
        }
        else if(ptr->ecap_events == 2)
        {
            status = (uint16_t)(status & ECAP_ISR_SOURCE_CAPTURE_EVENT_3);
            *CaptureFlags = (uint16_t)(status >> 3U);

            while(status & (ECAP_ISR_SOURCE_CAPTURE_EVENT_3 == 0U))
            {

            }
          
            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_1);
            data_Output+=4;
            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_2);
            data_Output+=4;
            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_3);

            if(!ptr->UseInterrupt | !ptr->EnableInterrupt | !ptr->Enable_Event3_ISR)
                ECAP_clearInterrupt(ptr->base, ECAP_ISR_SOURCE_CAPTURE_EVENT_3);   
           
        }
        else if(ptr->ecap_events == 3)
        {
            status = (uint16_t)(status & ECAP_ISR_SOURCE_CAPTURE_EVENT_4);
            *CaptureFlags = (uint16_t)(status >> 4U);

            while(status & (ECAP_ISR_SOURCE_CAPTURE_EVENT_4 == 0U))
            {

            }

            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_1);
            data_Output+=4;
            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_2);
            data_Output+=4;
            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_3);
            data_Output+=4;
            *data_Output = ECAP_getEventTimeStamp(ptr->base, ECAP_EVENT_4);

            if(!ptr->UseInterrupt | !ptr->EnableInterrupt | !ptr->Enable_Event4_ISR)
                ECAP_clearInterrupt(ptr->base, ECAP_ISR_SOURCE_CAPTURE_EVENT_4);
           
        }  

        status = (uint16_t)(ECAP_getInterruptSource(ptr->base) & ECAP_ISR_SOURCE_COUNTER_OVERFLOW);  

        if(status != 0U)
        {
            if(!ptr->UseInterrupt | !ptr->EnableInterrupt | !ptr->Counter_Overflow_ISR)
                 ECAP_clearInterrupt(ptr->base, ECAP_ISR_SOURCE_COUNTER_OVERFLOW); 
        }
          
    }
    else
    {
        uint16_t ECAPIntFlagsClearAPWM = 0x0U;

        status = ECAP_getInterruptSource(ptr->base) & ECAP_ISR_SOURCE_COUNTER_PERIOD;

        if(status != 0U)
        {
            if(!ptr->UseInterrupt | !ptr->EnableInterrupt | !ptr->Counter_Period_ISR)
                ECAP_clearInterrupt(ptr->base, ECAP_ISR_SOURCE_COUNTER_PERIOD);
        }

        status = ECAP_getInterruptSource(ptr->base) & ECAP_ISR_SOURCE_COUNTER_COMPARE;

        if(status != 0U)
        {
            if(!ptr->UseInterrupt | !ptr->EnableInterrupt | !ptr->Counter_Compare_ISR)
                ECAP_clearInterrupt(ptr->base, ECAP_ISR_SOURCE_COUNTER_COMPARE);
        }
       

    }
    
}

void ECAP_release(ECAPReleaseStruct *ptr)
{
    ECAP_stopCounter(ptr->base);
    ECAP_disableInterrupt(ptr->base, ECAP_ISR_SOURCE_ALL);
    ECAP_clearInterrupt(ptr->base, ECAP_ISR_SOURCE_ALL);
}