#ifndef EQEP_MW_H_
#define EQEP_MW_H_
#include <stdint.h>
#include <stdbool.h>
#include <drivers/soc.h>
#include <drivers/eqep.h>
#ifdef SOC_AM263X
#include <drivers/hw_include/am263x/cslr_soc.h>
#include <drivers/hw_include/am263x/cslr_soc_baseaddress.h>
#endif
#ifdef SOC_AM263PX
#include <drivers/hw_include/am263px/cslr_soc.h>
#include <drivers/hw_include/am263px/cslr_soc_baseaddress.h>
#endif



typedef struct{

    uint32_t base;
    EQEP_StrobeSource strbsrc;
    bool invertQEPA;
    bool invertQEPB;
    bool invertIndex;
    bool invertStrobe;
    uint16_t posCntrSrc;
    uint16_t resolution;
    uint16_t swap;
    bool changeDirIndx;
    EQEP_EmulationMode emulationMode;
    EQEP_PositionResetMode posCntrMode;
    bool enableUnitTimer;
    uint32_t unitTimerPeriod;
    bool enableWatchdog;
    uint32_t QEPCaptureLatch;
    uint32_t StrobeEventLatch;
    uint32_t IndexEventLatch;
    EQEP_QMAMode QMAModuleMode;
    uint16_t PositionStrobeEvents;
    uint16_t PositionIndexEvents;
    bool SWInitPositionCntr;
    uint32_t initialPosition;
    bool Enable_PositionCntr;
    bool Config_Pos_CmpUnit;
    uint16_t SPSEL;
    uint16_t Pos_Cmp_ShadowLoad;
    uint32_t compareValues;
    uint32_t compareCycles;
    bool EnablePosCmpUnit;
    bool Config_Edge_CapUnit;
    EQEP_CAPCLKPrescale Capture_Prescaler;
    EQEP_UPEVNTPrescale evntPrescale;
    bool Enable_edgecaptureunit;
    uint32_t maxPosition;
    uint32_t posCntr;
    uint16_t watchdogTimerPrd;
    uint16_t watchdogTimerValue;
    EQEP_Source QEPASource;
    EQEP_Source QEPBSource;
    EQEP_Source IndexSource;
    bool Use_Interrupt;
    bool Enable_Interrupt;
    bool Global_int_flag;
    bool Position_cntr_error;
    bool Quad_phase_error;
    bool Quad_dir_change;
    bool Watchdog_timeout;
    bool Pos_cntr_underflow;
    bool Pos_cntr_overflow;
    bool Pos_cmp_ready;
    bool Pos_cmp_match;
    bool Strobe_event_latch;
    bool Index_event_latch;
    bool Unit_timeout;
    bool QMA_error;
   
}EQEPSetupStruct;

typedef struct{

    uint32_t base;
    bool getPositionCounter;
    bool getDirectionFlagValue;
    bool CaptureTimerValue;
    bool CapturePeriodValue;
    bool CaptureTimerLatchValue;
    bool CapturePeriodLatchValue;
    bool PositionCounterLatchValue;
    bool WatchdogTimerValue;
    bool IndexPositionLatchValue;
    bool StrobePositionLatchValue;
    uint32_t QEPCaptureLatch;
    uint32_t StrobeEventLatch;
    uint32_t IndxEventLatch;
    bool use_interrupt;
    bool enable_interrupt;
    bool strobe_pos_latch_intr;
    bool indx_pos_latch_intr;
    bool unit_timeout_intr;

}EQEPStepStruct;

typedef struct{

}EQEPReleaseStruct;

extern void
EQEP_setup(EQEPSetupStruct *ptr);

extern void
EQEP_step(EQEPStepStruct *ptr, const void *EQEPOutput, const void *directionOutput);

extern void
EQEP_release(EQEPReleaseStruct *ptr);
#endif