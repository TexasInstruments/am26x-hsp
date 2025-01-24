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
#include "FreeRTOS.h"
#include "task.h"
#include "mw_thread.h"

#define _unused(x) ((void)(x))

/*------------------------------------------------------------------------------------------------------*/
/*------------------------------------- Thread functions -----------------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/
/* Create a thread and add it to Active Threads and set it to state READY. */
int mw_osThreadCreate(mw_thread_t * aThread, TaskFunction_t aTaskFunction, void *aFcnParamter)
{
	if( aThread==NULL )
    {
        return 1;
    }
    else
    {
        BaseType_t out = xTaskCreate(aTaskFunction, aThread->name, aThread->stackdepth, aFcnParamter, aThread->priority, &(aThread->taskhandle));	
        configASSERT( aThread->taskhandle );
        return (out!=pdPASS);
    }
}

/* Return the thread handle of the current running thread.  */
TaskHandle_t mw_osThreadGetId   (void)
{
#if ( INCLUDE_xTaskGetCurrentTaskHandle == 1 )
	return xTaskGetCurrentTaskHandle();
#else
    return NULL;
#endif
}

/* Terminate execution of a thread and remove it from Active Threads. */
void   mw_osThreadTerminate 	(mw_thread_t * thread_def)
{
	if (thread_def!=NULL)
    {
        vTaskDelete(thread_def->taskhandle);
    }
}

/* Change priority of an active thread. */
void  mw_osThreadSetPriority 	(mw_thread_t * thread_def, UBaseType_t  priority)
{
	if (thread_def!=NULL)
    {
        vTaskPrioritySet(thread_def->taskhandle, priority);
        thread_def->priority = priority;
    }
}

/* Get current priority of an active thread. */
UBaseType_t mw_osThreadGetPriority 	(mw_thread_t * thread_def)
{
#if INCLUDE_vTaskPriorityGet==1
    if (thread_def!=NULL)
    {
        return uxTaskPriorityGet(thread_def->taskhandle);
    }
    else
    {
        return thread_def->priority;
    }
#else
    return thread_def->priority;
#endif
}

/* Pass control to next thread that is in state READY. */
void   mw_osThreadYield 		(void)
{
	taskYIELD();
}

void   mw_osThreadExit 	(void * value_ptr)
{
    _unused(value_ptr);
#if INCLUDE_vTaskDelete /* vTaskDelete() enabled */
      vTaskDelete(NULL);
#endif
}

int   mw_osThreadJoin 	(mw_thread_t thread_def, void ** value_ptr)
{
    _unused(value_ptr);
    _unused(thread_def);
#if INCLUDE_vTaskDelete /* vTaskDelete() enabled */
      vTaskDelete(thread_def.taskhandle);
      return 0;
#else
      return 1; /* vTaskDelete() not available */
     
#endif
}

void   mw_osThreadSleep 	(const long msToDelay )
{
	vTaskDelay( (TickType_t) msToDelay/portTICK_PERIOD_MS );
}



