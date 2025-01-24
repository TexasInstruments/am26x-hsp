/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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
 */
#ifndef __FREERTOS_INIT_H__
#define __FREERTOS_INIT_H__

#include <stdio.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portable.h"
#include "portmacro.h"

#include "mw_thread.h"
#include "mw_semaphore.h"
#include "mw_timer.h"

/* ---------------------------- */
/* Required Coder Target header */
/* ---------------------------- */
#include "MW_custom_RTOS_header.h"
#include "MW_target_hardware_resources.h"

#define CHECK_STATUS(status, expStatus, fcn)  		configASSERT((status)==(expStatus))
//#define CHECK_STATUS(status, expStatus, fcn)
#define CHECK_STATUS_NOT(status, errStatus, fcn)	configASSERT((status)!=(expStatus))
    
#define getStackDepth(x) ((x/sizeof(portSTACK_TYPE))>=configMINIMAL_STACK_SIZE ? (x/sizeof(portSTACK_TYPE)) : configMINIMAL_STACK_SIZE)

void mw_WaitForThisEvent(int sigNo);
void mw_AddBlockForThisEvent(int sigNo);
void mw_AddHandlerForThisEvent(int sigNo, int sigToBlock[], int numSigToBlock, void *sigHandler);
void mw_RestoreHandlerForThisEvent(int sigNo);


#ifdef MW_HAS_TARGET_SERVICES
void terminateCommService();
extern void runCommService();
#endif

#ifdef MW_STANDALONE_EXECUTION_PROFILER_ON
    extern SemaphoreHandle_t  profilingDataStoreMutex;
#endif

#if 0
#if (configUSE_TRACE_FACILITY == 1)
/* The user trace event posted to the trace recording on each tick interrupt.
Note tick events will not appear in the trace recording with regular period
because this project runs in a Windows simulator, and does not therefore
exhibit deterministic behaviour. */
traceLabel xTickTraceUserEvent;
static portBASE_TYPE xTraceRunning = pdTRUE;
#endif
#endif

extern void mw_RTOSInit(float baseRatePeriod, int numSubrates);

#endif //__FREERTOS_INIT_H__
