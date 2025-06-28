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
#include "MW_MCSPIREAD.h"
#include "MW_target_hardware_resources.h"


void MCSPIREAD_step(MCSPIReadStepStruct *ptr, const void* RxBuffer, uint32_t output_data_len)
{
    MCSPI_ExtendedParams extended_params_rx;
    uint32_t timeout = MCSPI_WAIT_FOREVER;
    uint32_t McspiRxBuffer[output_data_len], dataWidth, bufWidthShift;
    uint8_t *rxDataPtr8 = NULL, *outPtr8 = NULL;
    uint16_t *rxDataPtr16 = NULL, *outPtr16 = NULL;
    uint32_t *rxDataPtr32 = NULL, *outPtr32 = NULL;
    uint32_t cnt = 0;
    

    if(ptr->Enable_CH0)
    {
        extended_params_rx.channel = MCSPI_CHANNEL_0;
        extended_params_rx.dataSize = ptr->CH0_DataSize;
        extended_params_rx.csDisable = ptr->CH0_CSDisable;
    }
    else
    {
        extended_params_rx.channel = MCSPI_CHANNEL_1;
        extended_params_rx.dataSize = ptr->CH1_DataSize;
        extended_params_rx.csDisable = ptr->CH1_CSDisable;
    }

    dataWidth = extended_params_rx.dataSize;
    bufWidthShift = MCSPI_getBufWidthShift(dataWidth);

    if(dataWidth < 9U)
    {
        rxDataPtr8 = (uint8_t *)McspiRxBuffer;
        outPtr8 = (uint8_t *)RxBuffer;
      
    }
    else if(dataWidth < 17U)
    {
        rxDataPtr16 = (uint16_t *)McspiRxBuffer;
        outPtr16 = (uint16_t *)RxBuffer;
    }
    else
    {
        rxDataPtr32 = (uint32_t *)McspiRxBuffer;
        outPtr32 = (uint32_t *)RxBuffer;
    }

    cnt = (output_data_len * (sizeof(uint32_t) / (1 << bufWidthShift)));
 
    MCSPI_lld_read(gMcspiHandle[ptr->mcspi_instance], McspiRxBuffer, output_data_len, timeout, &extended_params_rx);


    outPtr16 = (uint16_t *)RxBuffer;
    rxDataPtr16 = (uint16_t *)McspiRxBuffer;
    for(uint8_t i=0; i < output_data_len * 2; i++)
    {
        *outPtr16++ = *rxDataPtr16++;  
    }

}

void MCSPIREAD_release()
{
    
}