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
#ifndef __MW_THREAD__H__
#define __MW_THREAD__H__

#include "FreeRTOS.h"
#include "task.h"

#ifndef INCLUDE_vTaskPriorityGet 
#define INCLUDE_vTaskPriorityGet 1
#endif

typedef struct _mw_thread_t{
	char            name[configMAX_TASK_NAME_LEN];
    UBaseType_t     priority;
	unsigned short  stackdepth;
	TaskHandle_t    taskhandle;
} mw_thread_t;


/* Create a thread and add it to Active Threads and set it to state READY. */
int    mw_osThreadCreate        (mw_thread_t * aTaskHandle, TaskFunction_t aTaskFunction, void *aFcnParamter);
/* Return the thread ID of the current running thread.  */
TaskHandle_t  mw_osThreadGetId 	(void);
/* Terminate execution of a thread and remove it from Active Threads. */
void   mw_osThreadTerminate 	(mw_thread_t * thread_def);
/* Change priority of an active thread. */
void   mw_osThreadSetPriority 	(mw_thread_t * thread_def, UBaseType_t priority);
/* Get current priority of an active thread. */
UBaseType_t mw_osThreadGetPriority 	(mw_thread_t * thread_def);
/* Pass control to next thread that is in state READY. */
void   mw_osThreadYield 		(void);
/* Exit from thread */
void   mw_osThreadExit          (void * value_ptr);
/* Join */
int    mw_osThreadJoin          (mw_thread_t thread_def, void ** value_ptr);
/* Delay a thread for a certain no of milliseconds */
void   mw_osThreadSleep         (const long msToDelay);
#endif /* __MW_THREAD__H__ */
