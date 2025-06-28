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
#include "MW_UART.h"


#include "MW_target_hardware_resources.h"



extern UARTLLD_Handle gUartHandleLld[CONFIG_UART_NUM_LLD_INSTANCES];
extern UARTLLD_Object gUartObject[CONFIG_UART_NUM_LLD_INSTANCES];
extern UARTLLD_InitObject gUartInitObject[CONFIG_UART_NUM_LLD_INSTANCES];


void uart_read_setup(uint8_t instance)
{
    
}

void uart_write_setup(uint8_t instance)
{
    
            
}



void uart_read_step( const void *dataOutput, uint16_t outputLength,  uint8_t dataType, uint8_t instance)
{
    bool transfer_polling = false;
    int32_t transferOK;
    char UartBuffer[1024];
    uint8_t *readBuffer;
    uint32_t readCount, readSizeRemaining;
    uint32_t  readSuccess;
    uint8_t  *readBuf, readChar;
    uint32_t uartBaseAddrRead = CSL_UART0_U_BASE + (instance*0x1000);
    uint8_t *bytestoread = (uint8_t *)outputLength;
   
         
    if(instance == 0 && (MW_UART0_TRANSFER_MODE_SEL == 0))
        transfer_polling = true;
    else if(instance == 0 && (MW_UART0_TRANSFER_MODE_SEL == 1))
        transfer_polling = false;
    if(instance == 1 && (MW_UART1_TRANSFER_MODE_SEL == 0))
        transfer_polling = true;
    else if(instance == 1 && (MW_UART1_TRANSFER_MODE_SEL == 1))
        transfer_polling = false;
    if(instance == 2 && (MW_UART2_TRANSFER_MODE_SEL == 0))
        transfer_polling = true;
    else if(instance == 2 && (MW_UART2_TRANSFER_MODE_SEL == 1))
        transfer_polling = false;
    if(instance == 3 && (MW_UART3_TRANSFER_MODE_SEL == 0))
        transfer_polling = true;
    else if(instance == 3 && (MW_UART3_TRANSFER_MODE_SEL == 1))
        transfer_polling = false;
    if(instance == 4 && (MW_UART4_TRANSFER_MODE_SEL == 0))
        transfer_polling = true;
    else if(instance == 4 && (MW_UART4_TRANSFER_MODE_SEL == 1))
        transfer_polling = false;
    if(instance == 5 && (MW_UART5_TRANSFER_MODE_SEL == 0))
        transfer_polling = true;
    else if(instance == 5 && (MW_UART5_TRANSFER_MODE_SEL == 1))
        transfer_polling = false;

    UART_Transaction trans;
    UART_lld_Transaction_init(&trans);

    
    uint8_t *data_output = (uint8_t *)dataOutput;
    trans.buf = &UartBuffer[0U];
    trans.count = outputLength;
    if(transfer_polling == true)
    {
        if(instance == 0)
            UART_lld_read(gUartHandleLld[0], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 1)
            UART_lld_read(gUartHandleLld[1], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 2)
            UART_lld_read(gUartHandleLld[2], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 3)
            UART_lld_read(gUartHandleLld[3], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 4)
            UART_lld_read(gUartHandleLld[4], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 5)
            UART_lld_read(gUartHandleLld[5], trans.buf, trans.count, trans.timeout, NULL);
    }
    else
    {

        readBuffer   = &UartBuffer[0U];
        readCount  = 0U;
        readSizeRemaining = outputLength;

         uint32_t       intrType;
    
        while(readCount < readSizeRemaining)
        {
            intrType = UART_getIntrIdentityStatus(uartBaseAddrRead);

            if((intrType & UART_INTID_RX_THRES_REACH) == UART_INTID_RX_THRES_REACH)
            {
                
                readBuf = (uint8_t *)readBuffer + readCount;
                readSuccess = UART_getChar(uartBaseAddrRead, &readChar);
                if(readSuccess == true)
                {
                        *readBuf = readChar;
                        readBuf++;
                        // readSuccess = UART_getChar(uartBaseAddrRead, &readChar); 
                        readCount++;  
                }
            }
        }

    }


    for(int i=0; i<outputLength; i++)
    {
        *data_output = UartBuffer[i];
        data_output++;
    }
}

