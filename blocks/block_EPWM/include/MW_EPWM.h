/********************************************************************
 * Copyright (C) 2025 Texas Instruments Incorporated.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *

*/
#ifndef EPWM_MW_H_
#define EPWM_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/epwm.h>
#include <drivers/soc.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/CycleCounterP.h>
#include <calibration/sfo.h>

#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif

#define HWREGH(x)       (*((volatile unsigned short *)(x)))
typedef struct {
    uint32_t base;
    uint32_t instance;
    EPWM_EmulationMode emulation_mode;
    EPWM_ClockDivider tbclk;
    EPWM_HSClockDivider hsclkdiv;
    uint16_t timebaseperiod;
    bool enable_timebase_global_load_mode;
    uint16_t globalLoadRegister;
    EPWM_PeriodLoadMode period_load;
    EPWM_PeriodShadowLoadMode periodShadowLoadMode;
    uint16_t timeBaseCountValue;
    EPWM_TimeBaseCountMode counterMode;
    bool phaseShiftLoadEnable;
    uint16_t phaseCount; 
    bool SoftwareForceSyncOut;
    bool CounterZeroSyncOut; 
    bool CounterCompareBSyncOut; 
    bool CounterCompareCSyncOut;
    bool CounterCompareDSyncOut;
    bool DCAEVT1SyncOut;
    bool DCAEVT2SyncOut;
    EPWM_SyncInPulseSource syncInPulseSource;
    EPWM_OneShotSyncOutTrigger oneShotTrigger;
} TimeBaseStruct;

typedef struct{
    uint32_t base;
    EPWM_CounterCompareModule counter_cmpa_module;
    uint16_t counter_cmpa_value;
    bool EnableGlobalLoadCMPA;
    uint16_t GlobalLoadRegisterCMPA;
    bool EnableShadowLoadCMPA;
    EPWM_CounterCompareLoadMode ShadowAMode;
    bool Enable_LinkCMPA;
    EPWM_CurrentLink CMPACurrentLink;
    EPWM_CounterCompareModule counter_cmpb_module;
    uint16_t counter_cmpb_value;
    bool EnableGlobalLoadCMPB;
    uint16_t GlobalLoadRegisterCMPB;
    bool EnableShadowLoadCMPB;
    EPWM_CounterCompareLoadMode ShadowBMode;
    bool Enable_LinkCMPB;
    EPWM_CurrentLink CMPBCurrentLink;
    EPWM_CounterCompareModule counter_cmpc_module;
    uint16_t counter_cmpc_value;
    bool EnableGlobalLoadCMPC;
    uint16_t GlobalLoadRegisterCMPC;
    bool EnableShadowLoadCMPC;
    EPWM_CounterCompareLoadMode ShadowCMode;
    bool Enable_LinkCMPC;
    EPWM_CurrentLink CMPCCurrentLink;
    EPWM_CounterCompareModule counter_cmpd_module;
    uint16_t counter_cmpd_value;
    bool EnableGlobalLoadCMPD;
    uint16_t GlobalLoadRegisterCMPD;
    bool EnableShadowLoadCMPD;
    EPWM_CounterCompareLoadMode ShadowDMode;
    bool Enable_LinkCMPD;
    EPWM_CurrentLink CMPDCurrentLink;

  
} CounterCompareStruct;

