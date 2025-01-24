#ifndef UART_MW_H_
#define UART_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/uart.h>
#include <drivers/uart/v0/lld/uart_lld.h>
#include <drivers/soc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif


#define CONFIG_UART_NUM_LLD_INSTANCES 6



extern void uart_read_setup( uint8_t instance);
extern void uart_write_setup( uint8_t instace);
extern void uart_read_step( const void *dataOutput, uint16_t outputLength,  uint8_t dataType, uint8_t instance);
extern void uart_write_step(const void *dataInput, uint32_t inputVecLength, uint8_t dataType, uint8_t instance);
extern void uart_release();



#endif
