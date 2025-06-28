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
#include <stdint.h>
#include <kernel/dpl/HwiP.h>
#include <drivers/soc.h>
#include <drivers/epwm.h>
#include <drivers/adc.h>
#include <drivers/ecap.h>
#include <drivers/gpio.h>
#include <drivers/mcan.h>
#include <drivers/sdfm.h>
#include <drivers/eqep.h>
#include <drivers/uart/v0/lld/uart_lld.h>
#include <drivers/ipc_notify.h>
#include <drivers/ipc_notify/v1/ipc_notify_v1.h>

void ti_sitara_EPWM_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_EPWM_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_ADC_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_ADC_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_GPIO_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_GPIO_intr_post_code(uint8_t ip_instance_number);

void interrupt_setup(uint16_t interrupt_number, uint8_t ip_instance_number, void (*intr_fcn)(void));

void ti_sitara_MCAN_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_MCAN_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_SDFM_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_SDFM_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_ECAP_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_ECAP_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_EQEP_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_EQEP_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_UART_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_UART_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_IPC_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_IPC_intr_post_code(uint8_t ip_instance_number);