typedef struct{
    uint32_t base;
    bool Enable_softwareForceGlobalLoad;
    uint16_t GlobalLoadRegisterContSWForce; 
    EPWM_ActionQualifierContForce cont_force_shadow_mode; 
    EPWM_ActionQualifierTriggerSource AQ_T1_TriggerSource; 
    EPWM_ActionQualifierTriggerSource AQ_T2_TriggerSource;
    bool Enable_ePWMXAGlobalLoad; 
    uint16_t ePWMXAGlobalLoadRegister; 
    bool Enable_ePWMXAShadowMode; 
    EPWM_ActionQualifierModule AQ_ModuleA;
    EPWM_ActionQualifierLoadMode  ePWMXAShadowLoadEvent; 
    EPWM_ActionQualifierOutput ePWMXA_OneTimeSWForceAction; 
    EPWM_ActionQualifierSWOutput ePWMXA_ContSWForceAction; 
    EPWM_ActionQualifierOutput ePWMXA_TbctrZero;
    EPWM_ActionQualifierOutput ePWMXA_TbctrPrd;
    EPWM_ActionQualifierOutput ePWMXA_TbctrCMPAUp; 
    EPWM_ActionQualifierOutput ePWMXA_TbctrCMPADown; 
    EPWM_ActionQualifierOutput ePWMXA_TbctrCMPBUp;
    EPWM_ActionQualifierOutput ePWMXA_TbctrCMPBDown; 
    EPWM_ActionQualifierOutput ePWMXA_TbctrT1Up; 
    EPWM_ActionQualifierOutput ePWMXA_TbctrT1Down; 
    EPWM_ActionQualifierOutput ePWMXA_TbctrT2Up; 
    EPWM_ActionQualifierOutput ePWMXA_TbctrT2Down;
    bool Enable_ePWMXBGlobalLoad; 
    uint16_t ePWMXBGlobalLoadRegister; 
    bool Enable_ePWMXBShadowMode; 
    EPWM_ActionQualifierModule AQ_ModuleB;
    EPWM_ActionQualifierLoadMode  ePWMXBShadowLoadEvent; 
    EPWM_ActionQualifierOutput ePWMXB_OneTimeSWForceAction; 
    EPWM_ActionQualifierSWOutput ePWMXB_ContSWForceAction; 
    EPWM_ActionQualifierOutput ePWMXB_TbctrZero;
    EPWM_ActionQualifierOutput ePWMXB_TbctrPrd;
    EPWM_ActionQualifierOutput ePWMXB_TbctrCMPAUp; 
    EPWM_ActionQualifierOutput ePWMXB_TbctrCMPADown; 
    EPWM_ActionQualifierOutput ePWMXB_TbctrCMPBUp;
    EPWM_ActionQualifierOutput ePWMXB_TbctrCMPBDown; 
    EPWM_ActionQualifierOutput ePWMXB_TbctrT1Up; 
    EPWM_ActionQualifierOutput ePWMXB_TbctrT1Down; 
    EPWM_ActionQualifierOutput ePWMXB_TbctrT2Up; 
    EPWM_ActionQualifierOutput ePWMXB_TbctrT2Down;
} ActionQualifierStruct;

typedef struct{
    uint32_t base; 
    bool advanced_epwm_tripzone_actions; 
    EPWM_TripZoneAdvancedAction TZB_D; 
    EPWM_TripZoneAdvancedAction TZB_U;
    EPWM_TripZoneAdvancedAction TZA_D; 
    EPWM_TripZoneAdvancedAction TZA_U;
    EPWM_TripZoneAdvancedAction DCAEVT1_U; 
    EPWM_TripZoneAdvancedAction DCAEVT1_D; 
    EPWM_TripZoneAdvancedAction DCAEVT2_U; 
    EPWM_TripZoneAdvancedAction DCAEVT2_D; 
    EPWM_TripZoneAdvancedAction DCBEVT1_U; 
    EPWM_TripZoneAdvancedAction DCBEVT1_D; 
    EPWM_TripZoneAdvancedAction DCBEVT2_U; 
    EPWM_TripZoneAdvancedAction DCBEVT2_D; 
    EPWM_TripZoneAction TZA_Event; 
    EPWM_TripZoneAction TZB_Event; 
    EPWM_TripZoneAction DCAEVT1_Event; 
    EPWM_TripZoneAction DCAEVT2_Event; 
    EPWM_TripZoneAction DCBEVT1_Event; 
    EPWM_TripZoneAction DCBEVT2_Event; 
    bool enable_EPWM_TZ_SIGNAL_OSHT1; 
    bool enable_EPWM_TZ_SIGNAL_OSHT2; 
    bool enable_EPWM_TZ_SIGNAL_OSHT3; 
    bool enable_EPWM_TZ_SIGNAL_OSHT4; 
    bool enable_EPWM_TZ_SIGNAL_OSHT5; 
    bool enable_EPWM_TZ_SIGNAL_OSHT6; 
    bool enable_EPWM_TZ_SIGNAL_DCAEVT1; 
    bool enable_EPWM_TZ_SIGNAL_DCBEVT1; 
    bool enable_EPWM_TZ_SIGNAL_CBC1; 
    bool enable_EPWM_TZ_SIGNAL_CBC2; 
    bool enable_EPWM_TZ_SIGNAL_CBC3; 
    bool enable_EPWM_TZ_SIGNAL_CBC4; 
    bool enable_EPWM_TZ_SIGNAL_CBC5; 
    bool enable_EPWM_TZ_SIGNAL_CBC6; 
    bool enable_EPWM_TZ_SIGNAL_DCAEVT2; 
    bool enable_EPWM_TZ_SIGNAL_DCBEVT2; 
    EPWM_CycleByCycleTripZoneClearMode CBC_Clear_Event; 
    bool CBC_interrupt; 
    bool OneShot_interrupt; 
    bool DCAEVT1_interrupt; 
    bool DCAEVT2_interrupt; 
    bool DCBEVT1_interrupt; 
    bool DCBEVT2_interrupt;
}TripZoneStruct;

