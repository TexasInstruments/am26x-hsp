

#include "MW_EPWM.h"


uint16_t MEP_ScaleFactor;
uint32_t gOttoCal_base = CSL_CONTROLSS_OTTOCAL0_U_BASE;
extern int SFO_CAL;
extern int MEP_SF[(PWM_CH_MAX + 1)];

  
void epwm_timebase_setup(TimeBaseStruct *ptr)
{
    SOC_setEpwmTbClk(ptr->instance, false);
    SOC_setEpwmGroup(ptr->instance, 0);
    Soc_enableEPWMHalt(ptr->instance);
    EPWM_setEmulationMode(ptr->base, ptr->emulation_mode);
    EPWM_setClockPrescaler(ptr->base, ptr->tbclk, ptr->hsclkdiv);
  
    if(ptr->enable_timebase_global_load_mode == true)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->globalLoadRegister);
    else
        EPWM_disableGlobalLoadRegisters(ptr->base, ptr->globalLoadRegister);
    EPWM_setPeriodLoadMode(ptr->base, ptr->period_load);
    EPWM_selectPeriodLoadEvent(ptr->base, ptr->periodShadowLoadMode);
    EPWM_setTimeBaseCounter(ptr->base, ptr->timeBaseCountValue);
    EPWM_setTimeBaseCounterMode(ptr->base, ptr->counterMode);
   
    if(ptr->phaseShiftLoadEnable == true)
    {
        
        EPWM_enablePhaseShiftLoad(ptr->base);
        EPWM_setPhaseShift(ptr->base, ptr->phaseCount);

    }
    else
    {
        EPWM_disablePhaseShiftLoad(ptr->base);
        EPWM_setPhaseShift(ptr->base, 0);
    }
   
    uint16_t mask = 0x0000;
    if(ptr->SoftwareForceSyncOut == true)
        mask = mask | (uint16_t)(CSL_EPWM_EPWMSYNCOUTEN_SWEN_MASK);
    if(ptr->CounterZeroSyncOut == true)
        mask = mask | (uint16_t)(CSL_EPWM_EPWMSYNCOUTEN_ZEROEN_MASK);
    if(ptr->CounterCompareBSyncOut == true)
        mask = mask | (uint16_t)(CSL_EPWM_EPWMSYNCOUTEN_CMPBEN_MASK);
    if(ptr->CounterCompareCSyncOut == true)
        mask = mask | (uint16_t)(CSL_EPWM_EPWMSYNCOUTEN_CMPCEN_MASK);
    if(ptr->CounterCompareDSyncOut == true)
        mask = mask | (uint16_t)(CSL_EPWM_EPWMSYNCOUTEN_CMPDEN_MASK);
    if(ptr->DCAEVT1SyncOut == true)
        mask = mask | (uint16_t)(CSL_EPWM_EPWMSYNCOUTEN_DCAEVT1EN_MASK);
    if(ptr->DCAEVT2SyncOut == true)
        mask = mask | (uint16_t)(CSL_EPWM_EPWMSYNCOUTEN_DCBEVT1EN_MASK);
    EPWM_enableSyncOutPulseSource(ptr->base, mask);
    EPWM_setSyncInPulseSource(ptr->base, ptr->syncInPulseSource);
    EPWM_setOneShotSyncOutTrigger(ptr->base, ptr->oneShotTrigger);
}
	
void epwm_counter_compare_setup(CounterCompareStruct *ptr)
{
  
    if(ptr->EnableGlobalLoadCMPA)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterCMPA);
    else
        EPWM_disableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterCMPA);
    if(ptr->EnableShadowLoadCMPA)
        EPWM_setCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpa_module, ptr->ShadowAMode);
    else
    {
        EPWM_disableCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpa_module);
        EPWM_setCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpa_module, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    }
    if(ptr->Enable_LinkCMPA)
        EPWM_setupEPWMLinks(ptr->base, ptr->CMPACurrentLink, EPWM_LINK_COMP_A);

   
    if(ptr->EnableGlobalLoadCMPB)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterCMPB);
    else
        EPWM_disableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterCMPB);
    if(ptr->EnableShadowLoadCMPB)
        EPWM_setCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpb_module, ptr->ShadowBMode);
    else
    {
        EPWM_disableCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpb_module);
        EPWM_setCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpb_module, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    }
    if(ptr->Enable_LinkCMPB)
        EPWM_setupEPWMLinks(ptr->base, ptr->CMPBCurrentLink, EPWM_LINK_COMP_B);

  
    if(ptr->EnableGlobalLoadCMPC)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterCMPC);
    else
        EPWM_disableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterCMPC);
    if(ptr->EnableShadowLoadCMPC)
        EPWM_setCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpc_module, ptr->ShadowCMode);
    else
    {
        EPWM_disableCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpc_module);
        EPWM_setCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpc_module, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    }
    if(ptr->Enable_LinkCMPC)
        EPWM_setupEPWMLinks(ptr->base, ptr->CMPCCurrentLink, EPWM_LINK_COMP_C);

  
    if(ptr->EnableGlobalLoadCMPD)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterCMPD);
    else
        EPWM_disableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterCMPD);
    if(ptr->EnableShadowLoadCMPD)
        EPWM_setCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpd_module, ptr->ShadowDMode);
    else
    {
        EPWM_disableCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpd_module);
        EPWM_setCounterCompareShadowLoadMode(ptr->base, ptr->counter_cmpd_module, EPWM_COMP_LOAD_ON_CNTR_ZERO);
    }
    if(ptr->Enable_LinkCMPD)
        EPWM_setupEPWMLinks(ptr->base, ptr->CMPDCurrentLink, EPWM_LINK_COMP_D);

  
}

void epwm_action_qualifier_setup(ActionQualifierStruct *ptr)
{
    if(ptr->Enable_softwareForceGlobalLoad)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->GlobalLoadRegisterContSWForce);
    else
        EPWM_disableGlobalLoadRegisters(ptr->base, EPWM_GL_REGISTER_AQCSFRC);

    EPWM_setActionQualifierContSWForceShadowMode(ptr->base, ptr->cont_force_shadow_mode);
    EPWM_setActionQualifierT1TriggerSource(ptr->base, ptr->AQ_T1_TriggerSource);
    EPWM_setActionQualifierT2TriggerSource(ptr->base, ptr->AQ_T2_TriggerSource);

    if(ptr->Enable_ePWMXAGlobalLoad)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->ePWMXAGlobalLoadRegister);
    else
        EPWM_disableGlobalLoadRegisters(ptr->base, ptr->ePWMXAGlobalLoadRegister);

    if(ptr->Enable_ePWMXAShadowMode)
        EPWM_setActionQualifierShadowLoadMode(ptr->base, ptr->AQ_ModuleA, ptr->ePWMXAShadowLoadEvent);
    else
    {
        EPWM_disableActionQualifierShadowLoadMode(ptr->base, ptr->AQ_ModuleA);
        EPWM_setActionQualifierShadowLoadMode(ptr->base, ptr->AQ_ModuleA, EPWM_AQ_LOAD_ON_CNTR_ZERO);
    }
    EPWM_setActionQualifierSWAction(ptr->base, 0, ptr->ePWMXA_OneTimeSWForceAction);
    EPWM_setActionQualifierContSWForceAction(ptr->base, 0, ptr->ePWMXA_ContSWForceAction);

    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrZero, 0);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrPrd, 2);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrCMPAUp, 4);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrCMPADown, 6);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrCMPBUp, 8);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrCMPBDown, 10);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrT1Up, 1);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrT1Down, 3);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrT2Up, 5);
    EPWM_setActionQualifierAction(ptr->base, 0, ptr->ePWMXA_TbctrT2Down, 7);

    if(ptr->Enable_ePWMXBGlobalLoad)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->ePWMXBGlobalLoadRegister);
    else
        EPWM_disableGlobalLoadRegisters(ptr->base, ptr->ePWMXBGlobalLoadRegister);

    if(ptr->Enable_ePWMXBShadowMode)
        EPWM_setActionQualifierShadowLoadMode(ptr->base, ptr->AQ_ModuleB, ptr->ePWMXBShadowLoadEvent);
    else
    {
        EPWM_disableActionQualifierShadowLoadMode(ptr->base, ptr->AQ_ModuleB);
        EPWM_setActionQualifierShadowLoadMode(ptr->base, ptr->AQ_ModuleB, EPWM_AQ_LOAD_ON_CNTR_ZERO);
    }
    EPWM_setActionQualifierSWAction(ptr->base, 4, ptr->ePWMXB_OneTimeSWForceAction);
    EPWM_setActionQualifierContSWForceAction(ptr->base, 4, ptr->ePWMXB_ContSWForceAction);

    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrZero, 0);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrPrd, 2);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrCMPAUp, 4);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrCMPADown, 6);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrCMPBUp, 8);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrCMPBDown, 10);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrT1Up, 1);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrT1Down, 3);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrT2Up, 5);
    EPWM_setActionQualifierAction(ptr->base, 4, ptr->ePWMXB_TbctrT2Down, 7);

    

}

