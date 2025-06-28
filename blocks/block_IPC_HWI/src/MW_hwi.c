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
#include "MW_hwi.h"
uint32_t gDelayCount = 0;


void ti_sitara_EPWM_intr_pre_code(uint8_t ip_instance_number)
{
    /*No pre code*/
   
}

void ti_sitara_EPWM_intr_post_code(uint8_t ip_instance_number)
{
    uint32_t epwmBaseAddr = CSL_CONTROLSS_G0_EPWM0_U_BASE + (ip_instance_number*0x1000);
    if(true == EPWM_getEventTriggerInterruptStatus(epwmBaseAddr))
    {
        EPWM_clearEventTriggerInterruptFlag(epwmBaseAddr);
    }
}

void ti_sitara_ADC_intr_pre_code(uint8_t ip_instance_number)
{
    /*No pre code*/
    

}

void ti_sitara_ADC_intr_post_code(uint8_t ip_instance_number)
{
    uint32_t adcBaseAddr = CSL_CONTROLSS_ADC0_U_BASE + (ip_instance_number*0x1000);
   
    if(true == ADC_getInterruptStatus(adcBaseAddr, ADC_INT_NUMBER1))
    {
        ADC_clearInterruptStatus(adcBaseAddr, ADC_INT_NUMBER1);
        if(ADC_getInterruptOverflowStatus(adcBaseAddr, ADC_INT_NUMBER1))
            ADC_clearInterruptOverflowStatus(adcBaseAddr, ADC_INT_NUMBER1);
        
    }

    if(true == ADC_getInterruptStatus(adcBaseAddr, ADC_INT_NUMBER2))
    {
        ADC_clearInterruptStatus(adcBaseAddr, ADC_INT_NUMBER2);
        if(ADC_getInterruptOverflowStatus(adcBaseAddr, ADC_INT_NUMBER2))
            ADC_clearInterruptOverflowStatus(adcBaseAddr, ADC_INT_NUMBER2);
   
    }

    if(true == ADC_getInterruptStatus(adcBaseAddr, ADC_INT_NUMBER3))
    {
        ADC_clearInterruptStatus(adcBaseAddr, ADC_INT_NUMBER3);
        if(ADC_getInterruptOverflowStatus(adcBaseAddr, ADC_INT_NUMBER3))
            ADC_clearInterruptOverflowStatus(adcBaseAddr, ADC_INT_NUMBER3);
     
    }

    if(true == ADC_getInterruptStatus(adcBaseAddr, ADC_INT_NUMBER4))
    {
        ADC_clearInterruptStatus(adcBaseAddr, ADC_INT_NUMBER4);
        if(ADC_getInterruptOverflowStatus(adcBaseAddr, ADC_INT_NUMBER4))
            ADC_clearInterruptOverflowStatus(adcBaseAddr, ADC_INT_NUMBER4);
      
    }    
}

void ti_sitara_GPIO_intr_pre_code(uint8_t ip_instance_number)
{
   
}

void ti_sitara_GPIO_intr_post_code(uint8_t ip_instance_number)
{
    uint32_t intrStatus;
    intrStatus = GPIO_getBankIntrStatus(CSL_GPIO0_U_BASE, GPIO_GET_BANK_INDEX(ip_instance_number));
    GPIO_clearBankIntrStatus(CSL_GPIO0_U_BASE, GPIO_GET_BANK_INDEX(ip_instance_number), intrStatus);

}

void ti_sitara_MCAN_intr_pre_code(uint8_t ip_instance_number)
{

}

