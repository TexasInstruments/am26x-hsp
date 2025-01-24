#ifndef MY_MAIN_H_
#define MY_MAIN_H_


#include <stdint.h>
#include <drivers/hw_include/cslr_soc.h>
#include <drivers/uart.h>
#include <drivers/soc.h>
#include <kernel/dpl/CycleCounterP.h>
#include <drivers/uart.h>
#include <drivers/gpio.h>
#include <kernel/dpl/HwiP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/MpuP_armv7.h>
#include <kernel/dpl/ClockP.h>

/*
 * Common Functions
 */
void System_init(void);



#define CONFIG_UART0 (0U)
#define CONFIG_UART_NUM_INSTANCES (1U)
#define CONFIG_UART_NUM_DMA_INSTANCES (0U)



/*
 * Common Functions
 */
void Drivers_open(void);
void Drivers_close(void);

/*
 * UART
 */


/* UART Driver handles */
extern UART_Handle gUartHandle[CONFIG_UART_NUM_INSTANCES];

/*
 * UART Driver Advance Parameters - to be used only when Driver_open() and
 * Driver_close() is not used by the application
 */
/* UART Driver Parameters */
extern UART_Params gUartParams[CONFIG_UART_NUM_INSTANCES];
/* UART Driver open/close - can be used by application when Driver_open() and
 * Driver_close() is not used directly and app wants to control the various driver
 * open/close sequences */
void Drivers_uartOpen(void);
void Drivers_uartClose(void);





void __disable_irq(void);

void __enable_irq(void);

void am263px_My_HardwareInit_Fcn(void);

void TickConfig(float modelBaseRate);


#endif /* MY_MAIN_H_ */