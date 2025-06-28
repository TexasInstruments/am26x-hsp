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
#ifndef CMPSS_MW_H_
#define CMPSS_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/cmpss.h>
#include <kernel/dpl/ClockP.h>

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
    uint16_t neg_inp_src_high_comp;
    uint16_t pos_inp_src_low_comp;
    bool output_inverted_high_comp;
    bool asynch_high_comp;
    bool output_inverted_low_comp;
    bool asynch_low_comp;
    uint16_t DAC_Load;
    uint16_t DAC_Ref_voltage;
    uint16_t DAC_value_src;
    uint16_t High_comp_digfilt_sample_prescale;
    uint16_t High_comp_digfilt_sample_window;
    uint16_t High_comp_digfilt_threshold;
    uint16_t Low_comp_digfilt_sample_prescale;
    uint16_t Low_comp_digfilt_sample_window;
    uint16_t Low_comp_digfilt_threshold;
    uint16_t signal_ctripouth_high_comp;
    uint16_t signal_ctriph_high_comp;
    uint16_t signal_ctripoutl_low_comp;
    uint16_t signal_ctripl_low_comp;
    bool initialize_dig_high_filter;
    bool initialize_dig_low_filter;
    bool clear_filt_latch_high_filt;
    bool clear_filter_latch_low_filter;
    uint16_t hysterisis_high_comp;
    uint16_t hysterisis_low_comp;
    uint16_t max_ramp_gen_ref_value;
    uint16_t ramp_gen_dec_value;
    uint16_t ramp_gen_delay_value;
    uint16_t pwmSync;
    uint16_t ramp_gen_reset_value;
    uint16_t pwm_blanking;
    bool enable_blanking;
    bool enable_diode_emulation;
    uint8_t cmpss_deactive_signal;
    uint16_t high_comp_dac_value2;
    uint16_t low_comp_dac_value2;
    bool enable_module;
    bool reset_dig_filter_latch_high_comp;
    bool reset_dig_filter_latch_low_comp;
    uint8_t specify_comph_dacramp;
    uint8_t specify_compl_dac;
    uint16_t High_comp_DAC_value;
    uint16_t Low_comp_DAC_value;
    uint16_t High_comp_DAC_value_init;
    uint16_t Low_comp_DAC_value_init;
    uint16_t max_ramp_gen_ref_value_init;
    uint16_t ramp_gen_dec_value_init;
    uint16_t ramp_gen_delay_value_init;
    uint8_t DAC_Src;
} CMPSSSetupStruct;

typedef struct{
    uint32_t base;
    bool Enable_high_comparator;
    bool Enable_low_comparator;
} CMPSSStepStruct;

extern void CMPSS_setup(CMPSSSetupStruct *ptr);
extern void CMPSS_step(CMPSSStepStruct *ptr, const void *sts, const void *sts_latch);
#endif