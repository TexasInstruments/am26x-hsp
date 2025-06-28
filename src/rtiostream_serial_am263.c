/*
 *  Copyright (C) 2025 Texas Instruments Incorporated
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

/*
 * Standard headers.
 */

#ifndef _rtiostream
#include "rtwtypes.h"
#include "rtiostream.h"
#include <stdint.h>
#define _rtiostream
#endif
#include "MW_target_hardware_resources.h"


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif



#define FIFO_SIZE  512

typedef struct {
    char buf[FIFO_SIZE];
    int  head;
    int tail;
    int dataLost;
   
} mw_fifo_t;

typedef struct
{
    
    uint32_t                baseAddr;
    /*< UART base address */
    uint32_t                txTrigLvl;
    /**< TX FIFO Trigger Level */
    uint32_t                rxTrigLvl;
   
    HwiP_Object             hwiObject;
    /**< HWI object */
    SemaphoreP_Object       writeDoneSem;
    /**< Write done semaphore */

    /*
     * UART write state variables - set for every transfer
     */
    const uint8_t          *writeBuf;
    /**< Buffer data pointer */
    volatile uint32_t       writeCount;
    /**< Number of Chars sent */
    volatile uint32_t       writeSizeRemaining;
    /**< Chars remaining in buffer */
    volatile uint32_t       txDataSent;
    /**< Flag to indicate all the data written to the TX FIFO */

    /*
     * UART receive state variables - set for every transfer
     */
    uint8_t                *readBuf;
    /**< Read Buffer data pointer */
    volatile uint32_t       readCount;
    /**< Number of Chars read */
    volatile uint32_t   readBytes;
    volatile uint32_t       rxOverflow;
   
} UART_AppPrms;

static UART_AppPrms gAppPrms;
UART_AppPrms *appPrms = &gAppPrms;

// extern double xcpDummyDoubleVariable;

static mw_fifo_t  extRxFifo;
static uint8_t  gAppUartRxBuffer[1024];
UARTLLD_Object rtiostreamUartObject;
UARTLLD_Handle rtiostreamUartHandleLld;
UARTLLD_InitObject rtiostreamUartInitObject;
UARTLLD_Handle gUartHandle0;
uint32_t rtiostreamUartConfigNum = 0;
extern void My_HardwareInit_Fcn();
static void App_uartInit(UART_AppPrms *appPrms);
static void App_uartUserISR(void *arg);
static void mw_push_fifo(char rx);
static int mw_pop_fifo(char *rx);
static bool rxDone;
bool flag = false;

void uart_rtiostream_init();
int rtIOStreamOpen(int argc, void* argv[])
{
    #ifdef MW_EXTMODE_RUNNING
        flag = true;
    #endif

    if(flag == false)
    {
       am263x_HardwareInit_Fcn();
    }
  
    extRxFifo.head = 0;
    extRxFifo.tail = 0;
    extRxFifo.dataLost = 0;
    // xcpDummyDoubleVariable = 1;
    uart_rtiostream_init();
    App_uartInit(appPrms);
    UART_intrEnable(appPrms->baseAddr, UART_INTR_RHR_CTI);

    return(RTIOSTREAM_NO_ERROR);
}

void uart_rtiostream_init()
{
        char UartBuffer[1024];
        int32_t transferOk;
        int32_t status = UART_STATUS_SUCCESS;
        rtiostreamUartInitObject.inputClkFreq       = 48000000U;
        rtiostreamUartInitObject.baudRate           = 115200;
        rtiostreamUartInitObject.dataLength         = UART_LEN_8;
        rtiostreamUartInitObject.stopBits           = UART_STOPBITS_1;
        rtiostreamUartInitObject.parityType         = UART_PARITY_NONE;
        rtiostreamUartInitObject.readReturnMode     = UART_READ_RETURN_MODE_FULL;
        rtiostreamUartInitObject.hwFlowControl      = FALSE;
        rtiostreamUartInitObject.hwFlowControlThr   = UART_RXTRIGLVL_1;
        rtiostreamUartInitObject.transferMode       = UART_CONFIG_MODE_USER_INTR;
        rtiostreamUartInitObject.intrNum            = 38U;
        rtiostreamUartInitObject.intrPriority       = 4U;
        rtiostreamUartInitObject.operMode           = UART_OPER_MODE_16X;
        rtiostreamUartInitObject.rxTrigLvl          = UART_RXTRIGLVL_1;
        rtiostreamUartInitObject.txTrigLvl          = UART_TXTRIGLVL_32;
        rtiostreamUartInitObject.clockP_get         = ClockP_getTicks;
        rtiostreamUartInitObject.clockP_usecToTick  = ClockP_usecToTicks;
        rtiostreamUartInitObject.writeCompleteCallbackFxn = NULL;
        rtiostreamUartInitObject.readCompleteCallbackFxn  = NULL;

        rtiostreamUartHandleLld = &rtiostreamUartObject;
        rtiostreamUartHandleLld->state = UART_STATE_RESET;
        rtiostreamUartHandleLld->baseAddr = CSL_UART0_U_BASE;
        rtiostreamUartHandleLld->hUartInit = &rtiostreamUartInitObject;

        status += UART_lld_init(rtiostreamUartHandleLld);

        return;

}

