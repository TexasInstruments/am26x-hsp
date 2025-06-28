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
/*
 
 *
 * File: rtiostream.h     
 *
 * Abstract:
 *  Function prototypes and defines for rtIOStream API.
 */

#ifndef RTIOSTREAM_H
#define RTIOSTREAM_H

#include <stddef.h>

#define RTIOSTREAM_ERROR (-1)
#define RTIOSTREAM_NO_ERROR (0)

/* Note: if the functions declared in this file should be compiled into a shared
 * library (e.g. a .dll file on Windows), you must ensure that the functions are
 * externally visible. The procedure to achieve this depends on the compiler and
 * linker you are using. For example, on Windows, you may need to provide an
 * exports definition .def file that lists all of the functions to be
 * exported; see ./rtiostream/rtiostream_pc.def for a suitable .def file.
 */

#ifndef RTIOSTREAMAPI
#define RTIOSTREAMAPI 
#endif 

#include <stdbool.h>
#include <stdint.h>
#include <drivers/uart.h>
#include <drivers/mcan.h>
#include <kernel/dpl/MutexArmP.h>
#include <kernel/dpl/HwiP.h>
#include <drivers/soc.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/ClockP.h>
#include <math.h>
#include <kernel/nortos/dpl/r5/HwiP_armv7r_vim.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/SemaphoreP.h>

#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif

#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif

RTIOSTREAMAPI int rtIOStreamOpen(
    int    argc,
    void * argv[]
);

RTIOSTREAMAPI int rtIOStreamSend(
    int          streamID,
    const void * src, 
    size_t       size,
    size_t     * sizeSent
    );

RTIOSTREAMAPI int rtIOStreamRecv(
    int      streamID,
    void   * dst, 
    size_t   size,
    size_t * sizeRecvd
    );

RTIOSTREAMAPI int rtIOStreamClose(
    int streamID
    );



#endif /* #ifndef RTIOSTREAM_H */