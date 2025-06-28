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
#include "MW_MCSPIWRITE.h"
#include "MW_target_hardware_resources.h"

void MCSPIWRITE_step(MCSPIWriteStepStruct* ptr, uint32_t count, const void* InputData)
{
    MCSPI_ExtendedParams extended_params_tx;
    uint32_t timeout = MCSPI_WAIT_FOREVER;
    uint32_t McspiTxBuffer[count], dataWidth;
    uint8_t i = 0;
    uint32_t cnt = 0, bufWidthShift;
    uint8_t *txDataPtr8 = NULL, *inpDataPtr8 = NULL;
    uint16_t *txDataPtr16 = NULL, *inpDataPtr16 = NULL;
    uint32_t *txDataPtr32 = NULL, *inpDataPtr32 = NULL;

    if(ptr->Enable_CH0)
    {
        extended_params_tx.channel = MCSPI_CHANNEL_0;
        extended_params_tx.dataSize = ptr->CH0_DataSize;
        extended_params_tx.csDisable = ptr->CH0_CSDisable;
    }
    else
    {
        extended_params_tx.channel = MCSPI_CHANNEL_1;
        extended_params_tx.dataSize = ptr->CH1_DataSize;
        extended_params_tx.csDisable = ptr->CH1_CSDisable;
    }

    dataWidth = extended_params_tx.dataSize;
    bufWidthShift = MCSPI_getBufWidthShift(dataWidth);

    if(dataWidth < 9U)
    {
        txDataPtr8 = (uint8_t *) &McspiTxBuffer[0U];
        inpDataPtr8 = (uint8_t *)InputData;
    }
    else if(dataWidth < 17U)
    {
        txDataPtr16 = (uint16_t *)McspiTxBuffer;
        inpDataPtr16 = (uint16_t *)InputData;
    }
    else
    {
        txDataPtr32 = (uint32_t *)McspiTxBuffer;
        inpDataPtr32 = (uint32_t *)InputData;
    }

    if(ptr->dataType == 0)
        cnt = (count * (sizeof(uint8_t) / (1 << bufWidthShift)));
    else if(ptr->dataType == 1)
        cnt = (count * (sizeof(uint16_t) / (1 << bufWidthShift)));
    else 
        cnt = (count * sizeof(uint32_t) / (1 << bufWidthShift));

    for(i = 0; i < cnt; i++)
    {
        if(dataWidth < 9U)
        {
            *txDataPtr8 = (uint8_t)(*inpDataPtr8);
            txDataPtr8++;
            inpDataPtr8++;
        }
        else if(dataWidth < 17U)
        {
            *txDataPtr16++ = (uint16_t)(*inpDataPtr16++);
        }
        else
        {
            *txDataPtr32++ = (uint32_t)(*inpDataPtr32++);
        }
        
    }
    
    
    MCSPI_lld_write(gMcspiHandle[ptr->mcspi_instance], &McspiTxBuffer, cnt, timeout, &extended_params_tx);

}

void MCSPIWRITE_release()
{
    
}