void uart_write_step(const void *dataInput, uint32_t inputVecLength, uint8_t dataType, uint8_t instance)
{
    bool transfer_polling_write = false;
    int32_t  transferOK;
    uint32_t i;
    uint8_t *writeBuffer;
    uint32_t writeSizeRemaining, writeCount;
    uint32_t uartBaseAddrWrite = CSL_UART0_U_BASE + (instance*0x1000);
    uint32_t   txTrigLevel;

    txTrigLevel = gUartInitObject[instance].txTrigLvl;
    

    if(instance == 0 && (MW_UART0_TRANSFER_MODE_SEL == 0))
        transfer_polling_write = true;
    else if(instance == 0 && (MW_UART0_TRANSFER_MODE_SEL == 1))
        transfer_polling_write = false;
    if(instance == 1 && (MW_UART1_TRANSFER_MODE_SEL == 0))
        transfer_polling_write = true;
    else if(instance == 1 && (MW_UART1_TRANSFER_MODE_SEL == 1))
        transfer_polling_write = false;
    if(instance == 2 && (MW_UART2_TRANSFER_MODE_SEL == 0))
        transfer_polling_write = true;
    else if(instance == 2 && (MW_UART2_TRANSFER_MODE_SEL == 1))
        transfer_polling_write = false;
    if(instance == 3 && (MW_UART3_TRANSFER_MODE_SEL == 0))
        transfer_polling_write = true;
    else if(instance == 3 && (MW_UART3_TRANSFER_MODE_SEL == 1))
        transfer_polling_write = false;
    if(instance == 4 && (MW_UART4_TRANSFER_MODE_SEL == 0))
        transfer_polling_write = true;
    else if(instance == 4 && (MW_UART4_TRANSFER_MODE_SEL == 1))
        transfer_polling_write = false;
    if(instance == 5 && (MW_UART5_TRANSFER_MODE_SEL == 0))
        transfer_polling_write = true;
    else if(instance == 5 && (MW_UART5_TRANSFER_MODE_SEL == 1))
        transfer_polling_write = false;
    
    UART_Transaction trans;
    UART_lld_Transaction_init(&trans);
 
    uint8_t char_read;
    uint32_t       intrType;
            
    trans.buf = (char *)dataInput;
    trans.count = inputVecLength;
    if(transfer_polling_write == true)
    {
        if(instance == 0)
            UART_lld_write(gUartHandleLld[0], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 1)
            UART_lld_write(gUartHandleLld[1], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 2)
            UART_lld_write(gUartHandleLld[2], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 3)
            UART_lld_write(gUartHandleLld[3], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 4)
            UART_lld_write(gUartHandleLld[4], trans.buf, trans.count, trans.timeout, NULL);
        else if(instance == 5)
            UART_lld_write(gUartHandleLld[5], trans.buf, trans.count, trans.timeout, NULL);
    }
    else
    {
        
        writeSizeRemaining = inputVecLength;
        writeBuffer = (char *)dataInput;
        writeCount         = 0U;

        intrType = UART_getIntrIdentityStatus(uartBaseAddrWrite);

        if((intrType & UART_INTID_TX_THRES_REACH) == UART_INTID_TX_THRES_REACH)
        {
            uint32_t       numBytesToTransfer;
            const uint8_t *writeBuf;

            while(writeSizeRemaining > 0U)
            {
                numBytesToTransfer = writeSizeRemaining;
                if(numBytesToTransfer > txTrigLevel)
                {
                    numBytesToTransfer = txTrigLevel;
                }
                writeSizeRemaining -= numBytesToTransfer;
                writeBuf = writeBuffer + writeCount;
                while(numBytesToTransfer != 0U)
                {
                    UART_putChar(uartBaseAddrWrite, *writeBuf);
                    writeBuf++;
                    numBytesToTransfer--;
                    writeCount++;
                }
            
            }
            return;
        }
    }
}

void uart_release()
{
    int32_t status = UART_STATUS_SUCCESS;
    status = UART_lld_deInit(gUartHandleLld[0]);
    DebugP_assert(status == UART_STATUS_SUCCESS);
}




        

