#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <drivers/uart/v0/lld/dma/edma/uart_dma_edma.h>
#include <drivers/uart/v0/lld/uart_lld.h>
#include "MW_UART.h"


UARTLLD_Object gUartObject[CONFIG_UART_NUM_LLD_INSTANCES];
UARTLLD_Handle gUartHandleLld[CONFIG_UART_NUM_LLD_INSTANCES];
uint32_t gUartDmaConfigNumLld = CONFIG_UART_NUM_DMA_LLD_INSTANCES;
UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_LLD_INSTANCES];


EDMA_Config gEdmaConfigLld[1] =
{
    {
        NULL,
        NULL,
    }
};

void uart_write_setup(UARTWriteSetupStruct *ptr)
{

        gUartInitObject[0].inputClkFreq       = 48000000;
        gUartInitObject[0].baudRate           = ptr->baudRate;
        gUartInitObject[0].dataLength         = UART_LEN_8;
        gUartInitObject[0].stopBits           = ptr->stopBits;
        gUartInitObject[0].parityType         = ptr->parityType;
        gUartInitObject[0].readReturnMode     = UART_READ_RETURN_MODE_FULL;
        gUartInitObject[0].hwFlowControl      = FALSE;
        gUartInitObject[0].hwFlowControlThr   = UART_RXTRIGLVL_16;
        gUartInitObject[0].transferMode       = UART_CONFIG_MODE_POLLED;
        gUartInitObject[0].intrNum            = ptr->intrNum;
        gUartInitObject[0].intrPriority       = 4U;
        gUartInitObject[0].operMode           = ptr->operMode;
        gUartInitObject[0].rxTrigLvl          = UART_RXTRIGLVL_8;
        gUartInitObject[0].txTrigLvl          = UART_TXTRIGLVL_32;
        gUartInitObject[0].clockP_get         = ClockP_getTicks;
        gUartInitObject[0].clockP_usecToTick  = ClockP_usecToTicks;
        gUartInitObject[0].writeCompleteCallbackFxn = NULL;
        gUartInitObject[0].readCompleteCallbackFxn  = NULL;

        

    int32_t status = UART_STATUS_SUCCESS;

    gUartHandleLld[0]             = &gUartObject[0];
    gUartHandleLld[0]->state      = UART_STATE_RESET;
    gUartHandleLld[0]->baseAddr   = ptr->baseaddr;
    gUartHandleLld[0]->hUartInit = &gUartInitObject[0];

    status += UART_lld_init(gUartHandleLld[0]);

    DebugP_assert(status == UART_STATUS_SUCCESS);

}

void uart_write_step(const void *dataInput, uint8_t dataType, uint32_t inputLength)
{

    uint32_t translength = inputLength;
    UART_Transaction trans;
    UART_lld_Transaction_init(&trans);
    uint8_t UartBuffer[translength];
    uint32_t b;
    
    for(int i= 0; i<inputLength; i++)
    {
    if(dataType == 0)
    {
        b = (double)b;
        b = *(double *)dataInput;
    }
    else if(dataType == 1)
    {
        b = (float)b;
        b = *(float *)dataInput;
    }
    else if(dataType == 2)
    {
        b = (uint32_t)b;
        b = *(uint32_t *)dataInput;
    }
    else if(dataType == 3)
    {
        b = (int32_t)b;
        b = *(int32_t *)dataInput;
    }
    else if(dataType == 4)
    {
        b = (int16_t)b;
        b = *(int16_t *)dataInput;
    }
    else if(dataType == 5)
    {
        b = (uint16_t)dataInput;
        b = *(uint16_t *)dataInput;
    }
    else if(dataType == 6)
    {
        b = (int8_t)b;
        b = *(int8_t *)dataInput;
    }
    else if(dataType == 7)
    {
        b = (uint8_t)b;
        b = *(uint8_t *)dataInput;
    }
    else if(dataType == 8)
    {
        b = (bool)b;
        b = *(bool *)dataInput;
    }
    
    // dataInput1++;

    trans.buf = &UartBuffer[0U];
    int n= sprintf(UartBuffer,"%d ", b);
    trans.count = n;
    UART_lld_write(gUartHandleLld[0], trans.buf, trans.count, trans.timeout, NULL);

    if(dataType == 0)
        dataInput+=8;
    else if(dataType == 1 || dataType == 2 || dataType == 3)
        dataInput+=4;
    else if(dataType == 4 || dataType == 5)
        dataInput+=2;
    else if(dataType == 6 || dataType == 7 || dataType == 8 || dataType == 9)
        dataInput+=1;
    }

}

void uart_write_release()
{
    int32_t status = UART_STATUS_SUCCESS;
    status = UART_lld_deInit(gUartHandleLld[0]);
    DebugP_assert(status == UART_STATUS_SUCCESS);
    
}
