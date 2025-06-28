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
#include "MW_CAN.h"
#include "MW_target_hardware_resources.h"

#define APP_MCAN_EXT_ID_MASK                     (0x1FFFFFFFU)
#define APP_MCAN_STD_ID_MASK                     (0x7FFU)
#define APP_MCAN_STD_ID_SHIFT                    (18U)

#define APP_MCAN_EXT_ID                          (0xD0U)
static const uint8_t gMcanDataSize[16U] = {0U,  1U,  2U,  3U,
                                           4U,  5U,  6U,  7U,
                                           8U,  12U, 16U, 20U,
                                           24U, 32U, 48U, 64U};
#define APP_MCAN_CLASSIC_BIT_MASK                (0xFFFFFFFFU);


extern MCAN_InitParams            initParams;
extern MCAN_ConfigParams          configParams;
extern MCAN_MsgRAMConfigParams    msgRAMConfigParams;
extern MCAN_BitTimingParams       bitTimes;
extern MCAN_ExtMsgIDFilterElement extFiltElem[MW_ELEMCNT_EXTFILT_MCAN0_SEL];
extern MCAN_StdMsgIDFilterElement stdFiltElem[MW_ELEMCNT_STDFILT_MCAN0_SEL];


void CANWRITE_step(uint32_t base, uint32_t instance, uint32_t txID, uint32_t len, const void* datainput, bool enable_blockingtx)
{

    uint32_t filter_cnt2 = 0;
    uint32_t i;
    uint32_t storeIndx, txStatus, bitPos = 0U;
    int32_t status1 = SystemP_SUCCESS;
    uint32_t msg_type, msg_storetx, can_mode, tx_fin_intr, tx_canc_intr;
    MCAN_ProtocolStatus     protStatus;
    uint8_t b;
    uint8_t *tempdataInput = (uint8_t *)datainput;
    
    if(instance == 0)
    {
        if(MW_STORE_TX_MSG_MCAN0_SEL == 0)
        {
            filter_cnt2 = MW_ELEMCNT_TXBUF_MCAN0_SEL;
            msg_storetx = 0;
        }
        else
        {
            filter_cnt2 = MW_ELEMCNT_TXFIFO_MCAN0_SEL;  
            msg_storetx = 1;
        }

        if(MW_ENABLE_STDFILT_MCAN0_SEL == 1)
            msg_type = 0;
        else if(MW_ENABLE_EXTFILT_MCAN0_SEL == 1)
            msg_type = 1;

        if(MW_TX_COMPLETED_INTR_MCAN0_SEL == 1)
            tx_fin_intr = 1;
        else if(MW_TX_COMPLETED_INTR_MCAN0_SEL == 0)
            tx_fin_intr = 0;

        if(MW_TX_CANCELFIN_INTR_MCAN0_SEL == 1)
            tx_canc_intr = 1;
        else if(MW_TX_CANCELFIN_INTR_MCAN0_SEL == 0)
            tx_canc_intr = 0;

        can_mode = MW_PROTOCOL_MCAN0_SEL;

    }
    else if(instance == 1)
    {
        if(MW_STORE_TX_MSG_MCAN1_SEL == 0)
        {
            filter_cnt2 = MW_ELEMCNT_TXBUF_MCAN1_SEL;
            msg_storetx = 0;
        }
        else
        {
            filter_cnt2 = MW_ELEMCNT_TXFIFO_MCAN1_SEL;  
            msg_storetx = 1;
        }  

        if(MW_ENABLE_STDFILT_MCAN1_SEL == 1)
            msg_type = 0;
        else if(MW_ENABLE_EXTFILT_MCAN1_SEL == 1)
            msg_type = 1;

        if(MW_TX_COMPLETED_INTR_MCAN1_SEL == 1)
            tx_fin_intr = 1;
        else if(MW_TX_COMPLETED_INTR_MCAN1_SEL == 0)
            tx_fin_intr = 0;

        if(MW_TX_CANCELFIN_INTR_MCAN1_SEL == 1)
            tx_canc_intr = 1;
        else if(MW_TX_CANCELFIN_INTR_MCAN1_SEL == 0)
            tx_canc_intr = 0;

        can_mode = MW_PROTOCOL_MCAN1_SEL;

    }
    else if(instance == 2)
    {
        if(MW_STORE_TX_MSG_MCAN2_SEL == 0)
        {
            filter_cnt2 = MW_ELEMCNT_TXBUF_MCAN2_SEL;
            msg_storetx = 0;
        }
        else
        {
            filter_cnt2 = MW_ELEMCNT_TXFIFO_MCAN2_SEL;  
            msg_storetx = 1;
        }  
        if(MW_ENABLE_STDFILT_MCAN2_SEL == 1)
            msg_type = 0;
        else if(MW_ENABLE_EXTFILT_MCAN2_SEL == 1)
            msg_type = 1;

        if(MW_TX_COMPLETED_INTR_MCAN2_SEL == 1)
            tx_fin_intr = 1;
        else if(MW_TX_COMPLETED_INTR_MCAN2_SEL == 0)
            tx_fin_intr = 0;

        if(MW_TX_CANCELFIN_INTR_MCAN2_SEL == 1)
            tx_canc_intr = 1;
        else if(MW_TX_CANCELFIN_INTR_MCAN2_SEL == 0)
            tx_canc_intr = 0;

        can_mode = MW_PROTOCOL_MCAN2_SEL;

    }
    else if(instance == 3)
    {
        if(MW_STORE_TX_MSG_MCAN3_SEL == 0)
        {
            filter_cnt2 = MW_ELEMCNT_TXBUF_MCAN3_SEL;
            msg_storetx = 0;
        }
        else
        {
            filter_cnt2 = MW_ELEMCNT_TXFIFO_MCAN3_SEL;  
            msg_storetx = 1;
        }  
        if(MW_ENABLE_STDFILT_MCAN3_SEL == 1)
            msg_type = 0;
        else if(MW_ENABLE_EXTFILT_MCAN3_SEL == 1)
            msg_type = 1;

        if(MW_TX_COMPLETED_INTR_MCAN3_SEL == 1)
            tx_fin_intr = 1;
        else if(MW_TX_COMPLETED_INTR_MCAN3_SEL == 0)
            tx_fin_intr = 0;

        if(MW_TX_CANCELFIN_INTR_MCAN3_SEL == 1)
            tx_canc_intr = 1;
        else if(MW_TX_CANCELFIN_INTR_MCAN3_SEL == 0)
            tx_canc_intr = 0;

        can_mode = MW_PROTOCOL_MCAN3_SEL;

    }
    else if(instance == 4)
    {
        if(MW_STORE_TX_MSG_MCAN4_SEL == 0)
        {
            filter_cnt2 = MW_ELEMCNT_TXBUF_MCAN4_SEL;
            msg_storetx = 0;
        }
        else
        {
            filter_cnt2 = MW_ELEMCNT_TXFIFO_MCAN4_SEL;  
            msg_storetx = 1;
        }  
        if(MW_ENABLE_STDFILT_MCAN4_SEL == 1)
            msg_type = 0;
        else if(MW_ENABLE_EXTFILT_MCAN4_SEL == 1)
            msg_type = 1;

        if(MW_TX_COMPLETED_INTR_MCAN4_SEL == 1)
            tx_fin_intr = 1;
        else if(MW_TX_COMPLETED_INTR_MCAN4_SEL == 0)
            tx_fin_intr = 0;

        if(MW_TX_CANCELFIN_INTR_MCAN4_SEL == 1)
            tx_canc_intr = 1;
        else if(MW_TX_CANCELFIN_INTR_MCAN4_SEL == 0)
            tx_canc_intr = 0;

        can_mode = MW_PROTOCOL_MCAN4_SEL;

    }
    else if(instance == 5)
    {
        if(MW_STORE_TX_MSG_MCAN5_SEL == 0)
        {
            filter_cnt2 = MW_ELEMCNT_TXBUF_MCAN5_SEL;
            msg_storetx = 0;
        }
        else
        {
            filter_cnt2 = MW_ELEMCNT_TXFIFO_MCAN5_SEL;  
            msg_storetx = 1;
        }  
        if(MW_ENABLE_STDFILT_MCAN5_SEL == 1)
            msg_type = 0;
        else if(MW_ENABLE_EXTFILT_MCAN5_SEL == 1)
            msg_type = 1;

        if(MW_TX_COMPLETED_INTR_MCAN5_SEL == 1)
            tx_fin_intr = 1;
        else if(MW_TX_COMPLETED_INTR_MCAN5_SEL == 0)
            tx_fin_intr = 0;

        if(MW_TX_CANCELFIN_INTR_MCAN5_SEL == 1)
            tx_canc_intr = 1;
        else if(MW_TX_CANCELFIN_INTR_MCAN5_SEL == 0)
            tx_canc_intr = 0;

        can_mode = MW_PROTOCOL_MCAN5_SEL;

    }
    else if(instance == 6)
    {
        if(MW_STORE_TX_MSG_MCAN6_SEL == 0)
        {
            filter_cnt2 = MW_ELEMCNT_TXBUF_MCAN6_SEL;
            msg_storetx = 0;
        }
        else
        {
            filter_cnt2 = MW_ELEMCNT_TXFIFO_MCAN6_SEL;  
            msg_storetx = 1;
        }  
        if(MW_ENABLE_STDFILT_MCAN6_SEL == 1)
            msg_type = 0;
        else if(MW_ENABLE_EXTFILT_MCAN6_SEL == 1)
            msg_type = 1;

        if(MW_TX_COMPLETED_INTR_MCAN6_SEL == 1)
            tx_fin_intr = 1;
        else if(MW_TX_COMPLETED_INTR_MCAN6_SEL == 0)
            tx_fin_intr = 0;

        if(MW_TX_CANCELFIN_INTR_MCAN6_SEL == 1)
            tx_canc_intr = 1;
        else if(MW_TX_CANCELFIN_INTR_MCAN6_SEL == 0)
            tx_canc_intr = 0;

        can_mode = MW_PROTOCOL_MCAN6_SEL;

    }
    else if(instance == 7)
    {
        if(MW_STORE_TX_MSG_MCAN7_SEL == 0)
        {
            filter_cnt2 = MW_ELEMCNT_TXBUF_MCAN7_SEL;
            msg_storetx = 0;
        }
        else
        {
            filter_cnt2 = MW_ELEMCNT_TXFIFO_MCAN7_SEL;  
            msg_storetx = 1;
        }  
        if(MW_ENABLE_STDFILT_MCAN7_SEL == 1)
            msg_type = 0;
        else if(MW_ENABLE_EXTFILT_MCAN7_SEL == 1)
            msg_type = 1;

        if(MW_TX_COMPLETED_INTR_MCAN7_SEL == 1)
            tx_fin_intr = 1;
        else if(MW_TX_COMPLETED_INTR_MCAN7_SEL == 0)
            tx_fin_intr = 0;

        if(MW_TX_CANCELFIN_INTR_MCAN7_SEL == 1)
            tx_canc_intr = 1;
        else if(MW_TX_CANCELFIN_INTR_MCAN7_SEL == 0)
            tx_canc_intr = 0;

        can_mode = MW_PROTOCOL_MCAN7_SEL;

    }
        

    MCAN_TxBufElement txMsg;
   
    uint8_t MsgData = 0;
    uint8_t j;

    if(msg_storetx == 0)
    {
        MCAN_initTxBufElement(&txMsg);
        if(msg_type == 1)
            txMsg.id = ((txID) & APP_MCAN_EXT_ID_MASK);
        else if(msg_type == 0)
            txMsg.id = (((txID) & APP_MCAN_STD_ID_MASK) << APP_MCAN_STD_ID_SHIFT);

        if(can_mode == 0)
        {
           txMsg.dlc = len;
           MsgData = MCAN_DATA_SIZE_64BYTES;
           txMsg.fdf = TRUE;
        }
        else if(can_mode == 1)
        {
           txMsg.dlc = len;
           MsgData = MCAN_DATA_SIZE_8BYTES;
           txMsg.fdf = FALSE;
        }

    if(msg_type == 0)  
      txMsg.xtd = FALSE;
    else
      txMsg.xtd = TRUE;

    for(j = 0U; j < len; j++)
    {
        b = (uint8_t)(*tempdataInput);
        txMsg.data[j] = b;
       tempdataInput++;

    }
    tempdataInput = datainput;
    storeIndx = 0U;
    if(tx_fin_intr == 1)
    {
        status1 = MCAN_txBufTransIntrEnable(base, storeIndx, (uint32_t)TRUE);
        DebugP_assert(status1 == CSL_PASS);
    }

    if(tx_canc_intr == 1)
    {
        status1 = MCAN_txBufCancellationIntrEnable(base, storeIndx, (uint32_t)TRUE);
        DebugP_assert(status1 == CSL_PASS);
    }

     MCAN_writeMsgRam(base, MCAN_MEM_TYPE_BUF, storeIndx, &txMsg);   

     status1 = MCAN_txBufAddReq(base, storeIndx);
            DebugP_assert(status1 == CSL_PASS); 

    bitPos = (1U << storeIndx);
            /* Poll for Tx completion */
    if(enable_blockingtx)
    {
        do
        {
            txStatus = MCAN_getTxBufTransmissionStatus(base);
        }while((txStatus & bitPos) != bitPos);

        MCAN_getProtocolStatus(base, &protStatus);
        /* Checking for Tx Errors */
        if (((MCAN_ERR_CODE_NO_ERROR != protStatus.lastErrCode) ||
            (MCAN_ERR_CODE_NO_CHANGE != protStatus.lastErrCode)) &&
            ((MCAN_ERR_CODE_NO_ERROR != protStatus.dlec) ||
            (MCAN_ERR_CODE_NO_CHANGE != protStatus.dlec)) &&
            (0U != protStatus.pxe))
        {
            DebugP_assert(FALSE);
        }

    }
    }
    else if(msg_storetx == 1)
    {
        uint8_t x = 0;
      
        MCAN_initTxBufElement(&txMsg);
        if(msg_type == 1)
            txMsg.id = ((txID) & APP_MCAN_EXT_ID_MASK);
        else if(msg_type == 0)
            txMsg.id = (((txID) & APP_MCAN_STD_ID_MASK) << APP_MCAN_STD_ID_SHIFT);

        if(can_mode == 0)
        {
           txMsg.dlc = len;
           MsgData = MCAN_DATA_SIZE_64BYTES;
           txMsg.fdf = TRUE;
        }
        else if(can_mode == 1)
        {
           txMsg.dlc = len;
           MsgData = MCAN_DATA_SIZE_8BYTES;
           txMsg.fdf = FALSE;
        }

    if(msg_type == 0)  
      txMsg.xtd = FALSE;
    else
      txMsg.xtd = TRUE;

    for(j = 0U; j < len; j++)
    {
        b = *(uint8_t *)tempdataInput;
        txMsg.data[j] = b;
       tempdataInput++;

    }
    tempdataInput = datainput;
    storeIndx = 0;

    if(tx_fin_intr == 1)
    {
        status1 = MCAN_txBufTransIntrEnable(base, storeIndx, (uint32_t)TRUE);
        DebugP_assert(status1 == CSL_PASS);
    }

    if(tx_canc_intr == 1)
    {
        status1 = MCAN_txBufCancellationIntrEnable(base, storeIndx, (uint32_t)TRUE);
        DebugP_assert(status1 == CSL_PASS);
    }

    
    MCAN_writeMsgRam(base, MCAN_MEM_TYPE_FIFO, storeIndx, &txMsg);  

    status1 = MCAN_txBufAddReq(base, storeIndx);
    DebugP_assert(status1 == CSL_PASS); 

    bitPos = (1U << storeIndx);
            /* Poll for Tx completion */
    if(enable_blockingtx)
    {
        do
        {
            txStatus = MCAN_getTxBufTransmissionStatus(base);
        }while((txStatus & bitPos) != bitPos);

        MCAN_getProtocolStatus(base, &protStatus);
        /* Checking for Tx Errors */
        if (((MCAN_ERR_CODE_NO_ERROR != protStatus.lastErrCode) ||
            (MCAN_ERR_CODE_NO_CHANGE != protStatus.lastErrCode)) &&
            ((MCAN_ERR_CODE_NO_ERROR != protStatus.dlec) ||
            (MCAN_ERR_CODE_NO_CHANGE != protStatus.dlec)) &&
            (0U != protStatus.pxe))
        {
            DebugP_assert(FALSE);
        }

    }
    }
}
       


