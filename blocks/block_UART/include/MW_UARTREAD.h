#ifndef UART_MWREAD_H_
#define UART_MWREAD_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/uart.h>
#include <drivers/uart/v0/lld/dma/uart_dma.h>
#include <drivers/uart/v0/lld/dma/edma/uart_dma_edma.h>
#include <drivers/uart/v0/lld/uart_lld.h>
#include <drivers/edma.h>
#include <drivers/soc.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/ClockP.h>

#define CONFIG_UART_NUM_LLD_INSTANCES (1U)
#define CONFIG_UART_NUM_DMA_LLD_INSTANCES (0U)

typedef struct 
{
    uint32_t baudRate;
    uint32_t dataLength;
    uint32_t stopBits;
    uint32_t parityType;
    uint32_t transferMode;
    uint32_t intrNum;
    uint8_t  intrPriority;
    uint32_t operMode;
    uint32_t rxTrigLvl;
    uint32_t txTrigLvl;
    uint32_t readReturnMode;
    uint32_t baseaddr;
}UARTReadSetupStruct;


extern UARTLLD_Object gUartObject[CONFIG_UART_NUM_LLD_INSTANCES];
extern UARTLLD_Handle gUartHandleLld[CONFIG_UART_NUM_LLD_INSTANCES];
extern UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_LLD_INSTANCES];
extern UART_EdmaChConfig gUartDmaChCfg[CONFIG_UART_NUM_LLD_INSTANCES];


extern void uart_read_setup(UARTReadSetupStruct *ptr);
extern void uart_read_step( const void *dataOutput, uint32_t outputLength,  uint8_t dataType);
extern void uart_read_release();

#endif