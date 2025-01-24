
#include "MW_CANREAD.h"


void CANRead_Step(CANREAD_step_struct *ptr, const void *data_len, const void *rx_data)
{
    MCAN_RxBufElement       rxMsg;
    MCAN_ErrCntStatus       errCounter;
    uint32_t                i, j, fifoStartIdx, bitPos = 0U;
    uint32_t                txStatus, loopCnt, fifoFillLvl, fifoCnt, bufNum, fifoNum, rxFIFOType;
    MCAN_RxFIFOStatus       fifoStatus;
    MCAN_RxNewDataStatus    newDataStatus;
    uint32_t msg_store, rx_cnt;
    uint32_t *dl = (uint32_t *)data_len;
    uint8_t *temp_rx = (uint8_t *)rx_data;
    bool received = false;
    uint32_t intrStatus;

     if(ptr->instance == 0)
    {
        if(ptr->rxSource == 0)
        {
            rx_cnt = MW_ELEMCNT_RXBUF_MCAN0_SEL;
            msg_store = 0;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 1)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO0_MCAN0_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 2)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO1_MCAN0_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_1;
        }
    }
    else if(ptr->instance == 1)
    {
        if(ptr->rxSource == 0)
        {
            rx_cnt = MW_ELEMCNT_RXBUF_MCAN1_SEL;
            msg_store = 0;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 1)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO0_MCAN1_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 2)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO1_MCAN1_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_1;
        }
    }
    else if(ptr->instance == 2)
    {
        if(ptr->rxSource == 0)
        {
            rx_cnt = MW_ELEMCNT_RXBUF_MCAN2_SEL;
            msg_store = 0;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 1)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO0_MCAN2_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 2)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO1_MCAN2_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_1;
        }
    }
    else if(ptr->instance == 3)
    {
        if(ptr->rxSource == 0)
        {
            rx_cnt = MW_ELEMCNT_RXBUF_MCAN3_SEL;
            msg_store = 0;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 1)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO0_MCAN3_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 2)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO1_MCAN3_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_1;
        }
    }
    else if(ptr->instance == 4)
    {
        if(ptr->rxSource == 0)
        {
            rx_cnt = MW_ELEMCNT_RXBUF_MCAN4_SEL;
            msg_store = 0;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 1)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO0_MCAN4_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 2)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO1_MCAN4_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_1;
        }
    }
    else if(ptr->instance == 5)
    {
        if(ptr->rxSource == 0)
        {
            rx_cnt = MW_ELEMCNT_RXBUF_MCAN5_SEL;
            msg_store = 0;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 1)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO0_MCAN5_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 2)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO1_MCAN5_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_1;
        }
    }
    else if(ptr->instance == 6)
    {
        if(ptr->rxSource == 0)
        {
            rx_cnt = MW_ELEMCNT_RXBUF_MCAN6_SEL;
            msg_store = 0;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 1)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO0_MCAN6_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 2)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO1_MCAN6_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_1;
        }
    }
    else if(ptr->instance == 7)
    {
        if(ptr->rxSource == 0)
        {
            rx_cnt = MW_ELEMCNT_RXBUF_MCAN7_SEL;
            msg_store = 0;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 1)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO0_MCAN7_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_0;
        }
        else if(ptr->rxSource == 2)
        {
            rx_cnt = MW_ELEMCNT_RXFIFO1_MCAN7_SEL;
            msg_store = 1;
            rxFIFOType = MCAN_RX_FIFO_NUM_1;
        }
    }



    if(msg_store == 0)
    {
        
            MCAN_getErrCounters(ptr->base, &errCounter);
            DebugP_assert((0U == errCounter.recErrCnt) &&
                      (0U == errCounter.canErrLogCnt));

            bufNum = ptr->buffernum;
            fifoNum = rxFIFOType;

            bitPos = (1U << bufNum);

            if(ptr->Enable_blockingrx)
            {
                do
                {
                    MCAN_getNewDataStatus(ptr->base, &newDataStatus);
                    MCAN_clearNewDataStatus(ptr->base, &newDataStatus);
                }while((newDataStatus.statusLow & bitPos) != bitPos);

                if (bitPos == (newDataStatus.statusLow & bitPos))
                {
                    MCAN_readMsgRam(ptr->base, MCAN_MEM_TYPE_BUF, bufNum, fifoNum, &rxMsg);
                   *dl = rxMsg.dlc;
         
                    for(i = 0; i<rxMsg.dlc; i++)
                    {
                        *temp_rx = (uint8_t)rxMsg.data[i];
                        temp_rx++;
                    }
                    
                }
       
            }
            else
            {
                 intrStatus = MCAN_getIntrStatus(ptr->base);
                 MCAN_clearIntrStatus(ptr->base, intrStatus);

                if (MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG ==
                    (intrStatus & MCAN_INTR_SRC_DEDICATED_RX_BUFF_MSG))
                {
                    MCAN_getNewDataStatus(ptr->base, &newDataStatus);
                    MCAN_clearNewDataStatus(ptr->base, &newDataStatus);

                    if (bitPos == (newDataStatus.statusLow & bitPos))
                    {
                        MCAN_readMsgRam(ptr->base, MCAN_MEM_TYPE_BUF, bufNum, fifoNum, &rxMsg);

                        *dl = rxMsg.dlc;
         
                        for(i = 0; i<rxMsg.dlc; i++)
                        {
                            *temp_rx = (uint8_t)rxMsg.data[i];
                            temp_rx++;
                        }
                     
                    }

                
                }
                  
            }
            
    }
    else if(msg_store == 1)
    {
      
        if(ptr->Enable_blockingrx)
        {
            do
            {
                MCAN_getRxFIFOStatus(ptr->base, &fifoStatus);
                
            }while(fifoStatus.fillLvl == 0);

            if(fifoStatus.fifoFull == 1)
            {
                MCAN_writeRxFIFOAck(ptr->base, fifoStatus.num,
                                        fifoStatus.getIdx); 
                MCAN_getRxFIFOStatus(ptr->base, &fifoStatus);
            }
                /* Checking for Rx Errors */
                MCAN_getErrCounters(ptr->base, &errCounter);
                DebugP_assert((0U == errCounter.recErrCnt) &&
                            (0U == errCounter.canErrLogCnt));

                MCAN_getRxFIFOStatus(ptr->base, &fifoStatus);
                MCAN_readMsgRam(ptr->base, MCAN_MEM_TYPE_FIFO, fifoStatus.getIdx,
                                fifoStatus.num, &rxMsg);

                *dl = rxMsg.dlc;
         
                for(i = 0; i<rxMsg.dlc; i++)
                {
                    *temp_rx = (uint8_t)rxMsg.data[i];
                    temp_rx++;
                }
               
            (void) MCAN_writeRxFIFOAck(ptr->base, fifoStatus.num,
                                        fifoStatus.getIdx);
            }
        
        else
        {
            
            intrStatus = MCAN_getIntrStatus(ptr->base);
            MCAN_clearIntrStatus(ptr->base, intrStatus);

            if ((MCAN_INTR_SRC_RX_FIFO0_NEW_MSG ==
                    (intrStatus &  MCAN_INTR_SRC_RX_FIFO0_NEW_MSG)) | (MCAN_INTR_SRC_RX_FIFO1_NEW_MSG ==
                    (intrStatus &  MCAN_INTR_SRC_RX_FIFO1_NEW_MSG)))
            {
            MCAN_getRxFIFOStatus(ptr->base, &fifoStatus);

            if(fifoStatus.fifoFull == 1)
            {
                MCAN_writeRxFIFOAck(ptr->base, fifoStatus.num,
                                        fifoStatus.getIdx); 
                MCAN_getRxFIFOStatus(ptr->base, &fifoStatus);
            }
               
                /* Checking for Rx Errors */
                MCAN_getErrCounters(ptr->base, &errCounter);
                DebugP_assert((0U == errCounter.recErrCnt) &&
                                (0U == errCounter.canErrLogCnt));

                MCAN_getRxFIFOStatus(ptr->base, &fifoStatus);
                MCAN_readMsgRam(ptr->base, MCAN_MEM_TYPE_FIFO, fifoStatus.getIdx,
                                    fifoStatus.num, &rxMsg);
                
                (void) MCAN_writeRxFIFOAck(ptr->base, fifoStatus.num,
                                            fifoStatus.getIdx);
                *dl = rxMsg.dlc;
         
                for(i = 0; i<rxMsg.dlc; i++)
                {
                    *temp_rx = (uint8_t)rxMsg.data[i];
                    temp_rx++;
                }
            }
                

        }
    }

 
    *dl = rxMsg.dlc;
         
    for(i = 0; i<rxMsg.dlc; i++)
    {
        *temp_rx = (uint8_t)rxMsg.data[i];
        temp_rx++;
    }
    

} 

void CANREAD_release(uint32_t base)
{
    MCAN_reset(base);
}


  