typedef struct{
    uint32_t base; 
    EPWM_DigitalCompareFilterInput DCFilter_InputEventSource; 
    bool EnableBlankingWindow; 
    EPWM_DigitalCompareBlankingPulse BWStartEvent; 
    bool Enable_MixedEvent_TBCTRZero; 
    bool Enable_MixedEvent_TBCTRPeriod; 
    bool Enable_MixedEvent_TBCTRCMPAIncreasing; 
    bool Enable_MixedEvent_TBCTRCMPADecreasing; 
    bool Enable_MixedEvent_TBCTRCMPBIncreasing; 
    bool Enable_MixedEvent_TBCTRCMPBDecreasing; 
    bool Enable_MixedEvent_TBCTRCMPCIncreasing; 
    bool Enable_MixedEvent_TBCTRCMPCDecreasing; 
    bool Enable_MixedEvent_TBCTRCMPDIncreasing; 
    bool Enable_MixedEvent_TBCTRCMPDDecreasing; 
    uint16_t BW_Offset; 
    uint16_t BW_Length; 
    bool Enable_InvertBW; 
    bool Enable_DCCounterCapture; 
    bool Enable_ShadowMode; 
    bool Enable_EdgeFilter; 
    EPWM_DigitalCompareEdgeFilterMode edgeMode; 
    EPWM_DigitalCompareEdgeFilterEdgeCount edgeCount; 
    bool Enable_EdgeFilterValleyCapture; 
    EPWM_ValleyTriggerSource trigger; 
    uint16_t startCount; 
    uint16_t stopCount; 
    bool selectDelayedOutputEdgeFilter; 
    uint16_t DelayOffset; 
    EPWM_ValleyDelayMode delayMode; 
    EPWM_DigitalCompareTripInput TripInputDCAH; 
    EPWM_DigitalCompareTripInput TripInputDCAL; 
    EPWM_TripZoneDigitalCompareOutputEvent Compare1A_Condition; 
    EPWM_TripZoneDigitalCompareOutputEvent Compare2A_Condition; 
    bool Enable_ADCSOCDCAEVT1; 
    bool Enable_SYNCOUTDCAEVT1; 
    EPWM_DigitalCompareSyncMode SynchModeDCAEVT1; 
    EPWM_DigitalCompareEventSource SignalSourceDCAEVT1; 
    EPWM_DigitalCompareCBCLatchMode CBCLatchModeDCAEVT1; 
    EPWM_DigitalCompareCBCLatchClearEvent CBCLatchClearDCAEVT1; 
    EPWM_DigitalCompareSyncMode SynchModeDCAEVT2; 
    EPWM_DigitalCompareEventSource SignalSourceDCAEVT2; 
    EPWM_DigitalCompareCBCLatchMode CBCLatchModeDCAEVT2; 
    EPWM_DigitalCompareCBCLatchClearEvent CBCLatchClearDCAEVT2; 
    EPWM_DigitalCompareTripInput TripInputDCBH; 
    EPWM_DigitalCompareTripInput TripInputDCBL; 
    EPWM_TripZoneDigitalCompareOutputEvent Compare1B_Condition; 
    EPWM_TripZoneDigitalCompareOutputEvent Compare2B_Condition; 
    bool Enable_ADCSOCDCBEVT1; 
    bool Enable_SYNCOUTDCBEVT1; 
    EPWM_DigitalCompareSyncMode SynchModeDCBEVT1; 
    EPWM_DigitalCompareEventSource SignalSourceDCBEVT1; 
    EPWM_DigitalCompareCBCLatchMode CBCLatchModeDCBEVT1; 
    EPWM_DigitalCompareCBCLatchClearEvent CBCLatchClearDCBEVT1; 
    EPWM_DigitalCompareSyncMode SynchModeDCBEVT2; 
    EPWM_DigitalCompareEventSource SignalSourceDCBEVT2; 
    EPWM_DigitalCompareCBCLatchMode CBCLatchModeDCBEVT2; 
    EPWM_DigitalCompareCBCLatchClearEvent CBCLatchClearDCBEVT2;
    bool Enable_Edge_Detection;
    EPWM_DigitalCompareTripInput Capture_Input;
    uint8_t Invert_Capture_Input_Polarity;
    EPWM_DigitalCompareTripInput Capture_Gate;
    uint8_t Capture_Gate_Polarity_Sel;
    bool Capture_Input_Trip1;
    bool Capture_Input_Trip2;
    bool Capture_Input_Trip3;
    bool Capture_Input_Trip4;
    bool Capture_Input_Trip5;
    bool Capture_Input_Trip6;
    bool Capture_Input_Trip7;
    bool Capture_Input_Trip8;
    bool Capture_Input_Trip9;
    bool Capture_Input_Trip10;
    bool Capture_Input_Trip11;
    bool Capture_Input_Trip12;
    bool Capture_Input_Trip13;
    bool Capture_Input_Trip14;
    bool Capture_Input_Trip15;
    bool Capture_Gate_Trip1;
    bool Capture_Gate_Trip2;
    bool Capture_Gate_Trip3;
    bool Capture_Gate_Trip4;
    bool Capture_Gate_Trip5;
    bool Capture_Gate_Trip6;
    bool Capture_Gate_Trip7;
    bool Capture_Gate_Trip8;
    bool Capture_Gate_Trip9;
    bool Capture_Gate_Trip10;
    bool Capture_Gate_Trip11;
    bool Capture_Gate_Trip12;
    bool Capture_Gate_Trip13;
    bool Capture_Gate_Trip14;
    bool Capture_Gate_Trip15;
    bool DCAHighCombTrip1;
    bool DCAHighCombTrip2;
    bool DCAHighCombTrip3;
    bool DCAHighCombTrip4;
    bool DCAHighCombTrip5;
    bool DCAHighCombTrip6;
    bool DCAHighCombTrip7;
    bool DCAHighCombTrip8;
    bool DCAHighCombTrip9;
    bool DCAHighCombTrip10;
    bool DCAHighCombTrip11;
    bool DCAHighCombTrip12;
    bool DCAHighCombTrip13;
    bool DCAHighCombTrip14;
    bool DCAHighCombTrip15;
    bool DCALowCombTrip1;
    bool DCALowCombTrip2;
    bool DCALowCombTrip3;
    bool DCALowCombTrip4;
    bool DCALowCombTrip5;
    bool DCALowCombTrip6;
    bool DCALowCombTrip7;
    bool DCALowCombTrip8;
    bool DCALowCombTrip9;
    bool DCALowCombTrip10;
    bool DCALowCombTrip11;
    bool DCALowCombTrip12;
    bool DCALowCombTrip13;
    bool DCALowCombTrip14;
    bool DCALowCombTrip15;
    bool DCBHighCombTrip1;
    bool DCBHighCombTrip2;
    bool DCBHighCombTrip3;
    bool DCBHighCombTrip4;
    bool DCBHighCombTrip5;
    bool DCBHighCombTrip6;
    bool DCBHighCombTrip7;
    bool DCBHighCombTrip8;
    bool DCBHighCombTrip9;
    bool DCBHighCombTrip10;
    bool DCBHighCombTrip11;
    bool DCBHighCombTrip12;
    bool DCBHighCombTrip13;
    bool DCBHighCombTrip14;
    bool DCBHighCombTrip15;
    bool DCBLowCombTrip1;
    bool DCBLowCombTrip2;
    bool DCBLowCombTrip3;
    bool DCBLowCombTrip4;
    bool DCBLowCombTrip5;
    bool DCBLowCombTrip6;
    bool DCBLowCombTrip7;
    bool DCBLowCombTrip8;
    bool DCBLowCombTrip9;
    bool DCBLowCombTrip10;
    bool DCBLowCombTrip11;
    bool DCBLowCombTrip12;
    bool DCBLowCombTrip13;
    bool DCBLowCombTrip14;
    bool DCBLowCombTrip15;


}DigitalCompareStruct;

