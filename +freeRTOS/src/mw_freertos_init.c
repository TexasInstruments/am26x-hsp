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

/*
 * Standard headers.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------------------------- */
/* RTOS-specific headers        */
/* Note: must be included first */
/* ---------------------------- */
#include "mw_freertos.h"

#include "MW_MODEL_INIT.h"
//#include "timer_utils.h"



/* ---------------------------- */
/* RTOS-specific declarations   */
/* ---------------------------- */

/* Other declarations */
#define _unused(x) ((void)(x))

static mw_thread_t modelInitThread;

/* Note: stdint.h is included in FreeRTOS.h
 * If stdint.h cannot be located then:
 *   + If using GCC ensure the -nostdint options is *not* being used.
 *   + Ensure the project's include path includes the directory in which your
 *     compiler stores stdint.h.
 *   + Set any compiler options necessary for it to support C99, as technically
 *     stdint.h is only mandatory with C99 (FreeRTOS does not require C99 in any
 *     other way).
 *   + The FreeRTOS download includes a simple stdint.h definition that can be
 *     used in cases where none is provided by the compiler.  The files only
 *     contains the typedefs required to build FreeRTOS.  Read the instructions
 *     in FreeRTOS/source/stdint.readme for more information.
 */

/* Scheduler timer */
mw_timer_t mw_scheduler_timer_def;
int32_t mw_timer_id;

#ifndef OS_STKSIZE
 #define OS_STKSIZE     1000    // this stack size value is in words
#endif

#ifndef MW_EXTMODE_STACKSIZE
#define MW_EXTMODE_STACKSIZE 10240
#endif


#ifdef MW_STANDALONE_EXECUTION_PROFILER_ON
    SemaphoreHandle_t  profilingDataStoreMutex;
#endif

static void model_initialize()
{
	MW_MODEL_INITIALIZE();

	vTaskDelete(NULL);
}

/* Scheduler task move base rate task from wait state to ready state */
void schedulerTask(TimerHandle_t arg)
{
    _unused(arg);
#ifdef DETECT_OVERRUNS
    testForRateOverrun(0);
#endif
    /* Release base rate semaphore to move base rate task from wait state to ready state */
    mw_osSemaphoreRelease(&baserateTaskSem);
}


void scheduleMwTasks()
{
     BaseType_t xHigherPriorityTaskWoken;
     xHigherPriorityTaskWoken = pdFALSE;
     xSemaphoreGiveFromISR( baserateTaskSem, &xHigherPriorityTaskWoken );
     portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
     
}


/* ---------------------------- */
/* Externally visible functions */
/* ---------------------------- */
void mw_WaitForThisEvent(int sigNo)
{
    _unused(sigNo);
}

void mw_AddBlockForThisEvent(int sigNo)
{
    _unused(sigNo);
}

void mw_AddHandlerForThisEvent(int sigNo, int sigToBlock[], int numSigToBlock, void *sigHandler)
{
    _unused(sigNo);
    _unused(sigToBlock);
    _unused(numSigToBlock);
    _unused(sigHandler);
}

void mw_RestoreHandlerForThisEvent(int sigNo)
{
}

void baseRateTaskWrapper(void *arg)
{
    baseRateTask(arg);
}
#ifdef MW_HAS_MULTIPLE_RATES
void subrateTaskWrapper(void *arg)
{
    subrateTask(arg);
}
#endif

