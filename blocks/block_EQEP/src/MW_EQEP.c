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
#include "MW_EQEP.h"

void EQEP_setup(EQEPSetupStruct *ptr)
{
    uint16_t int_mask = 0x0;
    if(ptr->Use_Interrupt && ptr->Enable_Interrupt)
    {
        if(ptr->Global_int_flag)
            int_mask = int_mask | EQEP_INT_GLOBAL;
        if(ptr->Position_cntr_error)
            int_mask = int_mask | EQEP_INT_POS_CNT_ERROR;
        if(ptr->Quad_phase_error)
            int_mask = int_mask | EQEP_INT_PHASE_ERROR;
        if(ptr->Quad_dir_change)
            int_mask = int_mask | EQEP_INT_DIR_CHANGE;
        if(ptr->Watchdog_timeout)
            int_mask = int_mask | EQEP_INT_WATCHDOG ;
        if(ptr->Pos_cntr_underflow)
            int_mask = int_mask | EQEP_INT_UNDERFLOW;
        if(ptr->Pos_cntr_overflow)
            int_mask = int_mask | EQEP_INT_OVERFLOW;
        if(ptr->Pos_cmp_ready)
            int_mask = int_mask | EQEP_INT_POS_COMP_READY;
        if(ptr->Pos_cmp_match)
            int_mask = int_mask | EQEP_INT_POS_COMP_MATCH;
        if(ptr->Strobe_event_latch)
            int_mask = int_mask | EQEP_INT_STROBE_EVNT_LATCH;
        if(ptr->Index_event_latch)
            int_mask = int_mask | EQEP_INT_INDEX_EVNT_LATCH;
        if(ptr->Unit_timeout) 
            int_mask = int_mask | EQEP_INT_UNIT_TIME_OUT;
        if(ptr->QMA_error)
            int_mask = int_mask | EQEP_INT_QMA_ERROR;

        EQEP_disableInterrupt(ptr->base, int_mask);
        EQEP_clearInterruptStatus(ptr->base, int_mask);

    }
    EQEP_setStrobeSource(ptr->base, ptr->strbsrc);
    EQEP_setInputPolarity(ptr->base, ptr->invertQEPA, ptr->invertQEPB, ptr->invertIndex, ptr->invertStrobe);
    uint16_t decoderconfig = ptr->posCntrSrc + ptr->resolution + ptr->resolution;
    EQEP_setDecoderConfig(ptr->base, decoderconfig);
    EQEP_setEmulationMode(ptr->base, ptr->emulationMode);
    EQEP_setPositionCounterConfig(ptr->base, ptr->posCntrMode, ptr->maxPosition);
    EQEP_setPosition(ptr->base, ptr->posCntr);
    if(ptr->enableUnitTimer)
        EQEP_enableUnitTimer(ptr->base, ptr->unitTimerPeriod);
    else
        EQEP_disableUnitTimer(ptr->base);
    if(ptr->enableWatchdog)
    {
        EQEP_enableWatchdog(ptr->base, ptr->watchdogTimerPrd);
        EQEP_setWatchdogTimerValue(ptr->base, ptr->watchdogTimerValue);
    }
    else
    {
        EQEP_disableWatchdog(ptr->base);
    }
    uint32_t latchMode = ptr->QEPCaptureLatch + ptr->StrobeEventLatch + ptr->IndexEventLatch;
    EQEP_setLatchMode(ptr->base, latchMode);
    EQEP_setQMAModuleMode(ptr->base, ptr->QMAModuleMode);
    if(ptr->changeDirIndx)
        EQEP_enableDirectionChangeDuringIndex(ptr->base);
    else
        EQEP_disableDirectionChangeDuringIndex(ptr->base);

    if(ptr->Use_Interrupt && ptr->Enable_Interrupt)
        EQEP_enableInterrupt(ptr->base, int_mask);
    uint16_t initevents;
    if(ptr->PositionStrobeEvents != 0)
        initevents = initevents | ptr->PositionStrobeEvents;
    if(ptr->PositionIndexEvents != 0)
        initevents = initevents | ptr->PositionIndexEvents;
    EQEP_setPositionInitMode(ptr->base, initevents);
    if(ptr->SWInitPositionCntr)
        EQEP_setSWPositionInit(ptr->base, true);
    else
        EQEP_setSWPositionInit(ptr->base, false);
    EQEP_setInitialPosition(ptr->base, ptr->initialPosition);

    if(ptr->Enable_PositionCntr)
        EQEP_enableModule(ptr->base);
    else
        EQEP_disableModule(ptr->base);
    if(ptr->Config_Edge_CapUnit)
    {
        EQEP_setCaptureConfig(ptr->base, ptr->Capture_Prescaler, ptr->evntPrescale);

        if(ptr->Enable_edgecaptureunit)
            EQEP_enableCapture(ptr->base);
        else
            EQEP_disableCapture(ptr->base);
    }

    uint16_t compConfig = ptr->SPSEL + ptr->Pos_Cmp_ShadowLoad;
    if(ptr->Config_Pos_CmpUnit)
    {
       EQEP_setCompareConfig(ptr->base, compConfig, ptr->compareValues, ptr->compareCycles);
       if(ptr->EnablePosCmpUnit)
        EQEP_enableCompare(ptr->base);
       else
        EQEP_disableCompare(ptr->base);       
    }

    EQEP_SourceSelect sourceSelect;
    sourceSelect.sourceA = ptr->QEPASource;
    sourceSelect.sourceB = ptr->QEPBSource;
    sourceSelect.sourceIndex = ptr->IndexSource;
    EQEP_selectSource(ptr->base, sourceSelect);  

}

