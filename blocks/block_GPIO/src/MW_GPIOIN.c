
#include "MW_GPIOIN.h"

void util_iomux_config_for_gpio(uint32_t gpioNum)
{
    *(volatile uint32_t *) ((uint32_t) (0x53100000ul + 0x00000298U)) = 0x83E70B13;
    *(volatile uint32_t *) ((uint32_t) (0x53100000ul + 0x0000029CU)) = 0x95A4F1E0;
    *(volatile uint32_t *) ((uint32_t) (0x53100000ul + gpioNum*4)) = 0x07;
    
}

void GPIOInput_setup(GPIOInputSetupStruct *ptr)
{
    GPIO_setDirMode(ptr->GPIOBaseAddress, ptr->pinInstance, 1);
    GPIO_setTrigType(ptr->GPIOBaseAddress, ptr->pinInstance, ptr->trigType);
    if(ptr->EnableInterrupt)
        GPIO_bankIntrEnable(ptr->GPIOBaseAddress, GPIO_GET_BANK_INDEX(ptr->pinInstance));

}

uint32_t GPIOInput_step(GPIOInputStepStruct *ptr)
{
    uint32_t pinValue;

    pinValue = GPIO_pinRead(ptr->GPIOBaseAddress, ptr->pinInstance);

    
    return pinValue;
   
}

void GPIOInput_release(GPIOInputReleaseStruct *ptr)
{
    uint32_t intrStatus;
    if(ptr->EnableInterrupt)
    {
        intrStatus = GPIO_getBankIntrStatus(ptr->GPIOBaseAddress, GPIO_GET_BANK_INDEX(ptr->pinInstance));
        GPIO_bankIntrDisable(ptr->GPIOBaseAddress, GPIO_GET_BANK_INDEX(ptr->pinInstance));
        GPIO_setTrigType(ptr->GPIOBaseAddress, ptr->pinInstance, GPIO_TRIG_TYPE_NONE);
        GPIO_clearIntrStatus(ptr->GPIOBaseAddress, ptr->pinInstance);
    }

}

