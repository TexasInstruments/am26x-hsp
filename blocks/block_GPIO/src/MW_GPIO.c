#include "MW_GPIO.h"

void util_iomux_config_for_gpio(uint32_t gpioNum)
{
    *(volatile uint32_t *) ((uint32_t) (0x53100000ul + 0x00000298U)) = 0x83E70B13;
    *(volatile uint32_t *) ((uint32_t) (0x53100000ul + 0x0000029CU)) = 0x95A4F1E0;
    *(volatile uint32_t *) ((uint32_t) (0x53100000ul + gpioNum*4)) = 0x07;
    
}

void MW_GPIO_set_dir_output(uint8_t pin)
{
    GPIO_setDirMode(0x52000000, pin, 0);
}

void MW_GPIO_high(uint8_t pin)
{
    GPIO_pinWriteHigh(0x52000000, pin);
}

void MW_GPIO_low(uint8_t pin)
{
    GPIO_pinWriteLow(0x52000000, pin);
}