typedef struct{
    uint32_t base;
    uint16_t RisingEdgeDelayInput; 
    uint16_t FallingEdgeDelayInput; 
    EPWM_DeadBandPolarity RED_DeadBandPolarity; 
    EPWM_DeadBandPolarity FED_DeadBandPolarity; 
    bool Enable_RisingEdgeDelay; 
    uint16_t Rising_Edge_Delay_Value; 
    bool Enable_FallingEdgeDelay; 
    uint16_t Falling_Edge_Delay_Value; 
    bool Swap_Output_ePWMXA; 
    bool Swap_Output_ePWMXB; 
    bool Enable_DeadbandControlGlobalLoad; 
    uint16_t DeadbandGlobalLoadRegister; 
    bool Enable_DeadbandControlShadowMode; 
    EPWM_DeadBandControlLoadMode DeadbandLoadMode; 
    bool Enable_REDGlobalLoad; 
    uint16_t REDGlobalLoadRegister; 
    bool Enable_REDShadowMode; 
    EPWM_RisingEdgeDelayLoadMode RisingEdgeLoadMode; 
    bool Enable_FEDGlobalLoad; 
    uint16_t FEDGlobalLoadRegister; 
    bool Enable_FEDShadowMode; 
    EPWM_FallingEdgeDelayLoadMode FallingEdgeLoadMode; 
    EPWM_DeadBandClockMode DeadBandClockRate;
    
    
}DeadBandStruct;

