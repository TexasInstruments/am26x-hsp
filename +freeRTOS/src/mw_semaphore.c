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
#include "mw_semaphore.h"

/* Create and Initialize a Semaphore object used for managing resources. */
int  mw_osSemaphoreCreate 	(mw_semaphore_t * semaphore_def, int32_t count)
{
    *semaphore_def = xSemaphoreCreateCounting( SEMAPHORE_MAX_COUNT, count);
    if (semaphore_def!=NULL)
        return 0;
    else
        return 1;
}
/* Wait until a Semaphore token becomes available. */
int  mw_osSemaphoreWait 	(mw_semaphore_t * semaphore_def, uint32_t millisec)
{
    if (semaphore_def==NULL || *semaphore_def==NULL)
        return 1;
    else
        return (xSemaphoreTake(*semaphore_def, millisec)!=pdTRUE);
}
/* Wait until a Semaphore token becomes available. */
int  mw_osSemaphoreWaitEver	(mw_semaphore_t * semaphore_def)
{
    if (semaphore_def==NULL || *semaphore_def==NULL)
        return 1;
    else
        return (xSemaphoreTake(*semaphore_def, portMAX_DELAY)!=pdTRUE);
}
/* Release a Semaphore token. */
int mw_osSemaphoreRelease  (mw_semaphore_t * semaphore_def)
{
    if (semaphore_def==NULL || *semaphore_def==NULL)
        return 1;
    else
        return (xSemaphoreGive(*semaphore_def)!=pdTRUE);
}
/* Delete a Semaphore that was created by osSemaphoreCreate. */
int mw_osSemaphoreDelete 	(mw_semaphore_t * semaphore_def)
{
    
 #if  0
    if (semaphore_def!=NULL)
    {
        vSemaphoreDelete(*semaphore_def);
        return 0;
    }
    else
    {
        return 1;
    }
#endif
    
    return 0;
}

/* The functions below contain the definitions for the binary semaphore */
/* Create and Initialize a Binary Semaphore  object used for managing resources. */
int  mw_osBinarySemaphoreCreate 	(mw_semaphore_t * semaphore_def)
{
    *semaphore_def = xSemaphoreCreateBinary();
    if (semaphore_def!=NULL)
    {
        /* The semaphore can now be used. Its handle is stored in the
        semaphore_def variable.  Calling xSemaphoreTake() on the semaphore here
        will fail until the semaphore has first been given. */
        return mw_osSemaphoreRelease (semaphore_def);
    }
    else
    { 
        /* There was insufficient OpenRTOS heap available for the semaphore to
        be created successfully. */
        return 1;
    }
}

/* Try Wait to check if the Binary Semaphore token is available. */
int  mw_osBinarySemaphoreTryLock	(mw_semaphore_t * semaphore_def)
{
    if (semaphore_def==NULL || *semaphore_def==NULL)
    {
        return 1;
    }
    else
    {
        /* See if we can obtain the semaphore.  If the semaphore is not
        available do not wait. Non-blocking with 0 ticks */
        return (xSemaphoreTake(*semaphore_def, ( TickType_t ) 0)!=pdTRUE);
    }
}
