/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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

#ifndef _rtiostreammcan
#include "rtwtypes.h"
#include "rtiostream_can.h"
#include <stdint.h>
#define _rtiostreammcan
#endif
#include "MW_target_hardware_resources.h"
#include <kernel/dpl/AddrTranslateP.h>


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#define FIFO_SIZE           (512U)
#define STD_FILT_CNT        (1U)
#define EXT_FILT_CNT        (1U)
#define TX_BUF_CNT          (0U)
#define TX_FIFO_CNT         (1U)
#define TX_EVENT_FIFO_CNT   (0U)
#define RX_FIFO0_CNT        (1U)
#define RX_FIFO1_CNT        (0U)
#define MCAN_EXT_ID_MASK    (0x1FFFFFFFU)

#define APP_MCAN_EXT_ID_MASK                     (0x1FFFFFFFU)
#define APP_MCAN_STD_ID_MASK                     (0x7FFU)
#define APP_MCAN_STD_ID_SHIFT                    (18U)


typedef struct {
    char buf[FIFO_SIZE];
    int  head;
    int tail;
    int dataLost;
   
} mw_fifo_t;

MCAN_BitTimingParams gMcanBitTimingDefaultParams;
MCAN_InitParams            XCPinitParams = {0U};
MCAN_ConfigParams          XCPconfigParams = {0U};
MCAN_MsgRAMConfigParams    msgRAMXCPconfigParams = {0U};
MCAN_BitTimingParams       XCPbitTimes = {0U};
MCAN_ExtMsgIDFilterElement extXCPFiltElem[EXT_FILT_CNT] = {0U};
MCAN_StdMsgIDFilterElement stdXCPFiltElem[STD_FILT_CNT] = {0U};

static void App_mcanUserISR();
static void mw_push_fifo(char rx);
static int mw_pop_fifo(char *rx);
static bool rxDone;
static mw_fifo_t  extRxFifo;
void MCAN_initialize();
void MCAN_bitrateinit();
void App_xcpmcanEnableIntr();
void mcanEnableTransceiver();

int rtIOStreamOpen(int argc, void * argv[])
{
    // mcanEnableTransceiver();
    MCAN_bitrateinit();
    mcanEnableTransceiver();
    MCAN_initialize();

    // MCAN_TxBufElement txMsg;
    // uint32_t j;
    // // uint8_t *dataPtr = (uint8_t *)src;
    // MCAN_initTxBufElement(&txMsg);
    // int32_t status = SystemP_SUCCESS;
    // uint32_t bufIndx = 0, bitPos = 0U, txStatus;
    // uint32_t baseAddr = (uint32_t)CSL_MCAN0_MSG_RAM_U_BASE;
  
    // if(!MW_CONNECTIONINFO_CAN_ISCANIDEXTENDED)
    //     txMsg.id = (((uint32_t)(MW_CONNECTIONINFO_CAN_CANIDCOMMAND) & APP_MCAN_STD_ID_MASK) << APP_MCAN_STD_ID_SHIFT);
    // else
    //     txMsg.id = ((uint32_t)(MW_CONNECTIONINFO_CAN_CANIDCOMMAND) & MCAN_EXT_ID_MASK);

    // txMsg.dlc = (uint32_t)11;
    // txMsg.fdf = false;
    // if(MW_CONNECTIONINFO_CAN_ISCANIDEXTENDED)
    //     txMsg.xtd = true;
    // else
    //     txMsg.xtd = false;

    // for(j=0; j<(uint32_t)txMsg.dlc; j++)
    // {
    //     txMsg.data[j] = 11;
    //     // dataPtr++;
    // }

    // MCAN_writeMsgRam(baseAddr, MCAN_MEM_TYPE_BUF, bufIndx, &txMsg);

    // status = MCAN_txBufAddReq(baseAddr, bufIndx);
    // DebugP_assert(status == CSL_PASS);

    // //Poll for Tx completion
    // bitPos = (1U << bufIndx);

    // do
    // {
    //     txStatus = MCAN_getTxBufTransmissionStatus(baseAddr);
    // }while((txStatus & bitPos) != bitPos);

    

    return(RTIOSTREAM_NO_ERROR);
}

