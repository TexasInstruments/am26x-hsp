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
#include "MW_GPIOOUT.h"


#ifndef  Delay_H
#define  Delay_H
#define DELAY(NUMBER) \
 for(int i = 0; i< NUMBER/5; i++)  \
  { __asm__ volatile ("NOP");   \
   } 
  
#endif
uint16_t cnt = 0;


void GPIOOutput_setup(GPIOOutputSetupStruct *ptr)
{
    GPIO_setDirMode(ptr->GPIOBaseAddress, ptr->pinInstance, 0);
    GPIO_setTrigType(ptr->GPIOBaseAddress, ptr->pinInstance, ptr->trigType);
    if(ptr->EnableInterrupt)
        GPIO_bankIntrEnable(ptr->GPIOBaseAddress, GPIO_GET_BANK_INDEX(ptr->pinInstance));
}

void GPIOOutput_step(GPIOOutputStepStruct *ptr)
{
    if(ptr->input_signal == 0)
    {
        GPIO_pinWriteLow(ptr->GPIOBaseAddress, ptr->pinInstance);
        
    }

    if(ptr->input_signal == 1)
    {
        GPIO_pinWriteHigh(ptr->GPIOBaseAddress, ptr->pinInstance);
       
    }
  

}

void GPIOOutput_release(GPIOOutputReleaseStruct *ptr)
{
    uint32_t intrStatus;

    intrStatus = GPIO_getBankIntrStatus(ptr->GPIOBaseAddress, GPIO_GET_BANK_INDEX(ptr->pinInstance));
    GPIO_bankIntrDisable(ptr->GPIOBaseAddress, GPIO_GET_BANK_INDEX(ptr->pinInstance));
    GPIO_setTrigType(ptr->GPIOBaseAddress, ptr->pinInstance, GPIO_TRIG_TYPE_NONE);
    GPIO_clearBankIntrStatus(ptr->GPIOBaseAddress, GPIO_GET_BANK_INDEX(ptr->pinInstance), intrStatus);
    GPIO_clearIntrStatus(ptr->GPIOBaseAddress, ptr->pinInstance);

}