void CANINTSTATUS_step(CANINTSTATUS_step_struct *ptr, const void* intstatus, const void* IR_reg)
{
    uint32_t intrmask_canintr = 0x0;
    uint32_t i, base, status, status_temp;
    bool *ptr_intr = (bool *)intstatus;
    uint32_t *ir_intr = (uint32_t *)IR_reg;
    status = MCAN_getIntrStatus(ptr->base);
    status_temp = status;
    uint32_t indx = 1;
    if(ptr->Interrupt_status)
    {
        *ir_intr = status_temp;
    
    }
    if(ptr->RXFIFO0newmsg)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RX_FIFO0_NEW_MSG);
        *ptr_intr =(bool)(status_temp & (1 << 0));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->RXFIFO0wtrmark)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RX_FIFO0_WATERMARK); 
        *ptr_intr =(bool)(status_temp & (1 << 1));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->RXFIFO0full)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RX_FIFO0_FULL);
        *ptr_intr =(bool)(status_temp & (1 << 2));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->RXFIFO0msglost)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RX_FIFO0_MSG_LOST);
        *ptr_intr =(bool)(status_temp & (1 << 3));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->RXFIFO1newmsg)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RX_FIFO1_NEW_MSG);
        *ptr_intr =(bool)(status_temp & (1 << 4));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->RXFIFO1wtrmark)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RX_FIFO1_WATERMARK);
        *ptr_intr =(bool)(status_temp & (1 << 5));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->RXFIFO1full)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RX_FIFO1_FULL); 
        *ptr_intr =(bool)(status_temp & (1 << 6));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->RXFIFO1msglost)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RX_FIFO1_MSG_LOST); 
        *ptr_intr =(bool)(status_temp & (1 << 7));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Highprioritymsg)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_HIGH_PRIO_MSG);
        *ptr_intr =(bool)(status_temp & (1 << 8));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->TXcomplete)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TRANS_COMPLETE);  
        *ptr_intr =(bool)(status_temp & (1 << 9));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->TXcancellationfin)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TRANS_CANCEL_FINISH);
        *ptr_intr =(bool)(status_temp & (1 << 10));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->TXFIFOempty)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TX_FIFO_EMPTY);
        *ptr_intr =(bool)(status_temp & (1 << 11));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->TXEventFIFOnewentry)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TX_EVT_FIFO_NEW_ENTRY);
        *ptr_intr =(bool)(status_temp & (1 << 12));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->TXEventFIFOwatermark)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TX_EVT_FIFO_WATERMARK);
        *ptr_intr =(bool)(status_temp & (1 << 13));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->TXEventFIFOfull)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TX_EVT_FIFO_FULL);
        *ptr_intr =(bool)(status_temp & (1 << 14));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->TXEventFIFOelementlost)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TX_EVT_FIFO_ELEM_LOST);
        *ptr_intr =(bool)(status_temp & (1 << 15));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Timestampwraparound)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TIMESTAMP_WRAPAROUND); 
        *ptr_intr =(bool)(status_temp & (1 << 16));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->MsgRAMacsfail)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_MSG_RAM_ACCESS_FAILURE); 
        *ptr_intr =(bool)(status_temp & (1 << 17));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Timeoutoccured)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_TIMEOUT);
        *ptr_intr =(bool)(status_temp & (1 << 18));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->MsgstrdedRxbuf)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG);
        *ptr_intr =(bool)(status_temp & (1 << 19));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Biterrorcorr)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_BIT_ERR_CORRECTED);
        *ptr_intr =(bool)(status_temp & (1 << 20));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Biterruncorr)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_BIT_ERR_UNCORRECTED);
        *ptr_intr =(bool)(status_temp & (1 << 21));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Errorloggingovf)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_ERR_LOG_OVRFLW);
        *ptr_intr =(bool)(status_temp & (1 << 22));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Errorpassive)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_ERR_PASSIVE);
        *ptr_intr =(bool)(status_temp & (1 << 23));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Warningstatus)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_WARNING_STATUS);
        *ptr_intr =(bool)(status_temp & (1 << 24));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Busoffstatus)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_BUS_OFF_STATUS);
        *ptr_intr =(bool)(status_temp & (1 << 25));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Watchdogintr)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_WATCHDOG);
        *ptr_intr =(bool)(status_temp & (1 << 26));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Protocolerrarbitphase)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_PROTOCOL_ERR_ARB);
        *ptr_intr =(bool)(status_temp & (1 << 27));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Protocolerrdataphase)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_PROTOCOL_ERR_DATA);
        *ptr_intr =(bool)(status_temp & (1 << 28));
        ptr_intr++;
        indx = indx + 1;
    }
    if(ptr->Accsresaddr)
    {
        intrmask_canintr = (intrmask_canintr | MCAN_INTR_SRC_RES_ADDR_ACCESS);
        *ptr_intr =(bool)(status_temp & (1 << 29));
        ptr_intr++;
        indx = indx + 1;
    }

    MCAN_clearIntrStatus(ptr->base, intrmask_canintr);

 

}

void CANWRITE_release(uint32_t base)
{
   MCAN_reset(base); 

}