typedef struct{
    uint32_t base; 
    bool UseChopper; 
    uint16_t DutyCycle; 
    uint16_t freqDiv; 
    uint16_t FirstPulseWidth;
}ChopperStruct;

typedef struct{
    uint32_t base; 
    bool Enable_EPWMInterrupt;
    bool Enable_RisingEdgeDelay;  
    uint16_t InterruptEventSource; 
    bool Enable_EventMixedSourceTbctrZero; 
    bool Enable_EventMixedSourceTbctrPeriod; 
    bool Enable_EventMixedSourceTbctrCMPAInc; 
    bool Enable_EventMixedSourceTbctrCMPADec; 
    bool Enable_EventMixedSourceTbctrCMPBInc; 
    bool Enable_EventMixedSourceTbctrCMPBDec; 
    bool Enable_EventMixedSourceTbctrCMPCInc; 
    bool Enable_EventMixedSourceTbctrCMPCDec; 
    bool Enable_EventMixedSourceTbctrCMPDInc; 
    bool Enable_EventMixedSourceTbctrCMPDDec; 
    bool Enable_EventMixedSourceDCxEVT1; 
    uint16_t eventCount; 
    bool Enable_InterruptEventCountInitialValue; 
    uint16_t EventCountInitValue; 
    bool SOCATriggerEnable; 
    EPWM_ADCStartOfConversionSource SOCASource; 
    bool Enable_SOCAMixedSourceTbctrZero; 
    bool Enable_SOCAMixedSourceTbctrPeriod; 
    bool Enable_SOCAMixedSourceTbctrCMPAInc; 
    bool Enable_SOCAMixedSourceTbctrCMPADec; 
    bool Enable_SOCAMixedSourceTbctrCMPBInc; 
    bool Enable_SOCAMixedSourceTbctrCMPBDec; 
    bool Enable_SOCAMixedSourceTbctrCMPCInc; 
    bool Enable_SOCAMixedSourceTbctrCMPCDec; 
    bool Enable_SOCAMixedSourceTbctrCMPDInc; 
    bool Enable_SOCAMixedSourceTbctrCMPDDec; 
    bool Enable_SOCAMixedSourceDCxEVT1; 
    uint16_t SOCA_TriggerEventCount; 
    bool Enable_SOCATriggerEventCountInit; 
    uint16_t SOCATriggerInitValue; 
    bool SOCA_ForceInterruptEvent; 
    bool SOCBTriggerEnable; 
    EPWM_ADCStartOfConversionSource SOCBSource; 
    bool Enable_SOCBMixedSourceTbctrZero; 
    bool Enable_SOCBMixedSourceTbctrPeriod; 
    bool Enable_SOCBMixedSourceTbctrCMPAInc; 
    bool Enable_SOCBMixedSourceTbctrCMPADec; 
    bool Enable_SOCBMixedSourceTbctrCMPBInc; 
    bool Enable_SOCBMixedSourceTbctrCMPBDec; 
    bool Enable_SOCBMixedSourceTbctrCMPCInc; 
    bool Enable_SOCBMixedSourceTbctrCMPCDec; 
    bool Enable_SOCBMixedSourceTbctrCMPDInc; 
    bool Enable_SOCBMixedSourceTbctrCMPDDec; 
    bool Enable_SOCBMixedSourceDCxEVT1; 
    uint16_t SOCB_TriggerEventCount; 
    bool Enable_SOCBTriggerEventCountInit; 
    uint16_t SOCBTriggerInitValue; 
    bool SOCB_ForceInterruptEvent;
}EventTriggerStruct;