void ti_sitara_MCAN_intr_post_code(uint8_t ip_instance_number)
{
    uint32_t intrStatus;
    uint32_t mcanBaseAddr = CSL_MCAN0_MSG_RAM_U_BASE + (ip_instance_number*0x1000);
    if(ip_instance_number == 0)
    {
        intrStatus = MCAN_getIntrStatus(mcanBaseAddr);
        if(intrStatus!=0)
            MCAN_clearIntrStatus(mcanBaseAddr, intrStatus);
    }
    else if(ip_instance_number == 1)
    {
        intrStatus = MCAN_getIntrStatus(mcanBaseAddr);
        if(intrStatus!=0)
            MCAN_clearIntrStatus(mcanBaseAddr, intrStatus); 
    }
    else if(ip_instance_number == 2)
    {
        intrStatus = MCAN_getIntrStatus(mcanBaseAddr);
        if(intrStatus!=0)
            MCAN_clearIntrStatus(mcanBaseAddr, intrStatus); 
    }
    else if(ip_instance_number == 3)
    {
        intrStatus = MCAN_getIntrStatus(mcanBaseAddr);
        if(intrStatus!=0)
            MCAN_clearIntrStatus(mcanBaseAddr, intrStatus); 
    }
    else if(ip_instance_number == 4)
    {
        intrStatus = MCAN_getIntrStatus(mcanBaseAddr);
        if(intrStatus!=0)
            MCAN_clearIntrStatus(mcanBaseAddr, intrStatus); 
    }
    else if(ip_instance_number == 5)
    {
        intrStatus = MCAN_getIntrStatus(mcanBaseAddr);
        if(intrStatus!=0)
            MCAN_clearIntrStatus(mcanBaseAddr, intrStatus); 
    }
    else if(ip_instance_number == 6)
    {
        intrStatus = MCAN_getIntrStatus(mcanBaseAddr);
        if(intrStatus!=0)
            MCAN_clearIntrStatus(mcanBaseAddr, intrStatus); 
    }
    else if(ip_instance_number == 7)
    {
        intrStatus = MCAN_getIntrStatus(mcanBaseAddr);
        if(intrStatus!=0)
            MCAN_clearIntrStatus(mcanBaseAddr, intrStatus); 
    }
}

void ti_sitara_SDFM_intr_pre_code(uint8_t ip_instance_number)
{

}

void ti_sitara_SDFM_intr_post_code(uint8_t ip_instance_number)
{
    uint32_t intrStatus;
    uint32_t sdfmBaseAddr = CSL_CONTROLSS_SDFM0_U_BASE + (ip_instance_number*0x1000);

    if(true == SDFM_getNewFilterDataStatus(sdfmBaseAddr, SDFM_FILTER_1))
    {
        SDFM_clearInterruptFlag(sdfmBaseAddr, SDFM_MAIN_INTERRUPT_FLAG  |  SDFM_FILTER_1_NEW_DATA_FLAG  | 0xFFF);
    }
    else if(true == SDFM_getNewFilterDataStatus(sdfmBaseAddr, SDFM_FILTER_2))
    {
        SDFM_clearInterruptFlag(sdfmBaseAddr, SDFM_MAIN_INTERRUPT_FLAG  |  SDFM_FILTER_2_NEW_DATA_FLAG  | 0xFFF);
    }
    else if(true == SDFM_getNewFilterDataStatus(sdfmBaseAddr, SDFM_FILTER_3))
    {
        SDFM_clearInterruptFlag(sdfmBaseAddr, SDFM_MAIN_INTERRUPT_FLAG  |  SDFM_FILTER_3_NEW_DATA_FLAG  | 0xFFF);
    }
    else if(true == SDFM_getNewFilterDataStatus(sdfmBaseAddr, SDFM_FILTER_4))
    {
        SDFM_clearInterruptFlag(sdfmBaseAddr, SDFM_MAIN_INTERRUPT_FLAG  |  SDFM_FILTER_4_NEW_DATA_FLAG  | 0xFFF);
    }

    if(true == SDFM_getFIFOISRStatus(sdfmBaseAddr, SDFM_FILTER_1))
    {
        SDFM_clearInterruptFlag(sdfmBaseAddr, SDFM_MAIN_INTERRUPT_FLAG  | SDFM_FILTER_1_FIFO_INTERRUPT_FLAG  | 0xFFF);    
    }
    else if(true == SDFM_getFIFOISRStatus(sdfmBaseAddr, SDFM_FILTER_2))
    {
        SDFM_clearInterruptFlag(sdfmBaseAddr, SDFM_MAIN_INTERRUPT_FLAG  | SDFM_FILTER_2_FIFO_INTERRUPT_FLAG  | 0xFFF);    
    }
    else if(true == SDFM_getFIFOISRStatus(sdfmBaseAddr, SDFM_FILTER_3))
    {
        SDFM_clearInterruptFlag(sdfmBaseAddr, SDFM_MAIN_INTERRUPT_FLAG  | SDFM_FILTER_3_FIFO_INTERRUPT_FLAG  | 0xFFF);    
    }
    else if(true == SDFM_getFIFOISRStatus(sdfmBaseAddr, SDFM_FILTER_4))
    {
        SDFM_clearInterruptFlag(sdfmBaseAddr, SDFM_MAIN_INTERRUPT_FLAG  | SDFM_FILTER_4_FIFO_INTERRUPT_FLAG  | 0xFFF);    
    }
}

void ti_sitara_ECAP_intr_pre_code(uint8_t ip_instance_number)
{

}

