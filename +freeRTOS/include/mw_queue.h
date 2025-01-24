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
#ifndef __MW_QUEUE__H__
#define __MW_QUEUE__H__
#include "FreeRTOS.h"
#include "semphr.h"

#define SEMAPHORE_MAX_COUNT 1
#ifndef  INCLUDE_vTaskSuspend
#define  INCLUDE_vTaskSuspend 1
#endif

typedef SemaphoreHandle_t mw_semaphore_t;

/* Create and Initialize a Semaphore object used for managing resources. */
int mw_osSemaphoreCreate 	(mw_semaphore_t * semaphore_def, int32_t count);
/* Wait until a Semaphore token becomes available. */
int mw_osSemaphoreWait 	    (mw_semaphore_t * semaphore_def, uint32_t millisec);
/* Wait until a Semaphore token becomes available. */
int mw_osSemaphoreWaitEver	(mw_semaphore_t * semaphore_def);
/* Release a Semaphore token. */
int mw_osSemaphoreRelease    (mw_semaphore_t * semaphore_def);
/* Delete a Semaphore that was created by osSemaphoreCreate. */
int mw_osSemaphoreDelete 	(mw_semaphore_t * semaphore_def);

/* The functions below contain the declarations for the binary semaphore */

/* Create and Initialize a Binary Semaphore  object used for managing resources. */
int  mw_osBinarySemaphoreCreate 	(mw_semaphore_t * semaphore_def);
/* Try Wait to check if the Binary Semaphore token is available. */
int  mw_osBinarySemaphoreTryLock	(mw_semaphore_t * semaphore_def);

#endif /* __MW_SEMAPHORE__H__ */
