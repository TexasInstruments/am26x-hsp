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
#include "MW_IPCRX.h"
#include "MW_IPCTX.h"
#include "MW_target_hardware_resources.h"

#define MAX_MSG_SIZE        (100u)
uint64_t recvmsgBuf[MAX_MSG_SIZE];
static RPMessage_Object gRecvMsgObject;


void main_handler(RPMessage_Object *obj, void *args, void *data, uint16_t dataLength, int32_t crcStatus, uint16_t remoteCoreID, uint16_t srcEndPt)
{
    uint16_t msg_len = sizeof(recvmsgBuf);;
    memcpy((void *) recvmsgBuf,(const void *) data, (size_t)dataLength);
    // ti_sitara_ipc_isr();x
    ti_sitara_ipc_isr();
}


void IPCRX_setup(IPCRXSetup *ptr)
{
    int32_t status;

    RPMessage_CreateParams createParams;

    RPMessage_CreateParams_init(&createParams);
    createParams.localEndPt = ptr->end_point;
    createParams.recvCallback = main_handler;
    createParams.recvCallbackArgs = NULL;
    status = RPMessage_construct(&gRecvMsgObject, &createParams);
    DebugP_assert(status==SystemP_SUCCESS);

    IpcNotify_syncAll(SystemP_WAIT_FOREVER);
}

void IPCRX_step(IPCRXStep *ptr, const void* msg_val, uint8_t input_length)
{
    uint32_t msg_data;
    uint8_t *msg_uint8 = (uint8_t *)msg_val;
    uint8_t *msg1_uint8 = &recvmsgBuf[0U];
    int8_t *msg_int8 = (int8_t *)msg_val;
    int8_t *msg1_int8 = &recvmsgBuf[0U];
    uint16_t *msg_uint16 = (uint16_t *)msg_val;
    uint16_t *msg1_uint16 = &recvmsgBuf[0U];
    int16_t *msg_int16 = (int16_t *)msg_val;
    int16_t *msg1_int16 = &recvmsgBuf[0U];
    uint32_t *msg_uint32 = (uint32_t *)msg_val;
    uint32_t *msg1_uint32 = &recvmsgBuf[0U];
    int32_t *msg_int32 = (int32_t *)msg_val;
    int32_t *msg1_int32 = &recvmsgBuf[0U];
    


    for(uint8_t i =0; i<input_length; i++)
    {
        if(ptr->dataType == 0)
        {
            *msg_uint8 = (*msg1_uint8);
            msg_uint8++;
            msg1_uint8++;
        }
        else if(ptr->dataType == 1)
        {
            *msg_int8 = (*msg1_int8);
            msg_int8++;
            msg1_int8++;
        }
        else if(ptr->dataType == 2)
        {
            *msg_uint16 = (*msg1_uint16);
            msg_uint16++;
            msg1_uint16++;
        }
        else if(ptr->dataType == 3)
        {
            *msg_int16 = (*msg1_int16);
            msg_int16++;
            msg1_int16++;
        }
        else if(ptr->dataType == 4)
        {
            *msg_uint32 = (*msg1_uint32);
            msg_uint32++;
            msg1_uint32++;
        }
        else if(ptr->dataType == 5)
        {
            *msg_int32 = (*msg1_int32);
            msg_int32++;
            msg1_int32++;
        }
    } 


}

void IPCRX_release()
{

}