void epwm_trip_zone_setup(TripZoneStruct *ptr)
{
    if(ptr->advanced_epwm_tripzone_actions == true)
    {
        EPWM_enableTripZoneAdvAction(ptr->base);
        EPWM_setTripZoneAdvAction(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_TZB_D, ptr->TZB_D);
        EPWM_setTripZoneAdvAction(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_TZB_U, ptr->TZB_U);
        EPWM_setTripZoneAdvAction(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_TZA_D, ptr->TZA_D);
        EPWM_setTripZoneAdvAction(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_TZA_U, ptr->TZA_U);
        EPWM_setTripZoneAdvDigitalCompareActionA(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_DCxEVT1_U, ptr->DCAEVT1_U);
        EPWM_setTripZoneAdvDigitalCompareActionA(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_DCxEVT1_D, ptr->DCAEVT1_D);
        EPWM_setTripZoneAdvDigitalCompareActionA(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_DCxEVT2_U, ptr->DCAEVT2_U);
        EPWM_setTripZoneAdvDigitalCompareActionA(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_DCxEVT2_D, ptr->DCAEVT2_D);
        EPWM_setTripZoneAdvDigitalCompareActionB(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_DCxEVT1_U, ptr->DCBEVT1_U);
        EPWM_setTripZoneAdvDigitalCompareActionB(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_DCxEVT1_D, ptr->DCBEVT1_D);
        EPWM_setTripZoneAdvDigitalCompareActionB(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_DCxEVT2_U, ptr->DCBEVT2_U);
        EPWM_setTripZoneAdvDigitalCompareActionB(ptr->base, EPWM_TZ_ADV_ACTION_EVENT_DCxEVT2_D, ptr->DCBEVT2_D);
    }
    else
    {
        EPWM_disableTripZoneAdvAction(ptr->base);
        EPWM_setTripZoneAction(ptr->base, EPWM_TZ_ACTION_EVENT_TZA, ptr->TZA_Event);
        EPWM_setTripZoneAction(ptr->base, EPWM_TZ_ACTION_EVENT_TZB, ptr->TZB_Event);
        EPWM_setTripZoneAction(ptr->base, EPWM_TZ_ACTION_EVENT_DCAEVT1, ptr->DCAEVT1_Event);
        EPWM_setTripZoneAction(ptr->base, EPWM_TZ_ACTION_EVENT_DCAEVT2, ptr->DCAEVT2_Event);
        EPWM_setTripZoneAction(ptr->base, EPWM_TZ_ACTION_EVENT_DCBEVT1, ptr->DCBEVT1_Event);
        EPWM_setTripZoneAction(ptr->base, EPWM_TZ_ACTION_EVENT_DCBEVT2, ptr->DCBEVT2_Event);
    }

    uint32_t tzSignal_mask = 0x0U;

    if(ptr->enable_EPWM_TZ_SIGNAL_OSHT1)
        tzSignal_mask = tzSignal_mask | (EPWM_TZ_SIGNAL_OSHT1);
    if(ptr->enable_EPWM_TZ_SIGNAL_OSHT2)
        tzSignal_mask = tzSignal_mask | (EPWM_TZ_SIGNAL_OSHT2);
    if(ptr->enable_EPWM_TZ_SIGNAL_OSHT3)
        tzSignal_mask = tzSignal_mask | (EPWM_TZ_SIGNAL_OSHT3);
    if(ptr->enable_EPWM_TZ_SIGNAL_OSHT4)
        tzSignal_mask = tzSignal_mask | (EPWM_TZ_SIGNAL_OSHT4);
    if(ptr->enable_EPWM_TZ_SIGNAL_OSHT5)
        tzSignal_mask = tzSignal_mask | (EPWM_TZ_SIGNAL_OSHT5);
    if(ptr->enable_EPWM_TZ_SIGNAL_OSHT6)
        tzSignal_mask = tzSignal_mask | (EPWM_TZ_SIGNAL_OSHT6);
    if(ptr->enable_EPWM_TZ_SIGNAL_DCAEVT1)
        tzSignal_mask = tzSignal_mask | (EPWM_TZ_SIGNAL_DCAEVT1);
    if(ptr->enable_EPWM_TZ_SIGNAL_DCBEVT1)
        tzSignal_mask = tzSignal_mask | (EPWM_TZ_SIGNAL_DCBEVT1);

    EPWM_enableTripZoneSignals(ptr->base, tzSignal_mask);

    uint32_t tzSignal_mask1 = 0x0U;

    if(ptr->enable_EPWM_TZ_SIGNAL_CBC1)
        tzSignal_mask1 = tzSignal_mask1 | (EPWM_TZ_SIGNAL_CBC1);
    if(ptr->enable_EPWM_TZ_SIGNAL_CBC2)
        tzSignal_mask1 = tzSignal_mask1 | (EPWM_TZ_SIGNAL_CBC2);
    if(ptr->enable_EPWM_TZ_SIGNAL_CBC3)
        tzSignal_mask1 = tzSignal_mask1 | (EPWM_TZ_SIGNAL_CBC3);
    if(ptr->enable_EPWM_TZ_SIGNAL_CBC4)
        tzSignal_mask1 = tzSignal_mask1 | (EPWM_TZ_SIGNAL_CBC4);
    if(ptr->enable_EPWM_TZ_SIGNAL_CBC5)
        tzSignal_mask1 = tzSignal_mask1 | (EPWM_TZ_SIGNAL_CBC5);
    if(ptr->enable_EPWM_TZ_SIGNAL_CBC6)
        tzSignal_mask1 = tzSignal_mask1 | (EPWM_TZ_SIGNAL_CBC6);
    if(ptr->enable_EPWM_TZ_SIGNAL_DCAEVT2)
        tzSignal_mask1 = tzSignal_mask1 | (EPWM_TZ_SIGNAL_DCAEVT2);
    if(ptr->enable_EPWM_TZ_SIGNAL_DCBEVT2)
        tzSignal_mask1 = tzSignal_mask1 | (EPWM_TZ_SIGNAL_DCBEVT2);


    EPWM_enableTripZoneSignals(ptr->base, tzSignal_mask1);

    EPWM_selectCycleByCycleTripZoneClearEvent(ptr->base, ptr->CBC_Clear_Event);

    uint16_t tzSignal_interrupt = 0x0U;

    if(ptr->CBC_interrupt)
        tzSignal_interrupt = tzSignal_interrupt | (EPWM_TZ_INTERRUPT_CBC);
    if(ptr->OneShot_interrupt)
        tzSignal_interrupt = tzSignal_interrupt | (EPWM_TZ_INTERRUPT_OST);
    if(ptr->DCAEVT1_interrupt)
        tzSignal_interrupt = tzSignal_interrupt | (EPWM_TZ_INTERRUPT_DCAEVT1);
    if(ptr->DCAEVT2_interrupt)
        tzSignal_interrupt = tzSignal_interrupt | (EPWM_TZ_INTERRUPT_DCAEVT2);
    if(ptr->DCBEVT1_interrupt)
        tzSignal_interrupt = tzSignal_interrupt | (EPWM_TZ_INTERRUPT_DCBEVT1);
    if(ptr->DCBEVT2_interrupt)
        tzSignal_interrupt = tzSignal_interrupt | (EPWM_TZ_INTERRUPT_DCBEVT2);

    EPWM_enableTripZoneInterrupt(ptr->base, tzSignal_interrupt);


}
void epwm_digital_compare_setup(DigitalCompareStruct *ptr)
{
    EPWM_setDigitalCompareFilterInput(ptr->base, ptr->DCFilter_InputEventSource);
    if(ptr->EnableBlankingWindow)
    {
            EPWM_enableDigitalCompareBlankingWindow(ptr->base);
            if(ptr->BWStartEvent != 3)
                EPWM_setDigitalCompareBlankingEvent(ptr->base, ptr->BWStartEvent, 0);
            else
            {
               uint16_t mixedsource_mask = 0X0;

               if(ptr->Enable_MixedEvent_TBCTRZero)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x1);
               if(ptr->Enable_MixedEvent_TBCTRPeriod)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x2);
               if(ptr->Enable_MixedEvent_TBCTRCMPAIncreasing)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x4);
               if(ptr->Enable_MixedEvent_TBCTRCMPADecreasing)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x8);
               if(ptr->Enable_MixedEvent_TBCTRCMPBIncreasing)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x10);
               if(ptr->Enable_MixedEvent_TBCTRCMPBDecreasing)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x20);
               if(ptr->Enable_MixedEvent_TBCTRCMPCIncreasing)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x40);
               if(ptr->Enable_MixedEvent_TBCTRCMPCDecreasing)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x80);
               if(ptr->Enable_MixedEvent_TBCTRCMPDIncreasing)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x100);
               if(ptr->Enable_MixedEvent_TBCTRCMPDDecreasing)
                mixedsource_mask = mixedsource_mask | (uint16_t)(0x200);

               EPWM_setDigitalCompareBlankingEvent(ptr->base, ptr->BWStartEvent, mixedsource_mask);
            }

            EPWM_setDigitalCompareWindowOffset(ptr->base, ptr->BW_Offset);
            EPWM_setDigitalCompareWindowLength(ptr->base, ptr->BW_Length);

            if(ptr->Enable_InvertBW)
                EPWM_enableDigitalCompareWindowInverseMode(ptr->base);
            else
                EPWM_disableDigitalCompareWindowInverseMode(ptr->base);

    }
    else
    {
        EPWM_disableDigitalCompareBlankingWindow(ptr->base);

    }

    if(ptr->Enable_DCCounterCapture)
    {
        EPWM_enableDigitalCompareCounterCapture(ptr->base);
    }
    else
    {
        EPWM_disableDigitalCompareCounterCapture(ptr->base);
    }

    EPWM_setDigitalCompareCounterShadowMode(ptr->base, ptr->Enable_ShadowMode);

    if(ptr->Enable_EdgeFilter)
    {
        EPWM_enableDigitalCompareEdgeFilter(ptr->base);
        EPWM_setDigitalCompareEdgeFilterMode(ptr->base, ptr->edgeMode);
        EPWM_setDigitalCompareEdgeFilterEdgeCount(ptr->base, ptr->edgeCount);
    }
    else
    {
        EPWM_disableDigitalCompareEdgeFilter(ptr->base);
    }

    if(ptr->Enable_EdgeFilterValleyCapture)
        EPWM_enableValleyCapture(ptr->base);
    else
        EPWM_disableValleyCapture(ptr->base);

    EPWM_setValleyTriggerSource(ptr->base, ptr->trigger);
    EPWM_setValleyTriggerEdgeCounts(ptr->base, ptr->startCount, ptr->stopCount);

    if(ptr->selectDelayedOutputEdgeFilter)
    {
        EPWM_enableValleyHWDelay(ptr->base);
        EPWM_setValleySWDelayValue(ptr->base, ptr->DelayOffset);
        EPWM_setValleyDelayDivider(ptr->base, ptr->delayMode);
    }
    else
    {
        EPWM_disableValleyHWDelay(ptr->base);
        EPWM_setValleySWDelayValue(ptr->base, 0);
        EPWM_setValleyDelayDivider(ptr->base, EPWM_VALLEY_DELAY_MODE_SW_DELAY);
    }

    EPWM_enableDigitalCompareTripCombinationInput(ptr->base, 0, EPWM_DC_TYPE_DCAH);
    if(ptr->TripInputDCAH != 15)
    {
        EPWM_enableDigitalCompareTripCombinationInput(ptr->base, 0, EPWM_DC_TYPE_DCAH);
        EPWM_selectDigitalCompareTripInput(ptr->base, ptr->TripInputDCAH, EPWM_DC_TYPE_DCAH);
    
    }
    else
    {
            uint16_t DCAHighCombTrip = 0x0U; 

            if(ptr->DCAHighCombTrip1)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN1;
            if(ptr->DCAHighCombTrip2)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN2;
            if(ptr->DCAHighCombTrip3)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN3;
            if(ptr->DCAHighCombTrip4)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN4;
            if(ptr->DCAHighCombTrip5)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN5;
            if(ptr->DCAHighCombTrip6)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN6;
            if(ptr->DCAHighCombTrip7)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN7;
            if(ptr->DCAHighCombTrip8)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN8;
            if(ptr->DCAHighCombTrip9)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN9;
            if(ptr->DCAHighCombTrip10)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN10;
            if(ptr->DCAHighCombTrip11)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN11;
            if(ptr->DCAHighCombTrip12)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN12;
            if(ptr->DCAHighCombTrip13)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN13;
            if(ptr->DCAHighCombTrip14)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN14;
            if(ptr->DCAHighCombTrip15)
                DCAHighCombTrip = DCAHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN15;

            EPWM_enableDigitalCompareTripCombinationInput(ptr->base, DCAHighCombTrip, EPWM_DC_TYPE_DCAH);
            EPWM_selectDigitalCompareTripInput(ptr->base, EPWM_DC_TRIP_COMBINATION, EPWM_DC_TYPE_DCAH);  
            
    }

    if(ptr->TripInputDCAL != 15)
    {
        EPWM_enableDigitalCompareTripCombinationInput(ptr->base, 0, EPWM_DC_TYPE_DCAL);
        EPWM_selectDigitalCompareTripInput(ptr->base, ptr->TripInputDCAL, EPWM_DC_TYPE_DCAL);
         
    }
    else
    {
            uint16_t DCALowCombTrip = 0x0U; 

            if(ptr->DCALowCombTrip1)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN1;
            if(ptr->DCALowCombTrip2)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN2;
            if(ptr->DCALowCombTrip3)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN3;
            if(ptr->DCALowCombTrip4)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN4;
            if(ptr->DCALowCombTrip5)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN5;
            if(ptr->DCALowCombTrip6)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN6;
            if(ptr->DCALowCombTrip7)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN7;
            if(ptr->DCALowCombTrip8)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN8;
            if(ptr->DCALowCombTrip9)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN9;
            if(ptr->DCALowCombTrip10)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN10;
            if(ptr->DCALowCombTrip11)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN11;
            if(ptr->DCALowCombTrip12)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN12;
            if(ptr->DCALowCombTrip13)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN13;
            if(ptr->DCALowCombTrip14)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN14;
            if(ptr->DCALowCombTrip15)
                DCALowCombTrip = DCALowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN15;

            EPWM_enableDigitalCompareTripCombinationInput(ptr->base, DCALowCombTrip, EPWM_DC_TYPE_DCAL);
            EPWM_selectDigitalCompareTripInput(ptr->base, EPWM_DC_TRIP_COMBINATION, EPWM_DC_TYPE_DCAL);   

    }
   
    EPWM_setTripZoneDigitalCompareEventCondition(ptr->base, EPWM_TZ_DC_OUTPUT_A1, ptr->Compare1A_Condition);
    EPWM_setTripZoneDigitalCompareEventCondition(ptr->base, EPWM_TZ_DC_OUTPUT_A2, ptr->Compare2A_Condition);

    if(ptr->Enable_ADCSOCDCAEVT1)
        EPWM_enableDigitalCompareADCTrigger(ptr->base, EPWM_DC_MODULE_A);
    else
        EPWM_disableDigitalCompareADCTrigger(ptr->base, EPWM_DC_MODULE_A);

    if(ptr->Enable_SYNCOUTDCAEVT1)
        EPWM_enableDigitalCompareSyncEvent(ptr->base, EPWM_DC_MODULE_A);
    else
        EPWM_disableDigitalCompareSyncEvent(ptr->base, EPWM_DC_MODULE_A);

    EPWM_setDigitalCompareEventSyncMode(ptr->base, EPWM_DC_MODULE_A, EPWM_DC_EVENT_1, ptr->SynchModeDCAEVT1);
    EPWM_setDigitalCompareEventSource(ptr->base, EPWM_DC_MODULE_A, EPWM_DC_EVENT_1, ptr->SignalSourceDCAEVT1);
    EPWM_setDigitalCompareCBCLatchMode(ptr->base, EPWM_DC_MODULE_A, EPWM_DC_EVENT_1, ptr->CBCLatchModeDCAEVT1);
    EPWM_selectDigitalCompareCBCLatchClearEvent(ptr->base, EPWM_DC_MODULE_A, EPWM_DC_EVENT_1, ptr->CBCLatchClearDCAEVT1);
    EPWM_setDigitalCompareEventSyncMode(ptr->base, EPWM_DC_MODULE_A, EPWM_DC_EVENT_2, ptr->SynchModeDCAEVT2);
    EPWM_setDigitalCompareEventSource(ptr->base, EPWM_DC_MODULE_A,  EPWM_DC_EVENT_2, ptr->SignalSourceDCAEVT2);
    EPWM_setDigitalCompareCBCLatchMode(ptr->base, EPWM_DC_MODULE_A,  EPWM_DC_EVENT_2, ptr->CBCLatchModeDCAEVT2);
    EPWM_selectDigitalCompareCBCLatchClearEvent(ptr->base, EPWM_DC_MODULE_A,  EPWM_DC_EVENT_2, ptr->CBCLatchClearDCAEVT2);

    if(ptr->TripInputDCBH != 15)
    {
        EPWM_enableDigitalCompareTripCombinationInput(ptr->base, 0, EPWM_DC_TYPE_DCBH);
        EPWM_selectDigitalCompareTripInput(ptr->base, ptr->TripInputDCBH, EPWM_DC_TYPE_DCBH);
    }
    else
    {

         uint16_t DCBHighCombTrip = 0x0U; 

            if(ptr->DCBHighCombTrip1)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN1;
            if(ptr->DCBHighCombTrip2)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN2;
            if(ptr->DCBHighCombTrip3)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN3;
            if(ptr->DCBHighCombTrip4)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN4;
            if(ptr->DCBHighCombTrip5)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN5;
            if(ptr->DCBHighCombTrip6)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN6;
            if(ptr->DCBHighCombTrip7)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN7;
            if(ptr->DCBHighCombTrip8)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN8;
            if(ptr->DCBHighCombTrip9)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN9;
            if(ptr->DCBHighCombTrip10)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN10;
            if(ptr->DCBHighCombTrip11)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN11;
            if(ptr->DCBHighCombTrip12)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN12;
            if(ptr->DCBHighCombTrip13)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN13;
            if(ptr->DCBHighCombTrip14)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN14;
            if(ptr->DCBHighCombTrip15)
                DCBHighCombTrip = DCBHighCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN15;

            EPWM_enableDigitalCompareTripCombinationInput(ptr->base, DCBHighCombTrip, EPWM_DC_TYPE_DCBH);
            EPWM_selectDigitalCompareTripInput(ptr->base, EPWM_DC_TRIP_COMBINATION, EPWM_DC_TYPE_DCBH); 

    }

    if(ptr->TripInputDCBL != 15)
    {
        EPWM_enableDigitalCompareTripCombinationInput(ptr->base, 0, EPWM_DC_TYPE_DCBL);
        EPWM_selectDigitalCompareTripInput(ptr->base, ptr->TripInputDCBH, EPWM_DC_TYPE_DCBL);
    }
    else
    {

            uint16_t DCBLowCombTrip = 0x0U; 

            if(ptr->DCBLowCombTrip1)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN1;
            if(ptr->DCBLowCombTrip2)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN2;
            if(ptr->DCBLowCombTrip3)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN3;
            if(ptr->DCBLowCombTrip4)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN4;
            if(ptr->DCBLowCombTrip5)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN5;
            if(ptr->DCBLowCombTrip6)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN6;
            if(ptr->DCBLowCombTrip7)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN7;
            if(ptr->DCBLowCombTrip8)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN8;
            if(ptr->DCBLowCombTrip9)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN9;
            if(ptr->DCBLowCombTrip10)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN10;
            if(ptr->DCBLowCombTrip11)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN11;
            if(ptr->DCBLowCombTrip12)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN12;
            if(ptr->DCBLowCombTrip13)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN13;
            if(ptr->DCBLowCombTrip14)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN14;
            if(ptr->DCBLowCombTrip15)
                DCBLowCombTrip = DCBLowCombTrip | EPWM_DC_COMBINATIONAL_TRIPIN15;

            EPWM_enableDigitalCompareTripCombinationInput(ptr->base, DCBLowCombTrip, EPWM_DC_TYPE_DCBL);
            EPWM_selectDigitalCompareTripInput(ptr->base, EPWM_DC_TRIP_COMBINATION, EPWM_DC_TYPE_DCBL); 

    } 

    EPWM_setTripZoneDigitalCompareEventCondition(ptr->base, EPWM_TZ_DC_OUTPUT_B1, ptr->Compare1B_Condition);
    EPWM_setTripZoneDigitalCompareEventCondition(ptr->base, EPWM_TZ_DC_OUTPUT_B2, ptr->Compare2B_Condition);

    if(ptr->Enable_ADCSOCDCBEVT1)
        EPWM_enableDigitalCompareADCTrigger(ptr->base, EPWM_DC_MODULE_A);
    else
        EPWM_disableDigitalCompareADCTrigger(ptr->base, EPWM_DC_MODULE_A);

    if(ptr->Enable_SYNCOUTDCBEVT1)
        EPWM_enableDigitalCompareSyncEvent(ptr->base, EPWM_DC_MODULE_B);
    else
        EPWM_disableDigitalCompareSyncEvent(ptr->base, EPWM_DC_MODULE_B);

    EPWM_setDigitalCompareEventSyncMode(ptr->base, EPWM_DC_MODULE_B, EPWM_DC_EVENT_1, ptr->SynchModeDCBEVT1);
    EPWM_setDigitalCompareEventSource(ptr->base, EPWM_DC_MODULE_B, EPWM_DC_EVENT_1, ptr->SignalSourceDCBEVT1);
    EPWM_setDigitalCompareCBCLatchMode(ptr->base, EPWM_DC_MODULE_B, EPWM_DC_EVENT_1, ptr->CBCLatchModeDCBEVT1);
    EPWM_selectDigitalCompareCBCLatchClearEvent(ptr->base, EPWM_DC_MODULE_B, EPWM_DC_EVENT_1, ptr->CBCLatchClearDCBEVT1);
    EPWM_setDigitalCompareEventSyncMode(ptr->base, EPWM_DC_MODULE_B, EPWM_DC_EVENT_2, ptr->SynchModeDCBEVT2);
    EPWM_setDigitalCompareEventSource(ptr->base, EPWM_DC_MODULE_B,  EPWM_DC_EVENT_2, ptr->SignalSourceDCBEVT2);
    EPWM_setDigitalCompareCBCLatchMode(ptr->base, EPWM_DC_MODULE_B,  EPWM_DC_EVENT_2, ptr->CBCLatchModeDCBEVT2);
    EPWM_selectDigitalCompareCBCLatchClearEvent(ptr->base, EPWM_DC_MODULE_B,  EPWM_DC_EVENT_2, ptr->CBCLatchClearDCBEVT2);

    if(ptr->Enable_Edge_Detection)
    {
            EPWM_enableDigitalCompareCounterCapture(ptr->base);
            if(ptr->Capture_Input!=15)
                EPWM_selectCaptureTripInput(ptr->base, ptr->Capture_Input, EPWM_CAPTURE_INPUT);
            else
            {
                uint16_t Capture_combination_input_mask = 0x0U;

                if(ptr->Capture_Input_Trip1)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip2)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip3)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip4)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip5)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip6)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip7)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip8)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip9)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip10)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip11)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip12)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip13)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip14)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Input_Trip15)
                    Capture_combination_input_mask = Capture_combination_input_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                
                if(Capture_combination_input_mask == 0x0U)
                    EPWM_selectCaptureTripInput(ptr->base, EPWM_DC_TRIP_COMBINATION, EPWM_CAPTURE_INPUT);
                else
                    EPWM_enableCaptureTripCombinationInput(ptr->base, Capture_combination_input_mask, EPWM_CAPTURE_INPUT); 
            }

           EPWM_invertCaptureInputPolarity(ptr->base, ptr->Invert_Capture_Input_Polarity);

           if(ptr->Capture_Gate!=15)
                EPWM_selectCaptureTripInput(ptr->base, ptr->Capture_Gate, EPWM_CAPTURE_GATE);
            else
            {
                uint16_t Capture_combination_gate_mask = 0x0U;

                if(ptr->Capture_Gate_Trip1)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip2)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip3)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip4)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip5)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip6)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip7)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip8)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip9)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip10)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip11)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip12)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip13)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip14)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                if(ptr->Capture_Gate_Trip15)
                    Capture_combination_gate_mask = Capture_combination_gate_mask | EPWM_DC_COMBINATIONAL_TRIPIN1;
                
                if(Capture_combination_gate_mask == 0x0U)
                    EPWM_selectCaptureTripInput(ptr->base, EPWM_DC_TRIP_COMBINATION, EPWM_CAPTURE_GATE);
                else
                    EPWM_enableCaptureTripCombinationInput(ptr->base, Capture_combination_gate_mask, EPWM_CAPTURE_GATE); 
            }
    }
    else
        {
            EPWM_disableDigitalCompareCounterCapture(ptr->base);
            EPWM_selectCaptureTripInput(ptr->base, EPWM_DC_TRIP_TRIPIN1, EPWM_CAPTURE_GATE);
	        EPWM_configCaptureGateInputPolarity(ptr->base, EPWM_CAPGATE_INPUT_ALWAYS_ON);
            EPWM_selectCaptureTripInput(ptr->base, EPWM_DC_TRIP_TRIPIN1, EPWM_CAPTURE_INPUT);
            EPWM_invertCaptureInputPolarity(ptr->base, EPWM_CAPTURE_INPUT_CAPIN_SYNC);
            EPWM_disableIndependentPulseLogic(ptr->base);
        }

   

}