typedef struct{
    uint32_t base; 
    bool Enable_HRPWMSupport; 
    bool Enable_AutomaticHRMSTEPScaling; 
    bool Enable_SFOLibrary;
    HRPWM_MEPCtrlMode ControlModeChannelA; 
    HRPWM_MEPCtrlMode ControlModeChannelB; 
    uint16_t hrPhaseCount; 
    HRPWM_SyncPulseSource HRPWMSyncSource; 
    bool HighResolutionPhaseEnable; 
    HRPWM_MEPEdgeMode ChannelAEdgeMode; 
    uint16_t HighResolutionCMPA; 
    HRPWM_LoadMode CMPAHRLoadEvent; 
    HRPWM_MEPEdgeMode ChannelBEdgeMode; 
    uint16_t HighResolutionCMPB; 
    HRPWM_LoadMode CMPBHRLoadEvent; 
    bool Enable_HRPWMHighResolutionPeriodLoad; 
    uint16_t hrPeriodCount; 
    HRPWM_MEPDeadBandEdgeMode DeadbandEdgeMode; 
    uint16_t hrREDDelay; 
    HRPWM_LoadMode REDLoadMode; 
    bool Swap_AB_Outputs; 
    HRPWM_ChannelBOutput EPWMxBOutputPath; 
    uint16_t hrFEDDelay; 
    HRPWM_LoadMode FEDLoadMode;
    uint16_t MEP_SCALE;
}HRPWMStruct;

