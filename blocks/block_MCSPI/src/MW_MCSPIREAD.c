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