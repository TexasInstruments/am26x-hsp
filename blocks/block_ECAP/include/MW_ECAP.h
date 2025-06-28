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
#ifndef ECAP_MW_H_
#define ECAP_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/ecap.h>
#include <drivers/soc.h>
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
    ECAP_CaptureMode capture_mode;
    ECAP_Events ecap_events;
    uint16_t ECAP_Mode;
    uint16_t preScalerValue;
    ECAP_EventPolarity Event1_Polarity;
    ECAP_EventPolarity Event2_Polarity;
    ECAP_EventPolarity Event3_Polarity;
    ECAP_EventPolarity Event4_Polarity;
    bool Enable_CounterReset_Event1;
    bool Enable_CounterReset_Event2;
    bool Enable_CounterReset_Event3;
    bool Enable_CounterReset_Event4;
    uint32_t phaseValue;
    bool enable_LoadCounter;
    bool LoadCounter;
    ECAP_SyncOutMode sync_outmode;
    ECAP_InputCaptureSignals inputCaptureSignals;
    ECAP_QualPeriodSelect pulse_width_filter_select;
    bool reset_counters;
    bool rearm_ecap;
    uint32_t apwmPeriod;
    uint32_t apwmCompare;
    ECAP_APWMPolarity apwm_polarity;
    ECAP_EmulationMode emulation_mode;
    ECAP_SyncInPulseSource syncInPulseSrc;
    bool UseInterrupt;
    bool EnableInterrupt;
    bool Enable_Event1_ISR;
    bool Enable_Event2_ISR;
    bool Enable_Event3_ISR;
    bool Enable_Event4_ISR;
    bool Counter_Overflow_ISR;
    bool Counter_Period_ISR;
    bool Counter_Compare_ISR;
    ECAP_MunitTripInputSelect trip_signal;
    ECAP_MunitGlobalStrobeSelect global_strobe;
    bool Enable_signal_MUNIT1;
    ECAP_MonitoringTypeSelect Monitoring_type_MUNIT1;
    uint32_t Monitoring_minval_MUNIT1;
    uint32_t Monitoring_maxval_MUNIT1;
    bool Enable_syncin_MUNIT1;
    bool Force_copy_from_shadow_MUNIT1;
    uint32_t SHDW_Load_Mode_MUNIT1;
    uint32_t SHDW_min_val_MUNIT1;
    uint32_t SHDW_max_val_MUNIT1;
    bool Enable_Debug_MUNIT1;
    bool Enable_signal_MUNIT2;
    ECAP_MonitoringTypeSelect Monitoring_type_MUNIT2;
    uint32_t Monitoring_minval_MUNIT2;
    uint32_t Monitoring_maxval_MUNIT2;
    bool Enable_syncin_MUNIT2;
    bool Force_copy_from_shadow_MUNIT2;
    uint32_t SHDW_Load_Mode_MUNIT2;
    uint32_t SHDW_min_val_MUNIT2;
    uint32_t SHDW_max_val_MUNIT2;
    bool Enable_Debug_MUNIT2;
}ECAPSetupStruct;

typedef struct{

    uint32_t base;
    uint16_t ECAP_Mode;
    ECAP_CaptureMode capture_mode;
    bool UseInterrupt;
    bool EnableInterrupt;
    bool Enable_Event1_ISR;
    bool Enable_Event2_ISR;
    bool Enable_Event3_ISR;
    bool Enable_Event4_ISR;
    bool Counter_Overflow_ISR;
    bool Counter_Period_ISR;
    bool Counter_Compare_ISR;
    bool rearm_ecap;
    ECAP_Events ecap_events;

}ECAPStepStruct;
    
typedef struct{
    uint32_t base;
}ECAPReleaseStruct;

extern void
ECAP_setup(ECAPSetupStruct *ptr);
extern void
ECAP_step(ECAPStepStruct *ptr, const void* dataOutput, const void* Capture_Flags, const void* Overflow_Flags);
extern void
ECAP_release(ECAPReleaseStruct *ptr);
#endif