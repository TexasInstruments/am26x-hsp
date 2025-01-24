
#include "MW_CMPSS.h"

void CMPSS_setup(CMPSSSetupStruct *ptr)
{
    uint16_t config_high_comp = 0x0000U;
    if(ptr->neg_inp_src_high_comp == 0)
        config_high_comp = config_high_comp | CMPSS_INSRC_DAC;
    else if(ptr->neg_inp_src_high_comp == 1)
        config_high_comp = config_high_comp | CMPSS_INSRC_PIN;

    if(ptr->output_inverted_high_comp)
        config_high_comp = config_high_comp | CMPSS_INV_INVERTED;

    if(ptr->asynch_high_comp)
        config_high_comp = config_high_comp | CMPSS_OR_ASYNC_OUT_W_FILT;

    CMPSS_configHighComparator(ptr->base, config_high_comp);

    uint16_t config_low_comp = 0x0000U;
    if(ptr->pos_inp_src_low_comp == 0)
        config_low_comp = config_low_comp | CMPSS_INSRC_PIN_INL;
    else if(ptr->neg_inp_src_high_comp == 1)
        config_low_comp = config_low_comp | CMPSS_INSRC_PIN_INH;

    if(ptr->output_inverted_low_comp)
        config_low_comp = config_low_comp | CMPSS_INV_INVERTED;

    if(ptr->asynch_low_comp)
        config_low_comp = config_low_comp | CMPSS_OR_ASYNC_OUT_W_FILT;

    CMPSS_configLowComparator(ptr->base, config_low_comp);

    uint16_t configDAC = 0x0000U;
    if(ptr->DAC_Load == 0)
        configDAC = configDAC | CMPSS_DACVAL_SYSCLK;
    else
        configDAC = configDAC | CMPSS_DACVAL_PWMSYNC;

    if(ptr->DAC_Ref_voltage == 0)
        configDAC = configDAC | CMPSS_DACREF_VDDA;
    else
        configDAC = configDAC | CMPSS_DACREF_VDAC;

    if(ptr->DAC_value_src == 0)
        configDAC = configDAC | CMPSS_DACSRC_SHDW;
    else
        configDAC = configDAC | CMPSS_DACSRC_RAMP;

    CMPSS_configDAC(ptr->base, configDAC);

    CMPSS_configFilterHigh(ptr->base, ptr->High_comp_digfilt_sample_prescale, ptr->High_comp_digfilt_sample_window, ptr->High_comp_digfilt_threshold);
    CMPSS_configFilterLow(ptr->base, ptr->Low_comp_digfilt_sample_prescale, ptr->Low_comp_digfilt_sample_window, ptr->Low_comp_digfilt_threshold);

    if(ptr->initialize_dig_high_filter)
        CMPSS_initFilterHigh(ptr->base);

    if(ptr->initialize_dig_low_filter)
        CMPSS_initFilterLow(ptr->base);

    uint16_t output_signal_high_comp_ctripouth = 0x0000U;
    if(ptr->signal_ctripouth_high_comp == 0)
        output_signal_high_comp_ctripouth = output_signal_high_comp_ctripouth | CMPSS_TRIPOUT_ASYNC_COMP;
    else if(ptr->signal_ctripouth_high_comp == 16)
        output_signal_high_comp_ctripouth = output_signal_high_comp_ctripouth | CMPSS_TRIPOUT_SYNC_COMP;
    else if(ptr->signal_ctripouth_high_comp == 32)
        output_signal_high_comp_ctripouth = output_signal_high_comp_ctripouth | CMPSS_TRIPOUT_FILTER;
    else if(ptr->signal_ctripouth_high_comp == 48)
        output_signal_high_comp_ctripouth = output_signal_high_comp_ctripouth | CMPSS_TRIPOUT_LATCH;

    uint16_t output_signal_high_comp_ctriph = 0x0000U;
    if(ptr->signal_ctriph_high_comp == 0)
        output_signal_high_comp_ctriph = output_signal_high_comp_ctriph | CMPSS_TRIP_ASYNC_COMP;
    else if(ptr->signal_ctriph_high_comp == 4)
        output_signal_high_comp_ctriph = output_signal_high_comp_ctriph | CMPSS_TRIP_SYNC_COMP;
    else if(ptr->signal_ctriph_high_comp == 8)
        output_signal_high_comp_ctriph = output_signal_high_comp_ctriph | CMPSS_TRIP_FILTER;
    else if(ptr->signal_ctriph_high_comp == 12)
        output_signal_high_comp_ctriph = output_signal_high_comp_ctriph | CMPSS_TRIP_LATCH;

    CMPSS_configOutputsHigh(ptr->base, (output_signal_high_comp_ctripouth | output_signal_high_comp_ctriph));

    uint16_t output_signal_low_comp_ctripoutl = 0x0000U;
    if(ptr->signal_ctripoutl_low_comp == 0)
        output_signal_low_comp_ctripoutl = output_signal_low_comp_ctripoutl | CMPSS_TRIPOUT_ASYNC_COMP;
    else if(ptr->signal_ctripoutl_low_comp == 16)  
        output_signal_low_comp_ctripoutl = output_signal_low_comp_ctripoutl | CMPSS_TRIPOUT_SYNC_COMP;
    else if(ptr->signal_ctripoutl_low_comp == 32)  
        output_signal_low_comp_ctripoutl = output_signal_low_comp_ctripoutl | CMPSS_TRIPOUT_FILTER;
    else if(ptr->signal_ctripoutl_low_comp == 48)  
        output_signal_low_comp_ctripoutl = output_signal_low_comp_ctripoutl | CMPSS_TRIPOUT_LATCH;

    uint16_t output_signal_low_comp_ctrip = 0x0000U;
    if(ptr->signal_ctripl_low_comp == 0)
        output_signal_low_comp_ctrip = output_signal_low_comp_ctrip | CMPSS_TRIP_ASYNC_COMP;
    else if(ptr->signal_ctripl_low_comp == 4)
        output_signal_low_comp_ctrip = output_signal_low_comp_ctrip | CMPSS_TRIP_SYNC_COMP;
    else if(ptr->signal_ctripl_low_comp == 8)
        output_signal_low_comp_ctrip = output_signal_low_comp_ctrip | CMPSS_TRIP_FILTER;
    else if(ptr->signal_ctripl_low_comp == 12)
        output_signal_low_comp_ctrip = output_signal_low_comp_ctrip | CMPSS_TRIP_LATCH;

    CMPSS_configOutputsLow(ptr->base, (output_signal_low_comp_ctripoutl | output_signal_low_comp_ctrip));

    CMPSS_setComparatorHighHysteresis(ptr->base, ptr->hysterisis_high_comp);
    CMPSS_setComparatorLowHysteresis(ptr->base, ptr->hysterisis_low_comp);

    bool ramp_gen_reset = false;
    if(ptr->ramp_gen_reset_value == 0)
        ramp_gen_reset = true;
    else
        ramp_gen_reset = false;

    if((ptr->DAC_Src == 1) && (ptr->specify_comph_dacramp == 0))
        CMPSS_configRamp(ptr->base, ptr->max_ramp_gen_ref_value, ptr->ramp_gen_dec_value, ptr->ramp_gen_delay_value, ptr->pwmSync, ramp_gen_reset);
    else if((ptr->DAC_Src == 1) && (ptr->specify_comph_dacramp == 1))
        CMPSS_configRamp(ptr->base, ptr->max_ramp_gen_ref_value_init, ptr->ramp_gen_dec_value_init, ptr->ramp_gen_delay_value_init, ptr->pwmSync, ramp_gen_reset);

    if(ptr->specify_comph_dacramp == 0)
        CMPSS_setDACValueHigh(ptr->base, ptr->High_comp_DAC_value);
    else if(ptr->specify_comph_dacramp == 1)
        CMPSS_setDACValueHigh(ptr->base, ptr->High_comp_DAC_value_init);

    if(ptr->specify_compl_dac == 0)
        CMPSS_setDACValueLow(ptr->base, ptr->Low_comp_DAC_value);
    else if(ptr->specify_compl_dac == 1)
        CMPSS_setDACValueLow(ptr->base, ptr->Low_comp_DAC_value_init);

    CMPSS_configBlanking(ptr->base, ptr->pwm_blanking);
    if(ptr->enable_blanking)
        CMPSS_enableBlanking(ptr->base);
    else
        CMPSS_disableBlanking(ptr->base);

    if(ptr->enable_diode_emulation)
    {
        CMPSS_selectDEACTIVESource(ptr->base, ptr->cmpss_deactive_signal);
        CMPSS_configHighDACShadowValue2(ptr->base, ptr->high_comp_dac_value2);
        CMPSS_configLowDACShadowValue2(ptr->base, ptr->low_comp_dac_value2);
        CMPSS_enableDEmode(ptr->base);
    }
    else
    {
        CMPSS_disableDEmode(ptr->base);
    }
    
    CMPSS_configLatchOnPWMSYNC(ptr->base, ptr->reset_dig_filter_latch_high_comp, ptr->reset_dig_filter_latch_low_comp);

    if(ptr->enable_module)
        CMPSS_enableModule(ptr->base);
    else
        CMPSS_disableModule(ptr->base);

    ClockP_usleep(10);

    if(ptr->clear_filt_latch_high_filt)
        CMPSS_clearFilterLatchHigh(ptr->base);
    if(ptr->clear_filter_latch_low_filter)
        CMPSS_clearFilterLatchLow(ptr->base);

}

void CMPSS_step(CMPSSStepStruct *ptr, const void *sts, const void *sts_latch)
{

    uint16_t *cmpss_status = (uint16_t *)sts;
    uint16_t *cmpss_latch_status = (uint16_t *)sts_latch;
    if(ptr->Enable_high_comparator)
    {
        *cmpss_status = (uint16_t)(CMPSS_getStatus(ptr->base) & CMPSS_STS_HI_FILTOUT);
        *cmpss_latch_status = (uint16_t)((uint16_t)(CMPSS_getStatus(ptr->base) & CMPSS_STS_HI_LATCHFILTOUT) >> 1U);
        CMPSS_clearFilterLatchHigh(ptr->base);
    }
    else if(ptr->Enable_low_comparator)
    {
        *cmpss_status = (uint16_t)((uint16_t)(CMPSS_getStatus(ptr->base) & CMPSS_STS_LO_FILTOUT) >> 8U);
        *cmpss_latch_status = (uint16_t)((uint16_t)(CMPSS_getStatus(ptr->base) & CMPSS_STS_LO_LATCHFILTOUT) >> 9U);
        CMPSS_clearFilterLatchLow(ptr->base);
    }

    return;
}