typedef struct{
    uint32_t base;
    uint32_t instance;
    bool enable_XCMP;
    bool enable_Split;
    EPWM_XCMP_ALLOC_CMPA xcmp_cmpa;
    EPWM_XCMP_ALLOC_CMPB xcmp_cmpb;
    EPWM_XCMPXloadCtlLoadMode load_mode;
    EPWM_XCMP_XLOADCTL_SHDWLEVEL shadow_level;
    EPWM_XCMP_XLOADCTL_SHDWBUFPTR shadow_register_load_once;
    uint8_t rpt_cnt_shdw_buffer2;
    uint8_t rpt_cnt_shdw_buffer3;
    uint16_t xcmp1_active;
    uint16_t xcmp2_active;
    uint16_t xcmp3_active;
    uint16_t xcmp4_active;
    uint16_t xcmp5_active;
    uint16_t xcmp6_active;
    uint16_t xcmp7_active;
    uint16_t xcmp8_active;
    uint16_t xcmp1_shadow1;
    uint16_t xcmp2_shadow1;
    uint16_t xcmp3_shadow1;
    uint16_t xcmp4_shadow1;
    uint16_t xcmp5_shadow1;
    uint16_t xcmp6_shadow1;
    uint16_t xcmp7_shadow1;
    uint16_t xcmp8_shadow1;
    uint16_t xcmp1_shadow2;
    uint16_t xcmp2_shadow2;
    uint16_t xcmp3_shadow2;
    uint16_t xcmp4_shadow2;
    uint16_t xcmp5_shadow2;
    uint16_t xcmp6_shadow2;
    uint16_t xcmp7_shadow2;
    uint16_t xcmp8_shadow2;
    uint16_t xcmp1_shadow3;
    uint16_t xcmp2_shadow3;
    uint16_t xcmp3_shadow3;
    uint16_t xcmp4_shadow3;
    uint16_t xcmp5_shadow3;
    uint16_t xcmp6_shadow3;
    uint16_t xcmp7_shadow3;
    uint16_t xcmp8_shadow3;
    uint16_t xtbprd_active;
    uint16_t xtbprd_shadow1;
    uint16_t xtbprd_shadow2;
    uint16_t xtbprd_shadow3;
    uint16_t xmin_active;
    uint16_t xmax_active;
    uint16_t xmin_shadow1;
    uint16_t xmax_shadow1;
    uint16_t xmin_shadow2;
    uint16_t xmax_shadow2;
    uint16_t xmin_shadow3;
    uint16_t xmax_shadow3;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp1_active;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp2_active;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp3_active;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp4_active;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp5_active;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp6_active;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp7_active;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp8_active;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp5_active;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp6_active;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp7_active;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp8_active;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp1_shadow1;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp2_shadow1;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp3_shadow1;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp4_shadow1;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp5_shadow1;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp6_shadow1;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp7_shadow1;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp8_shadow1;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp5_shadow1;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp6_shadow1;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp7_shadow1;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp8_shadow1;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp1_shadow2;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp2_shadow2;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp3_shadow2;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp4_shadow2;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp5_shadow2;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp6_shadow2;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp7_shadow2;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp8_shadow2;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp5_shadow2;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp6_shadow2;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp7_shadow2;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp8_shadow2;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp1_shadow3;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp2_shadow3;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp3_shadow3;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp4_shadow3;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp5_shadow3;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp6_shadow3;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp7_shadow3;
    EPWM_ActionQualifierOutput epwmxa_tbctr_xcmp8_shadow3;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp5_shadow3;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp6_shadow3;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp7_shadow3;
    EPWM_ActionQualifierOutput epwmxb_tbctr_xcmp8_shadow3;

}XCMPStruct;

typedef struct{

    uint32_t base;
    EPWM_DiodeEmulationMode de_mode;
    uint8_t delay;
    EPWM_DiodeEmulationTripSource decompsel_tripl;
    EPWM_DiodeEmulationTripSource decompsel_triph;
    uint32_t PWMA_Signal_DE_Mode;
    EPWM_DiodeEmulationSignal Trip_ChannelA;
    uint32_t PWMB_Signal_DE_Mode;
    EPWM_DiodeEmulationSignal Trip_ChannelB;
    uint16_t de_monitor_threshold;
    uint8_t dec_step_size;
    uint8_t inc_step_size;
    bool Enable_DE_Mode;
    bool bypass_de_pwm;
    bool Force_DEACTIVE;
    bool Enable_Trip_Monitor;

}DiodeEmulation_Struct;