void epwm_deadband_setup(DeadBandStruct *ptr)
{
    EPWM_setRisingEdgeDeadBandDelayInput(ptr->base, ptr->RisingEdgeDelayInput);
    EPWM_setFallingEdgeDeadBandDelayInput(ptr->base, ptr->FallingEdgeDelayInput);
    EPWM_setDeadBandDelayPolarity(ptr->base, EPWM_DB_RED, ptr->RED_DeadBandPolarity);
    EPWM_setDeadBandDelayPolarity(ptr->base, EPWM_DB_FED, ptr->FED_DeadBandPolarity);
    if(ptr->Enable_RisingEdgeDelay)
    {
        EPWM_setDeadBandDelayMode(ptr->base, EPWM_DB_RED, true);
        EPWM_setRisingEdgeDelayCount(ptr->base, ptr->Rising_Edge_Delay_Value);
    }
    else
    {
        EPWM_setDeadBandDelayMode(ptr->base, EPWM_DB_RED, false);
        EPWM_setRisingEdgeDelayCount(ptr->base, 0);
    }
    if(ptr->Enable_FallingEdgeDelay)
    {
        EPWM_setDeadBandDelayMode(ptr->base, EPWM_DB_FED, true);
        EPWM_setFallingEdgeDelayCount(ptr->base, ptr->Falling_Edge_Delay_Value);
    }
    else
    {
        EPWM_setDeadBandDelayMode(ptr->base, EPWM_DB_FED, false);
        EPWM_setFallingEdgeDelayCount(ptr->base, 0);
    }
    if(ptr->Swap_Output_ePWMXA)
    {
        EPWM_setDeadBandOutputSwapMode(ptr->base, EPWM_DB_OUTPUT_A, true);
    }
    else{
        EPWM_setDeadBandOutputSwapMode(ptr->base, EPWM_DB_OUTPUT_A, false);
    }
    if(ptr->Swap_Output_ePWMXB)
    {
        EPWM_setDeadBandOutputSwapMode(ptr->base, EPWM_DB_OUTPUT_B, true);
    }
    else{
        EPWM_setDeadBandOutputSwapMode(ptr->base, EPWM_DB_OUTPUT_B, false);
    }
    if(ptr->Enable_DeadbandControlGlobalLoad)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->DeadbandGlobalLoadRegister);
    if(ptr->Enable_DeadbandControlShadowMode)
        EPWM_setDeadBandControlShadowLoadMode(ptr->base, ptr->DeadbandLoadMode);
    else
    {
        EPWM_disableDeadBandControlShadowLoadMode(ptr->base);
        EPWM_setDeadBandControlShadowLoadMode(ptr->base, EPWM_DB_LOAD_ON_CNTR_ZERO);
    }
    if(ptr->Enable_REDGlobalLoad)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->REDGlobalLoadRegister);
    if(ptr->Enable_REDShadowMode)
    {
        EPWM_setRisingEdgeDelayCountShadowLoadMode(ptr->base, ptr->RisingEdgeLoadMode);
        EPWM_setRisingEdgeDelayCount(ptr->base, ptr->Rising_Edge_Delay_Value);
    }
    else
    {
        EPWM_disableRisingEdgeDelayCountShadowLoadMode(ptr->base);
        EPWM_setRisingEdgeDelayCountShadowLoadMode(ptr->base, EPWM_RED_LOAD_ON_CNTR_ZERO);
        EPWM_setRisingEdgeDelayCount(ptr->base, ptr->Rising_Edge_Delay_Value);
    }
    if(ptr->Enable_FEDGlobalLoad)
        EPWM_enableGlobalLoadRegisters(ptr->base, ptr->FEDGlobalLoadRegister);
    if(ptr->Enable_FEDShadowMode)
    {
        EPWM_setFallingEdgeDelayCountShadowLoadMode(ptr->base, ptr->FallingEdgeLoadMode);
        EPWM_setFallingEdgeDelayCount(ptr->base, ptr->Falling_Edge_Delay_Value);
    }
    else
    {
        EPWM_disableFallingEdgeDelayCountShadowLoadMode(ptr->base);
        EPWM_setFallingEdgeDelayCountShadowLoadMode(ptr->base, EPWM_FED_LOAD_ON_CNTR_ZERO);
        EPWM_setFallingEdgeDelayCount(ptr->base, ptr->Falling_Edge_Delay_Value);
    }

    EPWM_setDeadBandCounterClock(ptr->base, ptr->DeadBandClockRate);


}