void ti_sitara_ECAP_intr_post_code(uint8_t ip_instance_number)
{
    uint32_t ecapBaseAddr = CSL_CONTROLSS_ECAP0_U_BASE + (ip_instance_number*0x1000);
    volatile uint16_t status;
    bool mode1 = false;
    status = ECAP_getInterruptSource(ecapBaseAddr);

    if(status & ECAP_ISR_SOURCE_CAPTURE_EVENT_1)
    {
        ECAP_clearInterrupt(ecapBaseAddr, ECAP_ISR_SOURCE_CAPTURE_EVENT_1);
        mode1 = true;
    }

    if(status & ECAP_ISR_SOURCE_CAPTURE_EVENT_2)
    {
        ECAP_clearInterrupt(ecapBaseAddr, ECAP_ISR_SOURCE_CAPTURE_EVENT_2);
        mode1 = true;
    }
        
    if(status & ECAP_ISR_SOURCE_CAPTURE_EVENT_3)
    {
        ECAP_clearInterrupt(ecapBaseAddr, ECAP_ISR_SOURCE_CAPTURE_EVENT_3);
        mode1 = true;
    }
        
    if(status & ECAP_ISR_SOURCE_CAPTURE_EVENT_4)
    {
        ECAP_clearInterrupt(ecapBaseAddr, ECAP_ISR_SOURCE_CAPTURE_EVENT_4);
        mode1 = true;
    }
    if(status & ECAP_ISR_SOURCE_COUNTER_OVERFLOW)
    {
        ECAP_clearInterrupt(ecapBaseAddr, ECAP_ISR_SOURCE_COUNTER_OVERFLOW);   
        mode1 = true;
    } 

    if(mode1)
        ECAP_clearGlobalInterrupt(ecapBaseAddr);

    if(status & ECAP_ISR_SOURCE_COUNTER_PERIOD)   
        ECAP_clearInterrupt(ecapBaseAddr, ECAP_ISR_SOURCE_COUNTER_PERIOD); 
    if(status & ECAP_ISR_SOURCE_COUNTER_COMPARE) 
        ECAP_clearInterrupt(ecapBaseAddr, ECAP_ISR_SOURCE_COUNTER_COMPARE); 
}

void ti_sitara_EQEP_intr_pre_code(uint8_t ip_instance_number)
{

}

void ti_sitara_EQEP_intr_post_code(uint8_t ip_instance_number)
{

    uint32_t ecqepBaseAddr = CSL_CONTROLSS_EQEP0_U_BASE + (ip_instance_number*0x1000);
    volatile uint16_t status;

    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_POS_CNT_ERROR) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_POS_CNT_ERROR);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_PHASE_ERROR) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_PHASE_ERROR);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_DIR_CHANGE) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_DIR_CHANGE);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_WATCHDOG) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_WATCHDOG);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_UNDERFLOW) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_UNDERFLOW);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_OVERFLOW) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_OVERFLOW);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_POS_COMP_READY) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_POS_COMP_READY);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_POS_COMP_MATCH) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_POS_COMP_MATCH);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_STROBE_EVNT_LATCH) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_STROBE_EVNT_LATCH);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_INDEX_EVNT_LATCH) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_INDEX_EVNT_LATCH);
    if((EQEP_getInterruptStatus(ecqepBaseAddr) & EQEP_INT_UNIT_TIME_OUT) != 0)
        EQEP_clearInterruptStatus(ecqepBaseAddr, EQEP_INT_UNIT_TIME_OUT);

}

void ti_sitara_UART_intr_pre_code(uint8_t ip_instance_number)
{

}
void ti_sitara_UART_intr_post_code(uint8_t ip_instance_number)
{


}

void ti_sitara_IPC_intr_pre_code(uint8_t ip_instance_number)
{

}

void ti_sitara_IPC_intr_post_code(uint8_t ip_instance_number)
{
    

}

void interrupt_setup(uint16_t interrupt_number, uint8_t ip_instance_number, void (*intr_fcn)(void))
{
    static HwiP_Object  periphHwiObject;
    HwiP_Params  hwiPrms;

    /* Register & enable interrupt */
    if(interrupt_number!=136)
    {
        HwiP_Params_init(&hwiPrms);
        hwiPrms.intNum      = interrupt_number;
        if(interrupt_number == 38 | interrupt_number == 39 | interrupt_number == 40 | interrupt_number == 41 | interrupt_number == 42 | interrupt_number == 43)
            hwiPrms.priority    = 4;    
        else
            hwiPrms.priority    = 0;                    
        hwiPrms.callback    = (HwiP_FxnCallback)intr_fcn;
        HwiP_construct(&periphHwiObject, &hwiPrms);
    }
    
}