void MCAN_initialize()
{

    uint32_t i, j;
    int32_t  status = SystemP_SUCCESS;

    MCAN_initOperModeParams(&XCPinitParams);

    XCPinitParams.fdMode       = FALSE;
    XCPinitParams.brsEnable    = FALSE;

    MCAN_initGlobalFilterConfigParams(&XCPconfigParams);
    MCAN_initSetBitTimeParams(&XCPbitTimes);
    MCAN_initMsgRamConfigParams(&msgRAMXCPconfigParams);

    msgRAMXCPconfigParams.lss = STD_FILT_CNT;
    msgRAMXCPconfigParams.lse = EXT_FILT_CNT;
    msgRAMXCPconfigParams.txBufCnt = TX_BUF_CNT;
    msgRAMXCPconfigParams.txFIFOCnt = TX_FIFO_CNT;
    msgRAMXCPconfigParams.txBufMode = MCAN_TX_MEM_TYPE_BUF;
    msgRAMXCPconfigParams.txEventFIFOCnt = TX_EVENT_FIFO_CNT;
    msgRAMXCPconfigParams.rxFIFO0Cnt = RX_FIFO0_CNT;
    msgRAMXCPconfigParams.rxFIFO1Cnt = RX_FIFO1_CNT;
    msgRAMXCPconfigParams.rxFIFO0OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;
    msgRAMXCPconfigParams.rxFIFO1OpMode = MCAN_RX_FIFO_OPERATION_MODE_BLOCKING;

    status = MCAN_calcMsgRamParamsStartAddr(&msgRAMXCPconfigParams);
    DebugP_assert(status == CSL_PASS);

    if(!MW_CONNECTIONINFO_CAN_ISCANIDEXTENDED)
    {
        for(j = 0; j < STD_FILT_CNT; j++)
        {
            stdXCPFiltElem[j].sfid1 = (uint32_t)(MW_CONNECTIONINFO_CAN_CANIDCOMMAND);
            stdXCPFiltElem[j].sfid2 = (uint32_t)j;
            stdXCPFiltElem[j].sfec = MCAN_STD_FILT_ELEM_BUFFER;
            stdXCPFiltElem[j].sft = MCAN_STD_FILT_TYPE_RANGE;
        }
    }

    if(MW_CONNECTIONINFO_CAN_ISCANIDEXTENDED)
    {
        for(j = 0; j < EXT_FILT_CNT; j++)
        {
            extXCPFiltElem[j].efid1 = (uint32_t)(MW_CONNECTIONINFO_CAN_CANIDCOMMAND);
            extXCPFiltElem[j].efid2 = (uint32_t)j;
            extXCPFiltElem[j].efec = MCAN_EXT_FILT_ELEM_BUFFER;
            extXCPFiltElem[j].eft = MCAN_STD_FILT_TYPE_RANGE;
        }

    }

    while (FALSE == MCAN_isMemInitDone(CSL_MCAN0_MSG_RAM_U_BASE))
    {}

    MCAN_setOpMode(CSL_MCAN0_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_SW_INIT);
    while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(CSL_MCAN0_MSG_RAM_U_BASE))
    {}

    MCAN_init(CSL_MCAN0_MSG_RAM_U_BASE, &XCPinitParams);
    MCAN_config(CSL_MCAN0_MSG_RAM_U_BASE, &XCPconfigParams);
    MCAN_setBitTime(CSL_MCAN0_MSG_RAM_U_BASE, &XCPbitTimes);
    MCAN_msgRAMConfig(CSL_MCAN0_MSG_RAM_U_BASE, &msgRAMXCPconfigParams);
    MCAN_setExtIDAndMask(CSL_MCAN0_MSG_RAM_U_BASE, MCAN_EXT_ID_MASK);

                
    if(!MW_CONNECTIONINFO_CAN_ISCANIDEXTENDED)
    {
        for(j = 0; j < STD_FILT_CNT; j++)
        {
            MCAN_addStdMsgIDFilter(CSL_MCAN0_MSG_RAM_U_BASE, j, &stdXCPFiltElem[j]);
        }
    }
                        
    if(MW_CONNECTIONINFO_CAN_ISCANIDEXTENDED)
    {
        for(j = 0; j < EXT_FILT_CNT; j++)
        {
            MCAN_addExtMsgIDFilter(CSL_MCAN0_MSG_RAM_U_BASE, j, &extXCPFiltElem[j]);
        }

    }


    MCAN_setOpMode(CSL_MCAN0_MSG_RAM_U_BASE, MCAN_OPERATION_MODE_NORMAL);
    while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(CSL_MCAN0_MSG_RAM_U_BASE))
    {}

    App_xcpmcanEnableIntr();
}

