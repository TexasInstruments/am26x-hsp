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

#include <kernel/dpl/ClockP.h>
#include "profiler_timer_am263px.h"

#define RTI_RTIGCTRL                                                (0x0U)
#define RTI_RTICOMPCTRL                                             (0xcU)
#define RTI_RTIFRC0                                                 (0x10U)
#define RTI_RTIUC0                                                  (0x14U)
#define RTI_RTICPUC0                                                (0x18U)
#define RTI_RTICOMP0                                                (0x50U)
#define RTI_RTIUDCP0                                                (0x54U)
#define RTI_RTISETINT                                               (0x80U)
#define RTI_RTICLEARINT                                             (0x84U)
#define RTI_RTIINTFLAG                                              (0x88U)
#define MAX_NUMBER_OF_COUNT                                         (0xFFFFFFFFU)

#define CPUFREQ           400

void mw_usleep(uint32_t usecs)
{
    ClockP_usleep(usecs);
}

uint32_t profileReadTimer(void)
{
    uint32_t cycle_count;
    float timer;
    timer = (float) ((CycleCounterP_getCount32() / (float) CPUFREQ) * (float)1000);

    cycle_count = (uint32_t)(timer);

    return cycle_count;
   

}

