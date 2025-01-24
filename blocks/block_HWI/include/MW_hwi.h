#include <stdint.h>
#include <kernel/dpl/HwiP.h>
#include <drivers/soc.h>
#include <drivers/epwm.h>
#include <drivers/adc.h>
#include <drivers/ecap.h>
#include <drivers/gpio.h>
#include <drivers/mcan.h>
#include <drivers/sdfm.h>
#include <drivers/eqep.h>
#include <drivers/uart/v0/lld/uart_lld.h>

void ti_sitara_EPWM_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_EPWM_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_ADC_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_ADC_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_GPIO_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_GPIO_intr_post_code(uint8_t ip_instance_number);

void interrupt_setup(uint16_t interrupt_number, uint8_t ip_instance_number, void (*intr_fcn)(void));

void ti_sitara_MCAN_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_MCAN_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_SDFM_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_SDFM_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_ECAP_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_ECAP_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_EQEP_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_EQEP_intr_post_code(uint8_t ip_instance_number);

void ti_sitara_UART_intr_pre_code(uint8_t ip_instance_number);

void ti_sitara_UART_intr_post_code(uint8_t ip_instance_number);

