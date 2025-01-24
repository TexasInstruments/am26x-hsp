#include <drivers/gpio.h>

void util_iomux_config_for_gpio(uint32_t gpioNum);

void MW_GPIO_set_dir_output(uint8_t pin);
void MW_GPIO_high(uint8_t pin);
void MW_GPIO_low(uint8_t pin);