void MCAN_bitrateinit()
{
    gMcanBitTimingDefaultParams.nomRatePrescalar = 0x3U;
    gMcanBitTimingDefaultParams.nomTimeSeg1 = 0X20U;
    gMcanBitTimingDefaultParams.nomTimeSeg2 = 0X5U;
    gMcanBitTimingDefaultParams.nomSynchJumpWidth = 0X0U;
    gMcanBitTimingDefaultParams.dataRatePrescalar = 0X1U;
    gMcanBitTimingDefaultParams.dataTimeSeg1 = 0X5U;
    gMcanBitTimingDefaultParams.dataTimeSeg2 = 0X0U;
    gMcanBitTimingDefaultParams.dataSynchJumpWidth = 0X0U;
}

void mcanEnableTransceiver()
{
    GPIO_setDirMode(CSL_GPIO0_U_BASE, 51, GPIO_DIRECTION_INPUT);
    uint32_t    gpioBaseAddr, pinNum;

    gpioBaseAddr = (uint32_t)AddrTranslateP_getLocalAddr(CSL_GPIO0_U_BASE);
    pinNum       = 51;

    GPIO_setDirMode(gpioBaseAddr, pinNum, GPIO_DIRECTION_OUTPUT);

    GPIO_pinWriteLow(gpioBaseAddr, pinNum);
}





void App_xcpmcanEnableIntr()
{
    HwiP_Params     hwiPrms;
    int32_t status;
    uint32_t baseAddr = (uint32_t)CSL_MCAN0_MSG_RAM_U_BASE; 
    HwiP_Object     gMcanHwiObject;

    MCAN_enableIntr(baseAddr, MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG , (uint32_t)TRUE);
    MCAN_enableIntr(baseAddr, MCAN_INTR_SRC_RES_ADDR_ACCESS, (uint32_t)FALSE);
    MCAN_selectIntrLine(baseAddr, MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG , MCAN_INTR_LINE_NUM_0);
    MCAN_enableIntrLine(baseAddr, MCAN_INTR_LINE_NUM_0, (uint32_t)TRUE);

    HwiP_Params_init(&hwiPrms);
    hwiPrms.intNum      = CSLR_R5FSS0_CORE0_INTR_MCAN0_MCAN_LVL_INT_0;
    hwiPrms.priority    = 4; 
    hwiPrms.callback    = &App_mcanUserISR;
    status              = HwiP_construct(&gMcanHwiObject, &hwiPrms);
}

