#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <drivers/uart/v0/lld/dma/edma/uart_dma_edma.h>
#include <drivers/uart/v0/lld/uart_lld.h>
#include "MW_UART.h"


UARTLLD_Object gUartReadObject[CONFIG_UART_NUM_LLD_INSTANCES];
UARTLLD_Handle gUartReadHandleLld[CONFIG_UART_NUM_LLD_INSTANCES];
uint32_t gUartReadDmaConfigNumLld = CONFIG_UART_NUM_DMA_LLD_INSTANCES;
UARTLLD_InitObject gUartReadInitObject[CONFIG_UART_NUM_LLD_INSTANCES];


EDMA_Config gUartReadEdmaConfigLld[1] =
{
    {
        NULL,
        NULL,
    }
};

void uart_read_setup(UARTReadSetupStruct *ptr)
{

        gUartReadInitObject[0].inputClkFreq       = 48000000;
        gUartReadInitObject[0].baudRate           = ptr->baudRate;
        gUartReadInitObject[0].dataLength         = UART_LEN_8;
        gUartReadInitObject[0].stopBits           = ptr->stopBits;
        gUartReadInitObject[0].parityType         = ptr->parityType;
        gUartReadInitObject[0].readReturnMode     = UART_READ_RETURN_MODE_FULL;
        gUartReadInitObject[0].hwFlowControl      = FALSE;
        gUartReadInitObject[0].hwFlowControlThr   = UART_RXTRIGLVL_16;
        gUartReadInitObject[0].transferMode       = UART_CONFIG_MODE_POLLED;
        gUartReadInitObject[0].intrNum            = ptr->intrNum;
        gUartReadInitObject[0].intrPriority       = 4U;
        gUartReadInitObject[0].operMode           = ptr->operMode;
        gUartReadInitObject[0].rxTrigLvl          = UART_RXTRIGLVL_8;
        gUartReadInitObject[0].txTrigLvl          = UART_TXTRIGLVL_32;
        gUartReadInitObject[0].clockP_get         = ClockP_getTicks;
        gUartReadInitObject[0].clockP_usecToTick  = ClockP_usecToTicks;
        gUartReadInitObject[0].writeCompleteCallbackFxn = NULL;
        gUartReadInitObject[0].readCompleteCallbackFxn  = NULL;

    int32_t status = UART_STATUS_SUCCESS;

    gUartReadHandleLld[0]             = &gUartReadObject[0];
    gUartReadHandleLld[0]->state      = UART_STATE_RESET;
    gUartReadHandleLld[0]->baseAddr   = ptr->baseaddr;
    gUartReadHandleLld[0]->hUartInit = &gUartReadInitObject[0];

    status += UART_lld_init(gUartReadHandleLld[0]);

    DebugP_assert(status == UART_STATUS_SUCCESS);

}

void uart_read_step( const void *dataOutput, uint32_t outputLength,  uint8_t dataType)
{

    uint32_t translength = outputLength;
    UART_Transaction trans;
    UART_lld_Transaction_init(&trans);
    uint8_t UartBuffer[outputLength];
    
    uint8_t *data_output = (uint8_t *)dataOutput;
    trans.buf = &UartBuffer[0U];
    trans.count = outputLength;
    UART_lld_read(gUartReadHandleLld[0], trans.buf, trans.count, trans.timeout, NULL);
   
   if(dataType!= 9)
   {
    for(int i=0; i<outputLength; i++)
    {
        *data_output = UartBuffer[i] - 48;
        data_output++;
    }
   }

}

void uart_read_release()
{
    int32_t status = UART_STATUS_SUCCESS;
    status = UART_lld_deInit(gUartReadHandleLld[0]);
    DebugP_assert(status == UART_STATUS_SUCCESS);
    
}