void mw_RTOSInit(float baseRatePeriod, int numSubrates)
{
    int32_t    status;
    uint64_t  timerPeriod;
    bool      autoLoad;
#ifdef MW_HAS_MULTIPLE_RATES
    int i;
#endif
    
#if ( configUSE_TRACE_FACILITY == 1 )
#if 0
    /* Initialise the trace recorder and create the label used to post user
     events to the trace recording on each tick interrupt. */
    vTraceInitTraceData();
    xTickTraceUserEvent = xTraceOpenLabel( "tick" );
    /* Start the trace recording */
    uiTraceStart();
#endif
#endif

	/*create initialize tread*/
	modelInitThread.priority = ( MW_BASERATE_PRIORITY );
    modelInitThread.taskhandle = NULL;
    modelInitThread.stackdepth = getStackDepth(OS_STKSIZE);
    strcpy(modelInitThread.name,"initThread");
	status = mw_osThreadCreate(&modelInitThread, &model_initialize, NULL);
	
    /* Create semaphore for base rate task */
    status = mw_osSemaphoreCreate(&baserateTaskSem, 0);
    CHECK_STATUS(status, 0, "mw_osSemaphoreCreate:baserateTaskSem");
    /* Create semaphore for stop task */
    status = mw_osSemaphoreCreate(&stopSem, 0);
    CHECK_STATUS(status, 0, "mw_osSemaphoreCreate:stopSem");
    
    /* Create baseRateTask thread */
    baseRateThread.priority = ( MW_BASERATE_PRIORITY );
    baseRateThread.taskhandle = NULL;
    baseRateThread.stackdepth = getStackDepth(OS_STKSIZE);
    strcpy(baseRateThread.name,"Baserate");
    status = mw_osThreadCreate(&baseRateThread, &baseRateTaskWrapper, NULL);
    CHECK_STATUS(status, 0, "mw_osThreadCreate");
    
#ifdef MW_HAS_MULTIPLE_RATES
    for (i = 0; i < MW_NUMBER_SUBRATES; i++) {
        taskId[i] = i;
        status = mw_osSemaphoreCreate(&subrateTaskSem[i], 0);
        CHECK_STATUS(status, 0,"mw_osSemaphoreCreate");
        /* Initialize thread structure */
        subRateThread[i].priority = MW_BASERATE_PRIORITY - (i+1);
#ifdef MW_NEEDS_BACKGROUND_TASK
        CHECK_STATUS(subRateThread[i].priority>(tskIDLE_PRIORITY+1), 1, "Too many rates");
#else
        CHECK_STATUS(subRateThread[i].priority>tskIDLE_PRIORITY, 1, "Too many rates");
#endif
        subRateThread[i].taskhandle = NULL;
        subRateThread[i].stackdepth = getStackDepth(OS_STKSIZE);
        status = mw_osThreadCreate(&subRateThread[i], &subrateTaskWrapper, (void *)&taskId[i]);
        
        CHECK_STATUS(status, 0, "mw_osThreadCreate");
#ifdef DETECT_OVERRUNS
        status = mw_osBinarySemaphoreCreate(&rateTaskFcnRunningMutex[i+1]);
        CHECK_STATUS(status, 0, "mw_osBinarySemaphoreCreate");
#endif
    }
#endif
    
    /* Set the SysTick priority */
#ifdef MW_SCHEDULER_PRIORITY
    NVIC_SetPriority((IRQn_Type)(-1), MW_SCHEDULER_PRIORITY);
#endif
    
#ifdef MW_NEEDS_BACKGROUND_TASK
    /* Create backgroundTask thread */
    backgroundThread.priority = 1; //tskIDLE_PRIORITY;
    backgroundThread.taskhandle = NULL;
    backgroundThread.stackdepth = getStackDepth(MW_EXTMODE_STACKSIZE);
    strcpy(backgroundThread.name,"Background");
    status = mw_osThreadCreate(&backgroundThread, backgroundTask, NULL);
    CHECK_STATUS(status, 0 ,"mw_osThreadCreate");
#endif
#ifdef DETECT_OVERRUNS
    status = mw_osBinarySemaphoreCreate(&rateTaskFcnRunningMutex[0]);
    CHECK_STATUS(status, 0, "mw_osBinarySemaphoreCreate");
#endif    

#ifdef MW_STANDALONE_EXECUTION_PROFILER_ON
    status = mw_osBinarySemaphoreCreate(&profilingDataStoreMutex);
    CHECK_STATUS(status, 0, "mw_osBinarySemaphoreCreate");
#endif
    
    /*
     * Create timer to call scheduler task 
     * If period is less than 1 second use high speed timer developed by Siemens,
     * otherwise use a FreeRTOS timer.
     */
              
   mw_timer_id = 0;
   mw_scheduler_timer_def.handle = NULL;
   mw_scheduler_timer_def.period = (TickType_t)(baseRatePeriod*1000/portTICK_PERIOD_MS);
   mw_scheduler_timer_def.autoreload = pdTRUE;
   mw_scheduler_timer_def.id = (void *)mw_timer_id;
   strcpy(mw_scheduler_timer_def.name, "Timer1");
   status = mw_osTimerCreate(&mw_scheduler_timer_def, &schedulerTask);
   CHECK_STATUS(status, 0,"mw_osTimerCreate");
   /* Start the timer */
   status = mw_osTimerStart(&mw_scheduler_timer_def, (TickType_t )1000);
   CHECK_STATUS(status, 0,"mw_osTimerCreate");
      /* Start the real time scheduler */
    #ifndef MW_DONOTSTART_SCHEDULER
    vTaskStartScheduler();
    #endif
 

    
}

