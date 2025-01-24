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
#include "mw_timer.h"

/*------------------------------------------------------------------------------------------------------*/
/*----------------------------- Timer Wrapper functions ------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------*/
int mw_osTimerCreate(mw_timer_t * timer_def, TimerCallbackFunction_t funcptr)
{
    timer_def->handle = xTimerCreate(timer_def->name, timer_def->period, timer_def->autoreload, timer_def->id, funcptr);
    if (timer_def->handle != NULL)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

int mw_osTimerStart(mw_timer_t * timer_def, TickType_t xBlockTime )
{
	if (timer_def!=NULL && timer_def->handle!=NULL)
    {
        return (xTimerStart(timer_def->handle, xBlockTime)!=pdPASS);
    }
    else
    {
        return 1;
    }
}

int mw_osTimerStop(mw_timer_t * timer_def)
{
	if (timer_def!=NULL && timer_def->handle!=NULL)
    {
        return (xTimerStop(timer_def->handle, 500000)!=pdPASS);
    }
    else
    {
        return 1;
    }
}

int mw_osTimerDelete(mw_timer_t * timer_def)
{
	if (timer_def!=NULL && timer_def->handle!=NULL)
    {
        return (xTimerDelete(timer_def->handle, 500000)!=pdPASS);
    }
    else
    {
        return 1;
    }
}
