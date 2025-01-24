

#include "MW_SDFM.h"

void SDFM_setup(SDFMSetupStruct* ptr)
{
    

    /* Enable main interrupt so that any of the filter interrupts can trigger
       by SDFM interrupt to CPU */

    if(ptr->filter_channel == 0)
    {
        SDFM_setupModulatorClock(ptr->base, SDFM_FILTER_1, ptr->filter1_sd_modulator_mode);
        SDFM_enableSynchronizer(ptr->base, SDFM_FILTER_1, SDFM_CLOCK_SYNCHRONIZER | SDFM_DATA_SYNCHRONIZER);
        SDFM_selectClockSource(ptr->base, SDFM_FILTER_1, SDFM_CLK_SOURCE_SD1_CLK);

        if(ptr->filter1_enable_comparator)
        {
            if(ptr->filter1_enable_highthresholdz)
                SDFM_configEnhancedComparator(ptr->base, (SDFM_FILTER_1 | ptr->filter1_comparator_filter_type | SDFM_SET_OSR(ptr->filter1_comparator_osr)), SDFM_THRESHOLD(ptr->filter1_comparator_highlvl_threshold1, ptr->filter1_comparator_lowlvl_threshold1), SDFM_THRESHOLD(ptr->filter1_comparator_highlvl_threshold2, ptr->filter1_comparator_lowlvl_threshold2), (SDFM_HTLZ_ENABLE | ptr->filter1_highthresholdz));
            else
                SDFM_configEnhancedComparator(ptr->base, (SDFM_FILTER_1 | ptr->filter1_comparator_filter_type | SDFM_SET_OSR(ptr->filter1_comparator_osr)), SDFM_THRESHOLD(ptr->filter1_comparator_highlvl_threshold1, ptr->filter1_comparator_lowlvl_threshold1), SDFM_THRESHOLD(ptr->filter1_comparator_highlvl_threshold2, ptr->filter1_comparator_lowlvl_threshold2), (SDFM_HTLZ_DISABLE | ptr->filter1_highthresholdz));

             SDFM_selectCompEventSource(ptr->base, SDFM_FILTER_1 , SDFM_COMP_EVENT_1 , ptr->filter1_comparator_event1_src);
             if(ptr->filter1_use_cevt1)
             {
                SDFM_CompEventFilterConfig CEVT1;
                CEVT1.clkPrescale  = ptr->filter1_cevt1_prescale;
	            CEVT1.threshold    = ptr->filter1_cevt1_threshold;
	            CEVT1.sampleWindow = ptr->filter1_cevt1_samplewindow;

                SDFM_configCompEventHighFilter(ptr->base, SDFM_FILTER_1, &CEVT1);
	            SDFM_initCompEventHighFilter(ptr->base, SDFM_FILTER_1);
             }

             SDFM_selectCompEventSource(ptr->base, SDFM_FILTER_1 , SDFM_COMP_EVENT_2 , ptr->filter1_comparator_event2_src);
             if(ptr->filter1_use_cevt2)
             {
                SDFM_CompEventFilterConfig CEVT2;
                CEVT2.clkPrescale  = ptr->filter1_cevt2_prescale;
                CEVT2.threshold    = ptr->filter1_cevt2_threshold;
                CEVT2.sampleWindow = ptr->filter1_cevt2_samplewindow;
                SDFM_configCompEventLowFilter(ptr->base, SDFM_FILTER_1, &CEVT2);
                SDFM_initCompEventLowFilter(ptr->base, SDFM_FILTER_1);
             }
        } 


        if(ptr->filter1_enable_data_filter)
        {
            if(ptr->filter1_use_fifo)
            {
                SDFM_configDataFilterFIFO(ptr->base, (SDFM_FILTER_1 | ptr->filter1_filtertype | SDFM_SET_OSR(ptr->filter1_dosr)), (SDFM_FILTER_ENABLE | (ptr->filter1_data_filter_output_rep) | SDFM_SHIFT_VALUE(ptr->filter1_shift) | SDFM_SET_FIFO_LEVEL(ptr->filter1_fifo_lvl)));
                if(ptr->filter1_clearfifo_sdsync)
                {
                    SDFM_setFIFOClearOnSyncMode(ptr->base, SDFM_FILTER_1, SDFM_FIFO_CLEARED_ON_SYNC);
                }
            }
            else
            {
                SDFM_configDataFilter(ptr->base, (SDFM_FILTER_1 | ptr->filter1_filtertype | SDFM_SET_OSR(ptr->filter1_dosr)), (SDFM_FILTER_ENABLE | (ptr->filter1_data_filter_output_rep) | SDFM_SHIFT_VALUE(ptr->filter1_shift)));
            }
           
            SDFM_enableMainFilter(ptr->base);

            if(ptr->filter1_pwm_synchronization)
            {
                if(ptr->filt1_enable_externalreset)
                    SDFM_enableExternalReset(ptr->base, SDFM_FILTER_1);
                   SDFM_setPWMSyncSource(ptr->base, SDFM_FILTER_1, ptr->filter1_src_sdsync_event);
                SDFM_enableWaitForSync(ptr->base, SDFM_FILTER_1);
                if(ptr->filter1_clear_wait_sync_flag_manually)
                {
                    SDFM_setWaitForSyncClearMode(ptr->base, SDFM_FILTER_1, SDFM_MANUAL_CLEAR_WAIT_FOR_SYNC);
                }

            }
        }

       
    }

    if(ptr->filter_channel == 1)
    {
        SDFM_setupModulatorClock(ptr->base, SDFM_FILTER_2, ptr->filter2_sd_modulator_mode);
        SDFM_enableSynchronizer(ptr->base, SDFM_FILTER_2, SDFM_CLOCK_SYNCHRONIZER | SDFM_DATA_SYNCHRONIZER);
        if(ptr->filter2_ch2_sdclk_src == 1)
            SDFM_selectClockSource(ptr->base, SDFM_FILTER_2, SDFM_CLK_SOURCE_SD1_CLK);
        else
            SDFM_selectClockSource(ptr->base, SDFM_FILTER_2, SDFM_CLK_SOURCE_CHANNEL_CLK); 

        if(ptr->filter2_enable_comparator)
        {
            if(ptr->filter2_enable_highthresholdz)
                SDFM_configEnhancedComparator(ptr->base, (SDFM_FILTER_2 | ptr->filter2_comparator_filter_type | SDFM_SET_OSR(ptr->filter2_comparator_osr)), SDFM_THRESHOLD(ptr->filter2_comparator_highlvl_threshold1, ptr->filter2_comparator_lowlvl_threshold1), SDFM_THRESHOLD(ptr->filter2_comparator_highlvl_threshold2, ptr->filter2_comparator_lowlvl_threshold2), (SDFM_HTLZ_ENABLE | ptr->filter2_highthresholdz));
            else
                SDFM_configEnhancedComparator(ptr->base, (SDFM_FILTER_2 | ptr->filter2_comparator_filter_type | SDFM_SET_OSR(ptr->filter2_comparator_osr)), SDFM_THRESHOLD(ptr->filter2_comparator_highlvl_threshold1, ptr->filter2_comparator_lowlvl_threshold1), SDFM_THRESHOLD(ptr->filter2_comparator_highlvl_threshold2, ptr->filter2_comparator_lowlvl_threshold2), (SDFM_HTLZ_DISABLE | ptr->filter2_highthresholdz));

             SDFM_selectCompEventSource(ptr->base, SDFM_FILTER_2 , SDFM_COMP_EVENT_1 , ptr->filter2_comparator_event1_src);
             if(ptr->filter2_use_cevt1)
             {
                SDFM_CompEventFilterConfig CEVT1;
                CEVT1.clkPrescale  = ptr->filter2_cevt1_prescale;
	            CEVT1.threshold    = ptr->filter2_cevt1_threshold;
	            CEVT1.sampleWindow = ptr->filter2_cevt1_samplewindow;

                SDFM_configCompEventHighFilter(ptr->base, SDFM_FILTER_2, &CEVT1);
	            SDFM_initCompEventHighFilter(ptr->base, SDFM_FILTER_2);
             }

             SDFM_selectCompEventSource(ptr->base, SDFM_FILTER_2 , SDFM_COMP_EVENT_2 , ptr->filter2_comparator_event2_src);
             if(ptr->filter2_use_cevt2)
             {
                SDFM_CompEventFilterConfig CEVT2;
                CEVT2.clkPrescale  = ptr->filter2_cevt2_prescale;
                CEVT2.threshold    = ptr->filter2_cevt2_threshold;
                CEVT2.sampleWindow = ptr->filter2_cevt2_samplewindow;
                SDFM_configCompEventLowFilter(ptr->base, SDFM_FILTER_2, &CEVT2);
                SDFM_initCompEventLowFilter(ptr->base, SDFM_FILTER_2);
             }
        }  

        if(ptr->filter2_enable_data_filter)
        {
            if(ptr->filter2_use_fifo)
            {
                SDFM_configDataFilterFIFO(ptr->base, (SDFM_FILTER_2 | ptr->filter2_filtertype | SDFM_SET_OSR(ptr->filter2_dosr)), (SDFM_FILTER_ENABLE | (ptr->filter2_data_filter_output_rep) | SDFM_SHIFT_VALUE(ptr->filter2_shift) | SDFM_SET_FIFO_LEVEL(ptr->filter2_fifo_lvl)));
                if(ptr->filter2_clearfifo_sdsync)
                {
                    SDFM_setFIFOClearOnSyncMode(ptr->base, SDFM_FILTER_2, SDFM_FIFO_CLEARED_ON_SYNC);
                }
            }
            else
            {
                SDFM_configDataFilter(ptr->base, (SDFM_FILTER_2 | ptr->filter2_filtertype | SDFM_SET_OSR(ptr->filter2_dosr)), (SDFM_FILTER_ENABLE | (ptr->filter2_data_filter_output_rep) | SDFM_SHIFT_VALUE(ptr->filter2_shift)));
            }
           
            SDFM_enableMainFilter(ptr->base);

            if(ptr->filter2_pwm_synchronization)
            {
                if(ptr->filt2_enable_externalreset)
                    SDFM_enableExternalReset(ptr->base, SDFM_FILTER_2);
                 SDFM_setPWMSyncSource(ptr->base, SDFM_FILTER_2, ptr->filter2_src_sdsync_event);

              
                SDFM_enableWaitForSync(ptr->base, SDFM_FILTER_2);
                if(ptr->filter2_clear_wait_sync_flag_manually)
                {
                    SDFM_setWaitForSyncClearMode(ptr->base, SDFM_FILTER_2, SDFM_MANUAL_CLEAR_WAIT_FOR_SYNC);
                }

            }
        }
        
    }

    if(ptr->filter_channel == 2)
    {
        SDFM_setupModulatorClock(ptr->base, SDFM_FILTER_3, ptr->filter3_sd_modulator_mode);
        SDFM_enableSynchronizer(ptr->base, SDFM_FILTER_3, SDFM_CLOCK_SYNCHRONIZER | SDFM_DATA_SYNCHRONIZER);
        if(ptr->filter3_ch3_sdclk_src == 1)
            SDFM_selectClockSource(ptr->base, SDFM_FILTER_3, SDFM_CLK_SOURCE_SD1_CLK);
        else
            SDFM_selectClockSource(ptr->base, SDFM_FILTER_3, SDFM_CLK_SOURCE_CHANNEL_CLK);

        if(ptr->filter3_enable_comparator)
        {
            if(ptr->filter3_enable_highthresholdz)
                SDFM_configEnhancedComparator(ptr->base, (SDFM_FILTER_3 | ptr->filter3_comparator_filter_type | SDFM_SET_OSR(ptr->filter3_comparator_osr)), SDFM_THRESHOLD(ptr->filter3_comparator_highlvl_threshold1, ptr->filter3_comparator_lowlvl_threshold1), SDFM_THRESHOLD(ptr->filter3_comparator_highlvl_threshold2, ptr->filter3_comparator_lowlvl_threshold2), (SDFM_HTLZ_ENABLE | ptr->filter3_highthresholdz));
            else
                SDFM_configEnhancedComparator(ptr->base, (SDFM_FILTER_3 | ptr->filter3_comparator_filter_type | SDFM_SET_OSR(ptr->filter3_comparator_osr)), SDFM_THRESHOLD(ptr->filter3_comparator_highlvl_threshold1, ptr->filter3_comparator_lowlvl_threshold1), SDFM_THRESHOLD(ptr->filter3_comparator_highlvl_threshold2, ptr->filter3_comparator_lowlvl_threshold2), (SDFM_HTLZ_DISABLE | ptr->filter3_highthresholdz));

             SDFM_selectCompEventSource(ptr->base, SDFM_FILTER_3 , SDFM_COMP_EVENT_1 , ptr->filter3_comparator_event1_src);
             if(ptr->filter3_use_cevt1)
             {
                SDFM_CompEventFilterConfig CEVT1;
                CEVT1.clkPrescale  = ptr->filter3_cevt1_prescale;
	            CEVT1.threshold    = ptr->filter3_cevt1_threshold;
	            CEVT1.sampleWindow = ptr->filter3_cevt1_samplewindow;

                SDFM_configCompEventHighFilter(ptr->base, SDFM_FILTER_3, &CEVT1);
	            SDFM_initCompEventHighFilter(ptr->base, SDFM_FILTER_3);
             }

             SDFM_selectCompEventSource(ptr->base, SDFM_FILTER_3 , SDFM_COMP_EVENT_2 , ptr->filter3_comparator_event2_src);
             if(ptr->filter3_use_cevt2)
             {
                SDFM_CompEventFilterConfig CEVT2;
                CEVT2.clkPrescale  = ptr->filter3_cevt2_prescale;
                CEVT2.threshold    = ptr->filter3_cevt2_threshold;
                CEVT2.sampleWindow = ptr->filter3_cevt2_samplewindow;
                SDFM_configCompEventLowFilter(ptr->base, SDFM_FILTER_3, &CEVT2);
                SDFM_initCompEventLowFilter(ptr->base, SDFM_FILTER_3);
             }
        }


        if(ptr->filter3_enable_data_filter)
        {
            if(ptr->filter3_use_fifo)
            {
                SDFM_configDataFilterFIFO(ptr->base, (SDFM_FILTER_3 | ptr->filter3_filtertype | SDFM_SET_OSR(ptr->filter3_dosr)), (SDFM_FILTER_ENABLE | (ptr->filter3_data_filter_output_rep) | SDFM_SHIFT_VALUE(ptr->filter3_shift) | SDFM_SET_FIFO_LEVEL(ptr->filter3_fifo_lvl)));
                if(ptr->filter3_clearfifo_sdsync)
                {
                    SDFM_setFIFOClearOnSyncMode(ptr->base, SDFM_FILTER_3, SDFM_FIFO_CLEARED_ON_SYNC);
                }
            }
            else
            {
                SDFM_configDataFilter(ptr->base, (SDFM_FILTER_3 | ptr->filter3_filtertype | SDFM_SET_OSR(ptr->filter3_dosr)), (SDFM_FILTER_ENABLE | (ptr->filter3_data_filter_output_rep) | SDFM_SHIFT_VALUE(ptr->filter3_shift)));
            }
           
            SDFM_enableMainFilter(ptr->base);

            if(ptr->filter3_pwm_synchronization)
            {
                if(ptr->filt3_enable_externalreset)
                    SDFM_enableExternalReset(ptr->base, SDFM_FILTER_3);
                 SDFM_setPWMSyncSource(ptr->base, SDFM_FILTER_3, ptr->filter3_src_sdsync_event);
                
                SDFM_enableWaitForSync(ptr->base, SDFM_FILTER_3);
                if(ptr->filter3_clear_wait_sync_flag_manually)
                {
                    SDFM_setWaitForSyncClearMode(ptr->base, SDFM_FILTER_3, SDFM_MANUAL_CLEAR_WAIT_FOR_SYNC);
                }

            }
        }

            
    }

    if(ptr->filter_channel == 3)
    {
        SDFM_setupModulatorClock(ptr->base, SDFM_FILTER_4, ptr->filter4_sd_modulator_mode);
        SDFM_enableSynchronizer(ptr->base, SDFM_FILTER_4, SDFM_CLOCK_SYNCHRONIZER | SDFM_DATA_SYNCHRONIZER);
        if(ptr->filter4_ch4_sdclk_src == 1)
            SDFM_selectClockSource(ptr->base, SDFM_FILTER_4, SDFM_CLK_SOURCE_SD1_CLK);
        else
            SDFM_selectClockSource(ptr->base, SDFM_FILTER_4, SDFM_CLK_SOURCE_CHANNEL_CLK); 

        if(ptr->filter4_enable_comparator)
        {
            if(ptr->filter4_enable_highthresholdz)
                SDFM_configEnhancedComparator(ptr->base, (SDFM_FILTER_4 | ptr->filter4_comparator_filter_type | SDFM_SET_OSR(ptr->filter4_comparator_osr)), SDFM_THRESHOLD(ptr->filter4_comparator_highlvl_threshold1, ptr->filter4_comparator_lowlvl_threshold1), SDFM_THRESHOLD(ptr->filter4_comparator_highlvl_threshold2, ptr->filter4_comparator_lowlvl_threshold2), (SDFM_HTLZ_ENABLE | ptr->filter4_highthresholdz));
            else
                SDFM_configEnhancedComparator(ptr->base, (SDFM_FILTER_4 | ptr->filter4_comparator_filter_type | SDFM_SET_OSR(ptr->filter4_comparator_osr)), SDFM_THRESHOLD(ptr->filter4_comparator_highlvl_threshold1, ptr->filter4_comparator_lowlvl_threshold1), SDFM_THRESHOLD(ptr->filter4_comparator_highlvl_threshold2, ptr->filter4_comparator_lowlvl_threshold2), (SDFM_HTLZ_DISABLE | ptr->filter4_highthresholdz));

             SDFM_selectCompEventSource(ptr->base, SDFM_FILTER_4 , SDFM_COMP_EVENT_1 , ptr->filter4_comparator_event1_src);
             if(ptr->filter4_use_cevt1)
             {
                SDFM_CompEventFilterConfig CEVT1;
                CEVT1.clkPrescale  = ptr->filter4_cevt1_prescale;
	            CEVT1.threshold    = ptr->filter4_cevt1_threshold;
	            CEVT1.sampleWindow = ptr->filter4_cevt1_samplewindow;

                SDFM_configCompEventHighFilter(ptr->base, SDFM_FILTER_4, &CEVT1);
	            SDFM_initCompEventHighFilter(ptr->base, SDFM_FILTER_4);
             }

             SDFM_selectCompEventSource(ptr->base, SDFM_FILTER_4 , SDFM_COMP_EVENT_2 , ptr->filter4_comparator_event2_src);
             if(ptr->filter4_use_cevt2)
             {
                SDFM_CompEventFilterConfig CEVT2;
                CEVT2.clkPrescale  = ptr->filter4_cevt2_prescale;
                CEVT2.threshold    = ptr->filter4_cevt2_threshold;
                CEVT2.sampleWindow = ptr->filter4_cevt2_samplewindow;
                SDFM_configCompEventLowFilter(ptr->base, SDFM_FILTER_4, &CEVT2);
                SDFM_initCompEventLowFilter(ptr->base, SDFM_FILTER_4);
             }
        }

        
        if(ptr->filter4_enable_data_filter)
        {
            if(ptr->filter4_use_fifo)
            {
                SDFM_configDataFilterFIFO(ptr->base, (SDFM_FILTER_4 | ptr->filter4_filtertype | SDFM_SET_OSR(ptr->filter4_dosr)), (SDFM_FILTER_ENABLE | (ptr->filter4_data_filter_output_rep) | SDFM_SHIFT_VALUE(ptr->filter4_shift) | SDFM_SET_FIFO_LEVEL(ptr->filter4_fifo_lvl)));
                if(ptr->filter4_clearfifo_sdsync)
                {
                    SDFM_setFIFOClearOnSyncMode(ptr->base, SDFM_FILTER_4, SDFM_FIFO_CLEARED_ON_SYNC);
                }
            }
            else
            {
                SDFM_configDataFilter(ptr->base, (SDFM_FILTER_4 | ptr->filter4_filtertype | SDFM_SET_OSR(ptr->filter4_dosr)), (SDFM_FILTER_ENABLE | (ptr->filter4_data_filter_output_rep) | SDFM_SHIFT_VALUE(ptr->filter4_shift)));
            }
           
            SDFM_enableMainFilter(ptr->base);

            if(ptr->filter4_pwm_synchronization)
            {
                if(ptr->filt4_enable_externalreset)
                    SDFM_enableExternalReset(ptr->base, SDFM_FILTER_4);
                SDFM_setPWMSyncSource(ptr->base, SDFM_FILTER_4, ptr->filter4_src_sdsync_event);
                
                SDFM_enableWaitForSync(ptr->base, SDFM_FILTER_4);
                if(ptr->filter4_clear_wait_sync_flag_manually)
                {
                    SDFM_setWaitForSyncClearMode(ptr->base, SDFM_FILTER_4, SDFM_MANUAL_CLEAR_WAIT_FOR_SYNC);
                }

            }
        }
      
    }

        if(ptr->use_interrupts)
        {
            if(ptr->filter_channel == 0)
            {
                if(ptr->filter1_modulator_clk_failure_int)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_1, SDFM_MODULATOR_FAILURE_INTERRUPT);

                if(ptr->filter1_data_ack_int && !(ptr->filter1_use_fifo))
                {
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_1, SDFM_DATA_FILTER_ACKNOWLEDGE_INTERRUPT);
	                SDFM_setDataReadyInterruptSource(ptr->base, SDFM_FILTER_1, SDFM_DATA_READY_SOURCE_DIRECT);
                }

                if(ptr->filter1_comparator_event1_int && ptr->filter1_enable_comparator)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_1, SDFM_CEVT1_INTERRUPT);  

                if(ptr->filter1_comparator_event2_int && ptr->filter1_enable_comparator)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_1, SDFM_CEVT2_INTERRUPT); 

                if(ptr->filter1_fifo_overflow_int && ptr->filter1_use_fifo)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_1, SDFM_FIFO_OVERFLOW_INTERRUPT);

                if(ptr->filter1_sdfm_fifo_int && ptr->filter1_use_fifo)
                {
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_1, SDFM_FIFO_INTERRUPT);
	                SDFM_setDataReadyInterruptSource(ptr->base, SDFM_FILTER_1, SDFM_DATA_READY_SOURCE_FIFO);
                }

             

            }

            if(ptr->filter_channel == 1)
            {
                if(ptr->filter2_modulator_clk_failure_int)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_2, SDFM_MODULATOR_FAILURE_INTERRUPT);

                if(ptr->filter2_data_ack_int && !(ptr->filter2_use_fifo))
                {
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_2, SDFM_DATA_FILTER_ACKNOWLEDGE_INTERRUPT);
	                SDFM_setDataReadyInterruptSource(ptr->base, SDFM_FILTER_2, SDFM_DATA_READY_SOURCE_DIRECT);
                }

                if(ptr->filter2_comparator_event1_int && ptr->filter2_enable_comparator)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_2, SDFM_CEVT1_INTERRUPT);  

                if(ptr->filter2_comparator_event2_int && ptr->filter2_enable_comparator)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_2, SDFM_CEVT2_INTERRUPT); 

                if(ptr->filter2_fifo_overflow_int && ptr->filter2_use_fifo)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_2, SDFM_FIFO_OVERFLOW_INTERRUPT);

                if(ptr->filter2_sdfm_fifo_int && ptr->filter2_use_fifo)
                {
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_2, SDFM_FIFO_INTERRUPT);
	                SDFM_setDataReadyInterruptSource(ptr->base, SDFM_FILTER_2, SDFM_DATA_READY_SOURCE_FIFO);
                }

            }

            if(ptr->filter_channel == 2)
            {
                if(ptr->filter3_modulator_clk_failure_int)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_3, SDFM_MODULATOR_FAILURE_INTERRUPT);

                if(ptr->filter3_data_ack_int && !(ptr->filter3_use_fifo))
                {
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_3, SDFM_DATA_FILTER_ACKNOWLEDGE_INTERRUPT);
	                SDFM_setDataReadyInterruptSource(ptr->base, SDFM_FILTER_3, SDFM_DATA_READY_SOURCE_DIRECT);
                }

                if(ptr->filter3_comparator_event1_int && ptr->filter3_enable_comparator)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_3, SDFM_CEVT1_INTERRUPT);  

                if(ptr->filter3_comparator_event2_int && ptr->filter3_enable_comparator)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_3, SDFM_CEVT2_INTERRUPT); 

                if(ptr->filter3_fifo_overflow_int && ptr->filter3_use_fifo)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_3, SDFM_FIFO_OVERFLOW_INTERRUPT);

                if(ptr->filter3_sdfm_fifo_int && ptr->filter3_use_fifo)
                {
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_3, SDFM_FIFO_INTERRUPT);
	                SDFM_setDataReadyInterruptSource(ptr->base, SDFM_FILTER_3, SDFM_DATA_READY_SOURCE_FIFO);
                }

            }

            if(ptr->filter_channel == 3)
            {
                if(ptr->filter4_modulator_clk_failure_int)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_4, SDFM_MODULATOR_FAILURE_INTERRUPT);

                if(ptr->filter4_data_ack_int && !(ptr->filter4_use_fifo))
                {
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_4, SDFM_DATA_FILTER_ACKNOWLEDGE_INTERRUPT);
	                SDFM_setDataReadyInterruptSource(ptr->base, SDFM_FILTER_4, SDFM_DATA_READY_SOURCE_DIRECT);
                }

                if(ptr->filter4_comparator_event1_int && ptr->filter4_enable_comparator)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_4, SDFM_CEVT1_INTERRUPT);  

                if(ptr->filter4_comparator_event2_int && ptr->filter4_enable_comparator)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_4, SDFM_CEVT2_INTERRUPT); 

                if(ptr->filter4_fifo_overflow_int && ptr->filter4_use_fifo)
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_4, SDFM_FIFO_OVERFLOW_INTERRUPT);

                if(ptr->filter4_sdfm_fifo_int && ptr->filter4_use_fifo)
                {
                    SDFM_enableInterrupt(ptr->base, SDFM_FILTER_4, SDFM_FIFO_INTERRUPT);
	                SDFM_setDataReadyInterruptSource(ptr->base, SDFM_FILTER_4, SDFM_DATA_READY_SOURCE_FIFO);
                }

            }

            // ClockP_usleep(39U);
            SDFM_enableMainInterrupt(ptr->base);

 
            }


}