void epwm_chopper_setup(ChopperStruct *ptr)
{

    if(ptr->UseChopper)
    {
        EPWM_enableChopper(ptr->base);
        EPWM_setChopperDutyCycle(ptr->base, ptr->DutyCycle);
        EPWM_setChopperFreq(ptr->base, ptr->freqDiv);
        EPWM_setChopperFirstPulseWidth(ptr->base, ptr->FirstPulseWidth);
    }
    else
    {
        EPWM_disableChopper(ptr->base);
        EPWM_setChopperDutyCycle(ptr->base, 0);
        EPWM_setChopperFreq(ptr->base, 0);
        EPWM_setChopperFirstPulseWidth(ptr->base, 0);

    }
}

void epwm_event_trigger_setup(EventTriggerStruct *ptr)
{

    if(ptr->Enable_EPWMInterrupt)
     {
        EPWM_enableInterrupt(ptr->base);
        if(ptr->InterruptEventSource != 3)
        {
           EPWM_setInterruptSource(ptr->base, ptr->InterruptEventSource, 0);
        }
         EPWM_setInterruptEventCount(ptr->base, ptr->eventCount);
        if(ptr->Enable_InterruptEventCountInitialValue)
        {
                EPWM_enableInterruptEventCountInit(ptr->base);
                EPWM_setInterruptEventCountInitValue(ptr->base, ptr->EventCountInitValue);
        }
        else
        {
                EPWM_disableInterruptEventCountInit(ptr->base);
                EPWM_setInterruptEventCountInitValue(ptr->base, 0);
     }
     }
    else
    {
        EPWM_disableInterrupt(ptr->base);
        EPWM_setInterruptSource(ptr->base, EPWM_INT_TBCTR_ZERO, 0);
        EPWM_setInterruptEventCount(ptr->base, 0);
	    EPWM_disableInterruptEventCountInit(ptr->base);
	    EPWM_setInterruptEventCountInitValue(ptr->base, 0);
        EPWM_disableADCTrigger(ptr->base, EPWM_SOC_A);
   
    }

    if(ptr->SOCATriggerEnable)
    {
        EPWM_enableADCTrigger(ptr->base, EPWM_SOC_A);
        if(ptr->SOCASource != 3)
            EPWM_setADCTriggerSource(ptr->base, EPWM_SOC_A, ptr->SOCASource, 0);

        EPWM_setADCTriggerEventPrescale(ptr->base, EPWM_SOC_A, ptr->SOCA_TriggerEventCount);

        if(ptr->Enable_SOCATriggerEventCountInit)
        {
            EPWM_enableADCTriggerEventCountInit(ptr->base, EPWM_SOC_A);
            EPWM_setADCTriggerEventCountInitValue(ptr->base, EPWM_SOC_A, ptr->SOCATriggerInitValue);
        }
        else
        {
            EPWM_disableADCTriggerEventCountInit(ptr->base, EPWM_SOC_A);
            EPWM_setADCTriggerEventCountInitValue(ptr->base, EPWM_SOC_A, 0);

        }
    }
    else
    {
        EPWM_disableADCTrigger(ptr->base, EPWM_SOC_A);
        EPWM_setADCTriggerSource(ptr->base, EPWM_SOC_A, 0, 0);
        EPWM_setADCTriggerEventPrescale(ptr->base, EPWM_SOC_A, 0);
        EPWM_disableADCTriggerEventCountInit(ptr->base, EPWM_SOC_A);
        EPWM_setADCTriggerEventCountInitValue(ptr->base, EPWM_SOC_A, 0);
    }

    if(ptr->SOCBTriggerEnable)
    {
        EPWM_enableADCTrigger(ptr->base, EPWM_SOC_B );
        if(ptr->SOCBSource != 3)
            EPWM_setADCTriggerSource(ptr->base, EPWM_SOC_B, ptr->SOCBSource, 0);
           
        EPWM_setADCTriggerEventPrescale(ptr->base, EPWM_SOC_B, ptr->SOCB_TriggerEventCount);

        if(ptr->Enable_SOCBTriggerEventCountInit)
        {
            EPWM_enableADCTriggerEventCountInit(ptr->base, EPWM_SOC_B);
            EPWM_setADCTriggerEventCountInitValue(ptr->base, EPWM_SOC_B, ptr->SOCBTriggerInitValue);
        }
        else
        {
            EPWM_disableADCTriggerEventCountInit(ptr->base, EPWM_SOC_B);
            EPWM_setADCTriggerEventCountInitValue(ptr->base, EPWM_SOC_B, 0);
        }
    }
    else
    {
        EPWM_disableADCTrigger(ptr->base, EPWM_SOC_B);
        EPWM_setADCTriggerSource(ptr->base, EPWM_SOC_B, 0, 0);
        EPWM_setADCTriggerEventPrescale(ptr->base, EPWM_SOC_B, 0);
        EPWM_disableADCTriggerEventCountInit(ptr->base, EPWM_SOC_B);
        EPWM_setADCTriggerEventCountInitValue(ptr->base, EPWM_SOC_B, 0);
    }
    
}
 

