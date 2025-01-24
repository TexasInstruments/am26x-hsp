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

}MCSPIReadStepStruct;

void MCSPIREAD_step(MCSPIReadStepStruct *ptr, const void* RxBuffer, uint32_t output_data_len);
void MCSPIREAD_release();

extern MCSPILLD_Handle gMcspiHandle[5U];


#endif