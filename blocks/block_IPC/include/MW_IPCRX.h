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
#ifndef IPCRX_MW_H_
#define IPCRX_MW_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <drivers/ipc_notify.h>
#include <drivers/ipc_notify/v1/ipc_notify_v1.h>
#include <drivers/soc.h>
#include <drivers/ipc_rpmsg.h>

#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif

typedef struct{

    uint16_t self_core;
    uint16_t remote_core;
    uint32_t client_id;
    uint16_t end_point;

}IPCRXSetup;

typedef struct{

    uint16_t self_core;
    uint16_t remote_core;
    uint32_t client_id;
    uint16_t end_point;
    uint8_t dataType;

}IPCRXStep;

extern void 
IPCRX_setup(IPCRXSetup *ptr);
extern void
IPCRX_step(IPCRXStep *ptr, const void* msg_val, uint8_t input_length);
// extern void ti_sitara_isr_136();
extern void 
main_handler(RPMessage_Object *obj, void *args, void *data, uint16_t dataLength, int32_t crcStatus, uint16_t remoteCoreID, uint16_t srcEndPt);



#endif