void epwm_hrpwm_setup(HRPWMStruct *ptr)
{
        int32_t  status = SFO_INCOMPLETE;
        uint16_t MEP_ScaleFactor;
        uint32_t gOttoCal_base = CSL_CONTROLSS_OTTOCAL0_U_BASE;
        extern int SFO_CAL;
        extern int MEP_SF[(PWM_CH_MAX + 1)];
     
     
        if(ptr->Enable_HRPWMSupport && ptr->Enable_AutomaticHRMSTEPScaling)
            HRPWM_enableAutoConversion(ptr->base);
        else
            HRPWM_disableAutoConversion(ptr->base);

           if(ptr->Enable_HRPWMSupport && ptr->Enable_AutomaticHRMSTEPScaling && ptr->Enable_SFOLibrary)
           {
            //  while(status == SFO_INCOMPLETE)
            // {
            //     status = SFO();
        
            // }
            HWREGH(gOttoCal_base + CSL_OTTOCAL_HRMSTEP) = ptr->MEP_SCALE;

            }

        
            HRPWM_setMEPControlMode(ptr->base, HRPWM_CHANNEL_A, ptr->ControlModeChannelA);
            HRPWM_setMEPControlMode(ptr->base, HRPWM_CHANNEL_B, ptr->ControlModeChannelB);
        
        
            HRPWM_setPhaseShift(ptr->base, ptr->hrPhaseCount);
            HRPWM_setSyncPulseSource(ptr->base, HRPWM_PWMSYNC_SOURCE_PERIOD);
            if(ptr->HighResolutionPhaseEnable)
                HRPWM_enablePhaseShiftLoad(ptr->base);
            else
                HRPWM_disablePhaseShiftLoad(ptr->base);
         
            HRPWM_setMEPEdgeSelect(ptr->base, HRPWM_CHANNEL_A, ptr->ChannelAEdgeMode);

            if(ptr->ChannelAEdgeMode == 1 ||  ptr->ChannelAEdgeMode == 2 || ptr->ChannelAEdgeMode == 3)
            {
                HRPWM_setCounterCompareValue(ptr->base, HRPWM_COUNTER_COMPARE_A, ptr->HighResolutionCMPA);
                HRPWM_setCounterCompareShadowLoadEvent(ptr->base, HRPWM_CHANNEL_A, ptr->CMPAHRLoadEvent);
            }
            else
            {
                HRPWM_setCounterCompareValue(ptr->base, HRPWM_COUNTER_COMPARE_A, 1);
                HRPWM_setCounterCompareShadowLoadEvent(ptr->base, HRPWM_CHANNEL_A, HRPWM_LOAD_ON_CNTR_ZERO);
            }
         
         
            HRPWM_setMEPEdgeSelect(ptr->base, HRPWM_CHANNEL_B, ptr->ChannelBEdgeMode);

            if(ptr->ChannelBEdgeMode == 1 ||  ptr->ChannelBEdgeMode == 2 || ptr->ChannelBEdgeMode == 3)
            {
                HRPWM_setCounterCompareValue(ptr->base, HRPWM_COUNTER_COMPARE_B, ptr->HighResolutionCMPB);
                HRPWM_setCounterCompareShadowLoadEvent(ptr->base, HRPWM_CHANNEL_B, ptr->CMPBHRLoadEvent);
            }
            else
            {
                HRPWM_setCounterCompareValue(ptr->base, HRPWM_COUNTER_COMPARE_B, 1);
                HRPWM_setCounterCompareShadowLoadEvent(ptr->base, HRPWM_CHANNEL_B, HRPWM_LOAD_ON_CNTR_ZERO);
            }
        

       
            if(ptr->Enable_HRPWMHighResolutionPeriodLoad)
            {
                HRPWM_enablePeriodControl(ptr->base);
                HRPWM_setHiResTimeBasePeriod(ptr->base, ptr->hrPeriodCount);
            }
            else
            {
                HRPWM_disablePeriodControl(ptr->base);
                HRPWM_setHiResTimeBasePeriod(ptr->base, 0);
            }


        if(ptr->Enable_HRPWMSupport && (ptr->DeadbandEdgeMode == 1 || ptr->DeadbandEdgeMode == 2 || ptr->DeadbandEdgeMode == 3))
        {
            HRPWM_setDeadbandMEPEdgeSelect(ptr->base, ptr->DeadbandEdgeMode);

            if(ptr->DeadbandEdgeMode == 1)
            {
                 HRPWM_setHiResRisingEdgeDelay(ptr->base,ptr->hrREDDelay);
                 HRPWM_setRisingEdgeDelayLoadMode(ptr->base, ptr->REDLoadMode);
                 HRPWM_setHiResFallingEdgeDelayOnly(ptr->base, 0);
                 HRPWM_setFallingEdgeDelayLoadMode(ptr->base, HRPWM_LOAD_ON_CNTR_ZERO);

                 if(ptr->Swap_AB_Outputs)
                    HRPWM_setOutputSwapMode(ptr->base, true);
                 else
                    HRPWM_setOutputSwapMode(ptr->base, false);

                HRPWM_setChannelBOutputPath(ptr->base, ptr->EPWMxBOutputPath);
            }


            if(ptr->DeadbandEdgeMode == 2)
            {
                HRPWM_setHiResRisingEdgeDelay(ptr->base, 0);
                HRPWM_setRisingEdgeDelayLoadMode(ptr->base, HRPWM_LOAD_ON_CNTR_ZERO);
                HRPWM_setHiResFallingEdgeDelayOnly(ptr->base, ptr->hrFEDDelay);
                HRPWM_setFallingEdgeDelayLoadMode(ptr->base, ptr->FEDLoadMode);

                 if(ptr->Swap_AB_Outputs)
                    HRPWM_setOutputSwapMode(ptr->base, true);
                 else
                    HRPWM_setOutputSwapMode(ptr->base, false);

                HRPWM_setChannelBOutputPath(ptr->base, ptr->EPWMxBOutputPath);
            }


            if(ptr->DeadbandEdgeMode == 3)
            {
                HRPWM_setHiResRisingEdgeDelay(ptr->base, ptr->hrREDDelay);
                HRPWM_setRisingEdgeDelayLoadMode(ptr->base, ptr->REDLoadMode);
                HRPWM_setHiResFallingEdgeDelayOnly(ptr->base, ptr->hrFEDDelay);
                HRPWM_setFallingEdgeDelayLoadMode(ptr->base, ptr->FEDLoadMode);
                if(ptr->Swap_AB_Outputs)
                    HRPWM_setOutputSwapMode(ptr->base, true);
                else
                    HRPWM_setOutputSwapMode(ptr->base, false);
                HRPWM_setChannelBOutputPath(ptr->base, ptr->EPWMxBOutputPath);

            }
        }
        else
        {
            HRPWM_setHiResRisingEdgeDelay(ptr->base, 0);
            HRPWM_setRisingEdgeDelayLoadMode(ptr->base, HRPWM_LOAD_ON_CNTR_ZERO);
            HRPWM_setHiResFallingEdgeDelayOnly(ptr->base, 0);
            HRPWM_setFallingEdgeDelayLoadMode(ptr->base, HRPWM_LOAD_ON_CNTR_ZERO);
            HRPWM_setOutputSwapMode(ptr->base, false);
            HRPWM_setChannelBOutputPath(ptr->base, HRPWM_OUTPUT_ON_B_NORMAL);

        }
}
   
