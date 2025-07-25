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
#ifndef SDFM_MW_H_
#define SDFM_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/adc.h>
#include <drivers/sdfm.h>
#include <kernel/dpl/DebugP.h>
#include <drivers/soc.h>
#include <kernel/dpl/ClockP.h>

#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif

#define SDFM_HTLZ_ENABLE  0x8000 //High Threshold (Z) enabled
#define SDFM_HTLZ_DISABLE 0x0000 //High Threshold (Z) disabled

typedef struct{
    uint32_t base;
    uint16_t filter_channel;
    uint32_t filter1_ch1_sdclk_src;
    uint32_t filter1_sd_modulator_mode;
    bool filter1_enable_data_filter;
    uint16_t filter1_filtertype;
    uint16_t filter1_dosr;
    uint16_t filter1_data_filter_output_rep;
    uint16_t filter1_shift;
    bool filter1_pwm_synchronization;
    uint32_t filter1_src_sdsync_event;
    bool filter1_clear_wait_sync_flag_manually;
    bool filter1_use_fifo;
    uint16_t filter1_fifo_lvl;
    bool filter1_clearfifo_sdsync;
    bool filter1_enable_comparator;
    uint16_t filter1_comparator_filter_type;
    uint16_t filter1_comparator_osr;
    uint32_t filter1_comparator_highlvl_threshold1;
    uint32_t filter1_comparator_highlvl_threshold2;
    uint32_t filter1_comparator_lowlvl_threshold1;
    uint32_t filter1_comparator_lowlvl_threshold2;
    bool filter1_enable_highthresholdz;
    uint16_t filter1_highthresholdz;
    uint32_t filter1_comparator_event1_src;
    bool filter1_use_cevt1;
    bool filter1_initialize_cevt1;
    uint16_t filter1_cevt1_prescale;
    uint16_t filter1_cevt1_threshold;
    uint16_t filter1_cevt1_samplewindow;
    uint32_t filter1_comparator_event2_src;
    bool filter1_use_cevt2;
    bool filter1_initialize_cevt2;
    uint16_t filter1_cevt2_prescale;
    uint16_t filter1_cevt2_threshold;
    uint16_t filter1_cevt2_samplewindow;
    bool use_filter_channel2;
    uint32_t filter2_ch2_sdclk_src;
    uint32_t filter2_sd_modulator_mode;
    bool filter2_enable_data_filter;
    uint16_t filter2_filtertype;
    uint16_t filter2_dosr;
    uint16_t filter2_data_filter_output_rep;
    uint16_t filter2_shift;
    bool filter2_pwm_synchronization;
    uint32_t filter2_src_sdsync_event;
    bool filter2_clear_wait_sync_flag_manually;
    bool filter2_use_fifo;
    uint16_t filter2_fifo_lvl;
    bool filter2_clearfifo_sdsync;
    bool filter2_enable_comparator;
    uint16_t filter2_comparator_filter_type;
    uint16_t filter2_comparator_osr;
    uint32_t filter2_comparator_highlvl_threshold1;
    uint32_t filter2_comparator_highlvl_threshold2;
    uint32_t filter2_comparator_lowlvl_threshold1;
    uint32_t filter2_comparator_lowlvl_threshold2;
    bool filter2_enable_highthresholdz;
    uint16_t filter2_highthresholdz;
    uint32_t filter2_comparator_event1_src;
    bool filter2_use_cevt1;
    bool filter2_initialize_cevt1;
    uint16_t filter2_cevt1_prescale;
    uint16_t filter2_cevt1_threshold;
    uint16_t filter2_cevt1_samplewindow;
    uint32_t filter2_comparator_event2_src;
    bool filter2_use_cevt2;
    bool filter2_initialize_cevt2;
    uint16_t filter2_cevt2_prescale;
    uint16_t filter2_cevt2_threshold;
    uint16_t filter2_cevt2_samplewindow;
    bool use_filter_channel3;
    uint32_t filter3_ch3_sdclk_src;
    uint32_t filter3_sd_modulator_mode;
    bool filter3_enable_data_filter;
    uint16_t filter3_filtertype;
    uint16_t filter3_dosr;
    uint16_t filter3_data_filter_output_rep;
    uint16_t filter3_shift;
    bool filter3_pwm_synchronization;
    uint32_t filter3_src_sdsync_event;
    bool filter3_clear_wait_sync_flag_manually;
    bool filter3_use_fifo;
    uint16_t filter3_fifo_lvl;
    bool filter3_clearfifo_sdsync;
    bool filter3_enable_comparator;
    uint16_t filter3_comparator_filter_type;
    uint16_t filter3_comparator_osr;
    uint32_t filter3_comparator_highlvl_threshold1;
    uint32_t filter3_comparator_highlvl_threshold2;
    uint32_t filter3_comparator_lowlvl_threshold1;
    uint32_t filter3_comparator_lowlvl_threshold2;
    bool filter3_enable_highthresholdz;
    uint16_t filter3_highthresholdz;
    uint32_t filter3_comparator_event1_src;
    bool filter3_use_cevt1;
    bool filter3_initialize_cevt1;
    uint16_t filter3_cevt1_prescale;
    uint16_t filter3_cevt1_threshold;
    uint16_t filter3_cevt1_samplewindow;
    uint32_t filter3_comparator_event2_src;
    bool filter3_use_cevt2;
    bool filter3_initialize_cevt2;
    uint16_t filter3_cevt2_prescale;
    uint16_t filter3_cevt2_threshold;
    uint16_t filter3_cevt2_samplewindow;
    bool use_filter_channel4;
    uint32_t filter4_ch4_sdclk_src;
    uint32_t filter4_sd_modulator_mode;
    bool filter4_enable_data_filter;
    uint16_t filter4_filtertype;
    uint16_t filter4_dosr;
    uint16_t filter4_data_filter_output_rep;
    uint16_t filter4_shift;
    bool filter4_pwm_synchronization;
    uint32_t filter4_src_sdsync_event;
    bool filter4_clear_wait_sync_flag_manually;
    bool filter4_use_fifo;
    uint16_t filter4_fifo_lvl;
    bool filter4_clearfifo_sdsync;
    bool filter4_enable_comparator;
    uint16_t filter4_comparator_filter_type;
    uint16_t filter4_comparator_osr;
    uint32_t filter4_comparator_highlvl_threshold1;
    uint32_t filter4_comparator_highlvl_threshold2;
    uint32_t filter4_comparator_lowlvl_threshold1;
    uint32_t filter4_comparator_lowlvl_threshold2;
    bool filter4_enable_highthresholdz;
    uint16_t filter4_highthresholdz;
    uint32_t filter4_comparator_event1_src;
    bool filter4_use_cevt1;
    bool filter4_initialize_cevt1;
    uint16_t filter4_cevt1_prescale;
    uint16_t filter4_cevt1_threshold;
    uint16_t filter4_cevt1_samplewindow;
    uint32_t filter4_comparator_event2_src;
    bool filter4_use_cevt2;
    bool filter4_initialize_cevt2;
    uint16_t filter4_cevt2_prescale;
    uint16_t filter4_cevt2_threshold;
    uint16_t filter4_cevt2_samplewindow;
    bool use_interrupts;
    bool filter1_modulator_clk_failure_int;
    bool filter2_modulator_clk_failure_int;
    bool filter3_modulator_clk_failure_int;
    bool filter4_modulator_clk_failure_int;
    bool filter1_comparator_event1_int;
    bool filter2_comparator_event1_int;
    bool filter3_comparator_event1_int;
    bool filter4_comparator_event1_int;
    bool filter1_comparator_event2_int;
    bool filter2_comparator_event2_int;
    bool filter3_comparator_event2_int;
    bool filter4_comparator_event2_int;
    bool filter1_fifo_overflow_int;
    bool filter2_fifo_overflow_int;
    bool filter3_fifo_overflow_int;
    bool filter4_fifo_overflow_int;
    bool filter1_data_ack_int;
    bool filter2_data_ack_int;
    bool filter3_data_ack_int;
    bool filter4_data_ack_int;
    bool filter1_sdfm_fifo_int;
    bool filter2_sdfm_fifo_int;
    bool filter3_sdfm_fifo_int;
    bool filter4_sdfm_fifo_int;
    uint32_t data_representation;
    bool filt1_enable_externalreset;
    bool filt2_enable_externalreset;
    bool filt3_enable_externalreset;
    bool filt4_enable_externalreset;
} SDFMSetupStruct;

typedef struct{
     uint32_t base;
     uint16_t filter_channel;
     uint16_t filter1_data_filter_output_rep;
     uint16_t filter2_data_filter_output_rep;
     uint16_t filter3_data_filter_output_rep;
     uint16_t filter4_data_filter_output_rep;
     bool filter1_use_fifo;
     bool filter2_use_fifo;
     bool filter3_use_fifo;
     bool filter4_use_fifo;
     uint16_t filter1_fifo_lvl;
     uint16_t filter2_fifo_lvl;
     uint16_t filter3_fifo_lvl;
     uint16_t filter4_fifo_lvl;
     bool enable_comparator;
     bool filt1_newdata_int;
     bool filt2_newdata_int;
     bool filt3_newdata_int;
     bool filt4_newdata_int;
     bool filt1_fifo_int;
     bool filt2_fifo_int;
     bool filt3_fifo_int;
     bool filt4_fifo_int;


} SDFMStepStruct;

extern void SDFM_setup(SDFMSetupStruct* ptr);
extern void SDFM_step(SDFMStepStruct* ptr, const void *DataFilterStatus, const void *data, const void *outputLen, const void *compStatus, const void *compOutput);

#endif