static void App_uartInit(UART_AppPrms *appPrms)
{
    int32_t         status;
    HwiP_Params     hwiPrms;

    /* Get UART Instance Base Address */
    appPrms->baseAddr = rtiostreamUartHandleLld->baseAddr;
    appPrms->rxTrigLvl = rtiostreamUartInitObject.rxTrigLvl;
    appPrms->txTrigLvl = rtiostreamUartInitObject.txTrigLvl;

    /* Reset other variables */
    appPrms->writeBuf           = NULL;
    appPrms->writeSizeRemaining = 0U;
    appPrms->writeCount         = 0U;
    appPrms->txDataSent         = FALSE;
    appPrms->readBuf            = NULL;
    appPrms->readCount          = 0U;
    appPrms->rxOverflow         = FALSE;

    /* Register interrupt */
    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = rtiostreamUartInitObject.intrNum;
    hwiPrms.priority    = rtiostreamUartInitObject.intrPriority;
    hwiPrms.callback    = &App_uartUserISR;
    hwiPrms.args        = (void *) appPrms;
    status              = HwiP_construct(&appPrms->hwiObject, &hwiPrms);
    

  
}

int rtIOStreamSend(int streamID, const void *src, size_t size, size_t *sizeSent)
{

    int32_t          transferOK;

    UART_Transaction trans;
    UART_lld_Transaction_init(&trans);
    trans.buf = (const char*)src;
    trans.count = (uint32_t)size;
    transferOK = UART_lld_write(rtiostreamUartHandleLld, trans.buf, trans.count, trans.timeout, NULL);

    *sizeSent = size;
    
    return RTIOSTREAM_NO_ERROR;
     
}

char gbl_recv_buf[FIFO_SIZE];
uint16_t ReceErrorCnt = 0; 
int gbl_recv_len=0;
int gbl_idx=0;

int rtIOStreamRecv(int streamID, void *dst, size_t size, size_t *sizeRecvd)
{
    char *ptr = (char *)dst;
    char rx;
    *sizeRecvd = 0;
    rxDone = false;

    while(*sizeRecvd < (uint32_t)size)
    {
        if(mw_pop_fifo(&rx))
        {
            *ptr++ = rx;
            *sizeRecvd += 1;
            if(gbl_recv_len < sizeof(gbl_recv_buf)/sizeof(gbl_recv_buf[0]))
            {
                gbl_recv_buf[gbl_recv_len++] = rx;
            }
        }
        else
        {
            break;
        }
    }


    return RTIOSTREAM_NO_ERROR;

}

int rtIOStreamClose(int streamID)
{
    return RTIOSTREAM_NO_ERROR;
}

void mw_push_fifo(char rx)
{
    int next;
    int nextp1;
    next =(extRxFifo.tail + 1)%FIFO_SIZE;
    nextp1 = (next+1)%FIFO_SIZE;
    if (nextp1 == extRxFifo.head) {
        extRxFifo.dataLost = 1;
    }
    else {
        extRxFifo.buf[extRxFifo.tail] = rx;
        extRxFifo.tail = next;
    }
}

int mw_pop_fifo(char *rx)
{
   if (extRxFifo.head == extRxFifo.tail) {
       *rx = '\0';
       return FALSE;
   }
   else {
       *rx = extRxFifo.buf[extRxFifo.head];
       extRxFifo.head = (extRxFifo.head+1)%FIFO_SIZE;
       return TRUE;
   }     
}

static void App_uartUserISR(void *arg)
{
    UART_AppPrms  *appPrms     = (UART_AppPrms *) arg;
    uint32_t       intrType;

    intrType = UART_getIntrIdentityStatus(appPrms->baseAddr);

    /* Check RX FIFO threshold is set */
    if((intrType & UART_INTID_RX_THRES_REACH) == UART_INTID_RX_THRES_REACH)
    {
        uint32_t       readSuccess;
        uint8_t       *readBuf, readChar;

        readSuccess = UART_getChar(appPrms->baseAddr, &readChar);
        while((readSuccess == true))
        {
                mw_push_fifo(readChar);
                readSuccess = UART_getChar(appPrms->baseAddr, &readChar);   
        }
      
    }


    return;
}