void epwm_xcmp_setup(XCMPStruct *ptr)
{
     if(ptr->enable_XCMP) 
    {
            EPWM_enableXCMPMode(ptr->base);
    }
    else
    {
            EPWM_disableXCMPMode(ptr->base);
    } 

    if(ptr->enable_Split)
        EPWM_enableSplitXCMP(ptr->base);
    else
        EPWM_disableSplitXCMP(ptr->base);

    EPWM_allocAXCMP(ptr->base, ptr->xcmp_cmpa);

    if(ptr->enable_Split)
    {
        EPWM_allocBXCMP(ptr->base, ptr->xcmp_cmpb);
    }
    else
        EPWM_disableSplitXCMP(ptr->base);

    EPWM_setXCMPLoadMode(ptr->base, ptr->load_mode);
    EPWM_setXCMPShadowLevel(ptr->base, ptr->shadow_level);
    if(ptr->load_mode == 1)
        EPWM_setXCMPShadowBufPtrLoadOnce(ptr->base, EPWM_XCMP_XLOADCTL_SHDWBUFPTR_NULL);
    else 
        EPWM_setXCMPShadowBufPtrLoadOnce(ptr->base, ptr->shadow_register_load_once);
    EPWM_setXCMPShadowRepeatBufxCount(ptr->base, EPWM_XCMP_SHADOW2, ptr->rpt_cnt_shdw_buffer2);
    EPWM_setXCMPShadowRepeatBufxCount(ptr->base, EPWM_XCMP_SHADOW3, ptr->rpt_cnt_shdw_buffer3);

    // /* Write values to Reg */
    
    EPWM_setXCMPRegValue(ptr->base, EPWM_XTBPRD_ACTIVE, ptr->xtbprd_active);
    EPWM_setXCMPRegValue(ptr->base, EPWM_XTBPRD_SHADOW1, ptr->xtbprd_shadow1);
    EPWM_setXCMPRegValue(ptr->base, EPWM_XTBPRD_SHADOW2, ptr->xtbprd_shadow2);
    EPWM_setXCMPRegValue(ptr->base, EPWM_XTBPRD_SHADOW3, ptr->xtbprd_shadow3);
    EPWM_setXMINMAXRegValue(ptr->base, EPWM_XMIN_ACTIVE, ptr->xmin_active);
    EPWM_setXMINMAXRegValue(ptr->base, EPWM_XMAX_ACTIVE, ptr->xmax_active);
    EPWM_setXMINMAXRegValue(ptr->base, EPWM_XMIN_SHADOW1, ptr->xmin_shadow1);
    EPWM_setXMINMAXRegValue(ptr->base, EPWM_XMAX_SHADOW1, ptr->xmax_shadow1);
    EPWM_setXMINMAXRegValue(ptr->base, EPWM_XMIN_SHADOW2, ptr->xmin_shadow2);
    EPWM_setXMINMAXRegValue(ptr->base, EPWM_XMAX_SHADOW2, ptr->xmax_shadow2);
    EPWM_setXMINMAXRegValue(ptr->base, EPWM_XMIN_SHADOW3, ptr->xmin_shadow3);
    EPWM_setXMINMAXRegValue(ptr->base, EPWM_XMAX_SHADOW3, ptr->xmax_shadow3);
    
    // // /* Events */
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp1_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP1);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp2_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP2);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp3_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP3);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp4_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP4);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp5_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP5);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp6_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP6);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp7_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP7);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp8_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP8);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp5_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP5);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp6_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP6);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp7_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP7);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_ACTIVE, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp8_active, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP8);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp1_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP1);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp2_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP2);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp3_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP3);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp4_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP4);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp5_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP5);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp6_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP6);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp7_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP7);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp8_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP8);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp5_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP5);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp6_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP6);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp7_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP7);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW1, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp8_shadow1, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP8);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp1_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP1);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp2_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP2);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp3_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP3);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp4_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP4);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp5_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP5);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp6_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP6);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp7_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP7);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp8_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP8);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp5_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP5);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp6_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP6);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp7_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP7);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW2, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp8_shadow2, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP8);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp1_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP1);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp2_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP2);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp3_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP3);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp4_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP4);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp5_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP5);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp6_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP6);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp7_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP7);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_A, ptr->epwmxa_tbctr_xcmp8_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP8);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp5_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP5);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp6_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP6);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp7_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP7);
    EPWM_setXCMPActionQualifierAction(ptr->base, EPWM_XCMP_SHADOW3, EPWM_AQ_OUTPUT_B, ptr->epwmxb_tbctr_xcmp8_shadow3, EPWM_AQ_OUTPUT_ON_TIMEBASE_XCMP8);
    
    if(ptr->shadow_level!=0)
        EPWM_enableXLoad(ptr->base);
 
    SOC_setEpwmTbClk(ptr->instance, true);
    
}