void EQEP_step(EQEPStepStruct *ptr, const void *EQEPOutput, const void *directionOutput)
{

    uint32_t *output = (uint32_t *)EQEPOutput;
    int16_t *diroutput = (int16_t *)directionOutput;

    if(ptr->getDirectionFlagValue)
    {
        *diroutput = (int16_t)EQEP_getDirection(ptr->base);
        
    }
    if(ptr->getPositionCounter)
    {
        *output = (uint32_t)EQEP_getPosition(ptr->base);
        output++;
    }
    if(ptr->PositionCounterLatchValue)
    {
        if(ptr->QEPCaptureLatch == 0)
            *output = (uint32_t)EQEP_getPositionLatch(ptr->base);
        else
        {
            while((EQEP_getInterruptStatus(ptr->base) & EQEP_INT_UNIT_TIME_OUT) == 0)
            {

            }

            *output = (uint32_t)EQEP_getPositionLatch(ptr->base);

            if(!ptr->use_interrupt | !ptr->enable_interrupt | !ptr->unit_timeout_intr)
                EQEP_clearInterruptStatus(ptr->base, EQEP_INT_UNIT_TIME_OUT);
        }   
        output++;
    }
    if(ptr->IndexPositionLatchValue)
    {
        while((EQEP_getInterruptStatus(ptr->base) & EQEP_INT_INDEX_EVNT_LATCH) == 0)
        {

        }

        *output = (uint32_t)EQEP_getIndexPositionLatch(ptr->base);
        output++;

        if(!ptr->use_interrupt | !ptr->enable_interrupt | !ptr->indx_pos_latch_intr)
            EQEP_clearInterruptStatus(ptr->base, EQEP_INT_INDEX_EVNT_LATCH);
    }
    if(ptr->StrobePositionLatchValue)
    {
        while((EQEP_getInterruptStatus(ptr->base) & EQEP_INT_STROBE_EVNT_LATCH) == 0)
        {

        }

        *output = (uint32_t)EQEP_getStrobePositionLatch(ptr->base);
        output++;

        if(!ptr->use_interrupt | !ptr->enable_interrupt | !ptr->strobe_pos_latch_intr)
            EQEP_clearInterruptStatus(ptr->base, EQEP_INT_STROBE_EVNT_LATCH);

    }
    if(ptr->CaptureTimerValue)
    {
        *output = (uint32_t)EQEP_getCaptureTimer(ptr->base);
        output++;
    }
    if(ptr->CapturePeriodValue)
    {
        *output = (uint32_t)EQEP_getCapturePeriod(ptr->base);
        output++;
    }
    if(ptr->CaptureTimerLatchValue)
    {
        while((EQEP_getStatus(ptr->base) & EQEP_STS_UNIT_POS_EVNT) == 0)
        {

        }
        if((EQEP_getStatus(ptr->base) & EQEP_STS_CAP_OVRFLW_ERROR) != 0)
            *output = (uint32_t)EQEP_getCaptureTimerLatch(ptr->base);
        else 
            *output = (uint32_t)0xFFFF;

        output++;
    }
    if(ptr->CapturePeriodLatchValue)
    {
        while((EQEP_getStatus(ptr->base) & EQEP_STS_UNIT_POS_EVNT) == 0)
        {

        }
        if((EQEP_getStatus(ptr->base) & EQEP_STS_CAP_OVRFLW_ERROR) != 0)
            *output = (uint32_t)EQEP_getCapturePeriodLatch(ptr->base);
        else
            *output = (uint32_t)0xFFFF;

        output++;
        EQEP_clearStatus(ptr->base, (EQEP_STS_UNIT_POS_EVNT |
                                      EQEP_STS_CAP_OVRFLW_ERROR));
    }
    if(ptr->WatchdogTimerValue)
    {
        *output = (uint32_t)EQEP_getWatchdogTimerValue(ptr->base);
        output++;
    }

    return;
        


}

void EQEP_release(EQEPReleaseStruct *ptr)
{

}