typedef struct{

    uint32_t base;
    bool enable_MDLA;
    uint32_t Ref_signal_MDLA;
    uint32_t Invert_RefSignal_BlockA;
    uint32_t Delay_PWMA;
    uint32_t Blocking_signal_PWMA;
    uint32_t Blocking_signal_AND_OR_PWMA;
    bool Enable_MDLB;
    uint32_t Ref_signal_MDLB;
    uint32_t Invert_Ref_signal_BlockB;
    uint32_t Delay_PWMB;
    uint32_t Blocking_signal_PWMB;
    uint32_t Blocking_signal_AND_OR_PWMB;

}MinimumDeadband_Struct;

typedef struct{

    uint32_t base; 
    bool enable_iclA;
    uint32_t IN3_BlockA;
    uint32_t BlockA000;
    uint32_t BlockA001;
    uint32_t BlockA010;
    uint32_t BlockA011;
    uint32_t BlockA100;
    uint32_t BlockA101;
    uint32_t BlockA110;
    uint32_t BlockA111;
    bool enable_iclB;
    uint32_t IN3_blockB;
    uint32_t BlockB000;
    uint32_t BlockB001;
    uint32_t BlockB010;
    uint32_t BlockB011;
    uint32_t BlockB100;
    uint32_t BlockB101;
    uint32_t BlockB110;
    uint32_t BlockB111;

}IllegalComboLogic_Struct;

typedef struct{

    uint32_t base; 
    uint32_t instance;
    bool EnableGlobalShadowtoActiveLoad;
    EPWM_GlobalLoadTrigger GlobalLoadPulseSelection;
    uint16_t GlobalLoadStrobePeriod;
    bool EnableOneShot;
    bool EnableReloadOneShotMode;
    bool EnableForceLoadEvent;
    EPWM_CurrentLink CurrentLink;

}GlobalLoad_Struct;

typedef struct{
    uint32_t base;  
    EPWM_TimeBaseCountMode counterMode; 
    bool phaseShiftLoadEnable; 
    uint16_t phaseCount; 
    uint16_t HighResolutionCMPA; 
    uint16_t HighResolutionCMPB; 
    bool Enable_HRPWMSupport;
    HRPWM_MEPEdgeMode ChannelAEdgeMode;
    HRPWM_MEPEdgeMode ChannelBEdgeMode;
    HRPWM_MEPCtrlMode ControlModeChannelA; 
    HRPWM_MEPCtrlMode ControlModeChannelB; 
    uint16_t hrPhaseCount; 
    bool HighResolutionPhaseEnable; 
    bool Enable_HRPWMHighResolutionPeriodLoad; 
    uint16_t hrPeriodCount; 
    bool Enable_AutomaticHRMSTEPScaling;
    bool Enable_SFOLibrary;
    bool Enable_XCMP;
    uint16_t shadow_level;
    
}EPWMStepStruct;

typedef struct{
    uint32_t base;
}EPWMReleaseStruct;

extern void
epwm_timebase_setup(TimeBaseStruct *ptr);
extern void
epwm_counter_compare_setup(CounterCompareStruct *ptr);
extern void
epwm_action_qualifier_setup(ActionQualifierStruct *ptr);
extern void
epwm_trip_zone_setup(TripZoneStruct *ptr);
extern void
epwm_digital_compare_setup(DigitalCompareStruct *ptr);
extern void
epwm_deadband_setup(DeadBandStruct *ptr);
extern void
epwm_chopper_setup(ChopperStruct *ptr);
extern void
epwm_event_trigger_setup(EventTriggerStruct *ptr); 
extern void
epwm_hrpwm_setup(HRPWMStruct *ptr);
extern void
epwm_xcmp_setup(XCMPStruct *ptr);
extern void
epwm_diode_emulation_setup(DiodeEmulation_Struct *ptr);
extern void
epwm_minimum_deadband_setup(MinimumDeadband_Struct *ptr);
extern void
epwm_illegal_combo_logic_setup(IllegalComboLogic_Struct *ptr);
extern void
epwm_global_load_setup(GlobalLoad_Struct *ptr);
extern void
epwm_step(EPWMStepStruct *ptr);
extern void
epwm_release(EPWMReleaseStruct *ptr);

#endif