#ifndef MCSPIREAD_MW_H_
#define MCSPIREAD_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/mcspi.h>
#include <kernel/dpl/DebugP.h>
#include <drivers/soc.h>

typedef struct{
    uint32_t base;
    uint8_t mcspi_instance;
    bool Enable_CH0;
    bool Enable_CH1;
    uint8_t CH0_DataSize;
    uint8_t CH1_DataSize;
    bool CH0_CSDisable;
    bool CH1_CSDisable;
    uint8_t dataType;   
    uint32_t output_data_length;
}MCSPIControllerTxStepStruct;

extern MCSPILLD_Handle gMcspiHandle[5U];

void MCSPICONTROLLERTX_step(MCSPIControllerTxStepStruct* ptr, uint32_t count, const void* InputData, const void* output_len, const void* InpRxBuffer);
void MCSPICONTROLLERTX_release();

#endif