int rtIOStreamSend(int streamID, const void *src, size_t size, size_t *sizeSent)
{

    MCAN_TxBufElement txMsg;
    uint32_t j;
    char *dataPtr = (const char*)src;
    MCAN_initTxBufElement(&txMsg);
    int32_t status = SystemP_SUCCESS;
    uint32_t bufIndx = 0, bitPos = 0U, txStatus;
    uint32_t baseAddr = (uint32_t)CSL_MCAN0_MSG_RAM_U_BASE;
  

   if(!MW_CONNECTIONINFO_CAN_ISCANIDEXTENDED)
        txMsg.id = (((uint32_t)(MW_CONNECTIONINFO_CAN_CANIDRESPONSE) & APP_MCAN_STD_ID_MASK) << APP_MCAN_STD_ID_SHIFT);
    else
        txMsg.id = ((uint32_t)(MW_CONNECTIONINFO_CAN_CANIDRESPONSE) & MCAN_EXT_ID_MASK);
    txMsg.dlc = (uint32_t)size;
    txMsg.fdf = false;
    if(MW_CONNECTIONINFO_CAN_ISCANIDEXTENDED)
        txMsg.xtd = true;
    else
        txMsg.xtd = false;

    for(j=0; j<(uint32_t)size; j++)
    {
        txMsg.data[j] = (char)(*dataPtr);
        dataPtr++;
    }

    MCAN_writeMsgRam(baseAddr, MCAN_MEM_TYPE_BUF, bufIndx, &txMsg);

    status = MCAN_txBufAddReq(baseAddr, bufIndx);
    DebugP_assert(status == CSL_PASS);

    //Poll for Tx completion
    bitPos = (1U << bufIndx);

    do
    {
        txStatus = MCAN_getTxBufTransmissionStatus(baseAddr);
    }while((txStatus & bitPos) != bitPos);

    *sizeSent = size;
    
    return RTIOSTREAM_NO_ERROR;

}

char gbl_recv_buf[FIFO_SIZE];
uint16_t ReceErrorCnt = 0; 
int gbl_recv_len=0;
int gbl_idx=0;

int rtIOStreamRecv(int streamID, void *dst, size_t size, size_t *sizeRecvd)
{
    uint32_t bufNum = 0U, fifoNum = 0U, i, recvLength;
    uint32_t bitPos;
    MCAN_RxNewDataStatus    newDataStatus;
    MCAN_RxBufElement       rxMsg;
    // char *ptr = (char *)dst;

    uint32_t baseAddr = (uint32_t)CSL_MCAN0_MSG_RAM_U_BASE;

    // //Code 1
    // {
    // bitPos = (1U << bufNum);

    // do
    // {
    //     MCAN_getNewDataStatus(baseAddr, &newDataStatus);
    //     MCAN_clearNewDataStatus(baseAddr, &newDataStatus);
    // }while((newDataStatus.statusLow & bitPos) != bitPos);

    // if (bitPos == (newDataStatus.statusLow & bitPos))
    // {
    //     MCAN_readMsgRam(baseAddr, MCAN_MEM_TYPE_BUF, bufNum, fifoNum, &rxMsg);

    //     if(rxMsg.dlc <= (uint32_t)size)
    //         recvLength = rxMsg.dlc;
    //     else
    //         recvLength = (uint32_t)size;

    //     for(i = 0; i<recvLength; i++)
    //     {
    //         *ptr = (char)rxMsg.data[i];
    //         ptr++;
    //     }
                    
    // }
    // }

    // //Code 2
    // {

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

    // }

}

static void App_mcanUserISR()
{
    uint32_t intrStatus, bufNum = 0U, bitPos = 0U, fifoNum = 0U, recvLength, cnt = 0U;
    uint32_t baseAddr = (uint32_t)CSL_MCAN0_MSG_RAM_U_BASE;
    MCAN_RxNewDataStatus    newDataStatus;
    MCAN_RxBufElement       rxMsg;


    intrStatus = MCAN_getIntrStatus(baseAddr);
    MCAN_clearIntrStatus(baseAddr, intrStatus);

    if((intrStatus & MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG) == MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG)
    {
        MCAN_getNewDataStatus(baseAddr, &newDataStatus);
        MCAN_clearNewDataStatus(baseAddr, &newDataStatus); 

        bitPos = (1U << bufNum);

        while(bitPos == (newDataStatus.statusLow & bitPos))
        {
            MCAN_readMsgRam(baseAddr, MCAN_MEM_TYPE_BUF, bufNum, fifoNum, &rxMsg); 
            recvLength = rxMsg.dlc;
            cnt = 0U;
            while(cnt < recvLength)
            {
                mw_push_fifo((char)(rxMsg.data[cnt]));
                cnt++;
            } 

            MCAN_getNewDataStatus(baseAddr, &newDataStatus);
            MCAN_clearNewDataStatus(baseAddr, &newDataStatus);   
        }   
    }

    return;
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

int rtIOStreamClose(int streamID)
{
    return RTIOSTREAM_NO_ERROR;
}