void SDFM_step(SDFMStepStruct* ptr, const void *DataFilterStatus, const void *data, const void *outputLen, const void *compStatus, const void *compOutput)
{

    bool *DFSPtr = (bool *)DataFilterStatus;

  
    int16_t *DataFilterPtr16 = (int16_t *)data;
    int32_t *DataFilterPtr32 = (int32_t *)data; 
    uint8_t *dataLen = (uint8_t *)outputLen;
    uint32_t *cmpsts = (uint32_t *)compStatus;
    uint16_t *cmpdata = (uint16_t *)compOutput;
    uint32_t i;

    if(ptr->filter_channel == 0)
    {
        if(ptr->filter1_use_fifo)
            *DFSPtr = (uint8_t)SDFM_getFIFOISRStatus(ptr->base, SDFM_FILTER_1);
        else
             *DFSPtr = (uint8_t)SDFM_getNewFilterDataStatus(ptr->base, SDFM_FILTER_1);
        
        if(ptr->filter1_use_fifo)
        {
            

            while(SDFM_getFIFOISRStatus(ptr->base, SDFM_FILTER_1)== false)
            {

            }
            
            for(i = 0; i<ptr->filter1_fifo_lvl; i++)
            {
                if(ptr->filter1_data_filter_output_rep == 0)
                {
                        *DataFilterPtr16 = (int16_t)SDFM_getFIFOData(ptr->base, SDFM_FILTER_1);
                        DataFilterPtr16++;
                }
                else
                {
                        *DataFilterPtr32 = (int32_t)SDFM_getFIFOData(ptr->base, SDFM_FILTER_1);
                        DataFilterPtr32++;
                }
            }
            

             *dataLen = ptr->filter1_fifo_lvl;
             
             if(!ptr->filt1_fifo_int)
             {
                 SDFM_clearInterruptFlag(ptr->base, SDFM_MAIN_INTERRUPT_FLAG  | SDFM_FILTER_1_FIFO_INTERRUPT_FLAG  | 0xFFF);  
             }
        }
        else
        {
           while(SDFM_getNewFilterDataStatus(ptr->base, SDFM_FILTER_1) == false)
            {
                    
            }
            
            if(ptr->filter1_data_filter_output_rep == 0)
                *DataFilterPtr16 = (int16_t)(SDFM_getFilterData(ptr->base, SDFM_FILTER_1)>> 16U);
            else
                *DataFilterPtr32 = (int32_t)(SDFM_getFilterData(ptr->base, SDFM_FILTER_1)>> CSL_SDFM_SDDATA1_DATA32HI_SHIFT);
            

            *dataLen = 1;

            if(!ptr->filt1_newdata_int)
            {
                 SDFM_clearInterruptFlag(ptr->base, SDFM_MAIN_INTERRUPT_FLAG  |  SDFM_FILTER_1_NEW_DATA_FLAG  | 0xFFF);
            }

        }

        if(ptr->enable_comparator)
        {
            *cmpsts = SDFM_getThresholdStatus(ptr->base, SDFM_FILTER_1);
            *cmpdata = SDFM_getComparatorSincData(ptr->base, SDFM_FILTER_1);

        }
    }
    else if(ptr->filter_channel == 1)
    {
        if(ptr->filter2_use_fifo)
            *DFSPtr = (uint8_t)SDFM_getFIFOISRStatus(ptr->base, SDFM_FILTER_2);
        else
             *DFSPtr = (uint8_t)SDFM_getNewFilterDataStatus(ptr->base, SDFM_FILTER_2);
        
        if(ptr->filter2_use_fifo)
        {
            while(SDFM_getFIFOISRStatus(ptr->base, SDFM_FILTER_2)== false)
            {

            }
            
                for(i = 0; i<ptr->filter2_fifo_lvl; i++)
                {
                    if(ptr->filter2_data_filter_output_rep == 0)
                    {
                        *DataFilterPtr16 = (int16_t)SDFM_getFIFOData(ptr->base, SDFM_FILTER_2);
                        DataFilterPtr16++;
                    }
                    else
                    {
                        *DataFilterPtr32 = (int32_t)SDFM_getFIFOData(ptr->base, SDFM_FILTER_2);
                        DataFilterPtr32++;
                    }
                }
            

             *dataLen = ptr->filter2_fifo_lvl;
             if(!ptr->filt2_fifo_int)
             {
                 SDFM_clearInterruptFlag(ptr->base, SDFM_MAIN_INTERRUPT_FLAG  | SDFM_FILTER_2_FIFO_INTERRUPT_FLAG  | 0xFFF);  
             }
        }
        else
        {
           
            
                while(SDFM_getNewFilterDataStatus(ptr->base, SDFM_FILTER_2) == false)
                {

                }
                if(ptr->filter2_data_filter_output_rep == 0)
                    *DataFilterPtr16 = (int16_t)(SDFM_getFilterData(ptr->base, SDFM_FILTER_2)>> 16U);
                else
                    *DataFilterPtr32 = (int32_t)(SDFM_getFilterData(ptr->base, SDFM_FILTER_2)>> CSL_SDFM_SDDATA1_DATA32HI_SHIFT);
            

            *dataLen = 1;

            if(!ptr->filt2_newdata_int)
            {
                 SDFM_clearInterruptFlag(ptr->base, SDFM_MAIN_INTERRUPT_FLAG  |  SDFM_FILTER_2_NEW_DATA_FLAG  | 0xFFF);
            }

        }

        if(ptr->enable_comparator)
        {
            *cmpsts = SDFM_getThresholdStatus(ptr->base, SDFM_FILTER_2);
            *cmpdata = SDFM_getComparatorSincData(ptr->base, SDFM_FILTER_2);

        }
    }
    else if(ptr->filter_channel == 2)
    {
        if(ptr->filter3_use_fifo)
            *DFSPtr = (uint8_t)SDFM_getFIFOISRStatus(ptr->base, SDFM_FILTER_3);
        else
             *DFSPtr = (uint8_t)SDFM_getNewFilterDataStatus(ptr->base, SDFM_FILTER_3);
        
        if(ptr->filter3_use_fifo)
        {
            while(SDFM_getFIFOISRStatus(ptr->base, SDFM_FILTER_3)== false)
            {

            }
                for(i = 0; i<ptr->filter3_fifo_lvl; i++)
                {
                    if(ptr->filter3_data_filter_output_rep == 0)
                    {
                        *DataFilterPtr16 = (int16_t)SDFM_getFIFOData(ptr->base, SDFM_FILTER_3);
                        DataFilterPtr16++;
                    }
                    else
                    {
                        *DataFilterPtr32 = (int32_t)SDFM_getFIFOData(ptr->base, SDFM_FILTER_3);
                        DataFilterPtr32++;
                    }
                }
            

             *dataLen = ptr->filter3_fifo_lvl;

             if(!ptr->filt3_fifo_int)
             {
                 SDFM_clearInterruptFlag(ptr->base, SDFM_MAIN_INTERRUPT_FLAG  | SDFM_FILTER_3_FIFO_INTERRUPT_FLAG  | 0xFFF);  
             }
        }
        else
        {
            while(SDFM_getNewFilterDataStatus(ptr->base, SDFM_FILTER_3) == false)
            {

            }
                if(ptr->filter3_data_filter_output_rep == 0)
                    *DataFilterPtr16 = (int16_t)(SDFM_getFilterData(ptr->base, SDFM_FILTER_3)>> 16U);
                else
                    *DataFilterPtr32 = (int32_t)(SDFM_getFilterData(ptr->base, SDFM_FILTER_3)>> CSL_SDFM_SDDATA1_DATA32HI_SHIFT);
            

            *dataLen = 1;

            if(!ptr->filt3_newdata_int)
            {
                 SDFM_clearInterruptFlag(ptr->base, SDFM_MAIN_INTERRUPT_FLAG  |  SDFM_FILTER_3_NEW_DATA_FLAG  | 0xFFF);
            }

        }

        if(ptr->enable_comparator)
        {
            *cmpsts = SDFM_getThresholdStatus(ptr->base, SDFM_FILTER_3);
            *cmpdata = SDFM_getComparatorSincData(ptr->base, SDFM_FILTER_3);

        }
    }
    else if(ptr->filter_channel == 3)
    {
        if(ptr->filter4_use_fifo)
            *DFSPtr = (uint8_t)SDFM_getFIFOISRStatus(ptr->base, SDFM_FILTER_4);
        else
             *DFSPtr = (uint8_t)SDFM_getNewFilterDataStatus(ptr->base, SDFM_FILTER_4);
        
        if(ptr->filter4_use_fifo)
        {

            while(SDFM_getFIFOISRStatus(ptr->base, SDFM_FILTER_4)== false)
            {

            }
                for(i = 0; i<ptr->filter4_fifo_lvl; i++)
                {
                    if(ptr->filter4_data_filter_output_rep == 0)
                    {
                        *DataFilterPtr16 = (int16_t)SDFM_getFIFOData(ptr->base, SDFM_FILTER_4);
                        DataFilterPtr16++;
                    }
                    else
                    {
                        *DataFilterPtr32 = (int32_t)SDFM_getFIFOData(ptr->base, SDFM_FILTER_4);
                        DataFilterPtr32++;
                    }
                }
            

             *dataLen = ptr->filter4_fifo_lvl;

             if(!ptr->filt4_fifo_int)
             {
                 SDFM_clearInterruptFlag(ptr->base, SDFM_MAIN_INTERRUPT_FLAG  | SDFM_FILTER_4_FIFO_INTERRUPT_FLAG  | 0xFFF);  
             }
        }
        else
        {
            while(SDFM_getNewFilterDataStatus(ptr->base, SDFM_FILTER_3) == false)
            {

            }
                if(ptr->filter4_data_filter_output_rep == 0)
                    *DataFilterPtr16 = (int16_t)(SDFM_getFilterData(ptr->base, SDFM_FILTER_4)>> 16U);
                else
                    *DataFilterPtr32 = (int32_t)(SDFM_getFilterData(ptr->base, SDFM_FILTER_4)>> CSL_SDFM_SDDATA1_DATA32HI_SHIFT);
            

            *dataLen = 1;

            if(!ptr->filt4_newdata_int)
            {
                 SDFM_clearInterruptFlag(ptr->base, SDFM_MAIN_INTERRUPT_FLAG  |  SDFM_FILTER_4_NEW_DATA_FLAG  | 0xFFF);
            }

        }
        if(ptr->enable_comparator)
        {
            *cmpsts = SDFM_getThresholdStatus(ptr->base, SDFM_FILTER_4);
            *cmpdata = SDFM_getComparatorSincData(ptr->base, SDFM_FILTER_4);

        }
    }

    

    return;
        
}