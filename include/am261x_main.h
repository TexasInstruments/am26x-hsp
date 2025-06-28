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
void am261x_HardwareInit_Fcn(void);





void __disable_irq(void);

void __enable_irq(void);

// void My_HardwareInit_Fcn(void);

void TickConfig(float modelBaseRate);


#endif /* MY_MAIN_H_ */