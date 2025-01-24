
#include "MW_DAC.h"


/* Frequency of Timer ISR */
#define TIMER_ISR_FREQUENCY 20000
/* Frequency of output since wave */
#define OUTPUT_FREQUENCY 50
/* Value of pi calculated from inverse sine operation*/
#define PI (2 * asin(1.0))
/* Time duration in seconds for which sine waves are produced */
#define SINCE_OUTPUT_TIME 10

/* Global variables and objects */
/*
 * Variable to store angle which is gradually incremented in the ISR
 * to create the sine wave.
 */
float gAlpha = 0;
/*
 * Value by which the angle is incremented in the Timer ISR. This determines
 * the frequency of the sine wave
 */
float gStepValue = 0;
/* Base address of DAC instance */
// uint32_t gDacBaseAddr = CONFIG_DAC0_BASE_ADDR;
/* Final value that gets written into DAC shadow register */
uint16_t gDacOutput = 0;

void DAC_setup(DACSetupStruct *ptr)
{
    DAC_setReferenceVoltage(ptr->base, ptr->DACRef);
    DAC_setLoadMode(ptr->base, ptr->LoadMode);
    if(ptr->LoadMode)
    {
        DAC_setPWMSyncSignal(ptr->base, ptr->PWMSyncSignal);    
    }
    if(ptr->Enable_output)
        DAC_enableOutput(ptr->base);
    else
        DAC_disableOutput(ptr->base);
    
    if((ptr->ShdwValue >= 0) && (ptr->ShdwValue <= 4095))
        DAC_setShadowValue(ptr->base, ptr->ShdwValue);
    else
        DAC_setShadowValue(ptr->base, 0);
    uint16_t lock_reg = 0x0U;
    if(ptr->DACLockControl)
    {
        lock_reg = lock_reg | DAC_LOCK_CONTROL;   
    }
    if(ptr->DACLockOutput)
    {
        lock_reg = lock_reg | DAC_LOCK_OUTPUT;
    }
    if(ptr->DACLockShadow)
    {
        lock_reg = lock_reg | DAC_LOCK_SHADOW;
    }
    if(ptr->DACLockControl | ptr->DACLockOutput | ptr->DACLockShadow)
        DAC_lockRegister(ptr->base, lock_reg);

    ClockP_usleep(10);
    
}

void DAC_step(uint32_t base, uint16_t DACVal)
{


    


}

void DAC_release(uint32_t base)
{
    DAC_setShadowValue(base, 0);
    DAC_disableOutput(base);
}