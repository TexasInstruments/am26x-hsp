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
#ifndef UART_MW_H_
#define UART_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include <drivers/dac.h>
#include <drivers/soc.h>
#include <math.h>
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
    DAC_ReferenceVoltage DACRef;
    DAC_LoadMode LoadMode;
    uint16_t PWMSyncSignal;
    bool Enable_output;
    uint16_t ShdwValue;
    bool DACLockControl;
    bool DACLockOutput;
    bool DACLockShadow;
} DACSetupStruct;

void DAC_setup(DACSetupStruct *ptr);
void DAC_step(uint32_t base, uint16_t DACVal);
void DAC_release(uint32_t base);



#endif