void epwm_diode_emulation_setup(DiodeEmulationStruct *ptr)
{

    if(ptr->Enable_DE_Mode)
    {
        EPWM_enableDiodeEmulationMode(ptr->base);
        EPWM_setDiodeEmulationMode(ptr->base, ptr->de_mode);
        EPWM_setDiodeEmulationReentryDelay(ptr->base, ptr->delay);
        EPWM_configureDiodeEmulationTripSources(ptr->base, ptr->decompsel_tripl, EPWM_DE_TRIPL);
        EPWM_configureDiodeEmulationTripSources(ptr->base, ptr->decompsel_triph, EPWM_DE_TRIPH);    
        EPWM_selectDiodeEmulationPWMsignal(ptr->base, EPWM_DE_CHANNEL_A, ptr->PWMA_Signal_DE_Mode);
        EPWM_selectDiodeEmulationTripSignal(ptr->base, EPWM_DE_CHANNEL_A, ptr->Trip_ChannelA);
        EPWM_selectDiodeEmulationPWMsignal(ptr->base, EPWM_DE_CHANNEL_B, ptr->PWMB_Signal_DE_Mode);
        EPWM_selectDiodeEmulationTripSignal(ptr->base, EPWM_DE_CHANNEL_B, ptr->Trip_ChannelB);
        if(ptr->bypass_de_pwm)
            EPWM_bypassDiodeEmulationLogic(ptr->base);
        else
            EPWM_nobypassDiodeEmulationLogic(ptr->base);
        if(ptr->Force_DEACTIVE)
            EPWM_forceDiodeEmulationActive(ptr->base);
        if(ptr->Enable_Trip_Monitor)
        {
            EPWM_enableDiodeEmulationMonitorModeControl(ptr->base);
            EPWM_setDiodeEmulationMonitorCounterThreshold(ptr->base, ptr->de_monitor_threshold);
            EPWM_setDiodeEmulationMonitorModeStep(ptr->base, EPWM_DE_COUNT_DOWN, ptr->dec_step_size);
            EPWM_setDiodeEmulationMonitorModeStep(ptr->base, EPWM_DE_COUNT_UP, ptr->inc_step_size);
        }
        else
            EPWM_disableDiodeEmulationMonitorModeControl(ptr->base);
           
    }
    else
    {
            EPWM_disableDiodeEmulationMode(ptr->base);
    }
}

