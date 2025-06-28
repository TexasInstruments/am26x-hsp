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
#include "MW_IPCTX.h"
#include "MW_target_hardware_resources.h"

RPMessage_Object gAckReplyMsgObject;
uint16_t gRemoteServiceEndPt = 13u;

void IPCTX_setup(IPCTXSetup *ptr)
{
    int32_t status;

    RPMessage_CreateParams createParams;
    RPMessage_CreateParams_init(&createParams);
    createParams.localEndPt = ptr->end_point;

    status = RPMessage_construct(&gAckReplyMsgObject, &createParams);
    DebugP_assert(status==SystemP_SUCCESS);
    
    IpcNotify_syncAll(SystemP_WAIT_FOREVER);
}

void IPCTX_step(IPCTXStep *ptr, const void* msg_val, uint16_t len)
{
    int32_t status;
    uint32_t *msgValue = (uint32_t *)msg_val;
    uint32_t val = *msgValue;
    uint16_t msgSize;
    uint16_t txLen;

    if((ptr->dataType == 2) || (ptr->dataType == 3))
        txLen = len * 4;
    else if((ptr->dataType == 4) || (ptr->dataType == 5))
        txLen = len * 2;
    else if((ptr->dataType == 6) || (ptr->dataType == 7))
        txLen = len * 1;
    

    RPMessage_send(
                msg_val, txLen,
                ptr->remote_core, ptr->dest_endpoint,
                RPMessage_getLocalEndPt(&gAckReplyMsgObject),
                SystemP_WAIT_FOREVER);
    
}

void IPCTX_release()
{
    
    
}