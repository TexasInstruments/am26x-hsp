/*

*
* File: xcp_platform_custom.h
* Abstract:
*   Implementation of XCP Platform Abstraction Layer interface
*/

#ifndef XCP_PLATFORM_CUSTOM_H
#define XCP_PLATFORM_CUSTOM_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
// #include "fsl_common.h"
#include "xcp_cfg.h"

extern  void  mw_printf(char *fmt,...);
extern  void  mw_xcp_mode_init();
extern  void  mw_sleep(uint32_t seconds, uint32_t microseconds);

#ifdef XCP_MAX_CTO_SIZE
#undef XCP_MAX_CTO_SIZE
#define XCP_MAX_CTO_SIZE  0x08
#else
#define XCP_MAX_CTO_SIZE 0x08
#endif


#define XCP_MUTEX_DEFINE(lock)      unsigned int lock
#define XCP_MUTEX_INIT(lock)        lock = 0;
#define XCP_MUTEX_LOCK(lock)      __disable_irq();
#define XCP_MUTEX_UNLOCK(lock)    __enable_irq();

// #define XCP_MUTEX_DEFINE(lock)      SemaphoreP_Object lock;
// #define XCP_MUTEX_INIT(lock)        SemaphoreP_constructMutex(lock);
// #define XCP_MUTEX_LOCK(lock)        SemaphoreP_pend(lock, SystemP_WAIT_FOREVER);
// #define XCP_MUTEX_UNLOCK(lock)      SemaphoreP_post(lock);


#define XCP_ADDRESS_GET(addressExtension, address)  (uint8_T*) ((uintptr_t) address)

//#define XCP_SLEEP(seconds,microseconds) SDK_DelayAtLeastUs(((uint32_t)seconds*1000000 + (uint32_t)microseconds),SystemCoreClock)

// #define XCP_SLEEP(seconds, microseconds)     if ((seconds==0) && (microseconds ==0)){   \
//                                             SDK_DelayAtLeastUs(1,SystemCoreClock);}    \
//                                             else {  \
//                                             SDK_DelayAtLeastUs(((uint32_t)seconds*1000000 + (uint32_t)microseconds),SystemCoreClock);}

#define XCP_SLEEP(seconds, microseconds)     if ((seconds==0) && (microseconds ==0)){   \
                                            ClockP_usleep(1);}    \
                                            else {  \
                                            ClockP_usleep(((uint32_t)seconds*1000000 + (uint32_t)microseconds));}
											
#define XCP_PRINTF  mw_printf


#define XCP_MEM_ALIGNMENT 4

#define XCP_MEM_BLOCK_1_SIZE        64
#define XCP_MEM_BLOCK_1_NUMBER      50 // doc example has 46

#define XCP_MEM_BLOCK_2_SIZE        256
#define XCP_MEM_BLOCK_2_NUMBER      50 // doc example has 10

#define XCP_MEM_BLOCK_3_SIZE        1024
#define XCP_MEM_BLOCK_3_NUMBER      10 // doc example doesn't have any

#endif