void epwm_step(EPWMStepStruct *ptr)
{
    int32_t  status1; 
    #ifdef BMC
    uint16_t t1, t2, t3, time_delta;
    time_delta = 0;
    CycleCounterP_reset();
    t1 = CycleCounterP_getCount32();
    //Code to benchmark in generated code
    t2 = CycleCounterP_getCount32();
    t3 = CycleCounterP_getCount32();
    time_delta += (t2 - t1) - (t3 - t2);
    #endif

   

    if(ptr->phaseShiftLoadEnable)
    {
         
        EPWM_enablePhaseShiftLoad(ptr->base);
        EPWM_setPhaseShift(ptr->base, ptr->phaseCount);
        
    }
    
    if(ptr->Enable_HRPWMSupport)
    {
    
        if(ptr->ChannelAEdgeMode == 1 ||  ptr->ChannelAEdgeMode == 2 || ptr->ChannelAEdgeMode == 3)
            HRPWM_setCounterCompareValue(ptr->base, HRPWM_COUNTER_COMPARE_A, ptr->HighResolutionCMPA);
        else
            HRPWM_setCounterCompareValue(ptr->base, HRPWM_COUNTER_COMPARE_A, 1);
        if(ptr->ChannelBEdgeMode == 1 ||  ptr->ChannelBEdgeMode == 2 || ptr->ChannelBEdgeMode == 3)
            HRPWM_setCounterCompareValue(ptr->base, HRPWM_COUNTER_COMPARE_B, ptr->HighResolutionCMPB);
        else
            HRPWM_setCounterCompareValue(ptr->base, HRPWM_COUNTER_COMPARE_B, 1);

           HRPWM_setPhaseShift(ptr->base, ptr->hrPhaseCount); 
             if(ptr->Enable_HRPWMHighResolutionPeriodLoad)
            {
             
                HRPWM_setHiResTimeBasePeriod(ptr->base, ptr->hrPeriodCount);
            }
        
        if(ptr->Enable_AutomaticHRMSTEPScaling && ptr->Enable_SFOLibrary)
        {
            status1 = SFO();
        }
    }
     
  
    if(ptr->shadow_level!=0)
    {
       
        EPWM_enableXLoad(ptr->base);
         
        
    }
        
}
    




void epwm_release(EPWMReleaseStruct *ptr)
{
    EPWM_disableInterrupt(ptr->base);
    EPWM_clearEventTriggerInterruptFlag(ptr->base);
    EPWM_disableInterruptEventCountInit(ptr->base);
}











