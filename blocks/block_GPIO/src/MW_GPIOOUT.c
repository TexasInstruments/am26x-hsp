
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

